/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterMgr.cpp"
*
*/

CMonsterMgr::CMonsterMgr()
{
	this->SetLastUpdate(0);
}

CMonsterMgr::~CMonsterMgr()
{
	this->ClearItem();

	MAP_CLEAR(MonsterTemplateMap::iterator, this->map_template);
	MAP_CLEAR(MonsterAIUnitMap::iterator, this->m_monster_ai_unit);
	MAP_CLEAR(MonsterAIElementMap::iterator, this->m_monster_ai_element);
	MAP_CLEAR(MonsterAIAutomataMap::iterator, this->m_monster_ai_automata);

	for ( MonsterSkillMap::iterator it = this->monster_skill_map.begin(); it != this->monster_skill_map.end(); ++it )
		it->second.clear();

	this->monster_skill_map.clear();

	for ( MonsterSkillSpecialMap::iterator it = this->monster_skill_special_map.begin(); it != this->monster_skill_special_map.end(); ++it )
	{
		LIST_CLEAR(MonsterSkillSpecialList::iterator, it->second);
	}

	MAP_CLEAR(MonsterMap::iterator, this->monster_map);

	for ( MonsterAIGroupMap::iterator it = this->monster_ai_group_map.begin(); it != this->monster_ai_group_map.end(); it++ )
	{
		LIST_CLEAR(MonsterAIGroupList::iterator, it->second);
	}

	for ( MonsterRespawnLocationMap::iterator i_list = this->m_monster_respawn_location_map.begin(); i_list != this->m_monster_respawn_location_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterRespawnLocationList::iterator, i_list->second);
	}

	this->m_monster_respawn_location_map.clear();

	for ( MonsterEquipmentMap::iterator i_list = this->m_monster_equipment_map.begin(); i_list != this->m_monster_equipment_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterEquipmentList::iterator, i_list->second);
	}

	this->m_monster_equipment_map.clear();

	
	this->monster_skill_special_map.clear();
	this->monster_ai_group_map.clear();

	LIST_CLEAR(MonsterEventList::iterator, this->monster_event_list);
	MAP_CLEAR(MonsterRespawnMap::iterator, this->monster_respawn_map);

	LIST_CLEAR(MonsterExclusiveList::iterator, this->monster_exclusive_list);
}

