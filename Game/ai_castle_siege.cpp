/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_castle_siege.cpp"
*
*/

class LifeStoneScript: public MonsterScriptAI
{
public:
	explicit LifeStoneScript(): ScriptAI(CS_LIFE_STONE_AI) { }

	MonsterAI* GetAI(Monster* monster) const { return new LifeStoneAI(monster); }

	struct LifeStoneAI: public LifeStoneBasicAI
	{
		explicit LifeStoneAI(Monster* monster): LifeStoneBasicAI(monster) { }
		virtual ~LifeStoneAI() {}

		bool Update()
		{
			me()->SetNextActionTime(1000);

			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
			{
				me()->Kill();
				return true;
			}

			if ( me()->GetWorldId() != WORLD_CASTLE_SIEGE )
			{
				me()->Kill();
				return true;
			}

			if ( !this->IsCreated() )
			{
				this->IncreaseCreationTime(1);
				uint8 creation_state_old = this->GetCreationState();

				if ( this->GetCreationTime() < 60 )
					this->SetCreationState(this->GetCreationTime() / 12);
				else
					this->SetCreationState(5);

				if ( this->GetCreationState() != creation_state_old )
					this->CreationStateSend();
			}

			if ( !this->IsCreated() )
				return true;

			Object * pObject = nullptr;
			Player* pPlayer = nullptr;
			me()->SetTarget(nullptr);
			bool update = false;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				pPlayer = pObject->ToPlayer();

				if ( !pPlayer )
					continue;

				if ( !pPlayer->IsLive() )
					continue;

				if ( pPlayer->GetCastleSiegeJoinSide() != me()->GetCastleSiegeJoinSide() )
					continue;

				if ( Util::Distance(me()->GetX(), me()->GetY(), pPlayer->GetX(), pPlayer->GetY()) > 3 )
					continue;

				for ( uint8 i = 0; i < POWER_MAX; ++i )
				{
					if ( i == POWER_SHIELD )
						continue;

					if ( pPlayer->PowerGet(i) < pPlayer->PowerGetTotal(i) )
					{
						pPlayer->PowerIncrease(i, pPlayer->PowerGetTotal(i) / 100, true);
					}
				}

			VIEWPORT_CLOSE

			return true;
		}

		void CreationStateSend()
		{
			LIFE_STONE_STATE_SEND pMsg(me()->GetEntry(), this->GetCreationState());
			me()->sendPacket_viewport(MAKE_PCT(pMsg));
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool Drop()
		{
			return true;
		}

		bool SendHPBar()
		{
			return false;
		}

		bool InmuneToRadiance() const
		{
			return true;
		}
	};
};

class MercenaryScript: public MonsterScriptAI
{
public:
	explicit MercenaryScript(): ScriptAI("castle_siege_mercenary_ai") { }

	MonsterAI* GetAI(Monster* monster) const { return new MercenaryAI(monster); }

	struct MercenaryAI: public MonsterAI
	{
		explicit MercenaryAI(Monster* monster): MonsterAI(monster) { }
		virtual ~MercenaryAI() {}

		bool Update()
		{
			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
			{
				me()->Kill();
			}
			else if ( me()->GetWorldId() == WORLD_CASTLE_SIEGE )
			{
				this->EnemySearch();

				if ( me()->GetTarget() )
				{
					me()->Attack();
				}

				me()->SetNextActionTime(me()->GetIntData(UNIT_INT_ATTACK_SPEED));
			}
			else
			{
				me()->Kill();
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

		bool SendHPBar()
		{
			return false;
		}

		bool InmuneToRadiance() const
		{
			return true;
		}

		bool ManageThreat() const { return false; }
		
		void EnemySearch()
		{
			Object * pObject = nullptr;
			me()->SetTarget(nullptr);

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsPlayer() || !pObject->IsLive() || pObject->ToUnit()->IsTeleporting() )
					continue;

				if ( pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( me()->GetCastleSiegeJoinSide() == pObject->ToUnit()->GetCastleSiegeJoinSide() )
					continue;

				int16 distance_x = me()->GetX() - pObject->GetX();
				int16 distance_y = me()->GetY() - pObject->GetY();

				if ( me()->GetDirection() == 1 && abs(distance_x) <= 2 )
				{
					int16 cy = me()->GetY() - me()->GetAttackRange();

					if ( cy <= pObject->GetY() && me()->GetY() >= pObject->GetY() )
					{
						me()->SetTarget(pObject->ToUnit());
						break;
					}
				}

				if ( me()->GetDirection() == 3 && abs(distance_y) <= 2 )
				{
					int16 cx = me()->GetX() - me()->GetAttackRange();

					if ( cx <= pObject->GetX() && me()->GetX() >= pObject->GetX())
					{
						me()->SetTarget(pObject->ToUnit());
						break;
					}
				}
			}
		}
	};
};

class GuardianStatueScript: public MonsterScriptAI
{
public:
	explicit GuardianStatueScript(): ScriptAI(CS_GUARDIAN_STATUE_AI) { }

