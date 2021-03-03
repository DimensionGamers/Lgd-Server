/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ShopMgr.cpp"
*
*/

Shop::Shop()
{
	for (int32 i = 0; i < max_shop_item; ++i)
	{
		GetItem(i)->clear();
		GetItem(i)->position.set(-1);
		_shopMap[i] = 0;
	}

	SetMaxBuyCount(0);
	SetMaxBuyType(SHOP_COUNT_NONE);
}

void Shop::AddItem(ItemShop item)
{
	auto const item_info = sItemMgr->GetItem(item.GetItem());
	if (!item_info)
		return;

	if (GetType() != SHOP_TYPE_RUUD)
	{
		uint8 blank = 0xFF;

		for (uint8 Y = 0; Y < 15; ++Y)
		{
			for (uint8 X = 0; X < 8; ++X)
			{
				if (_shopMap[X + Y * 8] == 0)
				{
					blank = MapCheck(X, Y, item_info->GetX(), item_info->GetY());
					if (blank != 0xFF)
					{
						goto SkipLoop;
					}
				}
			}
		}

		if (blank == 0xFF)
			return;

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

uint8 Shop::MapCheck(uint8 X, uint8 Y, uint8 W, uint8 H)
{
	if ((X + W) > 8)
		return 0xFF;

	if ((Y + H) > 15)
		return 0xFF;

	for (uint8 YY = 0; YY < H; ++YY)
	{
		for (uint8 XX = 0; XX < W; ++XX)
		{
			if (_shopMap[(Y + YY) * 8 + (X + XX)] == 1)
				return 0xFF;
		}
	}

	for (uint8 YY = 0; YY < H; ++YY)
	{
		for (uint8 XX = 0; XX < W; ++XX)
		{
			_shopMap[(Y + YY) * 8 + (X + XX)] = 1;
		}
	}

	return (X + Y * 8);
}

ShopMgr::ShopMgr() : _updateDay(-1)
{
	
}

ShopMgr::~ShopMgr()
{
	CLEAR_MAP(_shops);
	_characterPurchases.clear();
	_accountPurchases.clear();
	_pcPurchases.clear();
	_serverPurchases.clear();
}

void ShopMgr::LoadShopTemplate()
{
	sLog->outLoad(true, "Loading Shop Template...");

	CLEAR_MAP(_shops);

	auto result = GameServerDatabase.Query("SELECT * FROM shop_template");
	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			Shop * add_shop = new Shop;

			add_shop->SetID(reader.GetUInt8());
			add_shop->SetName(reader.GetString());
			add_shop->SetMaxPKLevel(reader.GetUInt8());
			add_shop->SetPKText(reader.GetString());
			add_shop->SetFlag(reader.GetUInt8());
			add_shop->SetType(reader.GetUInt8());
			add_shop->SetMaxBuyCount(reader.GetInt32());
			add_shop->SetMaxBuyType(reader.GetUInt8());
			add_shop->SetItemCount(0);

			_shops[add_shop->GetID()] = add_shop;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u shop definitions", _shops.size());
}

void ShopMgr::LoadShopItems()
{
	sLog->outLoad(true, "Loading Shop Items...");

	uint32 count = 0;

	auto result = GameServerDatabase.Query("SELECT * FROM shop_items ORDER BY shop ASC, id ASC");
	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());
			
			auto id = reader.GetUInt8();
			auto shop = GetShop(id);
			if (!shop)
				continue;

			ItemShop additem;
			additem.clear();
			additem.SetItem(reader.ReadItem());
			additem.SetLevel(reader.GetUInt8());
			additem.SetDurability(static_cast<float>(reader.GetUInt8()));
			additem.SetSkill(reader.GetUInt8());
			additem.SetLuck(reader.GetUInt8());
			additem.SetOption(reader.GetUInt8());
			additem.SetExe(reader.GetUInt8());
			additem.SetAncient(reader.GetUInt8());

			for (uint8 i = 0; i < MAX_SOCKET_SLOT; ++i)
				additem.SetSocket(i, reader.GetUInt16());

			additem.SetPrice(reader.GetUInt32());

			if (!sItemMgr->IsItem(additem.GetItem()))
			{
				sLog->outError("root", "%s :: Shop: %u, item: %d", __FUNCTION__, id, additem.GetItem());
				continue;
			}

			shop->AddItem(additem);
			count++;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u shop items", count);
}

Shop * ShopMgr::GetShop(uint8 id)
{
	auto itr = _shops.find(id);
	if (itr != _shops.end())
		return itr->second;
	else
		return nullptr;
}

Shop const* ShopMgr::GetShop(uint8 id) const
{
	auto itr = _shops.find(id);
	if (itr != _shops.end())
		return itr->second;
	else
		return nullptr;
}

Shop const* ShopMgr::GetShop(std::string const& name) const
{
	for (auto & itr : _shops)
	{
		auto shop = itr.second;
		if (shop->GetName() == name)
			return shop;
	}

	return nullptr;
}

bool ShopMgr::IsShop(std::string const& name) const
{
	return this->GetShop(name) != nullptr;
}

bool ShopMgr::EnableToBuy(Player* player, Shop const* shop)
{
	if (!player || !shop)
		return false;

	int32 count = 0;

	switch (shop->GetMaxBuyType())
	{
	case SHOP_COUNT_CHARACTER:
	{
								 auto & data_map = _characterPurchases[shop->GetID()];

								 auto it = data_map.find(player->GetGUID());
								 if (it != data_map.end())
									 count = it->second;
	} break;

	case SHOP_COUNT_ACCOUNT:
	{
							   auto & data_map = _accountPurchases[shop->GetID()];

							   auto it = data_map.find(player->GetAccountData()->GetGUID());
							   if (it != data_map.end())
								   count = it->second;
	} break;

	case SHOP_COUNT_PC:
	{
						  auto & data_map = _pcPurchases[shop->GetID()];

						  auto it = data_map.find(player->GetAccountData()->GetDiskSerial());
						  if (it != data_map.end())
							  count = it->second;
	} break;

	case SHOP_COUNT_SERVER:
	{
							  auto & data_map = _serverPurchases[shop->GetID()];

							  auto it = data_map.find(sGameServer->GetServerCode());
							  if (it != data_map.end())
								  count = it->second;
	} break;

	default:
		return true;
		break;
	}

	if (count >= shop->GetMaxBuyCount())
	{
		player->SendMessageBox(0, "Error", "Reached maximum day buy count.");
		return false;
	}

	++count;

	switch (shop->GetMaxBuyType())
	{
	case SHOP_COUNT_CHARACTER:
		_characterPurchases[shop->GetID()][player->GetGUID()] = count;
		break;

	case SHOP_COUNT_ACCOUNT:
		_accountPurchases[shop->GetID()][player->GetAccountData()->GetGUID()] = count;
		break;

	case SHOP_COUNT_PC:
		_pcPurchases[shop->GetID()][player->GetAccountData()->GetDiskSerial()] = count;
		break;

	case SHOP_COUNT_SERVER:
		_serverPurchases[shop->GetID()][sGameServer->GetServerCode()] = count;
		break;
	}

	return true;
}

void ShopMgr::Update()
{
	auto time = Custom::SystemTimer();
	if (time.GetDay() != _updateDay)
	{
		_characterPurchases.clear();
		_accountPurchases.clear();
		_pcPurchases.clear();
		_serverPurchases.clear();

		_updateDay = time.GetDay();
	}
}