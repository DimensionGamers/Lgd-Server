/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GenPacket.h"
*
*/

#ifndef GEN_PACKET_H
#define GEN_PACKET_H

struct GEN_JOIN_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 family;
};

struct GEN_JOIN_RESULT
{
	GEN_JOIN_RESULT(uint8 _result, uint8 _family)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_GEN_SYSTEM_JOIN_RESULT, sizeof(GEN_JOIN_RESULT));
		this->result = _result;
		this->family = _family;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 family;
};

struct GEN_DELETE_REQUEST
{
	PBMSG_HEAD2 h;
};

struct GEN_DELETE_RESULT
{
	GEN_DELETE_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_GEN_SYSTEM_DELETE_RESULT, sizeof(GEN_DELETE_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct GEN_REWARD_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 family;
};

struct GEN_REWARD_RESULT
{
	GEN_REWARD_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_GEN_SYSTEM_REWARD_RESULT, sizeof(GEN_REWARD_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};
#pragma pack(1)
struct GEN_STATUS
{
	GEN_STATUS(uint8 _family, int32 _ranking, uint8 _level, int32 _cont, int32 _next)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_GEN_SYSTEM_STATUS, sizeof(GEN_STATUS));
		this->family = _family;
		this->ranking = _ranking;
		this->level = _level;
		this->contribution = _cont;
		this->next_contribution = _next;
	}
	PBMSG_HEAD2 h;
	int32 family;
	int32 ranking;
	int32 level;
	int32 contribution;
	int32 next_contribution;
};
#pragma pack()
#endif