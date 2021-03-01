/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GuildMgr.h"
*
*/

#ifndef GUILDMGR_H
#define GUILDMGR_H

typedef std::unordered_map<uint32, Guild*> GuildMap;

class CGuildMgr
{
	SingletonInstance(CGuildMgr);

	public:
		CGuildMgr();
		virtual ~CGuildMgr();

		void LoadGuild();
		void LoadGuildMembers();

		void AddGuild(Guild* guild);
		void RemoveGuild(uint32 guildId);

		Guild* GetGuild(uint32 guildId);
		Guild* GetGuild(std::string const& name);

		void UpdateTime();

		void SendMsgToGuildAlliance(uint32 alliance, const char * name, const char * notice);

		uint8 GetRelationShip(Guild * Guild1, Guild * Guild2, bool alliance = true);

		uint8 GetInvolvedGuilds(uint32 alliance);

		void SendAllianceList(Player* pPlayer);

		void AddCastleSiegeGuildList(uint32 guild, int32 score, uint8 side);

		GuildMap const& GetGuildMap() const { return m_GuildMap; }

		void SendGuildNotice(Player* pPlayer);

		void AddScore(Guild* pGuild, int32 score);
		void AddAllianceScore(uint32 alliance, int32 score);
	protected:
		GuildMap m_GuildMap;
};

#define sGuildMgr SingNull(CGuildMgr)

#endif