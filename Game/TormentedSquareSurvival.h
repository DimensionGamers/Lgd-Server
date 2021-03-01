#ifndef TORMENTED_SQUARE_SURVIVAL_H
#define TORMENTED_SQUARE_SURVIVAL_H

struct TORMENTED_SQUARE_SURVIVAL_SCHEDULE
{
	TORMENTED_SQUARE_SURVIVAL_SCHEDULE()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_SCHEDULE, sizeof(TORMENTED_SQUARE_SURVIVAL_SCHEDULE));
		memset(this->preliminary, 0, 4);
		memset(this->semi_final, 0, 2);
		this->final = 0;
		this->current_day = 0;
		memset(this->preliminary_hour, -1, 10);
		memset(this->preliminary_minute, -1, 10);
		memset(this->semi_final_hour, -1, 10);
		memset(this->semi_final_minute, -1, 10);
		this->final_hour = -1;
		this->final_minute = -1;
		this->reward_start_day = 0;
		this->reward_end_day = 0;
	}

	PBMSG_HEAD2 h;
	uint8 preliminary[4];
	uint8 semi_final[2];
	uint8 final;
	uint8 current_day;
	uint8 preliminary_hour[10];
	uint8 preliminary_minute[10];
	uint8 semi_final_hour[10];
	uint8 semi_final_minute[10];
	uint8 final_hour;
	uint8 final_minute;
	uint8 reward_start_day;
	uint8 reward_end_day;
};

struct TORMENTED_SQUARE_SURVIVAL_ENTER
{
	TORMENTED_SQUARE_SURVIVAL_ENTER()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_ENTER_REQUEST, sizeof(TORMENTED_SQUARE_SURVIVAL_ENTER));
		this->result = 0;
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
		this->day = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 day;
};

struct TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT
{
	PBMSG_HEAD2 h;
	uint8 result;
};

struct TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT_RESULT
{
	TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT_RESULT()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT, sizeof(TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT_RESULT));
		this->result = 0;
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
	}

	PBMSG_HEAD2 h;
	uint8 result;
	char name[MAX_CHARACTER_LENGTH + 1];
};

struct TORMENTED_SQUARE_SURVIVAL_STATE
{
	TORMENTED_SQUARE_SURVIVAL_STATE()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_STATE, sizeof(TORMENTED_SQUARE_SURVIVAL_STATE));
		this->state = 0;
	}

	PBMSG_HEAD2 h;
	uint8 state;
};

struct TORMENTED_SQUARE_SURVIVAL_TEAM_HEAD
{
	PWMSG_HEAD2 h;
	int32 count;
	uint8 result;
};

struct TORMENTED_SQUARE_SURVIVAL_TEAM_BODY
{
	int32 team_index;
	char name1[MAX_CHARACTER_LENGTH + 1];
	char name2[MAX_CHARACTER_LENGTH + 1];
	int32 team_level;
};

struct TORMENTED_SQUARE_SURVIVAL_TIME
{
	TORMENTED_SQUARE_SURVIVAL_TIME()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_TIME, sizeof(TORMENTED_SQUARE_SURVIVAL_TIME));
		this->remain_time = 0;
		for ( int32 i = 0; i < 20; ++i )
		{
			this->team_score[i] = 0;
		}
		this->my_team = 0;
	}

	PBMSG_HEAD2 h;
	uint16 remain_time;
	int32 team_score[20];
	uint8 my_team;
};

struct TORMENTED_SQUARE_SURVIVAL_SCORE_RESULT
{
	TORMENTED_SQUARE_SURVIVAL_SCORE_RESULT()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_SCORE_RESULT, sizeof(TORMENTED_SQUARE_SURVIVAL_SCORE_RESULT));
		for ( int32 i = 0; i < 20; ++i )
		{
			this->team_score[i] = 0;
		}
	}

	PBMSG_HEAD2 h;
	int32 team_score[20];
};

#pragma pack(1)
struct TORMENTED_SQUARE_SURVIVAL_RANKING
{
	PBMSG_HEAD2 h;
	uint8 type;
};
#pragma pack()

struct TORMENTED_SQUARE_SURVIVAL_RANKING_BODY
{
	char name1[MAX_CHARACTER_LENGTH + 1];
	char name2[MAX_CHARACTER_LENGTH + 1];
	uint16 server_group1;
	uint16 server_group2;
	uint8 rank;
	uint8 type;
	int32 score;
	int32 enter_year;
	uint8 enter_month;
	uint8 enter_day;
};

struct TORMENTED_SQUARE_SURVIVAL_RANKING_HEAD
{
	PWMSG_HEAD2 h;
	uint8 result;
	uint8 count;
	uint8 type;
};

struct TormentedSquareSurvivalRanking
{
	TormentedSquareSurvivalRanking()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetID1(0);
		this->SetID2(0);
		this->ResetName1();
		this->ResetName2();
		this->SetRank(0);
		this->SetType(0);
		this->SetScore(0);
	}

	DECLARE_ENUM(uint32, ID1);
	DECLARE_ENUM(uint32, ID2);
	DECLARE_STRING_FIXED(Name1, MAX_CHARACTER_LENGTH + 1);
	DECLARE_STRING_FIXED(Name2, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(uint8, Rank);
	DECLARE_ENUM(uint8, Type);
	DECLARE_PROPERTY(int32, Score);
};

