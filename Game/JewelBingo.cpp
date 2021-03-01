JewelBingo::JewelBingo()
{
	this->SetActive(true);
}

JewelBingo::~JewelBingo()
{
	LIST_CLEAR(JewelBingoRewardList::iterator, this->reward_list);
}

void JewelBingo::LoadReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Jewel Bingo Reward...");

	LIST_CLEAR(JewelBingoRewardList::iterator, this->reward_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_jewel_bingo_reward");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			JewelBingoReward * add_item = new JewelBingoReward;

			add_item->SetType(fields[0].GetUInt8());
			add_item->SetScoreMin(fields[1].GetInt32());
			add_item->SetScoreMax(fields[2].GetInt32());
			add_item->SetItem(fields[3].GetUInt16());
			add_item->SetLevel(fields[4].GetUInt8());

			this->reward_list.push_back(add_item);

			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u jewel bingo reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void JewelBingo::Open(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	this->SendState(pPlayer, 0);
}

void JewelBingo::Start(Player* pPlayer, uint8 * Packet)
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
		pPlayer->SendMessageBox(1, "Jewel Bingo", "Can't play with an active transaction. \n Can't play with Personal Store open.");
		return;
	}

	if ( !this->IsActive() || !sGameServer->IsJewelBingoEnabled() )
	{
		sLog->outError("jewel_bingo", "%s -- Disabled -- %s", __FUNCTION__, pPlayer->BuildLog().c_str());
		return;
	}

	if ( pPlayer->GetJewelBingoState() == JEWEL_BINGO_STATE_NONE )
	{
		if ( sGameServer->IsJewelBingoRequiredTicket() )
		{
			uint8 ticket = pPlayer->ItemFind(ITEMGET(14, 216), 0, true);

			if ( ticket == uint8(-1) )
			{
				this->SendState(pPlayer, 0x0E);
				return;
			}
		}

		pPlayer->SetJewelBingoState(JEWEL_BINGO_STATE_JEWEL_PLACE);
		pPlayer->SetJewelBingoBox(0);
		pPlayer->SetJewelBingoCount(14);
		pPlayer->SetJewelBingoCurrentJewel(0);
		pPlayer->SetJewelBingoScore1(0);
		pPlayer->SetJewelBingoScore2(0);
		pPlayer->SetJewelBingoScore3(0);
		pPlayer->SetJewelBingoDate(time(nullptr));

		for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
		{
			pPlayer->GetJewelBingoGrid(i)->SetSlot(i);
			pPlayer->GetJewelBingoGrid(i)->SetType(0);
			pPlayer->GetJewelBingoGrid(i)->SetValue(-1);
		}

		for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
		{
			pPlayer->GetJewelBingoJewel(i)->SetSlot(i);
			pPlayer->GetJewelBingoJewel(i)->SetType(1);
			pPlayer->GetJewelBingoJewel(i)->SetValue(4);
		}

		for ( int32 i = 0; i < JEWEL_BINGO_MATCHING_SIZE; ++i )
		{
			pPlayer->GetJewelBingoMatching(i)->SetSlot(i);
			pPlayer->GetJewelBingoMatching(i)->SetType(2);
			pPlayer->GetJewelBingoMatching(i)->SetValue(0);
		}
	}

	switch ( pPlayer->GetJewelBingoState() )
	{
	case JEWEL_BINGO_STATE_BOX_SELECT:
		{
			this->SendState(pPlayer, 3);
			this->SendBoxSelect(pPlayer);
		} break;

	case JEWEL_BINGO_STATE_PLAYING:
		{
			this->SendState(pPlayer, 4);
			this->SendPlayInfo(pPlayer);
		} break;

	case JEWEL_BINGO_STATE_REWARD:
		{
			this->SendState(pPlayer, 3);
			this->SendPlayResult(pPlayer);
		} break;

	default:
		{
			this->SendState(pPlayer, 1);
			this->SendJewelState(pPlayer, 0);
		} break;
	}
}

