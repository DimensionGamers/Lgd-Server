#ifndef SERVER_TO_SERVER_H
#define SERVER_TO_SERVER_H

struct WorldServer
{
	DECLARE_ENUM(uint16, Server);
	DECLARE_ENUM(uint16, World);
};

typedef std::set<uint16> DisabledWorldList;

class ServerToServer
{
	SingletonInstance(ServerToServer);

	public:
		ServerToServer();
		virtual ~ServerToServer();

		bool IsWorldEnabled(uint16 world) const;

	private:
		DisabledWorldList disabled_world_list;
};

#endif