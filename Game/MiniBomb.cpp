MiniBomb::MiniBomb()
{
	this->SetActive(true);
}

MiniBomb::~MiniBomb()
{
	LIST_CLEAR(MiniBombRewardList::iterator, this->reward_list);
}

void MiniBomb::LoadReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mini Bomb Reward...");

	LIST_CLEAR(MiniBombRewardList::iterator, this->reward_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_mini_bomb_reward");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			MiniBombReward * add_item = new MiniBombReward;

			add_item->SetScoreMin(fields[0].GetUInt16());
			add_item->SetScoreMax(fields[1].GetUInt16());
			add_item->SetItem(fields[2].GetUInt16());
			add_item->SetLevel(fields[3].GetUInt8());

			this->reward_list.push_back(add_item);

			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mini bomb reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MiniBomb::Open(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	POINTER_PCT_LOG(MINI_BOMB_OPEN_REQUEST, lpMsg, Packet, 0);

	this->SendCell(pPlayer, pPlayer->GetMiniBombState());

	if ( pPlayer->GetMiniBombState() == MINI_BOMB_GAME_RESULT )
	{
		this->Finish(pPlayer, false);
	}
}

void MiniBomb::SendCell(Player* pPlayer, uint8 type)
{
	if ( !pPlayer )
	{
		return;
	}

	/*
	Consumir Ticket cuando es Game Start

	El buscaminas es de 6 x 8

	0 = Nada
	1 = Game Start
	2 = Game Current
	3 = Game Result
	*/

	uint8 buffer[1024];
	POINTER_PCT(MINI_BOMB_OPEN_HEAD, head, buffer, 0);
	POINTER_PCT(MINI_BOMB_OPEN_BODY, body, buffer, sizeof(MINI_BOMB_OPEN_HEAD));
	head->result = type;
	head->remain_bombs = 0;
	head->count = 0;
	head->junk = 0;
	head->current_score = 0;

	switch ( type )
	{
	case MINI_BOMB_GAME_START:
		{
			
		} break;

	case MINI_BOMB_GAME_CURRENT:
		{
			MINI_BOMB_CREATE_CELL pMsg;
			pMsg.x = MINI_BOMB_SIZE_W;
			pMsg.y = MINI_BOMB_SIZE_H;
			pMsg.effect = MINI_BOMB_TRAP_COUNT;
			pPlayer->SEND_PCT(pMsg);

			head->remain_bombs = this->GetRemainBombs(pPlayer);
			head->current_score = pPlayer->GetMiniBombCurrentScore();

			for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
			{
				if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_HIDE )
				{
					continue;
				}

				body[head->count].cell = h;

				if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_MARKED )
				{
					body[head->count].type = 0xFE;
				}
				else
				{
					body[head->count].type = pPlayer->GetMiniBombData(h)->GetValue();
				}

				++head->count;
			}
		} break;

	case MINI_BOMB_GAME_RESULT:
		{
		} break;
	}
	
	head->h.set(0x4D, 0x27, sizeof(MINI_BOMB_OPEN_HEAD) + (head->count * sizeof(MINI_BOMB_OPEN_BODY)));
	pPlayer->sendPacket(buffer, head->h.get_size());
}

bool IsMiniBombMineCell(Player* pPlayer, int32 h, int32 w)
{
    if ( (h >= 0) && (h < MINI_BOMB_SIZE_H) &&
         (w >= 0) && (w < MINI_BOMB_SIZE_W) )
	{
		if ( pPlayer->GetMiniBombData(h * 8 + w)->GetValue() == MINI_BOMB_CELL_VALUE_MINE )
		{
			return true;
		}
	}

	return false;
}

bool IsMiniBombNormalCell(Player* pPlayer, int32 h, int32 w)
{
    if ( (h >= 0) && (h < MINI_BOMB_SIZE_H) &&
         (w >= 0) && (w < MINI_BOMB_SIZE_W) )
	{
		if ( pPlayer->GetMiniBombData(h * 8 + w)->GetStatus() == MINI_BOMB_CELL_STATUS_HIDE )
		{
			if ( pPlayer->GetMiniBombData(h * 8 + w)->GetValue() != MINI_BOMB_CELL_VALUE_MINE )
			{
				return true;
			}
		}
	}

	return false;
}

