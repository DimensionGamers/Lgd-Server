/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ItemBag.cpp"
*
*/

ItemBag::ItemBag()
{

}
		
ItemBag::~ItemBag()
{
	this->Clear();
}

void ItemBag::Clear()
{
	for ( ItemBagItemGroup::iterator it = this->items.begin(); it != this->items.end(); ++it )
	{
		LIST_CLEAR(ItemBagItemList::iterator, it->second);
	}

	this->items.clear();
}

bool ItemBag::Execute(Unit* pUnit, int32 data_1, int32 data_2, int16 x, int16 y, uint16 serial_server, uint32 serial, std::string const& item_name)
{
	if ( !this->Valid() )
		return false;

	Monster* pMonster = pUnit->ToCreature();
	Player* pPlayer = pUnit->ToPlayer();
	Item new_item;

	switch ( this->GetType() )
	{
	case ITEM_BAG_ITEM:
	case ITEM_BAG_GROUP_ITEM:
		{
			if ( pPlayer )
			{
				if ( !sGameServer->SerialCheck(pPlayer, serial_server, serial, item_name, 2) )
				{
					return true;
				}

				this->Drop(pPlayer, pPlayer->GetWorldId(), x, y, ELEMENTAL_ATTRIBUTE_NONE);

				SQLTransaction trans = MuLogDatabase.BeginTransaction();

				PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_QUERY_ITEM_BAG_SERIAL_INSERT);
				stmt->setUInt32(0, serial);
				stmt->setUInt16(1, serial_server);
				stmt->setString(2, item_name);
				stmt->setUInt8(3, this->item.level);
				stmt->setUInt32(4, pPlayer->GetAccountData()->GetGUID());
				stmt->setString(5, pPlayer->GetAccountData()->GetAccount());
				stmt->setUInt32(6, pPlayer->GetGUID());
				stmt->setString(7, pPlayer->GetName());
				stmt->setUInt16(8, pPlayer->GetWorldId());
				stmt->setInt16(9, x);
				stmt->setInt16(10, y);
				stmt->setInt32(11, pPlayer->GetInstance());
				trans->Append(stmt);

				MuLogDatabase.CommitTransaction(trans);
			}
		} break;

	case ITEM_BAG_MONSTER:
	case ITEM_BAG_MONSTER_RATE:
	case ITEM_BAG_EVENT:
		{
			if ( pMonster )
			{
				this->Drop(pMonster->GetMaxAttacker() ? pMonster->GetMaxAttacker()->ToUnit(): pMonster, pMonster->GetWorldId(), pMonster->GetX(), pMonster->GetY(), pMonster->GetElementalAttribute());
			}
		} break;

	case ITEM_BAG_MONSTER_ALL:
		{
			if (pMonster)
			{
				World* pWorld = pMonster->GetWorld();
				if (pWorld)
				{
					WorldPlayerMap const& players = pWorld->GetAllPlayers();
					for (auto const& itr : players)
					{
						Player* pPl = itr.second;

						if (!pPl || !pPl->IsPlaying())
						{
							continue;
						}

						if (!IN_RANGE(pMonster, pPl, 20))
						{
							continue;
						}

						this->Create(pPl, pPl->GetElementalAttribute());
					}
				}
			}
		} break;

	case ITEM_BAG_MISC:
		{
			if ( pPlayer )
			{
				switch(this->misc.create_type)
				{
				case ITEM_BAG_CREATE_TYPE_FLOOR:
					{
						if (this->BuildItem(this->GetRandomItem(pPlayer), new_item, pPlayer, pPlayer->GetX(), pPlayer->GetY(), ELEMENTAL_ATTRIBUTE_NONE))
						{
							this->Drop(pPlayer, pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), ELEMENTAL_ATTRIBUTE_NONE);
						}
					} break;

				case ITEM_BAG_CREATE_TYPE_INVENTORY:
					{
						if (this->BuildItem(this->GetRandomItem(pPlayer), new_item, pPlayer, pPlayer->GetX(), pPlayer->GetY(), ELEMENTAL_ATTRIBUTE_NONE))
						{
							sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, new_item);
						}
					} break;

				case ITEM_BAG_CREATE_TYPE_MIX:
					{
						if (this->BuildItem(this->GetRandomItem(pPlayer), new_item, pPlayer, pPlayer->GetX(), pPlayer->GetY(), ELEMENTAL_ATTRIBUTE_NONE))
						{
							sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_chaos_box, new_item, 1);
						}
					} break;

				case ITEM_BAG_CREATE_TYPE_EVENT_REWARD:
					{
						if (this->BuildItem(this->GetRandomItem(pPlayer), new_item, pPlayer, pPlayer->GetX(), pPlayer->GetY(), ELEMENTAL_ATTRIBUTE_NONE))
						{
							sItemMgr->ItemSerialCreate(pPlayer, pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), new_item, false, true, true);
						}
					} break;

				case ITEM_BAG_CREATE_TYPE_GREMORY_CASE:
					{
						this->Create(pPlayer, pPlayer->GetElementalAttribute());
					} break;
				}
			}
		} break;

	case ITEM_BAG_MIX:
		{
			if ( pPlayer )
			{
				if (this->BuildItem(this->GetRandomItem(pPlayer), new_item, pPlayer, pPlayer->GetX(), pPlayer->GetY(), ELEMENTAL_ATTRIBUTE_NONE))
				{
					sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_chaos_box, new_item, 1);
				}
			}
		} break;

	case ITEM_BAG_WORLD:
	case ITEM_BAG_WORLD_2:
	case ITEM_BAG_ZONE:
		{
			if ( pMonster )
			{
				this->Drop(pMonster->GetMaxAttacker() ? pMonster->GetMaxAttacker()->ToUnit(): pMonster, pMonster->GetWorldId(), pMonster->GetX(), pMonster->GetY(), pMonster->GetElementalAttribute());
			}
		} break;

	case ITEM_BAG_NPC_TALK:
		{
		} break;

	case ITEM_BAG_INVENTORY_ITEM:
		{
			if ( pPlayer )
			{
				if ( !sGameServer->SerialCheck(pPlayer, serial_server, serial, item_name, 3) )
				{
					return true;
				}

				if ( this->BuildCurreny(pPlayer, pPlayer->GetX(), pPlayer->GetY()) )
				{
					SQLTransaction trans = MuLogDatabase.BeginTransaction();

					PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_QUERY_ITEM_BAG_SERIAL_INSERT);
					stmt->setUInt32(serial);
					stmt->setUInt16(serial_server);
					stmt->setString(item_name);
					stmt->setUInt8(this->item.level);
					stmt->setUInt32(pPlayer->GetAccountData()->GetGUID());
					stmt->setString(pPlayer->GetAccountData()->GetAccount());
					stmt->setUInt32(pPlayer->GetGUID());
					stmt->setString(pPlayer->GetName());
					stmt->setUInt16(pPlayer->GetWorldId());
					stmt->setInt16(pPlayer->GetX());
					stmt->setInt16(pPlayer->GetY());
					stmt->setInt32(pPlayer->GetInstance());
					trans->Append(stmt);

					MuLogDatabase.CommitTransaction(trans);
				}
				else if (this->BuildItem(this->GetRandomItem(pPlayer), new_item, pPlayer, pPlayer->GetX(), pPlayer->GetY(), ELEMENTAL_ATTRIBUTE_NONE))
				{
					sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory_box, new_item);

					SQLTransaction trans = MuLogDatabase.BeginTransaction();

					PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_QUERY_ITEM_BAG_SERIAL_INSERT);
					stmt->setUInt32(serial);
					stmt->setUInt16(serial_server);
					stmt->setString(item_name);
					stmt->setUInt8(this->item.level);
					stmt->setUInt32(pPlayer->GetAccountData()->GetGUID());
					stmt->setString(pPlayer->GetAccountData()->GetAccount());
					stmt->setUInt32(pPlayer->GetGUID());
					stmt->setString(pPlayer->GetName());
					stmt->setUInt16(pPlayer->GetWorldId());
					stmt->setInt16(pPlayer->GetX());
					stmt->setInt16(pPlayer->GetY());
					stmt->setInt32(pPlayer->GetInstance());
					trans->Append(stmt);

					MuLogDatabase.CommitTransaction(trans);
				}
			}
		} break;
	}

	return true;
}

