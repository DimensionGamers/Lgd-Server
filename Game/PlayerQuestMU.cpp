/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerQuestSupport.cpp"
*
*/

void Player::ElfSoldierBuffUP(bool levelup)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (!levelup)
	{
		if (this->GetInterfaceState()->GetID() != InterfaceData::QuestSupportGiver)
		{
			return;
		}
	}

	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() > sGameServer->GetElfSoldierMaxLevel())
	{
		if (!levelup)
			this->CommandSend(0x0D, 0, 0);
		else
		{
			if (this->HasBuff(BUFF_ELF_SOLDIER))
				this->RemoveBuff(BUFF_ELF_SOLDIER);
		}

		return;
	}

	if (levelup)
	{
		if (!sGameServer->IsElfSoldierUpdate())
			return;

		if (!this->HasBuff(BUFF_ELF_SOLDIER))
			return;
	}

	this->AddBuff(BUFF_ELF_SOLDIER,
		BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() / 3 + 45),
		BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() / 5 + 50),
		60 * ((this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() / 6) + 30), BUFF_FLAG_COMBAT);
}

QuestMUData const* Player::GetQuestMU(uint16 id) const
{
	QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.find(id);

	if (itr != this->m_QuestMUDataMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

QuestMUData* Player::GetQuestMU(uint16 id)
{
	QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.find(id);

	if (itr != this->m_QuestMUDataMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

void Player::AddQuestMU(uint16 id, uint8 state, uint16 objective)
{
	QuestMUData* pData = new QuestMUData;
	pData->SetID(id);
	pData->SetState(state);
	pData->SetObjective(objective);
	pData->SetDate(time(nullptr));

	this->m_QuestMUDataMap[pData->GetID()] = pData;
}

void Player::SetQuestMUState(uint16 id, uint8 state)
{
	QuestMUData* pData = this->GetQuestMU(id);

	if (!pData)
	{
		return;
	}

	pData->SetState(state);
}

void Player::RemoveQuestMU(uint16 id)
{
	QuestMUDataMap::iterator itr = this->m_QuestMUDataMap.find(id);

	if (itr != this->m_QuestMUDataMap.end())
	{
		delete itr->second;
		this->m_QuestMUDataMap.erase(itr);
	}
}

void Player::SendQuestMU()
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(QUEST_MU_STATUS_HEAD, head, buffer, 0);
	POINTER_PCT(QUEST_MU_STATUS_BODY, body, buffer, sizeof(QUEST_MU_STATUS_HEAD));
	head->count = 0;

	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pQuest = sQuestMgr->GetQuestMU(pData->GetID());

		if (!pQuest)
		{
			continue;
		}

		body[head->count].junk2 = 0;
		body[head->count].junk3 = 0;
		body[head->count].state = 1;

		body[head->count].chapter = pQuest->GetChapter();
		body[head->count].quest_id = pQuest->GetID();
		body[head->count].objective = pData->GetObjective();
		body[head->count].completed = 1;

		if (this->IsQuestMUComplete(pQuest->GetID()))
		{
			body[head->count].state = 2;
		}

		++head->count;
	}

	head->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_MU_STATUS, sizeof(QUEST_MU_STATUS_HEAD) + (head->count * sizeof(QUEST_MU_STATUS_BODY)));
	this->sendPacket(buffer);
}

void Player::QuestMUObjectiveUpdate(QuestMUObjectiveType type, Unit* pUnit, int32 data)
{
	switch (type)
	{
	case QUEST_MU_OBJECTIVE_MONSTER_KILL:
	{
		if (Monster* pMonster = pUnit->ToCreature())
		{
			this->QuestMUObjectiveMonsterKill(pMonster);
		}
	} break;

	case QUEST_MU_OBJECTIVE_LEVEL:
	{
		this->QuestMUObjectiveLevel();
	} break;

	case QUEST_MU_OBJECTIVE_LOCATION:
	{
		this->QuestMUObjectiveLocation();
	} break;

	case QUEST_MU_OBJECTIVE_FIND_NPC:
	{
		if (Monster* pMonster = pUnit->ToCreature())
		{
			this->QuestMUObjectiveFindNPC(pMonster);
		}
	} break;

	case QUEST_MU_OBJECTIVE_MONSTER_KILL_MAP:
	{
		if (Monster* pMonster = pUnit->ToCreature())
		{
			this->QuestMUObjectiveMonsterKillMap(pMonster);
		}
	} break;

	case QUEST_MU_OBJECTIVE_CHAOS_CASTLE_CLEAR:
	{
		this->QuestMUObjectiveChaosCastleClear();
	} break;

	case QUEST_MU_OBJECTIVE_CHAOS_CASTLE_PLAYER_KILL:
	{
		this->QuestMUObjectiveChaosCastlePlayerKill();
	} break;

	case QUEST_MU_OBJECTIVE_BLOOD_CASTLE_CLEAR:
	{
		this->QuestMUObjectiveBloodCastleClear();
	} break;

	case QUEST_MU_OBJECTIVE_BLOOD_CASTLE_GATE_DESTROY:
	{
		this->QuestMUObjectiveBloodCastleGateDestroy();
	} break;

	case QUEST_MU_OBJECTIVE_DEVIL_SQUARE_CLEAR:
	{
		this->QuestMUObjectiveDevilSquareClear();
	} break;

	case QUEST_MU_OBJECTIVE_DEVIL_SQUARE_POINTS:
	{
		this->QuestMUObjectiveDevilSquarePoints(data);
	} break;

	case QUEST_MU_OBJECTIVE_CHAOS_CASTLE_MONSTER_KILL:
	{
		this->QuestMUObjectiveChaosCastleMonsterKill();
	} break;
	}
}

void Player::QuestMUObjectiveItem(Item const& item)
{

}

void Player::QuestMUObjectiveMonsterKill(Monster* pMonster)
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_MONSTER_KILL)
		{
			continue;
		}

		if (pData->GetObjectiveMainType() != pMonster->GetClass())
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveLevel()
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_LEVEL)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetObjectiveMainType())
		{
			continue;
		}

		int32 value = pQuestData->GetObjective() + this->GetTotalLevel();

		if (value > pData->GetObjectiveMainType())
		{
			value = pData->GetObjectiveMainType();
		}

		pQuestData->SetObjective(value);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveLocation()
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_LOCATION)
		{
			continue;
		}

		if (pData->GetMapID() != this->GetWorldId() ||
			pData->GetX() != this->GetX() ||
			pData->GetY() != this->GetY())
		{
			continue;
		}

		if (pQuestData->GetObjective() > 0)
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveFindNPC(Monster* pMonster)
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_FIND_NPC)
		{
			continue;
		}

		if (pData->GetChapter() == 1)
		{
			if (pMonster->GetClass() != 257)
			{
				continue;
			}
		}
		else if (pData->GetChapter() == 2) ///- Chaos Castle Ticket
		{
			if (pMonster->GetClass() != 257)
			{
				continue;
			}
		}
		else if (pData->GetChapter() == 3)
		{
			if (pMonster->GetClass() != 719 && pMonster->GetClass() != 233)
			{
				continue;
			}
		}
		else if (pData->GetChapter() == 4)
		{
			if (pMonster->GetClass() != 720 && pMonster->GetClass() != 237)
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		if (pQuestData->GetObjective() > 0)
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveMonsterKillMap(Monster* pMonster)
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_MONSTER_KILL_MAP)
		{
			continue;
		}

		if (pData->GetMapID() != pMonster->GetWorldId())
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveChaosCastleClear()
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_CHAOS_CASTLE_CLEAR)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		if (!sEventMgr->IsEventGroundLevel(EVENT_CHAOS_CASTLE, this, pData->GetLevelMin(), pData->GetLevelMax()))
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveChaosCastlePlayerKill()
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_CHAOS_CASTLE_PLAYER_KILL)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		if (!sEventMgr->IsEventGroundLevel(EVENT_CHAOS_CASTLE, this, pData->GetLevelMin(), pData->GetLevelMax()))
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveBloodCastleClear()
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_BLOOD_CASTLE_CLEAR)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		if (!sEventMgr->IsEventGroundLevel(EVENT_BLOOD_CASTLE, this, pData->GetLevelMin(), pData->GetLevelMax()))
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveBloodCastleGateDestroy()
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_BLOOD_CASTLE_GATE_DESTROY)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		if (!sEventMgr->IsEventGroundLevel(EVENT_BLOOD_CASTLE, this, pData->GetLevelMin(), pData->GetLevelMax()))
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveDevilSquareClear()
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_DEVIL_SQUARE_CLEAR)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		if (!sEventMgr->IsEventGroundLevel(EVENT_DEVIL_SQUARE, this, pData->GetLevelMin(), pData->GetLevelMax()))
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveDevilSquarePoints(int32 points)
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_DEVIL_SQUARE_POINTS)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		if (!sEventMgr->IsEventGroundLevel(EVENT_DEVIL_SQUARE, this, pData->GetLevelMin(), pData->GetLevelMax()))
		{
			continue;
		}

		int32 total_points = pQuestData->GetObjective() + points;

		if (total_points > pData->GetTargetNumber())
		{
			total_points = pData->GetTargetNumber();
		}

		pQuestData->SetObjective(total_points);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

