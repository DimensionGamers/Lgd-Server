/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "EventPacket.h"
*
*/

#ifndef EVENT_PACKET_H
#define EVENT_PACKET_H

#pragma pack(1)
/* ============================================
		ILLUSION TEMPLE CHAMPIONSHIP
============================================ */
struct ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER_CONFIRM_RESULT
{
	ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER_CONFIRM_RESULT()
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER_CONFIRM, sizeof(ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER_CONFIRM_RESULT));
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER_RESULT
{
	ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER_RESULT()
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER, sizeof(ILLUSION_TEMPLE_CHAMPIONSHIP_ENTER_RESULT));
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};


#pragma pack()

/* ============================================
			ILLUSION TEMPLE
============================================ */

struct ILLUSION_TEMPLE_ENTER_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 ground;
	uint8 slot;
};

struct ILLUSION_TEMPLE_ENTER_RESULT
{
	ILLUSION_TEMPLE_ENTER_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_ILLUSION_TEMPLE_ENTER, sizeof(ILLUSION_TEMPLE_ENTER_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

/* ============================================
			CASTLE_SIEGE
============================================ */
struct CASTLE_SIEGE_COMMAND_RESULT
{
	CASTLE_SIEGE_COMMAND_RESULT(uint8 team, uint8 x, uint8 y, uint8 command)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_COMMAND_REQUEST, sizeof(CASTLE_SIEGE_COMMAND_RESULT));
		this->team = team;
		this->x = x;
		this->y = y;
		this->command = command;
	}
	PBMSG_HEAD2 h;
	uint8 team;
	uint8 x;
	uint8 y;
	uint8 command;
};

struct CASTLE_SIEGE_COMMAND_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 team;
	uint8 x;
	uint8 y;
	uint8 command;
};

struct CASTLE_SIEGE_MINI_MAP_NPC_HEAD
{
    PWMSG_HEAD h;
    int32 count;
};

struct CASTLE_SIEGE_MINI_MAP_NPC_BODY
{
	uint8 npc;
	uint8 x;
	uint8 y;
};

struct CASTLE_SIEGE_MINI_MAP_HEAD
{
    PWMSG_HEAD h;
    int32 count;
};

struct CASTLE_SIEGE_MINI_MAP_BODY
{
	uint8 x;
	uint8 y;
};

struct CASTLE_SIEGE_MACHINE_DAMAGE
{
	PBMSG_HEAD2 h;
	INDEX_DATA(Npc);
};

struct CASTLE_SIEGE_MACHINE_NOTIFY_REGION
{
	CASTLE_SIEGE_MACHINE_NOTIFY_REGION(uint8 type, uint8 x, uint8 y)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MACHINE, SUBCODE_CASTLE_SIEGE_MACHINE_NOTIFY_REGION, sizeof(CASTLE_SIEGE_MACHINE_NOTIFY_REGION));
		this->type = type;
		this->x = x;
		this->y = y;
	}
	PBMSG_HEAD2 h;
	uint8 type;
	uint8 x;
	uint8 y;
};

struct CASTLE_SIEGE_MACHINE_NOTIFY_TARGET
{
	CASTLE_SIEGE_MACHINE_NOTIFY_TARGET(uint16 target, uint8 type)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MACHINE, SUBCODE_CASTLE_SIEGE_MACHINE_NOTIFY_TARGET, sizeof(CASTLE_SIEGE_MACHINE_NOTIFY_TARGET));
		this->SetTarget(target);
		this->type = type;
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Target);
	uint8 type;
};

struct CASTLE_SIEGE_MACHINE_USE_RESULT
{
	CASTLE_SIEGE_MACHINE_USE_RESULT(uint8 result, uint16 index, uint8 type, uint8 x, uint8 y)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MACHINE, SUBCODE_CASTLE_SIEGE_MACHINE_USE, sizeof(CASTLE_SIEGE_MACHINE_USE_RESULT));
		this->result = result;
		this->SetIndex(index);
		this->type = type;
		this->x = x;
		this->y = y;
	}

	PBMSG_HEAD2 h;	// C1:B7:01
	uint8 result;	// [1:Attacker] [2:Targets]	4
	INDEX_DATA(Index);
	uint8 type;
	uint8 x;
	uint8 y;
};

struct CASTLE_SIEGE_MACHINE_USE_REQUEST
{
	PBMSG_HEAD2 h;
	INDEX_DATA(Npc);
	uint8 target;
};

struct CASTLE_SIEGE_MACHINE_INTERFACE
{
	CASTLE_SIEGE_MACHINE_INTERFACE(uint8 result, uint8 type, uint16 index)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MACHINE, SUBCODE_CASTLE_SIEGE_MACHINE_INTERFACE, sizeof(CASTLE_SIEGE_MACHINE_INTERFACE));
		this->result = result;
		this->type = type;
		this->SetIndex(index);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 type;
	INDEX_DATA(Index);
};

struct CASTLE_SIEGE_MONEY_OUT_RESULT
{
	CASTLE_SIEGE_MONEY_OUT_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_MONEY_OUT, sizeof(CASTLE_SIEGE_MONEY_OUT_RESULT));
		this->result = result;
		memset(this->money, 0, 8);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 money[8];
};