void JewelBingo::Move(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
	{
		return;
	}

	if ( pPlayer->GetJewelBingoState() != JEWEL_BINGO_STATE_JEWEL_PLACE )
	{
		this->SendJewelState(pPlayer, 0);
		return;
	}

	POINTER_PCT_LOG(JEWEL_BINGO_MOVE_REQUEST, lpMsg, Packet, 0);

	switch ( lpMsg->type )
	{
	case 0: ///- Manual
		{
			if ( lpMsg->jewel_type >= JEWEL_BINGO_JEWEL_COUNT )
			{
				this->SendJewelState(pPlayer, 0);
				return;
			}

			if ( lpMsg->target_slot >= JEWEL_BINGO_GRID_SIZE || lpMsg->target_slot == 12 )
			{
				this->SendJewelState(pPlayer, 0);
				return;
			}

			if ( pPlayer->GetJewelBingoJewel(lpMsg->jewel_type)->GetValue() <= 0 )
			{
				this->SendJewelState(pPlayer, 0);
				return;
			}

			if ( pPlayer->GetJewelBingoGrid(lpMsg->target_slot)->GetValue() != uint8(-1) )
			{
				this->SendJewelState(pPlayer, 0);
				return;
			}

			pPlayer->GetJewelBingoGrid(lpMsg->target_slot)->SetValue(lpMsg->jewel_type);
			pPlayer->GetJewelBingoJewel(lpMsg->jewel_type)->ReduceValue(1);

			this->SendJewelState(pPlayer, 0);
		} break;

	case 1: ///- Automatic
		{
			this->SortJewels(pPlayer);
			this->SendJewelState(pPlayer, 0);
		} break;
	}

	int32 total_jewel = 0;

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		total_jewel += pPlayer->GetJewelBingoJewel(i)->GetValue();
	}

	if ( total_jewel <= 0 )
	{
		pPlayer->SetJewelBingoState(JEWEL_BINGO_STATE_BOX_SELECT);

		for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
		{
			pPlayer->GetJewelBingoJewel(i)->SetSlot(i);
			pPlayer->GetJewelBingoJewel(i)->SetType(1);
			pPlayer->GetJewelBingoJewel(i)->SetValue(4);
		}

		this->SendState(pPlayer, 3);
		this->SendBoxSelect(pPlayer);
	}
}

void JewelBingo::BoxSelect(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false ,false) )
	{
		return;
	}

	if ( pPlayer->GetJewelBingoState() != JEWEL_BINGO_STATE_BOX_SELECT )
	{
		return;
	}

	POINTER_PCT_LOG(JEWEL_BINGO_BOX_SELECT_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->box > 2 )
	{
		lpMsg->box = 0;
	}

	this->GenerateRandomJewel(pPlayer);
	pPlayer->SetJewelBingoBox(lpMsg->box);
	pPlayer->SetJewelBingoState(JEWEL_BINGO_STATE_PLAYING);
	this->SendPlayInfo(pPlayer);
}
	
void JewelBingo::JewelSelect(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false ,false) )
	{
		return;
	}

	if ( pPlayer->GetJewelBingoState() != JEWEL_BINGO_STATE_PLAYING )
	{
		return;
	}

	POINTER_PCT_LOG(JEWEL_BINGO_JEWEL_SELECT, lpMsg, Packet, 0);

	if ( lpMsg->slot < JEWEL_BINGO_GRID_SIZE && lpMsg->jewel_type < JEWEL_BINGO_JEWEL_COUNT && pPlayer->GetJewelBingoCount() > 0 )
	{
		if ( pPlayer->GetJewelBingoGrid(lpMsg->slot)->GetValue() == lpMsg->jewel_type )
		{
			pPlayer->GetJewelBingoGrid(lpMsg->slot)->SetValue(lpMsg->jewel_type + 0xF0);
			
			pPlayer->SetJewelBingoCount(pPlayer->GetJewelBingoCount() - 1);

			this->GenerateRandomJewel(pPlayer);
			this->CheckMatching(pPlayer, lpMsg->slot);
		}
	}

	this->SendPlayInfo(pPlayer);

	if ( pPlayer->GetJewelBingoCount() <= 0 )
	{
		pPlayer->SetJewelBingoState(JEWEL_BINGO_STATE_REWARD);
		this->GenerateNotMatchedScore(pPlayer);
		this->SendPlayResult(pPlayer);
	}
}

