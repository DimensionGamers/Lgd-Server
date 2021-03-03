std::unordered_map<uint8, LoginHandler<AuthServer>> AuthServer::InitHandlers()
{
    std::unordered_map<uint8, LoginHandler<AuthServer>> handlers;

	handlers[HEADCODE_LOGIN_SERVER_CONNECT]					= { "ON_CONNECT", sizeof(GAMESERVER_CONNECT), &AuthServer::HandleHeadcodeOnConnect };
    handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_LOGIN]			= { "ACCOUNT_LOGIN", sizeof(ACCOUNT_LOGIN_RESULT), &AuthServer::PlayerLoginResult };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_MOVE]		= { "ACCOUNT_SERVER_MOVE", sizeof(ACCOUNT_SERVER_MOVE_RESULT), &AuthServer::PlayerServerMoveResult };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_AUTH]		= { "ACCOUNT_SERVER_AUTH", sizeof(ACCOUNT_SERVER_MOVE_AUTH_RESULT), &AuthServer::PlayerServerMoveAuthResult };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_KICK]			= { "ACCOUNT_KICK", sizeof(ACCOUNT_KICK), &AuthServer::PlayerAccountKick };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_ONLINE]			= { "ACCOUNT_ONLINE", sizeof(ACCOUNT_ONLINE), &AuthServer::PlayerAccountOnline };

	handlers[HEADCODE_LOGIN_SERVER_COMPLETE_BAN]			= { "COMPLETE_BAN", sizeof(ACCOUNT_COMPLETE_BAN), &AuthServer::AccountCompleteBan };

	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_DUPLICATE] = { "ACCOUNT_DUPLICATE", sizeof(SL_ACCOUNT_DUPLICATE), &AuthServer::AccountDuplicate };
	
    return handlers;
}

std::unordered_map<uint8, LoginHandler<AuthServer>> const Handlers = AuthServer::InitHandlers();

AuthServer::AuthServer(): TCPClientMgr("AuthServer")
{
	
}

void AuthServerSocket::OnStart()
{
	sLog->outInfo("root", "CONNECTED AUTHSERVER");

	sAuthServer->IncreaseConnectCount(1);
	sAuthServer->SetStopped(false);

	sAuthServer->SendData();
}

void AuthServerSocket::OnStop()
{
	sLog->outInfo("root", "DISCONNECTED AUTHSERVER");

	sAuthServer->GetReconnectTime()->Start();
	sAuthServer->SetStopped(true);
}

void AuthServerSocket::ReadHandler()
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

		sAuthServer->AddPacket(new WorldPacket(cmd, packet.GetReadPointer(), head->size));	

		packet.ReadCompleted(head->size);
    }
}

void AuthServer::HandleHeadcodeOnConnect(uint8 * Packet)
{
	PlayerMap const& account_map = sObjectMgr->GetAllAccounts();
	for ( PlayerMap::const_iterator it = account_map.begin(); it != account_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetConnectStatus() == CONNECT_STATUS_LOGGED ||
			 pPlayer->GetConnectStatus() == CONNECT_STATUS_PLAYING )
		{
			ACCOUNT_RECONNECT pMsg;
			pMsg.h.server = sGameServer->GetServerCode();
			pMsg.account_id = pPlayer->GetAccountData()->GetGUID();
			pMsg.start_server = pPlayer->GetStartServer();
			memcpy(pMsg.account, pPlayer->GetAccountData()->GetAccount(), MAX_ACCOUNT_LENGTH);
			memcpy(pMsg.mac, pPlayer->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH);
			memcpy(pMsg.ip, pPlayer->GetAccountData()->GetIP(), MAX_ACCOUNT_IP_LENGTH);
			pMsg.facebook_status = pPlayer->GetAccountData()->GetFacebookStatus();
			pMsg.golden_channel = pPlayer->GetAccountData()->GetGoldenChannel();
			pMsg.disk_serial = pPlayer->GetAccountData()->GetDiskSerial();
			pMsg.authorized = pPlayer->IsAuthorizationEnabled();
				
			this->SendPacket(MAKE_PCT(pMsg));
		}
	}
}

