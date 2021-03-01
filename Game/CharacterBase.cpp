/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerCharacterBase.cpp"
*
*/

CharacterBaseMgr::CharacterBaseMgr()
{

}

CharacterBaseMgr::~CharacterBaseMgr()
{
	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		this->char_skill_base[i].clear();

		CLEAR_LIST(this->m_character_stat_fruit[i]);
		CLEAR_LIST(this->m_character_item[i]);
		MAP_CLEAR(CharacterHelperMap::iterator, this->m_character_helper[i]);
		CLEAR_LIST(this->m_character_specialization[i]);
	}

	CLEAR_LIST(this->m_CharacterFormulaList);

	CLEAR_LIST(this->m_character_experience_adjust);
	CLEAR_LIST(this->m_character_experience_bonus);
	CLEAR_LIST(this->m_character_bonus_list);
	CLEAR_LIST(this->m_CharacterMaxLevelRewardList);
		
	for ( int32 i = 0; i < LEVEL_DATA_MAX; ++i )
	{
		MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[i]);
	}

	CLEAR_LIST(this->m_CharacterExperienceTableList);
}

void CharacterBaseMgr::LoadCharacterBase()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Base...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_base");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			uint8 rate = fields[loop++].GetUInt8();

			if ( rate >= Character::MAX_CLASS )
				continue;

			for ( uint8 i = 0; i < MAX_STAT_TYPE; ++i )
			{
				this->char_base[rate].SetStat(i, fields[loop++].GetUInt32());
			}

			this->char_base[rate].SetLife(fields[loop++].GetInt32());
			this->char_base[rate].SetMana(fields[loop++].GetInt32());
			this->char_base[rate].SetLevelToLife(fields[loop++].GetFloat());
			this->char_base[rate].SetLevelToMana(fields[loop++].GetFloat());
			this->char_base[rate].SetVitalityToLife(fields[loop++].GetFloat());
			this->char_base[rate].SetEnergyToMana(fields[loop++].GetFloat());

			this->char_base[rate].SetWorld(fields[loop++].GetUInt16());

			this->char_base[rate].SetLevel(fields[loop++].GetInt16());
			this->char_base[rate].SetPoints(fields[loop++].GetInt32());
		
			POWER_LOOP(i)
			{
				this->char_base[rate].SetAutomaticPowerRecovery(i, fields[loop++].GetFloat());
			}

			this->char_base[rate].SetLevelUpPoints(fields[loop++].GetUInt32());
			this->char_base[rate].SetPlusLevelUpPoints(fields[loop++].GetUInt32());
			this->char_base[rate].SetMasterLevelUpPoints(fields[loop++].GetUInt32());
			this->char_base[rate].SetMajesticLevelUpPoints(fields[loop++].GetUInt32());

			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character base definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

CharacterBaseData * CharacterBaseMgr::GetCharacterBase(uint8 character_class)
{
	if ( character_class >= Character::MAX_CLASS )
		return nullptr;

	return &this->char_base[character_class];
}

void CharacterBaseMgr::LoadCharacterBaseSkill()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Skill...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		this->char_skill_base[i].clear();
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM character_skill");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 race = fields[0].GetUInt8();

			if ( race >= Character::MAX_CLASS )
				continue;

			uint16 skill = fields[1].GetUInt16();

			if ( !sSkillMgr->IsSkill(skill) )
				continue;

			this->char_skill_base[race].push_back(skill);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character skill definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceNormal()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Normal...");

	MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[LEVEL_DATA_NORMAL]);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM experience_normal");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			CharacterExperience * add_experience = new CharacterExperience;
			add_experience->SetLevel(fields[0].GetUInt16());
			add_experience->SetExperience(fields[1].GetInt64());
			add_experience->SetAccumulatedExperience(fields[2].GetInt64());
			
			this->m_character_experience[LEVEL_DATA_NORMAL][add_experience->GetLevel()] = add_experience;

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience normal definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceMaster()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Master...");

	MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[LEVEL_DATA_MASTER]);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM experience_master");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			CharacterExperience * add_experience = new CharacterExperience;
			add_experience->SetLevel(fields[0].GetUInt16());
			add_experience->SetExperience(fields[1].GetInt64());
			add_experience->SetAccumulatedExperience(fields[2].GetInt64());
			
			this->m_character_experience[LEVEL_DATA_MASTER][add_experience->GetLevel()] = add_experience;

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience master definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceMajestic()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Majestic...");

	MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[LEVEL_DATA_MAJESTIC]);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM experience_majestic i ORDER BY i.level ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			CharacterExperience * add_experience = new CharacterExperience;
			add_experience->SetLevel(fields[0].GetUInt16());
			add_experience->SetExperience(fields[1].GetInt64());
			add_experience->SetAccumulatedExperience(fields[2].GetInt64());

			/*
			Acumulado Anterior + Exp Actual = Acumulado Actual
			*/

			if ( add_experience->GetLevel() > 0 )
			{
				add_experience->SetAccumulatedExperience(add_experience->GetExperience() + this->m_character_experience[LEVEL_DATA_MAJESTIC][add_experience->GetLevel() - 1]->GetAccumulatedExperience());
			}
			
			this->m_character_experience[LEVEL_DATA_MAJESTIC][add_experience->GetLevel()] = add_experience;

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience majestic definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

