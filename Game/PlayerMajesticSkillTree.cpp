void Player::MajesticSkillTreeSend(bool remove)
{
	if ( !this->IsMajestic() )
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(MAJESTIC_SKILL_TREE_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(MAJESTIC_SKILL_TREE_LIST_BODY, body, buffer, sizeof(MAJESTIC_SKILL_TREE_LIST_HEAD));
	head->count = 0;
	head->points = this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints();

	for ( PlayerMajesticSkillTreeList::const_iterator itr = this->majestic_skill_tree_list.begin(); itr != this->majestic_skill_tree_list.end(); ++itr )
	{
		MajesticSkill const* pSkill = *itr;

		if ( !pSkill )
		{
			continue;
		}

		skill_tree_majestic * pData = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if ( !pData )
		{
			continue;
		}

		body[head->count].section = pData->GetSection();
		body[head->count].id = pData->GetID();
		
		if ( !remove )
		{
			body[head->count].level = pSkill->GetLevel();
			body[head->count].current_level = pData->GetValue(pSkill->GetLevel());
			body[head->count].next_level = pData->GetValue(pSkill->GetLevel());

			if ( body[head->count].level < pData->GetMaxPoints() )
			{
				body[head->count].next_level = pData->GetValue(pSkill->GetLevel() + 1);
			}
		}
		else
		{
			body[head->count].current_level = 0;
			body[head->count].next_level = 0;
			body[head->count].level = 0;
		}

		++head->count;
	}

	head->h.set(HEADCODE_MAJESTIC_SKILL_TREE, SUBCODE_MAJESTIC_SKILL_TREE_LIST, sizeof(MAJESTIC_SKILL_TREE_LIST_HEAD) + (sizeof(MAJESTIC_SKILL_TREE_LIST_BODY) * head->count));
	this->sendPacket(buffer, head->h.get_size());
}

void Player::MajesticSkillTreeStatSend(bool remove)
{
	if (!this->IsMajestic())
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(MAJESTIC_SKILL_TREE_STAT_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(MAJESTIC_SKILL_TREE_STAT_LIST_BODY, body, buffer, sizeof(MAJESTIC_SKILL_TREE_STAT_LIST_HEAD));
	head->count = 0;

	for (PlayerMajesticSkillTreeList::const_iterator itr = this->majestic_skill_tree_stat_list.begin(); itr != this->majestic_skill_tree_stat_list.end(); ++itr)
	{
		MajesticSkill const* pSkill = *itr;

		if (!pSkill)
		{
			continue;
		}

		skill_tree_majestic_stat* pData = sSkillMgr->GetSkillTreeMajesticStat(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if (!pData)
		{
			continue;
		}

		auto & data = body[head->count++];

		data.section = pData->GetSection();
		data.id = pData->GetID();

		if (!remove)
		{
			data.level = pSkill->GetLevel();
			data.current_level = pData->GetValue(pSkill->GetLevel());
			data.next_level = pData->GetValue(pSkill->GetLevel());

			if (data.level < pData->GetMaxPoints(MAX_SKILL_TREE_MAJESTIC_STAT_STEP - 1))
			{
				data.next_level = pData->GetValue(pSkill->GetLevel() + 1);
			}
		}
		else
		{
			data.current_level = 0;
			data.next_level = 0;
			data.level = 0;
		}
	}

	head->h.set(HEADCODE_MAJESTIC_SKILL_TREE, SUBCODE_MAJESTIC_SKILL_TREE_STAT_LIST, sizeof(MAJESTIC_SKILL_TREE_STAT_LIST_HEAD) + (sizeof(MAJESTIC_SKILL_TREE_STAT_LIST_BODY) * head->count));
	this->sendPacket(buffer, head->h.get_size());
}

void Player::MajesticSkillTreeSendPoints()
{
	if ( !this->IsMajestic() )
	{
		return;
	}

	MAJESTIC_SKILL_TREE_DATA pMsg;
	pMsg.points = this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints();

	this->SEND_PCT(pMsg);
}

void Player::MajesticSkillTreeAdd(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->IsMajestic() )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		return;
	}

	this->Ping();

	POINTER_PCT_LOG(MAJESTIC_SKILL_TREE_ADD, lpMsg, Packet, 0);

	MAJESTIC_SKILL_TREE_ADD_RESULT pMsg;
	pMsg.points = this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints();

	skill_tree_majestic* pData = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), lpMsg->section, lpMsg->id);

	if ( !pData )
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if ( pData->GetSkillID() != lpMsg->skill_id )
	{
		this->SEND_PCT(pMsg);
		return;
	}

	skill_template const* skill_info = sSkillMgr->GetSkill(lpMsg->skill_id);

	if ( !skill_info )
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if ( this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints() < pData->GetRequiredPoints() )
	{
		sLog->outError(LOG_SKILL, "%s(LOW POINTS %d / %d): %s", __FUNCTION__, this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints(), pData->GetRequiredPoints(), this->BuildLog().c_str());
		this->SEND_PCT(pMsg);
		return;
	}

	MajesticSkill* pSkillData = this->GetMajesticSkillTreeSkill(0, lpMsg->section, lpMsg->id);

	if ( !pSkillData )
	{
		if( !this->CheckMajesticReplaceSkill(lpMsg->section, pData->GetParentSkill1(), pData->GetParentSkillPoints1()) )
		{
			sLog->outError(LOG_SKILL, "%s(PARENT 1 ERROR: %u / %u / %u): %s", __FUNCTION__, lpMsg->section, pData->GetParentSkill1(), pData->GetParentSkillPoints1(), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if( !this->CheckMajesticRequiredSkill(lpMsg->section, pData->GetParentID2(), pData->GetParentSkillPoints2()) )
		{
			sLog->outError(LOG_SKILL, "%s(PARENT 2 ERROR: %u / %u / %u): %s", __FUNCTION__, lpMsg->section, pData->GetParentID2(), pData->GetParentSkillPoints2(), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if( !this->CheckMajesticRequiredSkill(lpMsg->section, pData->GetParentID3(), pData->GetParentSkillPoints3()) )
		{
			sLog->outError(LOG_SKILL, "%s(PARENT 3 ERROR: %u / %u / %u): %s", __FUNCTION__, lpMsg->section, pData->GetParentID3(), pData->GetParentSkillPoints3(), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		/*if( pData->GetRank() > 1 && !this->CheckMajesticRequireGroup(pData->GetSection(), (pData->GetRank() - 1)) )
		{
			sLog->outError(LOG_SKILL, "%s - 7", __FUNCTION__);
			this->SEND_PCT(pMsg);
			return;
		}*/

		if( !this->ReplaceMajesticSkill(pData->GetParentSkill1(), pData->GetSkillID(), pData->GetRequiredPoints()) )
		{
			sLog->outError(LOG_SKILL, "%s(PARENT REPLACE ERROR: %u / %u / %u): %s", __FUNCTION__, lpMsg->section, pData->GetParentSkill1(), pData->GetParentSkillPoints1(), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if ( pData->GetRequiredPoints() == pData->GetMaxPoints() && skill_info->GetBaseSkill() == lpMsg->skill_id )
		{
			this->MagicAdd(pData->GetSkillID(), pData->GetRequiredPoints());
		}

		this->AddMajesticSkillTreeSkill(0, pData->GetSection(), pData->GetID(), pData->GetRequiredPoints());
	}
	else
	{
		if ( pSkillData->GetLevel() >= pData->GetMaxPoints() )
		{
			sLog->outError(LOG_SKILL, "%s(MAX POINTS: %u / %u): %s", __FUNCTION__, pSkillData->GetLevel(), pData->GetMaxPoints(), this->BuildLog().c_str());
			this->SEND_PCT(pMsg);
			return;
		}

		if( !this->ReplaceMajesticSkill(pData->GetSkillID(), pData->GetSkillID(), pSkillData->GetLevel() + pData->GetRequiredPoints()) )
		{
			this->SEND_PCT(pMsg);
			return;
		}

		pSkillData->SetLevel(pSkillData->GetLevel() + pData->GetRequiredPoints());
	}
	
	this->GetLevelData(LEVEL_DATA_MAJESTIC)->ReducePoints(pData->GetRequiredPoints());

	this->CheckSkill();

	this->WearedItemApplySkill();

	this->CalculateCharacter();

	pSkillData = this->GetMajesticSkillTreeSkill(0, lpMsg->section, lpMsg->id);

	if ( !pSkillData )
	{
		sLog->outError(LOG_SKILL, "%s(ERROR FINDING SKILL: %u / %u): %s", __FUNCTION__, lpMsg->section, lpMsg->id, this->BuildLog().c_str());
		this->SEND_PCT(pMsg);

		this->SendSkill();
		return;
	}

	pMsg.result = 1;
	pMsg.points = this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints();
	pMsg.section = pData->GetSection();
	pMsg.id = pData->GetID();
	pMsg.level = pSkillData->GetLevel();
	pMsg.current_level = pData->GetValue(pSkillData->GetLevel());
	pMsg.next_level = pData->GetValue(pSkillData->GetLevel());

	if ( pMsg.level < pData->GetMaxPoints() )
	{
		pMsg.next_level = pData->GetValue(pSkillData->GetLevel() + 1);
	}

	this->SEND_PCT(pMsg);

	this->SendSkill();
}

void Player::MajesticSkillTreeAddStat(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (!this->IsMajestic())
	{
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		return;
	}

	this->Ping();

	POINTER_PCT_LOG(MAJESTIC_SKILL_TREE_STAT_ADD, lpMsg, Packet, 0);

	MAJESTIC_SKILL_TREE_STAT_ADD_RESULT pMsg;
	pMsg.points = this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints();

	skill_tree_majestic_stat * pData = sSkillMgr->GetSkillTreeMajesticStat(this->GetClass(), lpMsg->section, lpMsg->id);

	if (!pData)
	{
		this->SEND_PCT(pMsg);
		return;
	}

	MajesticSkill* pSkillData = this->GetMajesticSkillTreeSkill(1, lpMsg->section, lpMsg->id);
	uint8 step = 0;
	//uint8 next_step = 0;

	if (pSkillData)
	{
		step = pData->GetCurrentStep(pSkillData->GetLevel());

		if (pSkillData->GetLevel() >= pData->GetMaxPoints(step))
		{
			sLog->outError(LOG_SKILL, "%s - 9", __FUNCTION__);
			pMsg.result = 4;
			this->SEND_PCT(pMsg);
			return;
		}

		//next_step = pData->GetCurrentStep(pSkillData->GetLevel() + 1);
	}

	uint16 added_count = this->GetMajesticSectionAddedPoints(lpMsg->section);

	if (added_count < pData->GetPointCondition(step))
	{
		sLog->outError(LOG_SKILL, "%s - 2", __FUNCTION__);
		pMsg.result = 4;
		this->SEND_PCT(pMsg);
		return;
	}

	/*if (step != next_step)
	{
		if (added_count < pData->GetPointCondition(next_step))
		{
			sLog->outError(LOG_SKILL, "%s - 11", __FUNCTION__);
			this->SEND_PCT(pMsg);
			return;
		}
	}*/

	if (this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints() < pData->GetRequiredPoints(step))
	{
		sLog->outError(LOG_SKILL, "%s - 3", __FUNCTION__);
		this->SEND_PCT(pMsg);
		return;
	}

	if ( !pSkillData )
	{
		this->AddMajesticSkillTreeSkill(1, pData->GetSection(), pData->GetID(), pData->GetRequiredPoints(step));
	}
	else
	{
		pSkillData->SetLevel(pSkillData->GetLevel() + pData->GetRequiredPoints(step));
	}

	this->GetLevelData(LEVEL_DATA_MAJESTIC)->ReducePoints(pData->GetRequiredPoints(step));

	this->CalculateCharacter();

	pSkillData = this->GetMajesticSkillTreeSkill(1, lpMsg->section, lpMsg->id);

	if (!pSkillData)
	{
		sLog->outError(LOG_SKILL, "%s - 10", __FUNCTION__);
		this->SEND_PCT(pMsg);
		return;
	}

	float current_value = pData->GetValue(pSkillData->GetLevel());

	if (pData->GetSkillID() == 1138 || pData->GetSkillID() == 1143)
	{
		this->SetMajesticAddStatPoint(true);
		current_value += this->GetMajesticStatPoints();
		this->IncreaseMajesticStatPoints(pData->GetValue(pSkillData->GetLevel()));
	}

	pMsg.result = 1;
	pMsg.points = this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints();
	pMsg.section = pData->GetSection();
	pMsg.id = pData->GetID();
	pMsg.level = pSkillData->GetLevel();
	pMsg.current_level = current_value;
	pMsg.next_level = current_value;

	if (pMsg.level < pData->GetMaxPoints(MAX_SKILL_TREE_MAJESTIC_STAT_STEP - 1))
	{
		pMsg.next_level = pData->GetValue(pSkillData->GetLevel() + 1);
	}

	this->SEND_PCT(pMsg);
}

void Player::MajesticSkillTreeAddPassive(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (!this->IsMajestic())
	{
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		return;
	}

	this->Ping();

	POINTER_PCT_LOG(MAJESTIC_SKILL_TREE_PASSIVE_ADD, lpMsg, Packet, 0);

	MAJESTIC_SKILL_TREE_PASSIVE_ADD_RESULT pMsg;
	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
	{
		pMsg.stat[i] = this->GetMajesticStat(i);
	}

	if (!this->IsMajesticAddStatPoint())
	{
		sLog->outError(LOG_SKILL, "%s(Not allowed to add stat - %s)", __FUNCTION__, this->BuildLog().c_str());

		this->SEND_PCT(pMsg);
		return;
	}

	this->SetMajesticAddStatPoint(false);

	uint16 add_points = 0;

	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
	{
		add_points += lpMsg->add_points[i];
	}

	if (this->GetMajesticStatPoints() < add_points)
	{
		sLog->outError(LOG_SKILL, "%s(Don't have required points [%u - %u] - %s)", __FUNCTION__, this->GetMajesticStatPoints(), add_points, this->BuildLog().c_str());

		this->SEND_PCT(pMsg);
		return;
	}

	this->ReduceMajesticStatPoints(add_points);

	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
	{
		this->IncreaseMajesticStat(i, lpMsg->add_points[i]);
	}

	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
	{
		pMsg.stat[i] = this->GetMajesticStat(i);
	}

	this->SEND_PCT(pMsg);

	this->CalculateCharacter();
}

MajesticSkill * Player::GetMajesticSkillTreeSkill(uint16 skill)
{
	for ( PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it )
	{
		MajesticSkill * pSkill = *it;

		if ( !pSkill )
		{
			continue;
		}

		skill_tree_majestic const* skill_info = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if ( !skill_info )
		{
			continue;
		}

		if ( skill_info->GetSkillID() == skill )
		{
			return pSkill;
		}
	}

	return nullptr;
}
	
MajesticSkill const* Player::GetMajesticSkillTreeSkill(uint16 skill) const
{
	for ( PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it )
	{
		MajesticSkill const* pSkill = *it;

		if ( !pSkill )
		{
			continue;
		}

		skill_tree_majestic const* skill_info = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if ( !skill_info )
		{
			continue;
		}

		if ( skill_info->GetSkillID() == skill )
		{
			return pSkill;
		}
	}

	return nullptr;
}

MajesticSkill * Player::GetMajesticSkillTreeSkill(uint8 type, uint8 section, uint16 id)
{
	switch (type)
	{
	case 0:
		{
			for (PlayerMajesticSkillTreeList::const_iterator itr = this->majestic_skill_tree_list.begin(); itr != this->majestic_skill_tree_list.end(); ++itr)
			{
				MajesticSkill * pSkill = *itr;

				if (!pSkill)
				{
					continue;
				}

				if (pSkill->GetSection() == section && pSkill->GetID() == id)
				{
					return pSkill;
				}
			}
		} break;

	case 1:
		{
			for (PlayerMajesticSkillTreeList::const_iterator itr = this->majestic_skill_tree_stat_list.begin(); itr != this->majestic_skill_tree_stat_list.end(); ++itr)
			{
				MajesticSkill * pSkill = *itr;

				if (!pSkill)
				{
					continue;
				}

				if (pSkill->GetSection() == section && pSkill->GetID() == id)
				{
					return pSkill;
				}
			}
		} break;
	}

	return nullptr;
}
	
MajesticSkill const* Player::GetMajesticSkillTreeSkill(uint8 type, uint8 section, uint16 id) const
{
	switch (type)
	{
	case 0:
		{
			for (PlayerMajesticSkillTreeList::const_iterator itr = this->majestic_skill_tree_list.begin(); itr != this->majestic_skill_tree_list.end(); ++itr)
			{
				MajesticSkill const* pSkill = *itr;

				if (!pSkill)
				{
					continue;
				}

				if (pSkill->GetSection() == section && pSkill->GetID() == id)
				{
					return pSkill;
				}
			}
		} break;

	case 1:
		{
			for (PlayerMajesticSkillTreeList::const_iterator itr = this->majestic_skill_tree_stat_list.begin(); itr != this->majestic_skill_tree_stat_list.end(); ++itr)
			{
				MajesticSkill const* pSkill = *itr;

				if (!pSkill)
				{
					continue;
				}

				if (pSkill->GetSection() == section && pSkill->GetID() == id)
				{
					return pSkill;
				}
			}
		} break;
	}

	return nullptr;
}

bool Player::CheckMajesticReplaceSkill(uint8 section, uint16 skill, uint8 level) const
{
	if ( skill == uint16(-1) )
	{
		return true;
	}

	Skill const* pSkill = this->MagicGet(skill);

	if ( !pSkill )
	{
		return false;
	}

	return level == uint8(-1) || pSkill->GetLevel() >= level;
}
	
bool Player::CheckMajesticRequiredSkill(uint8 section, uint16 id, uint8 level) const
{
	if ( id == uint16(-1) )
	{
		return true;
	}

	MajesticSkill const* pMasterSkill = this->GetMajesticSkillTreeSkill(0, section, id);

	if ( !pMasterSkill )
	{
		return false;
	}

	return level == uint8(-1) || pMasterSkill->GetLevel() >= level;
}

bool Player::AddMajesticSkillTreeSkill(uint8 type, uint8 section, uint16 id, uint8 level)
{
	if (this->GetMajesticSkillTreeSkill(type, section, id))
	{
		return false;
	}

	switch (type)
	{
	case 0:
		{
			skill_tree_majestic const* skill_info = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), section, id);

			if (!skill_info)
			{
				return false;
			}

			this->majestic_skill_tree_list.push_back(new MajesticSkill(section, id, level));

			return true;
		} break;

	case 1:
		{
			skill_tree_majestic_stat const* skill_info = sSkillMgr->GetSkillTreeMajesticStat(this->GetClass(), section, id);

			if (!skill_info)
			{
				return false;
			}

			this->majestic_skill_tree_stat_list.push_back(new MajesticSkill(section, id, level));

			return true;
		} break;
	}
	
	return false;
}

bool Player::ReplaceMajesticSkill(uint16 replace, uint16 skill, uint8 level)
{
	skill_template const* skill_info = sSkillMgr->GetSkill(replace);

	if ( skill_info )
	{
		if ( skill_info->GetRequireSkill() )
		{
			this->MagicAdd(skill, level);
		}
		else
		{
			Skill* pSkill = this->MagicGet(replace);

			if ( !pSkill )
			{
				return replace == skill;
			}
			else
			{
				pSkill->SetSkill(skill);
				pSkill->SetLevel(level);
			}
		}
	}

	return true;
}

bool Player::CheckMajesticRequireGroup(uint8 section, uint8 rank) const
{
	for ( PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it )
	{
		MajesticSkill const* pSkill = *it;

		if ( !pSkill )
		{
			continue;
		}

		if ( pSkill->GetSection() != section )
		{
			continue;
		}

		skill_tree_majestic const* skill_info = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if ( !skill_info )
		{
			continue;
		}

		if( skill_info->GetSection() == section && skill_info->GetRank() == rank && pSkill->GetLevel() >= 10 )
		{
			return true;
		}
	}

	return false;
}

void Player::ApplyMajesticSkillTree(bool stat)
{
	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it)
	{
		MajesticSkill const* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		if (pSkill->GetLevel() <= 0)
		{
			continue;
		}

		skill_tree_majestic * pData = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if (!pData)
		{
			continue;
		}

		switch (pData->GetSkillID())
		{
		case 1000: ///- Stats
			{
				if (stat)
				{
					this->ApplyMajesticSkillTree(pData->GetSection(), pData->GetSkillID(), pData->GetValue(pSkill->GetLevel()));
				}
			} break;

		default:
			{
				if (!stat)
				{
					this->ApplyMajesticSkillTree(pData->GetSection(), pData->GetSkillID(), pData->GetValue(pSkill->GetLevel()));
				}
			} break;
		}
	}
}

void Player::ApplyMajesticStats(bool stat)
{
	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_stat_list.begin(); it != this->majestic_skill_tree_stat_list.end(); ++it)
	{
		MajesticSkill const* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		if (pSkill->GetLevel() <= 0)
		{
			continue;
		}

		skill_tree_majestic_stat * pData = sSkillMgr->GetSkillTreeMajesticStat(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if (!pData)
		{
			continue;
		}

		switch (pData->GetSkillID())
		{
		case 1000: ///- Stats
			{
				if (stat)
				{
					this->ApplyMajesticStats(pData->GetSkillID(), pData->GetValue(pSkill->GetLevel()));
				}
			} break;

		default:
			{
				if (!stat)
				{
					this->ApplyMajesticStats(pData->GetSkillID(), pData->GetValue(pSkill->GetLevel()));
				}
			} break;
		}
	}

	if (stat)
	{
		for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
		{
			this->IncreaseAddStat(i, this->GetMajesticStat(i));
		}
	}
}

void Player::ApplyMajesticStats(uint16 skill, float value)
{
	Item const* Wings = this->GetInventory()->GetItem(WINGS);

	switch (skill)
	{
	case 1137:
	{
				 this->PowerIncreseAdd(POWER_LIFE, value, false);
	} break;

	case 1139:
	{
				 this->IncreaseIntData(UNIT_INT_DEFENSE_BASE, value);
	} break;

	case 1140:
	{
				 if (Wings && Wings->IsValid() && Wings->IsWingLvl4())
				 {
					 this->IncreaseIntData(UNIT_INT_DEFENSE, value);
				 }
	} break;

	case 1141:
	{
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
	} break;

	case 1142:
	{
				 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
				 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
	} break;

	case 1144:
	{
				 if (sGameServer->IsMajesticSkillDamageNormal())
				 {
					 this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
				 }
				 else
				 {
					 this->IncreaseData(PLAYER_DATA_MAJESTIC_SKILL_DAMAGE, value);
				 }
	} break;

	case 1145:
	{
				 if (Wings && Wings->IsValid() && Wings->IsWingLvl4())
				 {
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
				 }
	} break;

	case 1146:
	{
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
				 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
				 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
				 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
	} break;

	case 1157:
	{
				 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
				 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
				 this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
				 this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
	} break;

	case 1193:
	{
				 if (Wings && Wings->IsValid() && Wings->IsWingLvl4())
				 {
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
				 }
	} break;

	case 1194:
	{
				 if (Wings && Wings->IsValid() && Wings->IsWingLvl4())
				 {
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
					 this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
				 }
	} break;

	case 1195:
	{
				 if (Wings && Wings->IsValid() && Wings->IsWingLvl4())
				 {
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
					 this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
					 this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
					 this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
				 }
	} break;
	}
}

void Player::ApplyMajesticSkillTree(uint8 section, uint16 skill, float value)
{
	switch (skill)
	{
	case 1024:
	case 1046:
	case 1062:
	{
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_RATE, value);
	}
	break;

	case 1025:
	case 1047:
	case 1063:
	{
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_RATE, value);
	}
	break;

	case 1026:
	case 1048:
	case 1064:
	{
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_RATE, value);
	}
	break;

	case 1032:
	case 1054:
	{
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_DURATION, value);
	}
	break;

	case 1033:
	case 1055:
	{
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_DURATION, value);
	}
	break;

	case 1034:
	case 1056:
	{
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_DURATION, value);
	}
	break;

	case 1040:
	{
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_DEBUFF_UPGRADE_RATE, value);
	}
	break;

	case 1041:
	{
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_DEBUFF_UPGRADE_RATE, value);
	}
	break;

	case 1042:
	{
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_DEBUFF_UPGRADE_RATE, value);
	}
	break;

	case 1028:
	case 1050:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_POISONING_EFFECT, value);
	} break;

	case 1029:
	case 1051:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_CHILLING_EFFECT, value);
	} break;

	case 1030:
	case 1052:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_BLEEDING_EFFECT, value);
	} break;

	case 1036:
	case 1058:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_DEADLY_POISONING_EFFECT, value);
	} break;

	case 1037:
	case 1059:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_FREEZING_EFFECT, value);
	} break;

	case 1038:
	case 1060:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_HEMORRHAGE_EFFECT, value);
	} break;

	case 1166:
	{
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_1_TARGETS, value);
	} break;

	case 1167:
	{
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_1_TARGETS, value);
	} break;

	case 1168:
	{
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_1_TARGETS, value);
	} break;

	case 1169:
	{
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_2_TARGETS, value);
	} break;

	case 1170:
	{
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_2_TARGETS, value);
	} break;

	case 1171:
	{
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_2_TARGETS, value);
	} break;

	case 1190:
	{
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_1_TARGETS, value);
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_2_TARGETS, value);
	} break;

	case 1191:
	{
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_1_TARGETS, value);
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_2_TARGETS, value);
	} break;

	case 1192:
	{
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_1_TARGETS, value);
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_2_TARGETS, value);
	} break;


	case 1172:
	{
				 this->AddMajesticData(section, MAJESTIC_POISONING_CIRCLE_RANGE, value);
	} break;

	case 1174:
	{
				 this->AddMajesticData(section, MAJESTIC_BLEEDING_CIRCLE_RANGE, value);
	} break;

	case 1176:
	{
				 this->AddMajesticData(section, MAJESTIC_CHILLING_CIRCLE_RANGE, value);
	} break;

	case 1178:
	case 1184:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_POISONING_SUCCESS_RATE, value);
	} break;

	case 1179:
	case 1185:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_DEADLY_POISONING_SUCCESS_RATE, value);
	} break;

	case 1180:
	case 1186:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_BLEEDING_SUCCESS_RATE, value);
	} break;

	case 1181:
	case 1187:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_HEMORRHAGE_SUCCESS_RATE, value);
	} break;

	case 1182:
	case 1188:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_CHILLING_SUCCESS_RATE, value);
	} break;

	case 1183:
	case 1189:
	{
				 this->AddMajesticData(section, MAJESTIC_DEBUFF_FREEZING_SUCCESS_RATE, value);
	} break;


	case 1001: /* Increase Damage */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_DAMAGE, value);
	} break;

	case 1002: /* Linked Skill Damage +1*/
	case 1003: /* Linked Skill Damage +2*/
	case 1004: /* Linked Skill Damage +3*/
	case 1005: /* Linked Skill Damage +4*/
	case 1006: /* Linked Skill Damage +5*/
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT, value);
	} break;

	case 1007: /* Chance of Additional Damage */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT_RATE, value);
	} break;

	case 1008: /* Increase Attack Speed */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_ATTACK_SPEED, value);
	} break;

	case 1009: /* Increase attack range by X tiles */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_ATTACK_RANGE, value);
	} break;

	case 1010: /* Add Splash Damage */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_SPLASH_DAMAGE, value);
	} break;

	case 1011: /* Increase usage distance by X tiles */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_DISTANCE, value);
	} break;

	case 1012: /* Increase Linked Skill Damage by 1*/
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET, value);
	} break;

	case 1019: /* Cooldown Removed */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_REMOVE_COOLDOWN, value);
	} break;

	case 1020: /* Increase Weapon Power Sword */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_DAMAGE, value);
	} break;

	case 1021: /* Increase Weapon Power Staff */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_MAGIC_DAMAGE, value);
	} break;

	case 1156: /* Increase Weapon Power Staff */
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_CURSE_DAMAGE, value);
	} break;

	case 1204:
	{
				 this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_SP_BASE_DEFENSE, value);
	} break;

	case 1205:
	{
				   this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_SP_MAX_HP, value);
	} break;

	case 1206:
	{
				 this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_SP_MEMBER_DAMAGE, value);
	} break;

	case 1207:
	{
				 this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_SP_HP_TRANSFER, value);
	} break;

	case 1208:
	{
				 this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_SP_DAMAGE_TRANSFER, value);
	} break;

	case 1209:
	{
				 this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_SP_MEMBER_DEFENSE, value);
	} break;

	case 1210:
	{
				 this->AddMajesticData(section, MAJESTIC_LINKED_SKILL_SP_SHIELD_DEFENSE, value);
	} break;
	}
}

