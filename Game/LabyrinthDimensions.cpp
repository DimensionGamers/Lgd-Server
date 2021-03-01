LabyrinthDimensions::LabyrinthDimensions()
{
	this->SetLeagueState(LABYRINTH_OF_DIMENSIONS_LEAGUE_STATE_MAX);
}

LabyrinthDimensions::~LabyrinthDimensions()
{
	MAP_CLEAR(LabyrinthInfoMap::iterator, this->labyrinth_info_map);
	LIST_CLEAR(LabyrinthMissionList::iterator, this->labyrinth_mission_list);

	for ( LabyrinthAreaMap::iterator itr = this->labyrinth_area_map.begin(); itr != this->labyrinth_area_map.end(); ++itr )
	{
		LIST_CLEAR(LabyrinthMapSettingDataList::iterator, itr->second->data_list);
	}
	MAP_CLEAR(LabyrinthAreaMap::iterator, this->labyrinth_area_map);
	MAP_CLEAR(LabyrinthLevelMap::iterator, this->m_LabyrinthLevelMap);
	MAP_CLEAR(LabyrinthMonsterMap::iterator, this->m_LabyrinthMonsterMap);
	MAP_CLEAR(LabyrinthRankingPlayerMap::iterator, this->m_LabyrinthRankingPlayerMap);
	LIST_CLEAR(LabyrinthRewardList::iterator, this->m_LabyrinthRewardList);
}

void LabyrinthDimensions::LoadInfo()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Info...");

	MAP_CLEAR(LabyrinthInfoMap::iterator, this->labyrinth_info_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_labyrinth_info");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			LabyrinthInfo * pData = new LabyrinthInfo;
			pData->SetIndex(fields[loop++].GetUInt8());
			pData->SetWorld(fields[loop++].GetUInt16());
			pData->SetX1(fields[loop++].GetInt16());
			pData->SetY1(fields[loop++].GetInt16());
			pData->SetX2(fields[loop++].GetInt16());
			pData->SetY2(fields[loop++].GetInt16());

			this->labyrinth_info_map[pData->GetIndex()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions info definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void LabyrinthDimensions::LoadMission()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Mission...");

	LIST_CLEAR(LabyrinthMissionList::iterator, this->labyrinth_mission_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_labyrinth_mission");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			LabyrinthMission * pData = new LabyrinthMission;
			pData->SetAreaType(fields[loop++].GetUInt8());
			pData->SetIndex(fields[loop++].GetUInt8());

			for (int32 i = 0; i < LABYRINTH_MISSION_MAX; ++i)
			{
				pData->GetMission(i)->SetType(fields[loop++].GetUInt8());
				pData->GetMission(i)->SetValue(fields[loop++].GetInt32());
			}

			for (int32 i = 0; i < LABYRINTH_MISSION_MONSTER_MAX; ++i)
			{
				pData->GetMonster(i)->SetMonster(fields[loop++].GetUInt16());
				pData->GetMonster(i)->SetCount(fields[loop++].GetInt32());
			}

			this->labyrinth_mission_list.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions mission definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void LabyrinthDimensions::LoadArea()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Area...");

	MAP_CLEAR(LabyrinthAreaMap::iterator, this->labyrinth_area_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_labyrinth_area");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			LabyrinthArea * pData = new LabyrinthArea;
			pData->SetIndex(fields[loop++].GetUInt8());
			pData->SetStage(fields[loop++].GetUInt8());
			pData->SetRowCount(fields[loop++].GetUInt8());
			pData->SetColumnCount(fields[loop++].GetUInt8());
			pData->SetEntrance(nullptr);
			pData->SetExit(nullptr);

			this->labyrinth_area_map[pData->GetIndex()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions area definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void LabyrinthDimensions::LoadAreaData()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Area Data...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_labyrinth_area_data");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			LabyrinthAreaMap::iterator it = this->labyrinth_area_map.find(fields[loop++].GetUInt8());

			if (it != this->labyrinth_area_map.end())
			{
				LabyrinthAreaData * pData = new LabyrinthAreaData;
				//pData->SetIndex(fields[loop++].GetUInt8());
				//pData->SetSubIndex(fields[loop++].GetUInt8());
				pData->SetSubIndex(fields[loop++].GetUInt8());
				pData->SetIndex(fields[loop++].GetUInt8());
				pData->SetWorldIndex(fields[loop++].GetUInt8());
				pData->SetAreaType(fields[loop++].GetUInt8());
				pData->SetMissionID(fields[loop++].GetUInt8());

				for (int32 i = 0; i < LABYRINTH_GATE_MAX; ++i)
				{
					pData->SetGate(i, fields[loop++].GetUInt8());
				}

				it->second->data_list.push_back(pData);
				count++;
			}
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions area data definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");

	for (LabyrinthAreaMap::iterator itr = this->labyrinth_area_map.begin(); itr != this->labyrinth_area_map.end();)
	{
		if (itr->second->data_list.empty() || itr->second->data_list.size() < 2)
		{
			sLog->outError("root", "%s - ID:%u - Lest than 2", __FUNCTION__, itr->first);

			delete itr->second;
			this->labyrinth_area_map.erase(itr++);
			continue;
		}

		int32 entrance = 0;
		int32 exit = 0;

		for (LabyrinthMapSettingDataList::const_iterator it = itr->second->data_list.begin(); it != itr->second->data_list.end(); ++it)
		{
			if ((*it)->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_ENTRANCE)
			{
				itr->second->SetEntrance(*it);
				++entrance;
			}
			if ((*it)->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
			{
				itr->second->SetExit(*it);
				++exit;
			}
		}

		if (entrance != 1 || exit != 1 || !itr->second->GetEntrance() || !itr->second->GetExit())
		{
			sLog->outError("root", "%s - ID:%u - Not Entrance(%d) or Exit(%d)", __FUNCTION__, itr->first, entrance, exit);

			delete itr->second;
			this->labyrinth_area_map.erase(itr++);
			continue;
		}

		++itr;
	}

	for (LabyrinthAreaMap::iterator itr = this->labyrinth_area_map.begin(); itr != this->labyrinth_area_map.end(); ++itr)
	{
		for (LabyrinthMapSettingDataList::const_iterator it = itr->second->data_list.begin(); it != itr->second->data_list.end(); ++it)
		{
			for (int32 i = 0; i < LABYRINTH_GATE_MAX; ++i)
			{
				if ((*it)->GetGate(i) != LABYRINTH_OF_DIMENSIONS_GATE_STATE_NONE)
				{
					if ((*it)->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_ENTRANCE || (*it)->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_NORMAL)
					{
						(*it)->SetGate(i, LABYRINTH_OF_DIMENSIONS_GATE_STATE_OPEN);
					}
					else
					{
						(*it)->SetGate(i, LABYRINTH_OF_DIMENSIONS_GATE_STATE_CLOSED);
					}
				}
			}
		}
	}
}

void LabyrinthDimensions::LoadLevel()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Level...");

	MAP_CLEAR(LabyrinthLevelMap::iterator, this->m_LabyrinthLevelMap);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_labyrinth_level");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			LabyrinthLevel * pData = new LabyrinthLevel;
			pData->SetID(fields[loop++].GetUInt16());
			pData->SetLevel(fields[loop++].GetInt32());
			pData->SetFirstScore(fields[loop++].GetInt32());
			pData->SetScore(fields[loop++].GetInt32());
			
			this->m_LabyrinthLevelMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void LabyrinthDimensions::LoadMonster()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Monster...");

	MAP_CLEAR(LabyrinthMonsterMap::iterator, this->m_LabyrinthMonsterMap);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_labyrinth_monster");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			LabyrinthMonster * pData = new LabyrinthMonster;
			pData->SetID(fields[loop++].GetUInt16());
			pData->SetLevel(fields[loop++].GetFloat());
			pData->SetHP(fields[loop++].GetFloat());
			pData->SetDamage(fields[loop++].GetFloat());
			pData->SetDefense(fields[loop++].GetFloat());
			pData->SetAttackSuccessRate(fields[loop++].GetFloat());
			pData->SetDefenseSuccessRate(fields[loop++].GetFloat());
			pData->SetElementalDamage(fields[loop++].GetFloat());
			pData->SetElementalDefense(fields[loop++].GetFloat());
			pData->SetElementalAttackSuccessRate(fields[loop++].GetFloat());
			pData->SetElementalDefenseSuccessRate(fields[loop++].GetFloat());

			this->m_LabyrinthMonsterMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions monster definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void LabyrinthDimensions::LoadReward()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Reward...");

	LIST_CLEAR(LabyrinthRewardList::iterator, this->m_LabyrinthRewardList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_labyrinth_reward");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			LabyrinthReward * pData = new LabyrinthReward;
			pData->SetCategory(fields[0].GetUInt8());
			pData->SetRank(fields[1].GetUInt8());
			pData->SetClass(fields[2].GetUInt8());
			pData->SetItem(ITEMGET(fields[3].GetUInt8(), fields[4].GetUInt16()));
			pData->SetLevel(fields[5].GetUInt8());
			pData->SetDurability(fields[6].GetUInt8());
			pData->SetSkill(fields[7].GetUInt8());
			pData->SetLuck(fields[8].GetUInt8());
			pData->SetOption(fields[9].GetUInt8());
			pData->SetExcellent(fields[10].GetUInt8());
			pData->SetAncient(fields[11].GetUInt8());
			pData->SetDBFlags(fields[12].GetUInt32());

			if (!sItemMgr->GetItem(pData->GetItem()))
			{
				delete pData;
				continue;
			}

			this->m_LabyrinthRewardList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void LabyrinthDimensions::LoadRanking()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Ranking...");

	MAP_CLEAR(LabyrinthRankingPlayerMap::iterator, this->m_LabyrinthRankingPlayerMap);

	uint32 count = 0;

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_SELECT);

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			LabyrinthRankingPlayer* pData = new LabyrinthRankingPlayer();
			pData->SetCategory(fields[0].GetUInt8());
			pData->SetID(fields[1].GetUInt32());
			pData->SetName(fields[2].GetCString());
			pData->SetClass(fields[3].GetUInt8());
			pData->SetScore(fields[4].GetInt32());
			pData->SetLeagueScore(fields[5].GetInt32());
			pData->SetTime(fields[6].GetInt64());

			this->m_LabyrinthRankingPlayerMap[pData->GetID()] = pData;
			
			count++;
		} while (result->NextRow());
	}

	for (int32 i = 0; i < MAX_LABYRINTH_OF_DIMENSIONS_RANK; ++i)
	{
		this->UpdateRankingSimple(i);
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions ranking definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void LabyrinthDimensions::LoadSchedule()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Labyrinth of Dimensions Schedule...");

	uint32 count = 0;

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_SCHEDULE_SELECT);

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			
			this->SetLeagueState(fields[0].GetUInt8());
			this->SetLeagueTime(fields[1].GetInt64());
			count++;
		} while (result->NextRow());
	}

	this->UpdateState();

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u labyrinth of dimensions schedule definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool LabyrinthDimensions::EnterAllowed(Player* pPlayer)
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		pPlayer->SendMessageBox(1, "Labyrinth of Dimensions", "Event disabled.");
		return false;
	}

	if (this->GetLeagueTime() > time(nullptr))
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG DATE", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());
		return false;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_SAFE)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- NOT IN SAFE ZONE", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());

		return false;
	}

	if (sGameServer->IsLabyrinthOfDimensionsRequiredMaster() && !pPlayer->IsMaster())
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- NOT MASTER", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());

		this->EnterResult(pPlayer, 3, 0);
		return false;
	}

	if (pPlayer->GetTotalLevel() < sGameServer->GetLabyrinthOfDimensionsRequiredLevel())
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- NOT REQUIRED LEVEL", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());

		pPlayer->SendMessageBox(1, "Labyrinth of Dimensions", "Required level to enter : %u", sGameServer->GetLabyrinthOfDimensionsRequiredLevel());
		return false;
	}

	if (pPlayer->GetPartyID() != PARTY_NULL)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- IN PARTY", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());

		this->EnterResult(pPlayer, 4, 0);
		return false;
	}

	if (pPlayer->GetPersonalStore()->IsBusy())
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- PERSONAL STORE OPEN", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());

		this->EnterResult(pPlayer, 7, 0);
		return false;
	}

	if (!Player::IsPlayerTransactionFree(pPlayer))
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- IN TRANSACTION %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), pPlayer->GetInterfaceState()->GetID().c_str());

		pPlayer->SendMessageBox(1, "Labyrinth of Dimensions", "Can't enter while transaction is active.");
		return false;
	}

	if (pPlayer->GetPKLevel() > PK_STATUS_COMMONER)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- PK :%u", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), pPlayer->GetPKLevel());

		this->EnterResult(pPlayer, 8, 0);
		return false;
	}

	if (pPlayer->GetRegenStatus() != REGEN_NONE)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- REGEN STATUS: %u", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), pPlayer->GetRegenStatus());

		return false;
	}

	if (pPlayer->ItemFind(ITEMGET(14, 430), 0, false) == uint8(-1))
	{
		return false;
	}

	/*
	0 = Entering
	1 = Previous
	2 = Wait time
	5 = Maximum admission is not possible. Come back later
	6 = Stamina is not enought, come back after 10 or more
	7 = Cannot enter while personal store is open
	*/

	return true;
}

