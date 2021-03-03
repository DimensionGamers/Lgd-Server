DoppelgangerZone::DoppelgangerZone()
{
	
}

DoppelgangerZone::~DoppelgangerZone()
{

}

void DoppelgangerZone::Reset()
{
	this->SetPartyID(PARTY_NULL);
	this->SetMonsterCount(0);
	this->SetMonsterPosition(0);
	this->SetMonsterPassed(0);
	this->SetBossPosition(0);
	this->SetQuest(DOPPELGANGER_QUEST_NONE);

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		this->GetPlayer(i)->Reset();
	}
}

int32 DoppelgangerZone::PlayerGetCount() const
{
	int32 count = 0;

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( !this->GetPlayer(i)->GetPlayer() )
		{
			continue;
		}

		++count;
	}

	return count;
}
	
bool DoppelgangerZone::PartyAdd(Player* pPlayer)
{
	Party* pParty = pPlayer->GetParty();

	if ( !pParty )
	{
		return false;
	}

	return false;
}
		
void DoppelgangerZone::UpdatePlayer()
{
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		{
			this->GetPlayer(i)->SetPlayer(nullptr);
			continue;
		}

		if ( pPlayer->GetRegenStatus() != REGEN_NONE )
		{
			continue;
		}

		if ( !DG_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			pPlayer->EventResetData();

			if ( sDoppelganger->GetGround(this->GetGround())->GetState() == DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
			{
				this->SendEnd(pPlayer, 1);
			}

			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( pPlayer->GetPartyID() != this->GetPartyID() )
		{
			pPlayer->EventResetData();

			if ( sDoppelganger->GetGround(this->GetGround())->GetState() == DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
			{
				this->SendEnd(pPlayer, 1);
			}

			pPlayer->TeleportToGate(DOPPELGANGER_GATE);
			this->GetPlayer(i)->Reset();
			continue;
		}
	}
}

void DoppelgangerZone::UpdatePlayerPosition()
{
	int32 step = Util::Distance(g_DoppelgangerPosition[this->GetZone()].start_x, g_DoppelgangerPosition[this->GetZone()].start_y, 
								g_DoppelgangerPosition[this->GetZone()].end_x, g_DoppelgangerPosition[this->GetZone()].end_y) / DOPPELGANGER_DISTANCE;

	int32 distance = DOPPELGANGER_DISTANCE;

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		distance = Util::Distance(pPlayer->GetX(), pPlayer->GetY(), g_DoppelgangerPosition[this->GetZone()].end_x, g_DoppelgangerPosition[this->GetZone()].end_y);

		distance /= step;

		if ( distance > DOPPELGANGER_DISTANCE )
		{
			distance = DOPPELGANGER_DISTANCE;
		}

		if ( distance < 0 )
		{
			distance = 0;
		}

		this->GetPlayer(i)->SetPosition(distance);
	}
}
	
void DoppelgangerZone::UpdateMonsterPosition()
{
	this->SetMonsterPosition(0);
	this->SetBossPosition(0);

	World * pWorld = sWorldMgr->GetWorld(g_DoppelgangerZoneData[this->GetGround()][this->GetZone()].world);

	if ( !pWorld )
	{
		return;
	}

	int32 step = Util::Distance(g_DoppelgangerPosition[this->GetZone()].start_x, g_DoppelgangerPosition[this->GetZone()].start_y, 
								g_DoppelgangerPosition[this->GetZone()].end_x, g_DoppelgangerPosition[this->GetZone()].end_y) / DOPPELGANGER_DISTANCE;

	int32 distance = DOPPELGANGER_DISTANCE;

	WorldObjectMap const& object_map = pWorld->GetAllObjects();
	for ( WorldObjectMap::const_iterator it = object_map.begin(); it != object_map.end(); ++it )
	{
		Object * pObject = it->second;

		if ( !pObject )
		{
			continue;
		}

		Monster* pMonster = pObject->ToCreature();

		if ( !pMonster )
		{
			continue;
		}

		if ( !pMonster->IsPlaying() || !pMonster->IsLive() )
		{
			continue;
		}

		if ( pMonster->GetEventId() != EVENT_DOPPELGANGER )
		{
			continue;
		}

		if ( pMonster->GetEventGround() != this->GetGround() )
		{
			continue;
		}

		if ( pMonster->GetEventStage() != this->GetZone() )
		{
			continue;
		}

		distance = Util::Distance(pMonster->GetX(), pMonster->GetY(), g_DoppelgangerPosition[this->GetZone()].end_x, g_DoppelgangerPosition[this->GetZone()].end_y);

		distance /= step;

		if ( distance > DOPPELGANGER_DISTANCE )
		{
			distance = DOPPELGANGER_DISTANCE;
		}

		if ( distance < 0 )
		{
			distance = 0;
		}
		
		if ( pMonster->AIGroup("ai_doppelganger") )
		{
			this->LimitMinMonsterPosition(distance);
		}

		if ( pMonster->AIGroup("ai_doppelganger_final_boss") )
		{
			this->SetBossPosition(distance);
		}
	}
}

void DoppelgangerZone::UpdateMonsterPassed()
{
	if ( this->GetMonsterPassed() < MAX_DOPPELGANGER_MONSTER_PASSED )
	{
		return;
	}

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		this->SendEnd(pPlayer, 2);
	}
	
	this->PlayerMoveOut();
}
	
void DoppelgangerZone::PlayerMoveOut()
{
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( pPlayer )
		{
			pPlayer->TeleportToGate(DOPPELGANGER_GATE);
			pPlayer->EventResetData();
		}

		this->GetPlayer(i)->Reset();
	}
}

void DoppelgangerZone::PlayerSendPacket(uint8 * Packet, uint16 size)
{
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		pPlayer->sendPacket(Packet, size);
	}
}

