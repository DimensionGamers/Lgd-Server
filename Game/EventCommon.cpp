EventCommon::EventCommon()
{
	this->SetNotifyTime(0);
	this->SetUpdateTick(0);
	this->SetUpdateTime(0);
	this->SetSecondTick(0);
	this->SetCurrentMinute(-1);
}

EventCommon::~EventCommon()
{

}

void EventCommon::UpdateTime()
{
	this->Update();
}

void EventCommon::SendRemainMinutes(std::string const& message, int32 period, int32 max_minute)
{
	if ( !this->IsMinuteElapsed(period) )
		return;

	if ( max_minute != -1 && (this->GetCurrentMinute() + 1) > max_minute )
		return;

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, message.c_str(), this->GetCurrentMinute() + 1);
}

void EventCommon::SendMessageID(uint8 id)
{
	if ( this->IsMessageElapsed(id) )
	{
		EVENT_MESSAGE_SEND pMsg(id);

		Player* pPlayer = nullptr;

		PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
		{
			pPlayer = it->second;

			if ( !pPlayer )
				continue;

			if ( pPlayer->GetEventId() != EVENT_NONE )
				continue;

			pPlayer->sendPacket(MAKE_PCT(pMsg));
		}
	}
}

bool EventCommonMgr::HaveTicket(Item const* item, uint16 entrance, uint16 ticket, uint8 ground)
{
	if ( item->GetItem() != entrance && (item->GetItem() != ticket || item->GetDurability() <= 0.0f) )
	{
		return false;
	}

	if ( ground != uint8(-1) )
	{
		if ( item->GetItem() == entrance && item->GetLevel() != (ground + 1) )
		{
			return false;
		}
	}

	return true;
}

bool EventCommonMgr::HaveTicket(Player* pPlayer, uint16 entrance, uint16 ticket, uint8 ground)
{
	use_inventory_loop(i)
	{
		if ( !pPlayer->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		if ( !pPlayer->GetInventory()->GetItem(i)->IsItem() )
			continue;

		if ( (pPlayer->GetInventory()->GetItem(i)->GetItem() != entrance || (pPlayer->GetInventory()->GetItem(i)->GetLevel() != ground && ground != uint8(-1))) && 
			 (pPlayer->GetInventory()->GetItem(i)->GetItem() != ticket || pPlayer->GetInventory()->GetItem(i)->GetDurability() <= 0.0f) )
			continue;

		return true;
	}

	return false;
}

void EventCommonMgr::ConsumeTicket(Player* pPlayer, uint16 entrance, uint16 ticket, uint8 slot)
{
	if ( pPlayer->GetInventory()->GetItem(slot)->GetItem() == entrance )
	{
		pPlayer->ClearItem(slot);
	}
	else if ( pPlayer->GetInventory()->GetItem(slot)->GetItem() == ticket )
	{
		pPlayer->DecreaseItemDurabilityByUse(slot, 1.0f);
	}
}

int32 EventGroundCommon::GetPlayersCountByPC(Player* pPlayer)
{
	if (!pPlayer)
	{
		return 0;
	}

	int32 count = 0;

	for (uint32 i = 0; i < this->GetPlayerSize(); ++i)
	{
		if (!this->GetPlayer(i))
		{
			continue;
		}

		Player* pEventPlayer = this->GetPlayer(i)->GetPlayer();

		if (!pEventPlayer)
		{
			continue;
		}

		if (pEventPlayer->GetAccountData()->GetDiskSerial() == pPlayer->GetAccountData()->GetDiskSerial() &&
			!memcmp(pEventPlayer->GetAccountData()->GetMac(), pPlayer->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH))
		{
			++count;
		}
	}

	return count;
}

void EventGroundCommon::SendMessageID(uint8 id)
{
	if ( this->IsMessageElapsed(id) )
	{
		EVENT_MESSAGE_SEND pMsg(id);

		for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
		{
			PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->sendPacket(MAKE_PCT(pMsg));
		}
	}
}

bool EventGroundCommon::AddPlayer(Player * pPlayer)
{
	int32 slot = -1;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( !this->GetPlayer(i)->GetGUID() )
		{
			this->GetPlayer(i)->Reset();
			this->GetPlayer(i)->SetPlayer(pPlayer);
			this->GetPlayer(i)->SetGUID(pPlayer->GetGUID());
			pPlayer->SetEventId(this->GetEvent());
			pPlayer->SetEventGround(this->GetGround());
			pPlayer->SetEventScore(0);

			this->Log("Adding Player %s", pPlayer->BuildLog().c_str());
			slot = i;
			break;
		}
	}

	return slot != -1;
}

void EventGroundCommon::MovePlayersOut(uint16 gate)
{
	for ( int32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer || !pPlayer->IsPlaying() || !(*world_range)(pPlayer->GetWorldId()) || !pPlayer->IsLive() )
			continue;

		pPlayer->TeleportToGate(gate);
		pPlayer->EventResetData();
	}

	this->ErasePlayer();

	this->Log("Moving Players Out");
}

bool EventGroundCommon::IsIn(Player* pPlayer)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( this->GetPlayer(i)->GetGUID() != pPlayer->GetGUID() )
		{
			continue;
		}

		this->GetPlayer(i)->SetPlayer(pPlayer);
		this->GetPlayer(i)->SetReconnected(true);
		this->GetPlayer(i)->GetReconnectedTime()->Start();

		this->Log("Relogin Player %s", pPlayer->BuildLog().c_str());

		return true;
	}

	return false;
}

void EventGroundCommon::UpdatePlayer(bool playing)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		{
			if ( playing )
			{
				this->GetPlayer(i)->SetPlayer(nullptr);
			}
			else
			{
				this->GetPlayer(i)->Reset();
			}
			continue;
		}

		if ( pPlayer->GetRegenStatus() != REGEN_NONE )
			continue;

		if ( !(*world_range)(pPlayer->GetWorldId()) )
		{
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
		}
	}
}

void EventGroundCommon::RemovePlayer(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayerData = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayerData )
		{
			continue;
		}

		if ( pPlayerData->GetGUID() == pPlayer->GetGUID() )
		{
			this->GetPlayer(i)->Reset();
			return;
		}
	}
}

void EventGroundCommon::SendNotice(NoticeType type, const char * message, ...)
{
	ARG(buffer, message);

	for ( int32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		pPlayer->SendNoticeNormal(type, buffer);
	}
}

void EventGroundCommon::SendPacket(uint8 * Packet, uint16 size)
{
	for ( int32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		pPlayer->sendPacket(Packet, size);
	}
}

void EventGroundCommon::Log(const char * log, ...)
{
	ARG(buffer, log);

	sLog->outInfo(this->GetLogFilter(), "[%s - %d] %s", this->GetName().c_str(), this->GetGround() + 1, buffer);
}