void MiniBomb::Generate(Player* pPlayer, uint8 starting_cell)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( int32 i = 0; i < MINI_BOMB_SIZE; ++i )
	{
		pPlayer->GetMiniBombData(i)->SetStatus(MINI_BOMB_CELL_STATUS_HIDE);
		pPlayer->GetMiniBombData(i)->SetValue(MINI_BOMB_CELL_VALUE_EMPTY);
	}

	if ( starting_cell == uint8(-1) )
	{
		return;
	}

	for ( int32 i = 0; i < MINI_BOMB_TRAP_COUNT; )
	{
		int32 h = Random(MINI_BOMB_SIZE);
 
		if ( pPlayer->GetMiniBombData(h)->GetValue() != MINI_BOMB_CELL_VALUE_MINE && h != starting_cell )
		{
			pPlayer->GetMiniBombData(h)->SetValue(MINI_BOMB_CELL_VALUE_MINE);
			++i;
		}
	}

	for ( int32 h = 0; h < MINI_BOMB_SIZE_H; ++h )
	{
		for ( int32 w = 0; w < MINI_BOMB_SIZE_W; ++w )
		{
			int32 cur_cell = h * 8 + w;

			if ( pPlayer->GetMiniBombData(cur_cell)->GetValue() == MINI_BOMB_CELL_VALUE_MINE )
			{
				continue;
			}

			int32 count = 0;

			if ( IsMiniBombMineCell(pPlayer, h - 1, w) )
			{
				++count;
			}

			if ( IsMiniBombMineCell(pPlayer, h - 1, w - 1) )
			{
				++count;
			}

			if ( IsMiniBombMineCell(pPlayer, h - 1, w + 1) )
			{
				++count;
			}

			if ( IsMiniBombMineCell(pPlayer, h + 1, w) )
			{
				++count;
			}

			if ( IsMiniBombMineCell(pPlayer, h + 1, w - 1) )
			{
				++count;
			}

			if ( IsMiniBombMineCell(pPlayer, h + 1, w + 1) )
			{
				++count;
			}

			if ( IsMiniBombMineCell(pPlayer, h, w - 1) )
			{
				++count;
			}

			if ( IsMiniBombMineCell(pPlayer, h, w + 1) )
			{
				++count;
			}

			pPlayer->GetMiniBombData(cur_cell)->SetValue(count);
		}
	}
}

void MiniBomb::Start(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendMessageBox(0, "AUTH", "You are not authorized for this action.");
		return;
	}

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		pPlayer->SendMessageBox(1, "Mini Bomb", "Can't play with an active transaction. \n Can't play with Personal Store open.");
		return;
	}

	if ( !this->IsActive() || !sGameServer->IsMiniBombEnabled() )
	{
		sLog->outError("mini_bomb", "%s -- Disabled -- %s", __FUNCTION__, pPlayer->BuildLog().c_str());
		return;
	}

	POINTER_PCT_LOG(MINI_BOMB_START_REQUEST, lpMsg, Packet, 0);

	if ( pPlayer->GetMiniBombState() == MINI_BOMB_GAME_START )
	{
		if ( sGameServer->IsMiniBombRequiredTicket() )
		{
			uint8 ticket = pPlayer->ItemFind(ITEMGET(14, 216), 0, true);

			if ( ticket == uint8(-1) )
			{
				this->StartResult(pPlayer, 1);
				return;
			}
		}

		pPlayer->SetMiniBombState(MINI_BOMB_GAME_CURRENT);
		pPlayer->SetMiniBombCurrentScore(0);
		pPlayer->SetMiniBombDate(time(nullptr));
		
		this->Generate(pPlayer, -1);

		sLog->outInfo("mini_bomb", "%s -- Grid Generate -- %s", __FUNCTION__, pPlayer->BuildLog().c_str());
	}
	
	this->StartResult(pPlayer, 0);
	this->SendCell(pPlayer, MINI_BOMB_GAME_CURRENT);
}
	
