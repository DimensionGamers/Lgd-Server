/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "QuestPacket.h"
*
*/

#ifndef QUEST_PACKET_H
#define QUEST_PACKET_H

#pragma pack(1)
struct QUEST_GUIDED_OBJECTIVE_SEND
{
	QUEST_GUIDED_OBJECTIVE_SEND()
	{
		this->h.set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_GUIDED_OBJECTIVE, sizeof(QUEST_GUIDED_OBJECTIVE_SEND));
		this->quest = 0;
		this->count = 0;
		this->status = 0;
	}

	PBMSG_HEAD2 h;
	uint16 quest;
	uint8 count;
	uint8 status;

	/*
	1 = Active
	2 = Complete
	*/
};

struct QUEST_GUIDED_COMPLETE_RESULT
{
	QUEST_GUIDED_COMPLETE_RESULT()
	{
		this->h.set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_GUIDED_COMPLETE, sizeof(QUEST_GUIDED_COMPLETE_RESULT));
		this->quest = 0;
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint16 quest;
	uint16 result;
};

struct QUEST_GUIDED_GIVE_UP
{
	PBMSG_HEAD2 h;
	uint16 quest;
	uint16 result;
};

struct QUEST_GUIDED_GIVE_UP_CHAPTER
{
	PBMSG_HEAD2 h;
	uint16 quest;
	uint16 result;
};

struct QUEST_GUIDED_WARP
{
	PBMSG_HEAD2 h;
	uint16 quest;
	uint16 result;
};

struct QUEST_GUIDED_REACH_TARGET
{
	PBMSG_HEAD2 h;
	uint16 quest;
	uint16 result;
};

struct QUEST_GUIDED_WALK
{
	QUEST_GUIDED_WALK()
	{
		this->h.set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_GUIDED_START_MOVING, sizeof(QUEST_GUIDED_WALK));
	}
	PBMSG_HEAD2 h;
};

struct QUEST_EVOLUTION_MONSTER_BODY
{
	int32 monster_class;
	int32 count;
};

struct QUEST_EVOLUTION_MONSTER_KILL 
{
	QUEST_EVOLUTION_MONSTER_KILL(uint8 result_, uint8 quest_id_)
	{
		this->h.set(HEADCODE_QUEST_EVOLUTION_MONSTER_SKILL, sizeof(QUEST_EVOLUTION_MONSTER_KILL));
		this->result = result_;
		this->quest_id = quest_id_;

		for ( uint8 i = 0; i < QUEST_EVOLUTION_MONSTER_COUNT; ++i )
		{
			this->monster[i].monster_class = 0;
			this->monster[i].count = 0;
		}
	}
	PBMSG_HEAD h;
	uint8 unk;
	uint8 result;
	uint8 quest_id;
	uint8 unk2[2];
	QUEST_EVOLUTION_MONSTER_BODY monster[QUEST_EVOLUTION_MONSTER_COUNT];
};

struct QUEST_EVOLUTION_SET_STATE
{
	QUEST_EVOLUTION_SET_STATE(uint8 quest)
	{
		this->h.set(HEADCODE_QUEST_EVOLUTION_SET_STATE_REQUEST, sizeof(QUEST_EVOLUTION_SET_STATE));
		this->quest_id = quest;
	}
	PBMSG_HEAD h;
	uint8 quest_id;
	uint8 state;
};

struct QUEST_EVOLUTION_SET_STATE_RESULT
{
	QUEST_EVOLUTION_SET_STATE_RESULT(uint8 quest)
	{
		this->h.set(HEADCODE_QUEST_EVOLUTION_SET_STATE, sizeof(QUEST_EVOLUTION_SET_STATE_RESULT));
		this->quest_id = quest;
	}
	PBMSG_HEAD h;
	uint8 quest_id;
	uint8 result;
	uint8 state;
};

struct QUEST_EVOLUTION_PRIZE
{
	QUEST_EVOLUTION_PRIZE(uint32 entry, uint8 type, uint8 count)
	{
		this->h.set(HEADCODE_QUEST_EVOLUTION_PRICE, sizeof(QUEST_EVOLUTION_PRIZE));
		this->numberH = SET_NUMBERH(entry);
		this->numberL = SET_NUMBERL(entry);
		this->type = type;
		this->count = count;
	}

	PBMSG_HEAD h;
	uint8 numberH;
	uint8 numberL;
	uint8 type;
	uint8 count;
};

struct QUEST_EVOLUTION_INFO
{
	QUEST_EVOLUTION_INFO()
	{
		this->h.set(HEADCODE_QUEST_EVOLUTION_INFO, sizeof(QUEST_EVOLUTION_INFO) - 50);
		this->count = 0;
		memset(this->state, 0, 50);
	}
	PBMSG_HEAD h;
	uint8 count;
	uint8 state[50];
};

#pragma pack()

#endif