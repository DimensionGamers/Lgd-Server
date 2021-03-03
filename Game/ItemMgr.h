#ifndef ITEM_HANDLER_H
#define ITEM_HANDLER_H

struct item_stack
{
	DECLARE_ENUM(uint8, MaxStack);
	DECLARE_BOOL(StackOnLoot);
	DECLARE_ENUM(uint8, StackConvertType);
	DECLARE_ENUM(uint16, StackConvertIndex);
	DECLARE_PTR(item_template, ConvertedItem);

	uint16 GetConvert() const { return ITEMGET(this->GetStackConvertType(), this->GetStackConvertIndex()); }

	void Reset()
	{
		this->SetMaxStack(0);
		this->SetStackOnLoot(false);
		this->SetStackConvertType(uint8(-1));
		this->SetStackConvertIndex(uint16(-1));
		this->SetConvertedItem(nullptr);
	}
};

struct item_level_template
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Level);
	DECLARE_STRING_FIXED(Name, MAX_ITEM_NAME_LENGTH);
	DECLARE_PROPERTY(uint32, Price);
	DECLARE_ENUM(uint32, PriceFlag);
	DECLARE_ENUM(uint32, Flags);
};

typedef std::map<uint8, item_level_template*> ItemLevelTemplateMap;

struct item_template
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_STRING_FIXED(Name, MAX_ITEM_NAME_LENGTH);
	DECLARE_ENUM(uint8, Slot);
	DECLARE_ENUM(uint8, X);
	DECLARE_ENUM(uint8, Y);
	DECLARE_FLAG(uint32, Flag);
	DECLARE_ENUM(uint16, DropRate);
	DECLARE_ENUM(uint16, Skill);
	DECLARE_BOOL(Quest);
	DECLARE_ENUM(uint16, QuestType);
	DECLARE_ENUM(uint16, QuestIndex);
	DECLARE_ENUM(int16, MaxCount);
	DECLARE_ENUM(uint8, AncientAttribute);
	DECLARE_ENUM(uint8, Kind1);
	DECLARE_ENUM(uint8, Kind2);
	DECLARE_ENUM(uint8, Kind3);
	DECLARE_ENUM(uint16, Value);
	DECLARE_ENUM(uint32, BuyPrice);
	DECLARE_ENUM(uint32, PriceFlags);
	DECLARE_ENUM(int32, Level);
	DECLARE_ENUM(uint8, Durability);
	DECLARE_ENUM(uint8, DurabilityMagic);
	DECLARE_ENUM(int32, DurabilityResistance);
	DECLARE_ENUM(int32, DurabilityTime);
	DECLARE_ENUM(int32, DurabilityTimeSafe);
	DECLARE_ENUM(uint8, DurabilityReduction);
	DECLARE_ENUM(int16, RequiredLevel);
	DECLARE_PROPERTY_ARRAY(uint32, RequiredStat, MAX_STAT_TYPE);
	DECLARE_ENUM(uint32, MinDamage);
	DECLARE_ENUM(uint32, MaxDamage);
	DECLARE_ENUM(uint32, Power);
	DECLARE_ENUM(uint32, AttackSpeed);
	DECLARE_ENUM(uint32, WalkSpeed);
	DECLARE_ENUM(uint32, Defense);
	DECLARE_ENUM(uint32, DefenseMagic);
	DECLARE_ENUM(uint32, DefenseSuccess);
	DECLARE_PROPERTY_ARRAY(uint8, Resistance, Element::MAX);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredClass, Character::MAX_CLASS);
	DECLARE_ENUM(int16, HarmonySmeltLevel);
	DECLARE_ENUM(uint32, UseDelay);
	DECLARE_ENUM(uint32, DurationTime);
	DECLARE_ENUM(uint32, LootingTime);
	DECLARE_BOOL(Disabled);
	DECLARE_BOOL(NormalLevel);
	DECLARE_ENUM(uint8, MaxSocket);
	DECLARE_STRUCT(item_stack, StackData);
	DECLARE_ENUM(uint32, RuudPrice);
	DECLARE_ENUM(uint8, InventoryType);
	
	ItemLevelTemplateMap level_data;

	bool is_class_allowed(uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const
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

	bool IsExclusiveClass(uint8 race) const
	{
		for ( int32 i = 0; i < Character::MAX_CLASS; ++i )
		{
			if ( this->GetRequiredClass(i) != 0 && i != race )
				return false;
		}
		
		return true;
	}

	uint16 GetItem() const { return ITEMGET(this->GetType(), this->GetIndex()); }

	bool HasPriceFlag(uint32 flag, uint8 level) const { return this->GetPriceFlag(level) & flag; }

	const char* GetName(uint8 level) const
	{
		ItemLevelTemplateMap::const_iterator it = this->level_data.find(level);

		if ( it != this->level_data.end() )
		{
			return it->second->GetName();
		}
		else
		{
			return this->GetName();
		}
	}

	uint32 GetBuyPrice(uint8 level) const
	{
		ItemLevelTemplateMap::const_iterator it = this->level_data.find(level);

		if ( it != this->level_data.end() )
		{
			return it->second->GetPrice();
		}
		else
		{
			return this->GetBuyPrice();
		}
	}

	uint32 GetPriceFlag(uint8 level) const
	{
		ItemLevelTemplateMap::const_iterator it = this->level_data.find(level);

		if ( it != this->level_data.end() )
		{
			return it->second->GetPriceFlag();
		}
		else
		{
			return this->GetPriceFlags();
		}
	}

	uint32 GetFlags(uint8 level) const
	{
		ItemLevelTemplateMap::const_iterator it = this->level_data.find(level);

		if ( it != this->level_data.end() )
		{
			return it->second->GetFlags();
		}
		else
		{
			return this->GetFlag();
		}
	}

	bool IsLevelData(uint8 level) const
	{
		ItemLevelTemplateMap::const_iterator it = this->level_data.find(level);

		return it != this->level_data.end();
	}

	uint8 GetExcellentType() const
	{
		if (this->GetKind1() == ItemKind1::WEAPON || this->GetKind1() == ItemKind1::PENDANT)
		{
			return 1;
		}

		if (this->GetKind1() == ItemKind1::ARMOR || this->GetKind1() == ItemKind1::RING)
		{
			return 0;
		}

		if (this->GetKind2() == ItemKind::WINGS_LVL_2 || this->GetKind2() == ItemKind::LORD_CAPE || this->GetKind2() == ItemKind::CLOAK_LVL_2)
		{
			return 2;
		}

		if (this->GetKind2() == ItemKind::WINGS_LVL_3)
		{
			return 3;
		}

		if (this->GetKind2() == ItemKind::MONSTER_WINGS)
		{
			return 4;
		}

		if (this->GetKind2() == ItemKind::WINGS_ANGEL_AND_DEVIL)
		{
			return 5;
		}

		if (this->GetKind1() == ItemKind1::MASTERY_ARMOR_1)
		{
			return 6;
		}

		if (this->GetKind3() == ItemKind3::BLESSED_ARCHANGEL_WEAPON || this->GetKind1() == ItemKind1::MASTERY_WEAPON)
		{
			return 7;
		}

		if (this->GetKind1() == ItemKind1::MASTERY_ARMOR_2)
		{
			return 8;
		}

		if (this->GetKind2() == ItemKind::WINGS_POWER)
		{
			return 9;
		}

		return -1;
	}

	bool IsExcellent() const
	{
		return this->GetKind3() == ItemKind3::COMMON || this->GetKind3() == ItemKind3::SOCKET || this->GetKind3() == ItemKind3::PVP || this->GetKind3() == ItemKind3::CHAOS_WEAPON;
	}

	bool IsMasteryEquipment() const
	{
		return this->GetKind1() == ItemKind1::MASTERY_ARMOR_1 || this->GetKind1() == ItemKind1::MASTERY_ARMOR_2 || this->GetKind1() == ItemKind1::MASTERY_WEAPON;
	}

	bool IsMasteryWeaponDK() const
	{
		return (this->GetKind1() == ItemKind1::MASTERY_WEAPON || ((this->GetKind1() == ItemKind1::MASTERY_ARMOR_1 || this->GetKind1() == ItemKind1::MASTERY_ARMOR_2) && this->GetKind2() == ItemKind::SHIELD)) && this->GetRequiredClass(Character::DARK_KNIGHT) != 0;
	}
};

