MiningSystem::MiningSystem()
{

}

MiningSystem::~MiningSystem()
{
	LIST_CLEAR(MiningInfoList::iterator, this->m_MiningInfo);
}

void MiningSystem::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mining...");

	LIST_CLEAR(MiningInfoList::iterator, this->m_MiningInfo);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM mining");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			MiningInfo * info = new MiningInfo;

			info->SetIndex(fields[i++].GetUInt16());
			info->SetStage(fields[i++].GetUInt8());
			info->SetValue(fields[i++].GetUInt8());
			info->SetItemRate(fields[i++].GetUInt16());
			info->SetItemIndex(fields[i++].GetUInt16());
			info->SetItemCount(fields[i++].GetUInt16());
			info->SetBonusItemRate(fields[i++].GetUInt16());
			info->SetBonusItemIndex(fields[i++].GetUInt16());
			info->SetBonusItemCount(fields[i++].GetUInt16());

			this->m_MiningInfo.push_back(info);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mining definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

MiningInfo const* MiningSystem::GetMiningInfo(uint16 index, uint8 stage) const
{
	for( MiningInfoList::const_iterator it = this->m_MiningInfo.begin(); it != this->m_MiningInfo.end(); ++it )
	{
		if( (*it)->GetIndex() == index )
		{
			if( (*it)->GetStage() == stage )
			{
				return *it;
			}
		}
	}

	return nullptr;
}

void MiningSystem::CreateMiningReward(Player* pPlayer, uint16 item, int32 count)
{
	item_template const* pItemInfo = sItemMgr->GetItem(item);

	if (!pItemInfo)
	{
		return;
	}

	if (count <= 0)
	{
		return;
	}

	if (pItemInfo->GetStackData()->GetMaxStack() > 0)
	{
		int32 full_stack = count / pItemInfo->GetStackData()->GetMaxStack();
		int32 rest = count % pItemInfo->GetStackData()->GetMaxStack();

		for (int32 i = 0; i < full_stack; ++i)
		{
			sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(item, 0, pItemInfo->GetStackData()->GetMaxStack()));
		}

		if (rest > 0)
		{
			sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(item, 0, rest));
		}
	}
	else
	{
		for (int32 i = 0; i < count; ++i)
		{
			sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(item));
		}
	}

	sLog->outInfo("jewel", "%s -- Item: %u / Count %d / %s -- %s", __FUNCTION__, item, count, pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str());
}

