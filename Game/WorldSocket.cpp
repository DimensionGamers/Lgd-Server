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

WorldSocket::WorldSocket(tcp::socket&& socket): Socket(std::move(socket)), _worldSession(nullptr), antiflood_count(0)
{
	this->SetConnectTime(0);
	this->SetConnectTimeChecked(false);
	this->GetConnectKickTime()->Start();

	sLog->outInfo(LOG_NETWORK, "Attempt to add new socket: %s", GetRemoteIpAddress().to_string().c_str());
}

void WorldSocket::OnClose()
{
	this->GetConnectKickTime()->Start();

	sLog->outInfo(LOG_NETWORK, "Closing socket: %s", GetRemoteIpAddress().to_string().c_str());
}

void WorldSocket::Start()
{
	if ( sGameServer->GetMaxConnectionsPerIP() != -1 && sObjectMgr->GetIPConnected(GetRemoteIpAddress().to_string()) >= sGameServer->GetMaxConnectionsPerIP() )
	{
		sLog->outError(LOG_NETWORK, "IP connection count max: %s", GetRemoteIpAddress().to_string().c_str());
		CloseSocket();
		return;
	}

    AsyncRead();

	this->SetConnectTime(MyGetTickCount());
	this->SetConnectTimeChecked(false);

	CLIENT_CONNECT_CUSTOM pMsg;
	pMsg.time = this->GetConnectTime();

	this->SendPacket((uint8*)&pMsg, pMsg.GetSize());
}

bool WorldSocket::HandleSendAuthSession()
{
	if ( sGameServer->IsConnectionDisabled() )
	{
		CloseSocket();
		return false;
	}

	if ( sAuthServer->IsStopped() )
	{
		CloseSocket();
		return false;
	}

	if ( sServerLink->IsStopped() )
	{
		CloseSocket();
		return false;
	}

	if ( !sGameServer->IsActiveStatus() )
	{
		CloseSocket();
		return false;
	}

	_worldSession = sObjectMgr->PlayerAdd(shared_from_this(), false);

	if ( _worldSession )
	{
		this->SetConnectTimeChecked(true);

		_worldSession->LoginBoxSend();

		sLog->outInfo(LOG_NETWORK, "Added new socket: %s at [%d]", GetRemoteIpAddress().to_string().c_str(), _worldSession->GetEntry());
	}
	else
	{
		sLog->outError(LOG_NETWORK, "Failed to add new socket: %s", GetRemoteIpAddress().to_string().c_str());
		CloseSocket();
		return false;
	}

	return true;
}

void WorldSocket::ReadHandler()
{
    if (!IsOpen())
        return;

	uint8 headcode = 0;
	int32 read_size = 0;
	uint16 size = 0;
	uint8 decrypted[MAX_BUFFER_SIZE];
	uint8 buffer[MAX_BUFFER_SIZE];
	uint8 xcode = 0;
	uint8 serial = 0;
	
	MessageBuffer& packet = GetReadBuffer();

	Player* pPlayer = _worldSession;
	this->antiflood_count = 0;

	while (packet.GetActiveSize() > 0)
	{
		this->antiflood_count++;

		if (!this->GetEncDec()->Decode(packet.GetReadPointer(), decrypted, size, serial))
		{
			if (pPlayer)
			{
				sLog->outError("antihack", "DECRYPT ERROR (%d): %s", pPlayer->GetWrongDecryptCount(), pPlayer->BuildLog().c_str());

				pPlayer->IncreaseWrongDecryptCount(1);

				if (pPlayer->GetWrongDecryptCount() >= sGameServer->GetWrongDecryptCount())
				{
					CloseSocket();
				}
				else
				{
					packet.ReadCompleted(packet.GetActiveSize());
					AsyncRead();
				}
			}
			else
			{
				packet.ReadCompleted(packet.GetActiveSize());
				AsyncRead();
			}

			this->AntiFloodCheck();
			return;
		}

		if (this->GetEncDec()->SerialRECV != serial)
		{
			if (pPlayer)
			{
				sLog->outError("antihack", "SERIAL ERROR (%d)[%u / %u]: %s", pPlayer->GetWrongSerialCount(), this->GetEncDec()->SerialRECV, serial, pPlayer->BuildLog().c_str());

				pPlayer->IncreaseWrongSerialCount(1);

				if (pPlayer->GetWrongSerialCount() >= sGameServer->GetWrongSerialCount())
				{
					CloseSocket();
				}
				else
				{
					packet.ReadCompleted(packet.GetActiveSize());
					AsyncRead();
				}
			}
			else
			{
				packet.ReadCompleted(packet.GetActiveSize());
				AsyncRead();
			}

			this->GetEncDec()->SerialRECV = serial + 1;
			this->AntiFloodCheck();
			return;
		}

		this->GetEncDec()->SerialRECV++;

		xcode = decrypted[0];

		if (!DecryptPacket(headcode, size, decrypted, buffer))
		{
			if (pPlayer)
			{
				sLog->outError("antihack", "Decrypt Packet: %s [XCode: %02X - Size: %02X - Headcode: %02X]", pPlayer->BuildLog().c_str(), xcode, size, headcode);
			}

			packet.ReadCompleted(packet.GetActiveSize());
			AsyncRead();
			this->AntiFloodCheck();
			return;
		}

		if (sGameServer->IsPacketRecvLog())
		{
			if (pPlayer)
			{
				sLog->outPacket("antihack", buffer, size, "CLIENT TO SERVER %s: ", pPlayer->BuildLog().c_str());
			}
		}

		if (packet.GetActiveSize() < (size + ENCODE_DECODE_SIZE))
		{
			packet.ReadCompleted(packet.GetActiveSize());
			AsyncRead();
			this->AntiFloodCheck();
			return;
		}

		if (headcode == CLIENT_CONNECT_CUSTOM_HEADER)
		{
			if (!this->IsConnectTimeChecked())
			{
				CLIENT_CONNECT_CUSTOM_RESULT* lpCustom = (CLIENT_CONNECT_CUSTOM_RESULT*)buffer;

				if (lpCustom->result != 1 || lpCustom->time != this->GetConnectTime())
				{
					sLog->outError("antihack", "%s => Wrong connect time for %s - Result: %u Time: [%u - %u]", __FUNCTION__, this->GetRemoteIpAddress().to_string().c_str(), lpCustom->result, this->GetConnectTime(), lpCustom->time);

					CloseSocket();
					return;
				}

				if (!this->HandleSendAuthSession())
				{
					sLog->outError("network", "%s => Couldn't add %s - Result: %u Time: [%u - %u]", __FUNCTION__, this->GetRemoteIpAddress().to_string().c_str(), lpCustom->result, this->GetConnectTime(), lpCustom->time);

					return;
				}
			}

			packet.ReadCompleted(size + ENCODE_DECODE_SIZE);
			continue;
		}

		if (!this->IsConnectTimeChecked())
		{
			packet.ReadCompleted(size + ENCODE_DECODE_SIZE);
			continue;
		}

		if (pPlayer)
		{
			if (headcode == HEADCODE_TIME_CHECK)
			{
				pPlayer->Ping(buffer);
			}
			else
			{
				pPlayer->AddPacket(new WorldPacket(headcode, buffer, size));
			}
		}

		packet.ReadCompleted(size + ENCODE_DECODE_SIZE);
	}
	
    AsyncRead();
	this->AntiFloodCheck();
}