void CMonsterMgr::LoadMonsterTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Template...");

	MAP_CLEAR(MonsterTemplateMap::iterator, this->map_template);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			monster_template * add_monster = new monster_template;

			add_monster->id.set(fields[loop++].GetUInt16());
			add_monster->SetName(fields[loop++].GetCString());
			add_monster->model.set(fields[loop++].GetUInt16());
			add_monster->size.set(fields[loop++].GetFloat());
			add_monster->type.set(fields[loop++].GetUInt8());
			add_monster->min_level.set(fields[loop++].GetInt16());
			add_monster->max_level.set(fields[loop++].GetInt16());

			for ( uint8 i = 0; i < POWER_MAX; i++ )
			{
				add_monster->power[i].set(fields[loop++].GetInt32());
			}

			add_monster->attack_min_damage.set(fields[loop++].GetInt32());
			add_monster->attack_max_damage.set(fields[loop++].GetInt32());
			add_monster->magic_min_damage.set(fields[loop++].GetInt32());
			add_monster->magic_max_damage.set(fields[loop++].GetInt32());
			add_monster->critical_damage_rate.set(fields[loop++].GetInt32());
			add_monster->critical_damage_add.set(fields[loop++].GetInt32());
			add_monster->excellent_damage_rate.set(fields[loop++].GetInt32());
			add_monster->excellent_damage_add.set(fields[loop++].GetInt32());
			add_monster->attack_success.set(fields[loop++].GetInt32());
			add_monster->defense.set(fields[loop++].GetInt32());
			add_monster->defense_magic.set(fields[loop++].GetInt32());
			add_monster->defense_success.set(fields[loop++].GetInt32());
			add_monster->move_range.set(fields[loop++].GetUInt32());
			add_monster->move_speed.set(fields[loop++].GetUInt32());
			add_monster->attack_range.set(fields[loop++].GetUInt32());
			add_monster->attack_speed.set(fields[loop++].GetInt32());
			add_monster->view_range.set(fields[loop++].GetUInt32());

			for ( uint8 i = 0; i < Element::MAX; i++ )
			{
				add_monster->SetResistance(i, fields[loop++].GetUInt8());
			}

			add_monster->respawn_time_min.set(fields[loop++].GetUInt32());
			add_monster->respawn_time_max.set(fields[loop++].GetUInt32());
			add_monster->item_rate.set(fields[loop++].GetInt32());
			add_monster->zen_rate.set(fields[loop++].GetInt32());
			add_monster->item_max_level.set(fields[loop++].GetInt32());

			for ( uint8 i = 0; i < POWER_MAX; i++ )
			{
				add_monster->regen_power[i] = fields[loop++].GetFloat();
				add_monster->regen_time[i].set(fields[loop++].GetUInt32());
			}

			add_monster->faction.set(fields[loop++].GetUInt8());
			add_monster->faction_level.set(fields[loop++].GetUInt8());

			add_monster->SetScriptName(fields[loop++].GetString());

			add_monster->SetElementalAttribute(fields[loop++].GetUInt8());
			add_monster->SetElementalPattern(fields[loop++].GetInt32());
			add_monster->SetElementalDefense(fields[loop++].GetInt32());
			add_monster->SetElementalDamageMin(fields[loop++].GetInt32());
			add_monster->SetElementalDamageMax(fields[loop++].GetInt32());
			add_monster->SetElementalAttackRate(fields[loop++].GetInt32());
			add_monster->SetElementalDefenseRate(fields[loop++].GetInt32());
			add_monster->SetRadianceImmune(fields[loop++].GetUInt8());
			add_monster->SetDebuffResistance(fields[loop++].GetInt32());
			add_monster->SetDebuffDefense(fields[loop++].GetInt32());
			add_monster->SetCriticalDamageResistance(fields[loop++].GetUInt8());
			add_monster->SetExcellentDamageResistance(fields[loop++].GetUInt8());
			add_monster->SetDamageAbsorb(fields[loop++].GetUInt8());
			add_monster->SetElite(fields[loop++].GetBool());

			if ( !sScriptAI->IsScriptAI(add_monster->GetScriptName()) )
			{
				sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
				add_monster->ResetScriptName();
			}

			if ( add_monster->attack_range > add_monster->view_range )
				add_monster->view_range.set(add_monster->attack_range.get());

			add_monster->custom.set(false);

			this->map_template[add_monster->id.get()] = add_monster;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monsters definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterTemplateCustom()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Template Custom...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_template_custom");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			monster_template * add_monster = new monster_template;

			add_monster->id.set(fields[loop++].GetUInt16());
			add_monster->SetName(fields[loop++].GetCString());
			add_monster->model.set(fields[loop++].GetUInt16());
			add_monster->size.set(fields[loop++].GetFloat());
			add_monster->type.set(fields[loop++].GetUInt8());
			add_monster->min_level.set(fields[loop++].GetInt16());
			add_monster->max_level.set(fields[loop++].GetInt16());

			for ( uint8 i = 0; i < POWER_MAX; i++ )
			{
				add_monster->power[i].set(fields[loop++].GetInt32());
			}

			add_monster->attack_min_damage.set(fields[loop++].GetInt32());
			add_monster->attack_max_damage.set(fields[loop++].GetInt32());
			add_monster->magic_min_damage.set(fields[loop++].GetInt32());
			add_monster->magic_max_damage.set(fields[loop++].GetInt32());
			add_monster->critical_damage_rate.set(fields[loop++].GetInt32());
			add_monster->critical_damage_add.set(fields[loop++].GetInt32());
			add_monster->excellent_damage_rate.set(fields[loop++].GetInt32());
			add_monster->excellent_damage_add.set(fields[loop++].GetInt32());
			add_monster->attack_success.set(fields[loop++].GetInt32());
			add_monster->defense.set(fields[loop++].GetInt32());
			add_monster->defense_magic.set(fields[loop++].GetInt32());
			add_monster->defense_success.set(fields[loop++].GetInt32());
			add_monster->move_range.set(fields[loop++].GetUInt32());
			add_monster->move_speed.set(fields[loop++].GetUInt32());
			add_monster->attack_range.set(fields[loop++].GetUInt32());
			add_monster->attack_speed.set(fields[loop++].GetInt32());
			add_monster->view_range.set(fields[loop++].GetUInt32());

			for ( uint8 i = 0; i < Element::MAX; i++ )
			{
				add_monster->SetResistance(i, fields[loop++].GetUInt8());
			}

			add_monster->respawn_time_min.set(fields[loop++].GetUInt32());
			add_monster->respawn_time_max.set(fields[loop++].GetUInt32());
			add_monster->item_rate.set(fields[loop++].GetInt32());
			add_monster->zen_rate.set(fields[loop++].GetInt32());
			add_monster->item_max_level.set(fields[loop++].GetInt32());

			for ( uint8 i = 0; i < POWER_MAX; i++ )
			{
				add_monster->regen_power[i] = fields[loop++].GetFloat();
				add_monster->regen_time[i].set(fields[loop++].GetUInt32());
			}

			add_monster->faction.set(fields[loop++].GetUInt8());
			add_monster->faction_level.set(fields[loop++].GetUInt8());

			add_monster->SetScriptName(fields[loop++].GetString());

			add_monster->SetElementalAttribute(fields[loop++].GetUInt8());
			add_monster->SetElementalPattern(fields[loop++].GetInt32());
			add_monster->SetElementalDefense(fields[loop++].GetInt32());
			add_monster->SetElementalDamageMin(fields[loop++].GetInt32());
			add_monster->SetElementalDamageMax(fields[loop++].GetInt32());
			add_monster->SetElementalAttackRate(fields[loop++].GetInt32());
			add_monster->SetElementalDefenseRate(fields[loop++].GetInt32());
			add_monster->SetRadianceImmune(fields[loop++].GetUInt8());
			add_monster->SetDebuffResistance(fields[loop++].GetInt32());
			add_monster->SetDebuffDefense(fields[loop++].GetInt32());

			add_monster->SetCriticalDamageResistance(fields[loop++].GetUInt8());
			add_monster->SetExcellentDamageResistance(fields[loop++].GetUInt8());
			add_monster->SetDamageAbsorb(fields[loop++].GetUInt8());
			add_monster->SetElite(fields[loop++].GetBool());

			if ( !sScriptAI->IsScriptAI(add_monster->GetScriptName()) )
			{
				sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
				add_monster->ResetScriptName();
			}

			if ( add_monster->attack_range > add_monster->view_range )
				add_monster->view_range.set(add_monster->attack_range.get());

			add_monster->custom.set(true);

			this->map_template[add_monster->id.get()] = add_monster;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u custom monsters definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterItems()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Items...");

	this->ClearItem();

	for ( MonsterTemplateMap::const_iterator it = this->map_template.begin(); it != this->map_template.end(); ++it )
	{
		this->CreateItem(it->second->max_level.get(), it->second->item_max_level.get());
	}
}

