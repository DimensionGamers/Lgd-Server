bool StartNetwork();

Config * sConfig = new Config;
Config * sConfigCommon = sConfig;

boost::asio::io_service _ioService;
boost::asio::signal_set signals(_ioService, SIGINT, SIGTERM);

MainApplication::MainApplication(): AppConsole("CSServer", "CONNECT SERVER", MAIN_DB_FLAG_MU_LOGIN, sConfig)
{
}

MainApplication::~MainApplication()
{
	
}

int main(int argc, char *argv[])
{
	sLog->LoadFolder();

	sLog->LoadAppender("Console", APPENDER_CONSOLE, LOG_LEVEL_TRACE, APPENDER_FLAGS_NONE, "", "", "w");
	sLog->LoadAppender("ConnectServer", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "ConnectServer.log", "w");
	sLog->LoadAppender("Database", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "Database.log", "w");

	sLog->LoadLogger("root", LOG_LEVEL_TRACE, " Console ConnectServer");
	sLog->LoadLogger("database", LOG_LEVEL_TRACE, "Database");

	//StartStackLogging();
	if ( !InitConfig("") )
	{
		system("PAUSE");
		return 1;
	}

	if ( !sMain->Start() )
		return 0;

	sMain->UpdateLoop();

	sMain->End();

	return sMain->GetExitCode();
}

void MainApplication::End()
{
	sMainSocketMgr.StopNetwork();
	sGSSocketMgr.StopNetwork();

	AppConsole::End();
}

bool MainApplication::Run()
{
	sServer->Initialize(sConfig->GetIntDefault("ServerStateUpdateDelay", 0));
	sMain->LoadVersion();

	LoginDatabase.Execute("UPDATE server_list SET online = 0");

	if ( !StartNetwork() )
		return false;

	m_timers[WUPDATE_PINGDB].SetInterval(this->GetPingDB() * MINUTE * IN_MILLISECONDS);
	m_timers[WUPDATE_CHANNEL].SetInterval(30 * IN_MILLISECONDS);

	return true;
}

bool StartNetwork()
{
    uint16 worldPort = uint16(sConfig->GetIntDefault("Network.Port", 0));
    std::string worldListener = sConfig->GetStringDefault("Network.BindIP", "0.0.0.0");

    if ( !sMainSocketMgr.StartNetwork(_ioService, worldListener, worldPort, 1) )
		return false;

	worldPort = uint16(sConfig->GetIntDefault("CSNetwork.Port", 0));
    worldListener = sConfig->GetStringDefault("CSNetwork.BindIP", "0.0.0.0");

    if ( !sGSSocketMgr.StartNetwork(_ioService, worldListener, worldPort, 1) )
		return false;

	return true;
}

void MainApplication::LoadVersion()
{
	this->update_address = sConfig->GetStringDefault("UpdateAddress", "");
}

void MainApplication::LoadAccountTime()
{
	this->SetMaxConnectionCount(sConfig->GetIntDefault("Network.MaxConnectionCount", 0));
	this->SetMaxConnectionIdle(sConfig->GetIntDefault("Network.MaxConnectionIdle", 0) * IN_MILLISECONDS);

	this->SetUpdateHeadcode(sConfig->GetIntDefault("Update.Headcode", 0));
}

void MainApplication::Update(uint32 diff)
{
	for (int32 i = 0; i < WUPDATE_COUNT; ++i)
	{
		if (m_timers[i].GetCurrent() >= 0)
			m_timers[i].Update(diff);
		else
			m_timers[i].SetCurrent(0);
	}

	if (m_timers[WUPDATE_PINGDB].Passed())
	{
		m_timers[WUPDATE_PINGDB].Reset();

		KeepDatabaseAlive();
	}

	if (m_timers[WUPDATE_CHANNEL].Passed())
	{
		m_timers[WUPDATE_CHANNEL].Reset();

		sServer->SendChannel();
	}
}