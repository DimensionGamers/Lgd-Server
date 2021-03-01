class EvomonPortalScript: public MonsterScriptAI
{
public:
	explicit EvomonPortalScript(): ScriptAI("evomon_portal_ai") { }
	virtual ~EvomonPortalScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_ENUM(uint32, PlayerGUID);
		DECLARE_PTR(Player, Player);
		DECLARE_PTR(Monster, Evomon);
		DECLARE_ENUM(uint8, EvomonID);
		DECLARE_ENUM(uint16, EvomonMonster);

		DECLARE_BOOL(Finished);
		DECLARE_STRUCT(TickTimer, EvomonTime);
		DECLARE_BOOL(EvomonTimeFinished);

		DECLARE_PROPERTY(int32, Score);
		DECLARE_BOOL(EvomonSpecial);

		DECLARE_STRUCT(TickTimer, GlobalTime);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			this->SetPlayerGUID(0);
			this->SetPlayer(nullptr);
			this->SetEvomon(nullptr);
			this->SetEvomonID(0);
			this->SetEvomonMonster(-1);
			this->SetFinished(false);
			this->GetEvomonTime()->Reset();
			this->SetEvomonTimeFinished(false);
			this->SetScore(0);
			this->SetEvomonSpecial(false);
		}

		virtual ~AI() { }

		bool Update()
		{
			me()->SetNextActionTime(1000);

			if (!this->GetPlayer())
			{
				this->RemoveEvomon();
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetConnectStatus() != CONNECT_STATUS_PLAYING)
			{
				this->SetPlayer(nullptr);
				return true;
			}

			if (this->GetPlayer()->GetGUID() != this->GetPlayerGUID())
			{
				this->SetPlayer(nullptr);
				return true;
			}

			if (this->IsFinished())
			{
				if (this->GetPlayer())
				{
					this->GetPlayer()->SetEvomonPortal(nullptr);
					this->GetPlayer()->SetEvomonTime(0);
				}

				this->RemoveEvomon();
				me()->Remove();

				return true;
			}

			if (MyGetTickCount() > this->GetPlayer()->GetEvomonTime())
			{
				this->SetFinished(true);
				return true;
			}

			this->CreateEvomon();

			return true;
		}

		void OnCreate()
		{
			this->SetPlayerGUID(me()->m_AdditionalDataInt[0]);
			this->SetPlayer((Player*)me()->m_AdditionalDataPtr[0]);
			this->SetEvomonMonster(me()->m_AdditionalDataInt[1]);
		}

		void CreateEvomon()
		{
			if (this->IsFinished())
			{
				return;
			}

			if (this->GetEvomon())
			{
				if (!this->GetEvomon()->IsPlaying())
				{
					if (this->IsEvomonSpecial())
					{
						sEvomon->GiveSpecialReward(this->GetPlayer());
					}

					this->SetEvomon(nullptr);
					this->SetEvomonID(sEvomon->GetNextEvomon(this->GetEvomonID(), this->GetEvomonMonster()));
					this->GetEvomonTime()->Reset();
					this->SetEvomonTimeFinished(false);
					me()->ViewportCreate(VIEWPORT_CREATE_FLAG_ME);
				}

				return;
			}

			if (!this->IsEvomonTimeFinished())
			{
				if (!this->GetEvomonTime()->Elapsed(3000))
				{
					return;
				}

				this->SetEvomonTimeFinished(true);
			}

			EvomonInfo const* pEvomonInfo = sEvomon->GetEvomon(this->GetEvomonID(), this->GetEvomonMonster());

			if (!pEvomonInfo)
			{
				this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Awakening Failed. Awakening has failed due to low chance.");
				sEvomon->GiveReward(this->GetPlayer(), this->GetScore());

				me()->AddBuff(BUFF_EVOMON_AWAKENING_FAILURE, 5, 0, me());
				this->SetFinished(true);
				return;
			}

			me()->SetLevel(pEvomonInfo->GetLevel());
			this->SetScore(pEvomonInfo->GetLevel());

			this->SetEvomonSpecial(roll_chance_i(pEvomonInfo->GetSpecialRate(), 10000));

			uint16 monster_id = this->IsEvomonSpecial() ? pEvomonInfo->GetSpecialMonster(): pEvomonInfo->GetMonster();

			Monster* pMonster = sObjectMgr->MonsterTryAdd(monster_id, me()->GetWorldId());

			if ( pMonster )
			{
				pMonster->SetLevel(pEvomonInfo->GetLevel());
				pMonster->PowerSet(POWER_LIFE, pEvomonInfo->GetLife());
				pMonster->PowerSetMax(POWER_LIFE, pEvomonInfo->GetLife());
				pMonster->SetScriptMaxPower(POWER_LIFE, pEvomonInfo->GetLife());
				pMonster->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, pEvomonInfo->GetAttackDamageMin());
				pMonster->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, pEvomonInfo->GetAttackDamageMax());
				pMonster->SetIntData(UNIT_INT_ATTACK_RATE, pEvomonInfo->GetAttackSuccessRate());
				pMonster->SetIntData(UNIT_INT_DEFENSE, pEvomonInfo->GetDefense());
				pMonster->SetIntData(UNIT_INT_DEFENSE_RATE, pEvomonInfo->GetDefenseSuccessRate());
				pMonster->SetSummonType(2);
				pMonster->SetWorldId(me()->GetWorldId());
				pMonster->SetBasicLocation(me()->GetX(), me()->GetY(), me()->GetX(), me()->GetY());
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
				pMonster->SetSummonPlayer(this->GetPlayer());
				pMonster->SetInstance(me()->GetInstance());
				pMonster->SetScriptName("evomon_ai");
				pMonster->AddToWorld();

				this->SetEvomon(pMonster);

				me()->AddBuff(BUFF_EVOMON_AWAKENING_SUCCESS, 3, 0, me());

				this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Awakening Succeeded. After a moment awakened monster appears.");
			}
		}

		void RemoveEvomon()
		{
			if (this->GetEvomon())
			{
				this->GetEvomon()->Remove();
				this->SetEvomon(nullptr);
			}
		}
	};
};

class EvomonScript: public MonsterScriptAI
{
public:
	explicit EvomonScript(): ScriptAI("evomon_ai") { }
	virtual ~EvomonScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster) { }

		virtual ~AI() { }

		bool Drop()
		{
			return true;
		}
	};
};

void AddSC_Evomon()
{
	sScriptAI->AddScriptAI(new EvomonPortalScript());
	sScriptAI->AddScriptAI(new EvomonScript());
}