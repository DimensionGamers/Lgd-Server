/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuMaya.h"
*
*/
#ifndef KANTURU_MAYA_H
#define KANTURU_MAYA_H

class KanturuMaya
{
	public:
		explicit KanturuMaya();
		virtual ~KanturuMaya();

		void Init();
		void Load(const char * filename);
		void Update();

		void ChangeState(uint8 state);
		void SetStateNext() { this->ChangeState(this->GetState() + 1); }
		int32 GetRemainTime() const;

		void CheckStateTime();

		void SetState_None();
		void SetState_StandBy_1();
		void SetState_Notify_1();
		void SetState_Start_1();
		void SetState_Maya_1();
		void SetState_End_1();
		void SetState_EndCycle_1();
		void SetState_StandBy_2();
		void SetState_Start_2();
		void SetState_Maya_2();
		void SetState_End_2();
		void SetState_EndCycle_2();
		void SetState_StandBy_3();
		void SetState_Start_3();
		void SetState_Maya_3();
		void SetState_End_3();
		void SetState_EndCycle_3();
		void SetState_End();
		void SetState_EndCycle();

		void ProcState_None();
		void ProcState_StandBy_1();
		void ProcState_Notify_1();
		void ProcState_Start_1();
		void ProcState_Maya_1();
		void ProcState_End_1();
		void ProcState_EndCycle_1();
		void ProcState_StandBy_2();
		void ProcState_Start_2();
		void ProcState_Maya_2();
		void ProcState_End_2();
		void ProcState_EndCycle_2();
		void ProcState_StandBy_3();
		void ProcState_Start_3();
		void ProcState_Maya_3();
		void ProcState_End_3();
		void ProcState_EndCycle_3();
		void ProcState_End();
		void ProcState_EndCycle();

		void UserUpdate();
	public:
		DECLARE_ARRAY_STRUCT(KanturuStateInfo, StateData, KANTURU_MAYA_MAX);
		DECLARE_ARRAY_STRUCT(KanturuStateInfo, SceneData, KANTURU_MAYA_SCENE_MAX);
		DECLARE_ENUM(int32, SceneState);
		DECLARE_BOOL(SceneSuccess);
		DECLARE(uint32, m_maya_tick);
		DECLARE_ENUM(uint8, State);
		DECLARE_BOOL(Entrance);
		DECLARE_BOOL(Success);
};

#endif