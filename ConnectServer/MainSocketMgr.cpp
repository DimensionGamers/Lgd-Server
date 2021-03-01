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

#include <boost/system/error_code.hpp>

static void OnSocketAccept(tcp::socket&& sock)
{
    sMainSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock));
}

class MainSocketThread : public NetworkThread<MainSocket>
{
public:
    void SocketAdded(std::shared_ptr<MainSocket> sock) override
    {
		sMainSocketMgr.IncreaseIP(sock->GetRemoteIpAddress().to_string());
    }

    void SocketRemoved(std::shared_ptr<MainSocket> sock) override
    {
		sMainSocketMgr.DecreaseIP(sock->GetRemoteIpAddress().to_string());
    }
};

MainSocketMgr::MainSocketMgr() : BaseSocketMgr(), _instanceAcceptor(nullptr), _socketSendBufferSize(-1), m_SockOutUBuff(65536), _tcpNoDelay(true), _max_connection_per_ip(0)
{
}

MainSocketMgr::~MainSocketMgr()
{
	if ( _instanceAcceptor )
		delete _instanceAcceptor;
}

bool MainSocketMgr::StartNetwork(boost::asio::io_service& service, std::string const& bindIp, uint16 port, int32 threadCount)
{
	_max_connection_per_ip = sConfig->GetIntDefault("Network.MaxConnectionPerIP", 0);

    BaseSocketMgr::StartNetwork(service, bindIp, port, threadCount);

    _acceptor->AsyncAcceptManaged(&OnSocketAccept);
    return true;
}

void MainSocketMgr::OnSocketOpen(tcp::socket&& sock)
{
    // set some options here
    if (_socketSendBufferSize >= 0)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::socket_base::send_buffer_size(_socketSendBufferSize), err);
        if (err && err != boost::system::errc::not_supported)
        {
			sLog->outError(LOG_DEFAULT, "MainSocketMgr::OnSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = %s", err.message().c_str());
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
			sLog->outError(LOG_DEFAULT, "MainSocketMgr::OnSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = %s", err.message().c_str());
			return;
        }
    }

    BaseSocketMgr::OnSocketOpen(std::forward<tcp::socket>(sock));
}

bool MainSocketMgr::ConnectIPAllowed(std::shared_ptr<MainSocket> sock)
{
	return GetIPCount(sock->GetRemoteIpAddress().to_string()) < _max_connection_per_ip || _max_connection_per_ip == -1;
}

NetworkThread<MainSocket>* MainSocketMgr::CreateThreads() const
{
    return new MainSocketThread[GetNetworkThreadCount()];
}