NixiesLake::NixiesLake()
{
	this->ChangeState(NIXIES_LAKE_STATE_NONE);
}

NixiesLake::~NixiesLake()
{

}

void NixiesLake::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !sGameServer->IsNixiesLakeEnabled() || !sGameServer->IsActiveStatus() )
	{
		pPlayer->SendMessageBox(0, "Nixies Lake", "Event is disabled.");
		return;
	}

	POINTER_PCT_LOG(NIXIES_LAKE_ENTER_REQUEST, lpMsg, Packet, 0);

	if ( this->GetState() == NIXIES_LAKE_STATE_NONE )
	{
		this->EnterResult(pPlayer, 2);
		return;
	}

	if ( this->GetState() == NIXIES_LAKE_STATE_STANDBY ||
		 this->GetState() == NIXIES_LAKE_STATE_OCCUPY ||
		 this->GetState() == NIXIES_LAKE_STATE_END )
	{
		this->EnterResult(pPlayer, 1);
		return;
	}

	if ( !this->EnterCheck(pPlayer) )
	{
		return;
	}

	Party* pParty = pPlayer->GetParty();

	if ( pParty )
	{
		if ( pParty->GetLeader() != pPlayer )
		{
			this->EnterResult(pPlayer, 5);
			return;
		}

		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			Player* pMember = pParty->GetMember(i)->GetPlayer();

			if ( !pMember )
			{
				continue;
			}

			if ( !Player::IsPlayerBasicState(pMember, false) )
			{
				this->EnterResult(pPlayer, 6);
				continue;
			}

			if ( !this->EnterCheck(pMember) )
			{
				this->EnterResult(pPlayer, 6);
				return;
			}

			if ( !this->FindEntranceItem(pMember, false) )
			{
				this->EnterResult(pPlayer, 4);
				return;
			}
		}

		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			Player* pMember = pParty->GetMember(i)->GetPlayer();

			if ( !pMember )
			{
				continue;
			}

			this->GetPlayer(i)->SetID(pMember->GetGUID());
			this->GetPlayer(i)->SetState(0);
			this->GetPlayer(i)->GetTime()->Start(sGameServer->GetNixiesLakeStandbyTime());

			if ( pMember == pPlayer )
			{
				this->EnterResult(pPlayer, 0);
			}
			else
			{
				this->MemberEnterRequest(pMember);
			}

			sLog->outInfo("nixieslake", "%s -- %d: %s", __FUNCTION__, i + 1, pMember->BuildLog().c_str());
		}
	}
	else
	{
		if ( !this->FindEntranceItem(pPlayer, false) )
		{
			this->EnterResult(pPlayer, 3);
			return;
		}

		this->GetPlayer(0)->SetID(pPlayer->GetGUID());
		this->GetPlayer(0)->SetState(0);
		this->GetPlayer(0)->GetTime()->Start(sGameServer->GetNixiesLakeStandbyTime());
		this->EnterResult(pPlayer, 0);

		sLog->outInfo("nixieslake", "%s -- 1: %s", __FUNCTION__, pPlayer->BuildLog().c_str());
	}

	this->ChangeState(NIXIES_LAKE_STATE_STANDBY);
}
	
void NixiesLake::EnterResult(Player* pPlayer, uint8 result)
{
	if ( !pPlayer )
	{
		return;
	}

	NIXIES_LAKE_ENTER_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);

	/*
	0 = Boss Monster You can enter as a resting place of Nixies. Would you like to enter? ( Podés entrar )
	1 = The other player approached the Nixies' sanctuary and the door to the sanctuary was closed. ( Ya hay 1 party adentro )
	2 = Nixie still have not appeared in the sanctuary. ( No está el boss )
	3 = You can not move because there is no debris of the elementary Nixie City. ( No tenes el item para entrar )
	4 = There is a party member who does not have the fragments of the spiritual Nixie. ( Uno de tu party no tiene el item para entrar )
	5 = Only the Team Leader can invite members. ( Solamente el Lider del party puede hablar con el NPC )
	6 = All party members must be in the same map and you can apply for admission. ( Todos los miembros del party tienen que estar en el mapa )
	*/
}

