/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerPersonalStore.cpp"
*
*/

int32 GetJewelAmount(uint16 item, uint32 amount)
{
	auto pItemInfo = sItemMgr->GetItem(item);
	if (!pItemInfo)
	{
		return -1;
	}

	int32 max_stack = pItemInfo->GetStackData()->GetMaxStack();
	if (max_stack <= 0)
	{
		max_stack = 1;
	}

	int32 result = amount / max_stack;

	if ((amount % max_stack) != 0)
	{
		result++;
	}

	return result;
}

void CreateJewel(Player* pPlayer, uint16 item, uint32 amount)
{
	if (amount <= 0)
	{
		return;
	}

	auto pItemInfo = sItemMgr->GetItem(item);
	if (!pItemInfo)
	{
		return;
	}

	int32 full_stack = amount / pItemInfo->GetStackData()->GetMaxStack();
	int32 rest = amount % pItemInfo->GetStackData()->GetMaxStack();

	for (int32 i = 0; i < full_stack; ++i)
	{
		sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(item, 0, pItemInfo->GetStackData()->GetMaxStack()), GREMORY_CASE_TYPE_PERSONAL_STORE, GremoryCaseReward::EVENT);
	}

	if (rest > 0)
	{
		sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(item, 0, rest), GREMORY_CASE_TYPE_PERSONAL_STORE, GremoryCaseReward::EVENT);
	}
}

void Player::PersonalStoreClose(bool complete)
{
	if (!this->IsPlaying())
	{
		return;
	}

	if (!this->GetPersonalStore()->IsOpen())
	{
		return;
	}

	sLog->outInfo(LOG_PERSONAL_SHOP, "PersonalStoreClose() %s Personal Store Name: %s",
		this->BuildLog().c_str(), this->GetPersonalStore()->GetName());

	if (complete)
	{
		this->GetPersonalStore()->Clear();
	}
	else
	{
		this->GetPersonalStore()->End();
		this->SaveCharacter(3);
	}

	PERSONAL_STORE_CLOSE_RESULT pMsg;
	pMsg.result = 1;
	pMsg.SetIndex(this->GetEntry());
	this->SendPacket(&pMsg);
	this->SendPacketViewport(&pMsg);
}

void Player::UpdatePersonalStore()
{
	if (!this->GetPersonalStore()->IsOpen())
	{
		return;
	}

	if (this->GetPersonalStore()->IsEmpty())
	{
		this->PersonalStoreClose(false);
		this->SetAutoLoginInfo(0);
		return;
	}

	if (this->GetPersonalStore()->IsOff() && (!sGameServer->IsPersonalStoreOff() || !this->IsWorldFlag(WORLD_FLAG_ALLOW_OFFLINE_TRADE)))
	{
		this->PersonalStoreClose(false);
		this->SetAutoLoginInfo(0);
		return;
	}

	if (this->GetPersonalStore()->IsOff())
	{
		if (sGameServer->GetPersonalStoreOffDuration() > 0)
		{
			if ((MyGetTickCount() - this->GetPersonalStore()->GetOffTime()) > sGameServer->GetPersonalStoreOffDuration())
			{
				this->PersonalStoreClose(false);
				this->SetAutoLoginInfo(0);
			}
		}
	}
}

void Player::PersonalStoreBuyLog(Player* pPlayer01, Player* pPlayer02, Item const& item, std::string const& log, bool main)
{
	if (!sGameServer->IsLogDBPersonalStore())
	{
		return;
	}

	if (!pPlayer01 || !pPlayer02)
	{
		return;
	}

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_LOG_PERSONAL_STORE);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt32(pPlayer01->GetAccountData()->GetGUID());
	stmt->setString(pPlayer01->GetAccountData()->GetAccount());
	stmt->setUInt32(pPlayer01->GetGUID());
	stmt->setString(pPlayer01->GetName());
	stmt->setString(pPlayer01->BuildLogDB());
	stmt->setUInt32(pPlayer02->GetAccountData()->GetGUID());
	stmt->setString(pPlayer02->GetAccountData()->GetAccount());
	stmt->setUInt32(pPlayer02->GetGUID());
	stmt->setString(pPlayer02->GetName());
	stmt->setString(pPlayer02->BuildLogDB());
	stmt->setUInt8(main ? 2 : 0);
	stmt->setString(log);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void Player::HandlePersonalStoreSearch(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PERSONAL_STORE_SEARCH, lpMsg, Packet, 0);

	this->SendPersonalStoreList(0, lpMsg->data);
}

void Player::HandlePersonalStoreSearchItemList(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_SEARCH_ITEM_LIST, lpMsg, Packet, 0);

	this->SendPersonalStoreItemList(0, lpMsg->data, -1, "");
}

