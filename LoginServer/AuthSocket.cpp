/*
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/asio/basic_socket.hpp>

std::unordered_map<uint8, LoginHandler<AuthSocket>> AuthSocket::InitHandlers()
{
    std::unordered_map<uint8, LoginHandler<AuthSocket>> handlers;

	handlers[HEADCODE_LOGIN_SERVER_CONNECT]						= { "CONNECT", sizeof(GAMESERVER_CONNECT), &AuthSocket::GameServerConnect };
	handlers[HEADCODE_LOGIN_SERVER_PERCENT]						= { "PERCENT", sizeof(GAMESERVER_USER_PERCENT), &AuthSocket::GameServerPercent };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_RECONNECT]			= { "ACCOUNT_RECONNECT", sizeof(ACCOUNT_RECONNECT), &AuthSocket::AccountReconnect };
    handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_LOGIN]				= { "ACCOUNT_LOGIN", sizeof(ACCOUNT_LOGIN_REQUEST), &AuthSocket::AccountLoginRequest };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_LOGOUT]				= { "ACCOUNT_LOGOUT", sizeof(ACCOUNT_LOGOUT), &AuthSocket::AccountLogout };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_MOVE]			= { "ACCOUNT_SERVER_MOVE", sizeof(ACCOUNT_SERVER_MOVE_REQUEST), &AuthSocket::AccountServerMove };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_SERVER_AUTH]			= { "ACCOUNT_SERVER_AUTH", sizeof(ACCOUNT_SERVER_MOVE_AUTH), &AuthSocket::AccountServerAuth };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_KICK]				= { "ACCOUNT_KICK", sizeof(ACCOUNT_KICK), &AuthSocket::AccountKick };

	handlers[HEADCODE_LOGIN_SERVER_COMPLETE_BAN]				= { "COMPLETE_BAN", sizeof(ACCOUNT_COMPLETE_BAN), &AuthSocket::AccountCompleteBan };
	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_VALIDATE]			= { "ACCOUNT_VALIDATE", sizeof(SL_ACCOUNT_VALIDATE), &AuthSocket::AccountValidate };

	handlers[HEADCODE_LOGIN_SERVER_ACCOUNT_TYPE]				= { "ACCOUNT_TYPE", sizeof(SL_ACCOUNT_CONNECT_TYPE), &AuthSocket::AccountConnectType };

    return handlers;
}

std::unordered_map<uint8, LoginHandler<AuthSocket>> const Handlers = AuthSocket::InitHandlers();

AuthSocket::AuthSocket(tcp::socket&& socket): Socket(std::move(socket))
{
	this->SetServerCode(-1);
}

void AuthSocket::Start()
{
    AsyncRead();
}

void AuthSocket::ReadHandler()
{
	if (!IsOpen())
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

		(*this.*Handlers.at(cmd).handler)(packet.GetReadPointer());

		CUSTOM_PACKET_HEAD * head = (CUSTOM_PACKET_HEAD*)packet.GetReadPointer();
		packet.ReadCompleted(head->size);
    }

    AsyncRead();
}

void AuthSocket::OnEnd()
{
	sServer->ServerClose(this->GetServerCode());
}

void AuthSocket::SendPacket(uint8 * packet, uint16 size)
{
	this->QueuePacket(packet, size);
}

void AuthSocket::GameServerConnect(uint8 * Packet)
{
	POINTER_PCT(GAMESERVER_CONNECT, lpMsg, Packet, 0);

	this->SetServerCode(lpMsg->h.server);

	sServer->ServerOpen(lpMsg->h.server, lpMsg->port, lpMsg->ip, lpMsg->flag, lpMsg->type);

	GAMESERVER_CONNECT pMsg(lpMsg->h.server);
	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void AuthSocket::GameServerPercent(uint8 * Packet)
{
	POINTER_PCT(GAMESERVER_USER_PERCENT, lpMsg, Packet, 0);

	sServer->ServerSetInfo(lpMsg->h.server, lpMsg->percent, lpMsg->flag, lpMsg->type);
}

void AuthSocket::AccountReconnect(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_RECONNECT, lpMsg, Packet, 0);

	sLoginQueue->ConnectAccount(lpMsg->start_server, lpMsg->h.server, lpMsg->account, lpMsg->account_id, lpMsg->security_code, lpMsg->ip, lpMsg->mac, lpMsg->facebook_status, lpMsg->golden_channel, lpMsg->disk_serial, lpMsg->authorized);
}

void AuthSocket::AccountLoginRequest(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_LOGIN_REQUEST, lpMsg, Packet, 0);

	LoginDataPtr add_data(new LoginData(lpMsg->entry, lpMsg->account, lpMsg->password));
	memcpy(add_data->mac, lpMsg->mac, MAX_ACCOUNT_MAC_LENGTH + 1);
	memcpy(add_data->ip, lpMsg->ip, MAX_ACCOUNT_IP_LENGTH + 1);
	add_data->ms_time = lpMsg->ms_time;
	add_data->disk_serial = lpMsg->disk_serial;
	add_data->account_id = lpMsg->account_id;
	add_data->autologin = lpMsg->autologin;
	add_data->server = lpMsg->h.server;
	add_data->socket = shared_from_this();
	
	sLoginQueue->Queue(add_data);
}

void AuthSocket::AccountLogout(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_LOGOUT, lpMsg, Packet, 0);

	sLoginQueue->DisconnectAccount(lpMsg->h.server, lpMsg->account_id);
}

void AuthSocket::AccountServerMove(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_SERVER_MOVE_REQUEST, lpMsg, Packet, 0);

	ACCOUNT_SERVER_MOVE_RESULT pMsg;
	pMsg.h.server = lpMsg->h.server;
	memcpy(&pMsg.data, &lpMsg->data, sizeof(ACCOUNT_SERVER_MOVE_DATA));
	pMsg.auth[0] = Random(1000);
	pMsg.auth[1] = 1000 + Random(1000);
	pMsg.auth[2] = 2000 + Random(1000);
	pMsg.auth[3] = 3000 + Random(1000);

	AccountDataPtr pAccountData = sLoginQueue->GetAccountData(lpMsg->h.GetServerGroup(), lpMsg->data.account_id);

	if ( !pAccountData )
	{
		pMsg.result = 3;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if ( pAccountData->GetCurrentServer() != lpMsg->data.cur_server )
	{
		pMsg.result = 3;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if ( pAccountData->IsMoving() )
	{
		pMsg.result = 4;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if (lpMsg->channel_change == 0)
	{
		lpMsg->data.dest_server = sServer->CheckDestServer(lpMsg->h.GetServerGroup(), lpMsg->data.dest_world, lpMsg->data.dest_server, pAccountData->GetStartServer());
	}

	ServerData & cur_srv = sServer->server_map[lpMsg->data.cur_server];
	ServerData & dest_srv = sServer->server_map[lpMsg->data.dest_server];

	if ( !dest_srv.IsOnline() || dest_srv.IsFlag(SERVER_FLAG_DISABLED) )
	{
		pMsg.result = 2;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if ( !sServer->IsServerOnlineAndFree(lpMsg->data.dest_server) )
	{
		pMsg.result = 5;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	pMsg.dest_port = dest_srv.GetPort();
	memcpy(pMsg.dest_ip, dest_srv.GetIP(), 16);

	pAccountData->SetDestServer(lpMsg->data.dest_server);
	pAccountData->SetDestWorld(lpMsg->data.dest_world);
	pAccountData->SetDestX(lpMsg->data.dest_x);
	pAccountData->SetDestY(lpMsg->data.dest_y);
	pAccountData->SetMoving(true);
	pAccountData->SetMovingTick(getMSTime());
	pAccountData->SetAuth(0, pMsg.auth[0]);
	pAccountData->SetAuth(1, pMsg.auth[1]);
	pAccountData->SetAuth(2, pMsg.auth[2]);
	pAccountData->SetAuth(3, pMsg.auth[3]);
	
	sLoginQueue->DBAccountStatus(pAccountData, true, false);

	sLog->outInfo("player", "%s - [%u][%s] Move To [%u][%u][%d][%d]", __FUNCTION__, lpMsg->data.account_id, lpMsg->data.account, 
		lpMsg->data.dest_server, lpMsg->data.dest_world, lpMsg->data.dest_x, lpMsg->data.dest_y);

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void AuthSocket::AccountServerAuth(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_SERVER_MOVE_AUTH, lpMsg, Packet, 0);

	ACCOUNT_SERVER_MOVE_AUTH_RESULT pMsg;
	pMsg.h.server = lpMsg->h.server;
	pMsg.index = lpMsg->index;
	memcpy(pMsg.account, lpMsg->account, MAX_ACCOUNT_LENGTH + 1);
	memcpy(pMsg.name, lpMsg->name, MAX_CHARACTER_LENGTH + 1);
	
	AccountDataPtr pAccountData = sLoginQueue->GetAccountData(lpMsg->h.GetServerGroup(), lpMsg->account);

	if ( !pAccountData )
	{
		pMsg.result = 1;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if ( pAccountData->IsOffline() || !pAccountData->IsMoving() )
	{
		pMsg.result = 2;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if ( pAccountData->GetCurrentServer() == uint16(-1) ||
		 pAccountData->GetStartServer() == uint16(-1) ||
		 pAccountData->GetDestServer() == -1 ||
		 pAccountData->GetDestServer() != lpMsg->h.server )
	{
		pMsg.result = 3;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if ( pAccountData->GetAuth(0) != lpMsg->auth[0] || 
		 pAccountData->GetAuth(1) != lpMsg->auth[1] || 
		 pAccountData->GetAuth(2) != lpMsg->auth[2] || 
		 pAccountData->GetAuth(3) != lpMsg->auth[3] )
	{
		pMsg.result = 4;
		this->SendPacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	pMsg.account_id = pAccountData->guid;
	pMsg.start_server = pAccountData->GetStartServer();
	pMsg.world = pAccountData->GetDestWorld();
	pMsg.x = pAccountData->GetDestX();
	pMsg.y = pAccountData->GetDestY();
	memcpy(pMsg.security_code, pAccountData->GetSecureCode(), MAX_SECURE_CODE_LENGTH);
	pMsg.facebook_status = pAccountData->GetFacebookStatus();
	pMsg.golden_channel = pAccountData->GetGoldenChannel();
	pMsg.disk_serial = pAccountData->GetDiskSerial();
	memcpy(pMsg.mac, pAccountData->GetMAC(), MAX_ACCOUNT_MAC_LENGTH);
	pMsg.authorized = pAccountData->IsAuthorized();
	
	pAccountData->SetCurrentServer(pAccountData->GetDestServer());
	pAccountData->SetDestServer(-1);
	pAccountData->SetDestWorld(-1);
	pAccountData->SetDestX(-1);
	pAccountData->SetDestY(-1);
	pAccountData->SetMoving(false);
	pAccountData->SetMovingTick(0);
	pAccountData->ResetAuth(0);

	sLoginQueue->DBAccountStatus(pAccountData, true, false);

	sLog->outInfo("player", "%s - [%u][%s] Success Move To [%u][%u][%d][%d]", __FUNCTION__, pMsg.account_id, lpMsg->account, lpMsg->h.server, pMsg.world, pMsg.x, pMsg.y);

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void AuthSocket::AccountKick(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_KICK, lpMsg, Packet, 0);

	sLoginQueue->KickAccount(lpMsg->h.GetServerGroup(), lpMsg->account_id);

	sAuthSocketMgr.SendPacketAll(Packet, lpMsg->h.get_size(), lpMsg->h.GetServerGroup());
}

void AuthSocket::AccountCompleteBan(uint8 * Packet)
{
	POINTER_PCT(ACCOUNT_COMPLETE_BAN, lpMsg, Packet, 0);

	sLoginQueue->CompleteAccountBan(lpMsg->ip, lpMsg->mac, lpMsg->disk_serial, lpMsg->type);

	sAuthSocketMgr.SendPacketAll(Packet, lpMsg->h.get_size(), lpMsg->h.GetServerGroup());
}

void AuthSocket::AccountValidate(uint8 * Packet)
{
	POINTER_PCT(SL_ACCOUNT_VALIDATE, lpMsg, Packet, 0);

	AccountDataPtr pAccountData = sLoginQueue->GetAccountData(lpMsg->h.GetServerGroup(), lpMsg->account_id);

	if ( !pAccountData )
	{
		return;
	}

	if ( lpMsg->warning )
	{
		if ( sLoginQueue->IncreaseWrongAccountAuthorization(lpMsg->account_id, pAccountData->GetDiskSerial()) )
		{
			SQLTransaction trans = LoginDatabase.BeginTransaction();

			PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNTS_WARNING_INSERT);
			stmt->setUInt32(0, pAccountData->guid);
			stmt->setUInt32(1, pAccountData->GetDiskSerial());
			stmt->setInt64(2, time(nullptr));
			trans->Append(stmt);

			LoginDatabase.CommitTransaction(trans);

			ACCOUNT_KICK pMsg(lpMsg->account_id);
			pMsg.h.server = lpMsg->h.server;
			this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
		}
	}
	else
	{
		SQLTransaction trans = LoginDatabase.BeginTransaction();

		PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNTS_VALIDATION_INSERT);
		stmt->setUInt32(0, pAccountData->guid);
		stmt->setUInt32(1, pAccountData->GetDiskSerial());
		trans->Append(stmt);

		stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNTS_WARNING_DELETE);
		stmt->setUInt32(0, pAccountData->guid);
		stmt->setUInt32(1, pAccountData->GetDiskSerial());
		trans->Append(stmt);

		LoginDatabase.CommitTransaction(trans);

		pAccountData->SetAuthorized(true);
	}
}

void AuthSocket::AccountConnectType(uint8 * Packet)
{
	POINTER_PCT(SL_ACCOUNT_CONNECT_TYPE, lpMsg, Packet, 0);

	SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNT_TYPE_UPDATE);
	stmt->setUInt8(0, lpMsg->type);
	stmt->setUInt32(1, lpMsg->account_id);
	stmt->setUInt16(2, lpMsg->h.GetServerGroup());
	trans->Append(stmt);

	LoginDatabase.CommitTransaction(trans);
}