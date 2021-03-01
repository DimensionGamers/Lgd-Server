/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ChaosCastle.cpp"
*
*/

ChaosCastle::ChaosCastle(): EventGroundCommon("ChaosCastle", LOG_CHAOS_CASTLE, CC_MAP_RANGE, EVENT_CHAOS_CASTLE)
{
	
}

ChaosCastle::~ChaosCastle()
{
	
}

void ChaosCastle::Update()
{
	switch( this->GetState() )
	{
	case CHAOS_CASTLE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case CHAOS_CASTLE_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case CHAOS_CASTLE_STATE_STANDBY:
		{
			this->ProcState_StandBy();
		} break;

	case CHAOS_CASTLE_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case CHAOS_CASTLE_STATE_PLAYEND:
		{
			this->ProcState_Playend();
		} break;
	}
}

void ChaosCastle::SetState_None()
{
	this->SetState(CHAOS_CASTLE_STATE_NONE);
	this->MovePlayersOut(this->GetRespawnGate());
	this->InitMessageSent();
	this->SetDuration(0);
	this->RunTime(0);
	//this->SetTrapStatus(0);
	this->SetTrapTriggered(false);
	this->SetMonsterCount(0);
	this->SetMonsterItemDrop(0);
	this->SetMonsterItemDropCounter(0);
	this->RestoreCastleZone();
	this->item_list.clear();

	LOG_EVENT_STATE();
}

void ChaosCastle::SetState_Open()
{
	this->InitPlayers<ChaosCastlePlayer>(this->GetSettings()->GetMaxPlayers());

	this->SetState(CHAOS_CASTLE_STATE_OPEN);
	this->RunTime(0);

	LOG_EVENT_STATE();
}

void ChaosCastle::SetState_StandBy()
{
	this->SetState(CHAOS_CASTLE_STATE_STANDBY);
	this->RunTime(CHAOS_CASTLE_STANDBY_TIME * MINUTE * IN_MILLISECONDS);

	LOG_EVENT_STATE();
}

void ChaosCastle::SetState_Playing()
{
	this->item_list.clear();

	if (this->GetEvent() == EVENT_CHAOS_CASTLE_SURVIVAL)
	{
		for (int32 h = 0; h < g_ChaosCastleSurvivalItemData[sChaosCastleSurvivalMgr->GetCurrentGround()].count; ++h)
		{
			ChaosCastleItemData add_data;
			add_data.item = g_ChaosCastleSurvivalItemData[sChaosCastleSurvivalMgr->GetCurrentGround()].item;
			add_data.level = g_ChaosCastleSurvivalItemData[sChaosCastleSurvivalMgr->GetCurrentGround()].level;
			add_data.durability = g_ChaosCastleSurvivalItemData[sChaosCastleSurvivalMgr->GetCurrentGround()].durability;
			add_data.count = 0;

			this->item_list.push_back(add_data);
		}
	}
	else
	{
		for (int32 i = 0; i < 2; ++i)
		{
			for (int32 h = 0; h < g_ChaosCastleItemData[this->GetGround()][i].count; ++h)
			{
				ChaosCastleItemData add_data;
				add_data.item = g_ChaosCastleItemData[this->GetGround()][i].item;
				add_data.level = g_ChaosCastleItemData[this->GetGround()][i].level;
				add_data.durability = g_ChaosCastleItemData[this->GetGround()][i].durability;
				add_data.count = 0;

				this->item_list.push_back(add_data);
			}
		}
	}

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( !pPlayer->IsPlaying() )
		{
			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( !CC_MAP_RANGE(pPlayer->GetWorldId()) || pPlayer->GetEventGround() != this->GetGround() || pPlayer->GetEventId() != this->GetEvent() )
		{
			this->GetPlayer(i)->Reset();
			continue;
		}
	}

	if ( this->GetPlayersCount(false) < this->GetSettings()->GetMinPlayers() )
	{
		this->RefundMoney();
		this->SetState_None();
		return;
	}

	this->SetState(CHAOS_CASTLE_STATE_PLAYING);
	this->RunTime(this->GetDuration() * MINUTE * IN_MILLISECONDS);
	this->SendStart();
	this->UnsafetyCastleZone();
	this->SendSafetyCastleZone(false);
	this->AddMonster();
	this->SendState(5);

	LOG_EVENT_STATE();
}

void ChaosCastle::SetState_Playend()
{
	this->SetState(CHAOS_CASTLE_STATE_PLAYEND);
	this->RunTime(CHAOS_CASTLE_END_TIME * MINUTE * IN_MILLISECONDS);
	this->SendState(7);

	LOG_EVENT_STATE();
}

void ChaosCastle::ProcState_None()
{

}

