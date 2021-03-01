#ifndef SERVER_LINK_PACKET_H
#define SERVER_LINK_PACKET_H

enum HeadCodeServerLink
{
	HEADCODE_SERVER_LINK_ON_CONNECT,

	HEADCODE_SERVER_LINK_GUILD_CHAT,
	HEADCODE_SERVER_LINK_ALLIANCE_CHAT,
	HEADCODE_SERVER_LINK_GUILD_NOTICE,
	HEADCODE_SERVER_LINK_POST,
	HEADCODE_SERVER_LINK_WHISPER,
	HEADCODE_SERVER_LINK_SIMPLE_MESSAGE,

	HEADCODE_SERVER_LINK_GUILD_ADD,
	HEADCODE_SERVER_LINK_GUILD_REMOVE,
	HEADCODE_SERVER_LINK_GUILD_MEMBER_ADD,
	HEADCODE_SERVER_LINK_GUILD_MEMBER_DEL,
	HEADCODE_SERVER_LINK_GUILD_MEMBER_STATUS,
	HEADCODE_SERVER_LINK_GUILD_RELATIONSHIP,
	HEADCODE_SERVER_LINK_GUILD_SCORE,

	HEADCODE_SERVER_LINK_NOTICE,

	HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_DATA,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_NPC,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_REGISTERED_GUILD,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_NPC,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_CLEAR_GUILD,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_INSERT_FINAL_GUILD,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_TAX_RATE,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_MONEY,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_OWNER_STATUS,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_INSERT_REGISTERED_GUILD,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_DELETE_REGISTERED_GUILD,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD,
	HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD_ALL,


	HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTER_GUILD,

	HEADCODE_SERVER_LINK_CHARACTER_ON_OFF,

	HEADCODE_SERVER_LINK_CHARACTER_CREATE,
	HEADCODE_SERVER_LINK_CHARACTER_DELETE,

	HEADCODE_SERVER_LINK_ADMIN_COMMAND,
	HEADCODE_SERVER_LINK_ADMIN_COMMAND_LOCATION,

	HEADCODE_SERVER_LINK_SIGNAL,

	HEADCODE_SERVER_LINK_CRYWOLF_REQUEST,
	HEADCODE_SERVER_LINK_CRYWOLF_SAVE,

	HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST_SEARCH,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_CANCEL,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INSERT,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_CANCEL,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_ACCEPT,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_LIST,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INFO,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT_SAVE,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_NOTIFY,
	HEADCODE_SERVER_LINK_GUILD_MATCHING_MASTER_NOTIFY,

	HEADCODE_SERVER_LINK_EVENT_NOTIFICATION,

	HEADCODE_SERVER_LINK_EVENT_STATE_UPDATE,

	HEADCODE_SERVER_LINK_ARKA_WAR_MASTER_REGISTER,
	HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_REGISTER,
	HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REGISTER,
	HEADCODE_SERVER_LINK_ARKA_WAR_ENTER,
	HEADCODE_SERVER_LINK_ARKA_WAR_LIST,
	HEADCODE_SERVER_LINK_ARKA_WAR_CLEAR,
	HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_COUNT,
	HEADCODE_SERVER_LINK_ARKA_WAR_SIGN,
	HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_REQUEST,
	HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_SAVE,
	HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REQUEST,

	HEADCODE_SERVER_LINK_CHARACTER_SIGNAL,

	MAX_SL_HEADCODE
};

#pragma pack(push, 1)

struct SL_CHAT_PACKET
{
	CUSTOM_PACKET_HEAD h;

	SL_CHAT_PACKET(uint8 headcode, uint32 guild, const char * name, const char * msg)
	{
		this->h.set(headcode, sizeof(SL_CHAT_PACKET));
		this->guild = guild;
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
		memset(this->msg, 0, MAX_CHAT_LENGTH + 1);
		strcpy(this->name, name);
		strcpy(this->msg, msg);
	}
	
	uint32 guild;
	char name[MAX_CHARACTER_LENGTH + 1];
	char msg[MAX_CHAT_LENGTH + 1];
};