void Player::QuestMUObjectiveChaosCastleMonsterKill()
{
	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_CHAOS_CASTLE_MONSTER_KILL)
		{
			continue;
		}

		if (pQuestData->GetObjective() >= pData->GetTargetNumber())
		{
			continue;
		}

		if (!sEventMgr->IsEventGroundLevel(EVENT_CHAOS_CASTLE, this, pData->GetLevelMin(), pData->GetLevelMax()))
		{
			continue;
		}

		pQuestData->IncreaseObjective(1);

		this->SendQuestMUState(pQuestData->GetID());
	}
}

bool Player::QuestMUObjectiveItemDrop(Monster* pMonster)
{
	Unit* mMember[MAX_PARTY_MEMBERS];
	uint8 count = 0;

	this->GetPartyMembers(mMember, count, MAX_PARTY_MONSTER_KILL_DISTANCE);

	if (count > MAX_PARTY_MEMBERS)
	{
		count = 1;
	}

	for (uint8 i = 0; i < count; ++i)
	{
		if (!mMember[i])
		{
			continue;
		}

		Player* pPlayer = mMember[i]->ToPlayer();

		if (!pPlayer)
		{
			continue;
		}

		for (QuestMUDataMap::const_iterator itr = pPlayer->m_QuestMUDataMap.begin(); itr != pPlayer->m_QuestMUDataMap.end(); ++itr)
		{
			QuestMUData* pQuestData = itr->second;

			if (!pQuestData)
			{
				continue;
			}

			if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
			{
				continue;
			}

			QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

			if (!pData)
			{
				continue;
			}

			if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_ITEM)
			{
				continue;
			}

			if (pData->GetDropRate() <= 0)
			{
				continue;
			}

			if (pData->GetTargetMaxLevel() != -1 && pData->GetTargetMaxLevel() != pMonster->GetClass())
			{
				continue;
			}

			if (!roll_chance_i(pData->GetDropRate(), 10000))
			{
				continue;
			}

			sItemMgr->ItemSerialCreate(pPlayer, pMonster->GetWorldId(), pMonster->GetX(), pMonster->GetY(), Item(pData->GetObjectiveItem()), true, true, true);

			return true;
		}
	}

	return false;
}