void Player::HandlePersonalStoreView(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_VIEW, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(PERSONAL_STORE_VIEW_HEAD, head, buffer, 0);
	POINTER_PCT(PERSONAL_STORE_VIEW_BODY, body, buffer, sizeof(PERSONAL_STORE_VIEW_HEAD));
	head->result = 1;
	head->count = 0;
	head->data = lpMsg->data;
	memset(head->seller, 0, MAX_CHARACTER_LENGTH + 1);
	memset(head->description, 0, PERSONAL_STORE_NAME_LENGTH + 1);

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->seller);
	
	if (pPlayer && pPlayer->IsPlaying() && pPlayer->GetPersonalStore()->IsOpen() && sGameServer->IsPersonalStoreEnabled())
	{
		memcpy(head->seller, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		memcpy(head->description, pPlayer->GetPersonalStore()->GetName(), PERSONAL_STORE_NAME_LENGTH);

		for (int32 i = 0; i < PERSONAL_STORE_SELL_MAX; ++i)
		{
			auto pItem = pPlayer->GetPersonalStore()->GetItem(i);
			if (!pItem || !pItem->IsItem())
			{
				continue;
			}

			auto & data = body[head->count++];
			data.slot = i;
			data.bundle = pPlayer->GetPersonalStore()->IsBundle(i) ? 2 : 1;
			data.zen = pItem->GetPersonalStorePrice();
			data.jewel_of_bless = pItem->GetPersonalStorePriceJoB();
			data.jewel_of_soul = pItem->GetPersonalStorePriceJoS();

			pItem->ConvertToBuffer(data.item_info);
		}
	}
	else
	{
		head->result = 2;
	}

	head->Set(0x7C, 0x02, sizeof(PERSONAL_STORE_VIEW_HEAD)+(sizeof(PERSONAL_STORE_VIEW_BODY)* head->count));
	this->sendPacket(buffer);
}

void Player::HandlePersonalStoreSearchItem(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_SEARCH_ITEM, lpMsg, Packet, 0);

	this->SendPersonalStoreItemList(0, lpMsg->data, lpMsg->item, lpMsg->item_name);
}

void Player::HandlePersonalStoreSelectItem(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_SELECT_ITEM, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(PERSONAL_STORE_SELECT_ITEM_HEAD, head, buffer, 0);
	POINTER_PCT(PERSONAL_STORE_SELECT_ITEM_BODY, body, buffer, sizeof(PERSONAL_STORE_SELECT_ITEM_HEAD));
	head->result = 1;
	head->count = 0;
	head->data = lpMsg->data;
	memset(head->seller, 0, MAX_CHARACTER_LENGTH + 1);
	memcpy(head->seller, lpMsg->seller, MAX_CHARACTER_LENGTH);
	head->slot = lpMsg->slot;

	this->GetPersonalStore()->ResetView();

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->seller);
	if (pPlayer && pPlayer->IsPlaying() && pPlayer->GetPersonalStore()->IsOpen() && pPlayer->GetPersonalStore()->IsSellRange(lpMsg->slot) && sGameServer->IsPersonalStoreEnabled())
	{
		auto pItem = pPlayer->GetPersonalStore()->GetItem(lpMsg->slot);
		if (pItem && pItem->IsItem())
		{
			this->GetPersonalStore()->SetViewStore(pPlayer);
			this->GetPersonalStore()->SetViewSlot(lpMsg->slot);
			this->GetPersonalStore()->SetViewType(6);

			sPentagramSystem->GCPentagramJewelPShopInfoSend(this, pPlayer, lpMsg->slot);

			head->zen = pItem->GetPersonalStorePrice();
			head->jewel_of_bless = pItem->GetPersonalStorePriceJoB();
			head->jewel_of_soul = pItem->GetPersonalStorePriceJoS();

			PersonalStoreItemMap map;
			pPlayer->GetPersonalStore()->GetItemList(lpMsg->slot, map);
			
			for (auto & itr : map)
			{
				pItem = itr.second;

				auto & data = body[head->count++];
				pItem->ConvertToBuffer(data.item_info);
			}
		}
		else
		{
			head->result = 2;
		}
	}
	else
	{
		head->result = 2;
	}

	head->Set(0x7C, 0x04, sizeof(PERSONAL_STORE_SELECT_ITEM_HEAD)+(sizeof(PERSONAL_STORE_SELECT_ITEM_BODY)* head->count));
	this->sendPacket(buffer);

	this->SendPersonalStorePet();
}

