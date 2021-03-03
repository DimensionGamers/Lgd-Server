#ifndef LABYRINTH_DIMENSIONS_H
#define LABYRINTH_DIMENSIONS_H

/*******************************************************
					PACKET INFO
*******************************************************/
#pragma pack(1)
struct LABYRINTH_OF_DIMENSIONS_ENTER_REQUEST : C1_HEADER_SUB
{
	uint16 level;
};

struct LABYRINTH_OF_DIMENSIONS_ENTER_RESULT : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_ENTER_RESULT(uint8 result)
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_ENTRANCE, sizeof(LABYRINTH_OF_DIMENSIONS_ENTER_RESULT));
		this->status = 0;
		this->result = result;
		memset(this->junk, 0, 2);
		this->tick_count = 0;
	}

	uint8 status;
	uint8 result;
	uint8 junk[2];
	uint32 tick_count;
};

struct LABYRINTH_OF_DIMENSIONS_ENTER_CONFIRM_REQUEST : C1_HEADER_SUB
{
	uint16 level;
};

struct LABYRINTH_OF_DIMENSIONS_ZONE_INFO : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_ZONE_INFO()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_ZONE_ENTER, sizeof(LABYRINTH_OF_DIMENSIONS_ZONE_INFO));
		this->junk1 = 0;
		this->junk2 = 0;
	}

	uint8 junk1;
	uint8 world;
	uint8 x;
	uint8 y;
	uint8 row_count;
	uint8 column_count;
	uint8 index;
	uint8 sub_index;
	uint8 texture;
	uint8 type;
	uint8 junk2;
	uint8 gate[LABYRINTH_GATE_MAX];
};

struct LABYRINTH_OF_DIMENSIONS_GATE_ENTER : C1_HEADER_SUB
{
	uint8 id;
};

struct LABYRINTH_OF_DIMENSIONS_GATE_BODY
{
	uint8 index;
	uint8 area_type;
	uint8 gate[LABYRINTH_GATE_MAX];
};

struct LABYRINTH_OF_DIMENSIONS_GATE_HEAD : C2_HEADER_SUB
{
	uint8 count;
};

struct LABYRINTH_OF_DIMENSIONS_MISSION : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_MISSION()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_MISSION, sizeof(LABYRINTH_OF_DIMENSIONS_MISSION));
		this->state = 0;
		this->type = 0;
		this->junk = 0;
		this->current_count = 0;
		this->total_count = 0;
	}

	uint8 state;
	uint8 type;
	uint16 junk;
	int32 current_count;
	int32 total_count;
};

struct LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST_BODY
{
	uint8 id;
	uint8 state;
	uint8 type;
	uint8 junk;
	int32 current_count;
	int32 total_count;
};

struct LABYRINTH_OF_DIMENSIONS_MISSION_FINAL_LIST_HEAD : C1_HEADER_SUB
{
	uint8 count;
};

struct LABYRINTH_OF_DIMENSIONS_MISSION_FINAL : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_MISSION_FINAL()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_MISSION_FINAL, sizeof(LABYRINTH_OF_DIMENSIONS_MISSION_FINAL));
		this->id = 0;
		this->state = 0;
		this->type = 0;
		this->junk = 0;
		this->current_count = 0;
		this->total_count = 0;
	}

	uint8 id;
	uint8 state;
	uint8 type;
	uint8 junk;
	int32 current_count;
	int32 total_count;
};

struct LABYRINTH_OF_DIMENSIONS_STAGE_RESULT : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_STAGE_RESULT()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_STAGE_RESULT, sizeof(LABYRINTH_OF_DIMENSIONS_STAGE_RESULT));
		this->stage = 0;
		memset(this->junk, 0, 3);
		this->earned_experience = 0;
		this->monster_killed = 0;
		this->item_reward = 0;
		this->score = 0;
		this->goblin_score = 0;
		this->first_day_score = 0;
		this->clear_time_points = 0;
		this->unk1 = 0;
		this->time = 0;
	}

	uint8 stage;
	uint8 junk[3];
	uint64 earned_experience;
	uint64 monster_killed;
	uint16 item_reward;
	uint16 score;
	uint16 goblin_score;
	uint16 first_day_score;
	uint16 clear_time_points;
	uint16 unk1;
	uint32 time;
};