void JewelBingo::Reward(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetJewelBingoState() != JEWEL_BINGO_STATE_REWARD )
	{
		return;
	}

	POINTER_PCT_LOG(JEWEL_BINGO_REWARD_REQUEST, lpMsg, Packet, 0);

	if ( sGameServer->IsJewelBingoReward() )
	{
		JewelBingoReward const* pReward = this->GetReward(pPlayer);

		if ( pReward )
		{
			Item item(pReward->GetItem(), pReward->GetLevel());
			item.SetExpireDate(time(nullptr) + DAY);

			sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
		}
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_JEWEL_BINGO);
	stmt->setUInt32(0, pPlayer->GetGUID());
	stmt->setUInt8(1, pPlayer->GetJewelBingoBox());
	stmt->setInt32(2, pPlayer->GetJewelBingoScore1());
	stmt->setInt32(3, pPlayer->GetJewelBingoScore2());
	stmt->setInt32(4, pPlayer->GetJewelBingoScore3());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	pPlayer->SetJewelBingoState(JEWEL_BINGO_STATE_NONE);
	this->Open(pPlayer);
}

void JewelBingo::SendState(Player* pPlayer, uint8 state)
{
	if ( !pPlayer )
	{
		return;
	}

	JEWEL_BINGO_STATE pMsg(state);
	pPlayer->SEND_PCT(pMsg);
}

void JewelBingo::SendJewelState(Player* pPlayer, uint8 result)
{
	if ( !pPlayer )
	{
		return;
	}

	JEWEL_BINGO_INFO pMsg;
	pMsg.result = result;

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		pMsg.grid[i] = pPlayer->GetJewelBingoGrid(i)->GetValue();
	}

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		pMsg.current_jewel[i] = pPlayer->GetJewelBingoJewel(i)->GetValue();
	}

	if ( pPlayer->GetJewelBingoState() != JEWEL_BINGO_STATE_JEWEL_PLACE )
	{
		for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
		{
			pMsg.current_jewel[i] = 0;
		}
	}

	pPlayer->SEND_PCT(pMsg);
}

void JewelBingo::SendBoxSelect(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	JEWEL_BINGO_BOX_SELECT pMsg;
	pPlayer->SEND_PCT(pMsg);
}

void JewelBingo::SendPlayInfo(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	JEWEL_BINGO_PLAY_INFO pMsg;
	pMsg.current_box = pPlayer->GetJewelBingoBox();
	pMsg.current_jewel = pPlayer->GetJewelBingoCurrentJewel();
	pMsg.jewel_count = pPlayer->GetJewelBingoCount();

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		pMsg.grid[i] = pPlayer->GetJewelBingoGrid(i)->GetValue();
	}

	for ( int32 i = 0; i < JEWEL_BINGO_MATCHING_SIZE; ++i )
	{
		pMsg.matching_jewel[i] = pPlayer->GetJewelBingoMatching(i)->GetValue();
	}

	pPlayer->SEND_PCT(pMsg);
}

void JewelBingo::SendPlayResult(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	JEWEL_BINGO_PLAY_RESULT pMsg;
	pMsg.score1 = pPlayer->GetJewelBingoScore1();
	pMsg.score2 = pPlayer->GetJewelBingoScore2();
	pMsg.score3 = pPlayer->GetJewelBingoScore3();

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		pMsg.grid[i] = pPlayer->GetJewelBingoGrid(i)->GetValue();
	}

	for ( int32 i = 0; i < JEWEL_BINGO_MATCHING_SIZE; ++i )
	{
		pMsg.matching_jewel[i] = pPlayer->GetJewelBingoMatching(i)->GetValue();
	}
	
	pPlayer->SEND_PCT(pMsg);
}

