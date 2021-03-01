NumericBaseball::NumericBaseball()
{
	this->SetActive(true);
}

NumericBaseball::~NumericBaseball()
{
	LIST_CLEAR(NumericBaseballRewardList::iterator, this->m_NumericBaseballRewardList);
}

void NumericBaseball::LoadReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Numeric Baseball Reward...");

	LIST_CLEAR(NumericBaseballRewardList::iterator, this->m_NumericBaseballRewardList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_numeric_baseball_reward");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			NumericBaseballReward * pData = new NumericBaseballReward;

			pData->SetScoreMin(fields[0].GetInt32());
			pData->SetScoreMax(fields[1].GetInt32());
			pData->SetItem(fields[2].GetUInt16());

			this->m_NumericBaseballRewardList.push_back(pData);

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u numeric baseball reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void NumericBaseball::Open(Player* pPlayer)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	if (pPlayer->GetInterfaceState()->GetID() != InterfaceData::None && !sShopMgr->IsShop(pPlayer->GetInterfaceState()->GetID()))
	{
		return;
	}

	if (pPlayer->GetPersonalStore()->IsBusy())
	{
		pPlayer->SendMessageBox(0, "Numeric Baseball", "Can't play game while personal store is open.");
		return;
	}

	NUMERIC_BASEBALL_OPEN pMsg;
	pMsg.result = 1;
	pPlayer->SendPacket(&pMsg);
}

void NumericBaseball::Start(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer))
	{
		return;
	}

	if (!pPlayer->IsAuthorizationEnabled())
	{
		pPlayer->SendMessageBox(0, "AUTH", "You are not authorized for this action.");
		return;
	}

	if (!Player::IsPlayerTransactionFree(pPlayer))
	{
		pPlayer->SendMessageBox(1, "Numeric Baseball", "Can't play with an active transaction. \n Can't play with Personal Store open.");
		return;
	}

	if (!this->IsActive() || !sGameServer->IsNumericBaseballEnabled())
	{
		sLog->outError("numeric_baseball", "%s -- Disabled -- %s", __FUNCTION__, pPlayer->BuildLog().c_str());
		return;
	}

	if (pPlayer->GetNumericBaseballState() == 0)
	{
		if (sGameServer->IsNumericBaseballRequiredTicket())
		{
			uint8 ticket = pPlayer->ItemFind(ITEMGET(14, 216), 0, true);

			if (ticket == uint8(-1))
			{
				this->SendState(pPlayer, 0x0E);
				return;
			}
		}

		pPlayer->SetNumericBaseballState(1);

		pPlayer->SetNumericBaseballScore(0);
		pPlayer->ResetNumericBaseballNumber(0xFF);
		pPlayer->ResetNumericBaseballUsedNumber(0xFF);
		pPlayer->ResetNumericBaseballStrike(0);
		pPlayer->ResetNumericBaseballBall(0);
		pPlayer->SetNumericBaseballCurrentNumber(0);

		for (int32 i = 0; i < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++i)
		{
			uint8 number = 0xFF;

			do
			{
				number = Random<uint8>(NUMERIC_BASEBALL_MAX_NUMBER);

				for (int32 n = 0; n < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++n)
				{
					if (pPlayer->GetNumericBaseballNumber(n) == number)
					{
						number = 0xFF;
					}
				}
			} while (number == 0xFF);

			pPlayer->SetNumericBaseballNumber(i, number);
		}
	}

	for (int32 i = 0; i < (NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW); ++i)
	{
		if (pPlayer->GetNumericBaseballUsedNumber(i) == 0xFF)
		{
			break;
		}

		pPlayer->IncreaseNumericBaseballCurrentNumber(1);
	}

	this->SendState(pPlayer, 0);

	switch (pPlayer->GetNumericBaseballState())
	{
	case 0:
		{
		} break;

	case 1:
		{
			this->SendPlayInfo(pPlayer);
		} break;
		
	case 2:
		{
		} break;
	}
}

