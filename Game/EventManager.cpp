/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "EventManager.cpp"
*
*/

CEventMgr::CEventMgr()
{
	this->GetUpdateTime()->Start();
}

CEventMgr::~CEventMgr()
{
	LIST_CLEAR(EventDataList::iterator, this->m_event_list);
	MAP_CLEAR(EventSeasonDataMap::iterator, this->m_season_data_map);
	LIST_CLEAR(EventContributionRewardList::iterator, this->m_event_contribution_reward);
	LIST_CLEAR(EventLevelList::iterator, this->m_event_level_list);
	MAP_CLEAR(EventEnterCountMap::iterator, this->m_event_enter_count_map);
}

void CEventMgr::LoadEventData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Event Manager...");

	LIST_CLEAR(EventDataList::iterator, this->m_event_list);

	uint32 count = 0;

	PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_SRVCFG_EVENT_MANAGER_SELECT);
	stmt->setUInt16(0, sGameServer->GetTransferServer());
	stmt->setUInt16(1, sGameServer->GetServerCode());

	PreparedQueryResult result = GameServerDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			event_data * add_event = new event_data;

			add_event->SetEventID(fields[1].GetUInt8());
			add_event->SetInvasion(fields[2].GetUInt32());
			add_event->SetDuration(fields[3].GetUInt32());
			add_event->SetNotifyTime(fields[4].GetInt32());
			add_event->SetHour(fields[5].GetUInt8());
			add_event->SetMinute(fields[6].GetUInt8());
			add_event->SetDayOfWeek(fields[7].GetUInt8());
			add_event->SetDayOfMonth(fields[8].GetUInt8());
			add_event->SetSeasonEvent(fields[9].GetUInt8());
			
			this->m_event_list.push_back(add_event);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u event manager definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CEventMgr::LoadEventSeasonData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Event Season...");

	MAP_CLEAR(EventSeasonDataMap::iterator, this->m_season_data_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT i.event, i.name, UNIX_TIMESTAMP(i.start_date), UNIX_TIMESTAMP(i.end_date), i.ocurrence, i.duration, i.start_message, i.end_message FROM event_season_manager i WHERE i.duration > 0");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			EventSeasonData * pData = new EventSeasonData;
			
			pData->SetName(fields[1].GetString());
			uint64 starttime = fields[2].GetUInt64();
			pData->SetStartDate(time_t(starttime));
			uint64 endtime = fields[3].GetUInt64();
			pData->SetEndDate(time_t(endtime));
			pData->SetOcurrence(fields[4].GetUInt32());
            pData->SetLength(fields[5].GetUInt32());
			pData->SetStartMessage(fields[6].GetString());
			pData->SetEndMessage(fields[7].GetString());

			pData->SetStarted(false);
		
			this->m_season_data_map[fields[0].GetUInt8()] = pData;
			
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u event season definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CEventMgr::LoadEventContributionReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Event Contribution Reward...");

	LIST_CLEAR(EventContributionRewardList::iterator, this->m_event_contribution_reward);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_contribution_reward");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			event_contribution_reward * pData = new event_contribution_reward;

			pData->SetEventID(fields[0].GetUInt8());
			pData->SetEventGround(fields[1].GetUInt8());
			pData->SetContributionMin(fields[2].GetInt32());
			pData->SetContributionMax(fields[3].GetInt32());
			pData->SetRewardBox(fields[4].GetUInt16());
		
			this->m_event_contribution_reward.push_back(pData);
			
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u event contribution reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CEventMgr::LoadEventLevel()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Event Level...");

	LIST_CLEAR(EventLevelList::iterator, this->m_event_level_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_level");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			event_level * pData = new event_level;

			pData->SetEventID(fields[0].GetUInt8());
			pData->SetEventGround(fields[1].GetUInt8());
			pData->SetNormalLevelMin(fields[2].GetInt16());
			pData->SetNormalLevelMax(fields[3].GetInt16());
		
			this->m_event_level_list.push_back(pData);
			
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u event level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CEventMgr::LoadEventEnterCount()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Event Enter Count...");

	MAP_CLEAR(EventEnterCountMap::iterator, this->m_event_enter_count_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_enter_count");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			EventEnterCount * pData = new EventEnterCount;

			pData->SetEventID(fields[0].GetUInt8());
			pData->SetCount(fields[1].GetUInt8());
			
			this->m_event_enter_count_map[pData->GetEventID()] = pData;
			
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u event enter count definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CEventMgr::Update()
{
	if (this->GetUpdateTime()->Elapsed(IN_MILLISECONDS))
	{
		Custom::SystemTimer time = Custom::SystemTimer();
		int32 TimeSecRemain = 0;

		for (EventDataList::const_iterator it = this->m_event_list.begin(); it != this->m_event_list.end(); ++it)
		{
			if ((*it)->GetDayOfWeek() != uint8(-1) && time.GetDayOfWeek() != static_cast<uint16>((*it)->GetDayOfWeek()))
				continue;

			if ((*it)->GetDayOfMonth() != uint8(-1) && time.GetDay() != static_cast<uint16>((*it)->GetDayOfMonth()))
				continue;

			if (!this->IsSeasonEventOn((*it)->GetSeasonEvent()))
				continue;

			TimeSecRemain = 0;

			if ((*it)->ConvertToMinutes() > time.getInMinutes())
			{
				TimeSecRemain = ((*it)->ConvertToMinutes() - time.getInMinutes()) * MINUTE;
			}
			else if ((*it)->ConvertToMinutes() < time.getInMinutes())
			{
				TimeSecRemain = ((24 * MINUTE + (*it)->ConvertToMinutes()) - time.getInMinutes()) * MINUTE;
			}

			if (TimeSecRemain > (*it)->GetNotifyTime())
				continue;

			if (TimeSecRemain < 0)
				TimeSecRemain = 0;

			this->Start((*it)->GetEventID(), (*it)->GetDuration(), TimeSecRemain / MINUTE, (*it)->GetInvasion());
		}
	}

	sInvasionMgr->UpdateTime();
	sGuildMgr->UpdateTime();
	sHappyHour->UpdateTime();
	sScramble->UpdateTime();
	sBattleSoccerMgr->Update();
	sRaklion->UpdateTime();
	sKanturuMgr->UpdateTime();
	sDungeonRace->UpdateTime();
	sLosttowerRace->UpdateTime();
	sNixiesLake->Update();
	sBloodCastleMgr->UpdateTime();
	sDevilSquareMgr->UpdateTime();
	sChaosCastleMgr->UpdateTime();
	sDoppelganger->Update();
	sImperialFortressMgr->Update();
	sIllusionTempleMgr->UpdateTime();
	sTormentedSquare->Update();
	sLastManStanding->Update();
	sCastleSiege->UpdateTime();
	sCrywolf->UpdateTime();
	sCastleDeep->Update();
	sProtectorOfAcheron->Update();
	sArkaWar->Update();
	sWorldBoss->Update();
	sSwampOfDarkness->Update();
	sLabyrinthDimensions->Update();
	sDungeon->Update();
	sTormentedSquareSurvival->Update();
	sChaosCastleSurvivalMgr->UpdateTime();
}

void CEventMgr::UpdateSeason()
{
	time_t currenttime = time(NULL);

	for ( EventSeasonDataMap::const_iterator it = this->m_season_data_map.begin(); it != this->m_season_data_map.end(); ++it )
	{
		EventSeasonData * pData = it->second;

		if ( !pData )
		{
			continue;
		}

		bool old_status = pData->IsStarted();

		if ( pData->GetStartDate() < currenttime && currenttime < pData->GetEndDate() && (currenttime - pData->GetStartDate()) % (pData->GetOcurrence() * MINUTE) < pData->GetLength() * MINUTE )
		{
			pData->SetStarted(true);
		}
		else
		{
			pData->SetStarted(false);
		}

		if ( old_status != pData->IsStarted() )
		{
			sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "%s", pData->IsStarted() ? pData->GetStartMessage().c_str() : pData->GetEndMessage().c_str());

			sLog->outInfo("root", "%s -- [%u - %s - %s]", __FUNCTION__, it->first, pData->GetName().c_str(), pData->IsStarted() ? "Enabled" : "Disabled");
		}
	}
}

void CEventMgr::Start(uint8 event_id, uint32 duration, int32 notify, uint32 invasion)
{
	switch(event_id)
	{
	case EVENT_BLOOD_CASTLE:
		{
			sBloodCastleMgr->Start(duration, notify);
		} break;

	case EVENT_DEVIL_SQUARE:
		{
			sDevilSquareMgr->Start(duration, notify);
		} break;

	case EVENT_CHAOS_CASTLE:
		{
			sChaosCastleMgr->Start(duration, notify);
		} break;

	case EVENT_ILLUSION_TEMPLE:
		{
			sIllusionTempleMgr->Start(duration, notify);
		} break;

	case EVENT_INVASION:
		{
			sInvasionMgr->LaunchInvasion(invasion, duration, notify);
		} break;

	case EVENT_CRYWOLF:
		{
			if ( sCrywolf->GetState() == CRYWOLF_STATE_NONE )
				sCrywolf->SetState_Notify_1();
		} break;

	case EVENT_HAPPY_HOUR:
		{
			sHappyHour->Start(duration, notify);
		} break;

	case EVENT_SCRAMBLE:
		{
			sScramble->Start(duration, notify);
		} break;

	case EVENT_DUNGEON_RACE:
		{
			sDungeonRace->Start(duration, notify);
		} break;

	case EVENT_LOSTTOWER_RACE:
		{
			sLosttowerRace->Start(duration, notify);
		} break;

	case EVENT_DOPPELGANGER:
		{
			sDoppelganger->Start();
		} break;

	case EVENT_CHAOS_CASTLE_SURVIVAL:
		{
			sChaosCastleSurvivalMgr->Start(duration, notify);
		} break;

	case EVENT_TORMENTED_SQUARE:
		{
			sTormentedSquare->Start(notify);
		} break;

	case EVENT_ARKA_WAR:
		{
			sArkaWar->Start();
		} break;

	case EVENT_PROTECTOR_OF_ACHERON:
		{
			sProtectorOfAcheron->Start();
		} break;

	case EVENT_LAST_MAN_STANDING:
		{
			sLastManStanding->Start(notify);
		} break;

	case EVENT_CASTLE_DEEP:
		{
			sCastleDeep->Start(notify);
		} break;

	case EVENT_WORLD_BOSS:
		{
			sWorldBoss->Start();
		} break;
	}
}

bool CEventMgr::IsEventOn(uint8 event_id, uint32 invasion)
{
	switch(event_id)
	{
	case EVENT_BLOOD_CASTLE:
		{
			return sBloodCastleMgr->GetState() == EVENT_MGR_STATE_PLAYING;
		} break;

	case EVENT_DEVIL_SQUARE:
		{
			return sDevilSquareMgr->GetState() == EVENT_MGR_STATE_PLAYING;
		} break;

	case EVENT_CHAOS_CASTLE:
		{
			return sChaosCastleMgr->GetState() == EVENT_MGR_STATE_PLAYING;
		} break;

	case EVENT_ILLUSION_TEMPLE:
		{
			return sIllusionTempleMgr->GetState() == EVENT_MGR_STATE_PLAYING;
		} break;

	case EVENT_INVASION:
		{
			return sInvasionMgr->IsInvasionActive(invasion);
		} break;

	case EVENT_CRYWOLF:
		{
			
		} break;

	case EVENT_HAPPY_HOUR:
		{
			return sHappyHour->GetState() == HAPPY_HOUR_STATE_START;
		} break;

	case EVENT_SCRAMBLE:
		{
			return sScramble->GetState() == SCRAMBLE_STATE_START;
		} break;

	case EVENT_DUNGEON_RACE:
		{
			return sDungeonRace->GetState() == COMMON_RACE_STATE_PLAYING;
		} break;

	case EVENT_LOSTTOWER_RACE:
		{
			return sLosttowerRace->GetState() == COMMON_RACE_STATE_PLAYING;
		} break;

	case EVENT_DOPPELGANGER:
		{
			return sDoppelganger->GetState() == DOPPELGANGER_STATE_PLAYING;
		} break;

	case EVENT_CHAOS_CASTLE_SURVIVAL:
		{
			return sChaosCastleSurvivalMgr->GetState() == EVENT_MGR_STATE_PLAYING;
		} break;

	case EVENT_TORMENTED_SQUARE:
		{
			return sTormentedSquare->GetState() == TORMENTED_SQUARE_STATE_PLAYING;
		} break;

	case EVENT_LAST_MAN_STANDING:
		{
			return sLastManStanding->GetState() == LAST_MAN_STANDING_STATE_PLAYING;
		} break;
	}

	return true;
}

int32 CEventMgr::GetNextRemainTime(uint8 event_id)
{
	int32 remain_min = -1;
	int32 remain = 0;
	Custom::SystemTimer time = Custom::SystemTimer();

	for ( EventDataList::const_iterator it = this->m_event_list.begin(); it != this->m_event_list.end(); ++it )
	{
		if ( !(*it) )
			continue;

		if ( (*it)->GetEventID() != event_id )
			continue;

		remain = 0;

		if ( (*it)->ConvertToMinutes() > time.getInMinutes() )
		{
			remain = ((*it)->ConvertToMinutes() - time.getInMinutes()) * MINUTE;
		}
		else if ( (*it)->ConvertToMinutes() < time.getInMinutes() )
		{
			remain = ((24 * MINUTE + (*it)->ConvertToMinutes()) - time.getInMinutes()) * MINUTE;
		}

		if ( remain < 0 )
			remain = 0;

		if ( remain < remain_min || remain_min == -1 )
			remain_min = remain;
	}

	if ( remain_min == -1 )
		remain_min = HOUR;

	return remain_min;
}

//bool CEventMgr::GetEventRemainTimeData(uint8 event_id, uint32 & remain_time, uint8 & players_count, uint8 race, int16 level, bool master)
bool CEventMgr::GetEventRemainTimeData(uint8 event_id, uint32 & remain_time, uint8 & players_count, Player* pPlayer)
{
	remain_time = 0;
	players_count = 0;

	switch ( event_id )
	{
	case 1:
		{
			if ( sDevilSquareMgr->GetState() != EVENT_MGR_STATE_OPEN )
			{
				remain_time = sEventMgr->GetNextRemainTime(EVENT_DEVIL_SQUARE);
			}
		} break;

	case 2:
		{
			if ( sBloodCastleMgr->GetState() != EVENT_MGR_STATE_OPEN )
			{
				remain_time = sEventMgr->GetNextRemainTime(EVENT_BLOOD_CASTLE);
			}
		} break;

	case 4:
		{
			uint8 ground = sEventMgr->GetEventLevel(EVENT_CHAOS_CASTLE, pPlayer);

			if ( sChaosCastleMgr->GetState() == EVENT_MGR_STATE_OPEN && ground < MAX_CHAOS_CASTLE_GROUND )
			{
				players_count = sChaosCastleMgr->GetGround(ground)->GetPlayersCount(false);
			}
			else
			{
				remain_time = sEventMgr->GetNextRemainTime(EVENT_CHAOS_CASTLE);
			}
		} break;

	case 5:
		{
			if ( sIllusionTempleMgr->GetState() != EVENT_MGR_STATE_OPEN )
			{
				remain_time = sEventMgr->GetNextRemainTime(EVENT_ILLUSION_TEMPLE);
			}
		} break;
	}

	remain_time = remain_time / MINUTE;
	return true;
}

EventSeasonData * CEventMgr::GetSeasonData(uint8 season_event) const
{
	EventSeasonDataMap::const_iterator it = this->m_season_data_map.find(season_event);

	if ( it != this->m_season_data_map.end() )
		return it->second;

	return nullptr;
}

bool CEventMgr::IsSeasonEventOn(uint8 season_event)
{
	if ( season_event == uint8(-1) )
		return true;

	EventSeasonData const* pData = this->GetSeasonData(season_event);

	if ( !pData )
		return false;

	return pData->IsStarted();
}

void CEventMgr::GiveContributionReward(Player* pPlayer, uint8 event_id, uint8 event_ground, int32 contribution, uint8 gremory_type)
{
	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->IsMaster() && !pPlayer->IsMajestic())
	{
		return;
	}

	for (EventContributionRewardList::const_iterator it = this->m_event_contribution_reward.begin(); it != this->m_event_contribution_reward.end(); ++it)
	{
		event_contribution_reward const* pReward = (*it);

		if (!pReward)
		{
			continue;
		}

		if (pReward->GetEventID() == event_id && pReward->GetEventGround() == event_ground)
		{
			if ((contribution >= pReward->GetContributionMin()) && (contribution <= pReward->GetContributionMax()))
			{
				sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(pReward->GetRewardBox(), 0, 0, 0, 0, 0, 0, 0, nullptr, 0xFF, 604800), GREMORY_CASE_TYPE_CHARACTER, gremory_type);
			}
		}
	}
}

