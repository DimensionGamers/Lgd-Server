// MuunSystem.cpp: implementation of the MuunSystem class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MuunSystem::MuunSystem()
{
	this->SetActive(true);
}

MuunSystem::~MuunSystem()
{
	MAP_CLEAR(MuunSystemInfoMap::iterator, this->m_MuunSystemInfo);
	MAP_CLEAR(MuunSystemOptionInfoMap::iterator, this->m_MuunSystemOptionInfo);
	MAP_CLEAR(MuunSystemEnergyMap::iterator, this->m_MuunSystemEnergy);
	MAP_CLEAR(MuunSystemExchangeMap::iterator, this->m_MuunSystemExchange);
}

void MuunSystem::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Muun...");

	MAP_CLEAR(MuunSystemInfoMap::iterator, this->m_MuunSystemInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM muun");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			MuunSystemInfo * info = new MuunSystemInfo;

			info->SetIndex(fields[i++].GetUInt16());
			info->SetRank(fields[i++].GetUInt8());
			info->SetOptionIndex(fields[i++].GetUInt8());
			info->SetSpecialOptionType(fields[i++].GetUInt8());
			info->SetSpecialOptionValue(fields[i++].GetInt32());
			info->SetEvolutionItemIndex(fields[i++].GetUInt16());
			
			this->m_MuunSystemInfo[info->GetIndex()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u muun definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void MuunSystem::LoadOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Muun Option...");

	MAP_CLEAR(MuunSystemOptionInfoMap::iterator, this->m_MuunSystemOptionInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM muun_option");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			MuunSystemOptionInfo * info = new MuunSystemOptionInfo;

			info->SetIndex(fields[i++].GetUInt8());
			info->SetOptionValue(0, fields[i++].GetInt32());
			info->SetOptionValue(1, fields[i++].GetInt32());
			info->SetOptionValue(2, fields[i++].GetInt32());
			info->SetOptionValue(3, fields[i++].GetInt32());
			info->SetOptionValue(4, fields[i++].GetInt32());
			info->SetMaxOptionValue(fields[i++].GetInt32());
			info->SetMapZone(fields[i++].GetUInt16());
			info->SetPlayTime(fields[i++].GetInt32());
			info->SetDayOfWeek(fields[i++].GetInt32());
			info->SetMinHour(fields[i++].GetInt32());
			info->SetMaxHour(fields[i++].GetInt32());
			info->SetMinLevel(fields[i++].GetInt32());
			info->SetMaxLevel(fields[i++].GetInt32());
			info->SetMinMasterLevel(fields[i++].GetInt32());
			info->SetMaxMasterLevel(fields[i++].GetInt32());
			
			this->m_MuunSystemOptionInfo[info->GetIndex()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u muun option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MuunSystem::LoadEnergy()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Muun Energy...");

	MAP_CLEAR(MuunSystemEnergyMap::iterator, this->m_MuunSystemEnergy);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM muun_energy");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			MuunSystemEnergy * info = new MuunSystemEnergy;

			info->SetRating(fields[i++].GetUInt8());
			info->SetValue(0, fields[i++].GetUInt8());
			info->SetValue(1, fields[i++].GetUInt8());
			info->SetValue(2, fields[i++].GetUInt8());
			info->SetValue(3, fields[i++].GetUInt8());
			info->SetValue(4, fields[i++].GetUInt8());
			info->SetEvolutionValue(fields[i++].GetUInt8());
			
			this->m_MuunSystemEnergy[info->GetRating()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u muun energy definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MuunSystem::LoadExchange()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Muun Exchange...");

	MAP_CLEAR(MuunSystemExchangeMap::iterator, this->m_MuunSystemExchange);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM muun_exchange");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			MuunSystemExchange * info = new MuunSystemExchange;

			info->SetID(fields[i++].GetUInt8());
			info->SetCount(fields[i++].GetUInt8());
			info->SetMuun(fields[i++].GetUInt16());
			
			this->m_MuunSystemExchange[info->GetID()] = info;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u muun exchange definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool MuunSystem::IsMuunItem(uint16 item) const
{
	return this->GetMuunSystemInfo(item) != nullptr;
}

bool MuunSystem::IsMuunItem(Item const* pItem) const
{
	return this->IsMuunItem(pItem->GetItem());
}

bool MuunSystem::IsMuunUtil(uint16 item) const
{
	if( item == MUUN_EVOLUTION_STONE || item == MUUN_WIZARDY_STONE || item == MUUN_POWER_STONE )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MuunSystem::IsMuunUtil(Item const* pItem) const
{
	return this->IsMuunUtil(pItem->GetItem());
}

MuunSystemInfo const* MuunSystem::GetMuunSystemInfo(uint16 index) const
{
	MuunSystemInfoMap::const_iterator it = this->m_MuunSystemInfo.find(index);

	if( it == this->m_MuunSystemInfo.end() )
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

MuunSystemOptionInfo const* MuunSystem::GetMuunSystemOptionInfo(uint8 index) const
{
	MuunSystemOptionInfoMap::const_iterator it = this->m_MuunSystemOptionInfo.find(index);

	if( it == this->m_MuunSystemOptionInfo.end() )
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

MuunSystemEnergy const* MuunSystem::GetMuunSystemEnergyInfo(uint8 rating) const
{
	MuunSystemEnergyMap::const_iterator it = this->m_MuunSystemEnergy.find(rating);

	if( it == this->m_MuunSystemEnergy.end() )
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

MuunSystemExchange const* MuunSystem::GetMuunSystemExchangeInfo(uint8 id) const
{
	MuunSystemExchangeMap::const_iterator it = this->m_MuunSystemExchange.find(id);

	if( it == this->m_MuunSystemExchange.end() )
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

bool MuunSystem::CheckSpecialOption(Player* pPlayer, MuunSystemInfo const* lpInfo) const
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( !lpInfo )
	{
		return false;
	}

	MuunSystemOptionInfo const* pOptionInfo = this->GetMuunSystemOptionInfo(lpInfo->GetOptionIndex());

	if ( !pOptionInfo )
	{
		return false;
	}

	Custom::SystemTimer SysTime;

	//SYSTEMTIME SystemTime;
	//GetSystemTime(&SystemTime);

	if( pOptionInfo->GetMapZone() != uint16(-1) && pOptionInfo->GetMapZone() != pPlayer->GetWorldId() )
	{
		return false;
	}

	if( pOptionInfo->GetPlayTime() != -1 && ((DWORD)(pOptionInfo->GetPlayTime() * (MINUTE * IN_MILLISECONDS))) > pPlayer->GetTimer(PLAYER_TIMER_PLAY_TIME)->GetElapsed() )
	{
		return false;
	}

	if( pOptionInfo->GetDayOfWeek() != -1 && ( pOptionInfo->GetDayOfWeek() & (1 << SysTime.GetDayOfWeek())) == 0 )
	{
		return false;
	}

	if( pOptionInfo->GetMinHour() != -1 && pOptionInfo->GetMinHour() > SysTime.GetHour() )
	{
		return false;
	}

	if( pOptionInfo->GetMaxHour() != -1 && pOptionInfo->GetMaxHour() < SysTime.GetHour() )
	{
		return false;
	}

	if( pOptionInfo->GetMinLevel() != -1 && pOptionInfo->GetMinLevel() > pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() )
	{
		return false;
	}

	if( pOptionInfo->GetMaxLevel() != -1 && pOptionInfo->GetMaxLevel() < pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() )
	{
		return false;
	}

	if( pOptionInfo->GetMinMasterLevel() != -1 && (pOptionInfo->GetMinMasterLevel() > pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() || !pPlayer->IsMaster()) )
	{
		return false;
	}

	if( pOptionInfo->GetMaxMasterLevel() != -1 && (pOptionInfo->GetMaxMasterLevel() < pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() || !pPlayer->IsMaster()) )
	{
		return false;
	}

	return true;
}

bool MuunSystem::CheckItemMoveToMuunInventory(Player* pPlayer, Item * pItem, uint8 slot)
{
	if( !MUUN_INVENTORY_WEAR_RANGE(slot) )
	{
		return true;
	}

	if( !pItem->IsMuunItem() )
	{
		return false;
	}

	if ( pPlayer->GetMuunInventory()->GetItem(slot)->IsItem() )
	{
		return false;
	}

	MuunSystemInfo const* pMuunSystemInfo = this->GetMuunSystemInfo(pItem->GetItem());

	if( !pMuunSystemInfo )
	{
		return false;
	}

	for( int32 n = 0; n < MUUN_INVENTORY_WEAR_SIZE; ++n )
	{
		if( n != slot )
		{
			if ( pPlayer->GetMuunInventory()->GetItem(n)->IsItem() )
			{
				MuunSystemInfo const* pMuunSystemInfoB = this->GetMuunSystemInfo(pPlayer->GetMuunInventory()->GetItem(n)->GetItem());

				if( !pMuunSystemInfoB )
				{
					return false;
				}

				if( pPlayer->GetMuunInventory()->GetItem(n)->GetItem() == pItem->GetItem() || 
					pPlayer->GetMuunInventory()->GetItem(n)->GetItem() == pMuunSystemInfo->GetEvolutionItemIndex() || 
					pMuunSystemInfoB->GetEvolutionItemIndex() == pItem->GetItem() )
				{
					return false;
				}
			}
		}
	}

	return true;
}

void MuunSystem::GetOptionValue(Player* pPlayer, Item const* pItem, MuunSystemInfo const* lpInfo, MuunSystemOptionInfo const* lpOption, int32 & value)
{
	if( pItem->GetLevel() == 0 )
	{
		value = lpOption->GetMaxOptionValue();
	}
	else
	{
		value = lpOption->GetOptionValue(pItem->GetLevel() - 1);
	}

	if ( !pItem->GetExpireDate() )
	{
		return;
	}

	if ((pPlayer->GetWorldId() == WORLD_CASTLE_SIEGE && sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START) ||
		pPlayer->GetWorldId() == WORLD_LAST_MAN_STANDING ||
		(AW_MAP_RANGE(pPlayer->GetWorldId()) && sArkaWar->GetState() == ARKA_WAR_STATE_PLAYING))
	{
		return;
	}

	if( !pItem->IsExpired() )
	{
		switch( lpInfo->GetSpecialOptionType() )
		{
		case 1:
			{
				value = value * lpInfo->GetSpecialOptionValue();
			} break;

		case 2:
			{
				value = lpInfo->GetSpecialOptionValue();
			} break;

		case 3:
			{
			} break;
		}
	}
}

void MuunSystem::GetMuunItemSocketOption(Item & item)
{
	MuunSystemInfo const* pMuunSystemInfo = this->GetMuunSystemInfo(item.GetItem());

	if( pMuunSystemInfo )
	{
		item.SetSocketBonus(pMuunSystemInfo->GetRank());
	}
}

bool MuunSystem::GetMuunUtilSocketOption(Item & item)
{
	if( item.GetItem() == ITEMGET(13, 211) || item.GetItem() == ITEMGET(16, 211) )
	{
		uint16 evolution_item = MAKE_NUMBERW(item.GetSocket(0), item.GetSocket(1));
		item.SetLevel(0);

		if( !this->IsMuunItem(evolution_item) )
		{
			RandomValue<uint16> m_RandomValue(-1);

			for( MuunSystemInfoMap::const_iterator it = this->m_MuunSystemInfo.begin(); it != this->m_MuunSystemInfo.end(); ++it )
			{
				if( it->second->GetEvolutionItemIndex() != uint16(-1) )
				{
					m_RandomValue.AddValue(it->first, 1);
				}
			}

			evolution_item = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
			item.SetSocket(0, HIBYTE(evolution_item));
			item.SetSocket(1, LOBYTE(evolution_item));
			return true;
		}
	}

	return false;
}

bool MuunSystem::GetMuunUtilSocketOption(Item * item)
{
	if( item->GetItem() == ITEMGET(13, 211) || item->GetItem() == ITEMGET(16, 211) )
	{
		uint16 evolution_item = MAKE_NUMBERW(item->GetSocket(0), item->GetSocket(1));
		item->SetLevel(0);

		if( !this->IsMuunItem(evolution_item) )
		{
			RandomValue<uint16> m_RandomValue(-1);

			for( MuunSystemInfoMap::const_iterator it = this->m_MuunSystemInfo.begin(); it != this->m_MuunSystemInfo.end(); ++it )
			{
				if( it->second->GetEvolutionItemIndex() != uint16(-1) )
				{
					m_RandomValue.AddValue(it->first, 1);
				}
			}

			evolution_item = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
			item->SetSocket(0, HIBYTE(evolution_item));
			item->SetSocket(1, LOBYTE(evolution_item));
			return true;
		}
	}

	return false;
}

bool MuunSystem::CharacterUseMuunLevelUp(Player* pPlayer, uint8 source_slot, uint8 target_slot)
{
	if( !MUUN_INVENTORY_BASE_RANGE(source_slot) )
	{
		return false;
	}

	if( !MUUN_INVENTORY_BASE_RANGE(target_slot) )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(source_slot)->IsItem() )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(target_slot)->IsItem() )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(source_slot)->IsMuunItem() )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(target_slot)->IsMuunItem() )
	{
		return false;
	}

	if( pPlayer->GetMuunInventory()->GetItem(source_slot)->GetItem() != pPlayer->GetMuunInventory()->GetItem(target_slot)->GetItem() )
	{
		return false;
	}

	if( pPlayer->GetMuunInventory()->GetItem(source_slot)->GetLevel() != 1 || pPlayer->GetMuunInventory()->GetItem(target_slot)->GetLevel() == 0 )
	{
		return false;
	}

	if( pPlayer->GetMuunInventory()->GetItem(target_slot)->GetLevel() >= ((pPlayer->GetMuunInventory()->GetItem(target_slot)->GetSocketBonus() & 0x0F)+1))
	{
		return false;
	}

	pPlayer->GetMuunInventory()->GetItem(target_slot)->IncreaseLevel(1);
	pPlayer->GetMuunInventory()->GetItem(target_slot)->SetState(ITEM_CHANGED);
	return true;
}

bool MuunSystem::CharacterUseEvolutionStone(Player* pPlayer, uint8 source_slot, uint8 target_slot)
{
	if( !MUUN_INVENTORY_BASE_RANGE(source_slot) )
	{
		return false;
	}

	if( !MUUN_INVENTORY_BASE_RANGE(target_slot) )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(source_slot)->IsItem() )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(target_slot)->IsItem() )
	{
		return false;
	}

	if ( pPlayer->GetMuunInventory()->GetItem(source_slot)->GetItem() != MUUN_EVOLUTION_STONE )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(target_slot)->IsMuunItem() )
	{
		return false;
	}

	if( pPlayer->GetMuunInventory()->GetItem(target_slot)->GetLevel() != ((pPlayer->GetMuunInventory()->GetItem(target_slot)->GetSocketBonus() & 0x0F)+1) )
	{
		return false;
	}

	if( MAKE_NUMBERW(pPlayer->GetMuunInventory()->GetItem(source_slot)->GetSocket(0) & 0xFF, pPlayer->GetMuunInventory()->GetItem(source_slot)->GetSocket(1) & 0xFF) != pPlayer->GetMuunInventory()->GetItem(target_slot)->GetItem() )
	{
		return false;
	}

	MuunSystemInfo const* pMuunSystemInfo = this->GetMuunSystemInfo(pPlayer->GetMuunInventory()->GetItem(target_slot)->GetItem());

	if( !pMuunSystemInfo )
	{
		return false;
	}

	if( pMuunSystemInfo->GetEvolutionItemIndex() == uint16(-1) )
	{
		return false;
	}

	float durability = pPlayer->GetMuunInventory()->GetItem(target_slot)->GetDurability();
	time_t expire_date = pPlayer->GetMuunInventory()->GetItem(target_slot)->GetExpireDate();

	pPlayer->GetMuunInventory()->DeleteItem(target_slot);
	this->GCMuunItemDeleteSend(pPlayer, target_slot);

	Item item(pMuunSystemInfo->GetEvolutionItemIndex(), 0, durability);
	item.SetExpireDate(expire_date);

	sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, item, target_slot);
	
	return true;
}

bool MuunSystem::CharacterUseJewelOfLife(Player* pPlayer, uint8 source_slot, uint8 target_slot)
{
	if( !use_inventory_range(source_slot) )
	{
		return false;
	}

	if (!MUUN_INVENTORY_RANGE(target_slot))
	{
		return false;
	}

	if ( !pPlayer->GetInventory()->GetItem(source_slot)->IsItem() )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(target_slot)->IsItem() )
	{
		return false;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(target_slot)->IsMuunItem() )
	{
		return false;
	}

	if ( pPlayer->GetInventory()->GetItem(source_slot)->GetItem() != JEWEL::LIFE )
	{
		return false;
	}

	if( pPlayer->GetMuunInventory()->GetItem(target_slot)->GetDurability() == 255.0f )
	{
		return false;
	}

	pPlayer->GetMuunInventory()->GetItem(target_slot)->AlterDurability(2, 255.0f);

	this->GCMuunItemDurSend(pPlayer, target_slot, pPlayer->GetMuunInventory()->GetItem(target_slot)->GetDurability());

	return true;
}

