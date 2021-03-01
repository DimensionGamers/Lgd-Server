#ifndef GUILD_MGR_H
#define GUILD_MGR_H

struct GuildMember
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_ENUM(uint8, Ranking);
	DECLARE_STRING_FIXED(Name, (MAX_CHARACTER_LENGTH + 1));
	DECLARE_ENUM(uint16, Server);

	void Reset()
	{
		this->SetID(0);
		this->SetRanking(GUILD_RANK_NONE);
		this->ResetName();
		this->SetServer(-1);
	}
};

class Guild
{
	public:
		explicit Guild();
		virtual ~Guild();

		void Create(uint32 id, const char * name, uint8 * emblem, bool db);
		void AddMember(uint32 id, const char * name, uint8 ranking, uint8 slot, uint16 server, bool db);

		uint8 GetEmptySlot() const;
		uint8 GetMemberSlot(const char * name) const;
		
		uint8 GetTotalMembers() const;
		std::string BuildLog() const;
		void UpdateRelationshipDB();
		GuildMember* GetMemberData(uint32 id);

		void ResetMatching();

		DECLARE_BOOL(Remove);
		DECLARE_ENUM(uint32, ID);
		DECLARE_ENUM(uint32, Alliance);
		DECLARE_ENUM(uint32, Hostil);
		DECLARE_ENUM(int32, Score);
		DECLARE_ENUM(uint32, RegisteredMarks);
		DECLARE_BOOL(CastleSiegeRegistered);
		DECLARE_STRING_FIXED(Name, MAX_GUILD_NAME_LENGTH + 1);
		uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
		DECLARE_STRING_FIXED(Notice, (MAX_GUILD_NOTICE_LENGTH + 1));
		DECLARE_ARRAY_STRUCT(GuildMember, Member, MAX_GUILD_MEMBER);

		///- Matching
		DECLARE_BOOL(MatchingEnabled);
		DECLARE_STRING_FIXED(MatchingText, GUILD_MATCHING_TEXT_LENGTH + 1);
		DECLARE_ENUM(uint8, MatchingInterestType);
		DECLARE_ENUM(uint8, MatchingLevelRange);
		DECLARE_ENUM(uint16, MatchingClassType);
		DECLARE_ENUM(uint32, MatchingBoardNumber);
		DECLARE_ENUM(uint8, MatchingMasterClass);
		DECLARE_ENUM(uint32, MatchingMasterLevel);
		DECLARE_ENUM(uint8, MatchingGensType);
};

typedef std::map<uint32, Guild*> GuildMap;

class GuildMgr
{
	SingletonInstance(GuildMgr);

	public:
		GuildMgr();
		virtual ~GuildMgr();

		void LoadGuilds();
		void LoadGuildMembers();

		Guild* GetGuild(uint32 id) const;
		Guild* GetGuild(std::string name) const;

		void AddGuild(Guild* pGuild);
		void RemoveGuild(uint32 id);
		uint8 GetInvolvedGuilds(uint32 alliance) const;

		void CreateRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void RemoveRequest(uint8 * Packet);
		void MemberAddRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void MemberDelRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void MemberStatusRequest(uint8 * Packet);
		void RelationshipRequest(uint8 * Packet);
		void Score(uint8 * Packet);
		void Notice(uint8 * Packet);
		void MemberOnlineOffline(CharacterDataPtr pCharacterData);

	public:
		GuildMap guild_map;
};

#endif