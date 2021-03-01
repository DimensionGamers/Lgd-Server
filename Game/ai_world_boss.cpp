/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_world_boss.cpp"
*
*/

enum ChampionPhase
{
	WORLD_BOSS_PHASE_INACTIVE,
	WORLD_BOSS_PHASE_WAIT,
	WORLD_BOSS_PHASE_TALK_1,
	WORLD_BOSS_PHASE_TALK_2,
	WORLD_BOSS_PHASE_TALK_3,
	WORLD_BOSS_PHASE_SIZE,
	WORLD_BOSS_PHASE_FIGHT,
};

#define TALKING_TEST_1	"I.... i'm free from the eternal dream"
#define TALKING_TEST_2	"Blood, revenge, suffering"
#define TALKING_TEST_3	"This world will burn under my feet"

#define TALKING_TEST_4	"My blade never misses it's mark."
#define TALKING_TEST_5	"Suffer! Suffer!"

enum
{
	WORLD_BOSS_TIME_PUSH_BACK,
	WORLD_BOSS_TIME_AOE_DAMAGE,
	WORLD_BOSS_TIME_STORM,
	WORLD_BOSS_TIME_DARKNESS,
	WORLD_BOSS_TIME_SUMMON,

	WORLD_BOSS_TIME_MAX
};

class WorldBossScript: public MonsterScriptAI
{
public:
	explicit WorldBossScript(): ScriptAI("world_boss_ai") { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_PROPERTY(int32, StormCount);
		DECLARE_ARRAY_STRUCT(TickTimer, Time, WORLD_BOSS_TIME_MAX);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			for ( int32 i = 0; i < WORLD_BOSS_TIME_MAX; ++i )
			{
				this->GetTime(i)->Start();
			}

			this->GetTime(WORLD_BOSS_TIME_PUSH_BACK)->Start(20000);
			this->GetTime(WORLD_BOSS_TIME_AOE_DAMAGE)->Start(2000);
			this->GetTime(WORLD_BOSS_TIME_STORM)->Start(1000);
			this->GetTime(WORLD_BOSS_TIME_DARKNESS)->Start(30000);
			this->GetTime(WORLD_BOSS_TIME_SUMMON)->Start(30000);
			this->SetStormCount(0);
		}

		virtual ~AI() {}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if ( this->phaseGet() == WORLD_BOSS_PHASE_INACTIVE )
			{
				if ( me()->GetPlayerViewportCount() >= sGameServer->GetWorldBossRequiredPlayer() )
				{
					this->phaseSetNext();
				}
				
				this->PushBackPlayers(false);
				return true;
			}

			if ( this->phaseGet() != WORLD_BOSS_PHASE_FIGHT )
			{
				if ( this->phaseTimeElapsed() )
					this->phaseSetNext();

				this->PushBackPlayers(false);
				return true;
			}
			else
			{
				if ( me()->GetPlayerViewportCount() <= 0 )
				{
					this->phaseSet(WORLD_BOSS_PHASE_INACTIVE);
					return true;
				}

				this->PushBackPlayers(true);

				if ( this->GetTime(WORLD_BOSS_TIME_PUSH_BACK)->Elapsed() )
				{
					this->PushBackAll();

					switch ( Random(2) )
					{
					case 0:
						{
							me()->Say(TALKING_TEST_4);
						} break;
						
					case 1:
						{
							me()->Say(TALKING_TEST_5);
						} break;
					}
				}

				if ( this->GetTime(WORLD_BOSS_TIME_AOE_DAMAGE)->Elapsed() )
				{
					this->AOEAll();
				}

				if ( this->GetTime(WORLD_BOSS_TIME_STORM)->Elapsed() )
				{
					this->IncreaseStormCount(1);

					if ( this->GetStormCount() == 30 ||
						 this->GetStormCount() == 31 ||
						 this->GetStormCount() == 32 ||
						 this->GetStormCount() == 33 ||
						 this->GetStormCount() == 34 )
					{
						this->StormMadness();
					}
					else if ( this->GetStormCount() == 35 )
					{
						this->SetStormCount(0);
					}
				}

				if ( this->GetTime(WORLD_BOSS_TIME_DARKNESS)->Elapsed() )
				{
					this->CallToDarkness();
				}

				if ( this->GetTime(WORLD_BOSS_TIME_SUMMON)->Elapsed() )
				{
					this->Summon();
				}
			}