int64 CharacterBaseMgr::GetCharacterExperience(uint8 type, int16 level, bool accumulated) const
{
	int16 level_tmp = level > sGameServer->GetMaxLevel(type) ? sGameServer->GetMaxLevel(type) : level < 0 ? 0 : level;

	CharacterExperienceMap::const_iterator it = this->m_character_experience[type].find(level_tmp);

	if ( it != this->m_character_experience[type].end() )
	{
		if ( accumulated )
		{
			return it->second->GetAccumulatedExperience();
		}
		else
		{
			return it->second->GetExperience();
		}
	}
	else
	{
		return 0;
	}
}

void CharacterBaseMgr::FixCharacterExperience(Player* pPlayer)
{
	uint8 level_type = LEVEL_DATA_NORMAL;

	if ( pPlayer->IsMaster() )
	{
		level_type = LEVEL_DATA_MASTER;
	}

	if ( pPlayer->IsMajestic() )
	{
		level_type = LEVEL_DATA_MAJESTIC;
	}

	LevelData* pLevelData = pPlayer->GetLevelData(level_type);

	//int64 experience_min = sCharacterBase->GetCharacterExperience(level_type, pLevelData->GetLevel());
	int64 experience_min = 0;
	int64 experience_max = sCharacterBase->GetCharacterExperience(level_type, pLevelData->GetLevel() + 1);
	
	if ( pLevelData->GetLevel() >= sGameServer->GetMaxLevel(level_type) )
	{
		pLevelData->SetExperience(experience_max);
	}

	if ( pLevelData->GetExperience() > experience_max )
	{
		pLevelData->SetExperience(experience_max);
	}

	if ( pLevelData->GetExperience() < experience_min )
	{
		pLevelData->SetExperience(experience_min);
	}
}

void CharacterBaseMgr::LoadCharacterHelper()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Helper...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		MAP_CLEAR(CharacterHelperMap::iterator, this->m_character_helper[i]);
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_helper");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 race = fields[0].GetUInt8();

			if ( race < Character::MAX_CLASS )
			{
				character_helper * add_helper = new character_helper;

				add_helper->SetStage(fields[1].GetInt32());
				add_helper->SetDuration(fields[2].GetUInt32() * MINUTE * IN_MILLISECONDS);
				add_helper->SetCost(fields[3].GetUInt32());
				add_helper->SetCostInterval(fields[4].GetUInt32() * MINUTE * IN_MILLISECONDS);
				add_helper->SetNextStage(fields[5].GetInt32());

				this->m_character_helper[race][add_helper->GetStage()] = add_helper;
			}

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character helper stages", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterFormula()
{
	sLog->outLoad(true, "Loading Character Formula...");

	CLEAR_LIST(this->m_CharacterFormulaList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM character_formula");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			character_formula * add_formula = new character_formula;

			add_formula->SetType(reader.GetUInt8());
			add_formula->SetClass(reader.GetUInt8());
			add_formula->SetLevelMul(reader.GetFloat());
			add_formula->SetLevelDiv(reader.GetFloat());

			if (add_formula->GetLevelDiv() == 0)
			{
				add_formula->SetLevelDiv(1);
			}

			for (int32 i = 0; i < MAX_STAT_TYPE; ++i)
			{
				add_formula->SetStatMul(i, reader.GetFloat());
				add_formula->SetStatDiv(i, reader.GetFloat());

				if (add_formula->GetStatDiv(i) == 0)
				{
					add_formula->SetStatDiv(i, 1);
				}
			}

			this->m_CharacterFormulaList.push_back(add_formula);
			count++;
		} while (result->NextRow());
	}
	
	sLog->outLoad(false, ">> Loaded %u character formula definitions", count);
}