void ChaosCastle::ProcState_Open()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer(false);
}

void ChaosCastle::ProcState_StandBy()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer(false);

	this->SendMessageID(this->GetEvent() == EVENT_CHAOS_CASTLE_SURVIVAL ? EVENT_MESSAGE_START_CHAOS_CASTLE_SURVIVAL : EVENT_MESSAGE_START_CHAOS_CASTLE);

	if ( this->TimePassed() )
		this->SetState_Playing();
}

void ChaosCastle::ProcState_Playing()
{
	if ( this->SecondPassed() )
	{
		this->UpdatePlayerStatus();

		if ( this->GetPlayersCount(false) <= 0 )
		{
			this->SetState_None();
			return;
		}

		if ( this->GetPlayersCount(false) == 1 && this->GetMonsterCount() <= 0 )
		{
			if ( ChaosCastlePlayer* player = this->GetWinner(false) )
			{
				if ( Player* pPlayer = player->GetPlayer() )
				{
					pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "The Spirits of the Guardsmen have been exorcised. Quest completed!");

					pPlayer->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_CHAOS_CASTLE_CLEAR);

					this->RewardExperience(player, true);
				}
			}

			this->SetState_Playend();
			return;
		}

		this->KillPlayersOnDieTile();
		this->UpdateTraps();
		this->SendState(6);
	}

	this->SendMessageID(this->GetEvent() == EVENT_CHAOS_CASTLE_SURVIVAL ? EVENT_MESSAGE_END_CHAOS_CASTLE_SURVIVAL : EVENT_MESSAGE_END_CHAOS_CASTLE);

	if ( this->TimePassed() )
	{
		ChaosCastlePlayer *player = this->GetWinner();

		if ( this->GetMonsterCount() <= 0 && player )
		{
			this->RewardExperience(player, true);
			this->SendFailToAll(player);
		}
		else
		{
			this->SendFailToAll(nullptr);
		}

		this->SetState_Playend();
		return;
	}
}

void ChaosCastle::ProcState_Playend()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer();

	this->SendMessageID(this->GetEvent() == EVENT_CHAOS_CASTLE_SURVIVAL ? EVENT_MESSAGE_CLEAR_CHAOS_CASTLE_SURVIVAL : EVENT_MESSAGE_CLEAR_CHAOS_CASTLE);
	
	if ( this->TimePassed() )
		this->SetState_None();
}

void ChaosCastle::UpdatePlayerStatus()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		if ( pPlayer->GetRegenStatus() != REGEN_NONE )
		{
			continue;
		}

		if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		{
			this->Log("Removed Player %s : Disconnected.", pPlayer->GetName());

			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( !pPlayer->IsLive() )
		{
			this->Log("Removed Player %s : Died.", pPlayer->GetName());

			this->RewardExperience(this->GetPlayer(i)->ToCC(), false);
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( !CC_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			this->Log("Removed Player %s : Not in event.", pPlayer->GetName());

			this->RewardExperience(this->GetPlayer(i)->ToCC(), false);
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
			continue;
		}
	}
}

void ChaosCastle::RefundMoney()
{
	Player* pPlayer = nullptr;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		pPlayer->MoneyAdd(this->GetSettings()->GetRequiredZen());
	}
}

void ChaosCastle::SendStart()
{
	if ( this->GetEvent() == EVENT_CHAOS_CASTLE_SURVIVAL )
	{
		this->SendNotice(NOTICE_GLOBAL, "Chaos Castle Survival of the Fittest has started");
	}
	else
	{
		this->SendNotice(NOTICE_GLOBAL, "Chaos Castle %d has already commenced!", this->GetGround() + 1);
	}
}

void ChaosCastle::SendState(uint8 state)
{
	CHAOS_CASTLE_STATE pMsg(state);

	pMsg.remain_seconds = this->GetRemain() / IN_MILLISECONDS;
	pMsg.monsters_max = MAX_CHAOS_CASTLE_ENEMY;
	pMsg.monsters_remain = this->GetMonsterCount() + this->GetPlayersCount(false);

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void ChaosCastle::KillMonster(Player* pPlayer)
{
	this->KillMonster();

	if ( !pPlayer )
		return;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( this->GetPlayer(i)->GetPlayer() != pPlayer )
			continue;

		this->GetPlayer(i)->ToCC()->IncreaseMonsterKill(1);

		this->Log("%s - Killed Monster [%d]", pPlayer->BuildLog().c_str(), this->GetPlayer(i)->ToCC()->GetMonsterKill());
		return;
	}
}
	
