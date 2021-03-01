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

#ifndef __LOGINSOCKET_H__
#define __LOGINSOCKET_H__

#include <boost/asio/buffer.hpp>

class AuthSocket : public Socket<AuthSocket>
{
	non_copyable(AuthSocket);

	public:
		static std::unordered_map<uint8, LoginHandler<AuthSocket>> InitHandlers();

		explicit AuthSocket(tcp::socket&& socket);
		virtual ~AuthSocket()
		{
			OnEnd();
		}
	
		void Start() override;
		void OnEnd();

		void SendPacket(uint8 * packet, uint16 size);

	protected:
		void ReadHandler() override;

	private:
		void GameServerConnect(uint8 * Packet);
		void GameServerPercent(uint8 * Packet);
		void AccountReconnect(uint8 * Packet);
		void AccountLoginRequest(uint8 * Packet);
		void AccountLogout(uint8 * Packet);
		void AccountServerMove(uint8 * Packet);
		void AccountServerAuth(uint8 * Packet);
		void AccountKick(uint8 * Packet);
		void AccountCompleteBan(uint8 * Packet);
		void AccountValidate(uint8 * Packet);
		void AccountConnectType(uint8 * Packet);

		DECLARE_ENUM(uint16, ServerCode);
};

#endif