	MonsterAI* GetAI(Monster* monster) const { return new GuardianStatueAI(monster); }

	struct GuardianStatueAI: public GuardianStatueBasicAI
	{
		explicit GuardianStatueAI(Monster* monster): GuardianStatueBasicAI(monster) { }
		virtual ~GuardianStatueAI() {}

		bool Update()
		{
			me()->SetNextActionTime(1000);

			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
				return true;

			if ( me()->GetWorldId() != WORLD_CASTLE_SIEGE )
				return true;

			CastleSiegeNpcData * pNpcData = sCastleSiege->GetNpcData(me()->GetClass(), this->GetID());

			if ( !pNpcData )
				return true;

			Object * pObject = nullptr;
			me()->SetTarget(nullptr);
			bool update = false;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsPlayer() || !pObject->IsLive() )
					continue;

				if ( pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( !pObject->ToUnit()->isCastleSiegeDefender() )
					continue;

				if ( Util::Distance(me()->GetX(), me()->GetY(), pObject->GetX(), pObject->GetY()) > 3 )
					continue;

				for ( uint8 i = 0; i < POWER_MAX; ++i )
				{
					if ( i == POWER_SHIELD )
						continue;

					if ( pObject->ToUnit()->PowerGet(i) < pObject->ToUnit()->PowerGetTotal(i) )
					{
						pObject->ToUnit()->PowerIncrease(i, pObject->ToUnit()->PowerGetTotal(i) * (pNpcData->regen_level.get() + 1) / 100, true);
					}
				}
			}

			return true;
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
				return false;

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

		bool SendHPBar()
		{
			return false;
		}

		void OnCreate()
		{
			this->SetID(static_cast<uint8>(me()->m_AdditionalDataInt[0]));
		}

		void OnRespawn()
		{
			CastleSiegeNpcData * pNpcData = sCastleSiege->GetNpcData(me()->GetClass(), this->GetID());

			if ( !pNpcData )
				return;

			me()->PowerSet(POWER_LIFE, pNpcData->current_life.get());
			me()->PowerSetMax(POWER_LIFE, sCastleSiege->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][pNpcData->life_level.get()].life.get());
			me()->SetScriptMaxPower(POWER_LIFE, sCastleSiege->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_STATUE][pNpcData->life_level.get()].life.get());
			me()->SetIntData(UNIT_INT_DEFENSE, sCastleSiege->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_STATUE][pNpcData->defense_level.get()].defense.get());
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


class CannonTowerScript: public MonsterScriptAI
{
public:
	explicit CannonTowerScript(): ScriptAI("castle_siege_cannon_tower_ai") { }

	MonsterAI* GetAI(Monster* monster) const { return new CannonTowerAI(monster); }

	struct CannonTowerAI: public MonsterAI
	{
		explicit CannonTowerAI(Monster* monster): MonsterAI(monster) { }
		virtual ~CannonTowerAI() {}

