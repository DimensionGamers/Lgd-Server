/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_trap.cpp"
*
*/

class TrapScript: public MonsterScriptAI
{
public:
	explicit TrapScript(): ScriptAI("trap_ai") { }
	virtual ~TrapScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new TrapAI(monster); }

	struct TrapAI: public MonsterAI
	{
		explicit TrapAI(Monster* monster): MonsterAI(monster) { }
		virtual ~TrapAI() {}

		bool Update()
		{
			if ( !me()->GetAttackRange() )
			{
				this->EnemySearch();
			}
			else
			{
				if ( me()->GetDirection() == 3 )
				{
					this->EnemySearchX();
				}
				else if ( me()->GetDirection() == 1 )
				{
					this->EnemySearchY();
				}
				else //if ( this->GetDirection() == 8 )
				{
					this->EnemySearchRange();
				}
			}

			if ( me()->GetTarget() )
			{
				me()->GetAction()->Attack = 1;
				me()->Attack();
			}

			me()->SetNextActionTime(me()->GetIntData(UNIT_INT_ATTACK_SPEED));

			return true;
		}

		bool ManageThreat() const { return false; }
		
		void EnemySearch()
		{
			Object * pObject = nullptr;
			me()->SetTarget(nullptr);

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsPlayer() || !pObject->IsLive() || pObject->ToPlayer()->IsTeleporting() )
					continue;

				if ( pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( pObject->GetX() != me()->GetX() || pObject->GetY() != me()->GetY() )
					continue;

				me()->SetTarget(pObject->ToUnit());
				break;
	
			VIEWPORT_CLOSE
		}
	
		void EnemySearchX()
		{
			Object * pObject = nullptr;
			me()->SetTarget(nullptr);
			int16 pos = me()->GetX();
			int32 count = me()->GetAttackRange() + 1;

			for( int32 n = 0; n < count; ++n )
			{
				pos++;

				VIEWPORT_LOOP_OBJECT(me(), pObject)

					if ( !pObject->IsPlayer() || !pObject->ToPlayer()->IsLive() || pObject->ToPlayer()->IsTeleporting() )
						continue;

					if ( pObject->ToPlayer()->IsAdministrator() )
						continue;

					if ( pObject->GetX() != pos || pObject->GetY() != me()->GetY() )
						continue;

					me()->SetTarget(pObject->ToUnit());
					break;

				VIEWPORT_CLOSE
			}
		}
	
		void EnemySearchY()
		{
			Object * pObject = nullptr;
			me()->SetTarget(nullptr);
			int16 pos = me()->GetY();
			int32 count = me()->GetAttackRange() + 1;

			for( int32 n = 0; n < count; ++n )
			{
				pos++;

				VIEWPORT_LOOP_OBJECT(me(), pObject)

					if ( !pObject->IsPlayer() || !pObject->ToPlayer()->IsLive() || pObject->ToPlayer()->IsTeleporting() )
						continue;

					if ( pObject->ToPlayer()->IsAdministrator() )
						continue;

					if ( pObject->GetX() != me()->GetX() || pObject->GetY() != pos )
						continue;

					me()->SetTarget(pObject->ToUnit());
					break;

				VIEWPORT_CLOSE
			}
		}
	
		void EnemySearchRange()
		{
			Object * pObject = nullptr;
			me()->SetTarget(nullptr);

			int32 rate = 100 / (me()->GetViewportCount() + 1);

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsPlayer() || !pObject->ToPlayer()->IsLive() || pObject->ToPlayer()->IsTeleporting() )
					continue;

				if ( pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( (me()->GetY() - me()->GetAttackRange()) > pObject->GetY() || (me()->GetY() + me()->GetAttackRange()) < pObject->GetY() )
					continue;

				if ( (me()->GetX() - me()->GetAttackRange()) > pObject->GetX() || (me()->GetX() + me()->GetAttackRange()) < pObject->GetX() )
					continue;

				me()->SetTarget(pObject->ToPlayer());

				if (roll_chance_i(rate))
					break;

			VIEWPORT_CLOSE
		}
	};
};

void AddSC_Trap()
{
	sScriptAI->AddScriptAI(new TrapScript());
}