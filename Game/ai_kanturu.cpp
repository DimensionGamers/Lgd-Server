/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_kanturu.cpp"
*
*/

bool KanturuBasicAI::IsInSamePhase() const
{
	return this->m_state == sKanturuMgr->GetState() && this->m_sub_state == sKanturuMgr->GetSubState();
}

class KanturuScript: public MonsterScriptAI
{
public:
	explicit KanturuScript(): ScriptAI(KANTURU_AI_NAME) { }
	virtual ~KanturuScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new KanturuAI(monster); }

	struct KanturuAI: public KanturuBasicAI
	{
		explicit KanturuAI(Monster* monster): KanturuBasicAI(monster) { }
		virtual ~KanturuAI() {}

		bool Update()
		{
			if ( !this->IsInSamePhase() )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnDie()
		{
			sKanturuMgr->KillMonster();
		}
	};
};

class KanturuMayaScript: public MonsterScriptAI
{
public:
	explicit KanturuMayaScript(): ScriptAI(KANTURU_MAYA_AI_NAME) { }
	virtual ~KanturuMayaScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new KanturuMayaAI(monster); }

	struct KanturuMayaAI: public KanturuBasicAI
	{
		DECLARE_STRUCT(TickTimer, MayaSkillTime);

		explicit KanturuMayaAI(Monster* monster): KanturuBasicAI(monster)
		{
			this->GetMayaSkillTime()->Reset();
		}
		virtual ~KanturuMayaAI() {}

		bool Update()
		{
			if ( !this->IsInSamePhase() )
			{
				me()->Remove();
				return true;
			}

			if ( sKanturuMgr->GetState() == KANTURU_STATE_BATTLE_OF_MAYA )
			{
				if ( sKanturuMgr->GetSubState() == KANTURU_MAYA_START_1 ||
					 sKanturuMgr->GetSubState() == KANTURU_MAYA_START_2 ||
					 sKanturuMgr->GetSubState() == KANTURU_MAYA_START_3 )
				{
					this->IceStorm();
				}

				if ( sKanturuMgr->GetSubState() == KANTURU_MAYA_1 ||
					 sKanturuMgr->GetSubState() == KANTURU_MAYA_2 ||
					 sKanturuMgr->GetSubState() == KANTURU_MAYA_3 )
				{
					this->Skill();
				}
			}

			return true;
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			return false;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void IceStorm()
		{
			if (!roll_chance_i(sGameServer->kanturu_ice_storm_rate.get(), 10000))
				return;

			KANTURU_WIDE_ATTACK_AREA pMsg(me()->GetClass(), 0);

			World* pWorld = sWorldMgr->GetWorld(WORLD_KANTURU_BOSS);

			if ( !pWorld )
				return;

			Player* pPlayer = nullptr;

			WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
			for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
			{
				pPlayer = it->second;

				if ( !pPlayer )
					continue;

				if ( !pPlayer->IsKanturuAllowed() )
					continue;

				pPlayer->sendPacket(MAKE_PCT(pMsg));
				me()->SpecialMagicAttack(SKILL_SPECIAL_MAYA_HAND_ICE_STORM, -1, pPlayer);
			}
		}

		void Skill()
		{
			if ( !this->GetMayaSkillTime()->Elapsed(sGameServer->kanturu_skill_time.get()) )
				return;

			me()->SpecialMagicAttack(SKILL_SPECIAL_BROKEN_SHOWER);

			KANTURU_WIDE_ATTACK_AREA pMsg(me()->GetClass(), 1);

			World* pWorld = sWorldMgr->GetWorld(WORLD_KANTURU_BOSS);

			if ( !pWorld )
				return;

			Player* pPlayer = nullptr;

			WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
			for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
			{
				pPlayer = it->second;

				if ( !pPlayer )
					continue;

				if ( !pPlayer->IsKanturuAllowed() )
					continue;

				pPlayer->sendPacket(MAKE_PCT(pMsg));
				me()->SpecialMagicAttack(SKILL_SPECIAL_BROKEN_SHOWER, -1, pPlayer);

				if ( pPlayer->hasMoonstoneRing() )
					continue;

				pPlayer->Kill();
			}
		}
	};
};

class KanturuMayaHandScript: public MonsterScriptAI
{
public:
	explicit KanturuMayaHandScript(): ScriptAI(KANTURU_MAYA_HAND_AI_NAME) { }
	virtual ~KanturuMayaHandScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new KanturuMayaHandAI(monster); }

	struct KanturuMayaHandAI: public KanturuBasicAI
	{
		DECLARE_STRUCT(TickTimer, MayaHandSkillTime);

		explicit KanturuMayaHandAI(Monster* monster): KanturuBasicAI(monster)
		{
			this->GetMayaHandSkillTime()->Reset();
			this->GetMayaHandSkillTime()->Start(1000);
		}
		virtual ~KanturuMayaHandAI() {}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			if ( !this->IsInSamePhase() )
			{
				me()->Remove();
				return true;
			}

			if ( this->GetMayaHandSkillTime()->Elapsed() )
			{
				me()->ChangeAIOrder(-1);
			}

			return false;
		}

		void OnDie()
		{
			sKanturuMgr->KillMonster();
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

#define MAX_NIGHTMARE_STATE		3

static const struct NightmareTeleportData
{
	coord_type x;
	coord_type y;
} NightmareTeleport[MAX_NIGHTMARE_STATE] =
{
	{79, 100},
	{78, 124},
	{78, 141}
};

class KanturuNightmareScript: public MonsterScriptAI
{
public:
	explicit KanturuNightmareScript(): ScriptAI(KANTURU_NIGHTMARE_NAME) { }
	virtual ~KanturuNightmareScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new KanturuNightmareAI(monster); }

	struct KanturuNightmareAI: public KanturuBasicAI
	{
		DECLARE_ENUM(uint8, LifeStatus);

		explicit KanturuNightmareAI(Monster* monster): KanturuBasicAI(monster)
		{
			this->SetLifeStatus(0);
		}

		virtual ~KanturuNightmareAI() {}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			if ( !this->IsInSamePhase() )
			{
				me()->Remove();
				return true;
			}

			//if ( this->UpdateTeleportStatus() )
			//	return true;

			return false;
		}

		void OnDie()
		{
			sKanturuMgr->SetNightmareAlive(false);

			me()->LogKillData(true, false);
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

		bool UpdateTeleportStatus()
		{
			uint8 life_percent = me()->PowerGetPercent(POWER_LIFE);
			bool update = false;

			switch ( this->GetLifeStatus() )
			{
			case 0:
				{
					if ( life_percent < 75 )
						update = true;
				} break;

			case 1:
				{
					if ( life_percent < 50 )
						update = true;
				} break;

			case 2:
				{
					if ( life_percent < 25 )
						update = true;
				} break;
			}

			if ( update )
			{
				me()->MagicAttackSend(ENTRY(me()), SKILL_TELEPORT);
				me()->SkillTeleportUse(NightmareTeleport[this->GetLifeStatus()].x, NightmareTeleport[this->GetLifeStatus()].y);
				me()->SetTarget(nullptr);

				sKanturuMgr->AddSummonedMonster();

				this->SetLifeStatus(this->GetLifeStatus() + 1);
			}
			
			return update;
		}
	};
};

void AddSC_Kanturu()
{
	sScriptAI->AddScriptAI(new KanturuScript());
	sScriptAI->AddScriptAI(new KanturuMayaScript());
	sScriptAI->AddScriptAI(new KanturuMayaHandScript());
	sScriptAI->AddScriptAI(new KanturuNightmareScript());
}