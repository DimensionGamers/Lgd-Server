/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "CastleSiege.cpp"
*
*/

CCastleSiege::CCastleSiege()
{
	this->SetState(CASTLE_SIEGE_STATE_IDLE_1);
	this->ResetCrownUser();
	this->SetTaxRateChaos(0);
	this->SetTaxRateStore(0);
	this->SetTributeMoney(0);
	this->SetTaxRateHunt(0);
	this->SetHuntEnabled(false);
	this->SetListLoaded(false);
	this->SetNpcLoaded(false);
	this->SetFirstRun(false);

	for ( uint8 i = 0; i < CASTLE_SIEGE_STATE_MAX; ++i )
		this->GetStateTimeData(i)->Reset();

	for ( uint8 i = 0; i < CASTLE_SIEGE_NPC_UPGRADE_MAX; ++i )
	{
		for ( uint8 h = 0; h < CASTLE_SIEGE_NPC_MAX_DEFENSE_LEVEL; ++h )
			this->m_npc_defense[i][h].Reset();
	}

	for ( uint8 i = 0; i < CASTLE_SIEGE_NPC_UPGRADE_MAX; ++i )
	{
		this->SetNpcBuyPrice(i, 0);
	}

	this->SetCastleOwner(0);
	this->SetMiddleOwner(0);
	this->SetOccupied(false);
	this->command.set(false);
	this->SetUpdateTick(0);
	this->SetUpdateTime(0);

	this->GetSaveNpcTime()->Reset();
	this->GetMiniMapTime()->Reset();
	this->GetLeftTime()->Reset();
	this->GetUpdateSideTime()->Reset();
	this->GetSwitchInfoTime()->Reset();
}

CCastleSiege::~CCastleSiege()
{
	this->ClearData();

	MAP_CLEAR(CastleSiegeGuildDataMap::iterator, this->m_guild_data);
	MAP_CLEAR(CastleSiegeMiniMapDataMap::iterator, this->m_mini_map_data);

	this->m_mini_map_request.clear();
}

void CCastleSiege::LoadData()
{
	if ( !sGameServer->IsCastleSiegeEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Castle Siege Data...");

	this->ClearData();

	ReadScript file("EventData/CastleSiege.txt", "rb");

	if ( !file.open() )
	{
		file.LogOpenError();
		return;
	}

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

					file.GetToken();
					uint8 day = file.GetUInt8();

					file.GetToken();
					uint8 hour = file.GetUInt8();

					file.GetToken();
					uint8 minute = file.GetUInt8();

					if ( state < CASTLE_SIEGE_STATE_MAX )
					{
						this->GetStateTimeData(state)->day.set(day);
						this->GetStateTimeData(state)->hour.set(hour);
						this->GetStateTimeData(state)->minute.set(minute);
						this->GetStateTimeData(state)->SetMSec();
					}
				} break;

			case 1:
				{
					CastleSiegeNpcData * pNpcData = new CastleSiegeNpcData;

					pNpcData->monster.set(file.GetInt32());

					file.GetToken();
					pNpcData->id.set(file.GetUInt8());

					file.GetToken();
					pNpcData->store_in_db.set(file.GetUInt8());

					file.GetToken();
					pNpcData->side.set(file.GetUInt8());

					file.GetToken();
					pNpcData->x.set(file.GetInt16());

					file.GetToken();
					pNpcData->y.set(file.GetInt16());

					file.GetToken();
					pNpcData->direction.set(file.GetUInt8());

					this->m_npc_data.push_back(pNpcData);
				} break;

			case 2:
				{
					uint8 type = file.GetUInt8();

					file.GetToken();
					uint32 price = file.GetUInt32();

					if ( type < CASTLE_SIEGE_NPC_UPGRADE_MAX )
					{
						this->SetNpcBuyPrice(type, price);
					}
				} break;

			case 3:
				{
					uint8 level = file.GetUInt8();

					file.GetToken();
					uint8 jog_count = file.GetUInt8();

					file.GetToken();
					uint32 required_zen = file.GetUInt32();

					file.GetToken();
					int32 defense = file.GetUInt32();

					if ( level < CASTLE_SIEGE_NPC_MAX_DEFENSE_LEVEL )
					{
						this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_GATE][level].jog_count.set(jog_count);
						this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_GATE][level].required_zen.set(required_zen);
						this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_GATE][level].defense.set(defense);
					}
				} break;

			case 4:
				{
					uint8 level = file.GetUInt8();

					file.GetToken();
					uint8 jog_count = file.GetUInt8();

					file.GetToken();
					uint32 required_zen = file.GetUInt32();

					file.GetToken();
					int32 defense = file.GetUInt32();

					if ( level < CASTLE_SIEGE_NPC_MAX_DEFENSE_LEVEL )
					{
						this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_STATUE][level].jog_count.set(jog_count);
						this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_STATUE][level].required_zen.set(required_zen);
						this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_STATUE][level].defense.set(defense);
					}
				} break;

			case 5:
				{
					uint8 level = file.GetUInt8();

					file.GetToken();
					uint8 jog_count = file.GetUInt8();

					file.GetToken();
					uint32 required_zen = file.GetUInt32();

					file.GetToken();
					int32 life = file.TokenNumber;

					if ( level < CASTLE_SIEGE_NPC_MAX_LIFE_LEVEL )
					{
						this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][level].jog_count.set(jog_count);
						this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][level].required_zen.set(required_zen);
						this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][level].life.set(life);
					}
				} break;

			case 6:
				{
					uint8 level = file.GetUInt8();

					file.GetToken();
					uint8 jog_count = file.GetUInt8();

					file.GetToken();
					uint32 required_zen = file.GetUInt32();

					file.GetToken();
					int32 life = file.TokenNumber;

					if ( level < CASTLE_SIEGE_NPC_MAX_LIFE_LEVEL )
					{
						this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][level].jog_count.set(jog_count);
						this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][level].required_zen.set(required_zen);
						this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][level].life.set(life);
					}
				} break;

			case 7:
				{
					uint8 level = file.GetUInt8();

					file.GetToken();
					uint8 jog_count = file.GetUInt8();

					file.GetToken();
					uint32 required_zen = file.GetUInt32();

					if ( level < CASTLE_SIEGE_NPC_MAX_LIFE_LEVEL )
					{
						this->m_npc_regen[level].jog_count.set(jog_count);
						this->m_npc_regen[level].required_zen.set(required_zen);
					}
				} break;
			}
		}
	}

	file.close();

	for ( uint8 i = 0; i < CASTLE_SIEGE_STATE_MAX; ++i )
	{
		if ( this->GetStateTimeData(i)->IsInUse() )
		{
			uint8 next_state = this->GetNextState(i);
			uint32 start_in_second = this->GetStateTimeData(i)->GetStartSecond();
			uint32 next_start_in_second = this->GetStateTimeData(next_state)->GetStartSecond();

			///- Si el actual es mayor al siguiente, quiere decir que el ciclo se reinicia
			if ( start_in_second > next_start_in_second )
			{
				next_start_in_second += (7 * DAY); ///- Le asigno el mismo valor
			}

			this->GetStateTimeData(i)->SetDuration(next_start_in_second - start_in_second);
			this->GetStateTimeData(i)->SetEndSecond(next_start_in_second);
		}

		sLog->outInfo("root", "Castle Siege State %d duration %d (%u - %u)", i, this->GetStateTimeData(i)->GetDuration(), this->GetStateTimeData(i)->GetStartSecond(), this->GetStateTimeData(i)->GetEndSecond());
	}
}

void CCastleSiege::ClearData()
{
	LIST_CLEAR(CastleSiegeNpcDataList::iterator, this->m_npc_data);
}

uint8 CCastleSiege::GetNextState(uint8 state)
{
	return state == CASTLE_SIEGE_STATE_IDLE_1 ? CASTLE_SIEGE_STATE_REGISTER_GUILD :
		   state == CASTLE_SIEGE_STATE_REGISTER_GUILD ? CASTLE_SIEGE_STATE_IDLE_2 :
		   state == CASTLE_SIEGE_STATE_IDLE_2 ? CASTLE_SIEGE_STATE_REGISTER_MARK :
		   state == CASTLE_SIEGE_STATE_REGISTER_MARK ? CASTLE_SIEGE_STATE_IDLE_3 :
		   state == CASTLE_SIEGE_STATE_IDLE_3 ? CASTLE_SIEGE_STATE_NOTIFY :
		   state == CASTLE_SIEGE_STATE_NOTIFY ? CASTLE_SIEGE_STATE_READY :
		   state == CASTLE_SIEGE_STATE_READY ? CASTLE_SIEGE_STATE_START :
		   state == CASTLE_SIEGE_STATE_START ? CASTLE_SIEGE_STATE_END :
		   state == CASTLE_SIEGE_STATE_END ? CASTLE_SIEGE_STATE_ENDCYCLE :
		   CASTLE_SIEGE_STATE_IDLE_1;
}

void CCastleSiege::CalculateCurrentState()
{
	Custom::SystemTimer m_timer = Custom::SystemTimer();

	uint8 current_day = m_timer.GetDayOfWeekMod();

	uint32 current_second = (m_timer.GetDayOfWeekMod() * DAY) + (m_timer.GetHour() * HOUR) + (m_timer.GetMinute() * MINUTE);
	uint8 current_state = CASTLE_SIEGE_STATE_IDLE_1;

	for ( int32 i = 0; i < CASTLE_SIEGE_STATE_MAX; ++i )
	{
		uint8 next_state = this->GetNextState(i);

		uint32 start_second = this->GetStateTimeData(i)->GetStartSecond();
		uint32 next_start_second = this->GetStateTimeData(next_state)->GetStartSecond();

		if ( next_start_second < start_second )
		{
			if ( (current_second >= start_second) || (current_second <= next_start_second) )
			{
				current_state = i;
				break;
			}
		}
		else
		{
			if ( (current_second >= start_second) && (current_second <= next_start_second) )
			{
				current_state = i;
				break;
			}
		}
	}

	this->ChangeState(current_state);
}

Monster* CCastleSiege::GetMonster(uint16 npc, uint8 id) const
{
	HASH_SUMMON(i)
	{
		Object* pObject = ObjectContainer[i];

		if ( !pObject )
		{
			continue;
		}

		if ( pObject->GetEventId() != EVENT_CASTLE_SIEGE )
		{
			continue;
		}

		if ( pObject->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		{
			continue;
		}

		Monster* pMonster = pObject->ToCreature();

		if ( !pMonster )
		{
			continue;
		}

		if ( pMonster->GetClass() != npc )
		{
			continue;
		}

		if ( pMonster->GetEventGround() != id )
		{
			continue;
		}

		return pMonster;
	}

	return nullptr;
}

void CCastleSiege::LoadNPC(uint8 * Packet)
{
	sLog->outInfo("root", "Loading Castle Siege NPC From ServerLink");

	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( !(*it)->store_in_db ) ///- Solamente debo dejar como inactivos los NPC que son de Base de Datos
		{
			continue;
		}

		(*it)->SetActive(false);

		sLog->outInfo("root", "Loading Castle Siege NPC [Stage 1] Disable Npc [%u/%u]", (*it)->monster.get(), (*it)->id.get());
	}

	///- Tengo que borrar los mobs así los agrego nuevamente
	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( !(*it)->store_in_db ) ///- Solamente debo borrar npc que se guarden en DB
		{
			continue;
		}

		Monster* pMonster = this->GetMonster((*it)->monster.get(), (*it)->id.get());

		if ( !pMonster )
		{
			continue;
		}

		sLog->outInfo("root", "Loading Castle Siege NPC [Stage 2] Remove Npc [%u/%u] [%u/%s]", (*it)->monster.get(), (*it)->id.get(), pMonster->GetEntry(), pMonster->GetName());

		pMonster->Remove();
		pMonster->SetEventId(EVENT_NONE);
		pMonster->SetEventGround(0);
	}
	
	POINTER_PCT(SL_CASTLE_SIEGE_LOAD_NPC_HEAD, head, Packet, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_LOAD_NPC_BODY, body, Packet, sizeof(SL_CASTLE_SIEGE_LOAD_NPC_HEAD));

	for ( int32 i = 0; i < head->count; ++i )
	{
		CastleSiegeNpcData * pNpcData = this->GetNpcData(body[i].npc, body[i].id);

		if ( !pNpcData )
		{
			continue;
		}

		pNpcData->defense_level.set(body[i].defense_level);
		pNpcData->regen_level.set(body[i].regen_level);
		pNpcData->life_level.set(body[i].life_level);
		pNpcData->current_life.set(body[i].life);
		pNpcData->SetActive(true);

		sLog->outInfo("root", "Loading Castle Siege NPC [Stage 3] Enable Npc [%u/%u]", pNpcData->monster.get(), pNpcData->id.get());
	}

	this->SetNpcLoaded(true);
	this->CreateDBNpc();
}

void CCastleSiege::VerifyNPCExist() ///- Esta función es para actualizar los datos de la DB en base al status actual de los monsters
{
	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( !(*it)->store_in_db ) ///- Solamente debo dejar como inactivos los NPC que son de Base de Datos
			continue;

		Monster* pMonster = this->GetMonster((*it)->monster.get(), (*it)->id.get());

		if ( !pMonster ) ///- Si el monster no existe entonces lo dejo inactivo
		{
			(*it)->SetActive(false);
			continue;
		}

		if ( !pMonster->IsLive() || !pMonster->IsPlaying() ) ///- Si el monster está muerto o fuera de juego lo dejo inactivo
		{
			(*it)->SetActive(false);
			continue;
		}

		(*it)->SetActive(true);
		(*it)->current_life.set(pMonster->PowerGet(POWER_LIFE));
	}
}

CastleSiegeNpcData * CCastleSiege::GetNpcData(uint16 npc, uint8 id)
{
	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( (*it)->monster == npc && (*it)->id == id )
			return (*it);
	}

	return nullptr;
}

