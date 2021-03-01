// PentagramSystem.cpp: implementation of the PentagramSystem class.
//
//////////////////////////////////////////////////////////////////////

PentagramSystem::PentagramSystem()
{

}

PentagramSystem::~PentagramSystem()
{
	MAP_CLEAR(PentagramTypeInfoMap::iterator, this->m_PentagramTypeInfo);
	MAP_CLEAR(PentagramOptionInfoMap::iterator, this->m_PentagramOptionInfo);
	CLEAR_LIST(this->m_ErrtelOptionList);
	MAP_CLEAR(PentagramJewelRemoveInfoMap::iterator, this->m_PentagramJewelRemoveInfo);
	MAP_CLEAR(PentagramJewelUpgradeRankInfoMap::iterator, this->m_PentagramJewelUpgradeRankInfo);
	MAP_CLEAR(PentagramJewelUpgradeLevelInfoMap::iterator, this->m_PentagramJewelUpgradeLevelInfo);
}

void PentagramSystem::LoadType()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Pentagram Type...");

	MAP_CLEAR(PentagramTypeInfoMap::iterator, this->m_PentagramTypeInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM pentagram_type");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			PentagramTypeInfo * info = new PentagramTypeInfo;

			info->SetIndex(fields[i++].GetUInt16());
			info->SetName(fields[i++].GetString());
			info->SetRate(fields[i++].GetUInt8());

			for ( int32 h = 0; h < MAX_PENTAGRAM_BONUS_OPTION; ++h )
			{
				info->SetOption(h, fields[i++].GetUInt8());
			}

			this->m_PentagramTypeInfo[info->GetIndex()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u pentagram type definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void PentagramSystem::LoadOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Pentagram Option...");

	MAP_CLEAR(PentagramOptionInfoMap::iterator, this->m_PentagramOptionInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM pentagram_option");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			PentagramOptionInfo * info = new PentagramOptionInfo;

			info->SetIndex(fields[i++].GetUInt8());
			info->SetName(fields[i++].GetString());
			info->SetValue(fields[i++].GetUInt16());

			for( int32 n = 0; n < MAX_PENTAGRAM_JEWEL_RANK; ++n )
			{
				info->SetType(n, fields[i++].GetUInt8());
				info->SetRank(n, fields[i++].GetUInt8());
				info->SetLevel(n, fields[i++].GetUInt8());
			}

			this->m_PentagramOptionInfo[info->GetIndex()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u pentagram option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void PentagramSystem::LoadJewelOption()
{
	sLog->outLoad(true, "Loading Pentagram Jewel Option...");

	CLEAR_LIST(this->m_ErrtelOptionList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM pentagram_jewel_option");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			ErrtelOption * info = new ErrtelOption;

			info->SetIndex(reader.GetUInt8());
			info->SetItem(reader.ReadItem());
			info->SetRankNumber(reader.GetUInt8());
			info->SetRankOption(reader.GetUInt8());
			info->SetRate(reader.GetUInt16());

			reader.Skip();

			for (int32 n = 0; n < MAX_PENTAGRAM_JEWEL_LEVEL; ++n)
			{
				info->SetValue(n, reader.GetInt32());
				info->SetUpgradeRate(n, reader.GetUInt16());
			}

			info->SetName(reader.GetString());
			info->SetVariableType(reader.GetUInt8());
			info->SetType(0);

			switch (info->GetItem())
			{
			case ITEMGET(12, 231):
			case ITEMGET(12, 232):
			case ITEMGET(12, 506):
				info->SetType(1);
				break;

			case ITEMGET(12, 241):
			case ITEMGET(12, 242):
			case ITEMGET(12, 507):
				info->SetType(2);
				break;

			case ITEMGET(12, 251):
			case ITEMGET(12, 252):
			case ITEMGET(12, 508):
				info->SetType(3);
				break;

			case ITEMGET(12, 261):
				info->SetType(4);
				break;
			}

			this->m_ErrtelOptionList.push_back(info);
			count++;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u pentagram jewel option definitions", count);
}

void PentagramSystem::LoadJewelRemove()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Pentagram Jewel Remove...");

	MAP_CLEAR(PentagramJewelRemoveInfoMap::iterator, this->m_PentagramJewelRemoveInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM pentagram_jewel_remove");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			PentagramJewelRemoveInfo * info = new PentagramJewelRemoveInfo;

			info->SetIndex(fields[i++].GetUInt8());

			for( int32 n = 0; n < MAX_PENTAGRAM_JEWEL_RANK; ++n )
			{
				info->SetMixRate(n, fields[i++].GetUInt16());
			}

			this->m_PentagramJewelRemoveInfo[info->GetIndex()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u pentagram jewel remove definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void PentagramSystem::LoadJewelUpgradeRank()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Pentagram Jewel Upgrade Rank...");

	MAP_CLEAR(PentagramJewelUpgradeRankInfoMap::iterator, this->m_PentagramJewelUpgradeRankInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM pentagram_jewel_upgrade_rank");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			PentagramJewelUpgradeRankInfo * info = new PentagramJewelUpgradeRankInfo;

			info->SetIndex(fields[i++].GetUInt8());
			info->SetMixMoney(fields[i++].GetUInt32());
			info->SetMixRate(fields[i++].GetUInt16());

			this->m_PentagramJewelUpgradeRankInfo[info->GetIndex()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u pentagram jewel upgrade rank definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void PentagramSystem::LoadJewelUpgradeLevel()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Pentagram Jewel Upgrade Level...");

	MAP_CLEAR(PentagramJewelUpgradeLevelInfoMap::iterator, this->m_PentagramJewelUpgradeLevelInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM pentagram_jewel_upgrade_level");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			PentagramJewelUpgradeLevelInfo * info = new PentagramJewelUpgradeLevelInfo;

			info->SetIndex(fields[i++].GetUInt8());
			info->SetMixMoney(fields[i++].GetUInt32());
			
			for( int32 n = 0; n < MAX_PENTAGRAM_JEWEL_RANK; ++n )
			{
				info->SetMixRate(n, fields[i++].GetUInt16());
			}

			for( int32 n = 0; n < MAX_PENTAGRAM_JEWEL_RANK; ++n )
			{
				info->SetRequiredElementalRune(n, fields[i++].GetInt32());
			}

			this->m_PentagramJewelUpgradeLevelInfo[info->GetIndex()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u pentagram jewel upgrade level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool PentagramSystem::IsPentagramItem(uint16 item) const
{
	return this->GetPentagramTypeInfo(item) != nullptr;
}

bool PentagramSystem::IsPentagramItem(Item const* pItem) const
{
	return this->IsPentagramItem(pItem->GetItem());
}

bool PentagramSystem::IsPentagramMithril(uint16 item) const
{
	if( item == ITEMGET(12, 144) || item == ITEMGET(12, 145) || item == ITEMGET(12, 148) )
	{
		return true;
	}

	return false;
}

bool PentagramSystem::IsPentagramMithril(Item const* pItem) const
{
	return this->IsPentagramMithril(pItem->GetItem());
}

PentagramJewelInfo* PentagramSystem::GetPentagramJewelInfo(Player* pPlayer, uint8 index, uint8 type)
{
	if( !CHECK_RANGE(index, MAX_PENTAGRAM_JEWEL_INFO) )
	{
		return nullptr;
	}

	if( type == PENTAGRAM_JEWEL_TYPE_INVENTORY )
	{
		if( pPlayer->GetPentagramJewelInfoInventory(index)->GetIndex() == index )
		{
			return pPlayer->GetPentagramJewelInfoInventory(index);
		}
	}
	else
	{
		if( pPlayer->GetPentagramJewelInfoWarehouse(index)->GetIndex() == index )
		{
			return pPlayer->GetPentagramJewelInfoWarehouse(index);
		}
	}

	return nullptr;
}

PentagramJewelInfo* PentagramSystem::AddPentagramJewelInfo(Player* pPlayer, Item const* pItem, uint8 type)
{
	PentagramJewelInfo* pPentagramJewelInfoStr = (type == PENTAGRAM_JEWEL_TYPE_INVENTORY) ? pPlayer->GetPentagramJewelInfoInventoryPtr() : pPlayer->GetPentagramJewelInfoWarehousePtr();

	for (int32 n = 0; n < MAX_PENTAGRAM_JEWEL_INFO; ++n)
	{
		auto pPentagramJewelInfo = &pPentagramJewelInfoStr[n];

		if (pPentagramJewelInfo->GetIndex() == uint8(-1))
		{
			pPentagramJewelInfo->SetType(type);
			pPentagramJewelInfo->SetIndex(n);
			pPentagramJewelInfo->SetAttribute(pItem->GetSocketBonus());
			pPentagramJewelInfo->SetItemSection(pItem->GetItem() / MAX_SUBTYPE_ITEMS);
			pPentagramJewelInfo->SetItemType(pItem->GetItem() % MAX_SUBTYPE_ITEMS);
			pPentagramJewelInfo->SetItemLevel(pItem->GetLevel());

			SOCKET_SLOT_LOOP(i)
			{
				pPentagramJewelInfo->GetRankData(i)->SetIndex((pItem->GetSocket(i) & 0xFF) % 16);
				pPentagramJewelInfo->GetRankData(i)->SetLevel((pItem->GetSocket(i) & 0xFF) / 16);
			}

			pPentagramJewelInfo->SetSerialServer(pItem->GetSerialServer());
			pPentagramJewelInfo->SetSerial(pItem->GetSerial());
			pPentagramJewelInfo->SetDBFlag(pItem->GetDBFlag());
			pPentagramJewelInfo->SetData(0, pItem->raw.data_extra[0]);
			pPentagramJewelInfo->SetData(1, pItem->raw.data_extra[1]);
			pPentagramJewelInfo->SetData(2, pItem->raw.data_extra[2]);

			return pPentagramJewelInfo;
		}
	}

	return nullptr;
}

PentagramJewelInfo* PentagramSystem::AddPentagramJewelInfo(Player* pPlayer, PentagramJewelInfo const* pInfo, uint8 type)
{
	PentagramJewelInfo* pPentagramJewelInfoStr = (type == PENTAGRAM_JEWEL_TYPE_INVENTORY) ? pPlayer->GetPentagramJewelInfoInventoryPtr() : pPlayer->GetPentagramJewelInfoWarehousePtr();

	for (int32 n = 0; n < MAX_PENTAGRAM_JEWEL_INFO; ++n)
	{
		auto pPentagramJewelInfo = &pPentagramJewelInfoStr[n];

		if (pPentagramJewelInfo->GetIndex() == uint8(-1))
		{
			pPentagramJewelInfo->SetType(type);
			pPentagramJewelInfo->SetIndex(n);
			pPentagramJewelInfo->SetAttribute(pInfo->GetAttribute());
			pPentagramJewelInfo->SetItemSection(pInfo->GetItemSection());
			pPentagramJewelInfo->SetItemType(pInfo->GetItemType());
			pPentagramJewelInfo->SetItemLevel(pInfo->GetItemLevel());

			SOCKET_SLOT_LOOP(i)
			{
				pPentagramJewelInfo->GetRankData(i)->SetIndex(pInfo->GetRankData(i)->GetIndex());
				pPentagramJewelInfo->GetRankData(i)->SetLevel(pInfo->GetRankData(i)->GetLevel());
			}

			pPentagramJewelInfo->SetSerialServer(pInfo->GetSerialServer());
			pPentagramJewelInfo->SetSerial(pInfo->GetSerial());
			pPentagramJewelInfo->SetDBFlag(pInfo->GetDBFlag());
			pPentagramJewelInfo->SetData(0, pInfo->GetData(0));
			pPentagramJewelInfo->SetData(1, pInfo->GetData(1));
			pPentagramJewelInfo->SetData(2, pInfo->GetData(2));

			return pPentagramJewelInfo;
		}
	}

	return nullptr;
}

void PentagramSystem::DelPentagramJewelInfo(Player* pPlayer, uint8 index, uint8 type, bool log, bool to_gremory, int32 talisman_count)
{
	if( !CHECK_RANGE(index, MAX_PENTAGRAM_JEWEL_INFO) )
	{
		return;
	}

	PentagramJewelInfo* pPentagramJewelInfoStr = (type == PENTAGRAM_JEWEL_TYPE_INVENTORY) ? pPlayer->GetPentagramJewelInfoInventoryPtr() : pPlayer->GetPentagramJewelInfoWarehousePtr();

	if( pPentagramJewelInfoStr[index].GetIndex() == index )
	{
		if (log)
		{
			sLog->outInfo(LOG_ITEMS, "[%s][%s][%s]", __FUNCTION__, pPlayer->BuildLog().c_str(), pPentagramJewelInfoStr[index].BuildLogDB().c_str());
		}

		if (to_gremory && sGameServer->IsErrtelToRecovery())
		{
			INIT_SOCKET_DATA(SocketOption);
			SOCKET_SLOT_LOOP(i)
			{
				SocketOption[i] = pPentagramJewelInfoStr[index].GetRankData(i)->GetIndex() | (pPentagramJewelInfoStr[index].GetRankData(i)->GetLevel() * 16);
			}
			
			uint8 attribute = pPentagramJewelInfoStr[index].GetAttribute() % 16;

			for (int32 i = 0; i < MAX_SOCKET_SLOT; ++i)
			{
				if (SocketOption[i] == 0xFF)
				{
					SocketOption[i] = SOCKET_SLOT_NONE;
				}
				else if (SocketOption[i] == 0xFE)
				{
					SocketOption[i] = SOCKET_SLOT_EMPTY;
				}
				else
				{
					attribute += 16;
				}
			}

			Item add_item(ITEMGET(pPentagramJewelInfoStr[index].GetItemSection(), pPentagramJewelInfoStr[index].GetItemType()), pPentagramJewelInfoStr[index].GetItemLevel(), 0, 0, 0, 0, 0, 0, SocketOption, attribute);
			add_item.SetDBFlag(pPentagramJewelInfoStr[index].GetDBFlag());
			add_item.raw.data_extra[0] = pPentagramJewelInfoStr[index].GetData(0);
			add_item.raw.data_extra[1] = pPentagramJewelInfoStr[index].GetData(1);
			add_item.raw.data_extra[2] = pPentagramJewelInfoStr[index].GetData(2);

			if (pPentagramJewelInfoStr[index].GetSerial())
			{
				add_item.SetSerialServer(pPentagramJewelInfoStr[index].GetSerialServer());
				add_item.SetSerial(pPentagramJewelInfoStr[index].GetSerial());
			}
			else
			{
				add_item.SetSerialServer(sGameServer->GetServerCode());
				add_item.SetSerial(sObjectMgr->GenerateSerial());
			}

			if (talisman_count <= 0)
			{
				talisman_count = 1;
			}

			add_item.common.talisman = talisman_count + sGameServer->GetErrtelMixFailCount();

			pPlayer->AddRecoveryMixItem(&add_item);
		}

		pPentagramJewelInfoStr[index].Clear();
	}
}

void PentagramSystem::DelAllPentagramJewelInfo(Player* pPlayer, Item * pItem, uint8 type)
{
	if (pItem->IsPentagramItem())
	{
		for (int32 n = 0; n < MAX_SOCKET_SLOT; ++n)
		{
			this->DelPentagramJewelInfo(pPlayer, pItem->GetSocket(n) & 0xFF, type, true, true, pItem->common.talisman);

			if (pItem->GetSocket(n) != SOCKET_SLOT_NONE)
			{
				pItem->SetSocket(n, SOCKET_SLOT_EMPTY);
			}
		}
	}
}

bool PentagramSystem::CheckExchangePentagramItem(Player* pPlayer)
{
	trade_loop(i)
	{
		if ( !this->CheckExchangePentagramItem(pPlayer->GetTradeInventory()->GetItem(i)) )
		{
			return false;
		}
	}

	return true;
}

bool PentagramSystem::CheckExchangePentagramItem(Item const* pItem)
{
	if( pItem->IsItem() && pItem->IsPentagramItem() )
	{
		if( pItem->GetDurability() <= 0.0f )
		{
			return false;
		}
	}

	return true;
}

bool PentagramSystem::CheckPentagramOption(Player* pPlayer, PentagramOptionInfo const* pInfo)
{
	SOCKET_SLOT_LOOP(n)
	{
		if (pInfo->GetType(n) != uint8(-1))
		{
			PentagramJewelInfo const* pPentagramJewelInfo = this->GetPentagramJewelInfo(pPlayer, pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT)->GetSocket(n) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

			if (!pPentagramJewelInfo)
			{
				return false;
			}

			/*if( (pPentagramJewelInfo->GetAttribute() / 16) < pInfo->GetRank(n) )
			{
			return false;
			}*/

			if (pPentagramJewelInfo->GetRank() < pInfo->GetRank(n))
			{
				return false;
			}

			if (pInfo->GetRank(n) >= 1 && pInfo->GetRank(n) <= 5)
			{
				if (pPentagramJewelInfo->GetRankData(pInfo->GetRank(n) - 1)->GetLevel() < pInfo->GetLevel(n))
				{
					return false;
				}
			}
		}
	}

	return true;
}

void PentagramSystem::MovePentagramToInventoryFromWarehouse(Player* pPlayer, Item * pItem)
{
	if( !pItem->IsPentagramItem() )
	{
		return;
	}

	int32 count = 0;

	for( int32 n = 0; n < MAX_SOCKET_SLOT; ++n )
	{
		PentagramJewelInfo const* pPentagramJewelInfo_Warehouse = this->GetPentagramJewelInfo(pPlayer, pItem->GetSocket(n) & 0xFF, PENTAGRAM_JEWEL_TYPE_WAREHOUSE);

		if ( pPentagramJewelInfo_Warehouse )
		{
			PentagramJewelInfo const* pPentagramJewelInfo_Inventory = this->AddPentagramJewelInfo(pPlayer, pPentagramJewelInfo_Warehouse, PENTAGRAM_JEWEL_TYPE_INVENTORY);

			if ( pPentagramJewelInfo_Inventory )
			{
				this->DelPentagramJewelInfo(pPlayer, pPentagramJewelInfo_Warehouse->GetIndex(), PENTAGRAM_JEWEL_TYPE_WAREHOUSE);

				pItem->SetSocket(n, pPentagramJewelInfo_Inventory->GetIndex());

				count++;
			}
		}
	}

	if( count > 0 )
	{
		this->GCPentagramJewelInfoSend(pPlayer, PENTAGRAM_JEWEL_TYPE_INVENTORY);
		this->GCPentagramJewelInfoSend(pPlayer, PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
	}
}

void PentagramSystem::MovePentagramToWarehouseFromInventory(Player* pPlayer, Item * pItem)
{
	if( !pItem->IsPentagramItem() )
	{
		return;
	}

	int32 count = 0;

	for( int32 n = 0; n < MAX_SOCKET_SLOT; ++n )
	{
		PentagramJewelInfo const* pPentagramJewelInfo_Inventory = this->GetPentagramJewelInfo(pPlayer, pItem->GetSocket(n) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

		if( pPentagramJewelInfo_Inventory )
		{
			PentagramJewelInfo const* pPentagramJewelInfo_Warehouse = this->AddPentagramJewelInfo(pPlayer, pPentagramJewelInfo_Inventory, PENTAGRAM_JEWEL_TYPE_WAREHOUSE);

			if( pPentagramJewelInfo_Warehouse )
			{
				this->DelPentagramJewelInfo(pPlayer, pPentagramJewelInfo_Inventory->GetIndex(), PENTAGRAM_JEWEL_TYPE_INVENTORY);

				pItem->SetSocket(n, pPentagramJewelInfo_Warehouse->GetIndex());

				count++;
			}
		}
	}

	if( count > 0 )
	{
		this->GCPentagramJewelInfoSend(pPlayer, PENTAGRAM_JEWEL_TYPE_INVENTORY);
		this->GCPentagramJewelInfoSend(pPlayer, PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
	}
}

void PentagramSystem::ExchangePentagramItem(Player* pPlayer01, Player* pPlayer02)
{
	int32 count = 0;

	trade_loop(n)
	{
		if ( pPlayer01->GetTradeInventory()->GetItem(n)->IsItem() && pPlayer01->GetTradeInventory()->GetItem(n)->IsPentagramItem() )
		{
			for( int32 i = 0; i < MAX_SOCKET_SLOT; ++i )
			{
				PentagramJewelInfo* lpPentagramJewelInfo_Source = this->GetPentagramJewelInfo(pPlayer01, pPlayer01->GetTradeInventory()->GetItem(n)->GetSocket(i) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

				if( lpPentagramJewelInfo_Source )
				{
					PentagramJewelInfo* lpPentagramJewelInfo_Target = this->AddPentagramJewelInfo(pPlayer02, lpPentagramJewelInfo_Source, PENTAGRAM_JEWEL_TYPE_INVENTORY);

					if( lpPentagramJewelInfo_Target )
					{
						this->DelPentagramJewelInfo(pPlayer01, lpPentagramJewelInfo_Source->GetIndex(), PENTAGRAM_JEWEL_TYPE_INVENTORY);

						pPlayer02->GetInventory()->GetItem(pPlayer01->GetTradeInventory()->GetTargetSlot(n))->SetSocket(i, lpPentagramJewelInfo_Target->GetIndex());

						count++;
					}
				}
			}

			pPlayer02->GetInventory()->GetItem(pPlayer01->GetTradeInventory()->GetTargetSlot(n))->AlterDurability(0, 1.0f);
		}
	}

	if( count > 0 )
	{
		this->GCPentagramJewelInfoSend(pPlayer02, PENTAGRAM_JEWEL_TYPE_INVENTORY);
	}
}

void PentagramSystem::ExchangePentagramItem(Player* pPlayer01, Player* pPlayer02, Item * pItem)
{
	if( !pItem->IsPentagramItem() )
	{
		return;
	}

	int32 count = 0;

	for( int32 n = 0; n < MAX_SOCKET_SLOT; ++n )
	{
		PentagramJewelInfo* lpPentagramJewelInfo_Source = this->GetPentagramJewelInfo(pPlayer01, pItem->GetSocket(n) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

		if( lpPentagramJewelInfo_Source )
		{
			PentagramJewelInfo* lpPentagramJewelInfo_Target = this->AddPentagramJewelInfo(pPlayer02, lpPentagramJewelInfo_Source, PENTAGRAM_JEWEL_TYPE_INVENTORY);

			if( lpPentagramJewelInfo_Target )
			{
				this->DelPentagramJewelInfo(pPlayer01, lpPentagramJewelInfo_Source->GetIndex(), PENTAGRAM_JEWEL_TYPE_INVENTORY);

				pItem->SetSocket(n, lpPentagramJewelInfo_Target->GetIndex());

				count++;
			}
		}
	}

	pItem->AlterDurability(0, 1.0f);

	if( count > 0 )
	{
		this->GCPentagramJewelInfoSend(pPlayer02, PENTAGRAM_JEWEL_TYPE_INVENTORY);
	}
}

PentagramTypeInfo const* PentagramSystem::GetPentagramTypeInfo(uint16 index) const
{
	PentagramTypeInfoMap::const_iterator it = this->m_PentagramTypeInfo.find(index);

	if( it != this->m_PentagramTypeInfo.end() )
	{
		return it->second;
	}
	
	return nullptr;
}

PentagramOptionInfo const* PentagramSystem::GetPentagramOptionInfo(uint8 index) const
{
	PentagramOptionInfoMap::const_iterator it = this->m_PentagramOptionInfo.find(index);

	if( it != this->m_PentagramOptionInfo.end() )
	{
		return it->second;
	}
	
	return nullptr;
}

ErrtelOption const* PentagramSystem::GetErrtelOption(uint16 item, uint8 rank_number, uint8 rank_option) const
{
	for (auto & pData : this->m_ErrtelOptionList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetItem() == item && pData->GetRankNumber() == rank_number && pData->GetRankOption() == rank_option)
		{
			return pData;
		}
	}

	return nullptr;
}

PentagramJewelRemoveInfo const* PentagramSystem::GetPentagramJewelRemoveInfo(uint8 index) const
{
	PentagramJewelRemoveInfoMap::const_iterator it = this->m_PentagramJewelRemoveInfo.find(index);

	if( it != this->m_PentagramJewelRemoveInfo.end() )
	{
		return it->second;
	}
	
	return nullptr;
}

PentagramJewelUpgradeRankInfo const* PentagramSystem::GetPentagramJewelUpgradeRankInfo(uint8 index) const
{
	PentagramJewelUpgradeRankInfoMap::const_iterator it = this->m_PentagramJewelUpgradeRankInfo.find(index);

	if( it != this->m_PentagramJewelUpgradeRankInfo.end() )
	{
		return it->second;
	}
	
	return nullptr;
}

PentagramJewelUpgradeLevelInfo const* PentagramSystem::GetPentagramJewelUpgradeLevelInfo(uint8 index) const
{
	PentagramJewelUpgradeLevelInfoMap::const_iterator it = this->m_PentagramJewelUpgradeLevelInfo.find(index);

	if( it != this->m_PentagramJewelUpgradeLevelInfo.end() )
	{
		return it->second;
	}
	
	return nullptr;
}

SocketDataType PentagramSystem::GetRandomErrtelOption(uint16 item, uint8 rank_number)
{
	RandomValue<SocketDataType> m_RandomValue(-1);

	for (auto & pData : this->m_ErrtelOptionList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetItem() == item && pData->GetRankNumber() == rank_number)
		{
			m_RandomValue.AddValue(pData->GetRankOption(), pData->GetRate());
		}
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_WEIGHT);
}

void PentagramSystem::GetPentagramRelationshipDamage(Unit* pUnit, Unit* pTarget, int32 & damage)
{
	int32 rate = 100;

	if (pUnit->IsPlayer() && pTarget->IsPlayer())
	{
		rate = gPentagramRelationshipDamageTablePvP[pUnit->GetElementalAttribute()][pTarget->GetElementalAttribute()];
	}
	else
	{
		rate = gPentagramRelationshipDamageTablePvM[pUnit->GetElementalAttribute()][pTarget->GetElementalAttribute()];
	}

	if (Player* pPlayer = pUnit->ToPlayer())
	{
		switch (pTarget->GetElementalAttribute())
		{
		case ELEMENTAL_ATTRIBUTE_FIRE:
			damage += pPlayer->GetPentagramJewelOption()->GetElementalDamageVsFire();
			break;

		case ELEMENTAL_ATTRIBUTE_WATER:
			damage += pPlayer->GetPentagramJewelOption()->GetElementalDamageVsWater();
			break;

		case ELEMENTAL_ATTRIBUTE_EARTH:
			damage += pPlayer->GetPentagramJewelOption()->GetElementalDamageVsEarth();
			break;

		case ELEMENTAL_ATTRIBUTE_WIND:
			damage += pPlayer->GetPentagramJewelOption()->GetElementalDamageVsWind();
			break;

		case ELEMENTAL_ATTRIBUTE_DARK:
			damage += pPlayer->GetPentagramJewelOption()->GetElementalDamageVsDark();
			break;
		}
	}

	damage = (damage * rate) / 100;
}

void PentagramSystem::GetPentagramRelationshipDefense(Unit* pUnit, Unit* pTarget, int32 & defense)
{
	int32 rate = 100;

	if (pUnit->IsPlayer() && pTarget->IsPlayer())
	{
		rate = gPentagramRelationshipDefenseTablePvP[pTarget->GetElementalAttribute()][pUnit->GetElementalAttribute()];
	}
	else
	{
		rate = gPentagramRelationshipDefenseTablePvM[pTarget->GetElementalAttribute()][pUnit->GetElementalAttribute()];
	}

	if (Player* pPlayer = pTarget->ToPlayer())
	{
		switch (pUnit->GetElementalAttribute())
		{
		case ELEMENTAL_ATTRIBUTE_FIRE:
			defense += pPlayer->GetPentagramJewelOption()->GetElementalDefenseVsFire();
			break;

		case ELEMENTAL_ATTRIBUTE_WATER:
			defense += pPlayer->GetPentagramJewelOption()->GetElementalDefenseVsWater();
			break;

		case ELEMENTAL_ATTRIBUTE_EARTH:
			defense += pPlayer->GetPentagramJewelOption()->GetElementalDefenseVsEarth();
			break;

		case ELEMENTAL_ATTRIBUTE_WIND:
			defense += pPlayer->GetPentagramJewelOption()->GetElementalDefenseVsWind();
			break;

		case ELEMENTAL_ATTRIBUTE_DARK:
			defense += pPlayer->GetPentagramJewelOption()->GetElementalDefenseVsDark();
			break;
		}
	}

	defense = (defense * rate) / 100;
}

void PentagramSystem::ApplyPentagramOption(Player* pPlayer)
{
	if (pPlayer->GetWorldId() == WORLD_VULCANUS_ROOM && sGameServer->IsDuelPentagramDisabled())
	{
		return;
	}

	const auto pPentagram = pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT);

	if (!pPentagram->IsItem() || !pPentagram->IsValid() || !pPentagram->IsPentagramItem())
	{
		return;
	}

	auto pPentagramTypeInfo = this->GetPentagramTypeInfo(pPentagram->GetItem());

	if (!pPentagramTypeInfo)
	{
		return;
	}

	for (int32 i = 0; i < MAX_PENTAGRAM_BONUS_OPTION; ++i)
	{
		auto pPentagramOptionInfo = this->GetPentagramOptionInfo(pPentagramTypeInfo->GetOption(i));

		if (!pPentagramOptionInfo)
		{
			continue;
		}

		if (!this->CheckPentagramOption(pPlayer, pPentagramOptionInfo))
		{
			continue;
		}

		this->ApplyPentagramOption(pPlayer, pPentagramOptionInfo->GetIndex(), pPentagramOptionInfo->GetValue());
	}
}

void PentagramSystem::ApplyPentagramOption(Player* pPlayer, uint8 index, uint16 value)
{
	switch (index)
	{
	case 1:
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDamage(value);
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDefense(value);
		break;

	case 2:
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDamage(value);
		break;

	case 3:
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDefense(value);
		break;

	case 4:
		pPlayer->GetPentagramOption()->IncreaseAddElementalCriticalDamageRate(value);
		break;

	case 5:
		pPlayer->GetPentagramOption()->AddElementalErrtelEffect[1][0] += value;
		break;

	case 21:
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDamage(value);
		break;

	case 22:
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDefense(value);
		break;

	case 23:
		pPlayer->GetPentagramOption()->IncreaseAddElementalCriticalDamageRate(value);
		break;

	case 24:
		pPlayer->GetPentagramOption()->IncreaseAddElementalCriticalDamageRate(value);
		break;

	case 25:
		pPlayer->GetPentagramOption()->IncreaseAddElementalCriticalDamageRate(value);
		break;

	case 26:
		pPlayer->GetPentagramOption()->IncreaseAddElementalDefenseTransferRate(value);
		break;

	case 27:
		pPlayer->GetPentagramOption()->IncreaseAddElementalAttackTransferRate(value);
		break;

	case 28:
		pPlayer->GetPentagramOption()->IncreaseAddElementalAvoidanceRate(value);
		break;

	case 29:
		pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, value);
		pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, value);
		break;

	case 30:
		pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, value);
		break;

	case 34:
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDamage(value);
		pPlayer->GetPentagramOption()->IncreaseMulPentagramDefense(value);
		break;

	case 41:
		pPlayer->GetPentagramOption()->IncreaseAddElementalAvoidanceRate(value);
		break;

	case 42:
		pPlayer->GetPentagramOption()->IncreaseAddElementalDefenseTransferRate(value);
		break;

	case 43:
		pPlayer->GetPentagramOption()->IncreaseAddElementalAttackTransferRate(value);
		break;
	}
}

void PentagramSystem::ApplyErrtelOption(Player* pPlayer)
{
	if (pPlayer->GetWorldId() == WORLD_VULCANUS_ROOM && sGameServer->IsDuelPentagramDisabled())
	{
		return;
	}

	Item* pPentagram = pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT);

	if (!pPentagram->IsItem() || !pPentagram->IsValid() || !pPentagram->IsPentagramItem())
	{
		return;
	}

	SOCKET_SLOT_LOOP(n)
	{
		auto pPentagramJewelInfo = this->GetPentagramJewelInfo(pPlayer, pPentagram->GetSocket(n) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

		if (!pPentagramJewelInfo)
		{
			continue;
		}

		uint16 errtel_item = ITEMGET(pPentagramJewelInfo->GetItemSection(), pPentagramJewelInfo->GetItemType());

		SOCKET_SLOT_LOOP(i)
		{
			auto pErrtelOption = this->GetErrtelOption(errtel_item, i + 1, pPentagramJewelInfo->GetRankData(i)->GetIndex());

			if (!pErrtelOption)
			{
				continue;
			}

			if (pPentagramJewelInfo->GetRankData(i)->GetLevel() >= MAX_PENTAGRAM_JEWEL_LEVEL)
			{
				continue;
			}

			int32 value = pErrtelOption->GetValue(pPentagramJewelInfo->GetRankData(i)->GetLevel());
			auto index = pErrtelOption->GetIndex();

			value += pPlayer->GetPentagramOption()->AddElementalErrtelEffect[pErrtelOption->GetType()][pErrtelOption->GetRankNumber() - 1];

			switch (index)
			{
			case 0:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamage(value);
				break;

			case 10:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsFire(value);
				break;

			case 11:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsWater(value);
				break;

			case 12:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsEarth(value);
				break;

			case 13:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsWind(value);
				break;

			case 14:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsDark(value);
				break;

			case 20:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamagePvP(value);
				break;

			case 21:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamagePvM(value);
				break;

			case 30:
				pPlayer->GetPentagramJewelOption()->IncreaseMulElementalDamagePvP(value);
				break;

			case 31:
				pPlayer->GetPentagramJewelOption()->IncreaseMulElementalDamagePvM(value);
				break;

			case 40:
				pPlayer->IncreaseFloatData(UNIT_FLOAT_ELEMENTAL_DAMAGE_PERCENT, value);
				break;

			case 50:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDefense(value);
				break;

			case 60:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsFire(value);
				break;

			case 61:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsWater(value);
				break;

			case 62:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsEarth(value);
				break;

			case 63:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsWind(value);
				break;

			case 64:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsDark(value);
				break;

			case 70:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDefensePvP(value);
				break;

			case 71:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDefensePvM(value);
				break;

			case 80:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamageReductionPvP(value);
				break;

			case 81:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamageReductionPvM(value);
				break;

			case 90:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamageReduction(value);
				break;

			case 100:
				pPlayer->IncreaseFloatData(UNIT_FLOAT_ELEMENTAL_ATTACK_SUCCESS_RATE_PERCENT, value);
				break;

			case 110:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsFire(value);
				break;

			case 111:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsWater(value);
				break;

			case 112:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsEarth(value);
				break;

			case 113:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsWind(value);
				break;

			case 114:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDamageVsDark(value);
				break;

			case 120:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamagePvP(value);
				break;

			case 121:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamagePvM(value);
				break;

			case 130:
				pPlayer->IncreaseData(PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVP, value);
				break;

			case 131:
				pPlayer->IncreaseData(PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVM, value);
				break;

			case 140:
				pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, value);
				break;

			case 150:
				pPlayer->IncreaseFloatData(UNIT_FLOAT_ELEMENTAL_DEFENSE_SUCCESS_RATE_PERCENT, value);
				break;

			case 160:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsFire(value);
				break;

			case 161:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsWater(value);
				break;

			case 162:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsEarth(value);
				break;

			case 163:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsWind(value);
				break;

			case 164:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalDefenseVsDark(value);
				break;

			case 170:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDefensePvP(value);
				break;

			case 171:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDefensePvM(value);
				break;

			case 180:
				pPlayer->IncreaseData(PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVP, value);
				break;

			case 181:
				pPlayer->IncreaseData(PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVM, value);
				break;

			case 190:
				pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, value);
				break;

			case 200:
			case 204:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDamage(value);
				break;

			case 201:
			case 205:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalDefense(value);
				break;

			case 202:
				pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, value);
				break;

			case 203:
				pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, value);
				break;

			case 210:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalShieldAbsorbRate(value);
				break;

			case 211:
				pPlayer->GetPentagramJewelOption()->IncreaseAddElementalLifeAbsorbRate(value);
				break;

			case 212:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalBastionDuration(value);
				break;

			case 220:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalBleeding(value);
				break;

			case 221:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalParalysisRate(value);
				break;

			case 222:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalBindingRate(value);
				break;

			case 223:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalPunishRate(value);
				break;

			case 224:
				pPlayer->GetPentagramJewelOption()->IncreaseElementalBlindness(value);
				break;
			}
		}
	}
}

void PentagramSystem::CGPentagramJewelInsertRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	PENTAGRAM_JEWEL_INSERT_SEND pMsg;

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	POINTER_PCT_LOG(PENTAGRAM_JEWEL_INSERT_RECV, lpMsg, Packet, 0);
	
	if( !use_inventory_range(lpMsg->target_slot) )
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Wrong Target Slot: %u", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->target_slot);

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if( !use_inventory_range(lpMsg->source_slot) )
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Wrong Source Slot: %u", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->source_slot);

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	Item * pPentagram = pPlayer->GetInventory()->GetItem(lpMsg->target_slot);
	Item * pErrtel = pPlayer->GetInventory()->GetItem(lpMsg->source_slot);

	if ( !pPentagram->IsItem() )
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Wrong Target Item: %u", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->target_slot);

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !pErrtel->IsItem() )
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Wrong Source Item: %u", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->target_slot);

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !pPentagram->IsPentagramItem() )
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Not Pentagram: [%u / %u]", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->target_slot, pPentagram->GetItem());

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if (pErrtel->GetKind2() != ItemKind::ERRTEL)
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Not Errtel: [%u / %u]", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->source_slot, pErrtel->GetItem());

		pPlayer->SEND_PCT(pMsg);
		return;
	}
	///- Pentagram for Beginners
	if ( pPentagram->GetItem() == ITEMGET(12, 215) ||
		 pPentagram->GetItem() == ITEMGET(12, 216) ||
		 pPentagram->GetItem() == ITEMGET(12, 218) )
	{
		if ( pErrtel->GetItem() != ITEMGET(12, 222) &&
			 pErrtel->GetItem() != ITEMGET(12, 232) &&
			 pErrtel->GetItem() != ITEMGET(12, 242) &&
			 pErrtel->GetItem() != ITEMGET(12, 252) )
		{
			pPlayer->SEND_PCT(pMsg);
			return;
		}
	}
	else
	{
		if ( pErrtel->GetItem() == ITEMGET(12, 222) ||
			 pErrtel->GetItem() == ITEMGET(12, 232) ||
			 pErrtel->GetItem() == ITEMGET(12, 242) ||
			 pErrtel->GetItem() == ITEMGET(12, 252) )
		{
			pPlayer->SEND_PCT(pMsg);
			return;
		}
	}

	int32 socket_slot = (pErrtel->GetItem() - ITEMGET(12, 221)) / 10;

	if( !CHECK_RANGE(socket_slot, MAX_SOCKET_SLOT) )
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Wrong Socket Slot: [%u / %d]", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->source_slot, socket_slot);

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( pPentagram->GetSocket(socket_slot) != SOCKET_SLOT_EMPTY )
	{
		sLog->outError(LOG_ITEMS, "%s -- %s -- Socket Not Empty [%u / %u]", __FUNCTION__, pPlayer->BuildLog().c_str(), lpMsg->target_slot, pPentagram->GetSocket(socket_slot));

		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if( (pPentagram->GetSocketBonus() % 16) != (pErrtel->GetSocketBonus() % 16) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	PentagramJewelInfo * pPentagramJewelInfo = this->AddPentagramJewelInfo(pPlayer, pPlayer->GetInventory()->GetItem(lpMsg->source_slot), PENTAGRAM_JEWEL_TYPE_INVENTORY);

	if( !pPentagramJewelInfo )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	pMsg.result = 1;
	pMsg.type = pPentagramJewelInfo->GetType();
	pMsg.index = pPentagramJewelInfo->GetIndex();
	pMsg.attribute = pPentagramJewelInfo->GetAttribute();
	pMsg.item_section = pPentagramJewelInfo->GetItemSection();
	pMsg.item_type = pPentagramJewelInfo->GetItemType();
	pMsg.item_level = pPentagramJewelInfo->GetItemLevel();
	SOCKET_SLOT_LOOP(i)
	{
		auto & data = pMsg.rank_data[i];

		data.index = pPentagramJewelInfo->GetRankData(i)->GetIndex();
		data.level = pPentagramJewelInfo->GetRankData(i)->GetLevel();
	}

	pPlayer->SEND_PCT(pMsg);

	this->Log(pPlayer, 0, 0, 1, pPentagram->BuildLogDB(lpMsg->target_slot, true), pErrtel->BuildLogDB(lpMsg->source_slot, true));

	sLog->outInfo(LOG_ITEMS, "%s -- %s -- Added Errtel [%s] - Pentagram [%s] on %d", __FUNCTION__, pPlayer->BuildLog().c_str(), 
		pErrtel->BuildLog(lpMsg->source_slot).c_str(),
		pPentagram->BuildLog(lpMsg->target_slot).c_str(),
		socket_slot);

	pPentagram->SetSocket(socket_slot, pPentagramJewelInfo->GetIndex());
	pPentagram->Convert(true);

	pPlayer->SendInventory(lpMsg->target_slot);
	
	pPlayer->ClearItem(lpMsg->source_slot, 0);

	this->GCPentagramJewelResultSend(pPlayer, 1);

	this->FixPentagram(pPlayer);
}

void PentagramSystem::CGPentagramJewelRemoveRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	PMSG_PENTAGRAM_JEWEL_REMOVE_SEND pMsg;

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");

		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	POINTER_PCT_LOG(PMSG_PENTAGRAM_JEWEL_REMOVE_RECV, lpMsg, Packet, 0);

	bool ignore_fail_rate = false;

	if (!use_inventory_range(lpMsg->source_slot))
	{
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	Item * pPentagram = pPlayer->GetInventory()->GetItem(lpMsg->source_slot);

	if ( !pPentagram->IsItem() )
	{
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	if ( !pPentagram->IsPentagramItem() )
	{
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	if( !CHECK_RANGE(lpMsg->socket_slot, MAX_SOCKET_SLOT) )
	{
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	if ( use_inventory_range(lpMsg->blessing_slot) )
	{
		if ( pPlayer->GetInventory()->GetItem(lpMsg->blessing_slot)->IsItem() &&
			 pPlayer->GetInventory()->GetItem(lpMsg->blessing_slot)->GetItem() == ITEMGET(14, 320) )
		{
			ignore_fail_rate = true;
		}
	}

	PentagramJewelInfo* lpPentagramJewelInfo = this->GetPentagramJewelInfo(pPlayer, pPentagram->GetSocket(lpMsg->socket_slot) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

	if( !lpPentagramJewelInfo )
	{
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	if ( !pPlayer->GetInventory()->IsEmptySpace(ITEMGET(lpPentagramJewelInfo->GetItemSection(), lpPentagramJewelInfo->GetItemType())) )
	{
		pMsg.result = 100;
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 100);
		return;
	}

	PentagramJewelRemoveInfo const* pPentagramJewelRemoveInfo = this->GetPentagramJewelRemoveInfo(lpPentagramJewelInfo->GetItemLevel());

	if( !pPentagramJewelRemoveInfo )
	{
		pPlayer->SEND_PCT(pMsg);
		this->GCPentagramJewelResultSend(pPlayer, 0);
		return;
	}

	uint8 rank = lpPentagramJewelInfo->GetAttribute() / 16;

	if ( rank > 0 )
	{
		rank -= 1;
	}

	if( !CHECK_RANGE(rank, MAX_PENTAGRAM_JEWEL_RANK) )
	{
		rank = 0;
	}

	bool success = ignore_fail_rate || roll_chance_i(pPentagramJewelRemoveInfo->GetMixRate(rank), 10000);

	this->Log(pPlayer, 1, ignore_fail_rate ? 1 : 0, success ? 1 : 0, pPentagram->BuildLogDB(lpMsg->source_slot, true), lpPentagramJewelInfo->BuildLogDB());

	INIT_SOCKET_DATA(SocketOption);
	SOCKET_SLOT_LOOP(i)
	{
		SocketOption[i] = lpPentagramJewelInfo->GetRankData(i)->GetIndex() | (lpPentagramJewelInfo->GetRankData(i)->GetLevel() * 16);
	}

	uint8 attribute = lpPentagramJewelInfo->GetAttribute() % 16;

	for (int32 i = 0; i < MAX_SOCKET_SLOT; ++i)
	{
		if (SocketOption[i] == 0xFF)
		{
			SocketOption[i] = SOCKET_SLOT_NONE;
		}
		else if (SocketOption[i] == 0xFE)
		{
			SocketOption[i] = SOCKET_SLOT_EMPTY;
		}
		else
		{
			attribute += 16;
		}
	}

	Item add_item(ITEMGET(lpPentagramJewelInfo->GetItemSection(), lpPentagramJewelInfo->GetItemType()), lpPentagramJewelInfo->GetItemLevel(), 0, 0, 0, 0, 0, 0, SocketOption, attribute);
	add_item.SetDBFlag(lpPentagramJewelInfo->GetDBFlag());
	add_item.raw.data_extra[0] = lpPentagramJewelInfo->GetData(0);
	add_item.raw.data_extra[1] = lpPentagramJewelInfo->GetData(1);
	add_item.raw.data_extra[2] = lpPentagramJewelInfo->GetData(2);

	if (lpPentagramJewelInfo->GetSerial())
	{
		add_item.SetSerialServer(lpPentagramJewelInfo->GetSerialServer());
		add_item.SetSerial(lpPentagramJewelInfo->GetSerial());
	}
	else
	{
		add_item.SetSerialServer(sGameServer->GetServerCode());
		add_item.SetSerial(sObjectMgr->GenerateSerial());
	}
	
	if( success )
	{
		pPlayer->AddItem(add_item, -1, ADD_ITEM_SEND_OK);

		pMsg.result = 1;
		pMsg.type = lpPentagramJewelInfo->GetType();
		pMsg.index = lpPentagramJewelInfo->GetIndex();

		pPlayer->SEND_PCT(pMsg);

		pPentagram->SetSocket(lpMsg->socket_slot, SOCKET_SLOT_EMPTY);
		pPentagram->Convert(true);
		pPlayer->SendInventory(lpMsg->source_slot);

		if ( ignore_fail_rate )
		{
			pPlayer->DecreaseItemDurabilityByUse(lpMsg->blessing_slot, 1.0f);
		}

		this->DelPentagramJewelInfo(pPlayer, lpPentagramJewelInfo->GetIndex(), PENTAGRAM_JEWEL_TYPE_INVENTORY);
		this->GCPentagramJewelResultSend(pPlayer, 1);
	}
	else
	{
		if (sGameServer->IsErrtelToRecovery())
		{
			add_item.common.talisman = sGameServer->GetErrtelRemoveFailCount();

			pPlayer->AddRecoveryMixItem(&add_item);
		}

		pMsg.result = 2;
		pMsg.type = lpPentagramJewelInfo->GetType();
		pMsg.index = lpPentagramJewelInfo->GetIndex();

		pPlayer->SEND_PCT(pMsg);

		pPentagram->SetSocket(lpMsg->socket_slot, SOCKET_SLOT_EMPTY);
		pPentagram->Convert(true);
		pPlayer->SendInventory(lpMsg->source_slot);

		this->DelPentagramJewelInfo(pPlayer, lpPentagramJewelInfo->GetIndex(), PENTAGRAM_JEWEL_TYPE_INVENTORY);
		this->GCPentagramJewelResultSend(pPlayer,2);
	}

	this->FixPentagram(pPlayer);
}

void PentagramSystem::CGPentagramJewelRefineRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetPersonalStore()->IsBusy() )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::Adniel, 0) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(PMSG_PENTAGRAM_JEWEL_REFINE_RECV, lpMsg, Packet, 0);

	MixHandler(pPlayer, 2).PentagramMix(lpMsg->type, lpMsg->slot, lpMsg->data);
}

void PentagramSystem::CGPentagramJewelUpgradeRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetPersonalStore()->IsBusy() )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::Adniel, 0) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(PMSG_PENTAGRAM_JEWEL_UPGRADE_RECV, lpMsg, Packet, 0);

	MixHandler(pPlayer, 3).PentagramMixErrtel(lpMsg->type, lpMsg->data, lpMsg->slot);
}

void PentagramSystem::CGPentagramUpgradeRadianceSlot(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	PENTAGRAM_UPGRADE_RACIANCE_SLOT_RESULT pMsg;

	POINTER_PCT_LOG(PENTAGRAM_UPGRADE_RACIANCE_SLOT, lpMsg, Packet, 0);

	if ( !use_inventory_range(lpMsg->pentagram_slot) )
	{
		return;
	}

	if ( !use_inventory_range(lpMsg->expansion_item_slot) )
	{
		pMsg.result = 2;
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	Item * pPentagram = pPlayer->GetInventory()->GetItem(lpMsg->pentagram_slot);
	Item * pExpansionItem = pPlayer->GetInventory()->GetItem(lpMsg->expansion_item_slot);

	if ( !pPentagram->IsPentagramItem() )
	{
		return;
	}

	if ( pPentagram->GetSocket(4) != SOCKET_SLOT_NONE )
	{
		pMsg.result = 3;
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( pExpansionItem->GetItem() != ITEMGET(14, 316) )
	{
		pMsg.result = 2;
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	pPlayer->GetInventory()->GetItem(lpMsg->pentagram_slot)->SetSocket(4, SOCKET_SLOT_EMPTY);
	pPlayer->GetInventory()->GetItem(lpMsg->pentagram_slot)->Convert(true);

	pPlayer->ClearItem(lpMsg->expansion_item_slot);
	pPlayer->SendInventory(lpMsg->pentagram_slot);

	pPlayer->SEND_PCT(pMsg);
}

void PentagramSystem::GCPentagramJewelResultSend(Player* pPlayer, uint8 result)
{
	PMSG_PENTAGRAM_JEWEL_RESULT_SEND pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void PentagramSystem::GCPentagramJewelInfoSend(Player* pPlayer, uint8 type)
{
	PentagramJewelInfo* pPentagramJewelInfoStr = (type == PENTAGRAM_JEWEL_TYPE_INVENTORY) ? pPlayer->GetPentagramJewelInfoInventoryPtr() : pPlayer->GetPentagramJewelInfoWarehousePtr();

	uint8 buffer[8192];
	POINTER_PCT(PENTAGRAM_ERRTEL_DATA_HEAD, head, buffer, 0);
	POINTER_PCT(PENTAGRAM_ERRTEL_DATA_BODY, body, buffer, sizeof(PENTAGRAM_ERRTEL_DATA_HEAD));
	head->result = 0;
	head->count = 0;
	head->type = type;

	for (int32 n = 0; n < MAX_PENTAGRAM_JEWEL_INFO; ++n)
	{
		if (pPentagramJewelInfoStr[n].GetIndex() != 0xFF)
		{
			auto & data = body[head->count++];

			data.type = pPentagramJewelInfoStr[n].GetType();
			data.index = pPentagramJewelInfoStr[n].GetIndex();
			data.attribute = pPentagramJewelInfoStr[n].GetAttribute();
			data.item_type = pPentagramJewelInfoStr[n].GetItemSection();
			data.item_index = pPentagramJewelInfoStr[n].GetItemType();
			data.item_level = pPentagramJewelInfoStr[n].GetItemLevel();

			SOCKET_SLOT_LOOP(i)
			{
				auto & errtel_rank = data.rank[i];

				errtel_rank.index = pPentagramJewelInfoStr[n].GetRankData(i)->GetIndex();
				errtel_rank.level = pPentagramJewelInfoStr[n].GetRankData(i)->GetLevel();
			}
		}
	}

	head->result = ((head->count > 0) ? 1 : 0);
	head->Set(0xEE, 0x01, sizeof(PENTAGRAM_ERRTEL_DATA_HEAD)+(head->count * sizeof(PENTAGRAM_ERRTEL_DATA_BODY)));

	pPlayer->sendPacket(buffer);
}

void PentagramSystem::GCPentagramJewelTradeInfoSend(Player* pPlayer01, Player* pPlayer02, Item* pItem)
{
	if (!pItem->IsItem() || !pItem->IsPentagramItem())
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(PENTAGRAM_ERRTEL_DATA_HEAD, head, buffer, 0);
	POINTER_PCT(PENTAGRAM_ERRTEL_DATA_BODY, body, buffer, sizeof(PENTAGRAM_ERRTEL_DATA_HEAD));
	head->result = 1;
	head->count = 0;
	head->type = PENTAGRAM_JEWEL_TYPE_TRADE;

	SOCKET_SLOT_LOOP(n)
	{
		auto lpPentagramJewelInfo = this->GetPentagramJewelInfo(pPlayer01, pItem->GetSocket(n) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

		if (lpPentagramJewelInfo)
		{
			auto & data = body[head->count++];

			data.type = PENTAGRAM_JEWEL_TYPE_TRADE;
			data.index = lpPentagramJewelInfo->GetIndex();
			data.attribute = lpPentagramJewelInfo->GetAttribute();
			data.item_type = lpPentagramJewelInfo->GetItemSection();
			data.item_index = lpPentagramJewelInfo->GetItemType();
			data.item_level = lpPentagramJewelInfo->GetItemLevel();

			SOCKET_SLOT_LOOP(i)
			{
				auto & rank_data = data.rank[i];

				rank_data.index = lpPentagramJewelInfo->GetRankData(i)->GetIndex();
				rank_data.level = lpPentagramJewelInfo->GetRankData(i)->GetLevel();
			}
		}
	}

	head->Set(0xEE, 0x01, sizeof(PENTAGRAM_ERRTEL_DATA_HEAD)+(head->count * sizeof(PENTAGRAM_ERRTEL_DATA_BODY)));

	if (head->count > 0)
	{
		pPlayer02->sendPacket(buffer);
	}
}

void PentagramSystem::GCPentagramJewelPShopInfoSend(Player* pPlayer01, Player* pPlayer02, uint32 slot)
{
	uint8 buffer[8192];
	POINTER_PCT(PENTAGRAM_ERRTEL_DATA_HEAD, head, buffer, 0);
	POINTER_PCT(PENTAGRAM_ERRTEL_DATA_BODY, body, buffer, sizeof(PENTAGRAM_ERRTEL_DATA_HEAD));
	head->result = 1;
	head->count = 0;
	head->type = PENTAGRAM_JEWEL_TYPE_PSHOP;

	PersonalStoreItemMap map;
	pPlayer02->GetPersonalStore()->GetItemList(slot, map);

	for (auto & itr : map)
	{
		auto pItem = itr.second;

		if (pItem && pItem->IsItem() && pItem->IsPentagramItem())
		{
			SOCKET_SLOT_LOOP(n)
			{
				auto lpPentagramJewelInfo = this->GetPentagramJewelInfo(pPlayer02, pItem->GetSocket(n) & 0xFF, PENTAGRAM_JEWEL_TYPE_INVENTORY);

				if (lpPentagramJewelInfo)
				{
					auto & data = body[head->count++];

					data.type = PENTAGRAM_JEWEL_TYPE_PSHOP;
					data.index = lpPentagramJewelInfo->GetIndex();
					data.attribute = lpPentagramJewelInfo->GetAttribute();
					data.item_type = lpPentagramJewelInfo->GetItemSection();
					data.item_index = lpPentagramJewelInfo->GetItemType();
					data.item_level = lpPentagramJewelInfo->GetItemLevel();
					
					SOCKET_SLOT_LOOP(i)
					{
						auto & rank_data = data.rank[i];

						rank_data.index = lpPentagramJewelInfo->GetRankData(i)->GetIndex();
						rank_data.level = lpPentagramJewelInfo->GetRankData(i)->GetLevel();
					}
				}
			}
		}
	}

	if (head->count <= 0)
	{
		return;
	}

	head->Set(0xEE, 0x01, sizeof(PENTAGRAM_ERRTEL_DATA_HEAD)+(head->count * sizeof(PENTAGRAM_ERRTEL_DATA_BODY)));
	pPlayer01->sendPacket(buffer);
}

void PentagramSystem::Log(Player* pPlayer, uint8 type, uint8 spirit_blessing, uint8 result, std::string const& pentagram_data, std::string const& errtel_data)
{
	if ( !pPlayer )
	{
		return;
	}

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_ERRTEL_PENTAGRAM);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt32(pPlayer->GetAccountData()->GetGUID());
	stmt->setString(pPlayer->GetAccountData()->GetAccount());
	stmt->setUInt32(pPlayer->GetGUID());
	stmt->setString(pPlayer->GetName());
	stmt->setString(pPlayer->BuildLogDB());
	stmt->setUInt8(type);
	stmt->setUInt8(spirit_blessing);
	stmt->setUInt8(result);
	stmt->setString(pentagram_data);
	stmt->setString(errtel_data);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

std::string PentagramJewelInfo::BuildLogDB() const
{
	std::ostringstream sstream;

	sstream << "[{";

	sstream << "\"type\":" << uint32(this->GetType());
	sstream << ",\"index\":" << uint32(this->GetIndex());
	sstream << ",\"serial_server\":" << this->GetSerialServer();
	sstream << ",\"serial\":" << this->GetSerial();
	sstream << ",\"item\":" << uint32(ITEMGET(this->GetItemSection(), this->GetItemType()));
	sstream << ",\"option_rank1\":" << uint32(this->GetRankData(0)->GetIndex());
	sstream << ",\"level_rank1\":" << uint32(this->GetRankData(0)->GetLevel());
	sstream << ",\"option_rank2\":" << uint32(this->GetRankData(1)->GetIndex());
	sstream << ",\"level_rank2\":" << uint32(this->GetRankData(1)->GetLevel());
	sstream << ",\"option_rank3\":" << uint32(this->GetRankData(2)->GetIndex());
	sstream << ",\"level_rank3\":" << uint32(this->GetRankData(2)->GetLevel());
	sstream << ",\"option_rank4\":" << uint32(this->GetRankData(3)->GetIndex());
	sstream << ",\"level_rank4\":" << uint32(this->GetRankData(3)->GetLevel());
	sstream << ",\"option_rank5\":" << uint32(this->GetRankData(4)->GetIndex());
	sstream << ",\"level_rank5\":" << uint32(this->GetRankData(4)->GetLevel());
	sstream << ",\"attribute\":" << uint32(this->GetAttribute());
	sstream << ",\"flags\":" << this->GetDBFlag();
	sstream << ",\"data_1\":" << this->GetData(0);
	sstream << ",\"data_2\":" << this->GetData(1);
	sstream << ",\"data_3\":" << this->GetData(2);

	sstream << "}]";

	return sstream.str();
}

void PentagramSystem::FixPentagram(Player* pPlayer)
{
	if (!sGameServer->IsPentagramErrtelFix())
	{
		return;
	}

	if (!pPlayer)
	{
		return;
	}

	std::map<uint16, uint8> StorageErrtelData;

	full_inventory_loop(i)
	{
		Item* pItem = pPlayer->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsPentagramItem())
		{
			continue;
		}

		bool send = false;

		SOCKET_SLOT_LOOP(n)
		{
			if (pItem->GetSocket(n) != SOCKET_SLOT_NONE &&
				pItem->GetSocket(n) != SOCKET_SLOT_EMPTY)
			{
				std::map<uint16, uint8>::const_iterator itr = StorageErrtelData.find(pItem->GetSocket(n));

				PentagramJewelInfo const* pPentagramJewelInfo = this->GetPentagramJewelInfo(pPlayer, pItem->GetSocket(n), PENTAGRAM_JEWEL_TYPE_INVENTORY);

				if (itr != StorageErrtelData.end())
				{
					sLog->outInfo(LOG_ITEMS, "%s(DUPLICATED INVENTORY ERRTEL INFORMATION) %s -- %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pItem->BuildLog(i).c_str(), pPentagramJewelInfo ? pPentagramJewelInfo->BuildLogDB().c_str() : "NO ERRTEL DATA");

					this->Log(pPlayer, 2, 0, 0, pItem->BuildLog(i), pPentagramJewelInfo ? pPentagramJewelInfo->BuildLogDB().c_str() : "NO ERRTEL DATA");

					pItem->SetSocket(n, SOCKET_SLOT_EMPTY);
					pItem->Convert(true);
					send = true;
				}
				else
				{
					if (!pPentagramJewelInfo)
					{
						sLog->outInfo(LOG_ITEMS, "%s(WRONG INVENTORY ERRTEL INFORMATION) %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pItem->BuildLog(i).c_str());

						this->Log(pPlayer, 3, 0, 0, pItem->BuildLog(i), "NO ERRTEL DATA");

						pItem->SetSocket(n, SOCKET_SLOT_EMPTY);
						pItem->Convert(true);
						send = true;
					}
					else
					{
						StorageErrtelData[pItem->GetSocket(n)] = 1;
					}
				}
			}
		}

		if (send)
		{
			pPlayer->SendInventory(i);
		}
	}

	StorageErrtelData.clear();

	warehouse_loop(i)
	{
		Item* pItem = pPlayer->GetWarehouse()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsPentagramItem())
		{
			continue;
		}

		SOCKET_SLOT_LOOP(n)
		{
			if (pItem->GetSocket(n) != SOCKET_SLOT_NONE &&
				pItem->GetSocket(n) != SOCKET_SLOT_EMPTY)
			{
				std::map<uint16, uint8>::const_iterator itr = StorageErrtelData.find(pItem->GetSocket(n));

				PentagramJewelInfo const* pPentagramJewelInfo = this->GetPentagramJewelInfo(pPlayer, pItem->GetSocket(n), PENTAGRAM_JEWEL_TYPE_WAREHOUSE);

				if (itr != StorageErrtelData.end())
				{
					sLog->outInfo(LOG_ITEMS, "%s(DUPLICATED WAREHOUSE ERRTEL INFORMATION) %s -- %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pItem->BuildLog(i).c_str(), pPentagramJewelInfo ? pPentagramJewelInfo->BuildLogDB().c_str() : "NO ERRTEL DATA");

					this->Log(pPlayer, 4, 0, 0, pItem->BuildLog(i), pPentagramJewelInfo ? pPentagramJewelInfo->BuildLogDB().c_str() : "NO ERRTEL DATA");

					pItem->SetSocket(n, SOCKET_SLOT_EMPTY);
					pItem->Convert(true);
				}
				else
				{
					if (!pPentagramJewelInfo)
					{
						sLog->outInfo(LOG_ITEMS, "%s(WRONG WAREHOUSE ERRTEL INFORMATION) %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), pItem->BuildLog(i).c_str());

						this->Log(pPlayer, 5, 0, 0, pItem->BuildLog(i), "NO ERRTEL DATA");

						pItem->SetSocket(n, SOCKET_SLOT_EMPTY);
						pItem->Convert(true);
					}
					else
					{
						StorageErrtelData[pItem->GetSocket(n)] = 1;
					}
				}
			}
		}
	}
}