monster_template * CMonsterMgr::GetMonsterTemplate(uint16 id) const
{
	MonsterTemplateMap::const_iterator it = this->map_template.find(id);

	if ( it != this->map_template.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

monster * CMonsterMgr::GetMonsterData(uint16 index) const
{
	MonsterMap::const_iterator it = this->monster_map.find(index);

	if ( it != this->monster_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void CMonsterMgr::LoadMonsterSkill()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Skill...");

	for ( MonsterSkillMap::iterator it = this->monster_skill_map.begin(); it != this->monster_skill_map.end(); ++it )
		it->second.clear();

	this->monster_skill_map.clear();

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_skill");

	if ( result )
	{
		do
		{

			Field* fields = result->Fetch();

			uint16 monster = fields[0].GetUInt16();
			uint16 skill = fields[1].GetUInt16();

			if ( !sSkillMgr->IsSkill(skill) )
				continue;
		
			this->monster_skill_map[monster].push_back(skill);

			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster skill", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterSkillSpecial()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Skill Special...");

	for ( MonsterSkillSpecialMap::iterator it = this->monster_skill_special_map.begin(); it != this->monster_skill_special_map.end(); it++ )
	{
		for ( MonsterSkillSpecialList::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			delete *it2;

		it->second.clear();
	}

	this->monster_skill_special_map.clear();
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_skill_special");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_skill_special * add_skill = new monster_skill_special;

			add_skill->SetMonster(fields[0].GetUInt16());
			add_skill->SetType(fields[1].GetInt16());
			add_skill->SetSkill(fields[2].GetUInt16());

			if ( sSkillMgr->GetSkillSpecial(add_skill->GetSkill()) == nullptr )
			{
				delete add_skill;
				continue;
			}
		
			this->monster_skill_special_map[add_skill->GetMonster()].push_back(add_skill);
		
			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster skill special", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterAIElement()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Element...");

	MAP_CLEAR(MonsterAIElementMap::iterator, this->m_monster_ai_element);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_ai_element");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_ai_element * ai_data = new monster_ai_element;
			ai_data->SetID(fields[0].GetUInt32());
			ai_data->SetName(fields[1].GetString());
			ai_data->SetState(fields[2].GetUInt8());
			ai_data->SetClass(fields[3].GetUInt8());
			ai_data->SetRate(fields[4].GetUInt8());
			ai_data->SetDelay(fields[5].GetUInt32());
			ai_data->SetTarget(fields[6].GetInt32());
			ai_data->SetX(fields[7].GetInt16());
			ai_data->SetY(fields[8].GetInt16());

			if ( ai_data->GetState() >= MONSTER_AI_ELEMENT_MAX )
			{
				delete ai_data;
				continue;
			}

			this->m_monster_ai_element[ai_data->GetID()] = ai_data;
			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai element definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void CMonsterMgr::LoadMonsterAIAutomata()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Automata...");

	MAP_CLEAR(MonsterAIAutomataMap::iterator, this->m_monster_ai_automata);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_ai_automata");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint32 id = fields[0].GetUInt32();

			monster_ai_automata * ai_data = this->GetMonsterAIAutomata(id);

			if ( !ai_data )
			{
				ai_data = new monster_ai_automata;
				ai_data->SetID(id);
				this->m_monster_ai_automata[id] = ai_data;
			}

			std::string name(fields[1].GetString());
			uint8 priority = fields[2].GetUInt8();
			uint8 current_state = fields[3].GetUInt8();
			uint8 next_state = fields[4].GetUInt8();
			int32 transaction_type = fields[5].GetInt32();
			uint8 transaction_rate = fields[6].GetUInt8();
			int32 transaction_value_type = fields[7].GetInt32();
			int32 transition_value = fields[8].GetInt32();
			int32 delay = fields[9].GetInt32();

			if ( current_state >= MAX_AI_STATE )
				continue;

			if ( next_state >= MAX_AI_STATE )
				continue;

			if ( priority >= MAX_AI_PRIORITY )
				continue;

			monster_ai_state * ai_state = &ai_data->m_AIState[current_state][priority];

			ai_state->SetPriority(priority);
			ai_state->SetCurrentState(current_state);
			ai_state->SetNextState(next_state);
			ai_state->SetTransitionType(transaction_type);
			ai_state->SetTransitionRate(transaction_rate);
			ai_state->SetTransitionValueType(transaction_value_type);
			ai_state->SetTransitionValue(transition_value);
			ai_state->SetDelay(delay);
			ai_state->SetName(name);
			ai_data->m_AIStatCount[current_state]++;

			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai automata definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void CMonsterMgr::LoadMonsterAIUnit()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Unit...");

	MAP_CLEAR(MonsterAIUnitMap::iterator, this->m_monster_ai_unit);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_ai_unit");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_ai_unit * ai_data = new monster_ai_unit;
			ai_data->SetID(fields[0].GetUInt32());
			ai_data->SetName(fields[1].GetString());
			ai_data->SetDelay(fields[2].GetUInt32());
			ai_data->SetAutomata(this->GetMonsterAIAutomata(fields[3].GetInt32()));

			ai_data->SetAIClass(0, this->GetMonsterAIElement(fields[4].GetInt32()));
			ai_data->SetAIClass(1, this->GetMonsterAIElement(fields[5].GetInt32()));
			ai_data->SetAIClass(2, this->GetMonsterAIElement(fields[6].GetInt32()));
			ai_data->SetAIClass(3, this->GetMonsterAIElement(fields[7].GetInt32()));
			ai_data->SetAIClass(4, this->GetMonsterAIElement(fields[8].GetInt32()));
			ai_data->SetAIClass(5, this->GetMonsterAIElement(fields[9].GetInt32()));
			ai_data->SetAIClass(6, this->GetMonsterAIElement(fields[10].GetInt32()));
			ai_data->SetAIClass(7, this->GetMonsterAIElement(fields[11].GetInt32()));

			this->m_monster_ai_unit[ai_data->GetID()] = ai_data;
			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai unit definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterAIGroup()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Group...");

	for ( MonsterAIGroupMap::iterator it = this->monster_ai_group_map.begin(); it != this->monster_ai_group_map.end(); it++ )
	{
		for ( MonsterAIGroupList::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			delete *it2;

		it->second.clear();
	}

	this->monster_ai_group_map.clear();
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_ai_data");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			AIData * add_ai_data = new AIData;

			add_ai_data->SetGroup(fields[0].GetUInt32());
			add_ai_data->SetID(fields[1].GetUInt32());
	
			for ( uint8 i = 0; i < MONSTER_MAX_AI_ORDER; ++i )
			{
				add_ai_data->SetAIOrder(i, fields[2 + i].GetUInt32());
			}

			for ( uint8 i = 0; i < MONSTER_MAX_AI_ORDER; ++i )
			{
				add_ai_data->SetAITime(i, fields[6 + i].GetUInt32());
			}
		
			this->monster_ai_group_map[add_ai_data->GetGroup()].push_back(add_ai_data);

			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai groups", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterRespawnLocation()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Respawn Location...");

	for ( MonsterRespawnLocationMap::iterator i_list = this->m_monster_respawn_location_map.begin(); i_list != this->m_monster_respawn_location_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterRespawnLocationList::iterator, i_list->second);
	}

	this->m_monster_respawn_location_map.clear();

	uint32 count = 0;

	PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_SRVCFG_MONSTER_RESPAWN_LOCATION_SELECT);
	stmt->setUInt16(0, sGameServer->GetTransferServer());

	PreparedQueryResult result = GameServerDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_respawn_location * add_location = new monster_respawn_location;

			add_location->SetGroup(fields[1].GetUInt32());
			add_location->SetWorld(fields[2].GetUInt16());
			add_location->SetX1(fields[3].GetInt16());
			add_location->SetY1(fields[4].GetInt16());
			add_location->SetX2(fields[5].GetInt16());
			add_location->SetY2(fields[6].GetInt16());
			add_location->SetDirection(fields[7].GetInt8());
			add_location->SetInstance(fields[8].GetInt32());
			add_location->SetRate(fields[9].GetUInt8());

			this->m_monster_respawn_location_map[add_location->GetGroup()].push_back(add_location);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster respawn locations", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterEquipment()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Equipment...");

	for ( MonsterEquipmentMap::iterator i_list = this->m_monster_equipment_map.begin(); i_list != this->m_monster_equipment_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterEquipmentList::iterator, i_list->second);
	}

	this->m_monster_equipment_map.clear();

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_equipment");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_equipment * add_monster = new monster_equipment;

			uint16 monster = fields[0].GetUInt16();
			add_monster->slot.set(fields[1].GetUInt8());
			add_monster->type.set(fields[2].GetUInt8());
			add_monster->index.set(fields[3].GetUInt16());
			add_monster->level.set(fields[4].GetUInt8());
			add_monster->excellent_ancient.set(fields[5].GetUInt8());

			if ( this->GetMonsterTemplate(monster) == nullptr )
			{
				sLog->outError(LOG_DEFAULT, "LoadMonsterEquipment() Wrong Monster ID: %d", monster);
				delete add_monster;
				continue;
			}

			this->m_monster_equipment_map[monster].push_back(add_monster);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster equipment definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonster()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster...");

	MAP_CLEAR(MonsterMap::iterator, this->monster_map);

	uint32 count = 0;

	PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_SRVCFG_MONSTER_SELECT);
	stmt->setUInt16(0, sGameServer->GetTransferServer());

	PreparedQueryResult result = GameServerDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster * add_monster = new monster;
			int32 loop = 1;

			add_monster->SetGUID(fields[loop++].GetUInt16());
			add_monster->SetID(fields[loop++].GetUInt16());
			add_monster->SetType(fields[loop++].GetUInt8());
			add_monster->SetName(fields[loop++].GetCString());
			add_monster->SetWorld(fields[loop++].GetUInt16());
			add_monster->SetX1(fields[loop++].GetInt16());
			add_monster->SetY1(fields[loop++].GetInt16());
			add_monster->SetX2(fields[loop++].GetInt16());
			add_monster->SetY2(fields[loop++].GetInt16());
			add_monster->SetDirection(fields[loop++].GetInt8());
			add_monster->SetSpawnDelay(fields[loop++].GetUInt32());
			add_monster->SetSpawnDistance(fields[loop++].GetUInt8());
			add_monster->SetRespawnTimeMin(fields[loop++].GetUInt32());
			add_monster->SetRespawnTimeMax(fields[loop++].GetUInt32());
			add_monster->SetRespawnID(fields[loop++].GetUInt32());
			add_monster->SetMoveDistance(fields[loop++].GetUInt8());
			add_monster->SetNpcFunction(fields[loop++].GetString());
			add_monster->SetItemBag(fields[loop++].GetString());
			add_monster->SetScriptName(fields[loop++].GetString());
			add_monster->SetElementalAttribute(fields[loop++].GetUInt8());

			if ( !sScriptAI->IsScriptAI(add_monster->GetScriptName()) )
			{
				sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
				add_monster->ResetScriptName();
			}

			if ( add_monster->GetX1() > 255 )
				add_monster->SetX1(255);

			if ( add_monster->GetX1() < 0 )
				add_monster->SetX1(0);

			if ( add_monster->GetY1() > 255 )
				add_monster->SetY1(255);

			if ( add_monster->GetY1() < 0 )
				add_monster->SetY1(0);

			if ( add_monster->GetX2() > 255 )
				add_monster->SetX2(255);

			if ( add_monster->GetX2() < 0 )
				add_monster->SetX2(0);

			if ( add_monster->GetY2() > 255 )
				add_monster->SetY2(255);

			if ( add_monster->GetY2() < 0 )
				add_monster->SetY2(0);

			if ( add_monster->GetX2() < add_monster->GetX1() )
				add_monster->SetX2(add_monster->GetX1());

			if ( add_monster->GetY2() < add_monster->GetY1() )
				add_monster->SetY2(add_monster->GetY1());

			if ( add_monster->GetGUID() >= sObjectMgr->GetMaxMonsters() )
			{
				sLog->outError(LOG_DEFAULT, "High Monster GUID: %u", add_monster->GetGUID());
				delete add_monster;
				continue;
			}

			if ( this->GetMonsterTemplate(add_monster->GetID()) == nullptr )
			{
				sLog->outError(LOG_DEFAULT, "Wrong Monster GUID: %u / ID: %u", add_monster->GetGUID(), add_monster->GetID());
				delete add_monster;
				continue;
			}

			if ( !this->IsMonsterExclusive(add_monster->GetGUID()) )
			{
				delete add_monster;
				continue;
			}

			this->monster_map[add_monster->GetGUID()] = add_monster;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monsters", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonster(uint16 guid)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster...");

	for ( MonsterMap::const_iterator it = this->monster_map.begin(); it != this->monster_map.end(); )
	{
		if ( it->first >= guid )
		{
			delete it->second;
			this->monster_map.erase(it++);
		}
		else
		{
			++it;
		}
	}

	uint32 count = 0;

	PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_SRVCFG_MONSTER_SELECT_GUID);
	stmt->setUInt16(0, sGameServer->GetTransferServer());
	stmt->setUInt16(1, guid);

	PreparedQueryResult result = GameServerDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster * add_monster = new monster;
			int32 loop = 1;

			add_monster->SetGUID(fields[loop++].GetUInt16());
			add_monster->SetID(fields[loop++].GetUInt16());
			add_monster->SetType(fields[loop++].GetUInt8());
			add_monster->SetName(fields[loop++].GetCString());
			add_monster->SetWorld(fields[loop++].GetUInt16());
			add_monster->SetX1(fields[loop++].GetInt16());
			add_monster->SetY1(fields[loop++].GetInt16());
			add_monster->SetX2(fields[loop++].GetInt16());
			add_monster->SetY2(fields[loop++].GetInt16());
			add_monster->SetDirection(fields[loop++].GetInt8());
			add_monster->SetSpawnDelay(fields[loop++].GetUInt32());
			add_monster->SetSpawnDistance(fields[loop++].GetUInt8());
			add_monster->SetRespawnTimeMin(fields[loop++].GetUInt32());
			add_monster->SetRespawnTimeMax(fields[loop++].GetUInt32());
			add_monster->SetRespawnID(fields[loop++].GetUInt32());
			add_monster->SetMoveDistance(fields[loop++].GetUInt8());
			add_monster->SetNpcFunction(fields[loop++].GetString());
			add_monster->SetItemBag(fields[loop++].GetString());
			add_monster->SetScriptName(fields[loop++].GetString());
			add_monster->SetElementalAttribute(fields[loop++].GetUInt8());

			if ( !sScriptAI->IsScriptAI(add_monster->GetScriptName()) )
			{
				sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
				add_monster->ResetScriptName();
			}

			if ( add_monster->GetX1() > 255 )
				add_monster->SetX1(255);

			if ( add_monster->GetX1() < 0 )
				add_monster->SetX1(0);

			if ( add_monster->GetY1() > 255 )
				add_monster->SetY1(255);

			if ( add_monster->GetY1() < 0 )
				add_monster->SetY1(0);

			if ( add_monster->GetX2() > 255 )
				add_monster->SetX2(255);

			if ( add_monster->GetX2() < 0 )
				add_monster->SetX2(0);

			if ( add_monster->GetY2() > 255 )
				add_monster->SetY2(255);

			if ( add_monster->GetY2() < 0 )
				add_monster->SetY2(0);

			if ( add_monster->GetX2() < add_monster->GetX1() )
				add_monster->SetX2(add_monster->GetX1());

			if ( add_monster->GetY2() < add_monster->GetY1() )
				add_monster->SetY2(add_monster->GetY1());

			if ( add_monster->GetGUID() >= sObjectMgr->GetMaxMonsters() )
			{
				sLog->outError(LOG_DEFAULT, "High Monster GUID: %u", add_monster->GetGUID());
				delete add_monster;
				continue;
			}

			if ( this->GetMonsterTemplate(add_monster->GetID()) == nullptr )
			{
				sLog->outError(LOG_DEFAULT, "Wrong Monster GUID: %u / ID: %u", add_monster->GetGUID(), add_monster->GetID());
				delete add_monster;
				continue;
			}

			if ( !this->IsMonsterExclusive(add_monster->GetGUID()) )
			{
				delete add_monster;
				continue;
			}

			this->monster_map[add_monster->GetGUID()] = add_monster;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monsters", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterEvent()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Event...");

	LIST_CLEAR(MonsterEventList::iterator, this->monster_event_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM monster_event WHERE monster_event.server = %u", sGameServer->GetTransferServer());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			int32 add_count = fields[24].GetInt32();

			for ( int32 i = 0; i < add_count; ++i )
			{
				monster_event * add_monster = new monster_event;
				int32 loop = 1;

				add_monster->SetID(fields[loop++].GetUInt16());
				add_monster->SetWorld(fields[loop++].GetUInt16());
				add_monster->SetX1(fields[loop++].GetInt16());
				add_monster->SetY1(fields[loop++].GetInt16());
				add_monster->SetX2(fields[loop++].GetInt16());
				add_monster->SetY2(fields[loop++].GetInt16());
				add_monster->SetDirection(fields[loop++].GetInt8());
				add_monster->SetSpawnDelay(fields[loop++].GetUInt32());
				add_monster->SetSpawnDistance(fields[loop++].GetUInt8());
				add_monster->SetRespawnTime(fields[loop++].GetUInt32());
				add_monster->SetRespawnID(fields[loop++].GetUInt32());
				add_monster->SetMoveDistance(fields[loop++].GetUInt8());
				add_monster->SetEventID(fields[loop++].GetUInt8());
				add_monster->raw.data[0] = fields[loop++].GetInt32();
				add_monster->raw.data[1] = fields[loop++].GetInt32();
				add_monster->raw.data[2] = fields[loop++].GetInt32();
				add_monster->raw.data[3] = fields[loop++].GetInt32();
				add_monster->raw.data[4] = fields[loop++].GetInt32();
				add_monster->SetNpcFunction(fields[loop++].GetString());
				add_monster->SetItemBag(fields[loop++].GetString());
				add_monster->SetScriptName(fields[loop++].GetString());
				add_monster->SetAIGroup(fields[loop++].GetUInt32());
				add_monster->SetAIGroupMember(fields[loop++].GetUInt32());

				if ( !sScriptAI->IsScriptAI(add_monster->GetScriptName()) )
				{
					sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
					add_monster->ResetScriptName();
				}

				if ( add_monster->GetX1() > 255 )
					add_monster->SetX1(255);

				if ( add_monster->GetX1() < 0 )
					add_monster->SetX1(0);

				if ( add_monster->GetY1() > 255 )
					add_monster->SetY1(255);

				if ( add_monster->GetY1() < 0 )
					add_monster->SetY1(0);

				if ( add_monster->GetX2() > 255 )
					add_monster->SetX2(255);

				if ( add_monster->GetX2() < 0 )
					add_monster->SetX2(0);

				if ( add_monster->GetY2() > 255 )
					add_monster->SetY2(255);

				if ( add_monster->GetY2() < 0 )
					add_monster->SetY2(0);

				if ( add_monster->GetX2() < add_monster->GetX1() )
					add_monster->SetX2(add_monster->GetX1());

				if ( add_monster->GetY2() < add_monster->GetY1() )
					add_monster->SetY2(add_monster->GetY1());

				if ( this->GetMonsterTemplate(add_monster->GetID()) == nullptr )
				{
					sLog->outError(LOG_DEFAULT, "Wrong Monster Event ID: %u", add_monster->GetID());
					delete add_monster;
					continue;
				}

				this->monster_event_list.push_back(add_monster);
				count++;
			}
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster event definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterEvent(uint8 event_id)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Event...");

	for ( MonsterEventList::iterator it = this->monster_event_list.begin(); it != this->monster_event_list.end(); )
	{
		if ( (*it)->GetEventID() == event_id )
		{
			delete *it;
			it = this->monster_event_list.erase(it);
		}
		else
		{
			++it;
		}
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM monster_event WHERE monster_event.server = %u AND event_id = %u", sGameServer->GetTransferServer(), event_id);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			int32 add_count = fields[24].GetInt32();

			for ( int32 i = 0; i < add_count; ++i )
			{
				monster_event * add_monster = new monster_event;
				int32 loop = 1;

				add_monster->SetID(fields[loop++].GetUInt16());
				add_monster->SetWorld(fields[loop++].GetUInt16());
				add_monster->SetX1(fields[loop++].GetInt16());
				add_monster->SetY1(fields[loop++].GetInt16());
				add_monster->SetX2(fields[loop++].GetInt16());
				add_monster->SetY2(fields[loop++].GetInt16());
				add_monster->SetDirection(fields[loop++].GetInt8());
				add_monster->SetSpawnDelay(fields[loop++].GetUInt32());
				add_monster->SetSpawnDistance(fields[loop++].GetUInt8());
				add_monster->SetRespawnTime(fields[loop++].GetUInt32());
				add_monster->SetRespawnID(fields[loop++].GetUInt32());
				add_monster->SetMoveDistance(fields[loop++].GetUInt8());
				add_monster->SetEventID(fields[loop++].GetUInt8());
				add_monster->raw.data[0] = fields[loop++].GetInt32();
				add_monster->raw.data[1] = fields[loop++].GetInt32();
				add_monster->raw.data[2] = fields[loop++].GetInt32();
				add_monster->raw.data[3] = fields[loop++].GetInt32();
				add_monster->raw.data[4] = fields[loop++].GetInt32();
				add_monster->SetNpcFunction(fields[loop++].GetString());
				add_monster->SetItemBag(fields[loop++].GetString());
				add_monster->SetScriptName(fields[loop++].GetString());
				add_monster->SetAIGroup(fields[loop++].GetUInt32());
				add_monster->SetAIGroupMember(fields[loop++].GetUInt32());

				if ( !sScriptAI->IsScriptAI(add_monster->GetScriptName()) )
				{
					sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
					add_monster->ResetScriptName();
				}

				if ( add_monster->GetX1() > 255 )
					add_monster->SetX1(255);

				if ( add_monster->GetX1() < 0 )
					add_monster->SetX1(0);

				if ( add_monster->GetY1() > 255 )
					add_monster->SetY1(255);

				if ( add_monster->GetY1() < 0 )
					add_monster->SetY1(0);

				if ( add_monster->GetX2() > 255 )
					add_monster->SetX2(255);

				if ( add_monster->GetX2() < 0 )
					add_monster->SetX2(0);

				if ( add_monster->GetY2() > 255 )
					add_monster->SetY2(255);

				if ( add_monster->GetY2() < 0 )
					add_monster->SetY2(0);

				if ( add_monster->GetX2() < add_monster->GetX1() )
					add_monster->SetX2(add_monster->GetX1());

				if ( add_monster->GetY2() < add_monster->GetY1() )
					add_monster->SetY2(add_monster->GetY1());

				if ( this->GetMonsterTemplate(add_monster->GetID()) == nullptr )
				{
					sLog->outError(LOG_DEFAULT, "Wrong Monster Event ID: %u", add_monster->GetID());
					delete add_monster;
					continue;
				}

				this->monster_event_list.push_back(add_monster);
				count++;
			}
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster event definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterRespawn()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Respawn...");

	MAP_CLEAR(MonsterRespawnMap::iterator, this->monster_respawn_map);

	uint32 count = 0;

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_RESPAWN_SELECT);
	stmt->setUInt16(0, sGameServer->GetServerCode());

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_respawn * add_monster = new monster_respawn;

			add_monster->SetGUID(fields[0].GetUInt16());
			add_monster->SetDate(fields[1].GetInt64());

			this->monster_respawn_map[add_monster->GetGUID()] = add_monster;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster respawn definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::LoadMonsterExclusive()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Exclusive...");

	LIST_CLEAR(MonsterExclusiveList::iterator, this->monster_exclusive_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_exclusive");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_exclusive * add_monster = new monster_exclusive;

			add_monster->SetServer(fields[0].GetUInt16());
			add_monster->SetGUID(fields[1].GetUInt16());
			add_monster->SetAvailableServer(fields[2].GetUInt16());

			this->monster_exclusive_list.push_back(add_monster);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster exclusive definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CMonsterMgr::SpawnMonsters()
{
	for ( MonsterMap::const_iterator it = this->monster_map.begin(); it != this->monster_map.end(); ++it )
	{
		Monster* mMonster = MONSTER_ADD_CONTINUE(it->second);

		if ( !mMonster )
		{
			continue;
		}
		
		if ( !mMonster->GetWorld() )
		{
			mMonster->SetConnectStatus(CONNECT_STATUS_NONE);
			continue;
		}

		MonsterRespawnMap::const_iterator itr = this->monster_respawn_map.find(mMonster->GetEntry());

		if ( itr != this->monster_respawn_map.end() )
		{
			time_t current_time = time(nullptr);

			if ( itr->second->GetDate() > current_time )
			{
				mMonster->SetSpawnTime((itr->second->GetDate() - current_time) * IN_MILLISECONDS);
			}
		}

		mMonster->AddToWorld();
	}
}

bool CMonsterMgr::IsMonsterExclusive(uint16 guid) const
{
	bool success = true;
	bool founded = false;

	for ( MonsterExclusiveList::const_iterator it = this->monster_exclusive_list.begin(); it != this->monster_exclusive_list.end(); ++it )
	{
		if ( (*it) && (*it)->GetServer() == sGameServer->GetTransferServer() && (*it)->GetGUID() == guid )
		{
			success = false;

			if ( (*it)->GetAvailableServer() == sGameServer->GetServerCode() )
			{
				founded = true;
			}
		}
	}

	if ( founded )
	{
		success = true;
	}

	return success;
}

void CMonsterMgr::CreateItem(int32 level, int32 max_level)
{
	if ( level < 1 )
	{
		return;
	}

	ItemTemplateMap const& item_template_map = sItemMgr->GetItemMap();
	for ( ItemTemplateMap::const_iterator it = item_template_map.begin(); it != item_template_map.end(); ++it )
	{
		item_template const* item_info = it->second;

		if ( !item_info )
		{
			continue;
		}

		if ( !item_info->IsFlag(ITEM_FLAG_MONSTER_DROP) )
		{
			continue;
		}

		if ( !item_info->GetDropRate() )
		{
			continue;
		}
		
		uint8 type = item_info->GetType();
		uint16 index = item_info->GetIndex();

		int32 level_tmp = this->GenerateItemLevel(item_info, level);

		if ( level_tmp < 0 )
		{
			continue;
		}

		if ( type < 12 )
		{
			level_tmp = 0;
		}

		if ( level_tmp > max_level )
		{
			level_tmp = max_level;
		}

		this->InsertItem(level, ITEMGET(type, index), level_tmp);
	}
}

int32 CMonsterMgr::GenerateItemLevel(item_template const* item_info, int32 level)
{
	if ( item_info->GetLevel() == 0 )
	{
		return -1;
	}

	if ( item_info->GetLevel() >= (level - 18) && item_info->GetLevel() <= level ) // Rango de niveles
	{
		int32 ilevel = (level - item_info->GetLevel()) / 3;

		if ( item_info->GetType() == 13 )
		{
			// Rings y Pendants
			if ( item_info->GetIndex() == 8 || item_info->GetIndex() == 9 || item_info->GetIndex() == 12 || item_info->GetIndex() == 13 || item_info->GetIndex() == 20 || 
				 item_info->GetIndex() == 21 || item_info->GetIndex() == 22 || item_info->GetIndex() == 23 || item_info->GetIndex() == 24 || item_info->GetIndex() == 25 || 
				 item_info->GetIndex() == 26 || item_info->GetIndex() == 27 || item_info->GetIndex() == 28 )
			{
				if ( ilevel > 4 )
				{
					ilevel = 4;
				}
			}
		}

		return ilevel;
	}

	return -1;
}

bool CMonsterMgr::InsertItem(int32 level, uint16 item, uint8 item_level)
{
	MonsterItemList & item_list = this->m_monster_item_map[level];

	for ( MonsterItemList::const_iterator it = item_list.begin(); it != item_list.end(); ++it )
	{
		if ( (*it)->GetItem() == item && (*it)->GetLevel() == item_level )
		{
			return false;
		}
	}

	MonsterItem* pNewItem = new MonsterItem;
	pNewItem->SetItem(item);
	pNewItem->SetLevel(item_level);

	item_list.push_back(pNewItem);
	return true;
}

bool CMonsterMgr::GenerateItem(Item & item, int32 level, bool exe)
{
	if ( level < 1 )
		return false;

	RandomValue<MonsterItem const*> m_randomizer(nullptr);
	m_randomizer.SetTotalWeight(10000);
	
	MonsterItemMap::iterator i_list = this->m_monster_item_map.find(level);

	if ( i_list != this->m_monster_item_map.end() )
	{
		for ( MonsterItemList::iterator it = i_list->second.begin(); it != i_list->second.end(); ++it )
		{
			MonsterItem const* pItem = (*it);

			if ( !pItem )
			{
				continue;
			}

			item_template const* item_info = sItemMgr->GetItem(pItem->GetItem());

			if ( !item_info )
			{
				continue;
			}

			if ( !item_info->GetDropRate() )
			{
				continue;
			}

			if ( exe && !item_info->IsFlag(ITEM_FLAG_MONSTER_DROP_EXE) )
			{
				continue;
			}

			m_randomizer.AddValue(pItem, item_info->GetDropRate());
		}
	}

	MonsterItem const* pItem = m_randomizer.GetRandomValue(RANDOM_POOL_RATE);

	if ( pItem )
	{
		item.SetItem(pItem->GetItem());
		item.SetLevel(pItem->GetLevel());
		return true;
	}

	return false;
}
	
void CMonsterMgr::ClearItem()
{
	for ( MonsterItemMap::iterator i_list = this->m_monster_item_map.begin(); i_list != this->m_monster_item_map.end(); ++i_list )
	{
		for ( MonsterItemList::iterator it = i_list->second.begin(); it != i_list->second.end(); ++it )
		{
			delete *it;
		}

		i_list->second.clear();
	}

	this->m_monster_item_map.clear();
}

void CMonsterMgr::GenerateRespawnLocation(Monster* pMonster)
{
	monster_respawn_location const* pData = this->GetRandomRespawnLocation(pMonster);

	if ( !pData )
	{
		return;
	}

	pMonster->SetWorldId(pData->GetWorld());
	pMonster->SetDirection(pData->GetDirection());
	pMonster->SetInstance(pData->GetInstance());
	pMonster->SetBasicLocation(pData->GetX1(), pData->GetY1(), pData->GetX2(), pData->GetY2());
}

monster_respawn_location const* CMonsterMgr::GetRandomRespawnLocation(Monster* pMonster) const
{
	if ( !pMonster->GetRespawnID() )
		return nullptr;

	MonsterRespawnLocationMap::const_iterator it = this->m_monster_respawn_location_map.find(pMonster->GetRespawnID());

	if ( it == this->m_monster_respawn_location_map.end() )
		return nullptr;

	if ( it->second.empty() )
		return nullptr;

	RandomValue<monster_respawn_location const*> m_randomizer(nullptr);
	m_randomizer.SetTotalWeight(100);

	for ( MonsterRespawnLocationList::const_iterator data = it->second.begin(); data != it->second.end(); ++data )
	{
		m_randomizer.AddValue(*data, (*data)->GetRate());
	}

	return m_randomizer.GetRandomValue(RANDOM_POOL_RATE);
}

bool CMonsterMgr::TeleportToRandomLocation(Monster* pMonster, int32 attempts)
{
	do
	{
		monster_respawn_location const* pLocation = this->GetRandomRespawnLocation(pMonster);

		if ( pLocation )
		{
			if ( pLocation->GetWorld() != pMonster->GetWorldId() )
			{
				pMonster->UpdateLastLocation();

				pMonster->ClearPathData();
				pMonster->SetBasicLocation(pLocation->GetX1(), pLocation->GetY1(), pLocation->GetX2(), pLocation->GetY2());
				pMonster->SetWorldId(pLocation->GetWorld());

				pMonster->MakeRespawnLocation(false);

				pMonster->MagicAttackSend(ENTRY(pMonster), SKILL_TELEPORT);
				pMonster->ViewportClose();

				//pMonster->TeleportToLocation(pLocation->GetWorld(), pMonster->GetX(), pMonster->GetY(), pLocation->GetDirection(), pLocation->GetInstance());
				return true;
			}
		}
	}
	while ( attempts-- > 0 );

	return false;
}

void CMonsterMgr::BuildMonsterCustomData(Monster* pMonster, Player* pPlayer)
{
	if ( !pMonster->IsCustom() )
	{
		return;
	}

	VIEWPORT_MONSTER_CUSTOM_DATA pMsg;
	pMsg.index = pMonster->GetEntry();
	pMsg.type = pMonster->GetSendType();
	memcpy(pMsg.name, pMonster->GetName(), 32);
	pMsg.size = pMonster->GetSize();
	
	MonsterEquipmentMap::const_iterator it = this->m_monster_equipment_map.find(pMonster->GetClass());

	if ( it != this->m_monster_equipment_map.end() )
	{
		for ( MonsterEquipmentList::const_iterator equipment = it->second.begin(); equipment != it->second.end(); ++equipment )
		{
			if ( (*equipment)->slot.get() < 12 )
			{
				pMsg.equipment[(*equipment)->slot.get()].item = ITEMGET((*equipment)->type.get(), (*equipment)->index.get());
				pMsg.equipment[(*equipment)->slot.get()].level = (*equipment)->level.get();
				pMsg.equipment[(*equipment)->slot.get()].excellent_ancient = (*equipment)->excellent_ancient.get();
			}
		}
	}

	if ( pPlayer )
	{
		pPlayer->sendPacket((uint8*)&pMsg, pMsg.h.get_size());
	}
	else
	{
		pMonster->sendPacket_viewport((uint8*)&pMsg, pMsg.h.get_size());
	}
}

uint16 CMonsterMgr::GetRandomSkillSpecial(Monster* pMonster, int16 type)
{
	MonsterSkillSpecialMap::const_iterator it = this->monster_skill_special_map.find(pMonster->GetClass());

	RandomValue<uint16> m_randomizer(0);

	if ( it != this->monster_skill_special_map.end() )
	{
		for ( MonsterSkillSpecialList::const_iterator itr = it->second.begin(); itr != it->second.end(); ++itr )
		{
			if ( (*itr)->GetType() == type )
			{
				m_randomizer.AddValue((*itr)->GetSkill(), 0);
			}
		}
	}

	return m_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);
}

bool AllowItemDropInWorld(world_type world, Item const& item)
{
	if ( item.GetItem() == JEWEL::BLESS ||
		 item.GetItem() == JEWEL::SOUL ||
		 item.GetItem() == JEWEL::CHAOS ||
		 item.GetItem() == JEWEL::LIFE ||
		 item.GetItem() == JEWEL::CREATION ||
		 item.GetItem() == JEWEL::DEATH ||
		 item.GetItem() == JEWEL::GUARDIAN )
	{
		if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL && sGameServer->IsCrywolfPenaltyApply() )
		{
			if ( Random(100) < sGameServer->GetCrywolfPenaltyJewelDrop() )
			{
				return false;
			}
		}
	}

	bool fail = false;

	if ( DS_MAP_RANGE(world) )
	{
		if ( item.GetItemType() >= 7 && item.GetItemType() <= 11 )
		{
			fail = item.GetItemIndex() == 17 || item.GetItemIndex() == 18 || item.GetItemIndex() == 19;
		}
		else if ( item.GetItemType() == 0 )
		{
			fail = item.GetItemIndex() == 17 || item.GetItemIndex() == 18;
		}
		else if ( item.GetItemType() == 12 )
		{
			fail = item.GetItemIndex() == 12 || item.GetItemIndex() == 13 || item.GetItemIndex() == 14 ||
				   item.GetItemIndex() == 16 || item.GetItemIndex() == 17 || item.GetItemIndex() == 18 ||
				   item.GetItemIndex() == 19;
		}
		else
		{
			fail = item.GetItem() == ITEMGET(4, 17) || item.GetItem() == ITEMGET(5, 9);
		}
	}
	else if ( DG_MAP_RANGE(world) )
	{
		if ( item.GetItem() == ITEMGET(14, 101) || item.GetItem() == ITEMGET(14, 110) )
		{
			fail = true;
		}
	}

	return fail ? RANDOM(10) == 0: true;
}