			return false;
		}

		void OnRespawn()
		{
			this->phaseSet(WORLD_BOSS_PHASE_INACTIVE);
		}

		bool EnableAttack(Unit* /*pAttacker*/, Skill*, int32 count)
		{
			return this->phaseGet() == WORLD_BOSS_PHASE_FIGHT;
		}

		bool UpdateMovement()
		{
			return this->phaseGet() == WORLD_BOSS_PHASE_FIGHT;
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
			sWorldBoss->SetState_End();

			me()->LogKillData(true, false);
		}

		void phaseSet(uint32 phase)
		{
			switch ( phase )
			{
			case WORLD_BOSS_PHASE_INACTIVE:
				{
					me()->AddBuff(BUFF_STUN, 0, BUFF_FLAG_CONSTANT, me());
					me()->AddBuff(BUFF_ICE_ARROW, 0, BUFF_FLAG_CONSTANT, me());

					me()->SetSendType(MAIN_OBJECT_FURNITURE);
					//me()->AlterData(1.0f, me()->GetName());
					me()->SendCustomObjectData();
				} break;

			case WORLD_BOSS_PHASE_WAIT:
				{
					this->phaseTimeSet(3000);

					me()->MagicAttackSend(-1, SKILL_COMBO);
					me()->RemoveBuff(BUFF_STUN);
					me()->RemoveBuff(BUFF_ICE_ARROW);
				} break;

			case WORLD_BOSS_PHASE_TALK_1:
				{
					this->phaseTimeSet(5000);
					me()->Say(TALKING_TEST_1);
				} break;

			case WORLD_BOSS_PHASE_TALK_2:
				{
					this->phaseTimeSet(3000);
					me()->Say(TALKING_TEST_2);
				} break;

			case WORLD_BOSS_PHASE_TALK_3:
				{
					this->phaseTimeSet(3000);
					me()->Say(TALKING_TEST_3);
				} break;

			case WORLD_BOSS_PHASE_SIZE:
				{
					this->phaseTimeSet(2000);
					//me()->AlterData(1.0f, me()->GetName());
					//me()->SendCustomObjectData();
				} break;

			case WORLD_BOSS_PHASE_FIGHT:
				{
					me()->SetSendType(MAIN_OBJECT_MONSTER);
					me()->SendCustomObjectData();
				} break;
			}

			this->MonsterAI::phaseSet(phase);
		}

		void PushBackAll()
		{
			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsUnit() )
					continue;

				if ( pObject->IsInSafeZone() )
					continue;

				if ( pObject->IsPlayer() && pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( pObject->IsCreature() && (!pObject->ToUnit()->GetSummoner() || !pObject->ToUnit()->GetSummoner()->IsPlayer()) )
					continue;

				if ( pObject->ToUnit()->IsTeleporting() )
					continue;

				if ( !IN_RANGE(me(), pObject, 5) )
					continue;

				if ( pObject->IsPlayer() && (pObject->ToPlayer()->GetCounter(PLAYER_COUNTER_WORLD_BOSS) >= sGameServer->GetWorldBossMaxPKPoints()) )
				{
					continue;
				}

				me()->PushBackCount(pObject->ToUnit(), 2);
				pObject->ToUnit()->AddBuff(BUFF_STUN, 3, 0, me());

			VIEWPORT_CLOSE

			me()->MagicAttackSend(ENTRY(me()), SKILL_COMBO);
		}

		void AOEAll()
		{
			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsUnit() )
					continue;

				if ( pObject->IsInSafeZone() )
					continue;

				if ( pObject->IsPlayer() && pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( pObject->IsCreature() && (!pObject->ToCreature()->GetSummoner() || !pObject->ToCreature()->GetSummoner()->IsPlayer()) )
					continue;

				if (!IN_RANGE(me(), pObject, sGameServer->GetWorldBossAOERange()))
					continue;

				me()->AttackProc(pObject->ToUnit(), nullptr, false, pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * 10 / 100, 0, DAMAGE_TYPE_DEBUFF);
				pObject->ToUnit()->AddBuff(BUFF_LIGHTNING_SHOCK, 1, 0, me());

			VIEWPORT_CLOSE
		}

		void StormMadness()
		{
			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsUnit() )
					continue;

				if ( pObject->IsInSafeZone() )
					continue;

				if ( pObject->IsPlayer() && pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( pObject->IsCreature() && (!pObject->ToUnit()->GetSummoner() || !pObject->ToUnit()->GetSummoner()->IsPlayer()) )
					continue;

				if (!IN_RANGE(me(), pObject, sGameServer->GetWorldBossAOERange()))
					continue;

				me()->AddDelayed(DELAYED_ATTACK, 50, pObject->ToUnit(), 0, pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * 10 / 100, DAMAGE_TYPE_IGNORE);
				pObject->ToUnit()->AddBuff(BUFF_GIGANTIC_STORM, 1, 0, me());
				
			VIEWPORT_CLOSE
		}

		void CallToDarkness()
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			Monster * mMonster = sObjectMgr->MonsterTryAdd(708, me()->GetWorldId());

			if ( !mMonster )
			{
				return;
			}

			coord_type x = me()->GetX();
			coord_type y = me()->GetY();

			if ( pWorld->GetFreeLocation(x, y, 4, 4, 10) )
			{
				
			}
	
			mMonster->SetScriptName("ground_darkness_ai");
			mMonster->SetWorldId(me()->GetWorldId());
			mMonster->SetInstance(me()->GetInstance());
			mMonster->SetBasicLocation(x, y, x, y);
			mMonster->SetDirection(me()->GetDirection());
			mMonster->SetDespawnTime(15 * IN_MILLISECONDS);
			mMonster->SetDespawnTick(MyGetTickCount());
			mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			mMonster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
			mMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
			mMonster->SetMoveDistance(0);

			if ( !mMonster->GetWorld() )
			{
				mMonster->SetConnectStatus(CONNECT_STATUS_NONE);
				return;
			}

			mMonster->AddToWorld();
		}

		void Summon()
		{

		}
	};
};