void Player::QuestMUObjectiveItemGet(Item const& item)
{
	for (QuestMUDataMap::iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_ITEM)
		{
			continue;
		}

		if (pData->GetObjectiveItem() != item.GetItem())
		{
			continue;
		}

		this->SendQuestMUState(pQuestData->GetID());
	}
}

bool Player::CanLootQuestMUItem(Item const& item) const
{
	int32 total_count = this->GetInventory()->GetItemCount(item.GetItem(), -1);
	int32 count = 0;

	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData const* pQuestData = itr->second;

		if (!pQuestData)
		{
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			continue;
		}

		if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_ITEM)
		{
			continue;
		}

		if (pData->GetObjectiveItem() != item.GetItem())
		{
			continue;
		}

		count += pData->GetTargetNumber();
	}

	if (count <= 0)
	{
		return true;
	}

	if (total_count >= count)
	{
		return false;
	}

	return true;
}

void Player::QuestMURemoveGens()
{
	for (QuestMUDataMap::iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end();)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			++itr;
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
		{
			++itr;
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			++itr;
			continue;
		}

		if (pData->GetCheckGens() != this->GetGen()->GetFamily())
		{
			++itr;
			continue;
		}

		delete itr->second;
		this->m_QuestMUDataMap.erase(itr++);

		QUEST_MU_GIVE_UP_RESULT pMsg;
		pMsg.chapter = pData->GetChapter();
		pMsg.quest_id = pData->GetID();

		this->SendPacket(&pMsg);
	}
}