void JewelBingo::SortJewels(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	int32 total_jewel = 0;
	int32 total_slot = 0;

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		total_jewel += pPlayer->GetJewelBingoJewel(i)->GetValue();
	}

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		if ( i == 12 )
		{
			continue;
		}

		if ( pPlayer->GetJewelBingoGrid(i)->GetValue() == uint8(-1) )
		{
			++total_slot;
		}
	}

	if ( total_jewel != total_slot )
	{
		return;
	}

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		if ( pPlayer->GetJewelBingoJewel(i)->GetValue() <= 0 )
		{
			continue;
		}

		for ( int32 h = 0; h < pPlayer->GetJewelBingoJewel(i)->GetValue(); ++h )
		{
			uint8 slot = -1;
			while(slot == uint8(-1))
			{
				slot = Random(JEWEL_BINGO_GRID_SIZE);

				if ( slot == 12 )
				{
					slot = -1;
					continue;
				}

				if ( pPlayer->GetJewelBingoGrid(slot)->GetValue() != uint8(-1) )
				{
					slot = -1;
					continue;
				}

				pPlayer->GetJewelBingoGrid(slot)->SetValue(i);
			}
		}

		pPlayer->GetJewelBingoJewel(i)->SetValue(0);
	}
}

void JewelBingo::GenerateRandomJewel(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	RandomValue<uint8> m_RandomValue(-1);

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		if ( pPlayer->GetJewelBingoJewel(i)->GetValue() > 0 )
		{
			m_RandomValue.AddValue(i, 0);
		}
	}

	uint8 random_jewel = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);

	if ( random_jewel != uint8(-1) )
	{
		pPlayer->GetJewelBingoJewel(random_jewel)->ReduceValue(1);
		pPlayer->SetJewelBingoCurrentJewel(random_jewel);
	}
}

bool JewelBingo::CheckMatching(Player* pPlayer, uint8 slot, bool result)
{
	if ( !pPlayer )
	{
		return true;
	}

	if ( slot >= JEWEL_BINGO_GRID_SIZE )
	{
		return true;
	}

	bool return_result = false;

	if ( this->CheckMatchingV(pPlayer, slot, result) )
	{
		return_result = true;
	}

	if ( this->CheckMatchingH(pPlayer, slot, result) )
	{
		return_result = true;
	}
	
	if ( this->CheckMatchingD1(pPlayer, slot, result) )
	{
		return_result = true;
	}

	if ( this->CheckMatchingD2(pPlayer, slot, result) )
	{
		return_result = true;
	}

	return return_result;
}

bool JewelBingo::CheckMatchingV(Player* pPlayer, uint8 slot, bool result)
{
	if ( !pPlayer )
	{
		return true;
	}

	if ( slot >= JEWEL_BINGO_GRID_SIZE )
	{
		return true;
	}

	///- Vertical son slots de a 5
	int32 count = 0;
	bool special = false;
	
	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		if ( i == 12 )
		{
			continue;
		}

		if ( pPlayer->GetJewelBingoGrid(i)->GetValue() < JEWEL_BINGO_JEWEL_COUNT )
		{
			continue;
		}

		if ( (slot % 5) != (i % 5) )
		{
			continue;
		}

		++count;
	}

	if ( (slot % 5) == 2 )
	{
		++count;
		special = true;
	}

	if ( count == 5 )
	{
		if ( !result )
		{
			pPlayer->GetJewelBingoMatching((slot % 5))->SetValue(1);
		
			if ( special )
			{
				pPlayer->IncreaseJewelBingoScore1(sGameServer->GetJewelBingoScoreSpecialV());
			}
			else
			{
				pPlayer->IncreaseJewelBingoScore2(sGameServer->GetJewelBingoScoreV());
			}
		}

		return true;
	}

	return false;
}
	
