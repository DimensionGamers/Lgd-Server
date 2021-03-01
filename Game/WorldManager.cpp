WorldMgr::WorldMgr()
{
	
}

WorldMgr::~WorldMgr()
{
	this->world_server_map.clear();
	MAP_CLEAR(WorldExperienceMap::iterator, this->world_experience_map);
	MAP_CLEAR(WorldMap::iterator, this->world_map);
	LIST_CLEAR(WorldFreePKList::iterator, this->m_WorldFreePKList);
}

void WorldMgr::LoadWorldList()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Worlds...");

	std::map<uint16, WorldStatusData> WorldStatusList;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		if ( itr->second->GetStatus() < MAX_WORLD_STATUS )
		{
			WorldStatusList[itr->first].SetStatus(itr->second->GetStatus());

			for ( int32 i = 0; i < WORLD_MAX_SIZE; ++i )
			{
				WorldStatusList[itr->first].GetGrid(i)->attribute = itr->second->WorldStatus[itr->second->GetStatus()].GetGrid(i)->attribute;
			}
		}
	}

	uint32 count = 0;
		
	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_template ORDER BY entry ASC, state ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			world_type entry = fields[0].GetUInt16();
			world_type display_entry = fields[1].GetUInt16();
			uint8 status = fields[2].GetUInt8();
			std::string file = "World/" + fields[3].GetString();
			uint16 spawn_gate = fields[5].GetUInt16();
			uint32 experience = fields[6].GetUInt32();
			uint32 zen_rate = fields[7].GetUInt32();
			uint32 flags = fields[8].GetUInt32();

			if ( status >= MAX_WORLD_STATUS )
				continue;

			World* pWorld = this->GetWorld(entry);

			if ( !pWorld )
			{
				++count;
				pWorld = new World;
				this->world_map[entry] = pWorld;
			}

			pWorld->SetEntry(entry);
			pWorld->SetDisplay(display_entry);

			WorldStatusData & WorldStatusAdd = pWorld->WorldStatus[status];

			if ( WorldStatusAdd.LoadWorld(file.c_str()) )
			{
				WorldStatusAdd.SetStatus(status);
				WorldStatusAdd.SetName(fields[4].GetCString());
				WorldStatusAdd.SetSpawnGate(spawn_gate);
				WorldStatusAdd.SetExperienceRate(experience);
				WorldStatusAdd.SetZenRate(zen_rate);
				WorldStatusAdd.SetFlags(flags);
				WorldStatusAdd.SetActive(true);
			}
		}
		while(result->NextRow());
	}

	for ( std::map<uint16, WorldStatusData>::const_iterator itr = WorldStatusList.begin(); itr != WorldStatusList.end(); ++itr )
	{
		World* pWorld = this->GetWorld(itr->first);

		if ( pWorld )
		{
			pWorld->statusChange(itr->second.GetStatus());

			for ( int32 i = 0; i < WORLD_MAX_SIZE; ++i )
			{
				pWorld->WorldStatus[itr->second.GetStatus()].GetGrid(i)->attribute = itr->second.GetGrid(i)->attribute;
			}
		}
	}

	WorldStatusList.clear();

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u worlds", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Worlds...");

	uint32 count = 0;
		
	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_template ORDER BY entry ASC, state ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			world_type entry = fields[0].GetUInt16();
			world_type display_entry = fields[1].GetUInt16();
			uint8 status = fields[2].GetUInt8();
			std::string file = "World/" + fields[3].GetString();
			uint16 spawn_gate = fields[5].GetUInt16();
			uint32 experience = fields[6].GetUInt32();
			uint32 zen_rate = fields[7].GetUInt32();
			uint32 flags = fields[8].GetUInt32();

			if ( status >= MAX_WORLD_STATUS )
			{
				continue;
			}

			World* pWorld = this->GetWorld(entry);

			if ( !pWorld )
			{
				continue;
			}

			pWorld->SetEntry(entry);
			pWorld->SetDisplay(display_entry);

			WorldStatusData & WorldStatusAdd = pWorld->WorldStatus[status];
			WorldStatusAdd.SetStatus(status);
			WorldStatusAdd.SetName(fields[4].GetCString());
			WorldStatusAdd.SetSpawnGate(spawn_gate);
			WorldStatusAdd.SetExperienceRate(experience);
			WorldStatusAdd.SetZenRate(zen_rate);
			WorldStatusAdd.SetFlags(flags);
			WorldStatusAdd.SetActive(true);
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded worlds");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAIPath()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World AI Path...");

	uint32 count = 0;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		LIST_CLEAR(WorldAIPathList::iterator, pWorld->m_path_list);

		QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_ai_path WHERE world = '%u' ORDER BY id ASC", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldAIPath * ai_path = new WorldAIPath;
				ai_path->SetID(fields[1].GetUInt32());
				ai_path->SetType(fields[2].GetUInt8());
				ai_path->SetX(fields[3].GetInt32());
				ai_path->SetY(fields[4].GetInt32());

				pWorld->m_path_list.push_back(ai_path);

				++count;
			}
			while(result->NextRow());
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world path definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldBuff()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Buff...");

	uint32 count = 0;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		MAP_CLEAR(WorldBuffMap::iterator, pWorld->m_buff);

		QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_buff WHERE world = '%u' ORDER BY buff ASC", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldBuff * buff = new WorldBuff;
				buff->SetBuff(fields[1].GetUInt16());
				buff->SetFlag(fields[2].GetUInt8());
				
				pWorld->m_buff[buff->GetBuff()] = buff;

				++count;
			}
			while(result->NextRow());
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world buff definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAreaRestriction()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Area Restriction...");

	uint32 count = 0;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		LIST_CLEAR(WorldAreaList::iterator, pWorld->m_area);

		QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_area_restriction WHERE world = '%u'", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldArea * area = new WorldArea;
				area->SetX(fields[1].GetInt32());
				area->SetY(fields[2].GetInt32());
				area->SetRange(fields[3].GetUInt8());
				area->SetFlags(fields[4].GetUInt8());
				
				pWorld->m_area.push_back(area);

				++count;
			}
			while(result->NextRow());
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world area restriction definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldServer()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Server...");

	this->world_server_map.clear();

	uint32 count = 0;

	QueryResult result = LoginDatabase.Query("SELECT * FROM world_server");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint16 server = fields[0].GetUInt16();
			world_type world = fields[1].GetUInt16();

			if ( (server / MAX_SERVER_PER_GROUP) != sGameServer->GetServerGroup() )
				continue;

			this->world_server_map[server].insert(world);

			++count;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world server definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAttribute()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Attribute...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM world_attribute");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			World* pWorld = this->GetWorld(fields[0].GetUInt16());
			uint8 status = fields[1].GetUInt8();

			if ( status >= MAX_WORLD_STATUS )
			{
				continue;
			}

			if ( !pWorld )
			{
				continue;
			}

			const char* data = fields[2].GetCString();
				
			if ( data )
			{
				Tokens tokens(data, ' ');
					
				for ( int32 i = 0; i < WORLD_MAX_SIZE; ++i )
				{
					pWorld->WorldStatus[status].GetGrid(i)->attribute = static_cast<uint8>(atoi(tokens[i]));
				}
			}

			++count;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world attribute definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldExperience()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Experience...");

	MAP_CLEAR(WorldExperienceMap::iterator, this->world_experience_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM world_experience");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			world_experience * data = new world_experience;
			data->SetWorld(fields[loop++].GetUInt16());
			
			for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
			{
				data->SetRate(i, fields[loop++].GetInt32());
			}

			this->world_experience_map[data->GetWorld()] = data;
			++count;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world experience definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAreaAttribute()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Area Attribute...");

	uint32 count = 0;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		LIST_CLEAR(WorldAreaAttributeList::iterator, pWorld->m_attribute);

		QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_area_attribute WHERE world_id = '%u'", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldAreaAttribute * area = new WorldAreaAttribute;
				area->SetX1(fields[1].GetInt16());
				area->SetY1(fields[2].GetInt16());
				area->SetX2(fields[3].GetInt16());
				area->SetY2(fields[4].GetInt16());
				area->SetAttribute(fields[5].GetUInt8());
				area->SetApply(fields[6].GetBool());
				area->SetSend(fields[7].GetBool());
				
				pWorld->m_attribute.push_back(area);

				coord_type coord[4] = { area->GetX1(), area->GetY1(), area->GetX2(), area->GetY2() };
				pWorld->ApplyAttribute(coord, area->GetAttribute(), area->IsApply());

				++count;
			}
			while(result->NextRow());
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world area attribute definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldFreePK()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Free PK...");

	LIST_CLEAR(WorldFreePKList::iterator, this->m_WorldFreePKList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM world_free_pk");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			WorldFreePK * pData = new WorldFreePK;
			pData->SetWorld(fields[loop++].GetUInt16());
			pData->SetX1(fields[loop++].GetInt16());
			pData->SetY1(fields[loop++].GetInt16());
			pData->SetX2(fields[loop++].GetInt16());
			pData->SetY2(fields[loop++].GetInt16());
			
			this->m_WorldFreePKList.push_back(pData);
			++count;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world free pk definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::Update()
{
	for ( WorldMap::iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( pWorld )
		{
			pWorld->Update();
			pWorld->UpdateItemQueue();
		}
	}
}

void WorldMgr::UpdateWeather()
{
	for ( WorldMap::iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( pWorld )
		{
			pWorld->UpdateWeather();
		}
	}
}

void WorldMgr::statusChange(uint16 entry, uint8 status)
{
	if ( status >= MAX_WORLD_STATUS )
	{
		return;
	}

	World* pWorld = this->GetWorld(entry);

	if ( !pWorld )
	{
		return;
	}

	pWorld->statusChange(status);
}

void WorldMgr::MakeItemVisible(Player* pPlayer)
{
	for ( WorldMap::iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( pWorld )
		{
			pWorld->MakeItemVisible(pPlayer);
		}
	}
}

uint16 WorldMgr::AllowMoveToWorld(Player* pPlayer, world_type world)
{
	if ( !pPlayer )
		return -1;

	WorldServerMap::const_iterator itrr = this->world_server_map.find(sGameServer->GetServerCode());

	if ( itrr != this->world_server_map.end() ) // Si el server tiene mapas y no están en la lista entonces no me puedo mover
	{
		WorldServerList::const_iterator it = itrr->second.find(world);

		if ( it != itrr->second.end() )
			return sGameServer->GetServerCode();
	}

	for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
	{
		for ( WorldServerList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			if ( (*it) == world ) // Busco el mapa en la lista
			{
				return itr->first;
			}
		}
	}

	if ( itrr != this->world_server_map.end() )
	{
		return -1;
	}

	return sGameServer->GetServerCode();
}

bool WorldMgr::AllowMoveToWorld(Player* pPlayer, world_type world, uint16 server)
{
	if ( !pPlayer )
		return false;

	WorldServerMap::const_iterator itr = this->world_server_map.find(server);

	if ( itr != this->world_server_map.end() )
	{
		for ( WorldServerList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			if ( (*it) == world )
			{
				return true;
			}
		}

		return false;
	}

	return true;
}

bool WorldMgr::IsWorldAllowed(world_type world)
{
	for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
	{
		for ( WorldServerList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			if ( (*it) == world ) // Busco el mapa en la lista
			{
				if ( itr->first == sGameServer->GetServerCode() ) // Solamente puedo moverme si el mapa pertenece al mismo server
					return true;

				return false;
			}
		}
	}

	// El mapa no está en la lista
	WorldServerMap::const_iterator itr = this->world_server_map.find(sGameServer->GetServerCode());

	if ( itr != this->world_server_map.end() ) // Si el server tiene mapas y no están en la lista entonces no me puedo mover
	{
		return false;
	}

	return true;
}

bool WorldMgr::IsWorld(world_type world)
{
	return this->GetWorld(world) != nullptr;
}

World* WorldMgr::GetWorld(world_type world)
{
	WorldMap::iterator itr = this->world_map.find(world);

	if ( itr != this->world_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

bool g_UrukMountainBlockStatus = false;
bool g_FereaBlockStatus = false;

void WorldMgr::UpdateUrukMountainZone(bool apply)
{
	if ( g_UrukMountainBlockStatus == apply )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_URUK_MOUNTAIN);

	if ( !pWorld )
	{
		return;
	}

	for ( int32 i = 0; i < URUK_MOUNTAIN_BLOCK_ZONE; ++i )
	{
		coord_type coords[4] = { g_UrukMountainBlock[i].x1, g_UrukMountainBlock[i].y1, g_UrukMountainBlock[i].x2, g_UrukMountainBlock[i].y2 };
		pWorld->ApplyAttribute(coords, URUK_MOUNTAIN_BLOCK_ATTRIBUTE, apply);
	}

	g_UrukMountainBlockStatus = apply;

	this->SendUrukMountainZone();
}
	
void WorldMgr::SendUrukMountainZone()
{
	World * pWorld = this->GetWorld(WORLD_URUK_MOUNTAIN);

	if ( !pWorld )
	{
		return;
	}

	WorldPlayerMap const& player_map = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		this->SendUrukMountainZone(pPlayer);
	}
}
	
void WorldMgr::SendUrukMountainZone(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_URUK_MOUNTAIN )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_URUK_MOUNTAIN);

	if ( !pWorld )
	{
		return;
	}

	coord_type coords[4 * URUK_MOUNTAIN_BLOCK_ZONE];

	for ( int32 i = 0; i < URUK_MOUNTAIN_BLOCK_ZONE; ++i )
	{
		coords[(i * 4) + 0] = g_UrukMountainBlock[i].x1;
		coords[(i * 4) + 1] = g_UrukMountainBlock[i].y1;
		coords[(i * 4) + 2] = g_UrukMountainBlock[i].x2;
		coords[(i * 4) + 3] = g_UrukMountainBlock[i].y2;
	}

	pPlayer->SendWorldAttribute(0, URUK_MOUNTAIN_BLOCK_ATTRIBUTE, URUK_MOUNTAIN_BLOCK_ZONE, g_UrukMountainBlockStatus, coords);
}

