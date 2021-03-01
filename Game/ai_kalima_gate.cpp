/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_kalima_gate.cpp"
*
*/

static const int32 g_KalimaGate[KALIMA_LEVEL] =
{
	88, 89, 90, 91, 92, 93, 116
};

#define KALIMA_GATE_TIMEOUT			180000

struct KalimaGateBasicAI: public MonsterAI
{
	DECLARE_ENUM(uint8, ID);

	explicit KalimaGateBasicAI(Monster* monster, uint8 id): MonsterAI(monster)
	{
		this->SetID(id);
	}
	virtual ~KalimaGateBasicAI() {}

	bool Update()
	{
		if ( !me()->IsLive() )
			return true;

		if ( !me()->IsSummoned() )
		{
			sLog->outInfo(LOG_MONSTER, "Monster::KalimaGateAction() Wrong Summoner. Deleting Kalima Gate %d",
				me()->GetEntry());

			me()->Remove();
			return true;
		}

		Player* pSummoner = me()->GetSummoner()->ToPlayer();

		if ( !pSummoner )
		{
			sLog->outInfo(LOG_MONSTER, "Monster::KalimaGateAction() Summoner is not player. Deleting Kalima Gate %d",
				me()->GetEntry());

			me()->Remove();
			return true;
		}

		if ( !pSummoner->Object::IsPlaying() )
		{
			sLog->outInfo(LOG_MONSTER, "Monster::KalimaGateAction() Summoner is offline. Deleting Kalima Gate %d",
				me()->GetEntry());

			me()->Remove();
			return true;
		}

		if ( !pSummoner->IsLive() )
		{
			sLog->outInfo(LOG_MONSTER, "Monster::KalimaGateAction() Summoner is dead. Deleting Kalima Gate %d",
				me()->GetEntry());

			pSummoner->SendNotice(NOTICE_NORMAL_BLUE, "Your Kalima gate has dissapeared.");
			me()->Remove();
			return true;
		}

		if ( (MyGetTickCount() - me()->GetSummonTick()) > KALIMA_GATE_TIMEOUT )
		{
			sLog->outInfo(LOG_MONSTER, "Monster::KalimaGateAction() Summon Time Passed. Deleting Kalima Gate %d",
				me()->GetEntry());

			pSummoner->SendNotice(NOTICE_NORMAL_BLUE, "Your Kalima gate has dissapeared.");
			me()->Remove();
			return true;
		}

		Object * pObject = nullptr;

		VIEWPORT_LOOP_OBJECT(me(), pObject)

			if ( !pObject->IsPlayer() || !pObject->IsLive() || !pObject->IsPlaying() )
				continue;

			if ( !IN_RANGE(me(), pObject, 2) )
				continue;

			if ( !me()->SameDimension(pObject) )
				continue;

			if ( pSummoner != pObject->ToPlayer() )
			{
				if ( pSummoner->GetPartyID() == PARTY_NULL )
					continue;

				if ( pSummoner->GetPartyID() != pObject->ToPlayer()->GetPartyID() )
					continue;
			}

			uint8 entrance_level = pObject->ToPlayer()->GetKalimaLevelEntrance();

			if ( entrance_level == uint8(-1) || entrance_level < this->GetID() )
				continue;

			pObject->ToPlayer()->MoveToGate(g_KalimaGate[this->GetID()]);

		VIEWPORT_CLOSE

		return true;
	}
};

class KalimaGateScript1: public MonsterScriptAI
{
public:
	explicit KalimaGateScript1(): ScriptAI("kalima_gate_ai_1") { }
	virtual ~KalimaGateScript1() {}

	MonsterAI* GetAI(Monster* monster) const { return new KalimaGateAI(monster); }

	struct KalimaGateAI: public KalimaGateBasicAI
	{
		explicit KalimaGateAI(Monster* monster): KalimaGateBasicAI(monster, 0) { }
		virtual ~KalimaGateAI() {}
	};
};

class KalimaGateScript2: public MonsterScriptAI
{
public:
	explicit KalimaGateScript2(): ScriptAI("kalima_gate_ai_2") { }
	virtual ~KalimaGateScript2() {}

	MonsterAI* GetAI(Monster* monster) const { return new KalimaGateAI(monster); }

	struct KalimaGateAI: public KalimaGateBasicAI
	{
		explicit KalimaGateAI(Monster* monster): KalimaGateBasicAI(monster, 1) { }
		virtual ~KalimaGateAI() {}
	};
};

class KalimaGateScript3: public MonsterScriptAI
{
public:
	explicit KalimaGateScript3(): ScriptAI("kalima_gate_ai_3") { }
	virtual ~KalimaGateScript3() {}

	MonsterAI* GetAI(Monster* monster) const { return new KalimaGateAI(monster); }

	struct KalimaGateAI: public KalimaGateBasicAI
	{
		explicit KalimaGateAI(Monster* monster): KalimaGateBasicAI(monster, 2) { }
		virtual ~KalimaGateAI() {}
	};
};