bool JewelBingo::CheckMatchingH(Player* pPlayer, uint8 slot, bool result)
{
	if ( !pPlayer )
	{
		return true;
	}

	if ( slot >= JEWEL_BINGO_GRID_SIZE )
	{
		return true;
	}

	///- Horizontal son slots de corrido
	int32 count = 0;
	bool special = false;
	
	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		if ( i == 12 )
		{
			continue;
		}

		if ( pPlayer->GetJewelBingoGrid(i)->GetValue() < JEWEL_BINGO_JEWEL_COUNT )
		{
			continue;
		}

		if ( (slot / 5) != (i / 5) )
		{
			continue;
		}

		++count;
	}

	if ( (slot / 5) == 2 )
	{
		++count;
		special = true;
	}

	if ( count == 5 )
	{
		if ( !result )
		{
			pPlayer->GetJewelBingoMatching((slot / 5) + 5)->SetValue(1);
		
			if ( special )
			{
				pPlayer->IncreaseJewelBingoScore1(sGameServer->GetJewelBingoScoreSpecialH());
			}
			else
			{
				pPlayer->IncreaseJewelBingoScore2(sGameServer->GetJewelBingoScoreH());
			}
		}

		return true;
	}

	return false;
}
	
bool JewelBingo::CheckMatchingD1(Player* pPlayer, uint8 slot, bool result)
{
	if ( !pPlayer )
	{
		return true;
	}

	if ( slot >= JEWEL_BINGO_GRID_SIZE )
	{
		return true;
	}

	if ( slot != 0 && slot != 6 && slot != 18 &&  slot != 24 )
	{
		return false;
	}

	///- Diagonal son slots de a 6 si comienza en 0
	int32 count = 0;
	bool special = false;

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		if ( i != 0 && i != 6 && i != 18 &&  i != 24 )
		{
			continue;
		}

		if ( pPlayer->GetJewelBingoGrid(i)->GetValue() < JEWEL_BINGO_JEWEL_COUNT )
		{
			continue;
		}

		++count;
	}

	if ( count != 4 )
	{
		return false;
	}

	++count;
	special = true;

	if ( count == 5 )
	{
		if ( !result )
		{
			pPlayer->GetJewelBingoMatching(10)->SetValue(1);
		
			pPlayer->IncreaseJewelBingoScore1(sGameServer->GetJewelBingoScoreD());
		}

		return true;
	}

	return false;
}

bool JewelBingo::CheckMatchingD2(Player* pPlayer, uint8 slot, bool result)
{
	if ( !pPlayer )
	{
		return true;
	}

	if ( slot >= JEWEL_BINGO_GRID_SIZE )
	{
		return true;
	}

	if ( slot != 4 && slot != 8 && slot != 16 &&  slot != 20 )
	{
		return false;
	}

	///- Diagonal son slots de a 4 si comienza en 4
	int32 count = 0;
	bool special = false;
	
	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		if ( i != 4 && i != 8 && i != 16 &&  i != 20 )
		{
			continue;
		}

		if ( pPlayer->GetJewelBingoGrid(i)->GetValue() < JEWEL_BINGO_JEWEL_COUNT )
		{
			continue;
		}

		++count;
	}

	if ( count != 4 )
	{
		return false;
	}

	++count;
	special = true;

	if ( count == 5 )
	{
		if ( !result )
		{
			pPlayer->GetJewelBingoMatching(11)->SetValue(1);

			pPlayer->IncreaseJewelBingoScore1(sGameServer->GetJewelBingoScoreD());
		}

		return true;
	}
	
	return false;
}

void JewelBingo::GenerateNotMatchedScore(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		if ( i == 12 )
		{
			continue;
		}

		if ( pPlayer->GetJewelBingoGrid(i)->GetValue() < JEWEL_BINGO_JEWEL_COUNT )
		{
			continue;
		}

		if ( !this->CheckMatching(pPlayer, i, true) )
		{
			pPlayer->IncreaseJewelBingoScore3(sGameServer->GetJewelBingoScoreNotMatched());
		}
	}
}

JewelBingoReward const* JewelBingo::GetReward(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return nullptr;
	}

	int32 score = pPlayer->GetJewelBingoScore1() + pPlayer->GetJewelBingoScore2() + pPlayer->GetJewelBingoScore3();

	for ( JewelBingoRewardList::const_iterator itr = this->reward_list.begin(); itr != this->reward_list.end(); ++itr )
	{
		JewelBingoReward const* pReward = *itr;

		if ( !pReward )
		{
			continue;
		}

		if ( score >= pReward->GetScoreMin() && score <= pReward->GetScoreMax() )
		{
			return pReward;
		}
	}

	return nullptr;
}