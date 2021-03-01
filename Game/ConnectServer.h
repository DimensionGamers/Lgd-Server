#ifndef CONNECT_SERVER_H
#define CONNECT_SERVER_H
class ConnectServerSocket: public TCPClient<ConnectServerSocket>
{
	public:
		explicit ConnectServerSocket(): TCPClient(_ioService, "ConnectServer", TCPCLIENT_ON_ERROR_CLOSE) {}

		void OnStart() override;
		void OnStop() override;
		void ReadHandler() override;
};

struct ChannelData
{
	DECLARE_ENUM(uint16, Server);
	DECLARE_ENUM(uint16, Data1);
	DECLARE_ENUM(uint16, Data2);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, Gold);
};

typedef std::map<uint16, ChannelData*> ChannelDataMap;

class ConnectServer: public TCPClientMgr<ConnectServerSocket>
{
	SingletonInstance(ConnectServer);

	public:
		ConnectServer();

		static std::unordered_map<uint8, LoginHandler<ConnectServer>> InitHandlers();

		void HandleHeadcodeOnConnect(uint8 * Packet);
		void HandleHeadcodeFlag(uint8 * Packet);
		void HandleHeadcodeChannel(uint8 * Packet);

		void SendData();
		void SendPlayerPercent();

		void HandlePacketData(uint8 headcode, uint8 * packet);

		ChannelDataMap m_ChannelDataMap;

		bool IsServerAvailable(uint16 server) const;
};

#endif