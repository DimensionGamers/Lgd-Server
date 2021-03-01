class ArkaWarObeliskScript: public MonsterScriptAI
{
public:
	explicit ArkaWarObeliskScript(): ScriptAI("arka_war_obelisk_ai") { }
	virtual ~ArkaWarObeliskScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_ENUM(uint8, Zone);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			this->SetZone(0);
		}
		virtual ~AI() { }

		bool Update()
		{
			if ( sArkaWar->GetState() != ARKA_WAR_STATE_STANDBY && 
				 sArkaWar->GetState() != ARKA_WAR_STATE_PLAYING &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_RESULT &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_CLOSE &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_END )
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

		/*void OnRespawn()
		{
			BloodCastle * pGround = sBloodCastleMgr->GetGround(me()->GetEventGround());

			if ( !pGround )
				return;

			me()->PowerSet(POWER_LIFE, pGround->GetSettings()->GetDoorLife());
			me()->PowerSetMax(POWER_LIFE, pGround->GetSettings()->GetDoorLife());
			me()->SetScriptMaxPower(POWER_LIFE, pGround->GetSettings()->GetDoorLife());
		}*/

		void OnDie()
		{
			Player* pPlayer = me()->GetMaxAttacker();

			if ( !pPlayer )
			{
				return;
			}

			ArkaWarPlayer * pArkaWarPlayer = sArkaWar->GetPlayerInfo(pPlayer->GetGUID());

			if ( !pArkaWarPlayer )
			{
				return;
			}

			Guild* pGuild = sGuildMgr->GetGuild(pPlayer->GetGuildID());

			if ( !pGuild )
			{
				return;
			}

			pArkaWarPlayer->IncreaseKillPoints(ARKA_WAR_KILL_OBELISK_SCORE);

			sLog->outInfo("arkawar", "Obelisk Occupy Add KillPoints [%u] to [%s]",
				pArkaWarPlayer->GetKillPoints(), pPlayer->BuildLog().c_str());

			ObeliskState * pObeliskState = sArkaWar->GetObeliskState(this->GetZone());

			if ( pObeliskState )
			{
				pObeliskState->SetState(1);
				pObeliskState->SetOccupyGuild(pPlayer->GetGuildID());

				sArkaWar->SendObeliskConquered(this->GetZone(), pPlayer->GetGuildID());
			}
		}

		void OnCreate()
		{
			this->SetZone(me()->m_AdditionalDataInt[0]);
		}

		bool StopRespawn()
		{
			if ( sArkaWar->GetState() != ARKA_WAR_STATE_STANDBY && 
				 sArkaWar->GetState() != ARKA_WAR_STATE_PLAYING &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_RESULT &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_CLOSE &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_END )
			{
				me()->Remove();
				return true;
			}

			ObeliskState const* pObeliskState = sArkaWar->GetObeliskState(this->GetZone());
			ArkaWarObeliskBasicData const* pObeliskBasicData = sArkaWar->GetObeliskBasicData(this->GetZone());

			if ( pObeliskState && pObeliskBasicData )
			{
				if ( pObeliskState->GetState() == 1 )
				{
					me()->SetScriptMaxPower(POWER_LIFE, pObeliskBasicData->GetOccupyLife());
				}
				else
				{
					me()->SetScriptMaxPower(POWER_LIFE, pObeliskBasicData->GetLife());
				}
			}

			return false;
		}

		bool EnableAttack(Unit* pAttacker, Skill*, int32 count)
		{
			if ( this->GetZone() >= MAX_ARKA_WAR_ZONES )
			{
				return true;
			}

			if ( sArkaWar->GetState() != ARKA_WAR_STATE_PLAYING )
			{
				return false;
			}

			Player* pPlayer = pAttacker ? pAttacker->ToPlayer() : nullptr;

			if ( !pPlayer )
			{
				return false;
			}

			if ( !pPlayer->GetGuildID() )
			{
				return false;
			}

			ObeliskState const* pObeliskState = sArkaWar->GetObeliskState(this->GetZone());

			if ( pObeliskState )
			{
				if ( pPlayer->GetGuildID() == pObeliskState->GetOccupyGuild() )
				{
					return false;
				}

				if ( pObeliskState->GetAuraState() == 2 && pObeliskState->GetAuraReleaseGuild() == pPlayer->GetGuildID() )
				{
					return true;
				}

				return false;
			}

			return true;
		}
	};
};

class ArkaWarMonsterScript: public MonsterScriptAI
{
public:
	explicit ArkaWarMonsterScript(): ScriptAI("arka_war_monster_ai") { }
	virtual ~ArkaWarMonsterScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_BOOL(BootyDrop);
		DECLARE_ENUM(uint8, Zone);

		explicit AI(Monster* monster): MonsterAI(monster) 
		{
			this->SetBootyDrop(false);
			this->SetZone(0);
		}
		virtual ~AI() { }

		bool Update()
		{
			if ( sArkaWar->GetState() != ARKA_WAR_STATE_STANDBY &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_PLAYING )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnDie()
		{
			Player* pPlayer = me()->GetMaxAttacker();

			if ( !pPlayer )
			{
				return;
			}

			ArkaWarPlayer * pArkaWarPlayer = sArkaWar->GetPlayerInfo(pPlayer->GetGUID());

			if ( !pArkaWarPlayer )
			{
				return;
			}

			if ( me()->GetClass() == 592 ||
				 me()->GetClass() == 593 ||
				 me()->GetClass() == 594 ||
				 me()->GetClass() == 595 ||
				 me()->GetClass() == 596 )
			{
				pArkaWarPlayer->IncreaseContributePoints(ARKA_WAR_REWARD_GENS_BOSS);

				sLog->outInfo("arkawar", "Boss Kill Add ContributePoints [%u] to [%s]",
					pArkaWarPlayer->GetContributePoints(), pPlayer->BuildLog().c_str());
			}

			if ( !this->IsBootyDrop() )
			{
				this->SetBootyDrop(true);

				sItemMgr->ItemSerialCreate(pPlayer, pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), Item(ARKA_WAR_KILL_TROPHY, 0, 1.0f), true, true, true);

				sLog->outInfo("arkawar", "Monster Dropped ArcaWar Item Booty To [%s]",
					pPlayer->BuildLog().c_str());
			}

			pArkaWarPlayer->IncreaseKillPoints(ARKA_WAR_KILL_MONSTER_SCORE);

			sLog->outInfo("arkawar", "Monster Kill Add KillPoints [%u] to [%s]",
				pArkaWarPlayer->GetKillPoints(), pPlayer->BuildLog().c_str());
		}

		bool StopRespawn()
		{
			if ( sArkaWar->GetState() != ARKA_WAR_STATE_STANDBY &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_PLAYING )
			{
				me()->Remove();
				return true;
			}

			ArkaWarMonsterRespawnArea const* pRespawnArea = sArkaWar->GetRespawnArea(me()->GetEventGround());

			if ( pRespawnArea )
			{
				me()->SetBasicLocation(pRespawnArea->GetX1(), pRespawnArea->GetY1(), pRespawnArea->GetX2(), pRespawnArea->GetY2());
			}

			return false;
		}

		void OnCreate()
		{
			this->SetZone(me()->m_AdditionalDataInt[0]);
		}
	};
};

void AddSC_ArkaWar()
{
	sScriptAI->AddScriptAI(new ArkaWarObeliskScript());
	sScriptAI->AddScriptAI(new ArkaWarMonsterScript());
}