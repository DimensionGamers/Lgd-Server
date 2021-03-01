/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Raklion.h"
*
*/
#ifndef RAKLION_H
#define RAKLION_H

class Raklion: public EventCommon
{
	SingletonInstance(Raklion);

	public:
		Raklion();
		virtual ~Raklion();

		void LoadData();
		void Update();

		void ChangeState(RaklionState state);

		void SetState_None();
		void SetState_Closed();
		void SetState_Open();
		void SetState_StandBy_1();
		void SetState_StandBy_2();
		void SetState_KillSelupan();
		void SetState_End();

		void ProcState_None();
		void ProcState_Closed();
		void ProcState_Open();
		void ProcState_StandBy_1();
		void ProcState_StandBy_2();
		void ProcState_KillSelupan();
		void ProcState_End();

		void AddEggs();

		void AddSelupan();

		void AddSummoned();

		void ClearMonster();

		bool EnterAllowed(Player* pPlayer);
	private:
		DECLARE_STRUCT(TickTimer, CloseGateTime);
		DECLARE_PROPERTY(int32, EggCount);
		DECLARE_PROPERTY(int32, BossCount);
		DECLARE_ARRAY_STRUCT(RaklionStateData, StateData, RAKLION_STATE_MAX);

		DECLARE_PROPERTY(int32, SummonCount);
		DECLARE_PROPERTY(int32, SummonMaxCount);
};

#endif