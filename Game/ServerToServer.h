#ifndef SERVER_TO_SERVER_H
#define SERVER_TO_SERVER_H

struct WorldServer
{
	DECLARE_ENUM(uint16, Server);
	DECLARE_ENUM(world_type, World);
};

typedef std::set<world_type> DisabledWorldList;

class ServerToServer
{
	SingletonInstance(ServerToServer);

	public:
		ServerToServer();
		virtual ~ServerToServer();

		bool IsWorldEnabled(world_type world) const;

	private:
		DisabledWorldList disabled_world_list;
};

#endif