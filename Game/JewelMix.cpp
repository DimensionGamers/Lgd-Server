/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "JewelMix.cpp"
*
*/

JewelMix::JewelMix()
{

}

JewelMix::~JewelMix()
{
	LIST_CLEAR(JewelMixDataList::iterator, this->m_jewel_mix);
}

void JewelMix::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Jewel Mix...");

	LIST_CLEAR(JewelMixDataList::iterator, this->m_jewel_mix);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM mix_jewel_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			JewelMixData * add_jewel = new JewelMixData;

			add_jewel->id.set(fields[0].GetUInt8());
			add_jewel->type.set(fields[1].GetUInt8());
			add_jewel->index.set(fields[2].GetUInt16());
			add_jewel->level.set(fields[3].GetUInt8());
			add_jewel->count.set(fields[4].GetUInt8());
			add_jewel->mix_money.set(fields[5].GetUInt32());
			add_jewel->unmix_money.set(fields[6].GetUInt32());
			add_jewel->pack_type.set(fields[7].GetUInt8());
			add_jewel->pack_index.set(fields[8].GetUInt16());
			add_jewel->flags.set(fields[9].GetUInt8());
		
			this->m_jewel_mix.push_back(add_jewel);
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u jewel mix definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

JewelMixData const* JewelMix::GetJewelData(uint8 id, uint8 level) const
{
	for ( JewelMixDataList::const_iterator it = this->m_jewel_mix.begin(); it != this->m_jewel_mix.end(); it++ )
	{
		if ( (*it)->id == id && (*it)->level == level )
			return (*it);
	}

	return nullptr;
}

JewelMixData const* JewelMix::GetJewelData(Item const* pItem) const
{
	for (JewelMixDataList::const_iterator itr = this->m_jewel_mix.begin(); itr != this->m_jewel_mix.end(); ++itr)
	{
		JewelMixData const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetPackItem() == pItem->GetItem() && pData->level.get() == pItem->GetLevel())
		{
			return pData;
		}
	}

	return nullptr;
}

void JewelMix::Mix(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::MixJewel || pPlayer->GetTransaction() == TRANSACTION_TYPE_BEGIN )
	{
		sLog->outError("jewel", "[ %s ] Wrong Interface --- %s", __FUNCTION__, pPlayer->BuildLog().c_str());

		this->MixResult(pPlayer, 6);
		return;
	}

	if ( pPlayer->GetPersonalStore()->IsBusy() )
	{
		sLog->outError("jewel", "[ %s ] PersonalStore --- %s", __FUNCTION__, pPlayer->BuildLog().c_str());

		this->MixResult(pPlayer, 6);
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->MixResult(pPlayer, 6);
		return;
	}

	POINTER_PCT_LOG(JEWEL_MIX_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->type == 11 )
	{
		this->MixElementalSymbols(pPlayer, Packet);
		return;
	}

	JewelMixData const* pJewelData = this->GetJewelData(lpMsg->type, lpMsg->level);

	if ( !pJewelData || !pJewelData->flagHas(JEWEL_MIX_FLAG_MIX) )
	{
		sLog->outError("jewel", "[ %s ] Wrong Data: [%d / %d] --- %s", __FUNCTION__, lpMsg->type, lpMsg->level, pPlayer->BuildLog().c_str());

		this->MixResult(pPlayer, 0);
		return;
	}

	if ( pJewelData->count <= 0 )
	{
		this->MixResult(pPlayer, 0);
		return;
	}

	if ( !pPlayer->MoneyHave(pJewelData->mix_money.get()) )
	{
		this->MixResult(pPlayer, 5);
		return;
	}

	int32 ccount = 0;

	use_inventory_loop(i)
	{
		if ( !pPlayer->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		if ( !pPlayer->GetInventory()->GetItem(i)->IsItem() )
		{
			continue;
		}

		if ( pPlayer->GetInventory()->GetItem(i)->GetItem() != pJewelData->GetItem() )
		{
			continue;
		}

		ccount++;

		if ( pJewelData->count <= ccount )
			break;
	}

	if ( pJewelData->count > ccount )
	{
		this->MixResult(pPlayer, 0);
		return;
	}

	ccount = 0;

	use_inventory_loop(i)
	{
		if ( !pPlayer->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		if ( !pPlayer->GetInventory()->GetItem(i)->IsItem() )
		{
			continue;
		}

		if ( pPlayer->GetInventory()->GetItem(i)->GetItem() != pJewelData->GetItem() )
		{
			continue;
		}

		pPlayer->ClearItem(i);
		ccount++;

		if ( pJewelData->count <= ccount )
			break;
	}

	sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(pJewelData->GetPackItem(), pJewelData->level.get()));

	pPlayer->MoneyReduce(pJewelData->mix_money.get());
	this->MixResult(pPlayer, 1);

	sLog->outInfo("jewel", "[ %s ] Success: [%d / %d] --- %s", __FUNCTION__, lpMsg->type, lpMsg->level, pPlayer->BuildLog().c_str());

	this->LogDB(pPlayer, 0, lpMsg->type, lpMsg->level);
}

