StoreScript::StoreScript(Player* pPlayer, int32 item_count, int32 map_count, ItemBox box)
{
	this->SetPlayer(pPlayer);
	this->SetItemCount(item_count);
	this->SetMapCount(map_count);
	this->SetBox(box);
	this->item = new Item[this->GetItemCount()];
	this->item_map = new uint8[this->GetMapCount()];
	this->item_map_tmp = new uint8[this->GetMapCount()];
	this->Clear();
}

StoreScript::~StoreScript()
{
	delete []item;
	delete []item_map;
	delete []item_map_tmp;
}

void StoreScript::Clear()
{
	for ( int32 i = 0; i < this->GetItemCount(); ++i )
	{
		this->GetItem(i)->clear();
	}

	for ( int32 i = 0; i < this->GetMapCount(); ++i )
	{
		this->item_map[i] = 0;
	}

	for ( int32 i = 0; i < this->GetMapCount(); ++i )
	{
		this->item_map_tmp[i] = 0;
	}
}

void StoreScript::ClearTmp()
{
	for ( int32 i = 0; i < this->GetMapCount(); ++i )
	{
		this->item_map_tmp[i] = 0;
	}
}

void StoreScript::StartTmp()
{
	for ( int32 i = 0; i < this->GetMapCount(); ++i )
	{
		this->item_map_tmp[i] = this->item_map[i];
	}
}

void StoreScript::EndTmp()
{
	for ( int32 i = 0; i < this->GetMapCount(); ++i )
	{
		this->item_map[i] = this->item_map_tmp[i];
	}
}

void StoreScript::Assign(StoreScript* store)
{
	for ( int32 i = 0; i < this->GetItemCount(); ++i )
	{
		this->item[i] = store->item[i];
	}

	for ( int32 i = 0; i < this->GetMapCount(); ++i )
	{
		this->item_map[i] = store->item_map[i];
	}
}

void StoreScript::AssignItem(Item const& add_item, uint8 slot)
{
	this->item[slot].Set(add_item);
	//this->item[slot] = add_item;
	this->item[slot].SetName(add_item.GetName());
	this->item[slot].SetOwner(this->GetPlayer() ? this->GetPlayer()->GetGUID(): 0);
	this->item[slot].SetOwnerSlot(slot);
	this->item[slot].SetState(ITEM_CHANGED);
	this->item[slot].SetBox(this->GetBox());
	this->item[slot].Convert();
}

void StoreScript::SaveDB(SQLTransaction & trans, uint8 rules)
{
	for ( int32 i = 0; i < this->GetItemCount(); ++i )
	{
		this->GetItem(i)->SaveDB(trans, rules);
	}
}

void StoreScript::LoadDBItem(Field* fields, uint32 owner, ItemBox box)
{
	uint32 slot = fields[ITEM_DB_DATA_SLOT].GetUInt32();
	this->item[slot].LoadDB(fields, owner, slot, box);

	if ( box == ITEM_BOX_INVENTORY && !INVENTORY_WEAR_RANGE(slot) )
	{
		this->ItemSet(slot, 1);
	}

	if (box == ITEM_BOX_EVENT_INVENTORY || box == ITEM_BOX_WAREHOUSE || box == ITEM_BOX_MIX_RECOVERY)
	{
		this->ItemSet(slot, 1);
	}
	
	if (box != ITEM_BOX_MUUN && box != ITEM_BOX_MIX_RECOVERY)
	{
		if (this->item[slot].IsExpired())
		{
			this->DeleteItem(slot);
		}
	}
}

uint8 StoreScript::RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const
{
	uint8 y_size = 4;

	if (this->GetBox() == ITEM_BOX_MIX_RECOVERY)
	{
		y_size = 15;
	}

	if ((x + width) > 8 || (y + height) > y_size)
	{
		return 0xFF;
	}

	for( int32 sy = 0; sy < height; sy++ )
	{
		for( int32 sx = 0; sx < width; sx++ )
		{
			if( this->item_map[(((sy+y)*8)+(sx+x))] != 0 )
			{
				return 0xFF;
			}
		}
	}

	return ((y * 8) + x);
}

void StoreScript::ItemSet(uint8 slot, uint8 set)
{
	if ( slot >= this->GetItemCount() )
	{
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(this->GetItem(slot)->GetItem());

	if ( !item_info )
	{
		return;
	}

	uint8 y_size = 4;
	uint8 iw = item_info->GetX();
	uint8 ih = item_info->GetY();

	if (this->GetBox() == ITEM_BOX_MIX_RECOVERY)
	{
		y_size = 15;
	}

	int32 x = slot % 8;
	int32 y = slot / 8;

	if ((x + iw) > 8 || (y + ih) > y_size)
	{
		return;
	}

	for (int32 sy = 0; sy < ih; sy++)
	{
		for (int32 sx = 0; sx < iw; sx++)
		{
			this->item_map[(((sy + y) * 8) + (sx + x))] = set;
		}
	}
}

void StoreScript::DeleteItem(uint8 slot)
{
	this->ItemSet(slot, 0);
	this->item[slot].clear(false);
}

Item* StoreScript::GetItem(uint8 slot)
{
	return &this->item[slot];
}
	
Item const* StoreScript::GetItem(uint8 slot) const
{
	return &this->item[slot];
}

uint8 StoreScript::AddItem(Item const& item, uint8 slot)
{
	if ( slot >= this->GetItemCount() )
	{
		return 0xFF;
	}

	if ( this->GetItem(slot)->IsItem() || !item.IsItem() )
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
	uint8 iw = item_info->GetX();
	uint8 ih = item_info->GetY();

	if (this->RectCheck(x, y, iw, ih) == 0xFF)
	{
		return 0xFF;
	}

	this->AssignItem(item, slot);
	this->ItemSet(slot, 1);
	return slot;
}

uint8 StoreScript::AddItem(Item const& item)
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

	uint8 iw = item_info->GetX();
	uint8 ih = item_info->GetY();

	uint8 y_size = 4;

	if (this->GetBox() == ITEM_BOX_MIX_RECOVERY)
	{
		y_size = 15;
	}

	for (int32 y = 0; y < y_size; y++)
	{
		for( int32 x = 0; x < 8; x++ )
		{
			if( this->item_map[((y*8)+x)] == 0 )
			{
				uint8 slot = this->RectCheck(x, y, iw, ih);

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

bool StoreScript::IsEmptySpace(uint8 iw, uint8 ih) const
{
	uint8 y_size = 4;

	if (this->GetBox() == ITEM_BOX_MIX_RECOVERY)
	{
		y_size = 15;
	}

	for (int32 y = 0; y < y_size; y++)
	{
		for (int32 x = 0; x < 8; x++)
		{
			if (this->item_map[((y * 8) + x)] == 0)
			{
				uint8 slot = this->RectCheck(x, y, iw, ih);

				if (slot != 0xFF)
				{
					return true;
				}
			}
		}
	}

	return false;
}