struct item_set_type
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_PROPERTY_ARRAY(uint8, AncientID, MAX_ITEM_ANCIENT_TYPE);

	uint16 GetItem() const { return ITEMGET(this->GetType(), this->GetIndex()); }
};

struct item_set_option
{
	DECLARE_ENUM(uint8, GUID);
	DECLARE_STRING_FIXED(Name, MAX_ITEM_SET_OPTION_NAME_LENGTH);
	DECLARE_PROPERTY(int32, OptionCount);
	uint8 option_table[MAX_ITEM_ANCIENT_OPTION][2];
	int32 option_table_value[MAX_ITEM_ANCIENT_OPTION][2];
	DECLARE_PROPERTY_ARRAY(uint8, ExtraOptionTable, 2);
	DECLARE_PROPERTY_ARRAY(int32, ExtraOptionTableValue, 2);
	DECLARE_PROPERTY_ARRAY(uint8, FullOptionTable, MAX_ITEM_ANCIENT_OPTION_FULL);
	DECLARE_PROPERTY_ARRAY(int32, FullOptionTableValue, MAX_ITEM_ANCIENT_OPTION_FULL);
	DECLARE_BOOL(AncientHeroSoul);
	DECLARE_BOOL(Disassemble);
};

struct item_set_values
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_PROPERTY_ARRAY(uint16, Value, MAX_ITEM_LEVEL + 1);
};