void JewelMix::MixResult(Player* pPlayer, uint8 result)
{
	JEWEL_MIX_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void JewelMix::MixElementalSymbols(Player* pPlayer, uint8 * Packet)
{
	int32 count[5] = {0, 0, 0, 0, 0};

	EVENT_INVENTORY_LOOP(i)
	{
		if ( pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 346) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 347) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 348) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 349) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 350) )
		{
			if ( count[pPlayer->GetEventInventory()->GetItem(i)->GetItem() - ITEMGET(14, 346)] <= 0 )
			{
				count[pPlayer->GetEventInventory()->GetItem(i)->GetItem() - ITEMGET(14, 346)]++;
			}
		}
	}

	if ( count[0] <= 0 || 
		 count[1] <= 0 || 
		 count[2] <= 0 || 
		 count[3] <= 0 || 
		 count[4] <= 0 )
	{
		this->MixResult(pPlayer, 0);
		return;
	}

	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	count[3] = 0;
	count[4] = 0;

	EVENT_INVENTORY_LOOP(i)
	{
		if ( pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 346) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 347) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 348) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 349) ||
			 pPlayer->GetEventInventory()->GetItem(i)->GetItem() == ITEMGET(14, 350) )
		{
			if ( count[pPlayer->GetEventInventory()->GetItem(i)->GetItem() - ITEMGET(14, 346)] <= 0 )
			{
				count[pPlayer->GetEventInventory()->GetItem(i)->GetItem() - ITEMGET(14, 346)]++;

				pPlayer->GetEventInventory()->GetItem(i)->AlterDurability(0, 1.0f);

				if ( pPlayer->GetEventInventory()->GetItem(i)->GetDurability() > 0.0f )
				{
					sEventInventory->GCEventItemDurSend(pPlayer, i, pPlayer->GetEventInventory()->GetItem(i)->GetDurability());
				}
				else
				{
					pPlayer->GetEventInventory()->DeleteItem(i);
					sEventInventory->GCEventItemDeleteSend(pPlayer, i);
				}
			}
		}
	}

	sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(ITEMGET(14, 351)), -1);

	this->MixResult(pPlayer, 1);

	sLog->outInfo("jewel", "[ %s ] Success: [%d / %d] --- %s", __FUNCTION__, 11, 0, pPlayer->BuildLog().c_str());

	this->LogDB(pPlayer, 2, 11, 0);
}

