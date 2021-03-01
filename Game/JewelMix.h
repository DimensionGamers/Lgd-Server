/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "JewelMix.h"
*
*/

#ifndef JEWEL_MIX_H
#define JEWEL_MIX_H

enum
{
	JEWEL_MIX_FLAG_NONE			= 0,
	JEWEL_MIX_FLAG_MIX			= 1 << 0,
	JEWEL_MIX_FLAG_UNMIX		= 1 << 1,
};

enum JewelMixType
{
	JEWEL_MIX_BLESS,
	JEWEL_MIX_SOUL,
	JEWEL_MIX_LIFE,
	JEWEL_MIX_CREATION,
	JEWEL_MIX_GUARDIAN,
	JEWEL_MIX_STONE,
	JEWEL_MIX_HARMONY,
	JEWEL_MIX_CHAOS,
	JEWEL_MIX_LOWREF,
	JEWEL_MIX_HIREF,
	JEWEL_MIX_MAX
};

#define JEWEL_MIX_TYPE_MAX		3

struct JewelMixData
{
	DECLARE(uint8, id);
	DECLARE(uint8, type);
	DECLARE(uint16, index);
	DECLARE(uint8, level);
	DECLARE(uint8, count);
	DECLARE(uint32, mix_money);
	DECLARE(uint32, unmix_money);
	DECLARE(uint8, pack_type);
	DECLARE(uint16, pack_index);
	DECLARE(uint8, flags);

	bool flagHas(uint8 flag) const { return this->flags.get() & flag; }

	uint16 GetItem() const { return ITEMGET(this->type.get(), this->index.get()); }
	uint16 GetPackItem() const { return ITEMGET(this->pack_type.get(), this->pack_index.get()); }
};

typedef std::vector<JewelMixData*> JewelMixDataList;

class JewelMix
{
	SingletonInstance(JewelMix);

	public:
		JewelMix();
		virtual ~JewelMix();

		void Load();

		JewelMixData const* GetJewelData(uint8 id, uint8 level) const;
		JewelMixData const* GetJewelData(Item const* pItem) const;

		void Mix(Player* pPlayer, uint8 * Packet);
		void MixResult(Player* pPlayer, uint8 result);

		void MixElementalSymbols(Player* pPlayer, uint8 * Packet);

		void UnMix(Player* pPlayer, uint8 * Packet);
		void UnMixResult(Player* pPlayer, uint8 result);

		void LogDB(Player* pPlayer, uint8 type, uint8 id, uint8 level);

		JewelMixDataList m_jewel_mix;
};

#endif