struct LABYRINTH_OF_DIMENSIONS_STAGE_DATA : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_STAGE_DATA()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_STAGE_DATA, sizeof(LABYRINTH_OF_DIMENSIONS_STAGE_DATA));
		this->stage = 0;
		memset(this->junk, 0, 3);
		this->final_level = 0;
	}
	
	uint8 stage;
	uint8 junk[3];
	uint32 final_level;
};

struct LABYRINTH_OF_DIMENSIONS_MISSION_FAIL : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_MISSION_FAIL()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_MISSION_FAIL, sizeof(LABYRINTH_OF_DIMENSIONS_MISSION_FAIL));
	}
};

struct LABYRINTH_OF_DIMENSIONS_TIMER : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_TIMER()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_TIMER, sizeof(LABYRINTH_OF_DIMENSIONS_TIMER));
		this->time = 0;
	}

	uint32 time;
};

struct LABYRINTH_OF_DIMENSIONS_ENTRANCE_BODY
{
	char name[MAX_CHARACTER_LENGTH + 1]; //24
	uint8 data1[5]; //34
	uint32 points; //40
	uint32 league_points; //44
	uint16 rank; //48
	uint8 race; //50
	uint8 data2[5]; //51
};

struct LABYRINTH_OF_DIMENSIONS_ENTRANCE_HEAD : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_ENTRANCE_HEAD()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_ENTRANCE_LAYER, sizeof(LABYRINTH_OF_DIMENSIONS_ENTRANCE_HEAD));
		this->rank = 0;
		this->category = MAX_LABYRINTH_OF_DIMENSIONS_RANK;
		this->unk1 = 0;
		this->current_stage = 0;
		this->top_category = MAX_LABYRINTH_OF_DIMENSIONS_RANK;
		this->remain_time = 0;
		this->unk5 = 0;
		for (int32 i = 0; i < 5; ++i)
		{
			memset(this->data[i].name, 0, MAX_CHARACTER_LENGTH + 1);
			memset(this->data[i].data1, 0, 5);
			memset(this->data[i].data2, 0, 5);
			this->data[i].points = 0;
			this->data[i].league_points = 0;
			this->data[i].rank = 0;
			this->data[i].race = 0;
		}
	}

	uint32 rank; //4
	uint8 category; //8 //Bronze, etc
	uint8 unk1; //9
	uint16 current_stage; //10 //1, 2, 3 o 4
	uint32 top_category; //12
	uint32 remain_time; //16
	uint32 unk5; //20
	LABYRINTH_OF_DIMENSIONS_ENTRANCE_BODY data[5];
};

struct LABYRINTH_OF_DIMENSIONS_0x19
{
	LABYRINTH_OF_DIMENSIONS_0x19()
	{
		this->h.set(HEADCODE_LABYRINTH_OF_DIMENSIONS, 0x19, sizeof(LABYRINTH_OF_DIMENSIONS_0x19));
		this->result = 0;
		this->rank = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 rank;
};

struct LABYRINTH_OF_DIMENSIONS_LEVEL_LIST : C1_HEADER_SUB
{
	LABYRINTH_OF_DIMENSIONS_LEVEL_LIST()
	{
		this->Set(HEADCODE_LABYRINTH_OF_DIMENSIONS, SUBCODE_LABYRINTH_OF_DIMENSIONS_LEVEL_LIST, sizeof(LABYRINTH_OF_DIMENSIONS_LEVEL_LIST));

		for (int32 i = 0; i < MAX_LABYRINTH_LEVEL; ++i)
		{
			this->level[i] = 0;
		}
	}

	uint8 level[MAX_LABYRINTH_LEVEL];
};
#pragma pack()

/*******************************************************
					DB INFO
*******************************************************/

struct LabyrinthInfo
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
};

struct LabyrinthMissionData
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(int32, Value);
};

struct LabyrinthMissionMonster
{
	DECLARE_ENUM(uint16, Monster);
	DECLARE_ENUM(int32, Count);
};

struct LabyrinthMission
{
	DECLARE_ENUM(uint8, AreaType);
	DECLARE_ENUM(uint8, Index);
	DECLARE_ARRAY_STRUCT(LabyrinthMissionData, Mission, LABYRINTH_MISSION_MAX);
	DECLARE_ARRAY_STRUCT(LabyrinthMissionMonster, Monster, LABYRINTH_MISSION_MONSTER_MAX);
};