bool MuunSystem::CharacterMuunEnergyConverter(Player* pPlayer, uint8 source_slot, uint8 target_slot)
{
	if( !MUUN_INVENTORY_BASE_RANGE(source_slot) )
	{
		return false;
	}

	if( !MUUN_INVENTORY_BASE_RANGE(target_slot) )
	{
		return false;
	}

	Item * pSourceItem = pPlayer->GetMuunInventory()->GetItem(source_slot);
	Item * pTargetItem = pPlayer->GetMuunInventory()->GetItem(target_slot);

	if ( !pSourceItem->IsItem() )
	{
		return false;
	}

	if ( !pTargetItem->IsItem() )
	{
		return false;
	}

	if ( pTargetItem->GetItem() != MUUN_WIZARDY_STONE )
	{
		return false;
	}

	if (!pSourceItem->IsMuunItem() && pSourceItem->GetItem() != MUUN_WIZARDY_STONE && pSourceItem->GetItem() != MUUN_EVOLUTION_STONE)
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(pTargetItem->GetItem());

	if ( !item_info )
	{
		return false;
	}

	int32 AddDur = 0;

	if (pSourceItem->GetItem() == MUUN_EVOLUTION_STONE)
	{
		MuunSystemInfo const* pMuunSystemInfo = this->GetMuunSystemInfo(MAKE_NUMBERW(pSourceItem->GetSocket(0) & 0xFF, pSourceItem->GetSocket(1) & 0xFF));

		if (!pMuunSystemInfo)
		{
			return false;
		}

		MuunSystemEnergy const* pMuunSystemEnergy = this->GetMuunSystemEnergyInfo(pMuunSystemInfo->GetRank());

		if (!pMuunSystemEnergy)
		{
			return false;
		}

		AddDur = pMuunSystemEnergy->GetValue(0);

		pPlayer->GetMuunInventory()->DeleteItem(source_slot);
		this->GCMuunItemDeleteSend(pPlayer, source_slot);
	}
	else if ( pSourceItem->IsMuunItem() )
	{
		MuunSystemInfo const* pMuunSystemInfo = this->GetMuunSystemInfo(pSourceItem->GetItem());

		if ( !pMuunSystemInfo )
		{
			return false;
		}

		MuunSystemEnergy const* pMuunSystemEnergy = this->GetMuunSystemEnergyInfo(pMuunSystemInfo->GetRank());

		if ( !pMuunSystemEnergy )
		{
			return false;
		}

		AddDur = pSourceItem->GetLevel() == 0 ? pMuunSystemEnergy->GetEvolutionValue() : pMuunSystemEnergy->GetValue(pSourceItem->GetLevel() - 1);

		pPlayer->ResetMount(pSourceItem->GetItem());

		pPlayer->GetMuunInventory()->DeleteItem(source_slot);
		this->GCMuunItemDeleteSend(pPlayer, source_slot);
	}
	else
	{
		int32 MaxStack = item_info->GetStackData()->GetMaxStack();
		AddDur = (int32)(MaxStack - pTargetItem->GetDurability());

		AddDur = ((AddDur > pSourceItem->GetDurability())?(int32)pSourceItem->GetDurability():AddDur);

		pSourceItem->AlterDurability(0, AddDur);

		if( pSourceItem->GetDurability() < 1.0f )
		{
			pPlayer->GetMuunInventory()->DeleteItem(source_slot);
			this->GCMuunItemDeleteSend(pPlayer, source_slot);
		}
		else
		{
			this->GCMuunItemDurSend(pPlayer, source_slot, (uint8)pSourceItem->GetDurability());
		}
	}
	
	pTargetItem->AlterDurability(1, AddDur);

	if ( pPlayer->GetMuunInventory()->GetItem(target_slot)->GetDurability() >= item_info->GetStackData()->GetMaxStack() )
	{
		pPlayer->GetMuunInventory()->DeleteItem(target_slot);
		this->GCMuunItemDeleteSend(pPlayer, target_slot);

		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(MUUN_POWER_STONE), target_slot);
	}
	else
	{
		this->GCMuunItemDurSend(pPlayer, target_slot, pTargetItem->GetDurability());
	}

	this->GCMuunItemListSend(pPlayer);
	this->GCMuunItemExpireSend(pPlayer);

	return true;
}