void Player::SendQuestMUState(uint16 id)
{
	QuestMUData const* pQuestData = this->GetQuestMU(id);

	if (!pQuestData)
	{
		return;
	}

	QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

	if (!pData)
	{
		return;
	}

	QUEST_MU_OBJECTIVE_UPDATE pMsg;
	pMsg.chapter = pData->GetChapter();
	pMsg.quest_id = pQuestData->GetID();
	pMsg.count = pQuestData->GetObjective();
	pMsg.state = 1;

	if (this->IsQuestMUComplete(pQuestData->GetID()))
	{
		pMsg.state = 2;
	}

	this->SendPacket(&pMsg);
}

void Player::UpdateQuestMULevel()
{
	for (QuestMUMap::const_iterator itr = sQuestMgr->m_QuestMUMap.begin(); itr != sQuestMgr->m_QuestMUMap.end(); ++itr)
	{
		QuestMU const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetStartType() != QUEST_MU_START_TYPE_LEVEL)
		{
			continue;
		}

		if (this->GetTotalLevel() < pData->GetStartSubType())
		{
			continue;
		}

		QuestMUData const* pQuestData = this->GetQuestMU(pData->GetID());

		if (pQuestData)
		{
			continue;
		}

		if (!this->IsQuestMURequirements(pData->GetID()))
		{
			continue;
		}

		this->AddQuestMU(pData->GetID(), QUEST_MU_STATE_ACTIVE, 0);

		QUEST_MU_ACCEPT_RESULT pMsg;
		pMsg.chapter = pData->GetChapter();
		pMsg.quest_id = pData->GetID();

		this->SendPacket(&pMsg);
	}
}

bool Player::IsQuestMUComplete(uint16 id) const
{
	QuestMUData const* pQuestData = this->GetQuestMU(id);

	if (!pQuestData)
	{
		return false;
	}

	QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

	if (!pData)
	{
		return false;
	}

	switch (pData->GetObjectiveType())
	{
	case QUEST_MU_OBJECTIVE_ITEM:
	{
		if (this->GetInventory()->GetItemCount(pData->GetObjectiveItem(), 0) < pData->GetTargetNumber())
		{
			return false;
		}
	} break;

	case QUEST_MU_OBJECTIVE_MONSTER_KILL:
	case QUEST_MU_OBJECTIVE_MONSTER_KILL_MAP:
	case QUEST_MU_OBJECTIVE_LOCATION:
	case QUEST_MU_OBJECTIVE_FIND_NPC:
	case QUEST_MU_OBJECTIVE_CHAOS_CASTLE_CLEAR:
	case QUEST_MU_OBJECTIVE_CHAOS_CASTLE_PLAYER_KILL:
	case QUEST_MU_OBJECTIVE_BLOOD_CASTLE_CLEAR:
	case QUEST_MU_OBJECTIVE_BLOOD_CASTLE_GATE_DESTROY:
	case QUEST_MU_OBJECTIVE_DEVIL_SQUARE_CLEAR:
	case QUEST_MU_OBJECTIVE_DEVIL_SQUARE_POINTS:
	case QUEST_MU_OBJECTIVE_CHAOS_CASTLE_MONSTER_KILL:
	{
		if (pQuestData->GetObjective() <= 0 || pQuestData->GetObjective() < pData->GetTargetNumber())
		{
			return false;
		}
	} break;

	case QUEST_MU_OBJECTIVE_LEVEL:
	{
		if (this->GetTotalLevel() < pData->GetTargetNumber())
		{
			return false;
		}
	} break;
	}

	return true;
}

void Player::UpdateQuestMU()
{
	if (!this->IsLive())
	{
		return;
	}

	if (this->GetInterfaceState()->GetID() != InterfaceData::None)
	{
		return;
	}

	if (!this->GetTimer(PLAYER_TIMER_QUEST_MU)->Elapsed(MINUTE * IN_MILLISECONDS))
	{
		return;
	}

	time_t current_date = time(nullptr);

	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end();)
	{
		QuestMUData* pQuestData = itr->second;

		if (!pQuestData)
		{
			this->m_QuestMUDataMap.erase(itr++);
			continue;
		}

		if (pQuestData->GetState() != QUEST_MU_STATE_COMPLETED)
		{
			++itr;
			continue;
		}

		QuestMU const* pData = sQuestMgr->GetQuestMU(pQuestData->GetID());

		if (!pData)
		{
			delete itr->second;
			this->m_QuestMUDataMap.erase(itr++);
			continue;
		}

		if (pData->GetRepeatCycle() == 0)
		{
			++itr;
			continue;
		}

		time_t diff = (current_date - pQuestData->GetDate()) / DAY;

		if (diff < pData->GetRepeatCycle())
		{
			++itr;
			continue;
		}

		delete itr->second;
		this->m_QuestMUDataMap.erase(itr++);
	}
}

