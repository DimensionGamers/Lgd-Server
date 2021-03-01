PersonalStoreScript::PersonalStoreScript(Player* pPlayer, ItemBox box, int32 size)
{
	this->SetPlayer(pPlayer);
	this->SetBox(box);
	this->CreateItem(size);
	this->SetViewStore(nullptr);
	this->SetViewSlot(-1);
	this->Init();
}

PersonalStoreScript::~PersonalStoreScript()
{
	this->EraseItem();
}

void PersonalStoreScript::Init()
{
	this->SetViewStore(nullptr);
	this->SetViewSlot(-1);

	for (int32 i = 0; i < this->GetItemSize(); ++i)
	{
		this->GetItem(i)->clear();
	}
}

void PersonalStoreScript::AssignItem(Item const& add_item, uint32 slot)
{
	//this->SetItem(slot, add_item);
	this->GetItem(slot)->Set(add_item);
	this->GetItem(slot)->SetName(add_item.GetName());
	this->GetItem(slot)->SetOwner(this->GetPlayer()->GetGUID());
	this->GetItem(slot)->SetOwnerSlot(slot);
	this->GetItem(slot)->SetState(ITEM_CHANGED);
	this->GetItem(slot)->SetBox(this->GetBox());
	this->GetItem(slot)->Convert();
}

void PersonalStoreScript::SaveDB(SQLTransaction & trans, uint8 rules)
{
	for (int32 i = 0; i < this->GetItemSize(); ++i)
	{
		this->GetItem(i)->SaveDB(trans, rules);
	}
}

void PersonalStoreScript::LoadDBItem(Field* fields, uint32 owner, ItemBox box)
{
	uint32 slot = fields[ITEM_DB_DATA_SLOT].GetUInt32();

	if (!this->IsSlotInRange(slot))
	{
		return;
	}

	this->GetItem(slot)->LoadDB(fields, owner, slot, box);
}

void PersonalStoreScript::DeleteItem(uint32 slot)
{
	this->GetItem(slot)->clear(false);
}

uint8 PersonalStoreScript::AddItem(Item const& item, uint8 slot)
{
	if (!this->IsSlotInRange(slot))
	{
		return -1;
	}

	if (this->GetItem(slot)->IsItem())
	{
		return -1;
	}

	if (!item.IsItem())
	{
		return -1;
	}

	this->AssignItem(item, slot);
	return slot;
}

bool PersonalStoreScript::IsSlotInRange(uint32 slot) const
{
	return slot >= 0 && slot < this->GetItemSize();
}

bool PersonalStoreScript::IsSellRange(uint32 slot) const
{
	return slot >= 0 && slot < PERSONAL_STORE_SELL_MAX;
}

bool PersonalStoreScript::IsBundle(uint32 slot)
{
	PersonalStoreItemMap map;
	this->GetItemList(slot, map);

	return map.size() > 1;
}

void PersonalStoreScript::GetItemList(uint32 slot, PersonalStoreItemMap & map)
{
	if (!this->IsSellRange(slot))
	{
		return;
	}

	auto pItem = this->GetItem(slot);

	if (!pItem || !pItem->IsItem())
	{
		return;
	}

	map[slot] = pItem;

	uint32 search_slot = PERSONAL_STORE_SELL_MAX + slot * PERSONAL_STORE_BUNDLE_MAX;
	
	for (uint32 i = search_slot; i < (search_slot + PERSONAL_STORE_BUNDLE_MAX); ++i)
	{
		pItem = this->GetItem(i);
		if (pItem && pItem->IsItem())
		{
			map[i] = pItem;
		}
	}
}

bool PersonalStoreScript::IsEmpty() const
{
	for (int32 i = 0; i < this->GetItemSize(); ++i)
	{
		auto pItem = this->GetItem(i);

		if (pItem && pItem->IsItem())
		{
			return false;
		}
	}

	return true;
}