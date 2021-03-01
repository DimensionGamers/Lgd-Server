/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_devil_square.cpp"
*
*/

class TormentedSquareScript: public MonsterScriptAI
{
public:
	explicit TormentedSquareScript(): ScriptAI("tormented_square_ai") { }
	virtual ~TormentedSquareScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster) { }
		virtual ~AI() {}

		bool Update()
		{
			if ( me()->GetEventId() != EVENT_TORMENTED_SQUARE )
			{
				me()->Remove();
				return true;
			}

			TormentedSquareGround * pGround = sTormentedSquare->GetGround(me()->GetEventGround());

			if ( !pGround )
			{
				me()->Remove();
				return true;
			}

			if ( (pGround->GetStage() - 1) != me()->GetEventStage() )
			{
				me()->Remove();
				return true;
			}

			if ( pGround->GetState() != TORMENTED_SQUARE_GROUND_STATE_PLAYING )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnDie()
		{
			if ( me()->GetEventId() != EVENT_TORMENTED_SQUARE )
			{
				return;
			}

			TormentedSquareGround * pGround = sTormentedSquare->GetGround(me()->GetEventGround());

			if ( !pGround )
			{
				return;
			}

			Player* pKiller = me()->GetMaxAttacker();

			if ( !pKiller )
			{
				return;
			}

			for ( int32 h = 0; h < pGround->GetTeamSize(); ++h )
			{
				for ( int32 i = 0; i < pGround->GetTeam(h)->GetPlayerSize(); ++i )
				{
					Player* pPlayer = pGround->GetTeam(h)->GetPlayer(i)->GetPlayer();

					if ( pPlayer == pKiller )
					{
						pGround->PlayerAddScore(pKiller, me()->GetRespawnType() == GAME_OBJECT_RESPAWN_DELETE);
						return;
					}
				}
			}
		}

		bool Drop()
		{
			if (me()->GetClass() == 709 ||
				me()->GetClass() == 710 ||
				me()->GetClass() == 711 ||
				me()->GetClass() == 712)
			{
				Item item(ITEMGET(14, 265 + (me()->GetClass() - 709)));

				sItemMgr->ItemSerialCreate(me()->GetMaxAttacker(), me()->GetWorldId(), me()->GetX(), me()->GetY(), item, false, true, true);
			}

			return true;
		}
	};
};

class TormentedSquareSurvivalScript: public MonsterScriptAI
{
public:
	explicit TormentedSquareSurvivalScript(): ScriptAI("tormented_square_survival_ai") { }
	virtual ~TormentedSquareSurvivalScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_ENUM(int32, Score);

		explicit AI(Monster* monster): MonsterAI(monster) 
		{
			this->SetScore(0);
		}

		virtual ~AI() {}

		bool Update()
		{
			if ( me()->GetEventId() != EVENT_TORMENTED_SQUARE_SURVIVAL )
			{
				me()->Remove();
				return true;
			}

			TormentedSquareSurvivalGround * pGround = sTormentedSquareSurvival->GetGround();

			if ( !pGround )
			{
				me()->Remove();
				return true;
			}

			if ( pGround->GetStage() != me()->GetEventStage() )
			{
				me()->Remove();
				return true;
			}

			if ( pGround->GetState() != TORMENTED_SQUARE_GROUND_STATE_PLAYING )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnDie()
		{
			if ( me()->GetEventId() != EVENT_TORMENTED_SQUARE_SURVIVAL )
			{
				return;
			}

			Player* pPlayer = me()->GetMaxAttacker();

			sTormentedSquareSurvival->AddScore(pPlayer, this->GetScore());
			sTormentedSquareSurvival->DropReward(pPlayer, me());
		}

		bool Drop()
		{
			return true;
		}

		void OnCreate()
		{
			this->SetScore(me()->m_AdditionalDataInt[0]);
		}
	};
};

void AddSC_TormentedSquare()
{
	sScriptAI->AddScriptAI(new TormentedSquareScript());
	sScriptAI->AddScriptAI(new TormentedSquareSurvivalScript());
}