		bool Update()
		{
			me()->SetNextActionTime(1500);

			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
				return true;

			if ( me()->GetWorldId() != WORLD_CASTLE_SIEGE )
				return true;

			Object * pObject = nullptr;
			int32 count = 0;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if ( !pObject->IsPlayer() || !pObject->IsLive() )
					continue;

				if ( pObject->ToPlayer()->IsAdministrator() )
					continue;

				if ( pObject->ToUnit()->isCastleSiegeDefender() )
					continue;

				if ( !IN_RANGE(me(), pObject, 6) )
					continue;

				me()->AttackProc(pObject->ToUnit(), me()->MagicGet(SKILL_FLAME_OF_EVIL), true);
				++count;

			VIEWPORT_CLOSE

			if ( count > 0 )
			{
				DURATION_MAGIC_ATTACK_RESULT pMsg(SKILL_FLAME_OF_EVIL, me()->GetEntry(), me()->GetX(), me()->GetY(), 0);
				me()->sendPacket_viewport(MAKE_PCT(pMsg));
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

		void ExtraDamage(Unit* pTarget, int32 & damage)
		{
			damage = pTarget->PowerGetTotal(POWER_LIFE) * 5 / 100;
		}

		bool SendHPBar()
		{
			return false;
		}
	};
};

class CrownScript: public MonsterScriptAI
{
public:
	explicit CrownScript(): ScriptAI(CS_CROWN_AI) { }

	MonsterAI* GetAI(Monster* monster) const { return new CrownAI(monster); }

	struct CrownAI: public CrownBasicAI
	{
		DECLARE_STRUCT(TickTimer, UpdateTime);

		explicit CrownAI(Monster* monster): CrownBasicAI(monster)
		{
			this->GetUpdateTime()->Reset();
			//this->CreateTimer(0);
		}
		virtual ~CrownAI() {}

		bool Update()
		{
			me()->SetNextActionTime(500);

			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
				return true;

			if ( me()->GetWorldId() != WORLD_CASTLE_SIEGE )
				return true;

			/*if (this->IsTimerElapsed(0, IN_MILLISECONDS))
			{
				if (me()->HasBuff(BUFF_CROWN_STATUS))
				{
					me()->StateInfoSend(BUFF_CROWN_STATUS, 1);
				}
				else
				{
					me()->StateInfoSend(BUFF_CROWN_STATUS, 0);
				}

				this->StateSendSimple();
			}*/

			Player* pPlayer = this->GetPlayer();

			if ( !pPlayer )
				return true;

			if ( !pPlayer->IsPlaying() || !pPlayer->IsLive() )
			{
				sEventMgr->CastleSiegeTime(pPlayer, 1);

				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL);
				this->ResetPlayer();
				return true;
			}

			if ( !me()->SameDimension(pPlayer) )
			{
				sEventMgr->CastleSiegeTime(pPlayer, 1);

				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL);
				this->ResetPlayer();
				return true;
			}

			if ( pPlayer->GetX() != this->GetX() || pPlayer->GetY() != this->GetY() )
			{
				sEventMgr->CastleSiegeTime(pPlayer, 1);

				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL);
				this->ResetPlayer();
				return true;
			}

			if ( !pPlayer->isCastleSiegeAttacker() )
			{
				sEventMgr->CastleSiegeTime(pPlayer, 1);

				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL);
				this->ResetPlayer();
				return true;
			}

			Player* pSwitcher01 = sCastleSiege->GetCrownSwitchUser(0);
			Player* pSwitcher02 = sCastleSiege->GetCrownSwitchUser(1);

			if ( !pSwitcher01 || !pSwitcher02 )
			{
				sEventMgr->CastleSiegeTime(pPlayer, 1);

				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL);
				this->ResetPlayer();
				return true;
			}

			if ( !pSwitcher01->IsPlaying() || !pSwitcher02->IsPlaying() )
			{
				sEventMgr->CastleSiegeTime(pPlayer, 1);

				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL);
				this->ResetPlayer();
				return true;
			}

