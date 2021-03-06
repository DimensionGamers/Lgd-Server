DungeonInstance::DungeonInstance()
{
	this->SetBossKilled(0);
	this->SetKilledMonsters(0);
	this->GetTime()->Start();
	this->GetSimpleTrapTime()->Start();

	this->SetPlayerCount(0);
}

DungeonInstance::~DungeonInstance()
{
	this->m_DungeonInstancePlayerMap.clear();
}

void DungeonInstance::Update()
{
	this->UpdatePlayers();

	switch (this->GetBossKilled())
	{
	case 0:
		{
			if (this->GetSimpleTrapTime()->Elapsed(sGameServer->GetDungeonSimpleTrapTime()))
			{
				this->CreateSimpleTrap();
			}

			if (this->GetTime()->Elapsed())
			{
				this->MovePlayersOut();
			}
		} break;

	case 1:
		{
			if (this->GetBossKillTime()->Elapsed(5 * IN_MILLISECONDS))
			{
				this->BossKill(2);
			}
		} break;

	case 2:
		{
			if (this->GetBossKillTime()->Elapsed(5 * MINUTE * IN_MILLISECONDS))
			{
				this->MovePlayersOut();
			}
		} break;
	}
}

void DungeonInstance::UpdatePlayers()
{
	for (DungeonInstancePlayerMap::const_iterator itr = this->m_DungeonInstancePlayerMap.begin(); itr != this->m_DungeonInstancePlayerMap.end();)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			sLog->outInfo("instanced_dungeon", "%s (%d) :: Player [%u] does not exist.", __FUNCTION__, this->GetInstance(), itr->first);

			this->m_DungeonInstancePlayerMap.erase(itr++);
			continue;
		}

		if (!pPlayer->Object::IsPlaying())
		{
			sLog->outInfo("instanced_dungeon", "%s (%d) :: Player [%s] login out.", __FUNCTION__, this->GetInstance(), pPlayer->BuildLog().c_str());

			this->m_DungeonInstancePlayerMap.erase(itr++);
			continue;
		}

		if (pPlayer->GetRegenStatus() != REGEN_NONE)
		{
			++itr;
			continue;
		}

		if (!pPlayer->IsLive())
		{
			++itr;
			continue;
		}

		if (pPlayer->GetWorldId() != this->GetWorld())
		{
			sLog->outInfo("instanced_dungeon", "%s (%d) :: Player [%s] not in world.", __FUNCTION__, this->GetInstance(), pPlayer->BuildLog().c_str());

			this->m_DungeonInstancePlayerMap.erase(itr++);
			continue;
		}

		if (pPlayer->GetInstance() != this->GetInstance() || (this->GetBossKilled() == 0 && pPlayer->GetPartyID() != this->GetPartyID()))
		{
			sLog->outInfo("instanced_dungeon", "%s (%d) :: Player [%s] not in instance.", __FUNCTION__, this->GetInstance(), pPlayer->BuildLog().c_str());

			pPlayer->MoveToGate(sGameServer->GetDungeonEndGate());
		}

		++itr;
	}
}

void DungeonInstance::MovePlayersOut()
{
	for (DungeonInstancePlayerMap::const_iterator itr = this->m_DungeonInstancePlayerMap.begin(); itr != this->m_DungeonInstancePlayerMap.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		if (pPlayer->GetWorldId() == this->GetWorld() && pPlayer->GetInstance() == this->GetInstance())
		{
			sLog->outInfo("instanced_dungeon", "%s (%d) :: Moving Player [%s].", __FUNCTION__, this->GetInstance(), pPlayer->BuildLog().c_str());

			pPlayer->MoveToGate(sGameServer->GetDungeonEndGate());
		}
	}

	this->m_DungeonInstancePlayerMap.clear();
}

bool DungeonInstance::IsEmpty() const
{
	return this->m_DungeonInstancePlayerMap.empty();
}

void DungeonInstance::AddPlayer(Player* pPlayer)
{
	DungeonInstanceData const* pInstanceData = sDungeon->GetInstanceData(this->GetID());

	if (!pInstanceData)
	{
		return;
	}

	if (pPlayer->MoveToGate(pInstanceData->GetGate()))
	{
		pPlayer->SetInstance(this->GetInstance());

		QUEST_SURVIVAL_TIMER pMsg;
		pMsg.type = 1;
		pMsg.increase = 0;
		pMsg.time = this->GetTime()->GetRemain();
		pPlayer->SEND_PCT(pMsg);

		this->m_DungeonInstancePlayerMap[pPlayer->GetGUID()] = pPlayer;

		sLog->outInfo("instanced_dungeon", "%s (%d) :: Added Player [%s].", __FUNCTION__, this->GetInstance(), pPlayer->BuildLog().c_str());
	}
}

