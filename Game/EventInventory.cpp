#include <atltime.h>

EventInventory::EventInventory()
{

}

EventInventory::~EventInventory()
{

}

bool EventInventory::EventInventoryInsertItemStack(Player* pPlayer, Item * pItem)
{
	if ( !pItem->IsItem() )
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(pItem->GetItem());

	if ( !item_info )
	{
		return false;
	}

	if ( !item_info->GetStackData()->IsStackOnLoot() )
	{
		return false;
	}

	int32 MaxStack = item_info->GetStackData()->GetMaxStack();

	for( int32 n = 0; n < EVENT_INVENTORY_SIZE; ++n )
	{
		if ( !pPlayer->GetEventInventory()->GetItem(n)->IsItem() )
		{
			continue;
		}

		if ( pPlayer->GetEventInventory()->GetItem(n)->GetItem() != pItem->GetItem() ||
			 pPlayer->GetEventInventory()->GetItem(n)->GetLevel() != pItem->GetLevel() ||
			 pPlayer->GetEventInventory()->GetItem(n)->GetSocketBonus() != pItem->GetSocketBonus() )
		{
			continue;
		}

		if ( pPlayer->GetEventInventory()->GetItem(n)->GetDurability() >= MaxStack )
		{
			continue;
		}

		int32 AddDur = (int32)(MaxStack - pPlayer->GetEventInventory()->GetItem(n)->GetDurability());

		AddDur = ((AddDur>pItem->GetDurability())?(int32)pItem->GetDurability() : AddDur);
		
		pItem->AlterDurability(0, AddDur);
		pPlayer->GetEventInventory()->GetItem(n)->AlterDurability(1, AddDur);

		if( pPlayer->GetEventInventory()->GetItem(n)->GetDurability() == MaxStack && item_info->GetStackData()->GetConvertedItem() )
		{
			pPlayer->GetEventInventory()->DeleteItem(n);
			this->GCEventItemDeleteSend(pPlayer, n);

			sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(item_info->GetStackData()->GetConvertedItem()->GetItem(), pItem->GetLevel(), 1, 0, 0, 0, 0, 0, nullptr, pItem->GetSocketBonus()));

			if(pItem->GetDurability() < 1){return true;}
		}
		else
		{
			this->GCEventItemDurSend(pPlayer, n, (uint8)pPlayer->GetEventInventory()->GetItem(n)->GetDurability());
			if(pItem->GetDurability() < 1){return true;}
		}
	}

	return false;
}

bool EventInventory::EventInventoryAddItemStack(Player* pPlayer, uint8 source_slot, uint8 target_slot)
{
	Item * pSourceItem = pPlayer->GetEventInventory()->GetItem(source_slot);
	Item * pTargetItem = pPlayer->GetEventInventory()->GetItem(target_slot);

	if ( !pSourceItem->IsItem() || !pTargetItem->IsItem() )
	{
		return false;
	}

	if ( pSourceItem->GetItem() != pTargetItem->GetItem() ||
		 pSourceItem->GetLevel() != pTargetItem->GetLevel() ||
		 pSourceItem->GetSocketBonus() != pTargetItem->GetSocketBonus() )
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(pSourceItem->GetItem());

	if ( !item_info )
	{
		return false;
	}

	int32 MaxStack = item_info->GetStackData()->GetMaxStack();

	if ( MaxStack <= 0 )
	{
		return false;
	}

	if( pTargetItem->GetDurability() >= MaxStack )
	{
		return false;
	}

	int32 AddDur = (int32)(MaxStack - pTargetItem->GetDurability());

	AddDur = ((AddDur > pSourceItem->GetDurability())?(int32)pSourceItem->GetDurability():AddDur);

	pSourceItem->AlterDurability(0, AddDur);
	pTargetItem->AlterDurability(1, AddDur);

	pPlayer->ItemMoveResult(-1);

	if( pTargetItem->GetDurability() == MaxStack && item_info->GetStackData()->GetConvertedItem() )
	{
		pPlayer->GetEventInventory()->DeleteItem(target_slot);
		this->GCEventItemDeleteSend(pPlayer, target_slot);

		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(item_info->GetStackData()->GetConvertedItem()->GetItem(), pSourceItem->GetLevel(), 1, 0, 0, 0, 0, 0, nullptr, pSourceItem->GetSocketBonus()));
	}
	else
	{
		this->GCEventItemDurSend(pPlayer, target_slot, (BYTE)pTargetItem->GetDurability());
	}

	if( pSourceItem->GetDurability() < 1.0f )
	{
		pPlayer->GetEventInventory()->DeleteItem(source_slot);
		this->GCEventItemDeleteSend(pPlayer, source_slot);
	}
	else
	{
		this->GCEventItemDurSend(pPlayer, source_slot, (uint8)pSourceItem->GetDurability());
	}

	return true;
}

