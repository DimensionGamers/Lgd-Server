#ifndef MAIN_H
#define MAIN_H

enum WorldTimers
{
    WUPDATE_SIGNAL,
	WUPDATE_QUEUE,
	WUPDATE_PINGDB,
    WUPDATE_COUNT
};

class MainApplication: public AppConsole
{
	SingletonInstance(MainApplication);

	public:
		MainApplication();

		void Load() { }
		void CreateSetting(std::string const& key, std::string const& default) { }

		bool Run();
		void Update(uint32 diff);

		void End();

		void UpdateSignal();

		IntervalTimer m_timers[WUPDATE_COUNT];
};

#endif