void DungeonInstance::AddMonster()
{
	auto event_monsters = sMonsterManager->GetEventMonsters(EVENT_INSTANCED_DUNGEON);
	for (auto itr = event_monsters.first; itr != event_monsters.second; ++itr)
	{
		auto const& event_monster = itr->second;

		if (event_monster->dungeon.id != this->GetID())
			continue;

		auto monster = sObjectMgr->MonsterTryAdd(event_monster->MonsterId, event_monster->MapId);
		if (monster)
		{
			monster->SetEventDBData(event_monster);
			monster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			monster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
			monster->SetRespawn(IN_MILLISECONDS);

			monster->AddAdditionalDataInt(0, GetInstance());
			monster->AddAdditionalDataInt(1, GetLevelID());

			monster->SetInstance(GetInstance());
			monster->AddToWorld();

			sLog->outInfo("instanced_dungeon", "%s (%d) :: Added Monster [%u][%u].", __FUNCTION__, GetInstance(), monster->GetEntry(), monster->GetClass());
		}
	}
}

void DungeonInstance::BossKill(uint8 id)
{
	if (id == 1)
	{
		if (this->GetBossKilled() == 0)
		{
			this->SetBossKilled(1);
			this->GetBossKillTime()->Start();

			sLog->outInfo("instanced_dungeon", "%s (%d) :: Killed Boss.", __FUNCTION__, this->GetInstance());
		}

		return;
	}

	if (id == 2 && this->GetBossKilled() != 1)
	{
		return;
	}

	this->SetBossKilled(2);
	this->GetBossKillTime()->Start();
	
	this->UpdatePlayers();

	this->SetPlayerCount(this->m_DungeonInstancePlayerMap.size());

	this->GetBossKillTime()->Start();

	this->CreateChest();

	this->SaveInstance();
}

void DungeonInstance::CreateChest()
{
	if (this->GetPlayerCount() <= 0)
	{
		return;
	}

	DungeonInstanceData const* pData = sDungeon->GetInstanceData(this->GetID());

	if (!pData)
	{
		return;
	}

	Monster* pMonster = sObjectMgr->MonsterTryAdd(5002, pData->GetWorld());

	if (pMonster)
	{
		pMonster->SetWorldId(pData->GetWorld());
		pMonster->SetDirection(2);
		pMonster->SetBasicLocation(pData->GetMainChestX(), pData->GetMainChestY(), pData->GetMainChestX(), pData->GetMainChestY());
		pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		pMonster->SetScriptName("ai_instanced_dungeon_chest");
		pMonster->AddAdditionalDataInt(0, this->GetInstance());
		pMonster->AddAdditionalDataInt(1, 0);
		pMonster->AddAdditionalDataInt(2, 0);

		pMonster->SetInstance(this->GetInstance());
		pMonster->AddToWorld();

		sLog->outInfo("instanced_dungeon", "%s (%d) :: Created Golden Chest [%u][%u].", __FUNCTION__, this->GetInstance(), pMonster->GetEntry(), pMonster->GetClass());
	}

	int32 silver_chest_count = this->GetKilledMonsters() / sGameServer->GetDungeonSilverChestMonster();

	if (silver_chest_count <= 0)
	{
		return;
	}

	if (silver_chest_count > MAX_DUNGEON_SECONDARY_CHEST)
	{
		silver_chest_count = MAX_DUNGEON_SECONDARY_CHEST;
	}

	for (int32 i = 0; i < silver_chest_count; ++i)
	{
		pMonster = sObjectMgr->MonsterTryAdd(5001, pData->GetWorld());

		if (pMonster)
		{
			pMonster->SetWorldId(pData->GetWorld());
			pMonster->SetDirection(Unit::GetPathPacketDirPos(pData->GetMainChestX() - pData->GetChestX(i), pData->GetMainChestY() - pData->GetChestY(i)));
			pMonster->SetBasicLocation(pData->GetChestX(i), pData->GetChestY(i), pData->GetChestX(i), pData->GetChestY(i));
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->SetScriptName("ai_instanced_dungeon_chest");
			pMonster->AddAdditionalDataInt(0, this->GetInstance());
			pMonster->AddAdditionalDataInt(1, 1);
			pMonster->AddAdditionalDataInt(2, i);

			pMonster->SetInstance(this->GetInstance());
			pMonster->AddToWorld();

			sLog->outInfo("instanced_dungeon", "%s (%d) :: Created Silver Chest [%u][%u].", __FUNCTION__, this->GetInstance(), pMonster->GetEntry(), pMonster->GetClass());
		}
	}
}

