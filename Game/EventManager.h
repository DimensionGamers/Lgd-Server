/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "EventManager.h"
*
*/

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

struct event_data
{
	DECLARE_ENUM(uint8, EventID);
	DECLARE_ENUM(uint32, Invasion);
	DECLARE_ENUM(uint32, Duration);
	DECLARE_ENUM(int32, NotifyTime);
	DECLARE_ENUM(uint8, Hour);
	DECLARE_ENUM(uint8, Minute);
	DECLARE_ENUM(uint8, DayOfWeek);
	DECLARE_ENUM(uint8, DayOfMonth);
	DECLARE_ENUM(uint8, SeasonEvent);

	uint32 ConvertToMinutes() const { return (this->GetHour() * MINUTE + this->GetMinute()); }
};

struct EventSeasonData
{
	DECLARE_PROPERTY_STRING(Name);

	DECLARE_ENUM(time_t, StartDate);
	DECLARE_ENUM(time_t, EndDate);
	DECLARE_ENUM(uint32, Ocurrence);
	DECLARE_ENUM(uint32, Length);

	/*DECLARE_ENUM(uint8, StartHour);
	DECLARE_ENUM(uint8, StartMinute);
	DECLARE_ENUM(uint8, StartDay);
	DECLARE_ENUM(uint8, StartMonth);

	DECLARE_ENUM(uint8, EndHour);
	DECLARE_ENUM(uint8, EndMinute);
	DECLARE_ENUM(uint8, EndDay);
	DECLARE_ENUM(uint8, EndMonth);*/

	DECLARE_PROPERTY_STRING(StartMessage);
	DECLARE_PROPERTY_STRING(EndMessage);

	/*DECLARE_ENUM(uint32, Start);
	DECLARE_ENUM(uint32, End);*/
	DECLARE_BOOL(Started);

	/*bool IsIn(uint32 current)
	{
		uint32 start = this->GetStart();
		uint32 end = this->GetEnd();

		if ( end < start )
		{
			return (current >= start) || (current <= end);
		}
			
		return (current >= start) && (current <= end);
	}*/
};

struct event_contribution_reward
{
	DECLARE_ENUM(uint8, EventID);
	DECLARE_ENUM(uint8, EventGround);
	DECLARE_ENUM(int32, ContributionMin);
	DECLARE_ENUM(int32, ContributionMax);
	DECLARE_ENUM(uint16, RewardBox);
};

struct event_level
{
	DECLARE_ENUM(uint8, EventID);
	DECLARE_ENUM(uint8, EventGround);
	DECLARE_ENUM(int16, NormalLevelMin);
	DECLARE_ENUM(int16, NormalLevelMax);
};

struct EventEnterCount
{
	DECLARE_ENUM(uint8, EventID);
	DECLARE_ENUM(uint8, Count);
};

typedef std::vector<event_data*> EventDataList;
typedef std::map<uint8, EventSeasonData*> EventSeasonDataMap;
typedef std::list<event_contribution_reward*> EventContributionRewardList;
typedef std::list<event_level*> EventLevelList;
typedef std::map<uint8, EventEnterCount*> EventEnterCountMap;

class CEventMgr
{
	SingletonInstance(CEventMgr);

	public:
		CEventMgr();
		virtual ~CEventMgr();

		void LoadEventData();
		void LoadEventSeasonData();
		void LoadEventContributionReward();
		void LoadEventLevel();
		void LoadEventEnterCount();

		void Update();
		void UpdateSeason();

		void Start(uint8 event_id, uint32 duration, int32 notify, uint32 invasion);
		bool IsEventOn(uint8 event_id, uint32 invasion);

		int32 GetNextRemainTime(uint8 event_id);

		//bool GetEventRemainTimeData(uint8 event_id, uint32 & remain_time, uint8 & players_count, uint8 race, int16 level, bool master);
		bool GetEventRemainTimeData(uint8 event_id, uint32 & remain_time, uint8 & players_count, Player* pPlayer);

		EventSeasonData * GetSeasonData(uint8 season_event) const;
		bool IsSeasonEventOn(uint8 season_event);

		void GiveContributionReward(Player* pPlayer, uint8 event_id, uint8 event_ground, int32 contribution, uint8 gremory_type);

		void AddEventScore(uint32 guid, const char * name, uint8 event_id, uint8 event_ground, int32 score);

		uint8 GetEventLevel(EventID event_id, Player* pPlayer) const;
		bool IsEventGroundLevel(EventID event_id, Player* pPlayer, int16 level_min, int16 level_max) const;

		void BuildChaosCastleSurvivalSchedule();
		void BuildTormentedSquareSurvivalSchedule(uint8 * hour, uint8 * minute, uint8 type);

		void GetEventStage(uint8 event_id);

		uint8 GetEventEnterCount(uint8 event_id) const;
		bool CanEnterEvent(Player* pPlayer, uint8 event_id) const;
		void EventEnter(Player* pPlayer, uint8 event_id);

		void CastleSiegeKill(Player* pPlayer01, Player* pPlayer02);
		void ArkaWarKill(Player* pPlayer01, Player* pPlayer02);
		void CastleSiegeTime(Player* pPlayer, uint8 type);
	private:
		EventDataList m_event_list;
		EventSeasonDataMap m_season_data_map;
		EventContributionRewardList m_event_contribution_reward;
		EventLevelList m_event_level_list;
		EventEnterCountMap m_event_enter_count_map;
		DECLARE_STRUCT(TickTimer, UpdateTime);
};

#endif