void DoppelgangerZone::CreateChest()
{
	DoppelgangerLevelData const* pLevelData = sDoppelganger->GetLevel(this->GetGround());

	if ( !pLevelData )
	{
		return;
	}

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		if ( DG_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			Monster * pMonster = sObjectMgr->MonsterTryAdd(541, pPlayer->GetWorldId());
			
			if ( pMonster )
			{
				pMonster->SetWorldId(pPlayer->GetWorldId());
				pMonster->SetBasicLocation(pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetX(), pPlayer->GetY());
				pMonster->SetScriptName("ai_doppelganger_chest");
				pMonster->SetInstance(-1);
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
				pMonster->SetEventId(EVENT_DOPPELGANGER);
				pMonster->SetEventGround(pPlayer->GetEventGround());
				pMonster->SetEventStage(pPlayer->GetEventStage());
				pMonster->SetItemBag(pLevelData->GetSilverChestItemBag());
				pMonster->AddAdditionalDataInt(0, pPlayer->GetGUID());
				pMonster->AddToWorld();
			}
		}
	}
}

void DoppelgangerZone::OpenChest(Monster* pChest, Player* pPlayer)
{
	if ( !pChest->IsLive() )
	{
		return;
	}

	DoppelgangerLevelData const* pLevelData = sDoppelganger->GetLevel(this->GetGround());

	if ( pLevelData )
	{
		if (roll_chance_i(pLevelData->GetLarvaRate(), 10000))
		{
			for ( int32 i = 0; i < pLevelData->GetLarvaCount(); ++i )
			{
				this->AddLarva(pChest);
			}
		}
		else
		{
			sItemBagMgr->ExecuteItemBag(ITEM_BAG_MISC, pPlayer, pLevelData->GetSilverChestItemBag());
		}
	}

	pChest->Kill();
}

void DoppelgangerZone::AddLarva(Monster* pChest)
{
	Monster * pMonster = sObjectMgr->MonsterTryAdd(532, pChest->GetWorldId());

	if ( pMonster )
	{
		pMonster->SetWorldId(pChest->GetWorldId());
		pMonster->SetBasicLocation(pChest->GetX() - 2, pChest->GetY() - 2, pChest->GetX() + 2, pChest->GetY() + 2);
		pMonster->SetScriptName("ai_doppelganger_larva");
		pMonster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
		pMonster->SetInstance(-1);
		pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		pMonster->SetEventId(EVENT_DOPPELGANGER);
		pMonster->SetEventGround(pChest->GetEventGround());
		pMonster->SetEventStage(pChest->GetEventStage());
		pMonster->SetMoveDistance(100);

		pMonster->AddToWorld();
	}
}

DoppelgangerAttribute const* DoppelgangerZone::GetDoppelgangerAttribute(DoppelgangerWorldAttribute type) const
{
	DoppelgangerAttribute const* pDoppelgangerAttribute = nullptr;

	switch ( type )
	{
	case DOPPELGANGER_WORLD_ATTRIBUTE_START:
		{
			pDoppelgangerAttribute = &g_DoppelgangerStartAttribute[this->GetZone()];
		} break;

	case DOPPELGANGER_WORLD_ATTRIBUTE_FINAL:
		{
			pDoppelgangerAttribute = &g_DoppelgangerFinalAttribute[this->GetZone()];
		} break;

	case DOPPELGANGER_WORLD_ATTRIBUTE_MIDDLE:
		{
			pDoppelgangerAttribute = &g_DoppelgangerMiddleAttribute;
		} break;
	}

	return pDoppelgangerAttribute;
}

void DoppelgangerZone::UpdateWorldAttribute(DoppelgangerWorldAttribute type, bool apply)
{
	DoppelgangerAttribute const* pDoppelgangerAttribute = this->GetDoppelgangerAttribute(type);

	if ( !pDoppelgangerAttribute )
	{
		return;
	}

	World * pWorld = sWorldMgr->GetWorld(g_DoppelgangerZoneData[this->GetGround()][this->GetZone()].world);

	if ( pWorld )
	{
		for ( int16 x = pDoppelgangerAttribute->x1; x <= pDoppelgangerAttribute->x2; ++x )
		{
			for ( int16 y = pDoppelgangerAttribute->y1; y <= pDoppelgangerAttribute->y2; ++y )
			{
				pWorld->ApplyAttribute(x, y, 4, apply);
			}
		}
	}

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		this->UpdateWorldAttribute(pPlayer, type, apply);
	}
}
	
void DoppelgangerZone::UpdateWorldAttribute(Player* pPlayer, DoppelgangerWorldAttribute type, bool apply)
{
	DoppelgangerAttribute const* pDoppelgangerAttribute = this->GetDoppelgangerAttribute(type);

	if ( !pDoppelgangerAttribute )
	{
		return;
	}

	int16 coord[4] = { pDoppelgangerAttribute->x1, pDoppelgangerAttribute->y1, pDoppelgangerAttribute->x2, pDoppelgangerAttribute->y2 };

	pPlayer->SendWorldAttribute(0, 4, 1, apply, coord);
}

void DoppelgangerZone::RemovePlayer(uint32 guid)
{
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetPlayer(i)->GetGUID() == guid )
		{
			this->GetPlayer(i)->Reset();
			return;
		}
	}
}

void DoppelgangerZone::MonsterPassed()
{
	this->IncreaseMonsterPassed(1);
	this->SendMonsterPassed();
}