struct SL_GUILD_ADD
{
	SL_GUILD_ADD()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_ADD, sizeof(SL_GUILD_ADD));
		memset(this->name, 0, MAX_GUILD_NAME_LENGTH + 1);
		this->result = 0;
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint8 result;
	char name[MAX_GUILD_NAME_LENGTH + 1];
	uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
};

struct SL_GUILD_DEL
{
	SL_GUILD_DEL(uint32 guild)
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_REMOVE, sizeof(SL_GUILD_DEL));
		this->guild = guild;
	}
	
	CUSTOM_PACKET_HEAD h;
	uint32 guild;
};

struct SL_GUILD_MEMBER_ADD
{
	SL_GUILD_MEMBER_ADD(uint32 guild)
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MEMBER_ADD, sizeof(SL_GUILD_MEMBER_ADD));
		this->player.Reset();
		this->to_add.Reset();
		this->guild = guild;
		this->result = 0;
		this->slot = 0;
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	CUSTOM_PACKET_PLAYER to_add;
	uint32 guild;
	uint8 result;
	uint8 slot;
};

struct SL_GUILD_MEMBER_DEL
{
	SL_GUILD_MEMBER_DEL()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MEMBER_DEL, sizeof(SL_GUILD_MEMBER_DEL));
		this->guild = 0;
		this->id = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 guild;
	uint32 id;
};

struct SL_GUILD_MEMBER_STATUS
{
	SL_GUILD_MEMBER_STATUS(uint32 guild, const char * name, uint8 status)
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MEMBER_STATUS, sizeof(SL_GUILD_MEMBER_STATUS));
		this->guild = guild;
		this->status = status;
		memcpy(this->name, name, MAX_CHARACTER_LENGTH + 1);
	}

	CUSTOM_PACKET_HEAD h;
	uint32 guild;
	uint8 status;
	char name[MAX_CHARACTER_LENGTH + 1];
};

struct SL_GUILD_RELATIONSHIP
{
	SL_GUILD_RELATIONSHIP(uint32 guild01, uint32 guild02, uint8 type, bool operation)
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_RELATIONSHIP, sizeof(SL_GUILD_RELATIONSHIP));
		this->guild01 = guild01;
		this->guild02 = guild02;
		this->type = type;
		this->operation = operation;
		this->result = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 guild01;
	uint32 guild02;
	uint8 type;
	bool operation;
	uint8 result;
};

struct SL_GUILD_SCORE
{
	SL_GUILD_SCORE(uint32 guild, int32 score)
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_SCORE, sizeof(SL_GUILD_SCORE));
		this->guild = guild;
		this->score = score;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 guild;
	int32 score;
};

struct SL_CHARACTER_ON_OFF
{
	CUSTOM_PACKET_HEAD h;

	SL_CHARACTER_ON_OFF(uint32 guild, uint32 guid, uint16 server, const char * name, uint8 status)
	{
		this->h.set(HEADCODE_SERVER_LINK_CHARACTER_ON_OFF, sizeof(SL_CHARACTER_ON_OFF));
		this->guid = guid;
		this->account_id = 0;
		this->guild = guild;
		this->server = server;
		memcpy(this->name, name, MAX_CHARACTER_LENGTH + 1);
		this->status = status;
		this->authority = 0;
		this->admin_flags = 0;
	}

	uint32 guild;
	uint32 guid;
	uint32 account_id;
	uint16 server;
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 status;
	uint8 authority;
	uint32 admin_flags;
};

struct SL_NOTICE
{
	SL_NOTICE(const char * notice_buffer, uint8 type)
	{
		this->h.set(HEADCODE_SERVER_LINK_NOTICE, sizeof(SL_NOTICE));
		this->type = type;
		memset(this->notice, 0, 256);
		strcpy(this->notice, notice_buffer);
	}

	CUSTOM_PACKET_HEAD h;
	uint8 type;
	char notice[256];
};

struct SL_CASTLE_SIEGE_REGISTER_GUILD
{
	SL_CASTLE_SIEGE_REGISTER_GUILD(uint32 guild_, bool status_, uint32 mark_)
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTER_GUILD, sizeof(SL_CASTLE_SIEGE_REGISTER_GUILD));
		this->guild = guild_;
		this->status = status_;
		this->mark = mark_;
	}
	CUSTOM_PACKET_HEAD h;
	uint32 guild;
	bool status;
	uint32 mark;
};

