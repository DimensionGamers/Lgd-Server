/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_champion.cpp"
*
*/

enum ChampionPhase
{
	CHAMPION_PHASE_WAIT,
	CHAMPION_PHASE_TALK_1,
	CHAMPION_PHASE_TALK_2,
	CHAMPION_PHASE_TALK_3,
	CHAMPION_PHASE_SIZE,
	CHAMPION_PHASE_FIGHT,
};

#define TALKING_TEST_1	"What is going on?. Who are you people?"
#define TALKING_TEST_2	"How you dare to wake me up?"
#define TALKING_TEST_3	"You are going to suffer. Prepare yourself"
#define TALKING_TEST_4	"Restore my powers Lord Kundun!"
#define TALKING_TEST_5	"Suffer! Suffer!"

enum
{
	CHAMPION_TIME_PUSH_BACK,
	CHAMPION_TIME_AOE_DAMAGE,
	CHAMPION_TIME_STORM,
};

class ChampionScript: public MonsterScriptAI
{
public:
	explicit ChampionScript(): ScriptAI("champion_ai") { }

	MonsterAI* GetAI(Monster* monster) const { return new ChampionAI(monster); }

	struct ChampionAI: public MonsterAI
	{
		DECLARE_PROPERTY(int32, StormCount);
		DECLARE_STRUCT(TickTimer, PushBackTime);
		DECLARE_STRUCT(TickTimer, AOEDamageTime);
		DECLARE_STRUCT(TickTimer, StormTime);
		DECLARE_STRUCT(TickTimer, DarknessTime);
		
		explicit ChampionAI(Monster* monster): MonsterAI(monster)
		{
			this->GetPushBackTime()->Reset();
			this->GetAOEDamageTime()->Reset();
			this->GetStormTime()->Reset();

			this->GetPushBackTime()->Start(15000);
			this->GetAOEDamageTime()->Start(2000);
			this->GetStormTime()->Start(1000);
			this->GetDarknessTime()->Start(30000);
			this->SetStormCount(0);
		}

		virtual ~ChampionAI() {}

		bool Update()
		{
			if ( this->phaseGet() != CHAMPION_PHASE_FIGHT )
			{
				if ( this->phaseTimeElapsed() )
					this->phaseSetNext();

				return true;
			}
			else
			{
				if ( this->GetPushBackTime()->Elapsed() )
				{
					this->PushBackAll();
					me()->Say(TALKING_TEST_5);
				}

				if ( this->GetAOEDamageTime()->Elapsed() )
				{
					this->AOEAll();
				}

				if ( this->GetStormTime()->Elapsed() )
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

				if ( this->GetDarknessTime()->Elapsed() )
				{
					this->CallToDarkness();
				}
			}

			return false;
		}

		void OnDie()
		{
			Monster * mMonster = sObjectMgr->MonsterTryAdd(1501, me()->GetWorldId());

			if ( !mMonster )
				return;
	
			mMonster->SetWorldId(me()->GetWorldId());
			mMonster->SetInstance(me()->GetInstance());
			mMonster->SetBasicLocation(me()->GetX(),me()->GetY(),me()->GetX(),me()->GetY());
			mMonster->SetDirection(me()->GetDirection());
			mMonster->SetDespawnTime(MINUTE * IN_MILLISECONDS);
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

		void OnRespawn()
		{
			this->phaseSet(CHAMPION_PHASE_WAIT);
		}

		bool EnableAttack(Unit* /*pAttacker*/, Skill*, int32 count)
		{
			return this->phaseGet() == CHAMPION_PHASE_FIGHT;
		}

		bool UpdateMovement()
		{
			return this->phaseGet() == CHAMPION_PHASE_FIGHT;
		}

		bool Drop()
		{
			return true;
		}

		void phaseSet(uint32 phase)
		{
			switch ( phase )
			{
			case CHAMPION_PHASE_WAIT:
				{
					this->phaseTimeSet(3000);
					me()->SetSendType(MAIN_OBJECT_FURNITURE);
					me()->SendCustomObjectData();
				} break;

			case CHAMPION_PHASE_TALK_1:
				{
					this->phaseTimeSet(5000);
					me()->Say(TALKING_TEST_1);
				} break;

			case CHAMPION_PHASE_TALK_2:
				{
					this->phaseTimeSet(3000);
					me()->Say(TALKING_TEST_2);
				} break;

			case CHAMPION_PHASE_TALK_3:
				{
					this->phaseTimeSet(3000);
					me()->Say(TALKING_TEST_3);
				} break;

			case CHAMPION_PHASE_SIZE:
				{
					this->phaseTimeSet(5000);
					me()->Say(TALKING_TEST_4);
					me()->AlterData(1.5f, me()->GetName());
					me()->SendCustomObjectData();
				} break;

			case CHAMPION_PHASE_FIGHT:
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

				if ( !IN_RANGE(me(), pObject, 5) )
					continue;

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

				if ( !IN_RANGE(me(), pObject, 5) )
					continue;

				me()->AttackProc(pObject->ToUnit(), nullptr, false, pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * 8 / 100, 0, DAMAGE_TYPE_DEBUFF);
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

				if ( !IN_RANGE(me(), pObject, 5) )
					continue;

				me()->AddDelayed(DELAYED_ATTACK, 50, pObject->ToUnit(), 0, pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * 8 / 100, DAMAGE_TYPE_IGNORE);
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

/*
RANDOM PK MAP

*/

void AddSC_Champion()
{
	sScriptAI->AddScriptAI(new ChampionScript());
	sScriptAI->AddScriptAI(new ChampionChestScript());
}