struct item_harmony_data
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, Option);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, Rate);
	DECLARE_ENUM(uint8, MinLevel);
	DECLARE_PROPERTY_ARRAY(int32, Value, MAX_ITEM_HARMONY_LEVEL);
	DECLARE_PROPERTY_ARRAY(int32, RequiredZen, MAX_ITEM_HARMONY_LEVEL);
};

struct item_socket_seed
{
	DECLARE_ENUM(uint8, GUID);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, SubType);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, ValueType);
	DECLARE_PROPERTY_ARRAY(uint32, Level, MAX_SOCKET_LEVEL);
};

struct item_socket_bonus
{
	DECLARE_ENUM(uint8, GUID);
	DECLARE_ENUM(uint8, StartItemType);
	DECLARE_ENUM(uint8, EndItemType);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, ValueType);
	DECLARE_ENUM(uint32, Value);
	DECLARE_ENUM(uint8, SuccessRate);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredSocket, MAX_SOCKET_SLOT);
	DECLARE_ENUM(uint8, SeedLevelMin);
	DECLARE_ENUM(uint8, SeedLevelMax);
};

struct item_socket_package
{
	DECLARE_ENUM(uint8, GUID);
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, ValueType);
	DECLARE_ENUM(uint32, Value);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredType, MAX_SOCKET_TYPE);
};

struct item_socket_mix
{
	DECLARE_ENUM(uint8, Rank);
	DECLARE_ENUM(uint32, Price);
	DECLARE_ENUM(uint8, SuccessRate);
	DECLARE_ENUM(uint32, RequiredRune);
};

struct item_socket_upgrade
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, LevelMin);
	DECLARE_ENUM(uint8, LevelMax);
	DECLARE_ENUM(uint8, Skill);
	DECLARE_ENUM(uint8, Option);
	DECLARE_ENUM(uint8, SeedSphere);
	DECLARE_ENUM(uint8, SocketMin);
	DECLARE_ENUM(uint32, Price);
	DECLARE_ENUM(uint16, Rate);
	DECLARE_ENUM(uint8, UpgradeType);
	DECLARE_ENUM(uint16, UpgradeIndex);
	DECLARE_ENUM(uint8, UpgradeLevel);
	DECLARE_ENUM(uint16, UpgradeSkillRate);
	DECLARE_ENUM(uint16, UpgradeLuckRate);
	DECLARE_PROPERTY_ARRAY(uint16, UpgradeSocketRate, MAX_SOCKET_SLOT);

	uint16 GetItem() const { return ITEMGET(this->GetType(), this->GetIndex()); };
	uint16 GetUpgradeItem() const { return ITEMGET(this->GetUpgradeType(), this->GetUpgradeIndex()); };
};