struct CASTLE_SIEGE_MONEY_OUT
{
	PBMSG_HEAD2 h;
	uint8 money[4];
};

struct CASTLE_SIEGE_GUILD_REGISTER_HEAD
{
	PWMSG_HEAD h;
	uint8 result;
	int32 count;
};

struct CASTLE_SIEGE_GUILD_REGISTER_BODY
{
	char name[MAX_GUILD_NAME_LENGTH];
	uint8 marks[4];
	uint8 is_give_up;
	uint8 position;
};

struct CASTLE_SIEGE_NPC_LIST_HEAD
{
	PWMSG_HEAD h;
	uint8 result;
	int32 count;
};

struct CASTLE_SIEGE_NPC_LIST_BODY
{
    int32 number;
    int32 index;
    int32 defense_level;
    int32 regen_level;
    int32 max_hp;
    int32 hp;
    uint8 x;
    uint8 y;
    uint8 live;
};

struct CASTLE_SIEGE_NPC_LIST_REQUEST
{
	PBMSG_HEAD h;
	uint8 code;
};

struct CASTLE_SIEGE_HUNT_ZONE_ALLOW_RESULT
{
	CASTLE_SIEGE_HUNT_ZONE_ALLOW_RESULT(uint8 result, uint8 allow)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_ENABLE_HUNT, sizeof(CASTLE_SIEGE_HUNT_ZONE_ALLOW_RESULT));
		this->result = result;
		this->allow = allow;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 allow;
};

struct CASTLE_SIEGE_HUNT_ZONE_ALLOW
{
	PBMSG_HEAD2 h;
	uint8 allow;
};

struct CASTLE_SIEGE_TAX_RATE_CHANGE_RESULT
{
	CASTLE_SIEGE_TAX_RATE_CHANGE_RESULT()
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_TAX_RATE_INFO, sizeof(CASTLE_SIEGE_TAX_RATE_CHANGE_RESULT));
		this->result = 0;
		this->tax_type = 0;
		memset(this->tax_rate, 0, 4);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 tax_type;
	uint8 tax_rate[4];
};

struct CASTLE_SIEGE_TAX_RATE_CHANGE
{
	PBMSG_HEAD2 h;
	uint8 tax_type;
	uint8 tax_rate[4];
};

struct CASTLE_SIEGE_TAX_MONEY
{
	CASTLE_SIEGE_TAX_MONEY()
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_TAX_MONEY_INFO, sizeof(CASTLE_SIEGE_TAX_MONEY));
		this->result = 1;
		this->tax_rate_chaos = 0;
		this->tax_rate_store = 0;
		memset(this->money, 0, 8);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 tax_rate_chaos;
	uint8 tax_rate_store;
	uint8 money[8];
};

struct CASTLE_SIEGE_LAND_OF_TRIALS_MOVE_RESULT
{
	CASTLE_SIEGE_LAND_OF_TRIALS_MOVE_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MISC, 0x05, sizeof(CASTLE_SIEGE_LAND_OF_TRIALS_MOVE_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct CASTLE_SIEGE_LAND_OF_TRIALS_MOVE
{
	PBMSG_HEAD2 h;
	int32 price;
};

struct LIFE_STONE_STATE_SEND
{
	LIFE_STONE_STATE_SEND(uint16 npc, uint8 state)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MISC, SUBCODE_CASTLE_SIEGE_MISC_LIFE_STONE_STATE, sizeof(LIFE_STONE_STATE_SEND));
		this->SetNpc(npc);
		this->state = state;
	}

	PBMSG_HEAD2 h;
	INDEX_DATA(Npc);
	uint8 state;
};

struct CASTLE_SIEGE_OWNER_MARK
{
	CASTLE_SIEGE_OWNER_MARK(uint8 * emblem)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MISC, SUBCODE_CASTLE_SIEGE_MISC_OWNER_MARK, sizeof(CASTLE_SIEGE_OWNER_MARK));
		memcpy(this->emblem, emblem, 32);
	}
	PBMSG_HEAD2 h;
	uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
};

struct CASTLE_SIEGE_LAND_OF_TRIALS_GUARD
{
	CASTLE_SIEGE_LAND_OF_TRIALS_GUARD()
	{
		this->h.set(HEADCODE_CASTLE_SIEGE_MISC, SUBCODE_CASTLE_SIEGE_MISC_LAND_OF_TRIAL_GUARD, sizeof(CASTLE_SIEGE_LAND_OF_TRIALS_GUARD));
		this->result = 1;
		this->allowed = 0;
		this->current_price = 0;
		this->max_price = 0;
		this->unit_price = 0;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 allowed;
	int32 current_price;
	int32 max_price;
	int32 unit_price;
};

struct CASTLE_SIEGE_NPC_UPGRADE_RESULT
{
	CASTLE_SIEGE_NPC_UPGRADE_RESULT(uint8 result, int32 npc, int32 id, int32 type, int32 value)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_NPC_UPGRADE, sizeof(CASTLE_SIEGE_NPC_UPGRADE_RESULT));
		this->result = result;
		this->npc = npc;
		this->id = id;
		this->upgrade_type = type;
		this->upgrade_value = value;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	int32 npc;
	int32 id;
	int32 upgrade_type;
	int32 upgrade_value;
};