void MuunSystem::MuunSprite(Player* pPlayer, int32 damage)
{
	if( !pPlayer )
	{
		return;
	}

	for( int32 n = 0; n < MUUN_INVENTORY_WEAR_SIZE; ++n )
	{
		if ( !pPlayer->GetMuunInventory()->GetItem(n)->IsItem() ||
			 !pPlayer->GetMuunInventory()->GetItem(n)->IsMuunItem() ||
			 pPlayer->GetMuunInventory()->GetItem(n)->GetDurability() <= 0.0f )
		{
			continue;
		}

		float DurabilityValue = ((((damage*2.0f)/400.0f)/400.0f)*sGameServer->GetMuunDurabilityRate())/100.0f;

		if ( (pPlayer->GetMuunInventory()->GetItem(n)->GetDurability() - DurabilityValue) >= 1.0f )
		{
			pPlayer->GetMuunInventory()->GetItem(n)->AlterDurability(0, DurabilityValue);
			this->GCMuunItemDurSend(pPlayer, n, (uint8)pPlayer->GetMuunInventory()->GetItem(n)->GetDurability());
		}
		else
		{
			pPlayer->GetMuunInventory()->GetItem(n)->AlterDurability(2, 0.0f);
			this->GCMuunItemDurSend(pPlayer, n, (uint8)pPlayer->GetMuunInventory()->GetItem(n)->GetDurability());
		}
	}
}