struct SL_CHARACTER_CREATE_REQUEST
{
	CUSTOM_PACKET_HEAD h;

	SL_CHARACTER_CREATE_REQUEST()
	{
		this->h.set(HEADCODE_SERVER_LINK_CHARACTER_CREATE, sizeof(SL_CHARACTER_CREATE_REQUEST));
		this->entry = -1;
		this->ms_time = 0;
		this->account_id = 0;
		this->character_class = 0;
		memset(this->character_name, 0, MAX_CHARACTER_LENGTH + 1);
		this->strength = 0;
		this->agility = 0;
		this->vitality = 0;
		this->energy = 0;
		this->leadership = 0;
		this->life = 0;
		this->mana = 0;
		this->points = 0;
		this->level = 0;
		this->world = 0;
		this->x = 0;
		this->y = 0;
		this->slot = -1;
	}

	uint16 entry;
	uint32 ms_time;
	uint32 account_id;
	uint8 character_class;
	char character_name[MAX_CHARACTER_LENGTH + 1];
	uint32 strength;
	uint32 agility;
	uint32 vitality;
	uint32 energy;
	uint32 leadership;
	int32 life;
	int32 mana;
	uint32 points;
	int16 level;
	world_type world;
	coord_type x;
	coord_type y;
	uint8 slot;
};

struct SL_CHARACTER_CREATE_RESULT
{
	CUSTOM_PACKET_HEAD h;

	SL_CHARACTER_CREATE_RESULT()
	{
		this->h.set(HEADCODE_SERVER_LINK_CHARACTER_CREATE, sizeof(SL_CHARACTER_CREATE_RESULT));
		this->result = 0;
		this->entry = -1;
		this->ms_time = 0;
		this->character_class = 0;
		memset(this->character_name, 0, MAX_CHARACTER_LENGTH + 1);
		this->slot = -1;
		this->level = 0;
	}

	uint8 result;
	uint16 entry;
	uint32 ms_time;
	uint8 character_class;
	char character_name[MAX_CHARACTER_LENGTH + 1];
	uint8 slot;
	int16 level;
};

struct SL_CHARACTER_DELETE_REQUEST
{
	CUSTOM_PACKET_HEAD h;

	SL_CHARACTER_DELETE_REQUEST()
	{
		this->h.set(HEADCODE_SERVER_LINK_CHARACTER_DELETE, sizeof(SL_CHARACTER_DELETE_REQUEST));
		this->entry = -1;
		this->ms_time = 0;
		this->account_id = 0;
		memset(this->character_name, 0, MAX_CHARACTER_LENGTH + 1);
	}
	
	uint16 entry;
	uint32 ms_time;
	uint32 account_id;
	char character_name[MAX_CHARACTER_LENGTH + 1];
};

struct SL_CHARACTER_DELETE_RESULT
{
	CUSTOM_PACKET_HEAD h;

	SL_CHARACTER_DELETE_RESULT()
	{
		this->h.set(HEADCODE_SERVER_LINK_CHARACTER_DELETE, sizeof(SL_CHARACTER_DELETE_RESULT));
		this->result = 0;
		this->entry = -1;
		this->ms_time = 0;
		memset(this->character_name, 0, MAX_CHARACTER_LENGTH + 1);
	}
	
	uint8 result;
	uint16 entry;
	uint32 ms_time;
	char character_name[MAX_CHARACTER_LENGTH + 1];
};

/*******************************************
			CASTLE SIEGE DATA
*******************************************/

struct SL_CASTLE_SIEGE_LOAD_DATA_REQUEST
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_LOAD_DATA_REQUEST()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_DATA, sizeof(SL_CASTLE_SIEGE_LOAD_DATA_REQUEST));
	}
};

struct SL_CASTLE_SIEGE_LOAD_DATA_RESULT
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_LOAD_DATA_RESULT()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_DATA, sizeof(SL_CASTLE_SIEGE_LOAD_DATA_RESULT));
	}

	uint32 owner;
	uint8 status;
	int32 tax_hunt;
	uint8 tax_chaos;
	uint8 tax_store;
	bool hunt_allowed;
	int64 money;
};

