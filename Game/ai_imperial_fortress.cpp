/*============================================================================================*/
/*											GATE											  */
/*============================================================================================*/

class ImperialGateScript: public MonsterScriptAI
{
public:
	explicit ImperialGateScript(): ScriptAI("imperial_fortress_gate_ai") { }
	virtual ~ImperialGateScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new ImperialGateAI(monster); }

	struct ImperialGateAI: public MonsterAI
	{
		DECLARE_ENUM(uint32, Instance);
		DECLARE_ENUM(uint8, ID);
		DECLARE_ENUM(uint8, Zone);
		
		explicit ImperialGateAI(Monster* monster): MonsterAI(monster)
		{
			this->SetInstance(0);
			this->SetID(0);
			this->SetZone(0);
		}

		virtual ~ImperialGateAI()
		{

		}

		void OnCreate()
		{
			this->SetInstance(static_cast<uint32>(me()->m_AdditionalDataInt[0]));
			this->SetID(static_cast<uint8>(me()->m_AdditionalDataInt[1]));
			this->SetZone(static_cast<uint8>(me()->m_AdditionalDataInt[2]));
		}

		bool Update()
		{
			if (!IMPERIAL_FORTRESS_ZONE_RANGE(this->GetZone()))
			{
				me()->Remove();
			}

			if (this->GetInstance() != me()->GetInstance())
			{
				me()->Remove();
			}

			if (!sImperialFortressMgr->GetInstance(this->GetInstance()))
			{
				me()->Remove();
			}

			return true;
		}

		void OnDie()
		{
			ImperialFortressInstance * pInstance = sImperialFortressMgr->GetInstance(this->GetInstance());

			if (!pInstance)
			{
				return;
			}

			if (pInstance->GetState() == IMPERIAL_FORTRESS_STATE_DOOR_1 && this->GetID() == 0)
			{
				pInstance->SetState_Fight();
				//pInstance->BlockReleaseZone(IMPERIAL_FORTRESS_ZONE_FIGHT, false);
				pInstance->BlockReleaseZone(me());
				pInstance->AddMonster(IMPERIAL_FORTRESS_TYPE_MONSTER_NORMAL);
				pInstance->AddMonster(IMPERIAL_FORTRESS_TYPE_BOSS);
				return;
			}

			if (pInstance->GetState() == IMPERIAL_FORTRESS_STATE_DOOR_2 && this->GetID() == 1)
			{
				pInstance->SetState_Door3();
				//pInstance->BlockReleaseZone(IMPERIAL_FORTRESS_ZONE_TRAP, false);
				pInstance->BlockReleaseZone(me());
				return;
			}

			if (pInstance->GetState() == IMPERIAL_FORTRESS_STATE_DOOR_3 && this->GetID() == 2)
			{
				pInstance->SetState_Move();
				//pInstance->BlockReleaseZone(IMPERIAL_FORTRESS_ZONE_GATE, false);
				pInstance->BlockReleaseZone(me());
				return;
			}
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			ImperialFortressInstance * pInstance = sImperialFortressMgr->GetInstance(this->GetInstance());

			if (!pInstance)
			{
				return true;
			}

			if (pInstance->GetState() == IMPERIAL_FORTRESS_STATE_DOOR_1 && this->GetID() == 0)
			{
				return true;
			}

			if (pInstance->GetState() == IMPERIAL_FORTRESS_STATE_DOOR_2 && this->GetID() == 1)
			{
				return true;
			}

			if (pInstance->GetState() == IMPERIAL_FORTRESS_STATE_DOOR_3 && this->GetID() == 2)
			{
				return true;
			}

			return false;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool Drop()
		{
			return true;
		}
	};
};

/*============================================================================================*/
/*											STATUE											  */
/*============================================================================================*/

