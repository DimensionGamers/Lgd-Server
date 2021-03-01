#ifndef LOGIN_SERVER_PACKET_H
#define LOGIN_SERVER_PACKET_H

#pragma pack(push, 1)

template <class T>
struct LoginHandler
{
    const char * name;
    uint16 packetSize;
    void (T::*handler)(uint8 * Packet);
};

#pragma pack(pop)

enum
{
	HEADCODE_LOGIN_SERVER_CONNECT,
	HEADCODE_LOGIN_SERVER_PERCENT,
	HEADCODE_LOGIN_SERVER_ACCOUNT_RECONNECT,
	HEADCODE_LOGIN_SERVER_ACCOUNT_LOGIN,
	HEADCODE_LOGIN_SERVER_ACCOUNT_LOGOUT,
	HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_MOVE,
	HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_AUTH,
	HEADCODE_LOGIN_SERVER_ACCOUNT_KICK,
	HEADCODE_LOGIN_SERVER_ACCOUNT_ONLINE,

	HEADCODE_LOGIN_SERVER_COMPLETE_BAN,
	HEADCODE_LOGIN_SERVER_ACCOUNT_VALIDATE,

	HEADCODE_LOGIN_SERVER_ACCOUNT_TYPE,

	HEADCODE_LOGIN_SERVER_ACCOUNT_DUPLICATE,
};

#pragma pack(1)
struct GAMESERVER_CONNECT
{
	GAMESERVER_CONNECT(uint16 server)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_CONNECT, sizeof(GAMESERVER_CONNECT));
		this->h.server = server;
		this->port = -1;
		memset(this->ip, 0, 16);
		this->flag = 0;
		this->type = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint16 port;
	char ip[16];
	uint8 flag;
	uint8 type;
};

struct GAMESERVER_USER_PERCENT
{
	GAMESERVER_USER_PERCENT(uint8 percent, uint8 flag, uint8 type)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_PERCENT, sizeof(GAMESERVER_USER_PERCENT));
		this->percent = percent;
		this->flag = flag;
		this->type = type;
	}

	CUSTOM_PACKET_HEAD h;
	uint8 percent;
	uint8 flag;
	uint8 type;
};

struct ACCOUNT_LOGIN_REQUEST
{
	ACCOUNT_LOGIN_REQUEST(uint16 entry)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_LOGIN, sizeof(ACCOUNT_LOGIN_REQUEST));
		this->entry = entry;
		this->ms_time = 0;
		memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
		memset(this->password, 0, MAX_PASSWORD_LENGTH + 1);
		memset(this->ip, 0, MAX_ACCOUNT_IP_LENGTH + 1);
		memset(this->mac, 0, MAX_ACCOUNT_MAC_LENGTH + 1);
		this->disk_serial = 0;
		this->account_id = 0;
		this->autologin = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint16 entry;
	uint32 ms_time;
	char account[MAX_ACCOUNT_LENGTH + 1];
	char password[MAX_PASSWORD_LENGTH + 1];
	char ip[MAX_ACCOUNT_IP_LENGTH + 1];
	char mac[MAX_ACCOUNT_MAC_LENGTH + 1];
	uint32 disk_serial;
	uint32 account_id;
	uint8 autologin;
};

struct ACCOUNT_LOGIN_RESULT
{
	ACCOUNT_LOGIN_RESULT(uint8 result, uint16 entry)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_LOGIN, sizeof(ACCOUNT_LOGIN_RESULT));
		this->result = result;
		this->entry = entry;
		this->ms_time = 0;
		this->account_id = 0;
		memset(this->security_code, 0, MAX_SECURE_CODE_LENGTH + 1);
		this->facebook_status = 0;
		this->golden_channel = 0;
		this->authorized = false;
		memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
	}

	CUSTOM_PACKET_HEAD h;
	uint8 result;
	uint16 entry;
	uint32 ms_time;

	uint32 account_id;
	char security_code[MAX_SECURE_CODE_LENGTH + 1];
	uint8 facebook_status;
	time_t golden_channel;
	bool authorized;
	char account[MAX_ACCOUNT_LENGTH + 1];
};

