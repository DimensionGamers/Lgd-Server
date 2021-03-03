/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_guard.cpp"
*
*/

class GuardScript: public MonsterScriptAI
{
public:
	explicit GuardScript(): ScriptAI("guard_ai") { }
	virtual ~GuardScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new GuardAI(monster); }

	struct GuardAI: public MonsterAI
	{
		explicit GuardAI(Monster* monster): MonsterAI(monster)
		{
			
		}
		virtual ~GuardAI() {}

		void OnRespawn()
		{
			me()->SetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, 100);
		}

		bool Update()
		{
			if ( !me()->GetTarget() && me()->GetDirection() != me()->GetOriginalDirection() )
			{
				me()->SetDirection(me()->GetOriginalDirection());
				me()->GenerateDirection();
				me()->SendDirection();
			}

			return false;
		}

		bool SearchEnemy()
		{
			Object * pObject = nullptr;
			me()->SetTarget(nullptr);

			int32 min_distance = me()->GetViewRange();

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsPlayer() || !pObject->IsPlaying() || !pObject->IsLive() )
					continue;

				if ( !this->IsTargetAllowed(pObject->ToUnit()) )
					continue;

				if ( pObject->IsInSafeZone() )
					continue;
				
				int32 dis = Util::Distance(me()->GetX(), me()->GetY(), pObject->GetX(), pObject->GetY());

				if ( dis < min_distance )
				{
					me()->SetTarget(pObject->ToUnit());
					min_distance = dis;
				}

			VIEWPORT_CLOSE

			return true;
		}

		bool ManageThreat() const { return false; }

		bool MoveAllowed(int16 x, int16 y)
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
				return false;

			if ( !pWorld->GetGrid(x, y).IsLocked_1() && !pWorld->GetGrid(x, y).IsLocked_2() )
				return true;

			return false;
		}

		void ExtraDamage(Unit* pTarget, int32 & damage)
		{
			damage = pTarget->PowerGetTotal(POWER_LIFE) * 100 / 100;
		}

		bool IsTargetAllowed(Unit* pTarget)
		{
			if ( !pTarget )
				return false;

			Player* pPlayer = pTarget->ToPlayer();

			if ( !pPlayer )
				return false;

			if ( pPlayer->GetPKLevelGroup() < PK_STATUS_MURDER_LVL_2 || pPlayer->IsAdministrator() )
				return false;

			return true;
		}
	};
};

class MonsterKillBotScript: public MonsterScriptAI
{
public:
	explicit MonsterKillBotScript(): ScriptAI("monster_kill_bot_ai") { }
	virtual ~MonsterKillBotScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new GuardAI(monster); }

	struct GuardAI: public MonsterAI
	{
		explicit GuardAI(Monster* monster): MonsterAI(monster)
		{
			this->SetFlags(MONSTER_AI_FLAG_VIEWPORT);
		}
		virtual ~GuardAI() {}

		bool Update()
		{
			if ( !me()->GetSummoner() || !me()->GetSummoner()->IsPlayer() || !me()->GetSummoner()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnRespawn()
		{
			me()->SetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, 100);

			uint8 random = Random(ELEMENTAL_ATTRIBUTE_MAX);

			me()->SetElementalAttribute(random == 0 ? ELEMENTAL_ATTRIBUTE_FIRE: random);
			me()->SetScriptElementalAttribute(ELEMENTAL_ATTRIBUTE_MAX);
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, 1000);
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, 2000);
			me()->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, 1000);
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, 2000);
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, 1000);
		}

		bool ViewportListAddConditions(Unit* pAdd)
		{
			if ( pAdd->IsPlayer() )
				return true;

			if ( pAdd->IsCreature() && (pAdd->ToCreature()->IsMonster() || pAdd->ToCreature()->IsNeutral()) )
				return true;

			return false;
		}

		bool SearchEnemy()
		{
			Object * pObject = nullptr;
			me()->SetTarget(nullptr);

			int32 min_distance = me()->GetViewRange();

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsCreature() || !pObject->IsPlaying() || !pObject->IsLive() )
					continue;

				if ( !this->IsTargetAllowed(pObject->ToUnit()) )
					continue;
				
				int32 dis = Util::Distance(me()->GetX(), me()->GetY(), pObject->GetX(), pObject->GetY());

				if ( dis < min_distance )
				{
					me()->SetTarget(pObject->ToUnit());
					min_distance = dis;
				}

			VIEWPORT_CLOSE

			return true;
		}

		bool ManageThreat() const { return false; }

		bool MoveAllowed(int16 x, int16 y)
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
				return false;

			return pWorld->GetGrid(x, y).attribute == 0;
		}

		void ExtraDamage(Unit* pTarget, int32 & damage)
		{
			//damage = pTarget->PowerGetTotal(POWER_LIFE) * 100 / 100;
		}

		void FinalDamageMod(int32 & damage, bool victim)
		{
			if ( victim )
			{
				return;
			}

			damage = 10000;
		}

		bool IsTargetAllowed(Unit* pTarget)
		{
			if ( !pTarget )
				return false;

			if ( !pTarget->IsCreature() )
				return false;

			if ( pTarget->ToCreature()->IsSummoned() && pTarget->ToCreature()->GetSummoner()->IsPlayer() )
				return false;

			if ( pTarget->ToCreature()->IsMonster() )
				return true;

			return false;
		}

		bool IsViewportControl() const { return true; }
	};
};

class SetModelScript: public MonsterScriptAI
{
public:
	explicit SetModelScript(): ScriptAI("set_model_ai") { }
	virtual ~SetModelScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new GuardAI(monster); }

	struct GuardAI: public MonsterAI
	{
		DECLARE_STRUCT(TickTimer, UpdateTime);

		explicit GuardAI(Monster* monster): MonsterAI(monster)
		{
			this->GetUpdateTime()->Reset();
			this->GetUpdateTime()->Start(2000);
		}
		virtual ~GuardAI() {}

		bool Update()
		{
			if ( this->GetUpdateTime()->Elapsed() )
			{
				uint8 next_direction = me()->GetDirection() + 1;

				if ( next_direction >= 8 )
					next_direction = 0;

				me()->SetDirection(next_direction);
				me()->SendDirection();
			}

			return false;
		}
	};
};

void AddSC_Guard()
{
	sScriptAI->AddScriptAI(new GuardScript());
	sScriptAI->AddScriptAI(new MonsterKillBotScript());
	sScriptAI->AddScriptAI(new SetModelScript());
}