#define MAX_MEDUSA_SUMMON			7

class MedusaScript: public MonsterScriptAI
{
public:
	explicit MedusaScript(): ScriptAI("medusa_ai") { }
	virtual ~MedusaScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new MedusaAI(monster); }

	struct MedusaAI: public MonsterAI
	{
		DECLARE_ARRAY_STRUCT_PTR(Monster, Summoned, MAX_MEDUSA_SUMMON);

		explicit MedusaAI(Monster* monster): MonsterAI(monster)
		{
			
		}

		virtual ~MedusaAI()
		{

		}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

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

			me()->TeleportToRegen();

			this->PushBackPlayers(true);

			return false;
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

			if ( !me()->CheckWall(pUnit) )
			{
				return false;
			}

			return true;
		}

		void OnRespawn()
		{
			this->InitSummoned();

			for ( int32 i = 0; i < MAX_MEDUSA_SUMMON; ++i )
			{
				Monster* pMonster = sObjectMgr->MonsterTryAdd(560, me()->GetWorldId());

				if ( pMonster )
				{
					pMonster->SetBasicLocation(me()->GetSpawnX(0), me()->GetSpawnY(0), me()->GetSpawnX(1), me()->GetSpawnY(1));
					pMonster->SetWorldId(me()->GetWorldId());
					pMonster->SetInstance(me()->GetInstance());
					pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					pMonster->SetScriptName("medusa_summon_ai");
					pMonster->AddAdditionalDataPtr(0, me());
					pMonster->AddToWorld();
										
					this->SetSummoned(i, pMonster);
				}
			}
		}

		void OnDie()
		{
			me()->LogKillData(true);
			
			for ( int32 i = 0; i < MAX_MEDUSA_SUMMON; ++i )
			{
				Monster* pMonster = this->GetSummoned(i);

				if ( pMonster && pMonster->IsPlaying() && pMonster->IsAIGroup("medusa_summon_ai") )
				{
					pMonster->Kill();
				}

				this->SetSummoned(i, nullptr);
			}
		}
	};
};

class MedusaSummonScript: public MonsterScriptAI
{
public:
	explicit MedusaSummonScript(): ScriptAI("medusa_summon_ai") { }
	virtual ~MedusaSummonScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new MedusaAI(monster); }

	struct MedusaAI: public MonsterAI
	{
		DECLARE_PTR(Monster, Medusa);
		DECLARE_STRUCT(TickTimer, UpdateTime);

		explicit MedusaAI(Monster* monster): MonsterAI(monster)
		{
			this->GetUpdateTime()->Reset();
			this->GetUpdateTime()->Start(IN_MILLISECONDS * 2);
		}

		virtual ~MedusaAI()
		{

		}

		bool Update()
		{
			if ( !this->GetMedusa() )
			{
				me()->Remove();
				return true;
			}

			if ( this->GetUpdateTime()->Elapsed() && !me()->IsTeleporting() )
			{
				World* pWorld = me()->GetWorld();
				int32 distance = Util::Distance(me()->GetX(), me()->GetY(), this->GetMedusa()->GetX(), this->GetMedusa()->GetY());

				if ( distance > 10 && pWorld )
				{
					int16 x = this->GetMedusa()->GetX();
					int16 y = this->GetMedusa()->GetY();

					pWorld->GetFreeLocation(x, y, 3, 3, 10);

					me()->MagicAttackSend(ENTRY(me()), SKILL_TELEPORT);
					me()->SkillTeleportUse(x, y);
					return true;
				}
			}

			return false;
		}

		void OnCreate()
		{
			this->SetMedusa((Monster*)me()->m_AdditionalDataPtr[0]);
		}

		bool MoveAllowed(int16 x, int16 y)
		{
			if ( !MonsterAI::MoveAllowed(x, y) )
				return false;

			if ( !this->GetMedusa() )
				return false;

			int32 distance = Util::Distance(me()->GetX(), me()->GetY(), this->GetMedusa()->GetX(), this->GetMedusa()->GetY());

			return distance <= 10;
		}
	};
};

void AddSC_Medusa()
{
	sScriptAI->AddScriptAI(new MedusaScript());
	sScriptAI->AddScriptAI(new MedusaSummonScript());
}