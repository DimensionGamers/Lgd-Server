#ifndef ARKA_WAR_H
#define ARKA_WAR_H

struct ArkaWarGuild
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_PROPERTY(int32, Signs);
	DECLARE_PROPERTY(int32, MemberCount);
	DECLARE_BOOL(Admited);
	DECLARE_ENUM(time_t, RegisterTime);
};

struct ArkaWarMember
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(uint32, Guild);
};

typedef std::unordered_map<uint32, ArkaWarGuild*> ArkaWarGuildMap;
typedef std::unordered_map<uint32, ArkaWarMember*> ArkaWarMemberMap;

class ArkaWar
{
	SingletonInstance(ArkaWar);

	public:
		ArkaWar();
		virtual ~ArkaWar();

		void MasterRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void MemberRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void SignRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void Enter(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void MemberCount(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildSave(uint8 * Packet);
		void SignRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);

		void GuildRemove(uint32 id);
		void MemberGuildRemove(uint32 id);
		void MemberRemove(uint32 id);

		void GetGuildList(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GetMemberList(std::shared_ptr<ServerSocket> socket);
		
		void GetArkaWarGuildSort(std::vector<ArkaWarGuild> & guild_list, int32 min_member);

		void Clear();
	private:
		ArkaWarGuildMap arka_war_guild;
		ArkaWarMemberMap arka_war_member;
};

#endif