class CrywolfScript: public MonsterScriptAI
{
public:
	explicit CrywolfScript(): ScriptAI(CRYWOLF_AI) { }
	virtual ~CrywolfScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new CrywolfAI(monster); }

	struct CrywolfAI: public CrywolfMonsterBasicAI
	{
		DECLARE_ENUM(uint8, State);

		explicit CrywolfAI(Monster* monster): CrywolfMonsterBasicAI(monster) { this->SetState(sCrywolf->GetState()); }
		virtual ~CrywolfAI() {}

		void OnCreate()
		{
			this->SetScore(static_cast<int32>(me()->m_AdditionalDataInt[0]));
		}

		bool Update()
		{
			if ( sCrywolf->GetState() == CRYWOLF_STATE_END_CYCLE )
			{
				me()->Remove();
				return true;
			}

			if ( sCrywolf->GetState() != this->GetState() )
			{
				if ( sCrywolf->GetState() == CRYWOLF_STATE_START && me()->IsDarkElf() )
				{
					Object* pObject = nullptr;

					VIEWPORT_LOOP_OBJECT(me(), pObject)

						if ( !pObject->IsPlayer() )
							continue;

						if ( pObject->ToPlayer()->IsAdministrator() )
							continue;

						me()->PushBackCount(pObject->ToUnit(), 3);
					VIEWPORT_CLOSE
				}

				this->SetState(sCrywolf->GetState());
			}

			if ( sCrywolf->GetState() == CRYWOLF_STATE_READY || sCrywolf->GetState() == CRYWOLF_STATE_END )
			{
				return true;
			}

			return false;
		}

		bool EnableAttack(Unit*, Skill*, int32 count)
		{
			return sCrywolf->GetState() == CRYWOLF_STATE_START;
		}

		void OnDie()
		{
			if ( sCrywolf->GetState() != CRYWOLF_STATE_START )
				return;

			Player* pPlayer = me()->GetMaxAttacker();

			if ( pPlayer )
			{
				if ( me()->IsBalgass() )
				{
					sCrywolf->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "%s has been killed by %s",
						me()->GetName(), pPlayer->GetName());
				}
				else if ( me()->IsDarkElf() )
				{
					sCrywolf->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "%s number %d has been killed by %s",
						me()->GetName(), me()->GetGroupAI(), pPlayer->GetName());
				}

				if ( this->GetScore() )
				{
					pPlayer->IncreaseCrywolfScore(this->GetScore());
					pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Current MvP score is : %d", pPlayer->GetCrywolfScore());
				}
			}

			if ( me()->IsBalgass() )
			{
				sCrywolf->SetBoss(2);
				sCrywolf->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "Balgass has been defeated with the help of heroes");
				sCrywolf->SetOccupationState(CRYWOLF_OCCUPATION_SUCCESS);
				sCrywolf->SetState_End();
			}
		}
	};
};

