/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Kanturu.h"
*
*/
#ifndef KANTURU_H
#define KANTURU_H

class KanturuMgr
{
	SingletonInstance(KanturuMgr);

	public:
		KanturuMgr();
		virtual ~KanturuMgr();

		void Init();
		void Load();

		uint8 GetSubState() const;

		void ChangeState(uint8 state);
		void CheckStateTime();
		void UpdateTime();

		void EntranceInfoNotify(Player* pPlayer);
		void EnterRequest(Player* pPlayer);
		void EnterResult(Player* pPlayer, uint8 result);
		bool AddPlayer(Player* pPlayer);
		void StateNofity(Player* pPlayer);
		void StateChange();
		void SetStateNext() { this->ChangeState(this->GetState() + 1); }
		void MonsterAndUserNotify();
		void sendNoticeToPlayersOnMap(bool map_only, const char * notice, ...);
		void SendNotice(const char * notice, ...);
		void SendTime(int32 time);
		void SendSuccess(bool success);
		void SendWideAttack(uint16 entry, uint8 type);

		void InitUser(bool move = true);
		void UpdateUser();
		void MoveToGate(uint16 gate);
		int32 GetUserCount() const;
		int32 GetRemainTime() const;
		bool IsEntrance() const;

		void AddMonster();
		void KillMonster() { this->ReduceMonsterCount(1); }

		void AddSummonedMonster();
	private:
		DECLARE_ENUM(uint8, State);
		DECLARE_ARRAY_STRUCT(KanturuStateInfo, StateInfo, KANTURU_STATE_MAX);
		DECLARE_PROPERTY(int32, MonsterCount);
		DECLARE_BOOL(NightmareAlive);

		DECLARE_STRUCT(KanturuStandBy, KanturuStandBy);
		DECLARE_STRUCT(KanturuMaya, KanturuMaya);
		DECLARE_STRUCT(KanturuNightmare, KanturuNightmare);
		DECLARE_STRUCT(KanturuTower, KanturuTower);

		void SetState_None();
		void SetState_BattleStandBy();
		void SetState_BattleOfMaya();
		void SetState_BattleOfNightmare();
		void SetState_TowerOfRefinement();
		void SetState_End();

		void ProcState_None();
		void ProcState_BattleStandBy();
		void ProcState_BattleOfMaya();
		void ProcState_BattleOfNightmare();
		void ProcState_TowerOfRefinement();
		void ProcState_End();
};

#endif