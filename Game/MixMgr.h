#ifndef MIX_MGR_H
#define MIX_MGR_H

struct MixSpellStone
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Count);
	DECLARE_PROPERTY_ARRAY(uint16, Rate, MAX_SPELL_STONE_COUNT);
};

struct MixGuardian
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint16, MainItem);
	DECLARE_ENUM(uint8, MainItemLevel);
	DECLARE_ENUM(uint8, MainItemDurability);
	DECLARE_PROPERTY_ARRAY(uint16, MaterialItem, MAX_GUARDIAN_MIX_MATERIAL);
	DECLARE_PROPERTY_ARRAY(int32, MaterialAmount, MAX_GUARDIAN_MIX_MATERIAL);
	DECLARE_ENUM(int32, Price);
	DECLARE_ENUM(int32, SuccessRate);
	DECLARE_ENUM(uint16, ResultItem);
	DECLARE_ENUM(uint8, ResultLevel);
};

typedef std::map<uint16, uint8> MixDisabledList;
typedef std::map<uint8, MixDisabledList> MixDisabledMap;

typedef std::vector<MixSpellStone*> MixSpellStoneList;

typedef std::vector<MixGuardian*> MixGuardianList;

class MixMgr
{
	SingletonInstance(MixMgr);

	public:
		MixMgr();
		virtual ~MixMgr();
		void Clear();
		void LoadDisabled();
		void LoadSpellStone();
		void LoadGuardian();

		bool IsMixDisabled(uint8 type, uint16 id) const;

		uint16 GetSpiritStoneCount(uint8 type, int64 price) const;

		MixGuardian const* GetGuardianMix(uint8 type, uint8 id) const;

		MixDisabledMap mix_disabled;
		MixSpellStoneList m_MixSpellStoneList;

		MixGuardianList m_MixGuardianList;
};

#endif