/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "RaklionDef.h"
*
*/
#ifndef RAKLION_DEF_H
#define RAKLION_DEF_H

#define RAKLION_AI_EGGS		"raklion_egg_ai"
#define RAKLION_AI_SUMMON	"raklion_summon_ai"
#define RAKLION_AI_SELUPAN	"raklion_selupan_ai"

enum RaklionState
{
	RAKLION_STATE_NONE,
	RAKLION_STATE_CLOSED,
	RAKLION_STATE_OPEN,
	RAKLION_STATE_STANDBY_1,
	RAKLION_STATE_STANDBY_2,
	RAKLION_STATE_KILL_SELUPAN,
	RAKLION_STATE_END,
	RAKLION_STATE_MAX
};

struct RaklionStateData
{
	DECLARE_ENUM(int32, Duration);
	DECLARE_BOOL(EntraceAllowed);
	DECLARE_BOOL(PKAllowed);

	void Reset()
	{
		this->SetDuration(0);
		this->SetEntraceAllowed(false);
		this->SetPKAllowed(false);
	}
};

#endif