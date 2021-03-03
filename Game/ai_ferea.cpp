static const struct FereaZoneData
{
	int16 x;
	int16 y;
} g_FereaZoneData[2] =
{
	{ 21, 106 },
	{ 48, 139 },
};

class FereaMonster: public MonsterScriptAI
{
public:
	explicit FereaMonster(): ScriptAI("ferea_monster_ai") { }
	virtual ~FereaMonster() { }

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

			switch ( me()->GetWorldId() )
			{
			case WORLD_FEREA: 
				{
					if ( (me()->GetX() < g_FereaZoneData[0].x || me()->GetX() > g_FereaZoneData[1].x) ||
						 (me()->GetY() < g_FereaZoneData[0].y || me()->GetY() > g_FereaZoneData[1].y) )
					{
						me()->TeleportToLocation(me()->GetWorldId(), 27 + Random(5), 120 + Random(5), me()->GetDirection(), me()->GetInstance());
						return true;
					}
				} break;
			}

			return false;
		}

		void OnCreate()
		{
			this->SetSummoner((Monster*)me()->m_AdditionalDataPtr[0]);
		}
	};
};

#define FEREA_CRYSTAL_ORB_MONSTER		7

class FereaCrystalOrb: public MonsterScriptAI
{
public:
	explicit FereaCrystalOrb(): ScriptAI("ferea_crystal_orb_ai") { }
	virtual ~FereaCrystalOrb() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		Monster *pSupport[FEREA_CRYSTAL_ORB_MONSTER];
		DECLARE_STRUCT(TickTimer, SpawnSupportTime);
		DECLARE_BOOL(Inmortal);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			this->GetSpawnSupportTime()->Reset();
			this->SetInmortal(true);

			for ( int32 i = 0; i < FEREA_CRYSTAL_ORB_MONSTER; ++i )
			{
				this->pSupport[i] = nullptr;
			}
		}

		virtual ~AI() { }

		void OnRespawn()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);
		}

		bool Update()
		{
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

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			int32 count = 0;

			for ( int32 i = 0; i < FEREA_CRYSTAL_ORB_MONSTER; ++i )
			{
				if ( this->pSupport[i] && this->pSupport[i]->IsPlaying() )
				{
					++count;
					continue;
				}

				this->pSupport[i] = nullptr;
			}

			if ( this->IsInmortal() )
			{
				this->GetSpawnSupportTime()->Start();

				if ( count <= 0 )
				{
					this->SetInmortal(false);
				}
			}
			else
			{
				if ( count > 0 )
				{
					this->SetInmortal(true);
				}
			}

			this->SpawnSupport();

			return false;
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			return !this->IsInmortal();
		}

		void OnDie()
		{
			me()->LogKillData(true);

			for ( int32 i = 0; i < FEREA_CRYSTAL_ORB_MONSTER; ++i )
			{
				if ( this->pSupport[i] )
				{
					this->pSupport[i]->Remove();
				}

				this->pSupport[i] = nullptr;
			}

			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			Monster* pMonster = sObjectMgr->MonsterTryAdd(734, me()->GetWorldId());

			if ( pMonster )
			{
				pMonster->SetWorldId(me()->GetWorldId());
				pMonster->SetBasicLocation(27, 123, 27, 123);
				pMonster->SetDirection(2);
				pMonster->SetMoveDistance(10);
				pMonster->SetSpawnTime(1 * IN_MILLISECONDS);
				pMonster->SetSpawnTick(MyGetTickCount());
				pMonster->SetRespawn(IN_MILLISECONDS);
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
				pMonster->SetDespawnTime(me()->GetRespawnTimeRangeMin() - (MINUTE * IN_MILLISECONDS));
				pMonster->SetDespawnTick(MyGetTickCount());
				pMonster->SetKillID(me()->GetKillID());
				pMonster->AddToWorld();
			}
		}

		bool Drop()
		{
			return true;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void SpawnSupport()
		{
			if ( !this->GetSpawnSupportTime()->Elapsed(10 * IN_MILLISECONDS) )
			{
				return;
			}

			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			for ( int32 i = 0; i < FEREA_CRYSTAL_ORB_MONSTER; ++i )
			{
				if ( this->pSupport[i] )
				{
					continue;
				}

				Monster* pMonster = sObjectMgr->MonsterTryAdd(Random(3) + 736, me()->GetWorldId());

				if ( pMonster )
				{
					this->pSupport[i] = pMonster;

					int16 x = me()->GetX();
					int16 y = me()->GetY();

					pWorld->GetRandomLocation(x, y, 10);

					pMonster->SetWorldId(me()->GetWorldId());
					pMonster->SetBasicLocation(x, y, x, y);
					pMonster->SetMoveDistance(10);
					pMonster->SetRespawn(IN_MILLISECONDS);
					pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					pMonster->SetScriptName("ferea_monster_ai");
					pMonster->AddAdditionalDataPtr(0, me());
					pMonster->AddToWorld();

					this->pSupport[i] = pMonster;
				}
			}
		}
	};
};