struct ACCOUNT_LOGOUT
{
	ACCOUNT_LOGOUT(uint32 account_id)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_LOGOUT, sizeof(ACCOUNT_LOGOUT));
		this->account_id = account_id;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 account_id;
};

struct ACCOUNT_SERVER_MOVE_DATA
{
	char account[MAX_ACCOUNT_LENGTH + 1];
	uint32 account_id;
	uint16 start_server;
	uint16 cur_server;
	uint16 dest_server;
	world_type dest_world;
	coord_type dest_x;
	coord_type dest_y;
};

struct ACCOUNT_SERVER_MOVE_REQUEST
{
	ACCOUNT_SERVER_MOVE_REQUEST()
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_MOVE, sizeof(ACCOUNT_SERVER_MOVE_REQUEST));
	}

	CUSTOM_PACKET_HEAD h;
	ACCOUNT_SERVER_MOVE_DATA data;
	uint8 channel_change;
};

/*
result -> 0 = Success
result -> 1 = Error
result -> 2 = Dest Offline
result -> 3 = Wrong User Data
result -> 4 = User is already moving
*/

struct ACCOUNT_SERVER_MOVE_RESULT
{
	ACCOUNT_SERVER_MOVE_RESULT()
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_MOVE, sizeof(ACCOUNT_SERVER_MOVE_RESULT));
		this->result = 0;
		this->auth[0] = 0;
		this->auth[1] = 0;
		this->auth[2] = 0;
		this->auth[3] = 0;
		this->dest_port = -1;
		memset(this->dest_ip, 0, MAX_ACCOUNT_IP_LENGTH + 1);
	}

	CUSTOM_PACKET_HEAD h;
	ACCOUNT_SERVER_MOVE_DATA data;
	uint8 result;
	int32 auth[4];
	uint16 dest_port;
	char dest_ip[MAX_ACCOUNT_IP_LENGTH + 1];
};

struct ACCOUNT_SERVER_MOVE_AUTH
{
	ACCOUNT_SERVER_MOVE_AUTH()
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_AUTH, sizeof(ACCOUNT_SERVER_MOVE_AUTH));
		this->index = -1;
		memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
		this->auth[0] = 0;
		this->auth[1] = 0;
		this->auth[2] = 0;
		this->auth[3] = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint16 index;
	char account[MAX_ACCOUNT_LENGTH + 1];
	char name[MAX_CHARACTER_LENGTH + 1];
	int32 auth[4];
};

struct ACCOUNT_SERVER_MOVE_AUTH_RESULT
{
	ACCOUNT_SERVER_MOVE_AUTH_RESULT()
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_AUTH, sizeof(ACCOUNT_SERVER_MOVE_AUTH_RESULT));
		this->index = -1;
		memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
		this->start_server = -1;
		this->world = -1;
		this->x = 0;
		this->y = 0;
		this->result = 0;
		this->account_id = 0;
		memset(this->security_code, 0, MAX_SECURE_CODE_LENGTH + 1);
		this->facebook_status = 0;
		memset(this->mac, 0, MAX_ACCOUNT_MAC_LENGTH + 1);
		this->golden_channel = 0;
		this->disk_serial = 0;
		this->authorized = false;
	}

	CUSTOM_PACKET_HEAD h;
	uint16 index;
	char account[MAX_ACCOUNT_LENGTH + 1];
	char name[MAX_CHARACTER_LENGTH + 1];
	uint16 start_server;
	world_type world;
	coord_type x;
	coord_type y;
	uint8 result;
	uint32 account_id;
	char security_code[MAX_SECURE_CODE_LENGTH + 1];
	uint8 facebook_status;
	char mac[MAX_ACCOUNT_MAC_LENGTH + 1];
	time_t golden_channel;
	uint32 disk_serial;
	bool authorized;
};