void LabyrinthDimensions::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer))
	{
		return;
	}

	if (!this->EnterAllowed(pPlayer))
	{
		return;
	}

	POINTER_PCT_CHECK(LABYRINTH_OF_DIMENSIONS_ENTER_REQUEST, lpMsg, Packet, 0);

	if (lpMsg->level <= 0 || lpMsg->level > MAX_LABYRINTH_LEVEL)
	{
		return;
	}

	LabyrinthRankingPlayer* pData = this->FindPlayer(pPlayer->GetGUID());

	if (pData && pData->GetTime() == -1)
	{
		pPlayer->GetLabyrinthData()->Init();
		pData->SetTime(0);
	}

	if (!pData && pPlayer->GetLabyrinthData()->IsAnyLevelComplete())
	{
		pPlayer->GetLabyrinthData()->Init();
	}

	if (pPlayer->GetDate(PLAYER_DATE_LABYRINTH) == 0)
	{
		pPlayer->GetLabyrinthData()->Init();
		pPlayer->SetDate(PLAYER_DATE_LABYRINTH, time(nullptr));
	}

	this->EnterResult(pPlayer, pPlayer->GetLabyrinthData()->GetLevelCount(lpMsg->level) > 0 ? 1 : 0, 0);

	sLog->outInfo("labyrinthdimension", "%s -- %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());
}

void LabyrinthDimensions::EnterResult(Player* pPlayer, uint8 result, uint32 tick_count)
{
	if (!pPlayer)
	{
		return;
	}

	LABYRINTH_OF_DIMENSIONS_ENTER_RESULT pMsg(result);
	pMsg.tick_count = tick_count;
	pMsg.status = result == 0 || result == 1 ? 1 : 0;
	pPlayer->SendPacket(&pMsg);
}

void LabyrinthDimensions::EnterConfirm(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	if (!this->EnterAllowed(pPlayer))
	{
		return;
	}

	POINTER_PCT_CHECK(LABYRINTH_OF_DIMENSIONS_ENTER_CONFIRM_REQUEST, lpMsg, Packet, 0);

	if (lpMsg->level <= 0 || lpMsg->level > MAX_LABYRINTH_LEVEL)
	{
		return;
	}

	if (pPlayer->GetLabyrinthData()->GetLevel() == lpMsg->level && pPlayer->GetLabyrinthData()->GetStatus() == 1)
	{
		pPlayer->GetLabyrinthData()->InitData();
	}
	
	if (pPlayer->GetLabyrinthData()->GetLevel() != lpMsg->level || pPlayer->GetLabyrinthData()->GetStage() == 0)
	{
		pPlayer->GetLabyrinthData()->InitData();

		pPlayer->GetLabyrinthData()->SetStage(1);
		pPlayer->GetLabyrinthData()->SetLevel(lpMsg->level);
		pPlayer->GetLabyrinthData()->SetDate(time(nullptr));
	}

	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetStage());

	if (!pLabyrinthArea)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG LABYRINTH AREA [%u / %u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID());

		this->EnterResult(pPlayer, 9, 0);
		return;
	}

	pPlayer->GetLabyrinthData()->InitTime();

	pPlayer->ItemFind(ITEMGET(14, 430), 0, true);

	this->SetInitialGate(pPlayer, pLabyrinthArea->GetIndex());

	pPlayer->GetLabyrinthData()->SetID(pLabyrinthArea->GetIndex());

	if (LabyrinthAreaData const* pEntrance = pLabyrinthArea->GetEntrance())
	{
		this->TeleportToZone(pPlayer, pEntrance->GetIndex(), pEntrance->GetSubIndex());

		this->SendStage(pPlayer);

		sLog->outInfo("labyrinthdimension", "%s -- %s -- %s -- ENTER ENTRANCE [%u / %u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID());

		if (pPlayer->IsAdministrator())
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "ID: %u - Index: %u - SubIndex: %u", pPlayer->GetLabyrinthData()->GetID(), pEntrance->GetIndex(), pEntrance->GetSubIndex());
		}
	}
}

void LabyrinthDimensions::SendZoneInfo(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetStage());

	if (!pLabyrinthArea)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG LABYRINTH AREA [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG LABYRINTH AREA DATA [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	LABYRINTH_OF_DIMENSIONS_ZONE_INFO pMsg;
	pMsg.world = pPlayer->GetWorldId();
	pMsg.x = pPlayer->GetX();
	pMsg.y = pPlayer->GetY();
	pMsg.row_count = pLabyrinthArea->GetRowCount();
	pMsg.column_count = pLabyrinthArea->GetColumnCount();
	pMsg.index = pLabyrinthAreaData->GetIndex();
	pMsg.sub_index = pLabyrinthAreaData->GetSubIndex();
	pMsg.texture = pLabyrinthAreaData->GetWorldIndex();
	pMsg.type = pLabyrinthAreaData->GetAreaType();

	for (int32 i = 0; i < LABYRINTH_GATE_MAX; ++i)
	{
		pMsg.gate[i] = pLabyrinthAreaData->GetGate(i);

		if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT && pMsg.gate[i] == LABYRINTH_OF_DIMENSIONS_GATE_STATE_CLOSED)
		{
			pMsg.gate[i] = LABYRINTH_OF_DIMENSIONS_GATE_STATE_OPEN;
		}
	}

	pPlayer->SendPacket(&pMsg);

	pPlayer->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);

	if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		this->SendFinalMissionList(pPlayer);
	}
	else
	{
		this->SendMission(pPlayer);
	}

	this->SendGateList(pPlayer);
}

