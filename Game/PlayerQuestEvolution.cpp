/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerQuestEvolution.cpp"
*
*/

bool Player::SatisfyQuestEvolutionClass(QuestEvolutionData const* pData) const
{
	if ( !pData )
	{
		return false;
	}

	if ( pData->GetRequiredClass(this->GetClass()) == 0 )
		return false;

	if ( pData->GetRequiredClass(this->GetClass()) == 2 && !this->GetChangeUP(0) )
		return false;

	if ( pData->GetRequiredClass(this->GetClass()) == 3 && !this->GetChangeUP(1) )
		return false;

	if ( pData->GetRequiredClass(this->GetClass()) == 4 && !this->GetChangeUP(2) )
		return false;

	return true;
}

uint8 Player::QuestEvolutionUpdateState(uint8 quest_id)
{
	QuestEvolution const* pQuest = sQuestMgr->GetQuest(quest_id);

	if ( !pQuest )
	{
		return uint8(-1);
	}

	QuestEvolutionStatus & QuestStatus = this->quest_evolution[pQuest->GetID()];

	if ( QuestStatus.GetState() == QUEST_EVOLUTION_STATE_NONE || QuestStatus.GetState() == QUEST_EVOLUTION_STATE_CLEAR )
	{
		uint8 condition = this->QuestEvolutionConditionAccept(pQuest);

		if ( condition != 0 )
			return condition;

		this->QuestEvolutionTake(pQuest);
		QuestStatus.SetState(QUEST_EVOLUTION_STATE_ACCEPTED);
		QuestStatus.ResetMonsterCount(0);

		sLog->outInfo(LOG_QUEST, "%s -- Accepted Quest [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
	}
	else if ( QuestStatus.GetState() == QUEST_EVOLUTION_STATE_ACCEPTED )
	{
		uint8 condition = this->QuestEvolutionConditionClear(pQuest);

		if ( condition != 0 )
			return condition;

		this->QuestEvolutionGiveReward(pQuest);
		QuestStatus.SetState(QUEST_EVOLUTION_STATE_COMPLETE);
	}
	else
	{
		return uint8(-1);
	}
	
	return 0;
}

void Player::QuestEvolutionSetState(uint8 * Packet)
{
	if ( !this->IsPlaying() )
	{
		return;
	}

	POINTER_PCT_LOG(QUEST_EVOLUTION_SET_STATE, lpMsg, Packet, 0);

	QUEST_EVOLUTION_SET_STATE_RESULT pMsg(lpMsg->quest_id);
	pMsg.result = this->QuestEvolutionUpdateState(lpMsg->quest_id);
	pMsg.state = this->QuestEvolutionGetStateByteGroup(lpMsg->quest_id / 4);

	this->sendPacket(MAKE_PCT(pMsg));
}

uint8 Player::QuestEvolutionGetState(uint8 quest_id)
{
	QuestEvolution const* pQuest = sQuestMgr->GetQuest(quest_id);

	if ( pQuest )
	{
		QuestEvolutionStatus const* pQuestStatus = this->QuestEvolutionFind(quest_id);

		if ( pQuestStatus )
		{
			return pQuestStatus->GetState();
		}
	}

	return QUEST_EVOLUTION_STATE_NO_EXIST;
}

uint8 Player::QuestEvolutionGetStateByte(uint8 quest_id)
{
	QuestEvolution const* pQuest = sQuestMgr->GetQuest(quest_id);

	if ( pQuest )
	{
		QuestEvolutionStatus const* pQuestStatus = this->QuestEvolutionFind(quest_id);

		if ( pQuestStatus )
		{
			return pQuestStatus->GetStateByte();
		}
	}

	return QUEST_EVOLUTION_STATE_NO_EXIST;
}

uint8 Player::QuestEvolutionGetStateByteGroup(uint8 group)
{
	uint8 state = QUEST_EVOLUTION_STATE_NONE;

	for ( QuestEvolutionStatusMap::const_iterator it = this->quest_evolution.begin(); it != this->quest_evolution.end(); ++it )
	{
		if ( (it->first / 4) != group )
		{
			continue;
		}

		state |= it->second.GetStateByte();
	}

	return state;
}

void Player::QuestEvolutionSetState(uint8 quest_id, uint8 state)
{
	QuestEvolutionStatus * pQuestStatus = this->QuestEvolutionFind(quest_id);

	if ( pQuestStatus )
	{
		pQuestStatus->SetState(state);

		if ( state == QUEST_EVOLUTION_STATE_CLEAR )
		{
			pQuestStatus->ResetMonsterCount(0);
		}
	}
}

uint8 Player::QuestEvolutionConditionAccept(QuestEvolution const* pQuest)
{
	if ( !pQuest )
	{
		return -1;
	}

	bool founded = false;

	for ( QuestEvolutionDataList::const_iterator itr = pQuest->data.begin(); itr != pQuest->data.end(); ++itr )
	{
		QuestEvolutionData const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( !this->SatisfyQuestEvolutionClass(pData) )
		{
			continue;
		}

		founded = true;

		for ( QuestEvolutionConditionList::const_iterator it = pQuest->condition.begin(); it != pQuest->condition.end(); ++it )
		{
			QuestEvolutionCondition const* pCondition = *it;

			if ( !pCondition )
			{
				continue;
			}
			
			if ( pCondition->GetConditionID() != -1 && pCondition->GetConditionID() != pData->GetConditionIndex() )
			{
				continue;
			}

			if ( pCondition->GetRequiredQuestID() != uint8(-1) )
			{
				if ( this->QuestEvolutionGetState(pCondition->GetRequiredQuestID()) != QUEST_EVOLUTION_STATE_COMPLETE )
				{
					sLog->outInfo(LOG_QUEST, "%s -- Don't have quest %u required quest %u -- %s", __FUNCTION__, pQuest->GetID(), pCondition->GetRequiredQuestID(), this->BuildLog().c_str());
					return pCondition->GetContextStart();
				}
			}

			if ( pCondition->GetMinLevel() > this->GetTotalLevel() )
			{
				sLog->outInfo(LOG_QUEST, "%s -- Don't have quest %u required min level %u -- %s", __FUNCTION__, pQuest->GetID(), pCondition->GetMinLevel(), this->BuildLog().c_str());
				return pCondition->GetContextStart();
			}

			if ( pCondition->GetMaxLevel() != 0 && pCondition->GetMaxLevel() < this->GetTotalLevel() )
			{
				sLog->outInfo(LOG_QUEST, "%s -- Don't have quest %u required max level %u -- %s", __FUNCTION__, pQuest->GetID(), pCondition->GetMaxLevel(), this->BuildLog().c_str());
				return pCondition->GetContextStart();
			}

			if ( !this->MoneyHave(pCondition->GetRequiredZen()) )
			{
				sLog->outInfo(LOG_QUEST, "%s -- Don't have quest %u required money %u -- %s", __FUNCTION__, pQuest->GetID(), pCondition->GetRequiredZen(), this->BuildLog().c_str());
				return pCondition->GetContextStart();
			}
		}
	}
	
	return (founded ? 0: -1);
}

uint8 Player::QuestEvolutionConditionClear(QuestEvolution const* pQuest)
{
	QuestEvolutionStatus * pQuestData = this->QuestEvolutionFind(pQuest->GetID());
	bool founded = false;

	for ( QuestEvolutionDataList::const_iterator itr = pQuest->data.begin(); itr != pQuest->data.end(); ++itr )
	{
		QuestEvolutionData const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( !this->SatisfyQuestEvolutionClass(pData) )
		{
			continue;
		}

		founded = true;

		if ( pData->GetObjectiveType() == QUEST_EVOLUTION_OBJECTIVE_ITEM )
		{
			int32 item_count = this->GetInventory()->GetItemCount(pData->GetItem(), pData->GetItemLevel());

			if ( item_count < pData->GetCount() )
			{
				return 1;
			}
		}
		else if ( pData->GetObjectiveType() == QUEST_EVOLUTION_OBJECTIVE_MONSTER )
		{
			if ( pQuestData && pQuestData->GetMonsterCount(pData->GetMonsterSlot()) < pData->GetCount() )
			{
				return 1;
			}
		}
	}

	return (founded ? 0 : -1);
}

QuestEvolutionStatus * Player::QuestEvolutionFind(uint8 quest_id)
{
	QuestEvolutionStatusMap::iterator it = this->quest_evolution.find(quest_id);

	if ( it != this->quest_evolution.end() )
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}

void Player::QuestEvolutionNpcTalk(Monster* pNpc)
{
	uint8 quest_state = QUEST_EVOLUTION_STATE_NO_EXIST;
	uint8 quest_index = uint8(-1);

	for ( QuestEvolutionMap::const_iterator itr = sQuestMgr->m_quest_evolution.begin(); itr != sQuestMgr->m_quest_evolution.end(); ++itr )
	{
		QuestEvolution const* pQuest = itr->second;

		if ( !pQuest )
		{
			continue;
		}

		if ( pQuest->GetType() != QUEST_EVOLUTION_START_NPC_CLICK || pQuest->GetNPC() != pNpc->GetClass() )
		{
			continue;
		}

		if ( quest_state == QUEST_EVOLUTION_STATE_NO_EXIST || quest_state == QUEST_EVOLUTION_STATE_COMPLETE )
		{
			for ( QuestEvolutionDataList::const_iterator it = pQuest->data.begin(); it != pQuest->data.end(); ++it )
			{
				if ( !this->SatisfyQuestEvolutionClass(*it) )
				{
					continue;
				}

				quest_index = pQuest->GetID();
				quest_state = this->QuestEvolutionGetState(pQuest->GetID());
			}
		}
		/*else if ( quest_state == QUEST_EVOLUTION_STATE_COMPLETE )
		{
			for ( QuestEvolutionDataList::const_iterator it = pQuest->data.begin(); it != pQuest->data.end(); ++it )
			{
				if ( !this->SatisfyQuestEvolutionClass(*it) )
					continue;

				quest_index = pQuest->GetID();
				quest_state = this->QuestEvolutionGetState(pQuest->GetID());
			}
		}*/
	}

	if ( quest_state != QUEST_EVOLUTION_STATE_NO_EXIST )
	{
		this->QuestEvolutionSend(quest_index);
		this->GetInterfaceState()->Set(InterfaceData::QuestEvolutionGiver, pNpc);
	}
}

void Player::QuestEvolutionSendInfo()
{
	if ( this->IsQuestEvolutionSend() )
	{
		return;
	}

	this->SetQuestEvolutionSend(true);

	QUEST_EVOLUTION_INFO pMsg;

	for ( QuestEvolutionStatusMap::iterator it = this->quest_evolution.begin(); it != this->quest_evolution.end(); ++it )
	{
		pMsg.state[it->first / 4] |= it->second.GetStateByte();
		pMsg.count++;
		pMsg.h.size++;
	}
	
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::QuestEvolutionTake(QuestEvolution const* pQuest)
{
	if ( !pQuest )
	{
		return;
	}

	for ( QuestEvolutionDataList::const_iterator itr = pQuest->data.begin(); itr != pQuest->data.end(); ++itr )
	{
		QuestEvolutionData const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( !this->SatisfyQuestEvolutionClass(pData) )
		{
			continue;
		}

		for ( QuestEvolutionConditionList::const_iterator it = pQuest->condition.begin(); it != pQuest->condition.end(); ++it )
		{
			QuestEvolutionCondition const* pCondition = *it;

			if ( !pCondition )
			{
				continue;
			}

			if ( pCondition->GetConditionID() == -1 || pCondition->GetConditionID() == pData->GetConditionIndex() )
			{
				this->MoneyReduce(pCondition->GetRequiredZen());
			}
		}
	}
}

void Player::QuestEvolutionGiveReward(QuestEvolution const* pQuest)
{
	if ( !pQuest )
	{
		return;
	}

	for ( QuestEvolutionDataList::const_iterator itr = pQuest->data.begin(); itr != pQuest->data.end(); ++itr )
	{
		QuestEvolutionData const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( !this->SatisfyQuestEvolutionClass(pData) )
		{
			continue;
		}

		if ( pData->GetRewardType() == QUEST_EVOLUTION_COMPENSATION_STAT_UP )
		{
			this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(pData->GetRewardCount());
			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_STAT_UP, pData->GetRewardCount());

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest StatUp [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}
		else if ( pData->GetRewardType() == QUEST_EVOLUTION_COMPENSATION_CHANGE_UP )
		{
			this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(pData->GetRewardCount());
			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_STAT_UP, pData->GetRewardCount());
			this->SetChangeUP(0, 1);
			this->OrDBClass(1);
			this->PreviewMake();

			//uint8 db_class = ((this->GetClass()<<5)&0xE0) | ((1<<4)&0x10);

			uint8 db_class = this->GetDBClass() & ~3;
			db_class |= 8;

			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_CHANGE_UP, db_class);

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest ChangeUp [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}
		else if ( pData->GetRewardType() == QUEST_EVOLUTION_COMPENSATION_PLUS_STAT )
		{
			int32 level = this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() - QUEST_EVOLUTION_PLUS_STAT_MIN_LEVEL;

			if ( level < 0 )
				level = 0;

			this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(level);
			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_PLUS_STAT, level);

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest PlusStat [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}
		else if ( pData->GetRewardType() == QUEST_EVOLUTION_COMPENSATION_COMBO_SKILL )
		{
			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_COMBO_SKILL, 0);

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest ComboSkill [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}
		else if ( pData->GetRewardType() == QUEST_EVOLUTION_COMPENSATION_MASTER_UP )
		{
			this->SetChangeUP(1, 1);

			if ( this->IsSpecialCharacter() )
			{
				this->OrDBClass(3);
			}
			else
			{
				this->OrDBClass(2);
			}

			this->PreviewMake();
			
			uint8 db_class = this->GetDBClass() & ~3;
			db_class |= 8;
			db_class |= 4;

			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_MASTER_UP,db_class);

			CharacterBaseData const* base_info = sCharacterBase->GetCharacterBase(this->GetClass());

			if ( base_info )
			{
				this->GetLevelData(LEVEL_DATA_MASTER)->SetPoints(this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() * base_info->GetMasterLevelUpPoints());
			}
			
			this->CalculateNextExperience();
			this->MasterSendStatus();
			this->CalculateCharacter();

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest MasterUp [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}
		else if ( pData->GetRewardType() == QUEST_EVOLUTION_COMPENSATION_ALL_STAT_UP )
		{
			this->IncreaseStat(STRENGTH, pData->GetRewardCount());
			this->IncreaseStat(AGILITY, pData->GetRewardCount());
			this->IncreaseStat(VITALITY, pData->GetRewardCount());
			this->IncreaseStat(ENERGY, pData->GetRewardCount());

			if ( this->GetClass() == Character::DARK_LORD )
			{
				this->IncreaseStat(LEADERSHIP, pData->GetRewardCount());
			}

			this->CalculateCharacter();

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest AllStatUp [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}
		else if ( pData->GetRewardType() == QUEST_EVOLUTION_COMPENSATION_MAJESTIC_UP )
		{
			this->SetChangeUP(2, 1);
			this->OrDBClass(4);

			this->PreviewMake();
			
			uint8 db_class = this->GetDBClass() & ~15;
			db_class |= 8;
			db_class |= 4;
			db_class |= 2;

			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_MAJESTIC_UP, db_class);

			CharacterBaseData const* base_info = sCharacterBase->GetCharacterBase(this->GetClass());

			if ( base_info )
			{
				this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel() * base_info->GetMajesticLevelUpPoints());
			}
			
			this->CalculateNextExperience();
			this->MasterSendStatus();
			this->MajesticSkillTreeSend();
			this->MajesticSkillTreeStatSend();
			this->CalculateCharacter();

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest MajesticUp [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}

		if ( pData->GetRewardSubType() == QUEST_EVOLUTION_COMPENSATION_STAT_UP )
		{
			this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(pData->GetRewardCount());
			this->QuestEvolutionSendPrize(QUEST_EVOLUTION_COMPENSATION_STAT_UP,pData->GetRewardCount());

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest Sub StatUp [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}

		if ( pData->GetRewardSubType() == QUEST_EVOLUTION_COMPENSATION_ALL_STAT_UP )
		{
			this->IncreaseStat(STRENGTH, pData->GetRewardCount());
			this->IncreaseStat(AGILITY, pData->GetRewardCount());
			this->IncreaseStat(VITALITY, pData->GetRewardCount());
			this->IncreaseStat(ENERGY, pData->GetRewardCount());

			if ( this->GetClass() == Character::DARK_LORD )
			{
				this->IncreaseStat(LEADERSHIP, pData->GetRewardCount());
			}

			this->CalculateCharacter();

			sLog->outInfo(LOG_QUEST, "%s -- Rewared Quest Sub AllStatUp [%u - %s] %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), this->BuildLog().c_str());
		}

		if ( pData->GetObjectiveType() == QUEST_EVOLUTION_OBJECTIVE_ITEM )
		{
			this->ItemDelete(pData->GetItem(), pData->GetItemLevel(), pData->GetCount());
		}

		sQuestMgr->GiveQuestEvolutionItemReward(this, pQuest->GetID());
	}

	this->CheckAddInfinityArrow();
}

void Player::QuestEvolutionSendPrize(uint8 prize, uint8 count) ///- TODO: Cambiar count a uint16
{
	QUEST_EVOLUTION_PRIZE pMsg(this->GetEntry(), prize, count);

	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Player::QuestEvolutionSend(uint8 quest_id)
{
	QUEST_EVOLUTION_SET_STATE pMsg(quest_id);
	pMsg.state = this->QuestEvolutionGetStateByteGroup(quest_id / 4);

	this->QuestEvolutionSendMonsterKill(quest_id);

	if ( pMsg.state != 0 )
	{
		this->sendPacket(MAKE_PCT(pMsg));
	}
}

void Player::QuestEvolutionSendMonsterKill(uint8 quest_id)
{
	QuestEvolution const* pQuest = sQuestMgr->GetQuest(quest_id);

	if ( !pQuest )
	{
		return;
	}

	QuestEvolutionStatus * pQuestData = this->QuestEvolutionFind(quest_id);

	if ( !pQuestData )
	{
		return;
	}

	if ( pQuestData->GetState() != QUEST_EVOLUTION_STATE_ACCEPTED )
	{
		return;
	}

	QUEST_EVOLUTION_MONSTER_KILL pMsg(0, quest_id);

	for ( QuestEvolutionDataList::const_iterator itr = pQuest->data.begin(); itr != pQuest->data.end(); ++itr )
	{
		QuestEvolutionData const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( !this->SatisfyQuestEvolutionClass(pData) )
		{
			continue;
		}

		if ( pData->GetObjectiveType() != QUEST_EVOLUTION_OBJECTIVE_MONSTER )
		{
			continue;
		}

		pMsg.result = 1;

		pMsg.monster[pData->GetMonsterSlot()].monster_class = pData->GetMonsterID();
		pMsg.monster[pData->GetMonsterSlot()].count = pQuestData->GetMonsterCount(pData->GetMonsterSlot());
	}

	if ( pMsg.result )
	{
		this->sendPacket(MAKE_PCT(pMsg));
	}
}

void Player::QuestEvolutionSendMonsterKill()
{
	for ( QuestEvolutionStatusMap::const_iterator it = this->quest_evolution.begin(); it != this->quest_evolution.end(); ++it )
	{
		if ( it->second.GetState() == QUEST_EVOLUTION_STATE_ACCEPTED )
		{
			this->QuestEvolutionSendMonsterKill(it->first);
		}
	}
}

void Player::QuestEvolutionMonsterKill(Monster* pVictim)
{
	Unit* mMember[MAX_PARTY_MEMBERS];
	uint8 count = 0;

	this->GetPartyMembers(mMember, count, MAX_PARTY_MONSTER_KILL_DISTANCE);

	if ( count > MAX_PARTY_MEMBERS )
	{
		count = MAX_PARTY_MEMBERS;
	}

	for ( QuestEvolutionMap::const_iterator itr = sQuestMgr->m_quest_evolution.begin(); itr != sQuestMgr->m_quest_evolution.end(); ++itr )
	{
		QuestEvolution const* pQuest = itr->second;

		if ( !pQuest )
		{
			continue;
		}

		if ( !pQuest->GetParty() && this->GetPartyID() != PARTY_NULL )
		{
			continue;
		}

		for ( QuestEvolutionDataList::const_iterator it = pQuest->data.begin(); it != pQuest->data.end(); ++it )
		{
			QuestEvolutionData const* pData = *it;

			if ( !pData )
			{
				continue;
			}

			if ( pData->GetObjectiveType() != QUEST_EVOLUTION_OBJECTIVE_MONSTER )
			{
				continue;
			}

			if ( pData->GetMonsterID() != pVictim->GetClass() )
			{
				continue;
			}

			if ( pData->GetMonsterMinLevel() > pVictim->GetLevel() || pData->GetMonsterMaxLevel() < pVictim->GetLevel() )
			{
				continue;
			}

			for ( uint8 h = 0; h < count; ++h )
			{
				if ( !mMember[h] )
				{
					continue;
				}

				Player* pPlayer = mMember[h]->ToPlayer();

				if ( !pPlayer )
				{
					continue;
				}

				if ( !pPlayer->SatisfyQuestEvolutionClass(pData) )
				{
					continue;
				}

				QuestEvolutionStatus * pQuestData = pPlayer->QuestEvolutionFind(pQuest->GetID());

				if ( !pQuestData || pQuestData->GetState() != QUEST_EVOLUTION_STATE_ACCEPTED )
				{
					continue;
				}

				if ( pQuestData->GetMonsterCount(pData->GetMonsterSlot()) >= pData->GetCount() )
				{
					continue;
				}

				pQuestData->IncreaseMonsterCount(pData->GetMonsterSlot(), 1);

				sLog->outInfo(LOG_QUEST, "%s -- [%u - %s] Monster Kill [%u - %d/%u] - %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), pData->GetMonsterID(), 
					pQuestData->GetMonsterCount(pData->GetMonsterSlot()), pData->GetCount(), pPlayer->BuildLog().c_str());

				pPlayer->QuestEvolutionSendMonsterKill(pQuestData->GetID());
			}
		}
	}
}

bool Player::QuestEvolutionMonsterDrop(Monster* pVictim)
{
	Unit* mMember[MAX_PARTY_MEMBERS];
	uint8 count = 0;

	this->GetPartyMembers(mMember, count, MAX_PARTY_MONSTER_KILL_DISTANCE);

	if ( count > MAX_PARTY_MEMBERS )
		count = 1;

	for ( QuestEvolutionMap::const_iterator itr = sQuestMgr->m_quest_evolution.begin(); itr != sQuestMgr->m_quest_evolution.end(); ++itr )
	{
		QuestEvolution const* pQuest = itr->second;

		if ( !pQuest )
		{
			continue;
		}

		if ( !pQuest->GetParty() && this->GetPartyID() != uint16(-1) )
		{
			continue;
		}

		for ( QuestEvolutionDataList::const_iterator it = pQuest->data.begin(); it != pQuest->data.end(); ++it )
		{
			QuestEvolutionData const* pData = *it;

			if ( !pData )
			{
				continue;
			}

			if ( pData->GetObjectiveType() != QUEST_EVOLUTION_OBJECTIVE_ITEM )
			{
				continue;
			}

			if ( pData->GetMonsterID() != pVictim->GetClass() && pData->GetMonsterID() != -1 )
			{
				continue;
			}

			if ( pData->GetMonsterMinLevel() > pVictim->GetLevel() || pData->GetMonsterMaxLevel() < pVictim->GetLevel() )
			{
				continue;
			}

			for ( uint8 i = 0; i < count; ++i )
			{
				if ( !mMember[i] )
				{
					continue;
				}

				Player* pPlayer = mMember[i]->ToPlayer();

				if ( !pPlayer )
				{
					continue;
				}

				if ( !pPlayer->SatisfyQuestEvolutionClass(pData) )
				{
					continue;
				}

				if ( pPlayer->QuestEvolutionGetState(pQuest->GetID()) != QUEST_EVOLUTION_STATE_ACCEPTED )
				{
					continue;
				}

				if ( pData->GetDropRate() <= Random<uint16>(QUEST_EVOLUTION_DROP_RATE) )
				{
					continue;
				}

				int32 item_count = pPlayer->GetInventory()->GetItemCount(pData->GetItem(), pData->GetItemLevel());

				if ( pData->GetCount() <= item_count )
				{
					continue;
				}

				sItemMgr->ItemSerialCreate(pPlayer, pVictim->GetWorldId(), pVictim->GetX(), pVictim->GetY(), Item(pData->GetItem(), pData->GetItemLevel()), true, true, true);

				sLog->outInfo(LOG_QUEST, "%s -- [%u - %s] Item Drop [%d - %d] - %s", __FUNCTION__, pQuest->GetID(), pQuest->GetName().c_str(), pData->GetItem(), pData->GetItemLevel(), 
					pPlayer->BuildLog().c_str());

				return true;
			}
		}
	}

	return false;
}

void Player::QuestEvolutionUpdate()
{
	if ( this->GetWorldId() != WORLD_PLACE_OF_QUALIFICATION )
	{
		return;
	}

	Quest4thInstanceData const* pData = sQuestMgr->GetQuest4thInstanceData(this->GetInstance());

	if ( !pData )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong instance information. You will be moved to the safe zone.");
		this->MoveToGate(537);
		return;
	}

	if ( pData->GetPartyID() != this->GetPartyID() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Your party status has been changed. You will be moved to the safe zone.");
		this->MoveToGate(537);
		return;
	}
}

int32 Player::QuestEvolutionGetInstance() const
{
	Party* pParty = this->GetParty();

	if ( pParty )
	{
		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			{
				continue;
			}

			Player* pPlayer = pParty->GetMember(i)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetWorldId() != WORLD_PLACE_OF_QUALIFICATION )
			{
				continue;
			}

			return pPlayer->GetInstance();
		}
	}

	return -1;
}