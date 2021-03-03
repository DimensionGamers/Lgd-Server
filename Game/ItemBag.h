/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ItemBag.h"
*
*/

#ifndef ITEM_BAG_H
#define ITEM_BAG_H

struct ItemBagItem
{
	DECLARE_ENUM(uint8, Sort);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_RANGE(uint8, Level);
	DECLARE_ENUM(uint8, Durability);
	DECLARE_ENUM(uint16, SkillRate);
	DECLARE_ENUM(uint16, LuckRate);
	DECLARE_RANGE(uint8, Option);
	DECLARE_RANGE(uint8, Exe);
	DECLARE_ENUM(uint8, Ancient);
	DECLARE_RANGE(uint8, AncientOption);
	DECLARE_PROPERTY_ARRAY(uint16, SocketRate, MAX_SOCKET_SLOT);
	DECLARE_ENUM(uint32, Rate);
	DECLARE_FLAG(uint8, Flag);
	DECLARE_ENUM(uint32, Group);
	DECLARE_ENUM(time_t, Duration);

	uint16 GetItem() const { return ITEMGET(this->GetType(), this->GetIndex()); }
};

typedef std::vector<ItemBagItem*> ItemBagItemList;
typedef std::map<uint32, ItemBagItemList> ItemBagItemGroup;

class ItemBag
{
	friend class ItemBagMgr;

	private:
		DECLARE_ENUM(uint32, GUID);
		DECLARE_ENUM(uint8, Type);
		DECLARE_PROPERTY_STRING(Name);

		union
		{
			struct
			{
				int32 type;
				int32 index;
				int32 level;
			} item;

			struct
			{
				int32 monster;
				int32 level_min;
				int32 level_max;
			} monster;

			struct
			{
				int32 monster;
			} monster_all;

			struct
			{
				int32 guid;
				int32 create_type;
				int32 sub_id;
			} misc;

			struct
			{
				int32 id;
				int32 index;
			} mix;

			struct
			{
				int32 id;
				int32 rate;
				int32 level_min;
			} world;

			struct
			{
				int32 id;
				int32 rate1;
				int32 rate2;
			} world2;

			struct
			{
				int32 monster;
				int32 world;
				int32 rate;
			} monster_rate;

			struct
			{
				int32 id;
				int32 ground;
				int32 rate;
			} event;

			struct
			{
				int32 id;
				int32 rate;
			} zone;

			struct
			{
				int32 data[3];
			} raw;
		};

		DECLARE(int16, required_level);

		DECLARE_ENUM(uint16, MoneyRate);
		DECLARE_RANGE(uint32, Money);
		DECLARE_ENUM(uint16, RuudRate);
		DECLARE_RANGE(uint32, Ruud);

		DECLARE(uint32, drop_min);
		DECLARE(uint32, drop_max);
		DECLARE_FLAG(uint8, Flag);
		DECLARE(int16, range_x);
		DECLARE(int16, range_y);

		ItemBagItemGroup items;

	public:
		ItemBag();
		virtual ~ItemBag();

		void Clear();

		uint16 GetItem() const { return ITEMGET(this->item.type,this->item.index); }
		int32 GetDropCount() const
		{
			if ( this->drop_max <= 0 )
				return 0;

			if ( this->drop_max < this->drop_min )
				return 0;

			return RAND%((this->drop_max.get() - this->drop_min.get()) + 1) + this->drop_min.get();
		}

		bool Valid() const
		{
			int32 drop_count = this->GetDropCount();

			if ( drop_count <= 0 )
				return false;

			if ( this->items.empty() && this->GetMoneyRate() == 0 && this->GetRuudRate() == 0 )
				return false;
			
			return true;
		}

		bool Execute(Unit* pUnit, int32 data_1, int32 data_2, int16 x, int16 y, uint16 serial_server, uint32 serial, std::string const& item_name);
		void Drop(Unit* mOwner, uint16 world, int16 x, int16 y, uint8 attribute);
		void Create(Player* pPlayer, uint8 attribute);
		ItemBagItem* GetRandomItem(Player* pPlayer) const;
		bool IsItemClass(Player* pPlayer, ItemBagItem const* pItemBagItem) const;
		uint32 GetRandomGroup() const;
		bool BuildItem(ItemBagItem const* RandomItem, Item & item, Unit* pUnit, int16 x, int16 y, uint8 attribute);
		bool BuildCurreny(Player* pPlayer, int16 x, int16 y);

		bool IsEmpty() const { return this->items.empty(); }
};

typedef std::map<uint32, ItemBag*> ItemBagMap;

typedef std::vector<ItemBag*> ItemBagList;
typedef std::map<uint8, ItemBagList> ItemBagListMap;

class ItemBagMgr
{
	SingletonInstance(ItemBagMgr);

	public:
		ItemBagMgr();
		virtual ~ItemBagMgr();
		void Clear();

		void LoadItemBagTemplate();
		void LoadItemBagItems();

		ItemBag * GetItemBag(uint32 guid);
		ItemBag * GetItemBag(std::string const& name) const;

		uint8 ExecuteItemBag(ItemBagType type, Unit* pUnit, std::string const& name, int32 data_1 = 0, int32 data_2 = 0, int16 x = 0, int16 y = 0, uint16 serial_server = 0, uint32 serial = 0, std::string const& item_name = "");

		bool RunItemBag(Player* pPlayer, std::string const& name, Item & item, bool gremory_case = false, uint8 gremory_case_type = 0);
	private:
		ItemBagMap m_item_bag;			/*Ordenado por GUID*/
		ItemBagListMap m_item_bag_list;	/*Ordenado por Type*/
};

#endif