void CCastleSiege::SaveDBNpc()
{
	if ( !this->IsNpcLoaded() )
		return;

	this->GetSaveNpcTime()->Start();

	uint8 buffer[15000];
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_NPC_HEAD, head, buffer, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_NPC_BODY, body, buffer, sizeof(SL_CASTLE_SIEGE_SAVE_NPC_HEAD));
	head->count = 0;

	///- Solamente tengo que enviar la información de los NPC que están vivos
	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( !(*it)->store_in_db || !(*it)->IsActive() )
			continue;

		body[head->count].npc = (*it)->monster.get();
		body[head->count].id = (*it)->id.get();
		body[head->count].defense_level = (*it)->defense_level.get();
		body[head->count].regen_level = (*it)->regen_level.get();
		body[head->count].life_level = (*it)->life_level.get();
		body[head->count].life = (*it)->current_life.get();

		++head->count;
	}

	head->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_NPC, sizeof(SL_CASTLE_SIEGE_SAVE_NPC_HEAD) + (sizeof(SL_CASTLE_SIEGE_SAVE_NPC_BODY) * head->count));

	sServerLink->SendPacket(buffer, head->h.get_size());
}

void CCastleSiege::CreateDBNpc()
{
	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( !(*it)->store_in_db || !(*it)->IsActive() )
		{
			continue;
		}

		this->AddDBNpc(*it);
	}
}

void CCastleSiege::CreateNonDBNpc()
{
	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( (*it)->store_in_db.get() )
		{
			continue;
		}

		this->AddDBNpc(*it);
	}
}

void CCastleSiege::ClearNonDBNpc()
{
	for ( CastleSiegeNpcDataList::iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( (*it)->store_in_db.get() )
			continue;

		Monster* pMonster = this->GetMonster((*it)->monster.get(), (*it)->id.get());

		if ( !pMonster )
			continue;

		pMonster->Remove();
		pMonster->SetEventId(EVENT_NONE);
		pMonster->SetEventGround(0);
	}
}

bool CCastleSiege::AddDBNpc(CastleSiegeNpcData* NpcData)
{
	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_CASTLE_SIEGE )
		{
			continue;
		}

		if ( (*it)->GetID() != NpcData->monster.get() )
		{
			continue;
		}

		if ( (*it)->castle_siege.id != NpcData->id.get() )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
			pMonster->SetRespawn(1000);
			pMonster->SetCastleSiegeJoinSide(NpcData->side.get());
			pMonster->SetEventGround(NpcData->id.get());

			switch ( pMonster->GetClass() )
			{
			case 283:
			case 277:
			case 219:
				{
					pMonster->AddAdditionalDataInt(0, NpcData->id.get());
				} break;
	
			case 222:
				{
					pMonster->AddAdditionalDataInt(0, CASTLE_SIEGE_MACHINE_DEFENSE);
				} break;

			case 221:
				{
					pMonster->AddAdditionalDataInt(0, CASTLE_SIEGE_MACHINE_ATTACK);
				} break;
			}

			sLog->outInfo("root", "Loading Castle Siege NPC [Stage 4] Creating Npc [%u/%u] [%u/%s]", NpcData->monster.get(), NpcData->id.get(), pMonster->GetEntry(), pMonster->GetName());

			pMonster->AddToWorld();
			return true;
		}
	}

	return false;
}

void CCastleSiege::BlockReleaseCastleGate(int16 X, int16 Y, bool block)
{
	World* pWorld = sWorldMgr->GetWorld(WORLD_CASTLE_SIEGE);

	if ( !pWorld )
	{
		return;
	}

	int16 data[4] = { X - 2, Y, X + 3, Y + 1 };

	pWorld->ApplyAttribute(data, WORLD_ATTRIBUTE_LOCK_3, block);
}

void CCastleSiege::CloseCastleGate()
{
	HASH_SUMMON(i) ///- Recorro todos los monsters activos
	{
		Object* pObject = ObjectContainer[i];
		
		if ( !pObject ) ///- Reviso que sea un objeto válido
			continue;

		Monster* pMonster = pObject->ToCreature();
		
		if ( !pMonster ) ///- Reviso que sea un monster válido
			continue;
		
		if ( !pMonster->IsPlaying() ) ///- Reviso que esté activo
			continue;

		if ( !pMonster->AIGroup(CS_GATE_AI) ) ///- Reviso que sea una puerta de CS
			continue;
		
		this->BlockReleaseCastleGate(pMonster->GetX(), pMonster->GetY(), true);
		pMonster->RemoveBuff(BUFF_CASTLE_SIEGE_GATE_STATUS);
	}
}

CrownBasicAI* CCastleSiege::GetCrown() const
{
	HASH_SUMMON(i) ///- Recorro todos los monsters activos
	{
		Object* pObject = ObjectContainer[i];
		
		if ( !pObject ) ///- Reviso que sea un objeto válido
			continue;

		Monster* pMonster = pObject->ToCreature();
		
		if ( !pMonster ) ///- Reviso que sea un monster válido
			continue;
		
		if ( !pMonster->IsPlaying() ) ///- Reviso que esté activo
			continue;

		MonsterAI* pAI = pMonster->AIGroup(CS_CROWN_AI);

		if ( !pAI ) ///- Reviso que sea la corona
			continue;

		return AI_TO(CrownBasicAI, pAI);
	}

	return nullptr;
}

void CCastleSiege::SetCrownState(uint8 state)
{
	if ( CrownBasicAI* pCrown = this->GetCrown() )
	{
		pCrown->SetState(state);
	}
}

Player * CCastleSiege::GetCrownUser()
{
	if ( CrownBasicAI* pCrown = this->GetCrown() )
	{
		return pCrown->GetPlayer();
	}

	return nullptr;
}

void CCastleSiege::SetCrownAvailable(bool value)
{
	if ( CrownBasicAI* pCrown = this->GetCrown() )
	{
		pCrown->SetAvailable(value);
	}
}

bool CCastleSiege::IsCrownAvailable() const
{
	if ( CrownBasicAI* pCrown = this->GetCrown() )
	{
		return pCrown->IsAvailable();
	}

	return false;
}

Player * CCastleSiege::GetCrownSwitchUser(uint8 Switch)
{
	if ( CrownSwitchBasicAI* pSwitchAI = this->GetCrownSwitch(Switch) )
	{
		return pSwitchAI->GetPlayer();
	}

	return nullptr;
}

CrownSwitchBasicAI* CCastleSiege::GetCrownSwitch(uint8 Switch) const
{
	HASH_SUMMON(i) ///- Recorro todos los monsters activos
	{
		Object* pObject = ObjectContainer[i];
		
		if ( !pObject ) ///- Reviso que sea un objeto válido
			continue;

		Monster* pMonster = pObject->ToCreature();
		
		if ( !pMonster ) ///- Reviso que sea un monster válido
			continue;
		
		if ( !pMonster->IsPlaying() ) ///- Reviso que esté activo
			continue;

		MonsterAI* pAI = pMonster->AIGroup(CS_CROWN_SWITCH_AI_1 + std::string(" ") + CS_CROWN_SWITCH_AI_2);

		if ( !pAI ) ///- Reviso que sea un switch
			continue;

		CrownSwitchBasicAI* pSwitchAI = AI_TO(CrownSwitchBasicAI, pAI);

		if ( pSwitchAI->GetID() != Switch ) ///- Reviso que sea el switch indicado
			continue;

		return pSwitchAI;
	}

	return nullptr;
}

bool CCastleSiege::IsCrownUser(uint8 Switch)
{
	Player* pPlayer = this->GetCrownSwitchUser(Switch);

	if ( !pPlayer )
		return false;

	if ( !pPlayer->IsPlaying() )
		return false;

	return true;
}

void CCastleSiege::ResetCrownUser()
{
	if ( CrownBasicAI* pCrown = this->GetCrown() )
	{
		if (Player* pPlayer = pCrown->GetPlayer())
		{
			sEventMgr->CastleSiegeTime(pPlayer, 1);
		}

		pCrown->ResetPlayer();
	}
}

void CCastleSiege::ResetCrownSwitchUser(uint8 Switch)
{
	if ( CrownSwitchBasicAI* pSwitchAI = this->GetCrownSwitch(Switch) )
	{
		if (Player* pPlayer = pSwitchAI->GetPlayer())
		{
			sEventMgr->CastleSiegeTime(pPlayer, 0);
		}

		pSwitchAI->ResetPlayer();
	}
}

void CCastleSiege::LockCrown()
{
	if ( this->IsCrownAvailable() )
	{
		this->SetCrownAvailable(false);
		this->SetCrownState(1);
	}
}
	
void CCastleSiege::UnlockCrown()
{
	if ( !this->IsCrownAvailable() )
	{
		this->SetCrownAvailable(true);
		this->SetCrownState(0);
	}
}

void CCastleSiege::RespawnPlayers(bool defense)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_CASTLE_SIEGE && pPlayer->GetWorldId() != WORLD_LAND_OF_TRIALS )
			continue;

		uint8 state = pPlayer->GetCastleSiegeJoinSide();
		int16 x = pPlayer->GetX();
		int16 y = pPlayer->GetY();

		if ( state == CASTLE_SIEGE_JOIN_SIDE_DEFENSE && !defense )
			continue;

		switch ( state )
		{
		case CASTLE_SIEGE_JOIN_SIDE_NONE:
			{
				this->GetRespawnCoord(x, y, 0);
			} break;

		case CASTLE_SIEGE_JOIN_SIDE_DEFENSE:
			{
				this->GetRespawnCoord(x, y, 1);
			} break;

		case CASTLE_SIEGE_JOIN_SIDE_ATTACK_1:
		case CASTLE_SIEGE_JOIN_SIDE_ATTACK_2:
		case CASTLE_SIEGE_JOIN_SIDE_ATTACK_3:
			{
				this->GetRespawnCoord(x, y, 2);
			} break;
		}

		if ( x != pPlayer->GetX() || y != pPlayer->GetY() || pPlayer->GetWorldId() == WORLD_LAND_OF_TRIALS )
		{
			pPlayer->TeleportToLocation(WORLD_CASTLE_SIEGE, x, y, pPlayer->GetDirection(), pPlayer->GetInstance());
		}
	}
}

void CCastleSiege::GetRespawnCoord(int16 & x, int16 & y, uint8 type)
{
	World* pWorld = sWorldMgr->GetWorld(WORLD_CASTLE_SIEGE);

	if ( !pWorld )
	{
		return;
	}

	int32 count = 100;
	int16 tx = x;
	int16 ty = y;

	while ( count-- > 0 )
	{
		tx = RANDOM(g_CastleSiegeRespawnArea[type].x2 - g_CastleSiegeRespawnArea[type].x1) + g_CastleSiegeRespawnArea[type].x1;
		ty = RANDOM(g_CastleSiegeRespawnArea[type].y2 - g_CastleSiegeRespawnArea[type].y1) + g_CastleSiegeRespawnArea[type].y1;

		WorldGrid const& grid = pWorld->GetGrid(tx, ty);

		if ( !grid.IsLocked_1() )
		{
			x = tx;
			y = ty;
			break;
		}
	}
}

void CCastleSiege::UpdateTime()
{
	if ( !sGameServer->IsCastleSiegeEnabled() )
		return;

	if ( !this->IsFirstRun() )
	{
		this->SetFirstRun(true);
		this->CalculateCurrentState();
	}

	switch ( this->GetState() )
	{
	case CASTLE_SIEGE_STATE_IDLE_1:
		{
			this->ProcState_Idle_1();
		} break;

	case CASTLE_SIEGE_STATE_REGISTER_GUILD:
		{
			this->ProcState_RegisterGuild();
		} break;

	case CASTLE_SIEGE_STATE_IDLE_2:
		{
			this->ProcState_Idle_2();
		} break;

	case CASTLE_SIEGE_STATE_REGISTER_MARK:
		{
			this->ProcState_RegisterMark();
		} break;

	case CASTLE_SIEGE_STATE_IDLE_3:
		{
			this->ProcState_Idle_3();
		} break;

	case CASTLE_SIEGE_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case CASTLE_SIEGE_STATE_READY:
		{
			this->ProcState_Ready();
		} break;

	case CASTLE_SIEGE_STATE_START:
		{
			this->ProcState_Start();
		} break;

	case CASTLE_SIEGE_STATE_END:
		{
			this->ProcState_End();
		} break;

	case CASTLE_SIEGE_STATE_ENDCYCLE:
		{
			this->ProcState_EndCycle();
		} break;
	}

	if ( this->IsNpcLoaded() ) ///- Solamente hago un guardado periódico si me llegó la lista de NPC
	{
		if ( this->GetSaveNpcTime()->Elapsed(120 * IN_MILLISECONDS) )
		{
			if ( this->GetState() != CASTLE_SIEGE_STATE_START )
			{
				this->VerifyNPCExist(); ///- En este save, verifico que existan los NPC
				this->SaveDBNpc();
			}
		}
	}
}

bool CCastleSiege::CastleOwnerMember(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return false;
	}

	if (!pPlayer->IsPlaying())
	{
		return false;
	}

	if (!this->GetCastleOwner())
	{
		return false;
	}

	if (pPlayer->GetGuildID() != this->GetCastleOwner())
	{
		return false;
	}

	return true;
}
	
bool CCastleSiege::CastleOwnerUnionMember(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return false;
	}

	if (!pPlayer->IsPlaying())
	{
		return false;
	}

	if (!this->GetCastleOwner())
	{
		return false;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if (!pGuild)
	{
		return false;
	}

	if (pGuild->GetAlliance() != this->GetCastleOwner())
	{
		return false;
	}

	return true;
}

uint8 CCastleSiege::GetTaxRateChaos(Player* pPlayer) const
{
	if ( !this->IsOccupied() )
	{
		return 0;
	}

	uint8 rate = this->GetTaxRateChaos();

	if (pPlayer && (this->CastleOwnerMember(pPlayer) || this->CastleOwnerUnionMember(pPlayer)))
	{
		rate = 0;
	}

	return rate;
}

void CCastleSiege::SendTaxRate(Player* pPlayer, uint8 type)
{
	CASTLE_SIEGE_TAX_INFO pMsg(type, this->GetTaxRateChaos(pPlayer));
	pPlayer->SEND_PCT(pMsg);
}

int32 CCastleSiege::GetTaxRateStore(Player* pPlayer) const
{
	if ( !this->IsOccupied() )
	{
		return 0;
	}

	int32 rate = this->GetTaxRateStore();

	if (pPlayer && (this->CastleOwnerMember(pPlayer) || this->CastleOwnerUnionMember(pPlayer)))
	{
		rate = 0;
	}

	return rate;
}