class FereaGeneral: public MonsterScriptAI
{
public:
	explicit FereaGeneral(): ScriptAI("ferea_general_ai") { }
	virtual ~FereaGeneral() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster) { this->SetSummoner(nullptr); }
		virtual ~AI() { }

		DECLARE_PTR(Monster, Summoner);

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

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

			if ( (me()->GetX() < g_FereaZoneData[0].x || me()->GetX() > g_FereaZoneData[1].x) ||
				 (me()->GetY() < g_FereaZoneData[0].y || me()->GetY() > g_FereaZoneData[1].y) )
			{
				me()->TeleportToLocation(me()->GetWorldId(), me()->GetSpawnX(0), me()->GetSpawnY(0), me()->GetDirection(), me()->GetInstance());
				return true;
			}

			return false;
		}

		void OnCreate()
		{
			this->SetSummoner((Monster*)me()->m_AdditionalDataPtr[0]);
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
	};
};

enum 
{
	FEREA_BOSS_SKILL_HIT_1,
	FEREA_BOSS_SKILL_MINE_EXPLOSION,
	FEREA_BOSS_SKILL_BRANDISH,
	FEREA_BOSS_SKILL_ROAR,
	FEREA_BOSS_SKILL_LORD_SUMMON,
	FEREA_BOSS_SKILL_MAX,

	FEREA_BOSS_SKILL_ROAR_HP_PERCENT = 20,

	FEREA_BOSS_MINE_STATE_NONE		= 0,
	FEREA_BOSS_MINE_STATE_ACTIVE	= 1,
};

#define SPAWN_SUPPORT_COUNT				20
#define MAX_SUMMON_STAGE				3
#define FEREA_BOSS_MINE_MAX				5

static const struct LordFereaSummon
{
	uint8 hp_percent;
} g_LordFereaSummon[MAX_SUMMON_STAGE] =
{
	75,
	50,
	25
};

class FereaBoss: public MonsterScriptAI
{
public:
	explicit FereaBoss(): ScriptAI("ferea_boss_ai") { }
	virtual ~FereaBoss() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_PTR(Monster, General);
		DECLARE_PROPERTY(int32, SkillState);
		DECLARE_PROPERTY(int32, RoarCount);
		DECLARE_PROPERTY(int32, SummonGeneralCount);
		DECLARE_STRUCT(TickTimer, UpdateWorldTime);
		DECLARE_STRUCT(TickTimer, SpawnSupportTime);
		DECLARE_STRUCT(TickTimer, HPRecoveryTime);
		Monster *pSupport[SPAWN_SUPPORT_COUNT];

		DECLARE_ENUM(uint8, MineState);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			this->SetGeneral(nullptr);
			this->SetSkillState(0);
			this->SetRoarCount(0);
			this->SetSummonGeneralCount(0);
			this->GetUpdateWorldTime()->Reset();
			this->GetSpawnSupportTime()->Reset();
			this->GetHPRecoveryTime()->Reset();
			
			for ( int32 i = 0; i < SPAWN_SUPPORT_COUNT; ++i )
			{
				this->pSupport[i] = nullptr;
			}

			this->SetMineState(FEREA_BOSS_MINE_STATE_NONE);

