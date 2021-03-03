/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ClientPacket.h"
*
*/

#ifndef CLIENT_PACKET_H
#define CLIENT_PACKET_H
#pragma pack(1)
struct MU_HELPER_PLUS_90 : C1_HEADER_SUB
{
	MU_HELPER_PLUS_90()
	{
		this->Set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_PLUS_90, sizeof(MU_HELPER_PLUS_90));
	}

	uint8 result;
};

struct MU_HELPER_PLUS_88 : C1_HEADER_SUB
{
	MU_HELPER_PLUS_88()
	{
		this->Set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_PLUS_88, sizeof(MU_HELPER_PLUS_88));
	}

	uint8 result;
};

struct MU_HELPER_PLUS_86 : C1_HEADER_SUB
{
	MU_HELPER_PLUS_86()
	{
		this->Set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_PLUS_86, sizeof(MU_HELPER_PLUS_86));
	}
};

struct MU_HELPER_PLUS_RUN_SEND : C1_HEADER_SUB
{
	MU_HELPER_PLUS_RUN_SEND()
	{
		this->Set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_PLUS_RUN_SEND, sizeof(MU_HELPER_PLUS_RUN_SEND));
		this->result = 1;
	}

	uint8 result;
};

struct MU_HELPER_PLUS_83 : C1_HEADER_SUB
{
	MU_HELPER_PLUS_83()
	{
		this->Set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_PLUS_83, sizeof(MU_HELPER_PLUS_83));
	}

	uint8 result;

	/*
	Result

	0 = The selected target is not allowed to join the party
	*/
};

struct MU_HELPER_PLUS_82 : C1_HEADER_SUB
{
	MU_HELPER_PLUS_82()
	{
		this->Set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_PLUS_82, sizeof(MU_HELPER_PLUS_82));
	}

	uint8 result;
	uint8 data1;
	uint8 junk[2];
	uint32 data2;
	uint32 data3;

	/*
	Result 

	1 = Success

	Data1

	0 = Moving
	1 = Waiting for party members
	*/
};

struct MU_HELPER_PLUS_81 : C1_HEADER_SUB
{
	MU_HELPER_PLUS_81()
	{
		this->Set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_PLUS_81, sizeof(MU_HELPER_PLUS_81));
	}

	uint8 result;
	uint8 data1;
	uint8 junk[2];
	uint32 data2;
	uint32 data3;
};

struct MU_HELPER_PLUS_RUN : C1_HEADER_SUB
{
	uint8 zone_id;
	uint8 unk[3];
	uint8 spot;
	uint8 element;
	uint8 status;
};

struct MU_HELPER_PLUS : C2_HEADER
{
	uint8 result;
	uint16 data[6];
};

struct MU_HELPER_PLUS_DATA : C2_HEADER
{
	uint8 result;
	uint8 zone_id;
	uint8 unk1;
	uint8 spot;
	uint8 unk2;
	uint8 element;
	uint8 unk3;
	uint8 party_members;
	uint8 unk4;
};

struct PERSONAL_STORE_ITEM_SOLD : C2_HEADER_SUB
{
	PERSONAL_STORE_ITEM_SOLD()
	{
		this->Set(0x7C, 0x0C, sizeof(PERSONAL_STORE_ITEM_SOLD));
		memset(this->seller, 0, MAX_CHARACTER_LENGTH + 1);
		this->result = 0;
		memset(this->buyer, 0, MAX_CHARACTER_LENGTH + 1);
	}

	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 result;
	uint8 slot;
	uint8 state;
	char buyer[MAX_CHARACTER_LENGTH + 1];
};

struct PRESONAL_STORE_UNK_0B : C2_HEADER_SUB
{
	uint32 data;
	char description[PERSONAL_STORE_NAME_LENGTH + 1];
	uint8 state;
	uint32 index;
};

struct PERSONAL_STORE_STATE_RESULT : C2_HEADER_SUB
{
	PERSONAL_STORE_STATE_RESULT()
	{
		this->Set(0x7C, 0x0A, sizeof(PERSONAL_STORE_STATE_RESULT));
	}

	uint32 data;
	uint8 result;
	uint8 state;
};

struct PERSONAL_STORE_STATE : C2_HEADER_SUB
{
	PERSONAL_STORE_STATE()
	{
		this->Set(0x7C, 0x0A, sizeof(PERSONAL_STORE_STATE));
		this->data = -1;
		this->state = 1;
		memset(this->seller, 0, MAX_CHARACTER_LENGTH + 1);
		memset(this->description, 0, PERSONAL_STORE_NAME_LENGTH + 1);
	}

	uint32 data;
	uint8 state;
	char seller[MAX_CHARACTER_LENGTH + 1];
	char description[PERSONAL_STORE_NAME_LENGTH + 1];
};

struct PERSONAL_STORE_SELL_ITEM_CANCEL : C2_HEADER_SUB
{
	PERSONAL_STORE_SELL_ITEM_CANCEL()
	{
		this->Set(0x7C, 0x09, sizeof(PERSONAL_STORE_SELL_ITEM_CANCEL));
	}

	uint32 data;
	uint8 result;
	uint8 slot;
};

struct PERSONAL_STORE_SELL_ITEM_CANCEL_BODY
{
	uint8 item_info[MAX_ITEM_INFO];
};

struct PERSONAL_STORE_SELL_ITEM_CANCEL_HEAD : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 slot;
	uint8 count;
};

struct PERSONAL_STORE_SELL_ITEM_RESULT : C2_HEADER_SUB
{
	PERSONAL_STORE_SELL_ITEM_RESULT()
	{
		this->Set(0x7C, 0x08, sizeof(PERSONAL_STORE_SELL_ITEM_RESULT));
		this->data = 0;
		this->result = 0;
	}

	uint8 data;
	uint8 result;
	uint8 slot;
	uint8 bundle;
	uint8 item_info[MAX_ITEM_INFO];
	uint32 zen;
	uint32 jewel_of_bless;
	uint32 jewel_of_soul;
};

struct PERSONAL_STORE_SELL_ITEM : C2_HEADER_SUB
{
	uint8 slot;
	uint8 modify;
	uint32 zen;
	uint32 jewel_of_bless;
	uint32 jewel_of_soul;
	uint32 inventory_slot[PERSONAL_STORE_BUNDLE_MAX];
};

struct PERSONAL_STORE_SELL_ITEM_VIEW_BODY
{
	uint8 item_info[MAX_ITEM_INFO];
};

struct PERSONAL_STORE_SELL_ITEM_VIEW_HEAD : C2_HEADER_SUB
{
	uint8 result;
	uint8 slot;
	uint32 zen;
	uint32 jewel_of_bless;
	uint32 jewel_of_soul;
	uint32 count;
};

struct PERSONAL_STORE_SELL_ITEM_VIEW : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 slot;
};

struct PERSONAL_STORE_SELL_LIST_BODY
{
	uint8 slot;
	uint8 bundle;
	uint8 item_info[MAX_ITEM_INFO];
	uint32 zen;
	uint32 jewel_of_bless;
	uint32 jewel_of_soul;
};

struct PERSONAL_STORE_SELL_LIST_HEAD : C2_HEADER_SUB
{
	uint32 data;
	uint8 result; //9
	char description[PERSONAL_STORE_NAME_LENGTH + 1]; //10
	uint8 state;
	uint32 count;
};

struct PERSONAL_STORE_SELL_LIST : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
};

struct PERSONAL_STORE_PURCHASE_RESULT : C2_HEADER_SUB
{
	PERSONAL_STORE_PURCHASE_RESULT()
	{
		this->Set(0x7C, 0x05, sizeof(PERSONAL_STORE_PURCHASE_RESULT));
	}

	uint32 data;
	uint8 result;
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 slot;
};

struct PERSONAL_STORE_PURCHASE_BODY
{
	uint8 item_info[MAX_ITEM_INFO];
};

struct PERSONAL_STORE_PURCHASE_HEAD : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 slot;
	uint8 unk1[11];
	uint32 count;
};

struct PERSONAL_STORE_SELECT_ITEM_BODY
{
	uint8 item_info[MAX_ITEM_INFO];
};

struct PERSONAL_STORE_SELECT_ITEM_HEAD : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 result;
	uint8 slot;
	uint32 zen;
	uint32 jewel_of_bless;
	uint32 jewel_of_soul;
	uint32 count;
};

struct PERSONAL_STORE_SELECT_ITEM : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 slot;
};

struct PERSONAL_STORE_SEARCH_ITEM : C2_HEADER_SUB
{
	char item_name[PERSONAL_STORE_SEARCH_ITEM_LENGTH + 1];
	uint32 data;
	uint16 item;
};

struct PERSONAL_STORE_VIEW_BODY
{
	uint8 slot;
	uint8 bundle;
	uint8 item_info[MAX_ITEM_INFO];
	uint32 zen;
	uint32 jewel_of_bless;
	uint32 jewel_of_soul;
};

struct PERSONAL_STORE_VIEW_HEAD : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1]; //9
	char description[PERSONAL_STORE_NAME_LENGTH + 1]; //20
	uint8 result; //65
	uint32 count; //66
};

struct PERSONAL_STORE_VIEW : C2_HEADER_SUB
{
	uint32 data;
	char seller[MAX_CHARACTER_LENGTH + 1];
};

struct PERSONAL_STORE_SEARCH_ITEM_LIST_BODY
{
	char seller[MAX_CHARACTER_LENGTH + 1];
	uint8 slot;
	uint8 bundle;
	uint8 item_info[MAX_ITEM_INFO];
	uint32 zen;
	uint32 jewel_of_bless;
	uint32 jewel_of_soul;
};

struct PERSONAL_STORE_SEARCH_ITEM_LIST_HEAD : C2_HEADER_SUB
{
	uint32 data;
	uint32 count;
	uint8 unk1;
	uint8 result;
};

struct PERSONAL_STORE_SEARCH_ITEM_LIST : C2_HEADER_SUB
{
	uint32 data;
};

struct PERSONAL_STORE_SEARCH_BODY
{
	char seller[MAX_CHARACTER_LENGTH + 1];
	char description[PERSONAL_STORE_NAME_LENGTH + 1];
};

struct PERSONAL_STORE_SEARCH_HEAD : C2_HEADER_SUB
{
	uint32 data; //5
	uint32 count; //9
	uint8 unk1;//13
	uint8 unk2;//14
};

struct PERSONAL_STORE_SEARCH : C2_HEADER_SUB
{
	uint32 data;
};

struct TEST_PACKET_9E : C1_HEADER ///- GOLDEN ARCHER
{
	TEST_PACKET_9E()
	{
		this->Set(0x9E, sizeof(TEST_PACKET_9E));
	}

	uint8 result;
	uint16 data;
};

struct TEST_PACKET_99 : C1_HEADER ///- GOLDEN ARCHER
{
	TEST_PACKET_99()
	{
		this->Set(0x99, sizeof(TEST_PACKET_99));
	}

	uint8 result;
};

struct TEST_PACKET_96 : C1_HEADER ///- GOLDEN ARCHER
{
	TEST_PACKET_96()
	{
		this->Set(0x96, sizeof(TEST_PACKET_96));
	}

	uint8 result;
	uint16 data1;
	uint16 data2;
	uint16 data3;
};

struct TEST_PACKET_95 : C1_HEADER ///- GOLDEN ARCHER
{
	TEST_PACKET_95()
	{
		this->Set(0x95, sizeof(TEST_PACKET_95));
	}

	uint8 result;
	uint16 index;
};

struct TEST_PACKET_94 : C1_HEADER ///- GOLDEN ARCHER
{
	TEST_PACKET_94()
	{
		this->Set(0x94, sizeof(TEST_PACKET_94));
	}

	uint8 result;
	uint16 index;
};

struct ITEM_ENHANCEMENT_RESULT : C1_HEADER_SUB
{
	ITEM_ENHANCEMENT_RESULT()
	{
		this->Set(0xE8, 0x01, sizeof(ITEM_ENHANCEMENT_RESULT));
		this->unk = 0;
		this->result = 0;
	}

	uint8 unk;
	uint8 result;
};

struct ITEM_ENHANCEMENT_DATA
{
	uint32 item;
	uint8 slot;
	uint8 count;
	uint16 data;
};

struct ITEM_ENHANCEMENT_REQUEST : C1_HEADER_SUB
{
	uint32 id;
	ITEM_ENHANCEMENT_DATA item;
	ITEM_ENHANCEMENT_DATA materials[5];
};

struct SWITCH_MODE_ON : C1_HEADER_SUB
{
	SWITCH_MODE_ON()
	{
		this->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SWITCH_SCROLL, sizeof(SWITCH_MODE_ON));
		this->result = 0;
	}

	uint8 result;
};

struct UNIT_DETECTION_BODY
{
	uint8 id;
	uint16 x;
	uint16 y;
};

struct UNIT_DETECTION_HEAD : C2_HEADER_SUB // 0xE7, 0x05
{
	uint16 junk;
	uint16 count;
};

struct CLIENT_CONNECT_CUSTOM_RESULT : C1_HEADER
{
	uint8 result;
	uint32 time;
};

struct CLIENT_CONNECT_CUSTOM : C1_HEADER
{
	CLIENT_CONNECT_CUSTOM()
	{
		this->Set(CLIENT_CONNECT_CUSTOM_HEADER, sizeof(CLIENT_CONNECT_CUSTOM));
		this->time = 0;
	}

	uint32 time;
};

struct PET_TRAINER_MIX_2_RESULT : C1_HEADER_SUB
{
	PET_TRAINER_MIX_2_RESULT()
	{
		this->Set(0x4C, 0x11, sizeof(PET_TRAINER_MIX_2_RESULT));
		this->result = 0;
	}

	uint8 result;
};

struct PET_TRAINER_MIX_2 : C1_HEADER_SUB
{
	uint8 id;
	uint8 horse_slot;
	uint8 material_slot;
};

struct PET_TRAINER_MIX_1_RESULT : C1_HEADER_SUB
{
	PET_TRAINER_MIX_1_RESULT()
	{
		this->Set(0x4C, 0x10, sizeof(PET_TRAINER_MIX_1_RESULT));
		this->type = 0;
		this->result = 0;
	}

	uint8 type;
	uint8 result;
};

struct PET_TRAINER_MIX_1 : C1_HEADER_SUB
{
	uint8 type;
	uint8 id;
	uint8 main_material;
	uint8 materials[MAX_PET_TRAINER_MIX_SLOTS];
};

struct KUBERA_MINE_CART : C1_HEADER_SUB
{
	uint8 type;
};

struct SWITCH_SCROLL_INFORMATION : C1_HEADER_SUB
{
	SWITCH_SCROLL_INFORMATION()
	{
		this->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SWITCH_SCROLL, sizeof(SWITCH_SCROLL_INFORMATION));
		this->result = 1;
	}

	uint8 result;
};

struct QUEST_MU_83 : C1_HEADER_SUB
{
	QUEST_MU_83()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_UNK_83, sizeof(QUEST_MU_83));
		this->chapter = 0;
		this->quest_id = 0;
		this->unk1 = 0;
	}

	uint16 chapter;
	uint16 quest_id;
	uint8 unk1;
};

struct QUEST_MU_82 : C1_HEADER_SUB
{
	QUEST_MU_82()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_UNK_82, sizeof(QUEST_MU_82));
		this->chapter = 0;
		this->quest_id = 0;
		this->unk1 = 0;
	}

	uint16 chapter;
	uint16 quest_id;
	uint32 unk1;
};

struct QUEST_MU_CANT_MOVE : C1_HEADER_SUB
{
	QUEST_MU_CANT_MOVE()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_MU_CANT_MOVE, sizeof(QUEST_MU_CANT_MOVE));
		this->world = 0;
	}

	uint16 world;
};

struct QUEST_MU_MOVE : C1_HEADER_SUB
{
	uint16 chapter;
	uint16 quest_id;
};

struct QUEST_MU_INFO : C1_HEADER_SUB
{
	uint16 chapter;
	uint16 quest_id;
};

struct QUEST_MU_OBJECTIVE_UPDATE : C1_HEADER_SUB
{
	QUEST_MU_OBJECTIVE_UPDATE()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_MU_OBJECTIVE_UPDATE, sizeof(QUEST_MU_OBJECTIVE_UPDATE));
		this->chapter = 0;
		this->quest_id = 0;
		this->junk1 = 0;
		this->count = 0;
		this->state = 0;
	}

	uint16 chapter;
	uint16 quest_id;
	uint8 junk1; //8
	uint8 count; //9
	uint8 state; //10 1 = Active, 2 = Complete
};

struct QUEST_MU_COMPLETE_RESULT : C1_HEADER_SUB
{
	QUEST_MU_COMPLETE_RESULT()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_MU_COMPLETE, sizeof(QUEST_MU_COMPLETE_RESULT));
		this->chapter = 0;
		this->quest_id = 0;
		this->result = 0;
	}

	uint16 chapter;
	uint16 quest_id;
	uint8 result;
};

struct QUEST_MU_COMPLETE : C1_HEADER_SUB
{
	uint16 npc;
	uint16 chapter;
	uint16 quest_id;
};

struct QUEST_MU_GIVE_UP_RESULT : C1_HEADER_SUB
{
	QUEST_MU_GIVE_UP_RESULT()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_MU_GIVE_UP, sizeof(QUEST_MU_GIVE_UP_RESULT));
		this->chapter = 0;
		this->quest_id = 0;
		this->result = 0;
	}

	uint16 chapter;
	uint16 quest_id;
	uint8 result;
};

struct QUEST_MU_GIVE_UP : C1_HEADER_SUB
{
	uint16 chapter;
	uint16 quest_id;
};

struct QUEST_MU_ACCEPT_RESULT : C1_HEADER_SUB
{
	QUEST_MU_ACCEPT_RESULT()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_MU_ACCEPT, sizeof(QUEST_MU_ACCEPT_RESULT));
		this->chapter = 0;
		this->quest_id = 0;
		this->result = 0;
	}

	uint16 chapter; //0
	uint16 quest_id; //2
	uint8 result; //4
};

struct QUEST_MU_ACCEPT : C1_HEADER_SUB
{
	uint16 chapter;
	uint16 quest_id;
};

struct QUEST_MU_NPC_TALK_BODY
{
	uint16 chapter; //0
	uint16 quest_id; //2
	uint8 state; //4
	uint8 junk2; //5
};

struct QUEST_MU_NPC_TALK_HEAD : C1_HEADER_SUB
{
	uint16 count;
};

struct QUEST_MU_NPC_TALK : C1_HEADER_SUB
{
	uint16 npc;
};

struct QUEST_MU_STATUS_BODY
{
	uint16 chapter; //0
	uint16 quest_id; //2
	uint8 junk2; //4
	uint8 junk3; //5
	uint16 objective; //6
	uint8 state; //8
	uint8 completed; //9
};

struct QUEST_MU_STATUS_HEAD : C1_HEADER_SUB
{
	uint16 count;
};

struct QUEST_UNK_6F : C1_HEADER_SUB
{
	QUEST_UNK_6F()
	{
		this->Set(HEADCODE_QUEST_SUPPORT, SUBCODE_QUEST_UNK_6F, sizeof(QUEST_UNK_6F));
		this->unk = 0;
		this->value = 0;
	}