void ChaosCastle::KillPlayer(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( this->GetPlayer(i)->GetPlayer() != pPlayer )
			continue;

		this->GetPlayer(i)->ToCC()->IncreasePlayerKill(1);

		this->Log("%s - Killed Player [%d]", pPlayer->BuildLog().c_str(), this->GetPlayer(i)->ToCC()->GetPlayerKill());
		return;
	}
}

void ChaosCastle::AddMonster()
{
	int32 players_count = this->GetPlayersCount(false);

	this->SetMonsterItemDrop(0);
	this->SetMonsterItemDropCounter(0);

	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != this->GetEvent() )
		{
			continue;
		}

		if ( (*it)->chaos_castle.ground != this->GetGround() )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->SetEventGround(this->GetGround());
			pMonster->AddToWorld();

			this->Log("Added Monster [%u][%u]", pMonster->GetEntry(), pMonster->GetClass());

			this->IncreaseMonsterCount(1);

			++players_count;

			if ( players_count >= MAX_CHAOS_CASTLE_ENEMY )
				break;
		}
	}

	if ( this->item_list.size() > 0 )
	{
		this->SetMonsterItemDrop(this->GetMonsterCount() / this->item_list.size());
	}

	this->Log("Added %d monsters / Total Items %u / Item drop every %d monsters", this->GetMonsterCount(), this->item_list.size(), this->GetMonsterItemDrop());
}

void ChaosCastle::GiveItemToWinner(Player* pPlayer, int32 score)
{
	if (!pPlayer)
	{
		return;
	}

	if (this->GetEvent() == EVENT_CHAOS_CASTLE_SURVIVAL)
	{
		sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(g_ChaosCastleSurvivalFinalItemData[sChaosCastleSurvivalMgr->GetCurrentGround()]), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::CHAOS_CASTLE_SURVIVAL);
		
		if (sChaosCastleSurvivalMgr->GetStage() == CHAOS_CASTLE_SURVIVAL_STAGE_FINAL)
		{
			SQLTransaction trans = MuDatabase.BeginTransaction();

			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_HALL_OF_FAME_INSERT);
			stmt->setUInt32(pPlayer->GetGUID());
			stmt->setString(pPlayer->GetName());
			stmt->setInt32(score);
			trans->Append(stmt);

			MuDatabase.CommitTransaction(trans);
		}
	}
	else
	{
		sItemBagMgr->ExecuteItemBag(ITEM_BAG_MISC, pPlayer, g_ChaosCastleFinalReward[this->GetGround()].itembag);
	}

	this->Log("Giving Winner item to %s", pPlayer->GetName());
}

void ChaosCastle::BlowObjects(coord_type x, coord_type y)
{
	coord_type min_x = x - 3;
	coord_type max_x = x + 3;
	coord_type min_y = y - 3;
	coord_type max_y = y + 3;

	FIX_COORD(min_x);
	FIX_COORD(min_y);
	Player* pPlayer = nullptr;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsLive() || pPlayer->GetWorldId() != this->GetWorldID() )
			continue;

		if ( pPlayer->GetX() < min_x || pPlayer->GetX() > max_x ||
			 pPlayer->GetY() < min_y || pPlayer->GetY() > max_y )
			continue;

		if ( pPlayer->IsTeleporting() )
			continue;

		if ( pPlayer->IsAdministrator() )
			continue;

		this->BlowObjectsFromPoint(pPlayer, x, y);
	}
}

