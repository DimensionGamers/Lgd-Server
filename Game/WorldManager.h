#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

typedef std::set<world_type> WorldServerList;
typedef std::map<uint16, WorldServerList> WorldServerMap;

struct world_experience
{
	DECLARE_ENUM(world_type, World);
	DECLARE_PROPERTY_ARRAY(int32, Rate, MAX_PARTY_MEMBERS);
};

typedef std::map<world_type, world_experience*> WorldExperienceMap;

typedef std::map<world_type, World*> WorldMap;

struct WorldFreePK
{
	DECLARE_ENUM(world_type, World);
	DECLARE_ENUM(coord_type, X1);
	DECLARE_ENUM(coord_type, Y1);
	DECLARE_ENUM(coord_type, X2);
	DECLARE_ENUM(coord_type, Y2);
};

typedef std::vector<WorldFreePK*> WorldFreePKList;

class WorldMgr
{
	SingletonInstance(WorldMgr);

	private:
		
		WorldServerMap world_server_map;
		WorldFreePKList m_WorldFreePKList;

	public:
		WorldMgr();
		virtual ~WorldMgr();

		void LoadWorldList();
		void LoadWorldData();
		void LoadWorldAIPath();
		void LoadWorldBuff();
		void LoadWorldAreaRestriction();
		void LoadWorldServer();
		void LoadWorldAttribute();
		void LoadWorldExperience();
		void LoadWorldAreaAttribute();
		void LoadWorldFreePK();

		void Update();
		void UpdateWeather();
		bool IsWorld(world_type world);
		World* GetWorld(world_type world);

		void statusChange(world_type entry, uint8 status);
		void MakeItemVisible(Player* pPlayer);

		uint16 AllowMoveToWorld(Player* pPlayer, world_type world);
		bool AllowMoveToWorld(Player* pPlayer, world_type world, uint16 server);

		bool IsWorldAllowed(world_type world);

		void UpdateUrukMountainZone(bool apply);
		void SendUrukMountainZone();
		void SendUrukMountainZone(Player* pPlayer);

		void UpdateFereaZone(bool apply);
		void SendFereaZone();
		void SendFereaZone(Player* pPlayer);
		
		WorldMap world_map;
		WorldExperienceMap world_experience_map;

		int32 GetWorldExperienceRate(world_type world, uint8 count);

		void SetPKBoss(world_type world, bool enabled);
		bool IsPKBoss(world_type world);

		bool IsItemDropAllowed(Player* pPlayer, World* pWorld, coord_type x, coord_type y);
		bool IsFreePK(world_type world, coord_type x, coord_type y) const;
};

#endif