int32 CCastleSiege::GetTaxRateHunt(Player* pPlayer) const
{
	if ( !this->IsOccupied() )
	{
		return 0;
	}

	int32 rate = this->GetTaxRateHunt();

	if (pPlayer && (this->CastleOwnerMember(pPlayer) || this->CastleOwnerUnionMember(pPlayer)))
	{
		rate = 0;
	}

	return rate;
}

void CCastleSiege::AddTributeMoney(int32 money)
{
	if ( !this->IsOccupied() )
	{
		return;
	}

	if ( money <= 0 )
		return;

	if ( (this->GetTributeMoney() + money) > sGameServer->GetMaxCharacterZen() )
		return;

	this->UpdateDBMoney(money, 0);
}

void CCastleSiege::DeleteGuildRegister()
{
	sServerLink->CastleSiegeClearGuild();

	for ( int32 i = 0; i < CASTLE_SIEGE_JOIN_SIDE_MAX; ++i )
		this->GetBasicGuildData(i)->Reset();

	MAP_CLEAR(CastleSiegeGuildDataMap::iterator, this->m_guild_data);
}

bool CastleSiegeGuildCompare(CastleSiegeGuildData const& A, CastleSiegeGuildData const& B)
{
	return A.total_score >= B.total_score;
}

void CCastleSiege::LoadGuildRegister()
{
	if ( !sGameServer->IsCastleSiegeEnabled() )
		return;

	if ( this->IsListLoaded() )
		return;

	this->SetListLoaded(true);

	sServerLink->CastleSiegeLoadRegisteredGuildRequest();
}

void CCastleSiege::LoadGuildRegisterResult(uint8 * packet)
{
	CastleSiegeGuildDataList guild_list;

	{
		POINTER_PCT(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD, head, packet, 0);
		POINTER_PCT(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_BODY, body, packet, sizeof(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD));

		for ( int32 i = 0; i < head->count; ++i )
		{
			CastleSiegeGuildData add_guild;
			add_guild.guid.set(body[i].guid);
			add_guild.marks.set(body[i].marks);
			add_guild.register_id.set(body[i].register_id);
			add_guild.level.set(body[i].level);
			add_guild.members.set(body[i].members);
			add_guild.score.set(body[i].score);
			add_guild.total_score.set(body[i].total_score);

			guild_list.push_back(add_guild);
		}
	}
	
	if ( guild_list.empty() )
		return;

	std::sort(guild_list.begin(), guild_list.end(), CastleSiegeGuildCompare);

	int32 guild_count = guild_list.size();

	if ( guild_count > 3 )
		guild_count = 3;

	for ( int32 i = 0; i < CASTLE_SIEGE_JOIN_SIDE_MAX; ++i )
		this->GetBasicGuildData(i)->Reset();

	MAP_CLEAR(CastleSiegeGuildDataMap::iterator, this->m_guild_data);

	for ( int32 i = 0; i < guild_count; ++i )
	{
		this->GetBasicGuildData(CASTLE_SIEGE_JOIN_SIDE_ATTACK_1 + i)->SetGuild(guild_list[i].guid.get());
		this->GetBasicGuildData(CASTLE_SIEGE_JOIN_SIDE_ATTACK_1 + i)->SetSide(CASTLE_SIEGE_JOIN_SIDE_ATTACK_1 + i);
		this->GetBasicGuildData(CASTLE_SIEGE_JOIN_SIDE_ATTACK_1 + i)->SetScore(guild_list[i].score.get());
	}

	if ( this->IsOccupied() && this->GetCastleOwner() )
	{
		this->GetBasicGuildData(CASTLE_SIEGE_JOIN_SIDE_DEFENSE)->SetGuild(this->GetCastleOwner());
		this->GetBasicGuildData(CASTLE_SIEGE_JOIN_SIDE_DEFENSE)->SetSide(CASTLE_SIEGE_JOIN_SIDE_DEFENSE);
		this->GetBasicGuildData(CASTLE_SIEGE_JOIN_SIDE_DEFENSE)->SetScore(0);
	}

	for ( int32 i = 0; i < CASTLE_SIEGE_JOIN_SIDE_MAX; ++i )
	{
		if ( !this->GetBasicGuildData(i)->GetGuild() )
			continue;

		sGuildMgr->AddCastleSiegeGuildList(this->GetBasicGuildData(i)->GetGuild(), this->GetBasicGuildData(i)->GetScore(), this->GetBasicGuildData(i)->GetSide());
	}

	{
		uint8 buffer[15000];
		POINTER_PCT(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_HEAD, head, buffer, 0);
		POINTER_PCT(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_BODY, body, buffer, sizeof(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_HEAD));
		head->count = 0;

		for ( CastleSiegeGuildDataMap::iterator it = this->m_guild_data.begin(); it != this->m_guild_data.end(); ++it )
		{
			body[head->count].guild = it->first;
			body[head->count].side = it->second->GetSide();
			body[head->count].score = it->second->GetScore();

			++head->count;
		}

		head->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_INSERT_FINAL_GUILD, sizeof(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_HEAD) + (sizeof(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_BODY) * head->count));

		sServerLink->SendPacket(buffer, head->h.get_size());
	}

	this->SetPlayerJoinSide(false);
}

void CCastleSiege::AddGuildToFinalList(uint32 guild, int32 score, uint8 side)
{
	CastleSiegeGuildFinal * add_guild = new CastleSiegeGuildFinal;
	add_guild->SetGuild(guild);
	add_guild->SetScore(score);
	add_guild->SetSide(side);

	this->m_guild_data[guild] = add_guild;
}

void CCastleSiege::ResetPlayerJoinSide()
{
	Player* pPlayer = nullptr;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		this->ResetPlayerJoinSide(pPlayer);
	}
}
	
void CCastleSiege::ResetPlayerJoinSide(Player* pPlayer)
{
	pPlayer->SetCastleSiegeJoinSide(0);
	this->SendJoinSide(pPlayer);
}

void CCastleSiege::SetPlayerJoinSide(bool delete_life_stone)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr )
	{
		Player* pPlayer = itr->second;

		if ( !pPlayer )
			continue;

		if ( delete_life_stone )
		{
			pPlayer->SetAccumulatedTime(0);
			pPlayer->CancelDelayedTeleport();
		}

		CastleSiegeGuildDataMap::const_iterator it = this->m_guild_data.find(pPlayer->GetGuildID());

		if ( it != this->m_guild_data.end() )
		{
			pPlayer->SetCastleSiegeJoinSide(it->second->GetSide());
		}
		else
		{
			pPlayer->SetCastleSiegeJoinSide(0);
		}

		this->SendJoinSide(pPlayer);
	}

	if ( delete_life_stone )
	{
		GuildMap const& guild = sGuildMgr->GetGuildMap();

		for ( GuildMap::const_iterator it = guild.begin(); it != guild.end(); ++it )
		{
			if ( !it->second )
				continue;

			it->second->LifeStoneKill();
		}
	}
}

void CCastleSiege::CheckMiddleWinner()
{
	Player* pCrownUser = this->GetCrownUser();
	Player* pSwitchUser01 = this->GetCrownSwitchUser(0);
	Player* pSwitchUser02 = this->GetCrownSwitchUser(1);

	if ( !pCrownUser || !pCrownUser->IsPlaying() || !pCrownUser->GetGuildID() || 
		 !pSwitchUser01 || !pSwitchUser01->IsPlaying() || !pSwitchUser01->GetGuildID() || 
		 !pSwitchUser02 || !pSwitchUser02->IsPlaying() || !pSwitchUser02->GetGuildID() )
		return;

	if ( !pCrownUser->isCastleSiegeAttacker() ||
		 !pSwitchUser01->isCastleSiegeAttacker() ||
		 !pSwitchUser02->isCastleSiegeAttacker() )
		return;

	if ( pCrownUser->GetCastleSiegeJoinSide() != pSwitchUser01->GetCastleSiegeJoinSide() ||
		 pCrownUser->GetCastleSiegeJoinSide() != pSwitchUser02->GetCastleSiegeJoinSide() )
		return;

	if ( pCrownUser->GetAccumulatedTime() < sGameServer->GetCastleSiegeCrownTime() )
		return;
			
	Guild* pGuild = pCrownUser->GuildGet();

	if ( !pGuild )
		return;

	sLog->outInfo(LOG_CASTLE_SIEGE, "CheckMiddleWinner() Castle Winner has been changed [%d - %s]", pGuild->GetID(), pGuild->GetName());

	this->SendCrownAccessState(pCrownUser, CASTLE_SIEGE_CROWN_ACCESS_STATE_SUCCESS);
	this->SetCrownState(2);
	this->SetMiddleOwner(pGuild->GetID());
	this->ChangeWinnerGuild(pCrownUser->GetCastleSiegeJoinSide());
	this->SendNotifyProgress(1, pGuild->GetName());
	this->ResetCrownUser();
	this->ResetCrownSwitchUser(0);
	this->ResetCrownSwitchUser(1);
	this->SetCrownAvailable(false);
	this->RespawnPlayers(false);
	this->SendLeftTime();
}

void CCastleSiege::ChangeWinnerGuild(uint8 side)
{
	for ( CastleSiegeGuildDataMap::iterator it = this->m_guild_data.begin(); it != this->m_guild_data.end(); ++it )
	{
		if ( it->second->GetSide() == side )
		{
			it->second->SetSide(CASTLE_SIEGE_JOIN_SIDE_DEFENSE);
		}
		else if ( it->second->GetSide() == CASTLE_SIEGE_JOIN_SIDE_DEFENSE )
		{
			it->second->SetSide(side);
		}
	}

	this->SetPlayerJoinSide(true);
}

bool CCastleSiege::ExistAttackerGuild()
{
	for ( CastleSiegeGuildDataMap::iterator it = this->m_guild_data.begin(); it != this->m_guild_data.end(); ++it )
	{
		if ( it->second->GetSide() == CASTLE_SIEGE_JOIN_SIDE_ATTACK_1 ||
			 it->second->GetSide() == CASTLE_SIEGE_JOIN_SIDE_ATTACK_2 ||
			 it->second->GetSide() == CASTLE_SIEGE_JOIN_SIDE_ATTACK_3 )
			 return true;
	}

	return false;
}

bool CCastleSiege::CheckResult()
{
	bool result = false;
	Guild const* pGuild = sGuildMgr->GetGuild(this->GetCastleOwner());
	Guild const* pMiddle = sGuildMgr->GetGuild(this->GetMiddleOwner());

	if ( this->IsOccupied() && pGuild )
	{
		if ( !pMiddle || pMiddle == pGuild )
		{
			sServerLink->NoticeSend(NOTICE_GLOBAL, "%s guild has succeeded in defending the castle.", pGuild->GetName());
		}
		else
		{
			this->SetCastleOwner(this->GetMiddleOwner());
			sServerLink->NoticeSend(NOTICE_GLOBAL, "%s guild became a new castle lord.", pMiddle->GetName());
			result = true;
		}

		this->SetOccupied(true);
	}
	else
	{
		if ( !pMiddle )
		{
			this->SetOccupied(false);
			sServerLink->NoticeSend(NOTICE_GLOBAL, "The castle lord has not been assigned.");
		}
		else
		{
			this->SetOccupied(true);
			this->SetCastleOwner(this->GetMiddleOwner());
			sServerLink->NoticeSend(NOTICE_GLOBAL, "%s guild became a new castle lord.", pMiddle->GetName());
			result = true;
		}
	}

	this->InsertRanking();

	pGuild = sGuildMgr->GetGuild(this->GetCastleOwner());
	
	sLog->outInfo(LOG_CASTLE_SIEGE, "Castle Final Winner Guild : %s", pGuild ? pGuild->GetName() : "");
	//this->m_btIsSiegeEnded = 1;
	
	return result;
}