class KalimaGateScript4: public MonsterScriptAI
{
public:
	explicit KalimaGateScript4(): ScriptAI("kalima_gate_ai_4") { }
	virtual ~KalimaGateScript4() {}

	MonsterAI* GetAI(Monster* monster) const { return new KalimaGateAI(monster); }

	struct KalimaGateAI: public KalimaGateBasicAI
	{
		explicit KalimaGateAI(Monster* monster): KalimaGateBasicAI(monster, 3) { }
		virtual ~KalimaGateAI() {}
	};
};

class KalimaGateScript5: public MonsterScriptAI
{
public:
	explicit KalimaGateScript5(): ScriptAI("kalima_gate_ai_5") { }
	virtual ~KalimaGateScript5() {}

	MonsterAI* GetAI(Monster* monster) const { return new KalimaGateAI(monster); }

	struct KalimaGateAI: public KalimaGateBasicAI
	{
		explicit KalimaGateAI(Monster* monster): KalimaGateBasicAI(monster, 4) { }
		virtual ~KalimaGateAI() {}
	};
};

class KalimaGateScript6: public MonsterScriptAI
{
public:
	explicit KalimaGateScript6(): ScriptAI("kalima_gate_ai_6") { }
	virtual ~KalimaGateScript6() {}

	MonsterAI* GetAI(Monster* monster) const { return new KalimaGateAI(monster); }

	struct KalimaGateAI: public KalimaGateBasicAI
	{
		explicit KalimaGateAI(Monster* monster): KalimaGateBasicAI(monster, 5) { }
		virtual ~KalimaGateAI() {}
	};
};

class KalimaGateScript7: public MonsterScriptAI
{
public:
	explicit KalimaGateScript7(): ScriptAI("kalima_gate_ai_7") { }
	virtual ~KalimaGateScript7() {}

	MonsterAI* GetAI(Monster* monster) const { return new KalimaGateAI(monster); }

	struct KalimaGateAI: public KalimaGateBasicAI
	{
		explicit KalimaGateAI(Monster* monster): KalimaGateBasicAI(monster, 6) { }
		virtual ~KalimaGateAI() {}
	};
};

class IllusionKundunScript: public MonsterScriptAI
{
public:
	explicit IllusionKundunScript(): ScriptAI("ai_kundun_illusion") { }
	virtual ~IllusionKundunScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new KundunIllusionAI(monster); }

	struct KundunIllusionAI: public MonsterAI
	{
		explicit KundunIllusionAI(Monster* monster): MonsterAI(monster) {}
		virtual ~KundunIllusionAI() {}

		void OnDie()
		{
			if ( me()->GetWorldId() != WORLD_KALIMA_1 &&
				 me()->GetWorldId() != WORLD_KALIMA_2 && 
				 me()->GetWorldId() != WORLD_KALIMA_3 && 
				 me()->GetWorldId() != WORLD_KALIMA_4 &&
				 me()->GetWorldId() != WORLD_KALIMA_5 &&
				 me()->GetWorldId() != WORLD_KALIMA_6 )
				return;

			Player* pPlayer = me()->GetMaxAttacker();

			if ( !pPlayer )
				return;

			if ( !pPlayer->Object::IsPlaying() )
				return;

			if ( sObjectMgr->ExistKalimaGate(pPlayer) )
			{
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You already possess a Kalima gate.");
				return;
			}

			coord_type x = me()->GetX();
			coord_type y = me()->GetY();

			WorldGrid const& attr = me()->GetGrid();

			if ( attr.attribute != 0 )
			{
				if ( !pPlayer->GetKalimaGateRandomLocation(x, y) )
				{
					pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Couldn't find correct zone to summon Kalima gate.");
					return;
				}
			}

			int32 monster_class = 129 + me()->GetWorldId();

			Monster * mMonster = sObjectMgr->MonsterTryAdd(monster_class, me()->GetWorldId());

			if ( !mMonster )
			{
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Couldn't summon Kalima gate.");
				return;
			}

			mMonster->SetLevel((me()->GetWorldId() - WORLD_KALIMA_1) + 2);
			mMonster->SetWorldId(me()->GetWorldId());
			mMonster->SetBasicLocation(x, y, x, y);
			mMonster->SetInstance(me()->GetInstance());
			mMonster->SetDirection(1);
			mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			mMonster->SetSummoner(pPlayer);
			mMonster->SetSummonTick(MyGetTickCount());
			mMonster->AddToWorld();
			
			pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Kalima Gate created.");
		}
	};
};

void AddSC_KalimaGate()
{
	sScriptAI->AddScriptAI(new KalimaGateScript1());
	sScriptAI->AddScriptAI(new KalimaGateScript2());
	sScriptAI->AddScriptAI(new KalimaGateScript3());
	sScriptAI->AddScriptAI(new KalimaGateScript4());
	sScriptAI->AddScriptAI(new KalimaGateScript5());
	sScriptAI->AddScriptAI(new KalimaGateScript6());
	sScriptAI->AddScriptAI(new KalimaGateScript7());
	sScriptAI->AddScriptAI(new IllusionKundunScript());
}