void NixiesLake::EnterConfirm(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !this->EnterCheck(pPlayer) )
	{
		return;
	}

	if ( !this->SetEnterState(pPlayer, true, false) )
	{
		return;
	}

	this->ReadyToMove();
}

void NixiesLake::MemberEnterRequest(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	NIXIES_LAKE_MEMBER_ENTER_REQUEST pMsg;
	pPlayer->SEND_PCT(pMsg);
}

void NixiesLake::MemberEnterConfirm(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	POINTER_PCT_LOG(NIXIES_LAKE_MEMBER_ENTER_RESULT, lpMsg, Packet, 0);

	if ( !this->SetEnterState(pPlayer, lpMsg->result == 1, true) )
	{
		return;
	}

	this->ReadyToMove();
}

void NixiesLake::MemberEnterNotify(uint8 result)
{
	Player* pPlayer = sObjectMgr->FindPlayerByGUID(this->GetPlayer(0)->GetID());

	if ( !pPlayer )
	{
		return;
	}

	NIXIES_LAKE_MEMBER_ENTER_NOTIFY pMsg(result);
	pPlayer->SEND_PCT(pMsg);
	/*
	0 = Member canceled or it's time exceeded
	1 = OK
	2 = Wait time exceeded
	*/
}

bool NixiesLake::EnterCheck(Player* pPlayer)
{
	if ( pPlayer->GetWorldId() != WORLD_NIXIES_LAKE )
	{
		return false;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return false;
	}

	return true;
}

bool NixiesLake::FindEntranceItem(Player* pPlayer, bool remove)
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( sGameServer->IsNixiesLakeFree() )
	{
		return true;
	}

	use_inventory_loop(i)
	{
		Item const* pItem = pPlayer->GetInventory()->GetItem(i);

		if ( !pItem )
		{
			continue;
		}

		if ( pItem->GetItem() != ITEMGET(14, 390) )
		{
			continue;
		}

		if ( remove )
		{
			pPlayer->ClearItem(i);
		}

		return true;
	}

	return false;
}

bool NixiesLake::SetEnterState(Player* pPlayer, bool state, bool member)
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( !member )
	{
		if ( this->GetPlayer(0)->GetID() == pPlayer->GetGUID() && !this->GetPlayer(0)->GetState() )
		{
			if ( this->GetPlayer(0)->GetTime()->Elapsed() )
			{
				state = false;
			}

			this->GetPlayer(0)->SetState(state ? 1: 2);

			sLog->outInfo("nixieslake", "%s -- 1 (State: %d): %s", __FUNCTION__, this->GetPlayer(0)->GetState(), pPlayer->BuildLog().c_str());
			return true;
		}
	}
	else
	{
		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			if ( this->GetPlayer(i)->GetID() == pPlayer->GetGUID() && !this->GetPlayer(i)->GetState() )
			{
				if ( this->GetPlayer(i)->GetTime()->Elapsed() )
				{
					state = false;
				}

				this->MemberEnterNotify(state ? 1: 0);
				this->GetPlayer(i)->SetState(state ? 1: 2);

				sLog->outInfo("nixieslake", "%s -- %d (State: %d): %s", __FUNCTION__, i + 1, this->GetPlayer(i)->GetState(), pPlayer->BuildLog().c_str());
				return true;
			}
		}
	}

	return false;
}

void NixiesLake::IceBreathDamage(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	POINTER_PCT_LOG(NIXIES_LAKE_ICE_BREATH_DAMAGE, lpMsg, Packet, 0);

	if ( pPlayer->GetEntry() != lpMsg->target )
	{
		return;
	}

	if ( !this->IsPlayerInSanctuary(pPlayer) )
	{
		return;
	}

	Monster* pMonster = sObjectMgr->FindMonster(lpMsg->boss);
	
	if ( !pMonster )
	{
		return;
	}

	MonsterAI* pAI = pMonster->AIGroup("nixies_lake_boss_ai");

	if ( !pAI )
	{
		return;
	}

	pAI->OnAttack(pPlayer, false);
}

