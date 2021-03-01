/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_blood_castle.cpp"
*
*/

class BloodCastleMonsterBridgeScript: public MonsterScriptAI
{
public:
	explicit BloodCastleMonsterBridgeScript(): ScriptAI("blood_castle_monster_bridge_ai") { }
	virtual ~BloodCastleMonsterBridgeScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new BloodCastleAI(monster); }

	struct BloodCastleAI: public MonsterAI
	{
		explicit BloodCastleAI(Monster* monster): MonsterAI(monster) { }
		virtual ~BloodCastleAI() {}

		void OnDie()
		{
			if ( BloodCastle * Ground = sBloodCastleMgr->GetGround(me()->GetEventGround()) )
			{
				Ground->ProcessMonsterKilled(me()->GetMaxAttacker(), false);
			}
		}
	};
};

class BloodCastleMonsterBossScript: public MonsterScriptAI
{
public:
	explicit BloodCastleMonsterBossScript(): ScriptAI("blood_castle_monster_boss_ai") { }
	virtual ~BloodCastleMonsterBossScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new BloodCastleAI(monster); }

	struct BloodCastleAI: public MonsterAI
	{
		explicit BloodCastleAI(Monster* monster): MonsterAI(monster) { }
		virtual ~BloodCastleAI() {}

		void OnDie()
		{
			if ( BloodCastle * Ground = sBloodCastleMgr->GetGround(me()->GetEventGround()) )
			{
				Ground->ProcessMonsterKilled(me()->GetMaxAttacker(), true);
			}
		}
	};
};

class BloodCastleDoorScript: public MonsterScriptAI
{
public:
	explicit BloodCastleDoorScript(): ScriptAI("blood_castle_door_ai") { }
	virtual ~BloodCastleDoorScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new BloodCastleAI(monster); }

	struct BloodCastleAI: public MonsterAI
	{
		explicit BloodCastleAI(Monster* monster): MonsterAI(monster) { }
		virtual ~BloodCastleAI() {}

		void OnDie()
		{
			me()->LogKillData(false, false);

			if ( BloodCastle * Ground = sBloodCastleMgr->GetGround(me()->GetEventGround()) )
			{
				Ground->ProcessDoorDestroyed(me()->GetMaxAttacker(), me());
			}
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			if ( BloodCastle * Ground = sBloodCastleMgr->GetGround(me()->GetEventGround()) )
			{
				return Ground->GetSubState() == BLOOD_CASTLE_SUBSTATE_DOOR_KILL;
			}

			return true;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool Drop()
		{
			return true;
		}

		void OnRespawn()
		{
			BloodCastle * pGround = sBloodCastleMgr->GetGround(me()->GetEventGround());

			if ( !pGround )
				return;

			me()->PowerSet(POWER_LIFE, pGround->GetSettings()->GetDoorLife());
			me()->PowerSetMax(POWER_LIFE, pGround->GetSettings()->GetDoorLife());
			me()->SetScriptMaxPower(POWER_LIFE, pGround->GetSettings()->GetDoorLife());
		}
	};
};

class BloodCastleStatueScript: public MonsterScriptAI
{
public:
	explicit BloodCastleStatueScript(): ScriptAI("blood_castle_statue_ai") { }
	virtual ~BloodCastleStatueScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new BloodCastleAI(monster); }

	struct BloodCastleAI: public MonsterAI
	{
		explicit BloodCastleAI(Monster* monster): MonsterAI(monster) { }
		virtual ~BloodCastleAI() {}

		void OnDie()
		{
			me()->LogKillData(false, false);

			if ( BloodCastle * Ground = sBloodCastleMgr->GetGround(me()->GetEventGround()) )
			{
				Ground->ProcessStatueDestroyed(me()->GetMaxAttacker(), me());
			}
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool Drop()
		{
			return true;
		}

		void OnRespawn()
		{
			BloodCastle * pGround = sBloodCastleMgr->GetGround(me()->GetEventGround());

			if ( !pGround )
				return;

			me()->PowerSet(POWER_LIFE, pGround->GetSettings()->GetStatueLife());
			me()->PowerSetMax(POWER_LIFE, pGround->GetSettings()->GetStatueLife());
			me()->SetScriptMaxPower(POWER_LIFE, pGround->GetSettings()->GetStatueLife());
		}
	};
};

void AddSC_BloodCastle()
{
	sScriptAI->AddScriptAI(new BloodCastleMonsterBridgeScript());
	sScriptAI->AddScriptAI(new BloodCastleMonsterBossScript());
	sScriptAI->AddScriptAI(new BloodCastleDoorScript());
	sScriptAI->AddScriptAI(new BloodCastleStatueScript());
}