void MiningSystem::CGMiningStartRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(PMSG_MINING_START_RECV, lpMsg, Packet, 0);

	if ( lpMsg->index != pPlayer->GetEntry() )
	{
		sLog->outError("jewel", "%s -- Wrong Index[%u] %s", __FUNCTION__, lpMsg->index, pPlayer->BuildLog().c_str());

		return;
	}

	Monster* pMonster = sObjectMgr->FindMonster(lpMsg->monster_index);

	if ( !pMonster || !pMonster->IsPlaying() )
	{
		sLog->outError("jewel", "%s -- Wrong Monster Index[%u] %s", __FUNCTION__, lpMsg->monster_index, pPlayer->BuildLog().c_str());

		return;
	}

	MiningInfo const* pMiningInfo = this->GetMiningInfo(lpMsg->monster_class, lpMsg->stage + 1);

	if ( !pMiningInfo )
	{
		sLog->outError("jewel", "%s -- Wrong Mining Info[%u / %u] %s", __FUNCTION__, lpMsg->monster_class, lpMsg->stage + 1, pPlayer->BuildLog().c_str());

		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 3);
		//this->Restore(pPlayer);
		return;
	}

	if ( !pMonster->IsLive() )
	{
		sLog->outError("jewel", "%s -- Monster Not Live[%u] %s", __FUNCTION__, lpMsg->monster_index, pPlayer->BuildLog().c_str());

		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 3);
		//this->Restore(pPlayer);
		return;
	}

	if ( !pPlayer->SameDimension(pMonster) )
	{
		sLog->outError("jewel", "%s -- Monster Not in Dimension[%u] %s", __FUNCTION__, lpMsg->monster_index, pPlayer->BuildLog().c_str());

		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 3);
		//this->Restore(pPlayer);
		return;
	}

	if ( pMonster->GetWorldId() != lpMsg->map )
	{
		sLog->outError("jewel", "%s -- Monster Wrong World[%u / %u-%u] %s", __FUNCTION__, lpMsg->monster_index, pMonster->GetWorldId(), lpMsg->map, pPlayer->BuildLog().c_str());

		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 3);
		//this->Restore(pPlayer);
		return;
	}

	if ( pMonster->GetMiningIndex() && pMonster->GetMiningIndex() != pPlayer )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 8);
		//this->Restore(pPlayer);
		return;
	}

	if ( pPlayer->GetSkin() != uint16(-1) )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 10);
		//this->Restore(pPlayer);
		return;
	}

	if ( pPlayer->GetFreeInventorySlots() < MAX_MINING_JEWEL_PACK )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 11);
		//this->Restore(pPlayer);
		return;
	}

	int32 distance = Util::Distance(pPlayer->GetX(), pPlayer->GetY(), pMonster->GetX(), pMonster->GetY());

	if ( distance > 2 )
	{
		sLog->outError("jewel", "%s -- Monster Wrong Distance[%u / %d] %s", __FUNCTION__, lpMsg->monster_index, distance, pPlayer->BuildLog().c_str());

		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 3);
		//this->Restore(pPlayer);
		return;
	}

	if ( lpMsg->stage == 0 && pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 12);
		//this->Restore(pPlayer);
		return;
	}

	if ( lpMsg->stage != 0 && pPlayer->GetInterfaceState()->GetID() != InterfaceData::Mining )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 12);
		//this->Restore(pPlayer);
		return;
	}

	if ( pPlayer->GetPersonalStore()->IsBusy() )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 13);
		//this->Restore(pPlayer);
		return;
	}

	if ( pPlayer->GetMiningStage() != lpMsg->stage )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 17);
		//this->Restore(pPlayer);
		return;
	}

	if ( pPlayer->GetMiningIndex() && pPlayer->GetMiningIndex() != pMonster )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 17);
		//this->Restore(pPlayer);
		return;
	}

	if ( !pPlayer->GetInventory()->GetItem(WEAPON_01)->IsItem() ||
		 pPlayer->GetInventory()->GetItem(WEAPON_01)->GetItem() != ITEMGET(0, 41) ||
		 uint8(pPlayer->GetInventory()->GetItem(WEAPON_01)->GetDurability()) < pMiningInfo->GetValue() )
	{
		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 18);
		//this->Restore(pPlayer);
		return;
	}

	pPlayer->GetInventory()->GetItem(WEAPON_01)->AlterDurability(0, pMiningInfo->GetValue());
	pPlayer->SendItemDurability(WEAPON_01, 0);

	if ( pPlayer->GetInterfaceState()->GetID() == InterfaceData::None )
	{
		pPlayer->SetMiningStatus(1);
	}

	pPlayer->GetInterfaceState()->Set(InterfaceData::Mining, nullptr);

	sLog->outInfo("jewel", "%s -- Mining Start[%u - %u - %u] %s", __FUNCTION__, pPlayer->GetMiningStatus(), pMonster->GetEntry(), pMonster->GetClass(), pPlayer->BuildLog().c_str());

	if ( Random(10000) < pMiningInfo->GetItemRate() )
	{
		pPlayer->IncreaseMiningStage(1);
		pPlayer->SetMiningIndex(pMonster);
		pMonster->SetMiningStage(1);
		pMonster->SetMiningIndex(pPlayer);

		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, pPlayer->GetMiningStage(), pMiningInfo->GetItemCount(), ((pPlayer->GetMiningStage()==MAX_MINING_STAGE)?1:0));

		sLog->outInfo("jewel", "%s -- Mining Success[%u / %u] %s", __FUNCTION__, pPlayer->GetMiningStatus(), pPlayer->GetMiningStage(), pPlayer->BuildLog().c_str());
	}
	else
	{
		pPlayer->SetMiningStage(pPlayer->GetMiningStage());
		pPlayer->SetMiningIndex(pMonster);
		pMonster->SetMiningStage(1);
		pMonster->SetMiningIndex(pPlayer);

		pPlayer->SetMiningStatus(2);

		this->GCMiningStartSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 0, 2);

		sLog->outInfo("jewel", "%s -- Mining Failed[%u / %u] %s", __FUNCTION__, pPlayer->GetMiningStatus(), pPlayer->GetMiningStage(), pPlayer->BuildLog().c_str());
	}
}