bool EventInventory::CGEventItemGetRecv(Player* pPlayer, uint8 * Packet)
{
	PMSG_EVENT_ITEM_GET_SEND pMsg;
	pMsg.result = 0xFF;
	pMsg.SetIndex(-1);
	memset(pMsg.item_info, 0, MAX_ITEM_INFO);

	if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_LOOT) )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	if ( !sGameServer->IsEventInventoryEnabled() )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	POINTER_PCT_LOG(PMSG_EVENT_ITEM_GET_RECV, lpMsg, Packet, 0);

	uint16 index = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	World* pWorld = pPlayer->GetWorld();

	if ( !pWorld )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	WorldItem* world_item = pWorld->GetItem(index);

	if ( !world_item )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	if ( !world_item->GiveCheck(pPlayer) )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	if ( sItemMgr->GetItemInventoryType(world_item->item.GetItem()) != ITEM_INVENTORY_TYPE_EVENT )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	pMsg.SetIndex(world_item->GetEntry());

	if( !this->EventInventoryInsertItemStack(pPlayer, &world_item->item) )
	{
		if((pMsg.result=pPlayer->GetEventInventory()->AddItem(world_item->item)) == 0xFF)
		{
			pPlayer->SEND_PCT(pMsg);
			return false;
		}

		world_item->Give();
		world_item->item.ConvertToBuffer(pMsg.item_info);

		pPlayer->SEND_PCT(pMsg);

		pPlayer->PartyItemGetNotify(world_item);
	}
	else
	{
		world_item->Give();
		pMsg.result = 0xFD;
		pPlayer->SEND_PCT(pMsg);
	}

	return true;
}

void EventInventory::CGEventItemDropRecv(Player* pPlayer, uint8 * Packet)
{
	POINTER_PCT_LOG(PMSG_EVENT_ITEM_DROP_RECV, lpMsg, Packet, 0);

	PMSG_EVENT_ITEM_DROP_SEND pMsg;
	pMsg.result = 0;
	pMsg.slot = lpMsg->slot;

	if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_DROP) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !sGameServer->IsEventInventoryEnabled() )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if( !EVENT_INVENTORY_RANGE(lpMsg->slot) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	Item * pItem = pPlayer->GetEventInventory()->GetItem(lpMsg->slot);

	if ( !pItem->IsItem() )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(pItem->GetItem());

	if ( !item_info )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_DROP) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( sItemMgr->GetItemInventoryType(pItem->GetItem()) != ITEM_INVENTORY_TYPE_EVENT )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	World* pWorld = pPlayer->GetWorld();

	if ( !sWorldMgr->IsItemDropAllowed(pPlayer, pWorld, lpMsg->x, lpMsg->y) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if( sSummonScroll->CheckSummonScroll(pItem->GetItem()) )
	{
		if( !sSummonScroll->CreateSummonScrollMonster(pPlayer, pItem->GetItem(), pPlayer->GetWorldId(), lpMsg->x, lpMsg->y) )
		{
			pPlayer->SEND_PCT(pMsg);
		}
		else
		{
			pPlayer->GetEventInventory()->DeleteItem(lpMsg->slot);
			pMsg.result = 1;
			pPlayer->SEND_PCT(pMsg);
		}

		return;
	}

	uint8 is_box_drop = sItemBagMgr->ExecuteItemBag(ITEM_BAG_ITEM, pPlayer, "", 
		pItem->GetItem(), pItem->GetLevel(), lpMsg->x, lpMsg->y, pItem->GetSerialServer(), pItem->GetSerial(), pItem->GetName());

	if ( is_box_drop == ITEM_BAG_RESULT_SUCCESS )
	{
		pPlayer->GetEventInventory()->DeleteItem(lpMsg->slot);
		pMsg.result = 1;
		pPlayer->SEND_PCT(pMsg);
		return;
	}
	else if ( is_box_drop == ITEM_BAG_RESULT_LOW_LEVEL )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	is_box_drop = sItemBagMgr->ExecuteItemBag(ITEM_BAG_GROUP_ITEM, pPlayer, "", 
		pItem->GetItem(), pItem->GetLevel(), lpMsg->x, lpMsg->y, pItem->GetSerialServer(), pItem->GetSerial(), pItem->GetName());

	if ( is_box_drop == ITEM_BAG_RESULT_SUCCESS )
	{
		pPlayer->GetEventInventory()->DeleteItem(lpMsg->slot);
		pMsg.result = 1;
		pPlayer->SEND_PCT(pMsg);
		return;
	}
	else if ( is_box_drop == ITEM_BAG_RESULT_LOW_LEVEL )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	uint8 is_evomon = sEvomon->ScrollDrop(pPlayer, pItem, lpMsg->x, lpMsg->y);

	if ( is_evomon == 1 )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}
	else if ( is_evomon == 2 )
	{
		pPlayer->GetEventInventory()->DeleteItem(lpMsg->slot);
		pMsg.result = 1;
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !pWorld->add_item(pPlayer->GetEventInventory()->item[lpMsg->slot], pPlayer, lpMsg->x, lpMsg->y) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	pPlayer->GetEventInventory()->DeleteItem(lpMsg->slot);
	
	pMsg.result = 1;
	pPlayer->SEND_PCT(pMsg);
}

