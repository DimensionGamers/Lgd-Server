/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Item.cpp"
*
*/

Item::Item()
{
	this->clear();
}

Item::~Item()
{
	this->clear();
}

Item::Item(uint16 item, uint8 level, float durability, uint8 skill, uint8 luck, uint8 option, uint8 exe, uint8 ancient, SocketDataType const* socket, uint8 socket_bonus, uint32 expire_time)
{
	this->clear();
	this->SetItem(item);
	this->SetLevel(level);
	this->SetDurability(durability);
	this->SetSkill(skill);
	this->SetLuck(luck);
	this->SetOption(option);
	this->SetExe(exe);
	this->SetAncient(ancient);

	if ( socket )
	{
		for ( int32 i = 0; i < MAX_SOCKET_SLOT; ++i )
		{
			this->SetSocket(i, socket[i]);
		}
	}

	this->SetSocketBonus(socket_bonus);

	if ( expire_time > 0 )
	{
		this->SetExpireDate(time(nullptr) + expire_time);
	}
}

void Item::clear(bool all)
{
	this->SetItem(-1);
	this->SetLevel(0);
	this->SetDurability(0.0f);
	this->SetDurabilityState(0.0f);
	this->SetDurabilityBase(0.0f);
	this->SetDurabilityCurrentStatus(0);
	this->ResetDurabilityStatus(0.0f);
	this->GetDurabilityTime()->Reset();
	this->SetDurabilityTimeRequired(0);
	this->SetDurabilityTimeRequiredSafe(0);
	this->SetDurabilityReduction(0);
	this->SetDurabilityResistance(0.0f);
	this->SetDurabilityOriginal(0.0f);

	this->SetSkill(0);
	this->SetSkillID(0);
	this->SetLuck(0);
	this->SetOption(0);
	this->SetExe(0);
	this->SetAncient(0);

	this->SetHarmony(HARMONY_EFFECT_NONE);
	this->Set380(0);

	this->ResetSocket(SOCKET_SLOT_NONE);
	this->SetSocketBonus(uint8(-1));

	for ( uint8 i = 0; i < 3; ++i )
		this->raw.data_extra[i] = 0;

	this->SetDBFlag(0);

	this->SetQuestItem(0);

	this->SetBuyPrice(0);
	this->SetSellPrice(0);
	this->SetOldPrice(0);
	this->SetValue(0);

	this->SetRequiredLevel(0);
	this->ResetStatRequired(0);
	this->ResetStatDecrease(0);
	this->ResetRequiredClass(0);
	this->ResetResistance(0);

	this->SetDefense(0);
	this->SetDefenseMagic(0);
	this->SetDefenseSuccessRate(0);
	this->SetDefenseOriginal(0);

	this->SetMinDamage(0);
	this->SetMaxDamage(0);
	
	this->SetExcellent(false);
	this->SetExcellentType(0);
	this->SetAncientAttribute(-1);
	this->SetAncientAttributeValue(0);

	this->SetValid(false);

	this->SetMagicPower(0);
	this->SetCursePower(0);
	this->SetPetPower(0);

	this->SetTwoHand(false);
	this->SetKind1(0);
	this->SetKind2(0);
	this->SetKind3(0);

	this->SetSlot(-1);
	this->ResetName();

	this->SetPersonalStorePrice(0);
	this->SetPersonalStorePriceJoB(0);
	this->SetPersonalStorePriceJoS(0);

	this->SetAttackSpeed(0);
	this->SetFlag(0);
	this->SetUseDelay(0);
	this->SetDurationTime(0);
	this->SetLootingTime(0);
	this->SetForceUpgrade(false);

	if ( all )
	{
		this->SetSerialServer(0);
		this->SetSerial(0);
		this->SetSerialShop(0);
		this->SetOwner(0);
		this->SetOwnerSlot(-1);
		this->SetBox(ITEM_BOX_NONE);
		this->SetState(ITEM_UNCHANGED);
	}
	else
	{
		this->SetState(ITEM_REMOVED);
	}

	this->SetExpireDate(0);
	this->SetExpireNotify(-1);
	this->SetSocketDurabilityReduction(0);
	this->SetNormalLevel(true);
	this->SetMaxSocket(0);

	this->SetInventoryActive(false);

	this->SetExpiredStatus(false);

	this->m_OptionData.clear();
}

void Item::Set(Item const& item)
{
	this->SetItem(item.GetItem());
	this->SetSerialServer(item.GetSerialServer());
	this->SetSerial(item.GetSerial());
	this->SetSerialShop(item.GetSerialShop());
	this->SetLevel(item.GetLevel());
	this->SetDurability(item.GetDurability());
	this->SetDurabilityState(item.GetDurabilityState());
	this->SetSkill(item.GetSkill());
	this->SetLuck(item.GetLuck());
	this->SetOption(item.GetOption());
	this->SetExe(item.GetExe());
	this->SetAncient(item.GetAncient());
	this->Set380(item.Get380());
	this->SetHarmony(item.GetHarmony());

	for (uint32 i = 0; i < MAX_SOCKET_SLOT; ++i)
	{
		this->SetSocket(i, item.GetSocket(i));
	}

	this->SetSocketBonus(item.GetSocketBonus());
	this->SetInventoryActive(item.IsInventoryActive());
	this->raw.data_extra[0] = item.raw.data_extra[0];
	this->raw.data_extra[1] = item.raw.data_extra[1];
	this->raw.data_extra[2] = item.raw.data_extra[2];
	this->SetPersonalStorePrice(item.GetPersonalStorePrice());
	this->SetPersonalStorePriceJoB(item.GetPersonalStorePriceJoB());
	this->SetPersonalStorePriceJoS(item.GetPersonalStorePriceJoS());
	this->SetExpireDate(item.GetExpireDate());
	this->SetDBFlag(item.GetDBFlag());
}

void Item::Set(Item const* pItem)
{
	this->Set(*pItem);
}

bool Item::IsSameItem(Item const& item, bool price_check) const
{
	if (this->GetItem() != item.GetItem() ||
		this->GetLevel() != item.GetLevel() ||
		this->GetSkill() != item.GetSkill() ||
		this->GetLuck() != item.GetLuck() ||
		this->GetOption() != item.GetOption() ||
		this->GetExe() != item.GetExe() ||
		this->GetAncient() != item.GetAncient() ||
		this->Get380() != item.Get380() ||
		this->GetHarmony() != item.GetHarmony())
	{
		return false;
	}

	item_template const* pItemInfo = sItemMgr->GetItem(this->GetItem());

	if (pItemInfo && pItemInfo->GetStackData()->GetMaxStack() > 0)
	{
		if (this->GetDurability() != item.GetDurability())
		{
			return false;
		}
	}

	if (price_check)
	{
		if (this->GetPersonalStorePrice() == item.GetPersonalStorePrice() &&
			this->GetPersonalStorePriceJoB() == item.GetPersonalStorePriceJoB() &&
			this->GetPersonalStorePriceJoS() == item.GetPersonalStorePriceJoS())
		{
			return false;
		}
	}

	return true;
}

bool Item::IsSameItem(Item const* pItem, bool price_check) const
{
	return this->IsSameItem(*pItem, price_check);
}

uint8 Item::GetItemType() const
{
	return this->GetItem() / MAX_SUBTYPE_ITEMS;
}
	
uint16 Item::GetItemIndex() const
{
	return this->GetItem() % MAX_SUBTYPE_ITEMS;
}

bool Item::IsAncient() const
{
	return IS_ANCIENT(this->GetAncient()) != 0;
}

bool Item::IsSocket() const
{
	SOCKET_SLOT_LOOP(i)
	{
		if ( this->GetSocket(i) == SOCKET_SLOT_NONE )
			continue;

		return true;
	}

	return false;
}

bool Item::IsHarmonyUpgrade() const
{
	return ((this->GetHarmony() & 0xF0) >> 4) > 0;
}

uint8 Item::GetHarmonyOption() const
{
	return (this->GetHarmony() & 0xF0) >> 4;
}

uint8 Item::GetHarmonyLevel() const
{
	return (this->GetHarmony() & 0x0F);
}

bool Item::Is380Option() const
{
	return ((this->Get380() & 0x80) >> 7) == 1;
}

bool Item::IsJewel() const
{
	return this->GetItem() == JEWEL::CHAOS ||
		   this->GetItem() == JEWEL::BLESS ||
		   this->GetItem() == JEWEL::SOUL ||
		   this->GetItem() == JEWEL::LIFE ||
		   this->GetItem() == JEWEL::CREATION ||
		   this->GetItem() == JEWEL::GUARDIAN ||
		   this->GetItem() == JEWEL::GEMSTONE ||
		   this->GetItem() == JEWEL::HARMONY ||
		   this->GetItem() == JEWEL::LOW_REFINE_STONE ||
		   this->GetItem() == JEWEL::HIGH_REFINE_STONE ||
		   this->GetItem() == JEWEL::EXTENSION ||
		   this->GetItem() == JEWEL::ELEVATION;
}

uint8 Item::GetSocketCount() const
{
	uint8 count = 0;

	SOCKET_SLOT_LOOP(i)
	{
		if ( this->GetSocket(i) == SOCKET_SLOT_NONE )
		{
			continue;
		}

		++count;
	}

	return count;
}

uint8 Item::GetActiveSocketCount() const
{
	uint8 count = 0;

	SOCKET_SLOT_LOOP(i)
	{
		if ( this->GetSocket(i) == SOCKET_SLOT_NONE ||
			 this->GetSocket(i) == SOCKET_SLOT_EMPTY )
		{
			continue;
		}

		++count;
	}

	return count;
}

bool Item::IsPentagramItem() const
{
	return sPentagramSystem->IsPentagramItem(this);
}

bool Item::IsPentagramMithril() const
{
	return sPentagramSystem->IsPentagramMithril(this);
}

bool Item::IsMuunItem() const
{
	return sMuunSystem->IsMuunItem(this);
}

bool Item::IsMuunUtil() const
{
	return sMuunSystem->IsMuunUtil(this);
}

bool Item::IsBlessedArchangelWeapon() const
{
	return this->GetKind3() == ItemKind3::BLESSED_ARCHANGEL_WEAPON;
}

bool Item::IsArchangelWeapon() const
{
	return this->GetKind3() == ItemKind3::ARCHANGEL_WEAPON;
}

bool Item::IsMysteriousStoneItem() const
{
	return this->IsFlag(ITEM_FLAG_ALLOW_MYSTERIOUS_STONE);
}

bool Item::IsEarring() const
{
	return sItemMgr->GetEarringData(this->GetItem()) != nullptr;
}

bool Item::IsMount() const
{
	return this->GetItem() == PET_UNIRIA || 
		this->GetItem() == PET_DINORANT || 
		this->GetItem() == PET_DARK_HORSE ||
		this->GetItem() == PET_FENRIR ||
		this->GetKind2() == ItemKind::GUARDIAN_MOUNT;
}

bool Item::IsValidElementalAttribute() const
{
	uint8 attribute = this->GetSocketBonus() % 16;

	return (attribute > ELEMENTAL_ATTRIBUTE_NONE) && (attribute < ELEMENTAL_ATTRIBUTE_MAX);
}

bool Item::SocketAllowed(uint8 option) const
{
	SOCKET_SLOT_LOOP(i)
	{
		if ( this->GetSocket(i) == SOCKET_SLOT_NONE || this->GetSocket(i) == SOCKET_SLOT_EMPTY )
		{
			continue;
		}

		if ( (this->GetSocket(i) % MAX_SOCKET_OPTION) == option )
		{
			return false;
		}
	}

	return true;
}

uint8 Item::GetSocketType(uint8 option)
{
	uint8 seed_index = option % MAX_SOCKET_OPTION;
	item_socket_seed const* pSeed = sItemMgr->GetSocketSeed(seed_index);

	if ( pSeed )
	{
		return pSeed->GetType();
	}
	else
	{
		return 0;
	}
}

HarmonyItemType Item::GetHarmonyItem() const
{
	if (this->GetKind2() == ItemKind::RUNIC_MACE || this->GetKind2() == ItemKind::MAGIC_GUN)
	{
		return HARMONY_ITEM_STAFF;
	}
	else if ( this->GetItem() >= ITEMGET(0,0) && this->GetItem() < ITEMGET(5,0) )
		return HARMONY_ITEM_WEAPON;
	else if ( this->GetItem() >= ITEMGET(5,0) && this->GetItem() < ITEMGET(6,0) )
		return HARMONY_ITEM_STAFF;
	else if ( this->GetItem() >= ITEMGET(6,0) && this->GetItem() < ITEMGET(12,0) )
		return HARMONY_ITEM_DEFENSE;

	return HARMONY_ITEM_NONE;
}