void MuunSystem::CalcMuunOption(Player* pPlayer, bool flag)
{
	pPlayer->SetData(PLAYER_DATA_MUUN_SKILL_ATTACK_DELAY, sGameServer->GetMuunAttackDelay());

	if (pPlayer->GetWorldId() == WORLD_VULCANUS_ROOM && sGameServer->IsDuelMuunDisabled())
	{
		return;
	}

	for( int32 n = 0; n < MUUN_INVENTORY_WEAR_SIZE; ++n )
	{
		if ( !pPlayer->GetMuunInventory()->GetItem(n)->IsItem() ||
			 !pPlayer->GetMuunInventory()->GetItem(n)->IsMuunItem() ||
			 pPlayer->GetMuunInventory()->GetItem(n)->GetDurability() <= 0.0f )
		{
			continue;
		}

		MuunSystemInfo const* pMuunSystemInfo = this->GetMuunSystemInfo(pPlayer->GetMuunInventory()->GetItem(n)->GetItem());

		if( pMuunSystemInfo )
		{
			if ( !this->CheckSpecialOption(pPlayer, pMuunSystemInfo) )
			{
				continue;
			}

			MuunSystemOptionInfo const* pMuunSystemOptionInfo = this->GetMuunSystemOptionInfo(pMuunSystemInfo->GetOptionIndex());

			if( pMuunSystemOptionInfo )
			{
				this->InsertOption(pPlayer, pMuunSystemInfo, pMuunSystemOptionInfo, flag, pPlayer->GetMuunInventory()->GetItem(n));
			}
		}
	}
}

