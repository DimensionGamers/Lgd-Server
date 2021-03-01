/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Guild.h"
*
*/

#ifndef GUILD_H
#define GUILD_H

#define GUILD_TYPE_TEST 0x0

class Guild;

struct WarData
{
	void Reset()
	{
		this->GetUpdateTime()->Reset();
		this->SetState(GUILD_WAR_STATE_NONE);
		this->SetEnemy(nullptr);
		this->SetType(GUILD_WAR_TYPE_NONE);
		this->SetTime(0);
		this->SetScore(0);
		this->SetTeam(0);
	}

	void StartRequest(Guild* pGuild, GuildWarType type, uint32 time)
	{
		this->SetState(GUILD_WAR_STATE_DECLARE);
		this->SetEnemy(pGuild);
		this->SetType(type);
		this->SetTime(GetTickCount() + time + (IN_MILLISECONDS * 2));
		this->SetScore(0);
		this->SetTeam(0);
	}

	bool TimeOut()
	{
		return GetTickCount() > this->GetTime();
	}
	
	DECLARE_ENUM(GuildWarState, State);
	DECLARE_PROPERTY_PTR(Guild, Enemy);
	DECLARE_ENUM(GuildWarType, Type);
	DECLARE_ENUM(uint32, Time);
	DECLARE_PROPERTY(int32, Score);
	DECLARE_ENUM(uint8, Team);
	DECLARE_STRUCT(TickTimer, UpdateTime);
};

class GuildMember
{
	public:
		GuildMember()
		{
			this->Reset();
		}

		DECLARE_ENUM(uint32, ID);
		DECLARE_ENUM(uint8, Ranking);
		DECLARE_STRING_FIXED(Name, (MAX_CHARACTER_LENGTH + 1));
		DECLARE_ENUM(uint16, Server);
		DECLARE_PROPERTY_PTR(Player, Player);
		DECLARE_BOOL(WarInvolved);
		
		void Reset()
		{
			this->SetID(0);
			this->SetRanking(GUILD_RANK_NONE);
			this->ResetName();
			this->SetServer(-1);
			this->SetPlayer(nullptr);
			this->SetWarInvolved(false);
		}
};

class Guild
{
	public:
		explicit Guild();
		virtual ~Guild();

		DECLARE_ARRAY_STRUCT(GuildMember, Member, MAX_GUILD_MEMBER);

		uint8 * GetEmblem() { return this->emblem; }

		void UpdateAlliance(uint32 value);
		void UpdateHostil(uint32 value);

		uint8 GetTotalMembers();

		uint8 GetMemberRanking(Player const* pPlayer) const;

		void GetRankedMembersCount(int32 & asistant_count, int32 & battle_master_count);

		void UpdateTime();
		void UpdateRelationship();
		void UpdateLifeStone();

		void Create(uint32 id, const char * name, uint8 * emblem);
		void AddMember(uint32 id, const char * name, uint8 ranking, uint8 slot, Player* pPlayer = nullptr, uint16 server = -1);

		void DelMember(uint32 id);
		void DisbandNotify();
	
		void SendChatToMembers(const char * name, const char * msg);
		void SendNoticeToMembers(NoticeType type, const char * msg);
		void SendNoticeArgToMembers(NoticeType type, const char * msg, ...);

		void SendNotice(Player* pPlayer);

		void UpdateMemberRanking(const char * name, uint8 status);
		void UpdateMemberRankingNotify(const char * name, uint8 status);

		void UpdateMemberStatus(uint32 guid, int32 server, const char * name);

		void UpdateHostilNotify();

		bool LifeStoneTeleport(Player* pPlayer, coord_type & x, coord_type & y);
		void LifeStoneKill();

		void UpdateRegisteredInSiege();
		void CallBackRegisteredInSiege(PreparedQueryResult result);

		bool IsAllianceMaster();

		GuildMember* GetMemberData(uint32 id);
		GuildMember* FindMember(const char* name, bool master = false);
	public:
		DECLARE_ENUM(uint32, ID);
		DECLARE_STRING_FIXED(Name, (MAX_GUILD_NAME_LENGTH + 1));
		uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
		DECLARE_STRING_FIXED(Notice, (MAX_GUILD_NOTICE_LENGTH + 1));

		DECLARE_ENUM(uint32, Alliance);
		DECLARE_ENUM(uint32, Hostil);

		DECLARE_PROPERTY_PTR(Monster, LifeStone);

		DECLARE_PROPERTY(int32, Score);
		DECLARE_BOOL(RegisteredInCaslteSiege);
		DECLARE_PROPERTY(uint32, CastleSiegeMarks);

		PreparedQueryResultFuture _callbackRegisteredInSiege;
		DECLARE_ENUM(uint32, UpdateTick);

		DECLARE_STRUCT(WarData, WarData);
};

#endif