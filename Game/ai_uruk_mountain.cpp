class UrukMountainMonster: public MonsterScriptAI
{
public:
	explicit UrukMountainMonster(): ScriptAI("uruk_mountain_monster") { }
	virtual ~UrukMountainMonster() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster) { this->SetSummoner(nullptr); }
		virtual ~AI() { }

		DECLARE_PTR(Monster, Summoner);

		bool Update()
		{
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

			return false;
		}

		void OnCreate()
		{
			this->SetSummoner((Monster*)me()->m_AdditionalDataPtr[0]);
		}
	};
};

#define MAX_URUK_MOUNTAIN_SUMMON			3
#define MAX_URUK_MOUNTAIN_SUMMON_STATUS		3

enum
{
	URUK_MOUNTAIN_BOSS_SKILL_NORMAL,
	URUK_MOUNTAIN_BOSS_SKILL_TORNADO,
	URUK_MOUNTAIN_BOSS_SKILL_CURSE_OF_WIND,
	URUK_MOUNTAIN_BOSS_SKILL_MAX,
};

static const int32 UrukMountainBossSkillRate[URUK_MOUNTAIN_BOSS_SKILL_MAX] = { 100, 5, 50 };

#define SPAWN_SUPPORT_COUNT				10

class UrukMountainBoss: public MonsterScriptAI
{
public:
	explicit UrukMountainBoss(): ScriptAI("uruk_mountain_boss") { }
	virtual ~UrukMountainBoss() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		Monster* Summoned[MAX_URUK_MOUNTAIN_SUMMON];
		DECLARE_PROPERTY_ARRAY(int32, SummonStatus, MAX_URUK_MOUNTAIN_SUMMON_STATUS);
		DECLARE_STRUCT(TickTimer, UpdateWorldTime);
		DECLARE_BOOL(ViewportEmpty);
		DECLARE_STRUCT(TickTimer, SpawnSupportTime);
		Monster *pSupport[SPAWN_SUPPORT_COUNT];

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			for ( int32 i = 0; i < MAX_URUK_MOUNTAIN_SUMMON; ++i )
			{
				this->Summoned[i] = nullptr;
			}

			this->ResetSummonStatus(0);
			this->GetUpdateWorldTime()->Reset();
			this->SetViewportEmpty(true);

			this->GetSpawnSupportTime()->Reset();

