/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2008  MaNGOS <http://getmangos.com/>
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

std::unordered_map<uint8, LoginHandler<ServerSocket>> ServerSocketMgr::InitHandlers()
{
    std::unordered_map<uint8, LoginHandler<ServerSocket>> handlers;

	handlers[HEADCODE_SERVER_LINK_ON_CONNECT]							= { "ON_CONNECT", sizeof(SL_ON_CONNECT), &ServerSocket::HandleHeadcodeOnConnect };

    handlers[HEADCODE_SERVER_LINK_GUILD_CHAT]							= { "GUILD_CHAT", sizeof(SL_CHAT_PACKET), &ServerSocket::HandleHeadcodeCommon };
	handlers[HEADCODE_SERVER_LINK_ALLIANCE_CHAT]						= { "ALLIANCE_CHAT", sizeof(SL_CHAT_PACKET), &ServerSocket::HandleHeadcodeCommon };
	handlers[HEADCODE_SERVER_LINK_GUILD_NOTICE]							= { "GUILD_NOTICE", sizeof(SL_CHAT_PACKET), &ServerSocket::GuildNotice };
	handlers[HEADCODE_SERVER_LINK_POST]									= { "POST", sizeof(SL_CHAT_PACKET), &ServerSocket::HandleHeadcodeCommon };
	handlers[HEADCODE_SERVER_LINK_SIMPLE_MESSAGE]						= { "SIMPLE_MESSAGE", sizeof(SL_CHAT_PACKET), &ServerSocket::HandleHeadcodeCommon };
	handlers[HEADCODE_SERVER_LINK_WHISPER]								= { "WHISPER", sizeof(SL_WHISPER), &ServerSocket::HandleWhisper };
	
	handlers[HEADCODE_SERVER_LINK_GUILD_ADD]							= { "GUILD_ADD", sizeof(SL_GUILD_ADD), &ServerSocket::GuildCreateRequest };
	handlers[HEADCODE_SERVER_LINK_GUILD_REMOVE]							= { "GUILD_REMOVE", sizeof(SL_GUILD_DEL), &ServerSocket::GuildRemoveRequest };
	handlers[HEADCODE_SERVER_LINK_GUILD_MEMBER_ADD]						= { "GUILD_MEMBER_ADD", sizeof(SL_GUILD_MEMBER_ADD), &ServerSocket::GuildMemberAddRequest };
	handlers[HEADCODE_SERVER_LINK_GUILD_MEMBER_DEL]						= { "GUILD_MEMBER_DEL", sizeof(SL_GUILD_MEMBER_DEL), &ServerSocket::GuildMemberDelRequest };
	handlers[HEADCODE_SERVER_LINK_GUILD_MEMBER_STATUS]					= { "GUILD_MEMBER_STATUS", sizeof(SL_GUILD_MEMBER_STATUS), &ServerSocket::GuildMemberStatusRequest };
	handlers[HEADCODE_SERVER_LINK_GUILD_RELATIONSHIP]					= { "GUILD_RELATIONSHIP", sizeof(SL_GUILD_RELATIONSHIP), &ServerSocket::GuildRelationshipRequest };
	handlers[HEADCODE_SERVER_LINK_GUILD_SCORE]							= { "GUILD_SCORE", sizeof(SL_GUILD_SCORE), &ServerSocket::GuildScore };

	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_DATA]				= { "CASTLE_SIEGE_LOAD_DATA", sizeof(SL_CASTLE_SIEGE_LOAD_DATA_REQUEST), &ServerSocket::CastleSiegeLoadData };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_NPC]				= { "CASTLE_SIEGE_LOAD_NPC", sizeof(SL_CASTLE_SIEGE_LOAD_NPC_REQUEST), &ServerSocket::CastleSiegeLoadNpc };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_REGISTERED_GUILD]	= { "CASTLE_SIEGE_LOAD_REGISTERED_GUILD", sizeof(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD), &ServerSocket::CastleSiegeLoadRegisteredGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_NPC]				= { "CASTLE_SIEGE_SAVE_NPC", sizeof(SL_CASTLE_SIEGE_SAVE_NPC_HEAD), &ServerSocket::CastleSiegeSaveNpc };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_CLEAR_GUILD]				= { "CASTLE_SIEGE_CLEAR_GUILD", sizeof(SL_CASTLE_SIEGE_CLEAR_GUILD), &ServerSocket::CastleSiegeClearGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_INSERT_FINAL_GUILD]		= { "CASTLE_SIEGE_INSERT_FINAL_GUILD", sizeof(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_HEAD), &ServerSocket::CastleSiegeInsertFinalGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_TAX_RATE]			= { "CASTLE_SIEGE_SAVE_TAX_RATE", sizeof(SL_CASTLE_SIEGE_SAVE_TAX_RATE), &ServerSocket::CastleSiegeSaveTax };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_MONEY]				= { "CASTLE_SIEGE_SAVE_MONEY", sizeof(SL_CASTLE_SIEGE_SAVE_MONEY), &ServerSocket::CastleSiegeSaveMoney };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_OWNER_STATUS]		= { "CASTLE_SIEGE_SAVE_OWNER_STATUS", sizeof(SL_CASTLE_SIEGE_SAVE_OWNER_STATUS), &ServerSocket::CastleSiegeSaveOwnerStatus };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_INSERT_REGISTERED_GUILD]	= { "CASTLE_SIEGE_INSERT_REGISTERED_GUILD", sizeof(SL_CASTLE_SIEGE_INSERT_REGISTERED_GUILD), &ServerSocket::CastleSiegeInsertRegisteredGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_DELETE_REGISTERED_GUILD]	= { "CASTLE_SIEGE_DELETE_REGISTERED_GUILD", sizeof(SL_CASTLE_SIEGE_DELETE_REGISTERED_GUILD), &ServerSocket::CastleSiegeDeleteRegisteredGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD]	= { "CASTLE_SIEGE_UPDATE_REGISTERED_GUILD", sizeof(SL_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD), &ServerSocket::CastleSiegeUpdateRegisteredGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD]		= { "CASTLE_SIEGE_REGISTERED_GUILD", sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_REQUEST), &ServerSocket::CastleSiegeRegisteredGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD_ALL]	= { "CASTLE_SIEGE_REGISTERED_GUILD_ALL", sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_ALL_REQUEST), &ServerSocket::CastleSiegeRegisteredGuildAll };

	handlers[HEADCODE_SERVER_LINK_NOTICE]								= { "NOTICE", sizeof(SL_NOTICE), &ServerSocket::HandleHeadcodeCommon };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTER_GUILD]			= { "CASTLE_SIEGE_REGISTER_GUILD", sizeof(SL_CASTLE_SIEGE_REGISTER_GUILD), &ServerSocket::HandleHeadcodeCommon };
	handlers[HEADCODE_SERVER_LINK_CHARACTER_ON_OFF]						= { "CHARACTER_ON_OFF", sizeof(SL_CHARACTER_ON_OFF), &ServerSocket::CharacterLoginLogout };

	handlers[HEADCODE_SERVER_LINK_CHARACTER_CREATE]						= { "CHARACTER_CREATE", sizeof(SL_CHARACTER_CREATE_REQUEST), &ServerSocket::CharacterCreateRequest };
	handlers[HEADCODE_SERVER_LINK_CHARACTER_DELETE]						= { "CHARACTER_DELETE", sizeof(SL_CHARACTER_DELETE_REQUEST), &ServerSocket::CharacterDeleteRequest };

	handlers[HEADCODE_SERVER_LINK_ADMIN_COMMAND]						= { "ADMIN_COMMAND", sizeof(SL_ADMIN_COMMAND), &ServerSocket::AdminCommand };
	handlers[HEADCODE_SERVER_LINK_ADMIN_COMMAND_LOCATION]				= { "ADMIN_COMMAND_LOCATION", sizeof(SL_ADMIN_COMMAND_LOCATION), &ServerSocket::AdminCommandLocation };

	handlers[HEADCODE_SERVER_LINK_CRYWOLF_REQUEST]						= { "CRYWOLF_REQUEST", sizeof(SL_CRYWOLF_REQUEST), &ServerSocket::CrywolfDataRequest };
	handlers[HEADCODE_SERVER_LINK_CRYWOLF_SAVE]							= { "CRYWOLF_SAVE", sizeof(SL_CRYWOLF_SAVE), &ServerSocket::CrywolfDataSave };


	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST]					= { "GUILD_MATCHING_LIST", sizeof(SL_GUILD_MATCHING_LIST_RECV), &ServerSocket::GuildMatchingList };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST_SEARCH]			= { "GUILD_MATCHING_LIST_SEARCH", sizeof(SL_GUILD_MATCHING_LIST_SEARCH_RECV), &ServerSocket::GuildMatchingListSearch };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT]				= { "GUILD_MATCHING_INSERT", sizeof(SL_GUILD_MATCHING_INSERT_RECV), &ServerSocket::GuildMatchingInsert };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_CANCEL]				= { "GUILD_MATCHING_CANCEL", sizeof(SL_GUILD_MATCHING_CANCEL_RECV), &ServerSocket::GuildMatchingCancel };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INSERT]			= { "GUILD_MATCHING_JOIN_INSERT", sizeof(SL_GUILD_MATCHING_JOIN_INSERT_RECV), &ServerSocket::GuildMatchingJoinInsert };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_CANCEL]			= { "GUILD_MATCHING_JOIN_CANCEL", sizeof(SL_GUILD_MATCHING_JOIN_CANCEL_RECV), &ServerSocket::GuildMatchingJoinCancel };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_ACCEPT]			= { "GUILD_MATCHING_JOIN_ACCEPT", sizeof(SL_GUILD_MATCHING_JOIN_ACCEPT_RECV), &ServerSocket::GuildMatchingJoinAccept };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_LIST]				= { "GUILD_MATCHING_JOIN_LIST", sizeof(SL_GUILD_MATCHING_JOIN_LIST_RECV), &ServerSocket::GuildMatchingJoinList };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INFO]				= { "GUILD_MATCHING_JOIN_INFO", sizeof(SL_GUILD_MATCHING_JOIN_INFO_RECV), &ServerSocket::GuildMatchingJoinInfo };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT_SAVE]			= { "GUILD_MATCHING_INSERT_SAVE", sizeof(SL_GUILD_MATCHING_INSERT_SAVE_RECV), &ServerSocket::GuildMatchingInsertSave };

	handlers[HEADCODE_SERVER_LINK_EVENT_NOTIFICATION]					= { "EVENT_NOTIFICATION", sizeof(SL_EVENT_NOTIFICATION), &ServerSocket::HandleHeadcodeCommon };

	handlers[HEADCODE_SERVER_LINK_EVENT_STATE_UPDATE]					= { "EVENT_STATE_UPDATE", sizeof(SL_EVENT_STATE_UPDATE), &ServerSocket::EventStateUpdate };

	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_MASTER_REGISTER]				= { "ARKA_WAR_MASTER_REGISTER", sizeof(SL_ARKA_WAR_MASTER_REGISTER), &ServerSocket::ArkaWarMasterRegister };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_REGISTER]				= { "ARKA_WAR_MEMBER_REGISTER", sizeof(SL_ARKA_WAR_MEMBER_REGISTER), &ServerSocket::ArkaWarMemberRegister };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REGISTER]				= { "ARKA_WAR_SIGN_REGISTER", sizeof(SL_ARKA_WAR_SIGN_REGISTER), &ServerSocket::ArkaWarSignRegister };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_ENTER]						= { "ARKA_WAR_ENTER", sizeof(SL_ARKA_WAR_ENTER), &ServerSocket::ArkaWarEnter };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_LIST]						= { "ARKA_WAR_LIST", sizeof(SL_ARKA_WAR_LIST), &ServerSocket::ArkaWarList };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_CLEAR]						= { "ARKA_WAR_CLEAR", sizeof(SL_ARKA_WAR_CLEAR), &ServerSocket::ArkaWarClear };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_COUNT]				= { "ARKA_WAR_MEMBER_COUNT", sizeof(SL_ARKA_WAR_MEMBER_COUNT), &ServerSocket::ArkaWarMemberCount };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_REQUEST]				= { "ARKA_WAR_GUILD_REQUEST", sizeof(SL_ARKA_WAR_GUILD_REQUEST), &ServerSocket::ArkaWarGuildRequest };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_SAVE]					= { "ARKA_WAR_GUILD_SAVE", sizeof(SL_ARKA_WAR_GUILD_HEAD), &ServerSocket::ArkaWarGuildSave };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REQUEST]				= { "ARKA_WAR_SIGN_REQUEST", sizeof(SL_ARKA_WAR_SIGN_REQUEST), &ServerSocket::ArkaWarSignRequest };

	handlers[HEADCODE_SERVER_LINK_CHARACTER_SIGNAL]						= { "CHARACTER_SIGNAL", sizeof(SL_CHARACTER_SIGNAL), &ServerSocket::CharacterSignal };

    return handlers;
}

