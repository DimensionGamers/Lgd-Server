bool Player::InterfaceSharedCheck(Player* pPlayer01, Player* pPlayer02, std::string const& interface_id, uint8 state)
{
	if ( !pPlayer01 || !pPlayer02 )
		return false;

	if ( pPlayer01->GetInterfaceState()->GetID() != interface_id ||
		 pPlayer02->GetInterfaceState()->GetID() != interface_id )
	{
		return false;
	}

	if ( pPlayer01->GetInterfaceState()->GetState() != state ||
		 pPlayer02->GetInterfaceState()->GetState() != state )
	{
		return false;
	}

	if ( pPlayer01->GetInterfaceState()->GetTarget() != pPlayer02 ||
		 pPlayer02->GetInterfaceState()->GetTarget() != pPlayer01 )
	{
		return false;
	}

	return true;
}

bool Player::InterfaceCheck(Player* pPlayer, std::string const& interface_id, uint8 state)
{
	if ( !pPlayer )
		return false;

	if ( pPlayer->GetInterfaceState()->GetID() != interface_id )
		return false;

	if ( pPlayer->GetInterfaceState()->GetState() != state )
		return false;

	return true;
}

bool Player::TransactionSharedSerialCheck(Player* pPlayer01, Player* pPlayer02, std::string const& transaction)
{
	if ( !pPlayer01 || !pPlayer02 )
		return false;

	bool duplicated_founded = false;

	use_inventory_loop(i)
	{
		Item * pItem01 = pPlayer01->GetInventory()->GetItem(i);

		if ( !pItem01 || !pItem01->IsItem() )
			continue;

		use_inventory_loop(h)
		{
			Item * pItem02 = pPlayer02->GetInventory()->GetItem(h);

			if ( !pItem02 || !pItem02->IsItem() )
				continue;

			if ( pItem01->GetSerial() == pItem02->GetSerial() &&
				 pItem01->GetSerialServer() == pItem02->GetSerialServer() )
			{
				sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] - [%s] Founded same item serial. First Item: %s ..... Second Item: %s",
					transaction.c_str(), pPlayer01->BuildLog().c_str(), pPlayer02->BuildLog().c_str(), 
					pItem01->BuildLog(i).c_str(), pItem02->BuildLog(h).c_str());

				if ( sGameServer->IsTransactionSharedDuplicatedSerialDelete() )
					pPlayer01->ClearItem(i, 1);

				duplicated_founded = true;
			}
		}
	}

	if ( duplicated_founded )
	{
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duplicated Item Serial Founded. Contact Administrators.");
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duplicated Item Serial Founded. Contact Administrators.");

		sAuthServer->AccountCompleteBan(pPlayer01->GetAccountData()->GetIP(), pPlayer01->GetAccountData()->GetMac(), pPlayer01->GetAccountData()->GetDiskSerial(), 4);
		sAuthServer->AccountCompleteBan(pPlayer02->GetAccountData()->GetIP(), pPlayer02->GetAccountData()->GetMac(), pPlayer02->GetAccountData()->GetDiskSerial(), 5);
	}

	return duplicated_founded;
}

bool Player::TransactionSerialCheck(Player* pPlayer, std::string const& transaction)
{
	if ( !pPlayer )
		return false;

	bool duplicated_founded = false;
	Item * pItem01 = nullptr;
	Item * pItem02 = nullptr;

	use_inventory_loop(i)
	{
		pItem01 = pPlayer->GetInventory()->GetItem(i);

		if ( !pItem01 || !pItem01->IsItem() )
			continue;

		use_inventory_loop(h)
		{
			if ( i == h )
				continue;

			pItem02 = pPlayer->GetInventory()->GetItem(h);

			if ( !pItem02 || !pItem02->IsItem() )
				continue;

			if ( pItem01->GetSerial() == pItem02->GetSerial() &&
				 pItem01->GetSerialServer() == pItem02->GetSerialServer() )
			{
				sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial. First Item: %s ..... Second Item: %s",
					transaction.c_str(), pPlayer->BuildLog().c_str(), pItem01->BuildLog(i).c_str(), pItem02->BuildLog(h).c_str());

				if ( sGameServer->IsTransactionDuplicatedSerialDelete() )
					pPlayer->ClearItem(h, 1);

				duplicated_founded = true;
			}
		}

		for ( uint8 h = 0; h < warehouse_size; ++h )
		{
			pItem02 = pPlayer->GetWarehouse()->GetItem(h);

			if ( !pItem02 || !pItem02->IsItem() )
				continue;

			if ( pItem01->GetSerial() == pItem02->GetSerial() &&
				 pItem01->GetSerialServer() == pItem02->GetSerialServer() )
			{
				sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial in warehouse. First Item: %s ..... Second Item: %s",
					transaction.c_str(), pPlayer->BuildLog().c_str(), pItem01->BuildLog(i).c_str(), pItem02->BuildLog(h).c_str());

				if ( sGameServer->IsTransactionDuplicatedSerialDelete() )
					pPlayer->ClearItem(i, 1);

				duplicated_founded = true;
			}
		}
	}

	for ( uint8 i = 0; i < warehouse_size; ++i )
	{
		pItem01 = pPlayer->GetWarehouse()->GetItem(i);

		if ( !pItem01 || !pItem01->IsItem() )
			continue;

		for ( uint8 h = 0; h < warehouse_size; ++h )
		{
			if ( i == h )
				continue;

			pItem02 = pPlayer->GetWarehouse()->GetItem(h);

			if ( !pItem02 || !pItem02->IsItem() )
				continue;

			if ( pItem01->GetSerial() == pItem02->GetSerial() &&
				 pItem01->GetSerialServer() == pItem02->GetSerialServer() )
			{
				sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial in warehouse. First Item: %s ..... Second Item: %s",
					transaction.c_str(), pPlayer->BuildLog().c_str(), pItem01->BuildLog(i).c_str(), pItem02->BuildLog(h).c_str());

				//if ( sGameServer->IsTransactionDuplicatedSerialDelete() )
				//	pPlayer->ClearItem(i, 1);

				duplicated_founded = true;
			}
		}
	}

	if ( duplicated_founded )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duplicated Item Serial Founded. Contact Administrators.");

		sAuthServer->AccountCompleteBan(pPlayer->GetAccountData()->GetIP(), pPlayer->GetAccountData()->GetMac(), pPlayer->GetAccountData()->GetDiskSerial(), 6);
	}

	return duplicated_founded;
}

