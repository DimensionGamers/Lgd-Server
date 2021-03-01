/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "CastleSiegeDef.h"
*
*/
#ifndef CASTLE_SIEGEDEF_H
#define CASTLE_SIEGEDEF_H

#define CS_LEVER_AI				"castle_siege_lever_ai"
#define CS_LIFE_STONE_AI		"castle_siege_life_stone_ai"
#define CS_GUARDIAN_STATUE_AI	"castle_siege_guardian_statue_ai"
#define CS_CROWN_AI				"castle_siege_crown_ai"
#define CS_CROWN_SWITCH_AI_1	"castle_siege_crown_switch_ai_1"
#define CS_CROWN_SWITCH_AI_2	"castle_siege_crown_switch_ai_2"
#define CS_GATE_AI				"castle_siege_gate_ai"
#define CS_CASTLE_MACHINE_AI	"castle_siege_machine_ai"

enum CastleSiegeState
{
	CASTLE_SIEGE_STATE_IDLE_1			= 0,
	CASTLE_SIEGE_STATE_REGISTER_GUILD	= 1,
	CASTLE_SIEGE_STATE_IDLE_2			= 2,
	CASTLE_SIEGE_STATE_REGISTER_MARK	= 3,
	CASTLE_SIEGE_STATE_IDLE_3			= 4,
	CASTLE_SIEGE_STATE_NOTIFY			= 5,
	CASTLE_SIEGE_STATE_READY			= 6,
	CASTLE_SIEGE_STATE_START			= 7,
	CASTLE_SIEGE_STATE_END				= 8,
	CASTLE_SIEGE_STATE_ENDCYCLE			= 9,
	CASTLE_SIEGE_STATE_MAX
};

enum CastleSiegeCrownAccessState
{
	CASTLE_SIEGE_CROWN_ACCESS_STATE_ATTEMPT,
	CASTLE_SIEGE_CROWN_ACCESS_STATE_SUCCESS,
	CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL,
	CASTLE_SIEGE_CROWN_ACCESS_STATE_MAX
};

enum CastleSiegeJoinSide
{
	CASTLE_SIEGE_JOIN_SIDE_NONE,
	CASTLE_SIEGE_JOIN_SIDE_DEFENSE,
	CASTLE_SIEGE_JOIN_SIDE_ATTACK_1,
	CASTLE_SIEGE_JOIN_SIDE_ATTACK_2,
	CASTLE_SIEGE_JOIN_SIDE_ATTACK_3,
	CASTLE_SIEGE_JOIN_SIDE_MAX
};

#define CASTLE_SIEGE_NPC_MAX_DEFENSE_LEVEL		4
#define CASTLE_SIEGE_NPC_MAX_REGEN_LEVEL		4
#define CASTLE_SIEGE_NPC_MAX_LIFE_LEVEL			4

#define CASTLE_SIEGE_UPGRADE_DEF_RANGE(value) (value >= 0 && value < CASTLE_SIEGE_NPC_MAX_DEFENSE_LEVEL ? true: false)
#define CASTLE_SIEGE_UPGRADE_REG_RANGE(value) (value >= 0 && value < CASTLE_SIEGE_NPC_MAX_REGEN_LEVEL ? true: false)
#define CASTLE_SIEGE_UPGRADE_LIFE_RANGE(value) (value >= 0 && value < CASTLE_SIEGE_NPC_MAX_LIFE_LEVEL ? true: false)

enum CastleSiegeUpgradeType
{
	CASTLE_SIEGE_UPGRADE_NONE,
	CASTLE_SIEGE_UPGRADE_DEFENSE,
	CASTLE_SIEGE_UPGRADE_REGEN,
	CASTLE_SIEGE_UPGRADE_LIFE,
	CASTLE_SIEGE_UPGRADE_MAX
};

enum CastleSiegeNpcUpgrade
{
	CASTLE_SIEGE_NPC_UPGRADE_GATE,
	CASTLE_SIEGE_NPC_UPGRADE_STATUE,
	CASTLE_SIEGE_NPC_UPGRADE_MAX
};

enum CastleSiegeMachineType
{
	CASTLE_SIEGE_MACHINE_NONE,
	CASTLE_SIEGE_MACHINE_ATTACK,
	CASTLE_SIEGE_MACHINE_DEFENSE,
	CASTLE_SIEGE_MACHINE_MAX = 2
};

#define CASTLE_SIEGE_MACHINE_ATTACK_POINTS			4
#define CASTLE_SIEGE_MACHINE_ATTACK_TIME			10 * IN_MILLISECONDS
#define CASTLE_SIEGE_MACHINE_TARGET_TIME			100

static const struct CastleSiegeWeaponLocation
{
	coord_type x1;
	coord_type y1;
	coord_type x2;
	coord_type y2;
} g_CastleSiegeWeaponAttackData[CASTLE_SIEGE_MACHINE_ATTACK_POINTS] = {
	{ 0x3E, 0x67, 0x48, 0x70 }, 
	{ 0x58, 0x68, 0x7C, 0x6F }, 
	{ 0x74, 0x69, 0x7C, 0x70 }, 
	{ 0x49, 0x56, 0x69, 0x67 }
}, g_CastleSiegeWeaponDefenseData[CASTLE_SIEGE_MACHINE_ATTACK_POINTS] = {
	{ 0x3D, 0x58, 0x5D, 0x6C }, 
	{ 0x5C, 0x59, 0x7F, 0x6F }, 
	{ 0x54, 0x34, 0x66, 0x42 }, 
	{ 0x54, 0x34, 0x66, 0x42 }, 
};

static const struct CastleSiegeRespawnArea
{
	coord_type x1;
	coord_type y1;
	coord_type x2;
	coord_type y2;
} g_CastleSiegeRespawnArea[3] =
{
	35,		17,		144,	48,
	74,		144,	115,	154,
	35,		11,		144,	48
};

static const coord_type g_CastleSiegeWallStepAxisY[3] = 
{
	113, 160, 203
};

#define CASTLE_SIEGE_TAX_RATE_HUNT_MAX 300000
#define CASTLE_SIEGE_TAX_RATE_HUNT_UNIT 10000

#define CASTLE_SIEGE_MINI_MAP_POINT_MAX			1000

#endif