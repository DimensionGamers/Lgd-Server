#ifndef INSTANCE_DUNGEON_H
#define INSTANCE_DUNGEON_H

struct DungeonInstanceData
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(uint32, Flags);
	DECLARE_ENUM(uint16, Gate);

	DECLARE_ENUM(coord_type, MainChestX);
	DECLARE_ENUM(coord_type, MainChestY);
	DECLARE_PROPERTY_ARRAY(coord_type, ChestX, MAX_DUNGEON_SECONDARY_CHEST);
	DECLARE_PROPERTY_ARRAY(coord_type, ChestY, MAX_DUNGEON_SECONDARY_CHEST);
};

struct DungeonInstanceLevel
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(int16, MinLevel);
	DECLARE_ENUM(int16, MaxLevel);
	DECLARE_FLOAT(Level);
	DECLARE_FLOAT(HP);
	DECLARE_FLOAT(Damage);
	DECLARE_FLOAT(Defense);
	DECLARE_FLOAT(AttackSuccess);
	DECLARE_FLOAT(DefenseSuccess);
	DECLARE_FLOAT(ElementalDamage);
	DECLARE_FLOAT(ElementalDefense);
	DECLARE_FLOAT(ElementalAttackSuccess);
	DECLARE_FLOAT(ElementalDefenseSuccess);
	DECLARE_PROPERTY_STRING(ItemBag);
	DECLARE_PROPERTY_STRING(ItemBagBoss);
};

struct DungeonInstanceSaved
{
	DECLARE_ENUM(uint32, CharID);
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(time_t, Time);
};

typedef std::map<int32, DungeonInstance*> DungeonInstanceMap;
typedef std::map<uint8, DungeonInstanceData*> DungeonInstanceDataMap;
typedef std::map<uint16, DungeonInstanceLevel*> DungeonInstanceLevelMap;
typedef std::map<uint32, DungeonInstanceSaved*> DungeonInstanceSavedMap;

class Dungeon
{
	SingletonInstance(Dungeon);

	public:
		Dungeon();
		virtual ~Dungeon();

		void LoadSavedInstance();
		void LoadInstance();
		void LoadLevel();

		DungeonInstanceData const* GetRandomDungeon() const;
		DungeonInstanceData const* GetInstanceData(uint8 id) const;
		DungeonInstanceLevel const* GetInstanceLevel(uint16 level) const;
		DungeonInstanceLevel const* GetInstanceByLevel(int16 level) const;
		bool IsDungeon(uint16 world) const;
		void GetDungeonRespawn(Player* pPlayer, uint16 & world, coord_type & x, coord_type & y);
		DungeonInstance * FindInstance(int32 id) const;
		DungeonInstance * FindInstanceByParty(uint16 id) const;
		DungeonInstanceSaved * FindPlayerInstance(uint32 id) const;

		void Update();

		void EnterRequest(Player* pPlayer);
		void SaveInstance(uint32 char_id, uint8 id, int64 time);
		int32 GetCountByPC(Player* pPlayer) const;

		DungeonInstanceMap m_DungeonInstanceMap;
		DungeonInstanceDataMap m_DungeonInstanceDataMap;
		DungeonInstanceLevelMap m_DungeonInstanceLevelMap;
		DungeonInstanceSavedMap m_DungeonInstanceSavedMap;

		DECLARE_PROPERTY(int32, InstanceID);
};

#endif