	uint16 unk;
	uint16 value; ///- Quest ID
};

struct NEW_SKILL_VIEW_BODY
{
	uint16 target;
};

struct NEW_SKILL_VIEW_HEAD : C1_HEADER_SUB
{
	uint16 skill;
	uint16 source;
	uint16 count;
};

struct NEW_SKILL_INFO : C1_HEADER_SUB
{
	NEW_SKILL_INFO()
	{
		this->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_NEW_SKILL_INFO, sizeof(NEW_SKILL_INFO));
		this->SetSkill(-1);
		this->SetSource(-1);
		this->dark_plasma_x = 0;
		this->dark_plasma_y = 0;
		this->unk10 = 0;
		this->unk11 = 0;
		this->lightning_x = 0;
		this->lightning_y = 0;
		this->ball_direction = 0;
		this->ball_move_distance = 0;
		this->projectile_amount = 0;
		this->unk23 = 0;
		this->ball_move_speed = 0;
		this->ball_attack_distance = 0;
		this->start_direction = 0;
		this->ball_target_count = 0;
		this->ball_attack_speed = 0;
	}

	INDEX_DATA(Skill); //4
	INDEX_DATA(Source); //6
	uint8 dark_plasma_x; //8
	uint8 dark_plasma_y; //9
	uint8 unk10; //10
	uint8 unk11; //11
	int32 lightning_x; //12 - Plasma Ball & Lightning Storm s16
	int32 lightning_y; //16 - Plasma Ball & Lightning Storm s16
	uint8 ball_direction; //20 - Plasma Ball s16
	uint8 ball_move_distance; //21 - Plasma Ball s16
	uint8 projectile_amount; //22 - Magic Arrow & Sword Inertia s16
	uint8 unk23;
	float ball_move_speed; //24 - Plasma Ball s16
	uint8 ball_attack_distance; //28 - Plasma Ball & Lightning Storm s16
	uint8 start_direction; //29 - Magic Arrow s16
	uint8 ball_target_count; //30
	uint8 ball_attack_speed; //31 (seconds * 10)
};

struct EARRING_UPGRADE_RESULT : C1_HEADER_SUB
{
	EARRING_UPGRADE_RESULT()
	{
		this->Set(0xEC, 0x08, sizeof(EARRING_UPGRADE_RESULT));
		this->result = 0;
	}

	uint8 junk;
	uint8 result;
};

struct EARRING_UPGRADE : C1_HEADER_SUB
{
	uint8 slot[MAX_EARRING_UPGRADE_SLOTS];
};

struct PACKET_0xEC_0x60
{
	PACKET_0xEC_0x60()
	{
		this->h.set(0xEC, 0x60, sizeof(PACKET_0xEC_0x60));
	}

	PBMSG_HEAD2 h;
	uint32 data;
};

struct PACKET_CHANNEL_WARP_RESULT : C1_HEADER_SUB
{
	PACKET_CHANNEL_WARP_RESULT()
	{
		this->Set(0xEC, 0x56, sizeof(PACKET_CHANNEL_WARP_RESULT));
		this->result = 1;
	}

	uint8 result;
};

struct PACKET_CHANNEL_WARP : C1_HEADER_SUB
{
	uint16 server;
};

struct PACKET_CHANNEL_BODY
{
	uint16 server;
	uint16 data1;
	uint16 data2;
	uint8 type;
	uint8 gold;
};

struct PACKET_CHANNEL_HEAD : C2_HEADER_SUB
{
	uint8 count;
	uint8 data;
};

struct PACKET_GOLDEN_ARCHER_LUCKY_NUMBER_VALUE
{
	PACKET_GOLDEN_ARCHER_LUCKY_NUMBER_VALUE()
	{
		this->h.set(0x95, sizeof(PACKET_GOLDEN_ARCHER_LUCKY_NUMBER_VALUE));
	}

	PBMSG_HEAD h;
	uint8 type;
	uint16 value;
};

struct PACKET_GOLDEN_ARCHER_LUCKY_NUMBER_OPEN
{
	PACKET_GOLDEN_ARCHER_LUCKY_NUMBER_OPEN()
	{
		this->h.set(0x94, sizeof(PACKET_GOLDEN_ARCHER_LUCKY_NUMBER_OPEN));
	}

	PBMSG_HEAD h;
	uint8 type;
	uint16 value;
};

struct PACKET_PC_ROOM_VALUE
{
	PACKET_PC_ROOM_VALUE()
	{
		this->h.set(HEADCODE_PC_ROOM, SUBCODE_PC_ROOM_VALUE, sizeof(PACKET_PC_ROOM_VALUE));
		this->open = 0;
		this->value = 0;
	}

	PBMSG_HEAD2 h;
	uint32 open;
	uint32 value;
};

struct PACKET_PC_ROOM_OPEN
{
	PACKET_PC_ROOM_OPEN()
	{
		this->h.set(HEADCODE_PC_ROOM, SUBCODE_PC_ROOM_OPEN, sizeof(PACKET_PC_ROOM_OPEN));
		this->open = 0;
	}

	PBMSG_HEAD2 h;
	uint8 open;
};

struct PACKET_WARP_MONSTER_INFO_BODY
{
	uint32 id;
	uint32 monster;
	uint32 hp;
	uint16 attack_power;
	uint16 defense;
	uint16 elemental_damage;
	uint16 elemental_defense;
};

struct PACKET_WARP_MONSTER_INFO_HEAD : C2_HEADER_SUB
{
	uint8 unk;
	uint16 count;
};

/*
5370	"%s Area Major Monster Information"
5371	"HP : %d"
5372	"Attack Power : %d"
5373	"Defense : %d"
5374	"Elemental Damage : %d"
5375	"Elemental Defense : %d"
*/

struct PACKET_PROBABILITY_LOG
{
	PACKET_PROBABILITY_LOG()
	{
		this->h.set(0x59, 0x05, sizeof(PACKET_PROBABILITY_LOG));
	}

	PWMSG_HEAD2 h;
	char filename[67];
	uint32 length;
	char data[6278];
};

struct PACKET_HOW_TO_PLAY
{
	PACKET_HOW_TO_PLAY()
	{
		this->h.set(0x59, 0x03, sizeof(PACKET_HOW_TO_PLAY));
		this->data = 0;
	}

	PBMSG_HEAD2 h;
	uint16 data;
};

struct PACKET_POSITION_ATTACK_RESULT
{
	PACKET_POSITION_ATTACK_RESULT()
	{
		this->h.set(0x59, 0x01, sizeof(PACKET_POSITION_ATTACK_RESULT));
	}

	PBMSG_HEAD2 h;
	INDEX_DATA(Index);
	INDEX_DATA(Skill);
	INDEX_DATA(Target);
	uint8 x;
	uint8 y;
};

struct PACKET_POSITION_ATTACK
{
	uint16 GetMagic() const { return MAKE_NUMBERW(this->MagicH, this->MagicL); }
	uint16 GetTarget() const { return MAKE_NUMBERW(this->TargetH, this->TargetL); }

	PBMSG_HEAD2 h;
	uint8 TargetH;
	uint8 MagicH;
	uint8 TargetL;
	uint8 MagicL;
	uint8 x;
	uint8 y;
};

struct BOSS_PACKET_UNK1
{
	BOSS_PACKET_UNK1()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_BOSS_UNK1, sizeof(BOSS_PACKET_UNK1));
		this->index = 0;
		this->x = 0;
		this->y = 0;
	}

	PBMSG_HEAD2 h;
	uint16 index;
	uint8 x;
	uint8 y;
};

struct WING_SOCKET_OPTION_UPGRADE_RESULT
{
	WING_SOCKET_OPTION_UPGRADE_RESULT(uint8 result)
	{
		this->h.set(0xEC, 0x07, sizeof(WING_SOCKET_OPTION_UPGRADE_RESULT));
		this->slot = 0;
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 slot;
	uint8 result;
	
};

struct WING_SOCKET_OPTION_UPGRADE
{
	PBMSG_HEAD2 h;
	uint8 slot[11];
};

struct WING_SOCKET_OPTION_ADD_RESULT
{
	WING_SOCKET_OPTION_ADD_RESULT(uint8 result)
	{
		this->h.set(0xEC, 0x06, sizeof(WING_SOCKET_OPTION_ADD_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct WING_SOCKET_OPTION_ADD
{
	PBMSG_HEAD2 h;
	uint8 slot[15];
};

struct HUNTING_RECORD_SET_VISIBLE : C1_HEADER_SUB
{
	HUNTING_RECORD_SET_VISIBLE(uint8 result)
	{
		this->Set(0xEC, 0x54, sizeof(HUNTING_RECORD_SET_VISIBLE));
		this->result = result;
	}

	uint8 result;
};

struct PLAYER_RIGHT_CLICK
{
	PBMSG_HEAD2 h;
	uint16 index;
	uint16 data;
};

struct PARTY_SUMMON_REQUEST
{
	PBMSG_HEAD2 h;
	uint16 x;
	uint16 y;
	uint8 data;
};

struct PC_ROMM_53
{
	PC_ROMM_53()
	{
		this->h.set(HEADCODE_EVENT_MISC, 0x53, sizeof(PC_ROMM_53));
		this->data1 = 0;
		this->data2 = 0;
	}

	PBMSG_HEAD2 h;
	uint32 data1;
	uint32 data2;
};

struct PARTY_HELPER_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 data;
};
///- Este packet se usa luego del warp, para mover el pj hacia donde esta el member
struct PARTY_MOVE_RESULT
{
	PARTY_MOVE_RESULT()
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_PARTY_MOVE, sizeof(PARTY_MOVE_RESULT));
		this->x = 0;
		this->y = 0;
		this->world = 0;
	}

	PBMSG_HEAD2 h;
	uint16 x;
	uint16 y;
	uint16 world;
};

struct PARTY_MOVE_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 slot;
};

struct ITEM_SPLIT_RESULT
{
	ITEM_SPLIT_RESULT()
	{
		this->h.set(0xE8, sizeof(ITEM_SPLIT_RESULT));
		this->id = 0;
		this->result = 0;
	}

	PBMSG_HEAD h;
	uint8 id;
	uint8 result;
};

struct ITEM_SPLIT_REQUEST
{
	PBMSG_HEAD h;
	uint8 id;
	uint8 slot;
	uint8 type;
	uint8 amount;
};

struct QUEST_SURVIVAL_TIMER
{
	QUEST_SURVIVAL_TIMER()
	{
		this->h.set(HEADCODE_QUEST_TIMER, 0x01, sizeof(QUEST_SURVIVAL_TIMER));
		this->type = 5;
		this->increase = 0;
		memset(this->junk, 0, 2);
		this->time = 0;
	}

	PBMSG_HEAD2 h;
	uint8 type;
	uint8 increase;
	uint8 junk[2];
	uint32 time;

	/*
	type:
	0 = 
	1 = Time Limit
	2 = Remaining Time
	3 = Hunting Time
	4 = Disassembly Failed
	5 = Quest Survival Time


	*/
};

struct FEREA_MINE
{
	FEREA_MINE()
	{
		this->h.set(0xF9, 0x10, sizeof(FEREA_MINE));
		this->x = 0;
		this->y = 0;
		this->type = 0;
	}

	PBMSG_HEAD2 h;
	uint32 x;
	uint32 y;
	uint32 type;
};

struct FEREA_BOSS_EFFECT
{
	FEREA_BOSS_EFFECT()
	{
		this->h.set(0xF9, 0x12, sizeof(FEREA_BOSS_EFFECT));
	}

	PBMSG_HEAD2 h;
};

struct FEREA_DOOR_OPEN_CLOSE
{
	FEREA_DOOR_OPEN_CLOSE(uint8 closed)
	{
		this->h.set(0xF9, 0x11, sizeof(FEREA_DOOR_OPEN_CLOSE));
		this->closed = closed;
	}

	PBMSG_HEAD2 h;
	uint8 closed;
};

struct CENT_QUEST_START
{
	PBMSG_HEAD2 h;
};

struct MAJESTIC_SKILL_TREE_PASSIVE_ADD_RESULT
{
	MAJESTIC_SKILL_TREE_PASSIVE_ADD_RESULT()
	{
		this->h.set(HEADCODE_MAJESTIC_SKILL_TREE, SUBCODE_MAJESTIC_SKILL_TREE_PASSIVE_ADD, sizeof(MAJESTIC_SKILL_TREE_PASSIVE_ADD_RESULT));
		this->unk1 = 0;
		this->unk2 = 0;
		for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
		{
			this->stat[i] = 0;
		}
	}

	PBMSG_HEAD2 h;
	uint8 unk1;
	uint32 unk2;
	uint16 stat[MAX_SKILL_TREE_MAJESTIC_PASSIVE];


	/*
	1509154016 PACKET:  : C1 11 7E 07 B8 00 00 00 00 00 00 00 00 2C 00 00 00
	1509154019 PACKET:  : C1 11 7E 07 10 00 00 00 00 00 00 04 00 2C 00 00 00
	1509154022 PACKET:  : C1 11 7E 07 48 00 00 00 00 00 00 08 00 2C 00 00 00
	1509154025 PACKET:  : C1 11 7E 07 38 00 00 00 00 00 00 08 00 2C 00 04 00
	1509154029 PACKET:  : C1 11 7E 07 78 00 00 00 00 00 00 08 00 2C 00 08 00
	1509154032 PACKET:  : C1 11 7E 07 70 00 00 00 00 04 00 08 00 2C 00 08 00
	1509154035 PACKET:  : C1 11 7E 07 30 00 00 00 00 08 00 08 00 2C 00 08 00
	1509154039 PACKET:  : C1 11 7E 07 70 00 00 00 00 08 00 08 00 2C 00 0C 00
	1509154043 PACKET:  : C1 11 7E 07 28 00 00 00 00 08 00 08 00 30 00 0C 00
	1509154046 PACKET:  : C1 11 7E 07 38 00 00 00 00 08 00 08 00 34 00 0C 00
	*/
};

struct MAJESTIC_SKILL_TREE_PASSIVE_ADD
{
	PBMSG_HEAD2 h;
	uint16 add_points[MAX_SKILL_TREE_MAJESTIC_PASSIVE];
};

struct MAJESTIC_SKILL_TREE_DATA
{
	MAJESTIC_SKILL_TREE_DATA()
	{
		this->h.set(HEADCODE_MAJESTIC_SKILL_TREE, SUBCODE_MAJESTIC_SKILL_TREE_DATA, sizeof(MAJESTIC_SKILL_TREE_DATA));
		this->points = 0;
	}
	
	PBMSG_HEAD2 h;
	uint16 points;
};

struct MAJESTIC_SKILL_TREE_STAT_ADD
{
	PBMSG_HEAD2 h;
	uint8 section;
	uint8 id;
};

struct MAJESTIC_SKILL_TREE_STAT_ADD_RESULT
{
	MAJESTIC_SKILL_TREE_STAT_ADD_RESULT()
	{
		this->h.set(HEADCODE_MAJESTIC_SKILL_TREE, SUBCODE_MAJESTIC_SKILL_TREE_STAT_ADD, sizeof(MAJESTIC_SKILL_TREE_STAT_ADD_RESULT));
		this->result = 0;
		this->section = 0;
		this->points = 0;
		this->level = 0;
		this->id = 0;
		this->current_level = 0.0f;
		this->next_level = 0.0f;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 section;
	uint16 points;
	uint8 level;
	uint8 id; //- Primera columna
	float current_level;
	float next_level;
};

struct MAJESTIC_SKILL_TREE_STAT_LIST_BODY
{
	uint8 section;
	uint8 id;
	uint8 level;
	float current_level;
	float next_level;
};

struct MAJESTIC_SKILL_TREE_STAT_LIST_HEAD
{
	PWMSG_HEAD2 h;
	uint32 count;
};

struct MAJESTIC_SKILL_TREE_ADD_RESULT
{
	MAJESTIC_SKILL_TREE_ADD_RESULT()
	{
		this->h.set(HEADCODE_MAJESTIC_SKILL_TREE, SUBCODE_MAJESTIC_SKILL_TREE_ADD, sizeof(MAJESTIC_SKILL_TREE_ADD_RESULT));
		this->result = 0;
		this->points = 0;
		this->section = 0;
		this->level = 0;
		this->id = 0;
		this->current_level = 0.0f;
		this->next_level = 0.0f;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint16 points;
	uint8 section; //- Segunda columna
	uint8 level; //- Cantidad de puntos que tiene el skill
	uint16 id; //- Primera columna
	float current_level;
	float next_level;
};

struct MAJESTIC_SKILL_TREE_ADD
{
	PBMSG_HEAD2 h;
	uint8 section;
	uint16 skill_id;
	uint16 id;
};

struct MAJESTIC_SKILL_TREE_LIST_BODY
{
	uint8 section;
	uint16 id;
	uint8 level;
	float current_level;
	float next_level;
};

struct MAJESTIC_SKILL_TREE_LIST_HEAD
{
	PWMSG_HEAD2 h;
	uint16 points;
	uint32 count;
};

struct WARP_FAVORITE_LIST_DATA
{
	uint16 data;
	uint16 warp_id;
};

struct WARP_FAVORITE_LIST
{
	WARP_FAVORITE_LIST()
	{
		//this->h.set(0x6D, 0x01, sizeof(WARP_FAVORITE_LIST));
		this->h.set(0x59, 0x04, sizeof(WARP_FAVORITE_LIST));
		this->count = 0;

		for ( int32 i = 0; i < WARP_FAVORITE_COUNT; ++i )
		{
			this->data[i].data = -1;
			this->data[i].warp_id = -1;
		}
	}

	PBMSG_HEAD2 h;
	uint32 count;
	WARP_FAVORITE_LIST_DATA data[WARP_FAVORITE_COUNT];
};

struct EVENT_ENTER_COUNT
{
	EVENT_ENTER_COUNT(uint8 type) ///- 0x00 -> MaxCount --- 0x01 -> CurrentCount
	{
		this->h.set(0xF1, 0xFD, sizeof(EVENT_ENTER_COUNT));
		this->type = type;
		this->BloodCastleCount = 0;
		this->ChaosCastleCount = 0;
		this->DevilSquareCount = 0;
		this->DoppelgangerCount = 0;
		this->ImperialFortressCount = 0;
		this->IllusionTempleCount = 0;
	}

	PBMSG_HEAD2 h;
	uint8 type;
	uint8 BloodCastleCount;
	uint8 ChaosCastleCount;
	uint8 DevilSquareCount;
	uint8 DoppelgangerCount;
	uint8 ImperialFortressCount;
	uint8 IllusionTempleCount;
};

struct DARK_ANCIENT_ITEM_CRAFT
{
	PBMSG_HEAD2 h;
	uint8 slot;
};

struct DARK_ANCIENT_ITEM_CREATE
{
	DARK_ANCIENT_ITEM_CREATE()
	{
		this->h.set(0x70, 0x00, sizeof(DARK_ANCIENT_ITEM_CREATE));
		this->result = 0;
		this->slot = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 slot;
};

struct DARK_ANCIENT_ITEM_DESTROY
{
	DARK_ANCIENT_ITEM_DESTROY()
	{
		this->h.set(0x70, 0x01, sizeof(DARK_ANCIENT_ITEM_DESTROY));
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct MESSAGE_BOX_SEND
{
	MESSAGE_BOX_SEND()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MESSAGE_BOX, sizeof(MESSAGE_BOX_SEND));
		this->type = 0;
		memset(this->caption, 0, 100);
		memset(this->message, 0, 150);
	}
	PBMSG_HEAD2 h;
	uint8 type;
	char caption[100];
	char message[150];
};

struct DISSASEMBLE_RESULT
{
	DISSASEMBLE_RESULT()
	{
		this->h.set(HEADCODE_ITEM_DISASSEMBLE, sizeof(DISSASEMBLE_RESULT));
		this->result = 0;
		this->item = -1;
		this->count = 0;
	}

