/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "TeleportMgr.cpp"
*
*/

TeleportMgr::~TeleportMgr()
{
	MAP_CLEAR(GateDataMap::iterator, this->gate_template_map);
	MAP_CLEAR(TeleportDataMap::iterator, this->teleport_template_map);
	LIST_CLEAR(MoveLevelList::iterator, this->move_level_list);
}

void TeleportMgr::LoadGateTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Gate...");

	MAP_CLEAR(GateDataMap::iterator, this->gate_template_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM gate_template");

	if ( result )
	{
		do
		{
			FieldReader reader(result->Fetch());

			GateData * add_gate = new GateData;

			add_gate->id = reader.GetUInt16();
			add_gate->flag = reader.GetUInt8();
			add_gate->world = reader.GetUInt16();
			add_gate->x1 = reader.GetInt16();
			add_gate->y1 = reader.GetInt16();
			add_gate->x2 = reader.GetInt16();
			add_gate->y2 = reader.GetInt16();
			add_gate->target_id = reader.GetUInt16();
			add_gate->direction = reader.GetUInt8();
			add_gate->min_level = reader.GetInt16();
			add_gate->SetDescription(reader.GetString().c_str());

			if ( add_gate->flag >= MAX_GATE_FLAG )
			{
				add_gate->flag = GATE_FLAG_NORMAL;
			}

			if ( !sWorldMgr->IsWorld(add_gate->world) )
			{
				sLog->outError(LOG_DEFAULT, "Wrong Gate Loaded :%u - World: %u", add_gate->id, add_gate->world);
				delete add_gate;
				continue;
			}

			this->gate_template_map[add_gate->id] = add_gate;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u gate definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void TeleportMgr::LoadTeleportTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Teleport...");

	MAP_CLEAR(TeleportDataMap::iterator, this->teleport_template_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM teleport_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			TeleportData * add_teleport = new TeleportData;

			add_teleport->id = fields[0].GetUInt8();
			add_teleport->SetName(fields[1].GetString());
			add_teleport->zen = fields[2].GetUInt32();
			add_teleport->min_level = fields[3].GetInt16();
			add_teleport->gate = fields[4].GetUInt16();
			add_teleport->SetMonster(fields[5].GetUInt16());

			if ( !this->GetGate(add_teleport->gate) )
			{
				sLog->outError(LOG_DEFAULT, "Wrong Teleport Loaded :%u - Gate: %u", add_teleport->id, add_teleport->gate);
				delete add_teleport;
				continue;
			}

			this->teleport_template_map[add_teleport->id] = add_teleport;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u teleport definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void TeleportMgr::LoadMoveLevel()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Move Level...");

	LIST_CLEAR(MoveLevelList::iterator, this->move_level_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM move_level");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			MoveLevel * add_move_level = new MoveLevel;

			add_move_level->level = fields[0].GetInt16();
			add_move_level->world = fields[1].GetUInt16();
			add_move_level->x1 = fields[2].GetInt16();
			add_move_level->y1 = fields[3].GetInt16();
			add_move_level->x2 = fields[4].GetInt16();
			add_move_level->y2 = fields[5].GetInt16();
			add_move_level->SetDisabled(fields[6].GetBool());
			
			this->move_level_list.push_back(add_move_level);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u move level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

GateData const* TeleportMgr::GetGate(uint16 id, bool target)
{
	GateDataMap::const_iterator it = this->gate_template_map.find(id);

	if ( it != this->gate_template_map.end() )
	{
		return target && it->second->target_id ? this->GetGate(it->second->target_id): it->second;
	}
	else
	{
		return nullptr;
	}
}

TeleportData const* TeleportMgr::GetTeleport(uint8 id) const
{
	TeleportDataMap::const_iterator it = this->teleport_template_map.find(id);

	if (it != this->teleport_template_map.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

TeleportData const* TeleportMgr::GetTeleport(std::string map) const
{
	strToLower(map);

	for (TeleportDataMap::const_iterator it = this->teleport_template_map.begin(); it != this->teleport_template_map.end(); ++it)
	{
		StringLower(list_map, it->second->GetName());

		if (map != list_map)
		{
			continue;
		}

		return it->second;
	}

	return nullptr;
}

int16 TeleportMgr::GetMoveLevel(world_type world, coord_type x, coord_type y, bool special)
{
	int16 level = -1;

	for (MoveLevelList::const_iterator it = this->move_level_list.begin(); it != this->move_level_list.end(); ++it)
	{
		if ((*it)->world != world)
		{
			continue;
		}

		if ((x >= (*it)->x1 && x <= (*it)->x2) && (y >= (*it)->y1 && y <= (*it)->y2))
		{
			level = (*it)->level;

			if ((*it)->IsDisabled())
			{
				return -1;
			}
		}
	}

	return level;
}

void TeleportMgr::Update()
{
	if ( !sGameServer->IsGateHandle() )
	{
		return;
	}

	for ( GateDataMap::const_iterator itr = this->gate_template_map.begin(); itr != this->gate_template_map.end(); ++itr )
	{
		GateData const* pGate = itr->second;

		if ( !pGate )
		{
			continue;
		}

		if ( pGate->flag != GATE_FLAG_IN )
		{
			continue;
		}

		World const* pWorld = sWorldMgr->GetWorld(pGate->world);

		if ( !pWorld )
		{
			continue;
		}

		WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();

		if ( wplayers.empty() )
		{
			continue;
		}

		for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			if ( !pPlayer->IsPlaying() || !pPlayer->IsLive() )
			{
				continue;
			}

			if ( pPlayer->GetRegenStatus() != REGEN_NONE )
			{
				continue;
			}

			if ( pPlayer->IsBusy() )
			{
				continue;
			}

			if ( pPlayer->IsOffline() )
			{
				continue;
			}

			/*if( pPlayer->GetX() < (pGate->GetMinX() - 5) || pPlayer->GetX() > (pGate->GetMaxX() + 5) || pPlayer->GetY() < (pGate->GetMinY() - 5) || pPlayer->GetY() > (pGate->GetMaxY() + 5))
			{
				continue;
			}*/

			if( pPlayer->GetX() < pGate->GetMinX() || pPlayer->GetX() > pGate->GetMaxX() || pPlayer->GetY() < pGate->GetMinY() || pPlayer->GetY() > pGate->GetMaxY() )
			{
				continue;
			}

			int32 level = pGate->min_level;

			if( pGate->min_level != -1 && pPlayer->GetTotalLevel() < level )
			{
				continue;
			}

			pPlayer->MoveToGate(pGate->id);
		}
	}
}

void TeleportMgr::SendBattleZone(Player* pPlayer, world_type world)
{
	if ( !pPlayer )
		return;

	World* pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
	{
		return;
	}

	uint8 buffer[255];
	POINTER_PCT(GEN_BATTLE_LIST, head, buffer, 0);
	POINTER_PCT(uint8, body, buffer, sizeof(GEN_BATTLE_LIST));

	head->battlezone = pWorld->flagHas(WORLD_FLAG_BATTLE_ZONE);
	head->count = 0;

	for ( TeleportDataMap::const_iterator it = this->teleport_template_map.begin(); it != this->teleport_template_map.end(); ++it )
	{
		GateData const* pGate = this->GetGate(it->second->gate);

		if ( !pGate )
			continue;

		World* pWorldData = sWorldMgr->GetWorld(pGate->world);

		if ( !pWorldData )
		{
			continue;
		}

		if ( pWorldData->flagHas(WORLD_FLAG_BATTLE_ZONE) )
		{
			body[head->count] = it->second->id;
			++head->count;
		}
	}

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_BATTLEZONE_LIST, sizeof(GEN_BATTLE_LIST) + (head->count * sizeof(uint8)));

	pPlayer->sendPacket(buffer, head->h.get_size());
}

void TeleportMgr::SendMonsterInfo(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(PACKET_WARP_MONSTER_INFO_HEAD, head, buffer, 0);
	POINTER_PCT(PACKET_WARP_MONSTER_INFO_BODY, body, buffer, sizeof(PACKET_WARP_MONSTER_INFO_HEAD));
	head->count = 0;

	for (TeleportDataMap::const_iterator itr = this->teleport_template_map.begin(); itr != this->teleport_template_map.end(); ++itr)
	{
		TeleportData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetMonster() == uint16(-1))
		{
			continue;
		}

		monster_template const* pMonster = sMonsterMgr->GetMonsterTemplate(pData->GetMonster());

		if (!pMonster)
		{
			continue;
		}

		body[head->count].id = pData->id;
		body[head->count].monster = pData->GetMonster();
		body[head->count].hp = pMonster->power[POWER_LIFE].get();
		body[head->count].attack_power = pMonster->attack_max_damage.get();
		body[head->count].defense = pMonster->defense.get();
		body[head->count].elemental_damage = pMonster->GetElementalDamageMax();
		body[head->count].elemental_defense = pMonster->GetElementalDefense();

		++head->count;
	}

	head->Set(0x59, 0x06, sizeof(PACKET_WARP_MONSTER_INFO_HEAD) + (head->count * sizeof(PACKET_WARP_MONSTER_INFO_BODY)));

	pPlayer->sendPacket(buffer);
}