void ChaosCastle::BlowObjectsFromPoint(Unit* mUnit, coord_type x, coord_type y)
{
	World* pWorld = mUnit->GetWorld();

	if ( !pWorld )
		return;

	int32 distance = Util::Distance(mUnit->GetX(), mUnit->GetY(), x, y);

	if ( distance >= 4 )
		return;

	coord_type iSIGN_X = 1;
	coord_type iSIGN_Y = 1;
	coord_type iUX = mUnit->GetX();
	coord_type iUY = mUnit->GetY();

	(iUX > x)?iSIGN_X = 1:(iUX < x)?iSIGN_X = -1:(!(Random(2)))?iSIGN_X = 1:iSIGN_X = -1;
	(iUY > y)?iSIGN_Y = 1:(iUY < y)?iSIGN_Y = -1:(!(Random(2)))?iSIGN_Y = 1:iSIGN_Y = -1;

	for ( int32 iBLOWOUT_COUNT = 0; iBLOWOUT_COUNT < 5; ++iBLOWOUT_COUNT )
	{
		int32 iBLOW_MIN = g_ChaosCastle_BlowOutDistance[distance][0];
		int32 iBLOW_MAX = g_ChaosCastle_BlowOutDistance[distance][1];
		coord_type iBLOW_X = Random(iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;
		coord_type iBLOW_Y = Random(iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;
		int32 iRND = Random(2);

		if ( iRND )
		{
			if ( iBLOW_X >= iBLOW_MAX )
			{
				iBLOW_X = iBLOW_MAX;
				iBLOW_Y = iBLOW_MIN + (Random(2) - 1);

				if ( iBLOW_Y < 0 )
					iBLOW_Y = 0;
			}
		}
		else if ( iBLOW_Y >= iBLOW_MAX )
		{
			iBLOW_Y = iBLOW_MAX;
			iBLOW_X = iBLOW_MIN + (Random(2) - 1);

			if ( iBLOW_X < 0 )
				iBLOW_X = 0;
		}

		coord_type iTX = mUnit->GetX() + iBLOW_X * iSIGN_X;
		coord_type iTY = mUnit->GetY() + iBLOW_Y * iSIGN_Y;

		FIX_COORD(iTX);
		FIX_COORD(iTY);

		WorldGrid const& attr = pWorld->GetGrid(iTX, iTY);

		if ( attr.IsLocked_1() )
			continue;

		mUnit->PositionSend(iTX, iTY);
		
		if ( attr.IsLocked_2() )
		{
			mUnit->Kill();
			return;
		}

		int32 damage_dec = g_ChaosCastle_BlowOutDamage[distance % 4];

		mUnit->Hit(mUnit, damage_dec, 0, 0);
		return;
	}
}

void ChaosCastle::KillPlayersOnDieTile()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsLive() || pPlayer->GetWorldId() != this->GetWorldID() )
			continue;

		WorldGrid const& attr = pPlayer->GetGrid();

		if ( attr.IsLocked_2() )
		{
			pPlayer->Kill();
		}
	}
}

void ChaosCastle::UpdateTraps()
{
	int32 current_live = this->GetPlayersCount(false) + this->GetMonsterCount();
	//uint8 current_trap = this->GetTrapStatus();

	/*if ( current_live > 40 )
	{
		this->SetTrapStatus(0);
	}
	else if ( current_live > 30 )
	{
		this->SetTrapStatus(1);
		this->SendState(8);
	}
	else if ( current_live > 20 )
	{
		this->SetTrapStatus(2);
		this->SendState(9);
	}
	else
	{
		this->SetTrapStatus(3);
		this->SendState(10);
	}

	if ( this->GetTrapStatus() != current_trap )
	{
		this->SetTrapHollow(this->GetTrapStatus());
		this->DeleteItemsOnTrap(this->GetTrapStatus());
	}*/

	if ( current_live <= 40 && !this->IsTrapTriggered(1) )
	{
		this->SetTrapTriggered(1, true);
		this->SendState(8);
		this->SetTrapHollow(1);
		this->DeleteItemsOnTrap(1);
	}

	if ( current_live <= 30 && !this->IsTrapTriggered(2) )
	{
		this->SetTrapTriggered(2, true);
		this->SendState(9);
		this->SetTrapHollow(2);
		this->DeleteItemsOnTrap(2);
	}

	if ( current_live <= 20 && !this->IsTrapTriggered(3) )
	{
		this->SetTrapTriggered(3, true);
		this->SendState(10);
		this->SetTrapHollow(3);
		this->DeleteItemsOnTrap(3);
	}
}

void ChaosCastle::SetTrapHollow(uint8 trap)
{
	if ( trap >= MAX_CHAOS_CASTLE_TRAP_STEP )
	{
		return;
	}

	World * mWorld = sWorldMgr->GetWorld(this->GetWorldID());

	if ( !mWorld )
	{
		return;
	}

	for ( int32 a = 0; a < MAX_CHAOS_CASTLE_HOLLOW_ZONE; a++ )
	{
		for ( coord_type x = g_ChaosCastleMapHollowZone[trap][a].X1; x <= g_ChaosCastleMapHollowZone[trap][a].X2; x++ )
		{
			for ( coord_type y = g_ChaosCastleMapHollowZone[trap][a].Y1; y <= g_ChaosCastleMapHollowZone[trap][a].Y2; y++ )
			{
				mWorld->ApplyAttribute(x, y, WORLD_ATTRIBUTE_LOCK_2, true);
			}
		}
	}
}

void ChaosCastle::DeleteItemsOnTrap(uint8 trap)
{
	if ( trap >= MAX_CHAOS_CASTLE_TRAP_STEP )
	{
		return;
	}

	World * mWorld = sWorldMgr->GetWorld(this->GetWorldID());

	if ( !mWorld )
	{
		return;
	}

	for ( uint16 i = 0; i < MAX_WORLD_ITEM; ++i )
	{
		if ( !mWorld->GetItemData(i)->item.IsItem() )
			continue;

		coord_type x = mWorld->GetItemData(i)->GetLocation()->GetX();
		coord_type y = mWorld->GetItemData(i)->GetLocation()->GetY();

		if ( x < g_ChaosCastle_DamageAxis[trap][0] || x > g_ChaosCastle_DamageAxis[trap][2] ||
			 y < g_ChaosCastle_DamageAxis[trap][1] || y > g_ChaosCastle_DamageAxis[trap][3] )
		{
			mWorld->GetItemData(i)->SetState(WORLD_ITEM_STATE_DELETE);
			mWorld->GetItemData(i)->SetLive(false);
		}
	}
}