	PBMSG_HEAD h;
	uint8 result;
	uint32 item;
	uint32 count;
};

struct EVOMON_RESULT
{
	EVOMON_RESULT()
	{
		this->h.set(HEADCODE_MISC_EVENT, 0x03, sizeof(EVOMON_RESULT));
		this->result = 0;
		this->unk1 = 0;
		this->world = 0;
		this->x = 0;
		this->y = 0;
		this->unk2[0] = 0;
		this->unk2[1] = 0;
		this->remain_time = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 unk1;
	uint16 world;
	uint8 x;
	uint8 y;
	uint8 unk2[2];
	uint32 remain_time;

	/*
	result:
	0 -> ok

	1 -> "Cannot use as you already have an Evomon summoned."
	  -> "Location - %s (%d, %d)"
	  -> "Time left until dissipation - %dm"

	2 -> "Cannot use while trying to awaken Evomon."
	3 -> "Cannot use in areas where party member's Evomon is summoned."
	4 -> "You cannot use the item at certain applicable locations."
	*/
};

struct EVOMON_SCORE_DATA
{
	EVOMON_SCORE_DATA()
	{
		this->h.set(HEADCODE_EVOMON_SCORE, sizeof(EVOMON_SCORE_DATA));
		this->result = 0;
		this->data1 = 0;
		this->data2 = 0;
	}
	PBMSG_HEAD h;
	uint32 result;
	uint32 data1;
	uint32 data2;
};

struct CARD_REWARD_SELECT
{
	CARD_REWARD_SELECT()
	{
		this->h.set(HEADCODE_MISC_EVENT, 0x07, sizeof(CARD_REWARD_SELECT));
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct CARD_REWARD_ITEM_RESULT
{
	CARD_REWARD_ITEM_RESULT()
	{
		this->h.set(HEADCODE_MISC_EVENT, 0x08, sizeof(CARD_REWARD_ITEM_RESULT));
		this->result = 0;
		this->data = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 data;
};

struct EVOMON_PACKET_4
{
	EVOMON_PACKET_4()
	{
		this->h.set(HEADCODE_MISC_EVENT, 0x10, sizeof(EVOMON_PACKET_4));
		this->data = 0;
		this->index = -1;
	}

	PBMSG_HEAD2 h;
	uint16 data;
	uint16 index;
};

struct EVOMON_PACKET_5
{
	EVOMON_PACKET_5()
	{
		this->h.set(HEADCODE_MISC_EVENT, 0x11, sizeof(EVOMON_PACKET_5));
		this->index = -1;
	}

	PBMSG_HEAD2 h;
	uint16 index;
};

struct EXPERIENCE_EVENT
{
	EXPERIENCE_EVENT()
	{
		this->h.set(HEADCODE_EVENT_MISC, 0x52, sizeof(EXPERIENCE_EVENT));
		this->data1 = 0x1E;
		this->experience = 0;
		this->data2 = 0;
	}

	PBMSG_HEAD2 h;
	uint16 data1;
	uint16 experience;
	uint16 data2;
};

struct HUNTING_RECORD_TIME /* 0xEC 0x55 */
{
	HUNTING_RECORD_TIME()
	{
		this->h.set(0xEC, 0x55, sizeof(HUNTING_RECORD_TIME));
		this->time = 0;
		this->damage = 0;
		this->elemental_damage = 0;
		this->healing = 0;
		this->killed_count = 0;
		memset(this->experience, 0, 8);
	}

	PBMSG_HEAD2 h;
	uint32 time;
	uint64 damage;
	uint64 elemental_damage;
	uint32 healing;
	uint32 killed_count;
	uint8 experience[8];
};

struct HUNTING_RECORD_CURRENT_DAY
{
	HUNTING_RECORD_CURRENT_DAY()
	{
		this->h.set(0xEC, 0x53, sizeof(HUNTING_RECORD_CURRENT_DAY));
		this->id = 0;
		this->year = 0;
		this->month = 0;
		this->day = 0;
		this->level = 0;
		this->duration = 0;
		this->damage = 0;
		this->elemental_damage = 0;
		this->healing = 0;
		this->killed_count = 0;
		memset(this->experience, 0, 8);
	}

	PBMSG_HEAD2 h;
	uint8 id;
	uint32 year;
	uint8 month;
	uint8 day;
	uint32 level;
	uint32 duration;
	uint64 damage;
	uint64 elemental_damage;
	uint32 healing;
	uint32 killed_count;
	uint8 experience[8];
};

struct HUNTING_RECORD_VISIBLE : C1_HEADER_SUB
{
	uint8 visible;
};

struct HUNTING_RECORD_BODY
{
	uint32 unk1;
	uint32 year;
	uint8 month;
	uint8 day;
	uint32 level;
	uint32 duration;
	uint64 damage;
	uint64 elemental_damage;
	uint32 healing;
	uint32 killed_count;
	uint8 experience[8];
};

struct HUNTING_RECORD_HEAD /* 0xEC 0x50 */
{
	PWMSG_HEAD2 h;
	uint8 countH;
	uint8 countL;
	uint8 result;
};

struct HUNTING_RECORD_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 world;
	uint16 index;
	uint8 unk[2];
};

struct HUNTING_RECORD_CLOSE
{
	PBMSG_HEAD2 h;
};

struct SOCKET_MIX_RESULT
{
	SOCKET_MIX_RESULT(uint8 result, uint8 gremory, uint8 slot)
	{
		this->h.set(0x78, sizeof(SOCKET_MIX_RESULT));
		this->unk = 1;
		this->result = result;
		this->gremory = gremory;
		this->slot = slot;
	}

	PBMSG_HEAD h;
	uint8 unk;
	uint8 result;
	uint8 gremory;
	uint8 slot;
};

struct SOCKET_MIX_REQUEST
{
	PBMSG_HEAD h;
	uint8 mix_id;
	uint8 slot[36];
	uint8 data;
};

struct SCAN_PROCESS_REQUEST
{
	SCAN_PROCESS_REQUEST()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SCAN_PROCESS, sizeof(SCAN_PROCESS_REQUEST));
	}

	PBMSG_HEAD2 h;
};

struct SCAN_PROCESS_BODY
{
	char process_name[50];
};

struct SCAN_PROCESS_HEAD
{
	PWMSG_HEAD2 h;
	uint16 count;
};

struct CHEAT_SCAN_DETECT
{
	PWMSG_HEAD2 h;
	uint8 type;
	uint32 process_id;
	char process_name[256];
	char window_name[256];
};

struct SHOP_MISC_BODY
{
	uint8 slot;
	uint32 price;
};

struct SHOP_MISC_HEAD
{
	PWMSG_HEAD2 h;
	uint8 type;
	uint8 count;
};

struct ATTACK_SPEED_DATA
{
	ATTACK_SPEED_DATA(uint32 att_speed, uint32 mc_speed)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_ATTACK_SPEED, sizeof(ATTACK_SPEED_DATA));
		memset(this->attack_speed, 0, 4);
		memset(this->magic_speed, 0, 4);

		this->attack_speed[0] = HIBYTE(HIWORD(att_speed));
		this->attack_speed[1] = LOBYTE(HIWORD(att_speed));
		this->attack_speed[2] = HIBYTE(LOWORD(att_speed));
		this->attack_speed[3] = LOBYTE(LOWORD(att_speed));

		this->magic_speed[0] = HIBYTE(HIWORD(mc_speed));
		this->magic_speed[1] = LOBYTE(HIWORD(mc_speed));
		this->magic_speed[2] = HIBYTE(LOWORD(mc_speed));
		this->magic_speed[3] = LOBYTE(LOWORD(mc_speed));

		this->attack_speed[0] += 0x20;
		this->attack_speed[1] += 0x21;
		this->attack_speed[2] += 0x22;
		this->attack_speed[3] += 0x23;

		this->magic_speed[0] += 0x24;
		this->magic_speed[1] += 0x25;
		this->magic_speed[2] += 0x26;
		this->magic_speed[3] += 0x27;
	}