struct item_socket_seed_sphere
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(uint8, Attribute);
	DECLARE_ENUM(uint8, Rank);
	DECLARE_ENUM(uint16, NextID);
};

struct item_transformation
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint16, Skin);
	DECLARE_BOOL(Special);
};

struct item_drop_data
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_RANGE(uint8, Level);
	DECLARE_ENUM(uint8, Durability);
	DECLARE_ENUM(uint8, Skill);
	DECLARE_ENUM(uint8, Luck);
	DECLARE_RANGE(uint8, Option);
	DECLARE_RANGE(uint8, Excellent);
	DECLARE_ENUM(uint8, Ancient);
	DECLARE_PROPERTY_ARRAY(uint16, Socket, MAX_SOCKET_SLOT);
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(uint16, Monster);
	DECLARE_ENUM(int16, MonsterLevelMin);
	DECLARE_ENUM(int16, MonsterLevelMax);
	DECLARE_ENUM(uint8, EventID);
	DECLARE_ENUM(uint32, InvasionID);
	DECLARE_ENUM(uint8, SeasonEvent);
	DECLARE_ENUM(uint16, Rate);
	
	uint16 GetItem() const { return ITEMGET(this->GetType(), this->GetIndex()); }
};

struct item_mix_lucky_data
{
	DECLARE_ENUM(uint8, TicketType);
	DECLARE_ENUM(uint16, TicketIndex);
	DECLARE_ENUM(uint8, TicketLevel);
	DECLARE_ENUM(uint8, ItemType);
	DECLARE_ENUM(uint16, ItemIndex);

	uint16 GetTicket() const { return ITEMGET(this->GetTicketType(), this->GetTicketIndex()); }
	uint16 GetItem() const { return ITEMGET(this->GetItemType(), this->GetItemIndex()); }
};

struct item_price_data
{
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(int64, BuyPrice);
	DECLARE_ENUM(int64, SellPrice);
};

struct item_add_option
{
	DECLARE_ENUM(uint16, Item);
	DECLARE_PROPERTY_ARRAY(uint16, Effect, MAX_ITEM_ADD_OPTION_EFFECT);
	DECLARE_PROPERTY_ARRAY(uint16, Value, MAX_ITEM_ADD_OPTION_EFFECT);
};

struct item_wing_values
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(int32, DamageAdd);
	DECLARE_ENUM(int32, DamageAddLvl);
	DECLARE_ENUM(int32, AbsorbAdd);
	DECLARE_ENUM(int32, AbsorbAddLvl);
};

struct item_reduced_drop
{
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint16, Rate);
};

struct item_create_data
{
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
};

struct item_flagdb
{
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint32, Flags);
};

struct item_wing_graded_option
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_ARRAY(int32, Value, MAX_WING_GRADED_OPTION_LEVEL);
};

struct item_wing_socket_option
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(int32, Rate);
	DECLARE_PROPERTY_ARRAY(int32, Value, MAX_WING_SOCKET_OPTION_LEVEL);
};

struct item_wing_socket_upgrade
{
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(int32, Rate);
	DECLARE_PROPERTY_ARRAY(uint8, ItemType, MAX_WING_SOCKET_OPTION_UPGRADE_ITEM);
	DECLARE_PROPERTY_ARRAY(uint16, ItemIndex, MAX_WING_SOCKET_OPTION_UPGRADE_ITEM);
	DECLARE_PROPERTY_ARRAY(int32, ItemCount, MAX_WING_SOCKET_OPTION_UPGRADE_ITEM);
	DECLARE_ENUM(int32, Zen);

	uint16 GetItem(int32 slot) const { return ITEMGET(this->GetItemType(slot), this->GetItemIndex(slot)); }
};

