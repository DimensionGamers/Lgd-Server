/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerSkillTree.cpp"
*
*/

void Player::MasterSendStatus()
{
	if ( !this->IsMaster() )
		return;

	MASTER_STATUS_SEND pMsg;
	pMsg.level = this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() + this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel();
	pMsg.points = this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints();

	POWER_LOOP(i)
	{
		pMsg.power_max[i] = GET_MAX_WORD_VALUE(this->PowerGetTotal(i));
	}

	if ( this->IsMajestic() )
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetExperience(), pMsg.experience);
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetNextExperience(), pMsg.next_experience);
	}
	else
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MASTER)->GetExperience(), pMsg.experience);
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MASTER)->GetNextExperience(), pMsg.next_experience);
	}

	pMsg.ViewMaxHP = this->PowerGetTotal(POWER_LIFE);
	pMsg.ViewMaxMP = this->PowerGetTotal(POWER_MANA);
	pMsg.ViewMaxSD = this->PowerGetTotal(POWER_SHIELD);
	pMsg.ViewMaxAG = this->PowerGetTotal(POWER_STAMINA);

	this->SEND_PCT(pMsg);
}

void Player::MasterSendLevelUp()
{
	if ( !this->IsMaster() )
		return;

	MASTER_LEVELUP_SEND pMsg;
	pMsg.level = this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() + this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel();
	pMsg.points = this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints();

	POWER_LOOP(i)
	{
		pMsg.power_max[i] = GET_MAX_WORD_VALUE(this->PowerGetTotal(i));
	}

	pMsg.minuspoint = 0;
	pMsg.maxminuspoint = 0;

	pMsg.ViewMaxHP = this->PowerGetTotal(POWER_LIFE);
	pMsg.ViewMaxMP = this->PowerGetTotal(POWER_MANA);
	pMsg.ViewMaxSD = this->PowerGetTotal(POWER_SHIELD);
	pMsg.ViewMaxAG = this->PowerGetTotal(POWER_STAMINA);

	this->SEND_PCT(pMsg);

	this->EffectSend(0x10);
}

