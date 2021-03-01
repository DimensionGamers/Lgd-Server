#ifndef APP_H
#define APP_H

enum
{
	MAIN_DB_FLAG_NONE,
	MAIN_DB_FLAG_CONFIG			= 1 << 0,
	MAIN_DB_FLAG_MU				= 1 << 1,
	MAIN_DB_FLAG_MU_LOG			= 1 << 2,
	MAIN_DB_FLAG_MU_LOGIN		= 1 << 3,
	MAIN_DB_FLAG_MU_EDITOR		= 1 << 4,
};

enum MainAddType
{
	MAIN_APP_TYPE_CONSOLE		= 0,
};

extern boost::asio::io_service _ioService;
extern boost::asio::signal_set signals;

class Main: public SettingsMgr
{
	public:
		Main(std::string app_name, std::string app_log, uint8 flags, Config* pConfigPtr, MainAddType type):
			app_name(app_name),
			app_log(app_log),
			pConfigPtr(pConfigPtr)
		{
			this->SetDBFlag(flags);
			this->SetType(type);
		}
		virtual ~Main()
		{

		}

		virtual bool Start() = 0;
		virtual void End();
		virtual bool Run() = 0;

		bool StartDB();
		void StopDB();

		void StartThreadPool();
		void ShutdownThreadPool();

		void KeepDatabaseAlive();

	private:
		DECLARE_FLAG(uint8, DBFlag);
		DECLARE_ENUM(MainAddType, Type);
		DECLARE_ENUM(uint32, PingDB);

		std::vector<std::thread> thread_pool;
		std::string app_name;
		std::string app_log;
		Config* pConfigPtr;
};

class AppConsole: public Main
{
	public:
		AppConsole(std::string app_name, std::string app_log, uint8 flags, Config* pConfigPtr): Main(app_name, app_log, flags, pConfigPtr, MAIN_APP_TYPE_CONSOLE)
		{

		}

		virtual ~AppConsole()
		{

		}

		bool Start();
		void UpdateLoop();
		virtual void Update(uint32 diff) = 0;

		static uint8 GetExitCode() { return m_ExitCode; }
        static void StopNow(uint8 exitcode) { m_stopEvent = true; m_ExitCode = exitcode; }
        static bool IsStopped() { return m_stopEvent; }

		static std::atomic<bool> m_stopEvent;
        static uint8 m_ExitCode;
};

bool InitConfig(std::string const& filename);

#endif