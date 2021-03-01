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

#ifndef __WORLDSOCKET_H__
#define __WORLDSOCKET_H__

#include <boost/asio/buffer.hpp>

using boost::asio::ip::tcp;

class ServerSocket : public Socket<ServerSocket>
{
	public:
		DECLARE_ENUM(uint16, ServerCode);

		explicit ServerSocket(tcp::socket&& socket);

		ServerSocket(ServerSocket const& right) = delete;
		ServerSocket& operator=(ServerSocket const& right) = delete;

		void Start() override;

		void HandleHeadcodeOnConnect(uint8 * Packet);

		void HandleHeadcodeCommon(uint8 * Packet);
		void HandleWhisper(uint8 * Packet);

		void CharacterCreateRequest(uint8 * Packet);
		void CharacterDeleteRequest(uint8 * Packet);

		void CharacterLoginLogout(uint8 * Packet);

		void GuildCreateRequest(uint8 * Packet);
		void GuildRemoveRequest(uint8 * Packet);
		void GuildMemberAddRequest(uint8 * Packet);
		void GuildMemberDelRequest(uint8 * Packet);
		void GuildMemberStatusRequest(uint8 * Packet);
		void GuildRelationshipRequest(uint8 * Packet);
		void GuildScore(uint8 * Packet);
		void GuildNotice(uint8 * Packet);

		void CastleSiegeLoadData(uint8 * Packet);
		void CastleSiegeLoadNpc(uint8 * Packet);
		void CastleSiegeLoadRegisteredGuild(uint8 * Packet);
		void CastleSiegeSaveNpc(uint8 * Packet);
		void CastleSiegeClearGuild(uint8 * Packet);
		void CastleSiegeInsertFinalGuild(uint8 * Packet);
		void CastleSiegeSaveTax(uint8 * Packet);
		void CastleSiegeSaveMoney(uint8 * Packet);
		void CastleSiegeSaveOwnerStatus(uint8 * Packet);
		void CastleSiegeInsertRegisteredGuild(uint8 * Packet);
		void CastleSiegeDeleteRegisteredGuild(uint8 * Packet);
		void CastleSiegeUpdateRegisteredGuild(uint8 * Packet);
		void CastleSiegeRegisteredGuild(uint8 * Packet);
		void CastleSiegeRegisteredGuildAll(uint8 * Packet);

		void AdminCommand(uint8 * Packet);
		void AdminCommandLocation(uint8 * Packet);

		void CrywolfDataRequest(uint8 * Packet);
		void CrywolfDataSave(uint8 * Packet);

		void GuildMatchingList(uint8 * Packet);
		void GuildMatchingListSearch(uint8 * Packet);
		void GuildMatchingInsert(uint8 * Packet);
		void GuildMatchingCancel(uint8 * Packet);
		void GuildMatchingJoinInsert(uint8 * Packet);
		void GuildMatchingJoinCancel(uint8 * Packet);
		void GuildMatchingJoinAccept(uint8 * Packet);
		void GuildMatchingJoinList(uint8 * Packet);
		void GuildMatchingJoinInfo(uint8 * Packet);
		void GuildMatchingInsertSave(uint8 * Packet);

		void EventStateUpdate(uint8 * Packet);

		void ArkaWarMasterRegister(uint8 * Packet);
		void ArkaWarMemberRegister(uint8 * Packet);
		void ArkaWarSignRegister(uint8 * Packet);
		void ArkaWarEnter(uint8 * Packet);
		void ArkaWarList(uint8 * Packet);
		void ArkaWarClear(uint8 * Packet);
		void ArkaWarMemberCount(uint8 * Packet);
		void ArkaWarGuildRequest(uint8 * Packet);
		void ArkaWarGuildSave(uint8 * Packet);
		void ArkaWarSignRequest(uint8 * Packet);

		void CharacterSignal(uint8 * Packet);
	protected:
		void ReadHandler() override;
};

#endif
