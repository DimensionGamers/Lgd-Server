/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ShopMgr.h"
*
*/

#ifndef SHOP_TEMPLATE_H
#define SHOP_TEMPLATE_H

enum ShopFlag
{
	SHOP_FLAG_NONE,
	SHOP_FLAG_SIEGE_TAX			= 1 << 0,
	SHOP_FLAG_REPAIR			= 1 << 1,
	SHOP_FLAG_PK_SCORE			= 1 << 2,
	SHOP_FLAG_ADMINISTRATOR		= 1 << 3,
	SHOP_FLAG_NO_SELL			= 1 << 4,
	SHOP_FLAG_GUILD_MASTER		= 1 << 5,
	SHOP_FLAG_GUILD_BATTLE		= 1 << 6,
	SHOP_FLAG_GUILD_ASSISTANT	= 1 << 7,
	SHOP_FLAG_GUILD_ALL			= 1 << 8,
};

enum ShopType
{
	SHOP_TYPE_ZEN,
	SHOP_TYPE_RUUD,
	SHOP_TYPE_WCOIN,
	SHOP_TYPE_GOBLIN_POINTS,
	SHOP_TYPE_RUUD_MISC,
	SHOP_TYPE_RARE_TICKET,
	SHOP_TYPE_JEWEL_OF_BLESS,
	SHOP_TYPE_JEWEL_OF_SOUL,
	SHOP_TYPE_JEWEL_OF_CHAOS,
};

enum ShopCountType
{
	SHOP_COUNT_NONE,
	SHOP_COUNT_CHARACTER,
	SHOP_COUNT_ACCOUNT,
	SHOP_COUNT_PC,
	SHOP_COUNT_SERVER,
};

class ItemShop: public Item
{
	public:
		DECLARE(uint8, position);
		DECLARE_ENUM(uint32, Price);
};

typedef std::map<uint32, int32> ShopBuyCountMap;
typedef std::map<uint8, ShopBuyCountMap> ShopBuyMap;

class Shop
{
	friend class ShopMgr;

	public:
		explicit Shop();

		void insert_item(ItemShop item);
		uint8 map_check(uint8 X, uint8 Y, uint8 W, uint8 H);

		DECLARE_ENUM(uint8, ID);
		DECLARE_PROPERTY_STRING(Name);
		DECLARE_ENUM(uint8, MaxPKLevel);
		DECLARE_PROPERTY_STRING(PKText);
		DECLARE_FLAG(uint8, Flag);
		DECLARE_ARRAY_STRUCT(ItemShop, Item, max_shop_item);
		DECLARE(uint8, shop_map[max_shop_item]);
		DECLARE_ENUM(uint8, Type);
		DECLARE_PROPERTY(int32, ItemCount);

		DECLARE_ENUM(int32, MaxBuyCount);
		DECLARE_ENUM(uint8, MaxBuyType);
};

typedef std::map<uint8, Shop*> ShopMap;

class ShopMgr
{
	SingletonInstance(ShopMgr);

	private:
		ShopMap shop_map;

		ShopBuyMap buy_count_character;
		ShopBuyMap buy_count_account;
		ShopBuyMap buy_count_pc;
		ShopBuyMap buy_count_server;

		DECLARE_ENUM(uint16, UpdateDay);
	public:
		ShopMgr();
		virtual ~ShopMgr();

		void LoadShopTemplate();
		void LoadShopItems();
	
		Shop * GetShop(uint8 id);
		Shop const* GetShop(uint8 id) const;
		Shop const* GetShop(std::string const& name) const;

		bool IsShop(std::string const& name) const;

		bool EnableToBuy(Player* pPlayer, Shop const* pShop);
		void Update();
};

#endif