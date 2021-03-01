enum
{
	NARS_BOSS_SKILL_POISON,
	NARS_BOSS_SKILL_TWISTER,
	NARS_BOSS_SKILL_SHOUT,
	NARS_BOSS_SKILL_CLONE,
	NARS_BOSS_SKILL_BERSERKER,
	NARS_BOSS_SKILL_MAX,

	NARS_BOSS_CLONEID_1 = 717,
	NARS_BOSS_CLONEID_2 = 718,
	NARS_BOSS_CLONE_MAX = 2,
};

static const struct NarsBossSkillData
{
	int32 rate;
	int32 hp_percent;
} g_NarsBossSkillData[NARS_BOSS_SKILL_MAX] =
{
	{ 100, 100 },
	{ 50, 100 },
	{ 50, 75 },
	{ 100, 25 },
	{ 30, 25 },
};

#define SPAWN_SUPPORT_COUNT				10

class NarsBossCommon: public MonsterAI
{
	public:
		explicit NarsBossCommon(Monster* pMonster): MonsterAI(pMonster)
		{
			this->InitSupport();
		}

		DECLARE_STRUCT(TickTimer, SpawnSupportTime);
		Monster *pSupport[SPAWN_SUPPORT_COUNT];

		void InitSupport()
		{
			this->GetSpawnSupportTime()->Reset();

			for ( int32 i = 0; i < SPAWN_SUPPORT_COUNT; ++i )
			{
				this->pSupport[i] = nullptr;
			}
		}

		bool InmuneToRadiance() const
		{
			return true;
		}

		bool InmuneToDebuffBlow(bool blow) const
		{
			if ( blow )
			{
				if ( sGameServer->IsBossDebuffBlow(2) )
				{
					return true;
				}
			}
			else
			{
				if ( sGameServer->IsBossDebuffBlow(1) )
				{
					return true;
				}
			}

			return false;
		}

		void SpawnSupport()
		{
			if ( !this->GetSpawnSupportTime()->Elapsed(30 * IN_MILLISECONDS) )
			{
				return;
			}

			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			for ( int32 i = 0; i < SPAWN_SUPPORT_COUNT; ++i )
			{
				Monster* pMonster = this->pSupport[i];

				if ( pMonster && pMonster->IsLive() && pMonster->IsPlaying() )
				{
					continue;
				}

				this->pSupport[i] = nullptr;

				pMonster = sObjectMgr->MonsterTryAdd(Random(3) + 713, me()->GetWorldId());

				if ( pMonster )
				{
					this->pSupport[i] = pMonster;

					coord_type x = me()->GetX();
					coord_type y = me()->GetY();

					pWorld->GetRandomLocation(x, y, 10);

					pMonster->SetWorldId(me()->GetWorldId());
					pMonster->SetBasicLocation(x, y, x, y);
					pMonster->SetMoveDistance(10);
					pMonster->SetRespawn(IN_MILLISECONDS);
					pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					pMonster->SetScriptName("ferea_monster_ai");
					pMonster->AddAdditionalDataPtr(0, me());
					pMonster->AddToWorld();
				}
			}
		}
};