void WorldMgr::UpdateFereaZone(bool apply)
{
	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr )
	{
		Player* pPlayer = itr->second;

		if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != WORLD_FEREA )
		{
			continue;
		}

		if ( (apply && pPlayer->GetWorldState() == 1) || (!apply && pPlayer->GetWorldState() == 0) )
		{
			continue;
		}

		pPlayer->SetWorldState(apply ? 1 : 0);
		FEREA_DOOR_OPEN_CLOSE pMsg(apply ? 1 : 0);
		pPlayer->SEND_PCT(pMsg);
	}

	if ( g_FereaBlockStatus == apply )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_FEREA);

	if ( !pWorld )
	{
		return;
	}

	for ( int32 i = 0; i < FEREA_BLOCK_ZONE; ++i )
	{
		coord_type coords[4] = { g_FereaBlock[i].x1, g_FereaBlock[i].y1, g_FereaBlock[i].x2, g_FereaBlock[i].y2 };
		pWorld->ApplyAttribute(coords, FEREA_BLOCK_ATTRIBUTE, apply);
	}

	g_FereaBlockStatus = apply;

	this->SendFereaZone();
}

void WorldMgr::SendFereaZone()
{
	World * pWorld = this->GetWorld(WORLD_FEREA);

	if ( !pWorld )
	{
		return;
	}

	WorldPlayerMap const& player_map = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		this->SendFereaZone(pPlayer);
	}
}

