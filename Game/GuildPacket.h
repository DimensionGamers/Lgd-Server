/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GuildPacket.h"
*
*/

#ifndef GUILD_PACKET_H
#define GUILD_PACKET_H

struct GUILD_WAR_END
{
	GUILD_WAR_END(uint8 result, const char * guild)
	{
		this->h.set(HEADCODE_GUILD_WAR_END, sizeof(GUILD_WAR_END));
		this->result = result;
		memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
	}

	PBMSG_HEAD h;
	uint8 result;
	char guild[MAX_GUILD_NAME_LENGTH];
};

struct GUILD_WAR_SCORE
{
	GUILD_WAR_SCORE(uint8 score01, uint8 score02, uint8 type)
	{
		this->h.set(HEADCODE_GUILD_WAR_SCORE, sizeof(GUILD_WAR_SCORE));
		this->score01 = score01;
		this->score02 = score02;
		this->type = type;
	}

	PBMSG_HEAD h;
	uint8 score01;
	uint8 score02;
	uint8 type;
};

struct GUILD_WAR_START
{
	GUILD_WAR_START(const char * guild, uint8 type, uint8 team)
	{
		this->h.set(HEADCODE_GUILD_WAR_START, sizeof(GUILD_WAR_START));
		memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
		this->type = type;
		this->team = team;
	}

	PBMSG_HEAD h;
	char guild[MAX_GUILD_NAME_LENGTH];
	uint8 type;
	uint8 team;
};

struct GUILD_WAR_REQUEST_SEND
{
	GUILD_WAR_REQUEST_SEND(const char * guild, uint8 type)
	{
		this->h.set(HEADCODE_GUILD_WAR, sizeof(GUILD_WAR_REQUEST_SEND));
		memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
		this->type = type;
	}

	PBMSG_HEAD h;
	char guild[MAX_GUILD_NAME_LENGTH];
	uint8 type;
};

struct GUILD_WAR_REQUEST_RESULT
{
	GUILD_WAR_REQUEST_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_GUILD_WAR_REQUEST, sizeof(GUILD_WAR_REQUEST_RESULT));
		this->result = result;
	}

	PBMSG_HEAD h;
	uint8 result;
};

struct GUILD_NPC_ANSWER_RESULT
{
	GUILD_NPC_ANSWER_RESULT()
	{
		this->h.set(HEADCODE_GUILD_CREATE, sizeof(GUILD_NPC_ANSWER_RESULT));
	}
	PBMSG_HEAD h;
};

struct GUILD_NPC_ANSWER
{
	PBMSG_HEAD h;
	uint8 result;
};

struct GUILD_VIEWPORT_HEAD
{
	PWMSG_HEAD h;
	uint8 count;
};

struct GUILD_VIEWPORT_BODY
{
	INDEX_DATA(Index);
	uint32 guild;
	uint8 relationship;
	char alliance[MAX_GUILD_NAME_LENGTH];
};

struct GUILD_KICK_UNION
{
	GUILD_KICK_UNION(uint8 result, uint8 request_type, uint8 relationship)
	{
		this->h.set(HEADCODE_GUILD_ALLIANCE_KICK, 0x01, sizeof(GUILD_KICK_UNION));
		this->result = result;
		this->request_type = request_type;
		this->relationship = relationship;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 request_type;
	uint8 relationship;
};

struct GUILD_ALLIANCE_KICK
{
	PBMSG_HEAD2 h;
	char name[MAX_GUILD_NAME_LENGTH];
};

struct GUILD_ALLIANCE_LIST_HEAD
{
	PWMSG_HEAD h;
	uint8 count;
	uint8 result;
	uint8 master_hostil;
	uint8 master;
};

struct GUILD_ALLIANCE_LIST_BODY
{
	uint8 members;
	uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
	char name[MAX_GUILD_NAME_LENGTH];
};

struct GUILD_VIEWPORT_DEL
{
	GUILD_VIEWPORT_DEL(uint16 number, bool master)
	{
		this->h.set(HEADCODE_GUILD_VIEWPORT_DEL, sizeof(GUILD_VIEWPORT_DEL));
		this->numberH = SET_NUMBERH(number) & 0x7F;
		this->numberL = SET_NUMBERL(number);

		if ( master )
		{
			this->numberH |= 0x80;
		}
	}

	PBMSG_HEAD h;
	uint8 numberH;
	uint8 numberL;
};
#pragma pack(1)
struct GUILD_MEMBER_DELETE_REQUEST
{
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
	char secure_code[20];
};
#pragma pack()
struct GUILD_MEMBER_DELETE_RESULT
{
	GUILD_MEMBER_DELETE_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_GUILD_MEMBER_DEL,sizeof(GUILD_MEMBER_DELETE_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD h;
	uint8 result;
};

struct GUILD_RELATIONSHIP_REQUEST_SEND
{
	GUILD_RELATIONSHIP_REQUEST_SEND(uint8 _relation, uint8 _type, uint16 target)
	{
		this->h.set(HEADCODE_GUILD_RELATIONSHIP_REQUEST, sizeof(GUILD_RELATIONSHIP_REQUEST_SEND));
		this->relation_ship = _relation;
		this->request_type = _type;
		this->targetH = SET_NUMBERH(target);
		this->targetL = SET_NUMBERL(target);
	}
	PBMSG_HEAD h;
	uint8 relation_ship;
	uint8 request_type;
	uint8 targetH;
	uint8 targetL;
};
#pragma pack(1)
struct GUILD_RELATIONSHIP_REQUEST
{
	uint16 GetTarget()
	{
		return MAKE_NUMBERW(this->targetH,this->targetL);
	}
	PBMSG_HEAD h;
	uint8 relationship;
	uint8 request_type;
	uint8 targetH;
	uint8 targetL;
	uint8 junk[9];
};
#pragma pack()
struct GUILD_RELATIONSHIP_RESULT
{
	uint16 GetTarget()
	{
		return MAKE_NUMBERW(this->targetH, this->targetL);
	}

