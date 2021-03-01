/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Kanturu.cpp"
*
*/
KanturuMgr::KanturuMgr()
{
	this->SetState(KANTURU_STATE_NONE);
	this->Init();
	this->SetMonsterCount(0);
}

KanturuMgr::~KanturuMgr()
{
}

void KanturuMgr::Init()
{
	for ( uint8 i = 0; i < KANTURU_STATE_MAX; ++i )
	{
		this->GetStateInfo(i)->ResetTimeInfo();
	}
}

void KanturuMgr::Load()
{
	this->Init();

	if ( !sGameServer->IsKanturuEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Kanturu Data...");

	std::string filename("EventData/Kanturu.txt");

	ReadScript file(filename.c_str(), "r");
	
	if ( !file.open() )
		return;

	while ( true )
	{
		if ( file.GetToken() == END )
			break;

		uint8 section = file.TokenNumber;

		while ( true )
		{
			file.GetToken();

			if ( file.IsEnd("end") )
				break;

			switch ( section )
			{
			case 0:
				{
					uint8 state = file.GetUInt8();

					file.GetToken(); int32 condition = file.GetInt32();
					file.GetToken(); int32 value = file.GetInt32();

					if ( state < KANTURU_STATE_MAX )
					{
						this->GetStateInfo(state)->SetStateInfo(state);
						this->GetStateInfo(state)->SetCondition(condition);
						this->GetStateInfo(state)->SetValue(value * IN_MILLISECONDS);
					}
				} break;
			}
		}
	}

	this->GetKanturuStandBy()->Load(filename.c_str());
	this->GetKanturuMaya()->Load(filename.c_str());
	this->GetKanturuNightmare()->Load(filename.c_str());
	this->GetKanturuTower()->Load(filename.c_str());
}

uint8 KanturuMgr::GetSubState() const
{
	uint8 sub_state = KANTURU_STATE_NONE;

	switch ( this->GetState() )
	{
	case KANTURU_STATE_BATTLE_STANDBY:
		{
			sub_state = this->GetKanturuStandBy()->GetState();
		} break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		{
			sub_state = this->GetKanturuMaya()->GetState();
		} break;

	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		{
			sub_state = this->GetKanturuNightmare()->GetState();
		} break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		{
			sub_state = this->GetKanturuTower()->GetState();
		} break;
	}

	return sub_state;
}

int32 KanturuMgr::GetRemainTime() const
{
	int32 remain_time = 0;

	switch ( this->GetState() )
	{
	case KANTURU_STATE_BATTLE_STANDBY:
		{
			remain_time = this->GetKanturuStandBy()->GetRemainTime();
		} break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		{
			remain_time = this->GetKanturuMaya()->GetRemainTime();
		} break;

	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		{
			remain_time = this->GetKanturuNightmare()->GetRemainTime();
		} break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		{
			remain_time = this->GetKanturuTower()->GetRemainTime();
		} break;
	}

	return remain_time;
}
	
bool KanturuMgr::IsEntrance() const
{
	bool entrance = false;

	switch ( this->GetState() )
	{
	case KANTURU_STATE_BATTLE_STANDBY:
		{
			entrance = this->GetKanturuStandBy()->IsEntrance();
		} break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		{
			entrance = this->GetKanturuMaya()->IsEntrance();
		} break;

	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		{
			entrance = this->GetKanturuNightmare()->IsEntrance();
		} break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		{
			entrance = this->GetKanturuTower()->IsEntrance();
		} break;
	}

	return entrance;
}

void KanturuMgr::EntranceInfoNotify(Player* pPlayer)
{
	KANTURU_ENTRANCE_INFO pMsg(this->GetState(), this->GetSubState());
	pMsg.remain_time = this->GetRemainTime();
	pMsg.user_count = this->GetUserCount();
	pMsg.enter = this->IsEntrance();

	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void KanturuMgr::EnterRequest(Player* pPlayer)
{
	if ( !sGameServer->IsKanturuEnabled() )
		return;

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		this->EnterResult(pPlayer, uint8(-1));
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_KANTURU_CORE )
	{
		this->EnterResult(pPlayer, 4);
		return;
	}

	if ( !this->IsEntrance() )
	{
		this->EnterResult(pPlayer, 3);
		return;
	}

	if ( this->GetState() == KANTURU_STATE_BATTLE_OF_MAYA )
	{
		if ( !pPlayer->hasMoonstoneRing() )
		{
			this->EnterResult(pPlayer, 2);
			return;
		}

		if (!pPlayer->GetInventory()->GetItem(WINGS)->IsItem() && !pPlayer->HasMount())
		{
			this->EnterResult(pPlayer, 7);
			return;
		}

		if ( pPlayer->IsActiveInventoryItem(PET_UNIRIA) )
		{
			this->EnterResult(pPlayer, 6);
			return;
		}

		if ( (pPlayer->GetInventory()->GetItem(RING_01)->IsItem() && pPlayer->GetInventory()->GetItem(RING_01)->IsTransformationRing(false)) ||
			 (pPlayer->GetInventory()->GetItem(RING_02)->IsItem() && pPlayer->GetInventory()->GetItem(RING_02)->IsTransformationRing(false)) )
		{
			this->EnterResult(pPlayer, 6);
			return;
		}

		if ( this->AddPlayer(pPlayer) && pPlayer->MoveToGate(KANTURU_GATE_BATTLE) )
		{
			pPlayer->SetEventId(EVENT_KANTURU);
			this->StateNofity(pPlayer);
		}

		return;
	}
	else if ( this->GetState() == KANTURU_STATE_TOWER_OF_REFINEMENT )
	{
		if ( pPlayer->MoveToGate(KANTURU_GATE_TOWER) )
		{
			pPlayer->SetEventId(EVENT_KANTURU);
			this->StateNofity(pPlayer);
		}

		return;
	}

	this->EnterResult(pPlayer, 3);
}

void KanturuMgr::EnterResult(Player* pPlayer, uint8 result)
{
	KANTURU_ENTER_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

bool KanturuMgr::AddPlayer(Player* pPlayer)
{
	if ( this->GetUserCount() >= sGameServer->kanturu_max_players.get() )
		return false;

	pPlayer->SetKanturuAllowed(true);
	return true;
}

void KanturuMgr::StateNofity(Player* pPlayer)
{
	KANTURU_STATE_NOTIFY pMsg(this->GetState(), this->GetSubState());
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void KanturuMgr::StateChange()
{
	KANTURU_STATE_CHANGE pMsg(this->GetState(), this->GetSubState());

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
		{
			continue;
		}

		if ( !pPlayer->IsKanturuAllowed() )
		{
			continue;
		}

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void KanturuMgr::MonsterAndUserNotify()
{
	KANTURU_MONSTER_AND_USER_COUNT pMsg(this->GetMonsterCount(), this->GetUserCount());

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( !pPlayer->IsKanturuAllowed() )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void KanturuMgr::sendNoticeToPlayersOnMap(bool map_only, const char * notice, ...)
{
	ARG(output_notice, notice);

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->Object::IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU &&
			 pPlayer->GetWorldId() != WORLD_KANTURU_CORE &&
			 pPlayer->GetWorldId() != WORLD_KANTURU_BOSS &&
			 map_only )
			continue;

		pPlayer->SendNotice(NOTICE_GLOBAL, output_notice);
	}
}

void KanturuMgr::SendNotice(const char * notice, ...)
{
	ARG(output_notice, notice);

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( !pPlayer->IsKanturuAllowed() )
			continue;

		pPlayer->SendNotice(NOTICE_GLOBAL, output_notice);
	}
}

void KanturuMgr::SendTime(int32 time)
{
	KANTURU_SCENE_TIME pMsg(time);

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( !pPlayer->IsKanturuAllowed() )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void KanturuMgr::SendSuccess(bool success)
{
	KANTURU_SUCCESS pMsg(success);

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( !pPlayer->IsKanturuAllowed() )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void KanturuMgr::SendWideAttack(uint16 entry, uint8 type)
{
	KANTURU_WIDE_ATTACK_AREA pMsg(entry, type);

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( !pPlayer->IsKanturuAllowed() )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void KanturuMgr::ChangeState(uint8 state)
{
	if ( state >= KANTURU_STATE_MAX )
	{
		state = KANTURU_STATE_NONE;
	}

	this->GetStateInfo(state)->Start();

	sWorldMgr->statusChange(WORLD_KANTURU_BOSS, 0);

	switch ( state )
	{
	case KANTURU_STATE_NONE:
		{
			this->SetState_None();
		} break;

	case KANTURU_STATE_BATTLE_STANDBY:
		{
			this->SetState_BattleStandBy();
		} break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		{
			this->SetState_BattleOfMaya();
		} break;

	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		{
			this->SetState_BattleOfNightmare();
		} break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		{
			this->SetState_TowerOfRefinement();
		} break;

	case KANTURU_STATE_END:
		{
			this->SetState_End();
		} break;
	}
}

void KanturuMgr::CheckStateTime()
{
	KanturuStateInfo const* pKanturuStateInfo = this->GetStateInfo(this->GetState());

	if ( !pKanturuStateInfo )
	{
		this->SetStateNext();
		return;
	}

	if ( pKanturuStateInfo->GetCondition() == 1 && pKanturuStateInfo->Passed() )
	{
		this->SetStateNext();
	}
}

void KanturuMgr::UpdateTime()
{
	this->CheckStateTime();
	//this->CheckUserOnKanturuBossMap();

	switch ( this->GetState() )
	{
	case KANTURU_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case KANTURU_STATE_BATTLE_STANDBY:
		{
			this->ProcState_BattleStandBy();
		} break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		{
			this->ProcState_BattleOfMaya();
		} break;

	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		{
			this->ProcState_BattleOfNightmare();
		} break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		{
			this->ProcState_TowerOfRefinement();
		} break;

	case KANTURU_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void KanturuMgr::SetState_None()
{
	this->SetState(KANTURU_STATE_NONE);

	sLog->outInfo(LOG_KANTURU, "SetState_None()");
}

void KanturuMgr::SetState_BattleStandBy()
{
	this->SetState(KANTURU_STATE_BATTLE_STANDBY);
	this->GetKanturuStandBy()->ChangeState(KANTURU_STANBY_START);

	sLog->outInfo(LOG_KANTURU, "SetState_BattleStandBy()");
}

void KanturuMgr::SetState_BattleOfMaya()
{
	this->SetState(KANTURU_STATE_BATTLE_OF_MAYA);
	this->GetKanturuMaya()->ChangeState(KANTURU_MAYA_STANDBY_1);

	sLog->outInfo(LOG_KANTURU, "SetState_BattleOfMaya()");
}

void KanturuMgr::SetState_BattleOfNightmare()
{
	this->SetState(KANTURU_STATE_BATTLE_OF_NIGHTMARE);
	this->GetKanturuNightmare()->ChangeState(KANTURU_NIGHTMARE_IDLE);

	sLog->outInfo(LOG_KANTURU, "SetState_BattleOfNightmare()");
}

void KanturuMgr::SetState_TowerOfRefinement()
{
	this->SetState(KANTURU_STATE_TOWER_OF_REFINEMENT);
	this->GetKanturuTower()->ChangeState(KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION);
	sWorldMgr->statusChange(WORLD_KANTURU_BOSS, 1);

	sLog->outInfo(LOG_KANTURU, "SetState_TowerOfRefinement()");
}

void KanturuMgr::SetState_End()
{
	this->SetState(KANTURU_STATE_END);

	sLog->outInfo(LOG_KANTURU, "SetState_End()");
}

void KanturuMgr::ProcState_None()
{
	this->ChangeState(KANTURU_STATE_BATTLE_STANDBY);
}
	
void KanturuMgr::ProcState_BattleStandBy()
{
	if ( this->GetSubState() == KANTURU_STANBY_END )
	{
		this->GetKanturuStandBy()->ChangeState(KANTURU_STANBY_NONE);
		this->ChangeState(KANTURU_STATE_BATTLE_OF_MAYA);
	}
	else
	{
		this->GetKanturuStandBy()->Update();
	}
}
	
void KanturuMgr::ProcState_BattleOfMaya()
{
	if ( this->GetSubState() == KANTURU_MAYA_ENDCYCLE )
	{
		this->GetKanturuMaya()->ChangeState(KANTURU_MAYA_NONE);

		if ( this->GetKanturuMaya()->IsSuccess() )
		{
			this->ChangeState(KANTURU_STATE_BATTLE_OF_NIGHTMARE);
		}
		else
		{
			this->ChangeState(KANTURU_STATE_END);
		}
	}
	else
	{
		this->GetKanturuMaya()->Update();
	}
}
	
void KanturuMgr::ProcState_BattleOfNightmare()
{
	if ( this->GetSubState() == KANTURU_NIGHTMARE_ENDCYCLE )
	{
		this->GetKanturuNightmare()->ChangeState(KANTURU_NIGHTMARE_NONE);

		if ( this->GetKanturuNightmare()->IsSuccess() )
		{
			this->ChangeState(KANTURU_STATE_TOWER_OF_REFINEMENT);
		}
		else
		{
			this->ChangeState(KANTURU_STATE_END);
		}
	}
	else
	{
		this->GetKanturuNightmare()->Update();
	}
}

void KanturuMgr::ProcState_TowerOfRefinement()
{
	if ( this->GetSubState() == KANTURU_TOWER_OF_REFINEMENT_END )
	{
		this->GetKanturuTower()->ChangeState(KANTURU_TOWER_OF_REFINEMENT_NONE);
		this->ChangeState(KANTURU_STATE_END);
	}
	else
	{
		this->GetKanturuTower()->Update();
	}
}

void KanturuMgr::ProcState_End()
{
	this->ChangeState(KANTURU_STATE_NONE);
}

void KanturuMgr::InitUser(bool move)
{
	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		pPlayer->SetKanturuAllowed(false);

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( move )
			pPlayer->MoveToGate(KANTURU_GATE_OUT);
	}

	this->SetNightmareAlive(false);
}

void KanturuMgr::UpdateUser()
{
	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->IsKanturuAllowed() && pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
		{
			pPlayer->SetKanturuAllowed(false);
		}
		
		if ( !pPlayer->IsKanturuAllowed() && pPlayer->GetWorldId() == WORLD_KANTURU_BOSS )
		{
			pPlayer->MoveToGate(KANTURU_GATE_OUT);
		}
	}
}

void KanturuMgr::MoveToGate(uint16 gate)
{
	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( !pPlayer->IsKanturuAllowed() )
			continue;

		pPlayer->MoveToGate(gate);
	}
}

int32 KanturuMgr::GetUserCount() const
{
	int32 count = 0;

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		if ( !pPlayer->IsKanturuAllowed() )
			continue;

		++count;
	}

	return count;
}

void KanturuMgr::AddMonster()
{
	this->SetMonsterCount(0);

	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_KANTURU )
		{
			continue;
		}

		if ( (*it)->kanturu.state != this->GetState() || (*it)->kanturu.sub_state != this->GetSubState() )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->AddAdditionalDataInt(0, this->GetState());
			pMonster->AddAdditionalDataInt(1, this->GetSubState());
			pMonster->AddToWorld();

			this->SetNightmareAlive(true);

			sLog->outInfo(LOG_KANTURU, "Added Monster [%u][%u]", pMonster->GetEntry(), pMonster->GetClass());

			if ( pMonster->GetClass() == 364 )
			{
				continue;
			}

			this->IncreaseMonsterCount(1);
		}
	}
}

void KanturuMgr::AddSummonedMonster()
{
	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_KANTURU )
		{
			continue;
		}

		if ( (*it)->kanturu.state != this->GetState() || (*it)->kanturu.sub_state != this->GetSubState() )
		{
			continue;
		}

		if ( (*it)->GetID() == 364 )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->AddAdditionalDataInt(0, this->GetState());
			pMonster->AddAdditionalDataInt(1, this->GetSubState());
			pMonster->AddToWorld();

			sLog->outInfo(LOG_KANTURU, "Added Monster [%u][%u]", pMonster->GetEntry(), pMonster->GetClass());

			this->IncreaseMonsterCount(1);
		}
	}
}