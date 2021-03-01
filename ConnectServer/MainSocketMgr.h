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

/** \addtogroup u2w User to World Communication
 *  @{
 *  \file MainSocketMgr.h
 *  \author Derex <derex101@gmail.com>
 */

#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H

class MainSocket;

/// Manages all sockets connected to peers and network threads
class MainSocketMgr : public SocketMgr<MainSocket>
{
    typedef SocketMgr<MainSocket> BaseSocketMgr;

public:
    static MainSocketMgr& Instance()
    {
        static MainSocketMgr instance;
        return instance;
    }

	~MainSocketMgr();

    /// Start network, listen at address:port .
    bool StartNetwork(boost::asio::io_service& service, std::string const& bindIp, uint16 port, int32 threadCount) override;

    void OnSocketOpen(tcp::socket&& sock) override;
	bool ConnectIPAllowed(std::shared_ptr<MainSocket> sock);

	void IncreaseIP(std::string ip)
	{
		ip_count[ip]++;
	}

	void DecreaseIP(std::string ip)
	{
		ip_count[ip]--;
	}
protected:
    MainSocketMgr();

    NetworkThread<MainSocket>* CreateThreads() const override;

	int32 GetIPCount(std::string ip)
	{
		return ip_count[ip];
	}
private:
	AsyncAcceptor* _instanceAcceptor;
    int32 _socketSendBufferSize;
    int32 m_SockOutUBuff;
    bool _tcpNoDelay;
	int32 _max_connection_per_ip;
	std::unordered_map<std::string, std::atomic<int32>> ip_count;
};

#endif
/// @}