void CharacterBaseMgr::LoadCharacterStatFruit()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Stat Fruit...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		LIST_CLEAR(CharacterStatFruitList::iterator, this->m_character_stat_fruit[i]);
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_stat_fruit");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 race = fields[0].GetUInt8();
		
			if ( race < Character::MAX_CLASS )
			{
				character_stat_fruit * add_stat_fruit = new character_stat_fruit;

				add_stat_fruit->SetMinLevel(fields[1].GetInt16());
				add_stat_fruit->SetMaxLevel(fields[2].GetInt16());
				add_stat_fruit->SetTotalAddPoints(fields[3].GetUInt16());
				add_stat_fruit->SetTotalMinusPoints(fields[4].GetUInt16());

				this->m_character_stat_fruit[race].push_back(add_stat_fruit);
			}

			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character stat fruit definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterItem()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Item...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		LIST_CLEAR(CharacterItemList::iterator, this->m_character_item[i]);
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_item");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 race = fields[0].GetUInt8();
		
			if ( race < Character::MAX_CLASS )
			{
				character_item * add_item = new character_item;

				add_item->SetSlot(fields[1].GetUInt8());
				add_item->SetType(fields[2].GetUInt8());
				add_item->SetIndex(fields[3].GetUInt16());

				if ( !sItemMgr->IsItem(add_item->GetItem()) )
				{
					sLog->outError(LOG_DEFAULT, "LoadCharacterItem() Race: %u, slot: %d, item: %d %d", 
						race, add_item->GetSlot(), add_item->GetType(), add_item->GetIndex());

					delete add_item;
					continue;
				}

				add_item->SetLevel(fields[4].GetUInt8());
				add_item->SetDurability(fields[5].GetUInt8());
				add_item->SetSkill(fields[6].GetUInt8());
				add_item->SetLuck(fields[7].GetUInt8());
				add_item->SetOption(fields[8].GetUInt8());
				add_item->SetExcellent(fields[9].GetUInt8());
				add_item->SetAncient(fields[10].GetUInt8());
				add_item->SetDuration(fields[11].GetInt64());

				this->m_character_item[race].push_back(add_item);
			}

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character item definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterSpecialization()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Specialization...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		LIST_CLEAR(CharacterSpecializationList::iterator, this->m_character_specialization[i]);
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_specialization");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			uint8 race = fields[loop++].GetUInt8();
					
			if ( race < Character::MAX_CLASS )
			{
				character_specialization * add_data = new character_specialization;

				add_data->SetRace(race);
				add_data->SetStat(fields[loop++].GetUInt8());
				add_data->SetRequiredStat(fields[loop++].GetUInt8());
				add_data->SetRequiredStatMin(fields[loop++].GetInt32());
				add_data->SetRequiredStatMax(fields[loop++].GetInt32());
				add_data->SetPercentMin(fields[loop++].GetUInt8());
				add_data->SetPercentMax(fields[loop++].GetUInt8());
				add_data->SetRequiredStatLimit(fields[loop++].GetInt32());
				
				this->m_character_specialization[race].push_back(add_data);
			}

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character specialization definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceAdjust()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Adjust...");

	LIST_CLEAR(CharacterExperienceAdjust::iterator, this->m_character_experience_adjust);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_experience_adjust");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			character_experience_adjust * add_data = new character_experience_adjust;

			add_data->SetLevelMin(fields[loop++].GetInt16());
			add_data->SetLevelMax(fields[loop++].GetInt16());
			add_data->SetExperienceRate(fields[loop++].GetInt16());
				
			this->m_character_experience_adjust.push_back(add_data);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience adjust definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceBonus()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Bonus...");

	LIST_CLEAR(CharacterExperienceBonus::iterator, this->m_character_experience_bonus);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_experience_bonus");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			character_experience_bonus * add_data = new character_experience_bonus;

			add_data->SetRace(0, fields[loop++].GetUInt8());
			add_data->SetRace(1, fields[loop++].GetUInt8());
			add_data->SetRace(2, fields[loop++].GetUInt8());

			if ( add_data->GetRace(0) >= Character::MAX_CLASS ||
				 add_data->GetRace(1) >= Character::MAX_CLASS ||
				 add_data->GetRace(2) >= Character::MAX_CLASS )
			{
				delete add_data;
				continue;
			}
				
			this->m_character_experience_bonus.push_back(add_data);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience bonus definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterBonus()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Bonus...");

	LIST_CLEAR(CharacterBonusList::iterator, this->m_character_bonus_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM character_bonus i WHERE i.server = '%u'", sGameServer->GetServerCode());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 1;

			character_bonus * add_data = new character_bonus;

			add_data->SetLevelMin(fields[loop++].GetInt16());
			add_data->SetLevelMax(fields[loop++].GetInt16());
			add_data->SetExperienceRate(fields[loop++].GetInt16());
			add_data->SetDropRate(fields[loop++].GetInt16());

			this->m_character_bonus_list.push_back(add_data);

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character bonus definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterMaxLevelReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Max Level Reward...");

	LIST_CLEAR(CharacterMaxLevelRewardList::iterator, this->m_CharacterMaxLevelRewardList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM character_max_level_reward");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			CharacterMaxLevelReward * add_data = new CharacterMaxLevelReward;

			add_data->SetClass(fields[loop++].GetUInt8());
			add_data->SetMaxLevel(fields[loop++].GetInt16());
			add_data->SetType(fields[loop++].GetUInt8());
			add_data->SetIndex(fields[loop++].GetUInt16());
			add_data->SetLevel(fields[loop++].GetUInt8());
			add_data->SetDurability(fields[loop++].GetUInt8());
			add_data->SetSkill(fields[loop++].GetUInt8());
			add_data->SetLuck(fields[loop++].GetUInt8());
			add_data->SetOption(fields[loop++].GetUInt8());
			add_data->SetExcellent(fields[loop++].GetUInt8());
			add_data->SetAncient(fields[loop++].GetUInt8());
			add_data->SetHarmony(fields[loop++].GetUInt8());
			add_data->SetOption380(fields[loop++].GetUInt8());

			SOCKET_SLOT_LOOP(i)
			{
				add_data->SetSocket(i, fields[loop++].GetUInt16());
			}

			add_data->SetSocketBonus(fields[loop++].GetUInt8());
			add_data->SetFlags(fields[loop++].GetUInt32());

			this->m_CharacterMaxLevelRewardList.push_back(add_data);

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character max level reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceTable()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Table...");

	LIST_CLEAR(CharacterExperienceTableList::iterator, this->m_CharacterExperienceTableList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM character_experience_table");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			CharacterExperienceTable * add_data = new CharacterExperienceTable;

			add_data->SetLevelMin(fields[loop++].GetInt16());
			add_data->SetLevelMax(fields[loop++].GetInt16());
			add_data->SetExperienceRate(fields[loop++].GetInt32());
			
			this->m_CharacterExperienceTableList.push_back(add_data);

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience table definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

character_helper * CharacterBaseMgr::GetHelperStage(uint8 race, int32 stage) const
{
	if ( race >= Character::MAX_CLASS )
		return nullptr;

	CharacterHelperMap::const_iterator it = this->m_character_helper[race].find(stage);

	if ( it != this->m_character_helper[race].end() )
	{
		return it->second;
	}

	return nullptr;
}

uint16 CharacterBaseMgr::GetStatFruitMaxAdd(uint8 race, int16 level)
{
	if ( race >= Character::MAX_CLASS )
		return 0;

	uint16 stat_fruit_max = 0;

	for ( CharacterStatFruitList::iterator it = this->m_character_stat_fruit[race].begin(); it != this->m_character_stat_fruit[race].end(); ++it )
	{
		if ( !(*it) )
			continue;

		if ( level < (*it)->GetMinLevel() || level > (*it)->GetMaxLevel() )
			continue;

		stat_fruit_max = (*it)->GetTotalAddPoints();
	}

	return stat_fruit_max;
}
	
uint16 CharacterBaseMgr::GetStatFruitMaxDec(uint8 race, int16 level)
{
	if ( race >= Character::MAX_CLASS )
		return 0;

	uint16 stat_fruit_max = 0;

	for ( CharacterStatFruitList::const_iterator it = this->m_character_stat_fruit[race].begin(); it != this->m_character_stat_fruit[race].end(); ++it )
	{
		if ( !(*it) )
			continue;

		if ( level < (*it)->GetMinLevel() || level > (*it)->GetMaxLevel() )
			continue;

		stat_fruit_max = (*it)->GetTotalMinusPoints();
	}

	return stat_fruit_max;
}

void CharacterBaseMgr::GenerateCharacterItemList(Player* pPlayer)
{
	if ( pPlayer->GetClass() >= Character::MAX_CLASS )
	{
		return;
	}

	CharacterItemList const& item_list = this->m_character_item[pPlayer->GetClass()];

	for ( CharacterItemList::const_iterator it = item_list.begin(); it != item_list.end(); ++it )
	{
		Item item((*it)->GetItem(), (*it)->GetLevel(), (*it)->GetDurability(), (*it)->GetSkill(), (*it)->GetLuck(), (*it)->GetOption(), (*it)->GetExcellent(), (*it)->GetAncient());

		if ( (*it)->GetDuration() > 0 )
		{
			item.SetExpireDate(time(nullptr) + (*it)->GetDuration());
		}

		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, item, (*it)->GetSlot());
	}
}

void CharacterBaseMgr::CalculateStatFormula(Player* pPlayer)
{
	for (auto const& pData : this->m_CharacterFormulaList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() != pPlayer->GetClass())
		{
			continue;
		}


		int32 level = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel();

		if (pData->GetType() == STAT_ELEMENTAL_ATTACK_SUCCESS_RATE || pData->GetType() == STAT_ELEMENTAL_DEFENSE_SUCCESS_RATE)
		{
			level = pPlayer->GetTotalLevel();
		}

		float value = (level * pData->GetLevelMul()) / pData->GetLevelDiv();

		for (int32 n = 0; n < MAX_STAT_TYPE; ++n)
		{
			value += (pPlayer->GetTotalStat(n) * pData->GetStatMul(n)) / pData->GetStatDiv(n);
		}

		if (value < 0)
		{
			value = 0;
		}

		pPlayer->SetBaseStat(pData->GetType(), value);
	}

	if (pPlayer->GetBaseStat(STAT_ATTACK_RATE) > 0xFFFF)
	{
		pPlayer->SetBaseStat(STAT_ATTACK_RATE, 0xFFFF);
	}

	if (pPlayer->GetClass() == Character::FAIRY_ELF)
	{
		auto pWeapon01 = pPlayer->GetInventory()->GetItem(WEAPON_01);
		auto pWeapon02 = pPlayer->GetInventory()->GetItem(WEAPON_02);

		if (pWeapon01 && pWeapon01->IsValid() && (pWeapon01->GetKind2() == ItemKind::CROSSBOW || pWeapon01->GetKind2() == ItemKind::CROSSBOW))
		{
			pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MIN, pPlayer->GetTotalStat(STRENGTH) / 14 + pPlayer->GetTotalStat(AGILITY) / 7);
			pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MAX, (pPlayer->GetTotalStat(STRENGTH) >> 3) + (pPlayer->GetTotalStat(AGILITY) >> 2));
		}
	}

	if (pPlayer->GetBuffByGroup(325))
	{
		pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MIN, pPlayer->GetTotalStat(STRENGTH) / 9);
		pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MAX, pPlayer->GetTotalStat(STRENGTH) / 6);
		pPlayer->SetBaseStat(STAT_DEFENSE, pPlayer->GetTotalStat(AGILITY) / 2);
	}

	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MAX));
	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MAX));

	if (pPlayer->GetClass() == Character::FAIRY_ELF)
	{
		auto pWeapon01 = pPlayer->GetInventory()->GetItem(WEAPON_01);
		auto pWeapon02 = pPlayer->GetInventory()->GetItem(WEAPON_02);

		if (pWeapon01 && pWeapon01->IsValid() && (pWeapon01->GetKind2() == ItemKind::CROSSBOW || pWeapon01->GetKind2() == ItemKind::CROSSBOW))
		{
			if (pWeapon02 && pWeapon02->IsValid() && pWeapon02->GetKind2() == ItemKind::QUIVER)
			{
				pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 0);
				pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 0);
			}
		}
	}

	pPlayer->SetIntData(UNIT_INT_ATTACK_RATE, pPlayer->GetBaseStat(STAT_ATTACK_RATE));
	pPlayer->SetIntData(UNIT_INT_ATTACK_RATE_PVP, pPlayer->GetBaseStat(STAT_ATTACK_RATE_PVP));
	pPlayer->SetIntData(UNIT_INT_ATTACK_SPEED, pPlayer->GetBaseStat(STAT_ATTACK_SPEED));
	pPlayer->SetIntData(UNIT_INT_MAGIC_SPEED, pPlayer->GetBaseStat(STAT_MAGIC_SPEED));
	pPlayer->SetIntData(UNIT_INT_DEFENSE, pPlayer->GetBaseStat(STAT_DEFENSE));
	pPlayer->SetIntData(UNIT_INT_DEFENSE_RATE, pPlayer->GetBaseStat(STAT_DEFENSE_RATE));
	pPlayer->SetIntData(UNIT_INT_DEFENSE_RATE_PVP, pPlayer->GetBaseStat(STAT_DEFENSE_RATE_PVP));
	pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MAX));
	pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MAX));

	if (pPlayer->HasBuff(301) || pPlayer->HasBuff(302) || pPlayer->HasBuff(303))
	{
		pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MIN));
		pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MAX));
		pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MIN));
		pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MAX));
	}

	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, pPlayer->GetBaseStat(STAT_ELEMENTAL_DEFENSE));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_ELEMENTAL_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_ELEMENTAL_DAMAGE_MAX));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, pPlayer->GetBaseStat(STAT_ELEMENTAL_ATTACK_SUCCESS_RATE));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, pPlayer->GetBaseStat(STAT_ELEMENTAL_DEFENSE_SUCCESS_RATE));

	pPlayer->SetData(PLAYER_DATA_FENRIR_DAMAGE, pPlayer->GetBaseStat(STAT_FENRIR_DAMAGE));

	pPlayer->SetIntData(UNIT_INT_COMBO_DAMAGE, pPlayer->GetBaseStat(STAT_COMBO_DAMAGE));
}