std::unordered_map<uint8, LoginHandler<ServerSocket>> const Handlers = ServerSocketMgr::InitHandlers();

#include <boost/system/error_code.hpp>

static void OnSocketAccept(tcp::socket&& sock)
{
    sServerSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock));
}

class ServerSocketThread : public NetworkThread<ServerSocket>
{
public:
    void SocketAdded(std::shared_ptr<ServerSocket> sock) override
    {
        sServerSocketMgr._socket_list.insert(sock);
    }

    void SocketRemoved(std::shared_ptr<ServerSocket> sock) override
    {
        sServerSocketMgr._socket_list.erase(sock);
    }
};

ServerSocketMgr::ServerSocketMgr() : BaseSocketMgr(), _socketSendBufferSize(-1), m_SockOutUBuff(65536), _tcpNoDelay(true)
{
}

bool ServerSocketMgr::StartNetwork(boost::asio::io_service& service, std::string const& bindIp, uint16 port, int32 threadCount)
{
    BaseSocketMgr::StartNetwork(service, bindIp, port, threadCount);

    _acceptor->AsyncAcceptManaged(&OnSocketAccept);
    return true;
}

void ServerSocketMgr::StopNetwork()
{
    BaseSocketMgr::StopNetwork();

	ServerPacketData* packet;
	while(this->_recvPacket.next(packet))
	{
		if ( packet )
			delete packet;
	}
}