void MiniBomb::StartResult(Player* pPlayer, uint8 result)
{
	if ( !pPlayer )
	{
		return;
	}

	MINI_BOMB_START_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void MiniBomb::Reward(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendMessageBox(0, "AUTH", "You are not authorized for this action.");
		return;
	}

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		pPlayer->SendMessageBox(1, "Mini Bomb", "Can't play with an active transaction. \n Can't play with Personal Store open.");
		return;
	}

	POINTER_PCT_LOG(MINI_BOMB_GIFT_REQUEST, lpMsg, Packet, 0);

	if ( pPlayer->GetMiniBombState() != MINI_BOMB_GAME_RESULT )
	{
		sLog->outError("mini_bomb", "%s -- Wrong State %u -- %s", __FUNCTION__, pPlayer->GetMiniBombState(), pPlayer->BuildLog().c_str());

		this->RewardResult(pPlayer, 1);
		return;
	}

	sLog->outInfo("mini_bomb", "%s -- Rewarded -- %s", __FUNCTION__, pPlayer->BuildLog().c_str());

	this->RewardResult(pPlayer, 0);

	pPlayer->SetMiniBombState(MINI_BOMB_GAME_START);

	if ( sGameServer->IsMiniBombReward() )
	{
		MiniBombReward const* pData = this->GetReward(pPlayer);

		if ( pData )
		{
			Item item(pData->GetItem(), pData->GetLevel());
			item.SetExpireDate(time(nullptr) + DAY);

			sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);

			sLog->outInfo("mini_bomb", "%s -- RewardItem [%u/%u] -- %s", __FUNCTION__, pData->GetItem(), pData->GetLevel(), pPlayer->BuildLog().c_str());
		}
	}
}

void MiniBomb::RewardResult(Player* pPlayer, uint8 result)
{
	if ( !pPlayer )
	{
		return;
	}

	MINI_BOMB_GIFT_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void MiniBomb::Reveal(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendMessageBox(0, "AUTH", "You are not authorized for this action.");
		return;
	}

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		pPlayer->SendMessageBox(1, "Mini Bomb", "Can't play with an active transaction. \n Can't play with Personal Store open.");
		return;
	}

	if ( pPlayer->GetMiniBombState() != MINI_BOMB_GAME_CURRENT )
	{
		sLog->outError("mini_bomb", "%s -- Wrong State %u -- %s", __FUNCTION__, pPlayer->GetMiniBombState(), pPlayer->BuildLog().c_str());
		return;
	}

	POINTER_PCT_LOG(MINI_BOMB_REVEAL_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->cell >= MINI_BOMB_SIZE )
	{
		sLog->outError("mini_bomb", "%s -- Wrong Cell %u -- %s", __FUNCTION__, lpMsg->cell, pPlayer->BuildLog().c_str());
		return;
	}

	if ( !this->IsGenerated(pPlayer) )
	{
		this->Generate(pPlayer, lpMsg->cell);
	}

	if ( pPlayer->GetMiniBombData(lpMsg->cell)->GetStatus() != MINI_BOMB_CELL_STATUS_HIDE )
	{
		sLog->outError("mini_bomb", "%s -- Wrong Cell State [%u/%u] -- %s", __FUNCTION__, lpMsg->cell, pPlayer->GetMiniBombData(lpMsg->cell)->GetStatus(), pPlayer->BuildLog().c_str());
		return;
	}

	uint16 add_score = 0;

	uint8 buffer[1024];
	POINTER_PCT(MINI_BOMB_REVEAL_HEAD, head, buffer, 0);
	POINTER_PCT(MINI_BOMB_REVEAL_BODY, body, buffer, sizeof(MINI_BOMB_REVEAL_HEAD));
	head->cell = lpMsg->cell;
	head->scoreH = 0;
	head->scoreL = 0;
	head->count = 0;

	if ( pPlayer->GetMiniBombData(lpMsg->cell)->GetValue() == MINI_BOMB_CELL_VALUE_MINE )
	{
		pPlayer->GetMiniBombData(lpMsg->cell)->SetStatus(MINI_BOMB_CELL_STATUS_REVEALED);

		sLog->outInfo("mini_bomb", "%s -- Revealed Bomb [%u] -- %s", __FUNCTION__, lpMsg->cell, pPlayer->BuildLog().c_str());
	}
	else
	{
		this->RevealProcess(pPlayer, lpMsg->cell, add_score);

		pPlayer->IncreaseMiniBombCurrentScore(add_score);
		head->scoreH = HIBYTE(add_score);
		head->scoreL = LOBYTE(add_score);

		for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
		{
			if ( pPlayer->GetMiniBombData(h)->GetStatus() != MINI_BOMB_CELL_STATUS_REVEALED )
			{
				continue;
			}

			body[head->count].cell = h;
			body[head->count].type = pPlayer->GetMiniBombData(h)->GetValue();
			++head->count;
		}
	}
	
	head->h.set(0x4D, 0x24, sizeof(MINI_BOMB_REVEAL_HEAD) + (head->count * sizeof(MINI_BOMB_REVEAL_BODY)));
	pPlayer->sendPacket(buffer, head->h.get_size());

	if ( (pPlayer->GetMiniBombData(lpMsg->cell)->GetValue() == MINI_BOMB_CELL_VALUE_MINE) || this->IsRevealed(pPlayer) )
	{
		this->Finish(pPlayer, true);
	}
}

