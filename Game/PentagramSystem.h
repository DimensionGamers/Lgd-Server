// PentagramSystem.h: interface for the PentagramSystem class.
//
//////////////////////////////////////////////////////////////////////
#ifndef PENTRAGRAM_SYSTEM_H
#define PENTRAGRAM_SYSTEM_H

#pragma once

struct PentagramTypeInfo
{
	DECLARE_ENUM(uint16, Index);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, Rate);
	DECLARE_PROPERTY_ARRAY(uint8, Option, MAX_PENTAGRAM_BONUS_OPTION);
};

struct PentagramOptionInfo
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint16, Value);
	DECLARE_PROPERTY_ARRAY(uint8, Type, MAX_PENTAGRAM_JEWEL_RANK);
	DECLARE_PROPERTY_ARRAY(uint8, Rank, MAX_PENTAGRAM_JEWEL_RANK);
	DECLARE_PROPERTY_ARRAY(uint8, Level, MAX_PENTAGRAM_JEWEL_RANK);
};

struct ErrtelOption
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, RankNumber);
	DECLARE_ENUM(uint8, RankOption);
	DECLARE_ENUM(uint16, Rate);
	DECLARE_PROPERTY_ARRAY(int32, Value, MAX_PENTAGRAM_JEWEL_LEVEL);
	DECLARE_PROPERTY_ARRAY(uint16, UpgradeRate, MAX_PENTAGRAM_JEWEL_LEVEL);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, VariableType);
	DECLARE_ENUM(uint8, Type);
};

struct PentagramJewelRemoveInfo
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_PROPERTY_ARRAY(uint16, MixRate, MAX_PENTAGRAM_JEWEL_RANK);
};

struct PentagramJewelUpgradeRankInfo
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint32, MixMoney);
	DECLARE_ENUM(uint16, MixRate);
};

struct PentagramJewelUpgradeLevelInfo
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint32, MixMoney);
	DECLARE_PROPERTY_ARRAY(uint16, MixRate, MAX_PENTAGRAM_JEWEL_RANK);
	DECLARE_PROPERTY_ARRAY(int32, RequiredElementalRune, MAX_PENTAGRAM_JEWEL_RANK);
};

typedef std::map<uint16, PentagramTypeInfo*> PentagramTypeInfoMap;
typedef std::map<uint8, PentagramOptionInfo*> PentagramOptionInfoMap;
typedef std::vector<ErrtelOption*> ErrtelOptionList;
typedef std::map<uint8, PentagramJewelRemoveInfo*> PentagramJewelRemoveInfoMap;
typedef std::map<uint8, PentagramJewelUpgradeRankInfo*> PentagramJewelUpgradeRankInfoMap;
typedef std::map<uint8, PentagramJewelUpgradeLevelInfo*> PentagramJewelUpgradeLevelInfoMap;

class PentagramSystem
{
	SingletonInstance(PentagramSystem);

	public:
		PentagramSystem();
		virtual ~PentagramSystem();

		void LoadType();
		void LoadOption();
		void LoadJewelOption();
		void LoadJewelRemove();
		void LoadJewelUpgradeRank();
		void LoadJewelUpgradeLevel();

		bool IsPentagramItem(uint16 item) const;
		bool IsPentagramItem(Item const* pItem) const;
		bool IsPentagramMithril(uint16 item) const;
		bool IsPentagramMithril(Item const* pItem) const;

		PentagramJewelInfo* GetPentagramJewelInfo(Player* pPlayer, uint8 index, uint8 type);
		PentagramJewelInfo* AddPentagramJewelInfo(Player* pPlayer, Item const* pItem, uint8 type);
		PentagramJewelInfo* AddPentagramJewelInfo(Player* pPlayer, PentagramJewelInfo const* pInfo, uint8 type);
		void DelPentagramJewelInfo(Player* pPlayer, uint8 index, uint8 type, bool log = false, bool to_gremory = false, int32 talisman_count = 0);
		void DelAllPentagramJewelInfo(Player* pPlayer, Item * pItem, uint8 type);
		bool CheckExchangePentagramItem(Player* pPlayer);
		bool CheckExchangePentagramItem(Item const* pItem);
		bool CheckPentagramOption(Player* pPlayer, PentagramOptionInfo const* pInfo);
		void MovePentagramToInventoryFromWarehouse(Player* pPlayer, Item * pItem);
		void MovePentagramToWarehouseFromInventory(Player* pPlayer, Item * pItem);
		void ExchangePentagramItem(Player* pPlayer01, Player* pPlayer02);
		void ExchangePentagramItem(Player* pPlayer01, Player* pPlayer02, Item * pItem);
		PentagramTypeInfo const* GetPentagramTypeInfo(uint16 index) const;
		PentagramOptionInfo const* GetPentagramOptionInfo(uint8 index) const;
		ErrtelOption const* GetErrtelOption(uint16 item, uint8 rank_number, uint8 rank_option) const;
		PentagramJewelRemoveInfo const* GetPentagramJewelRemoveInfo(uint8 index) const;
		PentagramJewelUpgradeRankInfo const* GetPentagramJewelUpgradeRankInfo(uint8 index) const;
		PentagramJewelUpgradeLevelInfo const* GetPentagramJewelUpgradeLevelInfo(uint8 index) const;
		SocketDataType GetRandomErrtelOption(uint16 item, uint8 rank_number);
		void GetPentagramRelationshipDamage(Unit* pUnit, Unit* pTarget, int32 & damage);
		void GetPentagramRelationshipDefense(Unit* pUnit, Unit* pTarget, int32 & defense);
		void ApplyPentagramOption(Player* pPlayer);
		void ApplyPentagramOption(Player* pPlayer, uint8 index, uint16 value);
		void ApplyErrtelOption(Player* pPlayer);
		void CGPentagramJewelInsertRecv(Player* pPlayer, uint8 * Packet);
		void CGPentagramJewelRemoveRecv(Player* pPlayer, uint8 * Packet);
		void CGPentagramJewelRefineRecv(Player* pPlayer, uint8 * Packet);
		void CGPentagramJewelUpgradeRecv(Player* pPlayer, uint8 * Packet);
		void CGPentagramUpgradeRadianceSlot(Player* pPlayer, uint8 * Packet);

