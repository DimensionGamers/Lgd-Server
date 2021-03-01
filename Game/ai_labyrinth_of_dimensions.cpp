class LabyrinthOfDimensionsScript: public MonsterScriptAI
{
public:
	explicit LabyrinthOfDimensionsScript(): ScriptAI("labyrinth_of_dimensions_ai") { }
	virtual ~LabyrinthOfDimensionsScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_ENUM(uint8, Index);
		DECLARE_ENUM(uint8, SubIndex);
		DECLARE_PTR(Player, Player);
		DECLARE_ENUM(uint8, Stage);
		DECLARE_ENUM(uint16, Level);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			this->SetIndex(-1);
			this->SetSubIndex(-1);
			this->SetPlayer(nullptr);
		}
		virtual ~AI() {}

		bool Update()
		{
			if ( !this->GetPlayer() )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetPlayer()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetLabyrinthData()->GetGateIndex() != this->GetIndex() ||
				this->GetPlayer()->GetLabyrinthData()->GetGateSubIndex() != this->GetSubIndex() ||
				this->GetPlayer()->GetLabyrinthData()->GetStage() != this->GetStage())
			{
				me()->Remove();
				return true;
			}

			if ( this->GetPlayer()->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE )
			{
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetLabyrinthData()->GetStatus() == 1)
			{
				me()->Remove();
				return true;
			}

			me()->SetInstance(this->GetPlayer()->GetInstance());

			return false;
		}

		void OnCreate()
		{
			this->SetPlayer((Player*)me()->m_AdditionalDataPtr[0]);
			this->SetIndex(me()->m_AdditionalDataInt[0]);
			this->SetSubIndex(me()->m_AdditionalDataInt[1]);
			this->SetStage(me()->m_AdditionalDataInt[2]);
			this->SetLevel(me()->m_AdditionalDataInt[3]);
		}

		void OnRespawn()
		{
			LabyrinthMonster const* pLabyrinthMonster = sLabyrinthDimensions->GetMonster(this->GetLevel());

			if (!pLabyrinthMonster)
			{
				return;
			}

			monster_template const* pMonsterInfo = sMonsterMgr->GetMonsterTemplate(me()->GetClass());

			if (!pMonsterInfo)
			{
				return;
			}

			me()->PowerSet(POWER_LIFE, me()->GetScriptMaxPower(POWER_LIFE) * pLabyrinthMonster->GetHP());
			me()->PowerSetMax(POWER_LIFE, me()->GetScriptMaxPower(POWER_LIFE) * pLabyrinthMonster->GetHP());

			me()->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, pMonsterInfo->attack_min_damage.get() * pLabyrinthMonster->GetDamage());
			me()->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, pMonsterInfo->attack_max_damage.get() * pLabyrinthMonster->GetDamage());

			me()->SetIntData(UNIT_INT_DEFENSE, pMonsterInfo->defense.get() * pLabyrinthMonster->GetDefense());

			me()->SetIntData(UNIT_INT_ATTACK_RATE, pMonsterInfo->attack_success.get() * pLabyrinthMonster->GetAttackSuccessRate());
			me()->SetIntData(UNIT_INT_DEFENSE_RATE, pMonsterInfo->defense_success.get() * pLabyrinthMonster->GetDefenseSuccessRate());

			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, pMonsterInfo->GetElementalDamageMin() * pLabyrinthMonster->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, pMonsterInfo->GetElementalDamageMax() * pLabyrinthMonster->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, pMonsterInfo->GetElementalDefense() * pLabyrinthMonster->GetElementalDefense());
			me()->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, pMonsterInfo->GetElementalAttackRate() * pLabyrinthMonster->GetElementalAttackSuccessRate());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, pMonsterInfo->GetElementalDefenseRate() * pLabyrinthMonster->GetElementalDefenseSuccessRate());

			me()->SetLevel(pMonsterInfo->min_level.get() * pLabyrinthMonster->GetLevel());
		}

		void OnDie()
		{
			if ( !this->GetPlayer() )
			{
				return;
			}

			this->GetPlayer()->GetLabyrinthData()->IncreaseKilledMonsters(1);

			sLabyrinthDimensions->UpdateMission(this->GetPlayer(), LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS, 1);
			sLabyrinthDimensions->UpdateMission(this->GetPlayer(), LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_TIME, 1);
			sLabyrinthDimensions->UpdateMission(this->GetPlayer(), LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_NOT_HP, 1);
			sLabyrinthDimensions->UpdateMission(this->GetPlayer(), LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_NOT_MP, 1);
		}

		bool Drop()
		{
			return true;
		}
	};
};