void DungeonInstance::ChestOpen(uint8 type, uint8 id)
{
	DungeonInstanceChestMap::const_iterator itr = this->m_DungeonInstanceChestMap[type].find(id);

	if (itr != this->m_DungeonInstanceChestMap[type].end())
	{
		return;
	}

	this->m_DungeonInstanceChestMap[type][id] = true;

	DungeonInstanceLevel const* pLevel = sDungeon->GetInstanceLevel(this->GetLevelID());

	if (!pLevel)
	{
		return;
	}

	if (type == 0)
	{
		if (!pLevel->GetItemBagBoss().empty())
		{
			for (DungeonInstancePlayerMap::const_iterator itr2 = this->m_DungeonInstancePlayerMap.begin(); itr2 != this->m_DungeonInstancePlayerMap.end(); ++itr2)
			{
				Player* pPlayer = itr2->second;

				if (pPlayer)
				{
					sItemBagMgr->ExecuteItemBag(ITEM_BAG_MISC, pPlayer, pLevel->GetItemBagBoss());
				}
			}
		}
	}
	else if (type == 1)
	{
		if (!pLevel->GetItemBag().empty())
		{
			for (DungeonInstancePlayerMap::const_iterator itr2 = this->m_DungeonInstancePlayerMap.begin(); itr2 != this->m_DungeonInstancePlayerMap.end(); ++itr2)
			{
				Player* pPlayer = itr2->second;

				if (pPlayer)
				{
					sItemBagMgr->ExecuteItemBag(ITEM_BAG_MISC, pPlayer, pLevel->GetItemBag());
				}
			}
		}
	}
}

void DungeonInstance::SaveInstance()
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	time_t current_time = time(nullptr);

	for (DungeonInstancePlayerMap::const_iterator itr = this->m_DungeonInstancePlayerMap.begin(); itr != this->m_DungeonInstancePlayerMap.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_DUNGEON_INSTANCE_UPDATE);
		stmt->setUInt32(0, itr->first);
		stmt->setUInt8(1, this->GetID());
		stmt->setInt64(2, current_time);
		stmt->setUInt8(3, this->GetID());
		stmt->setInt64(4, current_time);
		trans->Append(stmt);

		sDungeon->SaveInstance(itr->first, this->GetID(), current_time);
	}

	MuDatabase.CommitTransaction(trans);
}

int32 DungeonInstance::GetCountByPC(Player* pPlayer) const
{
	int32 count = 0;

	for (DungeonInstancePlayerMap::const_iterator itr = this->m_DungeonInstancePlayerMap.begin(); itr != this->m_DungeonInstancePlayerMap.end(); ++itr)
	{
		Player* pMember = itr->second;

		if (!pMember)
		{
			continue;
		}

		if (pMember->GetAccountData()->GetDiskSerial() == pPlayer->GetAccountData()->GetDiskSerial() &&
			!memcmp(pMember->GetAccountData()->GetMac(), pPlayer->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH))
		{
			++count;
		}
	}

	return count;
}

void DungeonInstance::CreateSimpleTrap()
{
	RandomValue<Player*> m_RandomValue(nullptr);

	for (DungeonInstancePlayerMap::const_iterator itr = this->m_DungeonInstancePlayerMap.begin(); itr != this->m_DungeonInstancePlayerMap.end(); ++itr)
	{
		m_RandomValue.AddValue(itr->second, 0);
	}

	Player* pPlayer = m_RandomValue.GetRandomValue(RANDOM_POOL_RATE);

	if (!pPlayer)
	{
		return;
	}

	this->CreateTrap(pPlayer->GetX(), pPlayer->GetY(), 0);
}

void DungeonInstance::CreateTrap(int16 x, int16 y, uint8 type)
{
	DungeonInstanceData const* pData = sDungeon->GetInstanceData(this->GetID());

	if (!pData)
	{
		return;
	}

	Monster* pMonster = sObjectMgr->MonsterTryAdd(5004, pData->GetWorld());

	if (pMonster)
	{
		pMonster->SetWorldId(pData->GetWorld());
		pMonster->SetDirection(2);
		pMonster->SetBasicLocation(x, y, x, y);
		pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		pMonster->SetDespawnTime(10000);
		pMonster->SetDespawnTick(MyGetTickCount());
		pMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
		pMonster->SetScriptName("ai_instanced_dungeon_trap");
		pMonster->AddAdditionalDataInt(0, this->GetInstance());
		pMonster->AddAdditionalDataInt(1, type);

		pMonster->SetInstance(this->GetInstance());
		pMonster->AddToWorld();

		sLog->outInfo("instanced_dungeon", "%s (%d) :: Created Trap [%u][%u].", __FUNCTION__, this->GetInstance(), pMonster->GetEntry(), pMonster->GetClass());
	}
}