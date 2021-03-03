/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Item.h"
*
*/

#ifndef ITEM_H
#define ITEM_H

class Item
{
	public:

		DECLARE_ENUM(uint16, Item);
		DECLARE_PROPERTY(uint8, Level);
		DECLARE_ENUM(float, Durability);
		DECLARE_FLOAT(DurabilityState);
		DECLARE_FLOAT(DurabilityBase);
		DECLARE_FLOAT(DurabilityResistance);
		DECLARE_FLOAT(DurabilityCurrentStatus);
		DECLARE_PROPERTY_ARRAY(float, DurabilityStatus, MAX_ITEM_DURABILITY_STATE);
		DECLARE_FLOAT(DurabilityOriginal);
		DECLARE_STRUCT(TickTimer, DurabilityTime);
		DECLARE_ENUM(int32, DurabilityTimeRequired);
		DECLARE_ENUM(int32, DurabilityTimeRequiredSafe);
		DECLARE_ENUM(uint8, DurabilityReduction);
		DECLARE_ENUM(uint8, Skill);
		DECLARE_ENUM(uint16, SkillID);
		DECLARE_ENUM(uint8, Luck);
		DECLARE_ENUM(uint8, Option);
		DECLARE_PROPERTY(uint8, Exe);
		DECLARE_PROPERTY(uint8, Ancient);
		DECLARE_PROPERTY(uint8, Harmony);
		DECLARE_PROPERTY(uint8, 380);
		DECLARE_PROPERTY_ARRAY(SocketDataType, Socket, MAX_SOCKET_SLOT);
		DECLARE_PROPERTY(uint8, SocketBonus);

		DECLARE_ENUM(uint16, SerialServer);
		DECLARE_ENUM(uint32, Serial);
		DECLARE_ENUM(uint32, SerialShop);

		union
		{
			struct
			{
				int32 level;
				int32 experience;
			} pet_dark;

			struct
			{
				int32 ammount;
			} zen;

			struct
			{
				int32 fix_count;
			} dark_ancient;

			struct
			{
				int32 event_id;
				int32 event_ground;
			} bc_weapon;

			struct
			{
				int32 talisman;
				int32 attempts;
				int32 time;
			} common;

			struct
			{
				int32 data_extra[3];
			} raw;
		};

		DECLARE_FLAG(uint32, DBFlag);

		DECLARE_BOOL(QuestItem);

		DECLARE_ENUM(uint32, BuyPrice);
		DECLARE_ENUM(uint32, SellPrice);
		DECLARE_ENUM(uint32, OldPrice);

		DECLARE_ENUM(uint16, Value);

		DECLARE_PROPERTY(int16, RequiredLevel);
		DECLARE_PROPERTY_ARRAY(int32, StatRequired, MAX_STAT_TYPE);
		DECLARE_PROPERTY_ARRAY(int32, StatDecrease, MAX_STAT_TYPE);
		DECLARE_PROPERTY_ARRAY(uint8, RequiredClass, Character::MAX_CLASS);
		DECLARE_PROPERTY_ARRAY(uint8, Resistance, Element::MAX);

		DECLARE_BOOL(Excellent);
		DECLARE_ENUM(uint8, ExcellentType);

		DECLARE_PROPERTY(int32, Defense);
		DECLARE_PROPERTY(int32, DefenseMagic);
		DECLARE_PROPERTY(int32, DefenseSuccessRate);
		DECLARE_ENUM(int32, DefenseOriginal);

		DECLARE_PROPERTY(uint32, MinDamage);
		DECLARE_PROPERTY(uint32, MaxDamage);
		
		DECLARE_ENUM(uint8, AncientAttribute);
		DECLARE_ENUM(uint8, AncientAttributeValue);

		DECLARE_BOOL(Valid);

		DECLARE_PROPERTY(uint32, MagicPower);
		DECLARE_PROPERTY(uint32, CursePower);
		DECLARE_PROPERTY(uint32, PetPower);

		DECLARE_BOOL(TwoHand);
		DECLARE_ENUM(uint8, Slot);

		DECLARE_ENUM(uint8, Kind1);
		DECLARE_ENUM(uint8, Kind2);
		DECLARE_ENUM(uint8, Kind3);

		DECLARE_ENUM(uint32, PersonalStorePrice);
		DECLARE_ENUM(uint16, PersonalStorePriceJoB);
		DECLARE_ENUM(uint16, PersonalStorePriceJoS);

		DECLARE_STRING_FIXED(Name, MAX_ITEM_NAME_LENGTH);