void DoppelgangerZone::SendMonsterPassed()
{
	DOPPELGANGER_MONSTER_PASSED pMsg(MAX_DOPPELGANGER_MONSTER_PASSED, this->GetMonsterPassed());

	this->PlayerSendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void DoppelgangerZone::SendEnd(Player* pPlayer, uint8 type, uint32 experience)
{
	DOPPELGANGER_END pMsg(type, experience);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

/**********************************************************************************************

**********************************************************************************************/

DoppelgangerGround::DoppelgangerGround()
{
	
}

DoppelgangerGround::~DoppelgangerGround()
{

}

void DoppelgangerGround::Update()
{
	switch ( this->GetState() )
	{
	case DOPPELGANGER_GROUND_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case DOPPELGANGER_GROUND_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case DOPPELGANGER_GROUND_STATE_STANDBY_PVE:
		{
			this->ProcState_StandbyPVE();
		} break;

	case DOPPELGANGER_GROUND_STATE_PLAYING_PVE:
		{
			this->ProcState_PlayingPVE();
		} break;

	case DOPPELGANGER_GROUND_STATE_STANDBY_PVP:
		{
			this->ProcState_StandbyPVP();
		} break;

	case DOPPELGANGER_GROUND_STATE_PLAYING_PVP:
		{
			this->ProcState_PlayingPVP();
		} break;

	case DOPPELGANGER_GROUND_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void DoppelgangerGround::SetState_None()
{
	this->SetState(DOPPELGANGER_GROUND_STATE_NONE);
	this->GetTime()->Reset();
	this->SetMessageSent(false);
	this->SetWinnerZone(MAX_DOPPELGANGER_ZONE);

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		this->GetZone(i)->PlayerMoveOut();
		this->GetZone(i)->Reset();
	}

	this->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_START, true);
	this->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_FINAL, true);
	this->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_MIDDLE, true);
	
	sLog->outInfo("doppelganger", "%s (%u)", __FUNCTION__, this->GetGround() + 1);
}

void DoppelgangerGround::SetState_Open()
{
	this->SetState(DOPPELGANGER_GROUND_STATE_OPEN);
	this->GetTime()->Reset();

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		this->GetZone(i)->Reset();
	}

	sLog->outInfo("doppelganger", "%s (%u)", __FUNCTION__, this->GetGround() + 1);
}

void DoppelgangerGround::SetState_StandbyPVE()
{
	this->SetState(DOPPELGANGER_GROUND_STATE_STANDBY_PVE);
	this->GetTime()->Start(DOPPELGANGER_GROUND_STANDBY_PVE_TIME * IN_MILLISECONDS);

	sLog->outInfo("doppelganger", "%s (%u)", __FUNCTION__, this->GetGround() + 1);
}

void DoppelgangerGround::SetState_PlayingPVE()
{
	this->SetState(DOPPELGANGER_GROUND_STATE_PLAYING_PVE);
	this->GetTime()->Start(DOPPELGANGER_GROUND_PLAYING_PVE_TIME * MINUTE * IN_MILLISECONDS);
	this->SendTimer(2);
	this->SendMonsterPassed();
	this->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_START, false);
	this->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_MIDDLE, true);
	this->AddMonster();

	sLog->outInfo("doppelganger", "%s (%u)", __FUNCTION__, this->GetGround() + 1);
}

void DoppelgangerGround::SetState_StandbyPVP()
{
	this->SetState(DOPPELGANGER_GROUND_STATE_STANDBY_PVP);
	this->GetTime()->Start(DOPPELGANGER_GROUND_STANDBY_PVP_TIME * IN_MILLISECONDS);
	this->SendTimer(3);

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		for ( int32 n = 0; n < MAX_PARTY_MEMBERS; ++n )
		{
			Player* pPlayer = this->GetZone(i)->GetPlayer(n)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			pPlayer->SendNotice(NOTICE_GLOBAL, "[Doppelganger] PVP Battle will start in %d second(s)", DOPPELGANGER_GROUND_STANDBY_PVP_TIME);
		}
	}

	sLog->outInfo("doppelganger", "%s (%u)", __FUNCTION__, this->GetGround() + 1);
}

void DoppelgangerGround::SetState_PlayingPVP()
{
	this->SetState(DOPPELGANGER_GROUND_STATE_PLAYING_PVP);
	this->GetTime()->Start(DOPPELGANGER_GROUND_PLAYING_PVP_TIME * MINUTE * IN_MILLISECONDS);
	this->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_FINAL, true);
	this->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_MIDDLE, false);
	this->MovePlayersToMiddle();
	
	sLog->outInfo("doppelganger", "%s (%u)", __FUNCTION__, this->GetGround() + 1);
}

void DoppelgangerGround::SetState_End()
{
	this->SetState(DOPPELGANGER_GROUND_STATE_END);
	this->GetTime()->Start(MINUTE * IN_MILLISECONDS);
	this->SendTimer(3);

	sLog->outInfo("doppelganger", "%s (%u)", __FUNCTION__, this->GetGround() + 1);
}

void DoppelgangerGround::ProcState_None()
{

}

void DoppelgangerGround::ProcState_Open()
{
	if ( this->GetSecondTime()->Elapsed(IN_MILLISECONDS) )
	{
		this->UpdatePlayer();
	}
}

void DoppelgangerGround::ProcState_StandbyPVE()
{
	if ( this->GetSecondTime()->Elapsed(IN_MILLISECONDS) )
	{
		this->UpdatePlayer();
		this->SendMessageID(DOPPELGANGER_MESSAGE_START);
	}

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_PlayingPVE();
	}
}

void DoppelgangerGround::ProcState_PlayingPVE()
{
	if ( this->GetSecondTime()->Elapsed(IN_MILLISECONDS) )
	{
		this->UpdatePlayer();
		this->UpdatePlayerPosition();
		this->UpdateMonsterPosition();
		this->SendData();
		this->UpdateMonsterPassed();
	}

	if ( this->GetTime()->Elapsed() )
	{
		if ( this->GetActiveZoneCount() <= 1 )
		{
			this->SendEnd(3);
			this->SetState_End();
		}
		else
		{
			this->SetState_StandbyPVP();
		}
	}
}

