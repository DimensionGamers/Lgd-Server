class SwampOfDarknessPetFireScript : public MonsterScriptAI
{
public:
	explicit SwampOfDarknessPetFireScript() : ScriptAI("swamp_of_darkness_pet_fire_ai") { }
	virtual ~SwampOfDarknessPetFireScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI : public MonsterAI
	{
		explicit MobAI(Monster* monster) : MonsterAI(monster)
		{
			this->CreateTimer(0);
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if (sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_1 &&
				sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_2)
			{
				me()->Remove();
				return true;
			}

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if (!this->IsTimerElapsed(0))
			{
				return true;
			}

			this->CreateTimer(0);
			return false;
		}

		void OnRespawn()
		{
			this->StartTimer(0);
		}

		bool Drop()
		{
			return true;
		}

		bool OnAttack(Unit* pTarget, bool common)
		{
			if (roll_chance_i(5))
			{
				this->SkillBreath();
			}
			else
			{
				this->SkillNormal();
			}

			return true;
		}

		void SkillNormal()
		{
			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			me()->AddDelayed(DELAYED_ATTACK, 500, pTarget);
			this->StartTimer(0, 2000);
		}

		void SkillBreath()
		{
			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			sSwampOfDarkness->SendAttack(me(), 2);
			this->StartTimer(0, 3000);

			me()->AddDelayed(DELAYED_SWAMP_OF_DARKNESS_PET, 500, nullptr, pTarget->GetX(), pTarget->GetY(), 1, 0);
		}
	};
};

class SwampOfDarknessPetIceScript : public MonsterScriptAI
{
public:
	explicit SwampOfDarknessPetIceScript() : ScriptAI("swamp_of_darkness_pet_ice_ai") { }
	virtual ~SwampOfDarknessPetIceScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI : public MonsterAI
	{
		explicit MobAI(Monster* monster) : MonsterAI(monster)
		{
			this->CreateTimer(0);
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if (sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_1 &&
				sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_2)
			{
				me()->Remove();
				return true;
			}

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if (!this->IsTimerElapsed(0))
			{
				return true;
			}

			this->CreateTimer(0);
			return false;
		}

		void OnRespawn()
		{
			this->StartTimer(0);
		}

		bool Drop()
		{
			return true;
		}

		bool OnAttack(Unit* pTarget, bool common)
		{
			if (roll_chance_i(5))
			{
				this->SkillBreath();
			}
			else
			{
				this->SkillNormal();
			}

			return true;
		}

		void SkillNormal()
		{
			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			me()->AddDelayed(DELAYED_ATTACK, 500, pTarget);
			this->StartTimer(0, 2000);
		}

		void SkillBreath()
		{
			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			sSwampOfDarkness->SendAttack(me(), 2);
			this->StartTimer(0, 3000);

			me()->AddDelayed(DELAYED_SWAMP_OF_DARKNESS_PET, 500, nullptr, pTarget->GetX(), pTarget->GetY(), 1, 0);
		}
	};
};

#define SWAMP_OF_DARKNESS_PET_SUMMON		15