void Player::MasterSkillPointAdd(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->IsMaster() )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		return;
	}

	MASTER_SKILL_POINT_ADD_RESULT pMsg;
	pMsg.points = this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints();
	pMsg.result = 0;

	this->Ping();

	POINTER_PCT(MASTER_SKILL_POINT_ADD, lpMsg, Packet, 0);

	SkillTree * pSkillTreeData = sSkillMgr->GetSkillTree(lpMsg->skill, this->GetClass());

	if (!pSkillTreeData)
	{
		sLog->outError(LOG_SKILL, "%s -- Wrong Skill %u -- %s", __FUNCTION__, lpMsg->skill, this->BuildLog().c_str());
		this->SEND_PCT(pMsg);
		return;
	}

	if (this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints() < pSkillTreeData->GetRequiredPoints())
	{
		sLog->outError(LOG_SKILL, "%s -- Low Points %u [%d / %d] -- %s", __FUNCTION__, lpMsg->skill, this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints(), pSkillTreeData->GetRequiredPoints(), this->BuildLog().c_str());
		this->SEND_PCT(pMsg);
		return;
	}

	Skill* pSkillTree = this->GetSkillTreeSkill(lpMsg->skill);

	if ( !pSkillTree )
	{
		if (!this->CheckMasterReplaceSkill(pSkillTreeData->GetReplaceSkill()))
		{
			sLog->outError(LOG_SKILL, "%s -- Don't have required replace skill %u [%u] -- %s", __FUNCTION__, lpMsg->skill, pSkillTreeData->GetReplaceSkill(), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if (pSkillTreeData->GetRequiredSkill(0) != 0 && !this->CheckMasterRequireSkill(pSkillTreeData->GetRequiredSkill(0)))
		{
			sLog->outError(LOG_SKILL, "%s -- Don't have required skill1 %u [%u] -- %s", __FUNCTION__, lpMsg->skill, pSkillTreeData->GetRequiredSkill(0), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if (pSkillTreeData->GetRequiredSkill(1) != 0 && !this->CheckMasterRequireSkill(pSkillTreeData->GetRequiredSkill(1)))
		{
			sLog->outError(LOG_SKILL, "%s -- Don't have required skill2 %u [%u] -- %s", __FUNCTION__, lpMsg->skill, pSkillTreeData->GetRequiredSkill(1), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if (pSkillTreeData->GetRank() >= 1 && !this->CheckMasterRequireGroup(pSkillTreeData->GetGroup(), pSkillTreeData->GetRank() - 1))
		{
			sLog->outError(LOG_SKILL, "%s -- Don't have required skill rank %u -- %s", __FUNCTION__, lpMsg->skill, this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if (!this->ReplaceMasterSkill(pSkillTreeData->GetReplaceSkill(), pSkillTreeData->GetSkill(), pSkillTreeData->GetRequiredPoints()))
		{
			sLog->outError(LOG_SKILL, "%s -- Can't replace skill %u -- %s", __FUNCTION__, lpMsg->skill, this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if (pSkillTreeData->GetRelatedSkill() == pSkillTreeData->GetSkill())
		{
			this->MagicAdd(pSkillTreeData->GetSkill(), pSkillTreeData->GetRequiredPoints());
		}

		pSkillTree = this->AddSkillTreeSkill(pSkillTreeData->GetSkill(), pSkillTreeData->GetRequiredPoints());

		if (!pSkillTree)
		{
			sLog->outError(LOG_SKILL, "%s -- Can't add skill %u -- %s", __FUNCTION__, lpMsg->skill, this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}
	}
	else
	{
		if (pSkillTree->GetLevel() >= pSkillTreeData->GetMaxLevel())
		{
			sLog->outError(LOG_SKILL, "%s -- Max Skill Level Reached %u [%u / %u] -- %s", __FUNCTION__, lpMsg->skill, pSkillTree->GetLevel(), pSkillTreeData->GetMaxLevel(), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if (!this->ReplaceMasterSkill(pSkillTreeData->GetSkill(), pSkillTreeData->GetSkill(), pSkillTree->GetLevel() + pSkillTreeData->GetRequiredPoints()))
		{
			sLog->outError(LOG_SKILL, "%s -- Can't replace skill %u -- %s", __FUNCTION__, lpMsg->skill, this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		pSkillTree->SetLevel(pSkillTree->GetLevel() + pSkillTreeData->GetRequiredPoints());
	}

	this->GetLevelData(LEVEL_DATA_MASTER)->ReducePoints(pSkillTreeData->GetRequiredPoints());

	//this->MasterSkillSend();
	this->CheckSkill();

	this->WearedItemApplySkill();
	
	this->SendSkill();
	
	this->CalculateCharacter();

	pMsg.result = 1;
	pMsg.points = this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints();
	pMsg.skill = lpMsg->skill;
	pMsg.position = pSkillTreeData->GetID();
	pMsg.level = pSkillTree->GetLevel();
	pMsg.CurrentInfo = pSkillTreeData->GetValue(pSkillTree->GetLevel());

	if (pMsg.level < pSkillTreeData->GetMaxLevel())
		pMsg.NextInfo = pSkillTreeData->GetValue(pSkillTree->GetLevel() + 1);
	else
		pMsg.NextInfo = 0.0f;

	pMsg.type = pSkillTreeData->GetType();
	
	this->SEND_PCT(pMsg);
}

void Player::MasterSkillSend()
{
	uint8 buffer[8192];
	POINTER_PCT(MASTER_SKILL_HEAD, head, buffer, 0);
	POINTER_PCT(MASTER_SKILL_BODY, body, buffer, sizeof(MASTER_SKILL_HEAD));
	head->count = 0;
	head->unk[0] = 0;
	head->unk[1] = 0;
	head->unk[2] = 0;

	for (PlayerSkillTreeMap::const_iterator it = this->skill_tree_map.begin(); it != this->skill_tree_map.end(); ++it)
	{
		Skill const* pSkill = it->second;

		if (!pSkill)
		{
			continue;
		}

		SkillTree * pSkillTree = sSkillMgr->GetSkillTree(pSkill->GetSkill(), this->GetClass());

		if (!pSkillTree)
		{
			continue;
		}
		
		body[head->count].position = pSkillTree->GetID();
		body[head->count].level = pSkill->GetLevel();

		if (pSkill->GetLevel() > 0)
		{
			body[head->count].current_data = pSkillTree->GetValue(pSkill->GetLevel());
			body[head->count].next_data = pSkillTree->GetValue(pSkill->GetLevel());
			body[head->count].type = pSkillTree->GetType();

			if (pSkill->GetLevel() < pSkillTree->GetMaxLevel())
			{
				body[head->count].next_data = pSkillTree->GetValue(pSkill->GetLevel() + 1);
			}
		}
		else
		{
			body[head->count].current_data = 0;
			body[head->count].next_data = 0;
			body[head->count].type = 0;
		}

		head->count++;
	}

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MASTER_SKILL, sizeof(MASTER_SKILL_HEAD) + (head->count * sizeof(MASTER_SKILL_BODY)));

	this->sendPacket(buffer, head->h.get_size());
}

void Player::MasterSkillSend(uint16 skill)
{
	SkillTree const* pSkillTree = sSkillMgr->GetSkillTree(skill, this->GetClass());
	
	if (!pSkillTree)
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(MASTER_SKILL_HEAD, head, buffer, 0);
	POINTER_PCT(MASTER_SKILL_BODY, body, buffer, sizeof(MASTER_SKILL_HEAD));
	head->count = 0;
	head->unk[0] = 0;
	head->unk[1] = 0;
	head->unk[2] = 0;

	body[head->count].position = pSkillTree->GetID();
	body[head->count].level = 0;
	body[head->count].current_data = 0.0f;
	body[head->count].next_data = 0.0f;
	head->count++;
	
	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MASTER_SKILL, sizeof(MASTER_SKILL_HEAD) + (head->count * sizeof(MASTER_SKILL_BODY)));

	this->sendPacket(buffer, head->h.get_size());
}

Skill* Player::GetSkillTreeSkill(uint16 skill)
{
	PlayerSkillTreeMap::iterator it = this->skill_tree_map.find(skill);

	if ( it != this->skill_tree_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

Skill const* Player::GetSkillTreeSkill(uint16 skill) const
{
	PlayerSkillTreeMap::const_iterator it = this->skill_tree_map.find(skill);

	if ( it != this->skill_tree_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

bool Player::CheckMasterReplaceSkill(uint16 skill)
{
	return skill ? this->MagicGet(skill) != nullptr: true;
}
	
bool Player::CheckMasterRequireSkill(uint16 skill)
{
	Skill* pMasterSkill = this->GetSkillTreeSkill(skill);

	return pMasterSkill && pMasterSkill->GetLevel() >= MIN_SKILL_TREE_LEVEL;
}

bool Player::CheckMasterRequireGroup(uint8 group, uint8 rank) const
{
	for (PlayerSkillTreeMap::const_iterator it = this->skill_tree_map.begin(); it != this->skill_tree_map.end(); ++it)
	{
		Skill const* pSkill = it->second;

		if (!pSkill)
		{
			continue;
		}

		SkillTree const* pSkillTree = sSkillMgr->GetSkillTree(pSkill->GetSkill(), this->GetClass());

		if (!pSkillTree)
		{
			continue;
		}

		if (pSkillTree->GetGroup() == group && pSkillTree->GetRank() == rank && pSkill->GetLevel() >= MIN_SKILL_TREE_LEVEL)
		{
			return true;
		}
	}

	return false;
}

bool Player::ReplaceMasterSkill(uint16 replace, uint16 skill, uint8 level)
{
	Skill* pSkill = this->MagicGet(replace);

	if( !pSkill )
	{
		return ((replace > 0) ? ((replace == skill) ? true : false) : true);
	}
	else
	{
		pSkill->SetSkill(skill);
		pSkill->SetLevel(level);
		return true;
	}
}

Skill* Player::AddSkillTreeSkill(uint16 skill, uint8 level)
{
	skill_template const* skill_info = sSkillMgr->GetSkill(skill);

	if (!skill_info)
	{
		return nullptr;
	}

	if (!skill_info->IsRequiredClass(this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)))
	{
		return nullptr;
	}

	if (this->GetSkillTreeSkill(skill))
	{
		return nullptr;
	}

	Skill* pSkill = new Skill(skill, level);

	this->skill_tree_map[skill] = pSkill;
	return pSkill;
}

void Player::GetMasterSkillWeapon(uint16 & skill, uint8 & level)
{
	uint16 WeaponSkill = skill;

	switch( skill )
	{
	case SKILL_DEFENSE:
		{
		} break;

	case SKILL_FALLING_SLASH:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(328, level)) ? WeaponSkill : 328);
		} break;

	case SKILL_LUNGE:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(329, level)) ? WeaponSkill : 329);
		} break;

	case SKILL_UPPERCUT:
		{
		} break;

	case SKILL_CYCLONE:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(326, level)) ? WeaponSkill : 326);
			WeaponSkill = ((!this->GetMasterSkillLevel(479, level)) ? WeaponSkill : 479);
		} break;

	case SKILL_SLASH:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(327, level)) ? WeaponSkill : 327);
		} break;

	case SKILL_TRIPLE_SHOT:
		{
			/*WeaponSkill = (( !this->GetMasterSkillLevel(414, level) )?WeaponSkill:414);
			WeaponSkill = (( !this->GetMasterSkillLevel(418, level) )?WeaponSkill:418);
			WeaponSkill = (( !this->GetMajesticSkillLevel(1081, level) )?WeaponSkill:1081);*/

			uint16 skills[] = { 414, 418, 1081, 0 };
			for (int32 i = 0; skills[i] != 0; ++i)
			{
				if (this->GetMasterSkillLevel(skills[i], level) || this->GetMajesticSkillLevel(skills[i], level))
				{
					WeaponSkill = skills[i];
				}
				else
				{
					break;
				}
			}
		} break;

	case SKILL_FIRE_BREATH:
		{
		} break;

	case SKILL_POWER_SLASH:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(482, level)) ? WeaponSkill : 482);
			WeaponSkill = ((!this->GetMajesticSkillLevel(1091, level)) ? WeaponSkill : 1091);
		} break;

	case SKILL_EARTHSHAKE:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(512, level)) ? WeaponSkill : 512);
		} break;

	case SKILL_PLASMA_STORM:
		{
		} break;

	case SKILL_LARGE_RING_BLOWER:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(551, level)) ? WeaponSkill : 551);
			WeaponSkill = ((!this->GetMasterSkillLevel(554, level)) ? WeaponSkill : 554);
		} break;

	case SKILL_UPPER_BEAST:
		{
			WeaponSkill = ((!this->GetMasterSkillLevel(552, level)) ? WeaponSkill : 552);
			WeaponSkill = ((!this->GetMasterSkillLevel(555, level)) ? WeaponSkill : 555);
		} break;

	case SKILL_PHOENIX_SHOT:
		{
			/*
			WeaponSkill = (( !this->GetMasterSkillLevel(744, level) )?WeaponSkill:744);
			WeaponSkill = (( !this->GetMasterSkillLevel(745, level) )?WeaponSkill:745);
			WeaponSkill = (( !this->GetMajesticSkillLevel(1109, level) )?WeaponSkill:1109);*/

			uint16 skills[] = { 744, 745, 1109, 0 };
			for (int32 i = 0; skills[i] != 0; ++i)
			{
				if (this->GetMasterSkillLevel(skills[i], level) || this->GetMajesticSkillLevel(skills[i], level))
				{
					WeaponSkill = skills[i];
				}
				else
				{
					break;
				}
			}
		} break;
	}

	skill = WeaponSkill;
}