void CCastleSiege::SaveDBTaxRate()
{
	SL_CASTLE_SIEGE_SAVE_TAX_RATE pMsg;
	pMsg.tax_hunt = this->GetTaxRateHunt();
	pMsg.tax_chaos = this->GetTaxRateChaos();
	pMsg.tax_store = this->GetTaxRateStore();
	pMsg.hunt_allowed = this->IsHuntEnabled();

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::UpdateDBMoney(int32 money, uint8 type, Player* pPlayer)
{
	SL_CASTLE_SIEGE_SAVE_MONEY pMsg(money, type);
	pMsg.h.server = sGameServer->GetServerCode();
	if ( pPlayer )
	{
		pMsg.entry = pPlayer->GetEntry();
		pMsg.ms_time = pPlayer->GetMSTime();
		pMsg.account_id = pPlayer->GetAccountData()->GetGUID();
		pMsg.character_id = pPlayer->GetGUID();
	}

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::ResetTaxRate()
{
	this->SetTaxRate(0, 0, 0, false);
	this->UpdateDBMoney(0, 1);
	this->SaveDBTaxRate();
}

void CCastleSiege::SaveDBCastleOwner()
{
	SL_CASTLE_SIEGE_SAVE_OWNER_STATUS pMsg;
	pMsg.owner = this->GetCastleOwner();
	pMsg.status = this->IsOccupied();

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

bool CCastleSiege::GuardianStatueExist()
{
	HASH_SUMMON(i) ///- Recorro todos los monsters activos
	{
		Object* pObject = ObjectContainer[i];
		
		if ( !pObject ) ///- Reviso que sea un objeto válido
			continue;

		Monster* pMonster = pObject->ToCreature();
		
		if ( !pMonster ) ///- Reviso que sea un monster válido
			continue;
		
		if ( !pMonster->IsPlaying() || !pMonster->IsLive() ) ///- Reviso que esté activo
			continue;

		if ( !pMonster->IsAIGroup(CS_GUARDIAN_STATUE_AI) ) ///- Reviso que sea estatua
			continue;

		return true;
	}

	return false;
}

void CCastleSiege::ChangeState(uint8 state, bool command, int32 duration)
{
	this->command.set(command);

	if( state >= CASTLE_SIEGE_STATE_MAX )
	{
		this->command.set(false);
		return;
	}

	Custom::SystemTimer m_timer = Custom::SystemTimer();

	uint32 current_second = (m_timer.GetDayOfWeekMod() * DAY) + (m_timer.GetHour() * HOUR) + (m_timer.GetMinute() * MINUTE);

	if ( this->GetStateTimeData(state)->GetDuration() > 0 )
	{
		///- Tanto por comando como para el state de batalla, hay que ponerle la duración máxima
		if ( this->command == true || state == CASTLE_SIEGE_STATE_START )
		{
			this->RunTime(this->GetStateTimeData(state)->GetDuration() * IN_MILLISECONDS);
		}
		else
		{
			///- Si estoy dentro de la fecha entonces proceso la diferencia de horarios
			if ( current_second >= this->GetStateTimeData(state)->GetStartSecond() && current_second <= this->GetStateTimeData(state)->GetEndSecond() )
			{
				this->RunTime((this->GetStateTimeData(state)->GetDuration() - (current_second - this->GetStateTimeData(state)->GetStartSecond())) * IN_MILLISECONDS);
			}
			else
			{
				this->RunTime(this->GetStateTimeData(state)->GetDuration() * IN_MILLISECONDS);
			}
		}

		if ( duration > 0 )
		{
			this->RunTime(duration * MINUTE * IN_MILLISECONDS);
		}
	}
	else
	{
		this->RunTime(0 * IN_MILLISECONDS);
	}

	switch ( state )
	{
	case CASTLE_SIEGE_STATE_IDLE_1:
		{
			this->SetState_Idle_1();
		} break;

	case CASTLE_SIEGE_STATE_REGISTER_GUILD:
		{
			this->SetState_RegisterGuild();
		} break;

	case CASTLE_SIEGE_STATE_IDLE_2:
		{
			this->SetState_Idle_2();
		} break;

	case CASTLE_SIEGE_STATE_REGISTER_MARK:
		{
			this->SetState_RegisterMark();
		} break;

	case CASTLE_SIEGE_STATE_IDLE_3:
		{
			this->SetState_Idle_3();
		} break;

	case CASTLE_SIEGE_STATE_NOTIFY:
		{
			this->SetState_Notify();
		} break;

	case CASTLE_SIEGE_STATE_READY:
		{
			this->SetState_Ready();
		} break;

	case CASTLE_SIEGE_STATE_START:
		{
			this->SetState_Start();
		} break;

	case CASTLE_SIEGE_STATE_END:
		{
			this->SetState_End();
		} break;

	case CASTLE_SIEGE_STATE_ENDCYCLE:
		{
			this->SetState_EndCycle();
		} break;
	}
}

void CCastleSiege::SetState_Idle_1()
{
	this->SetState(CASTLE_SIEGE_STATE_IDLE_1);

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_IDLE_1, this->IsOccupied());

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_Idle_1()");
}
	
void CCastleSiege::SetState_RegisterGuild()
{
	this->SetState(CASTLE_SIEGE_STATE_REGISTER_GUILD);

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_REGISTER_GUILD, this->IsOccupied());
	
	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_RegisterGuild()");
}
	
void CCastleSiege::SetState_Idle_2()
{
	this->SetState(CASTLE_SIEGE_STATE_IDLE_2);

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_IDLE_2, this->IsOccupied());

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_Idle_2()");
}
	
void CCastleSiege::SetState_RegisterMark()
{
	this->SetState(CASTLE_SIEGE_STATE_REGISTER_MARK);

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_REGISTER_MARK, this->IsOccupied());

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_RegisterMark()");
}
		
void CCastleSiege::SetState_Idle_3()
{
	this->SetState(CASTLE_SIEGE_STATE_IDLE_3);

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_IDLE_3, this->IsOccupied());

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_Idle_3()");
}

void CCastleSiege::SetState_Notify()
{
	this->SetState(CASTLE_SIEGE_STATE_NOTIFY);

	this->SetListLoaded(false);
	this->LoadGuildRegister();

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_NOTIFY, this->IsOccupied());
	
	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_Notify()");
}
	
void CCastleSiege::SetState_Ready()
{
	this->SetState(CASTLE_SIEGE_STATE_READY);

	this->LoadGuildRegister();

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_READY, this->IsOccupied());

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_Ready()");
}
	
void CCastleSiege::SetState_Start()
{
	this->SetState(CASTLE_SIEGE_STATE_START);
	this->SetMiddleOwner(0);
	this->ResetCrownUser();
	this->ResetCrownSwitchUser(0);
	this->ResetCrownSwitchUser(1);
	mercenary_count = 0;

	this->InitCharacterCount();

	if ( !this->ExistAttackerGuild() )
	{
		this->CheckResult();
		this->ChangeState(CASTLE_SIEGE_STATE_END);
		sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_Start() - No Attack Guild");
	}
	else
	{
		MAP_CLEAR(CastleSiegeParticipantMap::iterator, this->m_CastleSiegeParticipantMap);

		sCastleDeep->SetState_None();

		this->VerifyNPCExist(); ///- Verifico la existencia de los NPC antes de arrancar
		this->SaveDBNpc();
		this->ClearNonDBNpc();
		this->CreateNonDBNpc();

		this->CloseCastleGate();
		this->SetPlayerJoinSide(true);
		this->RespawnPlayers(true);
		this->SendStartState(1);
	}

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_START, this->IsOccupied());

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_Start()");
}
	
void CCastleSiege::SetState_End()
{
	this->SetState(CASTLE_SIEGE_STATE_END);

	this->ClearNonDBNpc();
	this->ResetPlayerJoinSide();
	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Siege Warfare has been concluded.");

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_END, this->IsOccupied());

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_End()");
}
	
void CCastleSiege::SetState_EndCycle()
{
	this->SetState(CASTLE_SIEGE_STATE_ENDCYCLE);
	
	this->DeleteGuildRegister();

	sLog->outInfo(LOG_CASTLE_SIEGE, "SetState_EndCycle()");

	sServerLink->EventStateUpdate(EVENT_CASTLE_SIEGE, CASTLE_SIEGE_STATE_ENDCYCLE, this->IsOccupied());

	this->ChangeState(CASTLE_SIEGE_STATE_REGISTER_GUILD);
}
	
void CCastleSiege::ProcState_Idle_1()
{
	if ( this->command == true )
		return;

	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_REGISTER_GUILD);
}
	
void CCastleSiege::ProcState_RegisterGuild()
{
	if ( this->IsMinuteElapsed(30) )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Siege Warfare registration period commences.");
	}

	if ( this->command == true )
		return;

	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_IDLE_2);
}
	
void CCastleSiege::ProcState_Idle_2()
{
	if ( this->command == true )
		return;

	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_REGISTER_MARK);
}
	
void CCastleSiege::ProcState_RegisterMark()
{
	if ( this->IsMinuteElapsed(30) )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "All participating guilds for Siege Warfare: please register your Emblem of Lord.");
	}

	if ( this->command == true )
		return;

	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_IDLE_3);
}
	
void CCastleSiege::ProcState_Idle_3()
{
	if ( this->IsMinuteElapsed(120) )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "The Guild selection period for Siege Warfare commences.");
	}

	if ( this->command == true )
		return;

	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_NOTIFY);
}
	
void CCastleSiege::ProcState_Notify()
{
	if ( this->IsMinuteElapsed(120) )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Guilds have now been selected for Siege Warfare.");
	}

	if ( this->command == true )
		return;

	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_READY);
}
	
void CCastleSiege::ProcState_Ready()
{
	if ( this->GetLeftTime()->Elapsed(1800 * IN_MILLISECONDS) )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "The Siege Warfare preparation period begins! All the guild members, please get ready.");
	}

	if ( this->IsMinuteElapsed(1) && (this->GetCurrentMinute() + 1) <= 5 )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Siege Warfare will start in %d minute(s).", this->GetCurrentMinute() + 1);
	}

	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_START);
}
	
void CCastleSiege::ProcState_Start()
{
	this->CheckMiddleWinner();

	if ( this->GetLeftTime()->Elapsed(1 * IN_MILLISECONDS) )
	{
		this->SendLeftTime();
	}

	if ( this->GetUpdateSideTime()->Elapsed(5 * IN_MILLISECONDS) )
	{
		this->SetPlayerJoinSide(false);

		this->UpdateCharacterCount();

		this->UpdateParticipants();
	}

	if ( this->GetSwitchInfoTime()->Elapsed(1 * IN_MILLISECONDS) )
	{
		this->SendSwitchInfo();
	}

	if ( this->GetMiniMapTime()->Elapsed(3 * IN_MILLISECONDS) )
	{
		this->MiniMapUpdate();
	}

	if ( this->IsMinuteElapsed(30) )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Siege Warfare is being waged!");
	}

	if ( this->TimePassed() )
	{
		if( this->CheckResult() )
		{
			this->ResetTaxRate();
		}

		this->RespawnPlayers(false);

		this->GiveParticipantsRewards();

		this->SaveDBCastleOwner();

		this->VerifyNPCExist(); ///- Verifico la existencia de los NPC para actualizar la base de datos
		this->SaveDBNpc();
		this->SendStartState(0);
		this->ChangeState(CASTLE_SIEGE_STATE_END);
	}
}
	
void CCastleSiege::ProcState_End()
{
	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_ENDCYCLE);
}
	
void CCastleSiege::ProcState_EndCycle()
{
	if ( this->TimePassed() )
		this->ChangeState(CASTLE_SIEGE_STATE_IDLE_1);
}

void CCastleSiege::SendCrownAccessState(Player* pPlayer, uint8 state)
{
	Guild * pGuild = pPlayer->GuildGet();

	std::string guildName(pGuild ? pGuild->GetName(): "");

	switch ( state )
	{
	case CASTLE_SIEGE_CROWN_ACCESS_STATE_ATTEMPT:
		{
			sLog->outInfo(LOG_CASTLE_SIEGE, "[ Register Attempt ] Accumulated Crown Access Time : %d - Guild:%s - Player [%s][%s]",
				pPlayer->GetAccumulatedTime(), guildName.c_str(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());
		} break;

	case CASTLE_SIEGE_CROWN_ACCESS_STATE_SUCCESS:
		{
			sLog->outInfo(LOG_CASTLE_SIEGE, "[ Register Success ] Accumulated Crown Access Time : %d - Guild:%s - Player [%s][%s]",
				pPlayer->GetAccumulatedTime(), guildName.c_str(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());

			pPlayer->SetAccumulatedTime(0);
		} break;

	case CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL:
		{
			if ( pPlayer->GetAccumulatedTime() >= sGameServer->GetCastleSiegeCrownTime() )
			{
				pPlayer->SetAccumulatedTime(sGameServer->GetCastleSiegeCrownTime() - 1);
			}

			sLog->outInfo(LOG_CASTLE_SIEGE, "[ Register Fail ] Accumulated Crown Access Time : %d - Guild:%s - Player [%s][%s]",
				pPlayer->GetAccumulatedTime(), guildName.c_str(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());
		} break;
	}

	CROWN_ACCESS_STATE pMsg(state, pPlayer->GetAccumulatedTime() * IN_MILLISECONDS);

	pPlayer->SEND_PCT(pMsg);
}

void CCastleSiege::SendNotifyProgress(uint8 state, const char * guild)
{
	World* pWorld = sWorldMgr->GetWorld(WORLD_CASTLE_SIEGE);

	if ( !pWorld )
		return;

	CASTLE_SIEGE_NOTIFY_PROGRESS pMsg(state, guild);

	WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void CCastleSiege::SendSwitchState(Player* pPlayer, Monster* pSwitch, uint16 SwitchUser, uint8 state)
{
	CASTLE_SIEGE_SWITCH_STATE pMsg(pSwitch->GetEntry(), SwitchUser, state);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::SendSwitchInfo()
{
	if ( CrownSwitchBasicAI* pCrownAI01 = this->GetCrownSwitch(0) )
	{
		this->SendSwitchInfo(pCrownAI01->me());
	}

	if ( CrownSwitchBasicAI* pCrownAI02 = this->GetCrownSwitch(1) )
	{
		this->SendSwitchInfo(pCrownAI02->me());
	}
	
	Player* Switcher01 = this->GetCrownSwitchUser(0);
	Player* Switcher02 = this->GetCrownSwitchUser(1);

	if ( Switcher01 && Switcher02 )
	{
		if ( Switcher01->GetCastleSiegeJoinSide() == Switcher02->GetCastleSiegeJoinSide() )
		{
			sCastleSiege->UnlockCrown();
		}
		else
		{
			sCastleSiege->LockCrown();
		}
	}
}

void CCastleSiege::SendSwitchInfo(Monster* pMonster)
{
	if ( !pMonster )
	{
		return;
	}

	CrownSwitchBasicAI* pAI = AI_TO(CrownSwitchBasicAI, pMonster->GetAI());

	CASTLE_SIEGE_SWITCH_INFO pMsg(pMonster->GetEntry(), pAI->GetID(), 0, 0);
	CASTLE_SIEGE_SWITCH_INFO pMsg2(pMonster->GetEntry(), pAI->GetID(), 0, 0);

	Player* pPlayer = pAI->GetPlayer();
	
	if ( pPlayer && pPlayer->IsPlaying() )
	{
		pMsg.state = 1;
		pMsg.join_side = pPlayer->GetCastleSiegeJoinSide();
		memcpy(pMsg.user, pPlayer->GetName(), MAX_CHARACTER_LENGTH);

		if ( Guild* pGuild = pPlayer->GuildGet() )
		{
			memcpy(pMsg.guild, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);

			if ( Guild * pAlliance = sGuildMgr->GetGuild(pGuild->GetAlliance()) )
			{
				memcpy(pMsg.guild, pAlliance->GetName(), MAX_GUILD_NAME_LENGTH);
			}
		}
	}

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( pPlayer->IsInDragonTower() )
		{
			pPlayer->SEND_PCT(pMsg);
		}
		else
		{
			pPlayer->SEND_PCT(pMsg2);
		}
	}
}

void CCastleSiege::SendStartState(uint8 state)
{
	CASTLE_SIEGE_START_STATE pMsg(state);
	sObjectMgr->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::SendStartState(Player* pPlayer, uint8 state)
{
	CASTLE_SIEGE_START_STATE pMsg(state);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::SendJoinSide(Player* pPlayer)
{
	uint8 state = pPlayer->GetCastleSiegeJoinSide();
	bool add = this->GetState() == CASTLE_SIEGE_STATE_START && pPlayer->GetWorldId() == WORLD_CASTLE_SIEGE;

	switch ( state )
	{
	case CASTLE_SIEGE_JOIN_SIDE_DEFENSE:
		{
			if ( add )
			{
				pPlayer->AddBuff(BUFF_CASTLE_SIEGE_DEFENSE, 0, BUFF_FLAG_CONSTANT, pPlayer);
			}
			else
			{
				pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_DEFENSE);
			}
		} break;

	case CASTLE_SIEGE_JOIN_SIDE_ATTACK_1:
		{
			if ( add )
			{
				pPlayer->AddBuff(BUFF_CASTLE_SIEGE_ATTACK_1, 0, BUFF_FLAG_CONSTANT, pPlayer);
			}
			else
			{
				pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_ATTACK_1);
			}
		} break;

	case CASTLE_SIEGE_JOIN_SIDE_ATTACK_2:
		{
			if ( add )
			{
				pPlayer->AddBuff(BUFF_CASTLE_SIEGE_ATTACK_2, 0, BUFF_FLAG_CONSTANT, pPlayer);
			}
			else
			{
				pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_ATTACK_2);
			}
		} break;

	case CASTLE_SIEGE_JOIN_SIDE_ATTACK_3:
		{
			if ( add )
			{
				pPlayer->AddBuff(BUFF_CASTLE_SIEGE_ATTACK_3, 0, BUFF_FLAG_CONSTANT, pPlayer);
			}
			else
			{
				pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_ATTACK_3);
			}
		} break;

	default:
		{
			pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_DEFENSE);
			pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_ATTACK_1);
			pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_ATTACK_2);
			pPlayer->RemoveBuff(BUFF_CASTLE_SIEGE_ATTACK_3);
		} break;
	}

	CASTLE_SIEGE_JOIN_SIDE pMsg(state);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::SendLeftTime()
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		this->SendLeftTime(pPlayer);
	}
}