void ItemBag::Drop(Unit* mOwner, uint16 world, int16 x, int16 y, uint8 attribute)
{
	if (this->IsFlag(ITEM_BAG_FLAG_FIREWORKS))
	{
		mOwner->FireworksSend();
	}

	int32 drop_count = this->GetDropCount();
	int16 r_x = x;
	int16 r_y = y;
	Item item;

	World* pWorld = sWorldMgr->GetWorld(world);
	Player* pPlayer = mOwner->ToPlayer();

	if ( !pWorld )
	{
		return;
	}

	if ( this->GetType() == ITEM_BAG_GROUP_ITEM || this->GetType() == ITEM_BAG_GROUP_MONSTER )
	{
		if ( roll_chance_i(this->GetRuudRate(), ITEM_BAG_RATE) )
		{
			if ( pPlayer )
			{
				uint32 add_ruud = this->GetRandomRuud();
				pPlayer->IncreaseRuudMoney(add_ruud);
				pPlayer->SendRuudMoney(add_ruud);
			}
		}
		else if (roll_chance_i(this->GetMoneyRate(), ITEM_BAG_RATE))
		{
			pWorld->AddZen(mOwner, r_x, r_y, this->GetRandomMoney());
		}

		ItemBagItemGroup::const_iterator itr = this->items.find(this->GetRandomGroup());

		if ( itr == this->items.end() )
		{
			return;
		}
		
		for (auto const& pData : itr->second)
		{
			if (this->BuildItem(pData, item, mOwner, r_x, r_y, attribute))
			{
				sItemMgr->ItemSerialCreate(mOwner, pWorld->GetEntry(), r_x, r_y, item, false, true, true);

				if ( this->GetType() == ITEM_BAG_GROUP_MONSTER )
				{
					if ( !pWorld->GetRandomDropLocation(x, y, r_x, r_y, this->range_x.get(), this->range_y.get(), 10) )
					{
						r_x = x;
						r_y = y;
					}

					r_x = r_x & 0xFF;
					r_y = r_y & 0xFF;
				}
			}
		}
	}
	else
	{
		do
		{
			if (roll_chance_i(this->GetRuudRate(), ITEM_BAG_RATE))
			{
				if ( pPlayer )
				{
					uint32 add_ruud = this->GetRandomRuud();
					pPlayer->IncreaseRuudMoney(add_ruud);
					pPlayer->SendRuudMoney(add_ruud);
				}
			}
			else if (roll_chance_i(this->GetMoneyRate(), ITEM_BAG_RATE))
			{
				if ( !pWorld->AddZen(mOwner, r_x, r_y, this->GetRandomMoney()) )
				{
					continue;
				}
			}
			else
			{
				if (!this->BuildItem(this->GetRandomItem(pPlayer), item, mOwner, r_x, r_y, attribute))
				{
					continue;
				}

				sItemMgr->ItemSerialCreate(mOwner, pWorld->GetEntry(), r_x, r_y, item, false, true, true);
			}

			if ( !pWorld->GetRandomDropLocation(x, y, r_x, r_y, this->range_x.get(), this->range_y.get(), 10) )
			{
				r_x = x;
				r_y = y;
			}

			r_x = r_x & 0xFF;
			r_y = r_y & 0xFF;

			drop_count--;
		}
		while(drop_count > 0);
	}
}

