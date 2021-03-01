#ifndef CONNECT_SERVER_PACKET_H
#define CONNECT_SERVER_PACKET_H

enum ServerFlags
{
	SERVER_FLAG_NONE,
	SERVER_FLAG_DISPLAY = 1 << 0,
	SERVER_FLAG_ACCOUNT_ALLOWED = 1 << 1,
	SERVER_FLAG_DISABLED = 1 << 2,
};

enum
{
	HEADCODE_CONNECT_SERVER_CONNECT,
	HEADCODE_CONNECT_SERVER_USER_PERCENT,
	HEADCODE_CONNECT_SERVER_FLAG,
	HEADCODE_CONNECT_SERVER_CHANNELS,
};

#pragma pack(1)
struct CS_GAMESERVER_CONNECT
{
	CS_GAMESERVER_CONNECT(uint16 server)
	{
		this->h.set(HEADCODE_CONNECT_SERVER_CONNECT, sizeof(CS_GAMESERVER_CONNECT));
		this->h.server = server;
	}

	CUSTOM_PACKET_HEAD h;
};

struct CS_GAMESERVER_USER_PERCENT
{
	CS_GAMESERVER_USER_PERCENT(uint8 percent)
	{
		this->h.set(HEADCODE_CONNECT_SERVER_USER_PERCENT, sizeof(CS_GAMESERVER_USER_PERCENT));
		this->percent = percent;
		this->active = false;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 percent;
	bool active;
};

struct CS_GAMESERVER_FLAG
{
	CS_GAMESERVER_FLAG(uint8 flag)
	{
		this->h.set(HEADCODE_CONNECT_SERVER_FLAG, sizeof(CS_GAMESERVER_FLAG));
		this->flag = flag;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 flag;
};

struct CS_CHANNEL_BODY
{
	uint16 server;
	uint16 data1;
	uint16 data2;
	uint8 type;
	uint8 gold;
};

struct CS_CHANNEL_HEAD
{
	CUSTOM_PACKET_HEAD h;
	uint8 count;
};
#pragma pack()

#endif