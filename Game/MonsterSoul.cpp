MonsterSoul::MonsterSoul()
{

}

MonsterSoul::~MonsterSoul()
{
	MAP_CLEAR(MonsterSoulConverterMap::iterator, this->m_MonsterSoulConverterMap);
	LIST_CLEAR(MonsterSoulTransformationList::iterator, this->m_MonsterSoulTransformationList);
	LIST_CLEAR(MonsterSoulRewardList::iterator, this->m_MonsterSoulRewardList);
}

void MonsterSoul::LoadConverter()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Soul Converter...");

	MAP_CLEAR(MonsterSoulConverterMap::iterator, this->m_MonsterSoulConverterMap);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_soul_converter");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			MonsterSoulConverter * pData = new MonsterSoulConverter;

			pData->SetID(fields[0].GetUInt32());
			pData->SetItem(ITEMGET(fields[1].GetUInt8(), fields[2].GetUInt16()));
			pData->SetAmount(fields[3].GetInt32());

			this->m_MonsterSoulConverterMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster soul converter definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterSoul::LoadTransformation()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Soul Transformation...");

	LIST_CLEAR(MonsterSoulTransformationList::iterator, this->m_MonsterSoulTransformationList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_soul_transformation");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			MonsterSoulTransformation * pData = new MonsterSoulTransformation;

			pData->SetType(fields[0].GetUInt32());
			pData->SetID(fields[1].GetUInt32());
			pData->SetWorld(fields[2].GetUInt16());
			pData->SetMonster(fields[3].GetUInt16());
			pData->SetRequiredSoul(fields[4].GetInt32());
			pData->SetDropRate(fields[5].GetInt32());
			pData->SetLocation(fields[6].GetString());

			this->m_MonsterSoulTransformationList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster soul transformation definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterSoul::LoadReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Soul Reward...");

	LIST_CLEAR(MonsterSoulRewardList::iterator, this->m_MonsterSoulRewardList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_soul_reward");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			MonsterSoulReward * pData = new MonsterSoulReward;

			pData->SetID(fields[0].GetUInt32());
			pData->SetSubID(fields[1].GetUInt8());
			pData->SetItemBag(fields[2].GetString());
			pData->SetRandomItemBag(fields[3].GetString());

			this->m_MonsterSoulRewardList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster soul reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

MonsterSoulConverter const* MonsterSoul::GetConverter(uint32 id) const
{
	MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.find(id);

	if (itr != this->m_MonsterSoulConverterMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

MonsterSoulConverter const* MonsterSoul::GetConverterByItem(uint16 id) const
{
	for (MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.begin(); itr != this->m_MonsterSoulConverterMap.end(); ++itr)
	{
		MonsterSoulConverter const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetItem() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

MonsterSoulTransformation const* MonsterSoul::GetTransformation(uint32 type, uint32 id) const
{
	for (MonsterSoulTransformationList::const_iterator itr = this->m_MonsterSoulTransformationList.begin(); itr != this->m_MonsterSoulTransformationList.end(); ++itr)
	{
		MonsterSoulTransformation const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

MonsterSoulReward const* MonsterSoul::GetReward(uint32 id, uint8 sub_id) const
{
	for (MonsterSoulRewardList::const_iterator itr = this->m_MonsterSoulRewardList.begin(); itr != this->m_MonsterSoulRewardList.end(); ++itr)
	{
		MonsterSoulReward const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetID() == id && pData->GetSubID() == sub_id)
		{
			return pData;
		}
	}

	return nullptr;
}

bool MonsterSoul::IsSoulConverter(Item const* pItem) const
{
	if (!pItem)
	{
		return false;
	}

	return this->IsSoulConverter(pItem->GetItem());
}

bool MonsterSoul::IsSoulConverter(uint16 item) const
{
	return this->GetConverterByItem(item) != nullptr;
}

bool MonsterSoul::IsPurchaseAllowed(Player* pPlayer, Item const* pItem)
{
	if (!this->IsSoulConverter(pItem))
	{
		return true;
	}

	return !use_inventory_range(pPlayer->ItemFind(pItem->GetItem(), 0));
}

void MonsterSoul::ExpireSoulConverter(Player* pPlayer, Item const* pItem)
{
	MonsterSoulConverter const* pConverter = this->GetConverterByItem(pItem->GetItem());

	if (!pConverter)
	{
		return;
	}

	pPlayer->RemoveMonsterSoulData(pConverter->GetID());

	this->SendStatus(pPlayer);
}

void MonsterSoul::SendItemMove(Player* pPlayer, uint32 source, uint32 target)
{
	MONSTER_SOUL_ITEM_MOVE pMsg;
	pMsg.old_slot = source;
	pMsg.new_slot = target;

	pPlayer->SendPacket(&pMsg);
}

void MonsterSoul::SendPurchaseAvailable(Player* pPlayer)
{
	MONSTER_SOUL_RUUD_NPC pMsg;
	pMsg.result = 1;

	pPlayer->SendPacket(&pMsg);

	MONSTER_SOUL_AVAILABLE_PURCHASE pMsg2;
	pMsg2.amount = sGameServer->GetMonsterSoulPurchaseAmount();

	pPlayer->SendPacket(&pMsg2);
}

void MonsterSoul::SendPurchaseList(Player* pPlayer)
{
	uint8 buffer[8192];
	int32 write_size = 0;

	MONSTER_SOUL_NPC_DATA_HEAD* head = (MONSTER_SOUL_NPC_DATA_HEAD*)&buffer[write_size];
	write_size += sizeof(MONSTER_SOUL_NPC_DATA_HEAD);

	head->junk = 0;
	head->count = 0;

	for (MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.begin(); itr != this->m_MonsterSoulConverterMap.end(); ++itr)
	{
		MonsterSoulConverter const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		MONSTER_SOUL_NPC_DATA_BODY_1* body = (MONSTER_SOUL_NPC_DATA_BODY_1*)&buffer[write_size];
		write_size += sizeof(MONSTER_SOUL_NPC_DATA_BODY_1);

		body->type = pData->GetID();
		body->data[0] = 0;
		body->data[1] = 0;
		body->expire_date = time(nullptr) + sGameServer->GetMonsterSoulDurationTime();
		body->junk1 = 0;
		body->amount = 0;
		body->status = 0;
		body->slot = 0;

		uint8 slot = pPlayer->ItemFind(pData->GetItem(), 0);
		bool finished = true;

		if (use_inventory_range(slot))
		{
			body->slot = slot;
			body->expire_date = pPlayer->GetInventory()->GetItem(slot)->GetExpireDate();
			
			MonsterSoulData* pPlayerData = pPlayer->GetMonsterSoulData(pData->GetID());

			if (pPlayerData)
			{
				for (MonsterSoulAmountDataMap::const_iterator it = pPlayerData->m_AmountMap.begin(); it != pPlayerData->m_AmountMap.end(); ++it)
				{
					MonsterSoulTransformation const* pMonsterData = this->GetTransformation(pData->GetID(), it->first);

					if (pMonsterData)
					{
						MONSTER_SOUL_NPC_DATA_BODY_2* data = (MONSTER_SOUL_NPC_DATA_BODY_2*)&buffer[write_size];
						write_size += sizeof(MONSTER_SOUL_NPC_DATA_BODY_2);

						data->id = it->first;
						data->amount = it->second;

						if (data->amount >= pMonsterData->GetRequiredSoul())
						{
							data->amount = pMonsterData->GetRequiredSoul();
						}
						else
						{
							finished = false;
						}

						++body->amount;
					}
				}
			}
		}

		if (body->amount > 0 && finished)
		{
			body->status = 1;
		}

		++head->count;
	}

	head->Set(0x4D, 0x40, write_size);

	pPlayer->sendPacket(buffer);
}

void MonsterSoul::SoulConverterPurchase(Player* pPlayer, Item * pItem)
{
	MonsterSoulConverter const* pConverter = this->GetConverterByItem(pItem->GetItem());

	if (!pConverter)
	{
		return;
	}

	pPlayer->RemoveMonsterSoulData(pConverter->GetID());

	MonsterSoulData* pData = new MonsterSoulData;
	pData->SetType(pConverter->GetID());

	pPlayer->m_MonsterSoulDataMap[pConverter->GetID()] = pData;

	for (int32 i = 0; i < pConverter->GetAmount(); ++i)
	{
		RandomValue<uint32> m_RandomValue(0);

		for (MonsterSoulTransformationList::const_iterator itr = this->m_MonsterSoulTransformationList.begin(); itr != this->m_MonsterSoulTransformationList.end(); ++itr)
		{
			MonsterSoulTransformation const* pMonsterData = *itr;

			if (!pMonsterData)
			{
				continue;
			}

			if (pMonsterData->GetType() != pConverter->GetID())
			{
				continue;
			}

			if (pData->m_AmountMap.find(pMonsterData->GetID()) != pData->m_AmountMap.end())
			{
				continue;
			}

			m_RandomValue.AddValue(pMonsterData->GetID(), 0);
		}

		pData->m_AmountMap[m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM)] = 0;
	}

	this->SendStatus(pPlayer);
}

void MonsterSoul::SendStatus(Player* pPlayer)
{
	uint8 buffer[8192];
	int32 write_size = 0;

	MONSTER_SOUL_QUEST_HEAD* head = (MONSTER_SOUL_QUEST_HEAD*)&buffer[write_size];
	write_size += sizeof(MONSTER_SOUL_QUEST_HEAD);

	head->junk = 0;
	head->count = 0;

	for (MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.begin(); itr != this->m_MonsterSoulConverterMap.end(); ++itr)
	{
		MonsterSoulConverter const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		uint8 slot = pPlayer->ItemFind(pData->GetItem(), 0);

		if (!use_inventory_range(slot))
		{
			pPlayer->RemoveMonsterSoulData(pData->GetID());
			continue;
		}

		MonsterSoulData* pPlayerData = pPlayer->GetMonsterSoulData(pData->GetID());

		if (!pPlayerData)
		{
			pPlayer->ItemDelete(pData->GetItem(), 0, use_inventory_size);
			continue;
		}

		MONSTER_SOUL_QUEST_BODY_1* body = (MONSTER_SOUL_QUEST_BODY_1*)&buffer[write_size];
		write_size += sizeof(MONSTER_SOUL_QUEST_BODY_1);

		body->type = pData->GetID();
		body->data[0] = 0;
		body->data[1] = 0;
		body->expire_date = pPlayer->GetInventory()->GetItem(slot)->GetExpireDate();
		body->junk1 = 0;
		body->amount = 0;
		body->status = 0;
		body->slot = slot;

		bool finished = true;

		for (MonsterSoulAmountDataMap::const_iterator it = pPlayerData->m_AmountMap.begin(); it != pPlayerData->m_AmountMap.end(); ++it)
		{
			MonsterSoulTransformation const* pMonsterData = this->GetTransformation(pData->GetID(), it->first);

			if (pMonsterData)
			{
				MONSTER_SOUL_QUEST_BODY_2* data = (MONSTER_SOUL_QUEST_BODY_2*)&buffer[write_size];
				write_size += sizeof(MONSTER_SOUL_QUEST_BODY_2);

				data->id = it->first;
				data->amount = it->second;

				if (data->amount >= pMonsterData->GetRequiredSoul())
				{
					data->amount = pMonsterData->GetRequiredSoul();
				}
				else
				{
					finished = false;
				}

				++body->amount;
			}
		}

		if (finished)
		{
			body->status = 1;
		}

		++head->count;
	}

	head->Set(0x4D, 0x41, write_size);

	pPlayer->sendPacket(buffer);
}

void MonsterSoul::MonsterKill(Player* pPlayer, Monster* pMonster)
{
	if (!pPlayer || !pMonster)
	{
		return;
	}

	for (MonsterSoulDataMap::iterator itr = pPlayer->m_MonsterSoulDataMap.begin(); itr != pPlayer->m_MonsterSoulDataMap.end(); ++itr)
	{
		MonsterSoulData* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		for (MonsterSoulAmountDataMap::iterator it = pData->m_AmountMap.begin(); it != pData->m_AmountMap.end(); ++it)
		{
			MonsterSoulTransformation const* pMonsterData = this->GetTransformation(pData->GetType(), it->first);

			if (!pMonsterData)
			{
				continue;
			}

			if (it->second >= pMonsterData->GetRequiredSoul())
			{
				continue;
			}

			if ((pMonsterData->GetWorld() == pMonster->GetWorldId()) || (KUBERA_MAP_RANGE(pMonsterData->GetWorld()) && KUBERA_MAP_RANGE(pMonster->GetWorldId())))
			{
				if (pMonsterData->GetMonster() != pMonster->GetClass())
				{
					continue;
				}

				if (roll_chance_i(pMonsterData->GetDropRate(), 1000000))
				{
					it->second++;

					this->SendUpdateData(pPlayer, pMonsterData->GetType());

					this->ShareSoulToParty(pPlayer, pMonsterData);
				}
			}
		}
	}
}

void MonsterSoul::ShareSoulToParty(Player* pPlayer, MonsterSoulTransformation const* pMonsterData)
{
	if (!pPlayer || !pMonsterData)
	{
		return;
	}

	Unit* mMember[MAX_PARTY_MEMBERS];
	uint8 count = 0;

	pPlayer->GetPartyMembers(mMember, count, MAX_PARTY_MONSTER_KILL_DISTANCE);

	if (count > MAX_PARTY_MEMBERS)
	{
		count = MAX_PARTY_MEMBERS;
	}

	for (uint8 h = 0; h < count; ++h)
	{
		if (!mMember[h])
		{
			continue;
		}

		Player* pMember = mMember[h]->ToPlayer();

		if (!pMember)
		{
			continue;
		}

		if (pPlayer == pMember)
		{
			continue;
		}

		MonsterSoulData* pData = pMember->GetMonsterSoulData(pMonsterData->GetType());

		if (!pData)
		{
			continue;
		}

		MonsterSoulAmountDataMap::iterator itr = pData->m_AmountMap.find(pMonsterData->GetID());

		if (itr != pData->m_AmountMap.end())
		{
			itr->second++;

			this->SendUpdateData(pMember, pMonsterData->GetType());
		}
	}
}

void MonsterSoul::SendUpdateData(Player* pPlayer, uint32 type)
{
	if (!pPlayer)
	{
		return;
	}

	MonsterSoulConverter const* pConverter = this->GetConverter(type);

	if (!pConverter)
	{
		return;
	}

	MonsterSoulData const* pData = pPlayer->GetMonsterSoulData(type);

	if (!pData)
	{
		return;
	}

	uint8 slot = pPlayer->ItemFind(pConverter->GetItem(), 0);

	if (!use_inventory_range(slot))
	{
		return;
	}

	uint8 buffer[1024];
	POINTER_PCT(MONSTER_SOUL_UPDATE_HEAD, head, buffer, 0);
	POINTER_PCT(MONSTER_SOUL_UPDATE_BODY, body, buffer, sizeof(MONSTER_SOUL_UPDATE_HEAD));
	head->type = type;
	head->data[0] = 0;
	head->data[1] = 0;
	head->expire_date = pPlayer->GetInventory()->GetItem(slot)->GetExpireDate();
	head->junk1 = 0;
	head->amount = 0;
	head->status = 0;
	head->slot = slot;

	bool finished = true;

	for (MonsterSoulAmountDataMap::const_iterator it = pData->m_AmountMap.begin(); it != pData->m_AmountMap.end(); ++it)
	{
		MonsterSoulTransformation const* pMonsterData = this->GetTransformation(type, it->first);

		if (pMonsterData)
		{
			body[head->amount].id = it->first;
			body[head->amount].amount = it->second;

			if (body[head->amount].amount >= pMonsterData->GetRequiredSoul())
			{
				body[head->amount].amount = pMonsterData->GetRequiredSoul();
			}
			else
			{
				finished = false;
			}

			++head->amount;
		}
	}

	if (finished)
	{
		head->status = 1;
	}

	head->Set(0x4D, 0x45, sizeof(MONSTER_SOUL_UPDATE_HEAD)+(head->amount * sizeof(MONSTER_SOUL_UPDATE_BODY)));

	pPlayer->sendPacket(buffer);
}

bool MonsterSoul::SoulConverterUse(Player* pPlayer, uint16 item, uint8 slot, uint8 type)
{
	if (!pPlayer)
	{
		return true;
	}

	MonsterSoulConverter const* pConverter = this->GetConverterByItem(item);

	if (!pConverter)
	{
		return false;
	}

	MonsterSoulData const* pData = pPlayer->GetMonsterSoulData(pConverter->GetID());

	if (!pData)
	{
		return true;
	}

	bool finished = true;

	for (MonsterSoulAmountDataMap::const_iterator itr = pData->m_AmountMap.begin(); itr != pData->m_AmountMap.end(); ++itr)
	{
		MonsterSoulTransformation const* pMonsterInfo = this->GetTransformation(pData->GetType(), itr->first);

		if (!pMonsterInfo)
		{
			continue;
		}

		if (itr->second < pMonsterInfo->GetRequiredSoul())
		{
			finished = false;
			break;
		}
	}

	if (!finished)
	{
		return true;
	}

	MonsterSoulReward const* pReward = this->GetReward(pData->GetType(), type);

	if (!pReward)
	{
		return true;
	}

	pPlayer->ClearItem(slot);
	pPlayer->RemoveMonsterSoulData(pData->GetType());

	Item new_item;
	sItemBagMgr->RunItemBag(pPlayer, pReward->GetItemBag(), new_item);

	if (sItemBagMgr->RunItemBag(pPlayer, pReward->GetRandomItemBag(), new_item))
	{
		MONSTER_SOUL_REWARD pMsg;
		pMsg.id = pConverter->GetID();
		pMsg.reward_type = type;
		pMsg.item = new_item.GetItem();
		pMsg.amount = 1;

		item_template const* pItemInfo = sItemMgr->GetItem(new_item.GetItem());

		if (pItemInfo && pItemInfo->GetStackData()->GetMaxStack() > 0)
		{
			pMsg.amount = new_item.GetDurability();
		}
				
		pMsg.data = 0;

		pPlayer->SendPacket(&pMsg);
	}

	return true;
}