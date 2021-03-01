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

class MainSocket : public Socket<MainSocket>
{
	non_copyable(MainSocket);

	public:
		explicit MainSocket(tcp::socket&& socket);
		void Start() override;
		bool Update() override;

		void SendPacket(uint8 * packet, uint16 size);

	protected:
		void ReadHandler() override;

	private:
		void PrococolCore(uint8 * Packet, uint16 size, uint8 headcode);
		DECLARE_STRUCT(EncodeDecode, EncDec);
		DECLARE_STRUCT(TickTimer, Time);
};

#endif