void ChaosCastle::RewardExperience(ChaosCastlePlayer const* player, bool winner)
{
	if ( !player )
	{
		return;
	}

	Player* pPlayer = player->GetPlayer();

	if ( !pPlayer )
	{
		return;
	}

	if (this->GetEvent() == EVENT_CHAOS_CASTLE_SURVIVAL)
	{
		int32 score = (player->GetMonsterKill() * 1) + (player->GetPlayerKill() * 2);

		if (winner)
		{
			score += 3;
		}

		sChaosCastleSurvivalMgr->AddScore(pPlayer->GetGUID(), score);

		sEventMgr->AddEventScore(pPlayer->GetGUID(), pPlayer->GetName(), this->GetEvent(), this->GetGround(), score);

		CHAOS_CASTLE_SURVIVAL_SCORE pMsg;
		pMsg.score = score;
		pMsg.result = winner ? 1 : 2;
		pMsg.type = sChaosCastleSurvivalMgr->GetStage();
		pMsg.monster_kill_count = player->GetMonsterKill();
		pMsg.player_kill_count = player->GetPlayerKill();
		pPlayer->SEND_PCT(pMsg);

		if (!winner)
		{
			pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You lost in the Best of the Best Match. Please try again next time.");
		}
		else
		{
			pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "The souls of the Guardsmen have been purified. You won the Best of the Best Match.");
		}

		this->Log("RewardExperience (%s - %s): Killed Monsters: %d / Killed Players: %d",
			pPlayer->GetName(), winner ? "Success" : "Failed", player->GetMonsterKill(), player->GetPlayerKill());

		if (winner)
		{
			this->GiveItemToWinner(pPlayer, score);
		}
	}
	else
	{
		int32 contribution = player->GetMonsterKill() + player->GetPlayerKill();

		int64 total_experience = player->GetMonsterKill() * this->GetSettings()->GetExperienceMonster() + player->GetPlayerKill() * this->GetSettings()->GetExperienceUser();

		if ( winner )
		{
			contribution = MAX_CHAOS_CASTLE_ENEMY;
		}

		if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL && sGameServer->IsCrywolfPenaltyApply() )
		{
			total_experience = total_experience * sGameServer->GetCrywolfPenaltyExperienceGain() / 100;
		}

		pPlayer->GiveExperience(total_experience, true, true);

		CHAOS_CASTLE_REWARD pMsg(winner);
		memcpy(pMsg.data.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		pMsg.data.experience = total_experience;
		pMsg.data.killed_monster = player->GetMonsterKill();
		pMsg.data.killed_user = player->GetPlayerKill();

		pPlayer->UpdateStatistic(STATISTIC_CHAOS_CASTLE_POINTS, player->GetMonsterKill() + (player->GetPlayerKill() * 5));

		sEventMgr->AddEventScore(pPlayer->GetGUID(), pPlayer->GetName(), this->GetEvent(), this->GetGround(), player->GetMonsterKill() + (player->GetPlayerKill() * 5));

		pPlayer->sendPacket(MAKE_PCT(pMsg));

		if ( !winner )
			pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You have failed to complete the Quest. Summon more courage and try again.");

		this->Log("RewardExperience (%s - %s): Killed Monsters: %d / Killed Players: %d / Experience: %I64d",
			pPlayer->GetName(), winner ? "Success" : "Failed", player->GetMonsterKill(), player->GetPlayerKill(), total_experience);

		sEventMgr->GiveContributionReward(pPlayer, EVENT_CHAOS_CASTLE, this->GetGround() + 1, contribution, GremoryCaseReward::CHAOS_CASTLE);

		if ( winner )
		{
			this->GiveItemToWinner(pPlayer, 0);
		}
	}
}