/*******************************************
			CASTLE SIEGE NPC 
*******************************************/

struct SL_CASTLE_SIEGE_LOAD_NPC_REQUEST
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_LOAD_NPC_REQUEST()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_NPC, sizeof(SL_CASTLE_SIEGE_LOAD_NPC_REQUEST));
	}
};

struct SL_CASTLE_SIEGE_LOAD_NPC_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint8 count;
};

struct SL_CASTLE_SIEGE_LOAD_NPC_BODY
{
	uint16 npc;
	uint8 id;
	uint8 defense_level;
	uint8 regen_level;
	uint8 life_level;
	int32 life;
};

struct SL_CASTLE_SIEGE_SAVE_NPC_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint8 count;
};

struct SL_CASTLE_SIEGE_SAVE_NPC_BODY
{
	uint16 npc;
	uint8 id;
	uint8 defense_level;
	uint8 regen_level;
	uint8 life_level;
	int32 life;
};

/*******************************************
		CASTLE SIEGE REGISTERED GUILD 
*******************************************/

struct SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_REGISTERED_GUILD, sizeof(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD));
	}
};

struct SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint8 count;
};

struct SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_BODY
{
	uint32 guid;
	int32 marks;
	int32 register_id;
	int32 level;
	uint8 members;
	int64 total_score;
	int32 score;
	uint8 side;
};

/*******************************************
		CASTLE SIEGE CLEAR GUILD 
*******************************************/

struct SL_CASTLE_SIEGE_CLEAR_GUILD
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_CLEAR_GUILD()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_CLEAR_GUILD, sizeof(SL_CASTLE_SIEGE_CLEAR_GUILD));
	}
};

/*******************************************
	CASTLE SIEGE INSERT FINAL GUILD
*******************************************/
struct SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint8 count;
};

struct SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_BODY
{
	uint32 guild;
	uint8 side;
	int32 score;
};

/*******************************************
		CASTLE SIEGE SAVE TAX RATE
*******************************************/
struct SL_CASTLE_SIEGE_SAVE_TAX_RATE
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_SAVE_TAX_RATE()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_TAX_RATE, sizeof(SL_CASTLE_SIEGE_SAVE_TAX_RATE));
	}

	int32 tax_hunt;
	uint8 tax_chaos;
	uint8 tax_store;
	bool hunt_allowed;
};

struct SL_CASTLE_SIEGE_SAVE_MONEY
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_SAVE_MONEY(int32 money, uint8 type)
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_MONEY, sizeof(SL_CASTLE_SIEGE_SAVE_MONEY));
		this->money = money;
		this->type = type;
		this->entry = -1;
		this->ms_time = 0;
		this->account_id = 0;
		this->character_id = 0;
	}

	int32 money;
	uint8 type;
	uint16 entry;
	uint32 ms_time;
	uint32 account_id;
	uint32 character_id;
};

struct SL_CASTLE_SIEGE_SAVE_OWNER_STATUS
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_SAVE_OWNER_STATUS()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_OWNER_STATUS, sizeof(SL_CASTLE_SIEGE_SAVE_OWNER_STATUS));
	}

	uint32 owner;
	uint8 status;
};

/*******************************************
	CASTLE SIEGE INSERT REGISTERED GUILD
*******************************************/
struct SL_CASTLE_SIEGE_INSERT_REGISTERED_GUILD
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_INSERT_REGISTERED_GUILD()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_INSERT_REGISTERED_GUILD, sizeof(SL_CASTLE_SIEGE_INSERT_REGISTERED_GUILD));
	}

	uint32 guild;
};

struct SL_CASTLE_SIEGE_DELETE_REGISTERED_GUILD
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_DELETE_REGISTERED_GUILD()
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_DELETE_REGISTERED_GUILD, sizeof(SL_CASTLE_SIEGE_DELETE_REGISTERED_GUILD));
	}

	uint32 guild;
};

struct SL_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD(uint32 guild, uint32 marks)
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD, sizeof(SL_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD));
		this->guild = guild;
		this->marks = marks;
	}

	uint32 guild;
	uint32 marks;
};

