#ifndef MAIN_H
#define MAIN_H

enum WorldTimers
{
    WUPDATE_QUEUE,
	WUPDATE_PINGDB,
	WUPDATE_UPD,
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

	public:
		void LoadAccountTime();

		DECLARE_ENUM(uint32, AccountMovingTime);
		DECLARE_BOOL(AccountAuthorizationEnabled);
		DECLARE_ENUM(int32, AccountWrongAuthorizationCount);
		DECLARE_ENUM(uint32, AccountWrongAuthorizationTime);
		
		IntervalTimer m_timers[WUPDATE_COUNT];
};

#endif