/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuStandBy.h"
*
*/
#ifndef KANTURU_STAND_BY_H
#define KANTURU_STAND_BY_H

class KanturuStandBy
{
	public:
		explicit KanturuStandBy();
		virtual ~KanturuStandBy();

		void Init();
		void Load(const char * filename);
		void Update();

		void ChangeState(uint8 state);
		void SetStateNext() { this->ChangeState(this->GetState() + 1); }
		int32 GetRemainTime() const;

		void CheckStateTime();

		void SetState_None();
		void SetState_Start();
		void SetState_Notify();
		void SetState_End();

		void ProcState_None();
		void ProcState_Start();
		void ProcState_Notify();
		void ProcState_End();
	
	public:
		DECLARE_ARRAY_STRUCT(KanturuStateInfo, StateData, KANTURU_STANBY_MAX);
		DECLARE_ENUM(uint8, State);
		DECLARE_BOOL(Entrance);
		DECLARE_BOOL(Success);
};

#endif