std::unordered_map<uint8, LoginHandler<ConnectServer>> ConnectServer::InitHandlers()
{
    std::unordered_map<uint8, LoginHandler<ConnectServer>> handlers;

	handlers[HEADCODE_CONNECT_SERVER_CONNECT]					= { "ON_CONNECT", sizeof(CS_GAMESERVER_CONNECT), &ConnectServer::HandleHeadcodeOnConnect };
	handlers[HEADCODE_CONNECT_SERVER_FLAG]						= { "FLAG", sizeof(CS_GAMESERVER_FLAG), &ConnectServer::HandleHeadcodeFlag };
	handlers[HEADCODE_CONNECT_SERVER_CHANNELS]					= { "CHANNEL", sizeof(CS_CHANNEL_HEAD), &ConnectServer::HandleHeadcodeChannel };
	
    return handlers;
}

std::unordered_map<uint8, LoginHandler<ConnectServer>> const Handlers = ConnectServer::InitHandlers();

ConnectServer::ConnectServer(): TCPClientMgr("ConnectServer")
{

}

void ConnectServerSocket::OnStart()
{
	sLog->outInfo("root", "CONNECTED CONNECTSERVER");

	sConnectServer->IncreaseConnectCount(1);
	sConnectServer->SetStopped(false);

	sConnectServer->SendData();
}

void ConnectServerSocket::OnStop()
{
	sLog->outInfo("root", "DISCONNECTED CONNECTSERVER");

	sConnectServer->GetReconnectTime()->Start();
	sConnectServer->SetStopped(true);
}

void ConnectServerSocket::ReadHandler()
{
	if ( this->IsStopped() )
		return;

	MessageBuffer& packet = GetReadBuffer();
    while ( packet.GetActiveSize() > 0 )
    {
		uint8 cmd = packet.GetReadPointer()[0];
        auto itr = Handlers.find(cmd);
        if (itr == Handlers.end())
        {
            packet.Reset();
            break;
        }

        uint16 size = itr->second.packetSize;
        if (packet.GetActiveSize() < size)
            break;

		CUSTOM_PACKET_HEAD * head = (CUSTOM_PACKET_HEAD*)packet.GetReadPointer();

		sConnectServer->AddPacket(new WorldPacket(cmd, packet.GetReadPointer(), head->size));	

		packet.ReadCompleted(head->size);
    }
}

void ConnectServer::HandleHeadcodeOnConnect(uint8 * Packet)
{

}

void ConnectServer::HandleHeadcodeFlag(uint8 * Packet)
{
	POINTER_PCT(CS_GAMESERVER_FLAG, lpMsg, Packet, 0);

	sGameServer->SetFlag(lpMsg->flag);
}

void ConnectServer::HandleHeadcodeChannel(uint8 * Packet)
{
	MAP_CLEAR(ChannelDataMap::iterator, this->m_ChannelDataMap);

	POINTER_PCT(CS_CHANNEL_HEAD, lpHead, Packet, 0);

	if (lpHead->count <= 0)
	{
		return;
	}

	POINTER_PCT(CS_CHANNEL_BODY, lpBody, Packet, sizeof(CS_CHANNEL_HEAD));
	
	for (int32 i = 0; i < lpHead->count; ++i)
	{
		ChannelData* pData = new ChannelData();
		pData->SetServer(lpBody[i].server);
		pData->SetData1(lpBody[i].data1);
		pData->SetData2(lpBody[i].data2);
		pData->SetType(lpBody[i].type);
		pData->SetGold(lpBody[i].gold);

		this->m_ChannelDataMap[pData->GetServer()] = pData;
	}
}

void ConnectServer::SendData()
{
	CS_GAMESERVER_CONNECT pMsg(sGameServer->GetServerCode());
	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void ConnectServer::SendPlayerPercent()
{
	CS_GAMESERVER_USER_PERCENT pMsg(sObjectMgr->GetPlayerPercent());
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.active = sGameServer->IsActiveStatus();

	if ( sGameServer->IsConnectionDisabled() )
	{
		pMsg.active = false;
	}

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void ConnectServer::HandlePacketData(uint8 headcode, uint8 * packet)
{
    auto itr = Handlers.find(headcode);
    if (itr == Handlers.end())
        return;

	(*sConnectServer.*Handlers.at(headcode).handler)(packet);
}

bool ConnectServer::IsServerAvailable(uint16 server) const
{
	ChannelDataMap::const_iterator itr = this->m_ChannelDataMap.find(server);

	if (itr != this->m_ChannelDataMap.end())
	{
		return true;
	}

	return false;
}