MuunScript::MuunScript(Player* pPlayer)
{
	this->SetPlayer(pPlayer);
	this->Clear();
}

MuunScript::~MuunScript()
{
	
}

void MuunScript::Clear()
{
	for ( int32 i = 0; i < MUUN_INVENTORY_SIZE; ++i )
	{
		this->GetItem(i)->clear();
	}
}

void MuunScript::AssignItem(Item const& add_item, uint8 slot)
{
	this->GetItem(slot)->Set(add_item);
	this->GetItem(slot)->SetName(add_item.GetName());
	this->GetItem(slot)->SetOwner(this->GetPlayer() ? this->GetPlayer()->GetGUID(): 0);
	this->GetItem(slot)->SetOwnerSlot(slot);
	this->GetItem(slot)->SetState(ITEM_CHANGED);
	this->GetItem(slot)->SetBox(ITEM_BOX_MUUN);
	this->GetItem(slot)->Convert();
}

void MuunScript::SaveDB(SQLTransaction & trans, uint8 rules)
{
	for ( int32 i = 0; i < MUUN_INVENTORY_SIZE; ++i )
	{
		this->GetItem(i)->SaveDB(trans, rules);
	}
}

void MuunScript::LoadDBItem(Field* fields, uint32 owner, ItemBox box)
{
	uint32 slot = fields[ITEM_DB_DATA_SLOT].GetUInt32();
	this->GetItem(slot)->LoadDB(fields, owner, slot, box);
}
	
void MuunScript::DeleteItem(uint8 slot)
{
	this->GetItem(slot)->clear(false);
}
	
uint8 MuunScript::AddItem(Item const& item, uint8 slot)
{
	if( !MUUN_INVENTORY_RANGE(slot) )
	{
		return 0xFF;
	}

	if ( this->GetItem(slot)->IsItem() )
	{
		return 0xFF;
	}

	if ( !item.IsItem() )
	{
		return 0xFF;
	}

	this->AssignItem(item, slot);
	return slot;
}

uint8 MuunScript::AddItem(Item const& item)
{
	uint8 slot = this->GetEmptySlot();

	if ( slot != 0xFF )
	{
		this->AssignItem(item, slot);
	}

	return slot;
}

uint8 MuunScript::GetEmptySlot()
{
	for ( int32 i = MUUN_INVENTORY_WEAR_SIZE; i < MUUN_INVENTORY_SIZE; ++i )
	{
		if ( !this->GetItem(i)->IsItem() )
		{
			return i;
		}
	}

	return 0xFF;
}