void MiniBomb::RevealProcess(Player* pPlayer, uint8 cell, uint16 & score)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetMiniBombData(cell)->GetStatus() != MINI_BOMB_CELL_STATUS_HIDE )
	{
		return;
	}

	sLog->outInfo("mini_bomb", "%s -- Revealed [%u/%u] -- %s", __FUNCTION__, cell, pPlayer->GetMiniBombData(cell)->GetValue(), pPlayer->BuildLog().c_str());

	pPlayer->GetMiniBombData(cell)->SetStatus(MINI_BOMB_CELL_STATUS_REVEALED);
	score += pPlayer->GetMiniBombData(cell)->GetValue() * sGameServer->GetMiniBombRevealScore();
	
	if ( pPlayer->GetMiniBombData(cell)->GetValue() == 0 )
	{
		int32 h = cell / 8;
		int32 w = cell % 8;

		if ( IsMiniBombNormalCell(pPlayer, h - 1, w) )
		{
			this->RevealProcess(pPlayer, (h - 1) * 8 + (w), score);
		}

		if ( IsMiniBombNormalCell(pPlayer, h - 1, w - 1) )
		{
			this->RevealProcess(pPlayer, (h - 1) * 8 + (w - 1), score);
		}

		if ( IsMiniBombNormalCell(pPlayer, h - 1, w + 1) )
		{
			this->RevealProcess(pPlayer, (h - 1) * 8 + (w + 1), score);
		}

		if ( IsMiniBombNormalCell(pPlayer, h + 1, w) )
		{
			this->RevealProcess(pPlayer, (h + 1) * 8 + (w), score);
		}

		if ( IsMiniBombNormalCell(pPlayer, h + 1, w - 1) )
		{
			this->RevealProcess(pPlayer, (h + 1) * 8 + (w - 1), score);
		}

		if ( IsMiniBombNormalCell(pPlayer, h + 1, w + 1) )
		{
			this->RevealProcess(pPlayer, (h + 1) * 8 + (w + 1), score);
		}

		if ( IsMiniBombNormalCell(pPlayer, h, w - 1) )
		{
			this->RevealProcess(pPlayer, (h) * 8 + (w - 1), score);
		}

		if ( IsMiniBombNormalCell(pPlayer, h, w + 1) )
		{
			this->RevealProcess(pPlayer, (h) * 8 + (w + 1), score);
		}
	}
}