void DoppelgangerGround::ProcState_StandbyPVP()
{
	if ( this->GetSecondTime()->Elapsed(IN_MILLISECONDS) )
	{
		this->UpdatePlayer();
	}

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_PlayingPVP();
	}
}

void DoppelgangerGround::ProcState_PlayingPVP()
{
	if ( this->GetSecondTime()->Elapsed(IN_MILLISECONDS) )
	{
		this->UpdatePlayer();
	}
	
	if ( this->GetTime()->Elapsed() || this->GetActiveZoneCount() <= 1 )
	{
		this->ProcessReward();
		this->SetState_End();
	}
}

void DoppelgangerGround::ProcState_End()
{
	if ( this->GetSecondTime()->Elapsed(IN_MILLISECONDS) )
	{
		this->UpdatePlayer();
		this->SendMessageID(DOPPELGANGER_MESSAGE_END);
	}
	
	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_None();
	}
}

void DoppelgangerGround::UpdatePlayer()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		this->GetZone(i)->UpdatePlayer();
	}
}

void DoppelgangerGround::UpdatePlayerPosition()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetZone(i)->PlayerGetCount() <= 0 )
		{
			continue;
		}

		this->GetZone(i)->UpdatePlayerPosition();
	}
}

void DoppelgangerGround::UpdateMonsterPosition()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetZone(i)->PlayerGetCount() <= 0 )
		{
			continue;
		}

		this->GetZone(i)->UpdateMonsterPosition();
	}
}

void DoppelgangerGround::UpdateMonsterPassed()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetZone(i)->PlayerGetCount() <= 0 )
		{
			continue;
		}

		this->GetZone(i)->UpdateMonsterPassed();
	}
}

void DoppelgangerGround::SendData()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetZone(i)->PlayerGetCount() <= 0 )
		{
			continue;
		}

		uint8 buffer[1000];
		POINTER_PCT(DOPPELGANGER_UPADTE_PLAYER, head, buffer, 0);
		POINTER_PCT(DOPPELGANGER_UPADTE_PLAYER_BODY, body, buffer, sizeof(DOPPELGANGER_UPADTE_PLAYER));
		head->count = 0;
		head->time = this->GetTime()->GetRemain() / IN_MILLISECONDS;

		for ( int32 n = 0; n < MAX_PARTY_MEMBERS; ++n )
		{
			Player* pPlayer = this->GetZone(i)->GetPlayer(n)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			body[head->count].index = pPlayer->GetEntry();
			body[head->count].visible = 1;
			body[head->count].position = this->GetZone(i)->GetPlayer(n)->GetPosition();
			++head->count;
		}

		head->h.set(HEADCODE_EVENT_MISC, SUBCODE_DOUBLE_GOER_PLAYER_POSITION, sizeof(DOPPELGANGER_UPADTE_PLAYER) + (head->count * sizeof(DOPPELGANGER_UPADTE_PLAYER_BODY)));

		DOPPELGANGER_MONSTER_POSITION pMsg1(this->GetZone(i)->GetMonsterPosition());
		DOPPELGANGER_BOSS_POSITION pMsg2(this->GetZone(i)->GetQuest() == DOPPELGANGER_QUEST_COMPLETED, this->GetZone(i)->GetBossPosition());

		this->GetZone(i)->PlayerSendPacket(buffer, head->h.get_size());
		this->GetZone(i)->PlayerSendPacket((uint8*)&pMsg1, pMsg1.h.get_size());
		this->GetZone(i)->PlayerSendPacket((uint8*)&pMsg2, pMsg2.h.get_size());
	}
}

void DoppelgangerGround::SendMonsterPassed()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		this->GetZone(i)->SendMonsterPassed();
	}
}

void DoppelgangerGround::SendTimer(uint8 type)
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		for ( int32 n = 0; n < MAX_PARTY_MEMBERS; ++n )
		{
			this->SendTimer(this->GetZone(i)->GetPlayer(n)->GetPlayer(), type);
		}
	}
}

void DoppelgangerGround::SendTimer(Player* pPlayer, uint8 type)
{
	if ( !pPlayer )
	{
		return;
	}

	DOPPELGANGER_TIMER_INIT pMsg(type);
	pPlayer->SEND_PCT(pMsg);
}

void DoppelgangerGround::SendEnd(uint8 type)
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		for ( int32 n = 0; n < MAX_PARTY_MEMBERS; ++n )
		{
			Player* pPlayer = this->GetZone(i)->GetPlayer(n)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			this->GetZone(i)->SendEnd(pPlayer, type);
		}
	}
}

int32 DoppelgangerGround::GetActiveZoneCount() const
{
	int32 count = 0;

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetZone(i)->PlayerGetCount() <= 0 )
		{
			continue;
		}

		++count;
	}

	return count;
}

void DoppelgangerGround::AddMonster()
{
	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_DOPPELGANGER )
		{
			continue;
		}

		if ( (*it)->doppelganger.ground != this->GetGround() )
		{
			continue;
		}

		if ( (*it)->doppelganger.zone < 0 || (*it)->doppelganger.zone >= MAX_DOPPELGANGER_ZONE )
		{
			continue;
		}

		if ( this->GetZone((*it)->doppelganger.zone)->PlayerGetCount() <= 0 )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_NORMAL);
			pMonster->SetMoveDistance(200);

			if ( (*it)->doppelganger.despawn_time > 0 )
			{
				pMonster->SetDespawnTime((*it)->doppelganger.despawn_time * IN_MILLISECONDS);
				pMonster->SetDespawnTick(MyGetTickCount());
				pMonster->SetDespawnType(MONSTER_DESPAWN_DIE);
			}

			pMonster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
		
			if ( (*it)->doppelganger.boss )
			{
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			}

			pMonster->SetEventGround((*it)->doppelganger.ground);
			pMonster->SetEventStage((*it)->doppelganger.zone);
			pMonster->AddToWorld();
		}
	}
}