struct CASTLE_SIEGE_NPC_UPGRADE
{
	PBMSG_HEAD2 h;
	int32 npc;
	int32 id;
	int32 upgrade_type;
	int32 upgrade_value;
};

struct CASTLE_SIEGE_GATE_OPEN_STATE
{
	CASTLE_SIEGE_GATE_OPEN_STATE(uint8 operation, uint16 index)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_GATE_OPERATE_STATE, sizeof(CASTLE_SIEGE_GATE_OPEN_STATE));
		this->operation = operation;
		this->SetIndex(index);
	}
	PBMSG_HEAD2 h;
	uint8 operation;
	INDEX_DATA(Index);
};

struct CASTLE_SIEGE_GATE_OPERATE_RESULT
{
	CASTLE_SIEGE_GATE_OPERATE_RESULT(uint8 result, uint8 operation, uint16 index)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_GATE_OPERATE, sizeof(CASTLE_SIEGE_GATE_OPERATE_RESULT));
		this->result = result;
		this->operation = operation;
		this->SetIndex(index);
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 operation;
	INDEX_DATA(Index);
};

struct CASTLE_SIEGE_GATE_OPERATE
{
	PBMSG_HEAD2 h;
	uint8 operation;
	INDEX_DATA(Index);
};

struct CASTLE_SIEGE_GATE_STATE
{
	CASTLE_SIEGE_GATE_STATE(uint8 result, uint16 index)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_GATE_STATE, sizeof(CASTLE_SIEGE_GATE_STATE));
		this->result = result;
		this->SetIndex(index);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	INDEX_DATA(Index);
};

struct CASTLE_SIEGE_JOIN_SIDE
{
	CASTLE_SIEGE_JOIN_SIDE(uint8 side)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_JOIN_SIDE, sizeof(CASTLE_SIEGE_JOIN_SIDE));
		this->side = side;
	}
	PBMSG_HEAD2 h;
	uint8 side;
};

struct CASTLE_SIEGE_TAX_INFO
{
	CASTLE_SIEGE_TAX_INFO(uint8 type, uint8 rate)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_TAX_RATE, sizeof(CASTLE_SIEGE_TAX_INFO));
		this->type = type;
		this->rate = rate;
	}
	PBMSG_HEAD2 h;
	uint8 type;
	uint8 rate;
};

struct CASTLE_SIEGE_NPC_REPAIR_RESULT
{
	CASTLE_SIEGE_NPC_REPAIR_RESULT(uint8 result, int32 npc, int32 id, int32 hp, int32 max_hp)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_NPC_REPAIR, sizeof(CASTLE_SIEGE_NPC_REPAIR_RESULT));
		this->result = result;
		this->npc = npc;
		this->id = id;
		this->hp = hp;
		this->max_hp = max_hp;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	int32 npc;
	int32 id;
	int32 hp;
	int32 max_hp;
};

struct CASTLE_SIEGE_NPC_REPAIR
{
	PBMSG_HEAD2 h;
	int32 npc;
	int32 id;
};

struct CASTLE_SIEGE_NPC_BUY_RESULT
{
	CASTLE_SIEGE_NPC_BUY_RESULT(uint8 result, int32 npc, int32 id)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_NPC_BUY, sizeof(CASTLE_SIEGE_NPC_BUY_RESULT));
		this->result = result;
		this->npc = npc;
		this->id = id;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	int32 npc;
	int32 id;
};

struct CASTLE_SIEGE_NPC_BUY
{
	PBMSG_HEAD2 h;
	int32 npc;
	int32 id;
};

struct CASTLE_SIEGE_REGISTER_MARK_RESULT
{
	CASTLE_SIEGE_REGISTER_MARK_RESULT(uint8 result, const char * guild, uint32 mark)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_REGISTER_MARK, sizeof(CASTLE_SIEGE_REGISTER_MARK_RESULT));
		this->result = result;
		memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
		this->mark[0] = HIBYTE(HIWORD(mark));
		this->mark[1] = LOBYTE(HIWORD(mark));
		this->mark[2] = HIBYTE(LOWORD(mark));
		this->mark[3] = LOBYTE(LOWORD(mark));
	}
	PBMSG_HEAD2 h;
	uint8 result;
	char guild[MAX_GUILD_NAME_LENGTH];
	uint8 mark[4];
};

struct CASTLE_SIEGE_REGISTER_MARK
{
	PBMSG_HEAD2 h;
	uint8 slot;
};

struct CASTLE_SIEGE_GUILD_REGISTER_INFO
{
	CASTLE_SIEGE_GUILD_REGISTER_INFO(uint8 result, const char * guild, uint32 mark, uint8 ranking)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_REGISTER_GUILD_INFO, sizeof(CASTLE_SIEGE_GUILD_REGISTER_INFO));
		this->result = result;
		memset(this->guild, 0, MAX_GUILD_NAME_LENGTH);
		if ( guild )
		{
			memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
		}
		this->mark[0] = HIBYTE(HIWORD(mark));
		this->mark[1] = LOBYTE(HIWORD(mark));
		this->mark[2] = HIBYTE(LOWORD(mark));
		this->mark[3] = LOBYTE(LOWORD(mark));
		this->giveup = 0;
		this->ranking = ranking;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	char guild[MAX_GUILD_NAME_LENGTH];
	uint8 mark[4];
	uint8 giveup;
	uint8 ranking;
};

