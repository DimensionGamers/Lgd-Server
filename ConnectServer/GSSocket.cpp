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

std::unordered_map<uint8, LoginHandler<GSSocket>> GSSocket::InitHandlers()
{
    std::unordered_map<uint8, LoginHandler<GSSocket>> handlers;

	handlers[HEADCODE_CONNECT_SERVER_CONNECT]					= { "CONNECT", sizeof(CS_GAMESERVER_CONNECT), &GSSocket::GameServerConnect };
	handlers[HEADCODE_CONNECT_SERVER_USER_PERCENT]				= { "USER_PERCENT", sizeof(CS_GAMESERVER_USER_PERCENT), &GSSocket::GameServerUserPercent };
			
    return handlers;
}

std::unordered_map<uint8, LoginHandler<GSSocket>> const Handlers = GSSocket::InitHandlers();

GSSocket::GSSocket(tcp::socket&& socket): Socket(std::move(socket))
{
	this->SetServerCode(-1);
}

void GSSocket::Start()
{
    AsyncRead();
}

void GSSocket::ReadHandler()
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

void GSSocket::OnEnd()
{
	sServer->ServerClose(this->GetServerCode());
}

void GSSocket::SendPacket(uint8 * packet, uint16 size)
{
    this->QueuePacket(packet, size);
}

void GSSocket::GameServerConnect(uint8 * Packet)
{
	POINTER_PCT(CS_GAMESERVER_CONNECT, lpMsg, Packet, 0);

	this->SetServerCode(lpMsg->h.server);

	sServer->ServerConnect(this->GetServerCode());

	CS_GAMESERVER_CONNECT pMsg(lpMsg->h.server);
	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void GSSocket::GameServerUserPercent(uint8 * Packet)
{
	sServer->ServerSetInfo(shared_from_this(), Packet);
}