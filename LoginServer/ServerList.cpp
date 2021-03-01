CServerList::CServerList() : m_UpdateInterval(0), m_NextUpdateTime(time(NULL))
{

}

CServerList::~CServerList()
{
	this->world_server_map.clear();
	this->server_map.clear();
}

void CServerList::Initialize(uint32 updateInterval)
{
    m_UpdateInterval = updateInterval;

	this->LoadWorldServer();
	sMain->LoadAccountTime();
}

void CServerList::UpdateServerList()
{
	 if (!m_UpdateInterval || m_NextUpdateTime > time(NULL))
        return;

    m_NextUpdateTime = time(NULL) + m_UpdateInterval;

	this->LoadWorldServer();
	sMain->LoadAccountTime();
}

void CServerList::LoadWorldServer()
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Server...");

	this->world_server_map.clear();

	uint32 count = 0;

	QueryResult result = LoginDatabase.Query("SELECT * FROM world_server");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint16 server = fields[0].GetUInt16();
			world_type world = fields[1].GetUInt16();

			this->world_server_map[server].insert(world);

			++count;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world server definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CServerList::ServerOpen(uint16 server, uint16 port, const char * ip, uint8 flag, uint8 type)
{
	ServerData & data = this->server_map[server];
	data.SetPort(port);
	data.SetIP(ip, 16);
	data.SetPercent(0);
	data.SetFlag(flag);
	data.SetType(type);
	data.SetOnline(true);
}

void CServerList::ServerClose(uint16 server)
{
	ServerData & data = this->server_map[server];
	data.SetOnline(false);
	data.SetPercent(0);

	sLoginQueue->CloseServer(server);
}

void CServerList::ServerSetInfo(uint16 server, uint8 percent, uint8 flag, uint8 type)
{
	ServerData & data = this->server_map[server];

	if ( !data.IsOnline() )
	{
		return;
	}

	data.SetPercent(percent);
	data.SetFlag(flag);
	data.SetType(type);
}

bool CServerList::IsServerOnlineAndFree(uint16 server)
{
	ServerData & data = this->server_map[server];

	if ( !data.IsOnline() )
	{
		return false;
	}

	if ( data.IsFlag(SERVER_FLAG_DISABLED) )
	{
		return false;
	}

	return data.GetPercent() < 100;
}

bool CServerList::IsWorldInList(world_type world, uint16 server_group)
{
	for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
	{
		if ( (itr->first / MAX_SERVER_PER_GROUP) != server_group )
			continue;

		WorldServerList::const_iterator it = itr->second.find(world);

		if ( it != itr->second.end() )
			return true;
	}

	return false;
}

uint16 CServerList::CheckDestServer(uint16 server_group, world_type world, uint16 server, uint16 start_server)
{
	bool is_in_list = this->IsWorldInList(world, server_group);
	
	if ( !is_in_list )
	{
		server = start_server;
	}
	
	if ( !this->IsServerOnlineAndFree(server) )
	{
		server = -1;
	}
	else
	{
		WorldServerMap::const_iterator world_list_iterator = this->world_server_map.find(server);

		if ( is_in_list )
		{
			if ( world_list_iterator != this->world_server_map.end() )
			{
				WorldServerList::const_iterator it = world_list_iterator->second.find(world);

				if ( it != world_list_iterator->second.end() )
					return server;
			}
		}
		else
		{
			if ( world_list_iterator == this->world_server_map.end() )
				return server;
		}

		server = -1;
	}

	if ( is_in_list )
	{
		for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
		{
			if ( server_group != (itr->first / MAX_SERVER_PER_GROUP) )
				continue;

			WorldServerList::const_iterator it = itr->second.find(world);

			if ( it != itr->second.end() )
			{
				if ( this->IsServerOnlineAndFree(itr->first) )
					return itr->first;
			}
		}
	}
	else
	{
		for ( ServerDataMap::const_iterator it = this->server_map.begin(); it != this->server_map.end(); ++it )
		{
			if ( server_group != (it->first / MAX_SERVER_PER_GROUP) )
				continue;

			WorldServerMap::const_iterator itr = this->world_server_map.find(it->first);

			if ( itr != this->world_server_map.end() ) // Si el servidor tiene mapas sigo buscando
				continue;

			if ( this->IsServerOnlineAndFree(it->first) )
				return it->first;
		}
	}

	return -1;
}