void CEventMgr::AddEventScore(uint32 guid, const char * name, uint8 event_id, uint8 event_ground, int32 score)
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING);
	stmt->setUInt8(0, event_id);
	stmt->setUInt8(1, event_ground);
	stmt->setUInt32(2, guid);
	stmt->setInt64(3, score);
	stmt->setInt64(4, score);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

uint8 CEventMgr::GetEventLevel(EventID event_id, Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return -1;
	}

	for ( EventLevelList::const_iterator it = this->m_event_level_list.begin(); it != this->m_event_level_list.end(); ++it )
	{
		event_level const* pData = (*it);

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetEventID() != event_id )
		{
			continue;
		}

		if ( pPlayer->GetTotalLevel() >= pData->GetNormalLevelMin() && pPlayer->GetTotalLevel() <= pData->GetNormalLevelMax() )
		{
			return pData->GetEventGround();
		}
	}

	return -1;
}

bool CEventMgr::IsEventGroundLevel(EventID event_id, Player* pPlayer, int16 level_min, int16 level_max) const
{
	if (!pPlayer)
	{
		return false;
	}

	for (EventLevelList::const_iterator it = this->m_event_level_list.begin(); it != this->m_event_level_list.end(); ++it)
	{
		event_level const* pData = (*it);

		if (!pData)
		{
			continue;
		}

		if (pData->GetEventID() != event_id)
		{
			continue;
		}

		if (level_min >= pData->GetNormalLevelMin() && level_max <= pData->GetNormalLevelMax())
		{
			return pData->GetEventGround() == pPlayer->GetEventGround();
		}
	}

	return false;
}

