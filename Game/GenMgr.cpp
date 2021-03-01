/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GenMgr.cpp"
*
*/

CGenMgr::CGenMgr()
{
	this->SetRewardDate(0);
}

CGenMgr::~CGenMgr()
{
	LIST_CLEAR(GenRewardDataList::iterator, this->m_GenRewardDataList);
	LIST_CLEAR(GenRankingDataList::iterator, this->m_GenRankingDataList);
	LIST_CLEAR(GenKillPointsList::iterator, this->m_GenKillPointsList);
}

void CGenMgr::LoadGenReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Gen Reward...");

	LIST_CLEAR(GenRewardDataList::iterator, this->m_GenRewardDataList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM gen_reward");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			GenRewardData * reward = new GenRewardData;

			reward->SetFamily(fields[0].GetUInt8());
			reward->SetLevel(fields[1].GetUInt8());
			reward->SetType(fields[2].GetUInt8());
			reward->raw.data[0] = fields[3].GetUInt32();
			reward->raw.data[1] = fields[4].GetUInt32();
			reward->raw.data[2] = fields[5].GetUInt32();
			reward->raw.data[3] = fields[6].GetUInt32();

			if (reward->GetType() >= GEN_REWARD_MAX)
			{
				delete reward;
				continue;
			}

			this->m_GenRewardDataList.push_back(reward);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u gen rewards", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CGenMgr::LoadGenRanking()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Gen Ranking...");

	LIST_CLEAR(GenRankingDataList::iterator, this->m_GenRankingDataList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM gen_ranking AS i ORDER BY i.family ASC, i.level DESC");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			if (fields[2].GetUInt8() < GEN_RANKING_MAX)
			{
				GenRankingData * ranking = new GenRankingData;

				ranking->SetFamily(fields[0].GetUInt8());
				ranking->SetLevel(fields[1].GetUInt8());
				ranking->SetType(fields[2].GetUInt8());
				ranking->SetValue(fields[3].GetInt32());

				this->m_GenRankingDataList.push_back(ranking);
			}

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u gen rankings", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void CGenMgr::LoadGenKillPoints()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Gen Kill Point...");

	LIST_CLEAR(GenKillPointsList::iterator, this->m_GenKillPointsList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM gen_kill_point");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			GenKillPoints * points = new GenKillPoints;

			points->SetType(fields[0].GetUInt8());
			points->SetMin(fields[1].GetInt16());
			points->SetMax(fields[2].GetInt16());

			for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
			{
				points->SetPointsAdd(i, fields[3 + i].GetInt32());
			}

			for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
			{
				points->SetPointsDec(i, fields[8 + i].GetInt32());
			}

			this->m_GenKillPointsList.push_back(points);

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u gen kill points", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

GenRewardData const* CGenMgr::GetReward(uint8 family, uint8 level) const
{
	for (GenRewardDataList::const_iterator itr = this->m_GenRewardDataList.begin(); itr != this->m_GenRewardDataList.end(); ++itr)
	{
		GenRewardData const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetFamily() != family || 
			pData->GetLevel() != level)
		{
			continue;
		}

		return pData;
	}

	return nullptr;
}

GenRankingData const* CGenMgr::GetRanking(uint8 family, uint8 level) const
{
	for (GenRankingDataList::const_iterator itr = this->m_GenRankingDataList.begin(); itr != this->m_GenRankingDataList.end(); ++itr)
	{
		GenRankingData const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetFamily() != family ||
			pData->GetLevel() != level)
		{
			continue;
		}

		return pData;
	}

	return nullptr;
}
	
GenKillPoints const* CGenMgr::GetKillPoints(int16 level_1, int16 level_2) const
{
	int16 diff = level_1 - level_2;
	uint8 type = diff < 0 ? 1: 0;

	for (GenKillPointsList::const_iterator itr = this->m_GenKillPointsList.begin(); itr != this->m_GenKillPointsList.end(); ++itr)
	{
		GenKillPoints const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		if (pData->GetMin() > abs(diff))
		{
			continue;
		}

		if (pData->GetMax() < abs(diff))
		{
			continue;
		}
	
		return pData;
	}

	return nullptr;
}

uint8 CGenMgr::GetRankingLevel(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return GEN_LEVEL_PRIVATE;
	}

	bool general_rank = true;
	uint8 level = GEN_LEVEL_PRIVATE;

	for (GenRankingDataList::const_iterator itr = this->m_GenRankingDataList.begin(); itr != this->m_GenRankingDataList.end(); ++itr)
	{
		GenRankingData const* pData = *itr;

		if (!pData || pData->GetFamily() != pPlayer->GetGen()->GetFamily() || pData->GetType() != 0)
		{
			continue;
		}

		if (pPlayer->GetGen()->GetContribution() < pData->GetValue())
		{
			general_rank = false;
			break;
		}

		level = pData->GetLevel() - 1;
	}

	if (!general_rank || pPlayer->GetGen()->GetRanking() <= 0)
	{
		return level;
	}

	for (GenRankingDataList::const_iterator itr = this->m_GenRankingDataList.begin(); itr != this->m_GenRankingDataList.end(); ++itr)
	{
		GenRankingData const* pData = *itr;

		if (!pData || pData->GetFamily() != pPlayer->GetGen()->GetFamily() || pData->GetType() != 1)
		{
			continue;
		}

		if (pPlayer->GetGen()->GetRanking() > pData->GetValue())
		{
			break;
		}

		level = pData->GetLevel() - 1;
	}

	return level;
}