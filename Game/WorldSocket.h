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

class WorldSocket : public Socket<WorldSocket>
{
	public:
		explicit WorldSocket(tcp::socket&& socket);

		WorldSocket(WorldSocket const& right) = delete;
		WorldSocket& operator=(WorldSocket const& right) = delete;

		void Start() override;
		bool Update() override;
		
		void SendPacket(uint8 * packet, uint16 size);

		bool HandleSendAuthSession();

		void OnClose();

		void AntiFloodCheck();
	protected:
		void ReadHandler() override;

	private:
		DECLARE_STRUCT(EncodeDecode, EncDec);
		DECLARE_STRUCT(CStreamPacketEngine_Server, DataParse);

		bool DecryptPacket(uint8 & headcode, uint16 & size, uint8 * decrypted, uint8 * buffer);

		Player* _worldSession;
		int32 antiflood_count;

		DECLARE_ENUM(uint32, ConnectTime);
		DECLARE_BOOL(ConnectTimeChecked);

		DECLARE_STRUCT(TickTimer, ConnectKickTime);
};

#endif