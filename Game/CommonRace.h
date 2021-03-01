#ifndef COMMON_RACE_H
#define COMMON_RACE_H

enum CommonRaceState
{
	COMMON_RACE_STATE_NONE,
	COMMON_RACE_STATE_NOTIFY,
	COMMON_RACE_STATE_PLAYING,
};

struct CommonRaceGate
{
	explicit CommonRaceGate(int32 gate, int32 reward)
	{
		this->SetGate(gate);
		this->SetReward(reward);
	}

	DECLARE_ENUM(int32, Gate);
	DECLARE_ENUM(int32, Reward);
};

typedef std::vector<CommonRaceGate*> CommonRaceGateList;

struct CommonRacePlayer
{
	CommonRacePlayer()
	{
		this->SetStartedTime(time(nullptr));
		this->SetParticipating(false);
		this->GateList.clear();
	}

	std::vector<int32> GateList;
	DECLARE_BOOL(Participating);
	DECLARE_ENUM(time_t, StartedTime);
};

typedef std::map<uint32, CommonRacePlayer> CommonRacePlayerMap;

class CommonRace: public EventCommon
{
	public:
		CommonRace();
		virtual ~CommonRace();

		void Clear();
		void Start(int32 duration, int32 notify);

		virtual void Update();

		void SetState_None();
		void SetState_Notify();
		void SetState_Playing();

		void ProcState_None();
		void ProcState_Notify();
		void ProcState_Playing();

		void PlayerInGate(Player* pPlayer, int32 gate);
		void ResetPlayer(Player* pPlayer);
		
		DECLARE_ENUM(int32, Duration);
		CommonRaceGateList gate_list;
		CommonRacePlayerMap player_data;
		DECLARE_PROPERTY(size_t, GateReached);
		DECLARE_PROPERTY_STRING(RaceName);
};

#endif