/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_personal_merchant.cpp"
*
*/

#define PERSONAL_MERCHANT_TIMEOUT		(120 * IN_MILLISECONDS)

class PersonalMerchantScript: public MonsterScriptAI
{
public:
	explicit PersonalMerchantScript(): ScriptAI("personal_merchant_ai") { }
	virtual ~PersonalMerchantScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new PersonalMerchantAI(monster); }

	struct PersonalMerchantAI: public SummonPlayerAI
	{
		explicit PersonalMerchantAI(Monster* monster): SummonPlayerAI(monster) { }
		virtual ~PersonalMerchantAI() {}

		bool Update()
		{
			me()->SetNextActionTime(1000);

			if ( !me()->GetSummoner() || !me()->GetSummoner()->IsPlayer() || !me()->GetSummoner()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			if ((MyGetTickCount() - me()->GetSummonTick()) > PERSONAL_MERCHANT_TIMEOUT)
			{
				me()->GetSummoner()->ToPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Your Personal Merchant has dissapeared.");
				me()->Remove();
				return true;
			}
			
			if ( this->UpdateLocation() )
				return true;

			return false;
		}

		bool SearchEnemy()
		{
			return true;
		}

		bool ManageThreat() const { return false; }

		bool OnTalk(Player* pPlayer)
		{
			if ( !pPlayer )
				return false;

			if ( pPlayer != me()->GetSummoner() )
			{
				if ( !me()->GetSummoner()->IsPlayer() )
					return true;

				if ( me()->GetSummoner()->ToPlayer()->GetPartyID() == uint16(-1) )
				{
					me()->SayTo(pPlayer, "You are not allowed to buy here.");
					return true;
				}

				if ( pPlayer->GetPartyID() != me()->GetSummoner()->ToPlayer()->GetPartyID() )
				{
					me()->SayTo(pPlayer, "You are not allowed to buy here.");
					return true;
				}
			}

			return false;
		}
	};
};


class PersonalWarehouseScript: public MonsterScriptAI
{
public:
	explicit PersonalWarehouseScript(): ScriptAI("personal_warehouse_ai") { }
	virtual ~PersonalWarehouseScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new PersonalWarehouseAI(monster); }

	struct PersonalWarehouseAI: public SummonPlayerAI
	{
		explicit PersonalWarehouseAI(Monster* monster): SummonPlayerAI(monster) { }
		virtual ~PersonalWarehouseAI() {}

		bool Update()
		{
			me()->SetNextActionTime(1000);

			if ( !me()->GetSummoner() || !me()->GetSummoner()->IsPlayer() || !me()->GetSummoner()->IsPlaying() )
			{
				me()->Remove();
				return true;
			}

			if ((MyGetTickCount() - me()->GetSummonTick()) > PERSONAL_MERCHANT_TIMEOUT)
			{
				me()->GetSummoner()->ToPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Your Personal Warehouse has dissapeared.");
				me()->Remove();
				return true;
			}
			
			if ( this->UpdateLocation() )
				return true;

			return false;
		}

		bool SearchEnemy()
		{
			return true;
		}

		bool ManageThreat() const { return false; }

		bool OnTalk(Player* pPlayer)
		{
			if ( !pPlayer )
				return false;

			if ( pPlayer != me()->GetSummoner() )
			{
				if ( !me()->GetSummoner()->IsPlayer() )
					return true;

				if ( me()->GetSummoner()->ToPlayer()->GetPartyID() == uint16(-1) )
				{
					me()->SayTo(pPlayer, "You are not allowed to buy here.");
					return true;
				}

				if ( pPlayer->GetPartyID() != me()->GetSummoner()->ToPlayer()->GetPartyID() )
				{
					me()->SayTo(pPlayer, "You are not allowed to buy here.");
					return true;
				}
			}

			return false;
		}
	};
};

void AddSC_PersonalNPC()
{
	sScriptAI->AddScriptAI(new PersonalMerchantScript());
	sScriptAI->AddScriptAI(new PersonalWarehouseScript());
}