void ItemBag::Create(Player* pPlayer, uint8 attribute)
{
	if (this->IsFlag(ITEM_BAG_FLAG_FIREWORKS))
	{
		pPlayer->FireworksSend();
	}

	int32 drop_count = this->GetDropCount();
	Item item;

	do
	{
		if (roll_chance_i(this->GetRuudRate(), ITEM_BAG_RATE))
		{
			uint32 add_ruud = this->GetRandomRuud();
			pPlayer->IncreaseRuudMoney(add_ruud);
			pPlayer->SendRuudMoney(add_ruud);
		}
		else if (roll_chance_i(this->GetMoneyRate(), ITEM_BAG_RATE))
		{
			pPlayer->MoneyAdd(this->GetRandomMoney());
		}
		else
		{
			if (!this->BuildItem(this->GetRandomItem(pPlayer), item, pPlayer, pPlayer->GetX(), pPlayer->GetY(), attribute))
			{
				continue;
			}

			if (this->GetType() != ITEM_BAG_MONSTER_ALL)
			{
				item.raw.data_extra[2] = this->misc.sub_id == -1 ? 0 : this->misc.sub_id;
			}

			if (this->GetType() != ITEM_BAG_MONSTER_ALL)
			{
				sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
			}
			else
			{
				sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, item);
			}
		}

		drop_count--;
	}
	while(drop_count > 0);
}