class SwampOfDarknessMonsterScript : public MonsterScriptAI
{
public:
	explicit SwampOfDarknessMonsterScript() : ScriptAI("swamp_of_darkness_monster_ai") { }
	virtual ~SwampOfDarknessMonsterScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI : public MonsterAI
	{
		Monster* Pet[SWAMP_OF_DARKNESS_PET_SUMMON];

		explicit MobAI(Monster* monster) : MonsterAI(monster)
		{
			this->CreateTimer(0);
			this->CreateTimer(1);

			for (int32 i = 0; i < SWAMP_OF_DARKNESS_PET_SUMMON; ++i)
			{
				this->Pet[i] = nullptr;
			}
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if (sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_1 &&
				sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_2)
			{
				me()->Remove();
				return true;
			}

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			if (!this->IsTimerElapsed(0))
			{
				return true;
			}

			this->CreateTimer(0);
			return false;
		}

		void OnRespawn()
		{
			this->StartTimer(0);
			this->StartTimer(1);
		}

		bool PushBackAllowed()
		{
			return false;
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

		bool OnAttack(Unit* pTarget, bool common)
		{
			if (me()->GetPathData()->IsStartEnd())
			{
				return true;
			}

			if ((MyGetTickCount() - me()->GetLastMoveTime()) < 500)
			{
				return true;
			}

			if (this->IsTimerElapsed(1, 20000))
			{
				this->SkillSummon();
			}
			else
			{
				this->SkillNormal();
			}

			return true;
		}

		void SkillNormal()
		{
			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			me()->NormalAttack();
			this->StartTimer(0, 2000);
		}

		void SkillSummon()
		{
			World* pWorld = sWorldMgr->GetWorld(WORLD_SWAMP_OF_DARKNESS);

			if (!pWorld)
			{
				this->SkillNormal();
				return;
			}

			this->StartTimer(1);

			sSwampOfDarkness->SendAttack(me(), 1);
			this->StartTimer(0, 4000);

			for (int32 i = 0; i < SWAMP_OF_DARKNESS_PET_SUMMON; ++i)
			{
				Monster* pMonster = this->Pet[i];

				if (!pMonster || !pMonster->IsLive() || !pMonster->IsPlaying())
				{
					this->Pet[i] = nullptr;
				}
			}

			for (int32 i = 0; i < SWAMP_OF_DARKNESS_PET_SUMMON; ++i)
			{
				if (this->Pet[i])
				{
					continue;
				}

				Monster* pMonster = sObjectMgr->MonsterTryAdd(Random(2) + 791, me()->GetWorldId());

				if (pMonster)
				{
					coord_type x = me()->GetX();
					coord_type y = me()->GetY();

					pWorld->GetRandomLocation(x, y, 15);

					pMonster->SetWorldId(me()->GetWorldId());
					pMonster->SetBasicLocation(x, y, x, y);
					pMonster->SetMoveDistance(30);
					pMonster->SetRespawn(IN_MILLISECONDS);
					pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					pMonster->AddToWorld();

					this->Pet[i] = pMonster;
				}
			}
		}
	};
};

class SwampOfDarknessWaterMonsterScript : public MonsterScriptAI
{
public:
	explicit SwampOfDarknessWaterMonsterScript() : ScriptAI("swamp_of_darkness_water_monster_ai") { }
	virtual ~SwampOfDarknessWaterMonsterScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI : public MonsterAI
	{
		DECLARE_ENUM(uint8, WaterSourceState);
		DECLARE_BOOL(BossCreated);

		explicit MobAI(Monster* monster) : MonsterAI(monster)
		{
			this->CreateTimer(0);
			this->CreateTimer(1);
			this->CreateTimer(2);
			this->CreateTimer(3);
			this->SetBossCreated(false);
			this->SetWaterSourceState(false);
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if (sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_1)
			{
				me()->Remove();
				return true;
			}

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			this->BossAppear();

			if (this->IsBossCreated())
			{
				if (this->IsTimerElapsed(1))
				{
					me()->Remove();
				}
				return true;
			}

			this->UpdateWaterSource();

			this->PushBackPlayers(true);

			return false;
		}

		void OnDie()
		{
			this->BossAppear();
		}

		void OnRespawn()
		{
			this->StartTimer(0);
			this->StartTimer(1);
			this->StartTimer(2);
			this->StartTimer(3);
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool InmuneToRadiance() const
		{
			return true;
		}

		bool Drop()
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

		void UpdateWaterSource()
		{
			if (!this->GetWaterSourceState())
			{
				return;
			}

			switch (this->GetWaterSourceState())
			{
			case 1:
				{
					if (Util::Distance(me()->GetX(), me()->GetY(), g_SwampOfDarknessWaterSource[0].x,  g_SwampOfDarknessWaterSource[0].y) > 1)
					{
						me()->MoveToLocation(g_SwampOfDarknessWaterSource[0].x, g_SwampOfDarknessWaterSource[0].y);
						return;
					}

					if (me()->GetPathData()->IsStartEnd())
					{
						return;
					}

					if ((MyGetTickCount() - me()->GetLastMoveTime()) < 500)
					{
						return;
					}

					this->SetWaterSourceState(2);
					this->StartTimer(0, 5000);

					sSwampOfDarkness->SendAttack(me(), 5);
				} break;

			case 2:
			case 3:
			case 4:
				{
					if (this->IsTimerElapsed(3, 1000))
					{
						this->SetWaterSourceState(this->GetWaterSourceState() + 1);
						me()->PowerIncrease(POWER_LIFE, me()->PowerGetTotal(POWER_LIFE) * 5 / 100, true);
					}
				} break;

			default:
				{
					this->SetWaterSourceState(0);
				} break;
			}
		}

		void BossAppear()
		{
			sSwampOfDarkness->SetWaterMonsterX(me()->GetX());
			sSwampOfDarkness->SetWaterMonsterY(me()->GetY());

			if (me()->PowerGetPercent(POWER_LIFE) < 20)
			{
				if (sSwampOfDarkness->GetState() == SWAMP_OF_DARKNESS_STATE_BATTLE_1)
				{
					sSwampOfDarkness->ChangeState(SWAMP_OF_DARKNESS_STATE_BATTLE_2);
					this->SetBossCreated(true);
					this->StartTimer(1, 2000);
				}
			}
		}

		bool OnAttack(Unit* pTarget, bool common)
		{
			if (me()->GetPathData()->IsStartEnd())
			{
				return true;
			}

			if ((MyGetTickCount() - me()->GetLastMoveTime()) < 500)
			{
				return true;
			}

			if (this->GetWaterSourceState())
			{
				return true;
			}

			int32 skill = Random(4);

			switch (skill)
			{
			case 0:
				{
					this->SkillNormal();
				} break;

			case 1:
				{
					this->SkillWave();
				} break;

			case 2:
				{
					this->SkillWater();
				} break;

			case 3:
				{
					this->SkillWaterSource();
				} break;

			default:
				{
					this->SkillNormal();
				} break;
			}

			return true;
		}

		void SkillNormal()
		{
			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			me()->AddDelayed(DELAYED_ATTACK, 1000, pTarget);

			this->StartTimer(0, 2000);
		}

		void SkillWave()
		{
			if (roll_chance_i(70))
			{
				this->SkillNormal();
				return;
			}

			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			sSwampOfDarkness->SendAttack(me(), 3);

			int32 angle = me()->GetAngle(pTarget->GetX(), pTarget->GetY());

			uint8 mangle = angle * 255 / 360;

			me()->SkillAngleCalculate(mangle, 3.0f, 9.0f, 3.0f, 0.0f);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if (!mTargetSec->IsLive() || !mTargetSec->IsPlaying())
				{
					continue;
				}

				Player* pPlayer = mTargetSec->ToPlayer();

				if (!pPlayer)
				{
					continue;
				}

				if (!me()->SkillInAngle(pPlayer->GetX(), pPlayer->GetY()))
				{
					continue;
				}

				me()->AttackProc(pPlayer, nullptr);

			VIEWPORT_CLOSE
		}

		void SkillWater()
		{
			if (roll_chance_i(70))
			{
				this->SkillNormal();
				return;
			}

			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			sSwampOfDarkness->SendAttack(me(), 4);

			me()->AddDelayed(DELAYED_SWAMP_OF_DARKNESS_WATER, 1000, nullptr, pTarget->GetX(), pTarget->GetY(), 2);
		}

		void SkillWaterSource()
		{
			if (!this->IsTimerElapsed(2, 30000))
			{
				this->SkillNormal();
				return;
			}

			if (Util::Distance(me()->GetX(), me()->GetY(), g_SwampOfDarknessWaterSource[0].x, g_SwampOfDarknessWaterSource[0].y) >= Path::Max)
			{
				this->SkillNormal();
				return;
			}

			this->SetWaterSourceState(1);
		}
	};
};

enum
{
	SWAMP_OF_DARKNESS_BOSS_SKILL_NORMAL,
	SWAMP_OF_DARKNESS_BOSS_SKILL_FLAME,
	SWAMP_OF_DARKNESS_BOSS_SKILL_DARK,