void MuunSystem::InsertOption(Player* pPlayer, MuunSystemInfo const* lpInfo, MuunSystemOptionInfo const* lpOption, bool flag, Item const* pItem)
{
	if( flag )
	{
		return;
	}

	bool evolved = lpInfo->GetEvolutionItemIndex() == uint16(-1) ? true: false;

	int32 value = 0;
	this->GetOptionValue(pPlayer, pItem, lpInfo, lpOption, value);

	switch(lpOption->GetIndex())
	{
	case 0:
	case 12:
	case 44:
	case 58:
	case 59:
	case 61:
		{
			pPlayer->IncreaseIntData(UNIT_INT_DEFENSE, value);
		} break;

	case 1:
	case 3:
	case 11:
	case 14:
	case 19:
	case 29:
	case 47:
		{
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			pPlayer->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			pPlayer->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
			pPlayer->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
			pPlayer->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		} break;

	case 2:
	case 7:
	case 8:
		{
			pPlayer->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
		} break;

	case 4:
	case 5:
	case 9:
	case 10:
	case 31:
		{
			pPlayer->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, value);
		} break;

	case 6:
	case 15:
	case 17:
		{
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			pPlayer->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
			pPlayer->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		} break;

	case 20:
	case 21:
	case 30:
		{
			pPlayer->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case 23:
	case 49:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_PVM, value);
		} break;

	case 24:
	case 33:
	case 48:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_DAMAGE_PVM, value);
		} break;

	case 13:
	case 16:
	case 18:
	case 22:
	case 25:
	case 28:
	case 34:
	case 39:
	case 54:
	case 73:
		{
			if ( pItem->GetOwnerSlot() == 0 )
			{
				pPlayer->IncreaseData(PLAYER_DATA_MUUN_SKILL_ATTACK_PVE, value);
				pPlayer->IncreaseData(PLAYER_DATA_MUUN_SKILL_ATTACK_COUNT, evolved ? lpInfo->GetRank() + 2: pItem->GetLevel());

				if( pItem->GetExpireDate() > 0 && !pItem->IsExpired() )
				{
					pPlayer->SetData(PLAYER_DATA_MUUN_SKILL_ATTACK_DELAY, sGameServer->GetMuunAttackDelay() / lpInfo->GetSpecialOptionValue());
				}
			}
		} break;

	case 26:
	case 27:
	case 52:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVM, value);
		} break;

	case 32:
	case 36:
	case 53:
	case 56:
	case 60:
	case 70:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVM, value);
		} break;

	case 35:
		{
			pPlayer->IncreaseDataFloat(PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_RATE_PVM, value);
		} break;

	case 37:
		{
			pPlayer->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, value);
		} break;

	case 38:
	case 55:
		{
			pPlayer->IncreaseDataFloat(PLAYER_DATA_DECREASE_REPAIR_FEE, value);
		} break;

	case 40:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_DAMAGE_PVM, value);

			if ( evolved )
			{
				pPlayer->IncreaseData(PLAYER_DATA_INCREASE_DAMAGE_PVP, value);
			}
		} break;

	case 41:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_DAMAGE_PVP, value);
		} break;

	case 42:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVM, value);

			if ( evolved )
			{
				pPlayer->IncreaseData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVP, value);
			}
		} break;

	case 43:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVP, value);
		} break;

	case 45:
		{
			pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, value);
			pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, value);
		} break;

	case 46:
		{
			pPlayer->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, value);
		} break;

	case 50:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVM, value);

			if ( evolved )
			{
				pPlayer->IncreaseData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVP, value);
			}
		} break;

	case 51:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVP, value);
		} break;

	case 57:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_PVM, value);
		} break;

	case 68:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_DAMAGE_PVM, value);
		} break;

	case 71:
	case 72:
		{
			pPlayer->IncreaseData(PLAYER_DATA_INCREASE_DEFENSE_PVM, value);
		} break;
	}
}

bool MuunSystem::CGMuunItemGetRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return false;
	}

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		return false;
	}

	PMSG_MUUN_ITEM_GET_SEND pMsg;
	pMsg.result = 0xFF;
	memset(pMsg.item_info, 0, MAX_ITEM_INFO);

	POINTER_PCT_LOG(PMSG_MUUN_ITEM_GET_RECV, lpMsg, Packet, 0);

	if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_LOOT) )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

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

	if ( sItemMgr->GetItemInventoryType(world_item->item.GetItem()) != ITEM_INVENTORY_TYPE_MUUN )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	pMsg.result = pPlayer->GetMuunInventory()->AddItem(world_item->item);

	if( pMsg.result == 0xFF )
	{
		pPlayer->SEND_PCT(pMsg);
		return false;
	}

	world_item->Give();	
	world_item->item.ConvertToBuffer(pMsg.item_info);

	pPlayer->SEND_PCT(pMsg);

	pPlayer->PartyItemGetNotify(world_item);

	return true;
}

void MuunSystem::CGMuunItemUseRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	POINTER_PCT_LOG(PMSG_MUUN_ITEM_USE_RECV, lpMsg, Packet, 0);

	PMSG_MUUN_ITEM_USE_SEND pMsg;
	pMsg.result = 1;
	pMsg.type = lpMsg->type;

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SEND_PCT(pMsg);
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_USE) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	switch(lpMsg->type)
	{
	case 1:
		{
			if((pMsg.result=this->CharacterUseMuunLevelUp(pPlayer, lpMsg->source_slot, lpMsg->target_slot)) != 0)
			{
				pPlayer->GetMuunInventory()->DeleteItem(lpMsg->source_slot);
				this->GCMuunItemDeleteSend(pPlayer, lpMsg->source_slot);
				this->GCMuunItemModifySend(pPlayer, lpMsg->target_slot);
			}
		} break;

	case 2:
		{
			if((pMsg.result=this->CharacterUseEvolutionStone(pPlayer, lpMsg->source_slot, lpMsg->target_slot)) != 0)
			{
				pPlayer->GetMuunInventory()->DeleteItem(lpMsg->source_slot);
				this->GCMuunItemDeleteSend(pPlayer, lpMsg->source_slot);
				this->GCMuunItemListSend(pPlayer);
				this->GCMuunItemExpireSend(pPlayer);
			}
		} break;

	case 3:
		{
			if((pMsg.result=this->CharacterUseJewelOfLife(pPlayer, lpMsg->source_slot, lpMsg->target_slot)) != 0)
			{
				pPlayer->ItemDeleteByUse(lpMsg->source_slot);
			}
		} break;

	case 4:
		{
			if((pMsg.result=this->CharacterMuunEnergyConverter(pPlayer, lpMsg->source_slot, lpMsg->target_slot)) != 0)
			{
				
			}
		} break;

	default:
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	pMsg.result = ((pMsg.result == 0) ? 1 : 0);

	pPlayer->SEND_PCT(pMsg);
}

