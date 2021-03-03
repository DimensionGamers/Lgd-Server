/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_double_goer.cpp"
*
*/

DoppelgangerBasicAI::DoppelgangerBasicAI(Monster* monster): MonsterAI(monster)
{
	
}

DoppelgangerZone* DoppelgangerBasicAI::GetZone()
{
	DoppelgangerGround * Ground = sDoppelganger->GetGround(me()->GetEventGround());

	if ( !Ground )
	{
		return nullptr;
	}
	else
	{
		return Ground->GetZone(me()->GetEventStage());
	}
}

void DoppelgangerBasicAI::OnRespawn()
{
	DoppelgangerLevelData const* pLevelData = sDoppelganger->GetLevel(me()->GetEventGround());

	if ( !pLevelData )
	{
		me()->Remove();
		return;
	}

	if ( DoppelgangerMonster const* pMonsterData = pLevelData->GetMonster(me()->GetClass()) )
	{
		me()->SetLevel(pMonsterData->GetLevel());
		me()->PowerSet(POWER_LIFE, pMonsterData->GetLife());
		me()->PowerSetMax(POWER_LIFE, pMonsterData->GetLife());
		me()->SetScriptMaxPower(POWER_LIFE, pMonsterData->GetLife());
		me()->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, pMonsterData->GetAttackMin());
		me()->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, pMonsterData->GetAttackMax());
		me()->SetIntData(UNIT_INT_ATTACK_RATE, pMonsterData->GetAttackRate());
		me()->SetIntData(UNIT_INT_DEFENSE, pMonsterData->GetDefense());
		me()->SetIntData(UNIT_INT_DEFENSE_RATE, pMonsterData->GetDefenseRate());
		me()->SetItemBag(pMonsterData->GetItemBag());
	}
}

void DoppelgangerBasicAI::FindPath()
{
	World* pWorld = me()->GetWorld();

	if ( !pWorld )
	{
		return;
	}

	int16 tx = me()->GetX();
	int16 ty = me()->GetY();

	int32 maxmoverange = me()->GetMoveRange() * 2 + 1;

	if ( maxmoverange < 1 )
	{
		maxmoverange = 1;
	}

	int32 iMinCost = 1000000;
	int32 iMidX = -1;
	int32 iMidY = -1;

	for ( WorldAIPathList::const_iterator it = pWorld->m_path_list.begin(); it != pWorld->m_path_list.end(); ++it )
	{
		WorldAIPath const* pPath = *it;

		if ( !pPath )
		{
			continue;
		}

		if ( pPath->GetType() != me()->GetEventStage() )
		{
			continue;
		}

		float fDistX = (float)(me()->GetX() - pPath->GetX());
		float fDistY = (float)(me()->GetY() - pPath->GetY());
		int32 iPathSpotDist =  (int)sqrt( (fDistX*fDistX) + (fDistY*fDistY) );

		if ( iPathSpotDist < 10 )
		{
			fDistX = (float)(g_DoppelgangerPosition[pPath->GetType()].start_x - pPath->GetX());
			fDistY = (float)(g_DoppelgangerPosition[pPath->GetType()].start_y - pPath->GetY());
			int32 iMidDist = (int)sqrt( (fDistX*fDistX) + (fDistY*fDistY));

			if ( iMinCost > iMidDist )
			{
				if ( iMidDist )
				{
					iMinCost = iMidDist;
					iMidX = pPath->GetX();
					iMidY = pPath->GetY();
				}
			}
		}
	}

	if ( iMinCost != 1000000 )
	{
		tx = iMidX;
		ty = iMidY;
	}
	else
	{
		tx = (me()->GetX() - me()->GetMoveRange()) + Random(maxmoverange);
		ty = (me()->GetY() - me()->GetMoveRange()) + Random(maxmoverange);
	}

	me()->SetTarget(nullptr);
	me()->SetTempX(tx);
	me()->SetTempY(ty);
	me()->GetAction()->Emotion = EMOTION_REST;
	me()->GetAction()->Move = 1;
	me()->SetNextActionTime(500);
}

