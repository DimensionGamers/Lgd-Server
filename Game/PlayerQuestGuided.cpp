void Player::QuestGuidedSend()
{
	QUEST_GUIDED_OBJECTIVE_SEND pMsg;

	pMsg.quest = this->GetQuestGuidedID();

	PlayerQuestGuided const* pQuestGuided = this->GetQuestGuided();

	if (pQuestGuided)
	{
		pMsg.count = pQuestGuided->GetCount();
		pMsg.status = pQuestGuided->GetState();
	}
	else
	{
		pMsg.count = 0;
		pMsg.status = 1;
	}

	this->SEND_PCT(pMsg);
}

PlayerQuestGuided * Player::GetQuestGuided()
{
	return this->GetQuestGuided(this->GetQuestGuidedID());
}
	
PlayerQuestGuided const* Player::GetQuestGuided() const
{
	return this->GetQuestGuided(this->GetQuestGuidedID());
}

PlayerQuestGuided * Player::GetQuestGuided(uint16 quest)
{
	PlayerQuestGuidedMap::const_iterator itr = this->quest_guided_map.find(quest);

	if ( itr != this->quest_guided_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}
	
PlayerQuestGuided const* Player::GetQuestGuided(uint16 quest) const
{
	PlayerQuestGuidedMap::const_iterator itr = this->quest_guided_map.find(quest);

	if ( itr != this->quest_guided_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

void Player::QuestGuidedSetStarting()
{
	//uint16 current_id = this->GetQuestGuidedID();
	PlayerQuestGuided const* pData = this->GetQuestGuided();

	if ( !pData || pData->GetState() == QUEST_GUIDED_STATE_REWARDED || pData->GetState() == QUEST_GUIDED_STATE_CANCELED )
	{
		for ( QuestGuidedMap::const_iterator itr = sQuestMgr->m_quest_guided_map.begin(); itr != sQuestMgr->m_quest_guided_map.end(); ++itr )
		{
			QuestGuided const* pQuestGuided = itr->second;

			if ( !pQuestGuided )
			{
				continue;
			}

			if ( pQuestGuided->GetID() <= this->GetQuestGuidedID() )
			{
				continue;
			}

			if ( !pQuestGuided->IsClassAllowed(this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)) )
			{
				continue;
			}

			this->SetQuestGuidedID(pQuestGuided->GetID());

			PlayerQuestGuided * pNewQuest = new PlayerQuestGuided;
			pNewQuest->SetID(pQuestGuided->GetID());
			pNewQuest->SetCount(0);
			pNewQuest->SetState(QUEST_GUIDED_STATE_ACTIVE);
			this->quest_guided_map[pQuestGuided->GetID()] = pNewQuest;
			break;
		}
	}
}

void Player::QuestGuidedMonsterKill(Monster* pMonster)
{
	if ( !pMonster )
	{
		return;
	}

	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	QuestGuided const* pData = sQuestMgr->GetQuestGuided(pQuestGuided->GetID());

	if ( !pData )
	{
		return;
	}

	if ( pData->GetData()->GetObjectiveType() != QUEST_GUIDED_OBJECTIVE_TYPE_MONSTER_KILL )
	{
		return;
	}

	if ( pData->GetData()->GetQuestMainType() != pMonster->GetClass() )
	{
		return;
	}

	if ( pData->GetData()->GetMonsterMaxLevel() != -1 && pMonster->GetLevel() > pData->GetData()->GetMonsterMaxLevel() )
	{
		return;
	}

	if ( pQuestGuided->GetCount() >= pData->GetData()->GetCount() )
	{
		return;
	}

	pQuestGuided->IncreaseCount(1);

	if ( pQuestGuided->GetCount() >= pData->GetData()->GetCount() )
	{
		pQuestGuided->SetState(QUEST_GUIDED_STATE_COMPLETE);
	}

	this->QuestGuidedSend();
}

void Player::QuestGuidedReachLevel()
{
	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	QuestGuided const* pData = sQuestMgr->GetQuestGuided(pQuestGuided->GetID());

	if ( !pData )
	{
		return;
	}

	if ( pData->GetData()->GetObjectiveType() != QUEST_GUIDED_OBJECTIVE_TYPE_LEVEL )
	{
		return;
	}

	if ( this->GetTotalLevel() < pData->GetData()->GetQuestMainType() )
	{
		return;
	}
	
	pQuestGuided->SetState(QUEST_GUIDED_STATE_COMPLETE);
	this->QuestGuidedSend();
}

void Player::QuestGuidedQuestComplete()
{
	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	QuestGuided const* pData = sQuestMgr->GetQuestGuided(pQuestGuided->GetID());

	if ( !pData )
	{
		return;
	}

	if ( pData->GetData()->GetObjectiveType() != QUEST_GUIDED_OBJECTIVE_TYPE_QUEST )
	{
		return;
	}

	if ( this->QuestEvolutionGetState(pData->GetData()->GetQuestMainType()) != QUEST_EVOLUTION_STATE_COMPLETE )
	{
		return;
	}

	pQuestGuided->SetState(QUEST_GUIDED_STATE_COMPLETE);
	this->QuestGuidedSend();
}

void Player::QuestGuidedItemGet()
{
	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	QuestGuided const* pData = sQuestMgr->GetQuestGuided(pQuestGuided->GetID());

	if ( !pData )
	{
		return;
	}

	if ( pData->GetData()->GetObjectiveType() != QUEST_GUIDED_OBJECTIVE_TYPE_ITEM )
	{
		return;
	}

	if ( this->GetInventory()->GetItemCount(pData->GetData()->GetRequiredItem(), 0) >= pData->GetData()->GetCount() )
	{
		pQuestGuided->SetCount(pData->GetData()->GetCount());
		pQuestGuided->SetState(QUEST_GUIDED_STATE_COMPLETE);
		this->QuestGuidedSend();
	}
	else
	{
		if ( pQuestGuided->GetCount() < this->GetInventory()->GetItemCount(pData->GetData()->GetRequiredItem(), 0) )
		{
			pQuestGuided->SetCount(this->GetInventory()->GetItemCount(pData->GetData()->GetRequiredItem(), 0));
			this->QuestGuidedSend();
		}
	}
}

void Player::QuestGuidedComplete(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, true, false) )
	{
		return;
	}

	POINTER_PCT_LOG(QUEST_GUIDED_COMPLETE_RESULT, lpMsg, Packet, 0);

	if ( this->GetQuestGuidedID() != lpMsg->quest )
	{
		this->QuestGuidedCompleteResult(lpMsg->quest, 0);
		return;
	}

	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		this->QuestGuidedCompleteResult(lpMsg->quest, 0);
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_COMPLETE )
	{
		this->QuestGuidedCompleteResult(lpMsg->quest, 0);
		return;
	}

	this->QuestGuidedGiveReward();
	this->QuestGuidedSend();
}