struct CASTLE_SIEGE_GIVEUP_GUILD
{
	CASTLE_SIEGE_GIVEUP_GUILD(uint8 result, uint8 giveup, const char * guild)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_GIVEUP_GUILD, sizeof(CASTLE_SIEGE_GIVEUP_GUILD));
		this->result = result;
		this->giveup = giveup;
		memset(this->guild, 0, MAX_GUILD_NAME_LENGTH);
		if ( guild )
		{
			memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
		}
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 giveup;
	char guild[MAX_GUILD_NAME_LENGTH];
};

struct CASTLE_SIEGE_REGISTER_GUILD 
{
	CASTLE_SIEGE_REGISTER_GUILD(uint8 result, const char * guild)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_REGISTER_GUILD, sizeof(CASTLE_SIEGE_REGISTER_GUILD));
		this->result = result;
		memset(this->guild, 0, MAX_GUILD_NAME_LENGTH);
		if ( guild )
		{
			memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
		}
	}
	PBMSG_HEAD2 h;
	uint8 result;
	char guild[MAX_GUILD_NAME_LENGTH];
};

struct CROWN_STATE
{
	CROWN_STATE(uint8 state)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_CROWN_STATE, sizeof(CROWN_STATE));
		this->state = state;
	}
    PBMSG_HEAD2 h;
    uint8 state;
};

struct CASTLE_SIEGE_SWITCH_STATE
{
	CASTLE_SIEGE_SWITCH_STATE(uint16 switch_index, uint16 player, uint8 state)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_SWITCH_STATE, sizeof(CASTLE_SIEGE_SWITCH_STATE));
		this->switchH = SET_NUMBERH(switch_index);
		this->switchL = SET_NUMBERL(switch_index);
		this->playerH = SET_NUMBERH(player);
		this->playerL = SET_NUMBERL(player);
		this->state = state;
	}
	PBMSG_HEAD2 h;
	uint8 switchH;
	uint8 switchL;
	uint8 playerH;
	uint8 playerL;
	uint8 state;
};

/*struct CASTLE_SIEGE_SWITCH_INFO
{
	CASTLE_SIEGE_SWITCH_INFO(uint16 Switch, uint8 state, uint8 join_side)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_SWITCH_INFO, sizeof(CASTLE_SIEGE_SWITCH_INFO));
		this->switchH = SET_NUMBERH(Switch);
		this->switchL = SET_NUMBERL(Switch);
		this->state = state;
		this->join_side = join_side;
		memset(this->guild, 0, MAX_GUILD_NAME_LENGTH);
		memset(this->user, 0, MAX_CHARACTER_LENGTH);
	}

	PBMSG_HEAD2 h;
	uint8 switchH;
	uint8 switchL;
	uint8 state;
	uint8 join_side;
	char guild[MAX_GUILD_NAME_LENGTH];
	char user[MAX_CHARACTER_LENGTH];
};*/

struct CASTLE_SIEGE_SWITCH_INFO
{
	CASTLE_SIEGE_SWITCH_INFO(uint16 switch_index, uint8 id, uint8 state, uint8 join_side)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_SWITCH_INFO, sizeof(CASTLE_SIEGE_SWITCH_INFO));
		this->switch_index = switch_index;
		this->id = id;
		this->state = state;
		this->join_side = join_side;
		memset(this->guild, 0, MAX_GUILD_NAME_LENGTH + 1);
		memset(this->user, 0, MAX_CHARACTER_LENGTH + 1);
	}

	PBMSG_HEAD2 h;
	uint16 switch_index;
	uint8 id;
	uint8 state;
	uint8 join_side;
	char guild[MAX_GUILD_NAME_LENGTH + 1];
	char user[MAX_CHARACTER_LENGTH + 1];
};

struct CASTLE_SIEGE_NOTIFY_PROGRESS
{
	CASTLE_SIEGE_NOTIFY_PROGRESS(uint8 state, const char * guild)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_NOTIFY_PROGRESS, sizeof(CASTLE_SIEGE_NOTIFY_PROGRESS));
		this->state = state;
		memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH);
	}
	PBMSG_HEAD2 h;
	uint8 state;
	char guild[MAX_GUILD_NAME_LENGTH];
};
#pragma pack(1)
struct CROWN_ACCESS_STATE 
{
	CROWN_ACCESS_STATE(uint8 state, uint32 accumulated_time)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_CROWN_ACCESS, sizeof(CROWN_ACCESS_STATE));
		this->state = state;
		this->accumulated_time = accumulated_time;
	}

	PBMSG_HEAD2 h;
	uint8 state; // 4
	uint8 unk1; // 5
	uint8 unk2; // 6
	uint8 unk3; // 7
	uint32 accumulated_time; // 8
};
#pragma pack()