void Player::SpawnMajesticDebuffCircle(uint16 skill)
{
	skill_tree_majestic const* skill_info = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), skill);

	if (!skill_info)
	{
		return;
	}

	if (this->GetMajesticDebuff())
	{
		return;
	}

	if ((MyGetTickCount() - this->GetMajesticDebuffTick()) < 20000)
	{
		return;
	}

	float poisoning_spawn = this->GetMajesticDataValue(skill_info->GetSection(), MAJESTIC_POISONING_CIRCLE_RATE);
	if (poisoning_spawn > 0 && roll_chance_f(poisoning_spawn))
	{
		this->RunSpawnMajesticDebuffCircle(skill, 0, this->GetMajesticDataValue(skill_info->GetSection(), MAJESTIC_POISONING_CIRCLE_DURATION));
		return;
	}

	float freezing_spawn = this->GetMajesticDataValue(skill_info->GetSection(), MAJESTIC_CHILLING_CIRCLE_RATE);
	if (freezing_spawn > 0 && roll_chance_f(freezing_spawn))
	{
		this->RunSpawnMajesticDebuffCircle(skill, 1, this->GetMajesticDataValue(skill_info->GetSection(), MAJESTIC_CHILLING_CIRCLE_DURATION));
		return;
	}

	float bleeding_spawn = this->GetMajesticDataValue(skill_info->GetSection(), MAJESTIC_BLEEDING_CIRCLE_RATE);
	if (bleeding_spawn > 0 && roll_chance_f(bleeding_spawn))
	{
		this->RunSpawnMajesticDebuffCircle(skill, 2, this->GetMajesticDataValue(skill_info->GetSection(), MAJESTIC_BLEEDING_CIRCLE_DURATION));
		return;
	}
}