bool Item::IsRequiredClass(uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const
{
	if ( !this->GetRequiredClass(race) )
		return false;

	if ( this->GetRequiredClass(race) == 2 && change_up_1 == 0 )
		return false;

	if ( this->GetRequiredClass(race) == 3 && change_up_2 == 0 )
		return false;

	if ( this->GetRequiredClass(race) == 4 && change_up_3 == 0 )
		return false;

	return true;
}

int32 Item::GetRequiredStat(uint8 stat) const
{
	return this->GetStatRequired(stat) - this->GetStatDecrease(stat);
}

void Item::AlterDurability(uint8 type, float count)
{
	switch ( type )
	{
	case 0:
		{
			this->SetDurability(this->GetDurability() - count);
		} break;

	case 1:
		{
			this->SetDurability(this->GetDurability() + count);
		} break;

	case 2:
		{
			this->SetDurability(count);
		} break;

	case 3:
		{
			float durability = sItemMgr->CalculateDurability(this->GetItem(), this->GetLevel(), this->IsExcellent(), this->IsAncient());
			this->SetDurability(durability * this->GetDurability() / this->GetDurabilityBase());
		} break;
	}

	if ( this->GetDurability() < 1.0f )
		this->SetDurability(0.0f);

	if ( this->GetDurability() > 255.0f )
		this->SetDurability(255.0f);

	this->SetState(ITEM_CHANGED);
}

void Item::ConvertToBuffer(uint8 * buffer) const
{
	if (!this->IsItem())
	{
		memset(buffer, 0xFF, MAX_ITEM_INFO);
		return;
	}

	memset(buffer, 0, MAX_ITEM_INFO);

	if (this->GetItem() == ITEMGET(14, 15) || this->GetItem() == RUUD)
	{
		uint16 zen_h = SET_NUMBERHW(this->zen.ammount);
		uint16 zen_l = SET_NUMBERLW(this->zen.ammount);

		buffer[0] = uint8(this->GetItem()) % 255;
		buffer[1] = SET_NUMBERL(zen_h);
		buffer[2] = SET_NUMBERH(zen_l);
		buffer[3] = 0;
		buffer[4] = SET_NUMBERL(zen_l);
		buffer[5] = (this->GetItem() & 0x1E00) >> 5;
		buffer[6] = 0;
		SOCKET_SLOT_LOOP(i)
		{
			buffer[7 + i] = SOCKET_SLOT_NONE & 0xFF;
		}
		return;
	}

	/*
	C1 10 39 05 
	69 6C 93 40 00 A0 00 08 FF FF FF FF // Damage Decrease

	C1 10 39 02 
	69 4C 81 00 00 80 00 08 06 FF FF FF

	C1 10 39 00 
	69 4C 81 00 00 70 00 08 06 FF FF FF

	C1 10 39 16 
	69 4C 81 00 00 B0 00 06 08 FF FF FF
	*/

	buffer[0] = (uint8)this->GetItem();

	buffer[1] = 0;
	buffer[1] |= this->GetLevel() * 8;
	buffer[1] |= this->GetSkill() * 128;
	buffer[1] |= this->GetLuck() * 4;
	buffer[1] |= this->GetOption() & 3;
	buffer[2] = (uint8)this->GetDurability();
	buffer[3] = 0;
	buffer[3] |= (this->GetItem() & 0x100) >> 1;

	if ( this->GetOption() > 3 )
	{
		buffer[3] |= 0x40;
	}

	if (this->GetKind1() == ItemKind1::MASTERY_ARMOR_1 || this->GetKind1() == ItemKind1::MASTERY_ARMOR_2)
	{
		if (this->GetExe() & ExeOption::BDA_INCREASE_DEFENSE_SUCCESS_RATE)
		{
			buffer[3] |= ExeOption::BDA_INCREASE_DEFENSE_SUCCESS_RATE;
		}

		if (this->GetExe() & ExeOption::BDA_DAMAGE_REFLECT)
		{
			buffer[3] |= ExeOption::BDA_DAMAGE_REFLECT;
		}
	}
	else if (this->GetKind1() == ItemKind1::MASTERY_WEAPON || this->GetKind3() == ItemKind3::BLESSED_ARCHANGEL_WEAPON)
	{
		if (this->GetExe() & ExeOption::BA_INCREASE_MANA_RECOVERY_AFTER_KILL_MONSTER)
		{
			buffer[3] |= ExeOption::BA_INCREASE_MANA_RECOVERY_AFTER_KILL_MONSTER;
		}

		if (this->GetExe() & ExeOption::BA_INCREASE_HP_RECOVERY_AFTER_KILL_MONSTER)
		{
			buffer[3] |= ExeOption::BA_INCREASE_HP_RECOVERY_AFTER_KILL_MONSTER;
		}

		if (this->GetExe() & ExeOption::BA_INCREASE_ATTACK_SPEED)
		{
			buffer[3] |= ExeOption::BA_INCREASE_ATTACK_SPEED;
		}

		if (this->GetExe() & ExeOption::BA_INCREASE_EXE_DAMAGE_RATE)
		{
			buffer[3] |= ExeOption::BA_INCREASE_EXE_DAMAGE_RATE;
		}
	}
	else 
	{
		buffer[3] |= this->GetExe();
	}

	buffer[4] = this->GetAncient();

	buffer[5] |= (this->GetItem() & 0x1E00) >> 5;
	buffer[5] |= (this->Get380() & 0x80) >> 4;
	buffer[5] |= (this->GetItem() & 0x2000) >> 13;

	if (this->GetBox() != ITEM_BOX_MIX_RECOVERY && 
		this->GetBox() != ITEM_BOX_GREMORY_CASE_INV &&
		this->GetBox() != ITEM_BOX_GREMORY_CASE_ACC &&
		this->GetBox() != ITEM_BOX_GREMORY_CASE_MOBILE &&
		this->GetBox() != ITEM_BOX_GREMORY_CASE_PS)
	{
		if (this->GetExpireDate() && !this->IsMuunItem() && !sMonsterSoul->IsSoulConverter(this->GetItem()))
		{
			buffer[5] |= (1 << 1);

			if (this->IsExpired())
			{
				buffer[5] |= (1 << 2);
			}
		}
	}
	
	if (this->IsSocket() || this->IsPentagramItem() || this->GetKind2() == ItemKind::ERRTEL || this->IsPentagramMithril() || this->IsMuunItem() || this->IsWingLvl4() || this->GetKind2() == ItemKind::GUARDIAN_MOUNT)
	{
		buffer[6] = this->GetSocketBonus();

		if ( this->IsMuunItem() && (this->IsExpired() || !this->GetExpireDate()) )
		{
			buffer[6] |= 0x10;
		}
	}
	else
	{
		buffer[6] = this->GetHarmony();
	}

	if ( this->GetMaxSocket() > 0 )
	{
		buffer[4] = 0;

		SOCKET_SLOT_LOOP(i)
		{
			if ( this->GetSocket(i) == SOCKET_SLOT_EMPTY || this->GetSocket(i) == SOCKET_SLOT_NONE )
			{
				buffer[7 + i] = this->GetSocket(i) & 0xFF;
			}
			else
			{
				/// - Establezco la opción sin nivel
				uint8 socket_option = this->GetSocket(i) % MAX_SOCKET_OPTION;

				/// - Calculo el nivel actual y lo divido por 5
				uint8 current_level = this->GetSocket(i) / MAX_SOCKET_OPTION;
				uint8 level_jump = current_level / 5;

				if ( level_jump > 0 && this->GetSocket(i) > 253 )
				{
					int32 socket_data = 0;
					int32 socket_data_2 = 0;

					/*if ( i >= 4 )
					{
						buffer[3] |= level_jump << 4;
						socket_data_2 = level_jump << 4;
						socket_data = ((level_jump << 4) * 4);
					}
					else
					{
						buffer[4] |= ((level_jump << 6) >> (i * 2));
						socket_data_2 = ((level_jump << 6) >> (i * 2));
						socket_data = ((level_jump << 6) * 4);
					}*/

					switch ( i )
					{
					case 0:
						{
							socket_data_2 = (level_jump == 1 ? 0x40 : level_jump == 2 ? 0x80 : 0xC0);
							socket_data = (socket_data_2 << (2 * i)) * 4;
							buffer[4] |= socket_data_2;
						} break;

					case 1:
						{
							socket_data_2 = (level_jump == 1 ? 0x10 : level_jump == 2 ? 0x20 : 0x30);
							socket_data = (socket_data_2 << (2 * i)) * 4;
							buffer[4] |= socket_data_2;
						} break;

					case 2:
						{
							socket_data_2 = (level_jump == 1 ? 0x04 : level_jump == 2 ? 0x08 : 0x0C);
							socket_data = (socket_data_2 << (2 * i)) * 4;
							buffer[4] |= socket_data_2;
						} break;

					case 3:
						{
							socket_data_2 = (level_jump == 1 ? 0x01 : level_jump == 2 ? 0x02 : 0x03);
							socket_data = (socket_data_2 << (2 * i)) * 4;
							buffer[4] |= socket_data_2;
						} break;

					case 4:
						{
							socket_data_2 = (level_jump == 1 ? 0x10 : level_jump == 2 ? 0x20 : 0x30);
							socket_data = (socket_data_2 * 4 * 4);
							buffer[3] |= socket_data_2;
						} break;
					}

					buffer[7 + i] = (this->GetSocket(i) - socket_data) + 2;
				}
				else
				{
					buffer[7 + i] = this->GetSocket(i) & 0xFF;
				}
			}
		}
	}
	else
	{
		SOCKET_SLOT_LOOP(i)
		{
			buffer[7 + i] = this->GetSocket(i) & 0xFF;
		}
	}

	if (this->GetKind1() == ItemKind1::MASTERY_WEAPON || this->GetKind3() == ItemKind3::BLESSED_ARCHANGEL_WEAPON)
	{
		int32 bda_exe_loop = 0;

		if (this->GetExe() & 8 && bda_exe_loop < 2)
		{
			buffer[7 + bda_exe_loop++] = 7;
		}

		if (this->GetExe() & 16 && bda_exe_loop < 2)
		{
			buffer[7 + bda_exe_loop++] = 6;
		}
	}
	else if (this->GetKind1() == ItemKind1::MASTERY_ARMOR_1)
	{
		int32 bda_exe_loop = 0;

		if (this->GetExe() & 8 && bda_exe_loop < 3)
		{
			buffer[7 + bda_exe_loop++] = 8;
		}

		if (this->GetExe() & 32 && bda_exe_loop < 3)
		{
			buffer[7 + bda_exe_loop++] = 6;
		}

		if (this->GetExe() & 16 && bda_exe_loop < 3)
		{
			buffer[7 + bda_exe_loop++] = 7;
		}

		if (this->GetExe() & 1 && bda_exe_loop < 3)
		{
			buffer[7 + bda_exe_loop++] = 9;
		}
	}
	else if (this->GetKind1() == ItemKind1::MASTERY_ARMOR_2)
	{
		int32 bda_exe_loop = 0;

		if (this->GetExe() & 8 && bda_exe_loop < 4)
		{
			buffer[7 + bda_exe_loop++] = 8;
		}

		if (this->GetExe() & 32 && bda_exe_loop < 4)
		{
			buffer[7 + bda_exe_loop++] = 6;
		}

		if (this->GetExe() & 16 && bda_exe_loop < 4)
		{
			buffer[7 + bda_exe_loop++] = 7;
		}

		if (this->GetExe() & 1 && bda_exe_loop < 4)
		{
			buffer[7 + bda_exe_loop++] = 10;
		}
	}

	if (this->IsMysteriousStoneItem())
	{
		buffer[11] = this->GetSocketBonus();
	}
}

void Item::CalculatePrice()
{
	if (this->GetItem() == ITEMGET(14, 15) || this->GetItem() == RUUD)
	{
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(this->GetItem());

	if (!item_info)
	{
		return;
	}

	uint64 zen = 0;

	if (item_info->GetBuyPrice(this->GetLevel()))
	{
		zen = item_info->GetBuyPrice(this->GetLevel());

		if (item_info->HasPriceFlag(ITEM_PRICE_FLAG_DURABILITY_MULTIPLIER, this->GetLevel()))
		{
			zen *= this->GetDurability();
		}
		if (item_info->HasPriceFlag(ITEM_PRICE_FLAG_LEVEL_MULTIPLIER, this->GetLevel()))
		{
			zen *= this->GetLevel();
		}
		if (item_info->HasPriceFlag(ITEM_PRICE_FLAG_LEVEL_PLUS_MULTIPLIER, this->GetLevel()))
		{
			zen *= (this->GetLevel() + 1);
		}
		if (item_info->HasPriceFlag(ITEM_PRICE_FLAG_DURABILITY_DIFFERENCE, this->GetLevel()))
		{
			if (this->GetDurability() >= 1.0f)
			{
				zen *= float(item_info->GetDurability()) / this->GetDurability();
			}
		}

		this->SetBuyPrice(zen);
		this->SetSellPrice(zen / 3);

		return;
	}

	int32 level_tmp = (int32)sqrt((float)this->GetLevel());

	int32 Level2 = item_info->GetLevel() + this->GetLevel() * 3;

	if (item_info->GetKind1() != ItemKind1::WINGS && this->GetExe())
	{
		Level2 += 25;
	}

	switch (this->GetItem())
	{
	case PET_DINORANT:
		{
			zen = 960000;

			if (this->HasOptionData(ItemOption::DINORANT_DEFENSE))
			{
				zen += 300000;
			}
			if (this->HasOptionData(ItemOption::ATTACK_SPEED))
			{
				zen += 300000;
			}
			if (this->HasOptionData(ItemOption::DINORANT_STAMINA))
			{
				zen += 300000;
			}
		} break;

	default:
		{
			if (this->GetKind2() == ItemKind::PENTAGRAM)
			{
				zen = ((float)(this->GetLevel() * 10 * this->GetLevel() * this->GetLevel() + 50 * item_info->GetLevel()) + 100000) * (this->GetSocketCount() * 0.8f);
			}
			else if (this->GetKind2() == ItemKind::ERRTEL)
			{
				zen = 0;

				for (int32 i = 0; i < MAX_SOCKET_SLOT; ++i)
				{
					if (this->GetSocket(i) != SOCKET_SLOT_NONE)
					{
						zen += this->GetSocket(i) >> 4;
					}
				}

				uint64 v5 = 5 * zen + item_info->GetLevel();
				zen = (v5 / 8 * v5 * (v5 + 40) + 100) * 2 * this->GetSocketCount();
			}
			else if (this->GetItemType() != 12 && this->GetItemType() != 13 && this->GetItemType() != 15 || this->GetKind1() == ItemKind1::WINGS)
			{
				switch (this->GetLevel())
				{
				case  5:	Level2 += 4;	break;
				case  6:	Level2 += 10;	break;
				case  7:	Level2 += 25;	break;
				case  8:	Level2 += 45;	break;
				case  9:	Level2 += 65;	break;
				case 10:	Level2 += 95;	break;
				case 11:	Level2 += 135;	break;
				case 12:	Level2 += 185;	break;
				case 13:	Level2 += 245;	break;
				case 14:	Level2 += 305;	break;
				case 15:	Level2 += 365;	break;
				}

				if (this->GetKind1() != ItemKind1::WINGS || this->GetKind2() == ItemKind::MINI_WINGS)
				{
					zen = (Level2 + 40) * Level2 * Level2 / 8 + 100;
				}
				else
				{
					zen = (Level2 + 40) * Level2 * Level2 * 11 + 40000000;
				}

				if ((this->GetItemType() >= 0 && this->GetItemType() <= 6) && !this->IsTwoHand())
				{
					zen = zen * 80 / 100;
				}

				if (this->GetSkill())
				{
					zen += zen * 15 / 10;
				}

				for (auto & itr : this->m_OptionData)
				{
					const auto option_id = itr.first;
					auto option_value = itr.second;

					switch (option_id)
					{
					case ItemOption::HP_RECOVERY:
						switch (option_value)
						{
						case 1:
							zen += zen * 6 / 10;
							break;

						case 2:
							zen += zen * 14 / 10;
							break;

						case 3:
							zen += zen * 28 / 10;
							break;

						case 4:
							zen += zen * 56 / 10;
							break;
						}
						break;

					case ItemOption::PHYSICAL_DAMAGE:
					case ItemOption::WIZARDRY_DAMAGE:
					case ItemOption::CURSE_DAMAGE:
					case ItemOption::DEFENSE:
						switch (option_value)
						{
						case 4:
							zen += zen * 6 / 10;
							break;

						case 8:
							zen += zen * 14 / 10;
							break;

						case 12:
							zen += zen * 28 / 10;
							break;

						case 16:
							zen += zen * 56 / 10;
							break;
						}
						break;

					case ItemOption::DEFENSE_SUCCESS_RATE:
						switch (option_value)
						{
						case 5:
							zen += zen * 6 / 10;
							break;

						case 10:
							zen += zen * 14 / 10;
							break;

						case 15:
							zen += zen * 28 / 10;
							break;

						case 20:
							zen += zen * 56 / 10;
							break;
						}
						break;

					case ItemOption::LUCK:
						zen += zen * 25 / 100;
						break;

					case ItemOption::PHYSICAL_DAMAGE_MUL:
					case ItemOption::WIZARDRY_DAMAGE_MUL:
					case ItemOption::ATTACK_SPEED:
					case ItemOption::UNK12:
						zen *= 2;
						break;

					case ItemOption::DINORANT_STAMINA:
					case ItemOption::DINORANT_DEFENSE:
						zen += zen * 25 / 100;
						break;
					}
				}

				if (this->GetKind1() == ItemKind1::WINGS)
				{
					for (uint8 i = 0; i < 4; i++)
					{
						if ((this->GetExe() & (1 << i)) == (1 << i))
						{
							zen += zen * 25 / 100;
						}
					}
				}
				else
				{
					for (uint8 i = 0; i < MAX_OPTION; i++)
					{
						if ((this->GetExe() & (1 << i)) == (1 << i))
						{
							zen += zen;
						}
					}
				}

				if (this->Get380())
				{
					zen += zen * 16 / 100;
				}

				if (this->GetKind3() == ItemKind3::SOCKET)
				{
					zen += zen * 7 * this->GetSocketCount() / 10;
					SOCKET_SLOT_LOOP(i)
					{
						uint8 socket_id = this->GetSocket(i) % MAX_SOCKET_OPTION;
						uint8 socket_level = this->GetSocket(i) / MAX_SOCKET_OPTION;

						uint16 seed_sphere = -1;
						uint8 seed_sphere_level = 0;

						item_socket_seed const* pSeed = sItemMgr->GetSocketSeed(socket_id);

						if (pSeed)
						{
							seed_sphere = sItemMgr->GetSocketSeedSphere(pSeed->GetType(), socket_level);
							seed_sphere_level = pSeed->GetSubType();
						}

						if (seed_sphere == uint16(-1))
						{
							continue;
						}

						Item item(seed_sphere, seed_sphere_level);
						item.Convert();
						item.CalculatePrice();

						zen += item.GetBuyPrice();
					}
				}
			}
			else if (this->IsMuunItem())
			{
				zen = 1500000;
			}
			else if (item_info->GetValue())
			{
				zen += (item_info->GetValue() * item_info->GetValue() * 10) / 12;

				if (this->GetItem() == ITEMGET(14, 3) || this->GetItem() == ITEMGET(14, 6))
				{
					zen = 1500;
				}

				if (this->GetItem() >= ITEMGET(14, 0) && this->GetItem() <= ITEMGET(14, 8))	// Potions + Antidote
				{
					if (this->GetLevel() > 0)
					{
						zen *= (uint64)(pow(2.0f, level_tmp));
					}

					zen = zen / 10 * 10;
					zen *= this->GetDurability();

					this->SetBuyPrice(zen);
					this->SetSellPrice(zen / 3);
					this->SetSellPrice(this->GetSellPrice() / 10 * 10);
					return;
				}
			}
			else
			{
				zen = Level2 * Level2 * Level2 + 100;

				for (auto & itr : this->m_OptionData)
				{
					const auto option_id = itr.first;
					const auto option_value = itr.second;

					if (option_id == ItemOption::HP_RECOVERY)
					{
						zen += zen * option_value;
					}
					else if (option_id == ItemOption::PHYSICAL_DAMAGE)
					{
						zen += zen * (option_value / 4);
					}
				}
			}
		} break;
	}

	if (zen > 3000000000)
	{
		zen = 3000000000;
	}

	if (this->IsSocketPendant())
	{
		zen /= 100;
	}

	this->SetBuyPrice(zen);
	this->SetSellPrice(zen / 3);

	if (this->GetItem() < ITEMGET(12, 0) ||
		this->GetItem() == PET_UNIRIA ||
		this->GetItem() == PET_DINORANT ||
		this->GetItem() == PET_DARK_HORSE ||
		this->GetItem() == PET_DARK_RAVEN ||
		this->GetItem() == PET_FENRIR ||
		item_info->GetKind2() == ItemKind::HELPER ||
		item_info->GetKind2() == ItemKind::MINI_WINGS ||
		item_info->GetKind2() == ItemKind::WINGS_LVL_1 ||
		item_info->GetKind2() == ItemKind::WINGS_LVL_2 ||
		item_info->GetKind2() == ItemKind::WINGS_LVL_3 ||
		item_info->GetKind2() == ItemKind::WINGS_LVL_4 ||
		item_info->GetKind2() == ItemKind::LORD_CAPE ||
		item_info->GetKind2() == ItemKind::CLOAK_LVL_2 ||
		item_info->GetKind2() == ItemKind::MONSTER_WINGS ||
		item_info->GetKind2() == ItemKind::WINGS_CONQUERORS ||
		item_info->GetKind2() == ItemKind::WINGS_ANGEL_AND_DEVIL ||
		item_info->GetKind2() == ItemKind::WINGS_POWER ||
		item_info->GetKind2() == ItemKind::RING ||
		item_info->GetKind2() == ItemKind::PHYSICAL_PENDANT ||
		item_info->GetKind2() == ItemKind::WIZARD_PENDANT ||
		item_info->GetKind2() == ItemKind::SHOP_RING ||
		item_info->GetKind2() == ItemKind::SHOP_PENDANT ||
		item_info->GetKind2() == ItemKind::TRANSFORMATION_RING_1 ||
		item_info->GetKind2() == ItemKind::TRANSFORMATION_RING_2 ||
		item_info->GetKind2() == ItemKind::EARRING ||
		item_info->GetKind2() == ItemKind::DECORATION_RING ||
		item_info->GetKind2() == ItemKind::GUARDIAN_MOUNT)
	{
		float lc15 = 1.0f - (this->GetDurability() / this->GetDurabilityBase());
		int32 lc16 = this->GetSellPrice() * 0.6f * lc15;
		this->SetSellPrice(this->GetSellPrice() - lc16);
	}
}

void Item::CalculateOldPrice()
{
	if (this->GetItem() == ITEMGET(14, 15) || this->GetItem() == RUUD)
	{
		return;
	}

	uint32 Gold = this->GetBuyPrice();

	if (this->GetItem() == JEWEL::BLESS) // Bless
	{
		Gold = 100000 * this->GetDurability();
	}
	else if (this->GetItem() == JEWEL::SOUL) // Soul
	{
		Gold = 70000 * this->GetDurability();
	}
	else if (this->GetItem() == JEWEL::CHAOS) // Chaos
	{
		Gold = 40000 * this->GetDurability();
	}
	else if (this->GetItem() == JEWEL::CREATION) // Creation
	{
		Gold = 450000 * this->GetDurability();
	}
	else if (this->GetItem() == JEWEL::LIFE) // Life
	{
		Gold = 450000 * this->GetDurability();
	}

	this->SetOldPrice(Gold);
}

void Item::FixPrice()
{
	if (this->GetItem() == ITEMGET(14, 15) || this->GetItem() == RUUD)
	{
		return;
	}

	if ( this->GetBuyPrice() >= 1000 )
	{
		this->SetBuyPrice((this->GetBuyPrice() / 100) * 100);
	}
	else if ( this->GetBuyPrice() >= 100 )
	{
		this->SetBuyPrice((this->GetBuyPrice() / 10) * 10);
	}

	if ( this->GetSellPrice() >= 1000 )
	{
		this->SetSellPrice((this->GetSellPrice() / 100) * 100);
	}
	else if ( this->GetSellPrice() >= 100 )
	{
		this->SetSellPrice((this->GetSellPrice() / 10) * 10);
	}

	if ( this->GetOldPrice() >= 1000 )
	{
		this->SetOldPrice((this->GetOldPrice() / 100) * 100);
	}
	else if ( this->GetOldPrice() >= 100 )
	{
		this->SetOldPrice((this->GetOldPrice() / 10) * 10);
	}
}

int32 Item::GetRepairZen(uint8 repair_way)
{
	if ( this->GetDurabilityOriginal() <= 0.0f )
	{
		return 0;
	}

	if ( this->GetDurability() == this->GetDurabilityBase() )
	{
		return 0;
	}

	/*uint32 selling_price = this->GetSellPrice();

	if ( selling_price >= 400000000 )
	{
		selling_price = 400000000;
	}

	float v35 = 1.0 - (this->GetDurability() / this->GetDurabilityBase());
	float v34 = 0.0;
		
	if ( v35 <= 0.0 )
	{
		v34 = 0.0;
	}
	else
	{
		float v6 = sqrt(double(selling_price));
		float v7 = sqrt(double(sqrt(double(selling_price))));
		float v33 = v6 * 3.0f * v7;
		float v10 = v33 * v35;
		v34 = v10 + 1.0;

		if ( this->GetDurability() <= 0 )
		{
			if ( this->GetItem() != PET_DARK_RAVEN && this->GetItem() != PET_DARK_HORSE )
			{
				v34 = v34 * 0.4f + v34;
			}
			else
			{
				v34 = v34 + v34;
			}
		}

		if ( repair_way == 1 )
		{
			v34 = v34 * 2.5f;
		}
	}

	v11 = sub_96BA53();
	v12 = sub_54C236(v11);
	if ( (unsigned __int8)(*(int (__thiscall **)(_DWORD))((char *)&off_95A06C[5] + 1))(v12) && (v13 = sub_96BA53(), v14 = sub_54C236(v13), sub_60CFEA(v14)) )
	{
		v37 = (signed int)v34;
	}
	else
	{
		v15 = sub_96BA53();
		v16 = sub_4972F6(v15);

		if ( !(unsigned __int8)(*(int (__thiscall **)(int))((char *)&off_95A06C[5] + 1))(v16) || (v17 = sub_96BA53(), v18 = sub_54C236(v17), (unsigned __int8)(*(int (__thiscall **)(int))((char *)&off_95A06C[5] + 1))(v18)) )
			v37 = (signed int)v34;
		else
			v37 = (signed int)(v34 * 1.5 + v34);
	}
		

	int32 final_price = (int32)v34;

	if ( final_price >= 1000 )
	{
		final_price = (final_price / 100) * 100;
	}
	else if ( final_price >= 100 )
	{
		final_price = (final_price / 10) * 10;
	}

	return final_price;*/

	float lc6 = 1.0f - (this->GetDurability() / this->GetDurabilityBase());
	int32 lc7 = 0;
	float lc5 = 0.0f;
	int32 repair_cost = 0;

	if ( this->GetItem() == PET_DARK_RAVEN || this->GetItem() == PET_DARK_HORSE )
		lc7 = this->GetBuyPrice();
	else
		lc7 = this->GetBuyPrice() / 3;
	
	if ( lc7 > 400000000 )
		lc7 = 400000000;

	if ( lc7 >= 1000 )
		lc7 = lc7 / 100 * 100;
	else if ( lc7 >= 100 )
		lc7 = lc7 / 10 * 10;

	float lc8 = sqrt(double(lc7));
	float lc9 = sqrt(double(sqrt(double(lc7))));
	lc5 = 3.0 * lc8 * lc9;
	lc5 *= lc6;
	lc5 += 1.0f;

	if ( this->GetDurability() <= 0.0 )
	{
		if ( this->GetItem() == PET_DARK_RAVEN || this->GetItem() == PET_DARK_HORSE )
			lc5 *= 2;
		else
			lc5 *= 0.4f;
	}

	if ( repair_way == 1 )
		lc5 += (lc5 * 1.5f);

	repair_cost = lc5;

	if ( repair_cost >= 1000 )
		repair_cost = (repair_cost / 100) * 100;
	else if ( repair_cost >= 100 )
		repair_cost = (repair_cost / 10) * 10;

	return repair_cost;
}

bool Item::IsWingLvl1() const
{ 
	return this->GetKind2() == ItemKind::WINGS_LVL_1;
}

bool Item::IsWingLvl2() const
{ 
	return this->GetKind2() == ItemKind::WINGS_LVL_2 || this->GetKind2() == ItemKind::CLOAK_LVL_2 || this->GetKind2() == ItemKind::LORD_CAPE;
}

bool Item::IsWingLvl3() const 
{ 
	return this->GetKind2() == ItemKind::WINGS_LVL_3;
}

bool Item::IsWingMonster() const
{
	return this->GetKind2() == ItemKind::MONSTER_WINGS;
}

bool Item::IsMiniWing() const
{ 
	return this->GetKind2() == ItemKind::MINI_WINGS;
}

bool Item::IsSpecialWing() const 
{ 
	return this->GetKind2() == ItemKind::WINGS_CONQUERORS || this->GetKind2() == ItemKind::WINGS_ANGEL_AND_DEVIL || this->GetKind2() == ItemKind::WINGS_POWER;
}

bool Item::IsWingLvl4() const
{
	return this->GetKind2() == ItemKind::WINGS_LVL_4;
}

bool Item::IsWingType(uint8 type) const
{
	bool result = false;

	switch (this->GetItem())
	{
	case ITEMGET(12, 414):
	case ITEMGET(12, 438):
	case ITEMGET(12, 469):
	case ITEMGET(12, 470):
	{
				 if (type != 2)
				 {
					 break;
				 }

				 result = true;
	} break;

	case ITEMGET(12, 415):
	case ITEMGET(12, 416):
	case ITEMGET(12, 418):
	case ITEMGET(12, 420):
	case ITEMGET(12, 421):
	case ITEMGET(12, 439):
	case ITEMGET(12, 440):
	case ITEMGET(12, 442):
	case ITEMGET(12, 444):
	case ITEMGET(12, 445):
	case ITEMGET(12, 474):
	case ITEMGET(12, 477):
	{
				 if (type != 1)
				 {
					 break;
				 }

				 result = true;
	} break;

	case ITEMGET(12, 417):
	case ITEMGET(12, 441):
	{
				 if (type != 1 && type != 2)
				 {
					 break;
				 }

				 result = true;
	} break;

	case ITEMGET(12, 419):
	case ITEMGET(12, 443):
	{
				 if (type != 2 && type != 3)
				 {
					 break;
				 }

				 result = true;
	} break;

	case ITEMGET(12, 480):
	{
				 if (type != 1 && type != 2 && type != 3)
				 {
					 break;
				 }

				 result = true;
	} break;
	}

	return result;
}

bool Item::IsSocketPendant() const
{
	return this->GetKind3() == ItemKind3::SOCKET && this->GetKind1() == ItemKind1::PENDANT;
}

bool Item::GivePetExperience(int64 experience)
{
	if ( this->GetDurability() <= 0.0f )
	{
		return false;
	}

	if ( this->pet_dark.level < 1 )
	{
		this->pet_dark.level = 1;
		this->pet_dark.experience = 0;
		this->Convert(true);
		return false;
	}

	if ( this->pet_dark.level >= MAX_PET_LEVEL )
	{
		return false;
	}

	uint32 const* experience_table = nullptr;

	if ( this->GetItem() == PET_DARK_HORSE )
	{
		experience_table = sPetExperience->m_DarkHorse;
	}
	else if ( this->GetItem() == PET_DARK_RAVEN )
	{
		experience_table = sPetExperience->m_DarkSpirit;
	}

	if ( !experience_table )
	{
		return false;
	}

	int32 add_experience = cast(int32, experience);

	uint32 min_experience = experience_table[this->pet_dark.level];
	uint32 max_experience = experience_table[this->pet_dark.level + 1];
	int32 diff = max_experience - min_experience;

	if ( (this->pet_dark.experience + add_experience) < diff )
	{
		this->pet_dark.experience += add_experience;
		return false;
	}

	/*
	Min = 201 -> 301 -> 401 -> 501
	Max = 300 -> 400 -> 500 -> 600
	Dif = 99  -> 99  -> 99  -> 99

	Cur = 50 -> 0
	Add = 300 -> 251



	*/

	while ( true )
	{
		add_experience -= (diff - this->pet_dark.experience);
		this->pet_dark.level++;
		this->pet_dark.experience = 0;

		min_experience = experience_table[this->pet_dark.level];
		max_experience = experience_table[this->pet_dark.level + 1];
		diff = max_experience - min_experience;

		if ( this->pet_dark.level >= MAX_PET_LEVEL )
		{
			this->pet_dark.experience = (max_experience - min_experience);
			break;
		}

		if ( add_experience < 0 )
		{
			add_experience = 0;
			this->pet_dark.experience = 0;
			break;
		}

		if ( add_experience < diff )
		{
			this->pet_dark.experience = add_experience;
			break;
		}
	}

	this->Convert(true);
	return true;
}

bool Item::PetLevelDown(int32 experience)
{
	if ( this->pet_dark.level < 2 )
		return false;

	this->pet_dark.experience -= experience;
	this->pet_dark.level--;

	if ( this->GetItem() == PET_DARK_HORSE )
	{
		int32 tmp_level = this->pet_dark.level;
		uint32 tmp_exp = sPetExperience->m_DarkHorse[tmp_level];

		while ( this->pet_dark.experience <= tmp_exp )
		{
			if ( tmp_level <= 1 )
			{
				break;
			}

			tmp_level--;
			tmp_exp = sPetExperience->m_DarkHorse[tmp_level];
		}

		this->pet_dark.level = tmp_level;
	}
	else if ( this->GetItem() == PET_DARK_RAVEN )
	{
		int32 tmp_level = this->pet_dark.level;
		uint32 tmp_exp = sPetExperience->m_DarkSpirit[tmp_level];

		while ( this->pet_dark.experience <= tmp_exp )
		{
			if ( tmp_level <= 1 )
			{
				break;
			}

			tmp_level--;
			tmp_exp = sPetExperience->m_DarkSpirit[tmp_level];
		}

		this->pet_dark.level = tmp_level;
	}

	this->Convert(true);
	return true;
}

bool Item::PetExeDownPercent(int32 percent)
{
	if ( this->pet_dark.level < 1 )
	{
		this->pet_dark.level = 1;
		this->pet_dark.experience = 0;
		this->Convert(true);
		return false;
	}

	if ( this->GetItem() == PET_DARK_HORSE )
	{
		int32 experience = (sPetExperience->m_DarkHorse[1+this->pet_dark.level] - sPetExperience->m_DarkHorse[this->pet_dark.level]) * percent / 100;
		this->pet_dark.experience -= experience;

		if ( this->pet_dark.experience < 0 )
		{
			this->pet_dark.level--;
			this->pet_dark.experience = 0;

			if ( this->pet_dark.level < 1 )
			{
				this->pet_dark.level = 1;
			}

			this->Convert(true);
		}
	}
	else if ( this->GetItem() == PET_DARK_RAVEN )
	{
		int32 experience = (sPetExperience->m_DarkSpirit[1+this->pet_dark.level] - sPetExperience->m_DarkSpirit[this->pet_dark.level]) * percent / 100;
		this->pet_dark.experience -= experience;

		if ( this->pet_dark.experience < 0 )
		{
			this->pet_dark.level--;
			this->pet_dark.experience = 0;

			if ( this->pet_dark.level < 1 )
			{
				this->pet_dark.level = 1;
			}

			this->Convert(true);
		}
	}

	return false;
}

void Item::PetValue()
{
	if ( this->GetItem() == PET_DARK_HORSE )
	{
		this->SetBuyPrice(this->pet_dark.level * 2000000);
	}
	else if ( this->GetItem() == PET_DARK_RAVEN )
	{
		this->SetBuyPrice(this->pet_dark.level * 2000000);
	}	
}

int32 Item::CriticalDamageRate() const
{
	if ( !this->IsItem() )
		return 0;

	if ( !this->IsValid() )
		return 0;

	if ( this->GetDurability() <= 0.0f )
		return 0;

	return (this->GetLuck() ? 5 : 0);
}

int32 Item::FenrirIncLastDamage() const
{
	if ( (this->GetExe() & FENRIR_OPTION_INC_DMG) == FENRIR_OPTION_INC_DMG )
	{
		return 10;
	}

	return 0;
}
	
int32 Item::FenrirDecLastDamage() const
{
	if ( (this->GetExe() & FENRIR_OPTION_DEC_DMG) == FENRIR_OPTION_DEC_DMG )
	{
		return 10;
	}

	return 0;
}

int32 Item::FenrirIllusion() const
{
	if ( (this->GetExe() & FENRIR_OPTION_ILLUSION) == FENRIR_OPTION_ILLUSION )
	{
		return 20;
	}

	return 0;
}

float Item::GetMasteryShieldDefense() const
{
	float value = 0.1f;

	switch (this->GetLevel())
	{
	case 1:
	{
			  value = 0.3f;
	} break;

	case 2:
	{
			  value = 0.4f;
	} break;

	case 3:
	{
			  value = 0.6f;
	} break;

	case 4:
	{
			  value = 0.7f;
	} break;

	case 5:
	{
			  value = 0.8f;
	} break;

	case 6:
	{
			  value = 1.0f;
	} break;

	case 7:
	{
			  value = 1.1f;
	} break;

	case 8:
	{
			  value = 1.3f;
	} break;

	case 9:
	{
			  value = 1.5f;
	} break;

	case 10:
	{
			   value = 1.6f;
	} break;

	case 11:
	{
			   value = 1.8f;
	} break;

	case 12:
	{
			   value = 1.9f;
	} break;

	case 13:
	{
			   value = 2.0f;
	} break;

	case 14:
	{
			   value = 2.2f;
	} break;

	case 15:
	{
			   value = 2.4f;
	} break;
	}

	return value;
}

bool Item::FenrirIncLastDamage(uint8 exe)
{
	return (exe & FENRIR_OPTION_INC_DMG) == FENRIR_OPTION_INC_DMG;
}
	
bool Item::FenrirDecLastDamage(uint8 exe)
{
	return (exe & FENRIR_OPTION_DEC_DMG) == FENRIR_OPTION_DEC_DMG;
}
	
bool Item::FenrirIllusion(uint8 exe)
{
	return (exe & FENRIR_OPTION_ILLUSION) == FENRIR_OPTION_ILLUSION;
}

void Item::CalculateDurability()
{
	if ( this->GetDurability() != 0 )
		return;

	this->SetDurability(sItemMgr->CalculateDurability(this->GetItem(), this->GetLevel(), this->IsExcellent(), this->GetAncient()));
}

bool Item::CheckDurabilityState()
{
	if ( this->GetDurability() == 0.0f )
	{
		if ( this->GetDurabilityCurrentStatus() != 1.0f )
		{
			this->SetDurabilityCurrentStatus(1.0f);
			this->Convert(true);
			return true;
		}
	}
	else if ( this->GetDurability() < this->GetDurabilityStatus(2) )
	{
		if ( this->GetDurabilityCurrentStatus() != 0.5f )
		{
			this->SetDurabilityCurrentStatus(0.5f);
			this->Convert(true);
			return true;
		}
	}
	else if ( this->GetDurability() < this->GetDurabilityStatus(1) )
	{
		if ( this->GetDurabilityCurrentStatus() != 0.3f )
		{
			this->SetDurabilityCurrentStatus(0.3f);
			this->Convert(true);
			return true;
		}
	}
	else if ( this->GetDurability() < this->GetDurabilityStatus(0) )
	{
		if ( this->GetDurabilityCurrentStatus() != 0.2f )
		{
			this->SetDurabilityCurrentStatus(0.2f);
			this->Convert(true);
			return true;
		}
	}
	else if ( this->GetDurabilityCurrentStatus() != 0.0f )
	{
		this->SetDurabilityCurrentStatus(0.0f);
		this->Convert(true);
		return true;
	}

	return false;
}

uint8 Item::DurabilityDownSimple(float value)
{
	this->AlterDurability(0, value);

	if ( this->GetDurability() < 0.0f )
	{
		this->SetDurability(0.0f);
		return DURABILITY_STATE_NONE;
	}

	if ( this->CheckDurabilityState() )
	{
		return DURABILITY_STATE_CHANGED;
	}

	return DURABILITY_STATE_DOWN;
}

uint8 Item::DurabilityDownValue(float value, float reduction)
{
	if ( this->GetDurability() <= 0 )
	{
		return DURABILITY_STATE_NONE;
	}

	float small_tmp = this->GetDurabilityResistance();
	float durability_small = small_tmp + ((small_tmp * reduction) / 100.0f);

	this->IncreaseDurabilityState(value);
	this->SetState(ITEM_CHANGED);

	if ( this->GetDurabilityState() > durability_small )
	{
		this->SetDurabilityState(0);
		this->AlterDurability(0, 1.0f);

		if ( this->CheckDurabilityState() )
		{
			return DURABILITY_STATE_CHANGED;
		}
		else
		{
			return DURABILITY_STATE_DOWN;
		}
	}

	return DURABILITY_STATE_NONE;
}

DurabilityState Item::DurabilityDownArmor(float value, float reduction)
{
	if ( this->GetDurability() <= 0 )
	{
		return DURABILITY_STATE_NONE;
	}

	int32 def = this->GetDefenseOriginal();
	float durability_decrease = 0.0f;

	if ( !def )
	{
		return DURABILITY_STATE_NONE;
	}

	if (this->GetKind2() == ItemKind::SHIELD)
	{
		durability_decrease = value / (def * 5 + this->GetOptionValue(ItemOption::DEFENSE_SUCCESS_RATE));
	}
	else if ( this->GetRequiredClass(Character::DARK_WIZARD) != 0 )
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if ( this->GetRequiredClass(Character::DARK_KNIGHT) != 0 )
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if ( this->GetRequiredClass(Character::FAIRY_ELF) != 0 )
	{
		durability_decrease = value / (def * 2 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if ( this->GetRequiredClass(Character::MAGIC_GLADIATOR) != 0 )
	{
		durability_decrease = value / (def * 7 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if ( this->GetRequiredClass(Character::DARK_LORD) != 0 )
	{
		durability_decrease = value / (def * 6 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if ( this->GetRequiredClass(Character::SUMMONER) != 0 )
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if ( this->GetRequiredClass(Character::RAGE_FIGHTER) != 0 )
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if ( this->GetRequiredClass(Character::GROW_LANCER) != 0 )
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if (this->GetRequiredClass(Character::RUNE_WIZARD) != 0)
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if (this->GetRequiredClass(Character::SLAYER) != 0)
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}
	else if (this->GetRequiredClass(Character::GUN_CRUSHER) != 0)
	{
		durability_decrease = value / (def * 3 + this->GetOptionValue(ItemOption::DEFENSE));
	}

	this->IncreaseDurabilityState(durability_decrease);
	this->SetState(ITEM_CHANGED);

	float small_tmp = this->GetDurabilityResistance();

	float durability_small = small_tmp + ((small_tmp * reduction) / 100.0f);

	if ( this->GetDurabilityState() > durability_small )
	{
		this->SetDurabilityState(0);
		this->AlterDurability(0, 1.0f);

		if ( this->CheckDurabilityState() )
		{
			return DURABILITY_STATE_CHANGED;
		}
		else
		{
			return DURABILITY_STATE_DOWN;
		}
	}

	return DURABILITY_STATE_NONE;
}

DurabilityState Item::DurabilityDownWeapon(int32 defense, float reduction)
{
	if ( !this->IsValid() )
	{
		return DURABILITY_STATE_NONE;
	}

	if ( this->GetExpireDate() )
	{
		return DURABILITY_STATE_NONE;
	}

	int32 damage_min = this->GetMinDamage();
	int32 plus_damage = this->GetOptionValue(ItemOption::PHYSICAL_DAMAGE);

	if ( damage_min == 0 )
	{
		return DURABILITY_STATE_NONE;
	}

	int32 div = damage_min + damage_min / 2 + plus_damage;

	if ( div == 0 )
	{
		return DURABILITY_STATE_NONE;
	}

	int32 DecreaseDur = (defense * 2) / div;
	DecreaseDur -= DecreaseDur* this->GetSocketDurabilityReduction() / 100;
	this->IncreaseDurabilityState(DecreaseDur);
	this->SetState(ITEM_CHANGED);

	float small_tmp = this->GetDurabilityResistance();

	float durability_small = small_tmp + ((small_tmp * reduction) / 100.0f);

	if ( this->GetDurabilityState() > durability_small )
	{
		this->SetDurabilityState(0);
		this->AlterDurability(0, 1.0f);

		if ( this->CheckDurabilityState() )
		{
			return DURABILITY_STATE_CHANGED;
		}
		else
		{
			return DURABILITY_STATE_DOWN;
		}
	}

	return DURABILITY_STATE_NONE;
}

DurabilityState Item::DurabilityDownStaff(int32 defense, float reduction)
{
	if ( !this->IsValid() )
	{
		return DURABILITY_STATE_NONE;
	}

	if ( this->GetExpireDate() )
	{
		return DURABILITY_STATE_NONE;
	}

	int32 magic = this->GetMagicPower() / 2 + this->GetLevel() * 2;
	int32 plus_magic = this->GetOptionValue(ItemOption::WIZARDRY_DAMAGE);
	int32 div = magic + magic / 3 + plus_magic;

	if ( div == 0 )
	{
		return DURABILITY_STATE_NONE;
	}

	int32 DecreaseDur = defense / div;
	DecreaseDur -= DecreaseDur* this->GetSocketDurabilityReduction() / 100;
	this->IncreaseDurabilityState(DecreaseDur);
	this->SetState(ITEM_CHANGED);

	float small_tmp = this->GetDurabilityResistance();

	float durability_small = small_tmp + ((small_tmp * reduction) / 100.0f);

	if ( this->GetDurabilityState() > durability_small )
	{
		this->SetDurabilityState(0);
		this->AlterDurability(0, 1.0f);

		if ( this->CheckDurabilityState() )
		{
			return DURABILITY_STATE_CHANGED;
		}
		else
		{
			return DURABILITY_STATE_DOWN;
		}
	}

	return DURABILITY_STATE_NONE;
}

uint8 Item::ProcessDurabilityTime(Player* pPlayer)
{
	if (!this->IsItem())
	{
		return 0;
	}

	if (this->IsFlag(ITEM_FLAG_TALISMAN_ITEM_PROTECTION) && pPlayer->HasBuff(BUFF_TALISMAN_OF_ITEM_PROTECTION))
	{
		return 0;
	}

	if (this->GetExpireDate())
	{
		return 0;
	}

	bool decrease = false;

	if (pPlayer->IsInSafeZone())
	{
		decrease = this->IsFlag(ITEM_FLAG_DURABILITY_TIME_SAFE) && this->GetDurabilityTime()->Elapsed(this->GetDurabilityTimeRequiredSafe());
	}
	else
	{
		decrease = this->IsFlag(ITEM_FLAG_DURABILITY_TIME) && this->GetDurabilityTime()->Elapsed(this->GetDurabilityTimeRequired());
	}

	if (decrease)
	{
		return this->DurabilityDownValue(1, pPlayer->GetItemDurabilityReduction(this->GetDurabilityReduction()));
	}

	return 0;
}

bool Item::IsTransformationRing(bool shop) const
{
	if ( !this->IsItem() )
	{
		return false;
	}

	item_transformation const* pInfo = sItemMgr->GetItemTransformation(this->GetItem(), this->GetLevel());

	if ( !pInfo )
	{
		return false;
	}

	if ( !shop )
	{
		if ( pInfo->IsSpecial() )
		{
			return false;
		}
	}

	return true;
}

void Item::Convert(bool update)
{
	item_template const* item_info = sItemMgr->GetItem(this->GetItem());

	if ( !item_info )
	{
		if ( update && (this->GetBox() == ITEM_BOX_INVENTORY || this->GetBox() == ITEM_BOX_WAREHOUSE) )
		{
			this->SetState(ITEM_REMOVED);
		}

		return;
	}

	if ( this->GetOption() > sGameServer->GetJewelOfLifeMax() )
	{
		this->SetOption(sGameServer->GetJewelOfLifeMax());
	}

	this->LimitMaxLevel(MAX_ITEM_LEVEL);

	if ( this->GetItem() == ITEMGET(13, 16) ||
		 this->GetItem() == ITEMGET(13, 17) ||
		 this->GetItem() == ITEMGET(13, 18) )
	{
		this->LimitMinLevel(1);
		this->LimitMaxLevel(MAX_BLOOD_CASTLE_GROUND);
	}
	else if ( this->GetItem() == ITEMGET(14, 17) ||
			  this->GetItem() == ITEMGET(14, 18) ||
			  this->GetItem() == ITEMGET(14, 19) )
	{
		this->LimitMinLevel(1);
		this->LimitMaxLevel(MAX_DEVIL_SQUARE_GROUND);
	}

	this->SetSkillID(item_info->GetSkill());
	this->SetTwoHand(false);
	this->SetMaxSocket(item_info->GetMaxSocket());
	this->SetFlag(item_info->GetFlags(this->GetLevel()));

	this->SetKind1(item_info->GetKind1());
	this->SetKind2(item_info->GetKind2());
	this->SetKind3(item_info->GetKind3());

	if ( item_info->GetSkill() == 0 )
		this->SetSkill(0);

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_LUCK) )
		this->SetLuck(0);

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_OPTION) )
		this->SetOption(0);

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_HARMONY) && !sItemMgr->IsLuckyItem(this->GetItem()) )
	{
		this->SetHarmony(HARMONY_EFFECT_NONE);
	}

	if (item_info->GetKind3() != ItemKind3::PVP)
	{
		this->Set380(0);
	}

	if (item_info->GetKind2() != ItemKind::WINGS_LVL_2 &&
		item_info->GetKind2() != ItemKind::LORD_CAPE &&
		item_info->GetKind2() != ItemKind::CLOAK_LVL_2 &&
		item_info->GetKind2() != ItemKind::WINGS_LVL_3 &&
		item_info->GetKind2() != ItemKind::WINGS_LVL_4 &&
		item_info->GetKind2() != ItemKind::MONSTER_WINGS)
	{
		if (item_info->GetExcellentType() == uint8(-1) && item_info->GetItem() != PET_FENRIR)
			this->SetExe(0);
	}

	if (item_info->GetMaxSocket() == 0 && !this->IsPentagramItem() && this->GetKind2() != ItemKind::ERRTEL && !this->IsPentagramMithril() &&
		!this->IsMuunItem() && 
		!this->IsMuunUtil() && 
		!this->IsWingLvl4() && 
		!this->IsMysteriousStoneItem() && 
		!this->IsEarring() && 
		item_info->GetKind2() != ItemKind::GUARDIAN_MOUNT)
	{
		this->ResetSocket(SOCKET_SLOT_NONE);
		this->SetSocketBonus(uint8(-1));
	}

	if ( item_info->GetMaxSocket() > 0 )
	{
		this->SetAncient(0);
	}

	this->SetExcellent(this->GetExe() != 0 && item_info->IsExcellent());
	this->SetExcellentType(item_info->GetExcellentType());

	if (this->IsAncient())
	{
		if (sItemMgr->GetItemSetType(this->GetItem()) == nullptr)
		{
			this->SetAncient(0);
			this->SetAncientAttribute(-1);
			this->SetAncientAttributeValue(0);
		}
		else
		{
			this->SetAncientAttribute(item_info->GetAncientAttribute());

			if (this->GetAncientValue() == 0)
			{
				this->SetAncient(this->GetAncient() | ANCIENT_OPTION_1);
			}

			if (item_info->GetAncientAttribute() == MAX_STAT_TYPE)
			{
				uint8 ancient_value = this->GetAncientValue();

				if (ancient_value == 1)
				{
					this->SetAncientAttributeValue(7);
				}
				else if (ancient_value == 2)
				{
					this->SetAncientAttributeValue(15);
				}
			}
			else
			{
				this->SetAncientAttributeValue(this->GetAncientValue() * 5);
			}
		}
	}
	else
	{
		this->SetAncientAttribute(-1);
		this->SetAncientAttributeValue(0);
	}

	if ( this->GetItem() == PET_DARK_HORSE || this->GetItem() == PET_DARK_RAVEN )
	{
		if ( this->pet_dark.level >= 1 && this->pet_dark.level <= MAX_PET_LEVEL )
		{
			uint32 const* experience_table = nullptr;

			if ( this->GetItem() == PET_DARK_HORSE )
			{
				experience_table = sPetExperience->m_DarkHorse;
			}
			else if ( this->GetItem() == PET_DARK_RAVEN )
			{
				experience_table = sPetExperience->m_DarkSpirit;
			}

			if ( experience_table )
			{
				uint32 min_experience = experience_table[this->pet_dark.level];
				uint32 max_experience = experience_table[this->pet_dark.level + 1];

				int32 diff = max_experience - min_experience;

				if ( this->pet_dark.experience > diff )
				{
					this->pet_dark.experience = diff;
				}
			}
		}
	}

	if ( !this->GetSkill() )
		this->SetSkillID(0);

	if ( !this->GetSkillID() )
		this->SetSkill(0);

	this->ResetResistance(0);

	int32 chaos_item = 0;

	if (this->GetItem() == ITEMGET(2, 6))
	{
		chaos_item = 15;
	}
	else if (this->GetItem() == ITEMGET(4, 6))
	{
		chaos_item = 25;
	}
	else if (this->GetItem() == ITEMGET(5, 7))
	{
		chaos_item = 30;
	}
	
	if ( this->GetItem() == ITEMGET(13, 169) || 
		 this->GetItem() == ITEMGET(13, 170) )
	{
		if ( this->GetDurability() > item_info->GetDurability() )
		{
			this->SetDurability(item_info->GetDurability());
			this->SetState(ITEM_CHANGED);
		}
	}

	this->SetDurabilityBase(sItemMgr->CalculateDurability(this->GetItem(), this->GetLevel(), this->IsExcellent(), this->IsAncient(), item_info));
	this->SetDurabilityResistance(item_info->GetDurabilityResistance());

	this->SetDurabilityStatus(0, this->GetDurabilityBase() * 0.5f);
	this->SetDurabilityStatus(1, this->GetDurabilityBase() * 0.3f);
	this->SetDurabilityStatus(2, this->GetDurabilityBase() * 0.2f);
	this->SetDurabilityCurrentStatus(0.0f);
	this->SetDurabilityOriginal(item_info->GetDurability());

	if ( this->GetDurability() == 0 )
	{
		this->SetDurabilityCurrentStatus(1.0f);
	}
	else if ( this->GetDurability() < this->GetDurabilityStatus(2) )
	{
		this->SetDurabilityCurrentStatus(0.5f);
	}
	else if ( this->GetDurability() < this->GetDurabilityStatus(1) )
	{
		this->SetDurabilityCurrentStatus(0.3f);
	}
	else if ( this->GetDurability() < this->GetDurabilityStatus(0) )
	{
		this->SetDurabilityCurrentStatus(0.2f);
	}

	this->SetAttackSpeed(item_info->GetAttackSpeed());

	this->SetValue(item_info->GetValue());

	this->SetDefense(item_info->GetDefense());
	this->SetDefenseMagic(item_info->GetDefenseMagic());
	this->SetDefenseSuccessRate(item_info->GetDefenseSuccess());
	this->SetDefenseOriginal(0);
	this->SetMinDamage(item_info->GetMinDamage());
	this->SetMaxDamage(item_info->GetMaxDamage());

	this->SetQuestItem(item_info->IsQuest());

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		this->SetRequiredClass(i, item_info->GetRequiredClass(i));
	}

	this->SetRequiredLevel(0);
	this->ResetStatRequired(0);
	this->ResetStatDecrease(0);
	this->SetSocketDurabilityReduction(0);

	this->SetMagicPower(0);
	this->SetCursePower(0);
	this->SetPetPower(0);
	this->SetUseDelay(item_info->GetUseDelay());
	this->SetDurationTime(item_info->GetDurationTime());
	this->SetLootingTime(item_info->GetLootingTime());
	this->SetDurabilityTimeRequired(item_info->GetDurabilityTime());
	this->SetDurabilityTimeRequiredSafe(item_info->GetDurabilityTimeSafe());
	this->SetDurabilityReduction(item_info->GetDurabilityReduction());
	this->SetForceUpgrade(false);
	this->SetName(item_info->GetName(this->GetLevel()));
	this->SetNormalLevel(item_info->IsNormalLevel());

	if (item_info->GetX() > 1 && item_info->GetKind2() != ItemKind::SHIELD)
		this->SetTwoHand(true);
	
	this->SetSlot(item_info->GetSlot());

	for ( uint8 i = 0; i < Element::MAX; ++i )
	{
		if (item_info->GetResistance(i) > 0)
		{
			int32 tmp_resistance = item_info->GetResistance(i) + this->GetLevel();

			this->SetResistance(i, ((tmp_resistance > 255 || tmp_resistance < 0) ? 255 : tmp_resistance));
		}
	}

	bool is_excellent = this->IsExcellent();
	bool is_ancient = this->IsAncient();
	int32 item_level = item_info->GetLevel();
	if (is_excellent || is_ancient)
	{
		item_level = item_info->GetLevel() + 25;
	}

	this->CalculateRequiredStat(item_info, is_excellent, is_ancient, item_level);

	item_level = item_info->GetLevel() + 30;

	this->CalculateDamage(item_info, is_excellent, is_ancient, item_level, chaos_item);
	this->CalculatePower(item_info, is_excellent, is_ancient, item_level, chaos_item);
	this->CalculateDefenseSuccessRate(item_info, is_excellent, is_ancient);
	this->CalculateDefense(item_info, is_excellent, is_ancient, item_level);
	this->CalculateDefenseMagic(item_info);
	
	if ( this->IsExcellent() || this->IsAncient() )
	{
		if ( item_info->GetSkill() != 0 )
		{
			this->SetSkill(1);
			this->SetSkillID(item_info->GetSkill());
		}
	}

	if ( this->GetSkill() && this->GetSkillID() == 66 )
	{
		this->SetForceUpgrade(true);
		this->SetSkillID(0);
	}

	if ( this->GetItem() == PET_DINORANT ||
		 this->GetItem() == PET_DARK_HORSE ||
		 this->GetItem() == PET_FENRIR )
	{
		this->SetSkill(1);
		this->SetSkillID(item_info->GetSkill());
	}

	this->CalculateOptionValue(item_info);
	this->CalculateHarmonyOption();
	this->CalculateSocketOption();

	if ( this->GetItem() != ITEMGET(4, 7) && this->GetItem() != ITEMGET(4, 15) && !this->IsPentagramItem() )
	{
		this->SetDefenseOriginal(this->GetDefense());
		//this->ReduceMinDamage(this->GetMinDamage() * this->GetDurabilityCurrentStatus());
		//this->ReduceMaxDamage(this->GetMaxDamage() * this->GetDurabilityCurrentStatus());
		this->ReduceDefense(this->GetDefense() * this->GetDurabilityCurrentStatus());
		this->ReduceDefenseSuccessRate(this->GetDefenseSuccessRate() * this->GetDurabilityCurrentStatus());
	}
	
	if ( this->GetDurability() <= 0.0f )
	{
		this->ResetResistance(0);
	}

	this->CalculatePrice();
	this->CalculateOldPrice();
	this->PetValue();
	this->FixPrice();

	if ( update )
		this->SetState(ITEM_CHANGED);
}