void JewelMix::UnMix(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::MixJewel || pPlayer->GetTransaction() == TRANSACTION_TYPE_BEGIN )
	{
		sLog->outError("jewel", "[ %s ] Wrong Interface --- %s", __FUNCTION__, pPlayer->BuildLog().c_str());

		this->UnMixResult(pPlayer, 6);
		return;
	}

	if ( pPlayer->GetPersonalStore()->IsBusy() )
	{
		sLog->outError("jewel", "[ %s ] PersonalStore --- %s", __FUNCTION__, pPlayer->BuildLog().c_str());

		this->UnMixResult(pPlayer, 6);
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->UnMixResult(pPlayer, 6);
		return;
	}

	POINTER_PCT_LOG(JEWEL_UNMIX_REQUEST, lpMsg, Packet, 0);

	JewelMixData const* pJewelData = this->GetJewelData(lpMsg->type, lpMsg->level);

	if ( !pJewelData || !pJewelData->flagHas(JEWEL_MIX_FLAG_UNMIX) )
	{
		sLog->outError("jewel", "[ %s ] Wrong Data: [%d / %d] --- %s", __FUNCTION__, lpMsg->type, lpMsg->level, pPlayer->BuildLog().c_str());

		this->UnMixResult(pPlayer, 0);
		return;
	}

	if ( !use_inventory_range(lpMsg->slot) )
	{
		sLog->outError("jewel", "[ %s ] Wrong Slot: %d --- %s", __FUNCTION__, lpMsg->slot, pPlayer->BuildLog().c_str());

		this->UnMixResult(pPlayer, 5);
		return;
	}

	if ( !pPlayer->GetInventory()->GetItem(lpMsg->slot)->IsItem() )
	{
		sLog->outError("jewel", "[ %s ] No Item: %d --- %s", __FUNCTION__, lpMsg->slot, pPlayer->BuildLog().c_str());

		this->UnMixResult(pPlayer, 4);
		return;
	}

	if ( pPlayer->GetInventory()->GetItem(lpMsg->slot)->GetItem() != pJewelData->GetPackItem() || 
		 pPlayer->GetInventory()->GetItem(lpMsg->slot)->GetLevel() != pJewelData->level.get() )
	{
		sLog->outError("jewel", "[ %s ] Wrong Item: %d [%d / %d] --- %s", __FUNCTION__, lpMsg->slot, lpMsg->type, lpMsg->level, pPlayer->BuildLog().c_str());

		this->UnMixResult(pPlayer, 6);
		return;
	}

	if ( !pPlayer->MoneyHave(pJewelData->unmix_money.get()) )
	{
		this->UnMixResult(pPlayer, 8);
		return;
	}

	if ( pPlayer->GetFreeInventorySlots() < pJewelData->count.get() )
	{
		this->UnMixResult(pPlayer, 7);
		return;
	}

	if ( !sGameServer->SerialCheck(pPlayer, pPlayer->GetInventory()->GetItem(lpMsg->slot)->GetSerialServer(), pPlayer->GetInventory()->GetItem(lpMsg->slot)->GetSerial(), pPlayer->GetInventory()->GetItem(lpMsg->slot)->GetName(), 0) )
	{
		if ( sGameServer->IsAntiDupeBoxRemove() )
		{
			pPlayer->ClearItem(lpMsg->slot);
		}

		this->UnMixResult(pPlayer, 7);
		return;
	}

	pPlayer->ClearItem(lpMsg->slot);

	for ( uint8 i = 0; i < pJewelData->count.get(); ++i )
	{
		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(pJewelData->GetItem(), 0, 0.0f, 0, 0, 0, 0, 0));
	}

	pPlayer->MoneyReduce(pJewelData->unmix_money.get());
	this->UnMixResult(pPlayer, 1);

	sLog->outInfo("jewel", "[ %s ] Success: [%d / %d] --- %s", __FUNCTION__, lpMsg->type, lpMsg->level, pPlayer->BuildLog().c_str());

	this->LogDB(pPlayer, 1, lpMsg->type, lpMsg->level);
}

void JewelMix::UnMixResult(Player* pPlayer, uint8 result)
{
	JEWEL_UNMIX_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void JewelMix::LogDB(Player* pPlayer, uint8 type, uint8 id, uint8 level)
{
	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_JEWEL_MIX);
	stmt->setUInt32(pPlayer->GetGUID());
	stmt->setString(pPlayer->GetName());
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt8(type);
	stmt->setUInt8(id);
	stmt->setUInt8(level);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}