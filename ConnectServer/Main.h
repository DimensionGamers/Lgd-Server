#ifndef MAIN_H
#define MAIN_H

enum UPDATE_DOWNLOAD_TYPE
{
	UPDATE_DOWNLOAD_TYPE_FTP,
	UPDATE_DOWNLOAD_TYPE_FTP_TEST,
	UPDATE_DOWNLOAD_TYPE_HTTP,
	UPDATE_DOWNLOAD_TYPE_MAX
};

struct UpdateDownloadData
{
	std::string address;
	uint16 port;
	std::string user;
	std::string password;
};

enum WorldTimers
{
	WUPDATE_PINGDB,
	WUPDATE_CHANNEL,
    WUPDATE_COUNT
};

class MainApplication: public AppConsole
{
	SingletonInstance(MainApplication);

	public:
		MainApplication();
		virtual ~MainApplication();

		void Load() { }
		void CreateSetting(std::string const& key, std::string const& default) { }

		bool Run();
		void Update(uint32 diff);
		void End();

		uint8 version[3];
		std::string filename;

		DECLARE_ENUM(uint8, DownloadType);
		DECLARE_ARRAY_STRUCT(UpdateDownloadData, DownloadData, UPDATE_DOWNLOAD_TYPE_MAX);

	public:
		void LoadVersion();
		void LoadAccountTime();

		DECLARE_ENUM(size_t, MaxConnectionCount);
		DECLARE_ENUM(uint32, MaxConnectionIdle);

		IntervalTimer m_timers[WUPDATE_COUNT];

		DECLARE_ENUM(uint8, UpdateHeadcode);
		std::string update_address;
};

#endif