void CharacterBaseMgr::CalculateSpecialization(Player* pPlayer)
{
	for ( int32 i = 0; i < CHARACTER_SPECIALIZATION_MAX; ++i )
	{
		this->CalculateSpecialization(pPlayer, i);
	}
}
	
void CharacterBaseMgr::CalculateSpecialization(Player* pPlayer, uint8 stat)
{
	if ( pPlayer->GetClass() >= Character::MAX_CLASS )
	{
		return;
	}

	CharacterSpecializationList const& specialization_list = this->m_character_specialization[pPlayer->GetClass()];

	for ( CharacterSpecializationList::const_iterator it = specialization_list.begin(); it != specialization_list.end(); ++it )
	{
		if ( (*it)->GetStat() == stat && (*it)->GetRequiredStat() < MAX_STAT_TYPE )
		{
			if ( pPlayer->GetTotalStat((*it)->GetRequiredStat()) >= (*it)->GetRequiredStatMin() && (pPlayer->GetTotalStat((*it)->GetRequiredStat()) <= (*it)->GetRequiredStatMax() || (*it)->GetRequiredStatMax() == -1) )
			{
				float percent = (*it)->GetPercentMax() - (*it)->GetPercentMin();
				int32 stat_min = (*it)->GetRequiredStatMin() - 1;
				int32 stat_max = (*it)->GetRequiredStatMax();

				if ( stat_max == -1 || stat_max > (*it)->GetRequiredStatLimit() )
				{
					stat_max = (*it)->GetRequiredStatLimit();
				}

				int32 stat_value = pPlayer->GetTotalStat((*it)->GetRequiredStat());

				if ( stat_value > (*it)->GetRequiredStatLimit() )
				{
					stat_value = (*it)->GetRequiredStatLimit();
				}

				stat_value -= stat_min;

				float final_percent = (float)(stat_max - stat_min) / percent;
				
				this->ApplySpecialization(pPlayer, stat, (float)(stat_value / final_percent) + (*it)->GetPercentMin());
				return;
			}
		}
	}
}

