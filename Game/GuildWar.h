#ifndef GUILD_WAR_H
#define GUILD_WAR_H

class GuildWarData
{
	public:
		explicit GuildWarData(uint32 Guild01, uint32 Guild02)
		{
			this->SetGuild01(Guild01);
			this->SetGuild02(Guild02);
			this->SetCount(0);
			this->GetWarTime()->Reset();
			this->GetResetTime()->Reset();
			this->GetWarTime()->Start();
			this->GetResetTime()->Start();
		}

	private:
		DECLARE_ENUM(uint32, Guild01);
		DECLARE_ENUM(uint32, Guild02);
		DECLARE_PROPERTY(int32, Count);
		DECLARE_STRUCT(TickTimer, WarTime);
		DECLARE_STRUCT(TickTimer, ResetTime);
};

typedef std::list<GuildWarData*> GuildWarDataList;

class GuildWarMgr
{
	SingletonInstance(GuildWarMgr);

	public:
		GuildWarMgr();
		virtual ~GuildWarMgr();

		void Update();

		void StartWarRequest(Player* pPlayer, const char * guild);
		void StartWarRequestResult(Player* pPlayer, uint8 result);

		void StartWarRequestSend(Player* pPlayer, const char * guild, GuildWarType type);
		void StartWarResult(Player* pPlayer, uint8 * Packet);

		void SendStart(Player* pPlayer, const char * guild, uint8 type, uint8 team);
		void SendScore(Player* pPlayer, int32 score01, int32 score02);
		void SendEnd(Player* pPlayer, const char * guild, uint8 result);

		bool IsWarAllowed(Guild* pGuild01, Guild* pGuild02);
		void StartWar(Guild* pGuild01, Guild* pGuild02);
		void EndWar(Guild* pLooser, Guild* pWinner, uint8 reason);
		
		void IncreaseScore(Player* pPlayer, Player* pVictim);
	private:
		GuildWarDataList guild_war_list;
};

#endif