void NixiesLake::ChangeState(NixiesLakeState state)
{
	this->SetState(state);

	switch ( this->GetState() )
	{
	case NIXIES_LAKE_STATE_NONE:
		{
			this->SetState_None();
		} break;

	case NIXIES_LAKE_STATE_OPEN:
		{
			this->SetState_Open();
		} break;

	case NIXIES_LAKE_STATE_STANDBY:
		{
			this->SetState_Standby();
		} break;

	case NIXIES_LAKE_STATE_OCCUPY:
		{
			this->SetState_Occupy();
		} break;

	case NIXIES_LAKE_STATE_END:
		{
			this->SetState_End();
		} break;
	}
}

void NixiesLake::Update()
{
	switch ( this->GetState() )
	{
	case NIXIES_LAKE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case NIXIES_LAKE_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case NIXIES_LAKE_STATE_STANDBY:
		{
			this->ProcState_Standby();
		} break;

	case NIXIES_LAKE_STATE_OCCUPY:
		{
			this->ProcState_Occupy();
		} break;

	case NIXIES_LAKE_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}
	
void NixiesLake::SetState_None()
{
	this->GetTime()->Start(sGameServer->GetNixiesLakeClosedTime());
	this->GetSecondTimer()->Start();

	this->ClearPlayer();

	sLog->outInfo("nixieslake", "%s", __FUNCTION__);
}
	
void NixiesLake::SetState_Open()
{
	this->GetSecondTimer()->Start();

	this->ClearPlayer();
	
	sLog->outInfo("nixieslake", "%s", __FUNCTION__);
}

void NixiesLake::SetState_Standby()
{
	this->GetTime()->Start(sGameServer->GetNixiesLakeStandbyTime());
	this->GetSecondTimer()->Start();

	sLog->outInfo("nixieslake", "%s", __FUNCTION__);
}
	
void NixiesLake::SetState_Occupy()
{
	this->GetTime()->Start(sGameServer->GetNixiesLakeBattleTime());
	this->GetSecondTimer()->Start();

	this->CreateBoss();

	sLog->outInfo("nixieslake", "%s", __FUNCTION__);
}
	
void NixiesLake::SetState_End()
{
	this->GetTime()->Start(NIXIES_LAKE_END_TIME);
	this->GetSecondTimer()->Start();

	sLog->outInfo("nixieslake", "%s", __FUNCTION__);
}
	
void NixiesLake::ProcState_None()
{
	if ( !sGameServer->IsNixiesLakeEnabled() )
	{
		return;
	}

	if ( this->GetTime()->Elapsed() )
	{
		this->ChangeState(NIXIES_LAKE_STATE_OPEN);
	}
}
	
void NixiesLake::ProcState_Open()
{

}

void NixiesLake::ProcState_Standby()
{
	if ( this->GetTime()->Elapsed() )
	{
		this->MemberEnterNotify(2);

		this->ChangeState(NIXIES_LAKE_STATE_OPEN);
	}
}
	
void NixiesLake::ProcState_Occupy()
{
	if (this->GetSecondTimer()->Elapsed(IN_MILLISECONDS))
	{
		int32 count = 0;

		for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
		{
			if (!this->GetPlayer(i)->GetID())
			{
				continue;
			}

			Player* pPlayer = sObjectMgr->FindPlayerByGUID(this->GetPlayer(i)->GetID());

			if (!this->IsPlayerInSanctuary(pPlayer))
			{
				this->GetPlayer(i)->Reset();
				continue;
			}

			++count;
		}

		if (count <= 0)
		{
			this->ChangeState(NIXIES_LAKE_STATE_OPEN);
		}
	}

	if (this->GetTime()->Elapsed())
	{
		for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
		{
			if (!this->GetPlayer(i)->GetID())
			{
				continue;
			}

			Player* pPlayer = sObjectMgr->FindPlayerByGUID(this->GetPlayer(i)->GetID());

			if (this->IsPlayerInSanctuary(pPlayer))
			{
				pPlayer->TeleportToLocation(WORLD_NIXIES_LAKE);

				NIXIES_LAKE_LEAVE pMsg;
				pPlayer->SEND_PCT(pMsg);
			}
		}

		this->ChangeState(NIXIES_LAKE_STATE_OPEN);
	}
}
	
void NixiesLake::ProcState_End()
{
	if ( this->GetTime()->Elapsed() )
	{
		this->ChangeState(NIXIES_LAKE_STATE_NONE);
	}
}

void NixiesLake::CreateBoss()
{
	Monster* pMonster = sObjectMgr->MonsterTryAdd(746, WORLD_NIXIES_LAKE);

	if ( !pMonster )
	{
		this->ChangeState(NIXIES_LAKE_STATE_OPEN);
		return;
	}

	pMonster->SetWorldId(WORLD_NIXIES_LAKE);
	pMonster->SetBasicLocation(30, 218, 30, 218);
	pMonster->SetDirection(1);
	pMonster->SetMoveDistance(50);
	pMonster->SetSpawnTime(1 * IN_MILLISECONDS);
	pMonster->SetSpawnTick(GetTickCount());
	pMonster->SetRespawn(IN_MILLISECONDS);
	pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	pMonster->SetDespawnTick(GetTickCount());
	pMonster->AddToWorld();
}

void NixiesLake::ClearPlayer()
{
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		this->GetPlayer(i)->Reset();
	}
}