void EventInventory::CGEventInventoryOpenRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !sGameServer->IsEventInventoryEnabled() )
	{
		return;
	}

	POINTER_PCT_LOG(PMSG_EVENT_INVENTORY_OPEN_RECV, lpMsg, Packet, 0);

	if ( sGameServer->IsMuRoomyEnabled() && sMuRoomy->IsActive() && lpMsg->id == EVENT_INVENTORY_TYPE_MU_ROOMY )
	{
		sMuRoomy->Open(pPlayer);
		return;
	}

	if ( sGameServer->IsJewelBingoEnabled() && sJewelBingo->IsActive() && lpMsg->id == EVENT_INVENTORY_TYPE_JEWEL_BINGO )
	{
		sJewelBingo->Open(pPlayer);
	}

	if (sGameServer->IsNumericBaseballEnabled() && sNumericBaseball->IsActive() && lpMsg->id == EVENT_INVENTORY_TYPE_BASEBALL)
	{
		sNumericBaseball->Open(pPlayer);
	}

	PMSG_EVENT_INVENTORY_OPEN_SEND pMsg;
	pMsg.result = 3;
	pMsg.id = lpMsg->id;
	pPlayer->SEND_PCT(pMsg);
}

void EventInventory::GCEventItemListSend(Player* pPlayer)
{
	uint8 buffer[8192];
	POINTER_PCT(PMSG_EVENT_ITEM_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(PMSG_EVENT_ITEM_LIST_BODY, body, buffer, sizeof(PMSG_EVENT_ITEM_LIST_HEAD));
	head->count = 0;

	for( int32 n = 0; n < EVENT_INVENTORY_SIZE; ++n )
	{
		if( pPlayer->GetEventInventory()->GetItem(n)->IsItem() )
		{
			body[head->count].slot = n;
			pPlayer->GetEventInventory()->GetItem(n)->ConvertToBuffer(body[head->count].item_info);

			++head->count;
		}
	}

	head->h.setE(0x4D, 0x02, sizeof(PMSG_EVENT_ITEM_LIST_HEAD) + (head->count * sizeof(PMSG_EVENT_ITEM_LIST_BODY)));

	pPlayer->sendPacket(buffer, head->h.get_size());
}

void EventInventory::GCEventItemDurSend(Player* pPlayer, uint8 slot, uint8 dur)
{
	PMSG_EVENT_ITEM_DUR_SEND pMsg;
	pMsg.slot = slot;
	pMsg.dur = dur;

	pPlayer->SEND_PCT(pMsg);
}

void EventInventory::GCEventItemDeleteSend(Player* pPlayer, uint8 slot)
{
	PMSG_EVENT_ITEM_DELETE_SEND pMsg;
	pMsg.slot = slot;
	pPlayer->SEND_PCT(pMsg);
}

void EventInventory::GCEventItemModifySend(Player* pPlayer, uint8 slot)
{
	if ( !EVENT_INVENTORY_RANGE(slot) )
	{
		return;
	}

	if ( !pPlayer->GetEventInventory()->GetItem(slot)->IsItem() )
	{
		return;
	}

	PMSG_EVENT_ITEM_MODIFY_SEND pMsg;
	pMsg.slot = slot;

	pPlayer->GetEventInventory()->GetItem(slot)->ConvertToBuffer(pMsg.item_info);

	pPlayer->SEND_PCT(pMsg);
}

/*void EventInventory::GCEventItemChangeSend(Player* pPlayer, uint8 slot)
{
	if ( !EVENT_INVENTORY_RANGE(slot) )
	{
		return;
	}

	PMSG_EVENT_ITEM_CHANGE_SEND pMsg;
	pMsg.index[0] = SET_NUMBERH(pPlayer->GetEntry());
	pMsg.index[1] = SET_NUMBERL(pPlayer->GetEntry());
	pPlayer->GetEventInventory()->GetItem(slot)->ConvertToBuffer(pMsg.item_info);
	pMsg.item_info[1] = slot * 16;
	pMsg.item_info[1] |= ((pPlayer->GetEventInventory()->GetItem(slot)->GetLevel() - 1) / 2) & 0x0F;

	pPlayer->sendPacket_viewport(MAKE_PCT(pMsg));
}*/

void EventInventory::GCEventInventoryEnableSend(Player* pPlayer)
{
	if ( !sGameServer->IsEventInventoryEnabled() )
	{
		return;
	}

	PMSG_EVENT_INVENTORY_ENABLE_SEND pMsg;

	if ( sMuunSystem->IsActive() )
	{
		pMsg.result = EVENT_INVENTORY_TYPE_EGG;
		pPlayer->SEND_PCT(pMsg);
	}

	if ( sGameServer->IsEvomonEnabled() && sEvomon->IsActive() && sGameServer->IsEvomonSend() )
	{
		pMsg.result = EVENT_INVENTORY_TYPE_EVOMON;
		pPlayer->SEND_PCT(pMsg);
	}

	if ( sGameServer->IsMuRoomyEnabled() && sMuRoomy->IsActive() )
	{
		pMsg.result = EVENT_INVENTORY_TYPE_MU_ROOMY;
		pPlayer->SEND_PCT(pMsg);
	}

	if ( sGameServer->IsMiniBombEnabled() && sMiniBomb->IsActive() )
	{
		pMsg.result = EVENT_INVENTORY_TYPE_FIND_BOMB;
		pPlayer->SEND_PCT(pMsg);
	}

	if ( sGameServer->IsJewelBingoEnabled() && sJewelBingo->IsActive() )
	{
		pMsg.result = EVENT_INVENTORY_TYPE_JEWEL_BINGO;
		pPlayer->SEND_PCT(pMsg);
	}

	if (sGameServer->IsNumericBaseballEnabled() && sNumericBaseball->IsActive())
	{
		pMsg.result = EVENT_INVENTORY_TYPE_BASEBALL;
		pPlayer->SEND_PCT(pMsg);
	}
}

void EventInventory::GCEventInventoryExpireSend(Player* pPlayer)
{

}
	
void EventInventory::GCEventInventoryExpireSend(Player* pPlayer, uint8 slot)
{

}

void EventInventory::DecreaseItemDurabilityByUse(Player* pPlayer, uint8 slot, float durability)
{
	if ( !EVENT_INVENTORY_RANGE(slot) )
	{
		return;
	}

	if ( !pPlayer->GetEventInventory()->GetItem(slot)->IsItem() )
	{
		return;
	}

	pPlayer->GetEventInventory()->GetItem(slot)->AlterDurability(0, durability);

	if ( pPlayer->GetEventInventory()->GetItem(slot)->GetDurability() > 0.0f )
	{
		this->GCEventItemDurSend(pPlayer, slot, pPlayer->GetEventInventory()->GetItem(slot)->GetDurability());
	}
	else
	{
		pPlayer->GetEventInventory()->DeleteItem(slot);
		this->GCEventItemDeleteSend(pPlayer, slot);
	}
}