class ImperialStatueScript: public MonsterScriptAI
{
public:
	explicit ImperialStatueScript(): ScriptAI("imperial_fortress_statue_ai") { }
	virtual ~ImperialStatueScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new ImperialStatueAI(monster); }

	struct ImperialStatueAI: public MonsterAI
	{
		DECLARE_ENUM(uint32, Instance);
		DECLARE_ENUM(uint8, ID);
		DECLARE_ENUM(uint8, Zone);

		explicit ImperialStatueAI(Monster* monster): MonsterAI(monster)
		{
			this->SetInstance(0);
			this->SetID(0);
			this->SetZone(0);
		}
		virtual ~ImperialStatueAI() {}

		void OnCreate()
		{
			this->SetInstance(static_cast<uint32>(me()->m_AdditionalDataInt[0]));
			this->SetID(static_cast<uint8>(me()->m_AdditionalDataInt[1]));
			this->SetZone(static_cast<uint8>(me()->m_AdditionalDataInt[2]));
		}

		bool Update()
		{
			if (this->GetInstance() != me()->GetInstance())
			{
				me()->Remove();
			}

			if (!sImperialFortressMgr->GetInstance(this->GetInstance()))
			{
				me()->Remove();
			}

			return true;
		}

		bool PushBackAllowed()
		{
			return false;
		}
	};
};

/*============================================================================================*/
/*											MONSTER											  */
/*============================================================================================*/

class ImperialMonsterScript: public MonsterScriptAI
{
public:
	explicit ImperialMonsterScript(): ScriptAI("imperial_fortress_monster_ai") { }
	virtual ~ImperialMonsterScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new ImperialMonsterAI(monster); }

	struct ImperialMonsterAI: public MonsterAI
	{
		DECLARE_ENUM(uint32, Instance);
		DECLARE_ENUM(uint8, ID);
		DECLARE_ENUM(uint8, Zone);

		explicit ImperialMonsterAI(Monster* monster): MonsterAI(monster)
		{
			this->SetInstance(0);
			this->SetID(0);
			this->SetZone(0);
		}
		virtual ~ImperialMonsterAI() {}

		bool Update()
		{
			if (!IMPERIAL_FORTRESS_ZONE_RANGE(this->GetZone()))
			{
				me()->Remove();
				return true;
			}

			if (this->GetInstance() != me()->GetInstance())
			{
				me()->Remove();
				return true;
			}

			if (!sImperialFortressMgr->GetInstance(this->GetInstance()))
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnCreate()
		{
			this->SetInstance(static_cast<uint32>(me()->m_AdditionalDataInt[0]));
			this->SetID(static_cast<uint8>(me()->m_AdditionalDataInt[1]));
			this->SetZone(static_cast<uint8>(me()->m_AdditionalDataInt[2]));
		}

		void OnDie()
		{
			ImperialFortressInstance * pInstance = sImperialFortressMgr->GetInstance(this->GetInstance());

			if (!pInstance)
			{
				return;
			}

			pInstance->ReduceMonsterCount(1);
		}

		bool MoveAllowed(int16 x, int16 y)
		{
			ImperialFortressInstance * pInstance = sImperialFortressMgr->GetInstance(this->GetInstance());

			if (!pInstance)
			{
				return false;
			}

			World* pWorld = me()->GetWorld();

			if (!pWorld)
			{
				return false;
			}

			WorldGrid const& grid = pWorld->GetGrid(x, y);

			if (grid.IsLocked_3() && !pInstance->IsZoneReleased(x, y))
			{
				return false;
			}

			return MonsterAI::MoveAllowed(x, y);
		}
	};
};