class DoppelgangerScript: public MonsterScriptAI
{
public:
	explicit DoppelgangerScript(): ScriptAI("ai_doppelganger") { }
	virtual ~DoppelgangerScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public DoppelgangerBasicAI
	{
		explicit AI(Monster* monster): DoppelgangerBasicAI(monster) { }
		virtual ~AI() {}

		bool Update()
		{
			DoppelgangerGround * Ground = sDoppelganger->GetGround(me()->GetEventGround());

			if ( !Ground )
			{
				me()->Remove();
				return true;
			}

			if ( Ground->GetState() != DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
			{
				me()->Remove();
				return true;
			}

			DoppelgangerZone * Zone = this->GetZone();
			
			if ( !Zone )
			{
				me()->Remove();
				return true;
			}

			if ( Zone->PlayerGetCount() <= 0 )
			{
				me()->Remove();
				return true;
			}

			if ( this->IsInPortal() )
			{
				return true;
			}

			return false;
		}

		bool IsInPortal()
		{
			DoppelgangerZone * pZone = this->GetZone();

			if ( !pZone )
			{
				me()->Remove();
				return true;
			}

			if ( Util::Distance(me()->GetX(), me()->GetY(), g_DoppelgangerPosition[pZone->GetZone()].start_x, g_DoppelgangerPosition[pZone->GetZone()].start_y) <= 4 )
			{
				me()->AddBuff(BUFF_ASCENSION, 1, 0, me());
				me()->Kill();
				pZone->MonsterPassed();
				return true;
			}

			return false;
		}

		bool IgnoreTarget()
		{
			return roll_chance_i(90);
		}

		bool MoveAttempt()
		{
			this->FindPath();
			return true;
		}

		void OnDie()
		{
			DoppelgangerZone * pZone = this->GetZone();

			if ( pZone )
			{
				pZone->IncreaseMonsterCount(1);
			}
		}
	};
};

class DoppelgangerMiddleBossScript: public MonsterScriptAI
{
public:
	explicit DoppelgangerMiddleBossScript(): ScriptAI("ai_doppelganger_boss") { }
	virtual ~DoppelgangerMiddleBossScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new DoppelgangerAI(monster); }

	struct DoppelgangerAI: public DoppelgangerBasicAI
	{
		DECLARE_STRUCT(TickTimer, BossAttackTime);

		explicit DoppelgangerAI(Monster* monster): DoppelgangerBasicAI(monster)
		{
			this->GetBossAttackTime()->Reset();
			this->GetBossAttackTime()->Start(1000);
		}
		virtual ~DoppelgangerAI() {}

		bool Update()
		{
			DoppelgangerGround * Ground = sDoppelganger->GetGround(me()->GetEventGround());

			if ( !Ground )
			{
				me()->Remove();
				return true;
			}

			if ( Ground->GetState() != DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
			{
				me()->Remove();
				return true;
			}

			DoppelgangerZone * Zone = this->GetZone();
			
			if ( !Zone )
			{
				me()->Remove();
				return true;
			}

			if ( Zone->PlayerGetCount() <= 0 )
			{
				me()->Remove();
				return true;
			}

			this->BossAttack();
			return false;
		}

		void BossAttack()
		{
			if ( !this->GetBossAttackTime()->Elapsed() )
				return;

			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsUnit() )
					continue;

				if ( pObject->IsCreature() && (!pObject->ToUnit()->GetSummoner() || !pObject->ToUnit()->GetSummoner()->IsPlayer()) )
					continue;

				if ( !IN_RANGE(me(), pObject, 2) )
					continue;

				me()->AttackProc(pObject->ToUnit(), nullptr);
				me()->PushBackSimple(pObject->ToUnit());

			VIEWPORT_CLOSE
		}

		void OnDie()
		{
			DoppelgangerZone * pZone = this->GetZone();

			if ( pZone )
			{
				pZone->CreateChest();
				pZone->IncreaseMonsterCount(1);
			}
		}

		bool IgnoreTarget()
		{
			return roll_chance_i(20);
		}

		bool MoveAttempt()
		{
			if (roll_chance_i(20))
			{
				return false;
			}

			this->FindPath();

			return true;
		}
	};
};