void Player::RunSpawnMajesticDebuffCircle(uint16 skill, uint8 type, int32 add_duration)
{
	Monster* pMonster = sObjectMgr->MonsterTryAdd(851 + type, this->GetWorldId());

	if (!pMonster)
	{
		return;
	}

	int32 duration = 10 + add_duration;

	pMonster->SetWorldId(this->GetWorldId());
	pMonster->SetBasicLocation(this->GetX(), this->GetY(), this->GetX(), this->GetY());
	pMonster->SetInstance(this->GetInstance());
	pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	pMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
	pMonster->SetDespawnTime(duration * IN_MILLISECONDS);
	pMonster->SetDespawnTick(GetTickCount());
	pMonster->SetMoveDistance(0);
	pMonster->SetDirection(Random(8));

	pMonster->AddAdditionalDataPtr(0, this);
	pMonster->AddAdditionalDataInt(0, type);
	pMonster->AddAdditionalDataInt(1, skill);

	pMonster->AddToWorld();

	this->SetMajesticDebuff(pMonster);
	this->SetMajesticDebuffTick(MyGetTickCount());
}

void Player::ApplyMajesticLinkedSplashDamage(Unit* pTarget, uint16 skill, int32 damage)
{
	if ( !pTarget )
	{
		return;
	}

	float splash = this->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_SPLASH_DAMAGE);

	if ( splash <= 0.0f )
	{
		return;
	}

	int32 splash_damage = damage * sGameServer->GetSkillSplashDamagePercent() / 100;

	if ( splash_damage <= 0 )
	{
		return;
	}

	Object* mTargetSec = nullptr;
	
	VIEWPORT_LOOP_OBJECT(this, mTargetSec)

		if ( !this->AttackAllowed(mTargetSec->ToUnit()) )
		{
			continue;
		}

		if ( mTargetSec == pTarget )
		{
			continue;
		}

		if ( !IN_RANGE(pTarget, mTargetSec, 1) )
		{
			continue;
		}

		this->AttackProc(mTargetSec->ToUnit(), nullptr, false, splash_damage);

	VIEWPORT_CLOSE
}

