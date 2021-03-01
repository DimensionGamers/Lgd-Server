/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerSocket.cpp"
*
*/

void Player::sendPacket(uint8 * packet, uint16 len)
{
	if (!this->m_Socket)
		return;

	this->m_Socket->SendPacket(packet, len);
}

void Player::sendPacket(uint8 * packet)
{
	uint8 header = packet[0];

	switch (header)
	{
	case 0xC1:
	case 0xC3:
	{
				 C1_HEADER * pHeader = (C1_HEADER*)packet;

				 this->sendPacket(packet, pHeader->GetSize());
	} break;

	case 0xC2:
	case 0xC4:
	{
				 C2_HEADER * pHeader = (C2_HEADER*)packet;

				 this->sendPacket(packet, pHeader->GetSize());
	} break;
	}
}

void Player::SendPacket(C1_HEADER * packet)
{
	this->sendPacket((uint8*)packet, packet->GetSize());
}

void Player::SendPacket(C2_HEADER * packet)
{
	this->sendPacket((uint8*)packet, packet->GetSize());
}

void Player::SendPacket(GamePacket * packet)
{
	this->sendPacket(packet->End());
}

void Player::CloseSocket()
{
	if (this->m_Socket)
    {
        this->m_Socket->CloseSocket();
        this->m_Socket = nullptr;
    }
}

void Player::kick(std::string const& reason, const char * function)
{
	this->SetAutoLogin(false);
	this->PersonalStoreClose(false);
	this->HelperStop(0x01);
	this->GetHelper()->SetOffline(false);
	this->Close(1, CLOSE_FLAG_EXIT_GAME);
	this->GetAccountData()->SetCloseGame(true);
	//this->CloseSocket();

	sLog->outInfo("antihack", "%s - %s kicked. Reason: %s", function, this->BuildLog().c_str(), reason.c_str());
}

void Player::CreateSocket(std::shared_ptr<WorldSocket> sock)
{
	this->m_Socket = sock;
}

void Player::UpdatePacket()
{
	WorldPacket* packet;
	int32 packet_count = 0;

	while ( m_Socket && !_recvPacket.empty() && _recvPacket.next(packet) )
	{
		if ( !packet )
			continue;

		this->protocol_core(packet->GetReadPointer(), packet->GetActiveSize(), packet->GetHeadcode());
		delete packet;

		++packet_count;

		if ( packet_count >= sGameServer->GetRecvPacketQueueMax() )
		{
			sLog->outError("antihack", "%s -- %s", __FUNCTION__, this->BuildLog().c_str());

			if ( sGameServer->IsRecvPacketQueueKick() )
			{
				KICK_PLAYER(this, "Recv Packet Queue Max");

				sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Recv Packet Queue Max");
			}

			break;
		}
	}
}

void Player::AddPacket(WorldPacket* packet)
{
	_recvPacket.add(packet);
}