bool MiniBomb::IsRevealed(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return false;
	}

	for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
	{
		if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_HIDE && pPlayer->GetMiniBombData(h)->GetValue() != MINI_BOMB_CELL_VALUE_MINE )
		{
			return false;
		}
	}

	return true;
}

bool MiniBomb::IsGenerated(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return false;
	}

	for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
	{
		if ( pPlayer->GetMiniBombData(h)->GetValue() != MINI_BOMB_CELL_VALUE_EMPTY )
		{
			return true;
		}
	}

	return false;
}

void MiniBomb::Mark(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendMessageBox(0, "AUTH", "You are not authorized for this action.");
		return;
	}

	if ( !Player::IsPlayerTransactionFree(pPlayer) )
	{
		pPlayer->SendMessageBox(1, "Mini Bomb", "Can't play with an active transaction. \n Can't play with Personal Store open.");
		return;
	}

	if ( pPlayer->GetMiniBombState() != MINI_BOMB_GAME_CURRENT )
	{
		sLog->outError("mini_bomb", "%s -- Wrong State %u -- %s", __FUNCTION__, pPlayer->GetMiniBombState(), pPlayer->BuildLog().c_str());
		return;
	}

	if ( !this->IsGenerated(pPlayer) )
	{
		sLog->outError("mini_bomb", "%s -- Not generated -- %s", __FUNCTION__, pPlayer->BuildLog().c_str());
		return;
	}

	POINTER_PCT_LOG(MINI_BOMB_MARK_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->cell >= MINI_BOMB_SIZE )
	{
		sLog->outError("mini_bomb", "%s -- Wrong Cell %u -- %s", __FUNCTION__, lpMsg->cell, pPlayer->BuildLog().c_str());
		return;
	}

	MINI_BOMB_MARK_RESULT pMsg(2);
	pMsg.cell = lpMsg->cell;

	if ( pPlayer->GetMiniBombData(lpMsg->cell)->GetStatus() == MINI_BOMB_CELL_STATUS_MARKED )
	{
		pPlayer->GetMiniBombData(lpMsg->cell)->SetStatus(MINI_BOMB_CELL_STATUS_HIDE);
		pMsg.result = 0;

		sLog->outInfo("mini_bomb", "%s -- Cell Mark [%u/%u] -- %s", __FUNCTION__, lpMsg->cell, pPlayer->GetMiniBombData(lpMsg->cell)->GetValue(), pPlayer->BuildLog().c_str());
	}
	else if ( pPlayer->GetMiniBombData(lpMsg->cell)->GetStatus() == MINI_BOMB_CELL_STATUS_HIDE && this->GetRemainBombs(pPlayer) > 0 )
	{
		pPlayer->GetMiniBombData(lpMsg->cell)->SetStatus(MINI_BOMB_CELL_STATUS_MARKED);
		pMsg.result = 1;

		sLog->outInfo("mini_bomb", "%s -- Cell Unmark [%u/%u] -- %s", __FUNCTION__, lpMsg->cell, pPlayer->GetMiniBombData(lpMsg->cell)->GetValue(), pPlayer->BuildLog().c_str());
	}

	pMsg.remain_bombs = this->GetRemainBombs(pPlayer);

	pPlayer->SEND_PCT(pMsg);
}