void Item::CalculateDamage(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level, int32 chaos_weapon)
{
	if (this->GetMinDamage() > 0)
	{
		if ((is_excellent || is_ancient) && item_info->GetLevel() > 0)
		{
			if (chaos_weapon)
			{
				this->IncreaseMinDamage(chaos_weapon);
			}
			else
			{
				this->IncreaseMinDamage(item_info->GetMinDamage() * 25 / item_info->GetLevel() + 5);
			}
		}

		if (is_ancient)
		{
			this->IncreaseMinDamage(item_level / 40 + 5);
		}

		if (item_info->GetKind2() == ItemKind::PENTAGRAM)
		{
			this->IncreaseMinDamage(4 * this->GetLevel());
		}
		else
		{
			this->IncreaseMinDamage(3 * this->GetLevel());
		}

		if (this->GetLevel() > 9)
		{
			this->IncreaseMinDamage((this->GetLevel() - 8) * (this->GetLevel() - 9) / 2);
		}
	}

	if (this->GetMaxDamage() > 0)
	{
		if ((is_excellent || is_ancient) && item_info->GetLevel() > 0)
		{
			if (chaos_weapon)
			{
				this->IncreaseMaxDamage(chaos_weapon);
			}
			else
			{
				this->IncreaseMaxDamage(item_info->GetMinDamage() * 25 / item_info->GetLevel() + 5);
			}
		}

		if (is_ancient)
		{
			this->IncreaseMaxDamage(item_level / 40 + 5);
		}

		if (item_info->GetKind2() == ItemKind::PENTAGRAM)
		{
			this->IncreaseMaxDamage(4 * this->GetLevel());
		}
		else
		{
			this->IncreaseMaxDamage(3 * this->GetLevel());
		}

		if (this->GetLevel() > 9)
		{
			this->IncreaseMaxDamage((this->GetLevel() - 8) * (this->GetLevel() - 9) / 2);
		}
	}
}

