CServerList::CServerList() : m_UpdateInterval(0), m_NextUpdateTime(time(NULL))
{

}

CServerList::~CServerList()
{
	MAP_CLEAR(ServerDataMap::iterator, this->server_map);
}

ServerData * CServerList::FindServer(uint16 code)
{
	ServerDataMap::const_iterator it = this->server_map.find(code);

	return it != this->server_map.end() ? it->second : nullptr;
}

ServerData const* CServerList::FindServerByDisplay(uint16 id) const
{
	for (ServerDataMap::const_iterator itr = this->server_map.begin(); itr != this->server_map.end(); ++itr)
	{
		ServerData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetDisplayID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

void CServerList::Initialize(uint32 updateInterval)
{
    m_UpdateInterval = updateInterval;

	this->LoadServerList();
	sMain->LoadAccountTime();
	
	LoginDatabase.Execute("UPDATE server_list SET online = 0");
}

void CServerList::UpdateServerList()
{
	 if (!m_UpdateInterval || m_NextUpdateTime > time(NULL))
        return;

    m_NextUpdateTime = time(NULL) + m_UpdateInterval;

	this->LoadServerList();
	sMain->LoadAccountTime();
	sMain->LoadVersion();
}

void CServerList::SendChannel()
{
	uint8 buffer[4092];
	POINTER_PCT(CS_CHANNEL_HEAD, head, buffer, 0);
	POINTER_PCT(CS_CHANNEL_BODY, body, buffer, sizeof(CS_CHANNEL_HEAD));

	for (int32 i = 0; i < 10; ++i)
	{
		head->count = 0;

		for (ServerDataMap::const_iterator itr = this->server_map.begin(); itr != this->server_map.end(); ++itr)
		{
			ServerData const* pData = itr->second;

			if (!pData)
			{
				continue;
			}

			if ((pData->GetServer() / MAX_SERVER_PER_GROUP) != i)
			{
				continue;
			}

			if (!pData->IsActive() || !pData->IsOnline() || !pData->IsFlag(SERVER_FLAG_DISPLAY) || pData->IsFlag(SERVER_FLAG_DISABLED))
			{
				continue;
			}
			
			body[head->count].server = pData->GetServer();
			body[head->count].data1 = 0;
			body[head->count].data2 = 0;
			body[head->count].type = pData->GetType() & 1;
			body[head->count].gold = pData->GetType() >= 2 ? 1 : 0;
			++head->count;
		}

		if (head->count <= 0)
		{
			continue;
		}

		head->h.set(HEADCODE_CONNECT_SERVER_CHANNELS, sizeof(CS_CHANNEL_HEAD) + (head->count * sizeof(CS_CHANNEL_BODY)));

		sGSSocketMgr.SendPacketAll(buffer, head->h.size, i);
	}
}

void CServerList::LoadServerList()
{
	sLog->outInfo(LOG_DEFAULT, "Reading Server List...");

	QueryResult result = LoginDatabase.Query("SELECT * FROM server_list");
	int32 count = 0;

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			uint16 server = fields[0].GetUInt16();
			bool add = false;

			ServerData * add_server = this->FindServer(server);

			if ( !add_server )
			{
				add_server = new ServerData;
				add = true;
			}

			add_server->SetServer(server);
			add_server->SetDisplayID(fields[1].GetUInt16());
			add_server->SetName(fields[2].GetString());
			add_server->SetPort(fields[3].GetUInt16());
			add_server->SetIP(fields[4].GetString());
			add_server->SetFlag(fields[5].GetUInt8());
			add_server->SetType(fields[10].GetUInt8());

			if ( add )
			{
				add_server->SetPercent(0);
				add_server->SetOnline(false);
				add_server->SetActive(false);
				this->server_map[add_server->GetServer()] = add_server;
			}

			if ( !add_server->IsActive() )
			{
				add_server->AddFlag(SERVER_FLAG_DISABLED);
			}
			else
			{
				add_server->RemoveFlag(SERVER_FLAG_DISABLED);
			}
			
			sLog->outInfo(LOG_DEFAULT, "Added Server %u [%s]    Port: %u   IP: %s   Display: %d	AccountAllowed: %d	Disabled: %d", add_server->GetServer(),
			add_server->GetName().c_str(), add_server->GetPort(), add_server->GetIP().c_str(), 
			add_server->IsFlag(SERVER_FLAG_DISPLAY), 
			add_server->IsFlag(SERVER_FLAG_ACCOUNT_ALLOWED),
			add_server->IsFlag(SERVER_FLAG_DISABLED));

			++count;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u servers", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CServerList::ServerListRequest(std::shared_ptr<MainSocket> client)
{
	this->UpdateServerList();

	if ( this->server_map.empty() )
	{
		client->CloseSocket();
		return;
	}

	uint8 buffer[10000];
	POINTER_PCT(SERVER_DATA_HEAD, head, buffer, 0);
	POINTER_PCT(SERVER_DATA_BODY, body, buffer, sizeof(SERVER_DATA_HEAD));

	head->h.set(HEADCODE_CONNECT_SERVER, SUBCODE_CONNECT_SERVER_SERVER_LIST, sizeof(SERVER_DATA_HEAD));

	uint16 size = sizeof(SERVER_DATA_HEAD);
	uint16 count = 0;

	for (ServerDataMap::const_iterator it = this->server_map.begin(); it != this->server_map.end(); ++it)
	{
		ServerData const* pData = it->second;

		if (!pData)
		{
			continue;
		}

		if (!pData->IsFlag(SERVER_FLAG_DISPLAY) || pData->IsFlag(SERVER_FLAG_DISABLED) || !pData->IsOnline())
		{
			continue;
		}

		body[count].server_code = pData->GetDisplayID();
		body[count].percent = pData->GetPercent();
		body[count].type = pData->GetType();
		size += sizeof(SERVER_DATA_BODY);
		count++;
	}

	if ( count <= 0)
	{
		client->CloseSocket();
		return;
	}

	head->countH = SET_NUMBERH(count);
	head->countL = SET_NUMBERL(count);
	head->h.set_size(size);

	client->SendPacket(buffer, size);
}

void CServerList::ServerSelectRequest(std::shared_ptr<MainSocket> client, uint8 * Packet)
{
	POINTER_PCT(SERVER_INFO_REQUEST, lpMsg, Packet, 0);

	ServerData const* server_data = this->FindServerByDisplay(lpMsg->server_code);

	if ( !server_data )
	{
		client->CloseSocket();
		return;
	}

	if ( !server_data->IsFlag(SERVER_FLAG_DISPLAY) || server_data->IsFlag(SERVER_FLAG_DISABLED) || !server_data->IsOnline() )
	{
		client->CloseSocket();
		return;
	}

	SERVER_INFO_RESULT pMsg(server_data->GetIP().c_str(), server_data->GetPort());

	client->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void CServerList::ServerSetInfo(std::shared_ptr<GSSocket> client, uint8 * Packet)
{
	POINTER_PCT(CS_GAMESERVER_USER_PERCENT, lpMsg, Packet, 0);

	ServerData * server_data = this->FindServer(lpMsg->h.server);

	if ( !server_data )
	{
		return;
	}

	server_data->SetPercent(lpMsg->percent);
	server_data->SetActive(lpMsg->active);
	
	if ( !lpMsg->active )
	{
		server_data->AddFlag(SERVER_FLAG_DISABLED);
	}
	else
	{
		server_data->RemoveFlag(SERVER_FLAG_DISABLED);
	}

	CS_GAMESERVER_FLAG pMsg(server_data->GetFlag());
	client->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void CServerList::ServerVersionCheck(std::shared_ptr<MainSocket> client, uint8 * Packet)
{
	POINTER_PCT(MU_VERSION_CHECK, lpMsg, Packet, 0);

	MU_VERSION_CHECK_RESULT pMsg;
	memcpy(pMsg.address, sMain->update_address.c_str(), sMain->update_address.length());
	pMsg.data[0] = 1;
	pMsg.data[1] = 1;

	client->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void CServerList::LauncherError(std::shared_ptr<MainSocket> client, uint8 * Packet)
{
	POINTER_PCT(MU_LAUNCHER_ERROR, lpMsg, Packet, 0);

	sLog->outError("root", "LauncherError: %s", lpMsg->path_error);
}

void CServerList::ServerConnect(uint16 server)
{
	if ( ServerData * server_data = this->FindServer(server) )
	{
		server_data->SetOnline(true);
		sLog->outInfo("root", "[ SERVER CONNECT ] Connecting Server - %s / %d", server_data->GetName().c_str(), server);
	}

	SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GAMESERVER);
	stmt->setUInt8(0, 1);
	stmt->setUInt16(1, server);
	trans->Append(stmt);

	LoginDatabase.CommitTransaction(trans);
}

void CServerList::ServerClose(uint16 server)
{
	if ( ServerData * server_data = this->FindServer(server) )
	{
		server_data->SetOnline(false);
		sLog->outInfo("root", "[ SERVER CLOSE ] Closing Server - %s / %d", server_data->GetName().c_str(), server);
	}

	SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GAMESERVER);
	stmt->setUInt8(0, 0);
	stmt->setUInt16(1, server);
	trans->Append(stmt);

	LoginDatabase.CommitTransaction(trans);
}