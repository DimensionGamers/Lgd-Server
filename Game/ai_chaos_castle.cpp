class ChaosCastleScript: public MonsterScriptAI
{
public:
	explicit ChaosCastleScript(): ScriptAI("chaos_castle_ai") { }
	virtual ~ChaosCastleScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new ChaosCastleAI(monster); }

	struct ChaosCastleAI: public MonsterAI
	{
		explicit ChaosCastleAI(Monster* monster): MonsterAI(monster) { }
		virtual ~ChaosCastleAI() {}

		bool Update()
		{
			ChaosCastle * Ground = sChaosCastleMgr->GetGround(me()->GetEventGround());

			if ( !Ground )
			{
				me()->Remove();
				return true;
			}

			if ( Ground->GetState() == CHAOS_CASTLE_STATE_NONE || Ground->GetState() == CHAOS_CASTLE_STATE_PLAYEND )
			{
				me()->Remove();
				return true;
			}

			this->UpdatePosition();
			return false;
		}

		void OnDie()
		{
			ChaosCastle * Ground = sChaosCastleMgr->GetGround(me()->GetEventGround());

			if ( Ground )
			{
				Player* pPlayer = me()->GetMaxAttacker();

				if (roll_chance_i(25))
				{
					Ground->BlowObjects(me()->GetX(), me()->GetY());
				}

				Ground->KillMonster(pPlayer);

				if (pPlayer)
				{
					pPlayer->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_CHAOS_CASTLE_MONSTER_KILL);
				}
			}
		}

		bool Drop()
		{
			ChaosCastle * Ground = sChaosCastleMgr->GetGround(me()->GetEventGround());

			if ( Ground )
			{
				Ground->IncreaseMonsterItemDropCounter(1);

				if ( Ground->GetMonsterItemDropCounter() >= Ground->GetMonsterItemDrop() )
				{
					Ground->SetMonsterItemDropCounter(0);

					for ( ChaosCastleItemDataList::iterator it = Ground->item_list.begin(); it != Ground->item_list.end(); ++it )
					{
						if ( it->count != 0 )
						{
							continue;
						}

						it->count = 1;
						Item item(it->item, it->level, it->durability);

						sItemMgr->ItemSerialCreate(me()->GetMaxAttacker(), me()->GetWorldId(), me()->GetX(), me()->GetY(), item, false, true, true);
						break;
					}
				}
			}

			return true;
		}

		void UpdatePosition()
		{
			WorldGrid const& grid = me()->GetGrid();

			if ( !grid.IsLocked_2() )
				return;

			coord_type x = me()->GetX();
			coord_type y = me()->GetY();

			this->GetRandomSafeZone(x, y);
			me()->PositionSend(x, y);
			me()->ViewportCreate(VIEWPORT_CREATE_FLAG_ME);
		}

		void GetRandomSafeZone(coord_type &x, coord_type &y)
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
				return;

			int32 count = 100;
			coord_type x_1 = g_ChaosCastle_GroundAxis[0];
			coord_type y_1 = g_ChaosCastle_GroundAxis[1];
			coord_type x_2 = g_ChaosCastle_GroundAxis[2];
			coord_type y_2 = g_ChaosCastle_GroundAxis[3];

			coord_type tmp_x;
			coord_type tmp_y;

			while ( count-- > 0 )
			{
				tmp_x = x_1 + RANDOM(x_2 - x_1);
				tmp_y = y_1 + RANDOM(y_2 - y_1);

				if ( x_1 < g_ChaosCastle_GroundAxis[0] || x_1 > g_ChaosCastle_GroundAxis[2] ||
					 y_1 < g_ChaosCastle_GroundAxis[1] || y_1 > g_ChaosCastle_GroundAxis[3] )
					continue;

				WorldGrid const& attr = pWorld->GetGrid(tmp_x, tmp_y);

				if ( !attr.IsLocked_2() )
				{
					x = tmp_x;
					y = tmp_y;
					break;
				}
			}
		}

		bool SendHPBar()
		{
			return false;
		}
	};
};