void DoppelgangerGround::SendMessageID(uint8 id)
{
	if ( this->GetTime()->GetRemain() <= (30 * IN_MILLISECONDS) && !this->IsMessageSent(id) )
	{
		this->SetMessageSent(id, true);
		EVENT_MESSAGE_SEND pMsg(g_DoppelgangerMessage[id]);

		for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
		{
			this->GetZone(i)->PlayerSendPacket(MAKE_PCT(pMsg));
		}
	}
}

void DoppelgangerGround::UpdateWorldAttribute(DoppelgangerWorldAttribute type, bool apply)
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		this->GetZone(i)->UpdateWorldAttribute(type, apply);
	}
}

void DoppelgangerGround::RemovePlayer(uint32 guid)
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		this->GetZone(i)->RemovePlayer(guid);
	}
}

uint8 DoppelgangerGround::GetEmptyZone() const
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetZone(i)->PlayerGetCount() <= 0 )
		{
			return i;
		}
	}

	return MAX_DOPPELGANGER_ZONE;
}

void DoppelgangerGround::MovePlayersToMiddle()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		for ( int32 n = 0; n < MAX_PARTY_MEMBERS; ++n )
		{
			Player* pPlayer = this->GetZone(i)->GetPlayer(n)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			pPlayer->MoveToGate(g_DoppelgangerZoneData[this->GetGround()][i].middle);
		}
	}
}

void DoppelgangerGround::ProcessReward()
{
	uint8 winner = this->CalculateWinnerZone();

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( winner == i )
		{
			continue;
		}

		this->GetZone(i)->PlayerMoveOut();
	}

	if ( winner == MAX_DOPPELGANGER_ZONE )
	{
		return;
	}

	this->SetWinnerZone(winner);

	this->CreateGoldenChest();
	
	DoppelgangerLevelData const* pLevelData = sDoppelganger->GetLevel(this->GetGround());

	if ( !pLevelData )
	{
		return;
	}

	int32 experience_reward = pLevelData->GetExperience();

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pPlayer = this->GetZone(winner)->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		int64 experience = experience_reward;

		pPlayer->GiveExperience(experience, true, true);

		this->GetZone(winner)->SendEnd(pPlayer, 0, experience_reward);

		sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(JEWEL::LIFE, 0, 1), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::DOPPELGANGER);

		sEventMgr->GiveContributionReward(pPlayer, EVENT_DOPPELGANGER, this->GetGround() + 1, 1, GremoryCaseReward::DOPPELGANGER);
	}
}

uint8 DoppelgangerGround::CalculateWinnerZone() const
{
	int32 survival_count[MAX_DOPPELGANGER_ZONE] = { 0, 0, 0, 0 };
	int32 current_value = 0;
	int32 current_count = 0;

	int16 min_level[MAX_DOPPELGANGER_ZONE] = { 0, 0, 0, 0 };
	
	uint8 winner_zone = MAX_DOPPELGANGER_ZONE;

	///- Obtengo la cantidad de sobrevivientes y el nivel minimo de cada zona
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		for ( int32 n = 0; n < MAX_PARTY_MEMBERS; ++n )
		{
			Player* pPlayer = this->GetZone(i)->GetPlayer(n)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			survival_count[i]++;

			if ( min_level[i] == 0 || pPlayer->GetTotalLevel() < min_level[i] )
			{
				min_level[i] = pPlayer->GetTotalLevel();
			}

			current_count++;
			winner_zone = i;
		}
	}

	if ( current_count == 1 )
	{
		return winner_zone;
	}

	current_count = 0;
	current_value = 0;
	winner_zone = MAX_DOPPELGANGER_ZONE;

	///- Verifico las zonas con mayor cantidad de sobrevivientes
	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( survival_count[i] == 0 )
		{
			continue;
		}
		
		if ( survival_count[i] >= current_value )
		{
			if ( survival_count[i] > current_value )
			{
				current_count = 1;
				current_value = survival_count[i];
				winner_zone = i;
			}
			else if ( survival_count[i] == current_value )
			{
				current_count++;
			}
		}
	}

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( survival_count[i] < current_value )
		{
			survival_count[i] = 0;
		}
	}

	if ( current_count == 1 )
	{
		return winner_zone;
	}

	///- Verifico las zonas con mayor cantidad de monsters matados
	current_count = 0;
	current_value = 0;
	winner_zone = MAX_DOPPELGANGER_ZONE;

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( survival_count[i] == 0 )
		{
			continue;
		}

		if ( this->GetZone(i)->GetMonsterCount() >= current_value )
		{
			if ( this->GetZone(i)->GetMonsterCount() > current_value )
			{
				current_count = 1;
				current_value = this->GetZone(i)->GetMonsterCount();
				winner_zone = i;
			}
			else if ( this->GetZone(i)->GetMonsterCount() == current_value )
			{
				current_count++;
			}
		}
	}

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetZone(i)->GetMonsterCount() < current_value )
		{
			survival_count[i] = 0;
		}
	}

	if ( current_count == 1 )
	{
		return winner_zone;
	}

	///- Verifico las zonas con menor nivel
	current_count = 0;
	current_value = 0;
	winner_zone = MAX_DOPPELGANGER_ZONE;

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( survival_count[i] == 0 )
		{
			continue;
		}

		if ( current_value == 0 || min_level[i] < current_value )
		{
			current_value = min_level[i];
			winner_zone = i;
		}
	}

	return winner_zone;
}

