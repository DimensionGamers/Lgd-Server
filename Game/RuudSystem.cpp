RuudSystem::RuudSystem()
{

}

RuudSystem::~RuudSystem()
{

}

void RuudSystem::ItemBuyRequest(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer))
	{
		return;
	}

	if (!pPlayer->GetInterfaceState()->GetTarget())
	{
		this->ItemBuyResult(pPlayer, 254);
		return;
	}

	if (!pPlayer->IsAuthorizationEnabled())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->ItemBuyResult(pPlayer);
		return;
	}

	pPlayer->Ping();

	POINTER_PCT_LOG(RUUD_ITEM_BUY, lpMsg, Packet, 0);

	if (!shop_range(lpMsg->slot))
	{
		this->ItemBuyResult(pPlayer);
		return;
	}

	Monster* mNpc = pPlayer->GetInterfaceState()->GetTarget()->ToCreature();

	if (!mNpc)
	{
		this->ItemBuyResult(pPlayer, 254);
		return;
	}

	Shop const* shop = sShopMgr->GetShop(mNpc->GetNpcFunction());

	if (!shop)
	{
		this->ItemBuyResult(pPlayer, 254);
		return;
	}

	if (shop->GetItem(lpMsg->slot)->position == uint8(-1))
	{
		this->ItemBuyResult(pPlayer);
		return;
	}

	if (!sShopMgr->EnableToBuy(pPlayer, shop))
	{
		this->ItemBuyResult(pPlayer);
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(shop->GetItem(lpMsg->slot)->GetItem());

	if (!item_info)
	{
		this->ItemBuyResult(pPlayer);
		return;
	}

	if (pPlayer->GetRuudMoney() < item_info->GetRuudPrice())
	{
		this->ItemBuyResult(pPlayer, 252);
		return;
	}

	if (!pPlayer->GetInventory()->IsEmptySpace(item_info->GetX(), item_info->GetY()))
	{
		this->ItemBuyResult(pPlayer, 251);
		return;
	}

	if (!sMonsterSoul->IsPurchaseAllowed(pPlayer, shop->GetItem(lpMsg->slot)))
	{
		return;
	}

	sItemMgr->ItemSerialCreateRuudShop(pPlayer, shop->GetID(), lpMsg->slot);
}

void RuudSystem::ItemBuyResult(Player* pPlayer, uint8 result)
{
	RUUD_ITEM_BUY_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}