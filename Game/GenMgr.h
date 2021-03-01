/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GenMgr.h"
*
*/

#ifndef GEN_MGR_H
#define GEN_MGR_H

enum GenReward
{
	GEN_REWARD_ITEM,
	GEN_REWARD_ZEN,
	GEN_REWARD_EXP,
	GEN_REWARD_CONTRIBUTION,
	GEN_REWARD_MAX
};

enum GenRanking
{
	GEN_RANKING_CONTRIBUTION,
	GEN_RANKING_GENERAL_RANK,
	GEN_RANKING_MAX
};

struct GenRewardData
{
	DECLARE_ENUM(uint8, Family);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint8, Type);

	union
	{
		struct
		{
			uint32 type;
			uint32 index;
			uint32 level;
			uint32 count;
		} item;

		struct
		{
			uint32 value;
		} zen;

		struct
		{
			uint32 value;
		} experience;

		struct
		{
			uint32 value;
		} contribution;

		struct
		{
			uint32 data[4];
		} raw;
	};

	uint16 GetItem() const { return ITEMGET(this->item.type,this->item.index); }
};

struct GenRankingData
{
	DECLARE_ENUM(uint8, Family);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(int32, Value);
};

struct GenKillPoints
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(int16, Min);
	DECLARE_ENUM(int16, Max);
	DECLARE_PROPERTY_ARRAY(int32, PointsAdd, MAX_PARTY_MEMBERS);
	DECLARE_PROPERTY_ARRAY(int32, PointsDec, MAX_PARTY_MEMBERS);
};

typedef std::vector<GenRewardData*> GenRewardDataList;
typedef std::vector<GenRankingData*> GenRankingDataList;
typedef std::vector<GenKillPoints*> GenKillPointsList;

class CGenMgr
{
	SingletonInstance(CGenMgr);

	public:
		CGenMgr();
		virtual ~CGenMgr();

		void LoadGenReward();
		void LoadGenRanking();
		void LoadGenKillPoints();
		
		GenRewardData const* GetReward(uint8 family, uint8 level) const;
		GenRankingData const* GetRanking(uint8 family, uint8 level) const;
		GenKillPoints const* GetKillPoints(int16 level_1, int16 level_2) const;

		uint8 GetRankingLevel(Player* pPlayer) const;
	private:
		GenRewardDataList m_GenRewardDataList;
		GenRankingDataList m_GenRankingDataList;
		GenKillPointsList m_GenKillPointsList;
		DECLARE_ENUM(time_t, RewardDate);
};

#endif