void MiniBomb::Finish(Player* pPlayer, bool from_game)
{
	if ( !pPlayer )
	{
		return;
	}

	pPlayer->SetMiniBombState(MINI_BOMB_GAME_RESULT);

	bool failed = false;

	for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
	{
		if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_REVEALED && pPlayer->GetMiniBombData(h)->GetValue() == MINI_BOMB_CELL_VALUE_MINE )
		{
			failed = true;
			break;
		}
	}

	uint8 buffer[1024];
	POINTER_PCT(MINI_BOMB_RESULT_HEAD, head, buffer, 0);
	POINTER_PCT(MINI_BOMB_RESULT_BODY, body, buffer, sizeof(MINI_BOMB_RESULT_HEAD));
	head->result = failed ? 1 : 0;
	head->count = 0;
	head->score = pPlayer->GetMiniBombCurrentScore();
	head->bombs_found = failed ? this->GetFoundedBombs(pPlayer) * sGameServer->GetMiniBombFoundedBombsScore() : MINI_BOMB_TRAP_COUNT * sGameServer->GetMiniBombFoundedBombsScore();
	head->bombs_failure = failed ? this->GetWrongBombs(pPlayer) * sGameServer->GetMiniBombWrongBombsScore() : 0;
	head->stepped_on_bomb = failed ? sGameServer->GetMiniBombFailScore() : 0;
	head->clear = failed ? 0: sGameServer->GetMiniBombClearScore();

	int32 score = 0;
	score += head->score;
	score += head->bombs_found;
	score += head->clear;
	score -= head->bombs_failure;
	score -= head->stepped_on_bomb;

	if ( score < 0 )
	{
		score = 0;
	}

	head->total_score = score;

	pPlayer->SetMiniBombFinalScore(head->total_score);

	if ( from_game )
	{
		pPlayer->IncreaseMiniBombTotalScore(score);

		pPlayer->UpdateStatistic(failed ? STATISTIC_MINI_BOMB_FAIL : STATISTIC_MINI_BOMB_SUCCESS, 1);

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_MINI_BOMB);
		stmt->setUInt32(0, pPlayer->GetGUID());
		stmt->setUInt8(1, failed ? 0 : 1);
		stmt->setUInt16(2, pPlayer->GetMiniBombCurrentScore());
		stmt->setUInt8(3, failed ? this->GetFoundedBombs(pPlayer) : MINI_BOMB_TRAP_COUNT);
		stmt->setUInt8(4, failed ? this->GetWrongBombs(pPlayer) : 0);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}

	for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
	{
		if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_HIDE )
		{
			continue;
		}

		if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_MARKED && pPlayer->GetMiniBombData(h)->GetValue() != MINI_BOMB_CELL_VALUE_MINE )
		{
			body[head->count].cell = h;
			body[head->count].type = 1;
			++head->count;
		}
	}

	head->h.set(0x4D, 0x26, sizeof(MINI_BOMB_RESULT_HEAD) + (head->count * sizeof(MINI_BOMB_RESULT_BODY)));
	pPlayer->sendPacket(buffer, head->h.get_size());
}

uint8 MiniBomb::GetRemainBombs(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return 0;
	}

	int32 count = MINI_BOMB_TRAP_COUNT;

	for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
	{
		if ( count > 0 && pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_MARKED )
		{
			--count;
		}
	}

	return count;
}

uint8 MiniBomb::GetFoundedBombs(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return 0;
	}

	int32 count = 0;

	for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
	{
		if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_MARKED &&
			 pPlayer->GetMiniBombData(h)->GetValue() == MINI_BOMB_CELL_VALUE_MINE )
		{
			++count;
		}
	}

	return count;
}

uint8 MiniBomb::GetWrongBombs(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return 0;
	}

	int32 count = 0;

	for ( int32 h = 0; h < MINI_BOMB_SIZE; ++h )
	{
		if ( pPlayer->GetMiniBombData(h)->GetStatus() == MINI_BOMB_CELL_STATUS_MARKED &&
			 pPlayer->GetMiniBombData(h)->GetValue() != MINI_BOMB_CELL_VALUE_MINE )
		{
			++count;
		}
	}

	return count;
}

MiniBombReward const* MiniBomb::GetReward(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return nullptr;
	}

	for ( MiniBombRewardList::const_iterator itr = this->reward_list.begin(); itr != this->reward_list.end(); ++itr )
	{
		MiniBombReward const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( pPlayer->GetMiniBombFinalScore() >= pData->GetScoreMin() && pPlayer->GetMiniBombFinalScore() <= pData->GetScoreMax() )
		{
			return pData;
		}
	}

	return nullptr;
}