void WorldSocket::AntiFloodCheck()
{
	if ( sGameServer->GetAntiFloodCount() <= 0 )
	{
		return;
	}

	if ( this->antiflood_count >= sGameServer->GetAntiFloodCount() )
	{
		if ( Player* pPlayer = this->_worldSession )
		{
			sLog->outError("antihack", "%s -- Count: %d // %s", __FUNCTION__, this->antiflood_count, pPlayer->BuildLog().c_str());
		}
		else
		{
			sLog->outError("antihack", "%s -- Count: %d // %s", __FUNCTION__, this->antiflood_count, this->GetRemoteIpAddress().to_string().c_str());
		}

		if ( sGameServer->IsAntiFloodKick() )
		{
			this->CloseSocket();
		}
	}
}

bool WorldSocket::DecryptPacket(uint8 & headcode, uint16 & size, uint8 * decrypted, uint8 * buffer)
{
	int32 dec_size = -1;
	uint8 code = decrypted[0];

	switch ( code )
	{
	case 0xC1:
	case 0xC3:
		{
			headcode = decrypted[2];
		} break;

	case 0xC2:
	case 0xC4:
		{
			headcode = decrypted[3];
		} break;

	default:
		{
			return false;
		} break;
	}

	return this->GetDataParse()->Process(decrypted, size, buffer);
}

bool WorldSocket::Update()
{
	if (!IsOpen())
	{
		return false;
	}

	if (_worldSession)
	{
		return true;
	}

	if (sGameServer->GetConnectTimeKick() != 0 && this->GetConnectKickTime()->Elapsed(sGameServer->GetConnectTimeKick()))
	{
		return false;
	}

	return true;
}

void WorldSocket::SendPacket(uint8 * packet, uint16 size)
{
    if (!IsOpen())
        return;

	if (this->IsConnectTimeChecked() && !_worldSession)
		return;

	if (sGameServer->IsPacketSendLog() && _worldSession)
	{
		sLog->outPacket(LOG_NETWORK, packet, size, "%s %s: ", __FUNCTION__, _worldSession->BuildLog().c_str());
	}

    std::unique_lock<std::mutex> guard(_writeLock);

	if ( packet[0] == 0xC3 )
		packet[0] = 0xC1;

	if ( packet[0] == 0xC4 )
		packet[0] = 0xC2;

	uint8 buffer_enc[8192];
	memcpy(buffer_enc, packet, size);
	
	uint8 start_encrypt = buffer_enc[0] == 0xC1 ? 3: buffer_enc[0] == 0xC2 ? 4: 0;

	if ( !start_encrypt )
	{
		return;
	}

	uint8 headcode = buffer_enc[start_encrypt - 1];

	for ( int32 i = start_encrypt; i < size; ++i )
	{
		buffer_enc[i] += headcode;
	}

#ifndef TC_SOCKET_USE_IOCP
    if (_writeQueue.empty() && _writeBuffer.GetRemainingSpace() >= header.getHeaderLength() + packet.size())
    {
        _writeBuffer.Write(header.header, header.getHeaderLength());
        if (!packet.empty())
            _writeBuffer.Write(packet.contents(), packet.size());
    }
    else
#endif
    {
		MessageBuffer buffer(size);
		//buffer.Write(packet, size);
		buffer.Write(buffer_enc, size);
        
        QueuePacket(std::move(buffer), guard);
    }
}