/*******************************************
	CASTLE SIEGE REGISTERED GUILD
*******************************************/
struct SL_CASTLE_SIEGE_REGISTERED_GUILD_REQUEST
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_REGISTERED_GUILD_REQUEST(uint32 guild, uint16 entry)
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_REQUEST));
		this->guild = guild;
		this->entry = entry;
	}

	uint32 guild;
	uint16 entry;
};

struct SL_CASTLE_SIEGE_REGISTERED_GUILD_ALL_REQUEST
{
	CUSTOM_PACKET_HEAD h;

	SL_CASTLE_SIEGE_REGISTERED_GUILD_ALL_REQUEST(uint16 entry)
	{
		this->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD_ALL, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_ALL_REQUEST));
		this->entry = entry;
	}

	uint16 entry;
};

struct SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint16 entry;
	uint8 count;
};

struct SL_CASTLE_SIEGE_REGISTERED_GUILD_BODY
{
	uint32 guild;
	uint32 marks;
	uint32 register_id;
};

struct SL_ADMIN_COMMAND
{
	SL_ADMIN_COMMAND()
	{
		this->h.set(HEADCODE_SERVER_LINK_ADMIN_COMMAND, sizeof(SL_ADMIN_COMMAND));
		this->type = 0;
		this->sub_type = 0;
		this->entry = -1;
		this->guid = 0;
		memset(this->target_name, 0, MAX_CHARACTER_LENGTH + 1);
		memset(this->target_account, 0, MAX_ACCOUNT_LENGTH + 1);
		this->time = 0;
		this->result = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 type;
	uint16 sub_type;
	uint16 entry;
	uint32 guid;
	char target_name[MAX_CHARACTER_LENGTH + 1];
	char target_account[MAX_ACCOUNT_LENGTH + 1];
	time_t time;
	uint8 result;
};

struct SL_ADMIN_COMMAND_LOCATION
{
	SL_ADMIN_COMMAND_LOCATION()
	{
		this->h.set(HEADCODE_SERVER_LINK_ADMIN_COMMAND_LOCATION, sizeof(SL_ADMIN_COMMAND_LOCATION));
		this->type = 0;
		this->entry = -1;
		this->guid = 0;
		memset(this->target_name, 0, MAX_CHARACTER_LENGTH + 1);
		this->world = -1;
		this->x = 0;
		this->y = 0;
		this->direction = 0;
		this->server = -1;
		this->online = false;
		this->result = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 type; // 0 = trans, 1 = move
	uint16 entry;
	uint32 guid;
	char target_name[MAX_CHARACTER_LENGTH + 1];
	world_type world;
	coord_type x;
	coord_type y;
	uint8 direction;
	uint16 server;
	bool online;
	uint8 result;
};

struct SL_WHISPER
{
	SL_WHISPER()
	{
		this->h.set(HEADCODE_SERVER_LINK_WHISPER, sizeof(SL_WHISPER));
		this->result = 0;
		this->entry = -1;
		this->guid = 0;
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
		memset(this->target_name, 0, MAX_CHARACTER_LENGTH + 1);
		memset(this->message, 0, MAX_CHAT_LENGTH + 1);
	}

	CUSTOM_PACKET_HEAD h;
	uint8 result;
	uint16 entry;
	uint32 guid;
	char name[MAX_CHARACTER_LENGTH + 1];
	char target_name[MAX_CHARACTER_LENGTH + 1];
	char message[MAX_CHAT_LENGTH + 1];
};

struct SL_SIGNAL
{
	SL_SIGNAL()
	{
		this->h.set(HEADCODE_SERVER_LINK_SIGNAL, sizeof(SL_SIGNAL));
		this->signal = -1;
		this->data[0] = 0;
		this->data[1] = 0;
		this->data[2] = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint16 signal;
	int32 data[3];
};

struct SL_CRYWOLF_REQUEST
{
	SL_CRYWOLF_REQUEST()
	{
		this->h.set(HEADCODE_SERVER_LINK_CRYWOLF_REQUEST, sizeof(SL_CRYWOLF_REQUEST));
		this->status = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 status;
};

struct SL_CRYWOLF_SAVE
{
	SL_CRYWOLF_SAVE(uint8 status)
	{
		this->h.set(HEADCODE_SERVER_LINK_CRYWOLF_SAVE, sizeof(SL_CRYWOLF_SAVE));
		this->status = status;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 status;
};

struct SL_ON_CONNECT
{
	SL_ON_CONNECT(uint16 server)
	{
		this->h.set(HEADCODE_SERVER_LINK_ON_CONNECT, sizeof(SL_ON_CONNECT));
		this->h.server = server;
	}

