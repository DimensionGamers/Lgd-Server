/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "QuestMgr.cpp"
*
*/

CQuestMgr::CQuestMgr()
{
	this->SetTestingDay(-1);

	this->SetCurrent4thQuestInstance(0);
}

CQuestMgr::~CQuestMgr()
{
	for ( QuestEvolutionMap::iterator itr = this->m_quest_evolution.begin(); itr != this->m_quest_evolution.end(); ++itr )
	{
		LIST_CLEAR(QuestEvolutionDataList::iterator, itr->second->data);
		LIST_CLEAR(QuestEvolutionConditionList::iterator, itr->second->condition);
		delete itr->second;
	}

	this->m_quest_evolution.clear();

	LIST_CLEAR(QuestEvolutionRewardList::iterator, this->m_QuestEvolutionRewardList);

	MAP_CLEAR(QuestGuidedMap::iterator, this->m_quest_guided_map);

	MAP_CLEAR(Quest4thInstanceDataMap::iterator, this->m_quest_4th_instance_data_map);

	MAP_CLEAR(QuestMUMap::iterator, this->m_QuestMUMap);
}

void CQuestMgr::LoadQuestEvolution()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Template...");

	for ( QuestEvolutionMap::iterator itr = this->m_quest_evolution.begin(); itr != this->m_quest_evolution.end(); ++itr )
	{
		LIST_CLEAR(QuestEvolutionDataList::iterator, itr->second->data);
		LIST_CLEAR(QuestEvolutionConditionList::iterator, itr->second->condition);
		delete itr->second;
	}

	this->m_quest_evolution.clear();

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM quest_evolution_template i ORDER BY i.quest_id ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 id = fields[0].GetUInt8();

			if ( this->GetQuest(id) != nullptr )
			{
				continue;
			}

			QuestEvolution* newQuest = new QuestEvolution;

			newQuest->SetID(id);
			newQuest->SetType(fields[1].GetUInt8());
			newQuest->SetNPC(fields[2].GetUInt16());
			newQuest->SetParty(fields[3].GetUInt8());
			newQuest->SetName(fields[4].GetString());

			this->m_quest_evolution[id] = newQuest;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestEvolutionData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Data...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM quest_evolution_data");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			uint8 quest = fields[0].GetUInt8();

			QuestEvolution * Quest = this->GetQuest(quest);

			if ( !Quest )
			{
				continue;
			}

			QuestEvolutionData * add_data = new QuestEvolutionData;
			add_data->SetQuestID(quest);
			add_data->SetObjectiveType(fields[1].GetUInt8());
			add_data->SetItemType(fields[2].GetUInt8());
			add_data->SetItemIndex(fields[3].GetUInt16());
			add_data->SetItemLevel(fields[4].GetUInt8());
			add_data->SetCount(fields[5].GetUInt16());
			add_data->SetMonsterID(fields[6].GetInt16());
			add_data->SetMonsterMinLevel(fields[7].GetInt16());
			add_data->SetMonsterMaxLevel(fields[8].GetInt16());
			add_data->SetMonsterSlot(fields[9].GetUInt8());
			add_data->SetDropRate(fields[10].GetUInt16());
			add_data->SetRewardType(fields[11].GetUInt16());
			add_data->SetRewardSubType(fields[12].GetUInt16());
			add_data->SetRewardCount(fields[13].GetUInt16());
			add_data->SetConditionIndex(fields[14].GetInt16());
			add_data->SetContextBeforeReg(fields[15].GetUInt16());
			add_data->SetContextAfterReg(fields[16].GetUInt16());
			add_data->SetContextCompleteQuest(fields[17].GetUInt16());
			add_data->SetContextClearQuest(fields[18].GetUInt16());

			for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
			{
				add_data->SetRequiredClass(i, fields[19 + i].GetUInt8());
			}

			if ( add_data->GetMonsterSlot() >= QUEST_EVOLUTION_MONSTER_COUNT )
			{
				add_data->SetMonsterSlot(QUEST_EVOLUTION_MONSTER_COUNT - 1);
			}

			Quest->data.push_back(add_data);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution data definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void CQuestMgr::LoadQuestEvolutionCondition()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Condition...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM quest_evolution_condition");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			uint8 quest = fields[0].GetUInt8();

			QuestEvolution * Quest = this->GetQuest(quest);

			if ( !Quest )
			{
				continue;
			}

			QuestEvolutionCondition * add_condition = new QuestEvolutionCondition;
			add_condition->SetQuestID(quest);
			add_condition->SetConditionID(fields[1].GetInt32());
			add_condition->SetRequiredQuestID(fields[2].GetUInt8());
			add_condition->SetMinLevel(fields[3].GetUInt32());
			add_condition->SetMaxLevel(fields[4].GetUInt32());
			add_condition->SetRequiredZen(fields[5].GetUInt32());
			add_condition->SetContextStart(fields[6].GetUInt8());

			Quest->condition.push_back(add_condition);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution condition definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestEvolutionItemReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Item Reward...");

	LIST_CLEAR(QuestEvolutionRewardList::iterator, this->m_QuestEvolutionRewardList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM quest_evolution_item_reward");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;
			
			QuestEvolutionReward* pData = new QuestEvolutionReward;
			pData->SetQuestID(fields[loop++].GetUInt8());
			pData->SetClassFlag(fields[loop++].GetUInt32());
			pData->SetType(fields[loop++].GetUInt8());
			pData->SetIndex(fields[loop++].GetUInt16());
			pData->SetLevel(fields[loop++].GetUInt8());
			pData->SetDurability(fields[loop++].GetUInt8());
			pData->SetSkill(fields[loop++].GetUInt8());
			pData->SetLuck(fields[loop++].GetUInt8());
			pData->SetOption(fields[loop++].GetUInt8());
			pData->SetExcellent(fields[loop++].GetUInt8());
			pData->SetAncient(fields[loop++].GetUInt8());

			for (int32 i = 0; i < MAX_SOCKET_SLOT; ++i)
			{
				pData->SetSocket(i, fields[loop++].GetUInt16());
			}

			pData->SetSocketBonus(fields[loop++].GetUInt8());

			this->m_QuestEvolutionRewardList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution item reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestGuided()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Guided Setting...");

	MAP_CLEAR(QuestGuidedMap::iterator, this->m_quest_guided_map);

	ReadScript file("EventData/GuideQuest.txt", "rb");
	uint32 count = 0;

	if ( !file.open() )
	{
		file.LogOpenError();
		return;
	}

	while ( true )
	{
		if ( file.GetToken() == END )
			break;

		QuestGuided * pData = new QuestGuided;

		pData->SetID(file.GetUInt16());
		file.GetToken(); pData->SetName(file.TokenString);

		file.GetToken(); pData->GetCondition()->SetTemplateType(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetChapter(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetCategory(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetImportance(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetStartType(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetStartSubType(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetLevelMin(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetQuestRepeatCycle(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetPrecedentQuest(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetQuestStartItemType(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetQuestStartItemIndex(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetQuestCheckGens(file.GetUInt8());

		for ( int32 i = 0; i < Character::MAX_CLASS; ++i )
		{
			file.GetToken(); pData->GetCondition()->SetRequiredClass(i, file.GetUInt8());
		}

		file.GetToken(); pData->GetCondition()->SetStartText(file.GetUInt16());
		file.GetToken(); pData->GetCondition()->SetStartErrorText(file.GetUInt16());


		file.GetToken(); pData->GetData()->SetTemplateType(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetObjectiveType(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetQuestMainType(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetQuestSubType(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetCount(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetMonsterMaxLevel(file.GetInt16());
		file.GetToken(); pData->GetData()->SetDropRate(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetGateNumber(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetWorld(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetX(file.GetInt16());
		file.GetToken(); pData->GetData()->SetY(file.GetInt16());
		file.GetToken(); pData->GetData()->SetRewardExp(file.GetUInt32());
		file.GetToken(); pData->GetData()->SetRewardZen(file.GetUInt32());
		file.GetToken(); pData->GetData()->SetRewardItemType01(file.GetUInt8());
		file.GetToken(); pData->GetData()->SetRewardItemIndex01(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetRewardItemCount01(file.GetInt32());
		file.GetToken(); pData->GetData()->SetRewardItemType02(file.GetUInt8());
		file.GetToken(); pData->GetData()->SetRewardItemIndex02(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetRewardItemCount02(file.GetInt32());
		file.GetToken(); pData->GetData()->SetRewardItemType03(file.GetUInt8());
		file.GetToken(); pData->GetData()->SetRewardItemIndex03(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetRewardItemCount03(file.GetInt32());
		file.GetToken(); pData->GetData()->SetRewardType(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetRewardSubType(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetRewardNumber(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetProgressText(file.GetUInt16());
		file.GetToken(); pData->GetData()->SetCompleteText(file.GetUInt16());

		if ( this->GetQuestGuided(pData->GetID()) != nullptr )
		{
			sLog->outError("root", "Guided Quest [%u / %s] already exist", pData->GetID(), pData->GetName().c_str());
			delete pData;
			continue;
		}

		//sLog->outInfo("root", "ID: %u	- Type: %u	- MainType: %u	- SubType: %u	- RewExp: %u	- RewZen: %u	- RewType: %u	- RewSubType: %u	- RewNum: %u", 
		//	pData->GetID(), pData->GetData()->GetQuestType(), pData->GetData()->GetQuestMainType(), pData->GetData()->GetQuestSubType(),
		//	pData->GetData()->GetRewardExp(), pData->GetData()->GetRewardZen(), pData->GetData()->GetRewardType(), pData->GetData()->GetRewardSubType(), pData->GetData()->GetRewardNumber());

		this->m_quest_guided_map[pData->GetID()] = pData;
		++count;
	}

	sLog->outInfo(LOG_DEFAULT, "Loaded %u quest guided definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestMU()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest MU...");

	MAP_CLEAR(QuestMUMap::iterator, this->m_QuestMUMap);

	/*ReadScript file("EventData/MUQuest.txt", "rb");
	uint32 count = 0;

	if (!file.open())
	{
		file.LogOpenError();
		return;
	}

	while (true)
	{
		if (file.GetToken() == END)
			break;

		sLog->outInfo("root", "Read Quest");

		int32 loop = 0;
		PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_GAME_INSERT_QUEST);
		stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setString(loop++, "");
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setUInt8(loop++, file.GetUInt8());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		file.GetToken(); stmt->setInt32(loop++, file.GetInt32());
		
		GameServerDatabase.Execute(stmt);
		
		++count;
	}*/

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM quest_mu AS i WHERE i.Disabled = 0");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;
			
			QuestMU* pData = new QuestMU();

			pData->SetID(fields[loop++].GetUInt16());
			pData->SetName(fields[loop++].GetString());
			pData->SetServer(fields[loop++].GetUInt8());
			pData->SetDay(fields[loop++].GetUInt8());
			pData->SetTemplateType1(fields[loop++].GetUInt8());
			pData->SetChapter(fields[loop++].GetUInt8());
			pData->SetCategory(fields[loop++].GetUInt8());
			pData->SetImportante(fields[loop++].GetUInt8());
			pData->SetStartType(fields[loop++].GetUInt8());
			pData->SetStartSubType(fields[loop++].GetUInt16());
			pData->SetLevelMin(fields[loop++].GetUInt16());
			pData->SetLevelMax(fields[loop++].GetUInt16());
			pData->SetRepeatCycle(fields[loop++].GetInt8());
			pData->SetPrecedenceQuest(fields[loop++].GetInt16());
			pData->SetStartItemType(fields[loop++].GetInt8());
			pData->SetStartItemIndex(fields[loop++].GetInt16());
			pData->SetCheckGens(fields[loop++].GetInt8());
			pData->SetZen(fields[loop++].GetInt32());

			for (int32 i = 0; i < Character::MAX_CLASS; ++i)
			{
				pData->SetRequiredClass(i, fields[loop++].GetUInt8());
			}

			pData->SetAddClass(fields[loop++].GetUInt8());
			pData->SetTemplateType2(fields[loop++].GetUInt8());
			pData->SetObjectiveType(fields[loop++].GetUInt8());
			pData->SetObjectiveMainType(fields[loop++].GetInt16());
			pData->SetObjectiveMainSubType(fields[loop++].GetInt16());
			pData->SetTargetNumber(fields[loop++].GetInt16());
			pData->SetTargetMaxLevel(fields[loop++].GetInt16());
			pData->SetDropRate(fields[loop++].GetInt16());
			pData->SetGateID(fields[loop++].GetUInt16());
			pData->SetMapID(fields[loop++].GetUInt16());
			pData->SetX(fields[loop++].GetUInt8());
			pData->SetY(fields[loop++].GetUInt8());
			pData->SetRewardExperience(fields[loop++].GetInt32());
			pData->SetRewardZen(fields[loop++].GetInt32());
			pData->SetRewardGensPoints(fields[loop++].GetUInt8());

			for (int32 i = 0; i < QUEST_MU_MAX_REWARD; ++i)
			{
				pData->SetRewardItemType(i, fields[loop++].GetInt8());
				pData->SetRewardItemIndex(i, fields[loop++].GetInt16());
				pData->SetRewardItemCount(i, fields[loop++].GetInt16());
			}

			pData->SetRewardType(fields[loop++].GetInt16());
			pData->SetRewardSubType(fields[loop++].GetInt16());
			pData->SetRewardNumber(fields[loop++].GetInt16());

			this->m_QuestMUMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, "Loaded %u quest mu definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

QuestEvolution const* CQuestMgr::GetQuest(uint8 quest_id) const
{
	QuestEvolutionMap::const_iterator itr = this->m_quest_evolution.find(quest_id);

	if ( itr != this->m_quest_evolution.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

QuestEvolution * CQuestMgr::GetQuest(uint8 quest_id)
{
	QuestEvolutionMap::iterator itr = this->m_quest_evolution.find(quest_id);

	if ( itr != this->m_quest_evolution.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

QuestGuided * CQuestMgr::GetQuestGuided(uint16 quest)
{
	QuestGuidedMap::const_iterator itr = this->m_quest_guided_map.find(quest);

	if ( itr != this->m_quest_guided_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}
	
QuestGuided const* CQuestMgr::GetQuestGuided(uint16 quest) const
{
	QuestGuidedMap::const_iterator itr = this->m_quest_guided_map.find(quest);

	if ( itr != this->m_quest_guided_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

uint16 CQuestMgr::GetFinalQuestGuided(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return 0;
	}

	uint16 id = 0;

	for ( QuestGuidedMap::const_iterator itr = sQuestMgr->m_quest_guided_map.begin(); itr != sQuestMgr->m_quest_guided_map.end(); ++itr )
	{
		QuestGuided const* pQuestGuided = itr->second;

		if ( !pQuestGuided )
		{
			continue;
		}

		if ( !pQuestGuided->IsClassAllowed(pPlayer->GetClass(), pPlayer->GetChangeUP(0), pPlayer->GetChangeUP(1), pPlayer->GetChangeUP(2)) )
		{
			continue;
		}

		if ( pQuestGuided->GetID() > id )
		{
			id = pQuestGuided->GetID();
		}
	}

	return id;
}

Quest4thInstanceData * CQuestMgr::Get4thQuestInstance()
{
	int32 count = this->GetCurrent4thQuestInstance();
	int32 total_count = 0;
	
	while ( true )
	{
		if ( !this->GetQuest4thInstanceData(count) )
		{
			Quest4thInstanceData * pData = new Quest4thInstanceData(count);
			this->m_quest_4th_instance_data_map[pData->GetInstance()] = pData;

			this->IncreaseCurrent4thQuestInstance(1);

			if ( this->GetCurrent4thQuestInstance() >= 100000 )
			{
				this->SetCurrent4thQuestInstance(0);
			}

			return pData;
		}

		++count;

		if ( count >= 100000 )
		{
			count = 0;
		}

		++total_count;

		if ( total_count >= 100000 )
		{
			break;
		}
	}

	return nullptr;
}

Quest4thInstanceData const* CQuestMgr::GetQuest4thInstanceData(int32 instance) const
{
	Quest4thInstanceDataMap::const_iterator itr = this->m_quest_4th_instance_data_map.find(instance);

	if ( itr != this->m_quest_4th_instance_data_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}
	
Quest4thInstanceData * CQuestMgr::GetQuest4thInstanceData(int32 instance)
{
	Quest4thInstanceDataMap::const_iterator itr = this->m_quest_4th_instance_data_map.find(instance);

	if ( itr != this->m_quest_4th_instance_data_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

void CQuestMgr::Update4thQuestInstance()
{
	for ( Quest4thInstanceDataMap::iterator itr = this->m_quest_4th_instance_data_map.begin(); itr != this->m_quest_4th_instance_data_map.end(); )
	{
		Quest4thInstanceData * pData = itr->second;

		if ( !pData )
		{
			this->m_quest_4th_instance_data_map.erase(itr++);
			continue;
		}

		if ( sObjectMgr->GetPlayerCountOnInstance(WORLD_PLACE_OF_QUALIFICATION, itr->first) <= 0 )
		{
			delete itr->second;
			this->m_quest_4th_instance_data_map.erase(itr++);
		}
		else
		{
			if ( pData->GetState() == 3 && pData->GetQuestID() == 8 )
			{
				if ( pData->GetTime()->Elapsed(MINUTE * IN_MILLISECONDS) )
				{
					this->Change4thQuestInstanceState(pData->GetInstance(), 0, 0);
				}
			}
			
			++itr;
		}
	}
}

void CQuestMgr::Change4thQuestInstanceState(int32 instance, uint8 state, uint8 quest_id)
{
	Quest4thInstanceData * pInstanceData = this->GetQuest4thInstanceData(instance);

	if ( !pInstanceData )
	{
		return;
	}

	pInstanceData->SetState(state);
	pInstanceData->SetQuestID(quest_id);
	pInstanceData->GetTime()->Start();
	
	switch ( pInstanceData->GetState() )
	{
	case 0: ///- None, recien aparecen en el mapa
		{
			Monster* pMonster = sObjectMgr->MonsterTryAdd(766, WORLD_PLACE_OF_QUALIFICATION);

			if ( pMonster )
			{
				pMonster->SetWorldId(WORLD_PLACE_OF_QUALIFICATION);
				pMonster->SetBasicLocation(147, 29, 147, 29);
				pMonster->SetInstance(pInstanceData->GetInstance());
				pMonster->SetDirection(2);
				pMonster->SetNpcFunction("npc_talk_quest_evolution");
				pMonster->SetScriptName("cent_quest_ai");

				pMonster->AddToWorld();

				sLog->outInfo(LOG_QUEST, "%s -- [%d] Added Quest NPC", __FUNCTION__, pInstanceData->GetInstance());

				this->Change4thQuestInstanceState(pInstanceData->GetInstance(), 1, 0);
			}
		} break;

	case 1: ///- StandBy, no ha clickeado al NPC
		{
		} break;

	case 2: ///- Ready, clickeo al NPC, procesar el pedido en base a la quest
		{
			QuestEvolution const* pQuest = sQuestMgr->GetQuest(pInstanceData->GetQuestID());

			if ( !pQuest )
			{
				return;
			}

			bool success = false;

			for ( QuestEvolutionDataList::const_iterator itr = pQuest->data.begin(); itr != pQuest->data.end(); ++itr )
			{
				QuestEvolutionData const* pData = *itr;

				if ( !pData )
				{
					continue;
				}

				if ( pData->GetObjectiveType() != QUEST_EVOLUTION_OBJECTIVE_MONSTER )
				{
					continue;
				}
					
				int32 loop = 1;

				if ( pInstanceData->GetQuestID() == 8 )
				{
					loop = 3;
				}

				for ( int32 i = 0; i < loop; ++i )
				{
					Monster* pMonster = sObjectMgr->MonsterTryAdd(pData->GetMonsterID(), WORLD_PLACE_OF_QUALIFICATION);

					if ( pMonster )
					{
						pMonster->SetWorldId(WORLD_PLACE_OF_QUALIFICATION);
						pMonster->SetBasicLocation(147, 29, 147, 29);
						pMonster->SetInstance(pInstanceData->GetInstance());
						pMonster->SetDirection(2);
						pMonster->SetScriptName(pInstanceData->GetQuestID() == 8 ? "4th_monster_quest_ai" : "cent_monster_quest_ai");
						pMonster->SetRespawnType(pInstanceData->GetQuestID() == 8 ? GAME_OBJECT_RESPAWN_NORMAL : GAME_OBJECT_RESPAWN_DELETE);
						pMonster->SetMoveDistance(10);

						pMonster->AddToWorld();
						success = true;

						sLog->outInfo(LOG_QUEST, "%s -- [%d] [%u - %s] Added Quest Monster [%u - %s]", __FUNCTION__, pInstanceData->GetInstance(), pQuest->GetID(), pQuest->GetName().c_str(), 
							pMonster->GetClass(), pMonster->GetName());
					}
				}
			}

			if ( success )
			{
				this->Change4thQuestInstanceState(pInstanceData->GetInstance(), 3, pInstanceData->GetQuestID());

				if ( pInstanceData->GetQuestID() == 8 )
				{
					QUEST_SURVIVAL_TIMER pMsg;
					pMsg.time = MINUTE * IN_MILLISECONDS;
					pMsg.type = 5;
					pMsg.increase = 0;

					sObjectMgr->SendPacketToInstance(WORLD_PLACE_OF_QUALIFICATION, pInstanceData->GetInstance(), (uint8*)&pMsg, pMsg.h.get_size());
				}
			}
		} break;

	case 3: ///- Process, procesar el tiempo
		{
			if ( pInstanceData->GetQuestID() == 8 && pInstanceData->GetTime()->Elapsed(MINUTE * IN_MILLISECONDS) )
			{
				this->Change4thQuestInstanceState(pInstanceData->GetInstance(), 0, 0);
			}
		} break;
	}
}

QuestMU const* CQuestMgr::GetQuestMU(uint16 id) const
{
	QuestMUMap::const_iterator itr = this->m_QuestMUMap.find(id);

	if (itr != this->m_QuestMUMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

void CQuestMgr::GiveQuestEvolutionItemReward(Player* pPlayer, uint8 quest_id)
{
	for (QuestEvolutionRewardList::const_iterator itr = this->m_QuestEvolutionRewardList.begin(); itr != this->m_QuestEvolutionRewardList.end(); ++itr)
	{
		QuestEvolutionReward const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetQuestID() != quest_id)
		{
			continue;
		}

		if (!pData->IsClassFlag(1 << pPlayer->GetClass()))
		{
			continue;
		}

		INIT_SOCKET_DATA(socket);
		SOCKET_SLOT_LOOP(i)
		{
			socket[i] = pData->GetSocket(i);
		}

		if (pData->GetItem() == MUUN_EVOLUTION_STONE)
		{
			uint16 muun = ITEMGET(socket[0], socket[1]);

			socket[0] = HIBYTE(muun);
			socket[1] = muun % MAX_SUBTYPE_ITEMS;
		}

		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(pData->GetItem(), pData->GetLevel(), pData->GetDurability(), pData->GetSkill(), pData->GetLuck(), pData->GetOption(), pData->GetExcellent(), pData->GetAncient(), socket, pData->GetSocketBonus()));
	}
}