	PBMSG_HEAD2 h;
	uint8 attack_speed[4];
	uint8 magic_speed[4];
};

struct HACK_CHECK_PING
{
	PBMSG_HEAD2 h;
	uint8 id;
};

struct HACK_CHECK_MODIFIED_BYTE_OFFSET
{
	PBMSG_HEAD2 h;
	uint32 offset;
	uint8 move;
	uint8 value;
};

struct HACK_CHECK_MODIFIED_DOUBLE_OFFSET
{
	PBMSG_HEAD2 h;
	uint32 offset;
	double value;
};

struct HACK_CHECK_HOOKED_FUNCTION
{
	PBMSG_HEAD2 h;
	uint8 id;
	uint8 data;
};

struct HACK_CHECK_FRAME_COUNT
{
	PBMSG_HEAD2 h;
	int32 count;
};

struct HACK_CHECK_ADD_OFFSET_BODY
{
	uint32 offset;
	uint8 value;
	uint8 fix;
};

struct HACK_CHECK_ADD_OFFSET_HEAD
{
	PWMSG_HEAD2 h;
	uint8 count;
};

struct SOLD_ITEM_RECOVER_RESULT
{
	SOLD_ITEM_RECOVER_RESULT()
	{
		this->h.set(0x6F, 0x02, sizeof(SOLD_ITEM_RECOVER_RESULT));
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct SOLD_ITEM_RECOVER
{
	PBMSG_HEAD2 h;
	uint8 slot;
	uint8 unk[7];
	uint32 serial;
	uint32 unk2;
};

struct SOLD_ITEM_LIST_BODY
{
	uint32 time;
	uint8 item_info[MAX_ITEM_INFO];
	uint16 quantity;
	uint32 serial;
	uint8 unk;
};

struct SOLD_ITEM_LIST_HEAD
{
	PWMSG_HEAD2 h;
	uint8 countH;
	uint8 countL;
};

struct WARP_RESULT : C1_HEADER
{
	WARP_RESULT()
	{
		this->Set(HEADCODE_MOVE_REQUEST, sizeof(WARP_RESULT));
		this->result = 0x03;
		this->unk[0] = 0;
		this->unk[1] = 0;
	}

	uint8 result;
	uint8 unk[2];
};

struct MIX_PENTAGRAM_RESULT
{
	MIX_PENTAGRAM_RESULT(uint8 result, uint8 gremory)
	{
		this->h.set(0xEC, 0x02, sizeof(MIX_PENTAGRAM_RESULT));
		this->result = result;
		this->gremory = gremory;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 gremory;
};

struct RUUD_SEND
{
	RUUD_SEND(uint32 ruud)
	{
		this->h.set(HEADCODE_NPC_INTERACTION, SUBCODE_NPC_INTERACTION_RUUD_SEND, sizeof(RUUD_SEND));
		this->ruud = ruud;
		this->add_ruud = 0;
		this->add = 0;
	}

	PBMSG_HEAD2 h;
	uint32 ruud;
	uint32 add_ruud;
	uint8 add;
};

struct ACHERON_WARP_RESULT
{
	ACHERON_WARP_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_WARP_ACHERON_RESULT, sizeof(ACHERON_WARP_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct EVENT_NOTICE
{
	EVENT_NOTICE(uint32 result)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_EVENT_NOTICE, sizeof(EVENT_NOTICE));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint32 result;
};

struct RUUD_ITEM_BUY
{
	PBMSG_HEAD2 h;
	uint8 slot;
};

struct RUUD_ITEM_BUY_RESULT
{
	RUUD_ITEM_BUY_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_NPC_INTERACTION, SUBCODE_NPC_INTERACTION_RUUD_BUY, sizeof(RUUD_ITEM_BUY_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct PROTECTOR_OF_ACHERON_ENTER_RESULT
{
	PROTECTOR_OF_ACHERON_ENTER_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ENTER_AS_THE_PROTECTOR_OF_ACHERON, sizeof(PROTECTOR_OF_ACHERON_ENTER_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct CHARACTER_SPECIALIZATION_STAT_DATA
{
	uint8 type;
	uint8 type_unk;
	uint16 spl_value1;
	uint16 spl_value2;
};

struct CHARACTER_SPECIALIZATION_STAT
{
	CHARACTER_SPECIALIZATION_STAT()
	{
		this->h.set(0x59, 0x00, sizeof(CHARACTER_SPECIALIZATION_STAT));

		for ( int32 i = 0; i < 5; ++i )
		{
			this->data[i].type = 0;
			this->data[i].type_unk = 0;
			this->data[i].spl_value1 = 0;
			this->data[i].spl_value2 = 0;
		}
	}

	PBMSG_HEAD2 h;  //  0xC1 0x22 0x59
	CHARACTER_SPECIALIZATION_STAT_DATA data[5];
};

struct EVENT_NOTIFICATION
{
	EVENT_NOTIFICATION(uint8 event_id, uint8 active)
	{
		this->h.set(0xF1, 0xFE, sizeof(EVENT_NOTIFICATION));
		this->event_id = event_id;
		this->active = active;
	}
	PBMSG_HEAD2 h;
	uint8 event_id;
	uint8 active;
};

struct ITEM_DISASSEMBLE
{
	PBMSG_HEAD h;
	uint8 slot;
	uint8 type;
};

struct CHAT_BLOCK_ADD
{
	PBMSG_HEAD2 h;
	char name[MAX_CHARACTER_LENGTH + 1];
};

struct CHAT_BLOCK_DELETE
{
	PBMSG_HEAD2 h;
	uint8 position;
	char name[MAX_CHARACTER_LENGTH + 1];
};

struct CHAT_BLOCK_LIST_BODY
{
	uint8 position;
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 next;
};

struct CHAT_BLOCK_LIST_HEAD
{
	PWMSG_HEAD2 h; // 0x6E 0x00
	uint8 type; // 1 -> Normal, 2 -> Add, 3 -> Delete
	uint8 result;
	uint8 count;
	uint8 action; // 1 -> add, 0 -> delete
};

struct PARTY_LEADER_CHANGE
{
	PBMSG_HEAD2 h;
	char name[MAX_CHARACTER_LENGTH + 1];
};

struct PARTY_LEADER_CHANGE_RESULT : C1_HEADER_SUB
{
	PARTY_LEADER_CHANGE_RESULT(uint8 result)
	{
		this->Set(0xEF, 0x07, sizeof(PARTY_LEADER_CHANGE_RESULT));
		this->result = result;
		this->unk = 0;
	}

	uint8 result;
	uint8 unk;
};

struct OPEN_BOX_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 inventory_slot;
	uint8 data2;
	uint8 type;
};

struct OPEN_BOX_RESULT
{
	OPEN_BOX_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_NPC_INTERACTION, SUBCODE_NPC_INTERACTION_OPEN_BOX, sizeof(OPEN_BOX_RESULT));
		this->result = result;
		this->slot = 0;
	}
	PBMSG_HEAD2 h;
	uint32 result;
	uint32 slot;
};

struct CHARACTER_NEW_STAT
{
	CHARACTER_NEW_STAT()
	{
		this->h.set(0xEC, 0x29, sizeof(CHARACTER_NEW_STAT));
	}
	PBMSG_HEAD2 h;
	uint16 critical_damage_increase;	//4
	uint16 excellent_damage_increase;	//6
	uint16 skill_attack_damage_increase;	//8
	uint16 basic_defense;	//10
	uint16 strength;	//12
	uint16 strength_add;	//14
	uint16 agility;		//16
	uint16 agility_add;		//18
	uint16 vitality;	//20
	uint16 vitality_add;	//22
	uint16 energy;	//24
	uint16 energy_add;	//26
	uint16 leadership;	//28
	uint16 leadership_add;	//30
	uint16 sd_recovery_when_attacked;	//32
	uint16 sd_ignore_rate;	//34
	uint16 sd_recovery_when_attack;	//36
	uint16 zen_obtained_rate;	//38
	float defense_ignore_rate;	//40
	float hp_recovery_rate;	//44
	float mp_recovery_rate;	//48
	float stun_rate;	//52
	float stun_resistance_rate;	//56
	uint16 unk_data_1;	//60
	uint16 unk_data_2;	//62
	float shield_absorb;	//64
	float monster_attack_hp_recovery;	//68
	float monster_attack_mp_recovery;	//72
	float monster_attack_sd_recovery;	//76
	float sd_recovery_rate;	//80
	float restore_all_mp_rate;	//84
	float restore_all_hp_rate;	//88
	float restore_all_sd_rate;	//92
	float ag_recovery_rate;	//96
	float damage_absorb_rate;	//100
	float shield_block_rate;	//104
	float weapon_block_rate;	//108
	float hp_absorb_rate;	//112
	float sd_absorb;	//116
	float return_damage_rate;	//120
	float damage_increase_rate;	//124
	float mp_use_reduction_rate;	//128
	float critical_damage_rate;	//132
	float excellent_damage_rate;	//136
	float double_damage_rate;	//140
	float triple_damage_rate;	//144
	uint8 damage_reduction_rate;	//148
	uint16 damage_reduction_amount;	//149
	uint8 ag_use_reduction_rate;	//151
	uint8 damage_reflect_rate;	//152
};

struct CHARACTER_STAT_DATA
{
	CHARACTER_STAT_DATA()
	{
		this->h.set(0xEC, 0x25, sizeof(CHARACTER_STAT_DATA));
		this->strength = 0;
		this->strength_add = 0;
		this->agility = 0;
		this->agility_add = 0;
		this->vitality = 0;
		this->vitality_add = 0;
		this->energy = 0;
		this->energy_add = 0;
		this->leadership = 0;
		this->leadership_add = 0;
		this->unk7 = 0;
	}

	PBMSG_HEAD2 h;
	uint16 strength;
	uint16 strength_add;
	uint16 agility;
	uint16 agility_add;
	uint16 vitality;
	uint16 vitality_add;
	uint16 energy;
	uint16 energy_add;
	uint16 leadership;
	uint16 leadership_add;
	float unk7;
};

struct CHARACTER_ELEMENTAL_DATA
{
	CHARACTER_ELEMENTAL_DATA()
	{
		this->h.set(0xEC, 0x26, sizeof(CHARACTER_ELEMENTAL_DATA));
	}

	PBMSG_HEAD2 h;
	int32 elemental_damage_min;
	int32 elemental_damage_max;
	int32 elemental_pvp_damage_min;
	int32 elemental_pvp_damage_max;
	int32 elemental_attack_success_rate;
	int32 elemental_pvp_attack_success_rate;
	int32 elemental_defense;
	int32 elemental_pvp_defense;
	int32 elemental_defense_success_rate;
	int32 elemental_pvp_defense_success_rate;
	int32 data_1;
	int32 data_2;
	int32 data_3;
	int32 data_4;
	int32 data_5;
	int32 data_6;
	int32 data_7;
	int32 data_8;
	int32 critical_elemental_dmg_rate;
	int32 increase_damage_pve;
	int32 increase_damage_pvp;
	int32 absorb_damage_pve;
	int32 absorb_damage_pvp;
	int32 absorb_shield;
	int32 absorb_hp;
	int32 bleeding_dmg;
	int32 paralyzing;
	int32 bind;
	int32 punish;
	int32 blind;
	int32 res_to_str_elem;
	int32 res_to_elem_dmg;
	int32 elemental_add_attack_damage;
	int32 elemental_add_defense;
};

struct PARTY_EFFECT_LIST_HEAD : C1_HEADER
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 count;
};

struct PARTY_EFFECT_LIST_BODY
{
	uint16 effect;
	uint32 count;
};
#pragma pack()

struct MONSTER_LIFE_UPDATE_SEND
{
	MONSTER_LIFE_UPDATE_SEND(uint16 index, int32 hp, int32 max_hp)
	{
		this->h.set(0xEC, 0x10, sizeof(MONSTER_LIFE_UPDATE_SEND));
		this->index[0] = SET_NUMBERH(index);
		this->index[1] = SET_NUMBERL(index);

		this->MaxHP[0] = SET_NUMBERH(SET_NUMBERHW(max_hp));
		this->MaxHP[1] = SET_NUMBERH(SET_NUMBERLW(max_hp));
		this->MaxHP[2] = SET_NUMBERL(SET_NUMBERHW(max_hp));
		this->MaxHP[3] = SET_NUMBERL(SET_NUMBERLW(max_hp));

		this->CurHP[0] = SET_NUMBERH(SET_NUMBERHW(hp));
		this->CurHP[1] = SET_NUMBERH(SET_NUMBERLW(hp));
		this->CurHP[2] = SET_NUMBERL(SET_NUMBERHW(hp));
		this->CurHP[3] = SET_NUMBERL(SET_NUMBERLW(hp));
	}
	PBMSG_HEAD2 h; // C1:EC:10
	uint8 index[2];
	uint8 MaxHP[4];
	uint8 CurHP[4];
};

struct OPTION_CHANGE_SKIN
{
	PBMSG_HEAD2 h;
	uint8 change_skin;
};
#pragma pack(1)
struct LIFE_SEND : C1_HEADER
{
	LIFE_SEND(uint8 type, int32 life, int32 shield)
	{
		this->Set(0x26, sizeof(LIFE_SEND));
		this->type = type;
		this->flag = 0;
		this->life[0] = SET_NUMBERH(GET_MAX_WORD_VALUE(life));
		this->life[1] = SET_NUMBERL(GET_MAX_WORD_VALUE(life));
		this->shield[0] = SET_NUMBERH(GET_MAX_WORD_VALUE(shield));
		this->shield[1] = SET_NUMBERL(GET_MAX_WORD_VALUE(shield));

		this->ViewHP = life;
		this->ViewSD = shield;
	}

	uint8 type;
	uint8 life[2];
	uint8 flag;
	uint8 shield[2];
	//EXTRA
	uint32 ViewHP;
	uint32 ViewSD;
};

struct MANA_SEND : C1_HEADER
{
	MANA_SEND(uint8 type, int32 mana, int32 stamina)
	{
		this->Set(0x27, sizeof(MANA_SEND));
		this->type = type;
		this->mana[0] = SET_NUMBERH(GET_MAX_WORD_VALUE(mana));
		this->mana[1] = SET_NUMBERL(GET_MAX_WORD_VALUE(mana));
		this->bp[0] = SET_NUMBERH(GET_MAX_WORD_VALUE(stamina));
		this->bp[1] = SET_NUMBERL(GET_MAX_WORD_VALUE(stamina));
		this->ViewMP = mana;
		this->ViewBP = stamina;
	}

	uint8 type;
	uint8 mana[2];
	uint8 bp[2];
	//EXTRA
	uint32 ViewMP;
	uint32 ViewBP;
};
#pragma pack()
struct CHARACTER_ATTACK_SPEED_SEND
{
	CHARACTER_ATTACK_SPEED_SEND(uint32 attack_speed, uint32 magic_speed)
	{
		this->h.set(0xEC, 0x30, sizeof(CHARACTER_ATTACK_SPEED_SEND));
		this->attack_speed = attack_speed;
		this->magic_speed = magic_speed;
	}

	PBMSG_HEAD2 h; // C1:EC:30
	uint32 attack_speed;
	uint32 magic_speed;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//
struct PENTAGRAM_JEWEL_INSERT_RECV
{
	PBMSG_HEAD2 h; // C1:EC:00
	uint32 target_slot;
	uint32 source_slot;
};
#pragma pack(1)
struct PMSG_PENTAGRAM_JEWEL_REMOVE_RECV
{
	PBMSG_HEAD2 h; // C1:EC:01
	uint32 source_slot;
	uint8 socket_slot;
	uint32 blessing_slot;
};

struct PMSG_PENTAGRAM_JEWEL_REFINE_RECV
{
	PBMSG_HEAD2 h; // C1:EC:02
	uint8 type;
	uint8 slot[PENTAGRAM_MIX_MAX_SLOT];
	uint8 data;
};
#pragma pack()
struct PMSG_PENTAGRAM_JEWEL_UPGRADE_RECV
{
	PBMSG_HEAD2 h; // C1:EC:03
	uint8 type;
	uint8 data[3];
	uint8 slot[PENTAGRAM_MIX_UPGRADE_MAX_SLOT];
};
#pragma pack(1)
struct PENTAGRAM_ERRTEL_RANK_DATA
{
	uint8 index;
	uint8 level;
};

struct PENTAGRAM_UPGRADE_RACIANCE_SLOT
{
	PBMSG_HEAD2 h;
	uint32 pentagram_slot;
	uint32 expansion_item_slot;
};

struct PENTAGRAM_UPGRADE_RACIANCE_SLOT_RESULT
{
	PENTAGRAM_UPGRADE_RACIANCE_SLOT_RESULT()
	{
		this->h.set(0xEC, 0x05, sizeof(PENTAGRAM_UPGRADE_RACIANCE_SLOT_RESULT));
		this->result = 1;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct PENTAGRAM_JEWEL_INSERT_SEND
{
	PENTAGRAM_JEWEL_INSERT_SEND()
	{
		this->h.set(0xEC, 0x00, sizeof(PENTAGRAM_JEWEL_INSERT_SEND));
		this->result = 0;
	}

	PBMSG_HEAD2 h; // C1:EC:00
	uint8 result;
	uint8 type;
	uint8 index;
	uint8 attribute;
	uint8 item_section;
	uint16 item_type;
	uint8 item_level;
	PENTAGRAM_ERRTEL_RANK_DATA rank_data[MAX_SOCKET_SLOT];
};

struct PMSG_PENTAGRAM_JEWEL_REMOVE_SEND
{
	PMSG_PENTAGRAM_JEWEL_REMOVE_SEND()
	{
		this->h.set(0xEC, 0x01, sizeof(PMSG_PENTAGRAM_JEWEL_REMOVE_SEND));
		this->result = 0;
		this->type = 0;
		this->index = 0;
	}

	PBMSG_HEAD2 h; // C1:EC:01
	uint8 result;
	uint8 type;
	uint8 index;
};

struct PMSG_PENTAGRAM_JEWEL_RESULT_SEND
{
	PMSG_PENTAGRAM_JEWEL_RESULT_SEND(uint8 result)
	{
		this->h.set(0xEC, 0x04, sizeof(PMSG_PENTAGRAM_JEWEL_RESULT_SEND));
		this->result = result;
	}

	PBMSG_HEAD2 h; // C1:EC:04
	BYTE result;
};

struct PENTAGRAM_ERRTEL_DATA_BODY
{
	uint8 type;
	uint8 index;
	uint8 attribute;
	uint8 item_type;
	uint16 item_index;
	uint8 item_level;
	PENTAGRAM_ERRTEL_RANK_DATA rank[MAX_SOCKET_SLOT];
};

struct PENTAGRAM_ERRTEL_DATA_HEAD : C2_HEADER_SUB
{
	uint8 result;
	uint8 count;
	uint8 type;
};
#pragma pack()
struct MONSTER_ATTACK_REGION
{
	MONSTER_ATTACK_REGION(uint16 Class, uint8 sx, uint8 sy, uint8 x, uint8 y)
	{
		this->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_ATTACK_REGION, sizeof(MONSTER_ATTACK_REGION));
		this->SetClass(Class);
		this->sx = sx;
		this->sy = sy;
		this->x = x;
		this->y = y;
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Class);
	uint8 sx;
	uint8 sy;
	uint8 x;
	uint8 y;
};

struct EXPANDED_WAREHOUSE_REPORT
{
	EXPANDED_WAREHOUSE_REPORT(uint8 result)
	{
		this->h.set(HEADCODE_EXPANDED_NOTIFY, sizeof(EXPANDED_WAREHOUSE_REPORT));
		this->result = result;
	}
	PBMSG_HEAD h;
	uint8 result;
};

struct WEATHER_UPDATE
{
	WEATHER_UPDATE(uint8 weather)
	{
		this->h.set(HEADCODE_WEATHER, sizeof(WEATHER_UPDATE));
		this->weather = weather;
	}
	PBMSG_HEAD h;
	uint8 weather;
};

struct EVENT_STATE
{
	EVENT_STATE(uint8 event_id, uint8 state)
	{
		this->h.set(HEADCODE_EVENT_STATE, sizeof(EVENT_STATE));
		this->state = state;
		this->event_id = event_id;
	}
	PBMSG_HEAD h;
	uint8 state;
	uint8 event_id;
};

struct POSITION_SET
{
	PBMSG_HEAD h;
	uint8 x;
	uint8 y;
};

struct CHAIN_SKILL_BODY
{
	uint16 target;
};

struct CHAIN_SKILL_HEAD
{
	PBMSG_HEAD2 h;
	uint8 skillH;
	uint8 skillL;
	uint16 index;
	uint8 count;
};

struct KILL_COUNT
{
	KILL_COUNT(uint8 count)
	{
		this->h.set(HEADCODE_KILL_COUNT, 0x01, sizeof(KILL_COUNT));
		this->count = count;
	}
	PBMSG_HEAD2 h;
	uint8 count;
};
#pragma pack(1)
struct PK_LEVEL
{
	PK_LEVEL(uint16 index, uint8 level)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_PK_LEVEL, sizeof(PK_LEVEL));
		this->SetIndex(index);
		this->pk_level = level;
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Index);
	uint8 pk_level;
};

struct PREVIEW_SEND
{
	PREVIEW_SEND(uint16 index)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_PREVIEW, sizeof(PREVIEW_SEND));
		this->SetIndex(index);
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Index);
	uint8 preview[MAX_PREVIEW_DATA];
};
#pragma pack()
/* ============================================
			CASH SHOP
============================================ */

struct EXPIRE_ITEM_HEAD
{
	EXPIRE_ITEM_HEAD(uint8 count)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_EXPIRE, sizeof(EXPIRE_ITEM_HEAD));
		this->count = count;
	}
	PBMSG_HEAD2 h;
	uint8 count;
};

#pragma pack(push, 1)
struct EXPIRE_ITEM_DATA
{
	EXPIRE_ITEM_DATA(uint16 item, uint16 slot, long expire_date)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_EXPIRE_DATA, sizeof(EXPIRE_ITEM_DATA));
		this->item = item;
		this->slot = slot;
		this->expire_date = expire_date;
	}
	PBMSG_HEAD2 h;
	uint16 item;
	uint16 slot;
	long expire_date;
};
#pragma pack(pop)

#pragma pack(1)
struct CASH_SHOP_DATE
{
	CASH_SHOP_DATE(uint8 type, uint16 id, uint16 year, uint16 month)
	{
		this->h.set(HEADCODE_CASH_SHOP, type == CASH_SHOP_DATE_ITEM ? SUBCODE_CASH_SHOP_DATE_ITEM: SUBCODE_CASH_SHOP_DATE_BANNER, sizeof(CASH_SHOP_DATE));
		this->id = id;
		this->year = year;
		this->month = month;
	}
	PBMSG_HEAD2 h;
	uint16 id;
	uint16 year;
	uint16 month;
};

struct CASH_SHOP_OPEN_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 action;
};

struct CASH_SHOP_OPEN_RESULT
{
	CASH_SHOP_OPEN_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_OPEN, sizeof(CASH_SHOP_OPEN_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};
#pragma pack(1)
struct CASH_SHOP_COIN
{
	CASH_SHOP_COIN(uint8 result, double coin_c, double coin_p, double coin_goblin)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_COIN, sizeof(CASH_SHOP_COIN));
		this->result = result;
		this->coin_c = coin_c;
		this->coin_p = coin_p;
		this->coin_goblin = coin_goblin;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	double coin_c;
	double coin_p;
	double coin_goblin;

	/*
	C1,1D,D2,01,00,
	00,00,00,00,00,00,00,00,
	00,00,00,00,00,00,00,00,
	00,00,00,00,00,8B,B4,40,
	*/
};

struct CASH_SHOP_ITEM_LIST_REQUEST
{
	CASH_SHOP_ITEM_LIST_REQUEST()
	{
		this->h.set(0, 0, sizeof(CASH_SHOP_ITEM_LIST_REQUEST));
	}

	PBMSG_HEAD2 h;
	int32 page;
	uint8 type;
	uint8 unk1;
};
#pragma pack()
struct CASH_SHOP_ITEM_LIST_RESULT
{
	CASH_SHOP_ITEM_LIST_RESULT(uint16 page_current, uint16 page_total)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_LIST_PAGE, sizeof(CASH_SHOP_ITEM_LIST_RESULT));
		this->unk1 = 0;
		this->unk2 = 0;
		this->page_current = page_current;
		this->page_total = page_total;
	}
	PBMSG_HEAD2 h;
	uint16 unk1;
	uint16 unk2;
	uint16 page_current;
	uint16 page_total;
};

struct CASH_SHOP_ITEM_LIST_DATA
{
	CASH_SHOP_ITEM_LIST_DATA()
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_LIST_DATA_2, sizeof(CASH_SHOP_ITEM_LIST_DATA));
		this->unk1 = 0;
		this->unk2 = 0x50;
	}
	PBMSG_HEAD2	h;
	uint32 serial;
	uint32 code;
	uint32 serial_cash_shop;
	uint32 product;
	uint32 option;
	double unk1;
	uint8 unk2;
};
#pragma pack(1)
struct CASH_SHOP_ITEM_BUY_REQUEST
{
	PBMSG_HEAD2 h;
	uint32 package; // Tercera Columna de IBSPackage.txt
	uint32 category; // CashShopCategory Table
	uint32 option; // ID de la opcion del item ( cuando podes elegir la duracion del item )
	uint16 item_id; // Items.txt
};

/*
	CASH_SHOP_ITEM_BUY_RESULT
	Result = 0 -> Your purchase has been made
	Result = 1 -> You don't have enought coins
	Result = 2 -> You don't have enought space in store
	Result = 3 -> This item has sold out
	Result = 4 -> This item is not currently available
	Result = 5 -> This item is not longer available
	Result = 6 -> This item cannot be bought
	Result = 7 -> Event Items cannot be bought
	Result = 8 -> You have exceeded the maximum number of times you can purchase event items
*/

struct CASH_SHOP_ITEM_BUY_RESULT
{
	CASH_SHOP_ITEM_BUY_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_BUY, sizeof(CASH_SHOP_ITEM_BUY_RESULT));
		this->result = result;
		memset(this->data, 0xFF, 4);
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 data[4];
};

struct CASH_SHOP_ITEM_GIFT_REQUEST
{
	PBMSG_HEAD2 h;
	uint32 package;
	uint32 category;
	uint32 option;
	uint32 unk1;
	char name[MAX_CHARACTER_LENGTH + 1];
	char message[200];
};

/*
	CASH_SHOP_ITEM_GIFT_RESULT
	Result = 0 -> Gift has been delivered
	Result = 1 -> You don't have enought coins
	Result = 2 -> Not enought space in store
	Result = 3 -> Can't find player %s
	Result = 4 -> Item has been sold out
	Result = 5 -> This item is not longer available
	Result = 6 -> This item is not longer available
	Result = 7 -> This item cannot be sent as gift
	Result = 8 -> Event Items cannot sent as gift
	Result = 9 -> You have exceeded the maximum number of times you can purchase event items
	Result = 10 -> You have select wrong WCoin Type
*/

