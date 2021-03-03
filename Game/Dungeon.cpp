Dungeon::Dungeon()
{
	this->SetInstanceID(0);
}

Dungeon::~Dungeon()
{
	MAP_CLEAR(DungeonInstanceMap::iterator, this->m_DungeonInstanceMap);
	MAP_CLEAR(DungeonInstanceDataMap::iterator, this->m_DungeonInstanceDataMap);
	MAP_CLEAR(DungeonInstanceLevelMap::iterator, this->m_DungeonInstanceLevelMap);
	MAP_CLEAR(DungeonInstanceSavedMap::iterator, this->m_DungeonInstanceSavedMap);
}

void Dungeon::LoadSavedInstance()
{
	if (!sGameServer->IsDungeonEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Dungeon Saved Instances...");

	uint32 count = 0;

	MAP_CLEAR(DungeonInstanceSavedMap::iterator, this->m_DungeonInstanceSavedMap);

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_DUNGEON_INSTANCE_SELECT);

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			DungeonInstanceSaved * pData = new DungeonInstanceSaved;
			pData->SetCharID(fields[loop++].GetUInt32());
			pData->SetID(fields[loop++].GetUInt8());
			pData->SetTime(fields[loop++].GetInt64());
			
			this->m_DungeonInstanceSavedMap[pData->GetCharID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u dungeon saved instances definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Dungeon::LoadInstance()
{
	if (!sGameServer->IsDungeonEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Dungeon Instances...");

	uint32 count = 0;

	MAP_CLEAR(DungeonInstanceDataMap::iterator, this->m_DungeonInstanceDataMap);

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_dungeon");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			DungeonInstanceData * pData = new DungeonInstanceData();
			pData->SetID(fields[loop++].GetUInt8());
			pData->SetWorld(fields[loop++].GetUInt16());
			pData->SetFlags(fields[loop++].GetUInt32());
			pData->SetGate(fields[loop++].GetUInt16());
			pData->SetMainChestX(fields[loop++].GetInt16());
			pData->SetMainChestY(fields[loop++].GetInt16());

			for (int32 i = 0; i < MAX_DUNGEON_SECONDARY_CHEST; ++i)
			{
				pData->SetChestX(i, fields[loop++].GetInt16());
				pData->SetChestY(i, fields[loop++].GetInt16());
			}

			this->m_DungeonInstanceDataMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u dungeon instances definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Dungeon::LoadLevel()
{
	if (!sGameServer->IsDungeonEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Dungeon Level...");

	uint32 count = 0;

	MAP_CLEAR(DungeonInstanceLevelMap::iterator, this->m_DungeonInstanceLevelMap);

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_dungeon_level");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			DungeonInstanceLevel * pData = new DungeonInstanceLevel();
			pData->SetID(fields[loop++].GetUInt16());
			pData->SetMinLevel(fields[loop++].GetInt16());
			pData->SetMaxLevel(fields[loop++].GetInt16());
			pData->SetLevel(fields[loop++].GetFloat());
			pData->SetHP(fields[loop++].GetFloat());
			pData->SetDamage(fields[loop++].GetFloat());
			pData->SetDefense(fields[loop++].GetFloat());
			pData->SetAttackSuccess(fields[loop++].GetFloat());
			pData->SetDefenseSuccess(fields[loop++].GetFloat());
			pData->SetElementalDamage(fields[loop++].GetFloat());
			pData->SetElementalDefense(fields[loop++].GetFloat());
			pData->SetElementalAttackSuccess(fields[loop++].GetFloat());
			pData->SetElementalDefenseSuccess(fields[loop++].GetFloat());
			pData->SetItemBag(fields[loop++].GetString());
			pData->SetItemBagBoss(fields[loop++].GetString());
			
			this->m_DungeonInstanceLevelMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u dungeon level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

DungeonInstanceData const* Dungeon::GetRandomDungeon() const
{
	RandomValue<DungeonInstanceData const*> m_RandomValue(nullptr);

	for (DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.begin(); itr != this->m_DungeonInstanceDataMap.end(); ++itr)
	{
		DungeonInstanceData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		m_RandomValue.AddValue(pData, 0);
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

DungeonInstanceData const* Dungeon::GetInstanceData(uint8 id) const
{
	DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.find(id);

	if (itr != this->m_DungeonInstanceDataMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

DungeonInstanceLevel const* Dungeon::GetInstanceLevel(uint16 level) const
{
	DungeonInstanceLevelMap::const_iterator itr = this->m_DungeonInstanceLevelMap.find(level);

	if (itr != this->m_DungeonInstanceLevelMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

DungeonInstanceLevel const* Dungeon::GetInstanceByLevel(int16 level) const
{
	for (DungeonInstanceLevelMap::const_iterator itr = this->m_DungeonInstanceLevelMap.begin(); itr != this->m_DungeonInstanceLevelMap.end(); ++itr)
	{
		DungeonInstanceLevel const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (level >= pData->GetMinLevel() && level <= pData->GetMaxLevel())
		{
			return pData;
		}
	}

	return nullptr;
}

bool Dungeon::IsDungeon(uint16 world) const
{
	for (DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.begin(); itr != this->m_DungeonInstanceDataMap.end(); ++itr)
	{
		DungeonInstanceData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetWorld() == world)
		{
			return true;
		}
	}

	return false;
}

void Dungeon::GetDungeonRespawn(Player* pPlayer, uint16 & world, int16 & x, int16 & y)
{
	for (DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.begin(); itr != this->m_DungeonInstanceDataMap.end(); ++itr)
	{
		DungeonInstanceData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetWorld() == world)
		{
			pPlayer->GetValidCoordinates(pData->GetGate(), world, x, y);
			return;
		}
	}
}

DungeonInstance * Dungeon::FindInstance(int32 id) const
{
	DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.find(id);

	if (itr != this->m_DungeonInstanceMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

DungeonInstance * Dungeon::FindInstanceByParty(uint16 id) const
{
	for (DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.begin(); itr != this->m_DungeonInstanceMap.end(); ++itr)
	{
		DungeonInstance * pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetPartyID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

DungeonInstanceSaved * Dungeon::FindPlayerInstance(uint32 id) const
{
	DungeonInstanceSavedMap::const_iterator itr = this->m_DungeonInstanceSavedMap.find(id);

	if (itr != this->m_DungeonInstanceSavedMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

void Dungeon::Update()
{
	for (DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.begin(); itr != this->m_DungeonInstanceMap.end();)
	{
		DungeonInstance * pData = itr->second;

		if (!pData)
		{
			this->m_DungeonInstanceMap.erase(itr++);
			continue;
		}

		if (pData->IsEmpty())
		{
			delete itr->second;
			this->m_DungeonInstanceMap.erase(itr++);
			continue;
		}

		pData->Update();
		++itr;
	}
}

void Dungeon::EnterRequest(Player* pPlayer)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	if (!sGameServer->IsDungeonEnabled())
	{
		pPlayer->SendMessageBox(0, "Instanced Dungeon", "Instanced Dungeon system is disabled.");
		return;
	}

	time_t current_time = time(nullptr);

	DungeonInstanceSaved * pSavedData = this->FindPlayerInstance(pPlayer->GetGUID());

	if (pSavedData)
	{
		if ((pSavedData->GetTime() + sGameServer->GetDungeonWaitTime()) > current_time)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "You will be able to enter again in %s", secsToTimeString((pSavedData->GetTime() + sGameServer->GetDungeonWaitTime()) - current_time).c_str());
			return;
		}
	}

	Party* pParty = pPlayer->GetParty();

	if (!pParty)
	{
		pPlayer->SendMessageBox(0, "Instanced Dungeon", "You need to be in Party.");
		return;
	}

	if (sGameServer->GetDungeonMaxPerPC() > 0)
	{
		if (this->GetCountByPC(pPlayer) >= sGameServer->GetDungeonMaxPerPC())
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "You can only participate with %d character at the same time.", sGameServer->GetDungeonMaxPerPC());
			return;
		}
	}

	DungeonInstance* pInstance = this->FindInstanceByParty(pParty->GetID());

	if (!pInstance)
	{
		DungeonInstanceLevel const* pLevelData = this->GetInstanceByLevel(pPlayer->GetTotalLevel());

		if (!pLevelData)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "There is not suitable Dungeon level for you.");
			return;
		}

		DungeonInstanceData const* pData = this->GetRandomDungeon();

		if (!pData)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "Can't find Dungeon database information.");
			return;
		}

		this->IncreaseInstanceID(1);

		pInstance = new DungeonInstance();
		pInstance->SetInstance(this->GetInstanceID());
		pInstance->SetID(pData->GetID());
		pInstance->SetLevelID(pLevelData->GetID());
		pInstance->SetPartyID(pParty->GetID());
		pInstance->SetWorld(pData->GetWorld());
		pInstance->GetTime()->Start(sGameServer->GetDungeonDuration());

		pInstance->AddMonster();

		this->m_DungeonInstanceMap[pInstance->GetInstance()] = pInstance;
	}
	else
	{
		if (pInstance->GetBossKilled() != 0)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "Your party instance has already finished.");
			return;
		}

		DungeonInstanceLevel const* pLevelData = this->GetInstanceLevel(pInstance->GetLevelID());

		if (!pLevelData)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "Instance level error.");
			return;
		}

		if (pPlayer->GetTotalLevel() < pLevelData->GetMinLevel())
		{
			int32 diff = pLevelData->GetMinLevel() - pPlayer->GetTotalLevel();

			if (diff > sGameServer->GetDungeonGAP())
			{
				pPlayer->SendMessageBox(0, "Instanced Dungeon", "Your level is too low to enter this instance.");
				return;
			}
		}

		if (pPlayer->GetTotalLevel() > pLevelData->GetMaxLevel())
		{
			int32 diff = pPlayer->GetTotalLevel() - pLevelData->GetMaxLevel();

			if (diff > sGameServer->GetDungeonGAP())
			{
				pPlayer->SendMessageBox(0, "Instanced Dungeon", "Your level is too high to enter this instance.");
				return;
			}
		}
	}

	pInstance->AddPlayer(pPlayer);
}

void Dungeon::SaveInstance(uint32 char_id, uint8 id, int64 time)
{
	DungeonInstanceSaved* pData = this->FindPlayerInstance(char_id);

	if (!pData)
	{
		pData = new DungeonInstanceSaved;
		pData->SetCharID(char_id);

		this->m_DungeonInstanceSavedMap[pData->GetCharID()] = pData;
	}

	pData->SetID(id);
	pData->SetTime(time);
}

int32 Dungeon::GetCountByPC(Player* pPlayer) const
{
	int32 count = 0;

	for (DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.begin(); itr != this->m_DungeonInstanceMap.end(); ++itr)
	{
		DungeonInstance * pData = itr->second;

		if (!pData)
		{
			continue;
		}

		count += pData->GetCountByPC(pPlayer);
	}

	return count;
}