void NumericBaseball::SelectNumber(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	if (pPlayer->GetNumericBaseballState() != 1)
	{
		return;
	}

	POINTER_PCT_CHECK(NUMERIC_BASEBALL_NUMBER_SELECT, lpMsg, Packet, 0);

	if (pPlayer->GetNumericBaseballCurrentNumber() >= (NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW))
	{
		this->SendPlayInfo(pPlayer);
		return;
	}

	if (lpMsg->numbers[pPlayer->GetNumericBaseballCurrentNumber()] == 0xFF)
	{
		this->SendPlayInfo(pPlayer);
		return;
	}

	if (lpMsg->numbers[pPlayer->GetNumericBaseballCurrentNumber()] >= NUMERIC_BASEBALL_MAX_NUMBER)
	{
		this->SendPlayInfo(pPlayer);
		return;
	}

	pPlayer->SetNumericBaseballUsedNumber(pPlayer->GetNumericBaseballCurrentNumber(), lpMsg->numbers[pPlayer->GetNumericBaseballCurrentNumber()]);
	pPlayer->IncreaseNumericBaseballCurrentNumber(1);

	int32 matched = 0;
	int32 current_row = 0;

	if ((pPlayer->GetNumericBaseballCurrentNumber() % NUMERIC_BASEBALL_NUMBERS_PER_ROW) == 0)
	{
		current_row = (pPlayer->GetNumericBaseballCurrentNumber() / NUMERIC_BASEBALL_NUMBERS_PER_ROW) - 1;
		int32 start_loop = (pPlayer->GetNumericBaseballCurrentNumber() - NUMERIC_BASEBALL_NUMBERS_PER_ROW);

		pPlayer->SetNumericBaseballStrike(current_row, 0);
		pPlayer->SetNumericBaseballBall(current_row, 0);

		uint8 used_numbers[NUMERIC_BASEBALL_MAX_NUMBER];
		memset(used_numbers, 0, NUMERIC_BASEBALL_MAX_NUMBER);

		for (int32 i = 0; i < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++i)
		{
			if (pPlayer->GetNumericBaseballUsedNumber(i + start_loop) >= NUMERIC_BASEBALL_MAX_NUMBER)
			{
				continue;
			}

			if (used_numbers[pPlayer->GetNumericBaseballUsedNumber(i + start_loop)] == 1)
			{
				continue;
			}

			used_numbers[pPlayer->GetNumericBaseballUsedNumber(i + start_loop)] = 1;

			if (pPlayer->GetNumericBaseballNumber(i) == pPlayer->GetNumericBaseballUsedNumber(i + start_loop))
			{
				pPlayer->IncreaseNumericBaseballStrike(current_row, 1);
				++matched;
			}

			for (int32 n = 0; n < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++n)
			{
				if (i != n)
				{
					if (pPlayer->GetNumericBaseballNumber(i) == pPlayer->GetNumericBaseballUsedNumber(n + start_loop))
					{
						pPlayer->IncreaseNumericBaseballBall(current_row, 1);
					}
				}
			}
		}

		pPlayer->IncreaseNumericBaseballScore(pPlayer->GetNumericBaseballStrike(current_row) * sGameServer->GetNumericBaseballScoreStrike());
		pPlayer->IncreaseNumericBaseballScore(pPlayer->GetNumericBaseballBall(current_row) * sGameServer->GetNumericBaseballScoreBall());
	}

	this->SendPlayInfo(pPlayer);

	if (matched >= NUMERIC_BASEBALL_NUMBERS_PER_ROW)
	{
		pPlayer->IncreaseNumericBaseballScore(sGameServer->GetNumericBaseballScore(current_row));

		pPlayer->SetNumericBaseballState(2);

		this->SendPlayResult(pPlayer, 3);
	}
	else if ((current_row + 1) >= NUMERIC_BASEBALL_ROWS)
	{
		pPlayer->SetNumericBaseballState(2);

		this->SendPlayResult(pPlayer, 2);
	}

	if (pPlayer->GetNumericBaseballState() == 2)
	{
		if (sGameServer->IsNumericBaseballReward())
		{
			NumericBaseballReward const* pReward = this->GetReward(pPlayer);

			if (pReward)
			{
				Item item(pReward->GetItem());
				item.SetExpireDate(time(nullptr) + DAY);

				sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
			}
		}

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_NUMERIC_BASEBALL);
		stmt->setUInt32(0, pPlayer->GetGUID());
		stmt->setInt32(1, pPlayer->GetNumericBaseballScore());
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);

		pPlayer->SetNumericBaseballState(0);
	}
}

void NumericBaseball::SendState(Player* pPlayer, uint8 result)
{
	NUMERIC_BASEBALL_START pMsg;
	pMsg.result = result;

	pPlayer->SendPacket(&pMsg);
}

void NumericBaseball::SendPlayInfo(Player* pPlayer)
{
	NUMERIC_BASEBALL_OPEN pMsg;
	pMsg.result = 2;
	pMsg.score = pPlayer->GetNumericBaseballScore();

	for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
	{
		pMsg.strikes[i] = pPlayer->GetNumericBaseballStrike(i);
		pMsg.ball[i] = pPlayer->GetNumericBaseballBall(i);
	}
	
	for (int32 i = 0; i < (NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW); ++i)
	{
		pMsg.numbers[i] = pPlayer->GetNumericBaseballUsedNumber(i);
	}

	pPlayer->SendPacket(&pMsg);
}

void NumericBaseball::SendPlayResult(Player* pPlayer, uint8 result)
{
	NUMERIC_BASEBALL_RESULT pMsg;
	pMsg.result = result;
	pMsg.data4 = result;

	for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
	{
		pMsg.strikes[i] = pPlayer->GetNumericBaseballStrike(i);
		pMsg.ball[i] = pPlayer->GetNumericBaseballBall(i);
	}

	for (int32 i = 0; i < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++i)
	{
		pMsg.numbers[i] = pPlayer->GetNumericBaseballNumber(i);
	}

	pMsg.score = pPlayer->GetNumericBaseballScore();

	pPlayer->SendPacket(&pMsg);
}

NumericBaseballReward const* NumericBaseball::GetReward(Player* pPlayer) const
{
	for (NumericBaseballRewardList::const_iterator itr = this->m_NumericBaseballRewardList.begin(); itr != this->m_NumericBaseballRewardList.end(); ++itr)
	{
		NumericBaseballReward const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pPlayer->GetNumericBaseballScore() >= pData->GetScoreMin() && pPlayer->GetNumericBaseballScore() <= pData->GetScoreMax())
		{
			return pData;
		}
	}

	return nullptr;
}