void CharacterBaseMgr::ApplySpecialization(Player* pPlayer, uint8 stat, float percent)
{
	switch ( stat )
	{
	case CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MIN:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MIN) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pPlayer->GetSpecializationStat(stat));
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MAX:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MAX) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pPlayer->GetSpecializationStat(stat));
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_RATE) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_RATE, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE_PVP:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_RATE_PVP) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_RATE_PVP, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_DEFENSE:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_DEFENSE) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_DEFENSE, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_DEFENSE_RATE) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_DEFENSE_RATE, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE_PVP:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_DEFENSE_RATE_PVP) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_DEFENSE_RATE_PVP, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MIN:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MIN) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MAX:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MAX) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MIN:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MIN) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MAX:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MAX) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, pPlayer->GetSpecializationStat(stat));
		} break;
	}
}

void CharacterBaseMgr::ApplyExperienceAdjust(Player* pPlayer, int64 & experience)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( CharacterExperienceAdjust::const_iterator it = this->m_character_experience_adjust.begin(); it != m_character_experience_adjust.end(); ++it )
	{
		if ( pPlayer->GetTotalLevel() >= (*it)->GetLevelMin() && pPlayer->GetTotalLevel() <= (*it)->GetLevelMax() )
		{
			experience = (experience * (*it)->GetExperienceRate()) / 100;
			break;
		}
	}
}

