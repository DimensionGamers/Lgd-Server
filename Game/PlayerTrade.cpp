/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerTrade.cpp"
*
*/

void Player::TradeRequest(uint8 * Packet)
{
	POINTER_PCT_LOG(TRADE_REQUEST, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetIndex());

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_TRADE, pPlayer) )
		return;

	this->GetInterfaceState()->Set(InterfaceData::Trade, pPlayer, 0, sGameServer->trade_max_wait_interval.get());
	pPlayer->GetInterfaceState()->Set(InterfaceData::Trade, this, 0, sGameServer->trade_max_wait_interval.get());

	sLog->outInfo(LOG_TRADE, "TradeRequest() [%s][%s] Requested [%s][%s]", 
		this->GetAccountData()->GetAccount(), this->GetName(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());

	TRADE_REQUEST_SEND pMsg(this->GetName());
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeRequestAnswer(uint8 * Packet)
{
	Player * pPlayer = (this->GetInterfaceState()->GetTarget()) ? this->GetInterfaceState()->GetTarget()->ToPlayer(): nullptr;

	if ( !pPlayer )
		return;

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_TRADE_ANSWER, pPlayer) )
		return;

	POINTER_PCT_LOG(TRADE_ANSWER, lpMsg, Packet, 0);

	if ( lpMsg->answer == 0 ) // Cancel Trade
	{
		pPlayer->TradeRequestResult(0, this->GetName());

		pPlayer->GetInterfaceState()->Reset();
		this->GetInterfaceState()->Reset();

		sLog->outInfo(LOG_TRADE, "TradeRequestAnswer() %s Canceled %s", this->BuildLog().c_str(), pPlayer->BuildLog().c_str());
	}
	else
	{
		Player::TradeBegin(this, pPlayer);
	}
}

void Player::TradeBegin(Player* pPlayer01, Player* pPlayer02)
{
	pPlayer01->GetTradeInventory()->Clear();
	pPlayer02->GetTradeInventory()->Clear();

	if ( !pPlayer01->BeginTransaction() )
	{
		pPlayer02->TradeRequestResult(0, pPlayer01->GetName());
		pPlayer01->TradeRequestResult(0, pPlayer02->GetName());
		return;
	}

	if ( !pPlayer02->BeginTransaction() )
	{
		pPlayer01->RollBackTransaction();
		pPlayer02->TradeRequestResult(0, pPlayer01->GetName());
		pPlayer01->TradeRequestResult(0, pPlayer02->GetName());
		return;
	}

	if ( pPlayer01->GetInterfaceState()->GetType() == 1 )
	{
		pPlayer01->GetInterfaceState()->Set(InterfaceData::Trade, pPlayer02, 0, sGameServer->trade_max_wait_interval.get());
		pPlayer01->GetInterfaceState()->SetType(1);

		pPlayer02->GetInterfaceState()->Set(InterfaceData::Trade, pPlayer01, 0, sGameServer->trade_max_wait_interval.get());
		pPlayer02->GetInterfaceState()->SetType(1);
	}

	pPlayer01->GetInterfaceState()->SetState(1);
	pPlayer02->GetInterfaceState()->SetState(1);

	pPlayer02->TradeRequestResult(1, pPlayer01->GetName(), pPlayer01->GetTotalLevel(), pPlayer01->GetGuildID());
	pPlayer01->TradeRequestResult(1, pPlayer02->GetName(), pPlayer02->GetTotalLevel(), pPlayer02->GetGuildID());

	sLog->outInfo(LOG_TRADE, "TradeRequestAnswer() %s Accepted %s", pPlayer01->BuildLog().c_str(), pPlayer02->BuildLog().c_str());
}