void MiningSystem::CGMiningSuccessRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(PMSG_MINING_SUCCESS_RECV, lpMsg, Packet, 0);

	if ( lpMsg->index != pPlayer->GetEntry() )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);

		sLog->outError("jewel", "%s -- WRONG ENTRY[%u] %s", __FUNCTION__, lpMsg->index, pPlayer->BuildLog().c_str());

		return;
	}

	Monster* pMonster = sObjectMgr->FindMonster(lpMsg->monster_index);

	if ( !pMonster || !pMonster->IsPlaying() )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);

		sLog->outError("jewel", "%s -- WRONG TARGET[%u] %s", __FUNCTION__, lpMsg->monster_index, pPlayer->BuildLog().c_str());

		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::Mining )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);

		sLog->outError("jewel", "%s -- WRONG INTERFACE[%s] %s", __FUNCTION__, pPlayer->GetInterfaceState()->GetID().c_str(), pPlayer->BuildLog().c_str());
		
		return;
	}

	if ( pPlayer->GetMiningIndex() != pMonster )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);

		sLog->outError("jewel", "%s -- WRONG MINING INDEX[%u] %s", __FUNCTION__, pMonster->GetEntry(), pPlayer->BuildLog().c_str());

		return;
	}

	if ( pMonster->GetMiningIndex() != pPlayer )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);

		sLog->outError("jewel", "%s -- WRONG PLAYER INDEX %s", __FUNCTION__, pPlayer->BuildLog().c_str());

		return;
	}

	MiningInfo const* pMiningInfo = this->GetMiningInfo(lpMsg->monster_class, pPlayer->GetMiningStage());

	if ( !pMiningInfo )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);
		
		sLog->outError("jewel", "%s -- WRONG INFO[%u - %u] %s", __FUNCTION__, lpMsg->monster_class, pPlayer->GetMiningStage(), pPlayer->BuildLog().c_str());
		return;
	}

	if ( pPlayer->GetMiningStage() == MAX_MINING_STAGE && pPlayer->GetMiningStage() != (lpMsg->stage + 1) )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);
		
		sLog->outError("jewel", "%s -- WRONG STAGE 1[%u - %u] %s", __FUNCTION__, pPlayer->GetMiningStage(), lpMsg->stage, pPlayer->BuildLog().c_str());
		return;
	}
	
	if ( pPlayer->GetMiningStage() != MAX_MINING_STAGE && pPlayer->GetMiningStage() != (lpMsg->stage + 0) )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);
		
		sLog->outError("jewel", "%s -- WRONG STAGE 2[%u - %u] %s", __FUNCTION__, pPlayer->GetMiningStage(), lpMsg->stage, pPlayer->BuildLog().c_str());
		return;
	}

	if ( pPlayer->GetMiningStatus() != 1 )
	{
		this->GCMiningFailSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage);
		
		sLog->outError("jewel", "%s -- WRONG STATUS[%u] %s", __FUNCTION__, pPlayer->GetMiningStatus(), pPlayer->BuildLog().c_str());
		return;
	}

	sLog->outInfo("jewel", "%s -- SUCCESS[%u - %u - %u] %s", __FUNCTION__, lpMsg->monster_index, lpMsg->monster_class, pPlayer->GetMiningStage(), pPlayer->BuildLog().c_str());

	if ( pPlayer->GetMiningStage() == MAX_MINING_STAGE )
	{
		pPlayer->FireworksSend(1);
	}

	this->CreateMiningReward(pPlayer, pMiningInfo->GetItemIndex(), pMiningInfo->GetItemCount());

	if (pMiningInfo->GetBonusItemRate() > 0 && Random(10000) < pMiningInfo->GetBonusItemRate())
	{
		sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "%s won %d Jewel of Bless", pPlayer->GetName(), pMiningInfo->GetBonusItemCount());
		this->CreateMiningReward(pPlayer, pMiningInfo->GetBonusItemIndex(), pMiningInfo->GetBonusItemCount());
	}

	pPlayer->GetInterfaceState()->Reset();
	pPlayer->SetMiningIndex(nullptr);
	pPlayer->SetMiningStage(0);
	pPlayer->SetMiningStatus(0);
	pMonster->SetMiningIndex(nullptr);
	pMonster->SetMiningStage(0);
	pMonster->Kill();

	this->GCMiningSuccessSend(pPlayer, lpMsg->monster_index, lpMsg->monster_class, lpMsg->map, lpMsg->stage, 3);
}