void ServerSocketMgr::OnSocketOpen(tcp::socket&& sock)
{
    // set some options here
    if (_socketSendBufferSize >= 0)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::socket_base::send_buffer_size(_socketSendBufferSize), err);
        if (err && err != boost::system::errc::not_supported)
        {
			sLog->outError(LOG_DEFAULT, "ServerSocketMgr::OnSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = %s", err.message().c_str());
			return;
        }
    }

    // Set TCP_NODELAY.
    if (_tcpNoDelay)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::ip::tcp::no_delay(true), err);
        if (err)
        {
			sLog->outError(LOG_DEFAULT, "ServerSocketMgr::OnSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = %s", err.message().c_str());
			return;
        }
    }

    BaseSocketMgr::OnSocketOpen(std::forward<tcp::socket>(sock));
}

NetworkThread<ServerSocket>* ServerSocketMgr::CreateThreads() const
{
    return new ServerSocketThread[GetNetworkThreadCount()];
}

void ServerSocketMgr::SendPacketAll(uint8 * Packet, uint16 size)
{
	for (auto& socket : _socket_list)
    {
		socket->QueuePacket(Packet, size);
	}
}

void ServerSocketMgr::UpdatePacket()
{
	ServerPacketData* packet;
	int32 packet_count = 0;

	while ( !_recvPacket.empty() && _recvPacket.next(packet) )
	{
		if ( !packet )
			continue;

		this->HandlePacketData(packet->socket, packet->GetHeadcode(), packet->GetReadPointer());

		delete packet;

		++packet_count;

		if ( packet_count >= 100 )
			break;
	}
}
	
void ServerSocketMgr::AddPacket(ServerPacketData* packet)
{
	_recvPacket.add(packet);
}
	
void ServerSocketMgr::HandlePacketData(std::shared_ptr<ServerSocket> socket, uint8 headcode, uint8 * packet)
{
	auto itr = Handlers.find(headcode);
    if (itr == Handlers.end())
        return;

	(*socket.*Handlers.at(headcode).handler)(packet);
}