	CUSTOM_PACKET_HEAD h;
};



/*****************************************************
				GUILD MATCHING
******************************************************/

struct SL_GUILD_MATCHING_LIST_RECV
{
	SL_GUILD_MATCHING_LIST_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST, sizeof(SL_GUILD_MATCHING_LIST_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 page;
};

struct SL_GUILD_MATCHING_LIST_SEARCH_RECV
{
	SL_GUILD_MATCHING_LIST_SEARCH_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST_SEARCH, sizeof(SL_GUILD_MATCHING_LIST_SEARCH_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 page;
	char SearchWord[11];
};

struct SL_GUILD_MATCHING_INSERT_RECV
{
	SL_GUILD_MATCHING_INSERT_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT, sizeof(SL_GUILD_MATCHING_INSERT_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	char text[GUILD_MATCHING_TEXT_LENGTH + 1];
	uint32 GuildID;
	uint16 GuildMasterLevel;
	uint8 GuildMasterClass;
	uint8 InterestType;
	uint8 LevelRange;
	uint16 ClassType;
	uint8 GensType;
};

struct SL_GUILD_MATCHING_CANCEL_RECV
{
	SL_GUILD_MATCHING_CANCEL_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_CANCEL, sizeof(SL_GUILD_MATCHING_CANCEL_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 GuildID;
};

struct SL_GUILD_MATCHING_JOIN_INSERT_RECV
{
	SL_GUILD_MATCHING_JOIN_INSERT_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INSERT, sizeof(SL_GUILD_MATCHING_JOIN_INSERT_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 GuildID;
	uint8 Class;
	uint16 Level;
};

struct SL_GUILD_MATCHING_JOIN_CANCEL_RECV
{
	SL_GUILD_MATCHING_JOIN_CANCEL_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_CANCEL, sizeof(SL_GUILD_MATCHING_JOIN_CANCEL_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint8 flag;
};

struct SL_GUILD_MATCHING_JOIN_ACCEPT_RECV
{
	SL_GUILD_MATCHING_JOIN_ACCEPT_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_ACCEPT, sizeof(SL_GUILD_MATCHING_JOIN_ACCEPT_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint8 type;
	uint32 GuildID;
	char MemberName[MAX_CHARACTER_LENGTH + 1];
};

struct SL_GUILD_MATCHING_JOIN_LIST_RECV
{
	SL_GUILD_MATCHING_JOIN_LIST_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_LIST, sizeof(SL_GUILD_MATCHING_JOIN_LIST_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 GuildID;
};

struct SL_GUILD_MATCHING_JOIN_INFO_RECV
{
	SL_GUILD_MATCHING_JOIN_INFO_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INFO, sizeof(SL_GUILD_MATCHING_JOIN_INFO_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
};

struct SL_GUILD_MATCHING_INSERT_SAVE_RECV
{
	SL_GUILD_MATCHING_INSERT_SAVE_RECV()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT_SAVE, sizeof(SL_GUILD_MATCHING_INSERT_SAVE_RECV));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 GuildID;
	uint16 GuildMasterLevel;
	uint8 GuildMasterClass;
	uint8 GensType;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SL_GUILD_MATCHING_LIST_HEAD
{
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
	uint32 CurPage;
	uint32 MaxPage;
	uint32 count;
};

struct SL_GUILD_MATCHING_LIST_BODY
{
	char text[GUILD_MATCHING_TEXT_LENGTH + 1];
	char name[MAX_CHARACTER_LENGTH + 1];
	char GuildName[MAX_GUILD_NAME_LENGTH + 1];
	uint8 GuildMemberCount;
	uint8 GuildMasterClass;
	uint8 InterestType;
	uint8 LevelRange;
	uint16 ClassType;
	uint32 GuildMasterLevel;
	uint32 BoardNumber;
	uint32 GuildNumber;
	uint8 GensType;
};

struct SL_GUILD_MATCHING_INSERT_SEND
{
	SL_GUILD_MATCHING_INSERT_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT, sizeof(SL_GUILD_MATCHING_INSERT_SEND));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
};

struct SL_GUILD_MATCHING_CANCEL_SEND
{
	SL_GUILD_MATCHING_CANCEL_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_CANCEL, sizeof(SL_GUILD_MATCHING_CANCEL_SEND));
	}
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
};

struct SL_GUILD_MATCHING_JOIN_INSERT_SEND
{
	SL_GUILD_MATCHING_JOIN_INSERT_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INSERT, sizeof(SL_GUILD_MATCHING_JOIN_INSERT_SEND));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
};

struct SL_GUILD_MATCHING_JOIN_CANCEL_SEND
{
	SL_GUILD_MATCHING_JOIN_CANCEL_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_CANCEL, sizeof(SL_GUILD_MATCHING_JOIN_CANCEL_SEND));
	}
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
	uint8 flag;
};

struct SL_GUILD_MATCHING_JOIN_ACCEPT_SEND
{
	SL_GUILD_MATCHING_JOIN_ACCEPT_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_ACCEPT, sizeof(SL_GUILD_MATCHING_JOIN_ACCEPT_SEND));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
	uint32 type;
	uint32 GuildID;
	char MemberName[MAX_CHARACTER_LENGTH + 1];
};

struct SL_GUILD_MATCHING_JOIN_LIST_HEAD
{
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
	uint32 count;
};

struct SL_GUILD_MATCHING_JOIN_LIST_BODY
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 Class;
	uint32 Level;
};

struct SL_GUILD_MATCHING_JOIN_INFO_SEND
{
	SL_GUILD_MATCHING_JOIN_INFO_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INFO, sizeof(SL_GUILD_MATCHING_JOIN_INFO_SEND));
	}

