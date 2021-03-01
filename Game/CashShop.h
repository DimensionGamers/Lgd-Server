/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "CashShop.h"
*
*/

#ifndef CASH_SHOP_H
#define CASH_SHOP_H

struct CashShopCategory
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, Parent);
	DECLARE_ENUM(uint8, Main);
};

struct CashShopProduct
{
	DECLARE_ENUM(uint32, Product);
	DECLARE_ENUM(uint32, Package);
	DECLARE_ENUM(uint32, Option);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint32, Price);
	DECLARE_ENUM(uint8, BuyType);
	DECLARE_ENUM(uint8, UseType);
	DECLARE_FLAG(uint32, Flags);
	DECLARE_ENUM(time_t, Duration);
	DECLARE_ENUM(uint8, ItemType);
	DECLARE_ENUM(uint16, ItemIndex);
	DECLARE_ENUM(uint8, ItemLevel);
	DECLARE_FLOAT(ItemDurability);
	DECLARE_ENUM(uint8, ItemSkill);
	DECLARE_ENUM(uint8, ItemLuck);
	DECLARE_ENUM(uint8, ItemOption);
	DECLARE_ENUM(uint8, ItemExcellent);
	DECLARE_ENUM(uint8, ItemAncient);
	DECLARE_ENUM(int32, Count);

	uint16 GetItem() const { return ITEMGET(this->GetItemType(), this->GetItemIndex()); }
};

typedef std::map<uint16, CashShopCategory*> CashShopCategoryMap;
typedef std::vector<CashShopProduct*> CashShopProductList;

struct CashShopPackage
{
	DECLARE_ENUM(uint32, Package);
	DECLARE_ENUM(uint32, Category);
	DECLARE_ENUM(uint8, Position);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_PROPERTY_STRING(Description);
	DECLARE_ENUM(uint16, DisplayItem);
	DECLARE_FLAG(uint32, Flags);
	DECLARE_ENUM(uint8, PriceType);
	DECLARE_ENUM(time_t, StartDate);
	DECLARE_ENUM(time_t, EndDate);
};

typedef std::map<uint32, CashShopPackage*> CashShopPackageMap;

class CashShopMgr
{
	SingletonInstance(CashShopMgr);

	public:
		CashShopMgr();
		virtual ~CashShopMgr();

		void LoadCategory();
		void LoadItemList();
		void LoadPackageData();
		void LoadProductData();

		CashShopPackage const* GetPackage(uint32 package) const;
		CashShopProduct const* GetProduct(uint32 product, uint32 option) const;
		uint32 GetPackagePrice(uint32 package, uint32 option) const;

		void SaveBuyLog(Player* pPlayer, uint8 price_type, CashShopProduct const* pProduct, bool gift, const char * dest_char);

		void CashShopOpenRequest(Player* pPlayer, uint8 * Packet);
		void CashShopOpenResult(Player* pPlayer, uint8 result);

		void CashShopCoinRequest(Player* pPlayer);
		
		void CashShopItemListRequest(Player* pPlayer, uint8 * Packet);
		void CashShopItemListRequest(Player* pPlayer, uint16 page, uint8 type);
		void CashShopItemListResult(Player* pPlayer, uint16 page_current, uint16 page_total);
		void CashShopItemListSend(Player* pPlayer, uint8 type, uint16 page);

		void CashShopItemBuyRequest(Player* pPlayer, uint8 * Packet);
		void CashShopItemBuyResult(Player* pPlayer, uint8 result);
		void CashShopItemBuyCallBack(Player* pPlayer, SerialCreateData* data);

		void CashShopItemGiftRequest(Player* pPlayer, uint8 * Packet);
		void CashShopItemGiftResult(Player* pPlayer, uint8 result);

		void CashShopItemUseRequest(Player* pPlayer, uint8 * Packet);
		void CashShopItemUseResult(Player* pPlayer, uint8 result);

		void CashShopItemUseRequestNew(Player* pPlayer, uint8 * Packet);

		void SendUpdatedData(Player* pPlayer);
		void SendCategoryList(Player* pPlayer);
		void SendPackageList(Player* pPlayer);
		void SendProductList(Player* pPlayer);
	public:
		CashShopCategoryMap m_CategoryMap;
		CashShopPackageMap m_PackageMap;
		CashShopProductList m_ProductList;
};

#endif