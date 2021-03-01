class RaklionEggScript: public MonsterScriptAI
{
public:
	explicit RaklionEggScript(): ScriptAI(RAKLION_AI_EGGS) { }
	virtual ~RaklionEggScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new RaklionEggAI(monster); }

	struct RaklionEggAI: public MonsterAI
	{
		explicit RaklionEggAI(Monster* monster): MonsterAI(monster) { }
		virtual ~RaklionEggAI() {}

		bool Update()
		{
			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			return true;
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			return sRaklion->GetState() == RAKLION_STATE_STANDBY_1 ||
				   sRaklion->GetState() == RAKLION_STATE_STANDBY_2 ||
				   sRaklion->GetState() == RAKLION_STATE_KILL_SELUPAN;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool Drop()
		{
			return true;
		}

		void OnDie()
		{
			sRaklion->ReduceEggCount(1);

			if ( Player* pPlayer = me()->GetMaxAttacker() )
			{
				pPlayer->UpdateStatistic(STATISTIC_RAKLION_POINTS, 50);
			}
		}
	};
};

class RaklionSummonScript: public MonsterScriptAI
{
public:
	explicit RaklionSummonScript(): ScriptAI(RAKLION_AI_SUMMON) { }
	virtual ~RaklionSummonScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new RaklionAI(monster); }

	struct RaklionAI: public MonsterAI
	{
		explicit RaklionAI(Monster* monster): MonsterAI(monster) { }
		virtual ~RaklionAI() {}

		bool Drop()
		{
			return true;
		}

		void OnDie()
		{
			if ( Player* pPlayer = me()->GetMaxAttacker() )
			{
				pPlayer->UpdateStatistic(STATISTIC_RAKLION_POINTS, 10);
			}

			sRaklion->ReduceSummonCount(1);
		}
	};
};

#define MAX_SELUPAN_SKILL				9

class RaklionSelupanScript;
struct RaklionSelupanAI;

static const struct SelupanSkill
{
	int32 rate;
	bool able;
	uint8 start_hp_percent;
	uint8 end_hp_percent;
	uint32 delay;
	TickTimer m_time;
} m_SelupanSkill[MAX_SELUPAN_SKILL] =
{
	{ 100,	true,	100,	0,	0 },
	{ 100,	true,	100,	0,	0 },
	{ 100,	true,	100,	0,	0 },
	{ 100,	true,	75,		0,	10000 },
	{ 50,	true,	75,		0,	10000 },
	{ 100,	true,	100,	0,	0 },
	{ 100,	true,	100,	0,	8000 },
	{ 100,	false,	50,		0,	0 },
	{ 100,	true,	50,		0,	0 },
};

class RaklionSelupanScript: public MonsterScriptAI
{
public:
	explicit RaklionSelupanScript(): ScriptAI(RAKLION_AI_SELUPAN) { }
	virtual ~RaklionSelupanScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new RaklionSelupanAI(monster); }

	struct RaklionSelupanAI: public MonsterAI
	{
		DECLARE_BOOL(Inmortal);

		explicit RaklionSelupanAI(Monster* monster): MonsterAI(monster)
		{
			this->SetInmortal(false);
		}
		virtual ~RaklionSelupanAI() {}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

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

		void OnDie()
		{
			me()->LogKillData(true, false);

			sRaklion->ReduceBossCount(1);

			World* pWorld = sWorldMgr->GetWorld(WORLD_RAKLION_BOSS);

			if ( !pWorld )
				return;

			Player* pPlayer = nullptr;

			WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
			for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
			{
				pPlayer = it->second;

				if ( !pPlayer )
					continue;

				pPlayer->UpdateStatistic(STATISTIC_RAKLION_POINTS, 1000);
			}
		}

		void OnRespawn()
		{
			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_FALL, -1, me());
			this->SetInmortal(sRaklion->GetEggCount() > 0);
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			return sRaklion->GetState() == RAKLION_STATE_STANDBY_2 || sRaklion->GetState() == RAKLION_STATE_KILL_SELUPAN;
		}

		bool OnAttack(Unit* /*pVictim*/, bool common)
		{
			if ( me()->IsTeleporting() )
				return true;

			uint8 skill = RANDOM(uint8(MAX_SELUPAN_SKILL));
			int32 count = 50;

			while ( count-- > 0 )
			{
				SelupanSkill m_skill = m_SelupanSkill[skill];

				if ( !m_skill.able )
					continue;

				if ( !m_skill.m_time.Elapsed(m_skill.delay) )
					continue;

				if ( me()->PowerGetPercent(POWER_LIFE) < m_skill.end_hp_percent )
					continue;

				if ( me()->PowerGetPercent(POWER_LIFE) > m_skill.start_hp_percent )
					continue;

				if (roll_chance_i(m_skill.rate))
				{
					switch ( skill )
					{
					case 0:
						{
							this->SelupanPoison();
						} break;

					case 1:
						{
							this->SelupanFrostStorm();
						} break;

					case 2:
						{
							this->SelupanFrostStrike();
						} break;

					case 3:
						{
							this->SelupanSummon();
						} break;

					case 4:
						{
							this->SelupanHeal();
						} break;

					case 5:
						{
							this->SelupanFreeze();
						} break;

					case 6:
						{
							this->SelupanTeleport();
						} break;

					case 7:
						{
							this->SelupanInvincible();
						} break;

					case 8:
						{
							this->SelupanIncreaseAttack();
						} break;
					}
					break;
				}
			}

			return true;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void SelupanPoison()
		{
			if ( !me()->GetTarget() )
			{
				this->SelupanTeleport();
				return;
			}

			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_POISON);
		}
		
		void SelupanFrostStorm()
		{
			if ( !me()->GetTarget() )
			{
				this->SelupanTeleport();
				return;
			}

			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_FROST_STORM);
		}
		
		void SelupanFrostStrike()
		{
			if ( !me()->GetTarget() )
			{
				this->SelupanTeleport();
				return;
			}

			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_FROST_SHOCK);
		}

		void SelupanSummon()
		{
			if ( !me()->GetTarget() )
			{
				this->SelupanTeleport();
				return;
			}

			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_SUMMONS);
			sRaklion->AddSummoned();
		}

		void SelupanHeal()
		{
			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_TREARMENT);
		}

		void SelupanFreeze()
		{
			if ( !me()->GetTarget() )
			{
				this->SelupanTeleport();
				return;
			}

			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_FREEZE);
		}

		void SelupanTeleport()
		{
			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_TELEPORTED);
		}

		void SelupanInvincible()
		{
			if ( !me()->GetTarget() )
			{
				this->SelupanTeleport();
				return;
			}

			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_INVINCIBLE);
		}

		void SelupanIncreaseAttack()
		{
			if ( !me()->GetTarget() )
			{
				this->SelupanTeleport();
				return;
			}

			me()->SpecialMagicAttack(SKILL_SPECIAL_SELUPAN_BERSERKER);
		}
	};
};

void AddSC_Raklion()
{
	sScriptAI->AddScriptAI(new RaklionEggScript());
	sScriptAI->AddScriptAI(new RaklionSummonScript());
	sScriptAI->AddScriptAI(new RaklionSelupanScript());
}