void CEventMgr::BuildTormentedSquareSurvivalSchedule(uint8 * hour, uint8 * minute, uint8 type)
{
	int32 count = 0;

	for ( EventDataList::const_iterator it = this->m_event_list.begin(); it != this->m_event_list.end(); ++it )
	{
		if ( !(*it) )
		{
			continue;
		}

		if ( (*it)->GetEventID() != EVENT_TORMENTED_SQUARE_SURVIVAL )
		{
			continue;
		}

		switch ( type )
		{
		case TORMENTED_SQUARE_SURVIVAL_STAGE_PRELIMINARY:
			{
				if ( ((*it)->GetDayOfMonth() == sTormentedSquareSurvival->GetPreliminary(0)) && count < 10 )
				{
					hour[count] = (*it)->GetHour();
					minute[count] = (*it)->GetMinute();
					++count;
				}
			} break;

		case TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL:
			{
				if ( ((*it)->GetDayOfMonth() == sTormentedSquareSurvival->GetSemiFinal(0)) && count < 10 )
				{
					hour[count] = (*it)->GetHour();
					minute[count] = (*it)->GetMinute();
					++count;
				}
			} break;

		case TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL:
			{
				if ( (*it)->GetDayOfMonth() == sTormentedSquareSurvival->GetFinal() )
				{
					*hour = (*it)->GetHour();
					*minute = (*it)->GetMinute();
				}
			} break;
		}
	}
}