			this->CreateTimer(0);
		}

		virtual ~AI() { }

		bool Update()
		{
			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if ( this->GetGeneral() )
			{
				if ( !this->GetGeneral()->IsPlaying() )
				{
					this->SetGeneral(nullptr);
					this->SetSkillState(0);
					me()->RemoveBuff(BUFF_INMUNE_ATTACK);
					me()->RemoveBuff(BUFF_INMUNE_SKILL);
				}
				else
				{
					this->HPRecovery();
				}
			}

			if ( this->GetMineState() == FEREA_BOSS_MINE_STATE_ACTIVE )
			{
				if ( this->IsTimerElapsed(0, 60000) )
				{
					this->SetMineState(FEREA_BOSS_MINE_STATE_NONE);
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

			if ( me()->IsViewportEmpty() )
			{
				if ( me()->PowerGet(POWER_LIFE) == me()->PowerGetTotal(POWER_LIFE) )
				{
					this->SetSkillState(0);
					this->SetRoarCount(0);
					this->SetSummonGeneralCount(0);
				}

				if ( this->GetUpdateWorldTime()->Elapsed(1000) )
				{
					sWorldMgr->UpdateFereaZone(false);
				}

				if ( me()->GetX() != me()->GetSpawnX(0) ||
					 me()->GetY() != me()->GetSpawnY(0) )
				{
					me()->TeleportToLocation(me()->GetWorldId(), me()->GetSpawnX(0), me()->GetSpawnY(0), me()->GetDirection(), me()->GetInstance());
					return true;
				}
			}
			else
			{
				if ( this->GetUpdateWorldTime()->Elapsed(1000) )
				{
					sWorldMgr->UpdateFereaZone(true);
				}

				this->SpawnSupport();
			}

			if ( (me()->GetX() < g_FereaZoneData[0].x || me()->GetX() > g_FereaZoneData[1].x) ||
				 (me()->GetY() < g_FereaZoneData[0].y || me()->GetY() > g_FereaZoneData[1].y) )
			{
				me()->TeleportToLocation(me()->GetWorldId(), me()->GetSpawnX(0), me()->GetSpawnY(0), me()->GetDirection(), me()->GetInstance());
				return true;
			}

			this->PushBackPlayers(true);

			return false;
		}

		void OnRespawn()
		{
			sWorldMgr->UpdateFereaZone(false);

			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);
		}

		void OnDie() 
		{
			sWorldMgr->UpdateFereaZone(false);

			me()->LogKillData(true, false);
		}

		void OnDespawn()
		{
			sWorldMgr->UpdateFereaZone(false);
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

		bool EnableAttack(Unit* pUnit, Skill*, int32 count)
		{
			if ( !pUnit )
			{
				return false;
			}

			if ( this->GetGeneral() )
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
			if ( this->GetGeneral() )
			{
				return false;
			}

			if ( me()->IsViewportEmpty() )
			{
				return false;
			}

			return MonsterAI::MoveAllowed(x, y);
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

				pMonster = sObjectMgr->MonsterTryAdd(Random(3) + 736, me()->GetWorldId());

				if ( pMonster )
				{
					this->pSupport[i] = pMonster;

					int16 x = me()->GetX();
					int16 y = me()->GetY();

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
			me()->SetRegenPowerTime(MyGetTickCount() + 60000);

			if ( this->GetSkillState() == 0 )
			{
				if ( (this->GetSummonGeneralCount() == 0 && me()->PowerGetPercent(POWER_LIFE) < 75) ||
					 (this->GetSummonGeneralCount() == 1 && me()->PowerGetPercent(POWER_LIFE) < 50) ||
					 (this->GetSummonGeneralCount() == 2 && me()->PowerGetPercent(POWER_LIFE) < 25) )
				{
					this->SkillRoar();
					return;
				}
				/*if ( me()->PowerGetPercent(POWER_LIFE) <= FEREA_BOSS_SKILL_ROAR_HP_PERCENT )
				{
					this->SkillRoar();
					return;
				}*/
			}
			else if ( this->GetSkillState() == 1 )
			{
				this->SkillLordSummon();
				return;
			}

			int32 skill = Random(FEREA_BOSS_SKILL_MAX);

			switch ( skill )
			{
			case FEREA_BOSS_SKILL_HIT_1:
			case FEREA_BOSS_SKILL_ROAR:
			case FEREA_BOSS_SKILL_LORD_SUMMON:
				{
					this->SkillHit1();
				} break;

			case FEREA_BOSS_SKILL_MINE_EXPLOSION:
				{
					this->SkillMineCreate();
				} break;

			case FEREA_BOSS_SKILL_BRANDISH:
				{
					this->SkillBrandish();
				} break;
			}
		}

		void SkillHit1()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(2000);

			me()->SpecialMagicSend(me()->GetTarget(), 112);

			me()->AddDelayed(DELAYED_ATTACK, 500, me()->GetTarget());
		}

		void SkillMineCreate()
		{
			if ( this->GetMineState() != FEREA_BOSS_MINE_STATE_NONE )
			{
				this->SkillHit1();
				return;
			}

			me()->SetNextActionTime(4000);

			me()->SpecialMagicSend(me()->GetTarget(), 113);

			Object * mTargetSec;

			int32 count = 0;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				Player* pPlayer = mTargetSec->ToPlayer();

				if ( !pPlayer )
				{
					continue;
				}

				me()->AddDelayed(DELAYED_FEREA_MINE_CREATE, 1500, nullptr, pPlayer->GetX(), pPlayer->GetY(), 2);
				++count;

				if ( count >= FEREA_BOSS_MINE_MAX )
				{
					break;
				}

			VIEWPORT_CLOSE

			if ( count <= 0 )
			{
				return;
			}

			this->SetMineState(FEREA_BOSS_MINE_STATE_ACTIVE);
			this->StartTimer(0);
		}

		void SkillBrandish()
		{
			if ( this->GetGeneral() )
			{
				this->SkillHit1();
				return;
			}

			int16 x = me()->GetX();
			int16 y = me()->GetY();

			if ( me()->GetTarget() )
			{
				if ( me()->CheckWall(me()->GetTarget()) && me()->ViewportExist(me()->GetTarget()) )
				{
					x = me()->GetTarget()->GetX();
					y = me()->GetTarget()->GetY();
				}
			}

			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 114);
			me()->PositionSend(x, y);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() )
				{
					continue;
				}

				Unit* pUnit = mTargetSec->ToUnit();

				if ( !pUnit )
				{
					continue;
				}

				if ( pUnit->GetRegenStatus() != REGEN_NONE )
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

				if ( Util::Distance(x, y, mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
				{
					continue;
				}

				for ( int32 i = 0; i < 3; ++i )
				{
					me()->AddDelayed(DELAYED_ATTACK_RANGE, (i * 200), mTargetSec->ToUnit(), 3, me()->GetX(), me()->GetY());
				}

			VIEWPORT_CLOSE
		}

		void SkillRoar()
		{
			if ( this->GetSkillState() != 0 )
			{
				this->SkillHit1();
				return;
			}

			this->SetSkillState(1);
			this->SetRoarCount(1);

			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 115);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() || !mTargetSec->IsPlayer() )
				{
					continue;
				}

				mTargetSec->ToUnit()->PowerSet(POWER_MANA, 0, true);
				mTargetSec->ToUnit()->PowerSet(POWER_STAMINA, 0, true);

				this->IncreaseRoarCount(1);

			VIEWPORT_CLOSE
		}

		void SkillLordSummon()
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			if ( this->GetSkillState() != 1 )
			{
				this->SkillHit1();
				return;
			}

			this->SetSkillState(2);

			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 116);

			if ( this->GetGeneral() == nullptr )
			{
				this->SetGeneral(sObjectMgr->MonsterTryAdd(733, me()->GetWorldId()));

				if ( this->GetGeneral() )
				{
					int16 x = me()->GetX();
					int16 y = me()->GetY();

					//pWorld->GetRandomLocation(x, y, 3);

					this->GetGeneral()->SetWorldId(me()->GetWorldId());
					this->GetGeneral()->SetBasicLocation(x, y, x, y);
					this->GetGeneral()->SetMoveDistance(10);
					this->GetGeneral()->SetRespawn(IN_MILLISECONDS);
					this->GetGeneral()->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					this->GetGeneral()->AddAdditionalDataPtr(0, me());
					this->GetGeneral()->AddToWorld();

					this->IncreaseSummonGeneralCount(1);

					me()->AddBuff(BUFF_INMUNE_ATTACK, 0, BUFF_FLAG_CONSTANT, me());
					me()->AddBuff(BUFF_INMUNE_SKILL, 0, BUFF_FLAG_CONSTANT, me());

					me()->SkillTeleportUse(28, 123);
					me()->SetDirection(3);
				}
			}

			//me()->TeleportToLocation(me()->GetWorldId(), 28, 123, 3, me()->GetInstance());

			me()->SetRegenPowerTime(MyGetTickCount() + 60000);
		}

		void HPRecovery()
		{
			if ( !this->GetHPRecoveryTime()->Elapsed(sGameServer->GetFereaBossHPRecoveryTime()) )
			{
				return;
			}

			int32 hp_percent = this->GetRoarCount() * sGameServer->GetFereaBossHPRecoveryCount();

			if ( hp_percent <= 0 )
			{
				hp_percent = 1;
			}

			me()->PowerIncrease(POWER_LIFE, me()->PowerGetMax(POWER_LIFE) * hp_percent / 100.0f, true);
		}
	};
};

void AddSC_Ferea()
{
	sScriptAI->AddScriptAI(new FereaCrystalOrb());
	sScriptAI->AddScriptAI(new FereaGeneral());
	sScriptAI->AddScriptAI(new FereaMonster());
	sScriptAI->AddScriptAI(new FereaBoss());
}