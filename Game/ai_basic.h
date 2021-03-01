#ifndef AI_BASIC_H
#define AI_BASIC_H

struct LifeStoneBasicAI: public MonsterAI
{
	DECLARE_PROPERTY(int32, CreationTime);
	DECLARE_PROPERTY(uint8, CreationState);

	explicit LifeStoneBasicAI(Monster* monster): MonsterAI(monster)
	{
		this->SetCreationTime(0);
		this->SetCreationState(0);
	}
	virtual ~LifeStoneBasicAI() {}

	bool IsCreated() const { return this->GetCreationState() >= 5; }
};

struct GuardianStatueBasicAI: public MonsterAI
{
	explicit GuardianStatueBasicAI(Monster* monster): MonsterAI(monster)
	{
		this->SetID(0);
	}
	virtual ~GuardianStatueBasicAI() {}

	DECLARE_ENUM(uint8, ID);
};

struct CrownBasicAI: public MonsterAI
{
	DECLARE(uint8, state);
	DECLARE_PTR(Player, Player);
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
	DECLARE_BOOL(Available);

	explicit CrownBasicAI(Monster* monster): MonsterAI(monster)
	{
		this->SetPlayer(nullptr);
		this->state.set(uint8(-1));
		this->SetX(0);
		this->SetY(0);
		this->SetAvailable(false);
	}

	virtual ~CrownBasicAI() {}

	virtual void ResetPlayer(){ }

	void SetState(uint8 state)
	{
		this->state.set(state);
		this->StateSend();
	}

	virtual void StateSend() { }
};

struct CrownSwitchBasicAI: public MonsterAI
{
	DECLARE_PTR(Player, Player);
	DECLARE_ENUM(uint8, ID);

	explicit CrownSwitchBasicAI(Monster* monster, uint8 id): MonsterAI(monster)
	{
		this->SetPlayer(nullptr);
		this->SetID(id);
	}

	virtual ~CrownSwitchBasicAI() {}

	void ResetPlayer() { this->SetPlayer(nullptr); }

	bool PushBackAllowed() { return false; }
	bool Drop() { return true; }
	bool OnTalk(Player* pPlayer);
	bool Update();
};

struct CastleGateBasicAI: public MonsterAI
{
	DECLARE_ENUM(uint8, State);
	DECLARE_ENUM(uint8, ID);

	explicit CastleGateBasicAI(Monster* monster): MonsterAI(monster)
	{
		this->SetState(0);
		this->SetID(0);
	}

	virtual ~CastleGateBasicAI() {}

	void SendState(uint8 operation)
	{
		CASTLE_SIEGE_GATE_OPEN_STATE pMsg(operation, me()->GetEntry());
		me()->sendPacket_viewport(MAKE_PCT(pMsg));
	}
};

struct GateLeverBasicAI: public MonsterAI
{
	DECLARE_ENUM(uint8, ID);

	explicit GateLeverBasicAI(Monster* monster): MonsterAI(monster)
	{
		this->SetID(0);
	}

	virtual ~GateLeverBasicAI() {}
	
	virtual void Operate(Player* pPlayer, uint8 operation, uint16 /*gate*/) { }

	void OnCreate()
	{
		this->SetID(static_cast<uint8>(me()->m_AdditionalDataInt[0]));
	}
};

struct CastleMachineBasicAI: public MonsterAI
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_PTR(Player, Player);
	DECLARE_BOOL(Active);
	DECLARE_BOOL(Ready);
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
	DECLARE_STRUCT(TickTimer, Time);
	
	struct CastleMachineTargetData
	{
		DECLARE_PTR(Unit, Target);
		DECLARE_STRUCT(TickTimer, Time);
	};

	typedef std::vector<CastleMachineTargetData*> CastleMachineTargetDataList;
	CastleMachineTargetDataList m_target_data_list;
		
	explicit CastleMachineBasicAI(Monster* monster): MonsterAI(monster)
	{
		this->GetTime()->Reset();
		this->SetType(0);
		this->SetPlayer(nullptr);
		this->SetActive(false);
		this->SetReady(false);
		this->SetX(0);
		this->SetY(0);
	}

	virtual ~CastleMachineBasicAI()
	{
		LIST_CLEAR(CastleMachineTargetDataList::iterator, this->m_target_data_list);
	}

	void Start(coord_type x, coord_type y)
	{
		this->SetX(x);
		this->SetY(y);
		this->GetTime()->Start(CASTLE_SIEGE_MACHINE_ATTACK_TIME);
		this->SetActive(true);
	}

	void AddTarget(Unit* pTarget)
	{
		CastleMachineTargetData * pCastleMachineTargetData = new CastleMachineTargetData;
		pCastleMachineTargetData->SetTarget(pTarget);
		pCastleMachineTargetData->GetTime()->Reset();
		pCastleMachineTargetData->GetTime()->Start(CASTLE_SIEGE_MACHINE_TARGET_TIME);

		this->m_target_data_list.push_back(pCastleMachineTargetData);
	}

	void Ready()
	{
		this->SetReady(true);
	}

	virtual void DamageUpdate() { }

	void OnCreate()
	{
		this->SetType(static_cast<CastleSiegeMachineType>(me()->m_AdditionalDataInt[0]));
	}
};