void Item::CalculatePower(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level, int32 chaos_weapon)
{
	if (item_info->GetPower() <= 0)
	{
		return;
	}

	uint32 power = item_info->GetPower();

	if ((is_excellent || is_ancient) && item_info->GetLevel() > 0)
	{
		if (chaos_weapon)
		{
			power += chaos_weapon;
		}
		else
		{
			power += 25 * power / item_info->GetLevel() + 5;
		}
	}

	if (is_ancient)
	{
		power += item_level / 60 + 2;
	}

	power += 3 * this->GetLevel();
	if (this->GetLevel() > 9)
	{
		power += (this->GetLevel() - 9) * (this->GetLevel() - 8) / 2;
	}

	power = power >> 1;
	if (item_info->GetKind2() != ItemKind::SCEPTER)
	{
		power += 2 * this->GetLevel();
	}

	if (item_info->GetKind2() == ItemKind::SCEPTER)
	{
		this->SetPetPower(power);
	}
	else if (item_info->GetKind2() == ItemKind::BOOK)
	{
		this->SetCursePower(power);
	}
	else
	{
		this->SetMagicPower(power);
	}
}

void Item::CalculateDefenseSuccessRate(item_template const* item_info, bool is_excellent, bool is_ancient)
{
	if (this->GetDefenseSuccessRate() <= 0)
	{
		return;
	}

	if ((is_excellent || is_ancient) && item_info->GetLevel() > 0)
	{
		this->IncreaseDefenseSuccessRate((this->GetDefenseSuccessRate() * 25) / item_info->GetLevel() + 5);
	}

	this->IncreaseDefenseSuccessRate(this->GetLevel() * 3);

	if (this->GetLevel() > 9)
	{
		this->IncreaseDefenseSuccessRate((this->GetLevel() - 9) * (this->GetLevel() - 8) / 2);
	}
}
	
