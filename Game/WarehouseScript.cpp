void WarehouseScript::LoadDBData(PreparedQueryResult result)
{
	StoreScript::Clear();
	this->SetZen(0);
	this->SetPassword(0);

	if ( result )
	{
		Field* fields = result->Fetch();

		this->SetZen(fields[0].GetUInt32());
		this->SetPassword(fields[1].GetUInt16());

		if ( this->GetPassword() )
		{
			this->SetLocked(true);
		}
	}
}
	
void WarehouseScript::LoadDBItemList(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			if ( warehouse_range(fields[ITEM_DB_DATA_SLOT].GetUInt32()) && sItemMgr->IsItem(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) )
			{
				this->LoadDBItem(fields, this->GetPlayer()->GetAccountData()->GetGUID(), ITEM_BOX_WAREHOUSE);
			}
			else
			{
				sLog->outError(LOG_PLAYER, "%s Wrong Warehouse Item [%u / %u]", this->GetPlayer()->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
			}
		}
		while(result->NextRow());
	}
}

void WarehouseScript::Open()
{
	EXPANDED_WAREHOUSE_STATUS pMsg1(this->GetExpanded());
	this->GetPlayer()->sendPacket(MAKE_PCT(pMsg1));

	TALK_TO_NPC_RESULT pMsg2(0x02);
	this->GetPlayer()->sendPacket(MAKE_PCT(pMsg2));

	uint8 buffer[8192];
	SHOP_ITEM_LIST_HEAD * head = (SHOP_ITEM_LIST_HEAD*)buffer;
	SHOP_ITEM_LIST_BODY * body = (SHOP_ITEM_LIST_BODY*)&buffer[sizeof(SHOP_ITEM_LIST_HEAD)];
	head->count = 0;
	head->type = 0;

	for ( uint8 i = 0; i < warehouse_size; ++i )
	{
		if ( !this->GetItem(i)->IsItem() )
			continue;

		body[head->count].slot = i;
		this->GetItem(i)->ConvertToBuffer(body[head->count].item_info);
		head->count++;
	}

	head->h.set(0x31, sizeof(SHOP_ITEM_LIST_HEAD) + (sizeof(SHOP_ITEM_LIST_BODY) * head->count));
	
	this->GetPlayer()->sendPacket(buffer, head->h.get_size());

	this->GetPlayer()->WarehouseMoneyInOutResult(1, this->GetZen(), this->GetPlayer()->MoneyGet());

	if ( !this->GetPassword() )
	{
		this->GetPlayer()->WarehouseStateSend(0x00);
	}
	else if ( !this->IsLocked() )
	{
		this->GetPlayer()->WarehouseStateSend(0x0C);
	}
	else
	{
		this->GetPlayer()->WarehouseStateSend(0x01);
	}
}

void WarehouseScript::SaveDBData(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(ACCOUNT_WAREHOUSE_DATA_REPLACE);
	stmt->setUInt32(0, this->GetPlayer()->GetAccountData()->GetGUID());
	stmt->setUInt32(1, this->GetZen());
	stmt->setUInt16(2, this->GetPassword());
	trans->Append(stmt);
}

void WarehouseScript::Clear()
{
	StoreScript::Clear();
	this->SetZen(0);
	this->SetPassword(0);
	this->SetExpanded(0);
	this->SetExpandedTime(0);
	this->SetLocked(false);
}