	CUSTOM_PACKET_HEAD h; // C1:28:08
	CUSTOM_PACKET_PLAYER player;
	uint32 result;
	char GuildName[MAX_GUILD_NAME_LENGTH + 1];
	char GuildMasterName[MAX_CHARACTER_LENGTH + 1];
};

struct SL_GUILD_MATCHING_NOTIFY_SEND
{
	SL_GUILD_MATCHING_NOTIFY_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_NOTIFY, sizeof(SL_GUILD_MATCHING_NOTIFY_SEND));
	}

	CUSTOM_PACKET_HEAD h;
	uint32 id;
	uint32 result;
	uint32 GuildID;
};

struct SL_GUILD_MATCHING_NOTIFY_MASTER_SEND
{
	SL_GUILD_MATCHING_NOTIFY_MASTER_SEND()
	{
		this->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_MASTER_NOTIFY, sizeof(SL_GUILD_MATCHING_NOTIFY_MASTER_SEND));
	}

	CUSTOM_PACKET_HEAD h;
	uint32 id;
	uint32 result;
};

struct SL_EVENT_NOTIFICATION
{
	SL_EVENT_NOTIFICATION(uint8 event_id, uint8 open)
	{
		this->h.set(HEADCODE_SERVER_LINK_EVENT_NOTIFICATION, sizeof(SL_EVENT_NOTIFICATION));
		this->event_id = event_id;
		this->open = open;
	}
	CUSTOM_PACKET_HEAD h;
	uint8 event_id;
	uint8 open;
};

struct SL_EVENT_STATE_UPDATE
{
	SL_EVENT_STATE_UPDATE(uint8 event_id, uint8 state, uint8 occupation_state)
	{
		this->h.set(HEADCODE_SERVER_LINK_EVENT_STATE_UPDATE, sizeof(SL_EVENT_STATE_UPDATE));
		this->event_id = event_id;
		this->state = state;
		this->occupation_state = occupation_state;
	}
	CUSTOM_PACKET_HEAD h;
	uint8 event_id;
	uint8 state;
	uint8 occupation_state;
};

struct SL_ARKA_WAR_MASTER_REGISTER
{
	SL_ARKA_WAR_MASTER_REGISTER()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_MASTER_REGISTER, sizeof(SL_ARKA_WAR_MASTER_REGISTER));
		this->guild = 0;
		this->result = 0;
	}
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 guild;
	uint8 result;
};

