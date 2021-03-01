/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Crywolf.h"
*
*/
#ifndef CRYWOLF_H
#define CRYWOLF_H

class Crywolf
{
	SingletonInstance(Crywolf);

	public:
		Crywolf();
		virtual ~Crywolf();

		void ClearData();
		void LoadData();

		void AltarContractRequest(uint8 * packet, Player* pPlayer);
		void AltarContractResult(uint8 result, uint8 state, uint16 altar, Player* pPlayer);

		void PlusChaosRateRequest(Player* pPlayer);

		void sendStatueAndAltarData();
		void SendStateLeftTime();
		void sendMonsterInfo();
		void sendStateInfo();
		void sendStateInfo(Player* pPlayer);
		void sendPacket(uint8 * packet, uint16 size);

		void sendNoticeToPlayersOnMap(bool map_only, NoticeType type, const char * notice, ...);
		void removeCommonMonsters();
		void restoreCommonMonsters();
		void alterCommonNpc(uint8 value);
		void ResetPlayerScore();
		void NotifyPersonalRank();
		void NotifyHeroList();
		void AddMonster(std::string const& ai_group);
		void ChangeAI(std::string const& ai_group, int32 order);
		uint8 GetRankingPosition(Player* pPlayer);
		int64 GetExperience(Player* pPlayer, uint8 ranking);

		void UpdateTime();

		/* Altar Action */
		uint8 altarGetCount();
		uint32 altarGetHP();
		uint32 altarGetMaxHP();
		
		void altarPrepare();
		bool IsInAltar(Player* pPlayer);

		void SetState_None();
		void SetState_Notify_1();
		void SetState_Notify_2();
		void SetState_Ready();
		void SetState_Start();
		void SetState_End();
		void SetState_EndCycle();

		void ProcState_None();
		void ProcState_Notify_1();
		void ProcState_Notify_2();
		void ProcState_Ready();
		void ProcState_Start();
		void ProcState_End();
		void ProcState_EndCycle();

		void RunTime(uint32 time)
		{
			this->SetUpdateTick(GetTickCount());
			this->SetUpdateTime(time);
			this->SetCurrentMinute(-1);
		}

		bool TimePassed() const
		{
			return (GetTickCount() - this->GetUpdateTick()) > this->GetUpdateTime();
		}

		uint32 GetRemain() const
		{
			uint32 cur_time = GetTickCount();
			uint32 diff = cur_time - this->GetUpdateTick();

			if ( diff < this->GetUpdateTime() )
			{
				diff = this->GetUpdateTime() - diff;
			}
			else
			{
				diff = 0;
			}

			return diff;
		}

		bool IsMinuteElapsed(int32 period)
		{
			if ( !this->GetRemain() || (this->GetRemain() / (IN_MILLISECONDS * MINUTE)) == this->GetCurrentMinute() )
				return false;

			this->SetCurrentMinute(this->GetRemain() / (IN_MILLISECONDS * MINUTE));

			if ( (this->GetCurrentMinute() % period) != 0 )
				return false;

			if ( (this->GetCurrentMinute() + 1) <= 0 )
				return false;

			return true;
		}
	public:
		DECLARE_ENUM(uint8, OccupationState);
		DECLARE_ENUM(uint8, State);
		
		DECLARE_STRUCT(TickTimer, NotifyTime);
		DECLARE_STRUCT(TickTimer, BossTime);
		DECLARE_STRUCT(Timer, LeftTime);
		DECLARE_BOOL(ChangeAI);
		DECLARE_ENUM(uint8, Boss);

		DECLARE_ARRAY_STRUCT(CrywolfStateData, StateData, CRYWOLF_STATE_MAX);
		DECLARE_ARRAY_STRUCT(CrywolfRankingData, RankingData, CRYWOLF_MVP_RANK_MAX);

		DECLARE_ENUM(uint32, UpdateTick);
		DECLARE_ENUM(uint32, UpdateTime);
		DECLARE_ENUM(int32, CurrentMinute);
};

#endif