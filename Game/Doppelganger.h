#ifndef DOPPELGANGER_H
#define DOPPELGANGER_H

#define MAX_DOPPELGANGER_GROUND					7
#define MAX_DOPPELGANGER_ZONE					4
#define DOPPELGANGER_OPEN_TIME					5
//#define DOPPELGANGER_OPEN_TIME					1
#define DOPPELGANGER_GROUND_STANDBY_PVE_TIME	60
#define DOPPELGANGER_GROUND_PLAYING_PVE_TIME	10
//#define DOPPELGANGER_GROUND_PLAYING_PVE_TIME	1
#define DOPPELGANGER_GROUND_STANDBY_PVP_TIME	10
#define DOPPELGANGER_GROUND_PLAYING_PVP_TIME	3
#define DOPPELGANGER_GROUND_END_TIME			60

#define DOPPELGANGER_DISTANCE					22

#define MAX_DOPPELGANGER_MONSTER_PASSED			3

#define DOPPELGANGER_GATE						507

#define DOPPELGANGER_GROUND_RANGE(ground) (ground < 0 ? false: ground >= MAX_DOPPELGANGER_GROUND ? false: true)

enum DoppelgangerState
{
	DOPPELGANGER_STATE_NONE,
	DOPPELGANGER_STATE_OPEN,
	DOPPELGANGER_STATE_PLAYING,
};

enum DoppelgangerGroundState
{
	DOPPELGANGER_GROUND_STATE_NONE,
	DOPPELGANGER_GROUND_STATE_OPEN,
	DOPPELGANGER_GROUND_STATE_STANDBY_PVE,
	DOPPELGANGER_GROUND_STATE_PLAYING_PVE,
	DOPPELGANGER_GROUND_STATE_STANDBY_PVP,
	DOPPELGANGER_GROUND_STATE_PLAYING_PVP,
	DOPPELGANGER_GROUND_STATE_END,
};

enum
{
	DOPPELGANGER_MESSAGE_START,
	DOPPELGANGER_MESSAGE_BOSS,
	DOPPELGANGER_MESSAGE_END,
	DOPPELGANGER_MESSAGE_MAX
};

enum DoppelgangerQuest
{
	DOPPELGANGER_QUEST_NONE,
	DOPPELGANGER_QUEST_STANDBY,
	DOPPELGANGER_QUEST_COMPLETED,
	DOPPELGANGER_QUEST_FAILED,
};

enum DoppelgangerWorldAttribute
{
	DOPPELGANGER_WORLD_ATTRIBUTE_START,
	DOPPELGANGER_WORLD_ATTRIBUTE_FINAL,
	DOPPELGANGER_WORLD_ATTRIBUTE_MIDDLE
};

static const struct DoppelgangerZoneData
{
	world_type world;
	uint16 gate;
	uint16 middle;
} g_DoppelgangerZoneData[MAX_DOPPELGANGER_GROUND][MAX_DOPPELGANGER_ZONE] =
{
	{{82, 345, 381}, {82, 346, 382}, {82, 347, 383}, {82, 348, 384}},
	{{83, 349, 385}, {83, 350, 386}, {83, 351, 387}, {83, 352, 388}},
	{{84, 353, 389}, {84, 354, 390}, {84, 355, 391}, {84, 356, 392}},
	{{85, 357, 393}, {85, 358, 394}, {85, 359, 395}, {85, 360, 396}},
	{{86, 361, 397}, {86, 362, 398}, {86, 363, 399}, {86, 364, 400}},
	{{87, 365, 401}, {87, 366, 402}, {87, 367, 403}, {87, 368, 404}},
	{{88, 369, 405}, {88, 370, 406}, {88, 371, 407}, {88, 372, 408}},
	//{{89, 373, 409}, {89, 374, 410}, {89, 375, 411}, {89, 376, 412}},
	//{{90, 377, 413}, {90, 378, 414}, {90, 379, 415}, {90, 380, 416}},
};

struct DoppelgangerAttribute
{
	coord_type x1;
	coord_type y1;
	coord_type x2;
	coord_type y2;
};

