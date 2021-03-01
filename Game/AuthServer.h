#ifndef LOGIN_SERVER_H
#define LOGIN_SERVER_H

class AuthServerSocket: public TCPClient<AuthServerSocket>
{
	public:
		explicit AuthServerSocket(): TCPClient(_ioService, "AuthServer", TCPCLIENT_ON_ERROR_CLOSE) {}

		void OnStart() override;
		void OnStop() override;
		void ReadHandler() override;
};

class AuthServer: public TCPClientMgr<AuthServerSocket>
{
	SingletonInstance(AuthServer);

	public:
		AuthServer();

		static std::unordered_map<uint8, LoginHandler<AuthServer>> InitHandlers();

		void HandleHeadcodeOnConnect(uint8 * Packet);

		void SendData();
		void SendPlayerPercent();
		void PlayerLoginRequest(Player* pPlayer, PlayerLoginStep step);
		void PlayerLoginResult(uint8 * Packet);
		void PlayerServerMoveRequest(Player* pPlayer, uint16 server, world_type world, coord_type x, coord_type y, bool channel = false);
		void PlayerServerMoveResult(uint8 * Packet);
		void PlayerServerMoveAuth(Player* pPlayer, uint8 * Packet);
		void PlayerServerMoveAuthResult(Player* pPlayer, uint8 result);
		void PlayerServerMoveAuthResult(uint8 * Packet);

		void PlayerLogout(uint32 account_id);
		void PlayerAccountKick(uint32 account_id);
		void PlayerAccountKick(uint8 * Packet);
		void PlayerAccountOnline(uint8 * Packet);

		void AccountCompleteBan(const char * ip, const char * mac, uint32 disk_serial, uint8 type);
		void AccountCompleteBan(uint8 * Packet);

		void AccountDuplicate(uint8 * Packet);

		void AccountAuthorization(uint32 account_id, bool warning);

		void HandlePacketData(uint8 headcode, uint8 * packet);
};

#endif