void MuunSystem::CGMuunItemSellRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	

	POINTER_PCT_LOG(PMSG_MUUN_ITEM_SELL_RECV, lpMsg, Packet, 0);

	PMSG_MUUN_ITEM_SELL_SEND pMsg;
	pMsg.result = 0;
	pMsg.money = 0;

	if (!pPlayer->IsAuthorizationEnabled())
	{
		pPlayer->SEND_PCT(pMsg);
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_SELL_NPC) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if( !MUUN_INVENTORY_RANGE(lpMsg->slot) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	Item* lpItem = pPlayer->GetMuunInventory()->GetItem(lpMsg->slot);

	if( !lpItem->IsItem() )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	if ( !lpItem->IsFlag(ITEM_FLAG_ALLOW_SELL) )
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	lpItem->CalculatePrice();

	uint32 sell_price = lpItem->GetSellPrice();
	
	if ( pPlayer->MoneyReachMaximum(sell_price) )
	{
		pPlayer->MoneySet(sGameServer->GetMaxCharacterZen(), false);
	}
	else
	{
		pPlayer->MoneyAdd(sell_price, false);
	}

	pPlayer->ResetMount(lpItem->GetItem());

	pPlayer->GetMuunInventory()->DeleteItem(lpMsg->slot);

	pMsg.result = 1;
	pMsg.money = pPlayer->MoneyGet();

	pPlayer->SEND_PCT(pMsg);

	if( MUUN_INVENTORY_WEAR_RANGE(lpMsg->slot) )
	{
		pPlayer->CalculateCharacter();
		pPlayer->PreviewSend();

		this->GCMuunItemChangeSend(pPlayer, lpMsg->slot, true);
		this->GCMuunItemStatusSend(pPlayer, lpMsg->slot, 0);
		pPlayer->SetMuunItemStatus(lpMsg->slot, false);
	}
}

void MuunSystem::CGMuunMountRecv(Player* pPlayer, uint8 * Packet, bool from_packet)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( !pPlayer->IsPlaying() )
	{
		return;
	}

	POINTER_PCT_LOG(MUUN_MOUNT_RECV, lpMsg, Packet, 0);

	if (CC_MAP_RANGE(pPlayer->GetWorldId()))
	{
		return;
	}

	/*if (pPlayer->GetVisibleMount() == lpMsg->item)
	{
		return;
	}*/

	if (sGameServer->GetMuunMountTime() > 0 && from_packet && !pPlayer->GetTimer(PLAYER_TIMER_MUUN_MOUNT)->Elapsed(sGameServer->GetMuunMountTime()))
	{
		sLog->outInfo("antihack", "[Muun Mount]: [%u][%s][%s]", sGameServer->GetServerCode(), pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());

		pPlayer->SendMount(pPlayer);
		return;
	}

	pPlayer->SetVisibleMount(lpMsg->item);

	pPlayer->PreviewMake();

	pPlayer->SendMount();
}

void MuunSystem::CGMuunExchangeRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		this->GCMuunExchangeSend(pPlayer, 1);
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MUUN_EXCHANGE_RECV, lpMsg, Packet, 0);

	if ( pPlayer->GetMuunInventory()->GetEmptySlot() == uint8(-1) )
	{
		this->GCMuunExchangeSend(pPlayer, 1);
		return;
	}

	MuunSystemExchange const* pMuunSystemExchange = this->GetMuunSystemExchangeInfo(lpMsg->type);

	if ( !pMuunSystemExchange )
	{
		this->GCMuunExchangeSend(pPlayer, 1);
		return;
	}

	uint8 count = 0;

	for ( int32 i = MUUN_INVENTORY_WEAR_SIZE; i < MUUN_INVENTORY_SIZE; ++i )
	{
		if ( pPlayer->GetMuunInventory()->GetItem(i)->GetItem() == MUUN_POWER_STONE )
		{
			++count;
		}
	}

	if ( count < pMuunSystemExchange->GetCount() )
	{
		this->GCMuunExchangeSend(pPlayer, 1);
		return;
	}

	count = 0;

	for ( int32 i = MUUN_INVENTORY_WEAR_SIZE; i < MUUN_INVENTORY_SIZE; ++i )
	{
		if ( pPlayer->GetMuunInventory()->GetItem(i)->GetItem() == MUUN_POWER_STONE )
		{
			pPlayer->GetMuunInventory()->DeleteItem(i);
			this->GCMuunItemDeleteSend(pPlayer, i);
			++count;

			if ( count >= pMuunSystemExchange->GetCount() )
			{
				break;
			}
		}
	}

	this->GCMuunExchangeSend(pPlayer, 0);

	Item item(MUUN_EVOLUTION_STONE);
	item.SetSocket(0, HIBYTE(pMuunSystemExchange->GetMuun()));
	item.SetSocket(1, LOBYTE(pMuunSystemExchange->GetMuun()));
	
	sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, item);
}

void MuunSystem::SelectDisplayMuun(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(MUUN_SLOT_DISPLAY, lpMsg, Packet, 0);

	if (!MUUN_INVENTORY_WEAR_RANGE(lpMsg->slot))
	{
		return;
	}

	pPlayer->SetDisplayMuunSlot(lpMsg->slot);

	MUUN_DISPLAY_SLOT_VIEWPORT pMsg;
	pMsg.SetIndex(pPlayer->GetEntry());
	pMsg.slot = lpMsg->slot;

	pPlayer->SendPacketViewport(&pMsg);
}

