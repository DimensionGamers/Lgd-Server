/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ChaosCastleDef.h"
*
*/
#ifndef CHAOS_CASTLE_DEF_H
#define CHAOS_CASTLE_DEF_H

#define MAX_CHAOS_CASTLE_GROUND			7
#define CHAOS_CASTLE_STANDBY_TIME		1
#define CHAOS_CASTLE_END_TIME			1
#define MAX_CHAOS_CASTLE_TRAP_STEP		4
#define MAX_CHAOS_CASTLE_HOLLOW_ZONE	4

#define CHAOS_CASTLE_GROUND_RANGE(ground) (ground < 0 ? false: ground >= MAX_CHAOS_CASTLE_GROUND ? false: true)

#define MAX_CHAOS_CASTLE_ENEMY			100
#define CHAOS_CASTLE_GATE				503

enum ChaosCastleState
{
	CHAOS_CASTLE_STATE_NONE,
	CHAOS_CASTLE_STATE_OPEN,
	CHAOS_CASTLE_STATE_STANDBY,
	CHAOS_CASTLE_STATE_PLAYING,
	CHAOS_CASTLE_STATE_PLAYEND,
	CHAOS_CASTLE_STATE_MAX
};

typedef std::vector<POINT> ChaosCastlePoint;

static const EventCommonGroundData m_ChaosCastleData[MAX_CHAOS_CASTLE_GROUND] =
{
	WORLD_CHAOS_CASTLE_1,	82,
	WORLD_CHAOS_CASTLE_2,	83,		
	WORLD_CHAOS_CASTLE_3,	84,		
	WORLD_CHAOS_CASTLE_4,	85,		
	WORLD_CHAOS_CASTLE_5,	86,		
	WORLD_CHAOS_CASTLE_6,	87,		
	WORLD_CHAOS_CASTLE_7,	272
};

static const int32 g_ChaosCastle_BlowOutDistance[4][2] = {3,4,3,4,2,3,0,1};
static const int32 g_ChaosCastle_BlowOutDamage[4] = {15, 15, 10, 5};

static const coord_type g_ChaosCastle_GroundAxis[4] = { 23, 75, 44, 108 };

static const coord_type g_ChaosCastle_DamageAxis[MAX_CHAOS_CASTLE_TRAP_STEP][4] = 
{
	0x17, 0x4B,	0x2C, 0x6C,
	0x19, 0x4D,	0x2A, 0x6A,
	0x1B, 0x4F,	0x28, 0x68,
	0x1D, 0x51,	0x26, 0x66
};

static const coord_type g_ChaosCastleSafetyZone[4] = { 23, 75, 44, 108 };
static const coord_type g_ChaosCastleCenterHollowZone[2][4] = { 32, 84, 35, 87, 32, 96, 35, 99 };

static const struct ChaosCastleMapHollowZone
{
	coord_type X1;
	coord_type Y1;
	coord_type X2;
	coord_type Y2;
} g_ChaosCastleMapHollowZone[MAX_CHAOS_CASTLE_TRAP_STEP][MAX_CHAOS_CASTLE_HOLLOW_ZONE] =
{
	0x17, 0x4B,	0x2C, 0x4C,
	0x2B, 0x4D,	0x2C, 0x6C,
	0x17, 0x6B,	0x2A, 0x6C,
	0x17, 0x4D,	0x18, 0x6A,
	0x17, 0x4B,	0x2C, 0x4C,
	0x2B, 0x4D,	0x2C, 0x6C,
	0x17, 0x6B,	0x2A, 0x6C,
	0x17, 0x4D,	0x18, 0x6A,
	0x19, 0x4D,	0x2A, 0x4E,
	0x29, 0x4F,	0x2A, 0x6A,
	0x19, 0x69,	0x28, 0x6A,
	0x19, 0x4F,	0x1A, 0x68,
	0x1B, 0x4F,	0x28, 0x50,
	0x27, 0x51,	0x28, 0x68,
	0x1B, 0x67,	0x26, 0x68,
	0x1B, 0x51,	0x1C, 0x66
};

/**********************************************************************************************
							SURVIVAL OF THE FITTEST
**********************************************************************************************/
#define MAX_CHAOS_CASTLE_SURVIVAL_RANK					50
#define MAX_CHAOS_CASTLE_SURVIVAL_RANK_SEMI_FINAL		20
#define MAX_CHAOS_CASTLE_SURVIVAL_RANK_FINAL			10

enum ChaosCastleSurvivalStage
{
	CHAOS_CASTLE_SURVIVAL_STAGE_NONE,
	CHAOS_CASTLE_SURVIVAL_STAGE_PRELIMINARY,
	CHAOS_CASTLE_SURVIVAL_STAGE_SEMI_FINAL,
	CHAOS_CASTLE_SURVIVAL_STAGE_FINAL,
};

#define MAX_CHAOS_CASTLE_SURVIVAL_STAGE		3

#define CHAOS_CASTLE_SURVIVAL_GROUND_RANGE(ground) (ground < 0 ? false: ground >= MAX_CHAOS_CASTLE_SURVIVAL_STAGE ? false: true)


/**********************************************************************************************
									DROP DATA
**********************************************************************************************/
struct ChaosCastleItemData
{
	uint16 item;
	uint8 level;
	uint8 durability;
	uint8 count;
};

typedef std::vector<ChaosCastleItemData> ChaosCastleItemDataList;

struct ChaosCastleFinalReward
{
	std::string itembag;
};

static const ChaosCastleItemData g_ChaosCastleItemData[MAX_CHAOS_CASTLE_GROUND][2] = 
{
	{ { JEWEL::BLESS, 0, 1, 3 }, { JEWEL::SOUL, 0, 1, 3 } },
	{ { JEWEL::BLESS, 0, 1, 3 }, { JEWEL::SOUL, 0, 1, 3 } },
	{ { JEWEL::BLESS, 0, 1, 3 }, { JEWEL::SOUL, 0, 1, 3 } },
	{ { JEWEL::BLESS, 0, 1, 5 }, { JEWEL::SOUL, 0, 1, 5 } },
	{ { JEWEL::BLESS, 0, 1, 5 }, { JEWEL::SOUL, 0, 1, 5 } },
	{ { JEWEL::BLESS, 0, 1, 5 }, { JEWEL::SOUL, 0, 1, 5 } },
	{ { JEWEL::BLESS, 0, 1, 5 }, { JEWEL::SOUL, 0, 1, 5 } },
};

static const ChaosCastleItemData g_ChaosCastleSurvivalItemData[MAX_CHAOS_CASTLE_SURVIVAL_STAGE] = 
{
	{ ITEMGET(14, 254), 0, 0, 6 },
	{ ITEMGET(14, 256), 0, 0, 6 },
	{ ITEMGET(14, 257), 0, 0, 6 },
};

static const uint16 g_ChaosCastleSurvivalFinalItemData[MAX_CHAOS_CASTLE_SURVIVAL_STAGE] = 
{
	{ ITEMGET(14, 256) },
	{ ITEMGET(14, 257) },
	{ ITEMGET(13, 276) },
};

static const ChaosCastleFinalReward g_ChaosCastleFinalReward[MAX_CHAOS_CASTLE_GROUND] =
{
	{ "Chaos Castle 1" },
	{ "Chaos Castle 2" },
	{ "Chaos Castle 3" },
	{ "Chaos Castle 4" },
	{ "Chaos Castle 5" },
	{ "Chaos Castle 6" },
	{ "Chaos Castle 7" },
};

#endif