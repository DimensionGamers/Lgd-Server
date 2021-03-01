class CastleDeepScript: public MonsterScriptAI
{
public:
	explicit CastleDeepScript(): ScriptAI("castle_deep_ai") { }
	virtual ~CastleDeepScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_ENUM(int32, Type);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
		}

		virtual ~AI() {}

		bool Update()
		{
			if ( sCastleDeep->GetState() != CASTLE_DEEP_STATE_PLAYING )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnCreate()
		{
			this->SetType(me()->m_AdditionalDataInt[0]);
		}

		void OnDie()
		{
			Player* pPlayer = me()->GetMaxAttacker();

			if ( pPlayer )
			{
				if ( this->GetType() == 1 )
				{
					sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "%s has pushed back the attack of %s at Loren pass", pPlayer->GetName(), me()->GetName());
				}
				else if ( this->GetType() == 2 )
				{
					sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "%s has pushed back the attack of Erohim's army at Loren pass", pPlayer->GetName());
				}
			}
			else
			{
				if ( this->GetType() == 1 )
				{
					sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "The attack of %s has been pushed back at Loren pass", me()->GetName());
				}
				else if ( this->GetType() == 2 )
				{
					sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "The attack of Erohim's army has been pushed back at Loren pass");
				}
			}
		}

		bool Drop()
		{
			Player* pPlayer = me()->GetMaxAttacker();

			if ( !pPlayer )
			{
				return false;
			}

			if ( this->GetType() == 1 )
			{
				sItemMgr->ItemSerialCreate(pPlayer, me()->GetWorldId(), me()->GetX(), me()->GetY(), Item(JEWEL::BLESS, 0, 1), false, true, true);
				return true;
			}

			return false;
		}
	};
};

void AddSC_CastleDeep()
{
	sScriptAI->AddScriptAI(new CastleDeepScript());
}