			if ( pSwitcher01->GetCastleSiegeJoinSide() != pPlayer->GetCastleSiegeJoinSide() ||
				 pSwitcher02->GetCastleSiegeJoinSide() != pPlayer->GetCastleSiegeJoinSide() )
			{
				sEventMgr->CastleSiegeTime(pPlayer, 1);

				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_FAIL);
				this->ResetPlayer();
				return true;
			}

			if ( this->GetUpdateTime()->Elapsed(IN_MILLISECONDS) )
			{
				pPlayer->IncreaseAccumulatedTime(1);

				sLog->outInfo(LOG_CASTLE_SIEGE, "Accumulated Crown Access Time [%s][%s] -> [%d]",
					pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), pPlayer->GetAccumulatedTime());
			}

			return true;
		}

		bool OnTalk(Player* pPlayer)
		{
			if ( !pPlayer || !pPlayer->IsPlaying() )
				return true;

			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
				return true;

			if ( pPlayer->GetWorldId() != WORLD_CASTLE_SIEGE )
				return true;

			if ( !pPlayer->isCastleSiegeAttacker() )
				return true;

			if ( !pPlayer->GuildIsAllianceMaster() )
				return true;

			if ( !IN_RANGE(pPlayer, me(), 3) )
				return true;

			if ( pPlayer->GetPathData()->IsStartEnd() )
				return true;

			if ( !sCastleSiege->IsCrownAvailable() )
				return true;

			Guild* pGuild = pPlayer->GuildGet();

			if ( !pGuild )
				return true;

			if ( !this->GetPlayer() )
			{
				Player* pPlayerSwitch1 = sCastleSiege->GetCrownSwitchUser(0);
				Player* pPlayerSwitch2 = sCastleSiege->GetCrownSwitchUser(1);

				if ( !pPlayerSwitch1 || !pPlayerSwitch1->IsPlaying() || !pPlayerSwitch2 || !pPlayerSwitch2->IsPlaying() )
				{
					sCastleSiege->SendCrownAccessState(pPlayer, 4);

					sLog->outInfo(LOG_CASTLE_SIEGE, "[%s][%s] Failed to Register Castle Crown (GUILD:%s)", 
						pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), pGuild->GetName());

					return true;
				}

				if ( pPlayer->GetCastleSiegeJoinSide() != pPlayerSwitch1->GetCastleSiegeJoinSide() || 
					 pPlayer->GetCastleSiegeJoinSide() != pPlayerSwitch2->GetCastleSiegeJoinSide() )
				{
					sCastleSiege->SendCrownAccessState(pPlayer, 4);

					sLog->outInfo(LOG_CASTLE_SIEGE, "[%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s)(S2:%s)", 
						pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), pGuild->GetName(), pPlayerSwitch1->GetName(), pPlayerSwitch2->GetName());

					return true;
				}

				me()->SetNextActionTime(1000);
				this->SetPlayer(pPlayer);
				this->SetX(pPlayer->GetX());
				this->SetY(pPlayer->GetY());
				sCastleSiege->SendCrownAccessState(pPlayer, CASTLE_SIEGE_CROWN_ACCESS_STATE_ATTEMPT);
				sCastleSiege->SendNotifyProgress(0, pGuild->GetName());

				pPlayer->GetTimer(PLAYER_TIMER_CASTLE_SIEGE_TIME)->Start();
		
				sLog->outInfo(LOG_CASTLE_SIEGE, "[%s][%s] Start to Register Castle Crown (GUILD:%s)", 
					pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), pGuild->GetName());
			}
			else
			{
				sCastleSiege->SendCrownAccessState(pPlayer, 3);
			}

			return true;
		}

		void StateSend()
		{
			if ( this->state == 0 )
			{
				me()->AddBuff(BUFF_CROWN_STATUS, 0, BUFF_FLAG_CONSTANT, me());
			}
			else
			{
				me()->RemoveBuff(BUFF_CROWN_STATUS);
			}

			this->StateSendSimple();
		}

		void EnteringVP(Unit* pUnit)
		{
			if (!pUnit)
			{
				return;
			}

			Player* pPlayer = pUnit->ToPlayer();

			if (!pPlayer)
			{
				return;
			}

			if (this->state == 0)
			{
				me()->StateInfoSendSingle(pPlayer, BUFF_CROWN_STATUS, 1);
			}
			else
			{
				me()->StateInfoSendSingle(pPlayer, BUFF_CROWN_STATUS, 0);
			}
		}

		void StateSendSimple(Player* pPlayer = nullptr)
		{
			CROWN_STATE pMsg(this->state.get());

			if (!pPlayer)
			{
				me()->sendPacket_viewport(MAKE_PCT(pMsg));
			}
			else
			{
				pPlayer->SEND_PCT(pMsg);
			}
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool Drop()
		{
			return true;
		}

		void ResetPlayer()
		{
			this->SetPlayer(nullptr);
			this->SetX(0);
			this->SetY(0);
		}

		bool SendHPBar()
		{
			return false;
		}
	};
};