bool Player::MasterSkillReset(uint8 group)
{
	if (!this->IsMaster())
	{
		return false;
	}

	int32 group_count = 0;

	for (PlayerSkillTreeMap::const_iterator it = this->skill_tree_map.begin(); it != this->skill_tree_map.end(); ++it)
	{
		Skill const* pSkill = it->second;

		if (!pSkill)
		{
			continue;
		}

		SkillTree const* pSkillTree = sSkillMgr->GetSkillTree(pSkill->GetSkill(), this->GetClass());

		if (pSkillTree)
		{
			if (pSkillTree->GetGroup() == group || group == uint8(-1))
			{
				++group_count;
			}
		}
	}

	if (group_count <= 0)
	{
		return false;
	}

	std::vector<uint16> base_skill_list;

	for (PlayerSkillTreeMap::const_iterator it = this->skill_tree_map.begin(); it != this->skill_tree_map.end();)
	{
		Skill const* pSkill = it->second;

		if (!pSkill)
		{
			this->skill_tree_map.erase(it++);
			continue;
		}

		SkillTree const* pSkillTree = sSkillMgr->GetSkillTree(pSkill->GetSkill(), this->GetClass());

		if (!pSkillTree)
		{
			this->GetLevelData(LEVEL_DATA_MASTER)->IncreasePoints(pSkill->GetLevel());
			this->MasterSkillSend(pSkill->GetSkill());

			int32 result = this->MagicRemove(pSkill->GetSkill());

			if (result != -1)
			{
				this->SendSkill(result, pSkill->GetSkill(), pSkill->GetLevel(), false);
			}

			base_skill_list.push_back(pSkill->GetSkill());

			delete it->second;
			this->skill_tree_map.erase(it++);
			continue;
		}

		if (pSkillTree->GetGroup() == group || group == uint8(-1))
		{
			this->GetLevelData(LEVEL_DATA_MASTER)->IncreasePoints(pSkill->GetLevel());
			this->MasterSkillSend(pSkill->GetSkill());

			int32 result = this->MagicRemove(pSkill->GetSkill());

			if (result != -1)
			{
				this->SendSkill(result, pSkill->GetSkill(), pSkill->GetLevel(), false);
			}

			base_skill_list.push_back(pSkill->GetSkill());

			delete it->second;
			this->skill_tree_map.erase(it++);
			continue;
		}

		++it;
	}

	for (std::vector<uint16>::const_iterator it = base_skill_list.begin(); it != base_skill_list.end(); ++it)
	{
		uint16 skill_base = sSkillMgr->GetSkillBaseSkill(*it);

		if (!skill_base)
		{
			continue;
		}

		SkillTree const* pSkillTree = sSkillMgr->GetSkillTree(skill_base, this->GetClass());

		if (pSkillTree)
		{
			continue;
		}

		if (skill_base == SKILL_DEFENSE ||
			skill_base == SKILL_FALLING_SLASH ||
			skill_base == SKILL_LUNGE ||
			skill_base == SKILL_UPPERCUT ||
			skill_base == SKILL_SLASH ||
			skill_base == SKILL_TRIPLE_SHOT ||
			skill_base == SKILL_FIRE_BREATH ||
			skill_base == SKILL_POWER_SLASH ||
			skill_base == SKILL_FORCE ||
			skill_base == SKILL_FORCE_WAVE ||
			skill_base == SKILL_LARGE_RING_BLOWER ||
			skill_base == SKILL_UPPER_BEAST ||
			skill_base == SKILL_PHOENIX_SHOT)
		{
			continue;
		}

		this->MagicAdd(skill_base, 0);
	}

	base_skill_list.clear();

	this->MasterSkillSend();
	this->WearedItemApplySkill();

	this->CheckSkill();

	this->SendSkill();

	this->CalculateCharacter();

	this->MasterSendStatus();

	return true;
}