struct CASH_SHOP_ITEM_GIFT_RESULT
{
	CASH_SHOP_ITEM_GIFT_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_GIFT, sizeof(CASH_SHOP_ITEM_GIFT_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct CASH_SHOP_ITEM_USE_REQUEST
{
	PBMSG_HEAD2	h;
	uint32 serial;
	uint32 code;
	uint16 item;
	uint8 unk;
};
#pragma pack()
/*
	PMSG_CASHSHOP_ITEMUSE_ANS
	Result = 0 -> The item has been used
	Result = 1 -> [Use Storage] does not exist
	Result = 2 -> You can recive this item from a MU Master
	Result = 3 -> An active color plan exists in the selected period
	Result = 4 -> An active personal fixed plan exists in the selected period
*/

struct CASH_SHOP_ITEM_USE_RESULT
{
	CASH_SHOP_ITEM_USE_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_USE, sizeof(CASH_SHOP_ITEM_USE_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2	h;
	uint8 result;
};

struct CASH_SHOP_ITEM_USE_REQUEST_NEW
{
	PBMSG_HEAD2	h;
	uint16 item;
};

struct CASH_SHOP_ITEM_USE_RESULT_NEW
{
	CASH_SHOP_ITEM_USE_RESULT_NEW(uint8 result, uint16 item)
	{
		this->h.set(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_ITEM_USE_NEW, sizeof(CASH_SHOP_ITEM_USE_RESULT_NEW));
		this->result = result;
		this->item = item;
	}

	PBMSG_HEAD2	h;
	uint16 item;
	uint8 result;
};
#pragma pack()

struct NPC_INTERACTION
{
	NPC_INTERACTION(uint8 npc, uint8 result)
	{
		this->h.set(HEADCODE_NPC_INTERACTION, npc, sizeof(NPC_INTERACTION));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct TEXT_SAY
{
	TEXT_SAY(uint16 index, const char * message)
	{
		this->h.set(HEADCODE_TEXT_SAY, sizeof(TEXT_SAY));
		this->SetIndex(index);
		memset(this->message, 0, MAX_CHAT_LENGTH);
		memcpy(this->message, message, strlen(message));
		this->message[strlen(message) + 1] = 0;
	}
	PBMSG_HEAD h;
	INDEX_DATA(Index);
	char message[MAX_CHAT_LENGTH];
};

struct JEWEL_MIX_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 type;
	uint8 level;
};

struct JEWEL_MIX_RESULT
{
	JEWEL_MIX_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_JEWEL_MIX, SUBCODE_JEWEL_MIX, sizeof(JEWEL_MIX_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct JEWEL_UNMIX_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 type;
	uint8 level;
	uint8 slot;
};

struct JEWEL_UNMIX_RESULT
{
	JEWEL_UNMIX_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_JEWEL_MIX, SUBCODE_JEWEL_UNMIX, sizeof(JEWEL_UNMIX_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct INVENTORY_ITEM_USE_ANSWER
{
	INVENTORY_ITEM_USE_ANSWER(uint8 slot, uint8 use_type)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_INVENTORY_ITEM_USE, sizeof(INVENTORY_ITEM_USE_ANSWER));
		this->slot = slot;
		this->use_type = use_type;
	}
	PBMSG_HEAD2	h;
	uint8 slot;
	uint8 use_type;
};
#pragma pack(1)
struct INVENTORY_ITEM_USE
{
	PBMSG_HEAD2	h;
	uint8 slot;
	uint8 use_type;
};
#pragma pack()
//struct SPECIAL_ITEM_USE
//{
//	SPECIAL_ITEM_USE(uint8 type, uint16 time)
//	{
//		this->h.setE(HEADCODE_SPECIAL_ITEM_USE, sizeof(SPECIAL_ITEM_USE));
//		this->type = type;
//		this->time = time;
//	}
//	PBMSG_HEAD h;
//	uint8 type;
//	uint16 time;
//};

struct SPECIAL_SKILL_SEND
{
	SPECIAL_SKILL_SEND(uint16 caster, uint16 target, uint16 skill)
	{
		this->h.set(HEADCODE_SPECIAL_SKILL, sizeof(SPECIAL_SKILL_SEND));
		this->SetSkill(skill);
		this->caster = caster;
		this->target = target;
	}
	PBMSG_HEAD h;
	INDEX_DATA(Skill);
	uint16 caster;
	uint16 target;
};
#pragma pack(1)
struct FIREWORK_SEND
{
	FIREWORK_SEND(uint8 count, uint8 x, uint8 y)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_COMMAND, sizeof(FIREWORK_SEND));
		this->count = count;
		this->x = x;
		this->y = y;
	}
	PBMSG_HEAD2 h;
	uint8 count;
	uint8 x;
	uint8 y;
};
#pragma pack()
struct PET_ATTACK_COMMAND
{
	PET_ATTACK_COMMAND(uint8 pet, uint8 skill, uint16 caster, uint16 target)
	{
		this->h.set(HEADCODE_PET_ATTACK_COMMAND, sizeof(PET_ATTACK_COMMAND));
		this->pet = pet;
		this->skill = skill;
		this->SetCaster(caster);
		this->SetTarget(target);
	}
	PBMSG_HEAD h;
	uint8 pet;
	uint8 skill;
	INDEX_DATA(Caster);
	INDEX_DATA(Target);
};

struct PET_SEND_COMMAND
{
	PET_SEND_COMMAND(uint8 pet, uint8 command, uint16 target)
	{
		this->h.set(HEADCODE_PET_COMMAND, sizeof(PET_SEND_COMMAND));
		this->pet = pet;
		this->command = command;
		this->SetTarget(target);
	}
	PBMSG_HEAD h;
	uint8 pet;
	uint8 command;
	INDEX_DATA(Target);
};
#pragma pack(1)
struct PLAYER_RESPAWN 
{
	PLAYER_RESPAWN()
	{
		this->h.setE(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_RESPAWN, sizeof(PLAYER_RESPAWN));
		this->labyrinth_id = 0;
		this->unk2 = 0;
		this->unk3 = 0;
	}
	PBMSG_HEAD2 h;
	uint8 x; //4
	uint8 y; //5
	uint16 world; //6
	uint8 direction; //8
	uint8 labyrinth_id; //9
	uint16 power[POWER_MAX]; //10
	uint8 experience[8]; //18
	uint8 unk2; //26
	uint8 unk3; //27
	uint32 zen; //28

	///- Custom
	uint32 ViewHP;
	uint32 ViewMP;
	uint32 ViewAG;
	uint32 ViewSD;
};
#pragma pack()

struct SKILL_CAST
{
	SKILL_CAST(uint16 index, uint16 skill, uint8 count)
	{
		this->h.set(HEADCODE_SKILL_CAST, sizeof(SKILL_CAST));
		this->SetIndex(index);
		this->skill = skill;
		this->count = count;
	}
	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint16 skill;
	uint8 count;
};

struct SERVER_MESSAGE
{
	SERVER_MESSAGE(uint8 number)
	{
		this->h.set(HEADCODE_SERVER_MESSAGE, sizeof(SERVER_MESSAGE));
		this->number = number;
	}
	PBMSG_HEAD h;
	uint8 number;
};

struct WORLD_ATTRIBUTE_SET_HEAD
{
	PBMSG_HEAD h;
	uint8 type;
	uint8 attr;
	uint8 map_set_type;
	uint8 count;
};

struct WORLD_ATTRIBUTE_SET_BODY
{
	uint8 x;
	uint8 y;
};

struct EVENT_REMAIN_TIME_RESULT
{
	EVENT_REMAIN_TIME_RESULT(uint8 event_id)
	{
		this->h.set(HEADCODE_EVENT_REMAIN_TIME_REQUEST, sizeof(EVENT_REMAIN_TIME_RESULT));
		this->event_id = event_id;
		this->remain_time = 0;
		this->entered_user = 0;
		this->remain_time_low = 0;
	}

	PBMSG_HEAD h;
	uint8 event_id;
	uint8 remain_time;
	uint8 entered_user;
	uint8 remain_time_low;
};

struct EVENT_REMAIN_TIME_REQUEST
{
	PBMSG_HEAD h;
	uint8 event_id;
	uint8 level;
};

struct SERVER_COMMAND
{
	SERVER_COMMAND(uint8 type, uint8 cmd1, uint8 cmd2)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_COMMAND, sizeof(SERVER_COMMAND));
		this->type = type;
		this->cmd1 = cmd1;
		this->cmd2 = cmd2;
	}
	PBMSG_HEAD2 h;
	uint8 type;
	uint8 cmd1;
	uint8 cmd2;
};
#pragma pack(1)
struct PET_INFO_RESULT : C1_HEADER
{
	PET_INFO_RESULT(uint8 pet, uint8 type, uint8 slot)
	{
		this->Set(HEADCODE_PET_INFO, sizeof(PET_INFO_RESULT));
		this->pet = pet;
		this->type = type;
		this->slot = slot;
		this->level = 0;
		this->unk = 0;
		this->experience = 0;
		this->durability = 0;
	}

	uint8 pet;
	uint8 type;
	uint8 slot;
	uint8 level;
	uint8 unk;
	int32 experience;
	uint8 durability;
};
#pragma pack()
struct PET_COMMAND
{
	PBMSG_HEAD h;
	uint8 pet;
	uint8 command;
	INDEX_DATA(Target);
};

struct PET_INFO_REQUEST
{
	PBMSG_HEAD h;
	uint8 pet;
	uint8 type;
	uint8 slot;
};
#pragma pack(1)
struct USE_STAT_FRUIT
{
	USE_STAT_FRUIT(uint8 result, uint8 fruit, uint32 stat)
	{
		this->h.set(HEADCODE_STAT_FRUIT, sizeof(USE_STAT_FRUIT));
		this->result = result;
		this->stat_value = stat;
		this->fruit_type = fruit;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint16 stat_value;
	uint8 fruit_type;
};

struct SUMMONED_HP
{
	SUMMONED_HP(uint8 hp)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SUMMONED_HP, sizeof(SUMMONED_HP));
		this->hp = hp;
	}
	PBMSG_HEAD2 h;
	uint8 hp;
};
#pragma pack()
struct CHAOS_MIX_ITEM_BODY
{
	uint8 slot;
	uint8 item_info[MAX_ITEM_INFO];
};

struct CHAOS_MIX_ITEM
{
	CHAOS_MIX_ITEM(uint8 type)
	{
		this->h.set(0x31, sizeof(CHAOS_MIX_ITEM));
		this->type = type;
		this->count = 0;
	}
	PWMSG_HEAD h;
	uint8 type;
	uint8 count;
};

struct CHAOS_MIX_ITEM_SINGLE
{
	CHAOS_MIX_ITEM_SINGLE(uint8 type)
	{
		this->h.set(0x31, sizeof(CHAOS_MIX_ITEM_SINGLE));
		this->type = type;
		this->count = 0;
		this->slot = -1;
		memset(this->item_info, 0, MAX_ITEM_INFO);
	}

	PWMSG_HEAD h;
	uint8 type;
	uint8 count;
	uint8 slot;
	uint8 item_info[MAX_ITEM_INFO];
};

struct CHAOS_MIX_CLOSE_RESULT
{
	CHAOS_MIX_CLOSE_RESULT()
	{
		this->h.set(HEADCODE_MIX_CLOSE, sizeof(CHAOS_MIX_CLOSE_RESULT));
	}
	PBMSG_HEAD h;
};
#pragma pack(1)
struct CHAOS_MIX_RESULT
{
	CHAOS_MIX_RESULT(uint8 result, uint8 const* item, uint8 slot)
	{
		this->h.set(HEADCODE_MIX_BUTTON, sizeof(CHAOS_MIX_RESULT));
		this->result = result;
		if ( item ) memcpy(this->item_info, item, MAX_ITEM_INFO);
		this->slot = slot;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 item_info[MAX_ITEM_INFO];
	uint8 slot;
};

struct CHAOS_MIX_BUTTON
{
	PBMSG_HEAD h;
	uint16 mix;
	uint8 data;
	uint8 count;
};

struct CHAOS_MIX_MASS_COMBINATION
{
	PBMSG_HEAD h;
	uint8 count;
	uint16 mix;
};

struct CHAOS_MIX_MASS_COMBINATION_RESULT
{
	CHAOS_MIX_MASS_COMBINATION_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_MASS_COMBINATION, sizeof(CHAOS_MIX_MASS_COMBINATION_RESULT));
		this->result = result;
	}
	PBMSG_HEAD h;
	uint8 result;
};
#pragma pack()

/* ============================================
			DUEL
============================================ */

struct DUEL_END_WINNER
{
	DUEL_END_WINNER(const char * winner, const char * looser)
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_END_WINNER, sizeof(DUEL_END_WINNER));
		memcpy(this->winner, winner, MAX_CHARACTER_LENGTH);
		memcpy(this->looser, looser, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD2 h;
	char winner[MAX_CHARACTER_LENGTH];
	char looser[MAX_CHARACTER_LENGTH];
};
#pragma pack(1)
struct DUEL_END
{
	DUEL_END(uint8 result)
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_END, sizeof(DUEL_END));
		this->result = result;
		this->SetIndex(-1);
		memset(this->name, 0, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	INDEX_DATA(Index);
	char name[MAX_CHARACTER_LENGTH];
};

struct DUEL_STATUS_BODY
{
	char name1[MAX_CHARACTER_LENGTH];
	char name2[MAX_CHARACTER_LENGTH];
	uint8 running;
	uint8 open;
};

struct DUEL_STATUS_HEAD
{
	DUEL_STATUS_HEAD()
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_STATUS, sizeof(DUEL_STATUS_HEAD));

		for ( uint8 i = 0; i < MAX_DUEL_ROOM; ++i )
		{
			memset(this->room[i].name1, 0, MAX_CHARACTER_LENGTH);
			memset(this->room[i].name2, 0, MAX_CHARACTER_LENGTH);
			this->room[i].running = 0;
			this->room[i].open = 0;
		}
	}
	PBMSG_HEAD2 h;
	DUEL_STATUS_BODY room[MAX_DUEL_ROOM];
};
#pragma pack()
struct DUEL_SPECTATOR_LIST
{
	DUEL_SPECTATOR_LIST()
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_SPECTATOR_LIST, sizeof(DUEL_SPECTATOR_LIST));
		this->count = 0;
		for ( uint8 i = 0; i < MAX_DUEL_SPECTATORS; ++i )
		{
			memset(this->name[i], 0, MAX_CHARACTER_LENGTH);
		}
	}
	PBMSG_HEAD2 h;
	uint8 count;
	char name[MAX_DUEL_SPECTATORS][MAX_CHARACTER_LENGTH];
};

struct DUEL_SPECTATOR_DEL
{
	DUEL_SPECTATOR_DEL(const char * name)
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_SPECTATOR_DEL, sizeof(DUEL_SPECTATOR_DEL));
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD2 h;
	char name[MAX_CHARACTER_LENGTH];
};

struct DUEL_SPECTATOR_ADD
{
	DUEL_SPECTATOR_ADD(const char * name)
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_SPECTATOR_ADD, sizeof(DUEL_SPECTATOR_ADD));
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD2 h;
	char name[MAX_CHARACTER_LENGTH];
};
#pragma pack(1)
struct DUEL_LIFE_BAR_UPDATE
{
	DUEL_LIFE_BAR_UPDATE(uint16 dueler1, uint16 dueler2, uint8 life1, uint8 life2, uint8 shield1, uint8 shield2)
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_LIFEBAR_UPDATE, sizeof(DUEL_LIFE_BAR_UPDATE));
		this->SetDueler1(dueler1);
		this->SetDueler2(dueler2);
		this->life[0] = life1;
		this->life[1] = life2;
		this->shield[0] = shield1;
		this->shield[1] = shield2;
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Dueler1);
	INDEX_DATA(Dueler2);
	uint8 life[2];
	uint8 shield[2];
};

struct DUEL_LIFE_BAR_INIT
{
	DUEL_LIFE_BAR_INIT(uint8 type, uint8 room, const char * name1, const char * name2, uint16 dueler1, uint16 dueler2)
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_LIFEBAR_INIT, sizeof(DUEL_LIFE_BAR_INIT));
		this->result = result;
		this->room = room;
		memcpy(this->name1, name1, MAX_CHARACTER_LENGTH);
		memcpy(this->name2, name2, MAX_CHARACTER_LENGTH);
		this->SetDueler1(dueler1);
		this->SetDueler2(dueler2);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 room;
	char name1[MAX_CHARACTER_LENGTH];
	char name2[MAX_CHARACTER_LENGTH];
	INDEX_DATA(Dueler1);
	INDEX_DATA(Dueler2);
};
#pragma pack()
struct DUEL_SCORE
{
	DUEL_SCORE(uint16 dueler1, uint16 dueler2, uint8 score1, uint8 score2)
	{
		this->h.set(HEADCODE_DUEL, SUBCODE_DUEL_SCORE, sizeof(DUEL_SCORE));
		this->SetDueler1(dueler1);
		this->SetDueler2(dueler2);
		this->score1 = score1;
		this->score2 = score2;
	}
	PBMSG_HEAD2 h;	
	INDEX_DATA(Dueler1);
	INDEX_DATA(Dueler2);
	uint8 score1;
	uint8 score2;
};

struct DUEL_SPECTATOR_REMOVE
{
	PBMSG_HEAD2 h;
	uint8 room;
	uint8 unk;
};
#pragma pack(1)
struct DUEL_SPECTATOR
{
	PBMSG_HEAD2 h;
	uint8 room;
};
#pragma pack()
struct DUEL_RESULT
{
	DUEL_RESULT(uint8 result, uint16 target, const char * name)
	{
		this->h.set(HEADCODE_DUEL, 0x01, sizeof(DUEL_RESULT));
		this->result = result;
		this->SetTarget(target);
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD2 h;
	uint8 result;
	INDEX_DATA(Target);
	char name[MAX_CHARACTER_LENGTH];
};
#pragma pack(1)
struct DUEL_ANSWER
{
	PBMSG_HEAD2 h;
	uint8 result;
	INDEX_DATA(Target);
	char name[MAX_CHARACTER_LENGTH];
};

struct DUEL_REQUEST_SEND
{
	DUEL_REQUEST_SEND(uint16 target, const char * name)
	{
		this->h.set(HEADCODE_DUEL, 0x02, sizeof(DUEL_REQUEST_SEND));
		this->SetTarget(target);
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Target);
	char name[MAX_CHARACTER_LENGTH];

	//C1 10 AA 02 2D 42 6C 44 69 61 6E 65 00 00 00 00
};

struct DUEL_REQUEST
{
	PBMSG_HEAD2 h;
	INDEX_DATA(Target);
	char name[MAX_CHARACTER_LENGTH];
};
#pragma pack()
/* ============================================
			MASTER SKILL
============================================ */
#pragma pack(1)
struct MASTER_SKILL_BODY
{
	uint8 position;
	uint8 level;
	uint8 data1[2];
	float current_data;
	float next_data;
	uint8 type;
	uint8 junk[3];
};

struct MASTER_SKILL_HEAD
{
	PWMSG_HEAD2 h;
	uint8 unk[3];
	uint32 count;
};
#pragma pack()
struct PMSG_NOTICE
{
	PMSG_NOTICE()
	{
		this->h.set(HEADCODE_NOTICE, sizeof(PMSG_NOTICE) - sizeof(this->Notice));
		memset(this->Notice, 0, 200);
		this->type = 0;
		this->btCount = 0;
		this->btTest = 0;
		this->wDelay = 0;
		this->dwColor = 0;
		this->btSpeed = 0;
	}
	PBMSG_HEAD h;
	uint8 type; //3
	uint8 btCount; //4
	uint8 btTest; //5
	uint16 wDelay;	//6
	int32 dwColor; //8
	uint8 btSpeed; //12
	char Notice[200]; //13
};
#pragma pack(1)
struct HELPER_OPEN_RESULT
{
	HELPER_OPEN_RESULT(uint8 result, uint32 zen, uint16 spent_time)
	{
		this->h.set(HEADCODE_EVENT_MISC, SUBCODE_HELPER_OPEN, sizeof(HELPER_OPEN_RESULT));
		this->result = result;
		this->spent_time = spent_time;
		this->zen = zen;
	}
	PBMSG_HEAD2 h;
	uint32 spent_time; // 4
	uint32 zen; // 8
	uint8 result; // 12
};
#pragma pack()
struct HELPER_OPEN
{
	PBMSG_HEAD2 h;
	uint8 result;
};
#pragma pack(1)
struct HELPER_SETTINGS_SEND
{
	HELPER_SETTINGS_SEND()
	{
		this->h.set(HEADCODE_HELPER_SETTINGS_SAVE, sizeof(HELPER_SETTINGS_SEND));
		this->result = 0x00;
		memset(this->unk, 0, 74);
	}
	PWMSG_HEAD h;
	uint8 result;
	uint8 item_pick_flag;
	uint8 range; // Parte baja = Rango Skill---- Parte alta = Rango Items ( 0x83 -> 3 = rango skill, 8 = rango item )
	uint16 movement_distance;
	uint16 primary_skill;
	uint16 secondary_skill_1;
	uint16 secondary_delay_1;
	uint16 secondary_skill_2;
	uint16 secondary_delay_2;
	uint16 time_space_casting;
	uint16 buff_skill[3];
	uint16 heal_percent;
	uint32 opt_flags_1;
	char item_name[PLAYER_HELPER_MAX_ITEMS][HELPER_ITEM_LENGTH];
	uint16 buff_item[3];
	uint32 opt_flags_2;
	uint8 unk[74];
};

struct HELPER_SETTINGS_SAVE
{
	PWMSG_HEAD h;
	uint8 result;
	uint8 item_pick_flag;
	uint8 range; // Parte baja = Rango Skill---- Parte alta = Rango Items ( 0x83 -> 3 = rango skill, 8 = rango item )
	uint16 movement_distance;
	uint16 primary_skill;
	uint16 secondary_skill_1;
	uint16 secondary_delay_1;
	uint16 secondary_skill_2;
	uint16 secondary_delay_2;
	uint16 time_space_casting;
	uint16 buff_skill[3];
	uint16 heal_percent;
	uint32 opt_flags_1;
	char item_name[PLAYER_HELPER_MAX_ITEMS][HELPER_ITEM_LENGTH];
	uint16 buff_item[3];
	uint32 opt_flags_2;
	uint8 unk[74];
};

struct BUFF_SEND
{
	BUFF_SEND(uint16 id, uint32 time, uint8 remove)
	{
		this->h.set(HEADCODE_BUFF, sizeof(BUFF_SEND));
		this->unk = 0x00;
		this->group = 0;
		this->effect_type = 0;
		this->remove = remove;
		memset(this->unk2, 0, 3);
		//this->unk2[2] = 0x0B;
		this->time = time;
		this->buff_id = id;
		memset(this->item_info, 0, MAX_ITEM_INFO);
		this->effect_value = 0;
	}

