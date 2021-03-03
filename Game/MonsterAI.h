/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterAI.h"
*
*/

#ifndef MONSTER_AI_H
#define MONSTER_AI_H

enum MonsterAIFlag
{
	MONSTER_AI_FLAG_NONE,
	MONSTER_AI_FLAG_VIEWPORT,
};

struct MonsterAI
{
	friend class Monster;

	public:
		explicit MonsterAI(Monster*);
		virtual ~MonsterAI();

		virtual bool Update();
		virtual bool UpdateMovement();
		virtual bool MoveAttempt();
		virtual bool MoveAllowed(int16 x, int16 y);

		virtual void OnCreate() { }
		virtual void OnRespawn() { }
		virtual void OnDie() { }
		virtual void OnKillUnit(Unit*) { }
		virtual bool OnAttack(Unit*, bool common = true);
		virtual void OnBeenAttacked(Unit*) { }
		virtual bool EnableAttack(Unit*, Skill*, int32 count = 0);
		virtual bool SearchEnemy();
		virtual bool ManageThreat() const { return true; }
		virtual bool ViewportListRemoveConditions(Unit* pRemove) { return false; }
		virtual bool ViewportListAddConditions(Unit* pAdd) { return true; }
		virtual bool Drop();
		virtual bool OnTalk(Player*);
		virtual bool OnItemBuy(Player*, uint8);
		virtual bool PushBackAllowed();
		virtual bool UpdateLocation();
		virtual bool NormalAttack();
		virtual bool MagicAttack();
		virtual bool SpecialMagicAttack();
		virtual void ExtraDamage(Unit* pTarget, int32 & damage) { }
		virtual bool AddAggro(Player* pPlayer, int64 aggro) { return true; }
		virtual void OnDespawn() { }
		virtual bool IgnoreTarget() { return false; }
		virtual bool StopRespawn() { return false; }
		virtual bool SendHPBar() { return true; }
		virtual bool IsTargetAllowed(Unit*) { return true; }
		virtual bool IsViewportControl() const { return false; }
		virtual bool IsStopAction() const { return false; }
		virtual void EnteringVP(Unit* pUnit) { }
		virtual bool InmuneToRadiance() const { return false; }
		virtual bool InmuneToDebuffBlow(bool blow) const { return false; }

		virtual void PushBackPlayers(bool live);
						
		Monster* me() { return this->pMEMonster; }
		Monster const* me() const { return this->pMEMonster; }

		bool HasFlag(uint32 flag) const { return this->GetFlags() & flag; }

		virtual void phaseSet(uint32 phase) { this->m_phase.set(phase); }
		uint32 phaseGet() const { return this->m_phase.get(); }

		virtual void phaseTimeSet(uint32 time) { this->m_phase_time.set(time + GetTickCount()); }
		uint32 phaseTimeGet() const { return this->m_phase_time.get(); }

		bool phaseTimeElapsed() const { return ( GetTickCount() > this->phaseTimeGet() ); }

		virtual void phaseSetNext() { this->phaseSet(this->phaseGet() + 1); }

		virtual void FinalDamageMod(int32 & damage, bool victim) { }

		void CreateTimer(uint32 id);
		void StartTimer(uint32 id, uint32 time = 0);
		bool IsTimerElapsed(uint32 id, uint32 time = 0);
	protected:
		Monster * pMEMonster;
		DECLARE_PROPERTY(uint32, Flags);
		DECLARE(uint32, m_phase);
		DECLARE(uint32, m_phase_time);

		typedef std::map<uint32, TickTimer> AITimerMap;
		AITimerMap m_TimerMap;
};

#endif