bool Player::Update(uint32 diff)
{
	this->UpdateTimer();

	if ( sGameServer->IsConnectionDisabled() )
	{
		this->CloseSocket();
	}

	this->CloseSocketPassed(CLOSE_SOCKET_TIME_CONNECT, sGameServer->GetConnectCheckTimeDelay(), "ConnectCheckTime");

	if ( this->GetConnectStatus() == CONNECT_STATUS_PLAYING && this->IsServerQuit() )
	{
		this->CloseSocketPassed(CLOSE_SOCKET_TIME_SERVER_QUIT, sGameServer->GetConnectCheckTimeDelay(), "ServerQuit");
	}

	if ( (this->GetConnectStatus() == CONNECT_STATUS_LOGGED || 
		  this->GetConnectStatus() == CONNECT_STATUS_PLAYING) &&
		 this->GetAccountData()->IsCloseGame() )
	{
		this->CloseSocketPassed(CLOSE_SOCKET_TIME_GAME_CLOSE, sGameServer->GetGameCloseTime(), "GameClose");
	}

	if ( this->m_Socket && !this->m_Socket->IsOpen() )
	{
		this->m_Socket = nullptr;
	}

	if ( this->m_Socket )
	{
		if ( this->GetTimer(PLAYER_TIMER_WRONG_SERIAL)->Elapsed(sGameServer->GetWrongSerialTime()) )
		{
			this->SetWrongSerialCount(0);
		}

		if ( this->GetTimer(PLAYER_TIMER_WRONG_DECRYPT)->Elapsed(sGameServer->GetWrongDecryptTime()) )
		{
			this->SetWrongDecryptCount(0);
		}

		if ( this->GetConnectStatus() == CONNECT_STATUS_LOGGED || 
			 this->GetConnectStatus() == CONNECT_STATUS_PLAYING )
		{
			this->LogoutTimeCheck();
		}
	}

	this->UpdatePacket();
	this->ProcessQueryCallbacks();

	if ( !this->m_Socket )
	{
		if ( !sGameServer->IsConnectionDisabled() )
		{
			if ( !this->GetAccountData()->IsCloseGame() && !this->IsOffline() )
			{
				if ( sGameServer->IsOfflineAttackOnDisconnect() && this->GetHelper()->IsStarted() )
				{
					this->GetHelper()->SetDisconencted(true);
					this->GetHelper()->SetOffline(true);
					this->GetHelper()->SetDisconnectTime(GetTickCount());
				}
			}
		}

		this->Close(0, CLOSE_FLAG_NONE);

		if ( this->IsAutoLogin() )
		{
			if ( sGameServer->IsConnectionDisabled() )
			{
				this->SetAutoLogin(false);
			}
		}
		else if ( !this->IsOffline() )
		{
			if ( this->GetConnectStatus() == CONNECT_STATUS_PLAYING )
			{
				this->FinalClose();
			}

			if ( !this->IsServerQuit() )
			{
				sAuthServer->PlayerLogout(this->GetAccountData()->GetGUID());
			}

			if ( this->GetConnectStatus() == CONNECT_STATUS_PLAYING )
			{
				sObjectMgr->RemoveSessionCharacter(this);
				sObjectMgr->RemoveSessionAccount(this);
			}
			else if ( this->GetConnectStatus() == CONNECT_STATUS_LOGGED )
			{
				sObjectMgr->RemoveSessionAccount(this);
			}

			sObjectMgr->RemoveSessionPlayer(this);

			WorldPacket* packet;
			while(!_recvPacket.empty() && _recvPacket.next(packet))
			{
				if ( packet )
					delete packet;
			}

			this->Remove();
			this->ResetName();
			this->SetGUID(0);
			this->GetAccountData()->ResetAccount();
			this->GetAccountData()->SetGUID(0);

			this->SetTimeout(GetTickCount());

			return false;
		}
		else
		{
			if ( sGameServer->IsConnectionDisabled() )
			{
				if ( this->GetPersonalStore()->IsOff() )
				{
					this->PersonalStoreClose(false);
				}

				if ( this->GetHelper()->IsOffline() )
				{
					this->HelperStop(0x01);
					this->GetHelper()->SetOffline(false);
				}
			}
		}
	}

	this->AutoLoginProcess();

	if ( this->GetConnectStatus() == CONNECT_STATUS_PLAYING )
	{
		if ( this->IsHalfSecondPassed() )
			this->UpdateDelayed();

		this->UpdateWorld();

		if ( this->IsLive() )
		{
			this->UpdateMovement();
			this->CreateFrustrum();
			//this->UpdateTeleport();

			if ( this->IsHalfSecondPassed() )
			{
				this->SkillCastNovaUpdate(); // 500 Msec
				this->DarkSpiritRun(); // 500 Msec

				this->TrackingUpdate();
			}

			this->HelperOffline();

			if ( this->IsSecondPassed() )
			{
				this->UpdateDelayedTeleport();
				this->UpdateBuffStatus();

				this->AutoRefill();
				this->UpdateAleTime();

				this->UpdateRecoveryMix();
				this->UpdateDurabilityTime();
				sGremoryCase->Update(this);

				if ( this->GetShopTime() >= 1 && this->GetShopTime() < 255 )
				{
					this->IncreaseShopTime(1);
				}

				this->Autosave();
				this->UpdateInterface();
				this->SelfDefenseUpdate();
				this->HelperUpdate();
				this->UpdateQuestMU();
				this->UpdateWrongWorld();
				this->GenUpdate();
				this->GenUpdateVictim();
				this->UpdateGoblinPoints();
				this->UpdateRestriction();
				sGameServer->PostManageUpdate(this);
				this->UpdateCoordinateAttribute();
				sArkaWar->WinnerBuffVerify(this);

				this->MuunUpdate();
				sMiningSystem->UpdateMining(this);

				if ( (GetTickCount() - this->GetGuildMatchingUpdateTime()) > (MINUTE * IN_MILLISECONDS) )
				{
					sGuildMatching->Update(this);
				}

				this->UpdateBuffID();

				this->GoldenChannelUpdate();
				this->HackCheckVerify();

				sHuntingRecord->Update(this);

				if ( sGameServer->IsAttackSafeEnabled() )
				{
					if ( this->GetTimer(PLAYER_TIMER_ATTACK_SAFE)->Elapsed(sGameServer->GetAttackSafeTime()) )
					{
						this->SetCounter(PLAYER_COUNTER_ATTACK_SAFE, 0);
					}
				}

				if ( this->GetRegenStatus() == REGEN_NONE )
				{
					if ( this->GetAttackStandby() > 0 )
					{
						this->SendNotice(NOTICE_NORMAL_BLUE, "You are going to be able to attack in %d second(s)", this->GetAttackStandby());
						this->ReduceAttackStandby(1);
					}
				}

				if ( this->GetAccountData()->GetVIPStatus() != -1 && time(nullptr) > this->GetAccountData()->GetVIPDuration() )
				{
					this->GetAccountData()->SetVIPStatus(-1);
					this->GetAccountData()->SetVIPDuration(0);

					this->SendNotice(NOTICE_NORMAL_BLUE, "[ VIP ]: Finished.");
				}

				if ( this->GetTimer(PLAYER_TIMER_SPEED_SEND)->Elapsed(sGameServer->GetAttackSpeedSendTime()) )
				{
					this->SendStats(false);
				}

				if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START && !this->IsInDragonTower() )
				{
					this->SetAccumulatedTime(0);
				}

				if ( sGameServer->IsTest() && sGameServer->GetTestKickTime() > 0 )
				{
					if ( this->GetTimer(PLAYER_TIMER_TEST_MODE)->Elapsed(sGameServer->GetTestKickTime()) )
					{
						KICK_PLAYER(this, "Test Mode Kick");
					}
				}

				if ( sGameServer->IsTradeInterfaceCancelBan() )
				{
					if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN && this->GetInterfaceState()->GetID() != InterfaceData::Trade )
					{
						this->SetAuthority(1);
						KICK_PLAYER(this, "Trade Interface Cancel");

						sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Trade Interface Cancel");
					}
				}

				if ( this->GetTimer(PLAYER_TIMER_WALK_SPEED)->Elapsed(MINUTE * IN_MILLISECONDS) )
				{
					this->SetWalkSpeedCount(0);
				}

				if (this->GetStatisticData(STATISTIC_ATTACK_TIME)->GetCount() > 0)
				{
					if (time(nullptr) >= (this->GetStatisticData(STATISTIC_ATTACK_TIME_DATE)->GetCount() + HOUR))
					{
						this->GetStatisticData(STATISTIC_ATTACK_TIME_DATE)->SetCount(time(nullptr));
						this->GetStatisticData(STATISTIC_ATTACK_TIME)->SetCount(0);
					}
				}

				sLabyrinthDimensions->Update(this);

				this->UpdateEventEnterCount();
				this->QuestGuidedItemGet();
				this->QuestGuidedReachLevel();
				this->QuestGuidedQuestComplete();

				this->QuestEvolutionUpdate();

				this->UpdateMapState();

				this->UpdatePersonalStore();
			}
		}
		else
		{
			this->SetTeleportStatus(TELEPORT_NONE);

			if( this->GetDeadStatus() == DEAD_STANDBY )
			{
				if( (MyGetTickCount() - this->GetRespawnTick()) > (this->GetRespawnTime() + 1000) )
				{
					this->ClearAllBuff(0);
					this->SetDeadStatus(DEAD_READY);
					this->SetState(OBJECT_STATE_RESPAWN);
					this->SetRespawnTick(MyGetTickCount());
				}
			}
			else if( this->GetDeadStatus() == DEAD_READY )
			{
				if( (MyGetTickCount() - this->GetRespawnTick()) > 1000 )
				{
					this->Respawn();
				}
			}
		}
	}

	return true;
}

bool Player::CloseSocketPassed(CloseSocketTimes id, uint32 time, std::string const& name)
{
	if ( this->m_Socket && this->m_Socket->IsOpen() )
	{
		if ( (GetTickCount() - this->GetCloseSocketTime(id)) > time )
		{
			sLog->outInfo(LOG_PLAYER, "%s %s - Kicked.", name.c_str(), this->BuildLog().c_str());
			this->CloseSocket();
			return true;
		}
	}

	return false;
}

void Player::InitCloseSocketTime(CloseSocketTimes id)
{
	this->SetCloseSocketTime(id, GetTickCount());
}