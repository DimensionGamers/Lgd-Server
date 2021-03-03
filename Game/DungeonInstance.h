#ifndef DUNGEON_INSTANCE_H
#define DUNGEON_INSTANCE_H

typedef std::map<uint32, Player*> DungeonInstancePlayerMap;

typedef std::map<uint8, bool> DungeonInstanceChestMap;

class DungeonInstance
{
	public:
		DungeonInstance();
		virtual ~DungeonInstance();

		void Update();
		void UpdatePlayers();

		void AddPlayer(Player* pPlayer);
		void AddMonster();
		void BossKill(uint8 id);
		void CreateChest();
		void ChestOpen(uint8 type, uint8 id);
		void MovePlayersOut();
		bool IsEmpty() const;
		void SaveInstance();

		int32 GetCountByPC(Player* pPlayer) const;

		void CreateSimpleTrap();
		void CreateTrap(int16 x, int16 y, uint8 type);

		DECLARE_ENUM(uint8, ID);
		DECLARE_ENUM(uint16, LevelID);
		DECLARE_ENUM(uint16, World);
		DECLARE_ENUM(int32, Instance);
		DECLARE_ENUM(uint16, PartyID);
		DECLARE_PROPERTY(int32, KilledMonsters);
		DECLARE_ENUM(uint8, BossKilled);
		DECLARE_STRUCT(TickTimer, BossKillTime);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, SimpleTrapTime);

		DECLARE_ENUM(int32, PlayerCount);

		DungeonInstancePlayerMap m_DungeonInstancePlayerMap;
		DungeonInstanceChestMap m_DungeonInstanceChestMap[2];
};

#endif