class NarsBoss: public MonsterScriptAI
{
public:
	explicit NarsBoss(): ScriptAI("nars_boss_ai") { }
	virtual ~NarsBoss() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public NarsBossCommon
	{
		Monster* Summoned[NARS_BOSS_CLONE_MAX];
		DECLARE_BOOL(Cloned);

		explicit AI(Monster* monster): NarsBossCommon(monster) 
		{
			for ( int32 i = 0; i < NARS_BOSS_CLONE_MAX; ++i )
			{
				this->Summoned[i] = nullptr;
			}
		}

		virtual ~AI() { }

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if ( me()->PowerGetPercent(POWER_LIFE) > 50 )
			{
				this->SetCloned(false);
			}

			int32 summon_count = 0;

			for ( int32 i = 0; i < NARS_BOSS_CLONE_MAX; ++i )
			{
				if ( this->Summoned[i] == nullptr )
				{
					continue;
				}

				if ( !this->Summoned[i]->IsPlaying() )
				{
					this->Summoned[i] = nullptr;
					continue;
				}

				++summon_count;
			}

			if ( summon_count == 1 )
			{
				for ( int32 i = 0; i < NARS_BOSS_CLONE_MAX; ++i )
				{
					if ( this->Summoned[i] == nullptr )
					{
						continue;
					}

					this->Summoned[i]->Remove();
					this->Summoned[i] = nullptr;
				}

				summon_count = 0;
			}

			if ( summon_count <= 0 )
			{
				if ( me()->HasBuff(BUFF_NARS_CLONE) )
				{
					me()->SetRegenPowerTime(MyGetTickCount() + 10000);
					me()->RemoveBuff(BUFF_NARS_CLONE);
				}
			}

			if ( me()->IsViewportEmpty() )
			{
				if ( !sGameServer->IsActiveStatus() && me()->GetInstance() == -1 )
				{
					me()->SetInstance(BOSS_INACTIVE_INSTANCE);
				}
				else if ( sGameServer->IsActiveStatus() && me()->GetInstance() == BOSS_INACTIVE_INSTANCE )
				{
					me()->SetInstance(-1);
				}
			}
			else
			{
				this->SpawnSupport();
			}

			if ( me()->HasBuff(BUFF_NARS_CLONE) )
			{
				me()->SetNextActionTime(1000);
				return true;
			}

			this->PushBackPlayers(true);

			return false;
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			if ( me()->HasBuff(BUFF_NARS_CLONE) )
			{
				return false;
			}

			return true;
		}

		bool OnAttack(Unit*, bool common)
		{
			if ( !me()->HasBuff(BUFF_NARS_CLONE) )
			{
				this->RunSkill();
			}

			return true;
		}

		void OnBeenAttacked(Unit*)
		{
			this->IncreaseBerserker();
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void OnDie()
		{
			me()->LogKillData(true);
		}

		void RunSkill()
		{
			if ( !this->IsCloned() )
			{
				if ( me()->PowerGetPercent(POWER_LIFE) <= g_NarsBossSkillData[NARS_BOSS_SKILL_CLONE].hp_percent )
				{
					this->SkillClone();
					return;
				}
			}

			do
			{
				int32 skill = Random(NARS_BOSS_SKILL_MAX);

				if (!roll_chance_i(g_NarsBossSkillData[skill].rate))
				{
					continue;
				}

				if ( me()->PowerGetPercent(POWER_LIFE) > g_NarsBossSkillData[skill].hp_percent )
				{
					continue;
				}

				switch ( skill )
				{
				case NARS_BOSS_SKILL_POISON:
					{
						this->SkillPoison();
					} break;

				case NARS_BOSS_SKILL_TWISTER:
					{
						this->SkillTwister();
					} break;

				case NARS_BOSS_SKILL_SHOUT:
					{
						this->SkillShout();
					} break;

				case NARS_BOSS_SKILL_CLONE:
					{
						this->SkillPoison();
					} break;

				case NARS_BOSS_SKILL_BERSERKER:
					{
						this->SkillBerserker();
					} break;
				}

				return;
			}
			while (true);
		}

		void SkillPoison()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(1500);

			me()->SpecialMagicSend(me()->GetTarget(), 106);
			me()->AttackProc(me()->GetTarget(), nullptr);

			if (roll_chance_i(50))
			{
				if ( me()->GetTarget()->ResistElement(Element::POISON) )
				{
					return;
				}

				if ( me()->GetTarget()->HasBuff(BUFF_POISON) )
				{
					return;
				}

				me()->GetTarget()->AddBuff(BUFF_POISON, BuffEffect(BUFF_OPTION_NONE, 3), 20, 0, me());
			}
		}