struct CASTLE_SIEGE_START_STATE
{
	CASTLE_SIEGE_START_STATE(uint8 state)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_START_STATE, sizeof(CASTLE_SIEGE_START_STATE));
		this->state = state;
	}
	PBMSG_HEAD2 h;
	uint8 state;
};

struct CASTLE_SIEGE_REMAIN_TIME
{
	CASTLE_SIEGE_REMAIN_TIME(uint8 hour, uint8 minute)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_REMAIN_TIME, sizeof(CASTLE_SIEGE_REMAIN_TIME));
		this->hour = hour;
		this->minute = minute;
	}
	PBMSG_HEAD2 h;
	uint8 hour;
	uint8 minute;
};

struct CASTLE_SIEGE_MINI_MAP
{
	CASTLE_SIEGE_MINI_MAP(uint8 result)
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_MINI_MAP, sizeof(CASTLE_SIEGE_MINI_MAP));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};
#pragma pack(push, 1)
struct CASTLE_SIEGE_STATE
{
	CASTLE_SIEGE_STATE()
	{
		this->h.set(HEADCODE_CASTLE_SIEGE, SUBCODE_CASTLE_SIEGE_STATE, sizeof(CASTLE_SIEGE_STATE));
		this->result = 0;
		this->Setstart_year(0);
		this->start_month = 0;
		this->start_day = 0;
		this->start_hour = 0;
		this->start_minute = 0;
		this->Setend_year(0);
		this->end_month = 0;
		this->end_day = 0;
		this->end_hour = 0;
		this->end_minute = 0;
		this->Setsiege_start_year(0);
		this->siege_start_month = 0;
		this->siege_start_day = 0;
		this->siege_start_hour = 0;
		this->siege_start_minute = 0;
		memset(this->guild, 0, MAX_GUILD_NAME_LENGTH);
		memset(this->guild_master, 0, MAX_CHARACTER_LENGTH);
		memset(this->remain_time, 0, 4);
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 state;
	INDEX_DATA(start_year);
	uint8 start_month;
	uint8 start_day;
	uint8 start_hour;
	uint8 start_minute;
	INDEX_DATA(end_year);
	uint8 end_month;
	uint8 end_day;
	uint8 end_hour;
	uint8 end_minute;
	INDEX_DATA(siege_start_year);
	uint8 siege_start_month;
	uint8 siege_start_day;
	uint8 siege_start_hour;
	uint8 siege_start_minute;
	char guild[MAX_GUILD_NAME_LENGTH];
	char guild_master[MAX_CHARACTER_LENGTH];
	uint8 remain_time[4];
};
#pragma pack(pop)
struct CASTLE_SIEGE_GUILD_LIST_BODY
{
	uint8 side;
	uint8 involved;
	char guild[MAX_GUILD_NAME_LENGTH];
	int32 score;
};

struct CASTLE_SIEGE_GUILD_LIST_HEAD
{
	PWMSG_HEAD h;
	uint8 result;
	int32 count;
};

/* ============================================
			KANTURU
============================================ */

struct KANTURU_WIDE_ATTACK_AREA
{
	KANTURU_WIDE_ATTACK_AREA(uint16 index, uint8 type)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_WIDE_ATTACK, sizeof(KANTURU_WIDE_ATTACK_AREA));
		this->SetIndex(index);
		this->type = type;
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Index);
	uint8 type;
};

struct KANTURU_SUCCESS
{
	KANTURU_SUCCESS(uint8 result)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_RESULT, sizeof(KANTURU_SUCCESS));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct KANTURU_SCENE_TIME
{
	KANTURU_SCENE_TIME(int32 time)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_TIME, sizeof(KANTURU_SCENE_TIME));
		this->time = time;
	}
	PBMSG_HEAD2 h;
	int32 time;
};

struct KANTURU_ENTRANCE_INFO
{
	KANTURU_ENTRANCE_INFO(uint8 state, uint8 sub_state)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_NOTIFY_ENTRANCE, sizeof(KANTURU_ENTRANCE_INFO));
		this->state = state;
		this->sub_state = sub_state;
	}
	PBMSG_HEAD2 h;
	uint8 state;
	uint8 sub_state;
	uint8 enter;
	uint8 user_count;
	int32 remain_time;
};

struct KANTURU_MONSTER_AND_USER_COUNT
{
	KANTURU_MONSTER_AND_USER_COUNT(uint8 mcount, uint8 pcount)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_M_AND_P_COUNT, sizeof(KANTURU_MONSTER_AND_USER_COUNT));
		this->monster_count = mcount;
		this->user_count = pcount;
	}

	PBMSG_HEAD2 h;
	uint8 monster_count;
	uint8 user_count;
};

struct KANTURU_STATE_CHANGE
{
	KANTURU_STATE_CHANGE(uint8 state_, uint8 sub_state_)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_STATUS_CHANGE, sizeof(KANTURU_STATE_CHANGE));
		this->state = state_;
		this->sub_state = sub_state_;
	}

	PBMSG_HEAD2 h;
	uint8 state;
	uint8 sub_state;
};