void Player::QuestGuidedCompleteResult(uint16 quest, uint16 result)
{
	QUEST_GUIDED_COMPLETE_RESULT pMsg;
	pMsg.quest = quest;
	pMsg.result = result;

	this->SEND_PCT(pMsg);
}
	
void Player::QuestGuidedGiveUp(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false, false) )
	{
		return;
	}

	POINTER_PCT_LOG(QUEST_GUIDED_GIVE_UP, lpMsg, Packet, 0);

	if ( this->GetQuestGuidedID() != lpMsg->quest )
	{
		return;
	}

	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	pQuestGuided->SetState(QUEST_GUIDED_STATE_CANCELED);

	this->QuestGuidedSetStarting();
	this->QuestGuidedSend();
}

void Player::QuestGuidedGiveUpChapter(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false, false) )
	{
		return;
	}

	POINTER_PCT_LOG(QUEST_GUIDED_GIVE_UP_CHAPTER, lpMsg, Packet, 0);

	/*if ( this->GetQuestGuidedID() != lpMsg->quest )
	{
		return;
	}*/

	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	QuestGuided const* pCurrentData = sQuestMgr->GetQuestGuided(this->GetQuestGuidedID());

	if ( !pCurrentData )
	{
		return;
	}

	pQuestGuided->SetState(QUEST_GUIDED_STATE_CANCELED);

	for ( QuestGuidedMap::const_iterator itr = sQuestMgr->m_quest_guided_map.begin(); itr != sQuestMgr->m_quest_guided_map.end(); ++itr )
	{
		QuestGuided const* pData = itr->second;

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetID() <= this->GetQuestGuidedID() )
		{
			continue;
		}

		if ( !pData->IsClassAllowed(this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)) )
		{
			continue;
		}

		this->SetQuestGuidedID(pData->GetID());

		PlayerQuestGuided * pNewQuest = new PlayerQuestGuided;
		pNewQuest->SetID(pData->GetID());
		pNewQuest->SetCount(0);

		if ( pCurrentData->GetCondition()->GetChapter() == pData->GetCondition()->GetChapter() )
		{
			pNewQuest->SetState(QUEST_GUIDED_STATE_CANCELED);
			this->quest_guided_map[pData->GetID()] = pNewQuest;
		}
		else
		{
			pNewQuest->SetState(QUEST_GUIDED_STATE_ACTIVE);
			this->quest_guided_map[pData->GetID()] = pNewQuest;
			break;
		}
	}

	this->QuestGuidedSend();
}

