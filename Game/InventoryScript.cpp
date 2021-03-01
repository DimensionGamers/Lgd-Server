void InventoryScript::Clear()
{
	this->SetExpanded(0);
	this->SetTemporary(false);
	StoreScript::Clear();
}

bool InventoryScript::CanUseExpandedSlot(uint8 slot) const
{
	if ( expanded_inventory_1_range(slot) && this->GetExpanded() < 1 )
	{
		return false;
	}
	
	if ( expanded_inventory_2_range(slot) && this->GetExpanded() < 2 )
	{
		return false;
	}

	if ( expanded_inventory_3_range(slot) && this->GetExpanded() < 3 )
	{
		return false;
	}

	if ( expanded_inventory_4_range(slot) && this->GetExpanded() < 4 )
	{
		return false;
	}

	return true;
}

uint8 InventoryScript::RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const
{
	int32 slot = ((y * 8) + x) + PLAYER_MAX_EQUIPMENT;

	if(INVENTORY_MAIN_RANGE(slot) && ((x+width) > 8 || (y+height) > 8))
	{
		return 0xFF;
	}

	if(INVENTORY_EXT1_RANGE(slot) && ((x+width) > 8 || (y+height) > 12))
	{
		return 0xFF;
	}

	if(INVENTORY_EXT2_RANGE(slot) && ((x+width) > 8 || (y+height) > 16))
	{
		return 0xFF;
	}

	if(INVENTORY_EXT3_RANGE(slot) && ((x+width) > 8 || (y+height) > 20))
	{
		return 0xFF;
	}

	if(INVENTORY_EXT4_RANGE(slot) && ((x+width) > 8 || (y+height) > 24))
	{
		return 0xFF;
	}

	if (personal_store_range(slot) && ((x + width) > 8 || (y + height) > 28))
	{
		return 0xFF;
	}

	for( int32 sy = 0; sy < height; sy++ )
	{
		for( int32 sx = 0; sx < width; sx++ )
		{
			if ( this->item_map[(((sy+y)*8)+(sx+x))] != 0 )
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

bool InventoryScript::IsEmptySpace(uint8 W, uint8 H)
{
	int32 MaxY = (this->GetInventoryMaxValue() - PLAYER_MAX_EQUIPMENT) / 8;

	for( int32 y = 0; y < MaxY; y++ )
	{
		for( int32 x = 0; x < 8; x++ )
		{
			if( this->item_map[((y*8)+x)] == 0 )
			{
				uint8 slot = this->RectCheck(x, y, W, H);

				if( slot != 0xFF )
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool InventoryScript::IsEmptySpace(uint16 item)
{
	item_template const* item_info = sItemMgr->GetItem(item);

	if ( !item_info )
		return false;

	return this->IsEmptySpace(item_info->GetX(), item_info->GetY());
}

void InventoryScript::ItemSet(uint8 slot, uint8 set)
{
	if( !main_inventory_range(slot) )
	{
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(this->GetItem(slot)->GetItem());

	if ( !item_info )
		return;

	int32 x = (slot - PLAYER_MAX_EQUIPMENT) % 8;
	int32 y = (slot - PLAYER_MAX_EQUIPMENT) / 8;

	if( INVENTORY_MAIN_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 8) )
	{
		return;
	}

	if( INVENTORY_EXT1_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 12) )
	{
		return;
	}

	if( INVENTORY_EXT2_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 16) )
	{
		return;
	}

	if( INVENTORY_EXT3_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 20) )
	{
		return;
	}

	if( INVENTORY_EXT4_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 24) )
	{
		return;
	}

	if (personal_store_range(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 28))
	{
		return;
	}

	for(int32 sy = 0; sy < item_info->GetY(); ++sy )
	{
		for(int32 sx = 0; sx < item_info->GetX(); ++sx )
		{
			this->item_map[(((sy + y) * 8) + (sx + x))] = set;
		}
	}
}

void InventoryScript::DeleteItem(uint8 slot)
{
	if ( INVENTORY_WEAR_RANGE(slot) )
	{
		if ( this->GetItem(slot)->GetSkill() )
		{
			bool delete_skill = true;

			wear_inventory_loop(i)
			{
				if ( slot == i )
					continue;

				if ( !this->GetItem(i)->IsItem() )
					continue;

				if ( this->GetItem(i)->GetSkill() )
				{
					if ( this->GetItem(slot)->GetSkillID() == this->GetItem(i)->GetSkillID() )
					{
						delete_skill = false;
					}
				}
			}

			if ( delete_skill )
			{
				this->GetPlayer()->MagicDelWeapon(this->GetItem(slot)->GetSkillID(), 0);
			}
		}

		if ( this->GetItem(slot)->IsBlessedArchangelWeapon() )
		{
			bool delete_skill = true;

			wear_inventory_loop(i)
			{
				if ( slot == i )
				{
					continue;
				}

				if ( !this->GetItem(i)->IsItem() )
				{
					continue;
				}

				if ( this->GetItem(slot)->IsBlessedArchangelWeapon() && this->GetItem(i)->IsBlessedArchangelWeapon() )
				{
					delete_skill = false;
				}
			}

			if ( delete_skill )
			{
				this->GetPlayer()->MagicDelWeapon(SKILL_ARCHANGELS_WILL, 0);
			}
		}
	}
	else
	{
		this->ItemSet(slot, 0);
	}

	this->GetItem(slot)->clear(false);
}

int32 InventoryScript::GetItemCount(uint16 item, uint8 level, bool weared, bool weared_only)
{
	item_template const* item_info = sItemMgr->GetItem(item);
	
	if ( !item_info )
	{
		return 0;
	}
	
	int32 count = 0;

	if ( weared )
	{
		wear_inventory_loop(i)
		{
			if ( !this->GetItem(i)->IsItem() )
			{
				continue;
			}

			if ( this->GetItem(i)->GetItem() == item && (this->GetItem(i)->GetLevel() == level || level == uint8(-1)) )
			{
				count++;
			}
		}

		if (this->GetItem(PENTAGRAM_SLOT)->IsItem())
		{
			if (this->GetItem(PENTAGRAM_SLOT)->GetItem() == item && (this->GetItem(PENTAGRAM_SLOT)->GetLevel() == level || level == uint8(-1)))
			{
				count++;
			}
		}

		if (this->GetItem(EARRING_01)->IsItem())
		{
			if (this->GetItem(EARRING_01)->GetItem() == item && (this->GetItem(EARRING_01)->GetLevel() == level || level == uint8(-1)))
			{
				count++;
			}
		}

		if (this->GetItem(EARRING_02)->IsItem())
		{
			if (this->GetItem(EARRING_02)->GetItem() == item && (this->GetItem(EARRING_02)->GetLevel() == level || level == uint8(-1)))
			{
				count++;
			}
		}
	}

	if ( weared_only )
		return count;

	use_inventory_loop(i)
	{
		if ( !this->CanUseExpandedSlot(i) )
		{
			continue;
		}

		Item const* pItem = this->GetItem(i);

		if ( !pItem )
		{
			continue;
		}

		if ( !pItem->IsItem() )
		{
			continue;
		}

		if ( pItem->GetItem() == item && (pItem->GetLevel() == level || level == uint8(-1)) )
		{
			if ( item_info->GetStackData()->GetMaxStack() > 0 )
			{
				count += pItem->GetDurability();
			}
			else
			{
				++count;
			}
		}
	}
	
	return count;
}

Item* InventoryScript::IsWearingRing(uint16 item, uint8 level)
{
	if ( this->GetItem(RING_01)->GetItem() == item && this->GetItem(RING_01)->GetLevel() == level && this->GetItem(RING_01)->IsValid() )
	{
		return this->GetItem(RING_01);
	}

	if ( this->GetItem(RING_02)->GetItem() == item && this->GetItem(RING_02)->GetLevel() == level && this->GetItem(RING_02)->IsValid() )
	{
		return this->GetItem(RING_02);
	}

	return nullptr;
}

bool InventoryScript::IsExpirableItem(uint8 slot, uint16 item, uint8 level) const
{
	Item const* pItem = this->GetItem(slot);

	if (!pItem)
	{
		return false;
	}

	if (!pItem->IsItem())
	{
		return false;
	}

	if (!pItem->GetExpireDate())
	{
		return false;
	}

	if (pItem->GetItem() == item && pItem->GetLevel() == level)
	{
		return true;
	}

	return false;
}

void InventoryScript::AssignItem(Item const& add_item, uint8 slot)
{
	StoreScript::AssignItem(add_item, slot);
	this->item[slot].SetOwner(this->GetPlayer()->GetGUID());
	this->item[slot].SetBox(ITEM_BOX_INVENTORY);
}

uint8 InventoryScript::AddItem(Item const& item, uint8 slot)
{
	if( !inventory_range(slot) )
	{
		return 0xFF;
	}

	if ( this->GetItem(slot)->IsItem() || !item.IsItem() )
	{
		return 0xFF;
	}

	if( !INVENTORY_WEAR_RANGE(slot) )
	{
		item_template const* item_info = sItemMgr->GetItem(item.GetItem());

		if( !item_info )
		{
			return 0xFF;
		}

		int32 x = (slot - PLAYER_MAX_EQUIPMENT)%8;
		int32 y = (slot - PLAYER_MAX_EQUIPMENT)/8;

		if( this->RectCheck(x, y, item_info->GetX(), item_info->GetY()) == 0xFF )
		{
			return 0xFF;
		}

		this->AssignItem(item, slot);
		this->ItemSet(slot, 1);
		return slot;
	}
	else
	{
		if ( !item.IsRequiredClass(this->GetPlayer()->GetClass(), this->GetPlayer()->GetChangeUP(0), this->GetPlayer()->GetChangeUP(1), this->GetPlayer()->GetChangeUP(2)) )
		{
			return 0xFF;
		}

		this->AssignItem(item, slot);

		if ( !this->GetPlayer()->IsFirstTime() )
		{
			if ( this->GetItem(slot)->GetSkill() )
			{
				int32 result = this->GetPlayer()->MagicAddWeapon(this->GetItem(slot)->GetSkillID(), 0);

				if ( result != -1 )
				{
					this->GetPlayer()->SendSkill(result, this->GetPlayer()->GetSkill(result)->GetSkill(), this->GetPlayer()->GetSkill(result)->GetLevel());
				}
			}

			if ( this->GetItem(slot)->IsBlessedArchangelWeapon() )
			{
				int32 result = this->GetPlayer()->MagicAddWeapon(SKILL_ARCHANGELS_WILL, 0);

				if ( result != -1 )
				{
					this->GetPlayer()->SendSkill(result, SKILL_ARCHANGELS_WILL, 0);
				}
			}
		}

		return slot;
	}
}

uint8 InventoryScript::AddItem(Item const& item)
{
	if ( !item.IsItem() )
	{
		return 0xFF;
	}

	item_template const* item_info = sItemMgr->GetItem(item.GetItem());

	if( !item_info )
	{
		return 0xFF;
	}

	int32 MaxY = (this->GetInventoryMaxValue() - PLAYER_MAX_EQUIPMENT) / 8;

	for( int32 y = 0; y < MaxY; y++ )
	{
		for( int32 x = 0; x < 8; x++ )
		{
			if( this->item_map[((y*8)+x)] == 0 )
			{
				uint8 slot = this->RectCheck(x, y, item_info->GetX(), item_info->GetY());

				if( slot != 0xFF )
				{
					this->AssignItem(item, slot);
					this->ItemSet(slot, 1);
					return slot;
				}
			}
		}
	}

	return 0xFF;
}

int32 InventoryScript::GetInventoryMaxValue() const
{
	int32 MaxValue = expanded_inventory_1_start;

	switch( this->GetExpanded() )
	{
	case 0:
		MaxValue = expanded_inventory_1_start;
		break;
	case 1:
		MaxValue = INVENTORY_EXT1_SIZE;
		break;
	case 2:
		MaxValue = INVENTORY_EXT2_SIZE;
		break;
	case 3:
		MaxValue = INVENTORY_EXT3_SIZE;
		break;
	case 4:
		MaxValue = INVENTORY_EXT4_SIZE;
		break;
	default:
		MaxValue = INVENTORY_EXT4_SIZE;
		break;
	}

	return MaxValue;
}

void InventoryScript::TemporaryStart()
{
	if ( this->IsTemporary() )
	{
		return;
	}

	this->StartTmp();
	this->SetTemporary(true);
}
	
void InventoryScript::TemporaryEnd()
{
	if ( !this->IsTemporary() )
	{
		return;
	}

	this->EndTmp();
	this->SetTemporary(false);
}

bool InventoryScript::TemporaryItemAdd(Item const& item)
{
	if ( !this->IsTemporary() )
	{
		return false;
	}

	if ( !item.IsItem() )
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(item.GetItem());

	if( !item_info )
	{
		return false;
	}

	int32 MaxY = (this->GetInventoryMaxValue() - PLAYER_MAX_EQUIPMENT) / 8;

	for( int32 y = 0; y < MaxY; y++ )
	{
		for( int32 x = 0; x < 8; x++ )
		{
			if( this->item_map[((y*8)+x)] == 0 )
			{
				uint8 slot = this->RectCheck(x, y, item_info->GetX(), item_info->GetY());

				if( slot != 0xFF )
				{
					this->ItemSet(slot, 1);
					return true;
				}
			}
		}
	}

	return false;
}