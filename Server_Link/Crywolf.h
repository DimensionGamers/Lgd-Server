#ifndef CRYWOLF_H
#define CRYWOLF_H

class Crywolf
{
	SingletonInstance(Crywolf);

	public:
		Crywolf();
		virtual ~Crywolf();

		void GetStatus(std::shared_ptr<ServerSocket> socket);
		void UpdateStatus(uint8 * Packet);
};

#endif