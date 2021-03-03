/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "TeleportManager.cpp"
*
*/

TeleportManager::TeleportManager()
{

}

TeleportManager::~TeleportManager()
{
	CLEAR_MAP(_gates);
	CLEAR_MAP(_teleports);
	CLEAR_LIST(_moveLevels);
}

void TeleportManager::LoadGateTemplate()
{
	sLog->outLoad(true, "Loading Gate...");

	CLEAR_MAP(_gates);

	auto result = GameServerDatabase.Query("SELECT * FROM gate_template");
	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			GateData * new_data = new GateData;

			new_data->Id = reader.GetUInt16();
			new_data->Flag = reader.GetUInt8();
			new_data->MapId = reader.GetUInt16();
			new_data->X1 = reader.GetInt16();
			new_data->Y1 = reader.GetInt16();
			new_data->X2 = reader.GetInt16();
			new_data->Y2 = reader.GetInt16();
			new_data->TargetId = reader.GetUInt16();
			new_data->Direction = reader.GetUInt8();
			new_data->MinLevel = reader.GetInt16();
			new_data->Description = reader.GetString();

			if (new_data->Flag >= MAX_GATE_FLAG)
				new_data->Flag = GATE_FLAG_NORMAL;

			if (!sWorldMgr->IsWorld(new_data->MapId))
			{
				sLog->outError(LOG_DEFAULT, "Wrong Gate Loaded :%u - World: %u", new_data->Id, new_data->MapId);
				delete new_data;
				continue;
			}

			_gates[new_data->Id] = new_data;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u gate definitions", _gates.size());
}

void TeleportManager::LoadTeleportTemplate()
{
	sLog->outLoad(true, "Loading Teleport...");

	CLEAR_MAP(_teleports);

	auto result = GameServerDatabase.Query("SELECT * FROM teleport_template");
	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			TeleportData * new_data = new TeleportData;

			new_data->Id = reader.GetUInt8();
			new_data->Name = reader.GetString();
			new_data->Zen = reader.GetUInt32();
			new_data->MinLevel = reader.GetInt16();
			new_data->Gate = reader.GetUInt16();
			new_data->MonsterId = reader.GetUInt16();

			if (!this->GetGate(new_data->Gate))
			{
				sLog->outError(LOG_DEFAULT, "Wrong Teleport Loaded :%u - Gate: %u", new_data->Id, new_data->Gate);
				delete new_data;
				continue;
			}

			_teleports[new_data->Id] = new_data;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u teleport definitions", _teleports.size());
}

void TeleportManager::LoadMoveLevel()
{
	sLog->outLoad(true, "Loading Move Level...");

	CLEAR_LIST(_moveLevels);

	auto result = GameServerDatabase.Query("SELECT * FROM move_level");
	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			MoveLevel * new_data = new MoveLevel;

			new_data->Level = reader.GetInt16();
			new_data->MapId = reader.GetUInt16();
			new_data->X1 = reader.GetInt16();
			new_data->Y1 = reader.GetInt16();
			new_data->X2 = reader.GetInt16();
			new_data->Y2 = reader.GetInt16();
			new_data->Disabled = reader.GetBool();

			_moveLevels.push_back(new_data);
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u move level definitions", _moveLevels.size());
}

GateData const* TeleportManager::GetGate(uint16 id, bool target) const
{
	auto const itr = _gates.find(id);
	if (itr != _gates.end())
		return target && itr->second->TargetId ? GetGate(itr->second->TargetId) : itr->second;
	else
		return nullptr;
}

TeleportData const* TeleportManager::GetTeleport(uint8 id) const
{
	auto const itr = _teleports.find(id);
	if (itr != _teleports.end())
		return itr->second;
	else
		return nullptr;
}

TeleportData const* TeleportManager::GetTeleport(std::string const& name) const
{
	StringLower(lower_name, name);
	
	for (auto const itr : _teleports)
	{
		auto teleport_data = itr.second;

		StringLower(list_map, teleport_data->Name);
		if (lower_name != list_map)
			continue;

		return teleport_data;
	}

	return nullptr;
}