void Player::HandlePersonalStorePurchase(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	POINTER_PCT(PERSONAL_STORE_PURCHASE_HEAD, lpHead, Packet, 0);
	POINTER_PCT(PERSONAL_STORE_PURCHASE_BODY, lpBody, Packet, sizeof(PERSONAL_STORE_PURCHASE_HEAD));

	PERSONAL_STORE_PURCHASE_RESULT pMsg;
	pMsg.data = lpHead->data;
	pMsg.result = 0;
	memset(pMsg.seller, 0, MAX_CHARACTER_LENGTH + 1);
	memcpy(pMsg.seller, lpHead->seller, MAX_CHARACTER_LENGTH);
	pMsg.slot = lpHead->slot;

	PERSONAL_STORE_ITEM_SOLD pMsg1;
	pMsg1.data = lpHead->data;
	memcpy(pMsg1.seller, lpHead->seller, MAX_CHARACTER_LENGTH);
	pMsg1.slot = lpHead->slot;
	pMsg1.state = 0;
	memcpy(pMsg1.buyer, this->GetName(), MAX_CHARACTER_LENGTH);

	if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_BUY_PERSONAL_STORE))
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->SendPacket(&pMsg);
		return;
	}

	if (!sGameServer->IsPersonalStoreEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Personal Shop is Disabled.");
		this->SendPacket(&pMsg);
		return;
	}

	if (this->GetTransaction() == TRANSACTION_TYPE_BEGIN)
	{
		this->SendPacket(&pMsg);
		return;
	}

	Player* pPlayer = sObjectMgr->FindPlayerByName(lpHead->seller);

	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	if (pPlayer == this)
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (!pPlayer->GetPersonalStore()->IsOpen())
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (!pPlayer->GetPersonalStore()->IsSellRange(lpHead->slot))
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (!pPlayer->IsWorldFlag(WORLD_FLAG_ALLOW_EXCHANGE))
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	if (sGameServer->IsPersonalStoreLockSamePC())
	{
		if (this->GetAccountData()->GetDiskSerial() == pPlayer->GetAccountData()->GetDiskSerial() ||
			!memcmp(this->GetAccountData()->GetIP(), pPlayer->GetAccountData()->GetIP(), MAX_ACCOUNT_IP_LENGTH) ||
			!memcmp(this->GetAccountData()->GetMac(), pPlayer->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH))
		{
			sLog->outInfo(LOG_PERSONAL_SHOP, "PersonalStoreItemBuy() Same PC %s /// %s",
				this->BuildLog().c_str(), pPlayer->BuildLog().c_str());

			pMsg.result = 3;
			this->SendPacket(&pMsg);
			return;
		}
	}

	auto pItem = pPlayer->GetPersonalStore()->GetItem(lpHead->slot);
	if (!pItem || !pItem->IsItem())
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	if (pItem->GetPersonalStorePrice() <= 0 && pItem->GetPersonalStorePriceJoB() <= 0 && pItem->GetPersonalStorePriceJoS() <= 0)
	{
		this->SendPacket(&pMsg);
		return;
	}

	auto pGremoryCaseMine = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE);
	auto pGremoryCaseSeller = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE);

	if (!pGremoryCaseMine || !pGremoryCaseSeller)
	{
		this->SendPacket(&pMsg);
		return;
	}

	PersonalStoreItemMap map;
	pPlayer->GetPersonalStore()->GetItemList(lpHead->slot, map);

	if (pGremoryCaseMine->GetTotalFreeSlots() < map.size())
	{
		pMsg.result = 5;
		this->SendPacket(&pMsg);
		return;
	}

	if (pItem->GetPersonalStorePrice() > 0 && this->GetCredits() < pItem->GetPersonalStorePrice())
	{
		pMsg.result = 2;
		this->SendPacket(&pMsg);
		return;
	}

	if (pItem->GetPersonalStorePriceJoB() > 0 && this->GetInventory()->GetItemCount(JEWEL::BLESS, -1) < pItem->GetPersonalStorePriceJoB())
	{
		pMsg.result = 2;
		this->SendPacket(&pMsg);
		return;
	}

	if (pItem->GetPersonalStorePriceJoS() > 0 && this->GetInventory()->GetItemCount(JEWEL::SOUL, -1) < pItem->GetPersonalStorePriceJoS())
	{
		pMsg.result = 2;
		this->SendPacket(&pMsg);
		return;
	}

	int32 required_slot = 0;
	if (pItem->GetPersonalStorePriceJoB() > 0)
	{
		int32 amount = GetJewelAmount(JEWEL::BLESS, pItem->GetPersonalStorePriceJoB());

		if (amount == -1)
		{
			pMsg.result = 2;
			this->SendPacket(&pMsg);
			return;
		}

		required_slot += amount;
	}

	if (pItem->GetPersonalStorePriceJoS() > 0)
	{
		int32 amount = GetJewelAmount(JEWEL::SOUL, pItem->GetPersonalStorePriceJoS());

		if (amount == -1)
		{
			pMsg.result = 2;
			this->SendPacket(&pMsg);
			return;
		}

		required_slot += amount;
	}

	if (pGremoryCaseSeller->GetTotalFreeSlots() < required_slot)
	{
		pMsg.result = 6;
		this->SendPacket(&pMsg);

		pMsg1.result = 2;
		pMsg1.state = pPlayer->GetPersonalStore()->IsOpen();
		pPlayer->SendPacket(&pMsg1);
		return;
	}

	if (pItem->GetPersonalStorePrice() > 0)
	{
		this->ReduceCredits(pItem->GetPersonalStorePrice());
		pPlayer->IncreaseCredits(pItem->GetPersonalStorePrice());
	}

	if (pItem->GetPersonalStorePriceJoB() > 0)
	{
		this->ItemDelete(JEWEL::BLESS, -1, pItem->GetPersonalStorePriceJoB());
		CreateJewel(pPlayer, JEWEL::BLESS, pItem->GetPersonalStorePriceJoB());
	}

	if (pItem->GetPersonalStorePriceJoS() > 0)
	{
		this->ItemDelete(JEWEL::SOUL, -1, pItem->GetPersonalStorePriceJoS());
		CreateJewel(pPlayer, JEWEL::SOUL, pItem->GetPersonalStorePriceJoS());
	}

	for (auto & itr : map)
	{
		pItem = itr.second;

		const auto& item_log = pItem->BuildLogDB(-1, true);

		sLog->outInfo(LOG_PERSONAL_SHOP, "PersonalStoreItemBuy() %s -- %s bought %s -- %s ITEM -> %s. Off: %d",
			this->BuildLog().c_str(), this->BuildLocationLog().c_str(),
			pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(),
			item_log.c_str(), pPlayer->GetPersonalStore()->IsOff());

		PersonalStoreBuyLog(pPlayer, this, *pItem, item_log, pPlayer->GetPersonalStore()->IsSellRange(itr.first));

		sPentagramSystem->ExchangePentagramItem(pPlayer, this, pItem);

		sGremoryCase->ItemAdd(this, GREMORY_CASE_TYPE_PERSONAL_STORE, pGremoryCaseMine->AddItem(*pItem));
		pPlayer->GetPersonalStore()->DeleteItem(itr.first);
	}

	TransactionDBSave(this, pPlayer);
	
	if (pPlayer->GetPersonalStore()->IsEmpty())
	{
		pPlayer->PersonalStoreClose(!pPlayer->GetPersonalStore()->IsOff());
		pPlayer->SetAutoLoginInfo(0);
	}

	Player::TransactionSharedSerialCheck(pPlayer, this, "PERSONAL SHOP");

	pMsg1.result = 1;
	pMsg1.state = pPlayer->GetPersonalStore()->IsOpen();
	pPlayer->SendPacket(&pMsg1);

	/*
	Results

	1 = Success
	2 = Not enough currency
	3 = Already sold
	5 = Not enough space in Gremory Case
	6 = Not enough space in other Gremory Case
	7 = Can't purchase while mining
	*/

	pMsg.result = 1;
	this->SendPacket(&pMsg);
}

