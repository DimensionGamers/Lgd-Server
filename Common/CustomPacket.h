#ifndef CUSTOM_PACKET_H
#define CUSTOM_PACKET_H

struct SERVER_INFO_RESULT
{
	SERVER_INFO_RESULT(const char * ip, uint16 port)
	{
		this->h.set(HEADCODE_CONNECT_SERVER, SUBCODE_CONNECT_SERVER_SERVER_SELECT, sizeof(SERVER_INFO_RESULT));
		memset(this->ip, 0, 16);
		memcpy(this->ip, ip, strlen(ip));
		this->port = port;
	}

	PBMSG_HEAD2 h;
	char ip[16];
	uint16 port;
};

#pragma pack(1)
struct MU_VERSION_CHECK
{
	PBMSG_HEAD h;
	uint8 result;
};

struct MU_VERSION_CHECK_RESULT
{
	MU_VERSION_CHECK_RESULT()
	{
		this->h.set(0x0A, sizeof(MU_VERSION_CHECK_RESULT));
		memset(this->address, 0, 200);
		memset(this->data, 0, 2);
	}

	PBMSG_HEAD h;
	char address[200];
	uint8 data[2];
};

struct MU_LAUNCHER_ERROR
{
	PWMSG_HEAD h;
	uint8 data[2];
	char path_error[200];
};
#pragma pack()

#pragma pack(push, 1)
struct CASH_SHOP_DATA_UPDATE
{
	CASH_SHOP_DATA_UPDATE()
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_CASH_SHOP_DATE, sizeof(CASH_SHOP_DATA_UPDATE));
		memset(this->item_ip, 0, 16);
		memset(this->item_dir, 0, 32);
		memset(this->item_dir_test, 0, 32);

		memset(this->banner_ip, 0, 16);
		memset(this->banner_dir, 0, 32);
		memset(this->banner_dir_test, 0, 32);
	}
	PBMSG_HEAD2 h;
	char item_ip[16];
	char item_dir[32];
	char item_dir_test[32];

	char banner_ip[16];
	char banner_dir[32];
	char banner_dir_test[32];
};

struct ACCELERATOR_UDATE
{
	ACCELERATOR_UDATE(uint8 result)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_ACCELERATOR, sizeof(ACCELERATOR_UDATE));
		this->result = result;
		this->action = 0;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 action;
};

struct SHOP_ITEM_PRICE_HEAD
{
	PWMSG_HEAD2 h;
	bool close;
	uint8 count;
	int64 global_buy_price;
	int64 global_sell_price;
};

struct SHOP_ITEM_PRICE_BODY
{
	uint16 item;
	uint8 level;
	int64 buy_price;
	int64 sell_price;
};

struct CUSTOM_MESSAGE
{
	CUSTOM_MESSAGE(uint8 type, const char * message)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_CUSTOM_MESSAGE, sizeof(CUSTOM_MESSAGE));
		this->type = type;
		memcpy(this->message, message, 100);
	}

	PBMSG_HEAD2 h;
	uint8 type;
	char message[100];
};

struct PLAYER_CONFIG_FLAG_UPDATE
{
	PLAYER_CONFIG_FLAG_UPDATE(uint32 flags)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_CONFIG_FLAGS, sizeof(PLAYER_CONFIG_FLAG_UPDATE));
		this->flags = flags;
	}

	PBMSG_HEAD2 h;
	uint32 flags;
};

struct CUSTOM_PLAY_SOUND
{
	CUSTOM_PLAY_SOUND(int32 id, uint8 data1, uint8 data2)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_PLAY_SOUND, sizeof(CUSTOM_PLAY_SOUND));
		this->id = id;
		this->data1 = data1;
		this->data2 = data2;
	}

	PBMSG_HEAD2 h;
	int32 id;
	uint8 data1;
	uint8 data2;
};

struct GEN_BATTLE_LIST
{
	PBMSG_HEAD2 h;
	bool battlezone;
	uint8 count;
};

struct EXPANDED_WAREHOUSE_STATUS
{
	EXPANDED_WAREHOUSE_STATUS(uint8 status)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_EXPANDED_WAREHOUSE, sizeof(EXPANDED_WAREHOUSE_STATUS));
		this->status = status;
	}

	PBMSG_HEAD2 h;
	uint8 status;
};

struct CLIENT_OFFSET_BODY
{
	uint32 offset;
	uint8 data;
	uint8 loop;
};

struct CLIENT_OFFSET_HEAD
{
	PBMSG_HEAD2 h;
	uint8 count;
};

struct HACK_REPORT
{
	HACK_REPORT(uint8 type)
	{
		this->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_HACK_REPORT, sizeof(HACK_REPORT));
		this->type = type;
	}

	PBMSG_HEAD2 h;
	uint8 type;
};

struct CLIENT_MINIMIZE
{
	PBMSG_HEAD2 h;
	uint8 minimize;
};

struct PACKET_ANTI_MACRO : C1_HEADER_SUB
{
	PACKET_ANTI_MACRO()
	{
		this->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_ANTI_MACRO, sizeof(PACKET_ANTI_MACRO));
	}

	uint8 enabled;
	uint32 time;
	uint8 combo_enabled;
	uint32 combo_time;
};

struct PACKET_ANTI_MACRO_DETECT : C1_HEADER_SUB
{
	uint8 type;
};
#pragma pack(pop)

#endif