void WorldMgr::SendFereaZone(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_FEREA )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_FEREA);

	if ( !pWorld )
	{
		return;
	}

	coord_type coords[4 * FEREA_BLOCK_ZONE];

	for ( int32 i = 0; i < FEREA_BLOCK_ZONE; ++i )
	{
		coords[(i * 4) + 0] = g_FereaBlock[i].x1;
		coords[(i * 4) + 1] = g_FereaBlock[i].y1;
		coords[(i * 4) + 2] = g_FereaBlock[i].x2;
		coords[(i * 4) + 3] = g_FereaBlock[i].y2;
	}

	pPlayer->SendWorldAttribute(0, FEREA_BLOCK_ATTRIBUTE, FEREA_BLOCK_ZONE, g_FereaBlockStatus, coords);
}

int32 WorldMgr::GetWorldExperienceRate(world_type world, uint8 count)
{
	if ( count == 0 )
	{
		return 0;
	}

	count = count - 1;

	if ( count >= MAX_PARTY_MEMBERS )
	{
		return 0;
	}
	
	WorldExperienceMap::const_iterator it = this->world_experience_map.find(world);

	if ( it != this->world_experience_map.end() )
	{
		return it->second->GetRate(count);
	}
	else
	{
		return 0;
	}
}

void WorldMgr::SetPKBoss(world_type world, bool enabled)
{
	World * pWorld = this->GetWorld(world);

	if ( !pWorld )
	{
		return;
	}

	pWorld->SetPKBossEnabled(enabled);
	pWorld->GetPKBossTime()->Start();
}

