/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "SkillMgr.cpp"
*
*/

bool skill_template::IsValidStatus(uint8 status) const
{
	if ( this->GetStatus(0) == 0 && this->GetStatus(1) == 0 && this->GetStatus(2) == 0 )
		return true;

	return status == GUILD_RANK_MASTER && this->GetStatus(0) == 1 ||
		   status == GUILD_RANK_ASISTANT && this->GetStatus(1) == 1 ||
		   status == GUILD_RANK_BATTLE_MASTER && this->GetStatus(2) == 1;
}

float skill_tree_majestic_stat::GetValue(uint8 level)
{
	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_STAT_GROUP; ++i)
	{
		if (level >= this->GetStartPoint(i) && level <= this->GetEndPoint(i))
		{
			return sFormulaMgr->GetOrCreateValue(3, this->GetFormulaID(i), level);
		}
	}

	return 0;
}

float skill_tree_majestic::GetValue(uint8 level)
{
	return sFormulaMgr->GetOrCreateValue(3, this->GetFormulaID(), level);
}

float SkillTree::GetValue(uint8 level)
{
	return sFormulaMgr->GetOrCreateValue(3, this->GetFormulaID(), level);
}

SkillMgr::SkillMgr()
{
}

SkillMgr::~SkillMgr()
{
	MAP_CLEAR(SkillTemplateMap::iterator, this->skill_template_map);
	LIST_CLEAR(SkillTreeList::iterator, this->m_SkillTreeList);
	MAP_CLEAR(BuffTemplateMap::iterator, this->buff_template_map);
	MAP_CLEAR(SkillSpecialMap::iterator, this->skill_special_map);
	MAP_CLEAR(SkillSpecialElementMap::iterator, this->skill_special_element_map);
	LIST_CLEAR(SkillTreeMajesticList::iterator, this->skill_tree_majestic_list);
	LIST_CLEAR(SkillTreeMajesticStatList::iterator, this->skill_tree_majestic_stat_list);
	LIST_CLEAR(SkillAttackTimeList::iterator, this->m_SkillAttackTimeList);
}