		void GCPentagramJewelResultSend(Player* pPlayer, uint8 result);
		void GCPentagramJewelInfoSend(Player* pPlayer, uint8 type);
		void GCPentagramJewelTradeInfoSend(Player* pPlayer01, Player* pPlayer02, Item * pItem);
		void GCPentagramJewelPShopInfoSend(Player* pPlayer01, Player* pPlayer02, uint32 slot);

		void Log(Player* pPlayer, uint8 type, uint8 spirit_blessing, uint8 result, std::string const& pentagram_data, std::string const& errtel_data);

		void FixPentagram(Player* pPlayer);
	private:
		PentagramTypeInfoMap m_PentagramTypeInfo;
		PentagramOptionInfoMap m_PentagramOptionInfo;
		ErrtelOptionList m_ErrtelOptionList;
		PentagramJewelRemoveInfoMap m_PentagramJewelRemoveInfo;
		PentagramJewelUpgradeRankInfoMap m_PentagramJewelUpgradeRankInfo;
		PentagramJewelUpgradeLevelInfoMap m_PentagramJewelUpgradeLevelInfo;
};

static const int gPentagramRelationshipDamageTablePvP[ELEMENTAL_ATTRIBUTE_MAX][ELEMENTAL_ATTRIBUTE_MAX] = {
	//NN	F		W		E		WI		D
	{100,	100,	100,	100,	100,	100}, // NN
	{286,	260,	234,	247,	273,	286}, // FIRE
	{286,	286,	260,	234,	247,	273}, // WATER
	{286,	273,	286,	260,	234,	247}, // EARTH
	{286,	247,	273,	286,	260,	234}, // WIND
	{286,	234,	247,	273,	286,	260}, // DARKNESS
};

static const int gPentagramRelationshipDamageTablePvM[ELEMENTAL_ATTRIBUTE_MAX][ELEMENTAL_ATTRIBUTE_MAX] = {
	//NN	F		W		E		WI		D
	{100,	100,	100,	100,	100,	100}, // NN
	{90,	100,	90,		95,		105,	110}, // FIRE
	{90,	110,	100,	90,		95,		105}, // WATER
	{90,	105,	110,	100,	90,		95}, // EARTH
	{90,	95,		105,	110,	100,	90}, // WIND
	{90,	90,		95,		105,	110,	100}, // DARKNESS
};

static const int gPentagramRelationshipDefenseTablePvP[ELEMENTAL_ATTRIBUTE_MAX][ELEMENTAL_ATTRIBUTE_MAX] = {
	//NN	F		W		E		WI		D
	{100,	80,		80,		80,		80,		80}, // NN
	{80,	100,	90,		95,		105,	110}, // FIRE
	{80,	110,	100,	90,		95,		105}, // WATER
	{80,	105,	110,	100,	90,		95}, // EARTH
	{80,	95,		105,	110,	100,	90}, // WIND
	{80,	90,		95,		105,	110,	100}, // DARKNESS
};

static const int gPentagramRelationshipDefenseTablePvM[ELEMENTAL_ATTRIBUTE_MAX][ELEMENTAL_ATTRIBUTE_MAX] = {
	//NN	F		W		E		WI		D
	{100,	80,		80,		80,		80,		80}, // NN
	{100,	100,	90,		95,		105,	110}, // FIRE
	{100,	110,	100,	90,		95,		105}, // WATER
	{100,	105,	110,	100,	90,		95}, // EARTH
	{100,	95,		105,	110,	100,	90}, // WIND
	{100,	90,		95,		105,	110,	100}, // DARKNESS
};

#endif