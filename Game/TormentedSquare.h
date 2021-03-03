#ifndef TORMENTED_SQUARE_H
#define TORMENTED_SQUARE_H

struct TormentedSquareStage
{
	void Reset()
	{
		this->SetStandBy(0);
		this->SetDuration(0);
		this->SetScore(0);
	}

	DECLARE_ENUM(int32, StandBy);
	DECLARE_ENUM(int32, Duration);
	DECLARE_ENUM(int32, Score);
};

struct TormentedSquarePlayer
{
	TormentedSquarePlayer()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetPlayer(nullptr);
		this->SetGUID(0);
		this->SetLevel(0);
		this->ResetName();
		this->SetScore(0);
		this->SetQueueState(TORMENTED_SQUARE_QUEUE_STATE_NONE);
	}

	DECLARE_PTR(Player, Player);
	DECLARE_ENUM(uint32, GUID);
	DECLARE_ENUM(int32, Level);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_PROPERTY(int32, Score);
	DECLARE_ENUM(TormentedSquareQueueState, QueueState);
};

struct TormentedSquareTeam
{
	virtual ~TormentedSquareTeam()
	{
		this->ErasePlayer();
	}

	void Reset()
	{
		for ( int32 i = 0; i < this->GetPlayerSize(); ++i )
		{
			this->GetPlayer(i)->Reset();
		}

		this->SetPartyID(PARTY_NULL);
		this->SetInUse(false);
		this->SetQueued(false);
		this->SetQueueTime(GetTickCount());
		this->SetScore(0);
		this->SetLevel(0);
	}

	int32 GetPlayerCount() const
	{
		int32 count = 0;

		for ( int32 i = 0; i < this->GetPlayerSize(); ++i )
		{
			if ( this->GetPlayer(i)->GetGUID() )
			{
				++count;
			}
		}

		return count;
	}

	DECLARE_ENUM(uint16, PartyID);
	DECLARE_STRUCT_PTR(TormentedSquarePlayer, Player);
	DECLARE_BOOL(InUse);
	DECLARE_BOOL(Queued);
	DECLARE_ENUM(uint32, QueueTime);
	DECLARE_PROPERTY(int32, Score);
	DECLARE_ENUM(int32, Level);
};

#pragma pack(1)
struct TORMENTED_SQUARE_ROUND_DATA
{
	TORMENTED_SQUARE_ROUND_DATA()
	{
		this->h.set(0xDD, 0x05, sizeof(TORMENTED_SQUARE_ROUND_DATA));
		this->remain_time = 0;
		this->unk = 0;

		for ( int32 i = 0; i < TORMENTED_SQUARE_MAX_TEAM; ++i )
		{
			this->body[i] = 0;
		}

		this->my_ranking = 0;
	}
	PBMSG_HEAD2 h;
	uint16 remain_time;
	uint16 unk;
	uint32 body[TORMENTED_SQUARE_MAX_TEAM];
	uint8 my_ranking;
};

struct TORMENTED_SQUARE_RANKING_BODY
{
	uint8 ranking;
	char team_name[MAX_CHARACTER_LENGTH + 1];
	uint32 id;
};

struct TORMENTED_SQUARE_RANKING_HEAD
{
	PBMSG_HEAD2 h; // 0x06
	uint8 type;
	uint8 count;
};

struct TORMENTED_SQUARE_QUEUE
{
	PBMSG_HEAD2 h;
	uint8 result;
};

struct TORMENTED_SQUARE_ENTER
{
	PBMSG_HEAD2 h;
	uint8 ground;
	uint8 data[3];
};

struct TORMENTED_SQUARE_ENTER_RESULT
{
	TORMENTED_SQUARE_ENTER_RESULT(uint8 result, const char * team)
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE, SUBCODE_TORMENTED_SQUARE_ENTER, sizeof(TORMENTED_SQUARE_ENTER_RESULT));
		this->result = result;
		memset(this->team, 0, MAX_CHARACTER_LENGTH + 1);

		if ( team )
		{
			memcpy(this->team, team, MAX_CHARACTER_LENGTH);
		}
	}
	PBMSG_HEAD2 h;
	uint8 result;
	char team[MAX_CHARACTER_LENGTH + 1];
};

struct TORMENTED_SQUARE_QUEUE_RESULT
{
	TORMENTED_SQUARE_QUEUE_RESULT(uint8 result, const char * team)
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE, SUBCODE_TORMENTED_SQUARE_QUEUE, sizeof(TORMENTED_SQUARE_QUEUE_RESULT));
		this->result = result;
		memset(this->team, 0, MAX_CHARACTER_LENGTH + 1);

		if ( team )
		{
			memcpy(this->team, team, MAX_CHARACTER_LENGTH);
		}
	}
	PBMSG_HEAD2 h;
	uint8 result;
	char team[MAX_CHARACTER_LENGTH + 1];
};