void Player::AddMajesticData(uint8 section, uint8 id, float value)
{
	MajesticDataSectionMap & data_map = this->majestic_data_map[section];

	MajesticDataSectionMap::iterator it = data_map.find(id);

	if ( it == data_map.end() )
	{
		data_map[id] = value;
	}
	else
	{
		it->second += value;
	}
}

float Player::GetMajesticDataValue(uint8 section, uint8 id) const
{
	MajesticDataMap::const_iterator itr = this->majestic_data_map.find(section);

	if ( itr == this->majestic_data_map.end() )
	{
		return 0.0f;
	}

	MajesticDataSectionMap::const_iterator it = itr->second.find(id);

	if ( it == itr->second.end() )
	{
		return 0.0f;
	}

	return it->second;
}

float Player::GetMajesticDataValueBySkill(uint16 skill, uint8 id) const
{
	skill_tree_majestic const* skill_info = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), skill);

	if ( !skill_info )
	{
		return 0.0f;
	}

	MajesticDataMap::const_iterator itr = this->majestic_data_map.find(skill_info->GetSection());

	if ( itr == this->majestic_data_map.end() )
	{
		return 0.0f;
	}

	MajesticDataSectionMap::const_iterator it = itr->second.find(id);

	if ( it == itr->second.end() )
	{
		return 0.0f;
	}

	return it->second;
}