struct LabyrinthAreaData
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint8, SubIndex);
	DECLARE_ENUM(uint8, WorldIndex);
	DECLARE_ENUM(uint8, AreaType);
	DECLARE_ENUM(uint8, MissionID);
	DECLARE_PROPERTY_ARRAY(uint8, Gate, LABYRINTH_GATE_MAX);
};

typedef std::vector<LabyrinthAreaData*> LabyrinthMapSettingDataList;

struct LabyrinthArea
{
	virtual ~LabyrinthArea()
	{
		LIST_CLEAR(LabyrinthMapSettingDataList::iterator, this->data_list);
	}

	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint8, Stage);
	DECLARE_ENUM(uint8, RowCount);
	DECLARE_ENUM(uint8, ColumnCount);

	DECLARE_PTR(LabyrinthAreaData, Entrance);
	DECLARE_PTR(LabyrinthAreaData, Exit);

	LabyrinthMapSettingDataList data_list;
};

struct LabyrinthLevel
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(int32, Level);
	DECLARE_ENUM(int32, FirstScore);
	DECLARE_ENUM(int32, Score);
};

struct LabyrinthMonster
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_FLOAT(Level);
	DECLARE_FLOAT(HP);
	DECLARE_FLOAT(Damage);
	DECLARE_FLOAT(Defense);
	DECLARE_FLOAT(AttackSuccessRate);
	DECLARE_FLOAT(DefenseSuccessRate);
	DECLARE_FLOAT(ElementalDamage);
	DECLARE_FLOAT(ElementalDefense);
	DECLARE_FLOAT(ElementalAttackSuccessRate);
	DECLARE_FLOAT(ElementalDefenseSuccessRate);
};

struct LabyrinthReward
{
	DECLARE_ENUM(uint8, Category);
	DECLARE_ENUM(uint8, Rank);
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint8, Durability);
	DECLARE_ENUM(uint8, Skill);
	DECLARE_ENUM(uint8, Luck);
	DECLARE_ENUM(uint8, Option);
	DECLARE_ENUM(uint8, Excellent);
	DECLARE_ENUM(uint8, Ancient);
	DECLARE_ENUM(uint32, DBFlags);
};

typedef std::map<uint8, LabyrinthInfo*> LabyrinthInfoMap;
typedef std::list<LabyrinthMission*> LabyrinthMissionList;
typedef std::map<uint8, LabyrinthArea*> LabyrinthAreaMap;
typedef std::map<uint16, LabyrinthLevel*> LabyrinthLevelMap;
typedef std::map<uint16, LabyrinthMonster*> LabyrinthMonsterMap;
typedef std::vector<LabyrinthReward*> LabyrinthRewardList;

/*******************************************************
					RANKING INFO
*******************************************************/
struct LabyrinthRankingPlayer
{
	explicit LabyrinthRankingPlayer()
	{
		this->Init();
	}

	virtual ~LabyrinthRankingPlayer() { }

	void Init()
	{
		this->SetID(0);
		this->ResetName();
		this->SetClass(0);
		this->SetScore(0);
		this->SetLeagueScore(0);
		this->SetRank(0);
		this->SetCategory(0);
		this->SetTime(0);
	}

	DECLARE_ENUM(uint32, ID);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(uint8, Class);
	DECLARE_PROPERTY(int32, Score);
	DECLARE_PROPERTY(int32, LeagueScore);
	DECLARE_PROPERTY(uint16, Rank);
	DECLARE_ENUM(uint8, Category);
	DECLARE_ENUM(time_t, Time);
};

typedef std::map<uint32, LabyrinthRankingPlayer*> LabyrinthRankingPlayerMap;

class LabyrinthDimensions
{
	SingletonInstance(LabyrinthDimensions);

	public:
		LabyrinthDimensions();
		virtual ~LabyrinthDimensions();

		void LoadInfo();
		void LoadMission();
		void LoadArea();
		void LoadAreaData();
		void LoadLevel();
		void LoadMonster();
		void LoadReward();
		void LoadRanking();
		void LoadSchedule();
		
