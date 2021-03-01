GremoryCaseScript::GremoryCaseScript(Player* pPlayer, ItemBox box, int32 size)
{
	this->SetPlayer(pPlayer);
	this->SetBox(box);
	this->CreateItem(size);
	this->Clear();
}

GremoryCaseScript::~GremoryCaseScript()
{
	this->EraseItem();
}

void GremoryCaseScript::Clear()
{
	for ( int32 i = 0; i < this->GetItemSize(); ++i )
	{
		this->GetItem(i)->clear();
	}
}

void GremoryCaseScript::AssignItem(Item const& add_item, uint8 slot)
{
	this->GetItem(slot)->Set(add_item);
	this->GetItem(slot)->SetName(add_item.GetName());
	this->GetItem(slot)->SetOwner(this->GetPlayer() ? (this->GetBox() == ITEM_BOX_GREMORY_CASE_INV || this->GetBox() == ITEM_BOX_GREMORY_CASE_MOBILE || this->GetBox() == ITEM_BOX_GREMORY_CASE_PS ? this->GetPlayer()->GetGUID() : this->GetPlayer()->GetAccountData()->GetGUID()) : 0);
	this->GetItem(slot)->SetOwnerSlot(slot);
	this->GetItem(slot)->SetState(ITEM_CHANGED);
	this->GetItem(slot)->SetBox(this->GetBox());
	this->GetItem(slot)->Convert();
}

void GremoryCaseScript::SaveDB(SQLTransaction & trans, uint8 rules)
{
	for (int32 i = 0; i < this->GetItemSize(); ++i)
	{
		this->GetItem(i)->SaveDB(trans, rules);
	}
}

void GremoryCaseScript::LoadDBItem(Field* fields, uint32 owner, ItemBox box)
{
	uint32 slot = fields[ITEM_DB_DATA_SLOT].GetUInt32();

	if (!this->IsSlotInRange(slot))
	{
		return;
	}

	this->GetItem(slot)->LoadDB(fields, owner, slot, box);

	if ( this->GetItem(slot)->GetPersonalStorePrice() == GremoryCaseReward::NONE )
	{
		this->GetItem(slot)->SetPersonalStorePrice(GremoryCaseReward::EVENT);
	}
	
	if ( this->GetItem(slot)->IsExpired() )
	{
		this->DeleteItem(slot);
	}
}
	
void GremoryCaseScript::DeleteItem(uint8 slot)
{
	this->GetItem(slot)->clear(false);
}
	
uint8 GremoryCaseScript::AddItem(Item const& item, uint8 slot)
{
	if(!this->IsSlotInRange(slot))
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

uint8 GremoryCaseScript::AddItem(Item const& item)
{
	uint8 slot = this->GetEmptySlot();

	if ( slot != 0xFF )
	{
		this->AssignItem(item, slot);
	}

	return slot;
}

bool GremoryCaseScript::IsSlotInRange(uint8 slot) const
{
	return slot >= 0 && slot < this->GetItemSize();
}

uint8 GremoryCaseScript::GetEmptySlot() const
{
	for (int32 i = 0; i < this->GetItemSize(); ++i)
	{
		if ( !this->GetItem(i)->IsItem() )
		{
			return i;
		}
	}

	return -1;
}

uint32 GremoryCaseScript::GetTotalFreeSlots() const
{
	uint32 count = 0;

	for (int32 i = 0; i < this->GetItemSize(); ++i)
	{
		if (!this->GetItem(i)->IsItem())
		{
			++count;
		}
	}

	return count;
}