ItemBagItem* ItemBag::GetRandomItem(Player* pPlayer) const
{
	ItemBagItemGroup::const_iterator itr = this->items.find(this->GetRandomGroup());

	if (itr == this->items.end())
	{
		return nullptr;
	}

	if (itr->second.empty())
	{
		return nullptr;
	}

	RandomValue<ItemBagItem*> m_RandomValue(nullptr);
	RandomValue<ItemBagItem*> m_RandomClassValue(nullptr);

	if (!this->IsFlag(ITEM_BAG_FLAG_WEIGHT))
	{
		m_RandomValue.SetTotalWeight(ITEM_BAG_RATE);
		m_RandomClassValue.SetTotalWeight(ITEM_BAG_RATE);
	}

	for (auto const& pItemBagItem : itr->second)
	{
		if (!pItemBagItem)
		{
			continue;
		}

		if (this->IsFlag(ITEM_BAG_FLAG_CLASS) && this->IsItemClass(pPlayer, pItemBagItem))
		{
			m_RandomClassValue.AddValue(pItemBagItem, pItemBagItem->GetRate());
		}

		m_RandomValue.AddValue(pItemBagItem, pItemBagItem->GetRate());
	}

	ItemBagItem* pResult = nullptr;
	RandomPoolType pool_type = this->IsFlag(ITEM_BAG_FLAG_WEIGHT) ? RANDOM_POOL_WEIGHT : RANDOM_POOL_RATE;

	if (this->IsFlag(ITEM_BAG_FLAG_CLASS))
	{
		pResult = m_RandomClassValue.GetRandomValue(pool_type);
	}

	if (!pResult)
	{
		pResult = m_RandomValue.GetRandomValue(pool_type);
	}

	return pResult;
}

bool ItemBag::IsItemClass(Player* pPlayer, ItemBagItem const* pItemBagItem) const
{
	if (!pPlayer || !pItemBagItem)
	{
		return false;
	}

	uint16 item = pItemBagItem->GetItem();

	switch (pItemBagItem->GetSort())
	{
	case ITEM_BAG_SORT_ITEM:
	{
		item_template const* pItemInfo = sItemMgr->GetItem(item);

		if (!pItemInfo)
		{
			return false;
		}

		if (pItemInfo->GetSlot() == uint8(-1) || pItemInfo->GetRequiredClass(pPlayer->GetClass()) != 0)
		{
			return true;
		}
	} break;
	}

	return false;
}