class ChaosCastleSurvivalScript: public MonsterScriptAI
{
public:
	explicit ChaosCastleSurvivalScript(): ScriptAI("chaos_castle_survival_ai") { }
	virtual ~ChaosCastleSurvivalScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new ChaosCastleAI(monster); }

	struct ChaosCastleAI: public MonsterAI
	{
		explicit ChaosCastleAI(Monster* monster): MonsterAI(monster) { }
		virtual ~ChaosCastleAI() {}

		bool Update()
		{
			ChaosCastle * Ground = sChaosCastleSurvivalMgr->GetGround();

			if ( !Ground )
			{
				me()->Remove();
				return true;
			}

			if ( Ground->GetState() == CHAOS_CASTLE_STATE_NONE || Ground->GetState() == CHAOS_CASTLE_STATE_PLAYEND )
			{
				me()->Remove();
				return true;
			}

			this->UpdatePosition();
			return false;
		}

		void OnDie()
		{
			ChaosCastle * Ground = sChaosCastleSurvivalMgr->GetGround();

			if ( Ground )
			{
				Player* pPlayer = me()->GetMaxAttacker();

				if (roll_chance_i(25))
				{
					Ground->BlowObjects(me()->GetX(), me()->GetY());
				}

				Ground->KillMonster(pPlayer);
			}
		}

		bool Drop()
		{
			ChaosCastle * Ground = sChaosCastleSurvivalMgr->GetGround();

			if ( Ground )
			{
				Ground->IncreaseMonsterItemDropCounter(1);

				if ( Ground->GetMonsterItemDropCounter() >= Ground->GetMonsterItemDrop() )
				{
					Ground->SetMonsterItemDropCounter(0);

					for ( ChaosCastleItemDataList::iterator it = Ground->item_list.begin(); it != Ground->item_list.end(); ++it )
					{
						if ( it->count != 0 )
						{
							continue;
						}

						it->count = 1;
						Item item(it->item, it->level, it->durability);

						sItemMgr->ItemSerialCreate(me()->GetMaxAttacker(), me()->GetWorldId(), me()->GetX(), me()->GetY(), item, false, true, true);
						break;
					}
				}
			}

			return true;
		}

		void UpdatePosition()
		{
			WorldGrid const& grid = me()->GetGrid();

			if ( !grid.IsLocked_2() )
				return;

			coord_type x = me()->GetX();
			coord_type y = me()->GetY();

			this->GetRandomSafeZone(x, y);
			me()->PositionSend(x, y);
			me()->ViewportCreate(VIEWPORT_CREATE_FLAG_ME);
		}

		void GetRandomSafeZone(coord_type &x, coord_type &y)
		{
			World* pWorld = me()->GetWorld();

			if ( !pWorld )
				return;

			int32 count = 100;
			coord_type x_1 = g_ChaosCastle_GroundAxis[0];
			coord_type y_1 = g_ChaosCastle_GroundAxis[1];
			coord_type x_2 = g_ChaosCastle_GroundAxis[2];
			coord_type y_2 = g_ChaosCastle_GroundAxis[3];

			coord_type tmp_x;
			coord_type tmp_y;

			while ( count-- > 0 )
			{
				tmp_x = x_1 + RANDOM(x_2 - x_1);
				tmp_y = y_1 + RANDOM(y_2 - y_1);

				if ( x_1 < g_ChaosCastle_GroundAxis[0] || x_1 > g_ChaosCastle_GroundAxis[2] ||
					 y_1 < g_ChaosCastle_GroundAxis[1] || y_1 > g_ChaosCastle_GroundAxis[3] )
					continue;

				WorldGrid const& attr = pWorld->GetGrid(tmp_x, tmp_y);

				if ( !attr.IsLocked_2() )
				{
					x = tmp_x;
					y = tmp_y;
					break;
				}
			}
		}

		bool SendHPBar()
		{
			return false;
		}
	};
};

void AddSC_ChaosCastle()
{
	sScriptAI->AddScriptAI(new ChaosCastleScript());
	sScriptAI->AddScriptAI(new ChaosCastleSurvivalScript());
}