		DECLARE_ENUM(uint32, AttackSpeed);
		DECLARE_FLAG(uint32, Flag);
		DECLARE_ENUM(uint32, UseDelay);
		DECLARE_ENUM(uint32, DurationTime);
		DECLARE_ENUM(uint32, LootingTime);
		DECLARE_BOOL(ForceUpgrade);
		DECLARE_ENUM(uint32, Owner);
		DECLARE_ENUM(uint32, OwnerSlot);
		DECLARE_ENUM(ItemBox, Box);
		DECLARE_ENUM(ItemState, State);
		DECLARE_ENUM(time_t, ExpireDate);
		DECLARE_PROPERTY(int32, ExpireNotify);
		DECLARE_PROPERTY(int32, SocketDurabilityReduction);

		DECLARE_BOOL(NormalLevel);

		DECLARE_ENUM(uint8, MaxSocket);

		DECLARE_BOOL(InventoryActive);

		DECLARE_BOOL(ExpiredStatus);

	public:
		Item();
		~Item();
		Item(uint16 item, uint8 level = 0, float durability = 0, uint8 skill = 0, uint8 luck = 0, uint8 option = 0, uint8 exe = 0, uint8 ancient = 0, SocketDataType const* socket = nullptr, uint8 socket_bonus = uint8(-1), uint32 expire_time = 0);

		void clear(bool all = true);
		void Set(Item const& item);
		void Set(Item const* pItem);
		bool IsSameItem(Item const& item, bool price_check = false) const;
		bool IsSameItem(Item const* pItem, bool price_check = false) const;
						
		bool IsItem() const { return this->GetItem() != INVALID_ITEM; }
		uint8 GetItemType() const;
		uint16 GetItemIndex() const;
		uint8 GetAncientType() const
		{ 
			uint8 ancient_id = IS_ANCIENT(this->GetAncient()); 

			if ( ancient_id != ANCIENT_TYPE_1 && ancient_id != ANCIENT_TYPE_2 && ancient_id != ANCIENT_TYPE_3 )
			{
				return 0;
			}

			return ancient_id == ANCIENT_TYPE_1 ? 1:
				   ancient_id == ANCIENT_TYPE_2 ? 2:
				   ancient_id == ANCIENT_TYPE_3 ? 3:
				   1;
		}

		uint8 GetAncientValue() const
		{
			if ((this->GetAncient() & ANCIENT_OPTION_3) == ANCIENT_OPTION_3)
			{
				return 3;
			}

			if ((this->GetAncient() & ANCIENT_OPTION_2) == ANCIENT_OPTION_2)
			{
				return 2;
			}

			if ((this->GetAncient() & ANCIENT_OPTION_1) == ANCIENT_OPTION_1)
			{
				return 1;
			}

			return 0;
		}

		bool IsAncient() const;
		bool IsSocket() const;
		bool IsHarmonyUpgrade() const;
		HarmonyItemType GetHarmonyItem() const;
		uint8 GetHarmonyOption() const;
		uint8 GetHarmonyLevel() const;
		bool Is380Option() const;
		bool IsJewel() const;
		uint8 GetSocketCount() const;
		uint8 GetActiveSocketCount() const;

		bool IsPentagramItem() const;
		bool IsPentagramMithril() const;
		bool IsMuunItem() const;
		bool IsMuunUtil() const;

		bool IsBlessedArchangelWeapon() const;
		bool IsArchangelWeapon() const;
		bool IsMysteriousStoneItem() const;
		bool IsEarring() const;
		bool IsMount() const;

		bool IsValidElementalAttribute() const;
		
		bool SocketAllowed(uint8 option) const;
		static uint8 GetSocketType(uint8 option);