class CrywolfStatueScript: public MonsterScriptAI
{
public:
	explicit CrywolfStatueScript(): ScriptAI(CRYWOLF_STATUE_AI) { }
	virtual ~CrywolfStatueScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new CrywolfStatueAI(monster); }

	struct CrywolfStatueAI: public CrywolfStatueBasicAI
	{
		DECLARE(uint32, shield_max);
		DECLARE(uint32, shield);
		DECLARE(uint8, state);
		DECLARE(uint8, count);

		explicit CrywolfStatueAI(Monster* monster): CrywolfStatueBasicAI(monster)
		{
			this->shield_max.set(0);
			this->shield.set(0);
			this->state.set(0);
			this->count.set(0);
		}
		virtual ~CrywolfStatueAI() {}

		bool Update()
		{
			uint8 max_contracts = sCrywolf->altarGetCount();
			uint32 current_hp = sCrywolf->altarGetHP();
			uint32 max_hp = sCrywolf->altarGetMaxHP();

			if ( max_contracts == 0 || this->shield == 0 )
			{
				if ( this->state == 1 )
				{
					sObjectMgr->SendNoticeToMap(WORLD_CRYWOLF_FIRST_ZONE, NOTICE_GLOBAL, "The barrier of the holy wolf statue has dissapeared.");
					this->state.set(0);
					this->shield.set(0);
					this->shield_max.set(0);
					this->count.set(max_contracts);
					me()->AddBuff(cast(uint8, CrywolfStatueBuff[max_contracts]), 0, BUFF_FLAG_CONSTANT, me());
				}
				else if ( this->count != max_contracts )
				{
					this->state.set(1);
					this->shield.set(current_hp);
					this->shield_max.set(max_hp);
					this->count.set(max_contracts);
					sObjectMgr->SendNoticeToMap(WORLD_CRYWOLF_FIRST_ZONE, NOTICE_GLOBAL, "Stats of Muses in the holy wolf statue's barrier is %d, HP: %d", max_contracts, current_hp);
					me()->AddBuff(cast(uint8, CrywolfStatueBuff[max_contracts]), 0, BUFF_FLAG_CONSTANT, me());
				}

				return true;
			}
			else if ( this->state == 0 )
			{
				sObjectMgr->SendNoticeToMap(WORLD_CRYWOLF_FIRST_ZONE, NOTICE_GLOBAL, "The barrier of the holy wolf statue has been created.");
				me()->AddBuff(cast(uint8, CrywolfStatueBuff[max_contracts]), 0, BUFF_FLAG_CONSTANT, me());
			}
			else if ( this->count != max_contracts )
			{
				sObjectMgr->SendNoticeToMap(WORLD_CRYWOLF_FIRST_ZONE, NOTICE_GLOBAL, "Stats of Muses in the holy wolf statue's barrier is %d, HP: %d", max_contracts, current_hp);
				me()->AddBuff(cast(uint8, CrywolfStatueBuff[max_contracts]), 0, BUFF_FLAG_CONSTANT, me());
			}

			this->state.set(1);
			this->shield.set(current_hp);
			this->shield_max.set(max_hp);
			this->count.set(max_contracts);
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

		uint8 GetPercent()
		{
			if ( this->shield_max.get() )
				return (this->shield.get() * 100) / this->shield_max.get();

			return 0;
		}

		bool SendHPBar()
		{
			return false;
		}
	};
};

bool CrywolfAltarBasicAI::Update()
{
	if ( !this->pPlayer )
		return true;

	if ( !this->pPlayer->IsPlaying() || !this->pPlayer->IsLive() )
	{
		this->Reset();
		return true;
	}

	if ( !me()->SameDimension(pPlayer) )
	{
		this->Reset();
		return true;
	}

	if ( !IN_RANGE(this->pPlayer, me(), 0) )
	{
		this->Reset();
		return true;
	}

	if ( this->GetState() == CRYWOLF_ALTAR_STATE_CONTRACT_ATTEMPT )
	{
		if ( MyGetTickCount() > this->contract_valid_tick.get() )
		{
			this->pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Connected to the pedestal [%d] of the Holy wolf.", 
				this->GetId() + 1);

			sCrywolf->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "[%s] connected the Muse to the pedestal [%d]", 
				this->pPlayer->GetName(), this->GetId() + 1);

			sLog->outInfo(LOG_CRYWOLF, "[ Altar %d ] [%s][%s] Set Valid Contract",
				this->GetId(), this->pPlayer->GetAccountData()->GetAccount(), this->pPlayer->GetName());

			this->setContracted();
		}
	}

	return true;
}

void CrywolfAltarBasicAI::Reset()
{
	if ( this->pPlayer && this->pPlayer->IsPlaying() )
	{
		this->pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Connecting to the pedestal [%d] is canceled.",
			this->GetId() + 1);

		sLog->outInfo(LOG_CRYWOLF, "[ Altar %d ] [%s][%s] Remove contract",
			this->GetId(), this->pPlayer->GetAccountData()->GetAccount(), this->pPlayer->GetName());
	}

	if ( this->contract_count.get() >= sGameServer->crywolf_altar_max_contract.get() )
	{
		this->setDisabled();
	}
	else
	{
		this->setEnabled();
	}

	this->pPlayer = nullptr;
	this->contract_applied_tick.set(0);
	this->contract_valid_tick.set(0);
	this->contract_last_tick.set(MyGetTickCount());
}