struct SL_ARKA_WAR_MEMBER_REGISTER
{
	SL_ARKA_WAR_MEMBER_REGISTER()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_REGISTER, sizeof(SL_ARKA_WAR_MEMBER_REGISTER));
		this->guild = 0;
		this->max_member = 0;
		this->result = 0;
	}
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 guild;
	int32 max_member;
	uint8 result;
};

struct SL_ARKA_WAR_SIGN_REGISTER
{
	SL_ARKA_WAR_SIGN_REGISTER()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REGISTER, sizeof(SL_ARKA_WAR_SIGN_REGISTER));
		this->guild = 0;
		this->result = 0;
		this->signs = 0;
	}
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 guild;
	uint8 result;
	uint32 signs;
};

struct SL_ARKA_WAR_ENTER
{
	SL_ARKA_WAR_ENTER()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_ENTER, sizeof(SL_ARKA_WAR_ENTER));
		this->result = 0;
	}
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint8 result;
};

struct SL_ARKA_WAR_LIST
{
	SL_ARKA_WAR_LIST()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_LIST, sizeof(SL_ARKA_WAR_LIST));
		this->max_guild = 0;
		this->min_member = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 max_guild;
	int32 min_member;
};

struct SL_ARKA_WAR_LIST_GUILD_BODY
{
	uint32 guild;
	int32 members;
};

struct SL_ARKA_WAR_LIST_MEMBER_BODY
{
	uint32 id;
	char name[MAX_CHARACTER_LENGTH + 1];
	uint32 guild;
};

struct SL_ARKA_WAR_LIST_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint8 type;
	uint16 count;
};

struct SL_ARKA_WAR_CLEAR
{
	SL_ARKA_WAR_CLEAR()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_CLEAR, sizeof(SL_ARKA_WAR_CLEAR));
	}
	CUSTOM_PACKET_HEAD h;
};

struct SL_ARKA_WAR_MEMBER_COUNT
{
	SL_ARKA_WAR_MEMBER_COUNT()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_COUNT, sizeof(SL_ARKA_WAR_MEMBER_COUNT));
		this->guild = 0;
		this->result = 0;
		this->count = 0;
	}
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint32 guild;
	uint8 result;
	int32 count;
};

//HEADCODE_SERVER_LINK_ARKA_WAR_SIGN

struct SL_ARKA_WAR_GUILD_REQUEST
{
	SL_ARKA_WAR_GUILD_REQUEST()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_REQUEST, sizeof(SL_ARKA_WAR_GUILD_REQUEST));
	}

	CUSTOM_PACKET_HEAD h;
};

struct SL_ARKA_WAR_GUILD_BODY
{
	uint8 id;
	uint32 guild;
	uint8 attribute;
};

struct SL_ARKA_WAR_GUILD_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint8 count;
};

struct SL_ARKA_WAR_SIGN_REQUEST
{
	SL_ARKA_WAR_SIGN_REQUEST()
	{
		this->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REQUEST, sizeof(SL_ARKA_WAR_SIGN_REQUEST));
	}

	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
};

struct SL_ARKA_WAR_SIGN_REQUEST_BODY
{
	uint32 guild;
	int32 signs;
};

struct SL_ARKA_WAR_SIGN_REQUEST_HEAD
{
	CUSTOM_PACKET_HEAD h;
	CUSTOM_PACKET_PLAYER player;
	uint8 count;
};

struct SL_CHARACTER_SIGNAL
{
	SL_CHARACTER_SIGNAL()
	{
		this->h.set(HEADCODE_SERVER_LINK_CHARACTER_SIGNAL, sizeof(SL_CHARACTER_SIGNAL));
	}

	CUSTOM_PACKET_HEAD h;
	uint32 id;
	uint8 type;
};

#pragma pack(pop)

#endif