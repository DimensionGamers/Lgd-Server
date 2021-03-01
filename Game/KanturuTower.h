/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuTower.h"
*
*/
#ifndef KANTURU_TOWER_H
#define KANTURU_TOWER_H

class KanturuTower
{
	public:
		explicit KanturuTower();
		virtual ~KanturuTower();

		void Init();
		void Load(const char * filename);
		void Update();

		void ChangeState(uint8 state);
		void SetStateNext() { this->ChangeState(this->GetState() + 1); }
		int32 GetRemainTime() const;

		void CheckStateTime();

		void SetState_None();
		void SetState_Revitalization();
		void SetState_Notify_1();
		void SetState_Close();
		void SetState_Notify_2();
		void SetState_End();

		void ProcState_None();
		void ProcState_Revitalization();
		void ProcState_Notify_1();
		void ProcState_Close();
		void ProcState_Notify_2();
		void ProcState_End();
		
	public:
		DECLARE_ARRAY_STRUCT(KanturuStateInfo, StateData, KANTURU_TOWER_OF_REFINEMENT_MAX);
		DECLARE_ENUM(uint8, State);
		DECLARE_BOOL(Entrance);
		DECLARE_BOOL(Success);
};

#endif