void AuthServer::SendData()
{
	GAMESERVER_CONNECT pMsg(sGameServer->GetServerCode());
	pMsg.port = sGameServer->GetNetworkPort();
	memcpy(pMsg.ip, sGameServer->GetNetworkIP(), 16);
	pMsg.flag = sGameServer->GetFlag();
	pMsg.type = sGameServer->GetServerMode();
	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void AuthServer::SendPlayerPercent()
{
	GAMESERVER_USER_PERCENT pMsg(sObjectMgr->GetPlayerPercent(true), sGameServer->GetFlag(), sGameServer->GetServerMode());
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void AuthServer::PlayerLoginRequest(Player* pPlayer, PlayerLoginStep step)
{
	if ( this->IsStopped() )
	{
		pPlayer->SetAutoLogin(false);
		pPlayer->CloseSocket();
		return;
	}

	switch ( step )
	{
	case PLAYER_LOGIN_STEP_ACCOUNT:
		{
		} break;

	case PLAYER_LOGIN_STEP_CHARACTER:
		{
		} break;

	case PLAYER_LOGIN_STEP_OK:
		{
			ACCOUNT_LOGIN_REQUEST pMsg(pPlayer->GetEntry());
			pMsg.h.server = sGameServer->GetServerCode();
			pMsg.ms_time = pPlayer->GetMSTime();
			memcpy(pMsg.account, pPlayer->GetAccountSafe(), MAX_ACCOUNT_LENGTH + 1);
			memcpy(pMsg.password, pPlayer->GetPasswordSafe(), MAX_PASSWORD_LENGTH + 1);
			memcpy(pMsg.ip, pPlayer->GetAccountData()->GetIP(), MAX_ACCOUNT_IP_LENGTH + 1);
			memcpy(pMsg.mac, pPlayer->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH + 1);
			pMsg.disk_serial = pPlayer->GetAccountData()->GetDiskSerial();
			pMsg.account_id = pPlayer->GetAutoLoginAccount();
			pMsg.autologin = pPlayer->IsAutoLogin() ? 1 : 0;
			this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		} break;
	}
}
	
void AuthServer::PlayerLoginResult(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_LOGIN_RESULT, lpMsg, Packet, 0);

	if (lpMsg->h.server != sGameServer->GetServerCode())
	{
		return;
	}

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( !pPlayer )
	{
		if ( lpMsg->result == LOGIN_RESULT_SUCCESS )
		{
			this->PlayerLogout(lpMsg->account_id);
		}
		return;
	}

	if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_CONNECTED )
	{
		if ( lpMsg->result == LOGIN_RESULT_SUCCESS )
		{
			this->PlayerLogout(lpMsg->account_id);
		}
		return;
	}

	if ( pPlayer->GetMSTime() != lpMsg->ms_time ) // No permito que la cuenta loguee porque tienen diferente msec de inicio
	{
		if ( lpMsg->result == LOGIN_RESULT_SUCCESS )
		{
			this->PlayerLogout(lpMsg->account_id);
		}
		return;
	}
	
	pPlayer->SetLoginSent(false);

	if ( lpMsg->result != LOGIN_RESULT_SUCCESS )
	{
		pPlayer->SetAutoLogin(false);
		pPlayer->LoginResult(lpMsg->result);
		return;
	}

	pPlayer->GetAccountData()->SetGUID(lpMsg->account_id);

	memcpy(pPlayer->GetAccountData()->secure_code, lpMsg->security_code, MAX_SECURE_CODE_LENGTH + 1);

	pPlayer->GetAccountData()->SetAccount(pPlayer->GetAccountSafe());
	pPlayer->GetAccountData()->SetFacebookStatus(lpMsg->facebook_status);
	pPlayer->GetAccountData()->SetGoldenChannel(lpMsg->golden_channel);

	if ( pPlayer->IsAutoLogin() )
	{
		pPlayer->GetAccountData()->SetAccount(lpMsg->account);
	}

	pPlayer->SetConnectStatus(CONNECT_STATUS_LOGGED);
	sObjectMgr->AddSessionAccount(pPlayer);

	pPlayer->LoginResult(LOGIN_RESULT_SUCCESS);

	pPlayer->UpdateAccountStatusType(0);
	pPlayer->SetAuthorizationEnabled(lpMsg->authorized);

	sLog->outInfo(LOG_PLAYER, "[ LOGIN REQUEST ] Success %s", pPlayer->BuildLog().c_str());

	if ( pPlayer->IsAutoLogin() )
	{
		pPlayer->AutoLoginCharacterSelect();
	}

	//pPlayer->WhiteListRequest();
}