void LabyrinthDimensions::GateEnter(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	POINTER_PCT_CHECK(LABYRINTH_OF_DIMENSIONS_GATE_ENTER, lpMsg, Packet, 0);

	if (lpMsg->id >= LABYRINTH_GATE_MAX)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG GATE ID %u [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), lpMsg->id,
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	if (pPlayer->GetLabyrinthData()->GetID() == uint8(-1))
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG ID [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetStage());

	if (!pLabyrinthArea)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG LABYRINTH AREA [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG LABYRINTH AREA DATA [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	/*if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- ON EXIT [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}*/

	if (pLabyrinthAreaData->GetGate(lpMsg->id) == LABYRINTH_OF_DIMENSIONS_GATE_STATE_NONE)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- NO GATE %u [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), lpMsg->id,
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT && pLabyrinthAreaData->GetGate(lpMsg->id) == LABYRINTH_OF_DIMENSIONS_GATE_STATE_CLOSED && !this->IsMissionCompleted(pPlayer))
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- GATE CLOSED %u [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), lpMsg->id,
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	LabyrinthAreaData const* pNextLabyrinthAreaData = nullptr;

	switch (lpMsg->id)
	{
	case LABYRINTH_GATE_UP:
		{
			if (pLabyrinthAreaData->GetSubIndex() > 0)
			{
				pNextLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pLabyrinthAreaData->GetIndex(), pLabyrinthAreaData->GetSubIndex() - 1);
			}
		} break;

	case LABYRINTH_GATE_DOWN:
		{
			pNextLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pLabyrinthAreaData->GetIndex(), pLabyrinthAreaData->GetSubIndex() + 1);
		} break;

	case LABYRINTH_GATE_LEFT:
		{
			if (pLabyrinthAreaData->GetIndex() > 0)
			{
				pNextLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pLabyrinthAreaData->GetIndex() - 1, pLabyrinthAreaData->GetSubIndex());
			}
		} break;

	case LABYRINTH_GATE_RIGHT:
		{
			pNextLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pLabyrinthAreaData->GetIndex() + 1, pLabyrinthAreaData->GetSubIndex());
		} break;
	}

	if (!pNextLabyrinthAreaData)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG GATE DATA %u [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), lpMsg->id,
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	if (pPlayer->IsAdministrator())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "ID: %u - Index: %u - SubIndex: %u", pPlayer->GetLabyrinthData()->GetID(), pNextLabyrinthAreaData->GetIndex(), pNextLabyrinthAreaData->GetSubIndex());
	}

	this->AddGate(pPlayer, pPlayer->GetLabyrinthData()->GetID(), pNextLabyrinthAreaData->GetIndex(), pNextLabyrinthAreaData->GetSubIndex(), true);

	if (pNextLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		if ((pPlayer->GetLabyrinthData()->GetGoblinState() == 1) || ((pPlayer->GetLabyrinthData()->GetGateCount() - 1) <= sGameServer->GetLabyrinthOfDimensionsGoblinStage() && pPlayer->GetLabyrinthData()->GetGoblinState() == 0))
		{
			this->CreateGoblin(pPlayer, pNextLabyrinthAreaData);
		}
	}

	this->TeleportToZone(pPlayer, pNextLabyrinthAreaData->GetIndex(), pNextLabyrinthAreaData->GetSubIndex());

	if (pPlayer->GetLabyrinthData()->IsUsingTimer())
	{
		this->SendTimer(pPlayer);
	}

	sLog->outInfo("labyrinthdimension", "%s -- %s -- %s -- ENTER GATE %u/%u/%u [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
		lpMsg->id, pNextLabyrinthAreaData->GetIndex(), pNextLabyrinthAreaData->GetSubIndex(),
		pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());
}

void LabyrinthDimensions::LeagueInfo(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer))
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_SAFE)
	{
		return;
	}

	LabyrinthRankingPlayer const* pRankingData = this->FindPlayer(pPlayer->GetGUID());

	LABYRINTH_OF_DIMENSIONS_ENTRANCE_HEAD pMsg;

	if (pRankingData)
	{
		pMsg.rank = pRankingData->GetRank() + 1;
		pMsg.category = pRankingData->GetCategory();

		for (int32 i = 0; i < 5; ++i)
		{
			LabyrinthRankingPlayer const* pData = this->FindPlayer(pRankingData->GetCategory(), i);

			if (!pData)
			{
				continue;
			}

			memcpy(pMsg.data[i].name, pData->GetName(), MAX_CHARACTER_LENGTH);
			pMsg.data[i].points = pData->GetLeagueScore();
			pMsg.data[i].league_points = pData->GetLeagueScore();
			pMsg.data[i].rank = pData->GetRank() + 1;
			pMsg.data[i].race = pData->GetClass();
		}
	}

	pMsg.current_stage = this->GetLeagueState() + 1;
	pMsg.top_category = this->GetLeagueState();

	if (time(nullptr) >= this->GetLeagueTime())
	{
		pMsg.remain_time = (this->GetLeagueTime() + sGameServer->GetLabyrinthOfDimensionsStateDuration()) - time(nullptr);
	}

	pPlayer->SendPacket(&pMsg);
}

void LabyrinthDimensions::Leave(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	if (pPlayer->GetRegenStatus() != REGEN_NONE)
	{
		return;
	}

	if (pPlayer->IsTeleporting())
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- NOT IN BATTLE ZONE [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	if (pPlayer->GetLabyrinthData()->GetID() == uint8(-1))
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG ID [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- WRONG LABYRINTH AREA DATA [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- NOT IN EXIT [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	if (!pPlayer->GetLabyrinthData()->GetStatus())
	{
		sLog->outError("labyrinthdimension", "%s -- %s -- %s -- MISSION NOT COMPLETE [%u / %u / %u:%u]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			pPlayer->GetLabyrinthData()->GetStage(), pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());

		return;
	}

	pPlayer->MoveToGate(LABYRINTH_OF_DIMENSIONS_SAFE_GATE);

	pPlayer->GetLabyrinthData()->InitData();
}

void LabyrinthDimensions::TeleportToZone(Player* pPlayer, uint8 idx, uint8 subidx)
{
	if (!pPlayer)
	{
		return;
	}

	pPlayer->StartRegen(g_LabyrinthOfDimensionsWorldData[0].world, g_LabyrinthOfDimensionsWorldData[0].x, g_LabyrinthOfDimensionsWorldData[0].y, 0, pPlayer->GetGUID());
	pPlayer->GetLabyrinthData()->SetGateIndex(idx);
	pPlayer->GetLabyrinthData()->SetGateSubIndex(subidx);

	this->SendZoneInfo(pPlayer);
}

void LabyrinthDimensions::SendGateList(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetStage());

	if (!pLabyrinthArea)
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(LABYRINTH_OF_DIMENSIONS_GATE_HEAD, head, buffer, 0);
	POINTER_PCT(LABYRINTH_OF_DIMENSIONS_GATE_BODY, body, buffer, sizeof(LABYRINTH_OF_DIMENSIONS_GATE_HEAD));
	head->count = 0;

	for (PlayerLabyrinthGateList::const_iterator it = pPlayer->GetLabyrinthData()->m_GateList.begin(); it != pPlayer->GetLabyrinthData()->m_GateList.end(); ++it)
	{
		PlayerLabyrinthGate const* pData = *it;

		if (!pData)
		{
			continue;
		}

		LabyrinthAreaData const* pLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pData->GetIndex(), pData->GetSubIndex());

		if (!pLabyrinthAreaData)
		{
			continue;
		}

		body[head->count].index = pLabyrinthAreaData->GetSubIndex() * pLabyrinthArea->GetColumnCount() + pLabyrinthAreaData->GetIndex();
		body[head->count].area_type = pLabyrinthAreaData->GetAreaType();

		for (int32 i = 0; i < LABYRINTH_GATE_MAX; ++i)
		{
			body[head->count].gate[i] = 0;
		}

		if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT || this->FoundExit(pPlayer))
		{
			for (int32 i = 0; i < LABYRINTH_GATE_MAX; ++i)
			{
				body[head->count].gate[i] = pLabyrinthAreaData->GetGate(i);
			}

			if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT || this->IsMissionCompleted(pPlayer, pData->GetIndex(), pData->GetSubIndex()))
			{
				for (int32 i = 0; i < LABYRINTH_GATE_MAX; ++i)
				{
					if (body[head->count].gate[i] == LABYRINTH_OF_DIMENSIONS_GATE_STATE_CLOSED)
					{
						body[head->count].gate[i] = LABYRINTH_OF_DIMENSIONS_GATE_STATE_OPEN;
					}
				}
			}
		}

		++head->count;
	}

	head->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_GATE, sizeof(LABYRINTH_OF_DIMENSIONS_GATE_HEAD) + (head->count * sizeof(LABYRINTH_OF_DIMENSIONS_GATE_BODY)));
	pPlayer->sendPacket(buffer);
}

