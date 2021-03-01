GremoryCase::GremoryCase()
{

}

GremoryCase::~GremoryCase()
{

}

void GremoryCase::OpenRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	POINTER_PCT_CHECK(GREMORY_CASE_OPEN, lpMsg, Packet, 0);
	this->OpenResult(pPlayer, 3);
}

void GremoryCase::OpenResult(Player* pPlayer, uint8 result)
{
	GREMORY_CASE_OPEN_RESULT pMsg(result);
	pPlayer->SendPacket(&pMsg);
}

void GremoryCase::ItemAdd(Player* pPlayer, uint8 inventory, uint8 slot)
{
	if (inventory >= GREMORY_CASE_TYPE_MAX)
	{
		return;
	}

	auto pGremoryCase = pPlayer->GetGremoryCase(inventory);
	if (!pGremoryCase)
	{
		return;
	}

	if (!pGremoryCase->IsSlotInRange(slot))
	{
		return;
	}

	auto pItem = pGremoryCase->GetItem(slot);

	if (!pItem || !pItem->IsItem())
	{
		return;
	}

	GREMORY_CASE_ITEM_ADD pMsg;
	pMsg.item.inventory = inventory + 1;
	pMsg.item.reward_type = pItem->GetPersonalStorePrice();
	pItem->ConvertToBuffer(pMsg.item.item_info);
	pMsg.item.slot = slot;
	pMsg.item.serial = 0;
	pMsg.item.time = pItem->GetExpireDate();

	pPlayer->SendPacket(&pMsg);
}
	
void GremoryCase::ItemDel(Player* pPlayer, uint8 inventory, uint8 slot)
{
	if (inventory >= GREMORY_CASE_TYPE_MAX)
	{
		return;
	}

	auto pGremoryCase = pPlayer->GetGremoryCase(inventory);
	if (!pGremoryCase)
	{
		return;
	}

	if (!pGremoryCase->IsSlotInRange(slot))
	{
		return;
	}

	auto pItem = pGremoryCase->GetItem(slot);

	if (!pItem)
	{
		return;
	}

	GREMOERY_CASE_ITEM_REMOVE pMsg;
	pMsg.inventory = inventory + 1;
	pMsg.item = pItem->GetItem();
	pMsg.serial = 0;
	pMsg.slot = slot;

	pPlayer->SendPacket(&pMsg);
}