bool CharacterBaseMgr::IsExperienceBonus(bool const* members) const
{
	if ( !members )
	{
		return false;
	}

	for ( CharacterExperienceBonus::const_iterator it = this->m_character_experience_bonus.begin(); it != m_character_experience_bonus.end(); ++it )
	{
		if ( members[(*it)->GetRace(0)] &&
			 members[(*it)->GetRace(1)] &&
			 members[(*it)->GetRace(2)] )
		{
			return true;
		}
	}

	return false;
}

int32 CharacterBaseMgr::GetBonus(Player* pPlayer, bool drop)
{
	if ( !pPlayer )
	{
		return 0;
	}

	int32 value = 0;

	for ( CharacterBonusList::const_iterator it = this->m_character_bonus_list.begin(); it != this->m_character_bonus_list.end(); ++it )
	{
		if ( pPlayer->GetTotalLevel() >= (*it)->GetLevelMin() && pPlayer->GetTotalLevel() <= (*it)->GetLevelMax() )
		{
			if ( drop )
			{
				value = (*it)->GetDropRate();
			}
			else
			{
				value = (*it)->GetExperienceRate();
			}
			break;
		}
	}

	return value;
}

void CharacterBaseMgr::LevelUpReward(Player* pPlayer, int16 level)
{
	if (!pPlayer)
	{
		return;
	}

	for (CharacterMaxLevelRewardList::const_iterator itr = this->m_CharacterMaxLevelRewardList.begin(); itr != this->m_CharacterMaxLevelRewardList.end(); ++itr)
	{
		CharacterMaxLevelReward const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() != pPlayer->GetClass())
		{
			continue;
		}

		if (pPlayer->GetTotalLevel() < pData->GetMaxLevel())
		{
			continue;
		}

		if (level >= pData->GetMaxLevel())
		{
			continue;
		}

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_INSERT);
		stmt->setUInt32(pPlayer->GetGUID());
		stmt->setUInt16(ITEMGET(pData->GetType(), pData->GetIndex()));
		stmt->setUInt8(pData->GetLevel());
		stmt->setUInt8(pData->GetDurability());
		stmt->setUInt8(pData->GetSkill());
		stmt->setUInt8(pData->GetLuck());
		stmt->setUInt8(pData->GetOption());
		stmt->setUInt8(pData->GetExcellent());
		stmt->setUInt8(pData->GetAncient());
		stmt->setUInt8(pData->GetHarmony());
		stmt->setUInt8(pData->GetOption380());
		stmt->setUInt16(pData->GetSocket(0));
		stmt->setUInt16(pData->GetSocket(1));
		stmt->setUInt16(pData->GetSocket(2));
		stmt->setUInt16(pData->GetSocket(3));
		stmt->setUInt16(pData->GetSocket(4));
		stmt->setUInt8(pData->GetSocketBonus());
		stmt->setUInt32(pData->GetFlags());

		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}
}

CharacterExperienceTable const* CharacterBaseMgr::GetCharacterExperienceTable(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return nullptr;
	}

	for (CharacterExperienceTableList::const_iterator itr = this->m_CharacterExperienceTableList.begin(); itr != this->m_CharacterExperienceTableList.end(); ++itr)
	{
		CharacterExperienceTable const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pPlayer->GetTotalLevel() >= pData->GetLevelMin() && pPlayer->GetTotalLevel() <= pData->GetLevelMax())
		{
			return pData;
		}
	}

	return nullptr;
}