void Player::ApplyMajesticDamage(Monster* pMonster, int32 & damage, bool target)
{
	/*if ( !this->IsMajestic() )
	{
		return;
	}*/

	/*if ( !pMonster )
	{
		return;
	}

	if ( damage <= 0 )
	{
		return;
	}

	int32 player_level = this->GetTotalLevel();
	int32 monster_level = pMonster->GetTotalLevel() + 600;

	if (monster_level < (sGameServer->GetMaxLevel(LEVEL_DATA_NORMAL) + sGameServer->GetMaxLevel(LEVEL_DATA_MASTER)))
	{
		return;
	}

	int32 diff = monster_level - player_level;

	if (diff <= 0)
	{
		return;
	}

	if (diff > 70)
	{
		diff = 70;
	}

	if (!target)
	{
		damage -= (damage * diff / 100);
	}
	else
	{
		damage += (damage * diff / 100);
	}

	if (damage < 0)
	{
		damage = 0;
	}*/

	if (!pMonster)
	{
		return;
	}

	int32 character_level = this->GetTotalLevel();
	int32 monster_level = pMonster->GetLevel() + 600;

	if ( monster_level < (sGameServer->GetMaxLevel(LEVEL_DATA_NORMAL) + sGameServer->GetMaxLevel(LEVEL_DATA_MASTER)) )
	{
		return;
	}

	int32 difference = abs(character_level - monster_level);

	int32 add_damage = difference * 10;
	int32 reduce_damage = difference * 10;

	if ( add_damage > 70 )
	{
		add_damage = 70;
	}

	if ( reduce_damage > 70 )
	{
		reduce_damage = 70;
	}

	if ( character_level > monster_level )
	{
		if ( !target )
		{
			damage += damage * add_damage / 100;
		}
		else
		{
			damage -= damage * reduce_damage / 100;
		}
	}

	if ( character_level < monster_level )
	{
		if ( !target )
		{
			damage -= damage * add_damage / 100;
		}
		else
		{
			damage += damage * reduce_damage / 100;
		}
	}

	if ( damage < 0 )
	{
		damage = 0;
	}
}

