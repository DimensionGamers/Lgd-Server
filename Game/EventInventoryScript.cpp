void EventInventoryScript::AssignItem(Item const& add_item, uint8 slot)
{
	StoreScript::AssignItem(add_item, slot);
	this->item[slot].SetOwner(this->GetPlayer()->GetGUID());
	this->item[slot].SetBox(ITEM_BOX_EVENT_INVENTORY);
}

bool EventInventoryScript::IsEmptySpace(uint16 item)
{
	item_template const* item_info = sItemMgr->GetItem(item);

	if ( !item_info )
		return false;

	return this->IsEmptySpace(item_info->GetX(), item_info->GetY());
}

bool EventInventoryScript::IsEmptySpace(uint8 W, uint8 H)
{
	for( int32 y = 0; y < 4; y++ )
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