bool CrownSwitchBasicAI::Update()
{
	me()->SetNextActionTime(1000);

	if ( me()->GetWorldId() != WORLD_CASTLE_SIEGE )
		return true;

	if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		return true;

	Player* pPlayer = this->GetPlayer();

	if ( !pPlayer )
		return true;

	if ( !pPlayer->IsPlaying() || !pPlayer->IsLive() )
	{
		sEventMgr->CastleSiegeTime(pPlayer, 0);

		sCastleSiege->SendSwitchState(pPlayer, me(), uint16(-1), 0);
		this->ResetPlayer();
		sCastleSiege->LockCrown();
		return true;
	}

	if ( !me()->SameDimension(pPlayer) )
	{
		sEventMgr->CastleSiegeTime(pPlayer, 0);

		sCastleSiege->SendSwitchState(pPlayer, me(), uint16(-1), 0);
		this->ResetPlayer();
		sCastleSiege->LockCrown();
		return true;
	}

	if ( !pPlayer->isCastleSiegeAttacker() )
	{
		sEventMgr->CastleSiegeTime(pPlayer, 0);

		sCastleSiege->SendSwitchState(pPlayer, me(), uint16(-1), 0);
		this->ResetPlayer();
		sCastleSiege->LockCrown();
		return true;
	}

	if ( !IN_RANGE(me(), pPlayer, 3) )
	{
		sEventMgr->CastleSiegeTime(pPlayer, 0);

		sCastleSiege->SendSwitchState(pPlayer, me(), uint16(-1), 0);
		this->ResetPlayer();
		sCastleSiege->LockCrown();
		return true;
	}

	return true;
}

bool CrownSwitchBasicAI::OnTalk(Player* pPlayer)
{
	if ( !pPlayer || !pPlayer->IsPlaying() )
		return true;

	if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		return true;

	if ( pPlayer->GetWorldId() != WORLD_CASTLE_SIEGE )
		return true;

	if ( !pPlayer->ToPlayer()->isCastleSiegeAttacker() )
		return true;

	if ( !IN_RANGE(pPlayer, me(), 3) )
		return true;

	if ( sCastleSiege->GuardianStatueExist() )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You have to destroy Guardian Statues before using switch.");
		return true;
	}

	Player* pPlayerSwitch = this->GetPlayer();

	if ( !pPlayerSwitch )
	{
		sCastleSiege->SendSwitchState(pPlayer, me(), -1, 1);
		this->SetPlayer(pPlayer);

		pPlayer->GetTimer(PLAYER_TIMER_CASTLE_SIEGE_TIME)->Start();

		sLog->outInfo(LOG_CASTLE_SIEGE, "%s - Start to push Castle Crown Switch (%d)",
			pPlayer->BuildLog().c_str(), this->GetID() + 1);
	}
	else if ( pPlayerSwitch != pPlayer->ToPlayer() )
	{
		sCastleSiege->SendSwitchState(pPlayer, me(), pPlayerSwitch->GetEntry(), 2);
	}

	return true;
}

class CrownSwitchScript1: public MonsterScriptAI
{
public:
	explicit CrownSwitchScript1(): ScriptAI(CS_CROWN_SWITCH_AI_1) { }

	MonsterAI* GetAI(Monster* monster) const { return new CrownSwitchAI(monster); }

