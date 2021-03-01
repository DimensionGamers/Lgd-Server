/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "BattleSoccerDef.h"
*
*/
#ifndef BATTLE_SOCCER_DEF_H
#define BATTLE_SOCCER_DEF_H

enum BattleSoccerState
{
	BATTLE_SOCCER_STATE_NONE,
	BATTLE_SOCCER_STATE_STANDBY,
	BATTLE_SOCCER_STATE_PLAYING,
	BATTLE_SOCCER_STATE_PLAYEND
};

enum
{
	BATTLE_SOCCER_END_REASON_SCORE,
	BATTLE_SOCCER_END_REASON_LEFT,
	BATTLE_SOCCER_END_REASON_TIME_OUT,
};

static const struct BattleSoccerZones
{
	coord_type x1;
	coord_type y1;
	coord_type x2;
	coord_type y2;
} g_BattleSoccerZones[GUILD_WAR_TEAM_MAX] = {
	60, 181, 64, 182,
	60, 139, 64, 140
}, g_BattleSoccerStartZone[GUILD_WAR_TEAM_MAX] = {
	59, 153, 64, 153,
	59, 164, 64, 164
}, g_BattleSoccerGround = {
	54, 141, 70, 180
}, g_BattleSoccerFullGround = {
	54, 138, 70, 183
};

/*
WarEnd ID

0 = Lost
1 = Win
2 = Win Guild Master Left
3 = Lost Guild Master Left
4 = Oposing Guild Disbanded
*/

#endif