void CCastleSiege::SendLeftTime(Player* pPlayer)
{
	uint8 hour = (this->GetRemain() / IN_MILLISECONDS) / HOUR;
	uint8 minute = ((this->GetRemain() / IN_MILLISECONDS) % HOUR) / MINUTE;

	CASTLE_SIEGE_REMAIN_TIME pMsg(hour, minute);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::SendMiniMap(Player* pPlayer, uint8 result)
{
	CASTLE_SIEGE_MINI_MAP pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::SendGateState(Player* pPlayer, uint8 result, uint16 index)
{
	CASTLE_SIEGE_GATE_STATE pMsg(result, index);
	pPlayer->SEND_PCT(pMsg);
}

void CCastleSiege::StateRequest(Player* pPlayer)
{
	CASTLE_SIEGE_STATE pMsg;

	Guild* pGuild = sGuildMgr->GetGuild(this->GetCastleOwner());
	
	if ( pGuild )
	{
		memcpy(pMsg.guild, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);
		memcpy(pMsg.guild_master, pGuild->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH);
		pMsg.result = 1;
	}
	else
	{
		pMsg.result = 3;
	}

	if ( !this->IsOccupied() )
	{
		pMsg.result = 2;
	}

	pMsg.state = this->GetState();

	/*time_t rawtime;
	time(&rawtime);

	time_t start_date = rawtime - (this->GetStateTimeData(this->GetState())->duration.get() - (this->GetRemain() / IN_MILLISECONDS));
	time_t end_date = rawtime + (this->GetRemain() / IN_MILLISECONDS);

	if ( tm * timeinfo = localtime(&start_date) )
	{
		pMsg.Setstart_year(timeinfo->tm_year + 1900);
		pMsg.start_month = timeinfo->tm_mon + 1;
		pMsg.start_day = timeinfo->tm_mday;
		pMsg.start_hour = timeinfo->tm_hour;
		pMsg.start_minute = timeinfo->tm_min;
	}

	if ( tm * timeinfo = localtime(&end_date) )
	{
		pMsg.Setend_year(timeinfo->tm_year + 1900);
		pMsg.end_month = timeinfo->tm_mon + 1;
		pMsg.end_day = timeinfo->tm_mday;
		pMsg.end_hour = timeinfo->tm_hour;
		pMsg.end_minute = timeinfo->tm_min;
	}*/

	/*time_t start_date = time(nullptr) - (this->GetStateTimeData(this->GetState())->duration.get() - (this->GetRemain() / IN_MILLISECONDS));
	time_t end_date = time(nullptr) + (this->GetRemain() / IN_MILLISECONDS);
	time_t start_siege_date = time(nullptr);

	// Siege Start Date
	/*tmData = localtime(&start_siege_date);
	tmData->tm_mday = 0;
	tmData->tm_mon = 0;
	tmData->tm_sec = 0;
	tmData->tm_yday = 0;
	tmData->tm_year = 0;

	tmData->tm_hour = this->GetStateTimeData(CASTLE_SIEGE_STATE_START)->hour.get();
	tmData->tm_min = this->GetStateTimeData(CASTLE_SIEGE_STATE_START)->minute.get();
	tmData->tm_wday = this->GetStateTimeData(CASTLE_SIEGE_STATE_START)->day.get();

	mktime(tmData);

	pMsg.Setsiege_start_year(tmData->tm_year + 1900);
	pMsg.siege_start_month = tmData->tm_mon;
	pMsg.siege_start_day = tmData->tm_mday;
	pMsg.siege_start_hour = tmData->tm_hour;
	pMsg.siege_start_minute = tmData->tm_min;*/

	Util::ConvertToByte(int32(this->GetRemain() / IN_MILLISECONDS), pMsg.remain_time);

	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::RegisterGuildRequest(Player* pPlayer)
{
	/*
	0 -> Failed to register for Castle Siege
	1 -> Castle Siege registration successfully
	2 -> Already registered in castle siege
	3 -> You belong to the guild of de defending guild
	4 -> Incorrect Guild
	5 -> Guild Master Level is Insufficient
	6 -> No affiliated Guild
	7 -> It's not a registration perdiod for castle siege
	8 -> Number of guild members is lacking
	*/
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
		return;

	if ( this->GetState() != CASTLE_SIEGE_STATE_REGISTER_GUILD || !sGameServer->IsCastleSiegeEnabled() || !sGameServer->IsCastleSiegeRegisterEnabled() )
	{
		this->RegisterGuildResult(pPlayer, 7);
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->RegisterGuildResult(pPlayer, 4);
		return;
	}

	if ( !pGuild->IsAllianceMaster() )
	{
		this->RegisterGuildResult(pPlayer, 4);
		return;
	}
	
	if ( pGuild->GetID() != pPlayer->GetGUID() )
	{
		this->RegisterGuildResult(pPlayer, 0);
		return;
	}

	if ( pGuild->IsRegisteredInCaslteSiege() )
	{
		this->RegisterGuildResult(pPlayer, 2);
		return;
	}

	if ( this->GetCastleOwner() && (this->GetCastleOwner() == pGuild->GetID() || pGuild->GetAlliance() == this->GetCastleOwner()) )
	{
		this->RegisterGuildResult(pPlayer, 3);
		return;
	}

	if ( pPlayer->GetTotalLevel() < sGameServer->GetCastleSiegeRegisterMinLevel() )
	{
		this->RegisterGuildResult(pPlayer, 5);
		return;
	}

	if ( pGuild->GetTotalMembers() < sGameServer->GetCastleSiegeRegisterMinMembers() )
	{
		this->RegisterGuildResult(pPlayer, 8);
		return;
	}

	SL_CASTLE_SIEGE_INSERT_REGISTERED_GUILD pMsg;
	pMsg.guild = pGuild->GetID();
	sServerLink->SendPacket(MAKE_PCT(pMsg));

	this->RegisterGuildResult(pPlayer, 1, pGuild->GetName());

	sServerLink->CastleSiegeRegisterGuild(pGuild->GetID(), true, 0);
}

void CCastleSiege::RegisterGuildResult(Player* pPlayer, uint8 result, const char * guild)
{
	CASTLE_SIEGE_REGISTER_GUILD pMsg(result, guild);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::GiveupGuildRequest(Player* pPlayer)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
		return;

	if ( this->GetState() < CASTLE_SIEGE_STATE_REGISTER_GUILD ||
		 this->GetState() > CASTLE_SIEGE_STATE_REGISTER_MARK )
	{
		this->GiveupGuildResult(pPlayer, 3, 0);
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->GiveupGuildResult(pPlayer, 2, 0);
		return;
	}

	if ( !pGuild->IsRegisteredInCaslteSiege() || pGuild->GetID() != pPlayer->GetGUID() )
	{
		this->GiveupGuildResult(pPlayer, 2, 0);
		return;
	}

	SL_CASTLE_SIEGE_DELETE_REGISTERED_GUILD pMsg;
	pMsg.guild = pGuild->GetID();
	sServerLink->SendPacket(MAKE_PCT(pMsg));

	this->GiveupGuildResult(pPlayer, 1, 1, pGuild->GetName());

	sServerLink->CastleSiegeRegisterGuild(pGuild->GetID(), false, 0);
}
	
void CCastleSiege::GiveupGuildResult(Player* pPlayer, uint8 result, uint8 giveup, const char * guild)
{
	CASTLE_SIEGE_GIVEUP_GUILD pMsg(result, giveup, guild);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::RegisterGuildInfoRequest(Player* pPlayer)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
	{
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild || !pGuild->IsRegisteredInCaslteSiege() )
	{
		this->RegisterGuildInfoResult(pPlayer, 0, nullptr, 0, 0);
		return;
	}

	sServerLink->CastleSiegeRegisteredGuildRequest(pPlayer, pGuild);
}

void CCastleSiege::RegisterGuildInfoResult(Player* pPlayer, uint8 result, const char * guild, uint32 mark, uint8 ranking)
{
	CASTLE_SIEGE_GUILD_REGISTER_INFO pMsg(result, guild, mark, ranking);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::RegisterGuildInfoCallBack(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD, head, packet, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_BODY, body, packet, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD));

	Player* pPlayer = sObjectMgr->FindPlayer(head->entry);

	if ( !pPlayer || !pPlayer->IsPlaying() )
		return;

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
		return;

	if ( !head->count )
	{
		this->RegisterGuildInfoResult(pPlayer, 0, nullptr, 0, 0);
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->RegisterGuildInfoResult(pPlayer, 0, nullptr, 0, 0);
		return;
	}

	this->RegisterGuildInfoResult(pPlayer, 1, pGuild->GetName(), body[0].marks, body[0].register_id);
}

void CCastleSiege::RegisterMarkRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
		return;

	if ( this->GetState() != CASTLE_SIEGE_STATE_REGISTER_MARK || !sGameServer->IsCastleSiegeEnabled() || !sGameServer->IsCastleSiegeRegisterSignEnabled() )
	{
		this->RegisterMarkResult(pPlayer, 3, "", 0);
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild || !pGuild->IsRegisteredInCaslteSiege() || pGuild->GetID() != pPlayer->GetGUID() )
	{
		this->RegisterMarkResult(pPlayer, 3, "", 0);
		return;
	}

	POINTER_PCT_LOG(CASTLE_SIEGE_REGISTER_MARK, lpMsg, Packet, 0);
	
	uint8 slot = lpMsg->slot + PLAYER_MAX_EQUIPMENT;

	if ( !use_inventory_range(slot) )
	{
		this->RegisterMarkResult(pPlayer, 3, "", 0);
		return;
	}

	if ( pPlayer->GetInventory()->GetItem(slot)->GetItem() != ITEMGET(14, 21) ||
		 pPlayer->GetInventory()->GetItem(slot)->GetLevel() != 3 ||
		 pPlayer->GetInventory()->GetItem(slot)->GetDurability() < 1.0f )
	{
		this->RegisterMarkResult(pPlayer, 3, "", 0);
		return;
	}

	pPlayer->DecreaseItemDurabilityByUse(slot, 1.0f);
	pGuild->SetCastleSiegeMarks(pGuild->GetCastleSiegeMarks() + 1);

	SL_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD pMsg(pGuild->GetID(), pGuild->GetCastleSiegeMarks());
	sServerLink->SendPacket(MAKE_PCT(pMsg));

	this->RegisterMarkResult(pPlayer, 1, pGuild->GetName(), pGuild->GetCastleSiegeMarks());

	sServerLink->CastleSiegeRegisterGuild(pGuild->GetID(), pGuild->IsRegisteredInCaslteSiege(), pGuild->GetCastleSiegeMarks());
}

void CCastleSiege::RegisterMarkResult(Player* pPlayer, uint8 result, const char * guild, uint32 mark)
{
	CASTLE_SIEGE_REGISTER_MARK_RESULT pMsg(result, guild, mark);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::NpcBuyRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsNpcLoaded() )
		return;

	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::LordMix )
		return;

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
		return;

	if ( this->GetCastleOwner() != pGuild->GetID() )
		return;

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER && pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_ASISTANT )
		return;

	POINTER_PCT_LOG(CASTLE_SIEGE_NPC_BUY, lpMsg, Packet, 0);

	CastleSiegeNpcData * pNpcData = this->GetNpcData(lpMsg->npc, lpMsg->id);

	if ( !pNpcData )
	{
		this->NpcBuyResult(pPlayer, 2, lpMsg->npc, lpMsg->id);
		return;
	}

	Monster* pMonster = this->GetMonster(pNpcData->monster.get(), pNpcData->id.get());

	if ( pMonster && pMonster->IsLive() && pMonster->IsPlaying() ) ///- Si el monster está activo, entonces no debería poder comprarlo
	{
		this->NpcBuyResult(pPlayer, 4, lpMsg->npc, lpMsg->id);
		return;
	}

	uint8 type = lpMsg->npc == 277 ? CASTLE_SIEGE_NPC_UPGRADE_GATE: lpMsg->npc == 283 ? CASTLE_SIEGE_NPC_UPGRADE_STATUE: CASTLE_SIEGE_NPC_UPGRADE_MAX;

	if ( type == CASTLE_SIEGE_NPC_UPGRADE_MAX )
	{
		this->NpcBuyResult(pPlayer, 2, lpMsg->npc, lpMsg->id);
		return;
	}

	uint32 cost = this->GetNpcBuyPrice(type);

	if ( !pPlayer->MoneyHave(cost) )
	{
		this->NpcBuyResult(pPlayer, 3, lpMsg->npc, lpMsg->id);
		return;
	}

	pNpcData->current_life.set(this->m_npc_life[type][0].life.get());
	pNpcData->regen_level.set(0);
	pNpcData->defense_level.set(0);
	pNpcData->life_level.set(0);

	if ( !this->AddDBNpc(pNpcData) )
	{
		this->NpcBuyResult(pPlayer, 0, lpMsg->npc, lpMsg->id);
		return;
	}

	pNpcData->SetActive(true);

	this->SaveDBNpc();

	this->NpcBuyResult(pPlayer, 1, lpMsg->npc, lpMsg->id);
	pPlayer->MoneyReduce(cost);
}

