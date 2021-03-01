class NixiesLakeStoneScript: public MonsterScriptAI
{
public:
	explicit NixiesLakeStoneScript(): ScriptAI("nixies_lake_stone_ai") { }
	virtual ~NixiesLakeStoneScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI: public MonsterAI
	{
		DECLARE_PTR(Monster, Boss);

		explicit MobAI(Monster* monster): MonsterAI(monster)
		{
			this->SetBoss(nullptr);
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if ( sNixiesLake->GetState() != NIXIES_LAKE_STATE_OCCUPY )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetBoss() )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetBoss()->IsPlaying() || !this->GetBoss()->IsLive() )
			{
				me()->Remove();
				return true;
			}

			return true;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool Drop()
		{
			return true;
		}

		void OnCreate()
		{
			this->SetBoss((Monster*)me()->m_AdditionalDataPtr[0]);
		}
	};
};

class NixiesLakeMonsterScript: public MonsterScriptAI
{
public:
	explicit NixiesLakeMonsterScript(): ScriptAI("nixies_lake_monster_ai") { }
	virtual ~NixiesLakeMonsterScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI: public MonsterAI
	{
		DECLARE_PTR(Monster, Boss);

		explicit MobAI(Monster* monster): MonsterAI(monster)
		{
			this->SetBoss(nullptr);
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if ( sNixiesLake->GetState() != NIXIES_LAKE_STATE_OCCUPY )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetBoss() )
			{
				me()->Remove();
				return true;
			}

			if ( !this->GetBoss()->IsPlaying() || !this->GetBoss()->IsLive() )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnCreate()
		{
			this->SetBoss((Monster*)me()->m_AdditionalDataPtr[0]);
		}
	};
};


struct NixiesCircle
{
	void Reset()
	{
		this->SetX(0);
		this->SetY(0);
	}

	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
};

enum
{
	NIXIES_LAKE_BOSS_SKILL_NORMAL,
	NIXIES_LAKE_BOSS_SKILL_ENERGY_SPHERE,
	NIXIES_LAKE_BOSS_SKILL_ICE_RAIN,
	NIXIES_LAKE_BOSS_SKILL_ICE_BREATH,

	NIXIES_LAKE_BOSS_SKILL_MAX,
};

#define NIXIES_LAKE_SUMMON_STONES			2
#define NIXIES_LAKE_SUMMON_MONSTER			20