struct KANTURU_STATE_NOTIFY
{
	KANTURU_STATE_NOTIFY(uint8 state_, uint8 sub_state_)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_STATUS, sizeof(KANTURU_STATE_NOTIFY));
		this->state = state_;
		this->sub_state = sub_state_;
	}

	PBMSG_HEAD2 h;
	uint8 state;
	uint8 sub_state;
};

struct KANTURU_ENTER_RESULT
{
	KANTURU_ENTER_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_KANTURU, SUBCODE_KANTURU_ENTER, sizeof(KANTURU_ENTER_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct EVENT_MESSAGE_SEND
{
	EVENT_MESSAGE_SEND(uint8 _type)
	{
		this->h.set(HEADCODE_EVENT_MESSAGE,sizeof(EVENT_MESSAGE_SEND));
		this->type = _type;
	}

	PBMSG_HEAD h;
	uint8 type;
};

/* ============================================
			CRYWOLF
============================================ */

struct CRYWOLF_PERSONAL_RANK
{
	CRYWOLF_PERSONAL_RANK(uint8 ranking, int32 experience)
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_PERSONAL_RANK, sizeof(CRYWOLF_PERSONAL_RANK));
		this->ranking = ranking;
		this->experience = experience;
	}
	PBMSG_HEAD2 h;
	uint8 ranking;
	int32 experience;
};

struct CRYWOLF_HERO_LIST_HEAD
{
	PBMSG_HEAD2 h;
	uint8 count;
};

struct CRYWOLF_HERO_LIST_BODY
{
	uint8 ranking;
	char name[MAX_CHARACTER_LENGTH];
	int32 score;
	uint8 race;
};

struct CRYWOLF_BENEFIT_PLUS_CHAOS_RATE
{
	CRYWOLF_BENEFIT_PLUS_CHAOS_RATE(uint8 rate)
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_PLUS, sizeof(CRYWOLF_BENEFIT_PLUS_CHAOS_RATE));
		this->rate = rate;
	}
	PBMSG_HEAD2 h;
	uint8 rate;
};

struct CRYWOLF_STATE_INFO
{
	CRYWOLF_STATE_INFO(uint8 occupation, uint8 state)
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_STATE_INFO, sizeof(CRYWOLF_STATE_INFO));
		this->occupation = occupation;
		this->state = state;
	}
	PBMSG_HEAD2 h;
	uint8 occupation;
	uint8 state;
};

struct CRYWOLF_MONSTER_INFO
{
	CRYWOLF_MONSTER_INFO()
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_MONSTER_INFO, sizeof(CRYWOLF_MONSTER_INFO));
		this->balgass_hp = -1;
		this->dark_elf_count = 0;
	}
	PBMSG_HEAD2 h;
	int32 balgass_hp;
	uint8 dark_elf_count;
};

struct CRYWOLF_LEFT_TIME
{
	CRYWOLF_LEFT_TIME(uint8 hour, uint8 minute)
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_TIME_LEFT, sizeof(CRYWOLF_LEFT_TIME));
		this->hour = hour;
		this->minute = minute;
	}

	PBMSG_HEAD2 h;
	uint8 hour;
	uint8 minute;
};

struct CRYWOLF_STATUE_AND_ALTAR_DATA
{
	CRYWOLF_STATUE_AND_ALTAR_DATA()
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_ALTAR_STATUE, sizeof(CRYWOLF_STATUE_AND_ALTAR_DATA));
		this->statue_hp = 0;

		for ( uint8 i = 0; i < CRYWOLF_ALTAR_MAX; ++i )
		{
			this->altar[i] = 0;
		}
	}

	PBMSG_HEAD2 h;
	int32 statue_hp;
	uint8 altar[CRYWOLF_ALTAR_MAX];
};

struct CRYWOLF_ALTAR_RESULT
{
	CRYWOLF_ALTAR_RESULT(uint8 result, uint8 state, uint16 altar)
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_CONTRACT, sizeof(CRYWOLF_ALTAR_RESULT));
		this->result = result;
		this->state = state;
		this->SetAltar(altar);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 state;
	INDEX_DATA(Altar);
};

struct CRYWOLF_ALTAR_REQUEST
{
	PBMSG_HEAD2 h;
	INDEX_DATA(Altar);
};

/* ============================================
			BLOOD CASTLE
============================================ */

struct BLOOD_CASTLE_SCORE_DATA
{
	char name[MAX_CHARACTER_LENGTH];
	int32 score;
	int32 exp;
	uint32 zen;
};

#pragma pack(1)
struct BLOOD_CASTLE_SCORE
{
	BLOOD_CASTLE_SCORE(bool completed)
	{
		this->h.set(HEADCODE_BLOOD_CASTLE_SCORE, sizeof(BLOOD_CASTLE_SCORE));
		this->completed = completed;
		this->count = uint8(-1);
		memset(this->data.name, 0, MAX_CHARACTER_LENGTH);
		this->data.score = 0;
		this->data.exp = 0;
		this->data.zen = 0;
	}
	PBMSG_HEAD h;
	bool completed;
	uint8 count;
	BLOOD_CASTLE_SCORE_DATA data;
};
#pragma pack()