void Player::TradeRequestResult(uint8 result, const char * name, uint16 level, uint32 guild_id)
{
	TRADE_RESULT pMsg(result, name, level, guild_id);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeZen(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(this, InterfaceData::Trade, 1) )
	{
		return;
	}

	if ( !this->GetInterfaceState()->GetTarget() )
		return;

	Player * pPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer();

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::Trade, 1) )
	{
		return;
	}

	POINTER_PCT_LOG(TRADE_ZEN, lpMsg, Packet, 0);

	limitmax(lpMsg->zen, sGameServer->GetTradeMaxZen());

	if ( !this->MoneyHave(lpMsg->zen) )
		return;

	sLog->outInfo(LOG_TRADE, "TradeZen() [%s][%s] -- [%s][%s] -> Add Zen %u from %u ", 
		this->GetAccountData()->GetAccount(), this->GetName(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), lpMsg->zen, this->MoneyGet());

	this->MoneySend(this->MoneyGet() - lpMsg->zen);

	this->TradeZenResult(1);

	this->GetTradeInventory()->SetZen(lpMsg->zen);
	this->GetTradeInventory()->SetButtonEnabled(false);
	pPlayer->GetTradeInventory()->SetButtonEnabled(false);

	pPlayer->TradeButtonOKSend(2);
	this->TradeButtonOKSend(0);
	
	pPlayer->TradeZenAnswer(lpMsg->zen);
}

void Player::TradeZenResult(uint8 result)
{
	TRADE_ZEN_RESULT pMsg(result);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeZenAnswer(uint32 zen)
{
	TRADE_ZEN_ANSWER pMsg(zen);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeButtonOK(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(this, InterfaceData::Trade, 1) )
	{
		return;
	}

	if ( !this->GetInterfaceState()->GetTarget() || !this->GetInterfaceState()->GetTarget()->IsPlayer() )
		return;

	Player* pPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer();

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::Trade, 1) )
	{
		return;
	}

	POINTER_PCT_LOG(TRADE_BUTTON_OK, lpMsg, Packet, 0);

	if ( lpMsg->flag == 1 )
	{
		if ( !this->GetTradeInventory()->IsButtonEnabled() )
		{
			this->GetTradeInventory()->SetButtonEnabled(true);
			pPlayer->TradeButtonOKSend(1);
		}
	}
	else if ( lpMsg->flag == 0 )
	{
		if ( this->GetTradeInventory()->IsButtonEnabled() )
		{
			this->GetTradeInventory()->SetButtonEnabled(false);
			pPlayer->TradeButtonOKSend(0);
		}
	}

	Player::TradeOK(this, pPlayer);
}