class TormentedSquareSurvivalGround
{
	public:
		TormentedSquareSurvivalGround();
		virtual ~TormentedSquareSurvivalGround();

		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Open);
		CREATE_STATE(Standby1);
		CREATE_STATE(Playing);
		CREATE_STATE(Standby2);
		CREATE_STATE(End);

		void UpdateTeamQueue();
		void GetStageStandbyTime();
		void GetStageTime();
		int32 GetStageScore() const;
		void AddMonster();

		int32 PlayerGetCount() const;
		void PlayerUpdate();
		void PlayerMoveOut();
		void SendPacket(uint8 * Packet, uint16 size);

		void SendTime();
		void SendTeamList();
		void SendState(Player* pPlayer, uint8 state);
		void AddTeamRoundScore();
		void SendScoreResult(Player* pPlayer = nullptr);
		void CalculateTeamPosition();
		
		bool SecondPassed()
		{
			if ( (GetTickCount() - this->GetSecondTick()) > IN_MILLISECONDS )
			{
				this->SetSecondTick(GetTickCount());
				return true;
			}

			return false;
		}

		uint8 GetEmptyTeam() const;

		DECLARE_ARRAY_STRUCT(TormentedSquareStage, StageData, MAX_TORMENTED_SQUARE_SURVIVAL_ROUND);
		DECLARE_PROPERTY_ARRAY(uint16, Reward, TORMENTED_SQUARE_MAX_TEAM);

		DECLARE_ENUM(TormentedSquareSurvivalGroundState, State);
		DECLARE_ENUM(uint8, Ground);
		DECLARE_PROPERTY(uint8, Stage);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT_PTR(TormentedSquareTeam, Team);
		DECLARE_ENUM(uint32, SecondTick);
		DECLARE_BOOL(RankingSent);

		DECLARE_ENUM(world_type, WorldID);
		DECLARE_ENUM(uint16, RespawnGate);
		DECLARE_ENUM(uint16, EnterGate);

		DECLARE_BOOL(StartMessageSent);
};

class TormentedSquareSurvival
{
	SingletonInstance(TormentedSquareSurvival);

	public:
		TormentedSquareSurvival();
		virtual ~TormentedSquareSurvival();

		void Load();
		void LoadRanking(uint8 type);

		TormentedSquareSurvivalStage GetDay() const;

		uint8 GetCurrentGround() const
		{
			if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_NONE || this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER )
			{
				return 0;
			}

			return this->GetStage() - 1;
		}

		void ScheduleRequest(Player* pPlayer, uint8 * Packet);
		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result, const char * name = nullptr);

		void EnterAccept(Player* pPlayer, uint8 * Packet);
		void EnterAcceptResult(Player* pPlayer, uint8 result, const char * name = nullptr);
		void EnterAcceptResult(Party* pParty, uint8 result, const char * name = nullptr);

		void RankingRequest(Player* pPlayer, uint8 * Packet);

		void RemoveTeam(Player* pPlayer);
		void RemoveTeam(Party* pParty);
		bool IsTeam(Party* pParty);
		TormentedSquareTeam * GetTeam(Party* pParty);
		bool SetTeamQueueOK(TormentedSquareTeam * pTeam, Player* pPlayer);
		bool IsTeamQueueComplete(TormentedSquareTeam * pTeam);
		
		bool IsSemiFinalTeam(Party* pParty) const;
		bool IsFinalTeam(Party* pParty) const;

		bool IsSemiFinalPlayer(Player* pPlayer) const;
		bool IsFinalPlayer(Player* pPlayer) const;
				
		void AddScore(Player* pPlayer, int32 score);
		void DropReward(Player* pPlayer, Monster* pMonster);

		void Start(int32 notify);
		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		void RemovePlayer(Player* pPlayer);
		bool IsIn(Player* pPlayer, bool add = true);
		void SendPacket(uint8 * Packet, uint16 size);

		void SendRemainMinutes(std::string const& message, int32 period, int32 max_minute = -1);

		bool IsMinuteElapsed(int32 period)
		{
			if ( !this->GetTime()->GetRemain() || (this->GetTime()->GetRemain() / (IN_MILLISECONDS * MINUTE)) == this->GetCurrentMinute() )
				return false;

			this->SetCurrentMinute(this->GetTime()->GetRemain() / (IN_MILLISECONDS * MINUTE));

			if ( (this->GetCurrentMinute() % period) != 0 )
				return false;

			if ( (this->GetCurrentMinute() + 1) <= 0 )
				return false;

			return true;
		}

		void OnPlayerConnect(uint32 id, const char * name);

		DECLARE_ENUM(TormentedSquareSurvivalStage, Stage);

		DECLARE_PROPERTY_ARRAY(uint8, Preliminary, 4);
		DECLARE_PROPERTY_ARRAY(uint8, SemiFinal, 2);
		DECLARE_PROPERTY(uint8, Final);

		DECLARE_ENUM(uint8, RewardDayStart);
		DECLARE_ENUM(uint8, RewardDayEnd);

		DECLARE_ENUM(TormentedSquareState, State);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TormentedSquareSurvivalGround, Ground);
		DECLARE_ENUM(int32, CurrentMinute);
		DECLARE_ENUM(int32, NotifyTime);

		DECLARE_ARRAY_STRUCT(TormentedSquareSurvivalRanking, RankingSemiFinal, MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL);
		DECLARE_ARRAY_STRUCT(TormentedSquareSurvivalRanking, RankingFinal, MAX_TORMENTED_SQUARE_RANKING_FINAL);
		DECLARE_STRUCT(TormentedSquareSurvivalRanking, RankingWinner);

		DECLARE_BOOL(RankingLoaded);
};

#endif