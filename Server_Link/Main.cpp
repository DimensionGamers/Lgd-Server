/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Main.cpp"
*
*/

bool StartNetwork();

Config * sConfig = new Config;
Config * sConfigCommon = sConfig;

boost::asio::io_service _ioService;
boost::asio::signal_set signals(_ioService, SIGINT, SIGTERM);

MainApplication::MainApplication(): AppConsole("ServerLink", "SERVER LINK", MAIN_DB_FLAG_MU, sConfig)
{
}

int main()
{
	sLog->LoadFolder();

	sLog->LoadAppender("Console", APPENDER_CONSOLE, LOG_LEVEL_TRACE, APPENDER_FLAGS_NONE, "", "", "w");
	sLog->LoadAppender("ServerLink", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "ServerLink.log", "w");
	sLog->LoadAppender("Database", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "Database.log", "w");
	sLog->LoadAppender("Player", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "Player.log", "w");
	sLog->LoadAppender("Guild", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "Guild.log", "w");

	sLog->LoadLogger("root", LOG_LEVEL_TRACE, " Console ServerLink");
	sLog->LoadLogger("database", LOG_LEVEL_TRACE, "Database");
	sLog->LoadLogger("player", LOG_LEVEL_TRACE, "Player");
	sLog->LoadLogger("guild", LOG_LEVEL_TRACE, "Guild");

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
	sServerSocketMgr.StopNetwork();

	AppConsole::End();
}

bool StartNetwork()
{
    uint16 worldPort = uint16(sConfig->GetIntDefault("Network.Port", 0));
    std::string worldListener = sConfig->GetStringDefault("Network.BindIP", "0.0.0.0");

    return sServerSocketMgr.StartNetwork(_ioService, worldListener, worldPort, 1);
}

bool MainApplication::Run()
{
	MuDatabase.Execute("UPDATE character_info SET online = 0");

	if ( !StartNetwork() )
		return false;

	sGuildMgr->LoadGuilds();
	sGuildMatching->LoadGuildMatching();

	m_timers[WUPDATE_SIGNAL].SetInterval(MINUTE * IN_MILLISECONDS);
	m_timers[WUPDATE_QUEUE].SetInterval(1 * IN_MILLISECONDS);
	m_timers[WUPDATE_PINGDB].SetInterval(this->GetPingDB() * MINUTE * IN_MILLISECONDS);

	return true;
}

void MainApplication::UpdateSignal()
{
	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_SIGNAL_SELECT);

	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		SL_SIGNAL pMsg;
		pMsg.h.server = fields[0].GetUInt16();
		pMsg.signal = fields[1].GetUInt16();
		pMsg.data[0] = fields[2].GetInt32();
		pMsg.data[1] = fields[3].GetInt32();
		pMsg.data[2] = fields[4].GetInt32();

		sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());

		uint32 id = fields[5].GetUInt32();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_SIGNAL_DELETE);
		stmt->setUInt32(0, id);
		MuDatabase.Execute(stmt);
	}
	while( result->NextRow() );
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

	if ( m_timers[WUPDATE_QUEUE].Passed() )
	{
		m_timers[WUPDATE_QUEUE].Reset();

		sCharacterCreateQueue->Update();
		sGuildCreateQueue->Update();
		sCharacterMgr->Update();
	}

	if ( m_timers[WUPDATE_SIGNAL].Passed() )
	{
		m_timers[WUPDATE_SIGNAL].Reset();

		UpdateSignal();
	}

	sServerSocketMgr.UpdatePacket();

	if (m_timers[WUPDATE_PINGDB].Passed())
	{
		m_timers[WUPDATE_PINGDB].Reset();

		KeepDatabaseAlive();
	}
}