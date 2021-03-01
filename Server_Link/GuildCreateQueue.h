#ifndef GUILD_CREATE_QUEUE_H
#define GUILD_CREATE_QUEUE_H

struct GuildCreateData
{
	explicit GuildCreateData(const char * name, uint8 * emblem): server(-1)
	{
		this->player.Reset();
		memset(this->name, 0, MAX_GUILD_NAME_LENGTH + 1);
		memcpy(this->name, name, MAX_GUILD_NAME_LENGTH);
		memcpy(this->emblem, emblem, MAX_GUILD_EMBLEM_LENGTH);
	}

	CUSTOM_PACKET_PLAYER player;
	uint16 server;
	char name[MAX_GUILD_NAME_LENGTH + 1];
	uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];

	std::shared_ptr<ServerSocket> socket;

	uint16 GetServerGroup() const { return this->server / 20; }
};

typedef std::shared_ptr<GuildCreateData> GuildCreateDataPtr;

class GuildCreateQueue
{
	SingletonInstance(GuildCreateQueue);

	public:
		GuildCreateQueue();
		virtual ~GuildCreateQueue();

		void Queue(GuildCreateDataPtr data);
		void Update();
		void Process(GuildCreateDataPtr data);

	private:
		LockedQueue<GuildCreateDataPtr> GuildCreateDataQueue;
};

#endif