void AuthServer::PlayerLogout(uint32 account_id)
{
	ACCOUNT_LOGOUT pMsg(account_id);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void AuthServer::PlayerServerMoveRequest(Player* pPlayer, uint16 server, uint16 world, int16 x, int16 y, bool channel)
{
	if ( !pPlayer )
		return;

	if ( sGameServer->IsServerWarpTransactionCheck1() )
	{
		if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
		{
			sLog->outError(LOG_PLAYER, "[ SERVER MOVE REQUEST ] Active Interface %s // %s --- Server: %d [%d - %d/%d]", pPlayer->GetInterfaceState()->GetID().c_str(), pPlayer->BuildLog().c_str(),
				server, world, x, y);

			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Close interface before warp.");
			return;
		}
	}

	if ( sGameServer->IsServerWarpTransactionCheck2() )
	{
		if ( pPlayer->GetTransaction() == TRANSACTION_TYPE_BEGIN )
		{
			sLog->outError(LOG_PLAYER, "[ SERVER MOVE REQUEST ] Active Transaction // %s --- Server: %d [%d - %d/%d]", pPlayer->BuildLog().c_str(),
				server, world, x, y);

			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong game interface. Close client and try again.");
			return;
		}
	}

	if ( sGameServer->IsServerWarpTransactionCheck3() )
	{
		if ( pPlayer->GetPersonalStore()->IsBusy() )
		{
			sLog->outError(LOG_PLAYER, "[ SERVER MOVE REQUEST ] Personal Store Open // %s --- Server: %d [%d - %d/%d]", pPlayer->BuildLog().c_str(),
				server, world, x, y);

			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Close Personal Store before warp.");
			return;
		}
	}

	pPlayer->SetDestServer(server);
	pPlayer->SetDestWorld(world);
	pPlayer->SetDestX(x);
	pPlayer->SetDestY(y);

	ACCOUNT_SERVER_MOVE_REQUEST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	memcpy(pMsg.data.account, pPlayer->GetAccountData()->GetAccount(), MAX_ACCOUNT_LENGTH + 1);
	pMsg.data.account_id = pPlayer->GetAccountData()->GetGUID();
	pMsg.data.cur_server = sGameServer->GetServerCode();
	pMsg.data.dest_server = pPlayer->GetDestServer();
	pMsg.data.dest_world = pPlayer->GetDestWorld();
	pMsg.data.dest_x = pPlayer->GetDestX();
	pMsg.data.dest_y = pPlayer->GetDestY();
	pMsg.channel_change = channel ? 1 : 0;

	sLog->outInfo(LOG_PLAYER, "[ SERVER MOVE REQUEST ] %s --- Server: %d [%d - %d/%d]", pPlayer->BuildLog().c_str(),
		server, world, x, y);

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void AuthServer::PlayerServerMoveResult(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_SERVER_MOVE_RESULT, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
	{
		return;
	}

	Player* pPlayer = sObjectMgr->FindPlayerByAccount(lpMsg->data.account_id);

	if ( !pPlayer || !pPlayer->IsPlaying() )
	{
		return;
	}

	if ( lpMsg->result == 0 )
	{
		pPlayer->SaveCharacter(1);
		pPlayer->SetDestAuth(0, lpMsg->auth[0]);
		pPlayer->SetDestAuth(1, lpMsg->auth[1]);
		pPlayer->SetDestAuth(2, lpMsg->auth[2]);
		pPlayer->SetDestAuth(3, lpMsg->auth[3]);
		pPlayer->SetDestServer(lpMsg->data.dest_server);
		pPlayer->SetDestPort(lpMsg->dest_port);
		pPlayer->SetDestIP(lpMsg->dest_ip);
		pPlayer->InitCloseSocketTime(CLOSE_SOCKET_TIME_SERVER_QUIT);
		pPlayer->SetServerQuit(true);
	}
	else
	{
		if ( pPlayer->IsServerMoveFromCharacterSelect() )
		{
			pPlayer->SetWorldId(sGameServer->GetDefaultWorld());
			pPlayer->SetX(sGameServer->GetDefaultX());
			pPlayer->SetY(sGameServer->GetDefaultY());
			pPlayer->SetTX(pPlayer->GetX());
			pPlayer->SetTY(pPlayer->GetY());
			pPlayer->GetACPathData()->Reset();

			pPlayer->SendJoinInfo(0);
			pPlayer->SetServerMove(false);
			pPlayer->SetServerMoveFromCharacterSelect(false);
			pPlayer->SendJoinInfo();

			pPlayer->SendWeather();
			pPlayer->RestoreEventAnimation();
		}
		else
		{
			if ( lpMsg->result == 5 )
			{
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Server is full.");
			}
			else
			{
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "This location is unavailable.");
			}
		}

		sLog->outInfo(LOG_PLAYER, "[ SERVER MOVE ERROR %d ] %s --- Server: %d [%d - %d/%d]", lpMsg->result, pPlayer->BuildLog().c_str(),
			pPlayer->GetDestServer(), pPlayer->GetDestWorld(), pPlayer->GetDestX(), pPlayer->GetDestY());
	}
}

void AuthServer::PlayerServerMoveAuth(Player* pPlayer, uint8 * Packet)
{
	POINTER_PCT(SERVER_CHANGE_AUTH, lpMsg, Packet, 0);

	if ( memcmp(lpMsg->version, sGameServer->GetGameServerVersions(), 5) )
	{
		sLog->outError(LOG_PLAYER, "[ SERVER MOVE AUTH ] %s Wrong Version [%d/%d/%d/%d/%d] / [%d/%d/%d/%d/%d]", pPlayer->BuildLog().c_str(),
			lpMsg->version[0], lpMsg->version[1], lpMsg->version[2], lpMsg->version[3], lpMsg->version[4],
			sGameServer->GetGameServerVersion(0), sGameServer->GetGameServerVersion(1), sGameServer->GetGameServerVersion(2), 
			sGameServer->GetGameServerVersion(3), sGameServer->GetGameServerVersion(4));

		this->PlayerServerMoveAuthResult(pPlayer, 6);
		pPlayer->CloseSocket();
		return;
	}

	char client_serial_tmp[17];
	memset(client_serial_tmp, 0, 17);
	memcpy(client_serial_tmp, lpMsg->serial, 16);
	
	if ( memcmp(client_serial_tmp, sGameServer->GetGameServerSerial(), 16) )
	{
		sLog->outError(LOG_PLAYER, "[ SERVER MOVE AUTH ] %s Wrong Serial [%s / %s]", pPlayer->BuildLog().c_str(),
			 client_serial_tmp, sGameServer->GetGameServerSerial());

		this->PlayerServerMoveAuthResult(pPlayer, 6);
		pPlayer->CloseSocket();
		return;
	}

	if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_CONNECTED )
	{
		sLog->outError(LOG_PLAYER, "[ SERVER MOVE AUTH ] %s --- Not Connected", pPlayer->BuildLog().c_str());

		this->PlayerServerMoveAuthResult(pPlayer, 6);
		pPlayer->CloseSocket();
		return;
	}

	if ( pPlayer->IsLoginSent() )
		return;

	STRING_SAFE_COPY(account_temp, MAX_ACCOUNT_LENGTH + 1, lpMsg->account, MAX_ACCOUNT_LENGTH);
	Xor::Byte3(account_temp, MAX_ACCOUNT_LENGTH);
	STRING_SAFE(account, MAX_ACCOUNT_LENGTH + 1);
	strcpy(account, account_temp);

	StringToLower(account);

	pPlayer->SetLoginSent(true);
	pPlayer->SetServerMove(true);
	pPlayer->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);
	pPlayer->GetAccountData()->IncreaseLoginAttempts(1);
	pPlayer->GetAccountData()->ResetAccount();
	pPlayer->GetAccountData()->SetAccount(account, MAX_ACCOUNT_LENGTH);
	pPlayer->GetAccountData()->SetAccountToLower();
	pPlayer->GetAccountData()->SetGameServerSerial((const char*)lpMsg->serial, 16);
	pPlayer->ResetName();
	pPlayer->SetName(lpMsg->name);

	sLog->outInfo(LOG_PLAYER, "[ SERVER MOVE AUTH ] %s --- Send Join Request [%s][%s]", pPlayer->BuildLog().c_str(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());

	ACCOUNT_SERVER_MOVE_AUTH pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.index = pPlayer->GetEntry();
	memcpy(pMsg.account, account, MAX_ACCOUNT_LENGTH);
	memcpy(pMsg.name, lpMsg->name, MAX_CHARACTER_LENGTH);
	pMsg.auth[0] = lpMsg->auth[0];
	pMsg.auth[1] = lpMsg->auth[1];
	pMsg.auth[2] = lpMsg->auth[2];
	pMsg.auth[3] = lpMsg->auth[3];

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void AuthServer::PlayerServerMoveAuthResult(Player* pPlayer, uint8 result)
{
	SERVER_CHANGE_AUTH_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void AuthServer::PlayerServerMoveAuthResult(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_SERVER_MOVE_AUTH_RESULT, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
	{
		return;
	}

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->index);

	if ( !pPlayer )
	{
		if ( lpMsg->result == 0 )
		{
			this->PlayerLogout(lpMsg->account_id);
		}
		return;
	}

	if ( memcmp(pPlayer->GetAccountData()->GetAccount(), lpMsg->account, MAX_ACCOUNT_LENGTH) )
	{
		if ( lpMsg->result == 0 )
		{
			this->PlayerLogout(lpMsg->account_id);
		}
		return;
	}

	if ( !pPlayer->IsServerMove() || !pPlayer->IsLoginSent() )
	{
		sLog->outError(LOG_PLAYER, "[ SERVER MOVE AUTH RESULT ] %s --- No request [%s][%s]", 
			pPlayer->BuildLog().c_str(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());

		pPlayer->CloseSocket();
		return;
	}

	if ( lpMsg->result )
	{
		sLog->outError(LOG_PLAYER, "[ SERVER MOVE AUTH RESULT ] %s --- Failed(%d) [%s][%s]", 
			pPlayer->BuildLog().c_str(), lpMsg->result, pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());

		this->PlayerServerMoveAuthResult(pPlayer, 0);
		pPlayer->CloseSocket();
		return;
	}

	pPlayer->GetAccountData()->SetGUID(lpMsg->account_id);
	memcpy(pPlayer->GetAccountData()->secure_code, lpMsg->security_code, MAX_SECURE_CODE_LENGTH + 1);
	pPlayer->GetAccountData()->SetFacebookStatus(lpMsg->facebook_status);
	pPlayer->GetAccountData()->SetGoldenChannel(lpMsg->golden_channel);
	pPlayer->GetAccountData()->SetMac(lpMsg->mac, MAX_ACCOUNT_MAC_LENGTH);
	pPlayer->GetAccountData()->SetDiskSerial(lpMsg->disk_serial);
	pPlayer->SetConnectStatus(CONNECT_STATUS_LOGGED);
	sObjectMgr->AddSessionAccount(pPlayer);

	pPlayer->SetDestWorld(lpMsg->world);
	pPlayer->SetDestX(lpMsg->x);
	pPlayer->SetDestY(lpMsg->y);
	pPlayer->SetStartServer(lpMsg->start_server);
	pPlayer->SetLoginSent(false);
	pPlayer->SetAuthorizationEnabled(lpMsg->authorized);

	this->PlayerServerMoveAuthResult(pPlayer, 1);

	if ( sGameServer->IsArkaWarEnabled() )
	{
		sArkaWar->SendPlayInfo(pPlayer);
		sArkaWar->SendState(pPlayer);
	}

	pPlayer->UpdateAccountStatusType(0);

	sLog->outInfo(LOG_PLAYER, "[ SERVER MOVE AUTH RESULT ] %s --- Joined Successfully [%s][%s]", 
			pPlayer->BuildLog().c_str(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());

	//pPlayer->WhiteListRequest();

	pPlayer->SendClearBuff();

	CHARACTER_SELECT_REQUEST pMsg;
	memcpy(pMsg.character_name, lpMsg->name, MAX_CHARACTER_LENGTH);
	pMsg.index = 0;

	pPlayer->CharacterSelect((uint8*)&pMsg, true);
}

void AuthServer::PlayerAccountKick(uint32 account_id)
{
	ACCOUNT_KICK pMsg(account_id);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}

void AuthServer::PlayerAccountKick(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_KICK, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayerByAccount(lpMsg->account_id);

	if ( pPlayer )
	{
		KICK_PLAYER(pPlayer, "Administrator Account Kick");
		pPlayer->GetPersonalStore()->Clear();
		pPlayer->CloseSocket();
	}
}

void AuthServer::PlayerAccountOnline(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_ONLINE, lpMsg, Packet, 0);

	sObjectMgr->AccountOnlineCheck(lpMsg->account);
	sObjectMgr->PersonalStoreDisconnect(lpMsg->account);
}

void AuthServer::AccountCompleteBan(const char * ip, const char * mac, uint32 disk_serial, uint8 type)
{
	if ( !sGameServer->IsDupeCompleteBan() )
	{
		return;
	}

	ACCOUNT_COMPLETE_BAN pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	memcpy(pMsg.ip, ip, MAX_ACCOUNT_IP_LENGTH);
	memcpy(pMsg.mac, mac, MAX_ACCOUNT_MAC_LENGTH);
	pMsg.disk_serial = disk_serial;
	pMsg.type = type;
	this->SendPacket(MAKE_PCT(pMsg));
}

void AuthServer::AccountCompleteBan(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_COMPLETE_BAN, lpMsg, Packet, 0);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !memcmp(pPlayer->GetAccountData()->GetIP(), lpMsg->ip, MAX_ACCOUNT_IP_LENGTH) ||
			 !memcmp(pPlayer->GetAccountData()->GetMac(), lpMsg->mac, MAX_ACCOUNT_MAC_LENGTH) ||
			 pPlayer->GetAccountData()->GetDiskSerial() == lpMsg->disk_serial )
		{
			KICK_PLAYER(pPlayer, std::string("Automatic BAN" + std::to_string(lpMsg->type)));
		}
	}
}

void AuthServer::AccountDuplicate(uint8 * Packet)
{
	POINTER_PCT(SL_ACCOUNT_DUPLICATE, lpMsg, Packet, 0);

	sObjectMgr->KickCharactersByAccount(lpMsg->account_id);
}

void AuthServer::AccountAuthorization(uint32 account_id, bool warning)
{
	SL_ACCOUNT_VALIDATE pMsg(account_id, warning);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}

void AuthServer::HandlePacketData(uint8 headcode, uint8 * packet)
{
    auto itr = Handlers.find(headcode);
    if (itr == Handlers.end())
        return;

	(*sAuthServer.*Handlers.at(headcode).handler)(packet);
}