void CCastleSiege::NpcBuyResult(Player* pPlayer, uint8 result, int32 npc, int32 id)
{
	CASTLE_SIEGE_NPC_BUY_RESULT pMsg(result, npc, id);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::NpcRepairRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsNpcLoaded() )
		return;

	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::LordMix )
		return;

	if ( this->GetState() == CASTLE_SIEGE_STATE_START )
		return;

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
		return;

	if ( this->GetCastleOwner() != pGuild->GetID() )
		return;

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER && pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_ASISTANT )
		return;

	POINTER_PCT_LOG(CASTLE_SIEGE_NPC_REPAIR, lpMsg, Packet, 0);

	CastleSiegeNpcData * pNpcData = this->GetNpcData(lpMsg->npc, lpMsg->id);

	if ( !pNpcData || !pNpcData->IsActive() ) ///- El Npc a arreglar tiene que estar activo
	{
		this->NpcRepairResult(pPlayer, 0, lpMsg->npc, lpMsg->id, 0, 0);
		return;
	}

	Monster* pMonster = this->GetMonster(lpMsg->npc, lpMsg->id);

	if ( !pMonster || !pMonster->IsPlaying() || !pMonster->IsLive() ) ///- Si el monster no está activo, entonces no puedo repararlo
	{
		this->NpcRepairResult(pPlayer, 0, lpMsg->npc, lpMsg->id, 0, 0);
		return;
	}

	if ( !pMonster->IsAIGroup(CS_GUARDIAN_STATUE_AI) && !pMonster->IsAIGroup(CS_GATE_AI) ) ///- Solamente puedo arreglar estatuas y puertas
	{
		this->NpcRepairResult(pPlayer, 0, lpMsg->npc, lpMsg->id, 0, 0);
		return;
	}

	if ( pMonster->PowerGet(POWER_LIFE) > pMonster->PowerGetMax(POWER_LIFE) )
	{
		pMonster->PowerSet(POWER_LIFE, pMonster->PowerGetMax(POWER_LIFE));
		this->NpcRepairResult(pPlayer, 1, lpMsg->npc, lpMsg->id, pMonster->PowerGet(POWER_LIFE), pMonster->PowerGetMax(POWER_LIFE));
		return;
	}

	uint32 repair_cost = 0;

	switch ( lpMsg->npc )
	{
	case 277:
		{
			repair_cost = ((pMonster->PowerGetMax(POWER_LIFE) - pMonster->PowerGet(POWER_LIFE)) * 5) + (pNpcData->defense_level.get() * 1000000);
		} break;

	case 283:
		{
			repair_cost = ((pMonster->PowerGetMax(POWER_LIFE) - pMonster->PowerGet(POWER_LIFE)) * 3) + ((pNpcData->defense_level.get() + pNpcData->regen_level.get()) * 1000000);
		} break;
	}

	if ( !pPlayer->MoneyHave(repair_cost) )
	{
		this->NpcRepairResult(pPlayer, 3, lpMsg->npc, lpMsg->id, 0, 0);
		return;
	}

	pNpcData->current_life.set(pMonster->PowerGetMax(POWER_LIFE));
	pMonster->PowerSet(POWER_LIFE, pMonster->PowerGetMax(POWER_LIFE));

	pPlayer->MoneyReduce(repair_cost);
	this->SaveDBNpc();

	this->NpcRepairResult(pPlayer, 1, lpMsg->npc, lpMsg->id, pMonster->PowerGet(POWER_LIFE), pMonster->PowerGetMax(POWER_LIFE));
}

void CCastleSiege::NpcRepairResult(Player* pPlayer, uint8 result, int32 npc, int32 id, int32 hp, int32 max_hp)
{
	CASTLE_SIEGE_NPC_REPAIR_RESULT pMsg(result, npc, id, hp, max_hp);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::NpcUpgradeRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsNpcLoaded() )
		return;

	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( this->GetState() == CASTLE_SIEGE_STATE_START )
		return;

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::LordMix )
		return;

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
		return;

	if ( this->GetCastleOwner() != pGuild->GetID() )
		return;

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER && pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_ASISTANT )
		return;

	POINTER_PCT_LOG(CASTLE_SIEGE_NPC_UPGRADE, lpMsg, Packet, 0);

	CastleSiegeNpcData * pNpcData = this->GetNpcData(lpMsg->npc, lpMsg->id);

	if ( !pNpcData || !pNpcData->IsActive() )
	{
		this->NpcUpgradeResult(pPlayer, 7, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
		return;
	}

	Monster* pMonster = this->GetMonster(lpMsg->npc, lpMsg->id);

	if ( !pMonster || !pMonster->IsPlaying() || !pMonster->IsLive() )
	{
		this->NpcUpgradeResult(pPlayer, 7, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
		return;
	}

	if ( !pMonster->IsAIGroup(CS_GUARDIAN_STATUE_AI) && !pMonster->IsAIGroup(CS_GATE_AI) ) ///- Solamente puedo mejorar estatuas y puertas
	{
		this->NpcUpgradeResult(pPlayer, 6, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
		return;
	}

	bool upgrade = false;
	uint32 required_zen = 0;
	int32 required_jog = 0;
	int32 upgrade_value = 0;
	
	switch ( lpMsg->npc )
	{
	case 277:
		{
			switch ( lpMsg->upgrade_type )
			{
			case CASTLE_SIEGE_UPGRADE_DEFENSE:
				{
					if ( !CASTLE_SIEGE_UPGRADE_DEF_RANGE(pNpcData->defense_level.get()) )
						break;

					lpMsg->upgrade_value = pNpcData->defense_level.get() + 1;

					if ( !CASTLE_SIEGE_UPGRADE_DEF_RANGE(lpMsg->upgrade_value) )
						break;
					
					upgrade = true;
					required_jog = this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_GATE][lpMsg->upgrade_value].jog_count.get();
					required_zen = this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_GATE][lpMsg->upgrade_value].required_zen.get();
					upgrade_value = this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_GATE][lpMsg->upgrade_value].defense.get();
				} break;

			case CASTLE_SIEGE_UPGRADE_LIFE:
				{
					if ( !CASTLE_SIEGE_UPGRADE_LIFE_RANGE(pNpcData->life_level.get()) )
						break;

					lpMsg->upgrade_value = pNpcData->life_level.get() + 1;

					if ( !CASTLE_SIEGE_UPGRADE_LIFE_RANGE(lpMsg->upgrade_value) )
						break;

					upgrade = true;
					required_jog = this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][pNpcData->life_level.get() + 1].jog_count.get();
					required_zen = this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][pNpcData->life_level.get() + 1].required_zen.get();
					lpMsg->upgrade_value = upgrade_value = this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][pNpcData->life_level.get() + 1].life.get();
				} break;

			default:
				{
					this->NpcUpgradeResult(pPlayer, 5, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
					return;
				} break;
			}
		} break;

	case 283:
		{
			switch ( lpMsg->upgrade_type )
			{
			case CASTLE_SIEGE_UPGRADE_DEFENSE:
				{
					if ( !CASTLE_SIEGE_UPGRADE_DEF_RANGE(pNpcData->defense_level.get()) )
						break;

					lpMsg->upgrade_value = pNpcData->defense_level.get() + 1;

					if ( !CASTLE_SIEGE_UPGRADE_DEF_RANGE(lpMsg->upgrade_value) )
						break;
					
					upgrade = true;
					required_jog = this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_STATUE][lpMsg->upgrade_value].jog_count.get();
					required_zen = this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_STATUE][lpMsg->upgrade_value].required_zen.get();
					upgrade_value = this->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_STATUE][lpMsg->upgrade_value].defense.get();
				} break;

			case CASTLE_SIEGE_UPGRADE_REGEN:
				{
					if ( !CASTLE_SIEGE_UPGRADE_REG_RANGE(pNpcData->regen_level.get()) )
						break;

					lpMsg->upgrade_value = pNpcData->regen_level.get() + 1;

					if ( !CASTLE_SIEGE_UPGRADE_REG_RANGE(lpMsg->upgrade_value) )
						break;

					upgrade = true;
					required_jog = this->m_npc_regen[lpMsg->upgrade_value].jog_count.get();
					required_zen = this->m_npc_regen[lpMsg->upgrade_value].required_zen.get();
				} break;

			case CASTLE_SIEGE_UPGRADE_LIFE:
				{
					if ( !CASTLE_SIEGE_UPGRADE_LIFE_RANGE(pNpcData->life_level.get()) )
						break;

					lpMsg->upgrade_value = pNpcData->life_level.get() + 1;

					if ( !CASTLE_SIEGE_UPGRADE_LIFE_RANGE(lpMsg->upgrade_value) )
						break;

					upgrade = true;
					required_jog = this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][pNpcData->life_level.get() + 1].jog_count.get();
					required_zen = this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][pNpcData->life_level.get() + 1].required_zen.get();
					lpMsg->upgrade_value = upgrade_value = this->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][pNpcData->life_level.get() + 1].life.get();
				} break;

			default:
				{
					this->NpcUpgradeResult(pPlayer, 5, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
					return;
				} break;
			}
		} break;
	}

	if ( !upgrade )
	{
		this->NpcUpgradeResult(pPlayer, 6, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
		return;
	}

	if ( !pPlayer->MoneyHave(required_zen) )
	{
		this->NpcUpgradeResult(pPlayer, 3, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
		return;
	}

	if ( pPlayer->GetInventory()->GetItemCount(JEWEL::GUARDIAN, -1) < required_jog )
	{
		this->NpcUpgradeResult(pPlayer, 4, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);
		return;
	}

	pPlayer->ItemDelete(JEWEL::GUARDIAN, -1, required_jog);

	pPlayer->MoneyReduce(required_zen);

	switch ( lpMsg->upgrade_type )
	{
	case CASTLE_SIEGE_UPGRADE_DEFENSE:
		{
			pNpcData->defense_level.op_inc();
			pMonster->SetIntData(UNIT_INT_DEFENSE, upgrade_value);
		} break;

	case CASTLE_SIEGE_UPGRADE_REGEN:
		{
			pNpcData->regen_level.op_inc();
		} break;

	case CASTLE_SIEGE_UPGRADE_LIFE:
		{
			pNpcData->life_level.op_inc();
			uint32 power_max = pMonster->PowerGetMax(POWER_LIFE);
			pMonster->PowerSetMax(POWER_LIFE, upgrade_value);
			pMonster->PowerIncrease(POWER_LIFE, upgrade_value - power_max);
		} break;
	}

	this->SaveDBNpc();
	pPlayer->SaveCharacter();

	this->NpcUpgradeResult(pPlayer, 1, lpMsg->npc, lpMsg->id, lpMsg->upgrade_type, lpMsg->upgrade_value);

/*		LogAdd(4,"[ CASTLE SIEGE ] CGReqNpcUpgrade() - Pay For Npc Upgrade (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",
			lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE);
				
		LogAdd(4,"[ CASTLE SIEGE ] CGReqNpcUpgrade() REQ OK - [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d",
			gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].GuildName,gObj[iIndex].GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,iNXT_UPLVL);*/
}

void CCastleSiege::NpcUpgradeResult(Player* pPlayer, uint8 result, int32 npc, int32 id, int32 type, int32 value)
{
	CASTLE_SIEGE_NPC_UPGRADE_RESULT pMsg(result, npc, id, type, value);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::TaxMoneyRequest(Player* pPlayer)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !this->CastleOwnerMember(pPlayer) || pPlayer->GuildGetRanking() != GUILD_RANK_MASTER )
		return;

	CASTLE_SIEGE_TAX_MONEY pMsg;
	pMsg.tax_rate_chaos = this->GetTaxRateChaos();
	pMsg.tax_rate_store = this->GetTaxRateStore();
	Util::ConvertToByte(this->GetTributeMoney(), pMsg.money);

	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::TaxRateRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !this->CastleOwnerMember(pPlayer) || pPlayer->GuildGetRanking() != GUILD_RANK_MASTER )
		return;

	POINTER_PCT_LOG(CASTLE_SIEGE_TAX_RATE_CHANGE, lpMsg, Packet, 0);

	int32 max_tax = -1;

	switch ( lpMsg->tax_type )
	{
	case 1: // Chaos
		{
			max_tax = 3;
		} break;

	case 2: // Store
		{
			max_tax = 3;
		} break;

	case 3: // Hunt
		{
			max_tax = CASTLE_SIEGE_TAX_RATE_HUNT_MAX;
		} break;
	}

	int32 tax_rate = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->tax_rate[0], lpMsg->tax_rate[1]), MAKE_NUMBERW(lpMsg->tax_rate[2], lpMsg->tax_rate[3])); 

	if ( tax_rate < 0 || max_tax < 0 || tax_rate > max_tax )
		return;

	switch ( lpMsg->tax_type )
	{
	case 1: // Chaos
		{
			this->SetTaxRateChaos(tax_rate);
		} break;

	case 2: // Store
		{
			this->SetTaxRateStore(tax_rate);
		} break;

	case 3: // Hunt
		{
			this->SetTaxRateHunt(tax_rate);
		} break;
	}

	this->SaveDBTaxRate();

	CASTLE_SIEGE_TAX_RATE_CHANGE_RESULT pMsg;
	pMsg.result = 1;
	pMsg.tax_type = lpMsg->tax_type;
	memcpy(pMsg.tax_rate, lpMsg->tax_rate, 4);

	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::EnableHuntRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}
	
	POINTER_PCT_LOG(CASTLE_SIEGE_HUNT_ZONE_ALLOW, lpMsg, Packet, 0);

	if ( !this->CastleOwnerMember(pPlayer) || pPlayer->GuildGetRanking() != GUILD_RANK_MASTER )
	{
		this->EnableHuntResult(pPlayer, 2, lpMsg->allow);
		return;
	}

	if ( this->GetState() == CASTLE_SIEGE_STATE_START )
	{
		this->EnableHuntResult(pPlayer, 0, lpMsg->allow);
		return;
	}

	this->SetHuntEnabled(lpMsg->allow);

	this->SaveDBTaxRate();

	this->EnableHuntResult(pPlayer, 1, lpMsg->allow);
}