uint8 WarehouseScript::RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const
{
	int32 slot = ((y * 8) + x);

	if( WAREHOUSE_MAIN_RANGE(slot) && ((x+width) > 8 || (y+height) > 15))
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(slot) && ((x+width) > 8 || (y+height) > 30))
	{
		return 0xFF;
	}

	for( int32 sy = 0; sy < height; sy++)
	{
		for( int32 sx = 0; sx < width; sx++ )
		{
			if( this->item_map[(((sy+y)*8)+(sx+x))] != 0 )
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

void WarehouseScript::ItemSet(uint8 slot, uint8 set)
{
	if( !warehouse_range(slot) )
	{
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(this->GetItem(slot)->GetItem());

	if( !item_info )
	{
		return;
	}

	int32 x = slot % 8;
	int32 y = slot / 8;

	if( WAREHOUSE_MAIN_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 15) )
	{
		return;
	}

	if( WAREHOUSE_EXT1_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 30) )
	{
		return;
	}

	for( int32 sy = 0; sy < item_info->GetY(); sy++ )
	{
		for( int32 sx = 0; sx < item_info->GetX(); sx++ )
		{
			this->item_map[(((sy+y)*8)+(sx+x))] = set;
		}
	}
}

void WarehouseScript::DeleteItem(uint8 slot)
{
	this->ItemSet(slot, 0);
	this->item[slot].clear(false);
}

bool WarehouseScript::MoneyReachMaximum(uint32 ammount) const
{
	return (this->GetZen() + ammount) > sGameServer->GetWarehouseMaxMoney();
}

void WarehouseScript::MoneyAdd(uint32 ammount)
{
	if ( this->MoneyReachMaximum(ammount) )
	{
		this->SetZen(sGameServer->GetWarehouseMaxMoney());
	}
	else
	{
		this->SetZen(this->GetZen() + ammount);
	}
}

void WarehouseScript::MoneyReduce(uint32 ammount)
{
	if ( this->GetZen() < ammount )
	{
		this->SetZen(0);
	}
	else
	{
		this->SetZen(this->GetZen() - ammount);
	}
}

bool WarehouseScript::MoneyHave(uint32 ammount)
{
	return this->GetZen() >= ammount;
}

void Player::WarehouseClose()
{
	if ( !this->IsPlaying() )
		return;

	if ( this->GetInterfaceState()->GetID() != InterfaceData::Warehouse )
		return;

	this->WarehouseCloseResult();

	Player::TransactionSerialCheck(this, "WAREHOUSE");
	
	this->GetInterfaceState()->Reset();
}

void Player::WarehouseCloseResult()
{
	WAREHOUSE_CLOSE_RESULT pMsg;
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::WarehousePasswordRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	WAREHOUSE_PASSWORD * lpMsg = (WAREHOUSE_PASSWORD*)Packet;

	STRING_SAFE_COPY(secure_code, MAX_SECURE_CODE_LENGTH + 1, lpMsg->secure_code, MAX_SECURE_CODE_LENGTH);

	switch ( lpMsg->type )
	{
	case 0: // Warehouse Open
		{
			if ( this->GetWarehouse()->IsLocked() )
			{
				if ( this->GetWarehouse()->GetPassword() == lpMsg->password )
				{
					this->GetWarehouse()->SetLocked(false);
					this->WarehouseStateSend(0x0C);
				}
				else
				{
					this->WarehouseStateSend(0x0A);
				}
			}
		} break;

	case 1: // Password Set
		{
			if ( !this->GetWarehouse()->IsLocked() )
			{
				if ( !this->SecureCodeCheck(secure_code) )
				{
					this->WarehouseStateSend(0x0D);
					return;
				}

				this->GetWarehouse()->SetPassword(lpMsg->password);
				this->GetWarehouse()->SetLocked(false);
				this->WarehouseStateSend(0x0C);
			}
			else
			{
				this->WarehouseStateSend(0x0B);
			}
		} break;

	case 2: // Password Delete
		{
			if ( !this->SecureCodeCheck(secure_code) )
			{
				this->WarehouseStateSend(0x0D);
				return;
			}

			this->GetWarehouse()->SetPassword(0);
			this->GetWarehouse()->SetLocked(false);
			this->WarehouseStateSend(0x00);
		} break;
	}
}

void Player::WarehouseStateSend(uint8 state)
{
	WAREHOUSE_STATE pMsg(state);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::WarehouseMoneyInOut(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		this->WarehouseMoneyInOutResult(0,0,0);
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::Warehouse )
	{
		this->WarehouseMoneyInOutResult(0, 0, 0);
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->WarehouseMoneyInOutResult(0, 0, 0);
		return;
	}

	POINTER_PCT(WAREHOUSE_MONEY_IN_OUT const, lpMsg, Packet, 0);

	switch ( lpMsg->type )
	{
	case 0x00:
		{
			if ( lpMsg->money == 0 || lpMsg->money > sGameServer->GetWarehouseMaxMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( !sGameServer->IsWarehouseSaveMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( this->GetWarehouse()->MoneyReachMaximum(lpMsg->money) )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}


			if ( !this->MoneyHave(lpMsg->money) )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			this->MoneyReduce(lpMsg->money, false);
			this->GetWarehouse()->MoneyAdd(lpMsg->money);
		} break;

	case 0x01:
		{
			if ( sGameServer->IsWarehouseLock() && this->GetWarehouse()->IsLocked() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				this->SendNotice(NOTICE_NORMAL_BLUE,"The vault is locked");
				return;
			}

			if ( lpMsg->money == 0 || lpMsg->money > sGameServer->GetWarehouseMaxMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( !sGameServer->IsWarehouseGetMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( !this->GetWarehouse()->MoneyHave(lpMsg->money) )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			this->GetWarehouse()->MoneyReduce(lpMsg->money);
			this->MoneyAdd(lpMsg->money, false);

			uint32 used_money = this->WarehouseGetUsedHowMuch();

			if ( this->MoneyHave(used_money) )
			{
				this->MoneyReduce(used_money, false);
			}
			else if ( this->GetWarehouse()->MoneyHave(used_money) )
			{
				this->GetWarehouse()->MoneyReduce(used_money);
			}
			else
			{
				this->GetWarehouse()->MoneyAdd(lpMsg->money);
				this->MoneyReduce(lpMsg->money, false);
				this->WarehouseMoneyInOutResult(0, 0, 0);
				this->SendNotice(NOTICE_NORMAL_BLUE, "You need %u Zen to use your vault", used_money);
				return;
			}
		} break;

	default:
		{
			this->WarehouseMoneyInOutResult(0, 0, 0);
			return;
		} break;
	}

	this->WarehouseMoneyInOutResult(1, this->GetWarehouse()->GetZen(), this->MoneyGet());
}

void Player::WarehouseMoneyInOutResult(uint8 result, uint32 w_money, uint32 i_money)
{
	WAREHOUSE_MONEY_IN_OUT_RESULT pMsg(result, w_money, i_money);
	this->sendPacket(MAKE_PCT(pMsg));
}

uint32 Player::WarehouseGetUsedHowMuch()
{
	uint32 money = floor(double((this->GetTotalLevel() * this->GetTotalLevel()) * 0.1f * 0.4f));

	if ( sGameServer->IsWarehouseLock() && this->GetWarehouse()->IsLocked() )
	{
		money += this->GetLevel() * 2;
	}

	if ( money >= 1000 )
	{
		money = (money / 100) * 100;
	}
	else if ( money >= 100 )
	{
		money = (money / 10) * 10;
	}

	return money;
}

void WarehouseScript::AssignItem(Item const& add_item, uint8 slot)
{
	StoreScript::AssignItem(add_item, slot);
	this->item[slot].SetOwner(this->GetPlayer()->GetAccountData()->GetGUID());
	this->item[slot].SetBox(ITEM_BOX_WAREHOUSE);
}

uint8 WarehouseScript::AddItem(Item const& item, uint8 slot)
{
	if( !warehouse_range(slot) )
	{
		return 0xFF;
	}

	if( this->GetItem(slot)->IsItem() || !item.IsItem() )
	{
		return 0xFF;
	}

	item_template const* item_info = sItemMgr->GetItem(item.GetItem());

	if( !item_info )
	{
		return 0xFF;
	}

	int32 x = slot % 8;
	int32 y = slot / 8;

	if( this->RectCheck(x, y, item_info->GetX(), item_info->GetY()) == 0xFF)
	{
		return 0xFF;
	}
	
	this->AssignItem(item, slot);
	this->ItemSet(slot, 1);
	return slot;
}