void SkillMgr::LoadSkillTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Template...");

	MAP_CLEAR(SkillTemplateMap::iterator, this->skill_template_map);
	
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_template i WHERE i.disabled = 0");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			skill_template * add_skill = new skill_template;

			add_skill->SetID(fields[loop++].GetUInt16());
			add_skill->SetName(fields[loop++].GetString());
			add_skill->level.set(fields[loop++].GetUInt16());
			add_skill->SetDamage(fields[loop++].GetInt32());
			add_skill->mana_cost.set(fields[loop++].GetInt32());
			add_skill->stamina_cost.set(fields[loop++].GetInt32());
			add_skill->SetEffectRange(fields[loop++].GetUInt8());
			add_skill->SetEffectRadio(fields[loop++].GetUInt8());
			add_skill->SetCooldown(fields[loop++].GetUInt32());
			add_skill->SetType(fields[loop++].GetInt8());
			add_skill->SetAttribute(fields[loop++].GetUInt8());
			add_skill->SetBuffIcon(fields[loop++].GetUInt16());
			add_skill->SetRequiredLevel(fields[loop++].GetInt16());

			for ( uint32 i = 0; i < MAX_STAT_TYPE; ++i )
			{
				add_skill->SetRequiredStat(i, fields[loop++].GetInt32());
			}

			add_skill->SetRequiredKillCount(fields[loop++].GetUInt8());
			add_skill->SetStatus(0, fields[loop++].GetUInt8());
			add_skill->SetStatus(1, fields[loop++].GetUInt8());
			add_skill->SetStatus(2, fields[loop++].GetUInt8());
			add_skill->SetBaseSkill(fields[loop++].GetUInt16());

			for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
			{
				add_skill->SetRequiredClass(i, fields[loop++].GetUInt8());
			}

			add_skill->animation.set(fields[loop++].GetUInt8());
			add_skill->SetElementalAttribute(fields[loop++].GetUInt8());
			add_skill->SetElementalBonusDamage(fields[loop++].GetUInt16());
			add_skill->SetAttackDelay(fields[loop++].GetUInt32());
			add_skill->SetMountCheck(fields[loop++].GetUInt8());
			add_skill->SetPVMDamage(fields[loop++].GetUInt8());
			add_skill->SetPVPDamage(fields[loop++].GetUInt8());
			add_skill->SetStat1(fields[loop++].GetInt32());
			add_skill->SetStatValue1(fields[loop++].GetInt32());
			add_skill->SetStat2(fields[loop++].GetInt32());
			add_skill->SetStatValue2(fields[loop++].GetInt32());

			switch (add_skill->GetStat1())
			{
			case 1:
			{
					  add_skill->SetStat1(STRENGTH);
			}break;

			case 2:
			{
					  add_skill->SetStat1(AGILITY);
			}break;

			case 3:
			{
					  add_skill->SetStat1(ENERGY);
			}break;

			case 4:
			{
					  add_skill->SetStat1(VITALITY);
			}break;

			default:
			{
					   add_skill->SetStat1(MAX_STAT_TYPE);
					   add_skill->SetStatValue1(0);
			}break;
			}

			switch (add_skill->GetStat2())
			{
			case 1:
			{
				add_skill->SetStat2(STRENGTH);
			}break;

			case 2:
			{
				add_skill->SetStat2(AGILITY);
			}break;

			case 3:
			{
				add_skill->SetStat2(ENERGY);
			}break;

			case 4:
			{
				add_skill->SetStat2(VITALITY);
			}break;

			default:
			{
				add_skill->SetStat2(MAX_STAT_TYPE);
				add_skill->SetStatValue2(0);
			}break;
			}

			add_skill->SetLearnItem(-1);
			add_skill->SetLearnItemLevel(0);
			add_skill->SetRequireSkill(0);
			add_skill->SetRequireMasterSkill(0);
			add_skill->SetRequireMasterLevel(0);

			this->skill_template_map[add_skill->GetID()] = add_skill;

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillLearn()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Learn...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_learn");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint16 item = ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16());
			uint8 level = fields[2].GetUInt8();

			if ( !sItemMgr->IsItem(item) )
			{
				sLog->outError(LOG_DEFAULT, "LoadSkillLearn() item: %u +%u", item, level);
				continue;
			}

			uint16 skill = fields[3].GetUInt16();

			skill_template * skill_info = this->GetSkill(skill);

			if ( !skill_info )
			{
				sLog->outError(LOG_DEFAULT, "LoadSkillLearn() skill: %u", skill);
				continue;
			}

			skill_info->SetLearnItem(item);
			skill_info->SetLearnItemLevel(level);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill learn definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillTree()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Tree...");

	LIST_CLEAR(SkillTreeList::iterator, this->m_SkillTreeList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_tree");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			SkillTree * add_skill = new SkillTree;

			add_skill->SetID(fields[loop++].GetUInt8());
			add_skill->SetClass(fields[loop++].GetUInt8());
			add_skill->SetGroup(fields[loop++].GetUInt8());
			add_skill->SetRequiredPoints(fields[loop++].GetUInt8());
			add_skill->SetMaxLevel(fields[loop++].GetUInt8());
			add_skill->SetLink(fields[loop++].GetUInt8());
			add_skill->SetRequiredSkill(0, fields[loop++].GetUInt16());
			add_skill->SetRequiredSkillPoints(0, fields[loop++].GetUInt8());
			add_skill->SetRequiredSkill(1, fields[loop++].GetUInt16());
			add_skill->SetRequiredSkillPoints(1, fields[loop++].GetUInt8());
			add_skill->SetSkill(fields[loop++].GetUInt16());
			add_skill->SetRelatedSkill(fields[loop++].GetUInt16());
			add_skill->SetReplaceSkill(fields[loop++].GetUInt16());
			add_skill->SetFormulaID(fields[loop++].GetUInt16());
			add_skill->SetUnk5(fields[loop++].GetUInt8());
			add_skill->SetUnk6(fields[loop++].GetUInt8());
			add_skill->SetUnk7(fields[loop++].GetUInt8());
			add_skill->SetType(fields[loop++].GetUInt8());

			this->m_SkillTreeList.push_back(add_skill);

			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill tree", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillSpecial()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Special...");

	MAP_CLEAR(SkillSpecialMap::iterator, this->skill_special_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_special");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			skill_special * add_skill = new skill_special;

			add_skill->guid.set(fields[0].GetUInt16());
			add_skill->SetName(fields[1].GetString());
			add_skill->target.set(fields[2].GetUInt8());
			add_skill->scope_type.set(fields[3].GetInt32());
			add_skill->scope_value.set(fields[4].GetUInt8());

			for ( uint8 i = 0; i < MONSTER_SKILL_ELEMENT_MAX; i++ )
			{
				add_skill->element[i].set(fields[5+i].GetInt32());
				add_skill->SetElement(i, this->GetSkillSpecialElement(fields[5+i].GetInt32()));
			}

			this->skill_special_map[add_skill->guid.get()] = add_skill;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill special definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillSpecialElement()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Special Element...");

	MAP_CLEAR(SkillSpecialElementMap::iterator, this->skill_special_element_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_special_element");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			skill_special_element * add_skill = new skill_special_element;

			add_skill->guid.set(fields[0].GetUInt32());
			add_skill->SetName(fields[1].GetString());
			add_skill->type.set(fields[2].GetUInt8());
			add_skill->rate.set(fields[3].GetUInt8());
			add_skill->duration.set(fields[4].GetInt32());
			add_skill->calc_type.set(fields[5].GetUInt8());
			add_skill->calc_value.set(fields[6].GetInt32());
			add_skill->magic.set(fields[7].GetInt32());

			this->skill_special_element_map[add_skill->guid.get()] = add_skill;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill special element definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadBuffTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Buff Template...");

	MAP_CLEAR(BuffTemplateMap::iterator, this->buff_template_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM buff_template");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			buff_template * add_buff = new buff_template;

			add_buff->SetBuff(reader.GetUInt16());
			add_buff->SetGroup(reader.GetUInt16());
			add_buff->SetItem(reader.ReadItem());
			add_buff->SetName(reader.GetString());
			add_buff->SetDescription(reader.GetString());
			add_buff->SetSend(reader.GetBool());
			add_buff->SetDebuff(reader.GetBool());

			for (int32 n = 0; n < MAX_BUFF_DATA; ++n)
			{
				add_buff->SetEffect(n, reader.GetUInt8());
				add_buff->SetValue(n, reader.GetFloat());
			}

			add_buff->SetDuration(reader.GetUInt32());

			this->buff_template_map[add_buff->GetBuff()] = add_buff;

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u buff definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillRequire()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Require...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_require");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint16 skill = fields[0].GetUInt16();

			skill_template * skill_info = this->GetSkill(skill);

			if ( !skill_info )
			{
				sLog->outError(LOG_DEFAULT, "LoadSkillRequire() skill: %u", skill);
				continue;
			}

			skill_info->SetRequireSkill(fields[1].GetUInt16());
			skill_info->SetRequireMasterSkill(fields[2].GetUInt16());
			skill_info->SetRequireMasterLevel(fields[3].GetUInt8());
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill require definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillTreeMajestic()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Tree Majestic...");

	LIST_CLEAR(SkillTreeMajesticList::iterator, this->skill_tree_majestic_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_tree_majestic i WHERE i.disabled = 0");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;
			skill_tree_majestic * add_data = new skill_tree_majestic;
			add_data->SetClass(fields[loop++].GetUInt8());
			add_data->SetID(fields[loop++].GetUInt16());
			add_data->SetSection(fields[loop++].GetUInt8());
			add_data->SetUIGroup(fields[loop++].GetUInt8());
			add_data->SetPositionTemp(fields[loop++].GetUInt8());
			add_data->SetSkillID(fields[loop++].GetUInt16());
			add_data->SetBuffID(fields[loop++].GetUInt16());
			add_data->SetRank(fields[loop++].GetUInt8());
			add_data->SetFormulaID(fields[loop++].GetUInt16());

			for ( int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_LINK; ++i )
			{
				add_data->SetLinkSlot(i, fields[loop++].GetInt32());
				add_data->SetLinkLine(i, fields[loop++].GetInt32());
			}

			add_data->SetRequiredPoints(fields[loop++].GetUInt8());
			add_data->SetMaxPoints(fields[loop++].GetUInt8());
			add_data->SetActiveNextPoints(fields[loop++].GetUInt8());
			add_data->SetParentSkill1(fields[loop++].GetUInt16());
			add_data->SetParentSkillPoints1(fields[loop++].GetUInt8());
			add_data->SetParentSkill2(fields[loop++].GetUInt16());
			add_data->SetParentSkillPoints2(fields[loop++].GetUInt8());
			add_data->SetParentSkill3(fields[loop++].GetUInt16());
			add_data->SetParentSkillPoints3(fields[loop++].GetUInt8());

			add_data->SetParentID1(-1);
			add_data->SetParentID2(-1);
			add_data->SetParentID3(-1);

			this->skill_tree_majestic_list.push_back(add_data);
			count++;
		}
		while(result->NextRow());
	}

	for ( SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr )
	{
		skill_tree_majestic * pSkill = *itr;

		if ( !pSkill )
		{
			continue;
		}

		for ( SkillTreeMajesticList::const_iterator it = this->skill_tree_majestic_list.begin(); it != this->skill_tree_majestic_list.end(); ++it )
		{
			skill_tree_majestic const* pSkill2 = *it;

			if ( !pSkill2 )
			{
				continue;
			}

			if ( pSkill->GetClass() == pSkill2->GetClass() &&
				 pSkill->GetSection() == pSkill2->GetSection() &&
				 pSkill->GetID() == pSkill2->GetID() )
			{
				continue;
			}

			if ( pSkill->GetSection() != pSkill2->GetSection() )
			{
				continue;
			}

			if (pSkill->GetClass() != pSkill2->GetClass())
			{
				continue;
			}

			if (pSkill->GetParentSkill2() != uint16(-1))
			{
				if (pSkill->GetParentSkill2() == pSkill2->GetSkillID())
				{
					pSkill->SetParentID2(pSkill2->GetID());
				}
			}

			if (pSkill->GetParentSkill3() != uint16(-1))
			{
				if (pSkill->GetParentSkill3() == pSkill2->GetSkillID())
				{
					pSkill->SetParentID3(pSkill2->GetID());
				}
			}
		}
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill tree majestic definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillTreeMajesticStat()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Tree Majestic Stat...");

	LIST_CLEAR(SkillTreeMajesticStatList::iterator, this->skill_tree_majestic_stat_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_tree_majestic_stat AS i");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;
			skill_tree_majestic_stat * add_data = new skill_tree_majestic_stat;
			add_data->SetClass(fields[loop++].GetUInt8());
			add_data->SetSection(fields[loop++].GetUInt8());
			add_data->SetID(fields[loop++].GetUInt8());
			add_data->SetSkillID(fields[loop++].GetUInt16());
			
			for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_STAT_STEP; ++i)
			{
				add_data->SetPointCondition(i, fields[loop++].GetUInt8());
				add_data->SetRequiredPoints(i, fields[loop++].GetUInt8());
				add_data->SetMaxPoints(i, fields[loop++].GetUInt8());
			}

			for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_STAT_GROUP; ++i)
			{
				add_data->SetStartPoint(i, fields[loop++].GetUInt8());
				add_data->SetEndPoint(i, fields[loop++].GetUInt8());
				add_data->SetFormulaID(i, fields[loop++].GetUInt16());
			}

			this->skill_tree_majestic_stat_list.push_back(add_data);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill tree majestic stat definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillAttackTime()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Attack Time...");

	LIST_CLEAR(SkillAttackTimeList::iterator, this->m_SkillAttackTimeList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_attack_time");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			SkillAttackTime* pData = new SkillAttackTime();
			pData->SetClass(fields[0].GetUInt8());
			pData->SetSkill(fields[1].GetUInt16());
			pData->SetStep(fields[2].GetUInt8());
			pData->SetSpeedMin(fields[3].GetInt32());
			pData->SetSpeedMax(fields[4].GetInt32());
			pData->SetTime(fields[5].GetUInt32());
			pData->SetRealTime(fields[6].GetUInt32());
			
			this->m_SkillAttackTimeList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill attack time definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool SkillMgr::IsSkill(uint16 skill) const
{
	return this->GetSkill(skill) != nullptr;
}

skill_template * SkillMgr::GetSkill(uint16 skill)
{
	SkillTemplateMap::iterator it = this->skill_template_map.find(skill);

	if ( it != this->skill_template_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

skill_template const* SkillMgr::GetSkill(uint16 skill) const
{
	SkillTemplateMap::const_iterator it = this->skill_template_map.find(skill);

	if ( it != this->skill_template_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

skill_template const* SkillMgr::GetSkillFromItem(uint16 item, uint8 level) const
{
	for ( SkillTemplateMap::const_iterator it = this->skill_template_map.begin(); it != this->skill_template_map.end(); ++it )
	{
		if ( it->second->GetLearnItem() == item &&
			 it->second->GetLearnItemLevel() == level )
		{
			return it->second;
		}
	}

	return nullptr;
}

SkillTree const* SkillMgr::GetSkillTree(uint16 skill, uint8 character) const
{
	for (SkillTreeList::const_iterator itr = this->m_SkillTreeList.begin(); itr != this->m_SkillTreeList.end(); ++itr)
	{
		SkillTree const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetSkill() == skill && pData->GetClass() == character)
		{
			return pData;
		}
	}

	return nullptr;
}

SkillTree * SkillMgr::GetSkillTree(uint16 skill, uint8 character)
{
	for (SkillTreeList::const_iterator itr = this->m_SkillTreeList.begin(); itr != this->m_SkillTreeList.end(); ++itr)
	{
		SkillTree * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetSkill() == skill && pData->GetClass() == character)
		{
			return pData;
		}
	}

	return nullptr;
}

buff_template const* SkillMgr::GetBuff(uint16 buff) const
{
	BuffTemplateMap::const_iterator it = this->buff_template_map.find(buff);

	if ( it != this->buff_template_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

buff_template const* SkillMgr::GetBuffItem(uint16 item) const
{
	if ( item == uint16(-1) )
	{
		return nullptr;
	}

	for ( BuffTemplateMap::const_iterator it = this->buff_template_map.begin(); it != this->buff_template_map.end(); ++it )
	{
		if ( it->second->GetItem() == item )
		{
			return it->second;
		}
	}

	return nullptr;
}

skill_special const* SkillMgr::GetSkillSpecial(uint16 guid) const
{
	SkillSpecialMap::const_iterator it = this->skill_special_map.find(guid);

	if ( it != this->skill_special_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

skill_special_element const* SkillMgr::GetSkillSpecialElement(uint32 guid) const
{
	SkillSpecialElementMap::const_iterator it = this->skill_special_element_map.find(guid);

	if ( it != this->skill_special_element_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

uint8 SkillMgr::GetSkillAttribute(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetAttribute();
	}
	else
	{
		return uint8(-1);
	}
}

uint16 SkillMgr::GetSkillBaseSkill(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetBaseSkill();
	}
	else
	{
		return 0;
	}
}
	
int8 SkillMgr::GetSkillType(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetType();
	}
	else
	{
		return SkillNone;
	}
}

int32 SkillMgr::GetSkillDamage(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetDamage();
	}
	else
	{
		return 0;
	}
}

uint16 SkillMgr::GetSkillBuffIcon(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetBuffIcon();
	}
	else
	{
		return 0;
	}
}

uint8 SkillMgr::GetSkillElementalAttribute(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetElementalAttribute();
	}
	else
	{
		return ELEMENTAL_ATTRIBUTE_NONE;
	}
}
	
uint16 SkillMgr::GetSkillElementalBonus(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetElementalBonusDamage();
	}
	else
	{
		return 0;
	}
}

bool SkillMgr::IsSkillRequiredClass(uint16 skill, uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->IsRequiredClass(race, change_up_1, change_up_2, change_up_3);
	}
	else
	{
		return false;
	}
}

bool SkillMgr::IsSkillRequire(Player* pPlayer, uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);
	int32 count = 0;

	if ( skill_info != nullptr )
	{
		if ( skill_info->GetRequireSkill() == 0 && skill_info->GetRequireMasterSkill() == 0 )
		{
			return true;
		}

		if ( skill_info->GetRequireSkill() != 0 )
		{
			SKILL_LOOP(i)
			{
				if ( !pPlayer->GetSkill(i)->GetSkill() )
				{
					continue;
				}

				if ( this->GetSkillBaseSkill(pPlayer->GetSkill(i)->GetSkill()) == skill_info->GetRequireSkill() )
				{
					if ( count == 0 )
					{
						count++;
					}
				}
			}
		}
		else
		{
			count++;
		}

		if ( skill_info->GetRequireMasterSkill() != 0 )
		{
			Skill* pMasterSkill = pPlayer->GetSkillTreeSkill(skill_info->GetRequireMasterSkill());

			if ( pMasterSkill )
			{
				if ( pMasterSkill->GetLevel() >= skill_info->GetRequireMasterLevel() )
				{
					count++;
				}
			}
		}
		else
		{
			count++;
		}

		if ( count >= 2 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

skill_tree_majestic const* SkillMgr::GetSkillTreeMajestic(uint8 race, uint8 section, uint16 id) const
{
	for ( SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr )
	{
		skill_tree_majestic const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id )
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic const* SkillMgr::GetSkillTreeMajestic(uint8 race, uint16 skill) const
{
	for ( SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr )
	{
		skill_tree_majestic const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetClass() == race && pData->GetSkillID() == skill )
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic * SkillMgr::GetSkillTreeMajestic(uint8 race, uint8 section, uint16 id)
{
	for (SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr)
	{
		skill_tree_majestic * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic * SkillMgr::GetSkillTreeMajestic(uint8 race, uint16 skill)
{
	for (SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr)
	{
		skill_tree_majestic * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSkillID() == skill)
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic_stat const* SkillMgr::GetSkillTreeMajesticStat(uint8 race, uint8 section, uint8 id) const
{
	for (SkillTreeMajesticStatList::const_iterator itr = this->skill_tree_majestic_stat_list.begin(); itr != this->skill_tree_majestic_stat_list.end(); ++itr)
	{
		skill_tree_majestic_stat const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic_stat * SkillMgr::GetSkillTreeMajesticStat(uint8 race, uint8 section, uint8 id)
{
	for (SkillTreeMajesticStatList::const_iterator itr = this->skill_tree_majestic_stat_list.begin(); itr != this->skill_tree_majestic_stat_list.end(); ++itr)
	{
		skill_tree_majestic_stat * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

SkillAttackTime const* SkillMgr::GetSkillAttackTime(Player* pPlayer, uint16 skill) const
{
	for (SkillAttackTimeList::const_iterator itr = this->m_SkillAttackTimeList.begin(); itr != this->m_SkillAttackTimeList.end(); ++itr)
	{
		SkillAttackTime const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == pPlayer->GetClass() && pData->GetSkill() == skill)
		{
			if (pPlayer->GetIntData(UNIT_INT_ATTACK_SPEED) >= pData->GetSpeedMin() && pPlayer->GetIntData(UNIT_INT_ATTACK_SPEED) <= pData->GetSpeedMax())
			{
				return pData;
			}
		}
	}
	
	return nullptr;
}