bool Player::GetMasterSkillLevel(uint16 skill, uint8 & level) const
{
	Skill const* pSkill = this->GetSkillTreeSkill(skill);

	if ( !pSkill )
	{
		return false;
	}

	level = pSkill->GetLevel();
	return true;
}

void Player::ApplySkillTree(bool stat)
{
	Item const* Right = this->GetInventory()->GetItem(WEAPON_01);
	Item const* Left = this->GetInventory()->GetItem(WEAPON_02);
	Item const* Wings = this->GetInventory()->GetItem(WINGS);

	bool has_wings = Wings->IsValid() && Wings->IsWingLvl3() && Wings->GetItem() != ITEMGET(12, 268);

	if (!stat)
	{
		if (Left->IsValid() && Left->GetKind2() == ItemKind::SHIELD)
		{
			float shield_mastery = this->MagicGetValue(402) + this->MagicGetValue(440) + this->MagicGetValue(532) + this->MagicGetValue(707);

			this->IncreaseIntData(UNIT_INT_DEFENSE_RATE, static_cast<int32>(shield_mastery));
		}

		float success_rate = this->MagicGetValue(313) + this->MagicGetValue(556) + this->MagicGetValue(591);

		this->PercentIntData(UNIT_INT_DEFENSE_RATE, success_rate);
	}

	for (auto & itr : this->skill_tree_map)
	{
		auto pSkill = itr.second;
		if (!pSkill)
		{
			continue;
		}

		const auto skill = pSkill->GetSkill();
		const auto level = pSkill->GetLevel();

		if (skill == SKILL_TREE_ADD_STRENGTH1 ||
			skill == SKILL_TREE_ADD_DEXTERITY1 ||
			skill == SKILL_TREE_ADD_VITALITY1 ||
			skill == SKILL_TREE_ADD_ENERGY1 ||
			skill == SKILL_TREE_ADD_LEADERSHIP ||
			skill == SKILL_TREE_ADD_STRENGTH2 ||
			skill == SKILL_TREE_ADD_DEXTERITY2 ||
			skill == SKILL_TREE_ADD_VITALITY2 ||
			skill == SKILL_TREE_ADD_ENERGY2 ||
			skill == SKILL_TREE_ADD_STRENGTH3 ||
			skill == SKILL_TREE_ADD_DEXTERITY3 ||
			skill == SKILL_TREE_ADD_VITALITY3 ||
			skill == SKILL_TREE_ADD_ENERGY3)
		{
			if (!stat)
			{
				continue;
			}
		}
		else
		{
			if (stat)
			{
				continue;
			}
		}

		float value = this->MagicGetValue(skill);

		switch (skill)
		{
		case SKILL_TREE_ADD_STRENGTH1:
		case SKILL_TREE_ADD_STRENGTH2:
		case SKILL_TREE_ADD_STRENGTH3:
		{
										 this->IncreaseAddStat(STRENGTH, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_DEXTERITY1:
		case SKILL_TREE_ADD_DEXTERITY2:
		case SKILL_TREE_ADD_DEXTERITY3:
		{
										  this->IncreaseAddStat(AGILITY, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_VITALITY1:
		case SKILL_TREE_ADD_VITALITY2:
		case SKILL_TREE_ADD_VITALITY3:
		{
										 this->IncreaseAddStat(VITALITY, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_ENERGY1:
		case SKILL_TREE_ADD_ENERGY2:
		case SKILL_TREE_ADD_ENERGY3:
		{
									   this->IncreaseAddStat(ENERGY, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_LEADERSHIP:
		{
										  if (this->GetClass() == Character::DARK_LORD)
										  {
											  this->IncreaseAddStat(LEADERSHIP, static_cast<int32>(value));
										  }
		} break;

		case SKILL_TREE_AUTOMATIC_LIFE_RECOVERY_1:
		case SKILL_TREE_AUTOMATIC_LIFE_RECOVERY_2:
		{
													 this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, value);
		} break;

		case SKILL_TREE_AUTOMATIC_MANA_RECOVERY_1:
		case SKILL_TREE_AUTOMATIC_MANA_RECOVERY_2:
		{
													 this->IncreaseDataFloat(PLAYER_FLOAT_MP_RECOVERY_RATE, value);
		} break;

		case SKILL_TREE_AUTOMATIC_STAMINA_RECOVERY_1:
		case SKILL_TREE_AUTOMATIC_STAMINA_RECOVERY_2:
		{
														this->IncreaseDataFloat(PLAYER_FLOAT_AG_RECOVERY_RATE, value);
		} break;

		case SKILL_TREE_SHIELD_RECOVERY_INCREASE_1:
		case SKILL_TREE_SHIELD_RECOVERY_INCREASE_2:
		{
													  this->IncreaseDataFloat(PLAYER_FLOAT_SD_RECOVERY_RATE, value);
		} break;

		case SKILL_TREE_MANA_REDUCTION_1:
		case SKILL_TREE_MANA_REDUCTION_2:
		{
											this->IncreaseDataFloat(PLAYER_FLOAT_MANA_REDUCTION, value);
		} break;

		case SKILL_TREE_ADD_STEEL_ARMOR_1:
		case SKILL_TREE_ADD_STEEL_ARMOR_2:
		case SKILL_TREE_ADD_STEEL_ARMOR_3:
		case SKILL_TREE_ADD_STEEL_ARMOR_4:
		case SKILL_TREE_ADD_STEEL_ARMOR_5:
		case SKILL_TREE_ADD_STEEL_ARMOR_6:
		case SKILL_TREE_ADD_STEEL_ARMOR_7:
		case SKILL_TREE_ADD_STEEL_ARMOR_8:
		case SKILL_TREE_ADD_STEEL_ARMOR_9:
		case SKILL_TREE_ADD_STEEL_ARMOR_10:
		{
											  this->IncreaseIntData(UNIT_INT_DEFENSE_BASE, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_PROTECTION_SHIELD_1:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_2:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_3:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_4:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_5:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_6:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_7:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_8:
		case SKILL_TREE_ADD_PROTECTION_SHIELD_9:
		{
												   if (Left && Left->IsValid() && Left->GetKind2() == ItemKind::SHIELD)
												   {
													   this->IncreaseDataFloat(PLAYER_DATA_SHIELD_PROTECTION, value);
												   }
		} break;

		case SKILL_TREE_ADD_SHIELD_BLOCK_1:
		case SKILL_TREE_ADD_SHIELD_BLOCK_2:
		case SKILL_TREE_ADD_SHIELD_BLOCK_3:
		case SKILL_TREE_ADD_SHIELD_BLOCK_4:
		case SKILL_TREE_ADD_SHIELD_BLOCK_5:
		case SKILL_TREE_ADD_SHIELD_BLOCK_6:
		case SKILL_TREE_ADD_SHIELD_BLOCK_7:
		case SKILL_TREE_ADD_SHIELD_BLOCK_8:
		case SKILL_TREE_ADD_SHIELD_BLOCK_9:
		{
											  if (Left && Left->IsValid() && Left->GetKind2() == ItemKind::SHIELD)
											  {
												  this->IncreaseDataFloat(PLAYER_DATA_SHIELD_BLOCK, value);
											  }
		} break;

		case SKILL_TREE_ADD_WEAPON_BLOCK_1:
		case SKILL_TREE_ADD_WEAPON_BLOCK_2:
		case SKILL_TREE_ADD_WEAPON_BLOCK_3:
		{
											  if (Right && Right->IsValid())
											  {
												  this->IncreaseDataFloat(PLAYER_DATA_WEAPON_BLOCK, value);
											  }
		} break;

		case SKILL_TREE_ADD_ABSORB_HP_1:
		case SKILL_TREE_ADD_ABSORB_HP_2:
		case SKILL_TREE_ADD_ABSORB_HP_3:
		case SKILL_TREE_ADD_ABSORB_HP_4:
		case SKILL_TREE_ADD_ABSORB_HP_5:
		case SKILL_TREE_ADD_ABSORB_HP_6:
		case SKILL_TREE_ADD_ABSORB_HP_7:
		case SKILL_TREE_ADD_ABSORB_HP_8:
		case SKILL_TREE_ADD_ABSORB_HP_9:
		case SKILL_TREE_ADD_ABSORB_HP_10:
		{
											this->IncreaseDataFloat(PLAYER_DATA_ABSORB_HP, value);
		} break;

		case SKILL_TREE_ADD_ABSORB_SD_1:
		case SKILL_TREE_ADD_ABSORB_SD_2:
		case SKILL_TREE_ADD_ABSORB_SD_3:
		case SKILL_TREE_ADD_ABSORB_SD_4:
		case SKILL_TREE_ADD_ABSORB_SD_5:
		case SKILL_TREE_ADD_ABSORB_SD_6:
		case SKILL_TREE_ADD_ABSORB_SD_7:
		case SKILL_TREE_ADD_ABSORB_SD_8:
		case SKILL_TREE_ADD_ABSORB_SD_9:
		case SKILL_TREE_ADD_ABSORB_SD_10:
		{
											this->IncreaseDataFloat(PLAYER_DATA_ABSORB_SD, value);
		} break;

		case SKILL_TREE_ATTACK_DAMAGE_MIN_1:
		case SKILL_TREE_ATTACK_DAMAGE_MIN_2:
		case SKILL_TREE_ATTACK_DAMAGE_MIN_3:
		{
											   this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
											   this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
		} break;

		case SKILL_TREE_WEAPON_MASTERY_1:
		case SKILL_TREE_WEAPON_MASTERY_2:
		case SKILL_TREE_WEAPON_MASTERY_3:
		case SKILL_TREE_WEAPON_MASTERY_4:
		case SKILL_TREE_WEAPON_MASTERY_5:
		case SKILL_TREE_WEAPON_MASTERY_6:
		{
											this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
											this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
											this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
											this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_MAX_PHYSI_DAMAGE1:
		case SKILL_TREE_ADD_MAX_PHYSI_DAMAGE2:
		case SKILL_TREE_ADD_MAX_PHYSI_DAMAGE3:
		{
												 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
												 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_DK_WING_DAMAGE:
		case SKILL_TREE_ADD_FE_WING_DAMAGE:
		case SKILL_TREE_ADD_DL_WING_DAMAGE:
		case SKILL_TREE_ADD_RF_WING_DAMAGE:
		case SKILL_TREE_ADD_GL_WING_DAMAGE:
		case 786:
			if (has_wings)
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_ADD_DW_WING_DAMAGE:
		case SKILL_RUNE_WIZARD_WINGS_DAMAGE_ENHANCEMENT:
		case 819:
			if (has_wings)
			{
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_ADD_SU_WING_DAMAGE:
			if (has_wings)
			{
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_ADD_MG_WING_DAMAGE:
			if (has_wings)
			{
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_WIZARDY_MIN_INCREASE:
		{
												this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_MAX_MAGIC_DAMAGE1:
		{
												 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
		} break;

		case SKILL_TREE_WIZARD_INCREASE_1:
		case SKILL_TREE_WIZARD_INCREASE_2:
		{
											 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
											 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
		} break;

		case SKILL_TREE_WIZARDY_AND_CURSE_MIN_INCREASE:
		{
														  this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
														  this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_MAX_MAGIC_DAMAGE2:
		{
												 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
												 this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, static_cast<int32>(value));
		} break;

		case SKILL_TREE_WIZARDY_AND_CURSE_INCREASE:
		{
													  this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
													  this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
													  this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, static_cast<int32>(value));
													  this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ATTACK_SUCCESS_RATE_INCREASE_1:
		case SKILL_TREE_ATTACK_SUCCESS_RATE_INCREASE_2:
		{
														  this->IncreaseIntData(UNIT_INT_ATTACK_RATE, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ATTACK_SUCCESS_RATE_PVP_INCREASE_1:
		case SKILL_TREE_ATTACK_SUCCESS_RATE_PVP_INCREASE_2:
		{
															  this->IncreaseIntData(UNIT_INT_ATTACK_RATE_PVP, static_cast<int32>(value));
		} break;

		case SKILL_TREE_DEFENSE_SUCCESS_RATE_PVP_INCREASE_1:
		case SKILL_TREE_DEFENSE_SUCCESS_RATE_PVP_INCREASE_2:
		{
															   this->IncreaseIntData(UNIT_INT_DEFENSE_RATE_PVP, static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_CRITICAL_DAMAGE_RATE1:
		case SKILL_TREE_ADD_CRITICAL_DAMAGE_RATE2:
		{
													 this->IncreaseFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, value);
		} break;

		case SKILL_TREE_ADD_EXCELLENT_DAMAGE_RATE1:
		case SKILL_TREE_ADD_EXCELLENT_DAMAGE_RATE2:
		{
													  this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, value);
		} break;

		case SKILL_TREE_ADD_DOUBLE_DAMAGE_RATE1:
		case SKILL_TREE_ADD_DOUBLE_DAMAGE_RATE2:
		{
												   this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, value);
		} break;

		case SKILL_TREE_ADD_IGNORE_DEFENSE_RATE1:
		case SKILL_TREE_ADD_IGNORE_DEFENSE_RATE2:
		{
													this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, value);
		} break;

		case SKILL_TREE_ADD_RESTORE_MP_RATE1:
		case SKILL_TREE_ADD_RESTORE_MP_RATE2:
		{
												this->IncreaseDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_MP_RECOVERY, value);
		} break;

		case SKILL_TREE_ADD_RESTORE_HP_RATE1:
		case SKILL_TREE_ADD_RESTORE_HP_RATE2:
		{
												this->IncreaseDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_HP_RECOVERY, value);
		} break;

		case SKILL_TREE_ADD_RESTORE_SD_RATE1:
		case SKILL_TREE_ADD_RESTORE_SD_RATE2:
		{
												this->IncreaseDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_SD_RECOVERY, value);
		} break;

		case SKILL_TREE_SHIELD_DEFENSE_1:
		case SKILL_TREE_SHIELD_DEFENSE_2:
		case SKILL_TREE_SHIELD_DEFENSE_3:
		case SKILL_TREE_ADD_SHIELD_IMPROVED:
			if (Left->IsValid() && Left->GetKind2() == ItemKind::SHIELD)
			{
				this->IncreaseIntData(UNIT_INT_DEFENSE, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_HORSE_DEFENSE:
		{
										 if (this->IsActiveInventoryItem(PET_DARK_HORSE))
										 {
											 this->IncreaseIntData(UNIT_INT_DEFENSE, static_cast<int32>(value));
										 }
		} break;

		case SKILL_TREE_ADD_DK_WING_DEFENSE:
		case SKILL_TREE_ADD_DW_WING_DEFENSE:
		case SKILL_TREE_ADD_FE_WING_DEFENSE:
		case SKILL_TREE_ADD_SU_WING_DEFENSE:
		case SKILL_TREE_ADD_MG_WING_DEFENSE:
		case SKILL_TREE_ADD_DL_WING_DEFENSE:
		case SKILL_TREE_ADD_RF_WING_DEFENSE:
		case SKILL_TREE_ADD_GL_WING_DEFENSE:
		case SKILL_RUNE_WIZARD_WINGS_DEFENSE_ENHANCEMENT:
		case 785:
		case 818:
			if (has_wings)
			{
				this->IncreaseIntData(UNIT_INT_DEFENSE, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_DEFENSE_1:
		case SKILL_TREE_DEFENSE_2:
		{
									 this->IncreaseIntData(UNIT_INT_DEFENSE, static_cast<int32>(value));
		} break;

		case SKILL_TREE_MONSTER_DIE_GIVE_LIFE_1:
		case SKILL_TREE_MONSTER_DIE_GIVE_LIFE_2:
		{
												   this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_HP, value);
		} break;

		case SKILL_TREE_MONSTER_DIE_GIVE_MANA_1:
		case SKILL_TREE_MONSTER_DIE_GIVE_MANA_2:
		{
												   this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_MP, value);
		} break;

		case SKILL_TREE_MONSTER_DIE_GIVE_SHIELD_1:
		case SKILL_TREE_MONSTER_DIE_GIVE_SHIELD_2:
		{
													 this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_SD, value);
		} break;

		case SKILL_TREE_DURABILITY_REDUCTION_1:
		case SKILL_TREE_DURABILITY_REDUCTION_1_RF:
		{
													 this->SetItemDurabilityReduction(1, value);
		} break;

		case SKILL_TREE_DURABILITY_REDUCTION_2:
		case SKILL_TREE_DURABILITY_REDUCTION_2_RF:
		{
													 this->SetItemDurabilityReduction(2, value);
		} break;

		case SKILL_TREE_DURABILITY_REDUCTION_3:
		case SKILL_TREE_DURABILITY_REDUCTION_3_RF:
		{
													 this->SetItemDurabilityReduction(3, value);
		} break;

		case SKILL_TREE_ADD_ARMOR_SET_BONUS1:
		case SKILL_TREE_ADD_ARMOR_SET_BONUS2:
		{
												if (this->IsArmorSetBonus())
												{
													this->IncreaseIntData(UNIT_INT_DEFENSE, static_cast<int32>(value));
												}
		} break;

		case SKILL_TREE_ADD_REFLECT_DAMAGE1:
		case SKILL_TREE_ADD_REFLECT_DAMAGE2:
		{
											   this->IncreaseFloatData(UNIT_FLOAT_TOTAL_REFLECT_DAMAGE, value);
		} break;

		case SKILL_TREE_INCREASE_MAX_SHIELD_1:
		case SKILL_TREE_INCREASE_MAX_SHIELD_2:
		{
												 this->PowerIncreseAdd(POWER_SHIELD, static_cast<int32>(value), false);
		} break;

		case SKILL_TREE_INCREASE_MAX_LIFE_1:
		case SKILL_TREE_INCREASE_MAX_LIFE_2:
		{
											   this->PowerIncreseAdd(POWER_LIFE, static_cast<int32>(value), false);
		} break;

		case SKILL_TREE_INCREASE_MAX_MANA_1:
		case SKILL_TREE_INCREASE_MAX_MANA_2:
		{
											   this->PowerIncreseAdd(POWER_MANA, static_cast<int32>(value), false);
		} break;

		case SKILL_TREE_ADD_MAX_BP1:
		{
									   this->PowerIncreseAdd(POWER_STAMINA, static_cast<int32>(value), false);
		} break;

		case SKILL_TREE_MACE_ATTACK_DAMAGE:
		{
											  if ((Right->IsValid() && Right->GetKind2() == ItemKind::MACE) || (Left->IsValid() && Left->GetKind2() == ItemKind::MACE))
											  {
												  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
												  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
												  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
												  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
											  }
		} break;

		case 354:
		{
					if ((Right->IsValid() && Right->GetKind2() == ItemKind::MACE) || (Left->IsValid() && Left->GetKind2() == ItemKind::MACE))
					{
						this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, value);
					}
		} break;

		case SKILL_TREE_FIST_WEAPON_ATTACK_DAMAGE:
		{
													 if ((Right->IsValid() && Right->GetKind2() == ItemKind::FIST) || (Left->IsValid() && Left->GetKind2() == ItemKind::FIST))
													 {
														 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
														 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
														 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
														 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
													 }
		} break;

		case SKILL_TREE_FIST_WEAPON_DOUBLE_DAMAGE:
		{
													 if ((Right->IsValid() && Right->GetKind2() == ItemKind::FIST) || (Left->IsValid() && Left->GetKind2() == ItemKind::FIST))
													 {
														 this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, value);
													 }
		} break;

		case SKILL_TREE_ONE_HAND_SWORD_ATTACK_DAMAGE:
			if ((Right->IsValid() && (Right->GetKind2() == ItemKind::SWORD || Right->GetKind2() == ItemKind::MAGIC_SWORD) && !Right->IsTwoHand()) || (Left->IsValid() && (Left->GetKind2() == ItemKind::SWORD || Left->GetKind2() == ItemKind::MAGIC_SWORD) && !Left->IsTwoHand()))
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_ONE_HAND_SWORD_SPEED:
			if ((Right->IsValid() && (Right->GetKind2() == ItemKind::SWORD || Right->GetKind2() == ItemKind::MAGIC_SWORD) && !Right->IsTwoHand()) || (Left->IsValid() && (Left->GetKind2() == ItemKind::SWORD || Left->GetKind2() == ItemKind::MAGIC_SWORD) && !Left->IsTwoHand()))
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_TWO_HAND_SWORD_ATTACK_DAMAGE:
			if (Right->IsValid() && (Right->GetKind2() == ItemKind::SWORD || Right->GetKind2() == ItemKind::MAGIC_SWORD) && Right->IsTwoHand())
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_TWO_HAND_SWORD_PVP_DAMAGE:
			if (Right->IsValid() && (Right->GetKind2() == ItemKind::SWORD || Right->GetKind2() == ItemKind::MAGIC_SWORD) && Right->IsTwoHand())
			{
				this->IncreaseData(PLAYER_DATA_380_PVP_DAMAGE, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_ONE_HAND_STAFF_WIZARD_DAMAGE:
			if ((Right->IsValid() && Right->GetKind2() == ItemKind::STAFF && !Right->IsTwoHand()) || (Left->IsValid() && Left->GetKind2() == ItemKind::STAFF && !Left->IsTwoHand()))
			{
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_ONE_HAND_STAFF_SPEED:
			if ((Right->IsValid() && Right->GetKind2() == ItemKind::STAFF && !Right->IsTwoHand()) || (Left->IsValid() && Left->GetKind2() == ItemKind::STAFF && !Left->IsTwoHand()))
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_TWO_HAND_STAFF_WIZARD_DAMAGE:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::STAFF && Right->IsTwoHand())
			{
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_TWO_HAND_STAFF_PVP_DAMAGE:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::STAFF && Right->IsTwoHand())
			{
				this->IncreaseData(PLAYER_DATA_380_PVP_DAMAGE, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_SPEAR_ATTACK_DAMAGE:
		case SKILL_TREE_ADD_LANCE_IMPROVED:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::LANCE)
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
			}
		break;

		case SKILL_TREE_SPEAR_DOUBLE_DAMAGE:
		case SKILL_TREE_ADD_LANCE_MASTERY:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::LANCE)
			{
				this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, value);
			}
		break;

		case SKILL_TREE_CROSSBOW_ATTACK_DAMAGE:
		{
												  if ((Right->IsValid() && Right->GetKind2() == ItemKind::CROSSBOW) || (Left->IsValid() && Left->GetKind2() == ItemKind::CROSSBOW))
												  {
													  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
													  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
													  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
													  this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
												  }
		} break;

		case SKILL_TREE_CROSSBOW_PVP_DAMAGE:
		{
											   if ((Right->IsValid() && Right->GetKind2() == ItemKind::CROSSBOW) || (Left->IsValid() && Left->GetKind2() == ItemKind::CROSSBOW))
											   {
												   this->IncreaseData(PLAYER_DATA_380_PVP_DAMAGE, static_cast<int32>(value));
											   }
		} break;

		case SKILL_TREE_STICK_ATTACK_DAMAGE:
		{
											   if ((Right->IsValid() && Right->GetKind2() == ItemKind::STICK) || (Left->IsValid() && Left->GetKind2() == ItemKind::STICK))
											   {
												   this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
												   this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
											   }
		} break;

		case SKILL_TREE_STICK_PVP_DAMAGE:
		{
											if ((Right->IsValid() && Right->GetKind2() == ItemKind::STICK) || (Left->IsValid() && Left->GetKind2() == ItemKind::STICK))
											{
												this->IncreaseData(PLAYER_DATA_380_PVP_DAMAGE, static_cast<int32>(value));
											}
		} break;

		case SKILL_TREE_SCEPTER_ATTACK_DAMAGE:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::SCEPTER)
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_SCEPTER_PVP_DAMAGE:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::SCEPTER)
			{
				this->IncreaseData(PLAYER_DATA_380_PVP_DAMAGE, static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_SCEPTER_DARK_SPIRIT_DAMAGE:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::SCEPTER)
			{
				this->GetDarkSpirit()->IncreaseAttackDamageMin(static_cast<int32>(value));
				this->GetDarkSpirit()->IncreaseAttackDamageMax(static_cast<int32>(value));
			}
			break;

		case SKILL_TREE_SCEPTER_COMMAND:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::SCEPTER)
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, this->GetTotalStat(LEADERSHIP) / value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, this->GetTotalStat(LEADERSHIP) / value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, this->GetTotalStat(LEADERSHIP) / value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, this->GetTotalStat(LEADERSHIP) / value);

				this->GetDarkSpirit()->IncreaseAttackDamageMin(this->GetTotalStat(LEADERSHIP) / value);
				this->GetDarkSpirit()->IncreaseAttackDamageMax(this->GetTotalStat(LEADERSHIP) / value);
			}
			break;

		case SKILL_TREE_BOW_ATTACK_DAMAGE:
		{
											 if ((Right->IsValid() && Right->GetKind2() == ItemKind::BOW) || (Left->IsValid() && Left->GetKind2() == ItemKind::BOW))
											 {
												 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
												 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
												 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
												 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
											 }
		} break;

		case SKILL_TREE_BOW_SPEED:
		{
									 if ((Right->IsValid() && Right->GetKind2() == ItemKind::BOW) || (Left->IsValid() && Left->GetKind2() == ItemKind::BOW))
									 {
										 this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, static_cast<int32>(value));
										 this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, static_cast<int32>(value));
									 }
		} break;

		case SKILL_TREE_BOOK_CURSE_DAMAGE:
		{
											 if ((Right->IsValid() && Right->GetKind2() == ItemKind::BOOK) || (Left->IsValid() && Left->GetKind2() == ItemKind::BOOK))
											 {
												 this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, static_cast<int32>(value));
												 this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, static_cast<int32>(value));
											 }
		} break;

		case SKILL_TREE_BOOK_SPEED:
		{
									  if ((Right->IsValid() && Right->GetKind2() == ItemKind::BOOK) || (Left->IsValid() && Left->GetKind2() == ItemKind::BOOK))
									  {
										  this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, static_cast<int32>(value));
										  this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, static_cast<int32>(value));
									  }
		} break;

		case SKILL_TREE_DARK_SPIRIT_STRENGTHENER_1:
		{
													  this->GetDarkSpirit()->IncreaseAttackDamageMin(static_cast<int32>(value));
													  this->GetDarkSpirit()->IncreaseAttackDamageMax(static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_DARK_SPIRIT_ATTACK_SPEED:
		{
														this->GetDarkSpirit()->IncreaseAttackSpeed(static_cast<int32>(value));
		} break;

		case SKILL_TREE_DARK_SPIRIT_STRENGTHENER_2:
		{
													  this->GetDarkSpirit()->IncreaseCriticalDamageRate(static_cast<int32>(value));
		} break;

		case SKILL_TREE_DARK_SPIRIT_STRENGTHENER_3:
		{
													  this->GetDarkSpirit()->IncreaseExcellentDamageRate(static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_DARK_SPIRIT_DOUBLE_DAMAGE_RATE:
		{
															  this->GetDarkSpirit()->IncreaseDoubleDamageRate(static_cast<int32>(value));
		} break;

		case SKILL_TREE_ADD_DARK_SPIRIT_IGNORE_DEFENSE_RATE:
		{
															   this->GetDarkSpirit()->IncreaseIgnoreDefenseRate(static_cast<int32>(value));
		} break;

		case SKILL_TREE_MAXIMUM_VITALITY_BOOST:
		{
												  this->PowerIncreseAdd(POWER_LIFE, static_cast<int32>(value), false);
		} break;

		case 746:
		{
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, static_cast<int32>(value));
		} break;

		case 752:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::RUNIC_MACE)
			{
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
			}
		break;

		case 753:
			if (Right->IsValid() && Right->GetKind2() == ItemKind::RUNIC_MACE)
			{
				this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, value);
			}
		break;

		case 783:
		{
					if ((Right->IsValid() && Right->GetKind2() == ItemKind::SHORT_SWORD) || (Left->IsValid() && Left->GetKind2() == ItemKind::SHORT_SWORD))
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, static_cast<int32>(value));
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, static_cast<int32>(value));
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, static_cast<int32>(value));
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, static_cast<int32>(value));
					}
		} break;

		case 784:
		{
					if ((Right->IsValid() && Right->GetKind2() == ItemKind::SHORT_SWORD) || (Left->IsValid() && Left->GetKind2() == ItemKind::SHORT_SWORD))
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, value);
						this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, value);
					}
		} break;

		case 831:
		{
					if ((Right->IsValid() && Right->GetKind2() == ItemKind::MAGIC_GUN) || (Left->IsValid() && Left->GetKind2() == ItemKind::MAGIC_GUN))
					{
						this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, static_cast<int32>(value));
						this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, static_cast<int32>(value));
					}
		} break;

		case 832:
		{
					if ((Right->IsValid() && Right->GetKind2() == ItemKind::MAGIC_GUN) || (Left->IsValid() && Left->GetKind2() == ItemKind::MAGIC_GUN))
					{
						this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, value);
					}
		} break;
		}
	}
}