class DoppelgangerFinalBossScript: public MonsterScriptAI
{
public:
	explicit DoppelgangerFinalBossScript(): ScriptAI("ai_doppelganger_final_boss") { }
	virtual ~DoppelgangerFinalBossScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new DoppelgangerAI(monster); }

	struct DoppelgangerAI: public DoppelgangerBasicAI
	{
		explicit DoppelgangerAI(Monster* monster): DoppelgangerBasicAI(monster) {}
		virtual ~DoppelgangerAI() {}

		void OnCreate()
		{
			DoppelgangerZone * pZone = this->GetZone();

			if ( pZone )
			{
				pZone->SetQuest(DOPPELGANGER_QUEST_STANDBY);
			}
		}

		void OnDespawn()
		{
			DoppelgangerZone * pZone = this->GetZone();

			if ( pZone )
			{
				pZone->SetQuest(DOPPELGANGER_QUEST_FAILED);
			}
		}
		
		bool Update()
		{
			DoppelgangerGround * Ground = sDoppelganger->GetGround(me()->GetEventGround());

			if ( !Ground )
			{
				me()->Remove();
				return true;
			}

			if ( Ground->GetState() != DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
			{
				me()->Remove();
				return true;
			}

			DoppelgangerZone * Zone = this->GetZone();
			
			if ( !Zone )
			{
				me()->Remove();
				return true;
			}

			if ( Zone->PlayerGetCount() <= 0 )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnDie()
		{
			DoppelgangerZone * pZone = this->GetZone();

			if ( pZone )
			{
				pZone->CreateChest();
				pZone->SetQuest(DOPPELGANGER_QUEST_COMPLETED);
				pZone->IncreaseMonsterCount(1);
			}
		}

		bool IgnoreTarget()
		{
			return roll_chance_i(10);
		}

		bool MoveAttempt()
		{
			if (roll_chance_i(20))
			{
				return false;
			}

			this->FindPath();
			return true;
		}
	};
};

class DoppelgangerChestScript: public MonsterScriptAI
{
public:
	explicit DoppelgangerChestScript(): ScriptAI("ai_doppelganger_chest") { }
	virtual ~DoppelgangerChestScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new DoppelgangerChestAI(monster); }

	struct DoppelgangerChestAI: public DoppelgangerBasicAI
	{
		DECLARE_ENUM(uint32, PlayerGUID);

		explicit DoppelgangerChestAI(Monster* monster): DoppelgangerBasicAI(monster) { this->SetPlayerGUID(0); }
		virtual ~DoppelgangerChestAI() {}

		bool Update()
		{
			DoppelgangerGround * Ground = sDoppelganger->GetGround(me()->GetEventGround());

			if ( !Ground )
			{
				me()->Remove();
				return true;
			}

			if ( Ground->GetState() == DOPPELGANGER_GROUND_STATE_NONE )
			{
				me()->Remove();
				return true;
			}

			DoppelgangerZone * Zone = this->GetZone();
			
			if ( !Zone )
			{
				me()->Remove();
				return true;
			}

			if ( Zone->PlayerGetCount() <= 0 )
			{
				me()->Remove();
				return true;
			}

			me()->SetNextActionTime(1000);
			return true;
		}

		bool OnTalk(Player* pPlayer)
		{
			DoppelgangerGround * Ground = sDoppelganger->GetGround(me()->GetEventGround());

			if ( !Ground )
			{
				return true;
			}

			DoppelgangerZone * pZone = this->GetZone();

			if ( !pZone )
			{
				return true;
			}

			if ( me()->GetClass() == 541 )
			{
				if ( this->GetPlayerGUID() == pPlayer->GetGUID() )
				{
					pZone->OpenChest(me(), pPlayer);
				}
			}
			else
			{
				Ground->OpenGoldenChest(pPlayer, me());
			}
			
			return true;
		}

		void OnCreate()
		{
			this->SetPlayerGUID(me()->m_AdditionalDataInt[0]);
		}

		void OnRespawn()
		{

		}
	};
};

class DoppelgangerLarvaScript: public MonsterScriptAI
{
public:
	explicit DoppelgangerLarvaScript(): ScriptAI("ai_doppelganger_larva") { }
	virtual ~DoppelgangerLarvaScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new DoppelgangerAI(monster); }

	struct DoppelgangerAI: public DoppelgangerBasicAI
	{
		explicit DoppelgangerAI(Monster* monster): DoppelgangerBasicAI(monster) {}
		virtual ~DoppelgangerAI() {}

		bool Update()
		{
			DoppelgangerGround * Ground = sDoppelganger->GetGround(me()->GetEventGround());

			if ( !Ground )
			{
				me()->Remove();
				return true;
			}

			if ( Ground->GetState() != DOPPELGANGER_GROUND_STATE_PLAYING_PVE )
			{
				me()->Remove();
				return true;
			}

			DoppelgangerZone * Zone = this->GetZone();
			
			if ( !Zone )
			{
				me()->Remove();
				return true;
			}

			if ( Zone->PlayerGetCount() <= 0 )
			{
				me()->Remove();
				return true;
			}

			return false;
		}
	};
};

void AddSC_Doppelganger()
{
	sScriptAI->AddScriptAI(new DoppelgangerScript());
	sScriptAI->AddScriptAI(new DoppelgangerChestScript());
	sScriptAI->AddScriptAI(new DoppelgangerMiddleBossScript());
	sScriptAI->AddScriptAI(new DoppelgangerFinalBossScript());
	sScriptAI->AddScriptAI(new DoppelgangerLarvaScript());
}