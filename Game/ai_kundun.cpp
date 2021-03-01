class KundunScript: public MonsterScriptAI
{
public:
	explicit KundunScript(): ScriptAI("kundun_ai") { }
	virtual ~KundunScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new KundunAI(monster); }

	struct KundunAI: public MonsterAI
	{
		DECLARE_STRUCT(TickTimer, UpdateTime);

		explicit KundunAI(Monster* monster): MonsterAI(monster)
		{
			this->GetUpdateTime()->Reset();	
		}
		virtual ~KundunAI() {}

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

			this->RefillHP();

			this->PushBackPlayers(true);

			return false;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void RefillHP()
		{
			if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_SUCCESS && sGameServer->IsCrywolfBenefitApply() && sGameServer->IsCrywolfBenefitKundunRefill() )
				return;

			if ( me()->PowerGet(POWER_LIFE) >= me()->PowerGetTotal(POWER_LIFE) )
				return;

			if ( !this->GetUpdateTime()->Elapsed(sGameServer->kundun_refill_time.get()) )
				return;

			int32 add_hp = Random(sGameServer->kundun_refill_hp.get()) + sGameServer->kundun_refill_hp_sec.get();

			me()->PowerIncrease(POWER_LIFE, add_hp);
		}

		void OnDie()
		{
			me()->LogKillData(true);
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

void AddSC_Kundun()
{
	sScriptAI->AddScriptAI(new KundunScript());
}