	struct CrownSwitchAI: public CrownSwitchBasicAI
	{
		explicit CrownSwitchAI(Monster* monster): CrownSwitchBasicAI(monster, 0) { }
		virtual ~CrownSwitchAI() {}
	};
};

class CrownSwitchScript2: public MonsterScriptAI
{
public:
	explicit CrownSwitchScript2(): ScriptAI(CS_CROWN_SWITCH_AI_2) { }

	MonsterAI* GetAI(Monster* monster) const { return new CrownSwitchAI(monster); }

	struct CrownSwitchAI: public CrownSwitchBasicAI
	{
		explicit CrownSwitchAI(Monster* monster): CrownSwitchBasicAI(monster, 1) { }
		virtual ~CrownSwitchAI() {}
	};
};

class CastleGateScript: public MonsterScriptAI
{
public:
	explicit CastleGateScript(): ScriptAI(CS_GATE_AI) { }

	MonsterAI* GetAI(Monster* monster) const { return new CastleGateAI(monster); }

	struct CastleGateAI: public CastleGateBasicAI
	{
		explicit CastleGateAI(Monster* monster): CastleGateBasicAI(monster) { }
		virtual ~CastleGateAI() {}
		
		bool Update()
		{
			me()->SetNextActionTime(1000);

			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
			{
				if ( !me()->HasBuff(BUFF_CASTLE_SIEGE_GATE_STATUS) )
				{
					sCastleSiege->BlockReleaseCastleGate(me()->GetX(), me()->GetY(), false);
					me()->AddBuff(BUFF_CASTLE_SIEGE_GATE_STATUS, 0, BUFF_FLAG_CONSTANT, me());
				}
			}

			if (me()->HasBuff(BUFF_CASTLE_SIEGE_GATE_STATUS))
			{
				me()->StateInfoSend(BUFF_CASTLE_SIEGE_GATE_STATUS, 1);
				this->SendState(1);
			}
			else
			{
				me()->StateInfoSend(BUFF_CASTLE_SIEGE_GATE_STATUS, 0);
				this->SendState(0);
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

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
				return false;

			return !me()->HasBuff(BUFF_CASTLE_SIEGE_GATE_STATUS);
		}

		bool SendHPBar()
		{
			return false;
		}

		void OnCreate()
		{
			this->SetID(static_cast<uint8>(me()->m_AdditionalDataInt[0]));
		}

		void OnRespawn()
		{
			CastleSiegeNpcData * pNpcData = sCastleSiege->GetNpcData(me()->GetClass(), this->GetID());

			if ( !pNpcData )
				return;

			me()->PowerSet(POWER_LIFE, pNpcData->current_life.get());
			me()->PowerSetMax(POWER_LIFE, sCastleSiege->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][pNpcData->life_level.get()].life.get());
			me()->SetScriptMaxPower(POWER_LIFE, sCastleSiege->m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_GATE][pNpcData->life_level.get()].life.get());
			me()->SetIntData(UNIT_INT_DEFENSE, sCastleSiege->m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_GATE][pNpcData->defense_level.get()].defense.get());

			sCastleSiege->BlockReleaseCastleGate(me()->GetX(), me()->GetY(), false);
			me()->AddBuff(BUFF_CASTLE_SIEGE_GATE_STATUS, 0, BUFF_FLAG_CONSTANT, me());
		}

		void OnDie()
		{
			sCastleSiege->BlockReleaseCastleGate(me()->GetX(), me()->GetY(), false);

			///- Tengo que destruir la palanca correspondiente
			Monster* pLever = sCastleSiege->GetMonster(219, this->GetID());

			if ( pLever )
			{
				pLever->Remove();
			}
		}

		/*void EnteringVP(Unit* pUnit)
		{
			if ( me()->HasBuff(BUFF_CASTLE_SIEGE_GATE_STATUS) )
			{
				me()->StateInfoSend(BUFF_CASTLE_SIEGE_GATE_STATUS, 1);
			}
			else
			{
				me()->StateInfoSend(BUFF_CASTLE_SIEGE_GATE_STATUS, 0);
			}
		}*/

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