void Player::TradeOK(Player* pPlayer01, Player* pPlayer02)
{
	if ( pPlayer01->GetInterfaceState()->GetTarget() != pPlayer02 || 
		 pPlayer02->GetInterfaceState()->GetTarget() != pPlayer01 )
		return;

	if ( !pPlayer01->GetTradeInventory()->IsButtonEnabled() || 
		 !pPlayer02->GetTradeInventory()->IsButtonEnabled() )
		return;

	DuelRoom * room = nullptr;

	if( !sPentagramSystem->CheckExchangePentagramItem(pPlayer01) || 
		!sPentagramSystem->CheckExchangePentagramItem(pPlayer02) )
	{
		Player::TradeCancel(pPlayer01, pPlayer02, 6);
		return;
	}
		
	if ( pPlayer01->MoneyReachMaximum(pPlayer02->GetTradeInventory()->GetZen()) || pPlayer02->MoneyReachMaximum(pPlayer01->GetTradeInventory()->GetZen()) )
	{
		Player::TradeCancel(pPlayer01, pPlayer02, TRADE_INV_OR_ZEN_FULL);
		return;
	}

	if ( !pPlayer01->TradeToInventory() || !pPlayer02->TradeToInventory() )
	{
		Player::TradeCancel(pPlayer01, pPlayer02, TRADE_INV_OR_ZEN_FULL);
		return;
	}

	Player::TradeItemLog(pPlayer01);
	Player::TradeItemLog(pPlayer02);

	pPlayer01->MoneyReduce(pPlayer01->GetTradeInventory()->GetZen());
	pPlayer02->MoneyReduce(pPlayer02->GetTradeInventory()->GetZen());

	sPentagramSystem->ExchangePentagramItem(pPlayer01, pPlayer02);
	sPentagramSystem->ExchangePentagramItem(pPlayer02, pPlayer01);

	pPlayer01->CommitTransaction();
	pPlayer02->CommitTransaction();

	pPlayer01->MoneyAdd(pPlayer02->GetTradeInventory()->GetZen());
	pPlayer02->MoneyAdd(pPlayer01->GetTradeInventory()->GetZen());

	sLog->outInfo(LOG_TRADE, "[Success]: [%s][%s] with [%s][%s]. Money: %u / %u",
		pPlayer01->GetAccountData()->GetAccount(), pPlayer01->GetName(), pPlayer02->GetAccountData()->GetAccount(), pPlayer02->GetName(), 
		pPlayer01->GetTradeInventory()->GetZen(), pPlayer02->GetTradeInventory()->GetZen());

	TransactionDBSave(pPlayer01, pPlayer02);

	pPlayer01->GetInterfaceState()->Reset();
	pPlayer02->GetInterfaceState()->Reset();

	pPlayer01->GetTradeInventory()->Clear();
	pPlayer02->GetTradeInventory()->Clear();

	pPlayer01->TradeResult(TRADE_OK);
	pPlayer02->TradeResult(TRADE_OK);

	pPlayer01->SendInventory();
	pPlayer02->SendInventory();

	sEventInventory->GCEventItemListSend(pPlayer01);
	sEventInventory->GCEventItemListSend(pPlayer02);

	pPlayer01->CashShopSendExpireItem();
	pPlayer02->CashShopSendExpireItem();

	pPlayer01->InventoryItemSend();
	pPlayer02->InventoryItemSend();

	pPlayer01->PreviewSend();
	pPlayer02->PreviewSend();

	sItemMgr->IsUsingTransformationItem(pPlayer01);
	sItemMgr->IsUsingTransformationItem(pPlayer02);

	Player::TransactionSharedSerialCheck(pPlayer01, pPlayer02, "TRADE");

	/*else
	{
		pPlayer01->ViewportCreate(VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
		pPlayer02->ViewportCreate(VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
	}*/
}

bool Player::TradeToInventory()
{
	Player * pPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer();

	trade_loop(i)
	{
		if ( !this->GetTradeInventory()->GetItem(i)->IsItem() )
		{
			continue;
		}

		if ( sItemMgr->GetItemInventoryType(this->GetTradeInventory()->GetItem(i)->GetItem()) == ITEM_INVENTORY_TYPE_EVENT )
		{
			this->GetTradeInventory()->SetTargetSlot(i, pPlayer->GetEventInventory()->AddItem(this->GetTradeInventory()->item[i]));
		}
		else
		{
			this->GetTradeInventory()->SetTargetSlot(i, pPlayer->GetInventory()->AddItem(this->GetTradeInventory()->item[i]));
		}

		if( this->GetTradeInventory()->GetTargetSlot(i) == 0xFF )
		{
			return false;
		}
	}

	return true;
}