bool NixiesLake::IsPlayerInSanctuary(Player* pPlayer) const
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return false;
	}

	if (pPlayer->GetWorldId() != WORLD_NIXIES_LAKE)
	{
		return false;
	}

	if ((pPlayer->GetX() >= g_NixiesLakeZone[0].x && pPlayer->GetX() <= g_NixiesLakeZone[1].x) &&
		(pPlayer->GetY() >= g_NixiesLakeZone[0].y && pPlayer->GetY() <= g_NixiesLakeZone[1].y))
	{
		return true;
	}

	return false;
}

bool NixiesLake::IsInSanctuary(uint16 world, int16 x1, int16 y1, int16 x2, int16 y2) const
{
	if (world != WORLD_NIXIES_LAKE)
	{
		return false;
	}

	if ((x1 >= g_NixiesLakeZone[0].x && x1 <= g_NixiesLakeZone[1].x) &&
		(y1 >= g_NixiesLakeZone[0].y && y1 <= g_NixiesLakeZone[1].y))
	{
		return true;
	}

	if ((x2 >= g_NixiesLakeZone[0].x && x2 <= g_NixiesLakeZone[1].x) &&
		(y2 >= g_NixiesLakeZone[0].y && y2 <= g_NixiesLakeZone[1].y))
	{
		return true;
	}

	return false;
}

bool NixiesLake::IsPlayerAllowed(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return false;
	}

	for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetPlayer(i)->GetID() == pPlayer->GetGUID())
		{
			return true;
		}
	}

	return false;
}

void NixiesLake::ReadyToMove()
{
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( !this->GetPlayer(i)->GetID() )
		{
			continue;
		}

		if ( this->GetPlayer(i)->GetState() == 1 )
		{
			continue;
		}

		return;
	}

	this->ChangeState(NIXIES_LAKE_STATE_OCCUPY);

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( !this->GetPlayer(i)->GetID() )
		{
			continue;
		}

		Player* pPlayer = sObjectMgr->FindPlayerByGUID(this->GetPlayer(i)->GetID());

		if ( !Player::IsPlayerBasicState(pPlayer, false) )
		{
			continue;
		}

		if ( pPlayer->GetRegenStatus() != REGEN_NONE )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != WORLD_NIXIES_LAKE )
		{
			continue;
		}

		if ( !this->FindEntranceItem(pPlayer, true) )
		{
			continue;
		}

		pPlayer->MoveToGate(NIXIES_LAKE_GATE);

		QUEST_SURVIVAL_TIMER pMsg;
		pMsg.type = 1;
		pMsg.increase = 0;
		pMsg.time = sGameServer->GetNixiesLakeBattleTime();
		pPlayer->SEND_PCT(pMsg);
		
		sLog->outInfo("nixieslake", "%s -- %d: %s", __FUNCTION__, i + 1, pPlayer->BuildLog().c_str());
	}
}

void NixiesLake::SendPacket(uint8 * Packet, uint16 size)
{
	World* pWorld = sWorldMgr->GetWorld(WORLD_NIXIES_LAKE);

	if ( !pWorld )
	{
		return;
	}

	WorldPlayerMap const& character_map = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !sNixiesLake->IsPlayerInSanctuary(pPlayer) )
		{
			continue;
		}

		pPlayer->sendPacket(Packet, size);
	}
}