void CEventMgr::GetEventStage(uint8 event_id)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_EVENT_STAGE_SELECT);
	stmt->setUInt8(event_id);

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if (result)
	{
		Field* fields = result->Fetch();

		uint8 stage = fields[0].GetUInt8();

		switch (event_id)
		{
		case EVENT_CHAOS_CASTLE_SURVIVAL:
		{
			if (stage >= CHAOS_CASTLE_SURVIVAL_STAGE_NONE && stage <= CHAOS_CASTLE_SURVIVAL_STAGE_FINAL)
			{
				sChaosCastleSurvivalMgr->SetStage(ChaosCastleSurvivalStage(stage));
			}
		} break;

		case EVENT_TORMENTED_SQUARE_SURVIVAL:
		{
			if (stage >= TORMENTED_SQUARE_SURVIVAL_STAGE_NONE && stage <= TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER)
			{
				sTormentedSquareSurvival->SetStage(TormentedSquareSurvivalStage(stage));
			}
		} break;
		}
	}

	return;
}

uint8 CEventMgr::GetEventEnterCount(uint8 event_id) const
{
	EventEnterCountMap::const_iterator itr = this->m_event_enter_count_map.find(event_id);

	if ( itr != this->m_event_enter_count_map.end() )
	{
		return itr->second->GetCount();
	}
	else
	{
		return 0;
	}
}