ChaosCastlePlayer* ChaosCastle::GetWinner(bool strict)
{
	int32 score = -1;
	ChaosCastlePlayer* player = nullptr;
	int16 level = 0;
	int64 experience = 0;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsLive() || pPlayer->GetWorldId() != this->GetWorldID() )
			continue;

		int32 m_score = (this->GetPlayer(i)->ToCC()->GetPlayerKill() * 1) + (this->GetPlayer(i)->ToCC()->GetMonsterKill() * 2);
		int16 m_level = pPlayer->GetTotalLevel();
		int64 m_experience = pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetExperience();

		if ( score < m_score )
		{
			score = m_score;
			player = this->GetPlayer(i)->ToCC();
			level = m_level;
			experience = m_experience;
		}
		else
		{
			if ( score ==  m_score)
			{
				if ( level <= m_level )
				{
					if ( level == m_level )
					{
						if ( experience > m_experience )
						{
							score = m_score;
							player = this->GetPlayer(i)->ToCC();
							level = m_level;
							experience = m_experience;
						}
					}
				}
				else
				{
					score = m_score;
					player = this->GetPlayer(i)->ToCC();
					level = m_level;
					experience = m_experience;
				}
			}
		}
	}

	if ( player )
	{
		if ( player->GetMonsterKill() == 0 && player->GetPlayerKill() == 0 && this->GetPlayersCount(false) != 1 || this->GetMonsterCount() != 0 )
		{
			if ( strict )
				player = nullptr;
		}
	}

	return player;
}

void ChaosCastle::SendFailToAll(ChaosCastlePlayer const* player)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( !this->GetPlayer(i)->GetPlayer() )
			continue;

		if ( player && this->GetPlayer(i)->GetPlayer() == player->GetPlayer() )
			continue;

		this->RewardExperience(this->GetPlayer(i)->ToCC(), false);
	}
}

void ChaosCastle::RestoreCastleZone()
{
	World * pWorld = sWorldMgr->GetWorld(this->GetWorldID());

	if ( !pWorld )
	{
		return;
	}

	pWorld->ApplyAttribute(g_ChaosCastleSafetyZone, WORLD_ATTRIBUTE_SAFE, true);
	pWorld->ApplyAttribute(g_ChaosCastleCenterHollowZone[0], WORLD_ATTRIBUTE_LOCK_2, true);
	pWorld->ApplyAttribute(g_ChaosCastleCenterHollowZone[1], WORLD_ATTRIBUTE_LOCK_2, true);

	for ( int32 iTRAP_STEP = 1; iTRAP_STEP < MAX_CHAOS_CASTLE_TRAP_STEP; ++iTRAP_STEP )
	{
		for ( int32 a = 0; a < MAX_CHAOS_CASTLE_HOLLOW_ZONE; ++a )
		{
			for ( coord_type x = g_ChaosCastleMapHollowZone[iTRAP_STEP][a].X1; x <= g_ChaosCastleMapHollowZone[iTRAP_STEP][a].X2; x++ )
			{
				for ( coord_type y = g_ChaosCastleMapHollowZone[iTRAP_STEP][a].Y1; y <= g_ChaosCastleMapHollowZone[iTRAP_STEP][a].Y2; y++ )
				{
					pWorld->ApplyAttribute(x, y, WORLD_ATTRIBUTE_LOCK_2, false);
				}
			}
		}
	}
}

void ChaosCastle::UnsafetyCastleZone()
{
	World * pWorld = sWorldMgr->GetWorld(this->GetWorldID());

	if ( !pWorld )
		return;

	pWorld->ApplyAttribute(g_ChaosCastleSafetyZone, WORLD_ATTRIBUTE_SAFE, false);
}

void ChaosCastle::SendSafetyCastleZone(bool apply)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, WORLD_ATTRIBUTE_SAFE, 1, apply, g_ChaosCastleSafetyZone);
	}
}

CChaosCastleMgr::CChaosCastleMgr(): EventCommonMgr(MAX_CHAOS_CASTLE_GROUND)
{
	FOR_EACH_GROUND(CHAOS_CASTLE)->SetGround(i);
	FOR_EACH_GROUND(CHAOS_CASTLE)->SetEvent(EVENT_CHAOS_CASTLE);
	FOR_EACH_GROUND(CHAOS_CASTLE)->SetRespawnGate(CHAOS_CASTLE_GATE);
	FOR_EACH_GROUND(CHAOS_CASTLE)->SetEnterGate(m_ChaosCastleData[i].gate);
	FOR_EACH_GROUND(CHAOS_CASTLE)->SetWorldID(m_ChaosCastleData[i].world);
	FOR_EACH_GROUND(CHAOS_CASTLE)->SetState_None();

	this->SetState_None();
}

CChaosCastleMgr::~CChaosCastleMgr()
{
	for ( uint8 i = 0; i < MAX_CHAOS_CASTLE_GROUND; i++ )
	{
		this->GetGround(i)->item_list.clear();
	}
}