		bool EnterAllowed(Player* pPlayer);

		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result, uint32 tick_count);

		void EnterConfirm(Player* pPlayer, uint8 * Packet);
		void SendZoneInfo(Player* pPlayer);

		void GateEnter(Player* pPlayer, uint8 * Packet);

		void LeagueInfo(Player* pPlayer, uint8 * Packet);
		void Leave(Player* pPlayer, uint8 * Packet);

		void TeleportToZone(Player* pPlayer, uint8 idx, uint8 subidx);

		void SendGateList(Player* pPlayer);
		void SendMission(Player* pPlayer);
		void SendFinalMissionList(Player* pPlayer);
		void SendFinalMission(Player* pPlayer, uint8 index);
		void CreateGoblin(Player* pPlayer, LabyrinthAreaData const* pGate);

		void SendStage(Player* pPlayer);

		LabyrinthArea const* GetLabyrinthArea(uint8 index, uint8 stage) const;
		LabyrinthAreaData const* GetLabyrinthAreaData(uint8 index, uint8 idx, uint8 subidx) const;
		LabyrinthMission const* GetLabyrinthMission(uint8 area_type, uint8 index) const;
		uint8 GetRandomMission(uint8 area_type, uint8 index) const;
		LabyrinthLevel const* GetLevel(uint16 id) const;
		LabyrinthMonster const* GetMonster(uint16 id) const;
		void GetReward(uint8 category, uint8 rank, uint8 race, LabyrinthRewardList & list) const;
 
		void SetInitialGate(Player* pPlayer, uint8 index);
		void AddGate(Player* pPlayer, uint8 index, uint8 idx, uint8 subidx, bool enter);
		void StartMission(Player* pPlayer, PlayerLabyrinthGate * pGate);
		void CheckAddMission(Player* pPlayer, PlayerLabyrinthGate * pGate);
		void SpawnMonsters(Player* pPlayer, PlayerLabyrinthGate * pGate);

		LabyrinthAreaData const* GetCurrentAreaData(Player* pPlayer) const;
		PlayerLabyrinthGate * GetCurrentGate(Player* pPlayer);
		PlayerLabyrinthGate * GetGate(Player* pPlayer, uint8 idx, uint8 subidx);

		PlayerLabyrinthGate const* GetCurrentGate(Player* pPlayer) const;
		PlayerLabyrinthGate const* GetGate(Player* pPlayer, uint8 idx, uint8 subidx) const;

		bool IsMissionCompleted(Player* pPlayer) const;
		bool IsMissionCompleted(Player* pPlayer, uint8 idx, uint8 subidx) const;

		void UpdateMission(Player* pPlayer, LabyrinthOfDimensionsMissionType type, int32 ammount, bool reset = false, bool move = true);
		
		void Respawn(Player* pPlayer);
		void SendMissionFail(Player* pPlayer);
		void Finish(Player* pPlayer);

		void CompleteMission(Player* pPlayer);

		void Update(Player* pPlayer);
		void InsertRanking(Player* pPlayer);
		void InsertLeague(Player* pPlayer);

		void SendTimer(Player* pPlayer, bool init = false);
		bool FoundExit(Player* pPlayer) const;

		LabyrinthRankingPlayer * FindPlayer(uint32 id);
		LabyrinthRankingPlayer const* FindPlayer(uint8 category, uint8 rank) const;
		
		void SaveSchedule();
		void UpdateState();
		void ChangeState();
		void Update();
		void UpdateRankingSimple(uint8 category);
		void UpdateRanking();
		void UpdateCategory();
		void UpdateRanking(Player* pPlayer);
		void GiveRewards();
		void CleanRanking();

		void MovePlayersOut();
	private:
		LabyrinthInfoMap labyrinth_info_map;
		LabyrinthMissionList labyrinth_mission_list;
		LabyrinthAreaMap labyrinth_area_map;
		LabyrinthLevelMap m_LabyrinthLevelMap;
		LabyrinthMonsterMap m_LabyrinthMonsterMap;
		LabyrinthRewardList m_LabyrinthRewardList;

		LabyrinthRankingPlayerMap m_LabyrinthRankingPlayerMap;

		DECLARE_ENUM(time_t, LeagueTime);
		DECLARE_ENUM(uint8, LeagueState);
};

#endif