bool CEventMgr::CanEnterEvent(Player* pPlayer, uint8 event_id) const
{
	if ( !pPlayer )
	{
		return false;
	}

	EventEnterCountMap::const_iterator itr1 = this->m_event_enter_count_map.find(event_id);
	PlayerEventEnterCountMap::const_iterator itr2 = pPlayer->event_enter_count_map.find(event_id);

	if ( itr1 == this->m_event_enter_count_map.end() )
	{
		return true;
	}

	if ( itr1->second->GetCount() == 0 )
	{
		return true;
	}

	if ( itr2 != pPlayer->event_enter_count_map.end() )
	{
		return itr2->second->GetCount() < itr1->second->GetCount();
	}
	else
	{
		return true;	
	}
}

void CEventMgr::EventEnter(Player* pPlayer, uint8 event_id)
{
	if ( !pPlayer )
	{
		return;
	}

	EventEnterCountMap::const_iterator itr1 = this->m_event_enter_count_map.find(event_id);
	PlayerEventEnterCountMap::const_iterator itr2 = pPlayer->event_enter_count_map.find(event_id);

	if ( itr1 == this->m_event_enter_count_map.end() )
	{
		return;
	}

	Custom::SystemTimer m_system_time = Custom::SystemTimer();

	if ( itr2 != pPlayer->event_enter_count_map.end() )
	{
		itr2->second->IncreaseCount(1);
	}
	else
	{
		PlayerEventEnterCount * pData = new PlayerEventEnterCount;
		pData->SetEventID(event_id);
		pData->SetCount(1);
		pData->SetDay(m_system_time.GetDay());

		pPlayer->event_enter_count_map[event_id] = pData;
	}

	pPlayer->SendEventEnterCount();
}

