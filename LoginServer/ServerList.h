#ifndef SERVER_LIST_H
#define SERVER_LIST_H

struct SERVER_DATA_HEAD
{
	PWMSG_HEAD2 h;
	uint8 countH;
	uint8 countL;
};

struct SERVER_DATA_BODY
{
	uint16 server_code;
	uint8 percent;
	uint8 type;
};

struct SERVER_INFO_REQUEST
{
	PBMSG_HEAD2 h;
	uint16 server_code;
};

typedef std::set<world_type> WorldServerList;
typedef std::map<uint16, WorldServerList> WorldServerMap;

struct ServerData
{
	ServerData()
	{
		this->SetOnline(false);
		this->SetPercent(0);
		this->SetPort(-1);
		this->ResetIP();
		this->SetFlag(0);
		this->SetType(0);
	}

	DECLARE_BOOL(Online);
	DECLARE_ENUM(uint8, Percent);
	DECLARE_ENUM(uint16, Port);
	DECLARE_STRING_FIXED(IP, 16);
	DECLARE_FLAG(uint8, Flag);
	DECLARE_ENUM(uint8, Type);
};

typedef std::map<uint16, ServerData> ServerDataMap;

class CServerList
{
	SingletonInstance(CServerList);

	public:
		CServerList();
		virtual ~CServerList();

		bool IsServerOnlineAndFree(uint16 server);
		bool IsWorldInList(world_type world, uint16 server_group);

		void Initialize(uint32 updateInterval);
		void LoadWorldServer();
		void UpdateServerList();
		
		void ServerOpen(uint16 server, uint16 port, const char * ip, uint8 flag, uint8 type);
		void ServerClose(uint16 server);
		void ServerSetInfo(uint16 server, uint8 percent, uint8 flag, uint8 type);

		uint16 CheckDestServer(uint16 server_group, world_type world, uint16 server, uint16 start_server);

		uint32   m_UpdateInterval;
		time_t   m_NextUpdateTime;

		WorldServerMap world_server_map;
		ServerDataMap server_map;
};

#endif