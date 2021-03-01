#ifndef SERVER_LIST_H
#define SERVER_LIST_H

struct ServerData
{
	DECLARE_ENUM(uint16, Server);
	DECLARE_ENUM(uint16, DisplayID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint16, Port);
	DECLARE_PROPERTY_STRING(IP);
	DECLARE_FLAG(uint8, Flag);
	DECLARE_ENUM(uint8, Percent);
	DECLARE_BOOL(Online);
	DECLARE_ENUM(uint8, Type);
	DECLARE_BOOL(Active);
};

typedef std::map<uint16, ServerData*> ServerDataMap;

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

class CServerList
{
	SingletonInstance(CServerList);

	public:
		CServerList();
		virtual ~CServerList();

		ServerDataMap server_map;
		ServerData * FindServer(uint16 code);
		ServerData const* FindServerByDisplay(uint16 id) const;

		void Initialize(uint32 updateInterval);
		void LoadServerList();
		void UpdateServerList();
		void SendChannel();
		
		void ServerListRequest(std::shared_ptr<MainSocket> client);
		void ServerSelectRequest(std::shared_ptr<MainSocket> client, uint8 * Packet);
		void ServerSetInfo(std::shared_ptr<GSSocket> client, uint8 * Packet);
		void ServerVersionCheck(std::shared_ptr<MainSocket> client, uint8 * Packet);
		void LauncherError(std::shared_ptr<MainSocket> client, uint8 * Packet);

		void ServerConnect(uint16 server);
		void ServerClose(uint16 server);

		uint32   m_UpdateInterval;
		time_t   m_NextUpdateTime;
};

#endif