void DoppelgangerGround::CreateGoldenChest()
{
	Monster * pMonster = sObjectMgr->MonsterTryAdd(542, g_DoppelgangerZoneData[this->GetGround()][this->GetWinnerZone()].world);

	if ( !pMonster )
	{
		return;
	}

	pMonster->SetWorldId(g_DoppelgangerZoneData[this->GetGround()][this->GetWinnerZone()].world);
	pMonster->SetBasicLocation(127, 126, 127, 126);
	pMonster->SetScriptName("ai_doppelganger_chest");
	pMonster->SetInstance(-1);
	pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	pMonster->SetEventId(EVENT_DOPPELGANGER);
	pMonster->SetEventGround(this->GetGround());
	pMonster->SetEventStage(this->GetWinnerZone());
	pMonster->AddToWorld();
}

void DoppelgangerGround::OpenGoldenChest(Player* pPlayer, Monster* pMonster)
{
	DoppelgangerLevelData const* pLevelData = sDoppelganger->GetLevel(this->GetGround());

	if ( !pLevelData )
	{
		return;
	}

	if ( !pPlayer || !pMonster )
	{
		return;
	}

	if ( !pMonster->IsLive() )
	{
		return;
	}

	if ( pPlayer->GetEventStage() != pMonster->GetEventStage() )
	{
		return;
	}

	if ( pPlayer->GetEventStage() != this->GetWinnerZone() )
	{
		return;
	}

	sItemBagMgr->ExecuteItemBag(ITEM_BAG_MISC, pPlayer, pLevelData->GetGoldenChestItemBag());
	pMonster->Kill();
}

/**********************************************************************************************

**********************************************************************************************/

Doppelganger::Doppelganger()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
	{
		this->GetGround(i)->SetGround(i);

		for ( int32 n = 0; n < MAX_DOPPELGANGER_ZONE; ++n )
		{
			this->GetGround(i)->GetZone(n)->SetZone(n);
			this->GetGround(i)->GetZone(n)->SetGround(i);
		}
	}

	this->SetState_None();
}

Doppelganger::~Doppelganger()
{
	for ( DoppelgangerLevelMap::iterator it = this->level_map.begin(); it != this->level_map.end(); ++it )
	{
		MAP_CLEAR(DoppelgangerMonsterMap::iterator, it->second->monsters);
	}

	MAP_CLEAR(DoppelgangerLevelMap::iterator, this->level_map);
}

