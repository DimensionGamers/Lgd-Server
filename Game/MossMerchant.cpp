MossMerchant::MossMerchant()
{

}

MossMerchant::~MossMerchant()
{
	MAP_CLEAR(MossMerchantSectionMap::iterator, this->section_map);
}

void MossMerchant::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Moss Merchant Section...");

	MAP_CLEAR(MossMerchantSectionMap::iterator, this->section_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM moss_merchant_section i ORDER BY i.id ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			MossMerchantSection * pData = new MossMerchantSection;
			int32 loop = 0;

			pData->SetID(fields[loop++].GetUInt8());
			pData->SetName(fields[loop++].GetString());
			pData->SetRequiredItem(fields[loop++].GetUInt16());
			pData->SetRequiredZen(fields[loop++].GetUInt32());
			pData->SetItemBag(fields[loop++].GetString());
			pData->SetEnabled(fields[loop++].GetUInt8());

			this->section_map[pData->GetID()] = pData;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u moss merchant section definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MossMerchant::Open(Player* pPlayer)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::MossMerchant, 0) )
	{
		return;
	}

	uint8 buffer[1024];
	POINTER_PCT(MOSS_MERCHANT_OPEN, head, buffer, 0);
	POINTER_PCT(uint8, body, buffer, sizeof(MOSS_MERCHANT_OPEN));
	head->count = 0;

	for ( MossMerchantSectionMap::const_iterator itr = this->section_map.begin(); itr != this->section_map.end(); ++itr )
	{
		if ( !itr->second )
		{
			continue;
		}

		body[head->count] = itr->second->GetEnabled();
		++head->count;
	}

	head->h.set(0x70, 0x11, sizeof(MOSS_MERCHANT_OPEN) + head->count);

	pPlayer->sendPacket(buffer, head->h.get_size());

	pPlayer->MoneySend();
}

void MossMerchant::OpenBox(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::MossMerchant, 0) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MOSS_MERCHANT_OPEN_BOX, lpMsg, Packet, 0);

	MossMerchantSection const* pSection = this->GetMossMerchantSection(lpMsg->type);

	if ( !pSection )
	{
		sLog->outError("moss_merchant", "%s -- Wrong type: %u -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str());
		return;
	}

	if ( !pSection->GetEnabled() )
	{
		sLog->outError("moss_merchant", "%s -- Not enabled: %u -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str());
		return;
	}

	if ( pSection->GetRequiredItem() != uint16(-1) )
	{
		int32 count = pPlayer->GetInventory()->GetItemCount(pSection->GetRequiredItem(), 0);

		if ( count < pSection->GetRequiredZen() )
		{
			sLog->outError("moss_merchant", "%s -- Lack items %u [%u - %u]-- %s", __FUNCTION__, lpMsg->type, count, pSection->GetRequiredZen(), pPlayer->BuildLog().c_str());
			return;
		}
	}
	else
	{
		if ( !pPlayer->MoneyHave(pSection->GetRequiredZen()) )
		{
			sLog->outError("moss_merchant", "%s -- Lack money %u [%u - %u]-- %s", __FUNCTION__, lpMsg->type, pPlayer->MoneyGet(), pSection->GetRequiredZen(), pPlayer->BuildLog().c_str());
			return;
		}
	}

	Item item;
	if ( !sItemBagMgr->RunItemBag(pPlayer, pSection->GetItemBag(), item) )
	{
		sLog->outError("moss_merchant", "%s -- Can run itembag: %u -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str());
		return;
	}

	sLog->outInfo("moss_merchant", "%s -- %u succeed -- %s -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str(), item.BuildLog(0).c_str());

	if ( pSection->GetRequiredItem() != uint16(-1) )
	{
		for ( int32 i = 0; i < pSection->GetRequiredZen(); ++i )
		{
			pPlayer->ItemFind(pSection->GetRequiredItem(), 0, true);
		}
	}
	else
	{
		pPlayer->MoneyReduce(pSection->GetRequiredZen());
	}

	MOSS_MERCHANT_REWARD pMsg;
	item.ConvertToBuffer(pMsg.item_info);
	pPlayer->SEND_PCT(pMsg);
}

void MossMerchant::OpenBoxResult(Player* pPlayer, uint8 result)
{
	if ( !pPlayer )
	{
		return;
	}
}

MossMerchantSection const* MossMerchant::GetMossMerchantSection(uint8 id) const
{
	MossMerchantSectionMap::const_iterator itr = this->section_map.find(id);

	if ( itr != this->section_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}