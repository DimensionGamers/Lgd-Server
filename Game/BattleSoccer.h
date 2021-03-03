/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "BattleSoccer.h"
*
*/
#ifndef BATTLE_SOCCER_H
#define BATTLE_SOCCER_H

struct BattleSoccerPlayer
{
	DECLARE_PTR(Player, Player);
	DECLARE_BOOL(Master);

	void Reset()
	{
		this->SetPlayer(nullptr);
		this->SetMaster(false);
	}
};

class BattleSoccerTeam
{
	public:
		DECLARE_PTR(Guild, Guild);
		DECLARE_PROPERTY(int32, Score);
		DECLARE_ENUM(uint16, PartyID);

		void Reset()
		{
			this->SetGuild(nullptr);
			this->SetScore(0);
			this->SetPartyID(-1);
			
			PARTY_LOOP(i)
			{
				this->GetPlayer(i)->Reset();
			}
		}

	public:
		DECLARE_ARRAY_STRUCT(BattleSoccerPlayer, Player, MAX_PARTY_MEMBERS);
};

class CBattleSoccerMgr
{
	SingletonInstance(CBattleSoccerMgr);

	public:
		CBattleSoccerMgr();
		virtual ~CBattleSoccerMgr();

		void Update();

		void SetState_None();
		void SetState_StandBy();
		void SetState_Playing();
		void SetState_Playend();
		
		void ProcState_None();
		void ProcState_StandBy();
		void ProcState_Playing();
		void ProcState_Playend();

		void UpdateBall(Monster* Ball);

		bool IsInField(Player const* pPlayer) const;
		bool GetRespawnLocation(int16 & x, int16 & y, Player* pPlayer);
		bool IsAttackAllowed(Player* pPlayer01, Player* pPlayer02);

		void MovePlayersOut();

		void ProcessRequest(Player* pPlayer, const char * guild);
		bool PartySuccess(Party* pParty, Guild* pGuild);
		void ProcessTeams(Player* pPlayer01, Player* pPlayer02, Guild* pGuild01, Guild* pGuild02);
		void SetTeamData(Guild* pGuild01, Guild* pGuild02);

		void Finish(uint8 reason, uint8 winner, uint8 looser, int32 score);

		void SendScore();
	private:
		DECLARE_ARRAY_STRUCT(BattleSoccerTeam, Team, GUILD_WAR_TEAM_MAX);
		DECLARE_ENUM(BattleSoccerState, State);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, BallMoveTime);
		DECLARE_BOOL(BallMove);
};

#endif