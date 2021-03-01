/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "CrywolfDef.h"
*
*/
#ifndef CRYWOLFDEF_H
#define CRYWOLFDEF_H

#define CRYWOLF_AI			"crywolf_ai"
#define CRYWOLF_STATUE_AI	"crywolf_statue_ai"
#define CRYWOLF_ALTAR_1_AI	"crywolf_altar_ai_1"
#define CRYWOLF_ALTAR_2_AI	"crywolf_altar_ai_2"
#define CRYWOLF_ALTAR_3_AI	"crywolf_altar_ai_3"
#define CRYWOLF_ALTAR_4_AI	"crywolf_altar_ai_4"
#define CRYWOLF_ALTAR_5_AI	"crywolf_altar_ai_5"
#define CRYWOLF_ALTAR_AI	"crywolf_altar_ai_1 crywolf_altar_ai_2 crywolf_altar_ai_3 crywolf_altar_ai_4 crywolf_altar_ai_5"

#define CRYWOLF_ALTAR_MAX			5
#define CRYWOLF_COMMON_INSTANCE		-1000000

enum CrywolfState
{
	CRYWOLF_STATE_NONE,
	CRYWOLF_STATE_NOTIFY_1,
	CRYWOLF_STATE_NOTIFY_2,
	CRYWOLF_STATE_READY,
	CRYWOLF_STATE_START,
	CRYWOLF_STATE_END,
	CRYWOLF_STATE_END_CYCLE,
	CRYWOLF_STATE_MAX
};

enum CrywolfOccupationState
{
	CRYWOLF_OCCUPATION_SUCCESS,
	CRYWOLF_OCCUPATION_FAIL,
	CRYWOLF_OCCUPATION_FIGHT,
	CRYWOLF_OCCUPATION_MAX
};

enum CrywolfAltarState
{
	CRYWOLF_ALTAR_STATE_ENABLED,
	CRYWOLF_ALTAR_STATE_CONTRACTED,
	CRYWOLF_ALTAR_STATE_CONTRACT_ATTEMPT,
	CRYWOLF_ALTAR_STATE_DISABLED,
	CRYWOLF_ALTAR_STATE_MAX
};

enum CrywolfMVPScore
{
	CRYWOLF_MVP_SCORE_BALGASS,
	CRYWOLF_MVP_SCORE_DARKELF,
	CRYWOLF_MVP_SCORE_BALRAM,
	CRYWOLF_MVP_SCORE_SORAM,
	CRYWOLF_MVP_SCORE_DEATHSPIRIT,
	CRYWOLF_MVP_SCORE_TANKER,
	CRYWOLF_MVP_SCORE_CONTRACTOR,
	CRYWOLF_MVP_SCORE_MAX
};

enum CrywolfMVPRank
{
	CRYWOLF_MVP_RANK_D,
	CRYWOLF_MVP_RANK_C,
	CRYWOLF_MVP_RANK_B,
	CRYWOLF_MVP_RANK_A,
	CRYWOLF_MVP_RANK_S,
	CRYWOLF_MVP_RANK_MAX
};

static const uint8 CrywolfStatueBuff[CRYWOLF_ALTAR_MAX + 1] =
{
	0,
	BUFF_POISON,
	BUFF_ICE,
	BUFF_GREATER_ATTACK,
	BUFF_GREATER_DEFENSE,
	BUFF_MANA_SHIELD
};

struct CrywolfStateData
{
	DECLARE_ENUM(int32, Duration);
};

struct CrywolfRankingData
{
	void Reset()
	{
		this->SetScore(0);
		this->SetExperience(0);
	}

	DECLARE_ENUM(int32, Score);
	DECLARE_ENUM(int32, Experience);
};

#endif