void Item::CalculateDefense(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level)
{
	if (this->GetDefense() <= 0)
	{
		return;
	}

	if (item_info->GetKind2() == ItemKind::SHIELD)
	{
		this->IncreaseDefense(this->GetLevel());

		if (is_ancient)
		{
			this->IncreaseDefense((20 * this->GetDefense() / item_level) + 2);
		}
	}
	else
	{
		if ((is_excellent || is_ancient) && item_info->GetLevel())
		{
			this->IncreaseDefense(12 * this->GetDefense() / item_info->GetLevel() + item_info->GetLevel() / 5 + 4);
		}

		if (is_ancient)
		{
			this->IncreaseDefense(3 * this->GetDefense() / item_level + item_level / 30 + 2);
		}

		if (this->IsWingLvl2())
		{
			this->IncreaseDefense(this->GetLevel() * 2);

			if (this->GetLevel() > 9)
			{
				this->IncreaseDefense((this->GetLevel() - 9) * (this->GetLevel() - 8) / 2);
			}
		}
		else if (this->IsWingLvl3())
		{
			this->IncreaseDefense(this->GetLevel() * 4);

			if (this->GetLevel() > 9)
			{
				this->IncreaseDefense((this->GetLevel() - 9) * (this->GetLevel() - 8) / 2);
			}
		}
		else if (!this->IsWingLvl4())
		{
			this->IncreaseDefense(this->GetLevel() * 3);

			if (this->GetLevel() > 9)
			{
				this->IncreaseDefense((this->GetLevel() - 9) * (this->GetLevel() - 8) / 2);
			}
		}
		else
		{
			this->SetDefense(sFormulaMgr->GetOrCreateValue(6, 0, this->GetLevel()));
			if (this->GetLevel() > 9)
			{
				this->IncreaseDefense(sFormulaMgr->GetOrCreateValue(6, 1, this->GetLevel()));
			}
		}
	}
}