	GUILD_RELATIONSHIP_RESULT(uint8 _relation, uint8 _type, uint8 _result, uint16 target)
	{
		this->h.set(HEADCODE_GUILD_RELATIONSHIP_ANSWER, sizeof(GUILD_RELATIONSHIP_RESULT));
		this->relation_ship = _relation;
		this->request_type = _type;
		this->result = _result;
		this->targetH = SET_NUMBERH(target);
		this->targetL = SET_NUMBERL(target);
	}
	PBMSG_HEAD h;
	uint8 relation_ship;
	uint8 request_type;
	uint8 result;
	uint8 targetH;
	uint8 targetL;
};

struct GUILD_ASSIGN_TYPE_REQUEST
{
	PBMSG_HEAD h;
	uint8 type;
};

struct GUILD_ASSIGN_TYPE_RESULT
{
	GUILD_ASSIGN_TYPE_RESULT(uint8 _type, uint8 _result)
	{
		this->h.set(HEADCODE_GUILD_ASSIGN_TYPE,sizeof(GUILD_ASSIGN_TYPE_RESULT));
		this->type = _type;
		this->result = _result;
	}
	PBMSG_HEAD h;
	uint8 type;
	uint8 result;
};

struct GUILD_ASSIGN_REQUEST
{
	PBMSG_HEAD h;
	uint8 type;
	uint8 status;
	char name[MAX_CHARACTER_LENGTH];
};

struct GUILD_ASSIGN_RESULT
{
	GUILD_ASSIGN_RESULT(uint8 type, uint8 result, char * name)
	{
		this->h.set(HEADCODE_GUILD_ASSIGN_STATUS,sizeof(GUILD_ASSIGN_RESULT));
		this->type = type;
		this->result = result;
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD h;
	uint8 type;
	uint8 result;
	char name[MAX_CHARACTER_LENGTH];
};

struct GUILD_JOIN_ANSWER
{
	uint16 GetTarget()
	{
		return MAKE_NUMBERW(this->numberH, this->numberL);
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 numberH;
	uint8 numberL;
};

struct GUILD_JOIN_RESULT
{
	GUILD_JOIN_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_GUILD_ANSWER, sizeof(GUILD_JOIN_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD h;
	uint8 result;
};

struct GUILD_JOIN_REQUEST_SEND
{
	GUILD_JOIN_REQUEST_SEND(uint16 number)
	{
		this->h.set(HEADCODE_GUILD_REQUEST, sizeof(GUILD_JOIN_REQUEST_SEND));
		this->numberH = SET_NUMBERH(number);
		this->numberL = SET_NUMBERL(number);
	}

	PBMSG_HEAD h;
	uint8 numberH;
	uint8 numberL;
};

struct GUILD_JOIN_REQUEST
{
	uint16 GetTarget()
	{
		return MAKE_NUMBERW(this->numberH,this->numberL);
	}
	PBMSG_HEAD h;
	uint8 numberH;
	uint8 numberL;
};

struct GUILD_VIEWPORT
{
	GUILD_VIEWPORT()
	{
		this->h.set(HEADCODE_GUILD_UPDATE_VIEWPORT, sizeof(GUILD_VIEWPORT));
	}
	PBMSG_HEAD h;
	uint32 id;
	uint8 type;
	char alliance[MAX_GUILD_NAME_LENGTH];
	char name[MAX_GUILD_NAME_LENGTH];
	uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
};

struct GUILD_VIEWPORT_REQUEST
{
	PBMSG_HEAD h;
	uint32 id;
};

struct GUILD_LIST_ALL_BODY
{
	char name[MAX_CHARACTER_LENGTH];
	uint8 Number;
	uint8 server;
	uint8 ranking;
};

struct GUILD_LIST_ALL_HEAD
{
	PWMSG_HEAD h;
	uint8 result;
	uint8 count;
	int32 total_score;
	uint8 score;
	char rival[MAX_GUILD_NAME_LENGTH + 1];
	uint8 rival_count;
	uint8 rival_emblem[MAX_GUILD_EMBLEM_LENGTH];
};

#pragma pack(1)
struct GUILD_UPDATE_VIEWPORT_COMPLETE
{
	GUILD_UPDATE_VIEWPORT_COMPLETE()
	{
		this->h.set(HEADCODE_GUILD_UPDATE_COMPLETE, sizeof(GUILD_UPDATE_VIEWPORT_COMPLETE));
		this->count = 1;
	}

	PWMSG_HEAD h;
	uint8 count;
	uint32 guild_id;
	uint8 ranking;
	uint8 type;
	uint8 relationship;
	uint8 numberH;
	uint8 numberL;
	uint8 owner_status;
};
#pragma pack()

struct GUILD_CREATE_RESULT
{
	GUILD_CREATE_RESULT(uint8 _result, uint8 _type)
	{
		this->h.set(HEADCODE_GUILD_CREATE_RESULT,sizeof(GUILD_CREATE_RESULT));
		this->result = _result;
		this->type = _type;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 type;
};

struct GUILD_CREATE
{
	PBMSG_HEAD h;
	uint8 type;
	char name[MAX_GUILD_NAME_LENGTH];
	uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
};

struct GUILD_CREATE_MENU
{
	GUILD_CREATE_MENU()
	{
		this->h.set(HEADCODE_GUILD_NPC_ANSWER,sizeof(GUILD_CREATE_MENU));
	}
	PBMSG_HEAD h;
};

#endif