static const DoppelgangerAttribute g_DoppelgangerStartAttribute[MAX_DOPPELGANGER_ZONE] =
{
	{70, 64, 77, 69},
	{66, 175, 71, 184},
	{178, 184, 186, 189},
	{185, 70, 190, 77}
};

static const DoppelgangerAttribute g_DoppelgangerFinalAttribute[MAX_DOPPELGANGER_ZONE] = 
{
	{113, 124, 121, 129},
	{125, 133, 130, 142},
	{134, 124, 144, 129},
	{125, 110, 130, 120}
};

static const DoppelgangerAttribute g_DoppelgangerMiddleAttribute =
{
	122, 121, 133, 131,
};

static const struct DoppelgangerPosition
{
	coord_type start_x;
	coord_type start_y;
	coord_type end_x;
	coord_type end_y;
} g_DoppelgangerPosition[MAX_DOPPELGANGER_ZONE] =
{
	{63, 66, 119, 126},
	{69, 191, 127, 135},
	{193, 186, 136, 126},
	{188, 64, 128, 118}
};

/*
MOBS ZONA 1 -> 117, 121
MOBS ZONA 2 -> 128, 137
MOBS ZONA 3 -> 138, 126
MOBS ZONA 4 -> 128, 116
*/

static const uint8 g_DoppelgangerMessage[DOPPELGANGER_MESSAGE_MAX] = { EVENT_MESSAGE_DOPPELGANGER_START, EVENT_MESSAGE_DOPPELGANGER_BOSS, EVENT_MESSAGE_DOPPELGANGER_END };
static const coord_type g_DoppelgangerGoldenChest[2] = {128, 126};

struct DoppelgangerMonster
{
	DECLARE_ENUM(uint16, Monster);
	DECLARE_ENUM(int16, Level);
	DECLARE_ENUM(int32, Life);
	DECLARE_ENUM(int32, AttackMin);
	DECLARE_ENUM(int32, AttackMax);
	DECLARE_ENUM(int32, AttackRate);
	DECLARE_ENUM(int32, Defense);
	DECLARE_ENUM(int32, DefenseRate);
	DECLARE_PROPERTY_STRING(ItemBag);
};

typedef std::map<uint16, DoppelgangerMonster*> DoppelgangerMonsterMap;

struct DoppelgangerLevelData
{
	DECLARE_ENUM(uint32, GUID);
	DECLARE_ENUM(int16, LevelMin);
	DECLARE_ENUM(int16, LevelMax);
	DECLARE_ENUM(int32, Experience);
	DECLARE_ENUM(uint16, LarvaRate);
	DECLARE_ENUM(int32, LarvaCount);
	DECLARE_PROPERTY_STRING(SilverChestItemBag);
	DECLARE_PROPERTY_STRING(GoldenChestItemBag);

	DoppelgangerMonster* GetMonster(uint16 monster) const
	{
		DoppelgangerMonsterMap::const_iterator it = this->monsters.find(monster);

		if ( it != this->monsters.end() )
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}

	DoppelgangerMonsterMap monsters;
};

typedef std::map<uint32, DoppelgangerLevelData*> DoppelgangerLevelMap;

struct DoppelgangerPlayer
{
	DoppelgangerPlayer()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetPlayer(nullptr);
		this->SetGUID(0);
		this->SetPosition(0);
		this->SetFail(false);
	}

	DECLARE_PTR(Player, Player);
	DECLARE_ENUM(uint32, GUID);
	DECLARE_ENUM(uint8, Position);
	DECLARE_BOOL(Fail);
};

class DoppelgangerZone
{
	public:
		DoppelgangerZone();
		virtual ~DoppelgangerZone();

		void Reset();

		int32 PlayerGetCount() const;
		bool PartyAdd(Player* pPlayer);
		void UpdatePlayer();
		void UpdatePlayerPosition();
		void UpdateMonsterPosition();
		void UpdateMonsterPassed();
		void PlayerMoveOut();
		void PlayerSendPacket(uint8 * Packet, uint16 size);
		void CreateChest();
		void OpenChest(Monster* pChest, Player* pPlayer);
		void AddLarva(Monster* pChest);