int16 TeleportManager::GetMoveLevel(uint16 mapId, int16 x, int16 y) const
{
	int16 level = -1;

	for (auto const move_data : _moveLevels)
	{
		if (move_data->MapId != mapId)
			continue;

		if ((x >= move_data->X1 && x <= move_data->X2) && (y >= move_data->Y1 && y <= move_data->Y2))
		{
			level = move_data->Level;

			if (move_data->Disabled)
				return -1;
		}
	}

	return level;
}

void TeleportManager::Update()
{
	if (!sGameServer->IsGateHandle())
		return;

	for (auto const itr : _gates)
	{
		auto const gate = itr.second;
		if (!gate)
			continue;

		if (gate->Flag != GATE_FLAG_IN)
			continue;

		auto map = sWorldMgr->GetWorld(gate->MapId);
		if (!map)
			continue;

		auto const& players = map->GetAllPlayers();
		if (players.empty())
			continue;

		for (auto it : players)
		{
			auto player = it.second;
			if (!player)
				continue;

			if (!player->IsPlaying() || !player->IsLive())
				continue;

			if (player->GetRegenStatus() != REGEN_NONE)
				continue;

			if (player->IsBusy())
				continue;

			if (player->IsOffline())
				continue;

			if (player->GetX() < gate->GetMinX() || player->GetX() > gate->GetMaxX() || player->GetY() < gate->GetMinY() || player->GetY() > gate->GetMaxY())
				continue;

			if (gate->MinLevel != -1 && player->GetTotalLevel() < gate->MinLevel)
				continue;

			player->MoveToGate(gate->Id);
		}
	}
}

void TeleportManager::SendBattleZone(Player* player, uint16 mapId)
{
	if (!player)
		return;

	auto map = sWorldMgr->GetWorld(mapId);
	if (!map)
		return;

	uint8 buffer[255];
	POINTER_PCT(GEN_BATTLE_LIST, head, buffer, 0);
	POINTER_PCT(uint8, body, buffer, sizeof(GEN_BATTLE_LIST));

	head->battlezone = map->flagHas(WORLD_FLAG_BATTLE_ZONE);
	head->count = 0;

	for (auto const itr : _teleports)
	{
		auto const teleport_data = itr.second;
		if (!teleport_data)
			continue;

		auto const gate = GetGate(teleport_data->Gate);
		if (!gate)
			continue;

		auto gmap = sWorldMgr->GetWorld(gate->MapId);
		if (!gmap)
			continue;

		if (gmap->flagHas(WORLD_FLAG_BATTLE_ZONE))
			body[head->count++] = teleport_data->Id;
	}

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_BATTLEZONE_LIST, sizeof(GEN_BATTLE_LIST)+(head->count * sizeof(uint8)));

	player->sendPacket(buffer, head->h.get_size());
}

void TeleportManager::SendMonsterInfo(Player* player)
{
	if (!player)
		return;

	uint8 buffer[8192];
	POINTER_PCT(PACKET_WARP_MONSTER_INFO_HEAD, head, buffer, 0);
	POINTER_PCT(PACKET_WARP_MONSTER_INFO_BODY, body, buffer, sizeof(PACKET_WARP_MONSTER_INFO_HEAD));
	head->count = 0;

	for (auto itr : _teleports)
	{
		auto const teleport_data = itr.second;
		if (!teleport_data)
			continue;

		if (teleport_data->MonsterId == uint16(-1))
			continue;

		auto const monster_data = sMonsterMgr->GetMonsterTemplate(teleport_data->MonsterId);
		if (!monster_data)
			continue;

		auto & body_data = body[head->count++];

		body_data.id = teleport_data->Id;
		body_data.monster = teleport_data->MonsterId;
		body_data.hp = monster_data->power[POWER_LIFE].get();
		body_data.attack_power = monster_data->attack_max_damage.get();
		body_data.defense = monster_data->defense.get();
		body_data.elemental_damage = monster_data->GetElementalDamageMax();
		body_data.elemental_defense = monster_data->GetElementalDefense();
	}

	head->Set(0x59, 0x06, sizeof(PACKET_WARP_MONSTER_INFO_HEAD) + (head->count * sizeof(PACKET_WARP_MONSTER_INFO_BODY)));

	player->sendPacket(buffer);
}