class LabyrinthOfDimensionsGoblinScript : public MonsterScriptAI
{
public:
	explicit LabyrinthOfDimensionsGoblinScript() : ScriptAI("labyrinth_of_dimensions_goblin_ai") { }
	virtual ~LabyrinthOfDimensionsGoblinScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		DECLARE_ENUM(uint8, Index);
		DECLARE_ENUM(uint8, SubIndex);
		DECLARE_PTR(Player, Player);
		DECLARE_ENUM(uint8, Stage);
		DECLARE_ENUM(uint16, Level);

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			this->SetIndex(-1);
			this->SetSubIndex(-1);
			this->SetPlayer(nullptr);
		}
		virtual ~AI() {}

		bool Update()
		{
			if (!this->GetPlayer())
			{
				me()->Remove();
				return true;
			}

			if (!this->GetPlayer()->IsPlaying())
			{
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetLabyrinthData()->GetGateIndex() != this->GetIndex() ||
				this->GetPlayer()->GetLabyrinthData()->GetGateSubIndex() != this->GetSubIndex() ||
				this->GetPlayer()->GetLabyrinthData()->GetStage() != this->GetStage())
			{
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
			{
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetLabyrinthData()->GetStatus() == 1)
			{
				me()->Remove();
				return true;
			}

			me()->SetInstance(this->GetPlayer()->GetInstance());

			return false;
		}

		void OnCreate()
		{
			this->SetPlayer((Player*)me()->m_AdditionalDataPtr[0]);
			this->SetIndex(me()->m_AdditionalDataInt[0]);
			this->SetSubIndex(me()->m_AdditionalDataInt[1]);
			this->SetStage(me()->m_AdditionalDataInt[2]);
			this->SetLevel(me()->m_AdditionalDataInt[3]);
		}

		void OnRespawn()
		{
			LabyrinthMonster const* pLabyrinthMonster = sLabyrinthDimensions->GetMonster(this->GetLevel());

			if (!pLabyrinthMonster)
			{
				return;
			}

			monster_template const* pMonsterInfo = sMonsterMgr->GetMonsterTemplate(me()->GetClass());

			if (!pMonsterInfo)
			{
				return;
			}

			me()->PowerSet(POWER_LIFE, me()->GetScriptMaxPower(POWER_LIFE) * pLabyrinthMonster->GetHP());
			me()->PowerSetMax(POWER_LIFE, me()->GetScriptMaxPower(POWER_LIFE) * pLabyrinthMonster->GetHP());

			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pMonsterInfo->attack_min_damage.get() * pLabyrinthMonster->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pMonsterInfo->attack_max_damage.get() * pLabyrinthMonster->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pMonsterInfo->attack_min_damage.get() * pLabyrinthMonster->GetDamage());
			me()->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pMonsterInfo->attack_max_damage.get() * pLabyrinthMonster->GetDamage());

			me()->SetIntData(UNIT_INT_DEFENSE, pMonsterInfo->defense.get() * pLabyrinthMonster->GetDefense());

			me()->SetIntData(UNIT_INT_ATTACK_RATE, pMonsterInfo->attack_success.get() * pLabyrinthMonster->GetAttackSuccessRate());
			me()->SetIntData(UNIT_INT_DEFENSE_RATE, pMonsterInfo->defense_success.get() * pLabyrinthMonster->GetDefenseSuccessRate());

			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, pMonsterInfo->GetElementalDamageMin() * pLabyrinthMonster->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, pMonsterInfo->GetElementalDamageMax() * pLabyrinthMonster->GetElementalDamage());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, pMonsterInfo->GetElementalDefense() * pLabyrinthMonster->GetElementalDefense());
			me()->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, pMonsterInfo->GetElementalAttackRate() * pLabyrinthMonster->GetElementalAttackSuccessRate());
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, pMonsterInfo->GetElementalDefenseRate() * pLabyrinthMonster->GetElementalDefenseSuccessRate());

			me()->SetLevel(pMonsterInfo->min_level.get() * pLabyrinthMonster->GetLevel());
		}

		void OnDie()
		{
			if (!this->GetPlayer())
			{
				return;
			}

			this->GetPlayer()->GetLabyrinthData()->SetGoblinState(2);
		}

		bool Drop()
		{
			return true;
		}
	};
};

void AddSC_LabyrinthOfDimensions()
{
	sScriptAI->AddScriptAI(new LabyrinthOfDimensionsScript());
	sScriptAI->AddScriptAI(new LabyrinthOfDimensionsGoblinScript());
}