struct BLOOD_CASTLE_STATE
{
	BLOOD_CASTLE_STATE(uint8 _state)
	{
		this->h.set(HEADCODE_BLOOD_CASTLE_STATE,sizeof(BLOOD_CASTLE_STATE));
		this->state = _state;
		this->remain_seconds = 0;
		this->monsters_max = 0;
		this->monsters_current = 0;
		this->weapon_user = uint16(-1);
		this->weapon_type = 0;
	}
	PBMSG_HEAD h;
	uint8 state;
	uint16 remain_seconds;
	uint16 monsters_max;
	uint16 monsters_current;
	uint16 weapon_user;
	uint8 weapon_type;
};

struct BLOOD_CASTLE_ENTER_RESULT
{
	BLOOD_CASTLE_ENTER_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_BLOOD_CASTLE_ENTER_REQUEST,sizeof(BLOOD_CASTLE_ENTER_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD h;
	uint8 result;
};

struct BLOOD_CASTLE_ENTER_REQUEST
{
	PBMSG_HEAD h;
	uint8 ground;
	uint8 item_slot;
};

/* ============================================
			DEVIL SQUARE
============================================ */

struct DEVIL_SQUARE_ENTER_RESULT
{
	DEVIL_SQUARE_ENTER_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_DEVIL_SQUARE_ENTER_REQUEST,sizeof(DEVIL_SQUARE_ENTER_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD h;
	uint8 result;
};

struct DEVIL_SQUARE_ENTER_REQUEST
{
	PBMSG_HEAD h;
	uint8 ground;
	uint8 item_slot;
};

struct DEVIL_SQUARE_SCORE_BODY
{
	char name[MAX_CHARACTER_LENGTH];
	int32 score;
	int32 exp;
	uint32 zen;
};

#pragma pack(1)
struct DEVIL_SQUARE_SCORE_HEAD
{
	PBMSG_HEAD h;
	uint8 my_rank;
	uint8 count;
};
#pragma pack()

/* ============================================
			CHAOS CASTLE
============================================ */

struct CHAOS_CASTLE_ENTER_RESULT
{
	CHAOS_CASTLE_ENTER_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_CHAOS_CASTLE,SUBCODE_CHAOS_CASTLE_ENTER_REQUEST,sizeof(CHAOS_CASTLE_ENTER_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct CHAOS_CASTLE_ENTER_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 ground;
	uint8 item_slot;
};

struct CHAOS_CASTLE_STATE
{
	CHAOS_CASTLE_STATE(uint8 state)
	{
		this->h.set(0x9B,sizeof(CHAOS_CASTLE_STATE));
		this->state = state;
		this->remain_seconds = 0;
		this->monsters_max = 0;
		this->monsters_remain = 0;
		this->weapon_user = uint16(-1);
		this->weapon_type = 0;
	}
	PBMSG_HEAD h;
	uint8 state;
	uint16 remain_seconds;
	uint16 monsters_max;
	uint16 monsters_remain;
	uint16 weapon_user;
	uint8 weapon_type;
};
//#pragma pack(1)
struct CHAOS_CASTLE_REWARD_DATA
{
	char name[MAX_CHARACTER_LENGTH];
	int32 killed_monster;
	int32 experience;
	uint32 killed_user;
};

#pragma pack(1)
struct CHAOS_CASTLE_REWARD
{
	CHAOS_CASTLE_REWARD(bool completed)
	{
		this->h.set(HEADCODE_BLOOD_CASTLE_SCORE, sizeof(CHAOS_CASTLE_REWARD));
		this->completed = completed;
		this->count = 0xFE;
		memset(this->data.name, 0, MAX_CHARACTER_LENGTH);
		this->data.killed_monster = 0;
		this->data.experience = 0;
		this->data.killed_user = 0;
	}
	PBMSG_HEAD h;
	bool completed;
	uint8 count;
	CHAOS_CASTLE_REWARD_DATA data;
};
#pragma pack()

struct CHAOS_CASTLE_BATTLE_REWARD
{
	CHAOS_CASTLE_BATTLE_REWARD()
	{
		this->h.set(0x93, 0x01, sizeof(CHAOS_CASTLE_BATTLE_REWARD));
		memset(this->name, 0, MAX_CHARACTER_LENGTH);
		this->winner = 0;
		this->score = 0;
		this->killed_players = 0;
		this->killed_monsters = 0;
	}