struct item_earring
{
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Group);
	DECLARE_ENUM(uint8, Link);
	DECLARE_PROPERTY_ARRAY(uint8, Option, MAX_EARRING_OPTION);
	DECLARE_PROPERTY_ARRAY(uint8, Value, MAX_EARRING_OPTION);
	DECLARE_PROPERTY_ARRAY(uint8, Rate, MAX_SOCKET_SLOT);
};

struct item_earring_option
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, Type);
};

struct item_earring_option_value
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, Group);
	DECLARE_ENUM(int32, Value);
};

struct item_earring_bonus
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, Link1);
	DECLARE_ENUM(uint8, Link2);
	DECLARE_PROPERTY_ARRAY(uint8, Option, MAX_EARRING_BONUS);
	DECLARE_PROPERTY_ARRAY(uint8, Value, MAX_EARRING_BONUS);
};

struct item_earring_upgrade
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(int32, Rate);
	DECLARE_PROPERTY_ARRAY(uint16, Item, MAX_EARRING_UPGRADE_MATERIAL);
	DECLARE_PROPERTY_ARRAY(int32, Amount, MAX_EARRING_UPGRADE_MATERIAL);
	DECLARE_ENUM(int32, Price);
};

struct item_mastery_bonus
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(int32, Value);
	DECLARE_ENUM(uint16, Rate);
};

struct item_mysterious_stone
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, Luck);
	DECLARE_ENUM(uint8, Excellent);
	DECLARE_ENUM(uint8, Ancient);
	DECLARE_ENUM(uint16, Rate);
};

struct item_guardian_option
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_STRING(Name);
};

struct item_guardian_option_value
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, FormulaData);
	DECLARE_ENUM(uint8, ValueType);
	DECLARE_ENUM(int32, Condition);
};

struct item_guardian_elite_option
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_STRING(Name);
};

struct item_guardian_elite_option_value
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, Group);
	DECLARE_PROPERTY_ARRAY(uint8, ID, MAX_GUARDIAN_ELITE_OPTIONS);
	DECLARE_PROPERTY_ARRAY(uint8, FormulaData, MAX_GUARDIAN_ELITE_OPTIONS);
	DECLARE_PROPERTY_ARRAY(uint8, ValueType, MAX_GUARDIAN_ELITE_OPTIONS);
};

struct HarmonyOption
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_PROPERTY_ARRAY(int32, Value, MAX_HARMONY_ENHANCEMENT_LEVEL);
	DECLARE_ENUM(uint8, SeparationValue);
};

class SerialCreateData;

typedef std::map<uint16, item_template*> ItemTemplateMap;
typedef std::map<uint8, item_set_option*> ItemSetOptionMap;
typedef std::map<uint16, item_set_type*> ItemSetTypeMap;
typedef std::vector<item_harmony_data*> ItemHarmonyDataList;
typedef std::map<uint8, item_socket_seed*> ItemSocketSeedMap;
typedef std::map<uint8, item_socket_bonus*> ItemSocketBonusMap;
typedef std::map<uint8, item_socket_package*> ItemSocketPackageMap;
typedef std::vector<item_transformation*> ItemTransformationList;
typedef std::vector<item_drop_data*> ItemDropList;
typedef std::vector<item_mix_lucky_data*> ItemMixLuckyList;
typedef std::vector<item_price_data*> ItemPriceDataList;
typedef std::map<uint16, item_add_option*> ItemAddOptionMap;
typedef std::map<uint16, item_wing_values*> ItemWingValuesMap;
typedef std::vector<item_reduced_drop*> ItemReducedDropList;
typedef std::vector<item_create_data*> ItemCreateDataList;
typedef std::map<uint8, item_socket_mix*> ItemSocketMixMap;
typedef std::map<uint8, item_socket_upgrade*> ItemSocketUpgradeMap;
typedef std::map<uint16, item_socket_seed_sphere*> ItemSocketSeedSphereMap;
typedef std::map<uint16, item_flagdb*> ItemFlagDBMap;
typedef std::map<uint16, item_set_values*> ItemSetValuesMap;
typedef std::map<uint8, item_wing_graded_option*> ItemWingGradedOptionMap;
typedef std::vector<item_wing_socket_option*> ItemWingSocketOptionList;
typedef std::map<uint8, item_wing_socket_upgrade*> ItemWingSocketUpgradeMap;
typedef std::map<uint16, item_earring*> ItemEarringMap;
typedef std::map<uint8, item_earring_option*> ItemEarringOptionMap;
typedef std::vector<item_earring_option_value*> ItemEarringOptionValueList;
typedef std::map<uint8, item_earring_bonus*> ItemEarringBonusMap;
typedef std::map<uint8, item_earring_upgrade*> ItemEarringUpgradeMap;
typedef std::vector<item_mastery_bonus*> ItemMasteryBonusList;
typedef std::vector<item_mysterious_stone*> ItemMysteriousStoneList;
typedef std::vector<item_guardian_option*> ItemGuardianOptionList;
typedef std::vector<item_guardian_option_value*> ItemGuardianOptionValueList;
typedef std::vector<item_guardian_elite_option*> ItemGuardianEliteOptionList;
typedef std::vector<item_guardian_elite_option_value*> ItemGuardianEliteOptionValueList;
typedef std::vector<HarmonyOption*> HarmonyOptionList;

