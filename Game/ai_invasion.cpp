/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_invasion.cpp"
*
*/
class InvasionScript: public MonsterScriptAI
{
public:
	explicit InvasionScript(): ScriptAI("invasion_ai") { }
	virtual ~InvasionScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new InvasionAI(monster); }

	struct InvasionAI: public InvasionBasicAI
	{
		DECLARE_STRUCT(TickTimer, AOESkillTime);

		explicit InvasionAI(Monster* monster): InvasionBasicAI(monster) { }
		virtual ~InvasionAI() {}

		void OnCreate()
		{
			this->SetInvasionID(static_cast<uint32>(me()->m_AdditionalDataInt[0]));
			this->SetInvasionGroup(static_cast<uint32>(me()->m_AdditionalDataInt[1]));
			this->SetInvasionSubGroup(static_cast<uint32>(me()->m_AdditionalDataInt[2]));

			if ( me()->GetClass() == 79 )
			{
				this->SetEventAnimation(EVENT_STATE_GOLDEN_DRAGON);
			}
			else if ( me()->GetClass() == 44 || me()->GetClass() == 42 )
			{
				this->SetEventAnimation(EVENT_STATE_RED_DRAGON);
			}
		}

		void OnDespawn()
		{
			sObjectMgr->UpdateInvasionAnimation(this->GetEventAnimation(), me()->GetEntry(), me()->GetWorldId(), false);
		}

		void OnDie()
		{
			sObjectMgr->UpdateInvasionAnimation(this->GetEventAnimation(), me()->GetEntry(), me()->GetWorldId(), false);

			if ( me()->GetClass() == 476 )
			{
				Object* pObject = nullptr;

				VIEWPORT_LOOP_OBJECT(me(), pObject)

					if ( !pObject->IsPlayer() || !pObject->IsPlaying() )
						continue;

					if ( !IN_RANGE(me(), pObject, 10) )
						continue;

					pObject->ToPlayer()->AddBuff(BUFF_SANTA_BLESSING_CHRISTMAS, 
						BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, 100), 
						BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, 100), 
						1800, BUFF_FLAG_COMBAT | BUFF_FLAG_DB_STORE, me());

				VIEWPORT_CLOSE
			}

			InvasionData * pInvasionData = sInvasionMgr->GetInvasionData(this->GetInvasionID());

			if ( !pInvasionData )
				return;

			Invasion* pInvasion = pInvasionData->GetInvasion(this->GetInvasionID(), this->GetInvasionGroup(), this->GetInvasionSubGroup());

			if ( !pInvasion )
				return;

			Invasion::MonsterData const* data = pInvasion->monsterGet(me());

			if ( data && data->boss.Is(1) )
			{
				InvasionDataDB const* invasion = sInvasionMgr->GetInvasionDataDB(pInvasion->GetID());

				if ( !invasion || invasion->GetOnKillBoss().empty() )
					return;

				sObjectMgr->SendEventNotification(NOTICE_GLOBAL, sObjectMgr->MakeText(invasion->GetOnKillBoss(), me(), me()->GetMaxAttacker()).c_str());
			}
		}
	
		void OnRespawn()
		{
			sObjectMgr->UpdateInvasionAnimation(this->GetEventAnimation(), me()->GetEntry(), me()->GetWorldId(), true);
		}

		bool OnAttack(Unit* pVictim, bool common)
		{
			InvasionData * pInvasionData = sInvasionMgr->GetInvasionData(this->GetInvasionID());

			if ( !pInvasionData )
				return false;

			Invasion* pInvasion = pInvasionData->GetInvasion(this->GetInvasionID(), this->GetInvasionGroup(), this->GetInvasionSubGroup());

			if ( !pInvasion )
				return false;

			Invasion::MonsterData const* data = pInvasion->monsterGet(me());

			if ( data && data->boss.Is(1) )
			{
				InvasionGroupDB const* pInvasionGroupDB = sInvasionMgr->GetInvasionGroupDB(pInvasion->GetID(), pInvasion->GetGroup(), pInvasion->GetSubGroup());

				if (!pInvasionGroupDB || !pInvasionGroupDB->IsFlag(INVASION_FLAG_PROTECT_BOSS))
				{
					return false;
				}

				Invasion::MonsterDataMap monster_map = pInvasion->GetMonsterMap();

				for ( Invasion::MonsterDataMap::iterator it = monster_map.begin(); it != monster_map.end(); ++it )
				{
					if ( !it->first || !it->first->IsLive() || it->first == me() )
						continue;

					if (!roll_chance_i(data->attack_percent.get()))
						continue;

					it->first->SetTarget(pVictim);
				}
			}

			return false;
		}

		void OnBeenAttacked(Unit* pAttacker)
		{
			InvasionData * pInvasionData = sInvasionMgr->GetInvasionData(this->GetInvasionID());

			if ( !pInvasionData )
				return;

			Invasion* pInvasion = pInvasionData->GetInvasion(this->GetInvasionID(), this->GetInvasionGroup(), this->GetInvasionSubGroup());

			if ( !pInvasion )
				return;

			Invasion::MonsterData const* data = pInvasion->monsterGet(me());

			if ( data && data->boss.Is(1) )
			{
				InvasionGroupDB const* pInvasionGroupDB = sInvasionMgr->GetInvasionGroupDB(pInvasion->GetID(), pInvasion->GetGroup(), pInvasion->GetSubGroup());

				if (!pInvasionGroupDB || !pInvasionGroupDB->IsFlag(INVASION_FLAG_PROTECT_BOSS))
				{
					return;
				}

				Invasion::MonsterDataMap monster_map = pInvasion->GetMonsterMap();

				for ( Invasion::MonsterDataMap::iterator it = monster_map.begin(); it != monster_map.end(); ++it )
				{
					if ( !it->first || !it->first->IsLive() || it->first == me() )
						continue;

					if (!roll_chance_i(data->attack_percent.get()))
						continue;

					if (!it->first->GetTarget() || roll_chance_i(30))
					{
						it->first->SetTarget(pAttacker);
					}
				}
			}
		}

		bool MoveAllowed(coord_type x, coord_type y)
		{
			if ( !MonsterAI::MoveAllowed(x, y) )
				return false;

			InvasionData * pInvasionData = sInvasionMgr->GetInvasionData(this->GetInvasionID());

			if ( !pInvasionData )
				return true;

			Invasion* pInvasion = pInvasionData->GetInvasion(this->GetInvasionID(), this->GetInvasionGroup(), this->GetInvasionSubGroup());

			if ( !pInvasion )
				return true;

			Invasion::MonsterData const* data = pInvasion->monsterGet(me());
			InvasionGroupDB const* pInvasionGroupDB = sInvasionMgr->GetInvasionGroupDB(pInvasion->GetID(), pInvasion->GetGroup(), pInvasion->GetSubGroup());

			if (!pInvasionGroupDB || !pInvasionGroupDB->IsFlag(INVASION_FLAG_FOLLOW_BOSS))
			{
				return true;
			}

			return Util::Distance(x, y, pInvasion->GetX(), pInvasion->GetY()) <= 3;
		}

		bool EnableAttack(Unit* pUnit, Skill* pSkill, int32 count)
		{
			InvasionData * pInvasionData = sInvasionMgr->GetInvasionData(this->GetInvasionID());

			if (!pInvasionData)
			{
				return true;
			}

			Invasion* pInvasion = pInvasionData->GetInvasion(this->GetInvasionID(), this->GetInvasionGroup(), this->GetInvasionSubGroup());

			if (!pInvasion)
			{
				return true;
			}

			InvasionGroupDB const* pInvasionGroupDB = sInvasionMgr->GetInvasionGroupDB(pInvasion->GetID(), pInvasion->GetGroup(), pInvasion->GetSubGroup());

			if (!pInvasionGroupDB)
			{
				return true;
			}

			if (!pInvasionGroupDB->IsFlag(INVASION_FLAG_OFFATTACK_DAMAGE))
			{
				return true;
			}

			if (!pUnit)
			{
				return false;
			}

			Unit* pUnitOwner = pUnit->GetSummoner();

			if (!pUnitOwner)
			{
				return false;
			}

			Player* pPlayer = pUnitOwner->ToPlayer();

			if (!pPlayer)
			{
				return false;
			}

			if (pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
			{
				return false;
			}

			return true;
		}
	};
};

void AddSC_Invasion()
{
	sScriptAI->AddScriptAI(new InvasionScript());
}