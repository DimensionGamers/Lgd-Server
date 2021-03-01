/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ChaosCastle.h"
*
*/
#ifndef CHAOS_CASTLE_SURVIVAL_H
#define CHAOS_CASTLE_SURVIVAL_H

struct CHAOS_CASTLE_SURVIVAL_ENTER_RESULT
{
	CHAOS_CASTLE_SURVIVAL_ENTER_RESULT(uint8 result, uint16 remain_time)
	{
		this->h.set(HEADCODE_CHAOS_CASTLE, SUBCODE_CHAOS_CASTLE_SURVIVAL_ENTER, sizeof(CHAOS_CASTLE_SURVIVAL_ENTER_RESULT));
		this->result = result;
		this->remain_time = remain_time;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint16 remain_time;
};
#pragma pack(1)
struct CHAOS_CASTLE_SURVIVAL_RANKING_HEAD
{
	PWMSG_HEAD2 h;
	uint8 result;
	uint8 count;
};

struct CHAOS_CASTLE_SURVIVAL_RANKING_BODY
{
	uint32 rank;
	uint32 score;
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 unk2;
};
#pragma pack()
struct CHAOS_CASTLE_SURVIVAL_RANKING_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 type; // 1 = Preliminary, 2 = Final
};

struct CHAOS_CASTLE_SURVIVAL_SCHEDULE_BODY
{
	int32 hour;
	int32 minute;
	int32 preliminary;
	int32 semi_final;
	int32 final;
};

struct CHAOS_CASTLE_SURVIVAL_SCHEDULE_HEAD
{
	PWMSG_HEAD2 h;
	int32 count;
	uint8 preliminary[3];
	uint8 semi_final[2];
	uint8 final;
	uint8 type;
};

struct CHAOS_CASTLE_SURVIVAL_POSITION
{
	PBMSG_HEAD2 h;
	uint8 x;
	uint8 y;
};

struct CHAOS_CASTLE_SURVIVAL_SCORE
{
	CHAOS_CASTLE_SURVIVAL_SCORE()
	{
		this->h.set(HEADCODE_CHAOS_CASTLE, SUBCODE_CHAOS_CASTLE_SURVIVAL_SCORE, sizeof(CHAOS_CASTLE_SURVIVAL_SCORE));
		this->type = 0;
		this->result = 0;
		this->score = 0;
		this->monster_kill_count = 0;
		this->player_kill_count = 0;
	}

	PBMSG_HEAD2 h;
	int32 type;
	int32 result;
	int32 score;
	int32 monster_kill_count;
	int32 player_kill_count;
};

struct ChaosCastleSurvivalSchedule
{
	DECLARE_ENUM(uint8, Hour);
	DECLARE_ENUM(uint8, Minute);
};

typedef std::vector<ChaosCastleSurvivalSchedule*> ChaosCastleSurvivalScheduleList;

struct ChaosCastleSurvivalRanking
{
	ChaosCastleSurvivalRanking()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetID(0);
		this->ResetName();
		this->SetScore(0);
	}

	DECLARE_ENUM(uint32, ID);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(int32, Score);
};

typedef std::vector<ChaosCastleSurvivalRanking> ChaosCastleSurvivalRankingList;

class ChaosCastleSurvivalMgr: public EventCommonMgr
{
	SingletonInstance(ChaosCastleSurvivalMgr);

	public:
		ChaosCastleSurvivalMgr();
		~ChaosCastleSurvivalMgr();

		void LoadChaosCastleSetting();

		void Start(uint32 duration, int32 notify) override;
		void StartGround(uint32 duration);
		void Update();
	
		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		DECLARE_STRUCT(ChaosCastle, Ground);
		DECLARE_ENUM(ChaosCastleSurvivalStage, Stage);

		uint8 GetCurrentGround() const
		{
			if ( this->GetStage() == CHAOS_CASTLE_SURVIVAL_STAGE_NONE )
			{
				return 0;
			}

			return this->GetStage() - 1;
		}

		int32 GetCurrentRankSize() const
		{
			int32 size = MAX_CHAOS_CASTLE_SURVIVAL_RANK;

			switch ( this->GetStage() )
			{
			case CHAOS_CASTLE_SURVIVAL_STAGE_SEMI_FINAL:
				{
					size = MAX_CHAOS_CASTLE_SURVIVAL_RANK_SEMI_FINAL;
				} break;

			case CHAOS_CASTLE_SURVIVAL_STAGE_FINAL:
				{
					size = MAX_CHAOS_CASTLE_SURVIVAL_RANK_FINAL;
				} break;
			}

			return size;
		}
		
		bool IsInRanking(Player* pPlayer);
		ChaosCastleSurvivalStage GetDay() const;

		void DataRequest(Player* pPlayer, uint8 * Packet);
		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result, uint16 remain_time = 0);
		void RankingRequest(Player* pPlayer, uint8 * Packet);
		void Close(Player* pPlayer, uint8 * Packet);

		void PositionSet(Player* pPlayer, uint8 * Packet);
		void UpdateRanking();

		void AddScore(uint32 char_id, int32 score);
		void OnPlayerConnect(uint32 id, const char * name);

		DECLARE_PROPERTY_ARRAY(uint8, Preliminary, 3);
		DECLARE_PROPERTY_ARRAY(uint8, SemiFinal, 2);
		DECLARE_PROPERTY(uint8, Final);

		ChaosCastleSurvivalScheduleList m_StageTime[MAX_CHAOS_CASTLE_SURVIVAL_STAGE];

		DECLARE_ARRAY_STRUCT(ChaosCastleSurvivalRanking, Ranking, MAX_CHAOS_CASTLE_SURVIVAL_RANK);
};

#endif