int32 CrywolfAltarBasicAI::GetContractLeft() const
{
	int32 remain = sGameServer->crywolf_altar_max_contract.get() - this->contract_count.get();
	return (remain < 0) ? 0: remain;
}

void CrywolfAltarBasicAI::setEnabled()
{
	this->state.set(CRYWOLF_ALTAR_STATE_ENABLED);
	me()->ClearAllBuff(BUFF_CLEAR_FLAG_ALL);
	me()->AddBuff(BUFF_ALTAR_WOLF_CONTRACT_ENABLED, 0, BUFF_FLAG_CONSTANT);
}
	
void CrywolfAltarBasicAI::setContracted()
{
	this->state.set(CRYWOLF_ALTAR_STATE_CONTRACTED);
	me()->ClearAllBuff(BUFF_CLEAR_FLAG_ALL);
	me()->AddBuff(BUFF_ALTAR_WOLF_CONTRACT_STATUS, 0, BUFF_FLAG_CONSTANT);
}
	
void CrywolfAltarBasicAI::setContractAttempt()
{
	this->state.set(CRYWOLF_ALTAR_STATE_CONTRACT_ATTEMPT);
	me()->ClearAllBuff(BUFF_CLEAR_FLAG_ALL);
	me()->AddBuff(BUFF_ALTAR_WOLF_CONTRACT_ATTEMPT, 0, BUFF_FLAG_CONSTANT);
}
	
void CrywolfAltarBasicAI::setDisabled()
{
	this->state.set(CRYWOLF_ALTAR_STATE_DISABLED);
	me()->ClearAllBuff(BUFF_CLEAR_FLAG_ALL);
	me()->AddBuff(BUFF_ALTAR_WOLF_CONTRACT_DISABLED, 0, BUFF_FLAG_CONSTANT);
}

void CrywolfAltarBasicAI::Prepare()
{
	this->pPlayer = nullptr;
	this->contract_count.set(0);
	this->contract_applied_tick.set(0);
	this->contract_valid_tick.set(0);
	this->contract_last_tick.set(MyGetTickCount());
	this->setEnabled();
}

bool CrywolfAltarBasicAI::SetPlayer(Player* pPlayer)
{
	if ( this->GetState() != CRYWOLF_ALTAR_STATE_ENABLED )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "The pedestal [%d] of Holy wolf has been connected.", 
			this->GetId() + 1);

		return false;
	}

	if ( this->pPlayer )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "The pedestal [%d] of Holy wolf has been connected.", 
			this->GetId() + 1);

		return false;
	}

	if (MyGetTickCount() < (this->contract_last_tick.get() + sGameServer->crywolf_altar_contract_last_time.get()))
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "The connecting time of pedestal [%d] has not started yet.", 
			this->GetId() + 1);

		return false;
	}

	if ( this->contract_count.get() > sGameServer->crywolf_altar_max_contract.get() )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "The time for connecting to pedestal [%d] of the holy wolf is over.", 
			this->GetId() + 1);

		return false;
	}

	if ( !IN_RANGE(pPlayer, me(), 0) )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "The position to connect to pedestal [%d] of the holy wolf is incorrect. Try again.", 
			this->GetId() + 1);

		return false;
	}

	this->setContractAttempt();
	
	this->pPlayer = pPlayer;
	this->contract_applied_tick.set(MyGetTickCount());
	this->contract_valid_tick.set(MyGetTickCount() + sGameServer->crywolf_altar_contract_valid_time.get());
	this->contract_last_tick.set(MyGetTickCount());
	this->contract_count.op_inc();

	sCrywolf->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "%s is trying to contract altar %d!", 
		pPlayer->GetName(), this->GetId() + 1);

	sLog->outInfo(LOG_CRYWOLF, "[ Altar %d ] [%s][%s] Attempt to contract",
		this->GetId(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName());

	pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You can start connecting in %d second(s)", 
		sGameServer->crywolf_altar_contract_valid_time.get() / IN_MILLISECONDS);

	pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Do not move until the connecting is completed.");

	return true;
}