uint8 GetRandomExcOption(uint8 max_options);
void GetRandomExcOption(uint8 & exe, uint8 max_options);

class CItemMgr
{
	SingletonInstance(CItemMgr);

	private:
		ItemTemplateMap m_item_template;
		ItemSetTypeMap m_item_set_type;
		ItemSetOptionMap item_set_option_map;
		ItemHarmonyDataList m_item_harmony_list[HARMONY_ITEM_MAX];
		ItemSocketSeedMap m_item_socket_seed;
		ItemSocketBonusMap m_item_socket_bonus;
		ItemSocketPackageMap m_item_socket_package;
		ItemTransformationList m_item_transformation;
		ItemDropList m_item_drop;
		ItemMixLuckyList m_item_mix_lucky;
		ItemPriceDataList m_item_price_data_list;
		ItemAddOptionMap m_item_add_option;
		ItemWingValuesMap m_item_wing_values;
		ItemReducedDropList m_item_reduced_drop_list;
		ItemCreateDataList m_item_create_data_list;
		ItemSocketMixMap m_item_socket_mix;
		ItemSocketUpgradeMap m_item_socket_upgrade_map;
		ItemSocketSeedSphereMap m_item_socket_seed_sphere_map;
		ItemFlagDBMap m_item_flagdb_map;
		ItemSetValuesMap m_ItemSetValuesMap;
		ItemWingGradedOptionMap m_ItemWingGradedOptionMap;
		ItemWingSocketOptionList m_ItemWingSocketOptionList;
		ItemWingSocketUpgradeMap m_ItemWingSocketUpgradeMap;
		ItemEarringMap m_ItemEarringMap;
		ItemEarringOptionMap m_ItemEarringOptionMap;
		ItemEarringOptionValueList m_ItemEarringOptionValueList;
		ItemEarringBonusMap m_ItemEarringBonusMap;
		ItemEarringUpgradeMap m_ItemEarringUpgradeMap;
		ItemMasteryBonusList m_ItemMasteryBonusList;
		ItemMysteriousStoneList m_ItemMysteriousStoneList;
		ItemGuardianOptionList m_ItemGuardianOptionList;
		ItemGuardianOptionValueList m_ItemGuardianOptionValueList;
		ItemGuardianEliteOptionList m_ItemGuardianEliteOptionList;
		ItemGuardianEliteOptionValueList m_ItemGuardianEliteOptionValueList;
		HarmonyOptionList m_HarmonyOptionList;
		DECLARE_ENUM(time_t, LastItemUpdate);
	
		void _insert_serial(SerialCreateData* add_serial);