		DoppelgangerAttribute const* GetDoppelgangerAttribute(DoppelgangerWorldAttribute type) const;
		void UpdateWorldAttribute(DoppelgangerWorldAttribute type, bool apply);
		void UpdateWorldAttribute(Player* pPlayer, DoppelgangerWorldAttribute type, bool apply);
		void RemovePlayer(uint32 guid);
		void MonsterPassed();
		void SendMonsterPassed();
		void SendEnd(Player* pPlayer, uint8 type, uint32 experience = 0);

		DECLARE_ENUM(uint8, Zone);
		DECLARE_ENUM(uint8, Ground);
		DECLARE_ENUM(uint16, PartyID);
		DECLARE_PROPERTY(int32, MonsterCount);
		DECLARE_PROPERTY(uint8, MonsterPosition);
		DECLARE_PROPERTY(uint8, MonsterPassed);
		DECLARE_PROPERTY(uint8, BossPosition);
		DECLARE_ENUM(DoppelgangerQuest, Quest);
		DECLARE_ARRAY_STRUCT(DoppelgangerPlayer, Player, MAX_PARTY_MEMBERS);
};

class DoppelgangerGround
{
	public:
		DoppelgangerGround();
		virtual ~DoppelgangerGround();

		void Update();
						
		CREATE_STATE(None);
		CREATE_STATE(Open);
		CREATE_STATE(StandbyPVE);
		CREATE_STATE(PlayingPVE);
		CREATE_STATE(StandbyPVP);
		CREATE_STATE(PlayingPVP);
		CREATE_STATE(End);

		void UpdatePlayer();
		void UpdatePlayerPosition();
		void UpdateMonsterPosition();
		void UpdateMonsterPassed();
		void SendData();
		void SendMonsterPassed();
		void SendTimer(uint8 type);
		void SendTimer(Player* pPlayer, uint8 type);
		void SendEnd(uint8 type);

		int32 GetActiveZoneCount() const;
		void AddMonster();
		void SendMessageID(uint8 id);
		void UpdateWorldAttribute(DoppelgangerWorldAttribute type, bool apply);
		void RemovePlayer(uint32 guid);
		uint8 GetEmptyZone() const;

		void MovePlayersToMiddle();

		void ProcessReward();
		uint8 CalculateWinnerZone() const;
		void CreateGoldenChest();
		void OpenGoldenChest(Player* pPlayer, Monster* pMonster);
						
		DECLARE_ENUM(DoppelgangerGroundState, State);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, SecondTime);
		DECLARE_ENUM(uint8, Ground);
		DECLARE_ARRAY_BOOL(MessageSent, DOPPELGANGER_MESSAGE_MAX);
		DECLARE_ARRAY_STRUCT(DoppelgangerZone, Zone, MAX_DOPPELGANGER_ZONE);
		DECLARE_ENUM(uint8, WinnerZone);
};

class Doppelganger
{
	SingletonInstance(Doppelganger);

	public:
		Doppelganger();
		virtual ~Doppelganger();

		void LoadLevel();
		void LoadMonster();

		DoppelgangerLevelData* GetLevel(uint32 id) const;

		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result);
		uint8 GetEnterGround(int16 level, bool master) const;

		void SendRemainMinutes(std::string const& message, int32 period, int32 max_minute = -1);

		bool IsMinuteElapsed(int32 period)
		{
			if ( !this->GetTime()->GetRemain() || (this->GetTime()->GetRemain() / (IN_MILLISECONDS * MINUTE)) == this->GetCurrentMinute() )
				return false;

			this->SetCurrentMinute(this->GetTime()->GetRemain() / (IN_MILLISECONDS * MINUTE));

			if ( (this->GetCurrentMinute() % period) != 0 )
				return false;

			if ( (this->GetCurrentMinute() + 1) <= 0 )
				return false;

			return true;
		}

		void Start();
		void Update();
		
		CREATE_STATE(None);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		void RemovePlayer(uint32 guid);
		bool IsIn(Player* pPlayer);

		DECLARE_ENUM(DoppelgangerState, State);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, NotificationTime);
		DECLARE_ARRAY_STRUCT(DoppelgangerGround, Ground, MAX_DOPPELGANGER_GROUND);
		DECLARE_ENUM(int32, CurrentMinute);

		DoppelgangerLevelMap level_map;
};

#endif