class ImperialBossScript: public MonsterScriptAI
{
public:
	explicit ImperialBossScript(): ScriptAI("imperial_fortress_boss_ai") { }
	virtual ~ImperialBossScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new ImperialBossAI(monster); }

	struct ImperialBossAI: public MonsterAI
	{
		DECLARE_STRUCT(TickTimer, UpdateTime);
		DECLARE_ENUM(uint32, Instance);
		DECLARE_ENUM(uint8, ID);
		DECLARE_ENUM(uint8, Zone);

		explicit ImperialBossAI(Monster* monster): MonsterAI(monster)
		{
			this->GetUpdateTime()->Reset();
			this->GetUpdateTime()->Start(IN_MILLISECONDS);
			this->SetInstance(0);
			this->SetID(0);
			this->SetZone(0);
		}
		virtual ~ImperialBossAI() {}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			if (!IMPERIAL_FORTRESS_ZONE_RANGE(this->GetZone()))
			{
				me()->Remove();
				return true;
			}

			if (this->GetInstance() != me()->GetInstance())
			{
				me()->Remove();
				return true;
			}

			if (!sImperialFortressMgr->GetInstance(this->GetInstance()))
			{
				me()->Remove();
				return true;
			}

			if (this->GetUpdateTime()->Elapsed())
			{
				if (me()->PowerGetPercent(POWER_LIFE) < 30 && !me()->HasBuff(BUFF_BERSERKER))
				{
					me()->AddBuff(BUFF_BERSERKER, MINUTE, 0, me());
					me()->MagicAttackSend(me()->GetEntry(), SKILL_BERSERKER);
				}
			}

			return false;
		}

		void OnCreate()
		{
			this->SetInstance(static_cast<uint32>(me()->m_AdditionalDataInt[0]));
			this->SetID(static_cast<uint8>(me()->m_AdditionalDataInt[1]));
			this->SetZone(static_cast<uint8>(me()->m_AdditionalDataInt[2]));
		}

		void FinalDamageMod(int32 & damage, bool victim)
		{
			if ( !me()->HasBuff(BUFF_BERSERKER) )
				return;

			if ( victim )
			{
				damage /= 2;
			}
			else
			{
				damage *= 2;
			}
		}

		void OnDie()
		{
			ImperialFortressInstance * pInstance = sImperialFortressMgr->GetInstance(this->GetInstance());

			if (!pInstance)
			{
				return;
			}

			pInstance->ReduceMonsterCount(1);
		}

		bool EnableAttack(Unit* pUnit, Skill*, int32 count)
		{
			if ( !pUnit )
			{
				return false;
			}

			if ( !me()->CheckWall(pUnit) )
			{
				return false;
			}

			return true;
		}

		bool MoveAllowed(int16 x, int16 y)
		{
			ImperialFortressInstance * pInstance = sImperialFortressMgr->GetInstance(this->GetInstance());

			if (!pInstance)
			{
				return false;
			}

			World* pWorld = me()->GetWorld();

			if (!pWorld)
			{
				return false;
			}

			WorldGrid const& grid = pWorld->GetGrid(x, y);

			if (grid.IsLocked_3() && !pInstance->IsZoneReleased(x, y))
			{
				return false;
			}

			return MonsterAI::MoveAllowed(x, y);
		}

		bool Drop()
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
				return false;

			uint16 item = -1;

			if( me()->GetClass() == 508 )
			{
				item = ITEMGET(14, 103);
			}
			else if( me()->GetClass() == 509 )
			{
				item = ITEMGET(14, 104);
			}
			else if( me()->GetClass() == 510 )
			{
				item = ITEMGET(14, 105);
			}
			else if( me()->GetClass() == 511 )
			{
				item = ITEMGET(14, 106);
			}
			else if( me()->GetClass() == 507 )
			{
				item = ITEMGET(14, 107);
			}
			else if( me()->GetClass() == 506 )
			{
				item = ITEMGET(14, 108);
			}

			if ( item == uint16(-1) )
				return false;

			int32 count = 0;
			int32 rand_value = Random(100);

			if( rand_value >= 0 && rand_value < 60 )
			{
				count = 1;
			}
			else if( rand_value >= 60 && rand_value < 90 )
			{
				count = 2;
			}
			else
			{
				count = 3;
			}

			Player* pMaxAttacker = me()->GetMaxAttacker();
			Unit* pOwner = pMaxAttacker ? pMaxAttacker->ToUnit(): me()->ToUnit();
			int16 x = me()->GetX();
			int16 y = me()->GetY();
			
			for( int32 i = 0; i < count; ++i )
			{
				if ( !pWorld->GetRandomDropLocation(me()->GetX(), me()->GetY(), x, y, 2, 2, 10) )
				{
					x = me()->GetX();
					y = me()->GetY();
				}

				sItemMgr->ItemSerialCreate(pOwner, pWorld->GetEntry(), x, y, Item(item), false, true, true);
			}

			return false;
		}
	};
};

void AddSC_ImperialFortress()
{
	sScriptAI->AddScriptAI(new ImperialGateScript());
	sScriptAI->AddScriptAI(new ImperialStatueScript());
	sScriptAI->AddScriptAI(new ImperialMonsterScript());
	sScriptAI->AddScriptAI(new ImperialBossScript());
}