uint32 ItemBag::GetRandomGroup() const
{
	RandomValue<uint32> m_RandomValue(0);

	for (auto const& itr : this->items)
	{
		if (!itr.second.empty())
		{
			m_RandomValue.AddValue(itr.first, 0);
		}
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

bool ItemBag::BuildItem(ItemBagItem const* RandomItem, Item & new_item, Unit* pUnit, int16 x, int16 y, uint8 attribute)
{
	if ( !RandomItem )
	{
		return false;
	}

	uint16 item = RandomItem->GetItem();
	uint8 skill = roll_chance_i(RandomItem->GetSkillRate(), ITEM_BAG_RATE) ? 1 : 0;
	uint8 luck = roll_chance_i(RandomItem->GetLuckRate(), ITEM_BAG_RATE) ? 1 : 0;
	uint8 option = RandomItem->GetRandomOption();
	float durability = RandomItem->GetDurability();
	uint8 level = RandomItem->GetRandomLevel();
	uint8 exe = GetRandomExcOption(RandomItem->GetRandomExe());
	uint8 ancient = RandomItem->GetAncient() | (RandomItem->GetRandomAncientOption() << 2);
	INIT_SOCKET_DATA(socket);
	uint8 socket_bonus = uint8(-1);

	for ( uint8 i = 0; i < MAX_SOCKET_SLOT; ++i )
	{
		if (!roll_chance_i(RandomItem->GetSocketRate(i), ITEM_BAG_RATE))
			break;

		socket[i] = SOCKET_SLOT_EMPTY;
	}

	if (RandomItem->IsFlag(ITEM_BAG_ITEM_FLAG_FIXED_EXCELLENT))
	{
		exe = RandomItem->GetExeMin();
	}

	switch (RandomItem->GetSort())
	{
	case ITEM_BAG_SORT_SET:
		if (!sItemMgr->GetRandomSetPart(RandomItem->GetType(), item))
		{
			return false;
		}
	break;

	case ITEM_BAG_SORT_ANCIENT_SET:
		if (!sItemMgr->GetRandomAncientSetPart(RandomItem->GetType(), item, ancient))
		{
			return false;
		}

		ancient |= (RandomItem->GetRandomAncientOption() << 2);
		break;
	}

	auto pItemInfo = sItemMgr->GetItem(item);

	if (!pItemInfo)
	{
		return false;
	}

	if (item == MUUN_EVOLUTION_STONE)
	{
		uint16 muun = ITEMGET(RandomItem->GetSocketRate(0), RandomItem->GetSocketRate(1));

		socket[0] = HIBYTE(muun);
		socket[1] = muun % MAX_SUBTYPE_ITEMS;
	}

	if (pItemInfo->GetKind2() == ItemKind::ERRTEL)
	{
		socket[0] = sPentagramSystem->GetRandomErrtelOption(RandomItem->GetItem(), 1);
		socket[0] |= 16 * level;
		level = 0;
		socket_bonus = 16 | Random(ELEMENTAL_ATTRIBUTE_MAX);

		if (socket_bonus == 16)
		{
			socket_bonus |= ELEMENTAL_ATTRIBUTE_FIRE;
		}
	}

	if ( durability == 0 )
	{
		durability = sItemMgr->CalculateDurability(item, level, exe, ancient);
	}

	if ((sPentagramSystem->IsPentagramItem(item) || pItemInfo->GetKind2() == ItemKind::ERRTEL) && option > 0)
	{
		socket_bonus &= ~0x0F;
		socket_bonus |= option;

		if ( sPentagramSystem->IsPentagramItem(item) )
		{
			socket_bonus |= 16;
		}

		option = 0;
	}

	new_item = Item(item, level, durability, skill, luck, option, exe, ancient, socket, socket_bonus);

	if ( RandomItem->GetDuration() > 0 )
	{
		new_item.SetExpireDate(RandomItem->GetDuration() + time(nullptr));
	}

	if (RandomItem->IsFlag(ITEM_BAG_ITEM_FLAG_FIREWORKS))
	{
		sObjectMgr->FireworksSend(pUnit->GetWorldId(), pUnit->GetInstance(), x, y);
	}

	return true;
}

bool ItemBag::BuildCurreny(Player* pPlayer, int16 x, int16 y)
{
	if ( !pPlayer )
	{
		return false;
	}

	if (roll_chance_i(this->GetRuudRate(), ITEM_BAG_RATE))
	{
		uint32 ruud = this->GetRandomRuud();

		pPlayer->IncreaseRuudMoney(ruud);
		pPlayer->SendRuudMoney(ruud);
		pPlayer->OpenBoxResult(2, RUUD);
		return true;
	}

	if (roll_chance_i(this->GetMoneyRate(), ITEM_BAG_RATE))
	{
		pPlayer->MoneyAdd(this->GetRandomMoney());
		pPlayer->OpenBoxResult(2, ITEMGET(14, 15));
		return true;
	}

	return false;
}

ItemBagMgr::ItemBagMgr()
{

}

ItemBagMgr::~ItemBagMgr()
{
	this->Clear();
}

void ItemBagMgr::Clear()
{
	for ( ItemBagMap::iterator it = this->m_item_bag.begin(); it != this->m_item_bag.end(); ++it )
	{
		it->second->Clear();
		delete it->second;
	}

	for ( ItemBagListMap::iterator it = this->m_item_bag_list.begin(); it != this->m_item_bag_list.end(); ++it )
		it->second.clear();

	this->m_item_bag.clear();
	this->m_item_bag_list.clear();
}

void ItemBagMgr::LoadItemBagTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Bag Template...");

	this->Clear();

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM item_bag_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			ItemBag * item_bag = new ItemBag;
					
			item_bag->SetGUID(fields[loop++].GetUInt32());
			item_bag->SetType(fields[loop++].GetUInt8());
			item_bag->SetName(fields[loop++].GetString());
			item_bag->raw.data[0] = fields[loop++].GetInt32();
			item_bag->raw.data[1] = fields[loop++].GetInt32();
			item_bag->raw.data[2] = fields[loop++].GetInt32();
			item_bag->required_level.set(fields[loop++].GetInt16());
			item_bag->SetMoneyRate(fields[loop++].GetUInt16());
			item_bag->SetMoneyMin(fields[loop++].GetUInt32());
			item_bag->SetMoneyMax(fields[loop++].GetUInt32());
			item_bag->SetRuudRate(fields[loop++].GetUInt16());
			item_bag->SetRuudMin(fields[loop++].GetUInt32());
			item_bag->SetRuudMax(fields[loop++].GetUInt32());
			item_bag->drop_min.set(fields[loop++].GetUInt32());
			item_bag->drop_max.set(fields[loop++].GetUInt32());
			item_bag->SetFlag(fields[loop++].GetUInt8());

			if ( item_bag->GetType() < ITEM_BAG_MAX_TYPE )
			{
				if ( item_bag->GetType() == ITEM_BAG_MONSTER || 
					 item_bag->GetType() == ITEM_BAG_WORLD || 
					 item_bag->GetType() == ITEM_BAG_WORLD_2 || 
					 item_bag->GetType() == ITEM_BAG_MONSTER_RATE || 
					 item_bag->GetType() == ITEM_BAG_EVENT ||
					 item_bag->GetType() == ITEM_BAG_ZONE )
				{
					item_bag->range_x.set(4);
					item_bag->range_y.set(4);
				}
				else if ( item_bag->GetType() == ITEM_BAG_ITEM || 
						  item_bag->GetType() == ITEM_BAG_GROUP_ITEM )
				{
					item_bag->range_x.set(2);
					item_bag->range_y.set(2);
				}
				else
				{
					item_bag->range_x.set(0);
					item_bag->range_y.set(0);
				}

				this->m_item_bag[item_bag->GetGUID()] = item_bag;
				this->m_item_bag_list[item_bag->GetType()].push_back(item_bag);
			}
			else
			{
				delete item_bag;
				continue;
			}

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item bag definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void ItemBagMgr::LoadItemBagItems()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Bag Items...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM item_bag_items i WHERE i.rate > 0");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			ItemBagItem * item = new ItemBagItem;

			uint32 id = fields[loop++].GetUInt32();
			item->SetSort(fields[loop++].GetUInt8());
			item->SetType(fields[loop++].GetUInt8());
			item->SetIndex(fields[loop++].GetUInt16());

			if ( item->GetSort() == ITEM_BAG_SORT_ITEM )
			{
				if ( !sItemMgr->IsItem(item->GetItem()) )
				{
					sLog->outError(LOG_DEFAULT, "LoadItemBagItems() ItemBag: %u, item: %d %d", 
						id, item->GetType(), item->GetIndex());

					delete item;
					continue;
				}
			}

			item->SetLevelMin(fields[loop++].GetUInt8());
			item->SetLevelMax(fields[loop++].GetUInt8());
			item->SetDurability(fields[loop++].GetUInt8());
			item->SetSkillRate(fields[loop++].GetUInt16());
			item->SetLuckRate(fields[loop++].GetUInt16());
			item->SetOptionMin(fields[loop++].GetUInt8());
			item->SetOptionMax(fields[loop++].GetUInt8());
			item->SetExeMin(fields[loop++].GetUInt8());
			item->SetExeMax(fields[loop++].GetUInt8());
			item->SetAncient(fields[loop++].GetUInt8());
			item->SetAncientOptionMin(fields[loop++].GetUInt8());
			item->SetAncientOptionMax(fields[loop++].GetUInt8());

			for ( uint8 i = 0; i < MAX_SOCKET_SLOT; ++i )
			{
				item->SetSocketRate(i, fields[loop++].GetUInt16());
			}

			item->SetRate(fields[loop++].GetUInt32());
			item->SetFlag(fields[loop++].GetUInt8());
			item->SetGroup(fields[loop++].GetUInt32());
			item->SetDuration(fields[loop++].GetInt64());

			if ( item->GetRate() <= 0 )
			{
				delete item;
				continue;
			}

			if ( ItemBag * item_bag = this->GetItemBag(id) )
			{
				item_bag->items[item->GetGroup()].push_back(item);
			}
			else
			{
				delete item;
				continue;
			}

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item bag items", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

ItemBag * ItemBagMgr::GetItemBag(uint32 guid)
{
	ItemBagMap::const_iterator it = this->m_item_bag.find(guid);

	return (it != this->m_item_bag.end() ? it->second: nullptr);
}

ItemBag * ItemBagMgr::GetItemBag(std::string const& name) const
{
	if ( name.empty() )
	{
		return nullptr;
	}

	for ( ItemBagMap::const_iterator itr = this->m_item_bag.begin(); itr != this->m_item_bag.end(); ++itr )
	{
		ItemBag * pItemBag = itr->second;

		if ( !pItemBag )
		{
			continue;
		}

		if ( pItemBag->GetName() == name )
		{
			return pItemBag;
		}
	}

	return nullptr;
}

uint8 ItemBagMgr::ExecuteItemBag(ItemBagType type, Unit* pUnit, std::string const& name, int32 data_1, int32 data_2, int16 x, int16 y, uint16 serial_server, uint32 serial, std::string const& item_name)
{
	ItemBagListMap::const_iterator item_bag_list = this->m_item_bag_list.find(type);

	if ( item_bag_list == this->m_item_bag_list.end() )
	{
		return ITEM_BAG_RESULT_FAIL;
	}

	ItemBag* pItemBag = nullptr;
	Monster* pMonster = pUnit->ToCreature();
	Player* pPlayer = pUnit->ToPlayer();

	if ( !name.empty() )
	{
		for ( ItemBagList::const_iterator it = item_bag_list->second.begin(); it != item_bag_list->second.end(); ++it )
		{
			if ( (*it)->GetName() != name )
				continue;

			pItemBag = (*it);
			break;
		}
	}
	else
	{
		for ( ItemBagList::const_iterator it = item_bag_list->second.begin(); it != item_bag_list->second.end(); ++it )
		{
			switch ( type )
			{
			case ITEM_BAG_ITEM:
			case ITEM_BAG_GROUP_ITEM:
				{
					if ( !pPlayer )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->GetItem() != uint16(data_1) )
						continue;

					if ( (*it)->item.level != data_2 && (*it)->item.level != -1 )
						continue;

					if ( (*it)->required_level > pPlayer->GetLevel() )
					{
						return ITEM_BAG_RESULT_LOW_LEVEL;
					}

					pItemBag = (*it);
				} break;

			case ITEM_BAG_MONSTER:
				{
					if ( !pMonster )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->monster.monster != pMonster->GetClass() )
						continue;

					if ( (*it)->monster.level_min != -1 && pMonster->GetLevel() < (*it)->monster.level_min )
						continue;

					if ( (*it)->monster.level_max != -1 && pMonster->GetLevel() > (*it)->monster.level_max )
						continue;

					pItemBag = (*it);
				} break;

			case ITEM_BAG_MONSTER_ALL:
			{
				if (!pMonster)
				{
					return ITEM_BAG_RESULT_FAIL;
				}

				if ((*it)->monster_all.monster != pMonster->GetClass())
					continue;

				pItemBag = (*it);
			} break;

			case ITEM_BAG_MISC:
				{
					if ( !pPlayer )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->misc.guid == data_1 )
					{
						pItemBag = (*it);
					}
				} break;

			case ITEM_BAG_MIX:
				{
					if ( !pPlayer )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->mix.id == data_1 && (*it)->mix.index == data_2 )
					{
						pItemBag = (*it);
					}
				} break;

			case ITEM_BAG_WORLD:
				{
					if ( !pMonster )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->world.id != pMonster->GetWorldId() && (*it)->world.id != -1 )
						continue;

					if ( (*it)->world.level_min > pMonster->GetLevel() )
						continue;

					if (!roll_chance_i((*it)->world.rate, 10000))
						continue;

					pItemBag = (*it);
				} break;

			case ITEM_BAG_WORLD_2:
				{
					if ( !pMonster )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->world2.id != pMonster->GetWorldId() && (*it)->world2.id != -1 )
						continue;

					if (!roll_chance_i((*it)->world2.rate1, 10000))
						continue;

					if (!roll_chance_i((*it)->world2.rate2, 10000))
						continue;

					pItemBag = (*it);
				} break;

			case ITEM_BAG_NPC_TALK:
				{
				} break;

			case ITEM_BAG_INVENTORY_ITEM:
				{
					if ( !pPlayer )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->GetItem() != uint16(data_1) )
						continue;

					if ( (*it)->item.level != data_2 && (*it)->item.level != -1 )
						continue;

					if ( (*it)->required_level > pPlayer->GetLevel() )
					{
						pPlayer->SendMessageBox(1, "Warning", "This box can only be opened after you reach level: %d", (*it)->required_level);
						return ITEM_BAG_RESULT_LOW_LEVEL;
					}

					pItemBag = (*it);
				} break;

			case ITEM_BAG_MONSTER_RATE:
				{
					if ( !pMonster )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->monster_rate.monster != pMonster->GetClass() )
						continue;

					if ( (*it)->monster_rate.world != pMonster->GetWorldId() && (*it)->monster_rate.world != -1 )
						continue;

					if (!roll_chance_i((*it)->monster_rate.rate, 10000))
					{
						continue;
					}

					pItemBag = (*it);
				} break;

			case ITEM_BAG_EVENT:
				{
					if ( !pMonster )
					{
						return ITEM_BAG_RESULT_FAIL;
					}

					if ( (*it)->event.id != pMonster->GetEventId() )
						continue;

					if ( (*it)->event.ground != pMonster->GetEventGround() )
						continue;

					if (!roll_chance_i((*it)->event.rate, 10000))
					{
						continue;
					}

					pItemBag = (*it);
				} break;

			case ITEM_BAG_ZONE:
				{
					if ( (*it)->zone.id != data_1 )
					{
						continue;
					}

					if (!roll_chance_i((*it)->zone.rate, 10000))
					{
						continue;
					}

					pItemBag = (*it);
				} break;
			}

			if ( pItemBag )
				break;
		}
	}

	if ( !pItemBag )
	{
		return ITEM_BAG_RESULT_FAIL;
	}

	bool result = pItemBag->Execute(pUnit, data_1, data_2, x, y, serial_server, serial, item_name);

	return result ? ITEM_BAG_RESULT_SUCCESS: ITEM_BAG_RESULT_FAIL;
}

