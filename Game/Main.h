#ifndef MAIN_H
#define MAIN_H

/// Timers for different object refresh rates
enum WorldTimers
{
    WUPDATE_WEATHERS,
	WUPDATE_VIEWPORT,
    WUPDATE_PINGDB,
	WUPDATE_SHUTDOWN,
	WUPDATE_EVENT_ANIMATION,
    WUPDATE_COUNT
};

class MainApplication: public AppConsole
{
	SingletonInstance(MainApplication);

	public:
		MainApplication();
		virtual ~MainApplication();

		void Load();
		void CreateSetting(std::string const& key, std::string const& default);

		void End();
		bool Run();
		void Update(uint32 diff);
		
		IntervalTimer m_timers[WUPDATE_COUNT];
};

#define sMain SingNull(MainApplication)

#endif