void Item::CalculateDefenseMagic(item_template const* item_info)
{
	if (this->GetDefenseMagic() <= 0)
	{
		return;
	}

	this->IncreaseDefenseMagic(this->GetLevel() * 3);

	if (this->GetLevel() > 9)
	{
		this->IncreaseDefenseMagic((this->GetLevel() - 9) * (this->GetLevel() - 8) / 2);
	}
}

void Item::CalculateRequiredStat(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level)
{
	if (item_info->GetKind2() == ItemKind::WINGS_LVL_1 || item_info->GetKind2() == ItemKind::LORD_CAPE || item_info->GetKind2() == ItemKind::CLOAK_LVL_2 || item_info->GetKind2() == ItemKind::MONSTER_WINGS)
	{
		this->SetRequiredLevel(item_info->GetRequiredLevel() + (4 * this->GetLevel()));
	}
	else if (item_info->GetKind2() == ItemKind::WINGS_LVL_2)
	{
		this->SetRequiredLevel(item_info->GetRequiredLevel() + (5 * this->GetLevel()));
	}
	else if (this->GetItem() == ITEMGET(13, 10))
	{
		if (this->GetLevel() > 2)
		{
			this->SetRequiredLevel(50);
		}
		else
		{
			this->SetRequiredLevel(20);
		}
	}
	else if (item_info->GetKind1() == ItemKind1::PENDANT || item_info->GetKind1() == ItemKind1::RING)
	{
		this->SetRequiredLevel(item_info->GetRequiredLevel() + (4 * this->GetLevel()));

		if (is_excellent || is_ancient)
		{
			this->IncreaseRequiredLevel(20);
		}
	}
	else if (item_info->GetKind2() == ItemKind::PENTAGRAM)
	{
		this->SetRequiredLevel(item_info->GetRequiredLevel() + (4 * this->GetLevel()));
	}
	else if (this->GetItem() == PET_DARK_HORSE)
	{
		this->SetRequiredLevel((2 * this->pet_dark.level) + 218);
	}
	else if (item_info->GetRequiredLevel() > 0)
	{
		this->SetRequiredLevel(item_info->GetRequiredLevel());
	}
	else
	{
		this->SetRequiredLevel(0);
	}

	if (item_info->GetRequiredStat(STRENGTH) > 0)
	{
		if (item_info->GetLevel() < 220)
		{
			this->SetStatRequired(STRENGTH, 3 * (3 * this->GetLevel() + item_level) * item_info->GetRequiredStat(STRENGTH) / 100 + 20);
		}
		else
		{
			if (is_excellent)
			{
				this->SetStatRequired(STRENGTH, item_info->GetRequiredStat(STRENGTH) + item_info->GetRequiredStat(STRENGTH) * 0.75f + item_info->GetRequiredStat(STRENGTH) * 0.02f * this->GetLevel());
			}
			else
			{
				this->SetStatRequired(STRENGTH, item_info->GetRequiredStat(STRENGTH) + item_info->GetRequiredStat(STRENGTH) * 0.02f * this->GetLevel());
			}
		}
	}
	else
	{
		this->SetStatRequired(STRENGTH, 0);
	}

	if (item_info->GetRequiredStat(AGILITY) > 0)
	{
		if (item_info->GetLevel() < 220)
		{
			this->SetStatRequired(AGILITY, 3 * (3 * this->GetLevel() + item_level) * item_info->GetRequiredStat(AGILITY) / 100 + 20);
		}
		else
		{
			if (is_excellent)
			{
				this->SetStatRequired(AGILITY, item_info->GetRequiredStat(AGILITY) + item_info->GetRequiredStat(AGILITY) * 0.75f + item_info->GetRequiredStat(AGILITY) * 0.02f * this->GetLevel());
			}
			else
			{
				this->SetStatRequired(AGILITY, item_info->GetRequiredStat(AGILITY) + item_info->GetRequiredStat(AGILITY) * 0.02f * this->GetLevel());
			}
		}
	}
	else
	{
		this->SetStatRequired(AGILITY, 0);
	}

	if (item_info->GetRequiredStat(ENERGY) > 0)
	{
		if (item_info->GetLevel() < 220)
		{
			if (item_info->GetKind2() == ItemKind::BOOK)
			{
				this->SetStatRequired(ENERGY, 3 * (this->GetLevel() + item_level) * item_info->GetRequiredStat(ENERGY) / 100 + 20);
			}
			else if (item_info->GetRequiredLevel() > 0 && (item_info->GetKind2() == ItemKind::SKILL_SCROLL_1 || item_info->GetKind2() == ItemKind::SKILL_PARCHMENT))
			{
				this->SetStatRequired(ENERGY, 4 * item_info->GetRequiredLevel() * item_info->GetRequiredStat(ENERGY) / 100 + 20);
			}
			else
			{
				this->SetStatRequired(ENERGY, item_info->GetRequiredStat(ENERGY) * (3 * this->GetLevel() + item_level) * 4 / 100 + 20);
			}
		}
		else
		{
			if (is_excellent)
			{
				this->SetStatRequired(ENERGY, item_info->GetRequiredStat(ENERGY) * 2 + item_info->GetRequiredStat(ENERGY) * 0.035f * this->GetLevel());
			}
			else
			{
				this->SetStatRequired(ENERGY, item_info->GetRequiredStat(ENERGY) + item_info->GetRequiredStat(ENERGY) * 0.035f * this->GetLevel());
			}
		}
	}
	else
	{
		this->SetStatRequired(ENERGY, 0);
	}

	if (item_info->GetRequiredStat(VITALITY) > 0)
	{
		if (item_info->GetLevel() < 220)
		{
			this->SetStatRequired(VITALITY, 3 * (3 * this->GetLevel() + item_level) * item_info->GetRequiredStat(VITALITY) / 100 + 20);
		}
		else
		{
			if (is_excellent)
			{
				this->SetStatRequired(VITALITY, item_info->GetRequiredStat(VITALITY) + item_info->GetRequiredStat(VITALITY) * 0.75f + item_info->GetRequiredStat(VITALITY) * 0.02f * this->GetLevel());
			}
			else
			{
				this->SetStatRequired(VITALITY, item_info->GetRequiredStat(VITALITY) + item_info->GetRequiredStat(VITALITY) * 0.02f * this->GetLevel());
			}
		}
	}
	else
	{
		this->SetStatRequired(VITALITY, 0);
	}

	if (item_info->GetRequiredStat(LEADERSHIP) > 0)
	{
		this->SetStatRequired(LEADERSHIP, 3 * (3 * this->GetLevel() + item_level) * item_info->GetRequiredStat(LEADERSHIP) / 100 + 20);
	}
	else
	{
		this->SetStatRequired(LEADERSHIP, 0);
	}

	if (this->GetItem() == PET_DARK_RAVEN)
	{
		this->SetStatRequired(LEADERSHIP, (10 * this->pet_dark.level) + 185);
	}
	else
	{
		this->SetStatRequired(LEADERSHIP, item_info->GetRequiredStat(LEADERSHIP));
	}

	if (item_info->GetKind1() == ItemKind1::SKILL)
	{
		this->SetStatRequired(STRENGTH, item_info->GetRequiredStat(STRENGTH));
		this->SetStatRequired(AGILITY, item_info->GetRequiredStat(AGILITY));
		this->SetStatRequired(VITALITY, item_info->GetRequiredStat(VITALITY));
		this->SetStatRequired(ENERGY, item_info->GetRequiredStat(ENERGY));
		this->SetStatRequired(LEADERSHIP, item_info->GetRequiredStat(LEADERSHIP));

		if (this->GetItem() == ITEMGET(12, 11))
		{
			switch (this->GetLevel())
			{
			case 0:
				this->SetStatRequired(ENERGY, 30);
				break;
			case 1:
				this->SetStatRequired(ENERGY, 60);
				break;
			case 2:
				this->SetStatRequired(ENERGY, 90);
				break;
			case 3:
				this->SetStatRequired(ENERGY, 130);
				break;
			case 4:
				this->SetStatRequired(ENERGY, 170);
				break;
			case 5:
				this->SetStatRequired(ENERGY, 210);
				break;
			case 6:
				this->SetStatRequired(ENERGY, 300);
				break;
			default:
				this->SetStatRequired(ENERGY, 0);
				break;
			}
		}
	}

	if (this->GetOption() > 0 && this->GetStatRequired(STRENGTH) > 0)
	{
		if (item_info->GetKind1() == ItemKind1::WEAPON ||
			item_info->GetKind1() == ItemKind1::ARMOR ||
			item_info->GetKind1() == ItemKind1::MASTERY_WEAPON ||
			item_info->GetKind1() == ItemKind1::MASTERY_ARMOR_1 ||
			item_info->GetKind1() == ItemKind1::MASTERY_ARMOR_2)
		{
			if (item_info->GetKind3() != ItemKind3::LUCKY)
			{
				this->IncreaseStatRequired(STRENGTH, 5 * this->GetOption());
			}
		}
	}
}