class NixiesLakeScript: public MonsterScriptAI
{
public:
	explicit NixiesLakeScript(): ScriptAI("nixies_lake_boss_ai") { }
	virtual ~NixiesLakeScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new MobAI(monster); }

	struct MobAI: public MonsterAI
	{
		DECLARE_STRUCT(TickTimer, SpecialSkillTime);
		DECLARE_ARRAY_STRUCT(NixiesCircle, SafeMagicCircle, MAX_NIXIES_LAKE_MAGIC_CIRCLE);
		DECLARE_STRUCT(TickTimer, MagicCircleTime);
		DECLARE_ENUM(uint8, MagicCircleState);

		DECLARE_BOOL(IceBoom);
		DECLARE_STRUCT(TickTimer, IceBoomTime);
		DECLARE_ENUM(coord_type, IceBoomX);
		DECLARE_ENUM(coord_type, IceBoomY);

		DECLARE_ENUM(uint16, LastSkill);
		DECLARE_STRUCT(TickTimer, LastSkillTime);

		DECLARE_BOOL(AbsorbKnowledge);
		DECLARE_STRUCT(TickTimer, AbsorbKnowledgeTime);
		DECLARE_PROPERTY(int32, AbsorbKnowledgeState);

		DECLARE_ENUM(uint8, RecoveryState);
		DECLARE_STRUCT(TickTimer, RecoveryTime);

		Monster* Stones[NIXIES_LAKE_SUMMON_STONES];

		explicit MobAI(Monster* monster): MonsterAI(monster)
		{
			this->GetSpecialSkillTime()->Reset();

			for ( int32 i = 0; i < MAX_NIXIES_LAKE_MAGIC_CIRCLE; ++i )
			{
				this->GetSafeMagicCircle(i)->Reset();
			}

			this->GetMagicCircleTime()->Reset();
			this->SetMagicCircleState(0);

			this->GetIceBoomTime()->Reset();
			this->SetIceBoom(false);
			this->SetIceBoomX(0);
			this->SetIceBoomY(0);

			this->SetLastSkill(0);

			this->SetAbsorbKnowledge(false);
			this->GetAbsorbKnowledgeTime()->Reset();
			this->SetAbsorbKnowledgeState(0);

			for ( int32 i = 0; i < NIXIES_LAKE_SUMMON_STONES; ++i )
			{
				this->Stones[i] = nullptr;
			}

			this->SetRecoveryState(0);
			this->GetRecoveryTime()->Reset();
		}

		virtual ~MobAI()
		{

		}

		bool Update()
		{
			if ( sNixiesLake->GetState() != NIXIES_LAKE_STATE_OCCUPY )
			{
				me()->Remove();
				return true;
			}

			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			this->SkillMagicCircleProcess();
			this->SkillIceBoomProcess();
			this->SkillAbsorbKnowledgeProcess();

			this->RecoveryProcess();

			if ( !this->GetLastSkillTime()->Elapsed() )
			{
				return true;
			}

			this->GetLastSkillTime()->Reset();

			return false;
		}

		bool MoveAllowed(coord_type x, coord_type y)
		{
			if ( this->IsAbsorbKnowledge() )
			{
				return false;
			}

			return MonsterAI::MoveAllowed(x, y);
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			if ( this->IsAbsorbKnowledge() )
			{
				return false;
			}

			return true;
		}

		void OnRespawn()
		{
			this->GetSpecialSkillTime()->Start();
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

		void OnDie()
		{
			sNixiesLake->ChangeState(NIXIES_LAKE_STATE_END);

			me()->LogKillData(true, false);

			int32 count = 0;

			if ( World* pWorld = sWorldMgr->GetWorld(WORLD_NIXIES_LAKE) )
			{
				WorldPlayerMap const& character_map = pWorld->GetAllPlayers();
				for ( WorldPlayerMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
				{
					Player* pPlayer = it->second;

					if ( !sNixiesLake->IsPlayerInSanctuary(pPlayer) )
					{
						continue;
					}

					++count;
					pPlayer->ClearDebuff(MAX_BUFF);
					if ( !sGameServer->IsNixiesLakeFree() )
					{
						sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(ITEMGET(14, 352)), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
					}
				}
			}

			me()->ResetItemBag();
			
			switch ( count )
			{
			case 1:
				{
					me()->SetItemBag(sGameServer->GetNixiesLakeItemBag1());
				} break;

			case 2:
				{
					me()->SetItemBag(sGameServer->GetNixiesLakeItemBag2());
				} break;

			case 3:
				{
					me()->SetItemBag(sGameServer->GetNixiesLakeItemBag3());
				} break;

			case 4:
				{
					me()->SetItemBag(sGameServer->GetNixiesLakeItemBag4());
				} break;

			case 5:
				{
					me()->SetItemBag(sGameServer->GetNixiesLakeItemBag5());
				} break;
			}
		}

		bool OnAttack(Unit* pTarget, bool common)
		{
			if ( common )
			{
				this->RunSpecialSkill();
			}
			else
			{
				if ( this->GetLastSkill() == NIXIES_LAKE_BOSS_SKILL_ID_ICE_BREATH && pTarget )
				{
					pTarget->AddBuff(BUFF_COLD, MINUTE, 0, me());
					me()->AttackProc(pTarget, nullptr);
				}
			}

			return true;
		}

		void RecoveryProcess()
		{
			if ( me()->IsViewportEmpty() )
			{
				if ( !this->GetRecoveryState() )
				{
					this->SetRecoveryState(1);
					this->GetRecoveryTime()->Start();
				}
			}
			else
			{
				if ( this->GetRecoveryState() )
				{
					this->SetRecoveryState(0);
					this->GetRecoveryTime()->Start();
				}
			}

			switch ( this->GetRecoveryState() )
			{
			case 0:
				{
				} break;

			case 1:
				{
					if ( this->GetRecoveryTime()->Elapsed(sGameServer->GetNixiesLakeBossRecoveryStandyTime()) )
					{
						this->SetRecoveryState(2);
					}
				} break;

			case 2:
				{
					if ( this->GetRecoveryTime()->Elapsed(sGameServer->GetNixiesLakeBossRecoveryTime()) )
					{
						me()->PowerIncrease(POWER_LIFE, me()->PowerGetTotal(POWER_LIFE) * sGameServer->GetNixiesLakeBossRecoveryPercent() * 100, true);
					}
				} break;
			}
		}

		bool RunSpecialSkill()
		{
			if ( this->IsAbsorbKnowledge() )
			{
				this->SkillIceRain();
				return true;
			}

			if ( this->GetMagicCircleState() == 1 || 
				 this->GetMagicCircleState() == 2 )
			{
				me()->SpecialMagicSend(me(), NIXIES_LAKE_BOSS_SKILL_ID_MAGIC_CIRCLE);
				return true;
			}

			if ( this->GetLastSkill() == NIXIES_LAKE_BOSS_SKILL_ID_ICE_BREATH )
			{
				this->SkillIceBoom();
			}
			else
			{
				int32 skill = Random(NIXIES_LAKE_BOSS_SKILL_MAX);

				switch ( skill )
				{
				case NIXIES_LAKE_BOSS_SKILL_NORMAL:
					{
						this->SkillNormal();
					} break;

				case NIXIES_LAKE_BOSS_SKILL_ENERGY_SPHERE:
					{
						this->SkillEnergySphere();
					} break;

				case NIXIES_LAKE_BOSS_SKILL_ICE_RAIN:
					{
						if ( roll_chance_i(50) )
						{
							this->SkillNormal();
							return true;
						}

						this->SkillIceRain();
					} break;

				case NIXIES_LAKE_BOSS_SKILL_ICE_BREATH:
					{
						this->SkillIceBreath();
					} break;
				}
			}

			/*
			117 = Normal Hit
			118 = Ice Ball
			119 = Ice Rain Boss
			120 = Invoca algo ( puede ser skill ice individual )

			///- Pueden ser relleno
			121 = gira y hace animacion
			122 = gira y hace otra animacion
			123 = gira
			*/

			return true;
		}

		void SkillNormal()
		{
			Unit* pTarget = me()->GetTarget();

			if ( !pTarget )
			{
				this->SkillIceRain();
				return;
			}

			me()->SpecialMagicSend(pTarget, NIXIES_LAKE_BOSS_SKILL_ID_NORMAL);
			//me()->SetNextActionTime(2000);
			this->GetLastSkillTime()->Start(2000);
			me()->AddDelayed(DELAYED_ATTACK, 300, pTarget);

			this->SetLastSkill(NIXIES_LAKE_BOSS_SKILL_ID_NORMAL);
		}

		void SkillEnergySphere()
		{
			Unit* pTarget = me()->GetTarget();

			if ( !pTarget )
			{
				this->SkillIceRain();
				return;
			}

			me()->SpecialMagicSend(pTarget, NIXIES_LAKE_BOSS_SKILL_ID_ENERGY_SPHERE);
			this->SetLastSkill(NIXIES_LAKE_BOSS_SKILL_ID_ENERGY_SPHERE);

			//me()->SetNextActionTime(3000);
			this->GetLastSkillTime()->Start(2300);

			me()->AddDelayed(DELAYED_ATTACK_RANGE, 1100, pTarget, 0, 0, 0, 0, NIXIES_LAKE_BOSS_SKILL_ID_ENERGY_SPHERE);
		}

		void SkillIceRain()
		{
			me()->SpecialMagicSend(me(), NIXIES_LAKE_BOSS_SKILL_ID_ICE_RAIN);
			this->SetLastSkill(NIXIES_LAKE_BOSS_SKILL_ID_ICE_RAIN);
			//me()->SetNextActionTime(3000);
			this->GetLastSkillTime()->Start(2000);

			Object* pObject = nullptr;
			VIEWPORT_LOOP_OBJECT(me(), pObject)
				Unit* pUnit = pObject->ToUnit();

				if ( !pUnit )
				{
					continue;
				}

				me()->AddDelayed(DELAYED_ATTACK_RANGE, 500, pUnit, 2, me()->GetX(), me()->GetY(), 0, NIXIES_LAKE_BOSS_SKILL_ID_ICE_RAIN);
				me()->AddDelayed(DELAYED_ATTACK_RANGE, 1500, pUnit, 2, me()->GetX(), me()->GetY(), 0, NIXIES_LAKE_BOSS_SKILL_ID_ICE_RAIN);
				me()->AddDelayed(DELAYED_ATTACK_RANGE, 2500, pUnit, 2, me()->GetX(), me()->GetY(), 0, NIXIES_LAKE_BOSS_SKILL_ID_ICE_RAIN);
			VIEWPORT_CLOSE
		}

		void SkillIceBreath()
		{
			if (roll_chance_i(80))
			{
				this->SkillEnergySphere();
				return;
			}

			Unit* pTarget = me()->GetTarget();

			if ( !pTarget )
			{
				this->SkillIceRain();
				return;
			}

			me()->SpecialMagicSend(pTarget, NIXIES_LAKE_BOSS_SKILL_ID_ICE_BREATH);
			this->SetLastSkill(NIXIES_LAKE_BOSS_SKILL_ID_ICE_BREATH);
			this->GetLastSkillTime()->Start(8000);

			NIXIES_LAKE_ICE_BREATH pMsg(1, pTarget->GetEntry());
			sNixiesLake->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
		}

		void SkillIceBoom()
		{
			Unit* pTarget = me()->GetTarget();

			if ( !pTarget )
			{
				this->SkillIceRain();
				return;
			}

			me()->SpecialMagicSend(pTarget, NIXIES_LAKE_BOSS_SKILL_ID_ICE_BOOM);
			this->SetLastSkill(NIXIES_LAKE_BOSS_SKILL_ID_ICE_BOOM);

			this->SetIceBoom(true);
			this->SetIceBoomX(pTarget->GetX());
			this->SetIceBoomY(pTarget->GetY());
			this->GetIceBoomTime()->Start(2000);
			this->GetLastSkillTime()->Start(2000);

			
			/*NIXIES_LAKE_SINGLE_ICE_EFFECT pMsg(pTarget->GetEntry());
			pMsg.x = this->GetIceBoomX();
			pMsg.y = this->GetIceBoomY();*/
			NIXIES_LAKE_SINGLE_ICE_ATTACK pMsg(0, this->GetIceBoomX(), this->GetIceBoomY());
			sNixiesLake->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
		}

		void SkillIceBoomProcess()
		{
			if ( !this->IsIceBoom() )
			{
				return;
			}

			if ( !this->GetIceBoomTime()->Elapsed() )
			{
				return;
			}

			this->SetIceBoom(false);
						
			World* pWorld = sWorldMgr->GetWorld(WORLD_NIXIES_LAKE);

			if ( !pWorld )
			{
				return;
			}

			NIXIES_LAKE_SINGLE_ICE_ATTACK pMsg(1, this->GetIceBoomX(), this->GetIceBoomY());
			sNixiesLake->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
			
			WorldPlayerMap const& character_map = pWorld->GetAllPlayers();
			for ( WorldPlayerMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
			{
				Player* pPlayer = it->second;

				if ( !sNixiesLake->IsPlayerInSanctuary(pPlayer) )
				{
					continue;
				}

				if ( Util::Distance(pPlayer->GetX(), pPlayer->GetY(), this->GetIceBoomX(), this->GetIceBoomY()) <= 2 )
				{
					me()->AttackProc(pPlayer, nullptr);
				}
			}
		}

		void SkillMagicCircleProcess()
		{
			switch ( this->GetMagicCircleState() )
			{
			case 0:
				{
					if ( me()->PowerGetPercent(POWER_LIFE) < 50 && !this->IsAbsorbKnowledge() )
					{
						if ( this->GetSpecialSkillTime()->Elapsed(MINUTE * IN_MILLISECONDS) )
						{
							this->GenerateRandomMagicCircle();
						}
					}
				} break;

			case 1:
			case 2:
				{
					if ( this->GetMagicCircleTime()->Elapsed() )
					{
						this->GetMagicCircleTime()->Start(2000);

						this->SetMagicCircleState(this->GetMagicCircleState() + 1);

						NIXIES_LAKE_MAGIC_CIRCLE pMsg;
						pMsg.result = 1;
			
						for ( int32 i = 0; i < MAX_NIXIES_LAKE_MAGIC_CIRCLE; ++i )
						{
							pMsg.data[i].x = this->GetSafeMagicCircle(i)->GetX();
							pMsg.data[i].y = this->GetSafeMagicCircle(i)->GetY();
						}

						sNixiesLake->SendPacket((uint8*)&pMsg, pMsg.h.get_size());

						World* pWorld = sWorldMgr->GetWorld(WORLD_NIXIES_LAKE);

						if ( pWorld )
						{
							WorldPlayerMap const& character_map = pWorld->GetAllPlayers();
							for ( WorldPlayerMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
							{
								Player* pPlayer = it->second;

								if ( !sNixiesLake->IsPlayerInSanctuary(pPlayer) )
								{
									continue;
								}

								if ( this->IsInSafeCircle(pPlayer) )
								{
									continue;
								}

								pPlayer->Kill();
							}
						}
					}
				} break;

			case 3:
				{
					if ( this->GetMagicCircleTime()->Elapsed(60000) )
					{
						this->SetMagicCircleState(0);
					}
				} break;
			}
		}

		void SkillAbsorbKnowledgeProcess()
		{
			if ( !this->IsAbsorbKnowledge() )
			{
				if ( this->GetMagicCircleState() == 1 ||
					 this->GetMagicCircleState() == 2 )
				{
					return;
				}

				if ( this->GetAbsorbKnowledgeState() >= 0 && this->GetAbsorbKnowledgeState() <= 3 )
				{
					if ( me()->PowerGetPercent(POWER_LIFE) <= (80 - (this->GetAbsorbKnowledgeState() * 20)) )
					{
						this->SetAbsorbKnowledge(true);
						this->IncreaseAbsorbKnowledgeState(1);
						this->GetAbsorbKnowledgeTime()->Start();
						this->SkillAbsorbKnowledgeSummonStones();
					}
				}

				return;
			}
			///- Si rompen las estatuas, entonces el boss vuelve a la normalidad
			if ( !this->IsStoneAlive() )
			{
				this->SetAbsorbKnowledge(false);
				return;
			}
			///- Si no rompen las estatuas, entonces el boss vuelve a la normalidad y sumonea 20 mobs
			if ( this->GetAbsorbKnowledgeTime()->Elapsed(3 * MINUTE * IN_MILLISECONDS) )
			{
				this->SkillAbsorbKnowledgeSummonMonsters();
				this->SetAbsorbKnowledge(false);
				return;
			}

			/*
			This skill is used when the Knicks' physical strength drops to a certain level.
It becomes invincible for 3 minutes, and creates two stones around it.
Use ice lane skill for 3 minutes.
 
 
(* Envelope seals can only deal with property damage.)
			*/
		}

		bool IsStoneAlive()
		{
			for ( int32 i = 0; i < NIXIES_LAKE_SUMMON_STONES; ++i )
			{
				if ( this->Stones[i] && this->Stones[i]->IsPlaying() )
				{
					return true;
				}
			}

			return false;
		}

		void RemoveStones()
		{
			for ( int32 i = 0; i < NIXIES_LAKE_SUMMON_STONES; ++i )
			{
				if ( this->Stones[i] && this->Stones[i]->IsPlaying() )
				{
					this->Stones[i]->Remove();
				}

				this->Stones[i] = nullptr;
			}
		}

		void SkillAbsorbKnowledgeSummonStones()
		{
			this->RemoveStones();

			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			for ( int32 i = 0; i < NIXIES_LAKE_SUMMON_STONES; ++i )
			{
				Monster* pMonster = sObjectMgr->MonsterTryAdd(747, me()->GetWorldId());

				if ( pMonster )
				{
					coord_type x = me()->GetX();
					coord_type y = me()->GetY();

					pWorld->GetRandomLocation(x, y, 5);

					pMonster->SetWorldId(me()->GetWorldId());
					pMonster->SetBasicLocation(x, y, x, y);
					pMonster->SetMoveDistance(0);
					pMonster->SetRespawn(IN_MILLISECONDS);
					pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					pMonster->SetScriptName("nixies_lake_stone_ai");
					pMonster->AddAdditionalDataPtr(0, me());
					pMonster->AddToWorld();

					this->Stones[i] = pMonster;
				}
			}
		}

		void SkillAbsorbKnowledgeSummonMonsters()
		{
			this->RemoveStones();

			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			for ( int32 i = 0; i < NIXIES_LAKE_SUMMON_MONSTER; ++i )
			{
				Monster* pMonster = sObjectMgr->MonsterTryAdd(Random(3) + 743, me()->GetWorldId());

				if ( pMonster )
				{
					coord_type x = me()->GetX();
					coord_type y = me()->GetY();

					pWorld->GetRandomLocation(x, y, 10);

					pMonster->SetWorldId(me()->GetWorldId());
					pMonster->SetBasicLocation(x, y, x, y);
					pMonster->SetMoveDistance(30);
					pMonster->SetRespawn(IN_MILLISECONDS);
					pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					pMonster->SetScriptName("nixies_lake_monster_ai");
					pMonster->AddAdditionalDataPtr(0, me());
					pMonster->AddToWorld();
				}
			}
		}

		void GenerateRandomMagicCircle()
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
			{
				return;
			}

			for ( int32 i = 0; i < MAX_NIXIES_LAKE_MAGIC_CIRCLE; ++i )
			{
				coord_type x = me()->GetX();
				coord_type y = me()->GetY();

				pWorld->GetFreeLocation(x, y, 7, 7, 20);

				this->GetSafeMagicCircle(i)->SetX(x);
				this->GetSafeMagicCircle(i)->SetY(y);
			}

			this->GetMagicCircleTime()->Start(5000);
			this->SetMagicCircleState(1);

			NIXIES_LAKE_MAGIC_CIRCLE pMsg;
			
			for ( int32 i = 0; i < MAX_NIXIES_LAKE_MAGIC_CIRCLE; ++i )
			{
				pMsg.data[i].x = this->GetSafeMagicCircle(i)->GetX();
				pMsg.data[i].y = this->GetSafeMagicCircle(i)->GetY();
			}

			sNixiesLake->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
		}

		bool IsInSafeCircle(Player* pPlayer)
		{
			for ( int32 i = 0; i < MAX_NIXIES_LAKE_MAGIC_CIRCLE; ++i )
			{
				if ( Util::Distance(pPlayer->GetX(), pPlayer->GetY(), this->GetSafeMagicCircle(i)->GetX(), this->GetSafeMagicCircle(i)->GetY()) <= 2 )
				{
					return true;
				}
			}

			return false;
		}
	};
};

void AddSC_NixiesLake()
{
	sScriptAI->AddScriptAI(new NixiesLakeScript());
	sScriptAI->AddScriptAI(new NixiesLakeStoneScript());
	sScriptAI->AddScriptAI(new NixiesLakeMonsterScript());
}