void CChaosCastleMgr::LoadChaosCastleSetting()
{
	if ( !sGameServer->IsChaosCastleEnabled() )
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Chaos Castle Settings...");

	ReadScript file("EventData/ChaosCastle.txt", "rb");

	if ( !file.open() )
	{
		file.LogOpenError();
		return;
	}

	while ( true )
	{
		if ( file.GetToken() == END )
			break;

		uint8 section = file.TokenNumber;

		while ( true )
		{
			file.GetToken();

			if ( file.IsEnd("end") )
				break;

			switch ( section )
			{
			case 0:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	int32 min_players = file.GetInt32();
					file.GetToken();	int32 max_players = file.GetInt32();
					file.GetToken();	uint32 required_zen = file.GetUInt32();
					file.GetToken();	bool pk_check = file.GetBool();
					file.GetToken();	uint8 pk_level_max = file.GetUInt8();
					file.GetToken();	int32 experience_user = file.GetInt32();
					file.GetToken();	int32 experience_monster = file.GetInt32();
					file.GetToken();	bool remove_elf_soldier_buff = file.GetBool();

					if ( id < MAX_CHAOS_CASTLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetMinPlayers(min_players);
						this->GetGround(id)->GetSettings()->SetMaxPlayers(max_players);
						this->GetGround(id)->GetSettings()->SetRequiredZen(required_zen);
						this->GetGround(id)->GetSettings()->SetPKCheck(pk_check);
						this->GetGround(id)->GetSettings()->SetPKLevelMax(pk_level_max);
						this->GetGround(id)->GetSettings()->SetExperienceUser(experience_user);
						this->GetGround(id)->GetSettings()->SetExperienceMonster(experience_monster);
						this->GetGround(id)->GetSettings()->SetRemoveElfSoldierBuff(remove_elf_soldier_buff);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Chaos Castle Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CChaosCastleMgr::StartGround(uint32 duration)
{
	if ( !sGameServer->IsChaosCastleEnabled() )
	{
		return;
	}

	FOR_EACH_GROUND(CHAOS_CASTLE)->SetDuration(duration);
}

void CChaosCastleMgr::Update()
{
	FOR_EACH_GROUND(CHAOS_CASTLE)->UpdateTime();
}

void CChaosCastleMgr::SetState_None()
{
	this->SetState(EVENT_MGR_STATE_NONE);
	this->SetMessageSent(EVENT_MESSAGE_OPEN_CHAOS_CASTLE, false);
	this->RunTime(0);

	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleMgr] SetState_None()");
}
		
void CChaosCastleMgr::SetState_Notify()
{
	this->SetState(EVENT_MGR_STATE_NOTIFY);
	this->RunTime(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);

	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleMgr] SetState_Notify()");
}
		
void CChaosCastleMgr::SetState_Open()
{
	FOR_EACH_GROUND(CHAOS_CASTLE)->SetState_Open();

	this->SetState(EVENT_MGR_STATE_OPEN);
	this->RunTime(EVENT_MGR_OPEN_TIME * MINUTE * IN_MILLISECONDS);

	sServerLink->EventNotification(EVENT_NOTIFICATION_CHAOS_CASTLE, 1);

	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleMgr] SetState_Open()");
}
		
void CChaosCastleMgr::SetState_Playing()
{
	this->SetState(EVENT_MGR_STATE_PLAYING);
	this->RunTime(0);

	sServerLink->EventNotification(EVENT_NOTIFICATION_CHAOS_CASTLE, 0);

	FOR_EACH_GROUND(CHAOS_CASTLE)->SetState_StandBy();

	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleMgr] SetState_Playing()");
}

void CChaosCastleMgr::ProcState_None()
{

}
	
void CChaosCastleMgr::ProcState_Notify()
{
	if ( this->TimePassed() )
		this->SetState_Open();
}
		
void CChaosCastleMgr::ProcState_Open()
{
	if ( this->GetNotificationTime()->Elapsed(EVENT_NOTIFICATION_TIME) )
	{
		sServerLink->EventNotification(EVENT_NOTIFICATION_CHAOS_CASTLE, 1);
	}

	this->SendRemainMinutes("%d minute(s) left before Chaos Castle start", 1, 5);

	this->SendMessageID(EVENT_MESSAGE_OPEN_CHAOS_CASTLE);

	if ( this->TimePassed() )
		this->SetState_Playing();
}

void CChaosCastleMgr::ProcState_Playing()
{
	for ( uint8 i = 0; i < MAX_CHAOS_CASTLE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() != CHAOS_CASTLE_STATE_NONE )
		{
			return;
		}
	}

	this->SetState_None();
}

