class GroundDarknessScript: public MonsterScriptAI
{
public:
	explicit GroundDarknessScript(): ScriptAI("ground_darkness_ai") { }
	virtual ~GroundDarknessScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new GroundDarknessAI(monster); }

	struct GroundDarknessAI: public MonsterAI
	{
		DECLARE_STRUCT(TickTimer, AttackTime);

		explicit GroundDarknessAI(Monster* monster): MonsterAI(monster)
		{
			this->GetAttackTime()->Start();
		}

		virtual ~GroundDarknessAI() {}

		bool Update()
		{
			if ( this->GetAttackTime()->Elapsed(3000) )
			{
				this->DealDarknessDamage();
			}

			return true;
		}
				
		void DealDarknessDamage()
		{
			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsUnit() )
					continue;

				if ( pObject->IsInSafeZone() )
					continue;

				if ( pObject->IsPlayer() && pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( pObject->IsCreature() && (!pObject->ToUnit()->GetSummoner() || !pObject->ToUnit()->GetSummoner()->IsPlayer()) )
					continue;

				if ( !IN_RANGE(me(), pObject, 4) )
					continue;

				pObject->ToUnit()->Hit(me(), pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * 25 / 100, 0, DAMAGE_TYPE_DEBUFF, true);

			VIEWPORT_CLOSE
		}

		bool EnableAttack(Unit* pAttacker, Skill* pSkill, int32 count)
		{
			return false;
		}

		bool SendHPBar()
		{
			return false;
		}

		bool PushBackAllowed()
		{
			return false;
		}
	};
};

void AddSC_GroundDarkness()
{
	sScriptAI->AddScriptAI(new GroundDarknessScript());
}