void Player::HandlePersonalStoreSellList(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_SELL_LIST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(PERSONAL_STORE_SELL_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(PERSONAL_STORE_SELL_LIST_BODY, body, buffer, sizeof(PERSONAL_STORE_SELL_LIST_HEAD));
	head->data = lpMsg->data;
	head->result = 1;
	head->count = 0;
	head->state = this->GetPersonalStore()->IsOpen();
	memset(head->description, 0, PERSONAL_STORE_NAME_LENGTH + 1);
	memcpy(head->description, this->GetPersonalStore()->GetName(), PERSONAL_STORE_NAME_LENGTH);

	for (int32 i = 0; i < PERSONAL_STORE_SELL_MAX; ++i)
	{
		auto pItem = this->GetPersonalStore()->GetItem(i);
		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		auto & data = body[head->count++];
		data.slot = i;
		data.bundle = this->GetPersonalStore()->IsBundle(i) ? 2 : 1;
		data.zen = pItem->GetPersonalStorePrice();
		data.jewel_of_bless = pItem->GetPersonalStorePriceJoB();
		data.jewel_of_soul = pItem->GetPersonalStorePriceJoS();

		pItem->ConvertToBuffer(data.item_info);
	}

	head->Set(0x7C, 0x06, sizeof(PERSONAL_STORE_SELL_LIST_HEAD)+(sizeof(PERSONAL_STORE_SELL_LIST_BODY)* head->count));
	this->sendPacket(buffer);
}

void Player::HandlePersonalStoreSellItemView(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_SELL_ITEM_VIEW, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(PERSONAL_STORE_SELL_ITEM_VIEW_HEAD, head, buffer, 0);
	POINTER_PCT(PERSONAL_STORE_SELL_ITEM_VIEW_BODY, body, buffer, sizeof(PERSONAL_STORE_SELL_ITEM_VIEW_HEAD));
	head->result = 1;
	head->slot = lpMsg->slot;
	head->count = 0;

	this->GetPersonalStore()->ResetView();

	if (this->GetPersonalStore()->IsSellRange(lpMsg->slot))
	{
		auto pItem = this->GetPersonalStore()->GetItem(lpMsg->slot);
		if (pItem && pItem->IsItem())
		{
			this->GetPersonalStore()->SetViewStore(this);
			this->GetPersonalStore()->SetViewSlot(lpMsg->slot);
			this->GetPersonalStore()->SetViewType(5);

			sPentagramSystem->GCPentagramJewelPShopInfoSend(this, this, lpMsg->slot);

			head->zen = pItem->GetPersonalStorePrice();
			head->jewel_of_bless = pItem->GetPersonalStorePriceJoB();
			head->jewel_of_soul = pItem->GetPersonalStorePriceJoS();

			PersonalStoreItemMap map;
			this->GetPersonalStore()->GetItemList(lpMsg->slot, map);

			for (auto & itr : map)
			{
				pItem = itr.second;

				auto & data = body[head->count++];
				pItem->ConvertToBuffer(data.item_info);
			}
		}
		else
		{
			head->result = 2;
		}
	}
	else
	{
		head->result = 2;
	}

	head->Set(0x7C, 0x07, sizeof(PERSONAL_STORE_SELL_ITEM_VIEW_HEAD)+(sizeof(PERSONAL_STORE_SELL_ITEM_VIEW_BODY)* head->count));
	this->sendPacket(buffer);

	this->SendPersonalStorePet();
}

void Player::HandlePersonalStoreSellItem(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_SELL_ITEM, lpMsg, Packet, 0);

	PERSONAL_STORE_SELL_ITEM_RESULT pMsg;
	pMsg.slot = lpMsg->slot;

	if (!Player::IsPlayerBasicState(this, false, false))
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (!this->GetPersonalStore()->IsSellRange(lpMsg->slot))
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < sGameServer->GetPersonalStoreMinLevel())
	{
		this->SendPacket(&pMsg);
		return;
	}

	auto pItem = this->GetPersonalStore()->GetItem(lpMsg->slot);
	if (!pItem)
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (lpMsg->modify == 1)
	{
		if (!pItem->IsItem())
		{
			this->SendPacket(&pMsg);
			return;
		}

		pMsg.result = 1;

		pItem->SetState(ITEM_CHANGED);
	}
	else
	{
		if (pItem->IsItem())
		{
			this->SendPacket(&pMsg);
			return;
		}

		if (!sGameServer->IsPersonalStoreEnabled())
		{
			this->SendPacket(&pMsg);
			return;
		}

		for (int32 i = 0; i < PERSONAL_STORE_BUNDLE_MAX; ++i)
		{
			if (!inventory_range(lpMsg->inventory_slot[i]))
			{
				if (i == 0)
				{
					this->SendPacket(&pMsg);
					return;
				}

				continue;
			}

			auto pInventoryItem = this->GetInventory()->GetItem(lpMsg->inventory_slot[i]);
			if (!pInventoryItem || !pInventoryItem->IsItem())
			{
				if (i == 0)
				{
					this->SendPacket(&pMsg);
					return;
				}

				continue;
			}

			if (!this->CheckItemMoveToTrade(pInventoryItem))
			{
				this->SendPacket(&pMsg);
				return;
			}

			if (pInventoryItem->IsInventoryActive())
			{
				this->SendPacket(&pMsg);
				return;
			}

			uint32 to_add = lpMsg->slot;
			if (i != 0)
			{
				to_add = (PERSONAL_STORE_SELL_MAX + lpMsg->slot * PERSONAL_STORE_BUNDLE_MAX) + i;
			}

			auto pAddItem = this->GetPersonalStore()->GetItem(to_add);

			if (!pAddItem || pAddItem->IsItem())
			{
				continue;
			}
		}

		for (int32 i = 0; i < PERSONAL_STORE_BUNDLE_MAX; ++i)
		{
			if (!inventory_range(lpMsg->inventory_slot[i]))
			{
				continue;
			}

			auto pInventoryItem = this->GetInventory()->GetItem(lpMsg->inventory_slot[i]);

			if (!pInventoryItem || !pInventoryItem->IsItem())
			{
				continue;
			}

			uint32 to_add = lpMsg->slot;
			if (i != 0)
			{
				to_add = (PERSONAL_STORE_SELL_MAX + lpMsg->slot * PERSONAL_STORE_BUNDLE_MAX) + i;
			}

			this->GetPersonalStore()->AddItem(*pInventoryItem, to_add);
			this->ClearItem(lpMsg->inventory_slot[i]);
			this->WearedItemRemove(lpMsg->inventory_slot[i]);
		}

		pMsg.bundle = this->GetPersonalStore()->IsBundle(lpMsg->slot) ? 2 : 1;
		pItem->ConvertToBuffer(pMsg.item_info);
	}

	pItem->SetPersonalStorePrice(lpMsg->zen);
	pItem->SetPersonalStorePriceJoB(lpMsg->jewel_of_bless);
	pItem->SetPersonalStorePriceJoS(lpMsg->jewel_of_soul);

	pMsg.zen = pItem->GetPersonalStorePrice();
	pMsg.jewel_of_bless = pItem->GetPersonalStorePriceJoB();
	pMsg.jewel_of_soul = pItem->GetPersonalStorePriceJoS();

	pMsg.data = 1;

	this->SendPacket(&pMsg);
}