void LabyrinthDimensions::SendMission(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION)
	{
		return;
	}

	PlayerLabyrinthGate const* pPlayerLabyrinthGate = this->GetCurrentGate(pPlayer);

	if (!pPlayerLabyrinthGate)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if (pLabyrinthMission->GetMission(mission_id)->GetType() != uint8(-1))
		{
			LABYRINTH_OF_DIMENSIONS_MISSION pMsg;
			pMsg.type = pLabyrinthMission->GetMission(mission_id)->GetType();
			
			if (pPlayerLabyrinthGate->GetMissionCount(i) >= pLabyrinthMission->GetMission(mission_id)->GetValue())
			{
				pMsg.state = LABYRINTH_OF_DIMENSIONS_MISSION_STATE_COMPLETE;
			}
			else
			{
				pMsg.state = LABYRINTH_OF_DIMENSIONS_MISSION_STATE_ACTIVE;
			}

			pMsg.current_count = pPlayerLabyrinthGate->GetMissionCount(i);
			pMsg.total_count = pLabyrinthMission->GetMission(mission_id)->GetValue();

			pPlayer->SendPacket(&pMsg);
		}
	}
}

void LabyrinthDimensions::SendFinalMissionList(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		return;
	}

	PlayerLabyrinthGate const* pPlayerLabyrinthGate = this->GetCurrentGate(pPlayer);

	if (!pPlayerLabyrinthGate)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST_BODY, body, buffer, sizeof(LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST_HEAD));
	head->count = 0;

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if (pLabyrinthMission->GetMission(mission_id)->GetType() != uint8(-1))
		{
			body[head->count].id = i;
			body[head->count].type = pLabyrinthMission->GetMission(mission_id)->GetType();

			if (pPlayerLabyrinthGate->GetMissionCount(i) >= pLabyrinthMission->GetMission(mission_id)->GetValue())
			{
				body[head->count].state = LABYRINTH_OF_DIMENSIONS_MISSION_STATE_COMPLETE;
			}
			else
			{
				body[head->count].state = LABYRINTH_OF_DIMENSIONS_MISSION_STATE_ACTIVE;
			}

			body[head->count].current_count = pPlayerLabyrinthGate->GetMissionCount(i);
			body[head->count].total_count = pLabyrinthMission->GetMission(mission_id)->GetValue();

			++head->count;
		}
	}

	head->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST, sizeof(LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST_HEAD) + (head->count * sizeof(LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST_BODY)));
	pPlayer->sendPacket(buffer);
}
	
void LabyrinthDimensions::SendFinalMission(Player* pPlayer, uint8 index)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		return;
	}

	PlayerLabyrinthGate const* pPlayerLabyrinthGate = this->GetCurrentGate(pPlayer);

	if (!pPlayerLabyrinthGate)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	if (index >= MAX_LABYRINTH_PLAYER_MISSION)
	{
		return;
	}

	uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(index);

	if (mission_id >= LABYRINTH_MISSION_MAX)
	{
		return;
	}

	LABYRINTH_OF_DIMENSIONS_MISSION_FINAL pMsg;
	pMsg.id = index;
	pMsg.type = pLabyrinthMission->GetMission(mission_id)->GetType();

	if (pPlayerLabyrinthGate->GetMissionCount(index) >= pLabyrinthMission->GetMission(mission_id)->GetValue())
	{
		pMsg.state = LABYRINTH_OF_DIMENSIONS_MISSION_STATE_COMPLETE;
	}
	else
	{
		pMsg.state = LABYRINTH_OF_DIMENSIONS_MISSION_STATE_ACTIVE;
	}

	pMsg.current_count = pPlayerLabyrinthGate->GetMissionCount(index);
	pMsg.total_count = pLabyrinthMission->GetMission(mission_id)->GetValue();

	pPlayer->SendPacket(&pMsg);
}

void LabyrinthDimensions::CreateGoblin(Player* pPlayer, LabyrinthAreaData const* pGate)
{
	pPlayer->GetLabyrinthData()->SetGoblinState(1);

	World* pWorld = sWorldMgr->GetWorld(g_LabyrinthOfDimensionsWorldData[0].world);

	if (!pWorld)
	{
		return;
	}

	Monster* pMonster = sObjectMgr->MonsterTryAdd(757, g_LabyrinthOfDimensionsWorldData[0].world);

	if (pMonster)
	{
		coord_type x = g_LabyrinthOfDimensionsWorldData[0].x;
		coord_type y = g_LabyrinthOfDimensionsWorldData[0].y;

		pWorld->GetRandomLocation(x, y, 10);

		pMonster->SetWorldId(g_LabyrinthOfDimensionsWorldData[0].world);
		pMonster->SetBasicLocation(x, y, x, y);
		pMonster->SetMoveDistance(10);
		pMonster->SetRespawn(5 * IN_MILLISECONDS);
		pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		pMonster->SetScriptName("labyrinth_of_dimensions_goblin_ai");
		pMonster->AddAdditionalDataPtr(0, pPlayer);
		pMonster->AddAdditionalDataInt(0, pGate->GetIndex());
		pMonster->AddAdditionalDataInt(1, pGate->GetSubIndex());
		pMonster->AddAdditionalDataInt(2, pPlayer->GetLabyrinthData()->GetStage());
		pMonster->AddAdditionalDataInt(3, pPlayer->GetLabyrinthData()->GetLevel());
		pMonster->SetInstance(pPlayer->GetInstance());
		pMonster->AddToWorld();
	}
}

void LabyrinthDimensions::SendStage(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LABYRINTH_OF_DIMENSIONS_STAGE_DATA pMsg;
	pMsg.stage = pPlayer->GetLabyrinthData()->GetLevel();
	pMsg.final_level = pPlayer->GetLabyrinthData()->GetLevel() == MAX_LABYRINTH_LEVEL;

	pPlayer->SendPacket(&pMsg);
}

LabyrinthArea const* LabyrinthDimensions::GetLabyrinthArea(uint8 index, uint8 stage) const
{
	if (index == uint8(-1))
	{
		RandomValue<LabyrinthArea const*> m_RandomValue(nullptr);

		for (LabyrinthAreaMap::const_iterator it = this->labyrinth_area_map.begin(); it != this->labyrinth_area_map.end(); ++it)
		{
			LabyrinthArea const* pLabyrinthArea = it->second;

			if (!pLabyrinthArea)
			{
				continue;
			}

			if (pLabyrinthArea->GetStage() != stage)
			{
				continue;
			}

			m_RandomValue.AddValue(pLabyrinthArea, 0);
		}

		return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
	}
	else
	{
		LabyrinthAreaMap::const_iterator it = this->labyrinth_area_map.find(index);

		if (it != this->labyrinth_area_map.end())
		{
			return it->second;
		}
	}

	return nullptr;
}

LabyrinthAreaData const* LabyrinthDimensions::GetLabyrinthAreaData(uint8 index, uint8 idx, uint8 subidx) const
{
	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(index, 0);

	if (pLabyrinthArea)
	{
		for (LabyrinthMapSettingDataList::const_iterator it = pLabyrinthArea->data_list.begin(); it != pLabyrinthArea->data_list.end(); ++it)
		{
			if ((*it)->GetIndex() == idx && (*it)->GetSubIndex() == subidx)
			{
				return (*it);
			}
		}
	}

	return nullptr;
}

LabyrinthMission const* LabyrinthDimensions::GetLabyrinthMission(uint8 area_type, uint8 index) const
{
	for (LabyrinthMissionList::const_iterator itr = this->labyrinth_mission_list.begin(); itr != this->labyrinth_mission_list.end(); ++itr)
	{
		LabyrinthMission const* pMission = *itr;

		if (!pMission)
		{
			continue;
		}

		if (pMission->GetAreaType() == area_type && pMission->GetIndex() == index)
		{
			return pMission;
		}
	}

	return nullptr;
}