bool ItemBagMgr::RunItemBag(Player* pPlayer, std::string const& name, Item & item, bool gremory_case, uint8 gremory_case_type)
{
	ItemBag * pItemBag = sItemBagMgr->GetItemBag(name);

	if ( !pItemBag )
	{
		return false;
	}

	if (pItemBag->GetRuudRate() > 0 && roll_chance_i(pItemBag->GetRuudRate(), ITEM_BAG_RATE))
	{
		uint32 add_ruud = pItemBag->GetRandomRuud();
		pPlayer->IncreaseRuudMoney(add_ruud);
		pPlayer->SendRuudMoney(add_ruud);

		item.SetItem(RUUD);

		return true;
	}
	else if (pItemBag->GetMoneyRate() > 0 && roll_chance_i(pItemBag->GetMoneyRate(), ITEM_BAG_RATE))
	{
		pPlayer->MoneyAdd(pItemBag->GetRandomMoney());

		item.SetItem(ITEMGET(14, 15));

		return true;
	}

	if ( pItemBag->items.empty() )
	{
		return false;
	}

	ItemBagItem * pItemBagItem = pItemBag->GetRandomItem(false);

	if ( !pItemBagItem )
	{
		return false;
	}

	pItemBag->BuildItem(pItemBagItem, item, pPlayer, 0, 0, 0);

	if ( gremory_case )
	{
		sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item, GREMORY_CASE_TYPE_CHARACTER, gremory_case_type);
	}
	else
	{
		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, item);
	}

	return true;
}