void Player::HandlePersonalStoreSellItemCancel(uint8 * Packet)
{
	POINTER_PCT(PERSONAL_STORE_SELL_ITEM_CANCEL_HEAD, lpHead, Packet, 0);
	POINTER_PCT(PERSONAL_STORE_SELL_ITEM_CANCEL_BODY, lpBody, Packet, sizeof(PERSONAL_STORE_SELL_ITEM_CANCEL_HEAD));

	PERSONAL_STORE_SELL_ITEM_CANCEL pMsg;
	pMsg.data = lpHead->data;
	pMsg.result = 0;
	pMsg.slot = lpHead->slot;

	if (!Player::IsPlayerBasicState(this, false, false))
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (!this->GetPersonalStore()->IsSellRange(lpHead->slot))
	{
		this->SendPacket(&pMsg);
		return;
	}

	auto pItem = this->GetPersonalStore()->GetItem(lpHead->slot);
	if (!pItem || !pItem->IsItem())
	{
		this->SendPacket(&pMsg);
		return;
	}

	PersonalStoreItemMap map;
	this->GetPersonalStore()->GetItemList(lpHead->slot, map);

	auto pGremoryCase = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE);
	if (!pGremoryCase)
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (pGremoryCase->GetTotalFreeSlots() < map.size())
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	for (auto & itr : map)
	{
		pItem = itr.second;

		sGremoryCase->ItemAdd(this, GREMORY_CASE_TYPE_PERSONAL_STORE, pGremoryCase->AddItem(*pItem));
		this->GetPersonalStore()->DeleteItem(itr.first);
	}

	pMsg.result = 1;

	this->SendPacket(&pMsg);
}