	PBMSG_HEAD h;
	uint8 unk; //3
	uint16 group; //4
	uint16 effect_type; //6
	uint8 remove; //8
	uint8 unk2[3]; //9
	uint32 time; //12
	uint16 buff_id; //16
	uint8 item_info[MAX_ITEM_INFO]; //18
	uint16 effect_value; //30
};

struct SKILL_STATE_SEND
{
	SKILL_STATE_SEND(uint16 buff, uint8 state, uint16 index)
	{
		this->h.set(HEADCODE_SKILL_STATE, sizeof(SKILL_STATE_SEND));
		this->state = state;
		this->buff = buff;
		this->SetIndex(index);
	}
	PBMSG_HEAD h;
	uint8 state;
	INDEX_DATA(Index);
	uint16 buff;
};
#pragma pack()
struct MAGIC_CANCEL 
{
	MAGIC_CANCEL(uint16 magic, uint16 index)
	{
		this->h.set(HEADCODE_CANCEL_MAGIC, sizeof(MAGIC_CANCEL));
		this->SetMagic(magic);
		this->SetIndex(index);
	}

	PBMSG_HEAD h;
	INDEX_DATA(Magic);
	INDEX_DATA(Index);
};

#pragma pack(1)
struct DARK_SIDE_RESULT
{
	DARK_SIDE_RESULT(uint16 magic)
	{
		this->h.set(HEADCODE_SKILL_DARK_SIDE, sizeof(DARK_SIDE_RESULT));
		this->result = 0;
		this->magic = magic;

		for ( uint8 i = 0; i < 10; ++i )
		{
			this->target[i] = 10000;
		}
	}
	PBMSG_HEAD h;
	uint8 result;
	uint16 magic;
	uint16 target[10];
};
#pragma pack()

struct DARK_SIDE_REQUEST
{
	PBMSG_HEAD h;
	INDEX_DATA(Magic);
	INDEX_DATA(Target);
};

struct NEW_MAGIC_ATTACK_RESULT
{
	NEW_MAGIC_ATTACK_RESULT(uint16 skill, uint16 source, uint16 target, bool success)
	{
		this->h.setE(HEADCODE_SKILL_NEW, sizeof(NEW_MAGIC_ATTACK_RESULT));
		this->SetMagic(skill);
		this->SetSource(source);
		this->SetTarget(target);
		this->m_propTarget[0] &= 0x7F;

		if ( success )
			this->m_propTarget[0] |= 0x80;
	}
	PBMSG_HEAD h;
	INDEX_DATA(Source);
	INDEX_DATA(Magic);
	INDEX_DATA(Target);
};
#pragma pack(1)
struct NEW_MAGIC_ATTACK
{
	PBMSG_HEAD h;
	uint8 indexH;
	uint8 skillH;
	uint8 indexL;
	uint8 skillL;

	uint16 GetMagic() const { return MAKE_NUMBERW(this->skillH, this->skillL); }
	uint16 GetTarget() const { return MAKE_NUMBERW(this->indexH, this->indexL); }
};
#pragma pack()
struct NORMAL_MAGIC_ATTACK_RESULT
{
	NORMAL_MAGIC_ATTACK_RESULT(uint16 skill, uint16 source, uint16 target)
	{
		this->h.setE(HEADCODE_NORMAL_MAGIC_ATTACK, sizeof(NORMAL_MAGIC_ATTACK_RESULT));
		this->SetMagic(skill);
		this->SetSource(source);
		this->SetTarget(target);
	}

	NORMAL_MAGIC_ATTACK_RESULT(uint16 skill, uint16 source, uint16 target, bool success)
	{
		this->h.setE(HEADCODE_NORMAL_MAGIC_ATTACK,sizeof(NORMAL_MAGIC_ATTACK_RESULT));
		this->SetMagic(skill);
		this->SetSource(source);
		this->SetTarget(target);
		this->m_propTarget[0] &= 0x7F;

		if ( success )
		{
			this->m_propTarget[0] |= 0x80;
		}
	}
	PBMSG_HEAD h;
	INDEX_DATA(Source);
	INDEX_DATA(Magic);
	INDEX_DATA(Target);
};
#pragma pack(1)
struct NORMAL_MAGIC_ATTACK
{
	uint16 GetMagic() const { return MAKE_NUMBERW(this->MagicH, this->MagicL); }
	uint16 GetTarget() const { return MAKE_NUMBERW(this->TargetH, this->TargetL); }

	PBMSG_HEAD h;
	uint8 TargetH;
	uint8 MagicH;
	uint8 TargetL;
	uint8 MagicL;
	uint32 attack_time;

	//39 03 07 01 D8 FE EE 5C 00 00 00 00
};

struct DURATION_MAGIC_ATTACK_RESULT
{
	DURATION_MAGIC_ATTACK_RESULT(uint16 skill, uint16 source, uint8 x, uint8 y, uint8 dir)
	{
		this->h.setE(HEADCODE_DURATION_MAGIC_ATTACK, sizeof(DURATION_MAGIC_ATTACK_RESULT));
		this->SkillH = SET_NUMBERH(skill);
		this->SkillL = SET_NUMBERL(skill);
		this->IndexH = SET_NUMBERH(source);
		this->IndexL = SET_NUMBERL(source);
		this->x = x;
		this->y = y;
		this->dir = dir;
	}

	PBMSG_HEAD h;
	uint8 x; //3
	uint8 y; //4
	uint8 dir; //5
	uint8 SkillH; //6
	uint8 IndexH; //7
	uint8 SkillL; //8
	uint8 IndexL; //9
};

struct DURATION_MAGIC_ATTACK
{
	uint16 GetMagic() const { return MAKE_NUMBERW(this->SkillH, this->SkillL); }
	uint16 GetTarget() const { return MAKE_NUMBERW(this->TargetH, this->TargetL); }

	PBMSG_HEAD h;
	int32 x;
	uint8 SkillH;
	int32 y;
	uint8 SkillL;
	uint8 Dir;
	uint8 TargetH;
	uint8 Dis;
	uint8 TargetL;
	uint8 target_pos;
	uint8 MagicKey;
	uint32 attack_time;
};

struct MULTITARGET_MAGIC_ATTACK_HEAD
{
	uint16 GetMagic() const { return MAKE_NUMBERW(this->MagicH, this->MagicL); }

	PBMSG_HEAD h;
	uint8 MagicH;
	uint8 Count;
	uint8 MagicL;
	uint8 x;
	uint8 Serial;
	uint8 y;
};

struct MULTITARGET_MAGIC_ATTACK_BODY
{
	uint16 GetTarget() const { return MAKE_NUMBERW(this->TargetH, this->TargetL); }

	uint8 TargetH;
	uint8 MagicKey;
	uint8 TargetL;
};

struct KILL_EXPERIENCE_SEND
{
	KILL_EXPERIENCE_SEND(uint16 index, uint32 experience, uint32 damage, uint8 flag)
	{
		this->h.setE(HEADCODE_KILL_EXPERIENCE, sizeof(KILL_EXPERIENCE_SEND));
		this->SetIndex(index);
		this->flag = 0;
		this->experienceH = SET_NUMBERHW(experience);
		this->experienceL = SET_NUMBERLW(experience);
		this->SetDamage(GET_MAX_WORD_VALUE(damage));
	}
	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint8 flag;
	uint16 experienceH;
	uint16 experienceL;
	INDEX_DATA(Damage);
};

struct NORMAL_LEVELUP_SEND
{
	NORMAL_LEVELUP_SEND()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_NORMAL_LEVEL_UP, sizeof(NORMAL_LEVELUP_SEND));
	}

	PBMSG_HEAD2 h;
	uint16 level;
	uint16 points;
	uint16 power_max[POWER_MAX];
	uint16 add_point;
	uint16 add_point_max;
	uint16 minus_point;
	uint16 minus_point_max;

	///- CUSTOM
	uint32 ViewMaxHP;
	uint32 ViewMaxMP;
	uint32 ViewMaxAG;
	uint32 ViewMaxSD;
};
#pragma pack()
struct SEND_EFFECT
{
	SEND_EFFECT(uint16 index, uint8 effect)
	{
		this->h.set(HEADCODE_EFFECT, sizeof(SEND_EFFECT));
		this->SetIndex(index);
		this->effect = effect;
	}
	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint8 effect;
};

struct POSITION_SET_SEND
{
	POSITION_SET_SEND(uint16 index, uint8 x, uint8 y)
	{
		this->h.set(HEADCODE_POSITION_SET, sizeof(POSITION_SET_SEND));
		this->SetIndex(index);
		this->x = x;
		this->y = y;
	}
	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint8 x;
	uint8 y;
};

struct DIE_SEND
{
	DIE_SEND(uint16 target, uint16 skill, uint16 killer)
	{
		this->h.set(HEADCODE_DIE, sizeof(DIE_SEND));
		this->SetTarget(target);
		this->SetSkill(skill);
		this->SetKiller(killer);
	}
	PBMSG_HEAD h;
	INDEX_DATA(Target);
	INDEX_DATA(Skill);
	INDEX_DATA(Killer);
};
#pragma pack(1)
struct ELEMENTAL_DAMAGE_SEND
{
	ELEMENTAL_DAMAGE_SEND(uint16 target, uint8 elemental_attribute, uint32 damage)
	{
		this->h.set(HEADCODE_ELEMENTAL_DAMAGE, sizeof(ELEMENTAL_DAMAGE_SEND));
		this->SetTarget(target);
		this->elemental_attribute = elemental_attribute;
		this->damage = damage;
		memset(this->unk2, 0, 2);
		this->data = 0;
		this->ViewHP = 0;
		this->ViewSD = 0;
	}

	PBMSG_HEAD h;
	INDEX_DATA(Target);
	uint8 elemental_attribute;
	uint8 unk2[2];
	uint32 damage;
	uint8 data;

	///- CUSTOM
	uint32 ViewHP;
	uint32 ViewSD;
};

struct DAMAGE_SEND
{
	DAMAGE_SEND(uint16 target, uint16 type, int32 life, int32 shield)
	{
		this->h.set(HEADCODE_ATTACK_NORMAL, sizeof(DAMAGE_SEND));
		this->SetTarget(target);
		this->damage = life;
		this->typeH = SET_NUMBERH(type);
		this->typeL = SET_NUMBERL(type);
		this->shieldH = SET_NUMBERH(GET_MAX_WORD_VALUE(shield));
		this->shieldL = SET_NUMBERL(GET_MAX_WORD_VALUE(shield));
		this->attribute = 0;
		this->ViewHP = 0;
		this->ViewSD = 0;
	}

	PBMSG_HEAD h;
	
	INDEX_DATA(Target); //3
	uint8 junk[3]; //5
	uint32 damage; //8
	uint8 typeH; //12
	uint8 typeL; //13
	uint8 shieldH; //14
	uint8 shieldL; //15
	uint8 attribute; //16

	///- CUSTOM
	uint32 ViewHP;
	uint32 ViewSD;
};

struct ATTACK_REQUEST
{
	PBMSG_HEAD h;
	INDEX_DATA(Target);
	uint8 action;
	uint8 dir_dis;
	uint32 attack_time;
};

struct MASTER_SKILL_POINT_ADD_RESULT
{
	MASTER_SKILL_POINT_ADD_RESULT()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MASTER_SKILL_POINT_ADD, sizeof(MASTER_SKILL_POINT_ADD_RESULT));
		this->result = 0;
	}

	PBMSG_HEAD2 h;
	uint16 result;
	uint16 points;
	uint32 position;
	uint32 skill;
	uint32 level;
	float CurrentInfo;
	float NextInfo;
	uint8 type;
};
#pragma pack()
struct MASTER_SKILL_POINT_ADD
{
	PBMSG_HEAD2 h;
	uint16 skill;
	uint16 level;
};

#pragma pack(1)
struct MASTER_LEVELUP_SEND
{
	MASTER_LEVELUP_SEND()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MASTER_LEVEL_UP, sizeof(MASTER_LEVELUP_SEND));
	}
	PBMSG_HEAD2 h;
	uint16 level;
	uint16 minuspoint;
	uint16 points;
	uint16 maxminuspoint;
	uint16 power_max[POWER_MAX];

	///- CUSTOM
	uint32 ViewMaxHP;
	uint32 ViewMaxMP;
	uint32 ViewMaxAG;
	uint32 ViewMaxSD;
};

struct MASTER_STATUS_SEND
{
	MASTER_STATUS_SEND()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MASTER_STATUS, sizeof(MASTER_STATUS_SEND));
	}
	PBMSG_HEAD2 h;
	uint16 level;
	uint8 experience[8];
	uint8 next_experience[8];
	uint16 points;
	uint16 power_max[POWER_MAX];

	///- CUSTOM
	uint32 ViewMaxHP;
	uint32 ViewMaxMP;
	uint32 ViewMaxAG;
	uint32 ViewMaxSD;
};
#pragma pack()

/* ============================================
			PERSONAL STORE
============================================ */
#pragma pack(1)
struct PERSONAL_STORE_TEXT_CHANGE_SEND : C1_HEADER_SUB
{
	PERSONAL_STORE_TEXT_CHANGE_SEND()
	{
		this->Set(0x3F, 0x10, sizeof(PERSONAL_STORE_TEXT_CHANGE_SEND));
		memset(this->description, 0, PERSONAL_STORE_NAME_LENGTH + 1);
		memset(this->seller, 0, MAX_CHARACTER_LENGTH + 1);
	}

	INDEX_DATA(Index);
	char description[PERSONAL_STORE_NAME_LENGTH + 1];
	char seller[MAX_CHARACTER_LENGTH + 1];
};

struct PERSONAL_STORE_CLOSE_RESULT : C1_HEADER_SUB
{
	PERSONAL_STORE_CLOSE_RESULT()
	{
		this->Set(0x3F, 0x03, sizeof(PERSONAL_STORE_CLOSE_RESULT));
	}

	uint8 result;
	INDEX_DATA(Index);
};

struct PERSONAL_STORE_VIEWPORT : C2_HEADER_SUB
{
	PERSONAL_STORE_VIEWPORT()
	{
		this->Set(0x3F, 0x00, sizeof(PERSONAL_STORE_VIEWPORT));
		this->count = 1;
		memset(this->description, 0, PERSONAL_STORE_NAME_LENGTH + 1);
	}

	uint8 count;
	INDEX_DATA(Index);
	char description[PERSONAL_STORE_NAME_LENGTH + 1];
};
#pragma pack()

/* ============================================
			TRADE
============================================ */

struct TRADE_CANCEL
{
	TRADE_CANCEL(uint8 result)
	{
		this->h.set(HEADCODE_TRADE_CANCEL, sizeof(TRADE_CANCEL));
		this->result = result;
	}

	PBMSG_HEAD h;
	uint8 result;
};

struct TRADE_ITEM_ADD
{
	TRADE_ITEM_ADD(uint8 slot, uint8 * item_info)
	{
		this->h.set(HEADCODE_TRADE_ITEM_ADD, sizeof(TRADE_ITEM_ADD));
		this->slot = slot;
		if ( item_info ) 
			memcpy(this->item_info, item_info, MAX_ITEM_INFO);
	}
	PBMSG_HEAD h;
	uint8 slot;
	uint8 item_info[MAX_ITEM_INFO];
};

struct TRADE_ITEM_DELETE
{
	TRADE_ITEM_DELETE(uint8 slot)
	{
		this->h.set(HEADCODE_TRADE_ITEM_DELETE, sizeof(TRADE_ITEM_DELETE));
		this->slot = slot;
	}
	PBMSG_HEAD h;
	uint8 slot;
};

struct TRADE_BUTTON_OK_ANSWER
{
	TRADE_BUTTON_OK_ANSWER(uint8 flag)
	{
		this->h.set(HEADCODE_TRADE_OK, sizeof(TRADE_BUTTON_OK_ANSWER));
		this->flag = flag;
	}
	PBMSG_HEAD h;
	uint8 flag;
};

struct TRADE_BUTTON_OK
{
	PBMSG_HEAD h;
	uint8 flag;
};

struct TRADE_ZEN_ANSWER
{
	TRADE_ZEN_ANSWER(uint32 zen)
	{
		this->h.set(HEADCODE_TRADE_ZEN, sizeof(TRADE_ZEN_ANSWER));
		this->zen = zen;
	}
	PBMSG_HEAD h;
	uint32 zen;
};

struct TRADE_ZEN_RESULT
{
	TRADE_ZEN_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_TRADE_MONEY, sizeof(TRADE_ZEN_RESULT));
		this->result = result;
	}
	PBMSG_HEAD h;
	uint8 result;
};

struct TRADE_ZEN
{
	PBMSG_HEAD h;
	uint32 zen;
};

struct TRADE_ANSWER
{
	PBMSG_HEAD h;
	uint8 answer;
};

struct TRADE_RESULT
{
	TRADE_RESULT(uint8 answer, const char * name, uint16 level, uint32 guild_id)
	{
		this->h.set(HEADCODE_TRADE_ANSWER, sizeof(TRADE_RESULT));
		this->answer = answer;
		memset(this->name, 0, MAX_CHARACTER_LENGTH);
		strcpy(this->name, name);
		this->level = level;
		this->guild_id = guild_id;
	}
	PBMSG_HEAD h;
	uint8 answer;
	char name[MAX_CHARACTER_LENGTH];
	uint16 level;
	uint32 guild_id;
};