void CChaosCastleMgr::EnterRequest(Player * pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !this->EnterAllowed(pPlayer) )
		return;
	
	POINTER_PCT_LOG(CHAOS_CASTLE_ENTER_REQUEST, lpMsg, Packet, 0);

	uint8 ground = sEventMgr->GetEventLevel(EVENT_CHAOS_CASTLE, pPlayer);
	uint8 item_slot = lpMsg->item_slot;

	if ( ground >= MAX_CHAOS_CASTLE_GROUND )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( !use_inventory_range(item_slot) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( this->GetGround(ground)->GetSettings()->IsPKCheck() && (pPlayer->GetPKLevel() >= this->GetGround(ground)->GetSettings()->GetPKLevelMax()) )
	{
		this->EnterResult(pPlayer, 0x08);
		return;
	}

	if ( !sGameServer->IsChaosCastleEnabled() )
	{
		pPlayer->MoveToGate(CHAOS_CASTLE_GATE);
		return;
	}

	if ( this->GetState() != EVENT_MGR_STATE_OPEN )
	{
		this->EnterResult(pPlayer, 0x02);
		return;
	}

	if ( !sEventMgr->CanEnterEvent(pPlayer, EVENT_CHAOS_CASTLE) )
	{
		this->EnterResult(pPlayer, 0x0B);
		return;
	}

	if ( (pPlayer->GetInventory()->GetItem(RING_01)->IsItem() && pPlayer->GetInventory()->GetItem(RING_01)->IsTransformationRing()) ||
		 (pPlayer->GetInventory()->GetItem(RING_02)->IsItem() && pPlayer->GetInventory()->GetItem(RING_02)->IsTransformationRing()) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't enter Chaos Castle while wearing a Transformation Ring.");
		this->EnterResult(pPlayer, 0x00);
		return;
	}

	int32 current_players = this->GetGround(ground)->GetPlayersCount(true);

	if ( current_players >= this->GetGround(ground)->GetPlayerSize() )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	if ( !this->HaveTicket(pPlayer->GetInventory()->GetItem(item_slot), ITEMGET(13, 29), ITEMGET(13, 121), -1) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( !pPlayer->MoneyHave(this->GetGround(ground)->GetSettings()->GetRequiredZen()) )
	{
		this->EnterResult(pPlayer, 0x07);
		return;
	}

	pPlayer->GetInterfaceState()->Reset();
	
	if ( !this->GetGround(ground)->AddPlayer(pPlayer) )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	if ( !pPlayer->MoveToGate(this->GetGround(ground)->GetEnterGate()) )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	pPlayer->MoneyReduce(this->GetGround(ground)->GetSettings()->GetRequiredZen());
	this->ConsumeTicket(pPlayer, ITEMGET(13, 29), ITEMGET(13, 121), item_slot);

	if ( this->GetGround(ground)->GetSettings()->IsRemoveElfSoldierBuff() )
	{
		pPlayer->RemoveBuff(BUFF_ELF_SOLDIER);
	}

	pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You have entered Chaos Castle %d", ground + 1);
	this->EnterResult(pPlayer, 0);

	sEventMgr->EventEnter(pPlayer, EVENT_CHAOS_CASTLE);
}
	
void CChaosCastleMgr::EnterResult(Player * pPlayer, uint8 result)
{
	CHAOS_CASTLE_ENTER_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

bool CChaosCastleMgr::EnterAllowed(Player * pPlayer)
{
	if ( !pPlayer || pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
		return false;

	if ( !pPlayer->IsWorldFlag(WORLD_FLAG_ALLOW_ENTER_CHAOS_CASTLE) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You cannot enter Chaos Castle from here.");
		return false;
	}

	if ( !pPlayer->IsInSafeZone() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can enter Chaos Castle from safe zone.");
		return false;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( pGuild && pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You cannot enter Chaos Castle while in a Guild War.");
		return false;
	}

	if ( pPlayer->GetInventory()->GetItem(RING_01)->IsTransformationRing() || pPlayer->GetInventory()->GetItem(RING_02)->IsTransformationRing() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You cannot enter Chaos Castle while wearing a transformation ring.");
		return false;
	}

	return true;
}

bool CChaosCastleMgr::IsIn(Player* pPlayer)
{
	if ( this->GetState() != EVENT_MGR_STATE_PLAYING &&
		 this->GetState() != EVENT_MGR_STATE_OPEN )
		return false;

	for ( uint8 i = 0; i < MAX_CHAOS_CASTLE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() == CHAOS_CASTLE_STATE_NONE ||
			 this->GetGround(i)->GetState() == CHAOS_CASTLE_STATE_PLAYEND )
			continue;

		if ( !this->GetGround(i)->IsIn(pPlayer) )
			continue;

		pPlayer->SetEventId(EVENT_CHAOS_CASTLE);
		pPlayer->SetEventGround(i);
		return true;
	}

	return false;
}