uint8 LabyrinthDimensions::GetRandomMission(uint8 area_type, uint8 index) const
{
	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(area_type, index);

	if (!pLabyrinthMission)
	{
		return -1;
	}

	RandomValue<uint8> m_RandomValue(-1);

	for (int32 i = 0; i < LABYRINTH_MISSION_MAX; ++i)
	{
		if (pLabyrinthMission->GetMission(i)->GetType() != uint8(-1))
		{
			m_RandomValue.AddValue(i, 0);
		}
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

LabyrinthLevel const* LabyrinthDimensions::GetLevel(uint16 id) const
{
	LabyrinthLevelMap::const_iterator itr = this->m_LabyrinthLevelMap.find(id);

	if (itr != this->m_LabyrinthLevelMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

LabyrinthMonster const* LabyrinthDimensions::GetMonster(uint16 id) const
{
	LabyrinthMonsterMap::const_iterator itr = this->m_LabyrinthMonsterMap.find(id);

	if (itr != this->m_LabyrinthMonsterMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

void LabyrinthDimensions::GetReward(uint8 category, uint8 rank, uint8 race, LabyrinthRewardList & list) const
{
	for (LabyrinthRewardList::const_iterator itr = this->m_LabyrinthRewardList.begin(); itr != this->m_LabyrinthRewardList.end(); ++itr)
	{
		LabyrinthReward* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if ((pData->GetCategory() == category || pData->GetCategory() == 0xFF) && pData->GetRank() == rank && (pData->GetClass() == race || pData->GetClass() == 0xFF))
		{
			list.push_back(pData);
		}
	}
}

void LabyrinthDimensions::SetInitialGate(Player* pPlayer, uint8 index)
{
	if (!pPlayer)
	{
		return;
	}

	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(index, 0);

	if (pLabyrinthArea)
	{
		if (LabyrinthAreaData const* pEntrance = pLabyrinthArea->GetEntrance())
		{
			this->AddGate(pPlayer, index, pEntrance->GetIndex(), pEntrance->GetSubIndex(), true);
		}

		if (LabyrinthAreaData const* pExit = pLabyrinthArea->GetExit())
		{
			this->AddGate(pPlayer, index, pExit->GetIndex(), pExit->GetSubIndex(), false);
		}
	}
}

void LabyrinthDimensions::AddGate(Player* pPlayer, uint8 index, uint8 idx, uint8 subidx, bool enter)
{
	if (!pPlayer)
	{
		return;
	}

	PlayerLabyrinthGate * pPlayerLabyrinthGate = this->GetGate(pPlayer, idx, subidx);

	if (pPlayerLabyrinthGate)
	{
		if (enter)
		{
			this->StartMission(pPlayer, pPlayerLabyrinthGate);
		}

		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetLabyrinthAreaData(index, idx, subidx);

	if (pLabyrinthAreaData)
	{
		PlayerLabyrinthGate* pGate = new PlayerLabyrinthGate;
		pGate->SetIndex(idx);
		pGate->SetSubIndex(subidx);

		pPlayer->GetLabyrinthData()->m_GateList.push_back(pGate);

		if (enter)
		{
			this->StartMission(pPlayer, pGate);
		}
	}
}

void LabyrinthDimensions::StartMission(Player* pPlayer, PlayerLabyrinthGate * pGate)
{
	if (!pPlayer || !pGate)
	{
		return;
	}

	World* pWorld = pPlayer->GetWorld();

	if (!pWorld)
	{
		return;
	}

	/*if (pPlayer->GetLabyrinthData()->GetStatus() == 1)
	{
		return;
	}*/

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pGate->GetIndex(), pGate->GetSubIndex());

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION &&
		pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	this->CheckAddMission(pPlayer, pGate);

	for (int32 i = 0; i < LABYRINTH_MISSION_MONSTER_MAX; ++i)
	{
		if (pLabyrinthMission->GetMonster(i)->GetMonster() == uint16(-1) ||
			pLabyrinthMission->GetMonster(i)->GetCount() == -1)
		{
			continue;
		}

		for (int32 h = 0; h < pLabyrinthMission->GetMonster(i)->GetCount(); ++h)
		{
			Monster* pMonster = sObjectMgr->MonsterTryAdd(pLabyrinthMission->GetMonster(i)->GetMonster(), g_LabyrinthOfDimensionsWorldData[0].world);

			if (pMonster)
			{
				coord_type x = g_LabyrinthOfDimensionsWorldData[0].x;
				coord_type y = g_LabyrinthOfDimensionsWorldData[0].y;

				pWorld->GetRandomLocation(x, y, 10);

				pMonster->SetWorldId(g_LabyrinthOfDimensionsWorldData[0].world);
				pMonster->SetBasicLocation(x, y, x, y);
				pMonster->SetMoveDistance(10);
				pMonster->SetRespawn(5 * IN_MILLISECONDS);
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_NORMAL);
				pMonster->SetScriptName("labyrinth_of_dimensions_ai");
				pMonster->AddAdditionalDataPtr(0, pPlayer);
				pMonster->AddAdditionalDataInt(0, pGate->GetIndex());
				pMonster->AddAdditionalDataInt(1, pGate->GetSubIndex());
				pMonster->AddAdditionalDataInt(2, pPlayer->GetLabyrinthData()->GetStage());
				pMonster->AddAdditionalDataInt(3, pPlayer->GetLabyrinthData()->GetLevel());
				pMonster->SetInstance(pPlayer->GetInstance());
				pMonster->AddToWorld();
			}
		}
	}
}

void LabyrinthDimensions::CheckAddMission(Player* pPlayer, PlayerLabyrinthGate * pGate)
{
	LabyrinthAreaData const* pLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pGate->GetIndex(), pGate->GetSubIndex());

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION &&
		pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	bool active_mission = false;
	int32 completed_mission = 0;

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if (pLabyrinthMission->GetMission(mission_id)->GetType() == uint8(-1))
		{
			continue;
		}

		if (pGate->GetMissionCount(i) >= pLabyrinthMission->GetMission(mission_id)->GetValue())
		{
			completed_mission++;
		}
		else
		{
			active_mission = true;
		}
	}

	if (!active_mission)
	{
		if ((pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_MISSION && completed_mission < 1) ||
			(pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT && completed_mission < MAX_LABYRINTH_PLAYER_MISSION))
		{
			uint8 mission_id = this->GetRandomMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

			pGate->SetMissionID(completed_mission, mission_id);

			if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
			{
				this->SendFinalMission(pPlayer, completed_mission);
			}
			else
			{
				this->SendMission(pPlayer);
			}
		}
	}

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if (pGate->GetMissionCount(i) >= pLabyrinthMission->GetMission(mission_id)->GetValue())
		{
			continue;
		}

		if (pLabyrinthMission->GetMission(mission_id)->GetType() == LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_TIME)
		{
			this->SendTimer(pPlayer);
		}
	}
}

void LabyrinthDimensions::SpawnMonsters(Player* pPlayer, PlayerLabyrinthGate * pGate)
{
	if (!pPlayer || !pGate)
	{
		return;
	}

	World* pWorld = pPlayer->GetWorld();

	if (!pWorld)
	{
		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pGate->GetIndex(), pGate->GetSubIndex());

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if (pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION &&
		pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	for (int32 i = 0; i < LABYRINTH_MISSION_MONSTER_MAX; ++i)
	{
		if (pLabyrinthMission->GetMonster(i)->GetMonster() == uint16(-1) ||
			pLabyrinthMission->GetMonster(i)->GetCount() == -1)
		{
			continue;
		}

		for (int32 h = 0; h < pLabyrinthMission->GetMonster(i)->GetCount(); ++h)
		{
			Monster* pMonster = sObjectMgr->MonsterTryAdd(pLabyrinthMission->GetMonster(i)->GetMonster(), g_LabyrinthOfDimensionsWorldData[0].world);

			if (pMonster)
			{
				coord_type x = g_LabyrinthOfDimensionsWorldData[0].x;
				coord_type y = g_LabyrinthOfDimensionsWorldData[0].y;

				pWorld->GetRandomLocation(x, y, 10);

				pMonster->SetWorldId(g_LabyrinthOfDimensionsWorldData[0].world);
				pMonster->SetBasicLocation(x, y, x, y);
				pMonster->SetMoveDistance(10);
				pMonster->SetRespawn(5 * IN_MILLISECONDS);
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_NORMAL);
				pMonster->SetScriptName("labyrinth_of_dimensions_ai");
				pMonster->AddAdditionalDataPtr(0, pPlayer);
				pMonster->AddAdditionalDataInt(0, pGate->GetIndex());
				pMonster->AddAdditionalDataInt(1, pGate->GetSubIndex());
				pMonster->AddAdditionalDataInt(2, pPlayer->GetLabyrinthData()->GetStage());
				pMonster->SetInstance(pPlayer->GetInstance());
				pMonster->AddToWorld();
			}
		}
	}
}

PlayerLabyrinthGate * LabyrinthDimensions::GetCurrentGate(Player* pPlayer)
{
	if (!pPlayer)
	{
		return nullptr;
	}

	for (PlayerLabyrinthGateList::iterator itr = pPlayer->GetLabyrinthData()->m_GateList.begin(); itr != pPlayer->GetLabyrinthData()->m_GateList.end(); ++itr)
	{
		PlayerLabyrinthGate* pGate = *itr;

		if (!pGate)
		{
			continue;
		}

		if (pGate->GetIndex() == pPlayer->GetLabyrinthData()->GetGateIndex() &&
			pGate->GetSubIndex() == pPlayer->GetLabyrinthData()->GetGateSubIndex())
		{
			return pGate;
		}
	}

	return nullptr;
}

PlayerLabyrinthGate * LabyrinthDimensions::GetGate(Player* pPlayer, uint8 idx, uint8 subidx)
{
	if (!pPlayer)
	{
		return nullptr;
	}

	for (PlayerLabyrinthGateList::iterator itr = pPlayer->GetLabyrinthData()->m_GateList.begin(); itr != pPlayer->GetLabyrinthData()->m_GateList.end(); ++itr)
	{
		PlayerLabyrinthGate* pGate = *itr;

		if (!pGate)
		{
			continue;
		}

		if (pGate->GetIndex() == idx && pGate->GetSubIndex() == subidx)
		{
			return pGate;
		}
	}

	return nullptr;
}

PlayerLabyrinthGate const* LabyrinthDimensions::GetCurrentGate(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return nullptr;
	}

	for (PlayerLabyrinthGateList::const_iterator itr = pPlayer->GetLabyrinthData()->m_GateList.begin(); itr != pPlayer->GetLabyrinthData()->m_GateList.end(); ++itr)
	{
		PlayerLabyrinthGate* pGate = *itr;

		if (!pGate)
		{
			continue;
		}

		if (pGate->GetIndex() == pPlayer->GetLabyrinthData()->GetGateIndex() &&
			pGate->GetSubIndex() == pPlayer->GetLabyrinthData()->GetGateSubIndex())
		{
			return pGate;
		}
	}

	return nullptr;
}

PlayerLabyrinthGate const* LabyrinthDimensions::GetGate(Player* pPlayer, uint8 idx, uint8 subidx) const
{
	if (!pPlayer)
	{
		return nullptr;
	}

	for (PlayerLabyrinthGateList::const_iterator itr = pPlayer->GetLabyrinthData()->m_GateList.begin(); itr != pPlayer->GetLabyrinthData()->m_GateList.end(); ++itr)
	{
		PlayerLabyrinthGate* pGate = *itr;

		if (!pGate)
		{
			continue;
		}

		if (pGate->GetIndex() == idx && pGate->GetSubIndex() == subidx)
		{
			return pGate;
		}
	}

	return nullptr;
}

LabyrinthAreaData const* LabyrinthDimensions::GetCurrentAreaData(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return nullptr;
	}

	return this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetGateIndex(), pPlayer->GetLabyrinthData()->GetGateSubIndex());
}