struct TRADE_REQUEST_SEND
{
	TRADE_REQUEST_SEND(const char * name)
	{
		this->h.setE(HEADCODE_TRADE_REQUEST, sizeof(TRADE_REQUEST_SEND));
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
};

struct TRADE_REQUEST
{
	PBMSG_HEAD h;
	INDEX_DATA(Index);
};
#pragma pack(1)
struct ITEM_REPAIR
{
	PBMSG_HEAD h;
	uint8 slot;
	uint8 repair_way; // 1 = Personal Repair, 0 = Npc Repair, 3 = Pet Restore Life, 0x24 -> Dark Spirit
};
#pragma pack()
struct ITEM_REPAIR_RESULT
{
	ITEM_REPAIR_RESULT(uint32 zen)
	{
		this->h.set(HEADCODE_ITEM_REPAIR, sizeof(ITEM_REPAIR_RESULT));
		this->zen = zen;
	}
	PBMSG_HEAD h;
	uint32 zen;
};

#pragma pack(1)
struct ZEN_SEND
{
	ZEN_SEND(uint32 zen)
	{
		this->h.setE(HEADCODE_ZEN, sizeof(ZEN_SEND));
		this->result = 0xFE;
		this->SetIndex(-1);
		this->zen[0] = SET_NUMBERH(SET_NUMBERHW(zen));
		this->zen[1] = SET_NUMBERL(SET_NUMBERHW(zen));
		this->zen[2] = SET_NUMBERH(SET_NUMBERLW(zen));
		this->zen[3] = SET_NUMBERL(SET_NUMBERLW(zen));
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 indexH;
	uint8 junk;
	uint8 indexL;
	uint8 zen[4];

	void SetIndex(uint16 index)
	{
		this->indexH = SET_NUMBERH(index);
		this->indexL = SET_NUMBERL(index);
	}
};

struct TELEPORT_REQUEST_TARGET
{
	PBMSG_HEAD h;
	uint16 entry;
	uint8 x;
	uint8 y;
};

struct TELEPORT_RESULT
{
	TELEPORT_RESULT(uint16 id, uint16 world, uint8 x, uint8 y, uint8 direction)
	{
		this->h.setE(HEADCODE_TELEPORT, sizeof(TELEPORT_RESULT));
		//this->junk = 0x48;
		this->number = id;
		this->world = world;
		this->junk = 0;
		this->x = x;
		this->y = y;
		this->direction = direction;
	}
	PBMSG_HEAD h;
	uint8 junk;
	uint16 number;
	uint16 world;
	uint8 x;
	uint8 y;
	uint8 direction;
};

struct TELEPORT_REQUEST
{
	PBMSG_HEAD h;
	uint8 direction;
	uint16 gate;
	uint8 x;
	uint8 y;
	uint8 id;
	uint8 unk1;
	uint8 event_id;
};

struct SKILL_TELEPORT_REQUEST
{
	PBMSG_HEAD h;
	uint8 direction;
	uint16 gate;
	uint8 x;
	uint8 y;
};
#pragma pack()
/* ============================================
			PARTY
============================================ */
#pragma pack(push,1)
struct PARTY_ITEM_INFO
{
	PARTY_ITEM_INFO(uint16 index, uint32 item_info, uint8 level)
	{
		this->h.set(HEADCODE_PARTY_ITEM, sizeof(PARTY_ITEM_INFO));
		this->SetIndex(index);
		this->item_info = item_info;
		this->level = level;
	}

	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint8 unk[3];
	uint32 item_info;
	uint8 level;
};

struct PARTY_LIFE_HEAD
{
	PBMSG_HEAD h;
	uint8 count;
};

struct PARTY_LIFE_BODY
{
	uint8 life;
	uint8 mana;
	char name[MAX_CHARACTER_LENGTH + 1];
};

struct PARTY_USER_DELETE_REQUEST
{
	PBMSG_HEAD h;
	uint8 number;
	char name[MAX_CHARACTER_LENGTH + 1];
};
#pragma pack(pop)
struct PARTY_USER_DELETE
{
	PARTY_USER_DELETE()
	{
		this->h.set(HEADCODE_PARTY_DELETE, sizeof(PARTY_USER_DELETE));
	}

	PBMSG_HEAD h;
};

#pragma pack(1)
struct PARTY_LIST_BODY
{
	char name[MAX_CHARACTER_LENGTH]; //0
	uint16 number; //10
	uint16 level; //12
	uint16 world; //14
	uint8 world_x; //16
	uint8 world_y; //17
	uint16 unk18; //18
	int32 life; //20
	int32 life_max; //24
	uint32 server_code; //28
	int32 mana; //32
	int32 mana_max; //36
	uint8 helper; //40
	uint8 assistant; //41
	uint16 unk42; //42
};

struct PARTY_LIST_HEAD : C1_HEADER
{
	uint8 result;
	uint8 count;
};
#pragma pack()

struct PARTY_ANSWER
{
	PBMSG_HEAD h;
	uint8 result;
	INDEX_DATA(Target);
};

struct PARTY_REQUEST_RESULT
{
	PARTY_REQUEST_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_PARTY_ANSWER, sizeof(PARTY_REQUEST_RESULT));
		this->result = result;
	}
	PBMSG_HEAD h;
	uint8 result;
};

struct PARTY_REQUEST
{
	PARTY_REQUEST(uint16 target)
	{
		this->h.set(HEADCODE_PARTY_REQUEST, sizeof(PARTY_REQUEST));
		this->SetTarget(target);
	}

	PBMSG_HEAD h;
	INDEX_DATA(Target);
};

/* ============================================
			FRIEND
============================================ */

struct FRIEND_CHAT_ROOM_INVITATION_RESULT
{
	FRIEND_CHAT_ROOM_INVITATION_RESULT(uint8 result, uint32 window_guid)
	{
		this->h.set(HEADCODE_CHAT_ROOM_INVITATION, sizeof(FRIEND_CHAT_ROOM_INVITATION_RESULT));
		this->result = result;
		this->window_guid = window_guid;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint32 window_guid;
};

struct FRIEND_CHAT_ROOM_INVITATION_REQUEST
{
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
	uint16 room;
	uint32 window_guid;
};

struct FRIEND_CHAT_ROOM_CREATE_RESULT
{
	FRIEND_CHAT_ROOM_CREATE_RESULT(uint8 result, const char * ip, uint16 room, uint32 ticket, uint8 type, const char * name)
	{
		this->h.set(HEADCODE_CHAT_ROOM_CREATE, sizeof(FRIEND_CHAT_ROOM_CREATE_RESULT));
		if ( ip )
			memcpy(this->server_ip, ip, 15);

		this->room = room;
		this->ticket = ticket;
		this->type = type;
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
		this->result = result;
	}
	PBMSG_HEAD h;
	char server_ip[15];
	uint16 room;
	uint32 ticket;
	uint8 type;
	char name[MAX_CHARACTER_LENGTH];
	uint8 result;
};

struct FRIEND_CHAT_ROOM_CREATE_REQUEST
{
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
};

struct FRIEND_MAIL_DELETE
{
	PBMSG_HEAD h;
	uint16 id;
};

struct FRIEND_MAIL_DELETE_RESULT
{
	FRIEND_MAIL_DELETE_RESULT(uint8 result, uint16 id)
	{
		this->h.set(HEADCODE_FRIEND_MAIL_DELETE, sizeof(FRIEND_MAIL_DELETE_RESULT));
		this->result = result;
		this->id = id;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint16 id;
};

struct FRIEND_MAIL_READ
{
	PBMSG_HEAD h;
	uint16 id;
};
#pragma pack(1)
struct FRIEND_MAIL_READ_RESULT
{
	FRIEND_MAIL_READ_RESULT()
	{
		this->h.setE(HEADCODE_FRIEND_MAIL_READ, sizeof(FRIEND_MAIL_READ_RESULT));
		memset(this->message, 0, MAIL_MESSAGE_LENGTH);
	}
	PWMSG_HEAD h;
	uint16 id; //4
	int16 message_size; //6
	uint8 photo[MAX_PREVIEW_DATA]; //8
	uint8 direction; //28
	uint8 Action; //29
	char message[MAIL_MESSAGE_LENGTH]; //30
};
#pragma pack()
struct FRIEND_MAIL_LIST
{
	FRIEND_MAIL_LIST(uint16 id, const char * from, const char * date, const char * subject, uint8 read)
	{
		this->h.setE(HEADCODE_FRIEND_MAIL_LIST_SEND, sizeof(FRIEND_MAIL_LIST));
		this->id = id;
		memset(this->from, 0, MAX_CHARACTER_LENGTH);
		memset(this->date, 0, 30);
		memset(this->subject, 0, MAIL_SUBJECT_LENGTH);

		if ( from ) 
			memcpy(this->from, from, MAX_CHARACTER_LENGTH);

		if ( date ) 
			memcpy(this->date, date, 30);

		if ( subject ) 
			memcpy(this->subject, subject, MAIL_SUBJECT_LENGTH);
		this->read = read;
	}

	PBMSG_HEAD h;
	uint16 id;
	char from[MAX_CHARACTER_LENGTH];
	char date[30];
	char subject[MAIL_SUBJECT_LENGTH];
	uint8 read;
};

#pragma pack(1)
struct FRIEND_MAIL
{
	PWMSG_HEAD h;
	uint32 window_guid;
	char name[MAX_CHARACTER_LENGTH];
	char subject[MAIL_SUBJECT_LENGTH];
	uint8 direction;
	uint8 action;
	int16 mail_size;
	char mail[MAIL_MESSAGE_LENGTH];
};
#pragma pack()

struct FRIEND_MAIL_RESULT
{
	FRIEND_MAIL_RESULT(uint8 result, uint32 window_guid)
	{
		this->h.set(HEADCODE_FRIEND_MAIL, sizeof(FRIEND_MAIL_RESULT));
		this->result = result;
		this->window_guid = window_guid;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint32 window_guid;
};

struct FRIEND_STATE
{
	FRIEND_STATE(const char * name, uint8 server)
	{
		this->h.set(HEADCODE_FRIEND_STATE, sizeof(FRIEND_STATE));
		memcpy(this->name, name, MAX_CHARACTER_LENGTH);
		this->server = server;
	}
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
	uint8 server;
};

struct FRIEND_DEL
{
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
};

struct FRIEND_DEL_RESULT
{
	FRIEND_DEL_RESULT(uint8 result, const char * name)
	{
		this->h.set(HEADCODE_FRIEND_DEL, sizeof(FRIEND_DEL_RESULT));
		this->result = result;
		memset(this->name, 0, MAX_CHARACTER_LENGTH);
		if ( name ) 
			memcpy(this->name, name, MAX_CHARACTER_LENGTH);
	}
	PBMSG_HEAD h;
	uint8 result;
	char name[MAX_CHARACTER_LENGTH];
};

struct FRIEND_ADD
{
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
};

struct FRIEND_ADD_RESULT
{
	FRIEND_ADD_RESULT(uint8 result, const char * name, uint8 state)
	{
		this->h.set(HEADCODE_FRIEND_ADD, sizeof(FRIEND_ADD_RESULT));
		this->result = result;
		memset(this->name, 0, MAX_CHARACTER_LENGTH);
		if ( name ) 
			memcpy(this->name, name, MAX_CHARACTER_LENGTH);
		this->state = state;
	}
	PBMSG_HEAD h;
	uint8 result;
	char name[MAX_CHARACTER_LENGTH];
	uint8 state;
};
#pragma pack(1)
struct FRIEND_LIST_BODY
{
	char name[MAX_CHARACTER_LENGTH];
	uint8 server;
};

struct FRIEND_LIST_HEAD
{
	PWMSG_HEAD h;
	uint8 mail_count;
	uint8 mail_total;
	uint8 friend_count;
};
#pragma pack()

struct WAREHOUSE_MONEY_IN_OUT_RESULT
{
	WAREHOUSE_MONEY_IN_OUT_RESULT(uint8 result, uint32 w_money, uint32 i_money)
	{
		this->h.set(HEADCODE_WAREHOUSE_MONEY, sizeof(WAREHOUSE_MONEY_IN_OUT_RESULT));
		this->result = result;
		this->w_money = w_money;
		this->i_money = i_money;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint32 w_money;
	uint32 i_money;
};

struct WAREHOUSE_MONEY_IN_OUT
{
	PBMSG_HEAD h;
	uint8 type;
	uint32 money;
};

struct WAREHOUSE_STATE
{
	WAREHOUSE_STATE(uint8 state)
	{
		this->h.set(HEADCODE_WAREHOUSE_PASSWORD, sizeof(WAREHOUSE_STATE));
		this->state = state;
	}
	PBMSG_HEAD h;
	uint8 state;
};

struct WAREHOUSE_PASSWORD
{
	PBMSG_HEAD h;
	uint8 type;	// [0x00:Open Warehouse] [0x01:Set Pass] [0x02:Delete Pass] 3
	uint16 password;
	char secure_code[MAX_SECURE_CODE_LENGTH];
};

struct WAREHOUSE_CLOSE_RESULT
{
	WAREHOUSE_CLOSE_RESULT()
	{
		this->h.set(HEADCODE_WAREHOUSE_CLOSE, sizeof(WAREHOUSE_CLOSE_RESULT));
	}
	PBMSG_HEAD h;
};

struct ITEM_GET_RESULT
{
	ITEM_GET_RESULT(uint8 result, uint8 * item_info, uint16 index)
	{
		this->h.setE(HEADCODE_ITEM_GET, sizeof(ITEM_GET_RESULT));
		this->result = result;
		this->SetIndex(index);
		if (item_info)
		{
			memcpy(this->item_info, item_info, MAX_ITEM_INFO);
		}
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 indexH;
	uint8 junk;
	uint8 indexL;
	uint8 item_info[MAX_ITEM_INFO];

	void SetIndex(uint16 index)
	{
		this->indexH = SET_NUMBERH(index);
		this->indexL = SET_NUMBERL(index);
	}
};

struct ITEM_GET
{
	PBMSG_HEAD h;
	INDEX_DATA(Item);
};

struct ITEM_DROP_RESULT
{
	ITEM_DROP_RESULT(uint8 r, uint8 s)
	{
		this->h.set(HEADCODE_ITEM_DROP,sizeof(ITEM_DROP_RESULT));
		this->result = r;
		this->slot = s;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 slot;
};

struct ITEM_DROP
{
	ITEM_DROP(uint8 x, uint8 y, uint8 slot)
	{
		this->h.set(HEADCODE_ITEM_DROP, sizeof(ITEM_DROP));
		this->px = x;
		this->py = y;
		this->slot = slot;
	}

	PBMSG_HEAD h;
	uint8 px;
	uint8 py;
	uint8 slot;
};

struct ITEM_SELL_RESULT
{
	ITEM_SELL_RESULT(uint8 result, uint32 zen)
	{
		this->h.set(HEADCODE_ITEM_SELL, sizeof(ITEM_SELL_RESULT));
		this->result = result;
		this->zen = zen;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint32 zen;
};

struct ITEM_SELL
{
	PBMSG_HEAD h;
	uint8 slot;
};

struct ITEM_BUY_RESULT
{
	ITEM_BUY_RESULT(uint8 result, uint8 * item_info)
	{
		this->h.set(HEADCODE_ITEM_BUY, sizeof(ITEM_BUY_RESULT));
		this->result = result;
		if ( item_info ) memcpy(this->item_info, item_info, MAX_ITEM_INFO);
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 item_info[MAX_ITEM_INFO];
};

struct ITEM_BUY
{
	PBMSG_HEAD h;
	uint8 slot;
};

struct SHOP_ITEM_LIST_HEAD
{
	PWMSG_HEAD h;
	uint8 type; //4
	uint8 count; //5
};

struct SHOP_ITEM_LIST_BODY
{
	uint8 slot;
	uint8 item_info[MAX_ITEM_INFO];
};

struct TALK_TO_NPC_QUEST_SUPPORT
{
	TALK_TO_NPC_QUEST_SUPPORT(uint8 _flag, uint16 _npc, uint8 _family, uint8 _level, uint32 _contribution)
	{
		this->h.set(HEADCODE_QUEST_SUPPORT_NPC_TALK, sizeof(TALK_TO_NPC_QUEST_SUPPORT));
		this->flag = _flag;
		this->npc = _npc;
		this->family = _family;
		this->level = _level;
		this->contribution = _contribution;
	}
	PBMSG_HEAD h;
	uint8 flag;
	uint16 npc;
	uint8 family;
	uint8 level;
	uint32 contribution;
};

struct TALK_TO_NPC_RESULT
{
	TALK_TO_NPC_RESULT(uint8 result)
	{
		this->h.setE(HEADCODE_NPC_TALK, sizeof(TALK_TO_NPC_RESULT));
		this->result = result;

		memset(this->data, 0, 7);
	}
	PBMSG_HEAD h;
	uint8 result;
	uint8 data[7];
};

struct TALK_TO_NPC
{
	PBMSG_HEAD h;
	INDEX_DATA(Index);
};

struct VIEWPORT_CREATE_HEAD_C1
{
	PWMSG_HEAD h;
	uint8 count;
};
#pragma pack(1)
struct VIEWPORT_CREATE_GEN_COMPLETE
{
	VIEWPORT_CREATE_GEN_COMPLETE(uint8 family, uint16 index, uint8 level, uint32 contribution)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_GEN_SYSTEM_VIEWPORT, sizeof(VIEWPORT_CREATE_GEN_COMPLETE));
		this->count = 1;
		this->family = family;
		this->SetIndex(index);
		this->contribution = 0;
		this->level = level;
		memset(this->unk1, 0, 5);
		memset(this->unk2, 0, 3);
	}
	PWMSG_HEAD2 h;
	uint8 count;
	uint8 family;
	INDEX_DATA(Index);
	uint8 unk1[5];
	uint8 level;
	uint8 unk2[3];
	uint32 contribution;
};
#pragma pack()
struct VIEWPORT_CREATE_ITEM
{
	INDEX_DATA(Index);
	uint8 x;
	uint8 y;
	uint8 item_info[MAX_ITEM_INFO];
};

#pragma pack(1)
struct VIEWPORT_PET_DATA
{
	INDEX_DATA(ID);
	uint8 data;
};

struct VIEWPORT_BUFF_DATA
{
	uint16 buff;
	uint16 unk1;
	uint32 time;
};

struct VIEWPORT_CREATE
{
	INDEX_DATA(Index); //0
	uint8 x; //2
	uint8 y; //3
	uint8 preview[MAX_PREVIEW_DATA]; //4
	char name[MAX_CHARACTER_LENGTH]; //24
	uint8 tx; //34
	uint8 ty; //35
	uint8 dir_and_pk_level; //36
	uint8 elemental_attribute; //37
	VIEWPORT_PET_DATA Muun01; //38
	VIEWPORT_PET_DATA Muun02; //41
	VIEWPORT_PET_DATA Muun03; //44
	uint8 DisplayMuun; //47
	INDEX_DATA(Mount); //48
	uint8 dark_spirit; //50
	INDEX_DATA(Level); //51
	uint8 hp_max[4]; //53
	uint8 hp[4]; //57
	uint8 unk61; //61
	uint16 server; //62
	uint8 unk64; //64
	uint8 effect_count; //65
	uint8 unk66; //66
	uint8 unk67; //67
};

struct VIEWPORT_CREATE_CHANGE
{
	INDEX_DATA(Index); //0
	uint8 x; //2
	uint8 y; //3
	INDEX_DATA(Skin); //4
	char name[MAX_CHARACTER_LENGTH]; //6
	uint8 tx; //16
	uint8 ty; //17
	uint8 dir_and_pk_level; //18
	uint8 preview[MAX_PREVIEW_DATA]; //19
	uint8 elemental_attribute; //39
	VIEWPORT_PET_DATA Muun01; //40
	VIEWPORT_PET_DATA Muun02; //43
	VIEWPORT_PET_DATA Muun03; //46
	uint8 DisplayMuun; //49
	INDEX_DATA(Mount); //50
	uint8 dark_spirit; //52
	INDEX_DATA(Level); //53
	uint8 hp_max[4]; //55
	uint8 hp[4]; //59
	uint8 unk63; //63
	uint16 server; //64
	uint8 unk66; //66
	uint8 effect_count; //67
};

struct VIEWPORT_CREATE_MONSTER
{
	INDEX_DATA(Index); //0
	INDEX_DATA(Class); //2
	uint8 x; //4
	uint8 y; //5
	uint8 tx; //6
	uint8 ty; //7
	uint8 path; //8
	uint8 elemental_attribute; //9
	uint8 critical_damage_resistance; //10
	uint8 excellent_damage_resistance; //10
	INDEX_DATA(DebuffResistance);//12
	uint8 damage_absorb; //14
	uint8 elite; //15
	INDEX_DATA(Level); //16
	uint8 hp_max[4]; //18
	uint8 hp[4]; //22
	uint8 effect_count; //26
	uint8 unk27;
};

struct VIEWPORT_CREATE_MONSTER_SUMMONED
{
	INDEX_DATA(Index); //0
	INDEX_DATA(Class); //2
	uint8 x; //4
	uint8 y; //5
	uint8 tx; //6
	uint8 ty; //7
	uint8 path; //8
	char summoner_name[MAX_CHARACTER_LENGTH]; //9
	uint8 elemental_attribute; //19
	INDEX_DATA(Level); //20
	uint8 hp_max[4]; //22
	uint8 hp[4]; //26
	uint8 preview[MAX_PREVIEW_DATA]; //30
	uint8 type; //50
	uint8 effect_count; //51
};
#pragma pack()
struct VIEWPORT_GUILD_MEMBER
{
	INDEX_DATA(Index);
	INDEX_DATA(Guild);
};

struct VIEWPORT_GUILD_DATA
{
	INDEX_DATA(Index);
	char guild[MAX_GUILD_NAME_LENGTH];
	uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
};

struct CASTLE_SIEGE_MACHINE_NOTIFY_BODY
{
	uint8 type;
	INDEX_DATA(Class);
	INDEX_DATA(Index);
	uint8 x;
	uint8 y;
	uint8 preview[MAX_PREVIEW_DATA];
	uint8 effect_count;
};
#pragma pack(1)
struct VIEWPORT_MONSTER_CUSTOM_EQUIPMENT
{
	uint16 item;
	uint8 level;
	uint8 excellent_ancient;
};

struct VIEWPORT_MONSTER_CUSTOM_DATA
{
	VIEWPORT_MONSTER_CUSTOM_DATA()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MONSTER_CUSTOM_DATA, sizeof(VIEWPORT_MONSTER_CUSTOM_DATA));
		this->index = -1;
		this->type = 0;
		memset(this->name, 0, 32);
		this->size = 0.0f;
		this->race = 0;

