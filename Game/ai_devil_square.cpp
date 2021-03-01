/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_devil_square.cpp"
*
*/

class DevilSquareScript: public MonsterScriptAI
{
public:
	explicit DevilSquareScript(): ScriptAI("devil_square_ai") { }
	virtual ~DevilSquareScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new DevilSquareAI(monster); }

	struct DevilSquareAI: public MonsterAI
	{
		explicit DevilSquareAI(Monster* monster): MonsterAI(monster) { }
		virtual ~DevilSquareAI() {}

		void OnDie()
		{
			DevilSquare * Ground = sDevilSquareMgr->GetGround(me()->GetEventGround());

			if ( !Ground || me()->GetEventId() != EVENT_DEVIL_SQUARE )
			{
				return;
			}

			DamageDataList const& damage_data = me()->GetThreatManager()->GetDamageData();

			if ( damage_data.empty() )
			{
				return;
			}

			Player * pPlayer = nullptr;
			DamageData const* data = nullptr;

			for ( DamageDataList::const_iterator it = damage_data.begin(); it != damage_data.end(); ++it )
			{
				data = *it;

				if ( !data )
				{
					continue;
				}

				if ( !(pPlayer = sObjectMgr->FindPlayerByGUID(data->GetGuid())) )
					continue;

				if ( pPlayer->GetEventId() != EVENT_DEVIL_SQUARE || pPlayer->GetEventGround() != me()->GetEventGround() )
					continue;

				int32 add_score = ((data->GetTotalDamage() * 100 / me()->PowerGetTotal(POWER_LIFE)) * me()->GetLevel()) * (me()->GetEventGround() + 1);

				DevilSquarePlayer * pPlayerData = Ground->FindPlayer(pPlayer);

				if ( pPlayerData )
				{
					pPlayerData->IncreaseScore(add_score);
				}
			}
		}
	};
};

void AddSC_DevilSquare()
{
	sScriptAI->AddScriptAI(new DevilSquareScript());
}