		void SkillTwister()
		{
			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 107);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				if ( !mTargetSec->IsPlayer() )
				{
					if ( Monster* pMonster = mTargetSec->ToCreature() )
					{
						if ( !pMonster->GetSummoner() || !pMonster->GetSummoner()->IsPlayer() )
						{
							if ( !pMonster->GetSummonPlayer() )
							{
								continue;
							}
						}
					}
					else
					{
						continue;
					}
				}

				if ( Util::Distance(me()->GetX(), me()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
				{
					continue;
				}

				me()->AttackProc(mTargetSec->ToUnit(), nullptr);
				//me()->PushBackCount(mTargetSec->ToUnit(), 2);

			VIEWPORT_CLOSE
		}

		void SkillShout()
		{
			me()->SetNextActionTime(2000);

			me()->SpecialMagicSend(me()->GetTarget(), 108);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				if ( !mTargetSec->IsPlayer() )
				{
					if ( Monster* pMonster = mTargetSec->ToCreature() )
					{
						if ( !pMonster->GetSummoner() || !pMonster->GetSummoner()->IsPlayer() )
						{
							if ( !pMonster->GetSummonPlayer() )
							{
								continue;
							}
						}
					}
					else
					{
						continue;
					}
				}

				if ( Util::Distance(me()->GetX(), me()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
				{
					continue;
				}

				me()->AttackProc(mTargetSec->ToUnit(), nullptr);
				me()->PushBackCount(mTargetSec->ToUnit(), 2);

			VIEWPORT_CLOSE
		}

		void SkillClone()
		{
			if ( me()->HasBuff(BUFF_NARS_CLONE) )
			{
				return;
			}

			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			me()->SetNextActionTime(3000);

			this->SetCloned(true);

			me()->SpecialMagicSend(me()->GetTarget(), 109);
			me()->SpecialMagicSend(me()->GetTarget(), 111);

			me()->AddDelayed(DELAYED_BUFF_ADD, 1500, me(), BUFF_NARS_CLONE, 0, 0, 0, BUFF_FLAG_CONSTANT);

			for ( int32 i = 0; i < NARS_BOSS_CLONE_MAX; ++i )
			{
				if ( this->Summoned[i] == nullptr )
				{
					this->Summoned[i] = sObjectMgr->MonsterTryAdd(NARS_BOSS_CLONEID_1 + i, me()->GetWorldId());

					if ( this->Summoned[i] )
					{
						coord_type x = me()->GetX();
						coord_type y = me()->GetY();

						pWorld->GetRandomLocation(x, y, 3);

						this->Summoned[i]->SetWorldId(me()->GetWorldId());
						this->Summoned[i]->SetBasicLocation(x, y, x, y);
						this->Summoned[i]->SetMoveDistance(10);
						this->Summoned[i]->SetRespawn(IN_MILLISECONDS);
						this->Summoned[i]->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
						this->Summoned[i]->AddAdditionalDataPtr(0, me());
						this->Summoned[i]->SetSpawnTime(3000);
						this->Summoned[i]->SetSpawnTick(MyGetTickCount());
						this->Summoned[i]->AddToWorld();
					}
				}
			}
		}

		void SkillBerserker()
		{
			me()->SetNextActionTime(2000);

			if ( !me()->HasBuff(BUFF_NARS_BERSERKER) )
			{
				me()->SpecialMagicSend(me()->GetTarget(), 110);
				me()->AddBuff(BUFF_NARS_BERSERKER, BuffEffect(BUFF_OPTION_NONE, 100), 30, 0, me());
			}
			else
			{
				this->SkillPoison();
			}

		}

		void IncreaseBerserker()
		{
			me()->IncreaseBuffEffect(BUFF_NARS_BERSERKER, 0, 100);
		}
	};
};

class NarsBossClone1: public MonsterScriptAI
{
public:
	explicit NarsBossClone1(): ScriptAI("nars_boss_clone_1_ai") { }
	virtual ~NarsBossClone1() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public NarsBossCommon
	{
		explicit AI(Monster* monster): NarsBossCommon(monster) 
		{
		}

		virtual ~AI() { }

		DECLARE_PTR(Monster, Summoner);

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if ( !this->GetSummoner() )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetSummoner()->IsLive() )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetSummoner()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			this->SpawnSupport();

			this->PushBackPlayers(true);

			return false;
		}

		void OnCreate()
		{
			this->SetSummoner((Monster*)me()->m_AdditionalDataPtr[0]);
		}

		bool OnAttack(Unit*, bool common)
		{
			this->RunSkill();
			return true;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void RunSkill()
		{
			do
			{
				int32 skill = Random(NARS_BOSS_SKILL_MAX);

				if (!roll_chance_i(g_NarsBossSkillData[skill].rate))
				{
					continue;
				}

				if ( me()->PowerGetPercent(POWER_LIFE) > g_NarsBossSkillData[skill].hp_percent )
				{
					continue;
				}

				switch ( skill )
				{
				case NARS_BOSS_SKILL_POISON:
					{
						this->SkillPoison();
					} break;

				case NARS_BOSS_SKILL_TWISTER:
					{
						this->SkillTwister();
					} break;

				case NARS_BOSS_SKILL_SHOUT:
					{
						this->SkillShout();
					} break;

				case NARS_BOSS_SKILL_CLONE:
					{
						this->SkillPoison();
					} break;

				case NARS_BOSS_SKILL_BERSERKER:
					{
						this->SkillPoison();
					} break;
				}

				return;
			}
			while (true);
		}

		void SkillPoison()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(1500);

			me()->SpecialMagicSend(me()->GetTarget(), 106);
			me()->AttackProc(me()->GetTarget(), nullptr);

			if (roll_chance_i(50))
			{
				if ( me()->GetTarget()->ResistElement(Element::POISON) )
				{
					return;
				}

				if ( me()->GetTarget()->HasBuff(BUFF_POISON) )
				{
					return;
				}

				me()->GetTarget()->AddBuff(BUFF_POISON, BuffEffect(BUFF_OPTION_NONE, 3), 20, 0, me());
			}
		}

		void SkillTwister()
		{
			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 107);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				if ( !mTargetSec->IsPlayer() )
				{
					if ( Monster* pMonster = mTargetSec->ToCreature() )
					{
						if ( !pMonster->GetSummoner() || !pMonster->GetSummoner()->IsPlayer() )
						{
							if ( !pMonster->GetSummonPlayer() )
							{
								continue;
							}
						}
					}
					else
					{
						continue;
					}
				}

				if ( Util::Distance(me()->GetX(), me()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
				{
					continue;
				}

				me()->AttackProc(mTargetSec->ToUnit(), nullptr);
				//me()->PushBackCount(mTargetSec->ToUnit(), 2);

			VIEWPORT_CLOSE
		}

		void SkillShout()
		{
			me()->SetNextActionTime(2000);

			me()->SpecialMagicSend(me()->GetTarget(), 108);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				if ( !mTargetSec->IsPlayer() )
				{
					if ( Monster* pMonster = mTargetSec->ToCreature() )
					{
						if ( !pMonster->GetSummoner() || !pMonster->GetSummoner()->IsPlayer() )
						{
							if ( !pMonster->GetSummonPlayer() )
							{
								continue;
							}
						}
					}
					else
					{
						continue;
					}
				}

				if ( Util::Distance(me()->GetX(), me()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
				{
					continue;
				}

				me()->AttackProc(mTargetSec->ToUnit(), nullptr);
				me()->PushBackCount(mTargetSec->ToUnit(), 2);

			VIEWPORT_CLOSE
		}
	};
};

class NarsBossClone2: public MonsterScriptAI
{
public:
	explicit NarsBossClone2(): ScriptAI("nars_boss_clone_2_ai") { }
	virtual ~NarsBossClone2() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public NarsBossCommon
	{
		explicit AI(Monster* monster): NarsBossCommon(monster) 
		{
		}

		virtual ~AI() { }

		DECLARE_PTR(Monster, Summoner);

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if ( !this->GetSummoner() )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetSummoner()->IsLive() )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetSummoner()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			this->PushBackPlayers(true);

			return false;
		}

		void OnCreate()
		{
			this->SetSummoner((Monster*)me()->m_AdditionalDataPtr[0]);
		}

		bool OnAttack(Unit*, bool common)
		{
			this->RunSkill();
			return true;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void RunSkill()
		{
			do
			{
				int32 skill = Random(NARS_BOSS_SKILL_MAX);

				if (!roll_chance_i(g_NarsBossSkillData[skill].rate))
				{
					continue;
				}

				if ( me()->PowerGetPercent(POWER_LIFE) > g_NarsBossSkillData[skill].hp_percent )
				{
					continue;
				}

				switch ( skill )
				{
				case NARS_BOSS_SKILL_POISON:
					{
						this->SkillPoison();
					} break;

				case NARS_BOSS_SKILL_TWISTER:
					{
						this->SkillTwister();
					} break;

				case NARS_BOSS_SKILL_SHOUT:
					{
						this->SkillShout();
					} break;

				case NARS_BOSS_SKILL_CLONE:
					{
						this->SkillPoison();
					} break;

				case NARS_BOSS_SKILL_BERSERKER:
					{
						this->SkillPoison();
					} break;
				}

				return;
			}
			while (true);
		}

		void SkillPoison()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(1500);

			me()->SpecialMagicSend(me()->GetTarget(), 106);
			me()->AttackProc(me()->GetTarget(), nullptr);

			if (roll_chance_i(50))
			{
				if ( me()->GetTarget()->ResistElement(Element::POISON) )
				{
					return;
				}

				if ( me()->GetTarget()->HasBuff(BUFF_POISON) )
				{
					return;
				}

				me()->GetTarget()->AddBuff(BUFF_POISON, BuffEffect(BUFF_OPTION_NONE, 3), 20, 0, me());
			}
		}

		void SkillTwister()
		{
			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 107);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				if ( !mTargetSec->IsPlayer() )
				{
					if ( Monster* pMonster = mTargetSec->ToCreature() )
					{
						if ( !pMonster->GetSummoner() || !pMonster->GetSummoner()->IsPlayer() )
						{
							if ( !pMonster->GetSummonPlayer() )
							{
								continue;
							}
						}
					}
					else
					{
						continue;
					}
				}

				if ( Util::Distance(me()->GetX(), me()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
				{
					continue;
				}

				me()->AttackProc(mTargetSec->ToUnit(), nullptr);
				//me()->PushBackCount(mTargetSec->ToUnit(), 2);

			VIEWPORT_CLOSE
		}

		void SkillShout()
		{
			me()->SetNextActionTime(2000);

			me()->SpecialMagicSend(me()->GetTarget(), 108);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				if ( !mTargetSec->IsPlayer() )
				{
					if ( Monster* pMonster = mTargetSec->ToCreature() )
					{
						if ( !pMonster->GetSummoner() || !pMonster->GetSummoner()->IsPlayer() )
						{
							if ( !pMonster->GetSummonPlayer() )
							{
								continue;
							}
						}
					}
					else
					{
						continue;
					}
				}

				if ( Util::Distance(me()->GetX(), me()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
				{
					continue;
				}

				me()->AttackProc(mTargetSec->ToUnit(), nullptr);
				me()->PushBackCount(mTargetSec->ToUnit(), 2);

			VIEWPORT_CLOSE
		}
	};
};

void AddSC_Nars()
{
	sScriptAI->AddScriptAI(new NarsBoss());
	sScriptAI->AddScriptAI(new NarsBossClone1());
	sScriptAI->AddScriptAI(new NarsBossClone2());
}