		for ( int32 i = 0; i < 12; ++i )
		{
			this->equipment[i].item = -1;
			this->equipment[i].level = 0;
			this->equipment[i].excellent_ancient = 0;
		}
	}

	PBMSG_HEAD2 h;
	uint16 index;
	uint8 type;
	char name[32];
	float size;
	uint8 race;
	VIEWPORT_MONSTER_CUSTOM_EQUIPMENT equipment[12];
};

struct VIEWPORT_DESTROY_COMPLETE
{
	VIEWPORT_DESTROY_COMPLETE(uint16 index)
	{
		this->h.set(HEADCODE_VIEWPORT_DESTROY_COMPLETE, sizeof(VIEWPORT_DESTROY_COMPLETE));
		this->count = 1;
		this->SetIndex(index);
	}

	PBMSG_HEAD h;
	uint8 count;
	INDEX_DATA(Index);
};

struct VIEWPORT_DESTROY_ITEM_COMPLETE
{
	VIEWPORT_DESTROY_ITEM_COMPLETE(uint16 index)
	{
		this->h.set(HEADCODE_VIEWPORT_ITEM_DESTROY, sizeof(VIEWPORT_DESTROY_ITEM_COMPLETE));
		this->count = 1;
		this->SetIndex(index);
	}

	PWMSG_HEAD h;
	uint8 count;
	INDEX_DATA(Index);
};
#pragma pack()
struct MOVE_REQUEST
{
	PBMSG_HEAD h;
	uint8 x;
	uint8 y;
	uint8 path[8];
};

struct MOVE_RESULT
{
	MOVE_RESULT(uint16 index, uint8 x, uint8 y, uint8 path)
	{
		this->h.set(HEADCODE_MOVE, sizeof(MOVE_RESULT));
		this->SetIndex(index);
		this->x = x;
		this->y = y;
		this->path = path;
	}
	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint8 x;
	uint8 y;
	uint8 path;
};

struct STAT_POINT_ADD
{
	PBMSG_HEAD2 h;
	uint8 stat_type;
};

struct STAT_POINT_ADD_RESULT
{
	STAT_POINT_ADD_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_POINT_ADD, sizeof(STAT_POINT_ADD_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint16 MaxLifeAndMana;
	uint16 MaxShield;
	uint16 MaxStamina;
};
#pragma pack(1)
struct USE_ITEM
{
	USE_ITEM(uint8 source_slot, uint8 target_slot, uint8 use_type)
	{
		this->h.set(HEADCODE_ITEM_USE, sizeof(USE_ITEM));
		this->source_slot = source_slot;
		this->target_slot = target_slot;
		this->use_type = use_type;
	}
	PBMSG_HEAD h;
	uint8 source_slot;
	uint8 target_slot;
	uint8 use_type; // Para las frutas
};
#pragma pack()
struct USE_ITEM_RESULT
{
	USE_ITEM_RESULT()
	{
		this->h.set(HEADCODE_ITEM_USE, sizeof(USE_ITEM_RESULT));
		this->flag = 0xFD;
	}
	PBMSG_HEAD h;
	uint8 flag;
};

struct ITEM_MOVE
{
	PBMSG_HEAD h;
	uint8 source;
	uint8 source_slot;
	uint8 source_item_info[MAX_ITEM_INFO];
	uint8 target;
	uint8 target_slot;
};

#pragma pack(1)
struct ITEM_MOVE_RESULT
{
	ITEM_MOVE_RESULT(uint8 result, uint8 position, const uint8 * item)
	{
		this->h.setE(HEADCODE_ITEM_MOVE, sizeof(ITEM_MOVE_RESULT));
		this->result = result;
		this->position = position;
		if ( item ) 
			memcpy(this->item, item, MAX_ITEM_INFO);
	}
	PBMSG_HEAD h;
	uint8 junk1;
	uint8 result;
	uint8 junk2;
	uint8 position;
	uint8 item[MAX_ITEM_INFO];
};

struct SKILL_LIST_HEAD
{
	PWMSG_HEAD2 h;
	uint8 count;
	uint8 mode;
};

struct SKILL_LIST_BODY
{
	uint8 position;
	uint16 skill;
	uint8 level;
	///- TODO: Es probable que no se usen position & level
};

struct EQUIPMENT_SEND
{
	EQUIPMENT_SEND(uint16 index, const uint8 * item)
	{
		this->h.set(HEADCODE_EQUIPMENT, sizeof(EQUIPMENT_SEND));
		this->SetIndex(index);
		if ( item ) 
			memcpy(this->item, item, MAX_ITEM_INFO);
	}
	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint8 item[MAX_ITEM_INFO];
	uint8 attribute;
};
#pragma pack()
struct ITEM_DURABILITY
{
	ITEM_DURABILITY(uint8 slot, uint8 durability, uint8 flag)
	{
		this->h.set(HEADCODE_ITEM_DURABILITY, sizeof(ITEM_DURABILITY));
		this->slot = slot;
		this->durability = durability;
		this->flag = flag;
	}
	PBMSG_HEAD h;
	uint8 slot;
	uint8 durability;
	uint8 flag;
};
#pragma pack(1)
struct INVENTORY_LIST_MODIFY
{
	INVENTORY_LIST_MODIFY(uint8 position, const uint8 * item)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_INVENTORY_LIST, sizeof(INVENTORY_LIST_MODIFY));
		this->position = position;
		if (item)
			memcpy(this->item, item, MAX_ITEM_INFO);
	}

	PBMSG_HEAD2 h;
	uint8 junk1;
	uint8 position;
	uint8 junk2;
	uint8 item[MAX_ITEM_INFO];
};
#pragma pack()
struct INVENTORY_LIST
{
	INVENTORY_LIST(uint8 position, uint8 flag)
	{
		this->h.set(HEADCODE_INVENTORY_LIST, sizeof(INVENTORY_LIST));
		this->position = position;
		this->flag = flag;
	}
	PBMSG_HEAD h;
	uint8 position;
	uint8 flag;
};
#pragma pack(1)
struct INVENTORY_LIST_HEAD
{
	PWMSG_HEAD2 h;
	uint8 count;
};

struct INVENTORY_LIST_BODY
{
	uint8 slot;
	uint8 item[MAX_ITEM_INFO];
};

struct GAME_OPTION
{
	GAME_OPTION()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_GAME_OPTION, sizeof(GAME_OPTION));
	}
	PBMSG_HEAD2 h;
	uint8 skill_key[SKILL_KEY_COUNT * 2];
	uint8 game_option;
	uint16 additional_options;
	uint8 key_q;
	uint8 key_w;
	uint8 key_e;
	uint8 key_r;
	uint8 chat_window;
	int32 qwer_level;
	uint8 linked_button[BUTTON_KEY_COUNT];
};
#pragma pack()

struct CHARACTER_NAME_CHECK_REQUEST
{
	PBMSG_HEAD2 h;
	char character_name[MAX_CHARACTER_LENGTH];
};
#pragma pack(1)
struct CHARACTER_NAME_CHECK_RESULT
{
	CHARACTER_NAME_CHECK_RESULT(const char * name, uint8 result)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_NAME_CHECK, sizeof(CHARACTER_NAME_CHECK_RESULT));
		memcpy(this->character_name, name, MAX_CHARACTER_LENGTH);
		this->result = result;
	}
	PBMSG_HEAD2 h;
	char character_name[MAX_CHARACTER_LENGTH];
	uint8 result;
};

struct CHARACTER_JOIN_RESULT
{
	CHARACTER_JOIN_RESULT()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_JOIN, sizeof(CHARACTER_JOIN_RESULT));
		this->unk1 = 0;
		memset(this->unk2, 0, 3);
	}

	PBMSG_HEAD2 h;
	uint8 x; //4
	uint8 y; //5
	uint16 world; //6
	uint8 direction; //8
	uint8 experience[8]; //9
	uint8 next_experience[8]; //17
	uint8 unk1; //25
	uint16 normal_points; //26
	uint16 strength; //28
	uint16 agility; //30
	uint16 vitality; //32
	uint16 energy; //34
	uint16 life; //36
	uint16 life_max; //38
	uint16 mana; //40
	uint16 mana_max; //42
	uint16 shield; //44
	uint16 shield_max; //46
	uint16 stamina; //48
	uint16 stamina_max; //50
	uint32 zen;  // 52
	uint8 PkLevel; // 56
	uint8 authority; // 57
	uint16 add_point; // 58
	uint16 add_point_max; // 60
	uint16 leadership; // 62
	uint16 minus_point;  // 64
	uint16 minus_point_max;  // 66
	uint8 ExpandedInventory;  // 68
	uint8 unk2[3]; // 69
	uint32 r_money; // 72

	///- CUSTOM
	uint32 ViewHP;
	uint32 ViewMaxHP;
	uint32 ViewMP;
	uint32 ViewMaxMP;
	uint32 ViewAG;
	uint32 ViewMaxAG;
	uint32 ViewSD;
	uint32 ViewMaxSD;
};
#pragma pack()

struct CHARACTER_JOIN_INFO
{
	CHARACTER_JOIN_INFO(uint8 result)
	{
		this->h.set(HEADCODE_CHARACTER_JOIN_SUCCESS, sizeof(CHARACTER_JOIN_INFO));
		this->result = 0;
		this->m_dwKey = result;
	}
	PBMSG_HEAD h;
	uint8 result;
	uint16 m_dwKey;
};

struct ACTION_REQUEST
{
	PBMSG_HEAD h;
	uint8 dir;
	uint8 action;
	INDEX_DATA(Target);
};

struct ACTION_RESULT
{
	ACTION_RESULT(uint16 index, uint16 target, uint8 direction, uint8 action)
	{
		this->h.set(HEADCODE_ACTION, sizeof(ACTION_RESULT));
		this->SetIndex(index);
		this->SetTarget(target);
		this->direction = direction;
		this->action = action;
	}

	PBMSG_HEAD h;
	INDEX_DATA(Index);
	uint8 direction;
	uint8 action;
	INDEX_DATA(Target);
};

struct PACKET_CHAT_DATA
{
	PACKET_CHAT_DATA(const char * name, const char * message)
	{
		this->h.set(0x00, sizeof(PACKET_CHAT_DATA));
		memset(this->name, 0, MAX_CHARACTER_LENGTH);
		memset(this->message, 0, MAX_CHAT_LENGTH);

		memcpy(this->name, name, strlen(name));
		memcpy(this->message, message, strlen(message));
	}
	PBMSG_HEAD h;
	char name[MAX_CHARACTER_LENGTH];
	char message[MAX_CHAT_LENGTH];
};

#pragma pack(1)
struct CHARACTER_LIST_HEAD 
{
	PWMSG_HEAD2 h;
	uint8 unk1;
	uint8 special_character;
	uint8 unk2;
	uint8 count;
	uint8 max_character;
	uint8 expanded_warehouse;
};

struct CHARACTER_LIST_BODY
{
	uint8 index; // 0
	char name[MAX_CHARACTER_LENGTH + 1]; // 1
	uint16 level; // 12
	uint8 authority; // 14
	uint8 preview[MAX_PREVIEW_DATA]; // 15
	uint8 guild_rank; //35
	uint8 pk_level; //36
	uint8 unk37; //37
};
#pragma pack()

struct CHARACTER_CREATE_REQUEST
{
	PBMSG_HEAD2 h;
	char character_name[MAX_CHARACTER_LENGTH];
	uint8 character_class;
};
#pragma pack(1)
struct CHARACTER_CREATE_RESULT
{
	CHARACTER_CREATE_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_CREATE, sizeof(CHARACTER_CREATE_RESULT));
		this->result = result;
		this->position = 0;
		memset(this->character_name, 0, MAX_CHARACTER_LENGTH);
		memset(this->equipment, 0xFF, 24);
		this->junk = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	char character_name[MAX_CHARACTER_LENGTH];
	uint8 position;
	uint16 level;
	uint8 character_class;
	uint8 equipment[24];
	uint8 junk;
};

struct CHARACTER_DELETE_REQUEST
{
	PBMSG_HEAD2 h;
	char character_name[MAX_CHARACTER_LENGTH];
	char secure_code[20];
};

struct CHARACTER_SELECT_REQUEST
{
	CHARACTER_SELECT_REQUEST()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_JOIN, sizeof(CHARACTER_SELECT_REQUEST));
	}
	PBMSG_HEAD2 h;
	char character_name[MAX_CHARACTER_LENGTH];
	uint8 index;
};
#pragma pack()
struct CHARACTER_DELETE_RESULT
{
	CHARACTER_DELETE_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_DELETE, sizeof(CHARACTER_DELETE_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

#pragma pack(1)
struct JOIN_ACCOUNT_SEND
{
	JOIN_ACCOUNT_SEND(uint16 index, uint8 result, const uint8 * client_version)
	{
		this->h.set(HEADCODE_ACCOUNT_DATA, SUBCODE_ACCOUNT_DATA_LOGIN_SEND, sizeof(JOIN_ACCOUNT_SEND));
		this->result = result;
		this->SetIndex(index);
		memcpy(this->client_version, client_version, 5);
		this->key = 0;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 junk1;
	uint8 indexH;
	uint8 junk2[4];
	uint8 indexL;
	uint8 client_version[5];
	uint32 key;

	void SetIndex(uint16 index)
	{
		this->indexH = SET_NUMBERH(index);
		this->indexL = SET_NUMBERL(index);
	}
};

struct LOGIN_REQUEST
{
	PBMSG_HEAD2 h;
	char account[MAX_ACCOUNT_LENGTH];
	char password[MAX_PASSWORD_LENGTH];
	uint32 TickCount;
	uint8 client_version[5];
	char client_serial[16];
	uint8 language_code;
	char mac[MAX_ACCOUNT_MAC_LENGTH + 1];
	uint32 disk_serial;
};
#pragma pack()
struct LOGIN_RESULT
{
	LOGIN_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_ACCOUNT_DATA, SUBCODE_ACCOUNT_DATA_LOGIN, sizeof(LOGIN_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct LOGOUT_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 flag;
};

struct LOGOUT_RESULT
{
	LOGOUT_RESULT(uint8 flag)
	{
		this->h.setE(HEADCODE_ACCOUNT_DATA, SUBCODE_ACCOUNT_DATA_LOGOUT, sizeof(LOGOUT_RESULT));
		this->flag = flag;
	}
	PBMSG_HEAD2 h;
	uint8 flag;
};

/*struct LOGOUT_RESULT_ERROR
{
	LOGOUT_RESULT_ERROR()
	{
		this->h.setE(HEADCODE_ACCOUNT_DATA, SUBCODE_ACCOUNT_DATA_LOGOUT, sizeof(LOGOUT_RESULT_ERROR));
		this->flag = 0xFF;
	}
	PBMSG_HEAD2 h;
	uint8 flag;
};*/

struct PMSG_CLIENTTIME
{
	PBMSG_HEAD h;
	uint32 Time;
	uint16 AttackSpeed;
	uint16 MagicSpeed;
};

//#pragma pack()

struct SERVER_CHANGE_REQUEST
{
	SERVER_CHANGE_REQUEST()
	{
		this->h.setE(HEADCODE_SERVER_MOVE, SUBCODE_SERVER_MOVE_REQUEST, sizeof(SERVER_CHANGE_REQUEST));
		memset(this->ip, 0, MAX_ACCOUNT_IP_LENGTH);
		this->port = -1;
		this->server = -1;
		this->auth[0] = 0;
		this->auth[1] = 0;
		this->auth[2] = 0;
		this->auth[3] = 0;
	}

	PBMSG_HEAD2 h;
	char ip[MAX_ACCOUNT_IP_LENGTH];
	uint16 port;
	uint16 server;
	int32 auth[4];
};
#pragma pack(1)
struct SERVER_CHANGE_AUTH
{
	PBMSG_HEAD2 h;
	char account[12];
	char name[12];
	int32 auth[4];
	int32 tick_count;
	uint8 version[5];
	uint8 serial[16];
};
#pragma pack()
struct SERVER_CHANGE_AUTH_RESULT
{
	SERVER_CHANGE_AUTH_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_SERVER_MOVE, SUBCODE_SERVER_MOVE_RESULT, sizeof(SERVER_CHANGE_AUTH_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

#endif