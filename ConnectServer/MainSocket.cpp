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

using boost::asio::ip::tcp;

MainSocket::MainSocket(tcp::socket&& socket): Socket(std::move(socket))
{
	this->GetTime()->Start();
}

struct PMSG_CONNECT
{
	PMSG_CONNECT(uint8 result)
	{
		this->h.set(0x00, sizeof(PMSG_CONNECT));
		this->result = result;
	}

	PBMSG_HEAD h;
	uint8 result;
};

void MainSocket::Start()
{
	PMSG_CONNECT pMsg(1);
	this->SendPacket((uint8*)&pMsg, pMsg.h.size);

	if ( !sMainSocketMgr.ConnectIPAllowed(shared_from_this()) )
	{
		CloseSocket();
		return;
	}

	if ( sMainSocketMgr.GetConnectiounsCount() >= sMain->GetMaxConnectionCount() )
	{
		CloseSocket();
		return;
	}

	if ( !sConfig->GetBoolDefault("Network.ConnectionEnabled", true) )
	{
		CloseSocket();
		return;
	}

	AsyncRead();
}

bool MainSocket::Update()
{
	if ( !Socket::Update() )
		return false;

	if ( this->GetTime()->Elapsed(sMain->GetMaxConnectionIdle()) )
		return false;

	return true;
}

void MainSocket::ReadHandler()
{
    if (!IsOpen())
        return;

	uint8 headcode = 0;
	uint16 size = 0;
		
	MessageBuffer& packet = GetReadBuffer();

    while ( packet.GetActiveSize() > 0 )
    {
		if ( packet.GetReadPointer()[0] == 0xC1 || packet.GetReadPointer()[0] == 0xC3 )
		{
			size = packet.GetReadPointer()[1];
			headcode = packet.GetReadPointer()[2];
		}
		else if ( packet.GetReadPointer()[0] == 0xC2 || packet.GetReadPointer()[0] == 0xC4 )
		{
			size = packet.GetReadPointer()[1] * 256 + packet.GetReadPointer()[2];
			headcode = packet.GetReadPointer()[3];
		}

		if ( size < 3 )
		{
			sLog->outError("root", "%s -- [%s] Size less that 3", __FUNCTION__, this->GetRemoteIpAddress().to_string().c_str());

			this->CloseSocket();
			return;
		}

		if ( size > packet.GetActiveSize() )
		{
			sLog->outError("root", "%s -- [%s] Size [%u] highter than packet size [%u]", __FUNCTION__, this->GetRemoteIpAddress().to_string().c_str(), size, packet.GetActiveSize());

			this->CloseSocket();
			return;
		}

		this->PrococolCore(packet.GetReadPointer(), size, headcode);

		packet.ReadCompleted(size);
    }

    AsyncRead();
}

void MainSocket::SendPacket(uint8 * packet, uint16 size)
{
	this->QueuePacket(packet, size);
}

void MainSocket::PrococolCore(uint8 * Packet, uint16 size, uint8 headcode)
{
	uint8 subcode = 0;

	if ( size > 3 )
	{
		subcode = Packet[3];
	}

	sLog->outPacket("root", Packet, size, "ProtocolCore: ");

	switch ( headcode )
	{
	case HEADCODE_CONNECT_SERVER:
		{
			switch ( subcode )
			{
			case SUBCODE_CONNECT_SERVER_SERVER_SELECT:	// Server Select
				{
					sServer->ServerSelectRequest(shared_from_this(), Packet);
				} break;

			case SUBCODE_CONNECT_SERVER_SERVER_LIST:	// Client Connect
				{
					sServer->ServerListRequest(shared_from_this());
				} break;
			}
		} break;

	case 0x0A:
		{
			sServer->ServerVersionCheck(shared_from_this(), Packet);
		} break;

	case 0x0C:
		{
			sServer->LauncherError(shared_from_this(), Packet);
		} break;
	}
}