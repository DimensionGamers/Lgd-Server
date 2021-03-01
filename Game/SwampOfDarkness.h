#ifndef SWAMP_OF_DARKNESS_H
#define SWAMP_OF_DARKNESS_H

enum SwampOfDarknessState
{
	SWAMP_OF_DARKNESS_STATE_NONE,
	SWAMP_OF_DARKNESS_STATE_OPEN,
	SWAMP_OF_DARKNESS_STATE_BATTLE_1,
	SWAMP_OF_DARKNESS_STATE_BATTLE_2,
	SWAMP_OF_DARKNESS_STATE_END,

	SWAMP_OF_DARKNESS_STATE_MAX
};

static const struct SwampOfDarknessZone
{
	coord_type x;
	coord_type y;
} g_SwampOfDarkness[2] = {
	{ 160, 0 },
	{ 210, 40 },
}, g_SwampOfDarknessWaterSource[1] = {
	{ 175, 27 },
};

#pragma pack(1)
struct SWAMP_OF_DARKNESS_BOSS_AREA
{
	SWAMP_OF_DARKNESS_BOSS_AREA()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_SWAMP_OF_DARKNESS_BOSS_AREA, sizeof(SWAMP_OF_DARKNESS_BOSS_AREA));
		this->state = 0;
	}

	PBMSG_HEAD2 h;
	uint8 state;

	/*
	default = Boss
	3 = No Boss
	4 = 5 minutes before boss appear
	*/
};

struct SWAMP_OF_DARKNESS_BOSS_TIMER
{
	SWAMP_OF_DARKNESS_BOSS_TIMER()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_SWAMP_OF_DARKNESS_BOSS_TIMER, sizeof(SWAMP_OF_DARKNESS_BOSS_TIMER));
		this->time = 0;
	}

	PBMSG_HEAD2 h;
	uint32 time;
};

struct SWAMP_OF_DARKNESS_BOSS_WATER_MONSTER
{
	SWAMP_OF_DARKNESS_BOSS_WATER_MONSTER()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_SWAMP_OF_DARKNESS_BOSS_WATER_MONSTER, sizeof(SWAMP_OF_DARKNESS_BOSS_WATER_MONSTER));
		this->id = 0;
	}

	PBMSG_HEAD2 h;
	uint32 id;
};

struct SWAMP_OF_DARKNESS_BOSS_ATTACK
{
	SWAMP_OF_DARKNESS_BOSS_ATTACK()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_SWAMP_OF_DARKNESS_BOSS_ATTACK, sizeof(SWAMP_OF_DARKNESS_BOSS_ATTACK));
		this->monster = 0;
		this->id = 0;
	}

	PBMSG_HEAD2 h;
	uint32 monster;
	uint8 id;

	/*
	1 = Swamp Monster Summon
	2 = Swamp Pet Attack
	3 = Water Monster Attack Wave
	4 = Water Monster Attack Water Course
	5 = Water Monster HP
	6 = Flame Skill
	7 = Avoid black smoke and save from death!
	*/
};

struct SWAMP_OF_DARKNESS_BOSS_ENTER_RESULT
{
	SWAMP_OF_DARKNESS_BOSS_ENTER_RESULT()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_SWAMP_OF_DARKNESS_BOSS_ENTER, sizeof(SWAMP_OF_DARKNESS_BOSS_ENTER_RESULT));
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;

	/*
	1 = Success
	2 = Fail
	*/
};
#pragma pack()

class SwampOfDarkness
{
	SingletonInstance(SwampOfDarkness);

	public:
		SwampOfDarkness();
		virtual ~SwampOfDarkness();

		void ChangeState(SwampOfDarknessState state);
		void Update();

private:
		void SetState_None();
		void SetState_Open();
		void SetState_Battle_1();
		void SetState_Battle_2();
		void SetState_End();
		void ProcState_None();
		void ProcState_Open();
		void ProcState_Battle_1();
		void ProcState_Battle_2();
		void ProcState_End();

		void CreateBattle1();
		void CreateBattle2();
		void MovePlayersOut();

public:
		bool IsBossZone(Player* pPlayer) const;
		bool IsBossZone(world_type world, coord_type x1, coord_type y1, coord_type x2, coord_type y2) const;

		int32 GetPlayerCount() const;
		bool IsAllowedToEnter(Player* pPlayer) const;

		void SendPacket(uint8 * Packet, uint16 size);
		void SendTime();
		void SendState(Player* pPlayer = nullptr);
		void SendAttack(Monster* pMonster, uint8 id);
		void SendWaterMonster(uint16 id);
		
		DECLARE_ENUM(SwampOfDarknessState, State);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, SecondTimer);

		DECLARE_ENUM(coord_type, WaterMonsterX);
		DECLARE_ENUM(coord_type, WaterMonsterY);
};

#endif