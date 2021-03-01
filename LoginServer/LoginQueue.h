#ifndef LOGIN_QUEUE_H
#define LOGIN_QUEUE_H

struct LoginData
{
	explicit LoginData(uint16 entry, const char * account, const char * password): 
		entry(entry),
		server(-1),
		ms_time(0),
		disk_serial(0),
		account_id(0),
		autologin(0)
	{
		memcpy(this->account, account, MAX_ACCOUNT_LENGTH + 1);
		memcpy(this->password, password, MAX_PASSWORD_LENGTH + 1);
	}

	uint16 server;
	uint16 entry;
	uint32 ms_time;
	char account[MAX_ACCOUNT_LENGTH + 1];
	char password[MAX_PASSWORD_LENGTH + 1];
	std::shared_ptr<AuthSocket> socket;
	char ip[MAX_ACCOUNT_IP_LENGTH + 1];
	char mac[MAX_ACCOUNT_MAC_LENGTH + 1];
	uint32 disk_serial;
	uint32 account_id;
	uint8 autologin;

	uint16 GetServerGroup() const { return this->server / 20; }
};

typedef std::shared_ptr<LoginData> LoginDataPtr;

struct AccountData
{
	AccountData()
	{
		memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
		this->guid = 0;
		this->SetStartServer(-1);
		this->SetCurrentServer(-1);
		this->SetOffline(false);
		this->SetMoving(false);
		this->SetTime(time(nullptr));
		this->SetDestServer(-1);
		this->SetDestWorld(-1);
		this->SetDestX(0);
		this->SetDestY(0);
		this->ResetAuth(0);
		this->ResetMAC();
		this->ResetSecureCode();
		this->SetFacebookStatus(0);
		this->SetMovingTick(0);
		this->ResetIP();
		this->SetGoldenChannel(0);
		this->SetDiskSerial(0);
		this->SetAuthorized(false);
	}

	char account[MAX_ACCOUNT_LENGTH + 1];
	
	uint32 guid;

	DECLARE_ENUM(time_t, Time);
	DECLARE_ENUM(uint16, StartServer);
	DECLARE_ENUM(uint16, CurrentServer);
	DECLARE_BOOL(Offline);
	DECLARE_BOOL(Moving);
	DECLARE_ENUM(uint32, MovingTick);
	DECLARE_ENUM(int16, DestServer);
	DECLARE_ENUM(int16, DestWorld);
	DECLARE_ENUM(int16, DestX);
	DECLARE_ENUM(int16, DestY);
	DECLARE_PROPERTY_ARRAY(int32, Auth, 4);
	DECLARE_STRING_FIXED(MAC, MAX_ACCOUNT_MAC_LENGTH + 1);
	DECLARE_STRING_FIXED(SecureCode, MAX_SECURE_CODE_LENGTH + 1);
	DECLARE_STRING_FIXED(IP, MAX_ACCOUNT_IP_LENGTH + 1);
	DECLARE_ENUM(uint8, FacebookStatus);
	DECLARE_ENUM(time_t, GoldenChannel);
	DECLARE_ENUM(uint32, DiskSerial);
	DECLARE_BOOL(Authorized);
};

typedef std::shared_ptr<AccountData> AccountDataPtr;
typedef std::unordered_map<uint32, AccountDataPtr> AccountLoggedList;
typedef std::unordered_map<uint16, AccountLoggedList> AccountLoggedMap;

struct WrongAuthorizationData
{
	WrongAuthorizationData(uint32 account_id, uint32 disk_serial, int32 count): account_id(account_id), disk_serial(disk_serial), count(count)
	{

	}

	uint32 account_id;
	uint32 disk_serial;
	int32 count;
};

typedef std::vector<WrongAuthorizationData> WrongAuthorizationDataList;

class LoginQueue
{
	SingletonInstance(LoginQueue);

	public:
		LoginQueue();
		virtual ~LoginQueue();

		void Queue(LoginDataPtr data);
		void Update();
		void Process(LoginDataPtr data);

		void DisconnectAccount(uint16 server, uint32 account);

		void CloseServer(uint16 server);

		void KickAccount(uint16 server_group, uint32 account_id);

		bool IsAccountLogged(uint16 server_group, const char * account);
		AccountDataPtr GetAccountData(uint16 server_group, uint32 account);
		AccountDataPtr GetAccountData(uint16 server_group, const char * account);

		void DBAccountStatus(AccountDataPtr pAccount, bool online, bool on_connect);

		void ConnectAccount(uint16 start_server, uint16 current_server, const char * account, uint32 guid, const char * secure_code, const char * ip, const char * mac, uint8 facebook_status, time_t golden_channel, uint32 disk_serial, bool authorized);

		void CompleteAccountBan(const char * ip, const char * mac, uint32 disk_serial, uint8 type);

		void ClearWrongAccountAuthorization(uint32 account_id, uint32 disk_serial);
		bool IncreaseWrongAccountAuthorization(uint32 account_id, uint32 disk_serial);
	private:
		LockedQueue<LoginDataPtr> LoginDataQueue;

		AccountLoggedMap account_logged_map;
		WrongAuthorizationDataList account_wrong_authorization_list;

		DECLARE_ENUM(uint32, UpdateTick);
		DECLARE_ENUM(uint32, VerifyTick);
};

#endif