#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio/deadline_timer.hpp>

using boost::asio::deadline_timer;

enum
{
	TCPCLIENT_ON_ERROR_CLOSE			= 1 << 0,
};

template <class T>
class TCPClient
{
	public:
		TCPClient(boost::asio::io_service& ioService, std::string const& name, uint8 flags = 0): 
			m_ioService(ioService),
			_socket(ioService),
			stopped(false), 
			name(name),
			_flags(flags),
			_isWritingAsync(false)
		{
			_readBuffer.Resize(READ_BLOCK_SIZE);
		}

		virtual ~TCPClient()
		{
			OnEnd();
			this->Stop();
		}

		void Connect(std::string const& ip, uint16 port)
		{
			boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);

			_socket.async_connect(endpoint,
				boost::bind(&TCPClient::OnConnect, this, _1));
		}

		void Stop()
		{
			if ( IsStopped() )
				return;

			stopped = true;
			_socket.close();

			OnStop();
		}

		void OnConnect(const boost::system::error_code& ec)
		{
			//if (stopped)
			//	return;

			// The async_connect() function automatically opens the socket at the start
			// of the asynchronous operation. If the socket is closed at this time then
			// the timeout handler must have run first.
			if (!_socket.is_open())
			{
				sLog->outError(LOG_DEFAULT, "[ TCP Client %s ] Connect timed out", name.c_str());
				Stop();
				return;
			}

			// Check if the connect operation failed before the deadline expired.
			else if (ec)
			{
				sLog->outError(LOG_DEFAULT, "[ TCP Client %s ] Connect error: %s", name.c_str(), ec.message().c_str());
				Stop();
				return;
			}
			// Otherwise we have successfully established a connection.
			else
			{
				stopped = false;

				OnStart();
				// Start the input actor.
				StartRead();
			}
		}

		virtual void OnStart() { }
		virtual void OnEnd() { }
		virtual void OnStop() { }

		void StartRead()
		{
			if (stopped)
				return;

			_readBuffer.Normalize();
			_readBuffer.EnsureFreeSpace();
			_socket.async_read_some(boost::asio::buffer(_readBuffer.GetWritePointer(), _readBuffer.GetRemainingSpace()),
				std::bind(&TCPClient<T>::ReadHandlerInternal, this, std::placeholders::_1, std::placeholders::_2));
		}

		void ReadHandlerInternal(boost::system::error_code ec, size_t transferredBytes)
		{
			if (stopped)
				return;

			if (!ec)
			{
				_readBuffer.WriteCompleted(transferredBytes);
				ReadHandler();
				StartRead();
			}
			else
			{
				sLog->outError(LOG_DEFAULT, "[ TCP Client %s ] Error on receive: %s", name.c_str(), ec.message().c_str());

				if ( _flags & TCPCLIENT_ON_ERROR_CLOSE )
					Stop();
			}
		}

		void QueuePacket(uint8 * packet, uint16 size)
		{
			if (stopped)
				return;

			std::unique_lock<std::mutex> guard(_writeLock);

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
				buffer.Write(packet, size);
        
				QueuePacket(std::move(buffer), guard);
			}
		}

		void QueuePacket(MessageBuffer&& buffer, std::unique_lock<std::mutex>& guard)
		{
			_writeQueue.push(std::move(buffer));

	#ifdef TC_SOCKET_USE_IOCP
			AsyncProcessQueue(guard);
	#else
			(void)guard;
	#endif
		}

		bool AsyncProcessQueue(std::unique_lock<std::mutex>&)
		{
			if (_isWritingAsync)
				return false;

			_isWritingAsync = true;

	#ifdef TC_SOCKET_USE_IOCP
			MessageBuffer& buffer = _writeQueue.front();
			_socket.async_write_some(boost::asio::buffer(buffer.GetReadPointer(), buffer.GetActiveSize()), std::bind(&TCPClient::WriteHandler,
				this, std::placeholders::_1, std::placeholders::_2));
	#else
			_socket.async_write_some(boost::asio::null_buffers(), std::bind(&Socket<T>::WriteHandlerWrapper,
				this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	#endif

			return false;
		}

		void WriteHandler(boost::system::error_code error, std::size_t transferedBytes)
		{
			if (!error)
			{
				std::unique_lock<std::mutex> deleteGuard(_writeLock);

				_isWritingAsync = false;
				_writeQueue.front().ReadCompleted(transferedBytes);
				if (!_writeQueue.front().GetActiveSize())
					_writeQueue.pop();

				if (!_writeQueue.empty())
					AsyncProcessQueue(deleteGuard);
			}
			else
			{
				if ( _flags & TCPCLIENT_ON_ERROR_CLOSE )
					Stop();
			}
		}

		virtual void ReadHandler() = 0;

		MessageBuffer& GetReadBuffer() { return _readBuffer; }
		bool IsStopped() const { return stopped; }
		void CancelStop() { stopped = false; }

	private:
		std::string name;
		boost::asio::io_service& m_ioService;
		tcp::socket _socket;
		bool stopped;
		bool _isWritingAsync;

		std::mutex _writeLock;
		std::queue<MessageBuffer> _writeQueue;

		MessageBuffer _readBuffer;

		uint8 _flags;
};

template <class T>
class TCPClientMgr
{
	public:
		TCPClientMgr(std::string const& name): m_Socket(nullptr), name(name), upper_name(name)
		{
			this->SetConnectCount(0);
			this->SetStopped(true);
			this->GetReconnectTime()->Start();

			strToUpper(upper_name);
		}
		virtual ~TCPClientMgr()
		{
			WorldPacket* packet;
			while(this->_recvPacket.next(packet))
			{
				if ( packet )
					delete packet;
			}

			if ( m_Socket )
			{
				m_Socket.reset();
			}
		}

		bool Start()
		{
			uint16 port = sConfigCommon->GetIntDefault(name + ".Port", 0);
			std::string ip = sConfigCommon->GetStringDefault(name + ".BindIP", "0.0.0.0");

			this->GetReconnectTime()->Start();

			sLog->outInfo("root", "CONNECTING %s", upper_name.c_str());

			m_Socket.reset(new T);
			m_Socket->Connect(ip, port);
			return true;
		}
		
		void Stop()
		{
			if ( m_Socket )
			{
				m_Socket->Stop();
			}
		}

		void SendPacket(uint8 * packet, uint16 size)
		{
			if ( this->IsStopped() )
				return;

			if ( !m_Socket || m_Socket->IsStopped() )
				return;
		
			m_Socket->QueuePacket(packet, size);
		}

		void UpdatePacket()
		{
			WorldPacket* packet;
			int32 packet_count = 0;

			while ( !_recvPacket.empty() && _recvPacket.next(packet) )
			{
				if ( !packet )
					continue;

				this->HandlePacketData(packet->GetHeadcode(), packet->GetReadPointer());

				delete packet;

				++packet_count;

				if ( packet_count >= 300 )
					break;
			}
		}

		void AddPacket(WorldPacket* packet)
		{
			_recvPacket.add(packet);
		}

		virtual void HandlePacketData(uint8 headcode, uint8 * packet) = 0;

		LockedQueue<WorldPacket*> _recvPacket;

		std::unique_ptr<T> m_Socket;
		DECLARE_PROPERTY(int32, ConnectCount);
		DECLARE_BOOL(Stopped);
		DECLARE_STRUCT(TickTimer, ReconnectTime);
		std::string name;
		std::string upper_name;
};

#endif