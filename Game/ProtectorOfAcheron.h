#ifndef PROTECTOR_OF_ACHERON_H
#define PROTECTOR_OF_ACHERON_H

enum ProtectorOfAcheronState
{
	PROTECTOR_OF_ACHERON_STATE_NONE,
	PROTECTOR_OF_ACHERON_STATE_PLAYING,
	PROTECTOR_OF_ACHERON_STATE_END,
};

#define PROTECTOR_OF_ACHERON_MAX_ZONE			3
#define PROTECTOR_OF_ACHERON_PLAY_TIME			30 * MINUTE * IN_MILLISECONDS
#define PROTECTOR_OF_ACHERON_END_TIME			MINUTE * IN_MILLISECONDS

static const struct ProtectorOfAcheronZone
{
	uint16 world;
	int16 x;
	int16 y;
} g_ProtectorOfAcheronZone[PROTECTOR_OF_ACHERON_MAX_ZONE] =
{
	{92, 89, 41},
	{92, 188, 101},
	{96, 162, 54}
};

static const struct ProtectorOfAcheronMonster
{
	uint16 obelisk;
	uint16 boss;
} g_ProtectorOfAcheronMonster[PROTECTOR_OF_ACHERON_MAX_ZONE][ELEMENTAL_ATTRIBUTE_MAX] =
{
	{
		{0, 0},
		{627, 633},
		{628, 632},
		{629, 635},
		{630, 634},
		{631, 636}
	},

	{
		{0, 0},
		{627, 633},
		{628, 632},
		{629, 635},
		{630, 634},
		{631, 636}
	},

	{
		{0, 0},
		{627, 637},
		{628, 638},
		{629, 640},
		{630, 639},
		{631, 641}
	}
};

struct ProtectorOfAcheronBoss
{
	DECLARE_ENUM(uint16, TimeMin);
	DECLARE_ENUM(uint16, TimeMax);
	DECLARE_ENUM(uint16, Count);
	DECLARE_ENUM(uint32, Period);
	DECLARE_ENUM(int32, MaxCount);
};

typedef std::list<ProtectorOfAcheronBoss*> ProtectorOfAcheronBossList;

class ProtectorOfAcheron
{
	SingletonInstance(ProtectorOfAcheron);

	public:
		ProtectorOfAcheron();
		virtual ~ProtectorOfAcheron();

		void Load();

		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result);

		void Start();
		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Playing);
		CREATE_STATE(End);

		bool CreateObelisk();
		void BossCreate();
		void SendSuccess();
		void SendFail();
		void MovePlayersOut();
		
		DECLARE_ENUM(uint8, State);
		DECLARE_ENUM(PentagramElementalAttribute, ElementalAttribute);
		DECLARE_ENUM(uint8, Zone);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, BossTime);
		DECLARE_PROPERTY(int32, BossCount);

		ProtectorOfAcheronBossList boss_list;
};

#endif