struct InvasionBasicAI: public MonsterAI
{
	DECLARE_ENUM(uint32, InvasionID);
	DECLARE_ENUM(uint32, InvasionGroup);
	DECLARE_ENUM(uint32, InvasionSubGroup);
	DECLARE_ENUM(uint8, EventAnimation);

	explicit InvasionBasicAI(Monster* monster): MonsterAI(monster)
	{ 
		this->SetInvasionID(-1); 
		this->SetInvasionGroup(-1); 
		this->SetInvasionSubGroup(-1); 
		this->SetEventAnimation(-1);
	}

	virtual ~InvasionBasicAI() {}
};

struct KanturuBasicAI: public MonsterAI
{
	DECLARE(uint8, m_state);
	DECLARE(uint8, m_sub_state);

	explicit KanturuBasicAI(Monster* monster): MonsterAI(monster)
	{
		this->m_state.set(0);
		this->m_sub_state.set(0);
	}

	virtual ~KanturuBasicAI() {}

	void SetState(uint8 value) { this->m_state.set(value); }
	void SetSubState(uint8 value) { this->m_sub_state.set(value); }

	void OnCreate()
	{
		this->SetState(static_cast<uint8>(me()->m_AdditionalDataInt[0]));
		this->SetSubState(static_cast<uint8>(me()->m_AdditionalDataInt[1]));
	}

	bool IsInSamePhase() const;
};

struct CrywolfStatueBasicAI: public MonsterAI
{
	explicit CrywolfStatueBasicAI(Monster* monster): MonsterAI(monster) { }
	virtual ~CrywolfStatueBasicAI() {}

	virtual uint8 GetPercent() { return 0; }
};

struct CrywolfAltarBasicAI: public MonsterAI
{
	DECLARE(int32, contract_count);
	DECLARE(TCType, contract_last_tick);
	DECLARE(TCType, contract_valid_tick);
	DECLARE(TCType, contract_applied_tick);
	DECLARE(uint8, state);
	DECLARE(uint8, id);

	Player* pPlayer;

	explicit CrywolfAltarBasicAI(Monster* monster, uint8 id): MonsterAI(monster), pPlayer(nullptr)
	{
		this->contract_count.set(0);
		this->contract_last_tick.set(0);
		this->contract_valid_tick.set(0);
		this->contract_applied_tick.set(0);
		this->state.set(0);
		this->id.set(id);
	}

	virtual ~CrywolfAltarBasicAI() {}

	bool Update();
	void Reset();
	bool EnableAttack(Unit*, Skill*, int32 count) { return false; }
	bool PushBackAllowed() { return false; }
	int32 GetContractLeft() const;

	uint8 GetState() const { return this->state.get(); }
	Player* GetPlayer() const { return this->pPlayer; }
	uint8 GetId() const { return this->id.get(); }

	void setEnabled();
	void setContracted();
	void setContractAttempt();
	void setDisabled();
	bool SetPlayer(Player* pPlayer);
	void Prepare();
};

struct CrywolfMonsterBasicAI: public MonsterAI
{
	explicit CrywolfMonsterBasicAI(Monster* monster): MonsterAI(monster) { this->SetScore(0); }
	virtual ~CrywolfMonsterBasicAI() {}

	DECLARE_ENUM(int32, Score);
};

struct DoppelgangerBasicAI: public MonsterAI
{
	explicit DoppelgangerBasicAI(Monster* monster);
	virtual ~DoppelgangerBasicAI() {}

	DoppelgangerZone* GetZone();
	void OnRespawn();
	void FindPath();
};

#endif