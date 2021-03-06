class InstancedDungeonNPC : public MonsterScriptAI
{
public:
	explicit InstancedDungeonNPC() : ScriptAI("ai_instanced_dungeon_npc") { }
	virtual ~InstancedDungeonNPC() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		explicit AI(Monster* monster) : MonsterAI(monster)
		{

		}
		virtual ~AI()
		{

		}

		bool OnTalk(Player* pPlayer)
		{
			if (!pPlayer)
			{
				return true;
			}

			sDungeon->EnterRequest(pPlayer);
			return true;
		}
	};
};

class InstancedDungeonBoss : public MonsterScriptAI
{
public:
	explicit InstancedDungeonBoss() : ScriptAI("ai_instanced_dungeon_boss") { }
	virtual ~InstancedDungeonBoss() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		DECLARE_ENUM(uint16, Level);
		DECLARE_ENUM(int32, Instance);

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			this->SetLevel(-1);
			this->SetInstance(-1);
		}
		virtual ~AI()
		{

		}

		bool Update()
		{
			DungeonInstance* pInstance = sDungeon->FindInstance(this->GetInstance());

			if (!pInstance)
			{
				me()->Remove();
				return true;
			}

			if (me()->GetInstance() != this->GetInstance())
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnCreate()
		{
			this->SetInstance(me()->m_AdditionalDataInt[0]);
			this->SetLevel(me()->m_AdditionalDataInt[1]);
		}

		void OnRespawn()
		{
			const auto instance_level_data = sDungeon->GetInstanceLevel(GetLevel());
			if (!instance_level_data)
				return;

			me()->PowerSet(POWER_LIFE, me()->GetMonsterTemplate()->Stat[POWER_LIFE] * instance_level_data->GetHP());
			me()->PowerSetMax(POWER_LIFE, me()->GetMonsterTemplate()->Stat[POWER_LIFE] * instance_level_data->GetHP());

			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, me()->GetMonsterTemplate()->DamageMin * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, me()->GetMonsterTemplate()->DamageMax * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, me()->GetMonsterTemplate()->DamageMin * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, me()->GetMonsterTemplate()->DamageMax * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_DEFENSE, me()->GetMonsterTemplate()->Defense * instance_level_data->GetDefense());
			me()->SetIntData(UNIT_INT_ATTACK_RATE, me()->GetMonsterTemplate()->AttackSuccessRate * instance_level_data->GetAttackSuccess());
			me()->SetIntData(UNIT_INT_DEFENSE_RATE, me()->GetMonsterTemplate()->DefenseSuccessRate * instance_level_data->GetDefenseSuccess());

			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, me()->GetMonsterTemplate()->ElementalDamageMin * instance_level_data->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, me()->GetMonsterTemplate()->ElementalDamageMax * instance_level_data->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, me()->GetMonsterTemplate()->ElementalDefense * instance_level_data->GetElementalDefense());
			me()->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, me()->GetMonsterTemplate()->ElementalAttackSuccessRate * instance_level_data->GetElementalAttackSuccess());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, me()->GetMonsterTemplate()->ElementalDefenseSuccessRate * instance_level_data->GetElementalDefenseSuccess());

			me()->SetLevel(me()->GetMonsterTemplate()->Level * instance_level_data->GetLevel());
		}

		void OnDie()
		{
			DungeonInstance* pInstance = sDungeon->FindInstance(this->GetInstance());

			if (pInstance)
			{
				pInstance->BossKill(1);
			}
		}

		bool Drop()
		{
			return true;
		}

		bool InmuneToRadiance() const
		{
			return true;
		}

		bool InmuneToDebuffBlow(bool blow) const
		{
			if (blow)
			{
				if (sGameServer->IsBossDebuffBlow(2))
				{
					return true;
				}
			}
			else
			{
				if (sGameServer->IsBossDebuffBlow(1))
				{
					return true;
				}
			}

			return false;
		}
	};
};