struct ACCOUNT_KICK
{
	ACCOUNT_KICK(uint32 account_id)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_KICK, sizeof(ACCOUNT_KICK));
		this->account_id = account_id;
		this->type = 0;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 account_id;
	uint8 type;
};

struct ACCOUNT_RECONNECT
{
	ACCOUNT_RECONNECT()
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_RECONNECT, sizeof(ACCOUNT_RECONNECT));
		this->account_id = 0;
		memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
		memset(this->ip, 0, MAX_ACCOUNT_IP_LENGTH + 1);
		memset(this->mac, 0, MAX_ACCOUNT_MAC_LENGTH + 1);
		memset(this->security_code, 0, MAX_SECURE_CODE_LENGTH + 1);
		this->facebook_status = 0;
		this->start_server = -1;
		this->golden_channel = 0;
		this->disk_serial = 0;
		this->authorized = false;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 account_id;
	char account[MAX_ACCOUNT_LENGTH + 1];
	char ip[MAX_ACCOUNT_IP_LENGTH + 1];
	char mac[MAX_ACCOUNT_MAC_LENGTH + 1];
	char security_code[MAX_SECURE_CODE_LENGTH + 1];
	uint8 facebook_status;
	uint16 start_server;
	time_t golden_channel;
	uint32 disk_serial;
	bool authorized;
};

struct ACCOUNT_ONLINE
{
	ACCOUNT_ONLINE(const char * account)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_ONLINE, sizeof(ACCOUNT_ONLINE));
		this->account_id = 0;
		memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
		memcpy(this->account, account, MAX_ACCOUNT_LENGTH);
	}
	CUSTOM_PACKET_HEAD h;
	uint32 account_id;
	char account[MAX_ACCOUNT_LENGTH + 1];
};

struct ACCOUNT_COMPLETE_BAN
{
	ACCOUNT_COMPLETE_BAN()
	{
		this->h.set(HEADCODE_LOGIN_SERVER_COMPLETE_BAN, sizeof(ACCOUNT_COMPLETE_BAN));
		memset(this->ip, 0, MAX_ACCOUNT_IP_LENGTH + 1);
		memset(this->mac, 0, MAX_ACCOUNT_MAC_LENGTH + 1);
		this->disk_serial = 0;
		this->type = 0;
	}

	CUSTOM_PACKET_HEAD h;
	char ip[MAX_ACCOUNT_IP_LENGTH + 1];
	char mac[MAX_ACCOUNT_MAC_LENGTH + 1];
	uint32 disk_serial;
	uint8 type;
};

struct SL_ACCOUNT_VALIDATE
{
	SL_ACCOUNT_VALIDATE(uint32 account_id, bool warning)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_VALIDATE, sizeof(SL_ACCOUNT_VALIDATE));
		this->account_id = account_id;
		this->warning = warning;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 account_id;
	bool warning;
};

struct SL_ACCOUNT_CONNECT_TYPE
{
	SL_ACCOUNT_CONNECT_TYPE(uint32 account_id, uint8 type)
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_TYPE, sizeof(SL_ACCOUNT_CONNECT_TYPE));
		this->account_id = account_id;
		this->type = type;
	}

	CUSTOM_PACKET_HEAD h;
	uint32 account_id;
	uint8 type;
};

struct SL_ACCOUNT_DUPLICATE
{
	SL_ACCOUNT_DUPLICATE()
	{
		this->h.set(HEADCODE_LOGIN_SERVER_ACCOUNT_DUPLICATE, sizeof(SL_ACCOUNT_DUPLICATE));
		this->account_id = 0;
	}
	
	CUSTOM_PACKET_HEAD h;
	uint32 account_id;
};
#pragma pack()
#endif