		bool IsRequiredClass(uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const;
		int32 GetRequiredStat(uint8 stat) const;

		bool IsExclusiveClass(uint8 race) const
		{
			for ( int32 i = 0; i < Character::MAX_CLASS; ++i )
			{
				if ( this->GetRequiredClass(i) != 0 && i != race )
					return false;
			}
		
			return true;
		}

		void AlterDurability(uint8 type, float count);

		void ConvertToBuffer(uint8 * buffer) const;

		void Convert(bool update = false);

		void CalculatePrice();
		void CalculateOldPrice();
		void FixPrice();
		int32 GetRepairZen(uint8 repair_way);

		bool IsWingLvl1() const;
		bool IsWingLvl2() const;
		bool IsWingLvl3() const;
		bool IsWingMonster() const;
		bool IsMiniWing() const;
		bool IsSpecialWing() const;
		bool IsWingLvl4() const;
		bool IsWingType(uint8 type) const;
		bool IsSocketPendant() const;
				
		bool GivePetExperience(int64 experience);
		bool PetLevelDown(int32 experience);
		bool PetExeDownPercent(int32 percent);
		void PetValue();

		int32 CriticalDamageRate() const;

		int32 FenrirIncLastDamage() const;
		int32 FenrirDecLastDamage() const;
		int32 FenrirIllusion() const;

		float GetMasteryShieldDefense() const;

		static bool FenrirIncLastDamage(uint8 exe);
		static bool FenrirDecLastDamage(uint8 exe);
		static bool FenrirIllusion(uint8 exe);

		void CalculateDurability();
		bool CheckDurabilityState();
		uint8 DurabilityDownSimple(float value);
		uint8 DurabilityDownValue(float value, float reduction);
		DurabilityState DurabilityDownArmor(float value, float reduction);
		DurabilityState DurabilityDownWeapon(int32 defense, float reduction);
		DurabilityState DurabilityDownStaff(int32 defense, float reduction);
		uint8 ProcessDurabilityTime(Player* pPlayer);
	
		bool IsTransformationRing(bool shop = true) const;
		bool IsEventArchangelWeapon() const { return IsEventArchangelWeapon(this->GetItem()); }
		static bool IsEventArchangelWeapon(uint16 Item) { return Item == ITEMGET(13, 19); }

		std::string BuildLog(uint32 slot) const;
		std::string BuildLogDB(uint32 slot, bool unique) const;
		std::string BuildRegister() const;
		std::string BuildRegisterDB() const;
		void SaveDB(SQLTransaction & trans, uint8 rules);
		void LoadDB(Field* fields, uint32 owner, uint32 slot, ItemBox box);
		bool IsExpired() const;
		time_t GetRemainTime() const;

		void BuildPentagramInfo(PentagramJewelInfo * pInfo, uint8 slot, uint8 type);

		void BuildPentagramAttribute(Unit* pUnit);

		void UpdateMixRecovery();

		OptionDataMap m_OptionData;

		void AddOptionData(uint16 id, uint16 value);
		uint16 GetOptionValue(uint16 id) const;
		bool HasOptionData(uint16 id) const;

		void ApplyOption(uint16 id, int32 & value, float mod) const;
	private:
		void CalculateDamage(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level, int32 chaos_weapon);
		void CalculatePower(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level, int32 chaos_weapon);
		void CalculateDefenseSuccessRate(item_template const* item_info, bool is_excellent, bool is_ancient);
		void CalculateDefense(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level);
		void CalculateDefenseMagic(item_template const* item_info);
		void CalculateRequiredStat(item_template const* item_info, bool is_excellent, bool is_ancient, int32 item_level);

		void CalculateOptionValue(item_template const* pItemInfo);
		void CalculateHarmonyOption();
		void CalculateSocketOption();
		uint32 GetSocketOption(uint8 opt);
};

class CPetItemExperience
{
	SingletonInstance(CPetItemExperience);

	public:
		uint32 m_DarkSpirit[MAX_PET_LEVEL+2];
		uint32 m_DarkHorse[MAX_PET_LEVEL+2];

	public:
		CPetItemExperience()
		{
			this->m_DarkSpirit[0] = 0;
			this->m_DarkSpirit[1] = 0;

			for ( uint32 i = 2; i < MAX_PET_LEVEL+2; ++i )
			{
				this->m_DarkSpirit[i] = (i+10) * i * i * i * 100;
			}

			this->m_DarkHorse[0] = 0;
			this->m_DarkHorse[1] = 0;

			for ( uint32 i = 2; i < MAX_PET_LEVEL+2; ++i )
			{
				this->m_DarkHorse[i] = (i+10) * i * i * i * 100;
			}
		};
};

class SerialCreateData
{
	public:
		explicit SerialCreateData(SerialCreateType type)
		{
			this->Clear();
			this->type.set(type);
		}

		virtual ~SerialCreateData()
		{
			this->Clear();
		}

		void Clear()
		{
			this->owner = nullptr;
			this->world = -1;
			this->type.set(serial_create_none);
			this->SetSlot(0);
			this->x.set(0);
			this->y.set(0);
			this->server = 0;
			this->serial = 0;
			this->serial_cash_shop = 0;
			this->only_owner = false;
			this->item.clear();

			for ( uint8 i = 0; i < 6; i++ )
				this->raw.data[i] = uint32(-1);

			this->SetVisible(true);
			this->SetToQueue(false);
			this->SetCreateType(0);
		}

		DECLARE(SerialCreateType, type);
		DECLARE_ENUM(uint8, Slot);
		DECLARE_ENUM(uint8, CreateType);

		class Unit * owner;
		uint16 world;
		DECLARE(int16, x);
		DECLARE(int16, y);

		uint16 server;
		uint32 serial;
		uint32 serial_cash_shop;
		bool only_owner;
		DECLARE_BOOL(Visible);
		DECLARE_BOOL(ToQueue);

		Item item;

		union
		{
			struct
			{
				uint32 package;
				uint32 option;
				uint32 product;
				uint32 account_id;
			} cash_shop;

			struct
			{
				uint32 id;
			} shop;

			struct
			{
				uint32 inventory;
				uint32 type;
				uint32 recovery;
			} gremory_case;

			struct
			{
				uint32 data[6];
			} raw;
		};
};

#endif