bool Player::GetMajesticSkillLevel(uint16 skill, uint8 & level) const
{
	for ( PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it )
	{
		MajesticSkill const* pSkill = *it;

		if ( !pSkill )
		{
			continue;
		}

		if ( pSkill->GetLevel() <= 0)
		{
			continue;
		}

		skill_tree_majestic const* pData = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetSkillID() == skill )
		{
			level = pSkill->GetLevel();
			return true;
		}
	}

	return false;
}

void Player::ResetMajesticTree()
{
	std::set<uint16> base_skill;

	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it)
	{
		MajesticSkill const* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		if (pSkill->GetLevel() <= 0)
		{
			continue;
		}

		skill_tree_majestic const* pData = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if (!pData)
		{
			continue;
		}

		int32 result = this->MagicRemove(pData->GetSkillID());

		if (result != -1)
		{
			this->SendSkill(result, pData->GetSkillID(), pSkill->GetLevel(), false);
		}

		if (pData->GetParentID1() == uint16(-1))
		{
			continue;
		}

		/*skill_template const* pSkillInfo = sSkillMgr->GetSkill(pData->GetSkillID());

		if (pSkillInfo)
		{
			base_skill.insert(pSkillInfo->GetBaseSkill());

			
		}*/

		if (sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pData->GetParentID1()) == nullptr)
		{
			this->MagicAdd(pData->GetParentSkill1(), 0);
		}
	}

	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel());

	this->MajesticSkillTreeSend(true);
	this->MajesticSkillTreeStatSend(true);

	LIST_CLEAR(PlayerMajesticSkillTreeList::iterator, this->majestic_skill_tree_list);
	LIST_CLEAR(PlayerMajesticSkillTreeList::iterator, this->majestic_skill_tree_stat_list);
	this->SetMajesticAddStatPoint(false);
	this->SetMajesticStatPoints(0);
	this->ResetMajesticStat(0);

	this->WearedItemApplySkill();

	this->CheckSkill();

	this->SendSkill();
	this->CalculateCharacter();
	this->MasterSendStatus();
	this->MajesticSkillTreeSendPoints();
}

