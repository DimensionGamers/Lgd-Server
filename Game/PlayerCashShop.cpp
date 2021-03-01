/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerCashShop.cpp"
*
*/

void Player::CashShopSendPath()
{
	CASH_SHOP_DATA_UPDATE pMsg;
	strcpy(pMsg.item_ip, sGameServer->GetCashShopItemIP());
	strcpy(pMsg.item_dir, sGameServer->GetCashShopItemDir());
	strcpy(pMsg.item_dir_test, sGameServer->GetCashShopItemDirTest());

	strcpy(pMsg.banner_ip, sGameServer->GetCashShopBannerIP());
	strcpy(pMsg.banner_dir, sGameServer->GetCashShopBannerDir());
	strcpy(pMsg.banner_dir_test, sGameServer->GetCashShopBannerDirTest());

	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::CashShopSendDate()
{
	this->CashShopSendPath();

	this->CashShopSendDate(CASH_SHOP_DATE_ITEM, sGameServer->GetCashShopItemListNumber(), 
												sGameServer->GetCashShopItemListYear(), 
												sGameServer->GetCashShopItemListMonth());

	this->CashShopSendDate(CASH_SHOP_DATE_BANNER, sGameServer->GetCashShopBannerNumber(), 
												  sGameServer->GetCashShopBannerYear(), 
												  sGameServer->GetCashShopBannerMonth());
}

void Player::CashShopSendDate(uint8 type, uint16 id, uint16 year, uint16 month)
{
	CASH_SHOP_DATE pMsg(type, id, year, month);
	this->sendPacket(MAKE_PCT(pMsg));
}

CashShopItemData const* Player::CashShopGetItem(uint16 server, uint32 serial) const
{
	for (CashShopItemDataList::const_iterator itr = this->m_CashShopItemList.begin(); itr != this->m_CashShopItemList.end(); ++itr)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			continue;
		}

		if (pItemData->GetSerial() == serial && pItemData->GetServer() == server)
		{
			return pItemData;
		}
	}

	for (CashShopItemDataList::const_iterator itr = this->m_CashShopGiftList.begin(); itr != this->m_CashShopGiftList.end(); ++itr)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			continue;
		}

		if (pItemData->GetSerial() == serial && pItemData->GetServer() == server)
		{
			return pItemData;
		}
	}

	return nullptr;
}

void Player::CashShopDeleteItem(uint16 server, uint32 serial)
{
	for (CashShopItemDataList::const_iterator itr = this->m_CashShopItemList.begin(); itr != this->m_CashShopItemList.end();)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			++itr;
			continue;
		}

		if (pItemData->GetSerial() == serial && pItemData->GetServer() == server)
		{
			delete *itr;
			itr = this->m_CashShopItemList.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	for (CashShopItemDataList::const_iterator itr = this->m_CashShopGiftList.begin(); itr != this->m_CashShopGiftList.end();)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			++itr;
			continue;
		}

		if (pItemData->GetSerial() == serial && pItemData->GetServer() == server)
		{
			delete *itr;
			itr = this->m_CashShopGiftList.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void Player::CashShopCreditCoinUseRequest(uint8 level)
{
	this->IncreaseCredits(static_cast<uint32>(credit_coin[level]));
}

void Player::ReduceCredits(uint32 ammount)
{
	if ( this->GetCredits() >= ammount )
	{
		this->SetCredits(this->GetCredits() - ammount);
	}
	else
	{
		this->SetCredits(0);
	}
}

void Player::IncreaseCredits(uint32 ammount)
{
	this->SetCredits(this->GetCredits() + ammount);
}

int32 Player::CashShopGetTotalPage(uint8 type) const
{
	int32 count = this->m_CashShopItemList.size();

	if (type == CASH_SHOP_ITEM_LIST_GIFT)
	{
		count = this->m_CashShopGiftList.size();
	}

	return count;
}

bool Player::CashShopInsertItem(uint8 type, CashShopItemData * pItem, bool check)
{
	if (type == 1)
	{
		this->m_CashShopGiftList.push_back(pItem);
	}
	else
	{
		this->m_CashShopItemList.push_back(pItem);
	}

	return true;
}

void Player::CashShopSendExpireItem()
{
	full_inventory_loop(i)
	{
		if ( !this->GetInventory()->GetItem(i)->IsItem() )
			continue;

		if (sMonsterSoul->IsSoulConverter(this->GetInventory()->GetItem(i)->GetItem()))
		{
			continue;
		}

		if ( this->GetInventory()->GetItem(i)->GetExpireDate() > 0 )
		{
			EXPIRE_ITEM_DATA pMsg(this->GetInventory()->GetItem(i)->GetItem(), i, this->GetInventory()->GetItem(i)->GetExpireDate());
			this->SEND_PCT(pMsg);
		}
	}
}
	
void Player::CashShopSendExpireItem(uint8 slot)
{
	if ( !inventory_range(slot) )
	{
		return;
	}

	if (sMonsterSoul->IsSoulConverter(this->GetInventory()->GetItem(slot)->GetItem()))
	{
		return;
	}

	if ( this->GetInventory()->GetItem(slot)->GetExpireDate() > 0 )
	{
		EXPIRE_ITEM_DATA pMsg(this->GetInventory()->GetItem(slot)->GetItem(), slot, this->GetInventory()->GetItem(slot)->GetExpireDate());
		this->SEND_PCT(pMsg);
	}
}

void Player::CashShopCoinUpdate()
{
	if ( this->GetTimer(PLAYER_TIMER_CASH_SHOP_COIN)->Elapsed(sGameServer->GetCashShopCoinUpdateTime()) )
	{
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_COIN_REQUEST);
		stmt->setUInt32(0, this->GetAccountData()->GetGUID());

		this->CallBackCashShopCoin = MuDatabase.AsyncQuery(stmt);
	}
}
	