void MuunSystem::GCMuunItemListSend(Player* pPlayer)
{
	uint8 buffer[4096];
	POINTER_PCT(PMSG_MUUN_ITEM_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(PMSG_MUUN_ITEM_LIST_BODY, body, buffer, sizeof(PMSG_MUUN_ITEM_LIST_HEAD));
	head->count = 0;

	for( int32 n = 0; n < MUUN_INVENTORY_SIZE; ++n )
	{
		if( pPlayer->GetMuunInventory()->GetItem(n)->IsItem() )
		{
			body[head->count].slot = n;
			pPlayer->GetMuunInventory()->GetItem(n)->ConvertToBuffer(body[head->count].item_info);
			++head->count;
		}
	}

	head->h.setE(0x4E, 0x02, sizeof(PMSG_MUUN_ITEM_LIST_HEAD) + (head->count * sizeof(PMSG_MUUN_ITEM_LIST_BODY)));

	pPlayer->sendPacket(buffer, head->h.get_size());
}

void MuunSystem::GCMuunItemDurSend(Player* pPlayer, uint8 slot, uint8 dur)
{
	PMSG_MUUN_ITEM_DUR_SEND pMsg;
	pMsg.slot = slot;
	pMsg.dur = dur;

	pPlayer->SEND_PCT(pMsg);
}

void MuunSystem::GCMuunItemDeleteSend(Player* pPlayer, uint8 slot)
{
	PMSG_MUUN_ITEM_DELETE_SEND pMsg;
	pMsg.slot = slot;

	pPlayer->SEND_PCT(pMsg);
}

void MuunSystem::GCMuunItemModifySend(Player* pPlayer, uint8 slot)
{
	if ( !MUUN_INVENTORY_RANGE(slot) )
	{
		return;
	}

	if ( !pPlayer->GetMuunInventory()->GetItem(slot)->IsItem() )
	{
		return;
	}

	PMSG_MUUN_ITEM_MODIFY_SEND pMsg;
	pMsg.flag = 0;
	pMsg.slot = slot;
	pPlayer->GetMuunInventory()->GetItem(slot)->ConvertToBuffer(pMsg.item_info);

	pPlayer->SEND_PCT(pMsg);

	this->GCMuunItemExpireSend(pPlayer, slot);
}

void MuunSystem::GCMuunItemChangeSend(Player* pPlayer, uint8 slot, bool remove)
{
	if ( !MUUN_INVENTORY_RANGE(slot) )
	{
		return;
	}

	PMSG_MUUN_ITEM_CHANGE_SEND pMsg;
	pMsg.index[0] = SET_NUMBERH(pPlayer->GetEntry());
	pMsg.index[1] = SET_NUMBERL(pPlayer->GetEntry());
	pPlayer->GetMuunInventory()->GetItem(slot)->ConvertToBuffer(pMsg.item_info);
	pMsg.item_info[1] = slot*16;
	pMsg.item_info[1] |= ((pPlayer->GetMuunInventory()->GetItem(slot)->GetLevel() - 1) / 2) & 0x0F;

	pPlayer->sendPacket_viewport(MAKE_PCT(pMsg));
}

void MuunSystem::GCMuunItemStatusSend(Player* pPlayer, uint8 slot, uint8 status)
{
	PMSG_MUUN_ITEM_STATUS_SEND pMsg;
	pMsg.slot = slot;
	pMsg.status = status;

	pPlayer->SEND_PCT(pMsg);
}

void MuunSystem::GCMuunExchangeSend(Player* pPlayer, uint8 result)
{
	MUUN_EXCHANGE_SEND pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void MuunSystem::GCMuunItemExpireSend(Player* pPlayer)
{
	uint8 buffer[8192];
	POINTER_PCT(MUUN_EXPIRE_DATE_HEAD, head, buffer, 0);
	POINTER_PCT(MUUN_EXPIRE_DATE_BODY, body, buffer, sizeof(MUUN_EXPIRE_DATE_HEAD));
	head->count = 0;
	head->result = 1;

	for ( int32 i = 0; i < MUUN_INVENTORY_SIZE; ++i )
	{
		Item const* pItem = pPlayer->GetMuunInventory()->GetItem(i);

		if ( !pItem->IsItem() )
		{
			continue;
		}

		if ( !pItem->GetExpireDate() )
		{
			continue;
		}

		body[head->count].item = pItem->GetItem();
		body[head->count].time = pItem->GetExpireDate();
		body[head->count].slot = i;
		++head->count;
	}

	head->h.set(0x4E, 0x20, sizeof(MUUN_EXPIRE_DATE_HEAD) + (sizeof(MUUN_EXPIRE_DATE_BODY) * head->count));

	pPlayer->sendPacket(buffer, head->h.get_size());
}

void MuunSystem::GCMuunItemExpireSend(Player* pPlayer, uint8 slot)
{
	if ( !MUUN_INVENTORY_RANGE(slot) )
	{
		return;
	}

	Item const* pItem = pPlayer->GetMuunInventory()->GetItem(slot);

	if ( !pItem->IsItem() )
	{
		return;
	}

	if ( !pItem->GetExpireDate() )
	{
		return;
	}

	MUUN_EXPIRE_DATE pMsg;
	pMsg.item = pItem->GetItem();
	pMsg.time = pItem->GetExpireDate();
	pMsg.slot = slot;

	pPlayer->SEND_PCT(pMsg);
}

void MuunSystem::GCMuunItemExpireUpdate(Player* pPlayer, uint8 slot)
{
	if ( !MUUN_INVENTORY_RANGE(slot) )
	{
		return;
	}

	Item const* pItem = pPlayer->GetMuunInventory()->GetItem(slot);

	if ( !pItem->IsItem() )
	{
		return;
	}

	if ( !pItem->GetExpireDate() )
	{
		return;
	}

	/*MUUN_EXPIRE_UPDATE pMsg;
	pMsg.expire = 1;
	pMsg.slot = slot;

	pPlayer->SEND_PCT(pMsg);*/

	PMSG_MUUN_ITEM_MODIFY_SEND pMsg2;
	pMsg2.flag = 0;
	pMsg2.slot = slot;
	pItem->ConvertToBuffer(pMsg2.item_info);

	pPlayer->SEND_PCT(pMsg2);
}

void MuunSystem::MuunAttack(Player* pPlayer)
{
	if ( !sGameServer->IsMuunAttackEnabled() )
	{
		return;
	}

	if (pPlayer->GetWorldId() == WORLD_VULCANUS_ROOM && sGameServer->IsDuelMuunDisabled())
	{
		return;
	}
	
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->GetMuunTarget() )
	{
		return;
	}

	if ( !pPlayer->GetMuunTarget()->IsPlaying() ||
		 !pPlayer->GetMuunTarget()->IsLive() ||
		 !pPlayer->SameDimension(pPlayer->GetMuunTarget()) )
	{
		pPlayer->SetMuunTarget(nullptr);
		return;
	}

	if ( !IN_RANGE(pPlayer, pPlayer->GetMuunTarget(), sGameServer->GetMuunAttackRange()) )
	{
		return;
	}

	if ( pPlayer->GetRegenStatus() != REGEN_NONE )
	{
		return;
	}

	if ( pPlayer->IsTeleporting() )
	{
		return;
	}

	if ( !pPlayer->GetTimer(PLAYER_TIMER_MUUN_ATTACK)->Elapsed(pPlayer->GetData(PLAYER_DATA_MUUN_SKILL_ATTACK_DELAY)) )
	{
		return;
	}

	Item const* pItem = pPlayer->GetMuunInventory()->GetItem(0);

	if ( !pItem || !pItem->IsItem() || pItem->GetDurability() <= 0.0f )
	{
		return;
	}

	if ( !this->CheckSpecialOption(pPlayer, this->GetMuunSystemInfo(pItem->GetItem())) )
	{
		return;
	}

	int32 count = pPlayer->GetData(PLAYER_DATA_MUUN_SKILL_ATTACK_COUNT);

	for ( int32 i = 0; i < count; ++i )
	{
		pPlayer->AddDelayed(DELAYED_MUUN_ATTACK, (i + 1) * sGameServer->GetMuunAttackHitDelay(), pPlayer->GetMuunTarget());
	}
}

void MuunSystem::ProcessMuunAttack(Player* pPlayer, Unit* pTarget)
{
	if (pPlayer->GetWorldId() == WORLD_VULCANUS_ROOM && sGameServer->IsDuelMuunDisabled())
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->AttackAllowed(pTarget) )
	{
		return;
	}

	if ( !IN_RANGE(pPlayer, pTarget, sGameServer->GetMuunAttackRange()) )
	{
		return;
	}

	if ( pPlayer->GetRegenStatus() != REGEN_NONE )
	{
		return;
	}

	if ( pPlayer->IsTeleporting() )
	{
		return;
	}

	if ( !pTarget->IsPlaying() ||
		 !pTarget->IsLive() ||
		 !pPlayer->SameDimension(pTarget) )
	{
		return;
	}

	if ( pTarget->HasBuff(BUFF_INMUNE_ATTACK) || pTarget->HasBuff(BUFF_INMUNE_SKILL) )
	{
		return;
	}

	if ( pTarget->HasBuff(BUFF_ORDER_OF_PROTECTION) )
	{
		return;
	}

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->GetAI() && !pMonster->GetAI()->EnableAttack(pPlayer, nullptr, 0) )
		{
			return;
		}
	}

	Item const* pItem = pPlayer->GetMuunInventory()->GetItem(0);

	if ( !pItem || !pItem->IsItem() || pItem->GetDurability() <= 0.0f )
	{
		return;
	}

	if ( !this->CheckSpecialOption(pPlayer, this->GetMuunSystemInfo(pItem->GetItem())) )
	{
		return;
	}

	Unit* pTargetSummoner = pTarget->GetSummoner();

	if ( !pTargetSummoner )
	{
		return;
	}

	int32 attack = pPlayer->GetData(PLAYER_DATA_MUUN_SKILL_ATTACK);
	int32 attack_pve = pPlayer->GetData(PLAYER_DATA_MUUN_SKILL_ATTACK_PVE);

	if ( attack <= 0 && pTargetSummoner->IsPlayer() )
	{
		return;
	}

	if ( attack_pve > 0 )
	{
		attack += attack_pve;
	}

	if ( attack <= 0 )
	{
		return;
	}

	MUUN_ATTACK_SEND pMsg(SKILL_FLAME_OF_EVIL, ENTRY(pPlayer), ENTRY(pTarget));

	pPlayer->sendPacket(MAKE_PCT(pMsg));
	pPlayer->sendPacket_viewport(MAKE_PCT(pMsg));

	int32 damage = pPlayer->GetTotalLevel() * attack;
	uint16 effect = 0;
	int32 defense = pPlayer->GetTargetDefense(pTarget, effect, 2);

	damage -= defense;

	int32 shield_damage = pPlayer->DealDamage(pPlayer->GetMuunTarget(), damage, 0);

	pPlayer->SendDamage(pPlayer->GetMuunTarget(), damage, shield_damage, 0, 0);

	pTarget->Unit::AfterHitCheck(pPlayer, damage, shield_damage, 0);
}