class InstancedDungeonMonster : public MonsterScriptAI
{
public:
	explicit InstancedDungeonMonster() : ScriptAI("ai_instanced_dungeon_monster") { }
	virtual ~InstancedDungeonMonster() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		DECLARE_ENUM(uint16, Level);
		DECLARE_ENUM(int32, Instance);

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			this->SetLevel(-1);
			this->SetInstance(-1);
		}
		virtual ~AI()
		{

		}

		bool Update()
		{
			DungeonInstance* pInstance = sDungeon->FindInstance(this->GetInstance());

			if (!pInstance)
			{
				me()->Remove();
				return true;
			}

			if (me()->GetInstance() != this->GetInstance())
			{
				me()->Remove();
				return true;
			}

			if (pInstance->GetBossKilled() != 0)
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnCreate()
		{
			this->SetInstance(me()->m_AdditionalDataInt[0]);
			this->SetLevel(me()->m_AdditionalDataInt[1]);
		}

		void OnRespawn()
		{
			auto const instance_level_data = sDungeon->GetInstanceLevel(GetLevel());
			if (!instance_level_data)
				return;

			me()->PowerSet(POWER_LIFE, me()->GetMonsterTemplate()->Stat[POWER_LIFE] * instance_level_data->GetHP());
			me()->PowerSetMax(POWER_LIFE, me()->GetMonsterTemplate()->Stat[POWER_LIFE] * instance_level_data->GetHP());

			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, me()->GetMonsterTemplate()->DamageMin * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, me()->GetMonsterTemplate()->DamageMax * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, me()->GetMonsterTemplate()->DamageMin * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, me()->GetMonsterTemplate()->DamageMax * instance_level_data->GetDamage());
			me()->SetIntData(UNIT_INT_DEFENSE, me()->GetMonsterTemplate()->Defense * instance_level_data->GetDefense());
			me()->SetIntData(UNIT_INT_ATTACK_RATE, me()->GetMonsterTemplate()->AttackSuccessRate * instance_level_data->GetAttackSuccess());
			me()->SetIntData(UNIT_INT_DEFENSE_RATE, me()->GetMonsterTemplate()->DefenseSuccessRate * instance_level_data->GetDefenseSuccess());

			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, me()->GetMonsterTemplate()->ElementalDamageMin * instance_level_data->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, me()->GetMonsterTemplate()->ElementalDamageMax * instance_level_data->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, me()->GetMonsterTemplate()->ElementalDefense * instance_level_data->GetElementalDefense());
			me()->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, me()->GetMonsterTemplate()->ElementalAttackSuccessRate * instance_level_data->GetElementalAttackSuccess());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, me()->GetMonsterTemplate()->ElementalDefenseSuccessRate * instance_level_data->GetElementalDefenseSuccess());

			me()->SetLevel(me()->GetMonsterTemplate()->Level * instance_level_data->GetLevel());
		}

		void OnDie()
		{
			DungeonInstance* pInstance = sDungeon->FindInstance(this->GetInstance());

			if (pInstance)
			{
				pInstance->IncreaseKilledMonsters(1);
			}
		}

		bool Drop()
		{
			return true;
		}

		bool InmuneToRadiance() const
		{
			return true;
		}

		bool InmuneToDebuffBlow(bool blow) const
		{
			if (blow)
			{
				if (sGameServer->IsBossDebuffBlow(2))
				{
					return true;
				}
			}
			else
			{
				if (sGameServer->IsBossDebuffBlow(1))
				{
					return true;
				}
			}

			return false;
		}
	};
};

class InstancedDungeonChest : public MonsterScriptAI
{
public:
	explicit InstancedDungeonChest() : ScriptAI("ai_instanced_dungeon_chest") { }
	virtual ~InstancedDungeonChest() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		DECLARE_ENUM(int32, Instance);
		DECLARE_ENUM(uint8, Type);
		DECLARE_ENUM(uint8, ID);

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			this->SetInstance(-1);
		}
		virtual ~AI()
		{

		}

		void OnCreate()
		{
			this->SetInstance(me()->m_AdditionalDataInt[0]);
			this->SetType(me()->m_AdditionalDataInt[1]);
			this->SetID(me()->m_AdditionalDataInt[2]);
		}

		bool Update()
		{
			DungeonInstance* pInstance = sDungeon->FindInstance(this->GetInstance());

			if (!pInstance)
			{
				me()->Remove();
				return true;
			}

			if (me()->GetInstance() != this->GetInstance())
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		bool OnTalk(Player* pPlayer)
		{
			if (!pPlayer)
			{
				return true;
			}

			DungeonInstance* pInstance = sDungeon->FindInstance(this->GetInstance());

			if (!pInstance)
			{
				return true;
			}

			pInstance->ChestOpen(this->GetType(), this->GetID());
			me()->Remove();
					
			return true;
		}
	};
};

class InstancedDungeonTrap : public MonsterScriptAI
{
public:
	explicit InstancedDungeonTrap() : ScriptAI("ai_instanced_dungeon_trap") { }
	virtual ~InstancedDungeonTrap() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		DECLARE_ENUM(int32, Instance);
		DECLARE_ENUM(uint8, Type);

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			this->SetInstance(-1);
			this->CreateTimer(0);
		}
		virtual ~AI()
		{

		}

		bool Update()
		{
			DungeonInstance* pInstance = sDungeon->FindInstance(this->GetInstance());

			if (!pInstance)
			{
				me()->Remove();
				return true;
			}

			if (me()->GetInstance() != this->GetInstance())
			{
				me()->Remove();
				return true;
			}

			if (pInstance->GetBossKilled() != 0)
			{
				me()->Remove();
				return true;
			}

			switch (this->GetType())
			{
			case 0:
				{
					this->SearchForTargets();
				} break;
			}

			return false;
		}

		void SearchForTargets()
		{
			if (!this->IsTimerElapsed(0, IN_MILLISECONDS))
			{
				return;
			}

			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if (!pObject->IsUnit())
					continue;

				if (pObject->IsCreature() && (!pObject->ToUnit()->GetSummoner() || !pObject->ToUnit()->GetSummoner()->IsPlayer()))
					continue;

				if (!IN_RANGE(me(), pObject, 2))
					continue;

				me()->AttackProc(pObject->ToUnit(), nullptr, false, pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * sGameServer->GetDungeonSimpleTrapDamage() / 100, 0, DAMAGE_TYPE_DEBUFF);
				pObject->ToUnit()->AddBuff(BUFF_FLAME_STRIKE, 1, 0, me());
			
			VIEWPORT_CLOSE
		}

		void OnCreate()
		{
			this->SetInstance(me()->m_AdditionalDataInt[0]);
			this->SetType(me()->m_AdditionalDataInt[1]);
		}
	};
};

void AddSC_InstancedDungeonNPC()
{
	sScriptAI->AddScriptAI(new InstancedDungeonNPC());
	sScriptAI->AddScriptAI(new InstancedDungeonBoss());
	sScriptAI->AddScriptAI(new InstancedDungeonMonster());
	sScriptAI->AddScriptAI(new InstancedDungeonChest());
	sScriptAI->AddScriptAI(new InstancedDungeonTrap());
}