		void _serial_create_map(SerialCreateData* data);
		void _serial_create_inventory(SerialCreateData* data);
		void _serial_create_chaos_box(SerialCreateData* data);
		void _serial_create_warehouse(SerialCreateData* data);
		void _serial_create_shop(SerialCreateData* data);
		void _serial_create_ruud_shop(SerialCreateData* data);
		void _serial_create_cash_shop(SerialCreateData* data);
		void _serial_create_pentagram_mix(SerialCreateData* data);
		void _serial_create_gremory_case(SerialCreateData* data);
		void _serial_create_inventory_box(SerialCreateData* data);
		void _serial_create_socket_mix(SerialCreateData* data);

	public:
		CItemMgr();
		virtual ~CItemMgr();

		void LoadItemTemplate();
		void LoadItemAddOption();
		void LoadItemSetType();
		void LoadItemSetOption();
		void LoadItemHarmony();
		void LoadItemHarmonyEnhancement();
		void LoadItemSocket();
		void LoadItemSocketSeed();
		void LoadItemSocketBonus();
		void LoadItemSocketPackage();
		void LoadItemSocketMix();
		void LoadItemSocketUpgrade();
		void LoadItemSocketSeedSphere();
		void LoadItemTransformation();
		void LoadItemDrop();
		void LoadItemLevelTemplate();
		void LoadItemMixLucky();
		void LoadItemPriceData();
		void LoadItemStack();
		void LoadItemQuest();
		void LoadItemPriceRuud();
		void LoadItemWingValues();
		void LoadItemReducedDrop();
		void LoadItemCreateData();
		void LoadItemFlagDB();
		void LoadItemWingGradedOption();
		void LoadItemWingSocketOption();
		void LoadItemWingSocketUpgrade();
		void LoadEarring();
		void LoadEarringOption();
		void LoadEarringOptionValue();
		void LoadEarringBonus();
		void LoadEarringUpgrade();
		void LoadMasteryBonus();
		void LoadMysteriousStone();
		void LoadGuardianOption();
		void LoadGuardianOptionValue();
		void LoadGuardianEliteOption();
		void LoadGuardianEliteOptionValue();

		bool IsItem(uint16 item) const;

		uint8 LevelSmallConvert(int32 level);
		item_harmony_data * GetRandomHarmonyOption(Item * item);

		void ConvertToBuffer(uint8 * buffer, Item const& item);
		void ConvertToBuffer(uint8 * buffer, Item const* item);
		item_template const* GetItem(uint16 id) const;
		item_template * GetItem(uint16 id);
		item_add_option const* GetItemAddOption(uint16 item) const;
		item_set_type const* GetItemSetType(uint16 item) const;
		item_set_option const* GetItemSetOption(uint8 guid) const;
		item_harmony_data * GetHarmony(uint8 type, uint8 option) const;
		item_socket_seed * GetSocketSeed(uint8 guid) const;
		item_socket_bonus * GetSocketBonus(uint8 guid) const;
		uint8 GetSocketBonus(Item* pItem, uint8 seed_level) const;
		bool CheckSocketBonus(Item* pItem, item_socket_bonus const* pBonus) const;
		item_socket_mix const* GetSocketMix(uint8 rank) const;
		item_socket_upgrade const* GetSocketUpgrade(uint8 id) const;
		item_socket_upgrade const* GetSocketUpgrade(Item const* pItem) const;
		item_socket_seed_sphere const* GetSocketSeedSphere(uint16 item) const;
		uint16 GetSocketSeedSphere(uint8 type, uint8 level) const;
		void GetSocketPackage(Player* pPlayer, uint8 * socket_count);
		SocketDataType GetSocketByteOption(uint16 seed, uint8 level) const;
		uint8 GetRandomOptionAncient(uint16 item);
		item_transformation * GetItemTransformation(uint16 item, uint8 level) const;
		bool IsUsingTransformationItem(Player* pPlayer);
		bool RandomItemDrop(Monster* pMonster);
		bool IsItemMixLucky(uint16 item, uint8 level) const;
		item_mix_lucky_data * GetRandomItemMixLucky(uint16 item, uint8 level, Player* pPlayer);
		bool IsLuckyItem(uint16 item) const;
		bool GetRandomSetPart(uint16 index, uint16 & item);
		bool GetRandomAncientSetPart(uint8 guid, uint16 & item, uint8 & ancient);
		float CalculateDurability(uint16 id, uint8 level, uint8 exe, uint8 ancient, item_template const* item_info = nullptr);