bool LabyrinthDimensions::IsMissionCompleted(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return false;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		return false;
	}

	if ((pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION) &&
		(pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT))
	{
		return true;
	}

	PlayerLabyrinthGate const* pPlayerLabyrinthGate = this->GetCurrentGate(pPlayer);

	if (!pPlayerLabyrinthGate)
	{
		return false;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return false;
	}

	int32 count = 0;

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if (pLabyrinthMission->GetMission(mission_id)->GetType() != uint8(-1))
		{
			if (pPlayerLabyrinthGate->GetMissionCount(i) >= pLabyrinthMission->GetMission(mission_id)->GetValue())
			{
				++count;
			}
		}
	}

	if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		return count >= MAX_LABYRINTH_PLAYER_MISSION;
	}

	if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_MISSION)
	{
		return count >= 1;
	}

	return false;
}

bool LabyrinthDimensions::IsMissionCompleted(Player* pPlayer, uint8 idx, uint8 subidx) const
{
	if (!pPlayer)
	{
		return false;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetLabyrinthAreaData(pPlayer->GetLabyrinthData()->GetID(), idx, subidx);

	if (!pLabyrinthAreaData)
	{
		return false;
	}

	if ((pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION) &&
		(pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT))
	{
		return true;
	}

	PlayerLabyrinthGate const* pPlayerLabyrinthGate = this->GetGate(pPlayer, idx, subidx);

	if (!pPlayerLabyrinthGate)
	{
		return false;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return false;
	}

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}
		
		if (pLabyrinthMission->GetMission(mission_id)->GetType() != uint8(-1))
		{
			if (pPlayerLabyrinthGate->GetMissionCount(i) < pLabyrinthMission->GetMission(mission_id)->GetValue())
			{
				return false;
			}
		}
	}

	return true;
}

void LabyrinthDimensions::UpdateMission(Player* pPlayer, LabyrinthOfDimensionsMissionType type, int32 ammount, bool reset, bool move)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if ((pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION) &&
		(pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT))
	{
		return;
	}

	PlayerLabyrinthGate * pPlayerLabyrinthGate = this->GetCurrentGate(pPlayer);

	if (!pPlayerLabyrinthGate)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	bool fail = false;
	bool completed = false;

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if ((pLabyrinthMission->GetMission(mission_id)->GetType() == type) && (pPlayerLabyrinthGate->GetMissionCount(i) < pLabyrinthMission->GetMission(mission_id)->GetValue()))
		{
			if (reset)
			{
				if (pPlayerLabyrinthGate->GetMissionCount(i) > 0)
				{
					pPlayerLabyrinthGate->SetMissionCount(i, 0);
					fail = true;

					if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
					{
						this->SendFinalMission(pPlayer, i);
					}
					else
					{
						this->SendMission(pPlayer);
					}

					if (type == LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_TIME)
					{
						this->SendTimer(pPlayer, true);
					}
				}
			}
			else
			{
				pPlayerLabyrinthGate->IncreaseMissionCount(i, ammount);

				if (pPlayerLabyrinthGate->GetMissionCount(i) >= pLabyrinthMission->GetMission(mission_id)->GetValue())
				{
					pPlayerLabyrinthGate->SetMissionCount(i, pLabyrinthMission->GetMission(mission_id)->GetValue());
					completed = true;

					if (type == LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_TIME)
					{
						this->SendTimer(pPlayer, true);
					}
				}

				if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
				{
					this->SendFinalMission(pPlayer, i);
				}
				else
				{
					this->SendMission(pPlayer);
				}
			}
		}
	}

	if (completed)
	{
		this->CheckAddMission(pPlayer, pPlayerLabyrinthGate);

		if (this->IsMissionCompleted(pPlayer))
		{
			pPlayer->GetLabyrinthData()->IncreaseCompletedMissions(1);

			if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
			{
				this->Finish(pPlayer);
			}
		}
		else
		{
			if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
			{
				this->SendFinalMissionList(pPlayer);
			}
		}
	}

	if (fail && move)
	{
		this->SendMissionFail(pPlayer);
	}
}

void LabyrinthDimensions::Respawn(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetStage());

	if (!pLabyrinthArea)
	{
		return;
	}

	if (LabyrinthAreaData const* pLabyrinthAreaData = pLabyrinthArea->GetEntrance())
	{
		pPlayer->GetLabyrinthData()->SetGateIndex(pLabyrinthAreaData->GetIndex());
		pPlayer->GetLabyrinthData()->SetGateSubIndex(pLabyrinthAreaData->GetSubIndex());
	}

	this->SendZoneInfo(pPlayer);
}

void LabyrinthDimensions::SendMissionFail(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	if (sGameServer->IsLabyrinthOfDimensionsMoveOnFail())
	{
		LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

		if (pLabyrinthAreaData)
		{
			if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_MISSION)
			{
				LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetStage());

				if (pLabyrinthArea)
				{
					if (LabyrinthAreaData const* pEntrance = pLabyrinthArea->GetEntrance())
					{
						this->TeleportToZone(pPlayer, pEntrance->GetIndex(), pEntrance->GetSubIndex());
					}
				}
			}
		}
	}

	LABYRINTH_OF_DIMENSIONS_MISSION_FAIL pMsg;

	pPlayer->SendPacket(&pMsg);
}

void LabyrinthDimensions::Finish(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	if (pPlayer->GetLabyrinthData()->GetStatus() == 1)
	{
		return;
	}

	pPlayer->GetLabyrinthData()->SetStatus(1);
	pPlayer->GetLabyrinthData()->SetScore(0);
	pPlayer->GetLabyrinthData()->IncreaseLevel();

	LabyrinthLevel const* pLabyrinthLevel = this->GetLevel(pPlayer->GetLabyrinthData()->GetLevel());

	LABYRINTH_OF_DIMENSIONS_STAGE_RESULT pMsg;
	pMsg.stage = pPlayer->GetLabyrinthData()->GetStage();
	pMsg.earned_experience = pPlayer->GetLabyrinthData()->GetEarnedExperience();
	pMsg.monster_killed = pPlayer->GetLabyrinthData()->GetKilledMonsters();
	pMsg.item_reward = ITEMGET(14, 403 + Random(3));

	if (pLabyrinthLevel)
	{
		if (pPlayer->GetLabyrinthData()->GetLevelCount(pPlayer->GetLabyrinthData()->GetLevel()) > 1)
		{
			pMsg.score = pLabyrinthLevel->GetScore();
		}
		else
		{
			pMsg.score = pLabyrinthLevel->GetFirstScore();
		}

		pMsg.goblin_score = pPlayer->GetLabyrinthData()->GetGoblinState() == 2 ? sGameServer->GetLabyrinthOfDimensionsGoblinScore() : 0;
		pMsg.first_day_score = pPlayer->GetLabyrinthData()->GetDayFirst() == 0 ? sGameServer->GetLabyrinthOfDimensionsDayFirstScore() : 0;

		time_t elapsed_time = time(nullptr) - pPlayer->GetLabyrinthData()->GetDate();
		time_t diff = 0;

		if (elapsed_time < (20 * MINUTE))
		{
			diff = (20 * MINUTE) - elapsed_time;
		}

		pMsg.clear_time_points = diff * sGameServer->GetLabyrinthOfDimensionsTimeScore();
		pMsg.time = elapsed_time;
				
		pPlayer->GetLabyrinthData()->IncreaseScore(pMsg.score);
		pPlayer->GetLabyrinthData()->IncreaseScore(pMsg.goblin_score);
		pPlayer->GetLabyrinthData()->IncreaseScore(pMsg.first_day_score);
		pPlayer->GetLabyrinthData()->IncreaseScore(pMsg.clear_time_points);
	}

	if (pPlayer->GetLabyrinthData()->GetDayFirst() == 0)
	{
		Custom::SystemTimer cur_time = Custom::SystemTimer();

		pPlayer->GetLabyrinthData()->SetDayFirst(cur_time.GetDay());
	}

	this->UpdateRanking(pPlayer);
	this->InsertRanking(pPlayer);

	Item item(pMsg.item_reward);
	item.SetExpireDate(time(nullptr) + WEEK);

	sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::LABYRINTH_OF_DIMENSIONS);

	pPlayer->SendPacket(&pMsg);
}