void CCastleSiege::EnableHuntResult(Player* pPlayer, uint8 result, uint8 allow)
{
	CASTLE_SIEGE_HUNT_ZONE_ALLOW_RESULT pMsg(result, allow);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::RequestGuildList(Player* pPlayer)
{
	if ( !pPlayer || pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
		return;

	uint8 buffer[2500];
	POINTER_PCT(CASTLE_SIEGE_GUILD_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(CASTLE_SIEGE_GUILD_LIST_BODY, body, buffer, sizeof(CASTLE_SIEGE_GUILD_LIST_HEAD));
	uint16 size = sizeof(CASTLE_SIEGE_GUILD_LIST_HEAD);
	uint8 count = 0;
	
	head->h.c = 0xC2;
	head->h.headcode = HEADCODE_CASTLE_SIEGE_GUILD_LIST;
	head->h.set_size(size);
	head->count = count;

	if ( this->GetState() != CASTLE_SIEGE_STATE_NOTIFY && this->GetState() != CASTLE_SIEGE_STATE_READY )
	{
		head->result = 2;
		pPlayer->sendPacket(buffer, size);
		return;
	}

	Guild* pGuild = nullptr;

	for ( CastleSiegeGuildDataMap::const_iterator it = this->m_guild_data.begin(); it != this->m_guild_data.end(); ++it )
	{
		pGuild = sGuildMgr->GetGuild(it->second->GetGuild());

		if ( !pGuild )
			continue;

		memcpy(body[count].guild, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);
		body[count].involved = pGuild->IsAllianceMaster();
		body[count].score = it->second->GetScore();
		body[count].side = it->second->GetSide();

		++count;
		size += sizeof(CASTLE_SIEGE_GUILD_LIST_BODY);
	}
	
	head->h.set_size(size);
	head->result = 1;
	head->count = count;
	pPlayer->sendPacket(buffer, size);
}

void CCastleSiege::RequestCastleOwnerMark(Player* pPlayer)
{
	Guild* pGuild = sGuildMgr->GetGuild(this->GetCastleOwner());

	if ( !pGuild )
		return;

	CASTLE_SIEGE_OWNER_MARK pMsg(pGuild->emblem);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::LandOfTrialsMoveRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	POINTER_PCT_LOG(CASTLE_SIEGE_LAND_OF_TRIALS_MOVE, lpMsg, Packet, 0);

	int32 enter_money = this->GetTaxRateHunt(pPlayer);
	limitmin(enter_money, 0);
	bool allowed = this->IsHuntEnabled() || this->CastleOwnerMember(pPlayer) || this->CastleOwnerUnionMember(pPlayer);

	if ( !allowed )
	{
		this->LandOfTrialsMoveResult(pPlayer, 0);
		return;
	}

	if ( !pPlayer->MoneyHave(enter_money) )
	{
		this->LandOfTrialsMoveResult(pPlayer, 0);
		return;
	}

	//if ( pPlayer->GetWorldId() != WORLD_CASTLE_SIEGE )
	//{
	//	this->LandOfTrialsMoveResult(pPlayer, 0);
	//	return;
	//}

	if ( !pPlayer->MoveToGate(95) )
	{
		this->LandOfTrialsMoveResult(pPlayer, 0);
		return;
	}

	pPlayer->MoneyReduce(enter_money, true);
	this->AddTributeMoney(enter_money);

	this->LandOfTrialsMoveResult(pPlayer, 1);
}

void CCastleSiege::LandOfTrialsMoveResult(Player* pPlayer, uint8 result)
{
	CASTLE_SIEGE_LAND_OF_TRIALS_MOVE_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::RequestGateOperate(Player* pPlayer, uint8 * Packet)
{
	if ( !pPlayer )
		return;

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleSiegeLever )
		return;

	if ( !pPlayer->GetInterfaceState()->GetTarget() || !pPlayer->GetInterfaceState()->GetTarget()->IsCreature() )
		return;

	POINTER_PCT_LOG(CASTLE_SIEGE_GATE_OPERATE, lpMsg, Packet, 0);

	if ( this->GetState() == CASTLE_SIEGE_STATE_START )
	{
		if ( pPlayer->GetCastleSiegeJoinSide() != CASTLE_SIEGE_JOIN_SIDE_DEFENSE )
		{
			//LogAdd(5,"[ CASTLE SIEGE ] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
				//gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
			return;
		}
	}
	else
	{
		if ( !this->CastleOwnerMember(pPlayer) || !this->CastleOwnerUnionMember(pPlayer) )
		{
			//LogAdd(5,"[ CASTLE SIEGE ] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
				//gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
			return;
		}
	}

	MONSTER_AI_NAME(pPlayer->GetInterfaceState()->GetTarget()->ToCreature(), CS_LEVER_AI, GateLeverBasicAI)->Operate(pPlayer, lpMsg->operation, lpMsg->GetIndex());
}

void CCastleSiege::RequestGateOperateResult(Player* pPlayer, uint8 result, uint8 operation, uint16 gate)
{
	CASTLE_SIEGE_GATE_OPERATE_RESULT pMsg(result, operation, gate);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CCastleSiege::SetTaxRate(int32 tax_hunt, uint8 tax_chaos, uint8 tax_store, bool hunt_allowed)
{
	this->SetTaxRateHunt(tax_hunt);
	this->SetTaxRateChaos(tax_chaos);
	this->SetTaxRateStore(tax_store);
	this->SetHuntEnabled(hunt_allowed);
}

void CCastleSiege::NpcListRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	POINTER_PCT_LOG(CASTLE_SIEGE_NPC_LIST_REQUEST, lpMsg, Packet, 0);

	if ( !this->CastleOwnerMember(pPlayer) || (pPlayer->GuildGetRanking() != GUILD_RANK_MASTER && pPlayer->GuildGetRanking() != GUILD_RANK_ASISTANT) )
	{
		return;
	}

	uint8 buffer[10000];
	POINTER_PCT(CASTLE_SIEGE_NPC_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(CASTLE_SIEGE_NPC_LIST_BODY, body, buffer, sizeof(CASTLE_SIEGE_NPC_LIST_HEAD));
	head->count = 0;

	uint16 npc = lpMsg->code == 1 ? 277: lpMsg->code == 2 ? 283: 0;
	Monster* pMonster = nullptr;
		
	for ( CastleSiegeNpcDataList::const_iterator it = this->m_npc_data.begin(); it != this->m_npc_data.end(); ++it )
	{
		if ( (*it)->monster != npc )
			continue;

		pMonster = this->GetMonster(npc, (*it)->id.get());

		if ( pMonster && pMonster->IsPlaying() && pMonster->IsLive() )
		{
			body[head->count].hp = pMonster->PowerGet(POWER_LIFE);
			body[head->count].max_hp = pMonster->PowerGetMax(POWER_LIFE);
			body[head->count].live = 1;
		}
		else
		{
			body[head->count].hp = 0;
			body[head->count].max_hp = this->m_npc_life[lpMsg->code - 1][0].life.get();
			body[head->count].live = 0;
		}

		body[head->count].number = (*it)->monster.get();
		body[head->count].index = (*it)->id.get();
		body[head->count].defense_level = (*it)->defense_level.get();
		body[head->count].regen_level = (*it)->regen_level.get();
		body[head->count].x = (*it)->x.get();
		body[head->count].y = (*it)->y.get();

		++head->count;
	}
	
	head->h.set(HEADCODE_CASTLE_SIEGE_NPC_LIST, sizeof(CASTLE_SIEGE_NPC_LIST_HEAD) + (sizeof(CASTLE_SIEGE_NPC_LIST_BODY) * head->count));
	head->result = 1;

	pPlayer->sendPacket(buffer, head->h.get_size());
}

void CCastleSiege::GuildRegisterListRequest(Player* pPlayer)
{
	if ( !pPlayer || !pPlayer->IsPlaying() )
		return;

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
		return;

	sServerLink->CastleSiegeRegisteredGuildAllRequest(pPlayer);
}

void CCastleSiege::GuildRegisterListCallBack(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD, lpHead, packet, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_BODY, lpBody, packet, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD));

	Player* pPlayer = sObjectMgr->FindPlayer(lpHead->entry);

	if ( !pPlayer || !pPlayer->IsPlaying() )
		return;

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleGuard )
		return;

	uint8 buffer[10000];
	POINTER_PCT(CASTLE_SIEGE_GUILD_REGISTER_HEAD, head, buffer, 0);
	POINTER_PCT(CASTLE_SIEGE_GUILD_REGISTER_BODY, body, buffer, sizeof(CASTLE_SIEGE_GUILD_REGISTER_HEAD));
	head->count = 0;
	
	for ( int32 i = 0; i < lpHead->count; ++i )
	{
		if ( Guild* pGuild = sGuildMgr->GetGuild(lpBody[i].guild) )
		{
			body[head->count].is_give_up = 0;
			memcpy(body[head->count].name, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);
			Util::ConvertToByte(lpBody[i].marks, body[head->count].marks);
			body[head->count].position = lpBody[i].register_id;

			++head->count;
		}
	}

	head->h.set(HEADCODE_CASTLE_SIEGE_GUILD_REGISTER_LIST, sizeof(CASTLE_SIEGE_GUILD_REGISTER_HEAD) + (sizeof(CASTLE_SIEGE_GUILD_REGISTER_BODY) * head->count));
	head->result = 1;

	pPlayer->sendPacket(buffer, head->h.get_size());
}

void CCastleSiege::MoneyOutRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !this->CastleOwnerMember(pPlayer) || pPlayer->GuildGetRanking() != GUILD_RANK_MASTER )
		return;

	if ( pPlayer->IsCastleSiegeMoneyRemove() )
		return;

	POINTER_PCT_LOG(CASTLE_SIEGE_MONEY_OUT, lpMsg, Packet, 0);

	int32 money = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->money[3], lpMsg->money[2]), MAKE_NUMBERW(lpMsg->money[1], lpMsg->money[0]));

	if ( money <= 0 )
	{
		this->MoneyOutResult(pPlayer, 0, 0);
		return;
	}

	if ( this->GetTributeMoney() < money )
	{
		this->MoneyOutResult(pPlayer, 0, 0);
		return;
	}

	if ( pPlayer->MoneyReachMaximum(money) )
	{
		this->MoneyOutResult(pPlayer, 0, 0);
		return;
	}

	pPlayer->SetCastleSiegeMoneyRemove(true);

	this->UpdateDBMoney(money, 2, pPlayer);
}

void CCastleSiege::MoneyOutResult(Player* pPlayer, uint8 result, int64 money)
{
	CASTLE_SIEGE_MONEY_OUT_RESULT pMsg(result);
	Util::ConvertToByte(money, pMsg.money);
	pPlayer->SEND_PCT(pMsg);
}

void CCastleSiege::MachineUseRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !sGameServer->IsCastleSiegeEnabled() || !sGameServer->IsCastleSiegeMachineEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleMachine )
	{
		return;
	}

	POINTER_PCT_LOG(CASTLE_SIEGE_MACHINE_USE_REQUEST, lpMsg, Packet, 0);

	uint8 target_id = lpMsg->target - 1;

	Monster* pMonster = sObjectMgr->FindMonster(lpMsg->GetNpc());

	if ( !pMonster || !pMonster->IsPlaying() || !pMonster->IsLive() || target_id >= CASTLE_SIEGE_MACHINE_ATTACK_POINTS )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetTarget() != pMonster )
	{
		return;
	}

	MonsterAI *pAI = pMonster->AIGroup(CS_CASTLE_MACHINE_AI);

	if ( !pAI )
	{
		return;
	}

	CastleMachineBasicAI * pMachineAI = AI_TO(CastleMachineBasicAI, pAI);

	if ( pMachineAI->GetPlayer() != pPlayer )
		return;

	if ( pMachineAI->IsActive() )
		return;

	CASTLE_SIEGE_MACHINE_USE_RESULT pMsg(1, pMonster->GetEntry(), pMachineAI->GetType(), 0, 0);

	int16 hit_x = 0;
	int16 hit_y = 0;

	this->MachineUseGetCoord(pMachineAI->GetType(), target_id, hit_x, hit_y);

	pMsg.x = hit_x;
	pMsg.y = hit_y;

	pPlayer->SEND_PCT(pMsg);

	pMsg.result = 2;

	Object * pObject = nullptr;

	VIEWPORT_LOOP_OBJECT(pMonster, pObject)

		if ( !pObject->IsPlayer() || !pObject->IsLive() || pObject == pPlayer )
			continue;

		pObject->SEND_PCT(pMsg);

	VIEWPORT_CLOSE

	CASTLE_SIEGE_MACHINE_NOTIFY_REGION pNotifyRegionMsg(pMachineAI->GetType(), hit_x, hit_y);
	//CASTLE_SIEGE_MACHINE_NOTIFY_TARGET pNotifyTargetMsg(-1, pMachineAI->GetType());

	/*uint8 buffer[10000];
	POINTER_PCT(VIEWPORT_CREATE_HEAD_C1, head, buffer, 0);
	POINTER_PCT(CASTLE_SIEGE_MACHINE_NOTIFY_BODY, body, buffer, sizeof(VIEWPORT_CREATE_HEAD_C1));
	uint16 size = sizeof(VIEWPORT_CREATE_HEAD_C1);
	head->count = 0;*/

	HASH_UNIT(i)
	{
		Unit* pUnit = ObjectContainer[i]->ToUnit();

		if ( !pUnit || !pUnit->IsPlaying() || !pUnit->IsLive() )
			continue;

		if ( !pUnit->SameDimension(pPlayer) )
			continue;

		if ( Util::Distance(pUnit->GetX(), pUnit->GetY(), hit_x, hit_y) > 6 )
			continue;

		//Object::ViewportConstructCastleMachine(pPlayer, pUnit, buffer, size);
		//head->count++;

		pUnit->SEND_PCT(pNotifyRegionMsg);

		if ( Util::Distance(pUnit->GetX(), pUnit->GetY(), hit_x, hit_y) > 3 )
			continue;

		/*if ( pUnit->IsPlayer() )
			pNotifyTargetMsg.SetTarget(pUnit->GetEntry());*/

		//pPlayer->SEND_PCT(pMsg3);
	}

	//head->h.set(HEADCODE_CASTLE_MACHINE_VIEWPORT, size);
	//pPlayer->sendPacket(buffer, size);

	pMachineAI->Start(hit_x, hit_y);
}

