#ifndef SOCKET_SYSTEM_H
#define SOCKET_SYSTEM_H

class SocketSystem
{
	SingletonInstance(SocketSystem);

	public:
		SocketSystem();
		virtual ~SocketSystem();

		void MixRequest(Player* pPlayer, uint8 * Packet);
};

#endif