void Player::TransactionDBSave(Player* pPlayer01, Player* pPlayer02)
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	pPlayer01->SaveDBItem(trans, ITEM_SAVE_RULE_REMOVE);
	pPlayer02->SaveDBItem(trans, ITEM_SAVE_RULE_REMOVE);
	pPlayer01->SaveDBItem(trans, ITEM_SAVE_RULE_ADD);
	pPlayer02->SaveDBItem(trans, ITEM_SAVE_RULE_ADD);

	pPlayer01->SaveDBInfo(trans);
	pPlayer02->SaveDBInfo(trans);

	pPlayer01->SaveDBAccount(trans);
	pPlayer02->SaveDBAccount(trans);

	pPlayer01->SaveDBPentagramInfo(trans);
	pPlayer02->SaveDBPentagramInfo(trans);

	MuDatabase.CommitTransaction(trans);
}

bool Player::TransactionSerialFind(Player* pPlayer, uint16 serial_server, uint32 serial, std::string const& transaction)
{
	Item * pItem = nullptr;
	int32 count = 0;

	use_inventory_loop(i)
	{
		pItem = pPlayer->GetInventory()->GetItem(i);

		if ( !pItem || !pItem->IsItem() )
		{
			continue;
		}

		if ( pItem->GetSerialServer() == serial_server && pItem->GetSerial() == serial )
		{
			++count;

			if ( count > 1 )
			{
				sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial [%u/%u] in inventory. Item: %s",
					transaction.c_str(), pPlayer->BuildLog().c_str(), serial_server, serial, pItem->BuildLog(i).c_str());
			}
		}
	}

	for ( uint8 i = 0; i < warehouse_size; ++i )
	{
		pItem = pPlayer->GetWarehouse()->GetItem(i);

		if ( !pItem || !pItem->IsItem() )
		{
			continue;
		}

		if ( pItem->GetSerialServer() == serial_server && pItem->GetSerial() == serial )
		{
			++count;

			if ( count > 1 )
			{
				sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial [%u/%u] in warehouse. Item: %s",
					transaction.c_str(), pPlayer->BuildLog().c_str(), serial_server, serial, pItem->BuildLog(i).c_str());
			}
		}
	}

	for ( uint8 i = 0; i < EVENT_INVENTORY_SIZE; ++i )
	{
		pItem = pPlayer->GetEventInventory()->GetItem(i);

		if ( !pItem || !pItem->IsItem() )
		{
			continue;
		}

		if ( pItem->GetSerialServer() == serial_server && pItem->GetSerial() == serial )
		{
			++count;

			if ( count > 1 )
			{
				sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial [%u/%u] in event inventory. Item: %s",
					transaction.c_str(), pPlayer->BuildLog().c_str(), serial_server, serial, pItem->BuildLog(i).c_str());
			}
		}
	}

	if ( Player::InterfaceCheck(pPlayer, InterfaceData::Trade, 1) )
	{
		trade_loop(i)
		{
			pItem = pPlayer->GetTradeInventory()->GetItem(i);

			if ( !pItem || !pItem->IsItem() )
			{
				continue;
			}

			if ( pItem->GetSerialServer() == serial_server && pItem->GetSerial() == serial )
			{
				++count;

				if ( count > 1 )
				{
					sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial [%u/%u] in trade. Item: %s",
						transaction.c_str(), pPlayer->BuildLog().c_str(), serial_server, serial, pItem->BuildLog(i).c_str());
				}
			}
		}
	}

	if ( pPlayer->GetInterfaceState()->IsCommonMix() || pPlayer->GetInterfaceState()->GetID() == InterfaceData::PetTrainer )
	{
		chaos_box_loop(i)
		{
			pItem = pPlayer->GetMixInventory()->GetItem(i);

			if ( !pItem || !pItem->IsItem() )
			{
				continue;
			}

			if ( pItem->GetSerialServer() == serial_server && pItem->GetSerial() == serial )
			{
				++count;

				if ( count > 1 )
				{
					sLog->outError(LOG_ITEMS, "[ %s SERIAL ] [%s] Founded same item serial [%u/%u] in chaos box. Item: %s",
						transaction.c_str(), pPlayer->BuildLog().c_str(), serial_server, serial, pItem->BuildLog(i).c_str());
				}
			}
		}
	}

	return count > 1;
}