void Player::HandlePersonalStoreState(uint8 * Packet)
{
	POINTER_PCT_CHECK(PERSONAL_STORE_STATE, lpMsg, Packet, 0);

	PERSONAL_STORE_STATE_RESULT pMsg;
	pMsg.data = lpMsg->data;
	pMsg.state = 0;
	pMsg.result = 0;

	if (lpMsg->state == 0)
	{
		if (!Player::IsPlayerBasicState(this, false, false))
		{
			return;
		}

		this->PersonalStoreClose();
	}
	else if (lpMsg->state == 1)
	{
		if (this->GetPersonalStore()->IsOpen())
		{
			this->SendPacket(&pMsg);
			return;
		}

		if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_OPEN_PERSONAL_STORE))
		{
			this->SendPacket(&pMsg);
			return;
		}

		if (!this->IsAuthorizationEnabled())
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
			this->SendPacket(&pMsg);
			return;
		}

		if (!this->CanInteract())
		{
			this->SendPacket(&pMsg);
			return;
		}

		if (!sGameServer->IsPersonalStoreEnabled())
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Personal Shop is Disabled.");
			this->SendPacket(&pMsg);
			return;
		}

		if (this->GetLevel() < sGameServer->GetPersonalStoreMinLevel())
		{
			this->SendPacket(&pMsg);
			return;
		}

		if (!this->IsWorldFlag(WORLD_FLAG_ALLOW_EXCHANGE))
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to use personal shop here.");
			this->SendPacket(&pMsg);
			return;
		}

		if (this->GetInterfaceState()->GetID() != InterfaceData::None)
		{
			this->SendPacket(&pMsg);
			return;
		}

		if (this->GetTransaction() == TRANSACTION_TYPE_BEGIN)
		{
			this->SendPacket(&pMsg);
			return;
		}

		World* pWorld = this->GetWorld();
		
		if (!pWorld)
		{
			this->SendPacket(&pMsg);
			return;
		}
		
		if (pWorld->IsAreaRestriction(this->GetX(), this->GetY(), WORLD_AREA_FLAG_PERSONAL_STORE))
		{
			this->SendPacket(&pMsg);
			return;
		}
		
		for (uint32 i = 0; i < this->GetPersonalStore()->GetItemSize(); ++i)
		{
			auto pItem = this->GetPersonalStore()->GetItem(i);
			if (!pItem || !pItem->IsItem())
			{
				continue;
			}

			if (!pItem->IsFlag(ITEM_FLAG_ALLOW_EXCHANGE))
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s can't be sold on Personal Store.", this->GetInventory()->GetItem(i)->GetName());
				this->SendPacket(&pMsg);
				return;
			}
		}

		if (this->GetPersonalStore()->IsEmpty())
		{
			this->SendPacket(&pMsg);
			return;
		}

		this->GetPersonalStore()->SetOpen(true);

		sLog->outInfo(LOG_PERSONAL_SHOP, "PersonalStoreOpen() %s Personal Store Name: %s", this->BuildLog().c_str(), this->GetPersonalStore()->GetName());

		for (uint32 i = 0; i < this->GetPersonalStore()->GetItemSize(); ++i)
		{
			auto pItem = this->GetPersonalStore()->GetItem(i);
			if (!pItem || !pItem->IsItem())
			{
				continue;
			}

			sLog->outInfo(LOG_PERSONAL_SHOP, "PersonalStoreOpen() %s ITEM -> %s. Personal Store Price: [%u / %u / %u]",
				this->BuildLog().c_str(), 
				pItem->BuildLog(i).c_str(),
				pItem->GetPersonalStorePrice(),
				pItem->GetPersonalStorePriceJoB(),
				pItem->GetPersonalStorePriceJoS());
		}

		this->SaveCharacter();

		PERSONAL_STORE_VIEWPORT pMsg1;
		pMsg1.SetIndex(this->GetEntry());
		memcpy(pMsg1.description, lpMsg->description, PERSONAL_STORE_NAME_LENGTH);
		this->SendPacketViewport(&pMsg1);
	}
	else 
	{
		this->GetPersonalStore()->ResetName();
		this->GetPersonalStore()->SetName(lpMsg->description, PERSONAL_STORE_NAME_LENGTH);

		if (this->GetPersonalStore()->IsOpen())
		{
			PERSONAL_STORE_TEXT_CHANGE_SEND pMsg1;
			pMsg1.SetIndex(this->GetEntry());
			memcpy(pMsg1.seller, this->GetName(), MAX_CHARACTER_LENGTH);
			memcpy(pMsg1.description, lpMsg->description, PERSONAL_STORE_NAME_LENGTH);

			this->SendPacket(&pMsg1);
			this->SendPacketViewport(&pMsg1);
		}
	}

	pMsg.state = this->GetPersonalStore()->IsOpen();
	pMsg.result = 1;
	this->SendPacket(&pMsg);
}

