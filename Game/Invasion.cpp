/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Invasion.cpp"
*
*/

Invasion::Invasion(uint32 id, uint32 group, uint32 sub_group)
{
	this->SetID(id);
	this->SetGroup(group);
	this->SetSubGroup(sub_group);
	this->SetX(-1);
	this->SetY(-1);
}

Invasion::~Invasion()
{
	this->Clear();
}

void Invasion::Clear()
{
	Monster* pMonster = nullptr;

	for ( MonsterDataMap::iterator it = this->monsters.begin(); it != this->monsters.end(); ++it )
	{
		pMonster = it->first;

		if ( !pMonster )
			continue;

		if ( !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->AIGroup("invasion_ai") )
			continue;

		InvasionBasicAI * pAI = AI_TO(InvasionBasicAI, pMonster->GetAI());

		if ( pAI->GetInvasionID() == this->GetID() &&
			 pAI->GetInvasionGroup() == this->GetGroup() &&
			 pAI->GetInvasionSubGroup() == this->GetSubGroup() )
		{
			pMonster->Remove();

			sObjectMgr->UpdateInvasionAnimation(pAI->GetEventAnimation(), pMonster->GetEntry(), pMonster->GetWorldId(), false);
		}
	}

	MAP_CLEAR(MonsterDataMap::iterator, this->monsters);
}

void Invasion::GenerateRespawnLocation()
{
	InvasionGroupDB const* pGroup = sInvasionMgr->GetInvasionGroupDB(this->GetID(), this->GetGroup(), this->GetSubGroup());

	if ( !pGroup )
		return;

	if (pGroup->GetX1() == -1 || pGroup->GetY1() == -1)
		return;

	World* pWorld = sWorldMgr->GetWorld(pGroup->GetWorld());

	if ( !pWorld )
	{
		return;
	}

	int32 count = 100;
	int16 x1 = pGroup->GetX1();
	int16 y1 = pGroup->GetY1();
	int16 x2 = pGroup->GetX2();
	int16 y2 = pGroup->GetY2();

	FIX_COORD(x1);
	FIX_COORD(y1);
	FIX_COORD(x2);
	FIX_COORD(y2);

	limitmin(x2, x1);
	limitmin(y2, y1);

	int16 finalx = -1;
	int16 finaly = -1;

	while ( count-- > 0 )
	{
		finalx = x1 + Random(x2 - x1);
		finaly = y1 + Random(y2 - y1);

		WorldGrid const& attr = pWorld->GetGrid(finalx, finaly);

		if ( !attr.IsSafe() && !attr.IsLocked_1() && !attr.IsLocked_2() )
		{
			this->SetX(finalx);
			this->SetY(finaly);
			break;
		}
	}
}

void Invasion::AddMonsters()
{
	auto event_monsters = sMonsterManager->GetEventMonsters(EVENT_INVASION);
	for (auto itr = event_monsters.first; itr != event_monsters.second; ++itr)
	{
		auto const& event_monster = itr->second;

		if (GetID() != event_monster->invasion.id)
			continue;

		if (GetGroup() != event_monster->invasion.group)
			continue;

		if (GetSubGroup() != event_monster->invasion.sub_group)
			continue;

		auto monster = sObjectMgr->MonsterTryAdd(event_monster->MonsterId, event_monster->MapId);
		if (monster)
		{
			monster->SetEventDBData(event_monster);
			monster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			monster->AddAdditionalDataInt(0, static_cast<int64>(GetID()));
			monster->AddAdditionalDataInt(1, static_cast<int64>(GetGroup()));
			monster->AddAdditionalDataInt(2, static_cast<int64>(GetSubGroup()));

			if (GetX() != -1 && GetY() != -1)
				monster->SetBasicLocation(GetX(), GetY(), GetX(), GetY());

			monster->AddToWorld();
			monsters[monster] = new MonsterData(monster, event_monster->invasion.boss, 1, event_monster->invasion.attack_percent);
		}
	}
}

InvasionData::InvasionData(uint32 id, uint32 time, int32 notify)
{
	this->id.set(id);
	this->time.set(time);
	this->notify.set(notify);
}

InvasionData::~InvasionData()
{

}