void Item::CalculateOptionValue(item_template const* pItemInfo)
{
	this->m_OptionData.clear();

	if (this->GetLuck() &&
		(pItemInfo->GetKind1() == ItemKind1::WEAPON ||
		pItemInfo->GetKind1() == ItemKind1::ARMOR ||
		pItemInfo->GetKind1() == ItemKind1::WINGS ||
		pItemInfo->GetKind1() == ItemKind1::MASTERY_WEAPON ||
		pItemInfo->GetKind1() == ItemKind1::MASTERY_ARMOR_1 ||
		pItemInfo->GetKind1() == ItemKind1::MASTERY_ARMOR_2))
	{
		this->AddOptionData(ItemOption::LUCK, 0);
	}

	if (this->GetOption())
	{
		if (this->GetItem() == PET_DINORANT)
		{
			if (this->GetOption() & 1)
			{
				this->AddOptionData(ItemOption::DINORANT_DEFENSE, 5);
			}

			if (this->GetOption() & 2)
			{
				this->AddOptionData(ItemOption::DINORANT_STAMINA, 50);
			}

			if (this->GetOption() & 4)
			{
				this->AddOptionData(ItemOption::ATTACK_SPEED, 5);
			}
		}
		else
		{
			if (pItemInfo->GetKind2() == ItemKind::SWORD ||
				pItemInfo->GetKind2() == ItemKind::MAGIC_SWORD ||
				pItemInfo->GetKind2() == ItemKind::FIST ||
				pItemInfo->GetKind2() == ItemKind::AXE ||
				pItemInfo->GetKind2() == ItemKind::MACE ||
				pItemInfo->GetKind2() == ItemKind::SCEPTER ||
				pItemInfo->GetKind2() == ItemKind::LANCE ||
				pItemInfo->GetKind2() == ItemKind::BOW ||
				pItemInfo->GetKind2() == ItemKind::CROSSBOW ||
				pItemInfo->GetKind2() == ItemKind::ARROW ||
				pItemInfo->GetKind2() == ItemKind::BOLT ||
				pItemInfo->GetKind2() == ItemKind::QUIVER ||
				pItemInfo->GetKind2() == ItemKind::SHORT_SWORD)
			{
				this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
			}
			else if (pItemInfo->GetKind2() == ItemKind::STAFF)
			{
				this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
			}
			else if (pItemInfo->GetKind2() == ItemKind::STICK)
			{
				this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
			}
			else if (pItemInfo->GetKind2() == ItemKind::BOOK)
			{
				this->AddOptionData(ItemOption::CURSE_DAMAGE, 4 * this->GetOption());
			}
			else if (pItemInfo->GetKind2() == ItemKind::RUNIC_MACE)
			{
				this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
			}
			else if (pItemInfo->GetKind2() == ItemKind::SHIELD)
			{
				this->AddOptionData(ItemOption::DEFENSE_SUCCESS_RATE, 5 * this->GetOption());
			}
			else if (pItemInfo->GetKind2() == ItemKind::HELM ||
				pItemInfo->GetKind2() == ItemKind::ARMOR ||
				pItemInfo->GetKind2() == ItemKind::PANTS ||
				pItemInfo->GetKind2() == ItemKind::GLOVES ||
				pItemInfo->GetKind2() == ItemKind::BOOTS)
			{
				this->AddOptionData(ItemOption::DEFENSE, 4 * this->GetOption());
			}
			else if (pItemInfo->GetKind1() == ItemKind1::RING || pItemInfo->GetKind1() == ItemKind1::PENDANT)
			{
				if (this->GetItem() == ITEMGET(13, 24))
				{
					this->AddOptionData(ItemOption::MAX_MANA, this->GetOption());
				}
				else if (this->GetItem() == ITEMGET(13, 28))
				{
					this->AddOptionData(ItemOption::MAX_STAMINA, this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
			}
			else if (pItemInfo->GetKind2() == ItemKind::MAGIC_GUN)
			{
				this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
			}

			switch (this->GetItem())
			{
			case ITEMGET(12, 0):
				this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				break;

			case ITEMGET(12, 1):
			case ITEMGET(12, 41):
				this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				break;

			case ITEMGET(12, 2):
				this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				break;

			case ITEMGET(12, 3):
			case ITEMGET(12, 424):
				if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				break;

			case ITEMGET(12, 4):
			case ITEMGET(12, 422):
				if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 5):
			case ITEMGET(12, 49):
			case ITEMGET(12, 423):
			case ITEMGET(12, 428):
				if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 6):
			case ITEMGET(12, 425):
				if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				break;

			case ITEMGET(12, 42):
			case ITEMGET(12, 427):
				if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::CURSE_DAMAGE, 4 * this->GetOption());
				}
				break;

			case ITEMGET(13, 30):
			case ITEMGET(12, 426):
				this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				break;

			case ITEMGET(12, 36):
			case ITEMGET(12, 38):
			case ITEMGET(12, 40):
			case ITEMGET(12, 50):
			case ITEMGET(12, 270):
			case ITEMGET(12, 431):
			case ITEMGET(12, 432):
			case ITEMGET(12, 434):
			case ITEMGET(12, 436):
			case ITEMGET(12, 437):
			case ITEMGET(12, 415):
			case ITEMGET(12, 439):
			case ITEMGET(12, 416):
			case ITEMGET(12, 440):
			case ITEMGET(12, 418):
			case ITEMGET(12, 442):
			case ITEMGET(12, 420):
			case ITEMGET(12, 444):
			case ITEMGET(12, 421):
			case ITEMGET(12, 445):
			case ITEMGET(12, 472):
			case ITEMGET(12, 473):
			case ITEMGET(12, 474):
				if (this->GetExe() & 0x10)
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				else if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::DEFENSE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 37):
			case ITEMGET(12, 430):
			case ITEMGET(12, 414):
			case ITEMGET(12, 438):
			case ITEMGET(12, 467):
			case ITEMGET(12, 468):
			case ITEMGET(12, 469):
				if (this->GetExe() & 0x10)
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				else if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::DEFENSE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 39):
			case ITEMGET(12, 433):
			case ITEMGET(12, 417):
			case ITEMGET(12, 441):
				if (this->GetExe() & 0x10)
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				else if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 43):
			case ITEMGET(12, 435):
			case ITEMGET(12, 419):
			case ITEMGET(12, 443):
				if (this->GetExe() & 0x10)
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				else if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::CURSE_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 269):
			case ITEMGET(12, 429):
				if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 262):
			case ITEMGET(12, 263):
			case ITEMGET(12, 265):
			case ITEMGET(12, 279):
			case ITEMGET(12, 280):
			case ITEMGET(12, 282):
			case ITEMGET(12, 284):
			case ITEMGET(12, 285):
			case ITEMGET(12, 287):
				if (this->GetExe() & 0x10)
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;

			case ITEMGET(12, 264):
			case ITEMGET(12, 281):
			case ITEMGET(12, 286):
				if (this->GetExe() & 0x10)
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::CURSE_DAMAGE, 4 * this->GetOption());
				}
				break;

			case ITEMGET(12, 266):
				this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				break;

			case ITEMGET(12, 268):
				if (this->GetExe() & 0x20)
				{
					this->AddOptionData(ItemOption::WIZARDRY_DAMAGE, 4 * this->GetOption());
				}
				else if (this->GetExe() & 0x10)
				{
					this->AddOptionData(ItemOption::PHYSICAL_DAMAGE, 4 * this->GetOption());
				}
				else
				{
					this->AddOptionData(ItemOption::HP_RECOVERY, this->GetOption());
				}
				break;
			}
		}
	}

	if (this->GetItem() == PET_DARK_HORSE)
	{
		this->AddOptionData(ItemOption::DARK_HORSE_DEFENSE, 2 * this->pet_dark.level + 5);
	}

	if (this->GetItem() == ITEMGET(13, 20))
	{
		switch (this->GetLevel())
		{
		case 0:
		case 3:
			this->AddOptionData(ItemOption::WIZARDRY_DAMAGE_MUL, 10);
			this->AddOptionData(ItemOption::PHYSICAL_DAMAGE_MUL, 10);
			this->AddOptionData(ItemOption::ATTACK_SPEED, 10);
			break;
		}
	}

	if (this->GetItem() == ITEMGET(13, 107))
	{
		this->AddOptionData(ItemOption::WIZARDRY_DAMAGE_MUL, 15);
		this->AddOptionData(ItemOption::PHYSICAL_DAMAGE_MUL, 15);
		this->AddOptionData(ItemOption::ATTACK_SPEED, 10);
	}


	/*
	
  if ( sub_A95FBD(pItem) )
  {
    if ( excellent )
    {
      a3a = 0;
      v10 = 5;
      while ( v10 >= 0 )
      {
        if ( (excellent >> v10) & 1 )
          sub_A94F1D(pItem, 5 - v10, a3a);
        --v10;
        ++a3a;
      }
    }
    if ( !sub_A93F49(pItem) )
    {
      for ( i = 0; i < 3; ++i )
      {
        if ( pItem->mastery_excellent[i + 6] != 255 )
          sub_A94F1D(pItem, pItem->mastery_excellent[i + 6], i + 6);
      }
    }
  }
  if ( IsItemKind1(pItem, 19) == 1 )
  {
    Dst = 0;
    v8 = 0;
    memcpy_s(&Dst, 5u, pItem->mastery_excellent, 5u);
    v5 = GetItemInfo(pItem);
    sub_A94F9C(pItem, (int)&Dst, (unsigned __int16 *)(v5 + 6));
    memset(pItem->mastery_excellent, 0, 5u);
    sub_A9508C(pItem);
  }
	*/
}

void Item::CalculateHarmonyOption()
{
	if ( this->IsHarmonyUpgrade() )
	{
		uint8 h_option = this->GetHarmonyOption();
		uint8 level = this->GetHarmonyLevel();

		if ( level >= MAX_ITEM_HARMONY_LEVEL )
			return;

		if ( this->GetHarmonyItem() == HARMONY_ITEM_WEAPON )
		{
			item_harmony_data * data = sItemMgr->GetHarmony(this->GetHarmonyItem(), h_option);

			if ( !data )
				return;

			if ( h_option == HARMONY_WEAPON_DECREASE_REQUIRED_STRENGTH )
			{
				if (this->GetRequiredStat(STRENGTH) > 0)
				{
					this->IncreaseStatDecrease(STRENGTH, data->GetValue(level));
				}
			}
			else if ( h_option == HARMONY_WEAPON_DECREASE_REQUIRED_AGILITY )
			{
				if (this->GetRequiredStat(AGILITY) > 0)
				{
					this->IncreaseStatDecrease(AGILITY, data->GetValue(level));
				}
			}
		}
		else if ( this->GetHarmonyItem() == HARMONY_ITEM_STAFF )
		{
			item_harmony_data * data = sItemMgr->GetHarmony(this->GetHarmonyItem(), h_option);

			if ( !data )
				return;

			if ( h_option == HARMONY_STAFF_DECREASE_REQUIRED_STRENGTH )
			{
				if (this->GetRequiredStat(STRENGTH) > 0)
				{
					this->IncreaseStatDecrease(STRENGTH, data->GetValue(level));
				}
			}
			else if ( h_option == HARMONY_STAFF_DECREASE_REQUIRED_AGILITY )
			{
				if (this->GetRequiredStat(AGILITY) > 0)
				{
					this->IncreaseStatDecrease(AGILITY, data->GetValue(level));
				}
			}
		}
	}
}

void Item::CalculateSocketOption()
{
	for ( uint8 n = 0; n < MAX_SOCKET_SLOT; ++n )
	{
		if ( this->GetSocket(n) != SOCKET_SLOT_EMPTY && this->GetSocket(n) != SOCKET_SLOT_NONE )
		{
			uint8 index = this->GetSocket(n) % MAX_SOCKET_OPTION;
			uint8 level = this->GetSocket(n) / MAX_SOCKET_OPTION;

			if ( level >= MAX_SOCKET_LEVEL )
				return;

			item_socket_seed const* seed = sItemMgr->GetSocketSeed(index);

			if ( !seed )
				continue;

			uint32 value = seed->GetLevel(level);
			
			switch(index)
			{
			case 20:
				{
					this->IncreaseSocketDurabilityReduction(value);
				} break;
			}
		}
	}
}

uint32 Item::GetSocketOption(uint8 opt)
{
	uint8 index = 0;
	uint8 level = 0;
	item_socket_seed const* seed = nullptr;
	uint32 value = 0;

	for ( uint8 i = 0; i < MAX_SOCKET_SLOT; ++i )
	{
		if ( this->GetSocket(i) == SOCKET_SLOT_NONE || this->GetSocket(i) == SOCKET_SLOT_EMPTY )
			continue;

		index = this->GetSocket(i) % MAX_SOCKET_OPTION;
		level = this->GetSocket(i) / MAX_SOCKET_OPTION;

		if ( level >= MAX_SOCKET_LEVEL )
			continue;

		seed = sItemMgr->GetSocketSeed(index);

		if ( !seed )
			continue;

		if ( index == opt )
		{
			value += seed->GetLevel(level);
		}
	}

	return value;
}

std::string Item::BuildLog(uint32 slot) const
{
	std::ostringstream sstream;

	sstream << "[SERIAL]" << uint32(this->GetSerialServer()) << "/" << this->GetSerial() << "/" << this->GetSerialShop() << "[SERIAL]";

	if ( slot != 255 )
		sstream << "[SLOT]" << slot << "[SLOT]";

	sstream << "[ITEM]" << uint32(this->GetItem()) << " - " << this->GetName() << "[ITEM]";

	if ( this->GetItem() == ITEMGET(14, 15) )
	{
		sstream << "[ZEN]" << this->GetBuyPrice() << "[ZEN]";
	}
	else if (this->GetItem() == RUUD)
	{
		sstream << "[RUUD]" << this->GetBuyPrice() << "[RUUD]";
	}
	else
	{
		sstream << "[LVL]" << uint32(this->GetLevel()) << "[LVL]";
		sstream << "[DUR]" << uint32(this->GetDurability()) << "[DUR]";

		if ( this->GetSkill() )
			sstream << "[S]1[S]";

		if ( this->GetLuck() )
			sstream << "[L]1[L]";

		if ( this->GetOption() )
			sstream << "[O]" << uint32(this->GetOption()) << "[O]";

		if ( this->GetExe() )
			sstream << "[E]" << uint32(this->GetExe()) << "[E]";

		if ( this->GetAncient() )
			sstream << "[A]" << uint32(this->GetAncient()) << "[A]";

		if ( this->IsHarmonyUpgrade() )
			sstream << "[H]" << uint32(this->GetHarmony()) << "[H]";

		if ( this->Is380Option() )
			sstream << "[380]" << uint32(this->Get380()) << "[380]";

		if ( this->GetSocket(0) != SOCKET_SLOT_NONE )
			sstream << "[SOCK1]" << uint32(this->GetSocket(0)) << "[SOCK1]";
		
		if ( this->GetSocket(1) != SOCKET_SLOT_NONE )
			sstream << "[SOCK2]" << uint32(this->GetSocket(1)) << "[SOCK2]";
		
		if ( this->GetSocket(2) != SOCKET_SLOT_NONE )
			sstream << "[SOCK3]" << uint32(this->GetSocket(2)) << "[SOCK3]";
		
		if ( this->GetSocket(3) != SOCKET_SLOT_NONE )
			sstream << "[SOCK4]" << uint32(this->GetSocket(3)) << "[SOCK4]";
		
		if ( this->GetSocket(4) != SOCKET_SLOT_NONE )
			sstream << "[SOCK5]" << uint32(this->GetSocket(4)) << "[SOCK5]";

		if ( this->GetSocketBonus() != uint8(-1) )
			sstream << "[SOCKB]" << uint32(this->GetSocketBonus()) << "[SOCKB]";

		if ( this->raw.data_extra[0] != 0 )
			sstream << "[DATA1]" << this->raw.data_extra[0] << "[DATA1]";

		if ( this->raw.data_extra[1] != 0 )
			sstream << "[DATA2]" << this->raw.data_extra[1] << "[DATA2]";

		if ( this->raw.data_extra[2] != 0 )
			sstream << "[DATA3]" << this->raw.data_extra[2] << "[DATA3]";

		if ( this->GetDBFlag() != 0 )
			sstream << "[DBFLAG]" << uint32(this->GetDBFlag()) << "[DBFLAG]";
	}

	return sstream.str();
}