bool WorldMgr::IsPKBoss(world_type world)
{
	if ( !sGameServer->IsPKBossEnabled() )
	{
		return false;
	}

	World * pWorld = this->GetWorld(world);

	if ( !pWorld )
	{
		return false;
	}

	if ( pWorld->flagHas(WORLD_FLAG_NO_LIMIT_PK) )
	{
		return true;
	}

	return pWorld->IsPKBossEnabled();
}

bool WorldMgr::IsItemDropAllowed(Player* pPlayer, World* pWorld, coord_type x, coord_type y)
{
	if ( !pPlayer || !pWorld )
	{
		return false;
	}

	WorldGrid const& attr = pWorld->GetGrid(x, y);

	if ( attr.IsLocked_1() || attr.IsLocked_2() )
	{
		return false;
	}

	if ( pWorld->IsAreaRestriction(x, y, WORLD_AREA_FLAG_ITEM_DROP) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to drop items on this area.");
		return false;
	}

	return true;
}

bool WorldMgr::IsFreePK(world_type world, coord_type x, coord_type y) const
{
	for (WorldFreePKList::const_iterator itr = this->m_WorldFreePKList.begin(); itr != this->m_WorldFreePKList.end(); ++itr)
	{
		WorldFreePK const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetWorld() != world)
		{
			continue;
		}

		if ((x >= pData->GetX1() && x <= pData->GetX2()) &&
			(y >= pData->GetY1() && y <= pData->GetY2()))
		{
			return true;
		}
	}

	return false;
}