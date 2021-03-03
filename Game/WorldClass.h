#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

struct WorldAIPath
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(int16, X);
	DECLARE_ENUM(int16, Y);
};

typedef std::vector<WorldAIPath*> WorldAIPathList;

struct WorldBuff
{
	DECLARE_ENUM(uint16, Buff);
	DECLARE_FLAG(uint8, Flag);
};

typedef std::map<uint16, WorldBuff*> WorldBuffMap;

struct WorldArea
{
	DECLARE_ENUM(int16, X);
	DECLARE_ENUM(int16, Y);
	DECLARE_ENUM(uint8, Range);
	DECLARE_FLAG(uint8, Flags);
};

typedef std::vector<WorldArea*> WorldAreaList;

struct WorldAreaAttribute
{
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
	DECLARE_ENUM(uint8, Attribute);
	DECLARE_BOOL(Apply);
	DECLARE_BOOL(Send);
};

typedef std::vector<WorldAreaAttribute*> WorldAreaAttributeList;

class WorldStatusData
{
	public:
		WorldStatusData();
		virtual ~WorldStatusData();

		DECLARE_ENUM(uint8, Status);
		DECLARE_ARRAY_STRUCT(WorldGrid, Grid, WORLD_MAX_SIZE);
		DECLARE_STRING_FIXED(Name, MAX_WORLD_NAME_LENGTH);

		DECLARE_ENUM(uint16, SpawnGate);

		int32 width;
		int32 height;

		DECLARE_ENUM(uint32, Flags);
		DECLARE_ENUM(uint32, ExperienceRate);
		DECLARE_ENUM(uint32, ZenRate);
		DECLARE_BOOL(Active);
		
		bool LoadWorld(const char * filename);

		void Reset()
		{
			this->ResetName();
			this->SetActive(false);
		}
};

typedef std::unordered_map<uint16, Object*> WorldObjectMap;
typedef std::unordered_map<uint16, Player*> WorldPlayerMap;

class World
{
	public:
		friend class WorldMgr;

		const WorldObjectMap& GetAllObjects() const { return world_object_map; }
		const WorldPlayerMap& GetAllPlayers() const { return world_player_map; }

		WorldObjectMap world_object_map;
		WorldPlayerMap world_player_map;
		
		int32 m_world_item_loop;
		WorldAIPathList m_path_list;
		WorldBuffMap m_buff;
		WorldAreaList m_area;
		WorldAreaAttributeList m_attribute;
		
		World();
		virtual ~World();

		const char* GetCurrentStatusName() const;
		bool IsCurrentStatusFlag(uint32 flags)  const;
		uint32 GetCurrentStatusExperienceRate() const;
		uint32 GetCurrentStatusZenRate() const;
		uint16 GetCurrentGate() const;

		bool flagHas(uint32 flag) const { return this->IsCurrentStatusFlag(flag); }
		uint32 GetExperienceRate() const { return this->GetCurrentStatusExperienceRate(); }
		uint32 GetZenRate() const { return this->GetCurrentStatusZenRate(); }

		uint8 GetWeatherConverted() const { return (this->GetWeather() << 4) | this->GetWeatherVariation(); }

		void GetRespawn(uint16 & spawn_world, int16 & x1, int16 & y1, int16 & x2, int16 & y2);
		void GetRespawn(uint16 & spawn_world, int16 & x, int16 & y);
		void GetRandomLocation(int16 & x, int16 & y, int16 x1, int16 y1, int16 x2, int16 y2);
		bool GetFreeLocation(int16 & x, int16 & y, int16 range_x, int16 range_y, int32 count);
		bool GetRandomLocation(int16 & x, int16 & y, int32 length) const;
		
		bool IsObject(Object* pObject);
		void AddObject(Object* pObject);
		void RemoveInvalidBuff(Unit* pUnit);
		WorldBuff* GetBuffData(uint16 buff) const;
	
		void Update();
		void UpdateWeather();
		void UpdateItemQueue();

		bool add_item(Item item, Unit* owner, int16 x, int16 y, bool only_owner = false, bool visible = true, bool to_queue = false);
		WorldItem * GetItem(uint16 id);
		void ClearItem();
		WorldGrid GetGrid(int16 x, int16 y) const;

		bool GetRandomDropLocation(int16 stx, int16 sty, int16 & x, int16 & y, int16 range_x, int16 range_y, int32 loop) const;
		bool AddZen(Unit* pUnit, int16 x, int16 y, uint32 zen);

		bool CheckWall(int16 sx1, int16 sy1, int16 sx2, int16 sy2) const;

		void ApplyAttribute(int16 x, int16 y, uint8 attr, bool apply);
		void ApplyAttribute(int16 const* data, uint8 attr, bool apply);

		void statusChange(uint8 status);

		void MakeItemVisible(Player* pPlayer);

		bool IsAreaRestriction(int16 x, int16 y, uint8 flag) const;

		WorldItem* GetItemData(uint16 item) const
		{
			if ( item >= MAX_WORLD_ITEM )
				return nullptr;

			return this->ItemData[item];
		}

		void Reset()
		{
			this->SetEntry(-1);

			for ( int32 i = 0; i < MAX_WORLD_STATUS; ++i )
			{
				this->WorldStatus[i].Reset();
			}
		}

		int32 GetPlayersCountByPC(Player* pPlayer);
	private:
		WorldStatusData WorldStatus[MAX_WORLD_STATUS];

		DECLARE_ENUM(uint16, Entry);
		DECLARE_ENUM(uint16, Display);
		DECLARE_ENUM(uint8, Status);
		DECLARE_ENUM(uint8, Weather);
		DECLARE_ENUM(uint8, WeatherVariation);
		DECLARE_BOOL(PKBossEnabled);
		DECLARE_STRUCT(TickTimer, PKBossTime);
		TickTimer weather_tick;

		WorldItem* ItemData[MAX_WORLD_ITEM];

		DECLARE_STRUCT(PathFinder, PathFinderManager);

		WorldItemQueuedList world_item_queue;
};

#endif