	SWAMP_OF_DARKNESS_BOSS_SKILL_MAX,
};

class SwampOfDarknessBossScript : public MonsterScriptAI
{
public:
	explicit SwampOfDarknessBossScript() : ScriptAI("swamp_of_darkness_boss_ai") { }
	virtual ~SwampOfDarknessBossScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI : public MonsterAI
	{
		explicit MobAI(Monster* monster) : MonsterAI(monster)
		{
			this->CreateTimer(0);
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if (sSwampOfDarkness->GetState() != SWAMP_OF_DARKNESS_STATE_BATTLE_2)
			{
				me()->Remove();
				return true;
			}

			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			if (!this->IsTimerElapsed(0))
			{
				return true;
			}

			this->PushBackPlayers(true);

			this->CreateTimer(0);
			return false;
		}

		void OnRespawn()
		{
			this->StartTimer(0);
		}

		bool PushBackAllowed()
		{
			return false;
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

		void OnDie()
		{
			sSwampOfDarkness->ChangeState(SWAMP_OF_DARKNESS_STATE_END);

			me()->LogKillData(true, false);
		}

		bool OnAttack(Unit* pTarget, bool common)
		{
			if (me()->GetPathData()->IsStartEnd())
			{
				return true;
			}

			if ((MyGetTickCount() - me()->GetLastMoveTime()) < 500)
			{
				return true;
			}

			int32 skill = Random(SWAMP_OF_DARKNESS_BOSS_SKILL_MAX);

			switch (skill)
			{
			case SWAMP_OF_DARKNESS_BOSS_SKILL_NORMAL:
				{
					this->SkillNormal();
				} break;

			case SWAMP_OF_DARKNESS_BOSS_SKILL_FLAME:
				{
					this->SkillFlame();
				} break;

			case SWAMP_OF_DARKNESS_BOSS_SKILL_DARK:
				{
					this->SkillDarkness();
				} break;

			default:
				{
					this->SkillNormal();
				} break;
			}

			return true;
		}

		void SkillNormal()
		{
			Unit* pTarget = me()->GetTarget();

			if (!pTarget)
			{
				return;
			}

			me()->SpecialMagicSend(pTarget, 130);
			me()->AddDelayed(DELAYED_ATTACK, 1000, pTarget);

			this->StartTimer(0, 2000);
		}

		void SkillFlame()
		{
			if (roll_chance_i(70))
			{
				this->SkillNormal();
				return;
			}

			sSwampOfDarkness->SendAttack(me(), 6);
			this->StartTimer(0, 3000);

			me()->AddDelayed(DELAYED_SWAMP_OF_DARKNESS_FLAME, 2000, nullptr, me()->GetX(), me()->GetY(), 50);
		}

		void SkillDarkness()
		{
			if (roll_chance_i(80))
			{
				this->SkillNormal();
				return;
			}

			sSwampOfDarkness->SendAttack(me(), 7);
			this->StartTimer(0, 5000);

			me()->AddDelayed(DELAYED_SWAMP_OF_DARKNESS_DARK, 2500, nullptr, me()->GetX(), me()->GetY(), 5);
		}
	};
};

void AddSC_SwampOfDarkness()
{
	sScriptAI->AddScriptAI(new SwampOfDarknessPetFireScript());
	sScriptAI->AddScriptAI(new SwampOfDarknessPetIceScript());
	sScriptAI->AddScriptAI(new SwampOfDarknessMonsterScript());
	sScriptAI->AddScriptAI(new SwampOfDarknessWaterMonsterScript());
	sScriptAI->AddScriptAI(new SwampOfDarknessBossScript());
}