void MiningSystem::CGMiningFailureRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(PMSG_MINING_FAILURE_RECV, lpMsg, Packet, 0);

	if ( lpMsg->index != pPlayer->GetEntry() )
	{
		sLog->outError("jewel", "%s -- WRONG ENTRY[%u] %s", __FUNCTION__, lpMsg->index, pPlayer->BuildLog().c_str());

		return;
	}

	Monster* pMonster = sObjectMgr->FindMonster(lpMsg->monster_index);

	if ( !pMonster || !pMonster->IsPlaying() )
	{
		sLog->outError("jewel", "%s -- WRONG TARGET[%u] %s", __FUNCTION__, lpMsg->monster_index, pPlayer->BuildLog().c_str());

		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::Mining )
	{
		sLog->outError("jewel", "%s -- WRONG INTERFACE[%s] %s", __FUNCTION__, pPlayer->GetInterfaceState()->GetID().c_str(), pPlayer->BuildLog().c_str());

		return;
	}

	if ( pPlayer->GetMiningIndex() != pMonster )
	{
		sLog->outError("jewel", "%s -- WRONG MINING INDEX[%u] %s", __FUNCTION__, pMonster->GetEntry(), pPlayer->BuildLog().c_str());

		return;
	}

	if ( pMonster->GetMiningIndex() != pPlayer )
	{
		sLog->outError("jewel", "%s -- WRONG PLAYER INDEX %s", __FUNCTION__, pPlayer->BuildLog().c_str());

		return;
	}

	sLog->outInfo("jewel", "%s -- FAIL[%u - %u - %u] %s", __FUNCTION__, lpMsg->monster_index, pMonster->GetClass(), pPlayer->GetMiningStage(), pPlayer->BuildLog().c_str());

	pPlayer->GetInterfaceState()->Reset();
	pPlayer->SetMiningIndex(nullptr);
	pPlayer->SetMiningStage(0);
	pPlayer->SetMiningStatus(0);
	pMonster->SetMiningIndex(nullptr);
	pMonster->SetMiningStage(0);
	pMonster->Kill();
}

void MiningSystem::GCMiningStartSend(Player* pPlayer, uint16 monster_index, uint16 monster_class, uint8 map, uint8 stage, uint32 value, uint8 result)
{
	PMSG_MINING_START_SEND pMsg;
	pMsg.index = pPlayer->GetEntry();
	pMsg.MonsterIndex = monster_index;
	pMsg.MonsterClass = monster_class;
	pMsg.map = map;
	pMsg.stage = stage;
	pMsg.value = value;
	pMsg.result = result;

	pPlayer->SEND_PCT(pMsg);
}

void MiningSystem::GCMiningSuccessSend(Player* pPlayer, uint16 monster_index, uint16 monster_class, uint8 map, uint8 stage, uint8 result)
{
	PMSG_MINING_SUCCESS_SEND pMsg;
	pMsg.index = pPlayer->GetEntry();
	pMsg.MonsterIndex = monster_index;
	pMsg.MonsterClass = monster_class;
	pMsg.map = map;
	pMsg.stage = stage;
	pMsg.value = result;

	pPlayer->SEND_PCT(pMsg);
}

void MiningSystem::GCMiningFailSend(Player* pPlayer, uint16 monster_index, uint16 monster_class, uint8 map, uint8 stage)
{
	PMSG_MINING_CANCEL pMsg;
	pMsg.index = pPlayer->GetEntry();
	pMsg.MonsterIndex = monster_index;
	pMsg.MonsterClass = monster_class;
	pMsg.map = map;
	pMsg.stage = stage;

	pPlayer->SEND_PCT(pMsg);
}

void MiningSystem::Restore(Player* pPlayer)
{
	if ( pPlayer )
	{
		if ( pPlayer->GetInterfaceState()->GetID() == InterfaceData::Mining )
		{
			pPlayer->GetInterfaceState()->Reset();
		}

		Unit* pMiningIndex = pPlayer->GetMiningIndex();

		if ( pMiningIndex )
		{
			if ( pMiningIndex->GetMiningIndex() == pPlayer )
			{
				pMiningIndex->SetMiningIndex(nullptr);
				pMiningIndex->SetMiningStage(0);
			}
		}

		pPlayer->SetMiningIndex(nullptr);
		pPlayer->SetMiningStage(0);
	}
}

void MiningSystem::UpdateMining(Unit* pUnit)
{
	if ( pUnit->GetMiningIndex() )
	{
		if ( !pUnit->GetMiningIndex()->IsPlaying() || 
			 !pUnit->GetMiningIndex()->IsLive() ||
			 pUnit->GetMiningIndex()->GetMiningIndex() != pUnit )
		{
			pUnit->SetMiningIndex(nullptr);
			pUnit->SetMiningStage(0);

			if ( Player* pPlayer = pUnit->ToPlayer() )
			{
				if ( pPlayer->GetInterfaceState()->GetID() == InterfaceData::Mining )
				{
					pPlayer->GetInterfaceState()->Reset();
				}
			}
		}
	}
}