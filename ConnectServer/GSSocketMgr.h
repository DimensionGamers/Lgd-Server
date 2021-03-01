/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

/** \addtogroup u2w User to Login Communication
 *  @{
 *  \file GSSocketMgr.h
 *  \author Derex <derex101@gmail.com>
 */

#ifndef __LOGINSOCKETMGR_H
#define __LOGINSOCKETMGR_H

class GSSocket;

/// Manages all sockets connected to peers and network threads
class GSSocketMgr : public SocketMgr<GSSocket>
{
    typedef SocketMgr<GSSocket> BaseSocketMgr;
	typedef std::set<std::shared_ptr<GSSocket>> GSSocketList;

public:
    static GSSocketMgr& Instance()
    {
        static GSSocketMgr instance;
        return instance;
    }

	~GSSocketMgr();

    /// Start network, listen at address:port .
    bool StartNetwork(boost::asio::io_service& service, std::string const& bindIp, uint16 port, int32 threadCount) override;

    void OnSocketOpen(tcp::socket&& sock) override;

	GSSocketList _socket_list;

	void SendPacketAll(uint8 * Packet, uint16 size, uint16 server_group);
protected:
    GSSocketMgr();

    NetworkThread<GSSocket>* CreateThreads() const override;

private:
	AsyncAcceptor* _instanceAcceptor;
    int32 _socketSendBufferSize;
    int32 m_SockOutUBuff;
    bool _tcpNoDelay;
};

#endif
/// @}