void GremoryCase::ItemUseRequest(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer))
	{
		return;
	}

	if (pPlayer->GetInterfaceState()->GetID() != InterfaceData::None)
	{
		return;
	}

	if (pPlayer->GetTransaction() == TRANSACTION_TYPE_BEGIN)
	{
		return;
	}

	if (!pPlayer->IsAuthorizationEnabled())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_CHECK(GREMORY_CASE_ITEM_USE, lpMsg, Packet, 0);

	uint8 inventory = lpMsg->inventory - 1;
	if (inventory >= GREMORY_CASE_TYPE_MAX)
	{
		this->ItemUseResult(pPlayer, 1, -1, -1);
		return;
	}

	auto pGremoryCase = pPlayer->GetGremoryCase(inventory);
	if (!pGremoryCase)
	{
		this->ItemUseResult(pPlayer, 1, -1, -1);
		return;
	}

	if (!pGremoryCase->IsSlotInRange(lpMsg->slot))
	{
		this->ItemUseResult(pPlayer, 1, -1, -1);
		return;
	}

	auto pItem = pGremoryCase->GetItem(lpMsg->slot);

	if (!pItem || !pItem->IsItem() || pItem->GetItem() != lpMsg->item)
	{
		this->ItemUseResult(pPlayer, 1, -1, -1);
		return;
	}

	switch (sItemMgr->GetItemInventoryType(pItem->GetItem()))
	{
	case ITEM_INVENTORY_TYPE_NORMAL:
	{
									   if (sGameServer->IsGremoryCaseFreeSpaceCheck() && !pPlayer->GetInventory()->IsEmptySpace(4, 4))
									   {
										   this->ItemUseResult(pPlayer, 4, -1, -1);
										   return;
									   }

									   uint8 result = pPlayer->GetInventory()->AddItem(*pItem);

									   if (result == uint8(-1))
									   {
										   this->ItemUseResult(pPlayer, 4, -1, -1);
										   return;
									   }

									   pPlayer->GetInventory()->GetItem(result)->SetExpireDate(0);
									   pPlayer->GetInventory()->GetItem(result)->Convert();
									   pPlayer->SendInventory(result);

									   sLog->outInfo(LOG_ITEMS, "%s -- %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->GetInventory()->GetItem(result)->BuildLog(result).c_str());
	} break;

	case ITEM_INVENTORY_TYPE_EVENT:
	{
									  uint8 result = pPlayer->GetEventInventory()->AddItem(*pItem);

									  if (result == uint8(-1))
									  {
										  this->ItemUseResult(pPlayer, 4, -1, -1);
										  return;
									  }

									  pPlayer->GetEventInventory()->GetItem(result)->SetExpireDate(0);
									  pPlayer->GetEventInventory()->GetItem(result)->Convert();
									  sEventInventory->GCEventItemModifySend(pPlayer, result);

									  sLog->outInfo(LOG_ITEMS, "%s -- %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->GetEventInventory()->GetItem(result)->BuildLog(result).c_str());
	} break;

	case ITEM_INVENTORY_TYPE_MUUN:
	{
									 uint8 result = pPlayer->GetMuunInventory()->AddItem(*pItem);

									 if (result == uint8(-1))
									 {
										 this->ItemUseResult(pPlayer, 4, -1, -1);
										 return;
									 }

									 pPlayer->GetMuunInventory()->GetItem(result)->SetExpireDate(0);
									 pPlayer->GetMuunInventory()->GetItem(result)->Convert();
									 sMuunSystem->GCMuunItemModifySend(pPlayer, result);

									 sLog->outInfo(LOG_ITEMS, "%s -- %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pPlayer->GetMuunInventory()->GetItem(result)->BuildLog(result).c_str());
	} break;
	}

	this->ItemUseResult(pPlayer, 0, inventory, lpMsg->slot);
	this->ItemDel(pPlayer, inventory, lpMsg->slot);
	pGremoryCase->DeleteItem(lpMsg->slot);
}

void GremoryCase::ItemUseResult(Player* pPlayer, uint8 result, uint8 inventory, uint8 slot)
{
	if (inventory >= GREMORY_CASE_TYPE_MAX)
	{
		return;
	}

	GREMOERY_CASE_ITEM_USE_RESULT pMsg;
	pMsg.result = result;

	if (result == 0)
	{
		pMsg.result = 1;

		auto pGremoryCase = pPlayer->GetGremoryCase(inventory);
		if (pGremoryCase && pGremoryCase->IsSlotInRange(slot))
		{
			auto pItem = pGremoryCase->GetItem(slot);

			if (pItem)
			{
				pMsg.inventory = inventory + 1;
				pMsg.item = pItem->GetItem();
				pMsg.serial = 0;
				pMsg.slot = slot;
				pMsg.result = 0;
			}
		}
	}

	pPlayer->SendPacket(&pMsg);
}

void GremoryCase::ItemListRequest(Player* pPlayer, uint8 * Packet)
{
	GREMORY_CASE_LIST pMsg(0);
	pPlayer->SendPacket(&pMsg);
}

void GremoryCase::ItemListSend(Player* pPlayer, uint8 inventory)
{
	if (inventory >= GREMORY_CASE_TYPE_MAX)
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(GREMORY_CASE_ITEM_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(GREMORY_CASE_ITEM_LIST_BODY, body, buffer, sizeof(GREMORY_CASE_ITEM_LIST_HEAD));
	head->count = 0;

	auto pGremoryCase = pPlayer->GetGremoryCase(inventory);
	if (pGremoryCase)
	{
		for (int32 i = 0; i < pGremoryCase->GetItemSize(); ++i)
		{
			auto pItem = pGremoryCase->GetItem(i);

			if (pItem && pItem->IsItem())
			{
				body[head->count].reward_type = pItem->GetPersonalStorePrice();
				body[head->count].inventory = inventory + 1;
				pItem->ConvertToBuffer(body[head->count].item_info);
				body[head->count].time = pItem->GetExpireDate();
				body[head->count].serial = 0;
				body[head->count].slot = i;
				body[head->count].unk3 = 0;
				memset(body[head->count].unk4, 0, 12);
				++head->count;
			}
		}
	}

	if (head->count <= 0)
	{
		return;
	}

	head->Set(HEADCODE_GREMORY_CASE, SUBCODE_GREMORY_CASE_ITEM_LIST, sizeof(GREMORY_CASE_ITEM_LIST_HEAD) + (head->count * sizeof(GREMORY_CASE_ITEM_LIST_BODY)));
	pPlayer->sendPacket(buffer);
}

void GremoryCase::SendNotification(Player* pPlayer, uint8 result)
{
	GREMORY_CASE_NOTIFY pMsg(result);
	pPlayer->SendPacket(&pMsg);
}

void GremoryCase::Update(Player* pPlayer)
{
	if (pPlayer->GetInterfaceState()->GetID() != InterfaceData::None)
	{
		return;
	}

	if (!pPlayer->IsLive())
	{
		return;
	}

	time_t current_time = time(nullptr);

	for (int32 inventory = 0; inventory < GREMORY_CASE_TYPE_MAX; ++inventory)
	{
		auto pGremoryCase = pPlayer->GetGremoryCase(inventory);
		for (int32 i = 0; i < pGremoryCase->GetItemSize(); ++i)
		{
			auto pItem = pGremoryCase->GetItem(i);

			if (!pItem)
			{
				continue;
			}

			if (!pItem->IsItem())
			{
				continue;
			}

			if (!pItem->GetExpireDate())
			{
				continue;
			}

			if (pItem->IsExpired())
			{
				this->ItemDel(pPlayer, inventory, i);
				pGremoryCase->DeleteItem(i);
			}
			else
			{
				time_t diff = pItem->GetExpireDate() - current_time;

				if (diff < DAY)
				{
					int32 id = diff / HOUR;

					if (id <= 6 && pItem->GetExpireNotify() != id)
					{
						pItem->SetExpireNotify(id);
						this->SendNotification(pPlayer, 1);
					}
				}
			}
		}
	}
}