void Player::SendQuestMUAutoMove()
{
	QUEST_GUIDED_WALK pMsg;
	this->SEND_PCT(pMsg);
}

bool Player::IsQuestMURequirements(uint16 id) const
{
	QuestMU const* pData = sQuestMgr->GetQuestMU(id);

	if (!pData)
	{
		return false;
	}

	if (!pData->IsLevel(this->GetTotalLevel()))
	{
		return false;
	}

	if (!pData->IsClass(this->GetClass()))
	{
		return false;
	}

	if (pData->GetCheckGens() != -1 && this->GetGen()->GetFamily() != pData->GetCheckGens())
	{
		return false;
	}

	if (pData->GetDay() != 0)
	{
		Custom::SystemTimer timer = Custom::SystemTimer();

		if (timer.GetDayOfWeek() != (pData->GetDay() - 1))
		{
			return false;
		}
	}

	if (pData->GetPrecedenceQuest() != -1)
	{
		QuestMUData const* pPrecedentQuest = this->GetQuestMU(pData->GetPrecedenceQuest());

		if (!pPrecedentQuest)
		{
			return false;
		}

		if (pPrecedentQuest->GetState() != QUEST_MU_STATE_COMPLETED)
		{
			return false;
		}
	}

	return true;
}

void Player::QuestMUNpcTalk(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_NPC_TALK, lpMsg, Packet, 0);

	Unit* pTarget = this->GetInterfaceState()->GetTarget();

	if (!pTarget)
	{
		sLog->outError("quest_mu", "%s :: Wrong interface target: %u", __FUNCTION__, lpMsg->npc);
		return;
	}

	Monster* pCreature = pTarget->ToCreature();

	if (!pCreature)
	{
		sLog->outError("quest_mu", "%s :: Target not creature: %u", __FUNCTION__, lpMsg->npc);
		return;
	}

	if (pCreature->GetClass() != lpMsg->npc)
	{
		sLog->outError("quest_mu", "%s :: Target not class: %u - %u", __FUNCTION__, lpMsg->npc, pCreature->GetClass());
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(QUEST_MU_NPC_TALK_HEAD, head, buffer, 0);
	POINTER_PCT(QUEST_MU_NPC_TALK_BODY, body, buffer, sizeof(QUEST_MU_NPC_TALK_HEAD));
	head->count = 0;

	for (QuestMUMap::const_iterator itr = sQuestMgr->m_QuestMUMap.begin(); itr != sQuestMgr->m_QuestMUMap.end(); ++itr)
	{
		QuestMU const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetStartType() != QUEST_MU_START_TYPE_NPC)
		{
			continue;
		}

		if (pData->GetStartSubType() != lpMsg->npc)
		{
			continue;
		}

		QuestMUData const* pQuestData = this->GetQuestMU(pData->GetID());

		if (!pQuestData)
		{
			if (!this->IsQuestMURequirements(pData->GetID()))
			{
				continue;
			}
		}
		else
		{
			if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
			{
				continue;
			}
		}
		
		body[head->count].junk2 = 0;

		body[head->count].chapter = pData->GetChapter();
		body[head->count].quest_id = pData->GetID();
		body[head->count].state = 0;

		if (pQuestData)
		{
			body[head->count].state = 1;

			if (this->IsQuestMUComplete(pData->GetID()))
			{
				body[head->count].state = 2;
			}
		}

		++head->count;
	}

	head->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_MU_NPC_TALK, sizeof(QUEST_MU_NPC_TALK_HEAD) + (head->count * sizeof(QUEST_MU_NPC_TALK_BODY)));
	this->sendPacket(buffer);
}