void Player::CashShopCoinCallBack(PreparedQueryResult result)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	if ( !result )
	{
		return;
	}

	Field* fields = result->Fetch();

	uint32 web_credits = fields[6].GetUInt32();

	this->SetCashShopDiscountWC(fields[7].GetUInt8());
	this->SetCashShopDiscountGP(fields[8].GetUInt8());
	this->SetCashShopDiscountDate(fields[9].GetInt64());

	if (this->GetCashShopDiscountWC() > 100)
	{
		this->SetCashShopDiscountWC(100);
	}

	if (this->GetCashShopDiscountGP() > 100)
	{
		this->SetCashShopDiscountGP(100);
	}
	
	this->IncreaseCredits(web_credits);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_COIN_CLEAR);
	stmt->setUInt32(0, web_credits);
	stmt->setUInt32(1, this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	this->SaveDBAccount(trans);

	MuDatabase.CommitTransaction(trans);

	if ( web_credits > 0 )
	{
		sLog->outInfo(LOG_CASHSHOP, "CashShopCoinCallBack() - %s Success: [%u, %u]",
			this->BuildLog().c_str(), this->GetCredits(), this->GetGoblinPoints());
	}
}

void Player::CashShopGiftSendCallBack(PreparedQueryResult result)
{
	if ( !Player::IsPlayerBasicState(this, false, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(this, InterfaceData::CashShop, 1) )
	{
		return;
	}

	if ( !result )
	{
		this->GetInterfaceState()->SetState(0);
		sCashShopMgr->CashShopItemGiftResult(this, 3);
		return;
	}

	Field* fields = result->Fetch();

	uint32 GUID = fields[0].GetUInt32();
	uint32 AccountID = fields[1].GetUInt32();

	if ( !GUID || !AccountID )
	{
		this->GetInterfaceState()->SetState(0);
		sCashShopMgr->CashShopItemGiftResult(this, 3);
		return;
	}

	CashShopPackage const* package = sCashShopMgr->GetPackage(this->GetCashShopGiftPackage());

	if ( !package )
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Data not found. Package: %u, Option: %u",
			this->BuildLog().c_str(), this->GetCashShopGiftPackage(), this->GetCashShopGiftOption());

		this->GetInterfaceState()->SetState(0);
		sCashShopMgr->CashShopItemGiftResult(this, 6);
		return;
	}

	uint32 price = sCashShopMgr->GetPackagePrice(this->GetCashShopGiftPackage(), this->GetCashShopGiftOption());

	switch ( package->GetPriceType() )
	{
	case CASH_SHOP_COIN_C:
		{
			if (!package->IsFlags(CASH_SHOP_FLAG_NO_DISCOUNT) && price > 0)
			{
				if (this->IsDiscountWC())
				{
					price = price * this->GetCashShopDiscountWC() / 100;
				}
				else if (sGameServer->IsDiscountWC())
				{
					price = price * sGameServer->GetCashShopDiscountWC() / 100;
				}

				if (price <= 0)
				{
					price = 1;
				}
			}

			if (this->GetCredits() < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->ReduceCredits(price);
		} break;

	case CASH_SHOP_COIN_P:
		{
			this->GetInterfaceState()->SetState(0);
			sCashShopMgr->CashShopItemBuyResult(this, 1);
			return;
		} break;

	case CASH_SHOP_COIN_GOBLIN:
		{
			if (!package->IsFlags(CASH_SHOP_FLAG_NO_DISCOUNT) && price > 0)
			{
				if (this->IsDiscountGP())
				{
					price = price * this->GetCashShopDiscountGP() / 100;
				}
				else if (sGameServer->IsDiscountGP())
				{
					price = price * sGameServer->GetCashShopDiscountGP() / 100;
				}

				if (price <= 0)
				{
					price = 1;
				}
			}

			if (this->GetGoblinPoints() < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->ReduceGoblinPoints(price);
		} break;

	case CASH_SHOP_COIN_ZEN:
		{
			if (this->MoneyGet() < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->MoneyReduce(price);
		} break;

	case CASH_SHOP_COIN_RUUD:
		{
			if (this->GetRuudMoney() < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->ReduceRuudMoney(price);
			this->SendRuudMoney();
		} break;

	case CASH_SHOP_COIN_BLESS:
		{
			if (this->GetInventory()->GetItemCount(JEWEL::BLESS, -1) < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->ItemDelete(JEWEL::BLESS, -1, price);
		} break;

	case CASH_SHOP_COIN_SOUL:
		{
			if (this->GetInventory()->GetItemCount(JEWEL::SOUL, -1) < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->ItemDelete(JEWEL::SOUL, -1, price);
		} break;

	case CASH_SHOP_COIN_CHAOS:
		{
			if (this->GetInventory()->GetItemCount(JEWEL::CHAOS, -1) < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->ItemDelete(JEWEL::CHAOS, -1, price);
		} break;

	case CASH_SHOP_COIN_GUARDIAN:
		{
			if (this->GetInventory()->GetItemCount(JEWEL::GUARDIAN, -1) < price)
			{
				this->GetInterfaceState()->SetState(0);
				sCashShopMgr->CashShopItemBuyResult(this, 1);
				return;
			}

			this->ItemDelete(JEWEL::GUARDIAN, -1, price);
		} break;
	}

	for (auto const& pData : sCashShopMgr->m_ProductList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetPackage() != this->GetCashShopGiftPackage() || pData->GetOption() != this->GetCashShopGiftOption())
		{
			continue;
		}

		if (pData->IsFlags(CASH_SHOP_FLAG_BUY_DISABLED))
		{
			continue;
		}

		sCashShopMgr->SaveBuyLog(this, package->GetPriceType(), pData, true, this->GetCashShopGiftName());

		for (int32 i = 0; i < pData->GetCount(); ++i)
		{
			sItemMgr->ItemSerialCreateCashShop(this, this->GetCashShopGiftPackage(), this->GetCashShopGiftOption(), pData->GetProduct(), AccountID);
		}
	}

	this->GetInterfaceState()->SetState(0);
	sCashShopMgr->CashShopItemGiftResult(this, 0);

	sServerLink->CharacterSignal(GUID, 1);
}
	
void Player::CashShopGiftUpdate()
{
	if ( this->IsGiftQuery() )
	{
		return;
	}

	this->SetGiftQuery(true);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_GIFT_REQUEST);
	stmt->setUInt32(0, this->GetAccountData()->GetGUID());

	this->CallBackCashShopGift = MuDatabase.AsyncQuery(stmt);
}
	
void Player::CashShopGiftCallBack(PreparedQueryResult result)
{
	this->SetGiftQuery(false);

	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();
	uint8 received = 0;

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			CashShopItemData* pItem = new CashShopItemData;
			pItem->SetProduct(fields[0].GetUInt32());
			pItem->SetOption(fields[1].GetUInt32());
			pItem->SetSerial(fields[2].GetUInt32());
			pItem->SetSerialCashShop(fields[3].GetUInt32());
			pItem->SetServer(fields[4].GetUInt32());
			pItem->SetDate(fields[5].GetInt64());

			if (this->CashShopInsertItem(1, pItem, true))
			{
				PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_GIFT_REMOVE);
				stmt->setUInt32(0, pItem->GetSerial());
				stmt->setUInt32(1, pItem->GetSerialCashShop());
				stmt->setUInt16(2, pItem->GetServer());
				stmt->setUInt32(3, this->GetAccountData()->GetGUID());

				trans->Append(stmt);

				if (received == 0)
				{
					received = 1;
				}
			}
			else
			{
				delete pItem;
				received = 2;
			}
		} while (result->NextRow());
	}

	this->SaveDBCashShopItem(trans);

	MuDatabase.CommitTransaction(trans);

	if (received == 1)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Received CashShop Gift");
	}
	else if (received == 2)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "CashShop Gift Store is full. You can't receive more gifts.");
	}
}

bool Player::IsDiscountWC() const
{
	if (this->GetCashShopDiscountWC() <= 0)
	{
		return false;
	}

	return time(nullptr) < this->GetCashShopDiscountDate();
}

bool Player::IsDiscountGP() const
{
	if (this->GetCashShopDiscountGP() <= 0)
	{
		return false;
	}

	return time(nullptr) < this->GetCashShopDiscountDate();
}