void Doppelganger::LoadLevel()
{
	for ( DoppelgangerLevelMap::iterator it = this->level_map.begin(); it != this->level_map.end(); ++it )
	{
		MAP_CLEAR(DoppelgangerMonsterMap::iterator, it->second->monsters);
	}

	MAP_CLEAR(DoppelgangerLevelMap::iterator, this->level_map);

	if ( !sGameServer->IsDoppelgangerEnabled() )
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Doppelganger Level...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_doppelganger_level");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			DoppelgangerLevelData * level = new DoppelgangerLevelData;
			level->SetGUID(fields[loop++].GetUInt32());
			level->SetLevelMin(fields[loop++].GetInt16());
			level->SetLevelMax(fields[loop++].GetInt16());
			level->SetExperience(fields[loop++].GetInt32());
			level->SetLarvaRate(fields[loop++].GetUInt16());
			level->SetLarvaCount(fields[loop++].GetInt32());
			level->SetSilverChestItemBag(fields[loop++].GetString());
			level->SetGoldenChestItemBag(fields[loop++].GetString());

			this->level_map[level->GetGUID()] = level;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u doppelganger level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void Doppelganger::LoadMonster()
{
	if ( !sGameServer->IsDoppelgangerEnabled() )
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Doppelganger Monster Settings...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM event_doppelganger_monster_settings");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			if ( DoppelgangerLevelData * pLevel = this->GetLevel(fields[loop++].GetUInt32()) )
			{
				DoppelgangerMonster * monster = new DoppelgangerMonster;
				monster->SetMonster(fields[loop++].GetUInt16());
				monster->SetLevel(fields[loop++].GetInt16());
				monster->SetLife(fields[loop++].GetInt32());
				monster->SetAttackMin(fields[loop++].GetInt32());
				monster->SetAttackMax(fields[loop++].GetInt32());
				monster->SetAttackRate(fields[loop++].GetInt32());
				monster->SetDefense(fields[loop++].GetInt32());
				monster->SetDefenseRate(fields[loop++].GetInt32());
				monster->SetItemBag(fields[loop++].GetString());

				pLevel->monsters[monster->GetMonster()] = monster;

				count++;
			}
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u doppelganger monster settings", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

DoppelgangerLevelData* Doppelganger::GetLevel(uint32 id) const
{
	DoppelgangerLevelMap::const_iterator it = this->level_map.find(id);

	if ( it != this->level_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void Doppelganger::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return;
	}

	if ( !sGameServer->IsDoppelgangerEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Event is disabled.");
		return;
	}

	if ( this->GetState() != DOPPELGANGER_STATE_OPEN )
	{
		this->EnterResult(pPlayer, 6);
		return;
	}

	if ( !sEventMgr->CanEnterEvent(pPlayer, EVENT_DOPPELGANGER) )
	{
		this->EnterResult(pPlayer, 0x0B);
		return;
	}

	POINTER_PCT(DOPPELGANGER_ENTER, lpMsg, Packet, 0);

	uint8 mirror_slot = lpMsg->item_slot + static_cast<uint8>(PLAYER_MAX_EQUIPMENT);

	if ( !use_inventory_range(mirror_slot) )
	{
		this->EnterResult(pPlayer, 1);
		return;
	}

	if ( sGameServer->IsDoppelgangerPKCheck() && pPlayer->GetPKLevel() > sGameServer->GetDoppelgangerPKLevelMax() )
	{
		this->EnterResult(pPlayer, 3);
		return;
	}

	uint8 ground = this->GetEnterGround(pPlayer->GetTotalLevel(), pPlayer->IsMaster());

	if ( ground >= MAX_DOPPELGANGER_GROUND )
	{
		this->EnterResult(pPlayer, 2);
		return;
	}

	if ( pPlayer->GetInventory()->GetItem(mirror_slot)->GetItem() != ITEMGET(14, 111) && 
		(pPlayer->GetInventory()->GetItem(mirror_slot)->GetItem() != ITEMGET(13, 125) || pPlayer->GetInventory()->GetItem(mirror_slot)->GetDurability() <= 0.0f) )
	{
		this->EnterResult(pPlayer, 1);
		return;
	}

	Party* pParty = pPlayer->GetParty();

	if ( !pParty )
	{
		this->EnterResult(pPlayer, 7);
		return;
	}

	uint8 zone = this->GetGround(ground)->GetEmptyZone();
	bool already_exist = false;

	for ( int32 i = 0; i < MAX_DOPPELGANGER_ZONE; ++i )
	{
		if ( this->GetGround(ground)->GetZone(i)->GetPartyID() == PARTY_NULL )
		{
			continue;
		}

		if ( this->GetGround(ground)->GetZone(i)->GetPartyID() == pParty->GetID() )
		{
			already_exist = true;
			zone = i;
			break;
		}
	}

	if ( zone >= MAX_DOPPELGANGER_ZONE )
	{
		this->EnterResult(pPlayer, 2);
		return;
	}

	if ( !already_exist )
	{
		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			{
				this->EnterResult(pPlayer, 7);
				return;
			}

			Player* pMember = pParty->GetMember(i)->GetPlayer();

			if ( !pMember )
			{
				this->EnterResult(pPlayer, 7);
				return;
			}

			if ( pMember->GetWorldId() != WORLD_EVENT_ZONE )
			{
				this->EnterResult(pPlayer, 7);
				return;
			}

			if ( pMember->IsBusy() )
			{
				this->EnterResult(pPlayer, 7);
				return;
			}

			if ( this->GetEnterGround(pMember->GetTotalLevel(), pMember->IsMaster()) != ground )
			{
				this->EnterResult(pPlayer, 8);
				return;
			}

			if ( !sEventMgr->CanEnterEvent(pMember, EVENT_DOPPELGANGER) )
			{
				this->EnterResult(pPlayer, 0x0B);
				return;
			}
		}
	}

	if ( pPlayer->GetInventory()->GetItem(mirror_slot)->GetItem() == ITEMGET(14, 111) )
	{
		pPlayer->ClearItem(mirror_slot);
	}
	else if ( pPlayer->GetInventory()->GetItem(mirror_slot)->GetItem() == ITEMGET(13, 125) )
	{
		pPlayer->DecreaseItemDurabilityByUse(mirror_slot, 1.0f);
	}

	this->EnterResult(pPlayer, 0);

	if ( !already_exist )
	{
		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			Player* pMember = pParty->GetMember(i)->GetPlayer();

			if ( pMember )
			{
				if ( pMember->MoveToGate(g_DoppelgangerZoneData[ground][zone].gate) )
				{
					pMember->SetEventId(EVENT_DOPPELGANGER);
					pMember->SetEventGround(ground);
					pMember->SetEventStage(zone);

					this->GetGround(ground)->GetZone(zone)->GetPlayer(i)->SetPlayer(pMember);
					this->GetGround(ground)->GetZone(zone)->GetPlayer(i)->SetGUID(pMember->GetGUID());
					this->GetGround(ground)->GetZone(zone)->GetPlayer(i)->SetPosition(0);
					this->GetGround(ground)->SendTimer(pMember, 1);

					pMember->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Entered Doppelganger %d", ground + 1);

					sEventMgr->EventEnter(pMember, EVENT_DOPPELGANGER);
				}
			}
		}

		this->GetGround(ground)->GetZone(zone)->SetPartyID(pParty->GetID());
		this->GetGround(ground)->GetZone(zone)->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_START, true);
		this->GetGround(ground)->GetZone(zone)->UpdateWorldAttribute(DOPPELGANGER_WORLD_ATTRIBUTE_FINAL, false);
	}
	else
	{
		DoppelgangerPlayer* pDoppelgangerPlayer = nullptr;

		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			if ( this->GetGround(ground)->GetZone(zone)->GetPlayer(i)->GetGUID() == pPlayer->GetGUID() )
			{
				pDoppelgangerPlayer = this->GetGround(ground)->GetZone(zone)->GetPlayer(i);
				break;
			}
		}

		if ( !pDoppelgangerPlayer )
		{
			for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
			{
				if ( !this->GetGround(ground)->GetZone(zone)->GetPlayer(i)->GetGUID() )
				{
					pDoppelgangerPlayer = this->GetGround(ground)->GetZone(zone)->GetPlayer(i);
					break;
				}
			}
		}

		if ( !pDoppelgangerPlayer )
		{
			return;
		}

		if ( pPlayer->MoveToGate(g_DoppelgangerZoneData[ground][zone].gate) )
		{
			pPlayer->SetEventId(EVENT_DOPPELGANGER);
			pPlayer->SetEventGround(ground);
			pPlayer->SetEventStage(zone);

			pDoppelgangerPlayer->SetPlayer(pPlayer);
			pDoppelgangerPlayer->SetGUID(pPlayer->GetGUID());
			pDoppelgangerPlayer->SetPosition(0);
			this->GetGround(ground)->SendTimer(pPlayer, 1);

			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Entered Doppelganger %d", ground + 1);

			this->GetGround(ground)->GetZone(zone)->UpdateWorldAttribute(pPlayer, DOPPELGANGER_WORLD_ATTRIBUTE_START, true);
			this->GetGround(ground)->GetZone(zone)->UpdateWorldAttribute(pPlayer, DOPPELGANGER_WORLD_ATTRIBUTE_FINAL, false);

			sEventMgr->EventEnter(pPlayer, EVENT_DOPPELGANGER);
		}
	}
}
	