uint16 Player::GetMajesticSectionAddedPoints(uint8 section) const
{
	uint16 count = 0;

	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it)
	{
		MajesticSkill const* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		skill_tree_majestic const* pData = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if (!pData)
		{
			continue;
		}

		switch (section)
		{
		case 0:
			{
				//if (pData->GetUIGroup() == 0 && pData->GetID() < 10)
				if (pData->GetUIGroup() == 0 && (pData->GetRank() == 1 || pData->GetRank() == 2))
				{
					count += pSkill->GetLevel();
				}
			} break;

		case 1:
			{
				//if (pData->GetUIGroup() == 0 && pData->GetID() >= 10)
				if (pData->GetUIGroup() == 0 && (pData->GetRank() == 3 || pData->GetRank() == 4))
				{
					count += pSkill->GetLevel();
				}
			} break;
		}
	}

	return count;
}

void Player::CalculateMajesticStatPoints()
{
	uint16 points = 0;
	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
	{
		points += this->GetMajesticStat(i);
	}


}

bool Player::MajesticTreeReset()
{
	CharacterBaseData const* pCharacterInfo = sCharacterBase->GetCharacterBase(this->GetClass());

	if (!pCharacterInfo)
	{
		return false;
	}

	if (this->majestic_skill_tree_list.empty() && this->majestic_skill_tree_stat_list.empty())
	{
		return false;
	}

	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel() * pCharacterInfo->GetMajesticLevelUpPoints());

	for (PlayerMajesticSkillTreeList::const_iterator itr = this->majestic_skill_tree_list.begin(); itr != this->majestic_skill_tree_list.end(); ++itr)
	{
		MajesticSkill const* pSkill = *itr;

		if (!pSkill)
		{
			continue;
		}

		skill_tree_majestic const* pData = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if (!pData)
		{
			continue;
		}

		int32 result = this->MagicRemove(pData->GetSkillID());

		if (result != -1)
		{
			this->SendSkill(result, pData->GetSkillID(), pSkill->GetLevel(), false);
		}

		if (pData->GetParentSkill1() != uint16(-1))
		{
			uint8 level = 0;
			this->GetMasterSkillLevel(pData->GetParentSkill1(), level);

			result = this->MagicAdd(pData->GetParentSkill1(), level);

			if (result != -1)
			{
				this->SendSkill(result, pData->GetParentSkill1(), level);
			}
		}
	}

	this->MajesticSkillTreeSend(true);
	this->MajesticSkillTreeStatSend(true);

	LIST_CLEAR(PlayerMajesticSkillTreeList::iterator, this->majestic_skill_tree_list);
	LIST_CLEAR(PlayerMajesticSkillTreeList::iterator, this->majestic_skill_tree_stat_list);
	this->SetMajesticAddStatPoint(false);
	this->SetMajesticStatPoints(0);
	this->ResetMajesticStat(0);

	this->WearedItemApplySkill();
	this->SendSkill();
	this->CalculateCharacter();
	this->MajesticSkillTreeSendPoints();

	return true;
}