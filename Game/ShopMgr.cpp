/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ShopMgr.cpp"
*
*/

ShopMgr::ShopMgr()
{
	this->SetUpdateDay(-1);
}

ShopMgr::~ShopMgr()
{
	MAP_CLEAR(ShopMap::iterator, this->shop_map);
	this->buy_count_character.clear();
	this->buy_count_account.clear();
	this->buy_count_pc.clear();
	this->buy_count_server.clear();
}

void ShopMgr::LoadShopTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Shop Template...");

	MAP_CLEAR(ShopMap::iterator, this->shop_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM shop_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			Shop * add_shop = new Shop;
			
			add_shop->SetID(fields[loop++].GetUInt8());
			add_shop->SetName(fields[loop++].GetString());
			add_shop->SetMaxPKLevel(fields[loop++].GetUInt8());
			add_shop->SetPKText(fields[loop++].GetString());
			add_shop->SetFlag(fields[loop++].GetUInt8());
			add_shop->SetType(fields[loop++].GetUInt8());
			add_shop->SetMaxBuyCount(fields[loop++].GetInt32());
			add_shop->SetMaxBuyType(fields[loop++].GetUInt8());

			add_shop->SetItemCount(0);
			
			this->shop_map[add_shop->GetID()] = add_shop;

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u shop definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void ShopMgr::LoadShopItems()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Shop Items...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM shop_items ORDER BY shop ASC, id ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;
						
			uint8 id = fields[loop++].GetUInt8();

			Shop * shop = this->GetShop(id);

			if ( !shop )
			{
				continue;
			}

			uint8 type = fields[loop++].GetUInt8();
			uint16 index = fields[loop++].GetUInt16();
			ItemShop additem;
			additem.clear();
			additem.SetItem(ITEMGET(type, index));
			additem.SetLevel(fields[loop++].GetUInt8());
			additem.SetDurability(static_cast<float>(fields[loop++].GetUInt8()));
			additem.SetSkill(fields[loop++].GetUInt8());
			additem.SetLuck(fields[loop++].GetUInt8());
			additem.SetOption(fields[loop++].GetUInt8());
			additem.SetExe(fields[loop++].GetUInt8());
			additem.SetAncient(fields[loop++].GetUInt8());

			if ( !sItemMgr->IsItem(additem.GetItem()) )
			{
				sLog->outError(LOG_DEFAULT, "LoadShopItems() Shop: %u, item: %d", 
					id, additem.GetItem());

				continue;
			}

			for ( uint8 i = 0; i < MAX_SOCKET_SLOT; ++i )
			{
				additem.SetSocket(i, fields[loop++].GetUInt16());
			}

			additem.SetPrice(fields[loop++].GetUInt32());

			shop->insert_item(additem);
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u shop items", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

Shop * ShopMgr::GetShop(uint8 id)
{
	ShopMap::iterator it = this->shop_map.find(id);

	if ( it != this->shop_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

Shop const* ShopMgr::GetShop(uint8 id) const
{
	ShopMap::const_iterator it = this->shop_map.find(id);

	if ( it != this->shop_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

Shop const* ShopMgr::GetShop(std::string const& name) const
{
	for ( ShopMap::const_iterator it = this->shop_map.begin(); it != this->shop_map.end(); ++it )
	{
		if ( it->second->GetName() != name )
		{
			continue;
		}

		return it->second;
	}

	return nullptr;
}

bool ShopMgr::IsShop(std::string const& name) const
{
	return this->GetShop(name) != nullptr;
}

bool ShopMgr::EnableToBuy(Player* pPlayer, Shop const* pShop)
{
	if ( !pPlayer || !pShop )
	{
		return false;
	}

	int32 count = 0;

	switch ( pShop->GetMaxBuyType() )
	{
	case SHOP_COUNT_NONE:
		{
			return true;
		} break;

	case SHOP_COUNT_CHARACTER:
		{
			ShopBuyCountMap & data_map = this->buy_count_character[pShop->GetID()];

			ShopBuyCountMap::iterator it = data_map.find(pPlayer->GetGUID());

			if ( it != data_map.end() )
			{
				count = it->second;
			}
		} break;

	case SHOP_COUNT_ACCOUNT:
		{
			ShopBuyCountMap & data_map = this->buy_count_account[pShop->GetID()];

			ShopBuyCountMap::iterator it = data_map.find(pPlayer->GetAccountData()->GetGUID());

			if ( it != data_map.end() )
			{
				count = it->second;
			}
		} break;

	case SHOP_COUNT_PC:
		{
			ShopBuyCountMap & data_map = this->buy_count_pc[pShop->GetID()];

			ShopBuyCountMap::iterator it = data_map.find(pPlayer->GetAccountData()->GetDiskSerial());

			if ( it != data_map.end() )
			{
				count = it->second;
			}
		} break;

	case SHOP_COUNT_SERVER:
		{
			ShopBuyCountMap & data_map = this->buy_count_server[pShop->GetID()];

			ShopBuyCountMap::iterator it = data_map.find(sGameServer->GetServerCode());

			if ( it != data_map.end() )
			{
				count = it->second;
			}
		} break;

	default:
		{
			return true;
		} break;
	}

	if ( count >= pShop->GetMaxBuyCount() )
	{
		pPlayer->SendMessageBox(0, "Error", "Reached maximum day buy count.");

		return false;
	}

	++count;

	switch ( pShop->GetMaxBuyType() )
	{
	case SHOP_COUNT_CHARACTER:
		{
			ShopBuyCountMap & data_map = this->buy_count_character[pShop->GetID()];

			data_map[pPlayer->GetGUID()] = count;
		} break;

	case SHOP_COUNT_ACCOUNT:
		{
			ShopBuyCountMap & data_map = this->buy_count_account[pShop->GetID()];

			data_map[pPlayer->GetAccountData()->GetGUID()] = count;
		} break;

	case SHOP_COUNT_PC:
		{
			ShopBuyCountMap & data_map = this->buy_count_pc[pShop->GetID()];

			data_map[pPlayer->GetAccountData()->GetDiskSerial()] = count;
		} break;

	case SHOP_COUNT_SERVER:
		{
			ShopBuyCountMap & data_map = this->buy_count_server[pShop->GetID()];

			data_map[sGameServer->GetServerCode()] = count;
		} break;
	}

	return true;
}

void ShopMgr::Update()
{
	Custom::SystemTimer time = Custom::SystemTimer();

	if ( time.GetDay() != this->GetUpdateDay() )
	{
		this->buy_count_character.clear();
		this->buy_count_account.clear();
		this->buy_count_pc.clear();
		this->buy_count_server.clear();

		this->SetUpdateDay(time.GetDay());
	}
}

Shop::Shop()
{
	for ( int32 i = 0; i < max_shop_item; ++i )
	{
		this->GetItem(i)->clear();
		this->GetItem(i)->position.set(-1);
		this->shop_map[i].set(0);
	}

	this->SetMaxBuyCount(0);
	this->SetMaxBuyType(SHOP_COUNT_NONE);
}

void Shop::insert_item(ItemShop item)
{
	item_template const* item_info = sItemMgr->GetItem(item.GetItem());

	if ( !item_info )
		return;

	if ( this->GetType() != SHOP_TYPE_RUUD )
	{
		uint8 blank = 0xFF;

		for ( uint8 Y = 0; Y < 15; ++Y )
		{
			for ( uint8 X = 0; X < 8; ++X )
			{
				if ( this->shop_map[X + Y * 8] == 0 )
				{
					blank = this->map_check(X, Y, item_info->GetX(), item_info->GetY());

					if ( blank != 0xFF )
					{
						goto SkipLoop;
					}
				}
			}
		}

		if ( blank == 0xFF )
		{
			return;
		}

SkipLoop:
		item.CalculateDurability();
		item.Convert();

		item.position.set(blank);
		this->SetItem(blank, item);
		this->IncreaseItemCount(1);
	}
	else
	{
		item.CalculateDurability();
		item.Convert();

		item.position.set(this->GetItemCount());
		this->SetItem(this->GetItemCount(), item);
		this->IncreaseItemCount(1);
	}
}

uint8 Shop::map_check(uint8 X, uint8 Y, uint8 W, uint8 H)
{
	if ( (X + W) > 8 )
		return 0xFF;

	if ( (Y + H) > 15 )
		return 0xFF;

	for( uint8 YY = 0; YY < H; ++YY )
	{
		for( uint8 XX = 0; XX < W; ++XX )
		{
			if ( this->shop_map[(Y + YY) * 8 + (X + XX)].Is(1) )
			{
				return 0xFF;
			}
		}
	}

	for( uint8 YY = 0; YY < H; ++YY )
	{
		for( uint8 XX = 0; XX < W; ++XX )
		{
			this->shop_map[(Y + YY) * 8 + (X + XX)].set(1);
		}
	}

	return (X + Y * 8);
}