/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ImperialFortressDef.h"
*
*/
#ifndef IMPERIAL_FORTRESS_DEF_H
#define IMPERIAL_FORTRESS_DEF_H

#define IMPERIAL_FORTRESS_GROUND			7
#define IMPERIAL_FORTRESS_MAX_ZONES			4

#define IMPERIAL_FORTRESS_ZONE_RANGE(x)		(x < 0 ? false : x >= IMPERIAL_FORTRESS_MAX_ZONES ? false : true)

enum ImperialFortressMonsterType
{
	IMPERIAL_FORTRESS_TYPE_GATE = 0,
	IMPERIAL_FORTRESS_TYPE_TRAP = 1,
	IMPERIAL_FORTRESS_TYPE_MONSTER_NORMAL = 2,
	IMPERIAL_FORTRESS_TYPE_BOSS = 3,
	IMPERIAL_FORTRESS_TYPE_STATUE = 5
};

enum ImperialFortressState
{
	IMPERIAL_FORTRESS_STATE_NONE,
	IMPERIAL_FORTRESS_STATE_STANDBY,
	IMPERIAL_FORTRESS_STATE_DOOR_1,
	IMPERIAL_FORTRESS_STATE_FIGHT,
	IMPERIAL_FORTRESS_STATE_DOOR_2,
	IMPERIAL_FORTRESS_STATE_DOOR_3,
	IMPERIAL_FORTRESS_STATE_MOVE,
	IMPERIAL_FORTRESS_STATE_END,
};

static const world_type g_ImperialFortressWorld[IMPERIAL_FORTRESS_GROUND] = {
	WORLD_IMPERIAL_FORTRESS_1,
	WORLD_IMPERIAL_FORTRESS_2,
	WORLD_IMPERIAL_FORTRESS_3,
	WORLD_IMPERIAL_FORTRESS_1,
	WORLD_IMPERIAL_FORTRESS_2,
	WORLD_IMPERIAL_FORTRESS_3,
	WORLD_IMPERIAL_FORTRESS_4 };

static const struct ImperialFortressGates
{
	uint16 gate;
} g_ImperialFortressGates[IMPERIAL_FORTRESS_GROUND][IMPERIAL_FORTRESS_MAX_ZONES] = {
	{307, 308, 310, 0},		// Monday
	{312, 313, 315, 0},		// Tuesday
	{317, 318, 320, 0},		// Wednesday
	{307, 308, 310, 0},		// Thursday
	{312, 313, 315, 0},		// Friday
	{317, 318, 320, 0},		// Saturday
	{322, 323, 325, 327}	// Sunday
};

struct ImperialFortressZoneDef
{
	coord_type x1;
	coord_type y1;
	coord_type x2;
	coord_type y2;
};

enum
{
	IMPERIAL_FORTRESS_ZONE_FIGHT,
	IMPERIAL_FORTRESS_ZONE_TRAP,
	IMPERIAL_FORTRESS_ZONE_GATE,
	IMPERIAL_FORTRESS_ZONE_MAX
};

static const struct ImperialFortressZones
{
	ImperialFortressZoneDef zone[IMPERIAL_FORTRESS_MAX_ZONES][IMPERIAL_FORTRESS_ZONE_MAX];
} g_ImperialFortressZone[IMPERIAL_FORTRESS_GROUND] = {
	// MONDAY
	232, 28, 236, 33, 231, 55, 234, 59, 212, 78, 218, 82,

	190, 24, 194, 26, 162, 23, 168, 28, 153, 52, 156, 59,

	178, 78, 182, 85, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// TUESDAY
	71, 66, 75, 68, 46, 63, 51, 65, 15, 64, 20, 66,

	36, 93, 38, 97, 40, 117, 43, 121, 54, 153, 56, 158,

	103, 110, 107, 112, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// WEDNESDAY
	142, 190, 147, 192, 115, 191, 120, 193, 85, 195, 90, 197,

	222, 133, 224, 138, 222, 159, 224, 164, 222, 192, 224, 197,

	166, 216, 168, 221, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// THURSDAY
	232, 28, 236, 33, 231, 55, 234, 59, 212, 78, 218, 82,

	190, 24, 194, 26, 162, 23, 168, 28, 153, 52, 156, 59,

	178, 78, 182, 85, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// FRIDAY
	71, 66, 75, 68, 46, 63, 51, 65, 15, 64, 20, 66,

	35, 93, 38, 97, 40, 117, 43, 121, 54, 153, 56, 158,

	103, 110, 107, 112, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// SATURDAY
	142, 190, 147, 192, 115, 191, 120, 193, 85, 195, 90, 197,

	222, 133, 224, 138, 222, 159, 224, 164, 222, 192, 224, 197,

	166, 216, 168, 221, 0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// SUNDAY
	77, 67, 82, 69, 46, 68, 51, 70, 31, 89, 33, 94,

	33, 175, 35, 180, 51, 190, 56, 192, 68, 162, 69, 167,

	155, 131, 160, 133, 195, 131, 201, 133, 224, 158, 226, 163,

	210, 23, 215, 25, 0, 0, 0, 0, 0, 0, 0, 0
};

#endif