void Player::QuestMUAccept(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_ACCEPT, lpMsg, Packet, 0);

	QuestMU const* pData = sQuestMgr->GetQuestMU(lpMsg->quest_id);

	if (!pData)
	{
		sLog->outError("quest_mu", "%s :: Quest DB: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	if (pData->GetChapter() != lpMsg->chapter)
	{
		sLog->outError("quest_mu", "%s :: Wrong Chapter: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pData->GetChapter());
		return;
	}
	
	QuestMUData const* pQuestData = this->GetQuestMU(pData->GetID());

	if (pQuestData)
	{
		sLog->outError("quest_mu", "%s :: Already have quest: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pQuestData->GetState());
		return;
	}

	if (!this->IsQuestMURequirements(pData->GetID()))
	{
		sLog->outError("quest_mu", "%s :: Not meet requirements: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	QUEST_MU_ACCEPT_RESULT pMsg;
	pMsg.chapter = pData->GetChapter();
	pMsg.quest_id = pData->GetID();

	if (!this->MoneyHave(pData->GetZen()))
	{
		pMsg.result = 1;
		this->SendPacket(&pMsg);
		return;
	}

	this->MoneyReduce(pData->GetZen());

	this->AddQuestMU(pData->GetID(), QUEST_MU_STATE_ACTIVE, 0);

	this->SendPacket(&pMsg);
}

void Player::QuestMUGiveUp(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_GIVE_UP, lpMsg, Packet, 0);

	QuestMU const* pData = sQuestMgr->GetQuestMU(lpMsg->quest_id);

	if (!pData)
	{
		sLog->outError("quest_mu", "%s :: Quest DB: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	if (pData->GetChapter() != lpMsg->chapter)
	{
		sLog->outError("quest_mu", "%s :: Wrong Chapter: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pData->GetChapter());
		return;
	}

	QUEST_MU_GIVE_UP_RESULT pMsg;
	pMsg.chapter = pData->GetChapter();
	pMsg.quest_id = pData->GetID();

	QuestMUData* pQuestData = this->GetQuestMU(pData->GetID());

	if (!pQuestData)
	{
		pMsg.result = 1;
	}
	else
	{
		if (pQuestData->GetState() == QUEST_MU_STATE_ACTIVE)
		{
			pQuestData->SetID(QUEST_MU_STATE_CANCELLED);

			if (pData->GetRepeatCycle() == -1)
			{
				this->RemoveQuestMU(pData->GetID());
			}
		}
	}

	this->SendPacket(&pMsg);
}

void Player::QuestMUComplete(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_COMPLETE, lpMsg, Packet, 0);

	QuestMU const* pData = sQuestMgr->GetQuestMU(lpMsg->quest_id);

	if (!pData)
	{
		sLog->outError("quest_mu", "%s :: Quest DB: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	if (pData->GetChapter() != lpMsg->chapter)
	{
		sLog->outError("quest_mu", "%s :: Wrong Chapter: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pData->GetChapter());
		return;
	}

	if (pData->GetStartType() == QUEST_MU_START_TYPE_NPC)
	{
		if (pData->GetStartSubType() != lpMsg->npc)
		{
			sLog->outError("quest_mu", "%s :: Wrong NPC: %u - %u", __FUNCTION__, lpMsg->npc, pData->GetStartSubType());
			return;
		}
	}

	QUEST_MU_COMPLETE_RESULT pMsg;
	pMsg.chapter = pData->GetChapter();
	pMsg.quest_id = pData->GetID();

	QuestMUData* pQuestData = this->GetQuestMU(pData->GetID());

	if (!pQuestData)
	{
		return;
	}

	if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
	{
		return;
	}

	if (!this->IsQuestMUComplete(pQuestData->GetID()))
	{
		pMsg.result = 4;
		this->SendPacket(&pMsg);
		return;
	}

	if (this->MoneyReachMaximum(pData->GetRewardZen()))
	{
		pMsg.result = 1;
		this->SendPacket(&pMsg);
		return;
	}

	if (pData->GetObjectiveType() == QUEST_MU_OBJECTIVE_ITEM)
	{
		this->ItemDelete(pData->GetObjectiveItem(), -1, pData->GetTargetNumber());
	}

	for (int32 i = 0; i < QUEST_MU_MAX_REWARD; ++i)
	{
		item_template const* pItemInfo = sItemMgr->GetItem(pData->GetRewardItem(i));

		if (!pItemInfo)
		{
			continue;
		}

		int32 count = 1;

		Item item(pData->GetRewardItem(i));

		if (pItemInfo->GetStackData()->GetMaxStack() > 0)
		{
			item.SetDurability(pData->GetRewardItemCount(i));
		}
		else if (pItemInfo->GetType() <= 11 || !pItemInfo->level_data.empty())
		{
			item.SetLevel(pData->GetRewardItemCount(i));
		}
		else
		{
			count = pData->GetRewardItemCount(i);
		}

		for (int32 n = 0; n < count; ++n)
		{
			sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, item);
		}
	}

	this->GenAddContribution(pData->GetRewardGensPoints());

	if (pData->GetRewardZen() > 0)
	{
		this->MoneyAdd(pData->GetRewardZen());
	}

	if (pData->GetRewardExperience() > 0)
	{
		int64 experience = pData->GetRewardExperience();
		this->GiveExperience(experience, false, true);
	}

	if (pData->GetRepeatCycle() == -1)
	{
		this->RemoveQuestMU(pData->GetID());
	}
	else
	{
		pQuestData->SetState(QUEST_MU_STATE_COMPLETED);
	}

	this->SendPacket(&pMsg);

	this->UpdateQuestMULevel();
}

void Player::QuestMUReachTarget(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_INFO, lpMsg, Packet, 0);

	QuestMU const* pData = sQuestMgr->GetQuestMU(lpMsg->quest_id);

	if (!pData)
	{
		sLog->outError("quest_mu", "%s :: Quest DB: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	if (pData->GetChapter() != lpMsg->chapter)
	{
		sLog->outError("quest_mu", "%s :: Wrong Chapter: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pData->GetChapter());
		return;
	}

	QuestMUData* pQuestData = this->GetQuestMU(pData->GetID());

	if (!pQuestData)
	{
		return;
	}

	if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
	{
		return;
	}

	if (pData->GetObjectiveType() != QUEST_MU_OBJECTIVE_LOCATION)
	{
		return;
	}

	if (pData->GetMapID() != this->GetWorldId() ||
		pData->GetX() != this->GetX() ||
		pData->GetY() != this->GetY())
	{
		return;
	}

	this->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_LOCATION);
}

void Player::QuestMUInfo(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_INFO, lpMsg, Packet, 0);

	QuestMU const* pData = sQuestMgr->GetQuestMU(lpMsg->quest_id);

	if (!pData)
	{
		sLog->outError("quest_mu", "%s :: Quest DB: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	if (pData->GetChapter() != lpMsg->chapter)
	{
		sLog->outError("quest_mu", "%s :: Wrong Chapter: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pData->GetChapter());
		return;
	}

	this->SendQuestMUState(pData->GetID());
}

void Player::QuestMUMove(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_MOVE, lpMsg, Packet, 0);

	QuestMU const* pData = sQuestMgr->GetQuestMU(lpMsg->quest_id);

	if (!pData)
	{
		sLog->outError("quest_mu", "%s :: Quest DB: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	if (pData->GetChapter() != lpMsg->chapter)
	{
		sLog->outError("quest_mu", "%s :: Wrong Chapter: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pData->GetChapter());
		return;
	}

	QuestMUData const* pQuestData = this->GetQuestMU(pData->GetID());

	if (!pQuestData)
	{
		return;
	}

	if (pQuestData->GetState() != QUEST_MU_STATE_ACTIVE)
	{
		return;
	}

	if (this->GetRegenStatus() != REGEN_NONE)
	{
		return;
	}

	if (this->IsTeleporting())
	{
		return;
	}

	if (this->MoveToGate(pData->GetGateID()))
	{
		this->SetQuestMUAutoMoving(pData->GetID());
	}
	else
	{
		QUEST_MU_CANT_MOVE pMsg;
		pMsg.world = pData->GetMapID();
		this->SendPacket(&pMsg);
	}
}

void Player::QuestMUClickCompleted(uint8 * Packet)
{
	if (!sGameServer->IsQuestMUEnabled())
	{
		return;
	}

	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_CHECK(QUEST_MU_MOVE, lpMsg, Packet, 0);

	QuestMU const* pData = sQuestMgr->GetQuestMU(lpMsg->quest_id);

	if (!pData)
	{
		sLog->outError("quest_mu", "%s :: Quest DB: %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id);
		return;
	}

	if (pData->GetChapter() != lpMsg->chapter)
	{
		sLog->outError("quest_mu", "%s :: Wrong Chapter: %u - %u - %u", __FUNCTION__, lpMsg->chapter, lpMsg->quest_id, pData->GetChapter());
		return;
	}
}