void MuunSystem::CreateMuunList(Player* pPlayer, int32 start_count)
{
	/*for ( int32 i = 0; i < MUUN_INVENTORY_SIZE; ++i )
	{
		if ( pPlayer->GetMuunInventory()->GetItem(i)->IsItem() )
		{
			pPlayer->GetMuunInventory()->DeleteItem(i);
			sMuunSystem->GCMuunItemDeleteSend(pPlayer, i);
		}
	}

	int32 count = 0;
	int32 current_count = 0;

	for ( MuunSystemInfoMap::const_iterator it = this->m_MuunSystemInfo.begin(); it != this->m_MuunSystemInfo.end(); ++it )
	{
		if ( it->second->GetEvolutionItemIndex() != uint16(-1) )
		{
			continue;
		}

		if ( count >= 20 )
		{
			break;
		}

		if ( current_count >= start_count )
		{
			sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(it->second->GetIndex()));
			++count;
		}

		++current_count;
	}*/
}

void MuunSystem::SetMuunBonus(Item & item)
{
	if (!sGameServer->IsMuunEvolveBonus())
	{
		return;
	}

	MuunSystemInfo const* pMuunSystemInfo = this->GetMuunSystemInfo(item.GetItem());

	if (!pMuunSystemInfo)
	{
		return;
	}

	if (pMuunSystemInfo->GetEvolutionItemIndex() == uint16(-1))
	{
		item.SetExpireDate(time(nullptr) + sGameServer->GetMuunEvolveBonusTime());
	}
}