void Doppelganger::EnterResult(Player* pPlayer, uint8 result)
{
	DOPPELGANGER_ENTER_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}
	
uint8 Doppelganger::GetEnterGround(int16 level, bool master) const
{
	for (DoppelgangerLevelMap::const_iterator itr = this->level_map.begin(); itr != this->level_map.end(); ++itr)
	{
		DoppelgangerLevelData const* pLevel = itr->second;

		if (!pLevel)
		{
			continue;
		}

		if (level >= pLevel->GetLevelMin() && level <= pLevel->GetLevelMax())
		{
			return pLevel->GetGUID();
		}
	}

	return MAX_DOPPELGANGER_GROUND;
}

void Doppelganger::SendRemainMinutes(std::string const& message, int32 period, int32 max_minute)
{
	if ( !this->IsMinuteElapsed(period) )
	{
		return;
	}

	if ( max_minute != -1 && (this->GetCurrentMinute() + 1) > max_minute )
	{
		return;
	}

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, message.c_str(), this->GetCurrentMinute() + 1);
}

void Doppelganger::Start()
{
	if ( !sGameServer->IsDoppelgangerEnabled() )
	{
		return;
	}

	if ( this->GetState() != DOPPELGANGER_STATE_NONE )
	{
		return;
	}

	this->SetState_Open();
}
	
void Doppelganger::Update()
{
	switch ( this->GetState() )
	{
	case DOPPELGANGER_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case DOPPELGANGER_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case DOPPELGANGER_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;
	}

	for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
	{
		this->GetGround(i)->Update();
	}
}

void Doppelganger::SetState_None()
{
	this->SetState(DOPPELGANGER_STATE_NONE);
	this->GetTime()->Reset();
	this->GetNotificationTime()->Reset();
	this->SetCurrentMinute(-1);

	sLog->outInfo("doppelganger", "%s", __FUNCTION__);
}

void Doppelganger::SetState_Open()
{
	this->SetState(DOPPELGANGER_STATE_OPEN);
	this->GetTime()->Start(DOPPELGANGER_OPEN_TIME * MINUTE * IN_MILLISECONDS);

	sServerLink->EventNotification(EVENT_NOTIFICATION_DOPPELGANGER, 1);

	this->SetCurrentMinute(-1);

	for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
	{
		this->GetGround(i)->SetState_Open();
	}

	sLog->outInfo("doppelganger", "%s", __FUNCTION__);
}

void Doppelganger::SetState_Playing()
{
	this->SetState(DOPPELGANGER_STATE_PLAYING);

	sServerLink->EventNotification(EVENT_NOTIFICATION_DOPPELGANGER, 0);

	for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetActiveZoneCount() <= 0 )
		{
			this->GetGround(i)->SetState_None();
		}
		else
		{
			this->GetGround(i)->SetState_StandbyPVE();
		}
	}

	sLog->outInfo("doppelganger", "%s", __FUNCTION__);
}

void Doppelganger::ProcState_None()
{

}

void Doppelganger::ProcState_Open()
{
	if ( this->GetNotificationTime()->Elapsed(EVENT_NOTIFICATION_TIME) )
	{
		sServerLink->EventNotification(EVENT_NOTIFICATION_DOPPELGANGER, 1);
	}

	this->SendRemainMinutes("%d minute(s) left before Doppelganger start", 1, 5);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Playing();
	}
}

void Doppelganger::ProcState_Playing()
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() != DOPPELGANGER_GROUND_STATE_NONE )
		{
			return;
		}
	}

	this->SetState_None();
}

void Doppelganger::RemovePlayer(uint32 guid)
{
	for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
	{
		this->GetGround(i)->RemovePlayer(guid);
	}
}

bool Doppelganger::IsIn(Player* pPlayer)
{
	if ( this->GetState() != DOPPELGANGER_STATE_OPEN &&
		 this->GetState() != DOPPELGANGER_STATE_PLAYING )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() != DOPPELGANGER_GROUND_STATE_STANDBY_PVE &&
			 this->GetGround(i)->GetState() != DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
		{
			continue;
		}

		for ( int32 n = 0; n < MAX_DOPPELGANGER_ZONE; ++n )
		{
			if ( this->GetGround(i)->GetZone(n)->GetPartyID() != pPlayer->GetPartyID() )
			{
				continue;
			}

			for ( int32 j = 0; j < MAX_PARTY_MEMBERS; ++j )
			{
				if ( this->GetGround(i)->GetZone(n)->GetPlayer(j)->GetGUID() == pPlayer->GetGUID() )
				{
					this->GetGround(i)->GetZone(n)->GetPlayer(j)->SetPlayer(pPlayer);

					pPlayer->SetEventId(EVENT_DOPPELGANGER);
					pPlayer->SetEventGround(i);
					pPlayer->SetEventStage(n);
					this->GetGround(i)->SendTimer(pPlayer, 2);

					if ( this->GetGround(i)->GetState() == DOPPELGANGER_GROUND_STATE_STANDBY_PVE )
					{
						this->GetGround(i)->GetZone(n)->UpdateWorldAttribute(pPlayer, DOPPELGANGER_WORLD_ATTRIBUTE_START, true);
						this->GetGround(i)->GetZone(n)->UpdateWorldAttribute(pPlayer, DOPPELGANGER_WORLD_ATTRIBUTE_MIDDLE, true);
					}
					else if ( this->GetGround(i)->GetState() == DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
					{
						this->GetGround(i)->GetZone(n)->UpdateWorldAttribute(pPlayer, DOPPELGANGER_WORLD_ATTRIBUTE_MIDDLE, true);
					}

					return true;
				}
			}
		}
	}

	return false;
}