struct TORMENTED_SQUARE_NOTIFICATION
{
	TORMENTED_SQUARE_NOTIFICATION(uint8 result)
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE, SUBCODE_TORMENTED_SQUARE_NOTIFICATION, sizeof(TORMENTED_SQUARE_NOTIFICATION));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct TORMENTED_SQUARE_CHAOS_GOBLIN
{
	TORMENTED_SQUARE_CHAOS_GOBLIN()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE, SUBCODE_TORMENTED_SQUARE_GOBLIN, sizeof(TORMENTED_SQUARE_CHAOS_GOBLIN));
	}
	PBMSG_HEAD2 h;
};

struct TORMENTED_SQUARE_TIME
{
	TORMENTED_SQUARE_TIME()
	{
		this->h.set(HEADCODE_TORMENTED_SQUARE, SUBCODE_TORMENTED_SQUARE_TIME, sizeof(TORMENTED_SQUARE_TIME));
		this->remain_time = 0;
		this->unk1 = 0;
		this->unk2 = 0;

		for ( int32 i = 0; i < TORMENTED_SQUARE_MAX_TEAM; ++i )
		{
			this->score[i] = 0;
		}
	}

	PBMSG_HEAD2 h;
	uint16 remain_time;
	uint16 unk1;
	uint32 score[TORMENTED_SQUARE_MAX_TEAM];
	uint32 unk2;
};

struct TORMENTED_SQUARE_TEAM_BODY
{
	uint32 id;
	char name[MAX_PARTY_MEMBERS][MAX_CHARACTER_LENGTH + 1];
	uint8 unk[5];
};

struct TORMENTED_SQUARE_TEAM_HEAD
{
	PWMSG_HEAD2 h;
	uint8 unk1[3]; // FF FF FF
	uint32 count; // 08 
	uint8 unk2; // 00 00 00 00
	uint8 unk3[3]; // FF FF FF
};

//SUBCODE_TORMENTED_SQUARE_FINAL_RANKING
#pragma pack()

class TormentedSquareGround
{
	public:
		TormentedSquareGround();
		virtual ~TormentedSquareGround();

		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Open);
		CREATE_STATE(Standby);
		CREATE_STATE(Playing);
		CREATE_STATE(End);

		void UpdateTeamQueue();
		void GetStageStandbyTime();
		void GetStageTime();
		int32 GetStageScore() const;
		void AddMonster();

		int32 PlayerGetCount() const;
		void PlayerUpdate();
		void PlayerAddScore(Player* pKiller, bool boss);
		void PlayerMoveOut();
		void SendPacket(uint8 * Packet, uint16 size);
		void SendTeamInfo();
		void SendNotification(uint8 id);
		void SendTime();

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
		void CalculateTeamScore();

		DECLARE_ARRAY_STRUCT(TormentedSquareStage, StageData, MAX_TORMENTED_SQUARE_ROUND);
		DECLARE_PROPERTY_ARRAY(uint16, Reward, TORMENTED_SQUARE_MAX_TEAM);

		DECLARE_ENUM(TormentedSquareGroundState, State);
		DECLARE_ENUM(uint8, Ground);
		DECLARE_PROPERTY(uint8, Stage);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT_PTR(TormentedSquareTeam, Team);
		DECLARE_ENUM(uint32, SecondTick);
		DECLARE_BOOL(RankingSent);

		DECLARE_ENUM(uint16, WorldID);
		DECLARE_ENUM(uint16, RespawnGate);
		DECLARE_ENUM(uint16, EnterGate);
};

class TormentedSquare
{
	SingletonInstance(TormentedSquare);

	public:
		TormentedSquare();
		virtual ~TormentedSquare();

		void Load();
		void LoadRanking();

		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result, const char * team = nullptr);
		void QueueResult(Player* pPlayer, uint8 * Packet);
		void SendQueueResult(Player* pPlayer, uint8 result, const char * team = nullptr);
		void QueueResultProcess(uint8 ground, TormentedSquareTeam * pTeam, TormentedSquareQueueState state);
		uint8 GetEnterGround(int16 level, bool master);

		void Start(int32 notify);
		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		void RemovePlayer(Player* pPlayer);
		bool IsIn(Player* pPlayer, bool add = true);

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

		DECLARE_ENUM(TormentedSquareState, State);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_ARRAY_STRUCT(TormentedSquareGround, Ground, MAX_TORMENTED_SQUARE_GROUND);
		DECLARE_ENUM(int32, CurrentMinute);
		DECLARE_ENUM(int32, NotifyTime);
};

#endif