void Player::QuestGuidedWarp(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( this->GetRegenStatus() != REGEN_NONE )
	{
		return;
	}

	if ( this->IsTeleporting() )
	{
		return;
	}

	POINTER_PCT_LOG(QUEST_GUIDED_WARP, lpMsg, Packet, 0);

	if ( this->GetQuestGuidedID() != lpMsg->quest )
	{
		return;
	}

	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	QuestGuided const* pData = sQuestMgr->GetQuestGuided(pQuestGuided->GetID());

	if ( !pData )
	{
		return;
	}

	if ( this->MoveToGate(pData->GetData()->GetGateNumber()) )
	{
		this->SetQuestGuidedMove(true);
	}
}

void Player::QuestGuidedReachZone(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false, false) )
	{
		return;
	}

	POINTER_PCT_LOG(QUEST_GUIDED_REACH_TARGET, lpMsg, Packet, 0);

	if ( this->GetQuestGuidedID() != lpMsg->quest )
	{
		return;
	}

	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_ACTIVE )
	{
		return;
	}

	QuestGuided const* pData = sQuestMgr->GetQuestGuided(pQuestGuided->GetID());

	if ( !pData )
	{
		return;
	}

	if ( pData->GetData()->GetObjectiveType() != QUEST_GUIDED_OBJECTIVE_TYPE_REACH_ZONE )
	{
		return;
	}

	if ( this->GetWorldId() != pData->GetData()->GetWorld() ||
		 this->GetX() != pData->GetData()->GetX() ||
		 this->GetY() != pData->GetData()->GetY() )
	{
		return;
	}

	pQuestGuided->SetState(QUEST_GUIDED_STATE_COMPLETE);

	this->QuestGuidedSend();
}

void Player::QuestGuidedWalk()
{
	QUEST_GUIDED_WALK pMsg;
	this->SEND_PCT(pMsg);
}

void Player::QuestGuidedGiveReward()
{
	PlayerQuestGuided * pQuestGuided = this->GetQuestGuided();

	if ( !pQuestGuided )
	{
		this->QuestGuidedCompleteResult(this->GetQuestGuidedID(), 0);
		return;
	}

	if ( pQuestGuided->GetState() != QUEST_GUIDED_STATE_COMPLETE )
	{
		this->QuestGuidedCompleteResult(this->GetQuestGuidedID(), 0);
		return;
	}

	QuestGuided const* pData = sQuestMgr->GetQuestGuided(pQuestGuided->GetID());

	if ( !pData )
	{
		this->QuestGuidedCompleteResult(this->GetQuestGuidedID(), 0);
		return;
	}

	pQuestGuided->SetState(QUEST_GUIDED_STATE_REWARDED);
	pQuestGuided->SetDate(time(nullptr));

	int64 experience = pData->GetData()->GetRewardExp();
	this->GiveExperience(experience);

	this->MoneyAdd(pData->GetData()->GetRewardZen());

	if ( pData->GetData()->GetRewardItem1() != uint16(-1) && pData->GetData()->GetRewardItemCount01() > 0 )
	{
		for ( int32 i = 0; i < pData->GetData()->GetRewardItemCount01(); ++i )
		{
			sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(pData->GetData()->GetRewardItem1()));
		}
	}

	if ( pData->GetData()->GetRewardItem2() != uint16(-1) && pData->GetData()->GetRewardItemCount02() > 0 )
	{
		for ( int32 i = 0; i < pData->GetData()->GetRewardItemCount02(); ++i )
		{
			sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(pData->GetData()->GetRewardItem2()));
		}
	}

	if ( pData->GetData()->GetRewardItem3() != uint16(-1) && pData->GetData()->GetRewardItemCount03() > 0 )
	{
		for ( int32 i = 0; i < pData->GetData()->GetRewardItemCount03(); ++i )
		{
			sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(pData->GetData()->GetRewardItem3()));
		}
	}

	this->QuestGuidedCompleteResult(this->GetQuestGuidedID(), 1);

	this->QuestGuidedSetStarting();
}