			for ( int32 i = 0; i < SPAWN_SUPPORT_COUNT; ++i )
			{
				this->pSupport[i] = nullptr;
			}
		}

		virtual ~AI() { }

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			int32 power_percent = me()->PowerGetPercent(POWER_LIFE);

			if ( power_percent > 70 )
			{
				this->ResetSummonStatus(0);
			}
			
			int32 summon_count = 0;

			for ( int32 i = 0; i < MAX_URUK_MOUNTAIN_SUMMON; ++i )
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

			if ( summon_count <= 0 )
			{
				if ( me()->HasBuff(BUFF_INMUNE_SKILL) )
				{
					me()->RemoveBuff(BUFF_INMUNE_SKILL);
				}
			}

			if ( this->IsViewportEmpty() != me()->IsViewportEmpty() )
			{
				this->SetViewportEmpty(me()->IsViewportEmpty());

				if ( this->IsViewportEmpty() )
				{
					me()->TeleportToLocation(me()->GetWorldId(), 128, 116, -1, me()->GetInstance());
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

				if ( this->GetUpdateWorldTime()->Elapsed(5000) )
				{
					sWorldMgr->UpdateUrukMountainZone(false);
				}
			}
			else
			{
				if ( this->GetUpdateWorldTime()->Elapsed(5000) )
				{
					sWorldMgr->UpdateUrukMountainZone(true);
				}

				this->SpawnSupport();
			}

			this->PushBackPlayers(true);

			return false;
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

		void OnRespawn()
		{
			sWorldMgr->UpdateUrukMountainZone(false);
		}

		void OnDie() 
		{
			sWorldMgr->UpdateUrukMountainZone(false);

			me()->LogKillData(true);
		}

		void OnDespawn()
		{
			sWorldMgr->UpdateUrukMountainZone(false);
		}

		bool EnableAttack(Unit* pUnit, Skill*, int32 count)
		{
			if ( !pUnit )
			{
				return false;
			}

			for ( int32 i = 0; i < MAX_URUK_MOUNTAIN_SUMMON; ++i )
			{
				if ( this->Summoned[i] != nullptr )
				{
					return false;
				}
			}

			if ( !me()->CheckWall(pUnit) )
			{
				return false;
			}

			return true;
		}

		bool UpdateSummon()
		{
			int32 power_percent = me()->PowerGetPercent(POWER_LIFE);

			if ( power_percent <= 20 )
			{
				return this->RunSummon(2);
			}
			else if ( power_percent <= 50 )
			{
				return this->RunSummon(1);
			}
			else if ( power_percent <= 70 )
			{
				return this->RunSummon(0);
			}

			return false;
		}

		bool RunSummon(int32 id)
		{
			if ( this->GetSummonStatus(id) != 0 )
			{
				return false;
			}

			for ( int32 i = 0; i < MAX_URUK_MOUNTAIN_SUMMON; ++i )
			{
				if ( this->Summoned[i] != nullptr )
				{
					return false;
				}
			}

			for ( int32 i = id; i < MAX_URUK_MOUNTAIN_SUMMON_STATUS; ++i )
			{
				this->SetSummonStatus(i, 0);
			}

			this->SkillCallOfWind();
			this->SetSummonStatus(id, 1);

			return true;
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

				pMonster = sObjectMgr->MonsterTryAdd(Random(3) + 670, me()->GetWorldId());

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

		void RunSkill()
		{
			if ( this->UpdateSummon() )
			{
				return;
			}

			do
			{
				int32 skill = Random(URUK_MOUNTAIN_BOSS_SKILL_MAX);

				if (!roll_chance_i(UrukMountainBossSkillRate[skill]))
				{
					continue;
				}

				switch ( skill )
				{
				case URUK_MOUNTAIN_BOSS_SKILL_NORMAL:
					{
						this->SkillNormal();
					} break;

				case URUK_MOUNTAIN_BOSS_SKILL_TORNADO:
					{
						this->SkillTornado();
					} break;

				case URUK_MOUNTAIN_BOSS_SKILL_CURSE_OF_WIND:
					{
						this->SkillCurseOfWind();
					} break;
				}

				return;
			}
			while (true);
		}

		void SkillNormal()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(1500);

			me()->SpecialMagicSend(me()->GetTarget(), 97);

			me()->AttackProc(me()->GetTarget(), nullptr);

			if (roll_chance_i(5))
			{
				me()->GetTarget()->AddBuff(BUFF_STUN, 5, 0, me());
			}
		}

		void SkillTornado()
		{
			me()->SetNextActionTime(4000);

			me()->SpecialMagicSend(me()->GetTarget(), 98);

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

				me()->AddDelayed(DELAYED_ATTACK, 1500, mTargetSec->ToUnit());
				me()->AddDelayed(DELAYED_PUSH_BACK_COUNT, 1500, mTargetSec->ToUnit(), 2);

			VIEWPORT_CLOSE
		}

		void SkillCurseOfWind()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(4000);

			me()->SpecialMagicSend(me()->GetTarget(), 99);

			me()->AddDelayed(DELAYED_ATTACK, 1500, me()->GetTarget(), 0, (20 * me()->GetTarget()->PowerGetTotal(POWER_LIFE)) / 100);
			me()->AddDelayed(DELAYED_PUSH_BACK_COUNT, 1500, me()->GetTarget(), 2);
		}

		void SkillCallOfWind()
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			me()->AddBuff(BUFF_INMUNE_SKILL, 0, BUFF_FLAG_CONSTANT, me());

			//me()->SpecialMagicSend(me()->GetTarget(), 109);
			me()->SetNextActionTime(4000);

			for ( int32 i = 0; i < MAX_URUK_MOUNTAIN_SUMMON; ++i )
			{
				if ( this->Summoned[i] == nullptr )
				{
					this->Summoned[i] = sObjectMgr->MonsterTryAdd(678 + i, me()->GetWorldId());

					if ( this->Summoned[i] )
					{
						coord_type x = me()->GetX();
						coord_type y = me()->GetY();

						pWorld->GetRandomLocation(x, y, 3);

						this->Summoned[i]->SetWorldId(me()->GetWorldId());
						this->Summoned[i]->SetBasicLocation(x, y, x, y);
						this->Summoned[i]->SetMoveDistance(10);
						this->Summoned[i]->SetRespawn(IN_MILLISECONDS);
						this->Summoned[i]->SetScriptName("uruk_mountain_monster");
						this->Summoned[i]->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
						this->Summoned[i]->AddAdditionalDataPtr(0, me());
						this->Summoned[i]->AddToWorld();
					}
				}
			}
		}
	};
};

void AddSC_UrukMountain()
{
	sScriptAI->AddScriptAI(new UrukMountainBoss());
	sScriptAI->AddScriptAI(new UrukMountainMonster());
}