class CastleGateLeverScript: public MonsterScriptAI
{
public:
	explicit CastleGateLeverScript(): ScriptAI(CS_LEVER_AI) { }

	MonsterAI* GetAI(Monster* monster) const { return new GateLeverAI(monster); }

	struct GateLeverAI: public GateLeverBasicAI
	{
		explicit GateLeverAI(Monster* monster): GateLeverBasicAI(monster) { }
		virtual ~GateLeverAI() {}
		
		bool Update()
		{
			me()->SetNextActionTime(1000);

			return true;
		}

		bool OnTalk(Player* pPlayer)
		{
			if ( !pPlayer || !pPlayer->IsPlaying() )
				return true;

			if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
			{
				if ( pPlayer->GetCastleSiegeJoinSide() != CASTLE_SIEGE_JOIN_SIDE_DEFENSE )
				{
					sCastleSiege->SendGateState(pPlayer, 4, uint16(-1));
					return true;
				}
			}
			else
			{
				if ( !sCastleSiege->CastleOwnerMember(pPlayer) && !sCastleSiege->CastleOwnerUnionMember(pPlayer) )
				{
					sCastleSiege->SendGateState(pPlayer, 4, uint16(-1));
					return true;
				}
			}

			Monster* pGate = sCastleSiege->GetMonster(277, this->GetID()); ///- Busco el Gate que corresponde a la palanca

			if ( !pGate || !pGate->IsLive() || !pGate->IsPlaying() ) ///- Si no existe, entonces no proceso nada
			{
				sCastleSiege->SendGateState(pPlayer, 2, uint16(-1));
				return true;
			}

			sCastleSiege->SendGateState(pPlayer, 1, pGate->GetEntry());
			pPlayer->GetInterfaceState()->Set(InterfaceData::CastleSiegeLever, me());
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

		void Operate(Player* pPlayer, uint8 operation, uint16 /*gate*/)
		{
			Monster* pGate = sCastleSiege->GetMonster(277, this->GetID()); ///- Busco el Gate que corresponde a la palanca

			if ( !pGate || !pGate->IsLive() || !pGate->IsPlaying() ) ///- Si no existe, entonces no proceso nada
			{
				sCastleSiege->RequestGateOperateResult(pPlayer, 2, 0, uint16(-1));
				//LogAdd(5,"[ CASTLE SIEGE ] CGReqCsGateOperate() ERROR - Gate Doesn't Exist [%s][%s], Guild:(%s)(%d)",
				//	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
				return;
			}

			if ( operation == 0 )
			{
				sCastleSiege->BlockReleaseCastleGate(pGate->GetX(), pGate->GetY(), true);
				pGate->RemoveBuff(BUFF_CASTLE_SIEGE_GATE_STATUS);
			}
			else
			{
				sCastleSiege->BlockReleaseCastleGate(pGate->GetX(), pGate->GetY(), false);
				pGate->AddBuff(BUFF_CASTLE_SIEGE_GATE_STATUS, 0, BUFF_FLAG_CONSTANT, pGate);
			}

			sCastleSiege->RequestGateOperateResult(pPlayer, 1, operation, pGate->GetEntry());
			AI_TO(CastleGateBasicAI, pGate->GetAI())->SendState(operation);

			//LogAdd(5,"[ CASTLE SIEGE ] CGReqCsGateOperate() OK - [%s][%s], Guild:(%s)(%d), DOOR:(%d)(X:%d,Y:%d)(STATUE:%d)",
			//	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildNumber,iGateIndex,
			//	gObj[iGateIndex].X, gObj[iGateIndex].Y, gObj[iGateIndex].m_btCsGateOpen);
		}
	};
};

class CastleMachineScript: public MonsterScriptAI
{
public:
	explicit CastleMachineScript(): ScriptAI(CS_CASTLE_MACHINE_AI) { }

	MonsterAI* GetAI(Monster* monster) const { return new CastleMachineAI(monster); }