void Player::TradeButtonOKSend(uint8 result)
{
	TRADE_BUTTON_OK_ANSWER pMsg(result);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeItemDelete(uint8 slot)
{
	TRADE_ITEM_DELETE pMsg(slot);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeItemDeleteToTarget(uint8 slot)
{
	if ( !this->GetInterfaceState()->GetTarget() )
		return;

	Player * pPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer();

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::Trade )
		return;

	if ( pPlayer->GetInterfaceState()->GetTarget()->ToPlayer()->GetGUID() != this->GetGUID() )
		return;

	pPlayer->TradeItemDelete(slot);
}

void Player::TradeItemAdd(uint8 slot, uint8 * info)
{
	TRADE_ITEM_ADD pMsg(slot,info);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeItemAddToTarget(uint8 slot, uint8 * info)
{
	if ( !this->GetInterfaceState()->GetTarget() )
		return;

	Player * pPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer();

	if ( !pPlayer || pPlayer->GetInterfaceState()->GetID() != InterfaceData::Trade )
		return;

	if ( pPlayer->GetInterfaceState()->GetTarget()->ToPlayer()->GetGUID() != this->GetGUID() )
		return;

	pPlayer->TradeItemAdd(slot, info);
}

void Player::TradeCancelRequest()
{
	if ( !Player::InterfaceCheck(this, InterfaceData::Trade, 1) )
	{
		return;
	}

	Player* pPlayer = this->GetInterfaceState()->GetTarget() ? this->GetInterfaceState()->GetTarget()->ToPlayer():  nullptr;

	if ( pPlayer )
	{
		pPlayer->TradeCancel();
		pPlayer->TradeResult(TRADE_RESULT_CANCEL);
	}

	this->TradeCancel();
	this->TradeResult(TRADE_RESULT_CANCEL);

	sLog->outInfo(LOG_TRADE, "TradeCancelRequest() %s Canceled %s", this->BuildLog().c_str(), pPlayer ? pPlayer->BuildLog().c_str(): "NONE PLAYER");
}

void Player::TradeCancel()
{
	if ( !Player::InterfaceCheck(this, InterfaceData::Trade, 1) )
	{
		return;
	}

	this->RollBackTransaction();
	this->GetInterfaceState()->Reset();
	this->GetTradeInventory()->Clear();
	this->MoneySend();
	this->SendInventory();

	sEventInventory->GCEventItemListSend(this);

	this->CashShopSendExpireItem();

	this->InventoryItemSend();

	this->PreviewSend();
	this->SendSkill();
	sItemMgr->IsUsingTransformationItem(this);
	//this->ViewportCreate(VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);

	sLog->outInfo(LOG_TRADE, "TradeCancel() %s", this->BuildLog().c_str());
}

void Player::TradeResult(uint8 result)
{
	TRADE_CANCEL pMsg(result);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::TradeCancel(Player* pPlayer01, Player* pPlayer02, uint8 result)
{
	pPlayer01->TradeCancel();
	pPlayer02->TradeCancel();
	pPlayer01->TradeResult(result);
	pPlayer02->TradeResult(result);
}

void Player::TradeItemLog(Player* pPlayer)
{
	Player * pTarget = pPlayer->GetInterfaceState()->GetTarget()->ToPlayer();

	std::string item_data;

	trade_loop(i)
	{
		Item const* pItem = pPlayer->GetTradeInventory()->GetItem(i);

		if ( !pItem )
		{
			continue;
		}

		if ( !pItem->IsItem() )
		{
			continue;
		}

		std::string item_log = pItem->BuildLog(i);

		sLog->outInfo(LOG_TRADE, "[%s][%s] Trade Item  Opposite :[%s][%s] Item -> %s ",
			pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), 
			pTarget->GetAccountData()->GetAccount(), pTarget->GetName(),
			item_log.c_str());

		if (item_data.empty())
		{
			item_data = "[";
		}
		else
		{
			item_data = item_data + ",";
		}
		
		item_data = item_data + pItem->BuildLogDB(i, false);
	}

	if (!item_data.empty())
	{
		item_data = item_data + "]";
	}

	TradeBuyLog(pPlayer, pTarget, item_data);
}

void Player::TradeBuyLog(Player* pPlayer01, Player* pPlayer02, std::string const& log)
{
	if (!sGameServer->IsLogDBTrade())
	{
		return;
	}

	if (!pPlayer01 || !pPlayer02)
	{
		return;
	}

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_LOG_TRADE);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, pPlayer01->GetAccountData()->GetGUID());
	stmt->setString(2, pPlayer01->GetAccountData()->GetAccount());
	stmt->setUInt32(3, pPlayer01->GetGUID());
	stmt->setString(4, pPlayer01->GetName());
	stmt->setString(5, pPlayer01->BuildLogDB());
	stmt->setUInt32(6, pPlayer02->GetAccountData()->GetGUID());
	stmt->setString(7, pPlayer02->GetAccountData()->GetAccount());
	stmt->setUInt32(8, pPlayer02->GetGUID());
	stmt->setString(9, pPlayer02->GetName());
	stmt->setString(10, pPlayer02->BuildLogDB());
	stmt->setUInt32(11, pPlayer01->GetTradeInventory()->GetZen());
	stmt->setString(12, log);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}