	PBMSG_HEAD2 h;
	char name[MAX_CHARACTER_LENGTH];
	uint8 winner;
	int32 score;
	int32 killed_players;
	int32 killed_monsters;
};

/* ============================================
			DOUBLE GOER
============================================ */
#pragma pack(1)
struct DOPPELGANGER_ENTER
{
	PBMSG_HEAD2 h;
	uint8 item_slot;
};
#pragma pack()
/*
	Enter Packet 0xBF 0x0E
	Result = 0 -> Enter
	Result = 1 -> Deshabilita el boton
	Result = 2 -> Battle has already commenced, you cant go
	Result = 3 -> You can't enter if you are 1st PK
	Result = 6 -> Time to enter Doppleganger has passed
	Result = 7 -> Need a 5 person party
	Result = 8 -> All party members need to be same level
	Result = 9 -> Me lo deja deshabilitado hasta que relogueo
*/

struct DOPPELGANGER_ENTER_RESULT
{
	DOPPELGANGER_ENTER_RESULT(uint8 _result)
	{
		this->h.set(HEADCODE_EVENT_MISC,SUBCODE_DOUBLE_GOER_ENTER,sizeof(DOPPELGANGER_ENTER_RESULT));
		this->result = _result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct DOPPELGANGER_TIMER_INIT
{
	DOPPELGANGER_TIMER_INIT(uint8 timer)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_DOUBLE_GOER_TIMER_INIT, sizeof(DOPPELGANGER_TIMER_INIT));
		this->timer = timer;
	}
	PBMSG_HEAD2 h;	
	uint8 timer;
};

#pragma pack(1)
struct DOPPELGANGER_UPADTE_PLAYER_BODY
{
	uint16 index;
	uint8 visible;
	uint8 position;
};

struct DOPPELGANGER_UPADTE_PLAYER
{
	PBMSG_HEAD2 h;
	uint16 time;
	uint16 count;
};
#pragma pack()

struct DOPPELGANGER_BOSS_POSITION
{
	DOPPELGANGER_BOSS_POSITION(uint8 cleared, uint8 position)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_DOUBLE_GOER_BOSS_POSITION, sizeof(DOPPELGANGER_BOSS_POSITION));
		this->cleared = cleared;
		this->position = position;
	}
	PBMSG_HEAD2 h;
	uint8 cleared;
	uint8 position;
};

struct DOPPELGANGER_MONSTER_POSITION
{
	DOPPELGANGER_MONSTER_POSITION(uint8 position)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_DOUBLE_GOER_MONSTER_POSITION, sizeof(DOPPELGANGER_MONSTER_POSITION));
		this->position = position;
	}
	PBMSG_HEAD2 h;
	uint8 position;
};

struct DOPPELGANGER_MONSTER_PASSED
{
	DOPPELGANGER_MONSTER_PASSED(uint8 total, uint8 passed)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_DOUBLE_GOER_MONSTER_PASSED, sizeof(DOPPELGANGER_MONSTER_PASSED));
		this->total = total;
		this->passed = passed;
	}
	PBMSG_HEAD2 h;
	uint8 total;
	uint8 passed;
};

/*
	Result = 0 -> Succesfull Double Goer Exp:%d
	Result = 1 -> Defense failed
	Result = 2 -> Fallaste y los monsters pasaron por el portal
	Result = 3 -> Double goer va a terminar porque el otro party no entro o se fue del evento
*/

struct DOPPELGANGER_END
{
	DOPPELGANGER_END(uint8 result, uint32 experience)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_DOUBLE_GOER_END, sizeof(DOPPELGANGER_END));
		this->result = result;
		this->experience = experience;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint32 experience;
};

/* ============================================
			IMPERIAL FORTRESS
============================================ */



//0xC1, 0x0D, 0xF7, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28	
//Imperial Fort Timer Init
//Status : 01 - StandBy, 02 - Kill Mobs, 03 - Empty, 00 - Standby
struct IMPERIAL_FORTRESS_TIMER // 0x04
{
	IMPERIAL_FORTRESS_TIMER(uint8 status, uint32 time, uint8 monster)
	{
		this->h.set(HEADCODE_IMPERIAL_FORTRESS, SUBCODE_IMPERIAL_FORTRESS_TIMER, sizeof(IMPERIAL_FORTRESS_TIMER));
		this->status = status;
		this->time = time;
		this->monster = monster;
	}
	PBMSG_HEAD2 h;
	uint8 status; // +4
	uint8 day;
	uint8 zone;
	uint32 time; // +8
	uint8 monster; // +12
};

#pragma pack(1)
/*
	End Packet 0xF7 0x06
	Result = 0 -> You have failed
	Result = 1 -> You don't have ticket 1
	Result = 2 -> Has been cleared
*/
//BYTE Packet[13] = {0xC1, 0x0D, 0xF7, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
struct IMPERIAL_FORTRESS_END
{
	IMPERIAL_FORTRESS_END(uint8 result_, uint32 experience_)
	{
		this->h.set(HEADCODE_IMPERIAL_FORTRESS, SUBCODE_IMPERIAL_FORTRESS_FINISH, sizeof(IMPERIAL_FORTRESS_END));
		this->result = result_;
		this->experience = experience_;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 unk1;
	uint8 unk2;
	uint8 unk3;
	uint32 experience;
};

struct IMPERIAL_FORTRESS_ENTER_RESULT
{
	IMPERIAL_FORTRESS_ENTER_RESULT(uint8 result_, uint8 day_, uint8 state_, uint8 weather_, uint32 time_)
	{
		this->h.set(HEADCODE_IMPERIAL_FORTRESS, SUBCODE_IMPERIAL_FORTRESS_ENTER_RESULT, sizeof(IMPERIAL_FORTRESS_ENTER_RESULT));
		this->result = result_;
		this->day = day_;
		this->state = state_;
		this->weather = weather_;
		this->time = time_;
	}
	
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 day;
	uint8 state;
	uint8 weather;
	uint32 time;
};

#pragma pack()

#endif