	struct CastleMachineAI: public CastleMachineBasicAI
	{
		explicit CastleMachineAI(Monster* monster): CastleMachineBasicAI(monster) { }
		virtual ~CastleMachineAI() {}

		bool Update()
		{
			me()->SetNextActionTime(1000);

			this->DamageUpdate();

			Player* pPlayer = this->GetPlayer();

			if ( !pPlayer )
				return true;

			if ( !pPlayer->IsPlaying() || !pPlayer->IsLive() || !me()->SameDimension(pPlayer) )
			{
				this->SetPlayer(nullptr);
				return true;
			}

			if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::CastleMachine ||
				 pPlayer->GetInterfaceState()->GetTarget() != me() )
			{
				this->SetPlayer(nullptr);
				return true;
			}

			return true;
		}

		bool OnTalk(Player* pPlayer)
		{
			if ( !pPlayer || !pPlayer->IsPlaying() )
				return true;

			if ( !sGameServer->IsCastleSiegeEnabled() || !sGameServer->IsCastleSiegeMachineEnabled() )
				return true;

			CASTLE_SIEGE_MACHINE_INTERFACE pMsg(0, this->GetType(), me()->GetEntry());

			if ( this->GetPlayer() )
			{
				pPlayer->SEND_PCT(pMsg);
				return true;
			}

			pMsg.result = 1;
			pPlayer->SEND_PCT(pMsg);

			this->SetPlayer(pPlayer->ToPlayer());
			pPlayer->GetInterfaceState()->Set(InterfaceData::CastleMachine, me());
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

		void DamageUpdate()
		{
			if ( !this->IsActive() )
				return;

			if ( this->IsReady() )
			{
				for ( CastleMachineTargetDataList::iterator it = this->m_target_data_list.begin(); it != this->m_target_data_list.end(); )
				{
					CastleMachineTargetData * pCastleMachineTargetData = *it;

					if ( !pCastleMachineTargetData )
					{
						it = this->m_target_data_list.erase(it);
						continue;
					}

					if ( !pCastleMachineTargetData->GetTime()->Elapsed() )
					{
						++it;
						continue;
					}

					Unit* pTarget = pCastleMachineTargetData->GetTarget();

					if ( pTarget && pTarget->IsPlaying() && pTarget->IsLive() && me()->SameDimension(pTarget) )
					{
						this->Attack(pTarget);
					}

					delete *it;
					it = this->m_target_data_list.erase(it);
				}
			}

			if ( this->GetTime()->Elapsed() )
			{
				LIST_CLEAR(CastleMachineTargetDataList::iterator, this->m_target_data_list);
				this->SetActive(false);
				this->SetReady(false);
			}
		}

		void Attack(Unit* pTarget)
		{
			me()->AttackProc(pTarget, nullptr, false, pTarget->PowerGetTotal(POWER_LIFE) * 15 / 100);
		}
	};
};

class CastleSiegeLordMix: public MonsterScriptAI
{
public:
	explicit CastleSiegeLordMix(): ScriptAI("ai_castle_siege_lord_mix") { }
	virtual ~CastleSiegeLordMix() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster)
		{
		}
		virtual ~AI() {}

		bool Update()
		{
			if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
			{
				me()->SetInstance(0);
			}
			else
			{
				me()->SetInstance(-1);
			}
			
			return false;
		}
	};
};
		
void AddSC_CastleSiege()
{
	sScriptAI->AddScriptAI(new LifeStoneScript());
	sScriptAI->AddScriptAI(new MercenaryScript());
	sScriptAI->AddScriptAI(new GuardianStatueScript());
	sScriptAI->AddScriptAI(new CannonTowerScript());
	sScriptAI->AddScriptAI(new CrownScript());
	sScriptAI->AddScriptAI(new CrownSwitchScript1());
	sScriptAI->AddScriptAI(new CrownSwitchScript2());
	sScriptAI->AddScriptAI(new CastleGateScript());
	sScriptAI->AddScriptAI(new CastleGateLeverScript());
	sScriptAI->AddScriptAI(new CastleMachineScript());
	sScriptAI->AddScriptAI(new CastleSiegeLordMix());
}