std::string Item::BuildLogDB(uint32 slot, bool unique) const
{
	std::ostringstream sstream;

	if (unique)
	{
		sstream << "[{";
	}
	else
	{
		sstream << "{";
	}
	
	sstream << "\"serial_server\":" << this->GetSerialServer();
	sstream << ",\"serial\":" << this->GetSerial();
	sstream << ",\"serial_cash_shop\":" << this->GetSerialShop();

	if (slot != 255)
	{
		sstream << ",\"slot\":" << slot;
	}

	sstream << ",\"item\":" << this->GetItem();
	sstream << ",\"item_name\":" << "\"" << this->GetName() << "\"";

	if (this->GetItem() == ITEMGET(14, 15))
	{
		sstream << ",\"zen\":" << this->GetBuyPrice();
	}
	else if (this->GetItem() == RUUD)
	{
		sstream << ",\"ruud\":" << this->GetBuyPrice();
	}
	else
	{
		sstream << ",\"level\":" << uint32(this->GetLevel());
		sstream << ",\"durability\":" << uint32(this->GetDurability());
		sstream << ",\"skill\":" << uint32(this->GetSkill());
		sstream << ",\"luck\":" << uint32(this->GetLuck());
		sstream << ",\"option\":" << uint32(this->GetOption());
		sstream << ",\"excellent\":" << uint32(this->GetExe());
		sstream << ",\"ancient\":" << uint32(this->GetAncient());
		sstream << ",\"harmony\":" << uint32(this->GetHarmony());
		sstream << ",\"option_380\":" << uint32(this->Get380());
		sstream << ",\"socket_1\":" << uint32(this->GetSocket(0));
		sstream << ",\"socket_2\":" << uint32(this->GetSocket(1));
		sstream << ",\"socket_3\":" << uint32(this->GetSocket(2));
		sstream << ",\"socket_4\":" << uint32(this->GetSocket(3));
		sstream << ",\"socket_5\":" << uint32(this->GetSocket(4));
		sstream << ",\"socket_bonus\":" << uint32(this->GetSocketBonus());
		sstream << ",\"data_1\":" << this->raw.data_extra[0];
		sstream << ",\"data_2\":" << this->raw.data_extra[1];
		sstream << ",\"data_3\":" << this->raw.data_extra[2];
		sstream << ",\"ps_price\":" << this->GetPersonalStorePrice();
		sstream << ",\"ps_price_job\":" << this->GetPersonalStorePriceJoB();
		sstream << ",\"ps_price_jos\":" << this->GetPersonalStorePriceJoS();
		sstream << ",\"flags\":" << this->GetDBFlag();
	}

	if (unique)
	{
		sstream << "}]";
	}
	else
	{
		sstream << "}";
	}

	return sstream.str();
}

std::string Item::BuildRegister() const
{
	return this->BuildLog(this->GetOwnerSlot());
}

std::string Item::BuildRegisterDB() const
{
	return this->BuildLogDB(this->GetOwnerSlot(), true);
}

void Item::SaveDB(SQLTransaction & trans, uint8 rules)
{
	if (this->GetBox() == ITEM_BOX_NONE)
	{
		return;
	}

	if (this->GetState() == ITEM_UNCHANGED)
	{
		return;
	}

	if (this->GetState() == ITEM_REMOVED && rules != ITEM_SAVE_RULE_REMOVE)
		return;

	if (this->GetState() == ITEM_CHANGED && rules != ITEM_SAVE_RULE_ADD)
		return;

	if (this->IsEventArchangelWeapon())
	{
		return;
	}

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_DELETE);
	stmt->setUInt32(this->GetOwner());
	stmt->setUInt8(this->GetBox());
	stmt->setUInt32(this->GetOwnerSlot());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_DELETE_BY_SERIAL);
	stmt->setUInt16(this->GetSerialServer());
	stmt->setUInt32(this->GetSerial());
	stmt->setUInt32(this->GetSerialShop());
	trans->Append(stmt);
	
	switch (this->GetState())
	{
	case ITEM_CHANGED:
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_INSERT);
		stmt->setUInt32(this->GetOwner());
		stmt->setUInt8(this->GetBox());
		stmt->setUInt32(this->GetOwnerSlot());
		stmt->setUInt16(this->GetItem());
		stmt->setUInt16(this->GetSerialServer());
		stmt->setUInt32(this->GetSerial());
		stmt->setUInt32(this->GetSerialShop());
		stmt->setUInt8(this->GetLevel());
		stmt->setUInt8(static_cast<uint8>(this->GetDurability()));
		stmt->setInt32(static_cast<int32>(this->GetDurabilityState()));
		stmt->setUInt8(this->GetSkill());
		stmt->setUInt8(this->GetLuck());
		stmt->setUInt8(this->GetOption());
		stmt->setUInt8(this->GetExe());
		stmt->setUInt8(this->GetAncient());
		stmt->setUInt8(this->Get380());
		stmt->setUInt8(this->GetHarmony());

		for (uint8 h = 0; h < MAX_SOCKET_SLOT; ++h)
		{
			stmt->setUInt16(this->GetSocket(h));
		}

		stmt->setUInt8(this->GetSocketBonus());
		stmt->setUInt8(this->IsInventoryActive() ? 1 : 0);
		stmt->setInt32(this->raw.data_extra[0]);
		stmt->setInt32(this->raw.data_extra[1]);
		stmt->setInt32(this->raw.data_extra[2]);
		stmt->setUInt32(this->GetPersonalStorePrice());
		stmt->setUInt16(this->GetPersonalStorePriceJoB());
		stmt->setUInt16(this->GetPersonalStorePriceJoS());
		stmt->setInt64(this->GetExpireDate());
		stmt->setUInt32(this->GetDBFlag());

		trans->Append(stmt);
	} break;
	}

	this->SetState(ITEM_UNCHANGED);
}

void Item::LoadDB(Field* fields, uint32 owner, uint32 slot, ItemBox box)
{
	int32 loop = 3;

	this->SetItem(fields[loop++].GetUInt16());
	this->SetSerialServer(fields[loop++].GetUInt16());
	this->SetSerial(fields[loop++].GetUInt32());
	this->SetSerialShop(fields[loop++].GetUInt32());
	this->SetLevel(fields[loop++].GetUInt8());
	this->SetDurability(static_cast<float>(fields[loop++].GetUInt8()));
	this->SetDurabilityState(static_cast<float>(fields[loop++].GetInt32()));
	this->SetSkill(fields[loop++].GetUInt8());
	this->SetLuck(fields[loop++].GetUInt8());
	this->SetOption(fields[loop++].GetUInt8());
	this->SetExe(fields[loop++].GetUInt8());
	this->SetAncient(fields[loop++].GetUInt8());
	this->Set380(fields[loop++].GetUInt8());
	this->SetHarmony(fields[loop++].GetUInt8());

	for ( uint32 i = 0; i < MAX_SOCKET_SLOT; ++i )
	{
		this->SetSocket(i, fields[loop++].GetUInt16());
	}

	this->SetSocketBonus(fields[loop++].GetUInt8());
	this->SetInventoryActive(fields[loop++].GetBool());

	this->raw.data_extra[0] = fields[loop++].GetInt32();
	this->raw.data_extra[1] = fields[loop++].GetInt32();
	this->raw.data_extra[2] = fields[loop++].GetInt32();
	this->SetPersonalStorePrice(fields[loop++].GetUInt32());
	this->SetPersonalStorePriceJoB(fields[loop++].GetUInt16());
	this->SetPersonalStorePriceJoS(fields[loop++].GetUInt16());
	this->SetExpireDate(fields[loop++].GetInt64());
	this->SetDBFlag(fields[loop++].GetUInt32());

	if ( this->GetItem() == ITEMGET(13, 70) )
	{
		this->SetDurability(2.0f);
	}

	/*if ( this->GetItem() == PET_DARK_HORSE && this->GetDurability() <= 0.0f )
	{
		this->SetDurability(1.0f);
	}*/

	if (sMonsterSoul->IsSoulConverter(this->GetItem()))
	{
		if (this->GetExpireDate() <= 0)
		{
			this->SetExpireDate(time(nullptr) + sGameServer->GetMonsterSoulDurationTime());
		}
	}

	if ( this->IsExpired() )
	{
		this->SetExpiredStatus(true);
	}

	if ((sPentagramSystem->IsPentagramMithril(this->GetItem()) || sPentagramSystem->IsPentagramItem(this->GetItem()) || this->GetKind2() == ItemKind::ERRTEL) && !this->IsValidElementalAttribute())
	{
		this->SetSocketBonus(Random(5) + 1);
	}

	if (sPentagramSystem->IsPentagramItem(this->GetItem()) || this->GetKind2() == ItemKind::ERRTEL)
	{
		this->SetSocketBonus(this->GetSocketBonus() % 16);

		for ( int32 i = 0; i < MAX_SOCKET_SLOT; ++i )
		{
			if ( this->GetSocket(i) != SOCKET_SLOT_NONE )
			{
				this->IncreaseSocketBonus(16);
			}
		}
	}

	if ( sGameServer->IsLuckyItemFix() )
	{
		if ( sItemMgr->IsLuckyItem(this->GetItem()) && this->GetDurability() <= 0.0f )
		{
			if ( this->dark_ancient.fix_count < sGameServer->GetLuckyItemFixCount() )
			{
				this->dark_ancient.fix_count++;
				this->AlterDurability(2, 1.0f);
			}
		}
	}

	if ( this->GetItem() == ITEMGET(13, 211) || this->GetItem() == ITEMGET(16, 211) )
	{
		uint16 evolution_item = MAKE_NUMBERW(this->GetSocket(0), this->GetSocket(1));

		if ( evolution_item < ITEMGET(16, 0) )
		{
			evolution_item += (3 * 512);
		}

		this->SetSocket(0, HIBYTE(evolution_item));
		this->SetSocket(1, LOBYTE(evolution_item));
		sMuunSystem->GetMuunUtilSocketOption(this);
		this->SetItem(ITEMGET(16, 211));
		this->SetState(ITEM_CHANGED);
	}

	if ( this->GetItem() == ITEMGET(13, 239) )
	{
		this->SetItem(ITEMGET(16, 239));
		this->SetState(ITEM_CHANGED);
	}

	if ( this->GetItem() == ITEMGET(13, 240) )
	{
		this->SetItem(ITEMGET(16, 240));
		this->SetState(ITEM_CHANGED);
	}

	JewelMixData const* pJewelMixData = sJewelMix->GetJewelData(this);
	if (pJewelMixData)
	{
		this->SetItem(pJewelMixData->GetItem());
		this->SetLevel(0);
		this->SetDurability((pJewelMixData->level.get() + 1) * 10);
		this->SetState(ITEM_CHANGED);
	}

	item_template const* pItemInfo = sItemMgr->GetItem(this->GetItem());

	if (pItemInfo)
	{
		if (pItemInfo->GetStackData()->GetMaxStack() > 0)
		{
			if (this->GetDurability() <= 0 && this->GetItem() != MUUN_WIZARDY_STONE)
			{
				this->SetDurability(1);
				this->SetState(ITEM_CHANGED);
			}
		}
		///- TODO: Posiblemente sea solamente necesario en el test
		if (pItemInfo->GetKind2() == ItemKind::GUARDIAN_MOUNT)
		{
			for (int32 i = 0; i < 4; ++i)
			{
				item_guardian_option_value const* pData = sItemMgr->GetGuardianOptionValue(pItemInfo->GetLevel(), this->GetSocket(i));

				if (!pData)
				{
					this->SetSocket(i, SOCKET_SLOT_NONE);

					if (i < 3)
					{
						this->SetSocket(i, this->GetSocket(i + 1));
						this->SetSocket(i + 1, SOCKET_SLOT_NONE);
					}
				}
			}
		}
	}

	this->Convert();

	this->SetOwner(owner);
	this->SetOwnerSlot(slot);
	this->SetBox(box);
}

bool Item::IsExpired() const
{
	if ( !this->GetExpireDate() )
		return false;

	return time(nullptr) >= this->GetExpireDate();
}

time_t Item::GetRemainTime() const
{
	time_t cur_time = time(nullptr);

	if ( this->GetExpireDate() > cur_time )
	{
		return this->GetExpireDate() - cur_time;
	}

	return 0;
}

void Item::BuildPentagramInfo(PentagramJewelInfo * pInfo, uint8 slot, uint8 type)
{
	pInfo->SetType(type);
	pInfo->SetIndex(slot);
	pInfo->SetAttribute(this->GetSocketBonus());
	pInfo->SetItemSection(this->GetItem() / MAX_SUBTYPE_ITEMS);
	pInfo->SetItemType(this->GetItem() % MAX_SUBTYPE_ITEMS);
	pInfo->SetItemLevel(this->GetLevel());

	SOCKET_SLOT_LOOP(i)
	{
		pInfo->GetRankData(i)->SetIndex((this->GetSocket(i) & 0xFF) % 16);
		pInfo->GetRankData(i)->SetLevel((this->GetSocket(i) & 0xFF) / 16);
	}

	pInfo->SetSerialServer(this->GetSerialServer());
	pInfo->SetSerial(this->GetSerial());
	pInfo->SetDBFlag(this->GetDBFlag());
	pInfo->SetData(0, this->raw.data_extra[0]);
	pInfo->SetData(1, this->raw.data_extra[1]);
	pInfo->SetData(2, this->raw.data_extra[2]);
}

void Item::BuildPentagramAttribute(Unit* pUnit)
{
	if ( !pUnit )
	{
		return;
	}

	if ( (this->IsPentagramItem() || this->GetKind2() == ItemKind::ERRTEL || this->IsPentagramMithril()) && (pUnit->GetElementalAttribute() > ELEMENTAL_ATTRIBUTE_NONE && pUnit->GetElementalAttribute() < ELEMENTAL_ATTRIBUTE_MAX) )
	{
		this->SetSocketBonus(pUnit->GetElementalAttribute());

		if ( !this->IsPentagramMithril() )
		{
			SOCKET_SLOT_LOOP(i)
			{
				if ( this->GetSocket(i) != SOCKET_SLOT_NONE )
				{
					this->IncreaseSocketBonus(16);
				}
			}
		}
	}
}

void Item::UpdateMixRecovery()
{
	if (this->common.time <= 0)
	{
		return;
	}

	int64 current_time = time(nullptr);

	if (current_time >= this->common.time)
	{
		this->common.time = 0;
		this->common.attempts = 0;

		if (this->common.attempts < 0)
		{
			this->common.attempts = 0;
		}

		this->SetState(ITEM_CHANGED);
		return;
	}

	int64 diff = this->common.time - current_time;

	int32 attempts = (diff / sGameServer->GetMixRecoveryFailTime()) + 1;

	if (attempts != this->common.attempts)
	{
		this->common.attempts = attempts;

		if (this->common.attempts <= 0)
		{
			this->common.attempts = 0;
			this->common.time = 0;
		}

		this->SetState(ITEM_CHANGED);
	}
}

void Item::AddOptionData(uint16 id, uint16 value)
{
	this->m_OptionData[id] = value;
}

uint16 Item::GetOptionValue(uint16 id) const
{
	if (!this->IsValid())
	{
		return 0;
	}

	auto itr = this->m_OptionData.find(id);
	if (itr != this->m_OptionData.end())
	{
		return itr->second;
	}

	return 0;
}

bool Item::HasOptionData(uint16 id) const
{
	return this->m_OptionData.find(id) != this->m_OptionData.end();
}

void Item::ApplyOption(uint16 id, int32 & value, float mod) const
{
	for (auto & itr : this->m_OptionData)
	{
		const auto option_id = itr.first;

		if (option_id != id)
		{
			continue;
		}

		value += value * (mod / 100);
	}
}