void CEventMgr::CastleSiegeKill(Player* pPlayer01, Player* pPlayer02)
{
	if (!sGameServer->IsEventKillRankingEnabled())
	{
		return;
	}

	if (!pPlayer01)
	{
		return;
	}

	if (pPlayer01->GetWorldId() != WORLD_CASTLE_SIEGE)
	{
		return;
	}

	if (sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START)
	{
		return;
	}

	if (!pPlayer01->isCastleSiegeAttacker() && !pPlayer01->isCastleSiegeDefender())
	{
		return;
	}

	Guild* pGuild01 = pPlayer01->GuildGet();
	Guild* pGuild02 = pPlayer02 ? pPlayer02->GuildGet() : nullptr;

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_CASTLE_SIEGE_KILL);
	stmt->setUInt32(0, pPlayer01->GetGUID());
	stmt->setString(1, pPlayer01->GetName());
	stmt->setUInt8(2, pPlayer01->GuildGetRanking());

	if (sCastleSiege->GetCrownSwitchUser(0) == pPlayer01)
	{
		stmt->setUInt8(3, 1);
	}
	else if (sCastleSiege->GetCrownSwitchUser(1) == pPlayer01)
	{
		stmt->setUInt8(3, 2);
	}
	else if (sCastleSiege->GetCrownUser() == pPlayer01)
	{
		stmt->setUInt8(3, 3);
	}
	else
	{
		stmt->setUInt8(3, 0);
	}
	
	stmt->setString(4, pPlayer01->BuildLogDB());
	stmt->setUInt32(5, pGuild01 ? pGuild01->GetID() : 0);
	stmt->setString(6, pGuild01 ? pGuild01->GetName() : "");

	stmt->setUInt32(7, pPlayer02 ? pPlayer02->GetGUID() : 0);
	stmt->setString(8, pPlayer02 ? pPlayer02->GetName() : "");
	stmt->setUInt8(9, pPlayer02 ? pPlayer02->GuildGetRanking() : GUILD_RANK_NONE);
	stmt->setString(10, pPlayer02 ? pPlayer02->BuildLogDB() : "");
	stmt->setUInt32(11, pGuild02 ? pGuild02->GetID() : 0);
	stmt->setString(12, pGuild02 ? pGuild02->GetName() : "");

	stmt->setUInt16(13, pPlayer01->GetWorldId());
	stmt->setInt16(14, pPlayer01->GetX());
	stmt->setInt16(15, pPlayer01->GetY());

	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void CEventMgr::ArkaWarKill(Player* pPlayer01, Player* pPlayer02)
{
	if (!sGameServer->IsEventKillRankingEnabled())
	{
		return;
	}

	if (!pPlayer01)
	{
		return;
	}

	if (!AW_MAP_RANGE(pPlayer01->GetWorldId()))
	{
		return;
	}

	if (sArkaWar->GetState() != ARKA_WAR_STATE_PLAYING)
	{
		return;
	}

	Guild* pGuild01 = pPlayer01->GuildGet();
	Guild* pGuild02 = pPlayer02 ? pPlayer02->GuildGet() : nullptr;

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_ARKA_WAR_KILL);
	stmt->setUInt32(0, pPlayer01->GetGUID());
	stmt->setString(1, pPlayer01->GetName());
	stmt->setUInt8(2, pPlayer01->GuildGetRanking());
	stmt->setString(3, pPlayer01->BuildLogDB());
	stmt->setUInt32(4, pGuild01 ? pGuild01->GetID() : 0);
	stmt->setString(5, pGuild01 ? pGuild01->GetName() : "");

	stmt->setUInt32(6, pPlayer02 ? pPlayer02->GetGUID() : 0);
	stmt->setString(7, pPlayer02 ? pPlayer02->GetName() : "");
	stmt->setUInt8(8, pPlayer02 ? pPlayer02->GuildGetRanking() : GUILD_RANK_NONE);
	stmt->setString(9, pPlayer02 ? pPlayer02->BuildLogDB() : "");
	stmt->setUInt32(10, pGuild02 ? pGuild02->GetID() : 0);
	stmt->setString(11, pGuild02 ? pGuild02->GetName() : "");

	stmt->setUInt16(12, pPlayer01->GetWorldId());
	stmt->setInt16(13, pPlayer01->GetX());
	stmt->setInt16(14, pPlayer01->GetY());

	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void CEventMgr::CastleSiegeTime(Player* pPlayer, uint8 type)
{
	if (!sGameServer->IsEventTimeRankingEnabled())
	{
		return;
	}

	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetWorldId() != WORLD_CASTLE_SIEGE)
	{
		return;
	}

	if (sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START)
	{
		return;
	}

	uint32 timer = pPlayer->GetTimer(PLAYER_TIMER_CASTLE_SIEGE_TIME)->GetElapsed() / IN_MILLISECONDS;

	if (timer <= 0)
	{
		return;
	}

	pPlayer->GetTimer(PLAYER_TIMER_CASTLE_SIEGE_TIME)->Start();

	Guild* pGuild = pPlayer->GuildGet();

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_CASTLE_SIEGE_TIME);
	stmt->setUInt32(0, pPlayer->GetGUID());
	stmt->setString(1, pPlayer->GetName());
	stmt->setUInt8(2, pPlayer->GuildGetRanking());
	stmt->setString(3, pPlayer->BuildLogDB());
	stmt->setUInt32(4, pGuild ? pGuild->GetID() : 0);
	stmt->setString(5, pGuild ? pGuild->GetName() : "");
	stmt->setUInt8(6, type);
	stmt->setUInt32(7, timer);

	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}