void CCastleSiege::MachineUseGetCoord(uint8 type, uint8 target, int16 &x, int16 &y)
{
	CastleSiegeWeaponLocation const& location = type == 1 ? g_CastleSiegeWeaponAttackData[target] : g_CastleSiegeWeaponDefenseData[target];

	x = location.x1 + Random(location.x2 - location.x1);
	y = location.y1 + Random(location.y2 - location.y1);

	FIX_COORD(x);
	FIX_COORD(y);
}

void CCastleSiege::MachineDamage(Player* pPlayer, uint8 * Packet)
{
	if ( !sGameServer->IsCastleSiegeEnabled() || !sGameServer->IsCastleSiegeMachineEnabled() )
	{
		return;
	}

	if ( !pPlayer || !pPlayer->IsPlaying() )
	{
		return;
	}

	POINTER_PCT_LOG(CASTLE_SIEGE_MACHINE_DAMAGE, lpMsg, Packet, 0);

	Monster* pMonster = sObjectMgr->FindMonster(lpMsg->GetNpc());

	if ( !pMonster || !pMonster->IsLive() || !pMonster->IsPlaying() )
		return;

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleMachine ||
		 pPlayer->GetInterfaceState()->GetTarget() != pMonster )
		return;

	MonsterAI * pAI = pMonster->AIGroup(CS_CASTLE_MACHINE_AI);

	if ( !pAI )
		return;

	CastleMachineBasicAI * pMachineAI = AI_TO(CastleMachineBasicAI, pAI);

	if ( !pMachineAI->IsActive() || pMachineAI->IsReady() )
		return;

	int16 x = pMachineAI->GetX();
	int16 y = pMachineAI->GetY();

	HASH_UNIT(i)
	{
		Unit* pUnit = ObjectContainer[i]->ToUnit();

		if ( !pUnit || !pUnit->IsPlaying() || !pUnit->IsLive() )
			continue;

		if ( !pUnit->SameDimension(pMonster) || (pUnit->IsPlayer() && pUnit->ToPlayer()->IsAdministrator()) )
			continue;

		if ( Util::Distance(pUnit->GetX(), pUnit->GetY(), x, y) > 3 )
			continue;

		pMachineAI->AddTarget(pUnit);
	}

	pMachineAI->Ready();
}

void CCastleSiege::MiniMapRequest(Player* pPlayer)
{
	if ( !sGameServer->IsCastleSiegeEnabled() )
	{
		return;
	}

	if ( !pPlayer || !pPlayer->IsPlaying() )
	{
		return;
	}

	if ( !pPlayer->GetCastleSiegeJoinSide() )
	{
		this->SendMiniMap(pPlayer, 3);
		return;
	}

	if ( !pPlayer->GuildIsAllianceMaster() )
	{
		this->SendMiniMap(pPlayer, 3);
		return;
	}

	if ( this->GetState() != CASTLE_SIEGE_STATE_START )
	{
		this->SendMiniMap(pPlayer, 2);
		return;
	}

	this->SendMiniMap(pPlayer, 1);
	this->MiniMapAddPlayer(pPlayer);
}

void CCastleSiege::MiniMapClose(Player* pPlayer)
{
	if ( !pPlayer || !pPlayer->IsPlaying() )
		return;

	this->MiniMapDelPlayer(pPlayer);
}

void CCastleSiege::MiniMapAddPlayer(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	this->m_mini_map_request[pPlayer->GetGUID()] = pPlayer;
}

void CCastleSiege::MiniMapDelPlayer(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	CastleSiegeMiniMapRequestMap::iterator it = this->m_mini_map_request.find(pPlayer->GetGUID());

	if ( it != this->m_mini_map_request.end() )
	{
		this->m_mini_map_request.erase(it);
	}
}

void CCastleSiege::MiniMapCommandRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !sGameServer->IsCastleSiegeEnabled() )
		return;

	if ( !pPlayer || !pPlayer->IsPlaying() )
		return;

	if ( !pPlayer->GetCastleSiegeJoinSide() )
		return;

	if ( !pPlayer->GuildIsAllianceMaster() )
		return;

	if ( this->GetState() != CASTLE_SIEGE_STATE_START )
		return;

	POINTER_PCT_LOG(CASTLE_SIEGE_COMMAND_REQUEST, lpMsg, Packet, 0);

	CASTLE_SIEGE_COMMAND_RESULT pMsg(lpMsg->team, lpMsg->x, lpMsg->y, lpMsg->command);

	World* pWorld = sWorldMgr->GetWorld(WORLD_CASTLE_SIEGE);

	if ( !pWorld )
		return;

	Player* pUnit = nullptr;

	WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
	{
		pUnit = it->second;

		if ( !pUnit )
			continue;

		if ( pUnit->GetCastleSiegeJoinSide() != pPlayer->GetCastleSiegeJoinSide() )
			continue;

		pUnit->SEND_PCT(pMsg);
	}
}

uint8 buffer1[8192];
uint8 buffer2[8192];

void CCastleSiege::MiniMapUpdate()
{
	if ( !this->IsListLoaded() )
	{
		return;
	}

	MAP_CLEAR(CastleSiegeMiniMapDataMap::iterator, this->m_mini_map_data);

	POINTER_PCT(CASTLE_SIEGE_MINI_MAP_HEAD, head1, buffer1, 0);
	POINTER_PCT(CASTLE_SIEGE_MINI_MAP_BODY, body1, buffer1, sizeof(CASTLE_SIEGE_MINI_MAP_HEAD));

	POINTER_PCT(CASTLE_SIEGE_MINI_MAP_NPC_HEAD, head2, buffer2, 0);
	POINTER_PCT(CASTLE_SIEGE_MINI_MAP_NPC_BODY, body2, buffer2, sizeof(CASTLE_SIEGE_MINI_MAP_NPC_HEAD));
	head2->count = 0;
	
	for ( CastleSiegeGuildDataMap::const_iterator it = this->m_guild_data.begin(); it != this->m_guild_data.end(); ++it )
	{
		this->m_mini_map_data[it->second->GetGuild()] = new CastleSiegeMiniMapData(it->first);
	}

	if ( this->m_mini_map_data.empty() )
		return;

	Object* pObject = nullptr;
	Monster* pMonster = nullptr;
	
	HASH_SUMMON(i)
	{
		pObject = ObjectContainer[i];

		if ( !pObject )
			continue;

		pMonster = pObject->ToCreature();

		if ( !pMonster )
			continue;

		if ( !pMonster->IsPlaying() || !pMonster->IsLive() )
			continue;

		if ( pMonster->AIGroup(CS_GUARDIAN_STATUE_AI) || pMonster->AIGroup(CS_GATE_AI) )
		{
			body2[head2->count].npc = pMonster->AIGroup(CS_GUARDIAN_STATUE_AI) ? 1: 0;
			body2[head2->count].x = pMonster->GetX();
			body2[head2->count].y = pMonster->GetY();
			++head2->count;
		}
	}

	head2->h.set(HEADCODE_CASTLE_SIEGE_MINI_MAP, sizeof(CASTLE_SIEGE_MINI_MAP_NPC_HEAD) + sizeof(CASTLE_SIEGE_MINI_MAP_NPC_BODY) * head2->count);

	World* pWorld = sWorldMgr->GetWorld(WORLD_CASTLE_SIEGE);

	if ( pWorld )
	{
		Player* pPlayer = nullptr;

		WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
		for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
		{
			pPlayer = it->second;

			if ( !pPlayer )
				continue;

			if ( pPlayer->GetCastleSiegeJoinSide() == 0 )
				continue;

			CastleSiegeMiniMapDataMap::iterator itr = this->m_mini_map_data.find(pPlayer->GetGuildID());

			if ( itr == this->m_mini_map_data.end() )
				continue;
		
			if ( itr->second->points_count >= CASTLE_SIEGE_MINI_MAP_POINT_MAX )
				continue;

			if ( pPlayer->GuildGetRanking() == GUILD_RANK_MASTER && pPlayer->GetGuildID() == itr->second->guild )
			{
				itr->second->pGuildMaster = pPlayer;
				continue;
			}

			itr->second->points[itr->second->points_count].x = pPlayer->GetX();
			itr->second->points[itr->second->points_count].y = pPlayer->GetY();
			++itr->second->points_count;
		}
	}

	for ( CastleSiegeMiniMapDataMap::const_iterator it = this->m_mini_map_data.begin(); it != this->m_mini_map_data.end(); ++it )
	{
		head1->count = 0;

		if ( !it->second->pGuildMaster || !it->second->pGuildMaster->IsPlaying() )
			continue;

		head1->count = it->second->points_count;

		for ( int32 i = 0; i < head1->count; ++i )
		{
			body1[i].x = it->second->points[i].x;
			body1[i].y = it->second->points[i].y;
		}

		head1->h.set(HEADCODE_CASTLE_SIEGE_MINI_MAP_SINGLE, sizeof(CASTLE_SIEGE_MINI_MAP_HEAD) + sizeof(CASTLE_SIEGE_MINI_MAP_BODY) * head1->count);

		it->second->pGuildMaster->sendPacket(buffer1, head1->h.get_size());
		it->second->pGuildMaster->sendPacket(buffer2, head2->h.get_size());
	}
}

void CCastleSiege::UpdateStatus(Player* pPlayer)
{
	if ( pPlayer->GetWorldId() == WORLD_CASTLE_SIEGE )
	{
		this->SendStartState(pPlayer, this->GetState() == CASTLE_SIEGE_STATE_START);

		if ( this->GetState() == CASTLE_SIEGE_STATE_START )
		{
			this->SendJoinSide(pPlayer);
			this->SendLeftTime(pPlayer);
		}
	}

	pPlayer->KillCountSend();
}

bool CCastleSiege::CheckTeleportMagicAxisY(int16 start_y, int16 target_x, int16 target_y) const
{
	for( int32 iSTEP = 0; iSTEP < 3; ++iSTEP )
	{
		if( (g_CastleSiegeWallStepAxisY[iSTEP] >= start_y) && (g_CastleSiegeWallStepAxisY[iSTEP] <= target_y) && (target_x < 144) )
		{
			return false;
		}
	}

	return true;
}

void CCastleSiege::InsertRanking()
{
	Guild* pGuild = sGuildMgr->GetGuild(this->GetCastleOwner());

	sGuildMgr->AddScore(pGuild, sGameServer->GetGuildScoreCastleSiege());
	sGuildMgr->AddAllianceScore(this->GetCastleOwner(), sGameServer->GetGuildScoreCastleSiegeMembers());
	
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_CASTLE_SIEGE);
	stmt->setUInt32(0, this->GetCastleOwner());
	stmt->setString(1, pGuild ? pGuild->GetName() : "");
	stmt->setInt32(2, this->GetCharacterCount());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void CCastleSiege::UpdateCharacterCount()
{
	int32 count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for (PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		if (pPlayer->GetWorldId() != WORLD_CASTLE_SIEGE)
		{
			continue;
		}

		if (!pPlayer->GetGuildID())
		{
			continue;
		}

		for (CastleSiegeGuildDataMap::iterator it = this->m_guild_data.begin(); it != this->m_guild_data.end(); ++it)
		{
			CastleSiegeGuildFinal const* pGuild = it->second;

			if (!pGuild)
			{
				continue;
			}

			if (pGuild->GetGuild() == pPlayer->GetGuildID())
			{
				++count;

				CastleSiegeParticipantMap::const_iterator itr2 = this->m_CastleSiegeParticipantMap.find(pPlayer->GetGUID());

				if (itr2 == this->m_CastleSiegeParticipantMap.end())
				{
					this->m_CastleSiegeParticipantMap[pPlayer->GetGUID()] = new CastleSiegeParticipant(pPlayer->GetGUID());
				}
			}
		}
	}

	if (this->GetCharacterCount() < count)
	{
		this->SetCharacterCount(count);
	}
}

void CCastleSiege::UpdateParticipants()
{
	for (CastleSiegeParticipantMap::const_iterator itr = this->m_CastleSiegeParticipantMap.begin(); itr != this->m_CastleSiegeParticipantMap.end(); ++itr)
	{
		CastleSiegeParticipant* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		Player* pPlayer = sObjectMgr->FindPlayerByGUID(pData->GetID());

		if (!pPlayer || !pPlayer->IsPlaying())
		{
			continue;
		}

		if (pPlayer->GetWorldId() != WORLD_CASTLE_SIEGE)
		{
			continue;
		}

		pData->IncreaseSeconds(5);
	}
}

void CCastleSiege::GiveParticipantsRewards()
{
	for (CastleSiegeParticipantMap::const_iterator itr = this->m_CastleSiegeParticipantMap.begin(); itr != this->m_CastleSiegeParticipantMap.end(); ++itr)
	{
		CastleSiegeParticipant* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetSeconds() < sGameServer->GetCastleSiegeParticipantRewardMinutes())
		{
			continue;
		}

		Player* pPlayer = sObjectMgr->FindPlayerByGUID(pData->GetID());

		if (Player::IsPlayerBasicState(pPlayer, false, false))
		{
			sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(ITEMGET(14, 478)), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
		}
		else
		{
			SQLTransaction trans = MuDatabase.BeginTransaction();

			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_INSERT);
			stmt->setUInt32(0, pData->GetID());
			stmt->setUInt16(1, ITEMGET(14, 478));
			stmt->setUInt8(2, 0);
			stmt->setUInt8(3, 0);
			stmt->setUInt8(4, 0);
			stmt->setUInt8(5, 0);
			stmt->setUInt8(6, 0);
			stmt->setUInt8(7, 0);
			stmt->setUInt8(8, 0);
			stmt->setUInt8(9, 0);
			stmt->setUInt8(10, 0);
			stmt->setUInt16(11, SOCKET_SLOT_NONE);
			stmt->setUInt16(12, SOCKET_SLOT_NONE);
			stmt->setUInt16(13, SOCKET_SLOT_NONE);
			stmt->setUInt16(14, SOCKET_SLOT_NONE);
			stmt->setUInt16(15, SOCKET_SLOT_NONE);
			stmt->setUInt8(16, -1);
			stmt->setUInt32(17, 0);

			trans->Append(stmt);

			MuDatabase.CommitTransaction(trans);
		}
	}
}