void LabyrinthDimensions::CompleteMission(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = this->GetCurrentAreaData(pPlayer);

	if (!pLabyrinthAreaData)
	{
		return;
	}

	if ((pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_MISSION) &&
		(pLabyrinthAreaData->GetAreaType() != LABYRINTH_OF_DIMENSIONS_AREA_EXIT))
	{
		return;
	}

	PlayerLabyrinthGate * pPlayerLabyrinthGate = this->GetCurrentGate(pPlayer);

	if (!pPlayerLabyrinthGate)
	{
		return;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return;
	}

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if (pLabyrinthMission->GetMission(mission_id)->GetType() != uint8(-1))
		{
			if (pPlayerLabyrinthGate->GetMissionCount(i) < pLabyrinthMission->GetMission(mission_id)->GetValue())
			{
				pPlayerLabyrinthGate->SetMissionCount(i, pLabyrinthMission->GetMission(mission_id)->GetValue());

				if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
				{
					this->SendFinalMission(pPlayer, i);
				}
				else
				{
					this->SendMission(pPlayer);
				}
			}
		}
	}

	pPlayer->GetLabyrinthData()->IncreaseCompletedMissions(1);

	if (pLabyrinthAreaData->GetAreaType() == LABYRINTH_OF_DIMENSIONS_AREA_EXIT)
	{
		this->Finish(pPlayer);
	}
}

void LabyrinthDimensions::Update(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		if (pPlayer->GetLabyrinthData()->IsUsingTimer() && pPlayer->GetLabyrinthData()->GetTime()->Elapsed())
		{
			sLabyrinthDimensions->UpdateMission(pPlayer, LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_TIME, 0, true);
		}
	}
	else
	{
		if (pPlayer->GetLabyrinthData()->GetDayFirst() != 0)
		{
			Custom::SystemTimer cur_time = Custom::SystemTimer();

			if (pPlayer->GetLabyrinthData()->GetDayFirst() != cur_time.GetDay())
			{
				pPlayer->GetLabyrinthData()->SetDayFirst(0);
			}
		}
	}

	/*time_t cur_time = time(nullptr);

	if (cur_time > (pPlayer->GetLabyrinthData()->GetDate() + WEEK))
	{
		if (pPlayer->GetWorldId() == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
		{
			pPlayer->MoveToGate(LABYRINTH_OF_DIMENSIONS_SAFE_GATE);
		}

		pPlayer->GetLabyrinthData()->Init();
		pPlayer->GetLabyrinthData()->InitData();

		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[Labyrinth of Dimensions] Restarted.");
	}*/
}

void LabyrinthDimensions::InsertRanking(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_LABYRINTH_INSERT);
	stmt->setUInt32(0, pPlayer->GetGUID());
	stmt->setUInt16(1, pPlayer->GetLabyrinthData()->GetLevel());
	stmt->setUInt8(2, pPlayer->GetLabyrinthData()->GetStage());
	stmt->setUInt8(3, pPlayer->GetLabyrinthData()->GetID());
	stmt->setInt32(4, pPlayer->GetLabyrinthData()->GetKilledMonsters());
	stmt->setInt64(5, pPlayer->GetLabyrinthData()->GetEarnedExperience());
	stmt->setInt32(6, pPlayer->GetLabyrinthData()->GetCompletedMissions());
	stmt->setInt32(7, pPlayer->GetLabyrinthData()->GetScore());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void LabyrinthDimensions::InsertLeague(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}


}

void LabyrinthDimensions::SendTimer(Player* pPlayer, bool init)
{
	if (!pPlayer)
	{
		return;
	}

	LABYRINTH_OF_DIMENSIONS_TIMER pMsg;
	pMsg.time = init ? 0 : sGameServer->GetLabyrinthOfDimensionsMissionTime();

	pPlayer->SendPacket(&pMsg);

	if (init)
	{
		pPlayer->GetLabyrinthData()->InitTime();
	}
	else
	{
		pPlayer->GetLabyrinthData()->StartTime(sGameServer->GetLabyrinthOfDimensionsMissionTime());
	}
}

bool LabyrinthDimensions::FoundExit(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return false;
	}

	if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		return false;
	}

	LabyrinthArea const* pLabyrinthArea = this->GetLabyrinthArea(pPlayer->GetLabyrinthData()->GetID(), pPlayer->GetLabyrinthData()->GetStage());

	if (!pLabyrinthArea)
	{
		return false;
	}

	LabyrinthAreaData const* pLabyrinthAreaData = pLabyrinthArea->GetExit();

	if (!pLabyrinthAreaData)
	{
		return false;
	}

	PlayerLabyrinthGate const* pPlayerLabyrinthGate = this->GetGate(pPlayer, pLabyrinthAreaData->GetIndex(), pLabyrinthAreaData->GetSubIndex());

	if (!pPlayerLabyrinthGate)
	{
		return false;
	}

	LabyrinthMission const* pLabyrinthMission = this->GetLabyrinthMission(pLabyrinthAreaData->GetAreaType(), pLabyrinthAreaData->GetMissionID());

	if (!pLabyrinthMission)
	{
		return false;
	}

	for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
	{
		uint8 mission_id = pPlayerLabyrinthGate->GetMissionID(i);

		if (mission_id >= LABYRINTH_MISSION_MAX)
		{
			continue;
		}

		if (pLabyrinthMission->GetMission(mission_id)->GetType() != uint8(-1))
		{
			return true;
		}
	}

	return false;
}

LabyrinthRankingPlayer * LabyrinthDimensions::FindPlayer(uint32 id)
{
	LabyrinthRankingPlayerMap::iterator itr = this->m_LabyrinthRankingPlayerMap.find(id);

	if (itr != this->m_LabyrinthRankingPlayerMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

LabyrinthRankingPlayer const* LabyrinthDimensions::FindPlayer(uint8 category, uint8 rank) const
{
	for (LabyrinthRankingPlayerMap::const_iterator itr = this->m_LabyrinthRankingPlayerMap.begin(); itr != this->m_LabyrinthRankingPlayerMap.end(); ++itr)
	{
		LabyrinthRankingPlayer* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetCategory() == category && pData->GetRank() == rank)
		{
			return pData;
		}
	}

	return nullptr;
}

void LabyrinthDimensions::SaveSchedule()
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_SCHEDULE_UPDATE);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt8(1, this->GetLeagueState());
	stmt->setInt64(2, this->GetLeagueTime());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void LabyrinthDimensions::UpdateState()
{
	time_t current_time = time(nullptr);

	if (current_time < (this->GetLeagueTime() + sGameServer->GetLabyrinthOfDimensionsStateDuration()))
	{
		return;
	}

	while (current_time >= (this->GetLeagueTime() + sGameServer->GetLabyrinthOfDimensionsStateDuration()))
	{
		this->SetLeagueState(this->GetLeagueState() + 1);

		if (this->GetLeagueState() >= LABYRINTH_OF_DIMENSIONS_LEAGUE_STATE_MAX)
		{
			this->SetLeagueState(LABYRINTH_OF_DIMENSIONS_LEAGUE_STATE_BRONZE);
		}

		this->SetLeagueTime(this->GetLeagueTime() + sGameServer->GetLabyrinthOfDimensionsStateDuration());
	}

	this->SaveSchedule();
}

void LabyrinthDimensions::ChangeState()
{
	sLog->outInfo("labyrinthdimension", "%s - %u", __FUNCTION__, this->GetLeagueState());

	this->MovePlayersOut();

	this->SetLeagueState(this->GetLeagueState() + 1);

	if (this->GetLeagueState() < LABYRINTH_OF_DIMENSIONS_LEAGUE_STATE_MAX)
	{
		this->UpdateRanking();
	}

	if (this->GetLeagueState() >= LABYRINTH_OF_DIMENSIONS_LEAGUE_STATE_MAX)
	{
		this->GiveRewards();

		this->CleanRanking();

		this->SetLeagueState(LABYRINTH_OF_DIMENSIONS_LEAGUE_STATE_BRONZE);
	}

	this->SetLeagueTime(time(nullptr));

	this->SaveSchedule();
}