		void GetRandomSocketSeed(uint16 & item, uint8 & level);

		bool IsItemFlag(uint16 item, uint8 level, uint32 flag) const;
		
	#define option_serial_h uint8 skill = 0, uint8 luck = 0, uint8 option = 0, uint8 exe = 0, uint8 ancient = 0, uint8 * socket = nullptr
	#define option_serial uint8 skill, uint8 luck, uint8 option, uint8 exe, uint8 ancient, uint8 * socket

		void ItemSerialCreate(Unit* owner, uint16 world, int16 x, int16 y, Item const& item, bool only_owner, bool visible = true, bool to_queue = false);
		void ItemSerialCreateItem(Unit* owner, SerialCreateType type, Item const& item, uint8 slot = -1, uint8 create_type = 0);
		void ItemSerialCreateShop(Unit* owner, int32 shop, uint8 slot);
		void ItemSerialCreateRuudShop(Unit* owner, int32 shop, uint8 slot);
		void ItemSerialCreateCashShop(Unit* owner, uint32 package, uint32 option, uint32 product, uint32 account_id);
		void ItemSerialCreateGremoryCase(Unit* owner, Item const& item, uint8 inventory, uint8 type, uint8 recovery = 0);
	
		void _finally_create_item(SerialCreateData* data);

		uint8 GetItemMaxStack(uint16 item) const;

		ItemTemplateMap const& GetItemMap() const { return this->m_item_template; };

		int32 GetWingDamage(uint16 item, uint8 level) const;
		int32 GetWingAbsorb(uint16 item, uint8 level) const;

		bool IsItemDrop(uint16 item, uint8 level) const;
		bool IsItemCreateData(uint16 item, uint8 level) const;

		bool IsSeedSphere(uint16 item);

		uint8 GetItemInventoryType(uint16 item) const;

		uint32 GetItemFlagDB(uint16 item) const;

		int32 GetItemSetValue(uint16 option, uint8 level) const;

		int32 GetWingGradedOptionValue(uint8 id, uint8 level) const;
		SocketDataType GenerateWingGradedOption(SocketDataType const* socket) const;

		int32 GetWingSocketOptionValue(uint8 type, uint8 id, uint8 level) const;
		SocketDataType GenerateWingSocketOption(uint8 type) const;

		item_wing_socket_upgrade const* GetWingSocketUpgrade(uint8 level) const;

		item_earring const* GetEarringData(uint16 item) const;
		item_earring_option const* GetEarringOption(uint8 id) const;
		item_earring_option_value const* GetEarringOptionValue(uint8 id, uint8 group) const;
		item_earring_bonus const* GetEarringBonus(uint8 link_1, uint8 link_2) const;
		item_earring_upgrade const* GetEarringUpgrade(uint8 level) const;

		void GenerateEarringOptions(Item* pItem, int32 min_option);

		bool IsMasteryBonusMax(uint8 type, uint8 id) const;
		uint8 GetRandomMasteryBonus(uint8 type) const;
		item_mastery_bonus const* GetMasteryBonus(uint8 type, uint8 id) const;

		bool IsMysteriousStoneMax(uint8 type, Item const* pItem) const;
		item_mysterious_stone const* GetRandomMysteriousStone(uint8 type) const;

		void AddSetOption(uint16 id);

		item_guardian_option_value const* GetGuardianOptionValue(uint8 type, uint8 id) const;
		item_guardian_elite_option_value const* GetGuardianEliteOptionValue(uint8 type, uint8 id) const;

		void GenerateGuardianOptions(Item* pItem, int32 min_options = 1);

		ItemGuardianOptionValueList const& GetGuardianOptionValueList() const { return this->m_ItemGuardianOptionValueList; }
		ItemGuardianEliteOptionValueList const& GetGuardianEliteOptionValueList() const { return this->m_ItemGuardianEliteOptionValueList; }
};

#endif