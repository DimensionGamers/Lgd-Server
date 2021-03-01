#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/program_options.hpp>

void SignalHandler(const boost::system::error_code& error, int /*signalNumber*/)
{
    if (!error)
        AppConsole::StopNow(SHUTDOWN_EXIT_CODE);
}

void Main::End()
{
	this->StopDB();
	this->ShutdownThreadPool();
	
	sLog->outInfo(LOG_DEFAULT, "Closing %s", app_name.c_str());
}

bool Main::StartDB()
{
	if ( GetDBFlag() == MAIN_DB_FLAG_NONE )
		return true;

	sLog->outInfo(LOG_DEFAULT, "Starting DB...");

	MySQL::Library_Init();

	this->SetPingDB(this->pConfigPtr->GetIntDefault("DataBase.Ping", 30));

	if ( IsDBFlag(MAIN_DB_FLAG_CONFIG) )
	{
		if ( !GameServerDatabase.Init("GameServer", this->pConfigPtr) )
			return false;
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU) )
	{
		if ( !MuDatabase.Init("Mu", this->pConfigPtr) )
			return false;
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_LOG) )
	{
		if ( !MuLogDatabase.Init("MuLog", this->pConfigPtr) )
			return false;
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_LOGIN) )
	{
		if ( !LoginDatabase.Init("Login", this->pConfigPtr) )
			return false;
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_EDITOR) )
	{
		if ( !EditorDatabase.Init("Editor", this->pConfigPtr) )
			return false;
	}

	return true;
}

void Main::StopDB()
{
	if ( GetDBFlag() == MAIN_DB_FLAG_NONE )
		return;

	if ( IsDBFlag(MAIN_DB_FLAG_CONFIG) )
	{
		GameServerDatabase.Close();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU) )
	{
		MuDatabase.Close();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_LOG) )
	{
		MuLogDatabase.Close();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_LOGIN) )
	{
		LoginDatabase.Close();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_EDITOR) )
	{
		EditorDatabase.Close();
	}

	MySQL::Library_End();
}

void Main::KeepDatabaseAlive()
{
	sLog->outInfo(LOG_SQL, "%s", __FUNCTION__);

	if ( IsDBFlag(MAIN_DB_FLAG_CONFIG) )
	{
		GameServerDatabase.KeepAlive();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU) )
	{
		MuDatabase.KeepAlive();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_LOG) )
	{
		MuLogDatabase.KeepAlive();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_LOGIN) )
	{
		LoginDatabase.KeepAlive();
	}

	if ( IsDBFlag(MAIN_DB_FLAG_MU_EDITOR) )
	{
		EditorDatabase.KeepAlive();
	}
}

void Main::StartThreadPool()
{
	thread_pool.push_back(std::thread(boost::bind(&boost::asio::io_service::run, &_ioService)));
}
	
void Main::ShutdownThreadPool()
{
	_ioService.stop();

    for (auto& thread : thread_pool)
    {
        thread.join();
    }
}

/* ============================================================
						APP CONSOLE
============================================================ */

std::atomic<bool> AppConsole::m_stopEvent(false);
uint8 AppConsole::m_ExitCode = SHUTDOWN_EXIT_CODE;

bool AppConsole::Start()
{
	sMiniDump->Start(GetConfigFileName());

	SetConsoleTitle(sConfig->GetStringDefault("ConsoleTitle", "").c_str());

	sLog->outInfo(LOG_DEFAULT, "========================================================");
	sLog->outInfo(LOG_DEFAULT, "========================================================");
	sLog->outInfo(LOG_DEFAULT, ">>>>>>>>>>>>>>>>>>>>> %s <<<<<<<<<<<<<<<<<<<<<<", app_log.c_str());
	sLog->outInfo(LOG_DEFAULT, "========================================================");
	sLog->outInfo(LOG_DEFAULT, "========================================================");

	// Set signal handlers (this must be done before starting io_service threads, because otherwise they would unblock and exit)
#if PLATFORM == PLATFORM_WINDOWS
	signals.add(SIGBREAK);
#endif
	signals.async_wait(SignalHandler);

	StartThreadPool();

	SetProcessPriority();

	if ( !StartDB() )
	{
		ShutdownThreadPool();
        return false;
	}

	if ( !Run() )
	{
		StopDB();
		ShutdownThreadPool();
		return false;
	}

	return true;
}

#define WORLD_SLEEP_CONST	50

void AppConsole::UpdateLoop()
{
	sLog->outInfo(LOG_DEFAULT, " ");
	sLog->outInfo(LOG_DEFAULT, "%s Ready", app_name.c_str());

	uint32 realCurrTime = 0;
    uint32 realPrevTime = getMSTime();

    uint32 prevSleepTime = 0;                               // used for balanced full tick time length near WORLD_SLEEP_CONST

    ///- While we have not World::m_stopEvent, update the world
	while (!AppConsole::IsStopped())
    {
		//++_worldLoopCounter;
        realCurrTime = getMSTime();

        uint32 diff = getMSTimeDiff(realPrevTime, realCurrTime);

        Update(diff);
        realPrevTime = realCurrTime;

        // diff (D0) include time of previous sleep (d0) + tick time (t0)
        // we want that next d1 + t1 == WORLD_SLEEP_CONST
        // we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
        // d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
        if (diff <= WORLD_SLEEP_CONST + prevSleepTime)
        {
            prevSleepTime = WORLD_SLEEP_CONST + prevSleepTime - diff;

            std::this_thread::sleep_for(std::chrono::milliseconds(prevSleepTime));
        }
        else
            prevSleepTime = 0;
	}
}

bool InitConfig(std::string const& filename)
{
	if ( !filename.empty() )
	{
		std::string configError;
		if (!sConfigCommon->LoadInitial(filename, configError))
		{
			printf("Error in common config file: %s\n", configError.c_str());
			return false;
		}
	}

	std::string configError;
	if (!sConfig->LoadInitial(GetConfigFileName(), configError))
	{
		printf("Error in config file: %s\n", configError.c_str());
		return false;
	}

	return true;
}