void LabyrinthDimensions::Update()
{
	if (!sGameServer->IsLabyrinthOfDimensionsEnabled())
	{
		return;
	}

	if (time(nullptr) >= (this->GetLeagueTime() + sGameServer->GetLabyrinthOfDimensionsStateDuration()))
	{
		this->ChangeState();
	}
}


bool Sort(LabyrinthRankingPlayer* pPlayer1, LabyrinthRankingPlayer* pPlayer2)
{
	if (!pPlayer1)
	{
		return false;
	}

	if (!pPlayer2)
	{
		return true;
	}

	if (pPlayer1->GetLeagueScore() == pPlayer2->GetLeagueScore())
	{
		return pPlayer1->GetTime() < pPlayer2->GetTime();
	}

	return pPlayer1->GetLeagueScore() > pPlayer2->GetLeagueScore();
}

void LabyrinthDimensions::UpdateRankingSimple(uint8 category)
{
	std::vector<LabyrinthRankingPlayer*> list;

	for (LabyrinthRankingPlayerMap::const_iterator itr = this->m_LabyrinthRankingPlayerMap.begin(); itr != this->m_LabyrinthRankingPlayerMap.end(); ++itr)
	{
		LabyrinthRankingPlayer* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetCategory() != category)
		{
			continue;
		}

		list.push_back(pData);
	}

	std::sort(list.begin(), list.end(), Sort);

	int32 rank = 0;
	for (std::vector<LabyrinthRankingPlayer*>::const_iterator itr = list.begin(); itr != list.end(); ++itr)
	{
		LabyrinthRankingPlayer* pData = *itr;

		if (pData)
		{
			pData->SetRank(rank);
		}

		++rank;
	}

	list.clear();
}

void LabyrinthDimensions::UpdateRanking()
{
	sLog->outInfo("labyrinthdimension", "%s", __FUNCTION__);

	/*for (int32 i = 0; i < MAX_LABYRINTH_OF_DIMENSIONS_RANK; ++i)
	{
		this->UpdateRankingSimple(i);
	}*/

	this->UpdateCategory();

	for (int32 i = 0; i < MAX_LABYRINTH_OF_DIMENSIONS_RANK; ++i)
	{
		this->UpdateRankingSimple(i);
	}
	
	SQLTransaction trans = MuDatabase.BeginTransaction();

	for (LabyrinthRankingPlayerMap::const_iterator itr = this->m_LabyrinthRankingPlayerMap.begin(); itr != this->m_LabyrinthRankingPlayerMap.end(); ++itr)
	{
		LabyrinthRankingPlayer const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_CATEGORY_UPDATE);
		stmt->setUInt8(0, pData->GetCategory());
		stmt->setInt32(1, pData->GetLeagueScore());
		stmt->setInt64(2, pData->GetTime());
		stmt->setUInt32(3, pData->GetID());
		trans->Append(stmt);
	}

	MuDatabase.CommitTransaction(trans);
}

void LabyrinthDimensions::UpdateCategory()
{
	sLog->outInfo("labyrinthdimension", "%s", __FUNCTION__);

	for (LabyrinthRankingPlayerMap::const_iterator itr = this->m_LabyrinthRankingPlayerMap.begin(); itr != this->m_LabyrinthRankingPlayerMap.end(); ++itr)
	{
		LabyrinthRankingPlayer * pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (sGameServer->GetLabyrinthOfDimensionsMove(pData->GetCategory()) != 0)
		{
			if (pData->GetRank() < sGameServer->GetLabyrinthOfDimensionsMove(pData->GetCategory()))
			{
				pData->SetCategory(pData->GetCategory() + 1);
			}
		}

		pData->SetRank(0);
		pData->SetLeagueScore(0);
		pData->SetTime(-1);
	}
}

void LabyrinthDimensions::UpdateRanking(Player* pPlayer)
{
	sLog->outInfo("labyrinthdimension", "%s - %s", __FUNCTION__, pPlayer->BuildLog().c_str());

	LabyrinthRankingPlayer* pData = this->FindPlayer(pPlayer->GetGUID());

	if (!pData)
	{
		pData = new LabyrinthRankingPlayer();
		pData->SetID(pPlayer->GetGUID());
		pData->SetRank(0);
		pData->SetCategory(LABYRINTH_OF_DIMENSIONS_RANK_BRONZE);

		this->m_LabyrinthRankingPlayerMap[pData->GetID()] = pData;
	}

	pData->SetName(pPlayer->GetName(), MAX_CHARACTER_LENGTH);
	pData->SetClass(pPlayer->GetClass());
	pData->IncreaseScore(pPlayer->GetLabyrinthData()->GetScore());
	pData->IncreaseLeagueScore(pPlayer->GetLabyrinthData()->GetScore());
	pData->SetTime(time(nullptr));

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_UPDATE);
	stmt->setUInt8(0, pData->GetCategory());
	stmt->setUInt32(1, pData->GetID());
	stmt->setString(2, pData->GetName());
	stmt->setUInt8(3, pData->GetClass());
	stmt->setInt32(4, pData->GetScore());
	stmt->setInt32(5, pData->GetLeagueScore());
	stmt->setInt64(6, pData->GetTime());
	stmt->setString(7, pData->GetName());
	stmt->setUInt8(8, pData->GetClass());
	stmt->setInt32(9, pData->GetScore());
	stmt->setInt32(10, pData->GetLeagueScore());
	stmt->setInt64(11, pData->GetTime());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	this->UpdateRankingSimple(pData->GetCategory());
}

void LabyrinthDimensions::GiveRewards()
{
	sLog->outInfo("labyrinthdimension", "%s", __FUNCTION__);

	SQLTransaction trans = MuDatabase.BeginTransaction();
	SQLTransaction trans2 = MuLogDatabase.BeginTransaction();

	for (LabyrinthRankingPlayerMap::const_iterator itr = this->m_LabyrinthRankingPlayerMap.begin(); itr != this->m_LabyrinthRankingPlayerMap.end(); ++itr)
	{
		LabyrinthRankingPlayer const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		LabyrinthRewardList list;
		this->GetReward(pData->GetCategory(), pData->GetRank(), pData->GetClass(), list);

		if (list.empty())
		{
			continue;
		}

		for (LabyrinthRewardList::const_iterator itr2 = list.begin(); itr2 != list.end(); ++itr2)
		{
			LabyrinthReward const* pReward = *itr2;

			if (!pReward)
			{
				continue;
			}

			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_INSERT);
			stmt->setUInt32(0, pData->GetID());
			stmt->setUInt16(1, pReward->GetItem());
			stmt->setUInt8(2, pReward->GetLevel());
			stmt->setUInt8(3, pReward->GetDurability());
			stmt->setUInt8(4, pReward->GetSkill());
			stmt->setUInt8(5, pReward->GetLuck());
			stmt->setUInt8(6, pReward->GetOption());
			stmt->setUInt8(7, pReward->GetExcellent());
			stmt->setUInt8(8, pReward->GetAncient());
			stmt->setUInt8(9, 0);
			stmt->setUInt8(10, 0);
			stmt->setUInt16(11, SOCKET_SLOT_NONE);
			stmt->setUInt16(12, SOCKET_SLOT_NONE);
			stmt->setUInt16(13, SOCKET_SLOT_NONE);
			stmt->setUInt16(14, SOCKET_SLOT_NONE);
			stmt->setUInt16(15, SOCKET_SLOT_NONE);
			stmt->setUInt8(16, 0xFF);
			stmt->setUInt32(17, pReward->GetDBFlags());

			trans->Append(stmt);

			Item item = Item(pReward->GetItem(), pReward->GetLevel(), pReward->GetDurability(), pReward->GetSkill(), pReward->GetLuck(), pReward->GetOption(), pReward->GetExcellent(), pReward->GetAncient());
			item.SetDBFlag(pReward->GetDBFlags());

			PreparedStatement* stmt2 = MuLogDatabase.GetPreparedStatement(LOG_STATEMENT_REWARD_LABYRINTH_OF_DIMENSIONS);
			stmt2->setUInt16(0, sGameServer->GetServerCode());
			stmt2->setUInt32(1, pData->GetID());
			stmt2->setString(2, pData->GetName());
			stmt2->setUInt8(3, pData->GetCategory());
			stmt2->setInt32(4, pData->GetRank());
			stmt2->setString(5, item.BuildLogDB(-1, true));
			trans2->Append(stmt2);
		}
	}

	MuDatabase.CommitTransaction(trans);
	MuLogDatabase.CommitTransaction(trans2);
}

void LabyrinthDimensions::CleanRanking()
{
	sLog->outInfo("labyrinthdimension", "%s", __FUNCTION__);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_DELETE);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	MAP_CLEAR(LabyrinthRankingPlayerMap::iterator, this->m_LabyrinthRankingPlayerMap);
}

void LabyrinthDimensions::MovePlayersOut()
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for (PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		if (!pPlayer->IsPlaying())
		{
			continue;
		}

		if (pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
		{
			continue;
		}

		pPlayer->MoveToGate(503);
		pPlayer->GetLabyrinthData()->InitData();

		pPlayer->SendMessageBox(0, "Labyrinth of Dimensions League", "Labyrinth of Dimensions League stage has been changed.");
	}
}