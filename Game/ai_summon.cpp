/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_summon.cpp"
*
*/

class SummonScript: public MonsterScriptAI
{
public:
	explicit SummonScript(): ScriptAI("summon_ai") { }
	virtual ~SummonScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new SummonAI(monster); }

	struct SummonAI: public SummonPlayerAI
	{
		DECLARE_ENUM(int32, HP);

		explicit SummonAI(Monster* monster): SummonPlayerAI(monster)
		{
			this->SetHP(0);
		}

		virtual ~SummonAI() {}

		bool Update()
		{
			if ( !me()->GetSummoner() || !me()->GetSummoner()->IsPlayer() || !me()->GetSummoner()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			int32 hp = me()->PowerGet(POWER_LIFE);

			if ( this->GetHP() != hp )
			{
				me()->GetSummoner()->ToPlayer()->SendSummonedHP(hp, me()->PowerGetTotal(POWER_LIFE));
				this->SetHP(hp);
			}

			if ( this->UpdateLocation() )
				return true;

			return false;
		}
	};
};

class IllusionScript: public MonsterScriptAI
{
public:
	explicit IllusionScript(): ScriptAI("summon_illusion_ai") { }
	virtual ~IllusionScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new SummonAI(monster); }

	struct SummonAI: public SummonPlayerAI
	{
		DECLARE_ENUM(int32, HP);
		uint8 preview[MAX_PREVIEW_DATA];

		explicit SummonAI(Monster* monster): SummonPlayerAI(monster)
		{
			this->SetHP(0);
			memset(this->preview, 0, MAX_PREVIEW_DATA);
		}

		virtual ~SummonAI() {}

		bool Update()
		{
			if ( !me()->GetSummoner() || !me()->GetSummoner()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			Player* pPlayer = me()->GetSummoner()->ToPlayer();

			if ( !pPlayer )
			{
				me()->Remove();
				return true;
			}

			me()->MagicAdd(SKILL_BLAST, 0);

			me()->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, pPlayer->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN));
			me()->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, pPlayer->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX));
			me()->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pPlayer->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN));
			me()->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pPlayer->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX));

			me()->SetIntData(UNIT_INT_ATTACK_RATE, pPlayer->GetIntData(UNIT_INT_ATTACK_RATE));
			me()->SetIntData(UNIT_INT_ATTACK_SPEED, 1500);
			me()->SetIntData(UNIT_INT_MAGIC_SPEED, 1500);

			me()->SetIntData(UNIT_INT_DEFENSE, pPlayer->GetIntData(UNIT_INT_DEFENSE));
			me()->SetIntData(UNIT_INT_DEFENSE_RATE, pPlayer->GetIntData(UNIT_INT_DEFENSE_RATE));
			me()->SetIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, pPlayer->GetIntData(UNIT_INT_CRITICAL_DAMAGE_ADD));
			me()->SetIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, pPlayer->GetIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD));
			me()->SetIntData(UNIT_INT_DOUBLE_DAMAGE_ADD, pPlayer->GetIntData(UNIT_INT_DOUBLE_DAMAGE_ADD));
			me()->SetIntData(UNIT_INT_DAMAGE_DECREASE, pPlayer->GetIntData(UNIT_INT_DAMAGE_DECREASE));
			me()->SetIntData(UNIT_INT_SKILL_ADD_DAMAGE, pPlayer->GetIntData(UNIT_INT_SKILL_ADD_DAMAGE));

			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, pPlayer->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE));
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, pPlayer->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN));
			me()->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, pPlayer->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX));
			me()->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, pPlayer->GetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE));
			me()->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, pPlayer->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE));

			me()->SetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, pPlayer->GetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE));
			me()->SetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, pPlayer->GetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE));
			me()->SetFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, pPlayer->GetFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE));
			me()->SetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, pPlayer->GetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE));

			me()->SetElementalAttribute(pPlayer->GetElementalAttribute());

			me()->SetAttackRange(5);
			me()->SetViewRange(5);
			
			uint8 preview_tmp[MAX_PREVIEW_DATA];
			for ( int32 i = 0; i < MAX_PREVIEW_DATA; ++i )
			{
				preview_tmp[i] = pPlayer->preview[i].get();
			}

			if ( memcmp(preview, preview_tmp, MAX_PREVIEW_DATA) )
			{
				memcpy(preview, preview_tmp, MAX_PREVIEW_DATA);

				PREVIEW_SEND pMsg(me()->GetEntry());
				memcpy(pMsg.preview, preview, MAX_PREVIEW_DATA);
				me()->sendPacket_viewport(MAKE_PCT(pMsg));
			}

			int32 hp = me()->PowerGet(POWER_LIFE);

			if ( this->GetHP() != hp )
			{
				me()->GetSummoner()->ToPlayer()->SendSummonedHP(hp, me()->PowerGetTotal(POWER_LIFE));
				this->SetHP(hp);
			}

			if ( this->UpdateLocation() )
				return true;

			return false;
		}

		bool OnAttack(Unit* pTarget, bool common)
		{
			if ( !pTarget )
			{
				return true;
			}

			if ( !pTarget || 
				 !pTarget->IsLive() || 
				 !pTarget->IsPlaying() || 
				 pTarget->IsTeleporting() || 
				 pTarget->IsInSafeZone() ||
				 !me()->SameDimension(pTarget) )
			{
				me()->SetTarget(nullptr);
				me()->GetAction()->Emotion = EMOTION_REST;
				me()->GetAction()->Attack = 0;
				me()->GetAction()->Move = 0;
				me()->SetNextActionTime(1000);
				return true;
			}

			me()->SetNextActionTime(1500);

			me()->AttackProc(pTarget, me()->MagicGet(SKILL_BLAST), true);
			me()->GetAction()->Attack = 0;
			me()->SetLastActionTime(MyGetTickCount());
			return true;
		}
	};
};

void AddSC_Summon()
{
	sScriptAI->AddScriptAI(new SummonScript());
	sScriptAI->AddScriptAI(new IllusionScript());
}