void InvasionData::UpdateTime()
{
	switch ( this->GetState() )
	{
	case INVASION_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case INVASION_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case INVASION_STATE_START:
		{
			this->ProcState_Start();
		} break;

	case INVASION_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void InvasionData::SetState_None()
{
	this->SetState(INVASION_STATE_NONE);
	this->RunTime(0);
}

void InvasionData::SetState_Notify()
{
	this->SetState(INVASION_STATE_NOTIFY);
	this->RunTime(this->notify.get() * MINUTE * IN_MILLISECONDS);
}

void InvasionData::SetState_Start()
{
	this->SetState(INVASION_STATE_START);

	this->RunTime(this->time.get() * MINUTE * IN_MILLISECONDS);

	for ( InvasionList::iterator it = this->invasion_list.begin(); it != this->invasion_list.end(); ++it )
	{
		(*it)->GenerateRespawnLocation();
		(*it)->AddMonsters();
	}
}
	
void InvasionData::SetState_End()
{
	this->SetState(INVASION_STATE_END);

	if ( InvasionDataDB const* data = sInvasionMgr->GetInvasionDataDB(this->id.get()) )
	{
		if ( !data->GetOnTimeOut().empty() )
			sObjectMgr->SendEventNotification(NOTICE_GLOBAL, data->GetOnTimeOut().c_str());
	}

	for ( InvasionList::iterator it = this->invasion_list.begin(); it != this->invasion_list.end(); ++it )
	{
		(*it)->Clear();
	}

	this->RunTime(5 * IN_MILLISECONDS);
}

void InvasionData::ProcState_None()
{

}
	
void InvasionData::ProcState_Notify()
{
	if ( this->TimePassed() )
		this->SetState_Start();
}
	
void InvasionData::ProcState_Start()
{
	if ( this->TimePassed() )
		this->SetState_End();
}
	
void InvasionData::ProcState_End()
{
	if ( this->TimePassed() )
		this->SetState_None();
}

InvasionMgr::InvasionMgr()
{

}

InvasionMgr::~InvasionMgr()
{
	this->ClearData();

	for ( InvasionDataMap::iterator it = this->m_invasion.begin(); it != this->m_invasion.end(); ++it )
	{
		LIST_CLEAR(InvasionList::iterator, it->second->invasion_list);
	}

	this->m_invasion.clear();
}

void InvasionMgr::ClearData()
{
	for ( InvasionDataMapDB::iterator it = this->m_invasion_map.begin(); it != this->m_invasion_map.end(); ++it )
	{
		for ( InvasionGroupMapDB::iterator it2 = it->second->groups.begin(); it2 != it->second->groups.end(); ++it2 )
		{
			LIST_CLEAR(InvasionGroupListDB::iterator, it2->second);
			it2->second.clear();
		}

		it->second->groups.clear();
		delete it->second;
	}
	
	this->m_invasion_map.clear();
}

void InvasionMgr::LoadInvasionData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Invasion Data...");

	this->ClearData();

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM event_invasion_data");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			InvasionDataDB * invasion = new InvasionDataDB;

			invasion->invasion.set(fields[0].GetUInt32());
			invasion->SetName(fields[1].GetString());
			invasion->SetOnNotify(fields[2].GetString());
			invasion->SetOnStart(fields[3].GetString());
			invasion->SetOnEnd(fields[4].GetString());
			invasion->SetOnTimeOut(fields[5].GetString());
			invasion->SetOnKillBoss(fields[6].GetString());
			invasion->groups.clear();

			this->m_invasion_map[invasion->invasion.get()] = invasion;

			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u event invasion definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void InvasionMgr::LoadInvasionGroup()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Invasion Group...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_invasion_group");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			InvasionGroupDB * invasion_gp = new InvasionGroupDB;

			uint32 invasion = fields[0].GetUInt32();
			uint32 group = fields[1].GetUInt32();

			invasion_gp->SetID(fields[2].GetUInt32());
			invasion_gp->SetRate(fields[3].GetUInt8());
			invasion_gp->SetFlag(fields[4].GetUInt32());
			invasion_gp->SetWorld(fields[5].GetUInt16());
			invasion_gp->SetX1(fields[6].GetInt16());
			invasion_gp->SetY1(fields[7].GetInt16());
			invasion_gp->SetX2(fields[8].GetInt16());
			invasion_gp->SetY2(fields[9].GetInt16());

			if (invasion_gp->GetX2() < invasion_gp->GetX1())
			{
				invasion_gp->SetX2(invasion_gp->GetX1());
			}

			if (invasion_gp->GetY2() < invasion_gp->GetY1())
			{
				invasion_gp->SetY2(invasion_gp->GetY1());
			}

			if (InvasionDataDB * data = this->GetInvasionDataDB(invasion))
			{
				data->groups[group].push_back(invasion_gp);
			}
			else
			{
				delete invasion_gp;
				continue;
			}

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u event invasion group definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void InvasionMgr::LaunchInvasion(uint32 invasion, uint32 time, int32 notify)
{
	if ( this->IsInvasionActive(invasion) )
		return;

	InvasionDataDB const* data = this->GetInvasionDataDB(invasion);

	if ( !data )
		return;

	InvasionData * pInvasion = new InvasionData(invasion, time, notify);
	this->m_invasion[invasion] = pInvasion;

	for ( InvasionGroupMapDB::const_iterator it = data->groups.begin(); it != data->groups.end(); ++it )
	{
		for ( InvasionGroupListDB::const_iterator it2 = it->second.begin(); it2 != it->second.end(); )
		{
			if (roll_chance_i((*it2)->GetRate()))
			{
				pInvasion->invasion_list.push_back(new Invasion(invasion, it->first, (*it2)->GetID()));
				break;
			}
			else
			{
				it2++;

				if ( it2 == it->second.end() )
					it2 = it->second.begin();
			}
		}
	}

	if ( !data->GetOnStart().empty() )
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, data->GetOnStart().c_str());

	pInvasion->notify.Is(0) ? pInvasion->SetState_Start(): pInvasion->SetState_Notify();

	sLog->outInfo(LOG_INVASION, "Invasion %d [%s] launched", invasion, data->GetName().c_str());
}

InvasionDataDB * InvasionMgr::GetInvasionDataDB(uint32 invasion) const
{
	InvasionDataMapDB::const_iterator it = this->m_invasion_map.find(invasion);

	return ( it != this->m_invasion_map.end() ? it->second: nullptr );
}

InvasionGroupDB const* InvasionMgr::GetInvasionGroupDB(uint32 invasion, uint32 group, uint32 sub_group) const
{
	InvasionDataDB const* pInvasionDataDB = this->GetInvasionDataDB(invasion);

	if (pInvasionDataDB)
	{
		InvasionGroupMapDB::const_iterator itr = pInvasionDataDB->groups.find(group);

		if (itr != pInvasionDataDB->groups.end())
		{
			for (InvasionGroupListDB::const_iterator it = itr->second.begin(); it != itr->second.end(); it++)
			{
				InvasionGroupDB const* pInvasionGroupDB = *it;

				if (!pInvasionGroupDB)
				{
					continue;
				}

				if (pInvasionGroupDB->GetID() == sub_group)
				{
					return pInvasionGroupDB;
				}
			}
		}
	}
	
	return nullptr;
}

bool InvasionMgr::IsInvasionActive(uint32 invasion) const
{
	InvasionDataMap::const_iterator it = this->m_invasion.find(invasion);

	return it != this->m_invasion.end();
}

void InvasionMgr::UpdateTime()
{
	for ( InvasionDataMap::iterator it = this->m_invasion.begin(); it != this->m_invasion.end(); )
	{
		if ( it->second->GetState() == INVASION_STATE_NONE || it->second->invasion_list.empty() )
		{
			sLog->outInfo(LOG_INVASION, "Deleting Invasion %u", it->first);

			LIST_CLEAR(InvasionList::iterator, it->second->invasion_list);
			this->m_invasion.erase(it++);
			continue;
		}

		it->second->UpdateTime();
		++it;
	}
}

void InvasionMgr::StopInvasion(uint32 invasion)
{
	InvasionDataMap::const_iterator it = this->m_invasion.find(invasion);

	if ( it == this->m_invasion.end() )
		return;

	it->second->SetState(INVASION_STATE_NONE);
}

void InvasionMgr::StopInvasion()
{
	for ( InvasionDataMap::iterator it = this->m_invasion.begin(); it != this->m_invasion.end(); ++it )
	{
		sLog->outInfo(LOG_INVASION, "Deleting Invasion %u", it->first);

		LIST_CLEAR(InvasionList::iterator, it->second->invasion_list);
	}

	this->m_invasion.clear();
}