class ChampionChestScript: public MonsterScriptAI
{
public:
	explicit ChampionChestScript(): ScriptAI("champion_chest_ai") { }

	MonsterAI* GetAI(Monster* monster) const { return new ChampionAI(monster); }

	struct ChampionAI: public MonsterAI
	{
		typedef std::map<uint32, uint8> RewardedPlayerList;
		RewardedPlayerList m_rewarded;

		explicit ChampionAI(Monster* monster): MonsterAI(monster) { me()->SetNpcFunction("shop_champion_chest"); }
		virtual ~ChampionAI() { m_rewarded.clear(); }

		bool Update()
		{
			return true;
		}

		bool OnItemBuy(Player* pPlayer, uint8 slot)
		{
			Shop const* shop = sShopMgr->GetShop(me()->GetNpcFunction());

			if ( !shop )
				return true;

			item_template const* item_info = sItemMgr->GetItem(shop->GetItem(slot)->GetItem());

			if ( !item_info )
				return true;

			uint8 & count = this->m_rewarded[pPlayer->GetGUID()];

			if ( count >= 3 )
			{
				me()->SayTo(pPlayer, "You can't take more jewels.");
				return true;
			}

			if ( !pPlayer->GetInventory()->IsEmptySpace(shop->GetItem(slot)->GetItem()) )
			{
				me()->SayTo(pPlayer, "Your inventory is full.");
				return true;
			}

			count++;

			sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, *shop->GetItem(slot));
			pPlayer->ItemBuyResult();
			return true;
		}

		bool ViewportListAddConditions(Unit* pAdd)
		{
			Player* pPlayer = pAdd->ToPlayer();

			if ( !pPlayer )
			{
				return false;
			}

			uint8 & count = this->m_rewarded[pPlayer->GetGUID()];

			if ( count >= 3 )
			{
				return false;
			}

			return true;
		}

		bool ViewportListRemoveConditions(Unit* pRemove)
		{
			Player* pPlayer = pRemove->ToPlayer();

			if ( !pPlayer )
			{
				return true;
			}

			uint8 & count = this->m_rewarded[pPlayer->GetGUID()];

			if ( count >= 3 )
			{
				return true;
			}

			return false;
		}
	};
};

/*
Drop Random ( nixies, ferea, mcgriffi, sylvester )
PK incrementa daño 1% por kill
Para pegar al boss necesitas 3 kill
Para que inicie el boss necesitas 20 pjs
*/

void AddSC_WorldBoss()
{
	sScriptAI->AddScriptAI(new WorldBossScript());
}