void Player::SendPersonalStoreList(uint32 page, uint32 data)
{
	uint8 buffer[8192];
	POINTER_PCT(PERSONAL_STORE_SEARCH_HEAD, head, buffer, 0);
	POINTER_PCT(PERSONAL_STORE_SEARCH_BODY, body, buffer, sizeof(PERSONAL_STORE_SEARCH_HEAD));
	head->data = data;
	head->count = 0;
	head->unk1 = 0;
	head->unk2 = 0;

	if (sGameServer->IsPersonalStoreEnabled())
	{
		uint32 read_count = 0;

		auto& characters = sObjectMgr->GetAllCharacters();
		for (auto & itr : characters)
		{
			auto pPlayer = itr.second;

			if (!pPlayer || !pPlayer->IsPlaying() || !pPlayer->GetPersonalStore()->IsOpen()/* || pPlayer == this*/)
			{
				continue;
			}

			if (read_count >= (page * PERSONAL_STORE_SEARCH_MAX))
			{
				auto & body_data = body[head->count++];
				memset(body_data.seller, 0, MAX_CHARACTER_LENGTH + 1);
				memset(body_data.description, 0, PERSONAL_STORE_NAME_LENGTH + 1);

				memcpy(body_data.seller, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
				memcpy(body_data.description, pPlayer->GetPersonalStore()->GetName(), PERSONAL_STORE_NAME_LENGTH);

				if (head->count >= PERSONAL_STORE_SEARCH_MAX)
				{
					break;
				}
			}

			++read_count;
		}

		if (page > 0 && head->count <= 0)
		{
			return;
		}
	}

	head->Set(0x7C, 0x00, sizeof(PERSONAL_STORE_SEARCH_HEAD)+(sizeof(PERSONAL_STORE_SEARCH_BODY)* head->count));
	this->sendPacket(buffer);

	if (head->count >= PERSONAL_STORE_SEARCH_MAX)
	{
		this->SendPersonalStoreList(page + 1, data);
	}
}

void Player::SendPersonalStoreItemList(uint32 page, uint32 data, uint16 item, std::string const& item_name)
{
	uint8 buffer[8192];
	POINTER_PCT(PERSONAL_STORE_SEARCH_ITEM_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(PERSONAL_STORE_SEARCH_ITEM_LIST_BODY, body, buffer, sizeof(PERSONAL_STORE_SEARCH_ITEM_LIST_HEAD));
	head->data = data;
	head->result = 0;
	head->count = 0;
	head->unk1 = 0;

	if (sGameServer->IsPersonalStoreEnabled())
	{
		uint32 read_count = 0;

		auto& characters = sObjectMgr->GetAllCharacters();
		for (auto & itr : characters)
		{
			auto pPlayer = itr.second;

			if (!pPlayer || !pPlayer->IsPlaying() || !pPlayer->GetPersonalStore()->IsOpen()/* || pPlayer == this*/)
			{
				continue;
			}

			for (int32 i = 0; i < PERSONAL_STORE_SELL_MAX; ++i)
			{
				auto pItem = pPlayer->GetPersonalStore()->GetItem(i);
				if (!pItem || !pItem->IsItem())
				{
					continue;
				}

				if (item != uint16(-1) && pItem->GetItem() != item)
				{
					continue;
				}

				if (read_count >= (page * PERSONAL_STORE_SEARCH_MAX))
				{
					auto & body_data = body[head->count++];
					memset(body_data.seller, 0, MAX_CHARACTER_LENGTH + 1);
					memcpy(body_data.seller, pPlayer->GetName(), MAX_CHARACTER_LENGTH);

					body_data.slot = i;
					body_data.bundle = pPlayer->GetPersonalStore()->IsBundle(i) ? 2 : 1;
					body_data.zen = pItem->GetPersonalStorePrice();
					body_data.jewel_of_bless = pItem->GetPersonalStorePriceJoB();
					body_data.jewel_of_soul = pItem->GetPersonalStorePriceJoS();

					pItem->ConvertToBuffer(body_data.item_info);

					if (head->count >= PERSONAL_STORE_SEARCH_MAX)
					{
						break;
					}
				}

				++read_count;
			}
		}

		if (page > 0 && head->count <= 0)
		{
			return;
		}
	}

	head->Set(0x7C, 0x01, sizeof(PERSONAL_STORE_SEARCH_ITEM_LIST_HEAD)+(sizeof(PERSONAL_STORE_SEARCH_ITEM_LIST_BODY)* head->count));
	this->sendPacket(buffer);

	if (head->count >= PERSONAL_STORE_SEARCH_MAX)
	{
		this->SendPersonalStoreItemList(page + 1, data, item, item_name);
	}
}

void Player::SendPersonalStorePet()
{
	auto pPlayer = this->GetPersonalStore()->GetViewStore();
	auto slot = this->GetPersonalStore()->GetViewSlot();
	auto type = this->GetPersonalStore()->GetViewType();

	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	if (!pPlayer->GetPersonalStore()->IsSellRange(slot))
	{
		return;
	}

	auto pItem = pPlayer->GetPersonalStore()->GetItem(slot);
	if (!pItem)
	{
		return;
	}

	PersonalStoreItemMap map;
	pPlayer->GetPersonalStore()->GetItemList(slot, map);

	for (auto & itr : map)
	{
		pItem = itr.second;

		if (pItem->GetItem() == PET_DARK_RAVEN || pItem->GetItem() == PET_DARK_HORSE)
		{
			uint32 view_slot = itr.first;
			if (view_slot >= PERSONAL_STORE_SELL_MAX)
			{
				view_slot -= ((slot * PERSONAL_STORE_BUNDLE_MAX) + PERSONAL_STORE_SELL_MAX);
			}
			else
			{
				view_slot = 0;
			}

			PET_INFO_RESULT pMsg(pItem->GetItem() == PET_DARK_RAVEN ? 0 : 1, type, view_slot);
			pMsg.experience = pItem->pet_dark.experience;
			pMsg.level = pItem->pet_dark.level;
			pMsg.durability = pItem->GetDurability();

			this->SendPacket(&pMsg);
		}
	}
}