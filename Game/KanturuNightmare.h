/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuNightmare.h"
*
*/
#ifndef KANTURU_NIGHTMARE_H
#define KANTURU_NIGHTMARE_H

class KanturuNightmare
{
	public:
		explicit KanturuNightmare();
		virtual ~KanturuNightmare();

		void Init();
		void Load(const char * filename);
		void Update();

		void ChangeState(uint8 state);
		void SetStateNext() { this->ChangeState(this->GetState() + 1); }
		int32 GetRemainTime() const;

		void CheckStateTime();

		void SetState_None();
		void SetState_Idle();
		void SetState_Notify();
		void SetState_Start();
		void SetState_End();
		void SetState_EndCycle();

		void ProcState_None();
		void ProcState_Idle();
		void ProcState_Notify();
		void ProcState_Start();
		void ProcState_End();
		void ProcState_EndCycle();

		void UserUpdate();
	public:
		DECLARE_ARRAY_STRUCT(KanturuStateInfo, StateData, KANTURU_NIGHTMARE_MAX);
		DECLARE_ENUM(uint8, State);
		DECLARE_BOOL(Entrance);
		DECLARE_BOOL(Success);
};

#endif