class CrywolfAltarScript1: public MonsterScriptAI
{
public:
	explicit CrywolfAltarScript1(): ScriptAI(CRYWOLF_ALTAR_1_AI) { }
	virtual ~CrywolfAltarScript1() {}

	MonsterAI* GetAI(Monster* monster) const { return new CrywolfAltarAI(monster); }

	struct CrywolfAltarAI: public CrywolfAltarBasicAI
	{
		explicit CrywolfAltarAI(Monster* monster): CrywolfAltarBasicAI(monster, 0) { }
		virtual ~CrywolfAltarAI() {}
	};
};

class CrywolfAltarScript2: public MonsterScriptAI
{
public:
	explicit CrywolfAltarScript2(): ScriptAI(CRYWOLF_ALTAR_2_AI) { }
	virtual ~CrywolfAltarScript2() {}

	MonsterAI* GetAI(Monster* monster) const { return new CrywolfAltarAI(monster); }

	struct CrywolfAltarAI: public CrywolfAltarBasicAI
	{
		explicit CrywolfAltarAI(Monster* monster): CrywolfAltarBasicAI(monster, 1) { }
		virtual ~CrywolfAltarAI() {}
	};
};

class CrywolfAltarScript3: public MonsterScriptAI
{
public:
	explicit CrywolfAltarScript3(): ScriptAI(CRYWOLF_ALTAR_3_AI) { }
	virtual ~CrywolfAltarScript3() {}

	MonsterAI* GetAI(Monster* monster) const { return new CrywolfAltarAI(monster); }

	struct CrywolfAltarAI: public CrywolfAltarBasicAI
	{
		explicit CrywolfAltarAI(Monster* monster): CrywolfAltarBasicAI(monster, 2) { }
		virtual ~CrywolfAltarAI() {}
	};
};

class CrywolfAltarScript4: public MonsterScriptAI
{
public:
	explicit CrywolfAltarScript4(): ScriptAI(CRYWOLF_ALTAR_4_AI) { }
	virtual ~CrywolfAltarScript4() {}

	MonsterAI* GetAI(Monster* monster) const { return new CrywolfAltarAI(monster); }

	struct CrywolfAltarAI: public CrywolfAltarBasicAI
	{
		explicit CrywolfAltarAI(Monster* monster): CrywolfAltarBasicAI(monster, 3) { }
		virtual ~CrywolfAltarAI() {}
	};
};

class CrywolfAltarScript5: public MonsterScriptAI
{
public:
	explicit CrywolfAltarScript5(): ScriptAI(CRYWOLF_ALTAR_5_AI) { }
	virtual ~CrywolfAltarScript5() {}

	MonsterAI* GetAI(Monster* monster) const { return new CrywolfAltarAI(monster); }

	struct CrywolfAltarAI: public CrywolfAltarBasicAI
	{
		explicit CrywolfAltarAI(Monster* monster): CrywolfAltarBasicAI(monster, 4) { }
		virtual ~CrywolfAltarAI() {}
	};
};

void AddSC_Crywolf()
{
	sScriptAI->AddScriptAI(new CrywolfScript());
	sScriptAI->AddScriptAI(new CrywolfStatueScript());
	sScriptAI->AddScriptAI(new CrywolfAltarScript1());
	sScriptAI->AddScriptAI(new CrywolfAltarScript2());
	sScriptAI->AddScriptAI(new CrywolfAltarScript3());
	sScriptAI->AddScriptAI(new CrywolfAltarScript4());
	sScriptAI->AddScriptAI(new CrywolfAltarScript5());
}