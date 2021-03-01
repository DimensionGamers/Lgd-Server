/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Invasion.h"
*
*/
#ifndef INVASION_H
#define INVASION_H

enum InvasionFlags
{
	INVASION_FLAG_FOLLOW_BOSS		= 1 << 0,
	INVASION_FLAG_PROTECT_BOSS		= 1 << 1,
	INVASION_FLAG_OFFATTACK_DAMAGE	= 1 << 2,
};

enum InvasionState
{
	INVASION_STATE_NONE			= 0,
	INVASION_STATE_NOTIFY		= 1,
	INVASION_STATE_START		= 2,
	INVASION_STATE_END			= 4,
	INVASION_STATE_MAX
};

struct InvasionGroupDB
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_ENUM(uint8, Rate);
	DECLARE_FLAG(uint32, Flag);
	DECLARE_ENUM(world_type, World);
	DECLARE_ENUM(coord_type, X1);
	DECLARE_ENUM(coord_type, Y1);
	DECLARE_ENUM(coord_type, X2);
	DECLARE_ENUM(coord_type, Y2);
};

typedef std::list<InvasionGroupDB*> InvasionGroupListDB;
typedef std::map<uint32, InvasionGroupListDB> InvasionGroupMapDB;

struct InvasionDataDB
{
	DECLARE(uint32,invasion);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_PROPERTY_STRING(OnNotify);
	DECLARE_PROPERTY_STRING(OnStart);
	DECLARE_PROPERTY_STRING(OnEnd);
	DECLARE_PROPERTY_STRING(OnTimeOut);
	DECLARE_PROPERTY_STRING(OnKillBoss);

	InvasionGroupMapDB groups;
};

typedef std::map<uint32, InvasionDataDB*> InvasionDataMapDB;

class Invasion
{
	friend class InvasionMgr;

	public:
		struct MonsterData
		{
			explicit MonsterData(Monster* pMonster, uint8 boss, uint8 aggresive, uint8 attack_percent)
			{
				mMonster = pMonster;
				this->boss.set(boss);
				this->aggresive.set(aggresive);
				this->attack_percent.set(attack_percent);
			}
			Monster * mMonster;
			DECLARE(uint8,boss);
			DECLARE(uint8,aggresive);
			DECLARE(uint8,attack_percent);
		};

		typedef std::map<Monster*, MonsterData*> MonsterDataMap;

		explicit Invasion(uint32 id, uint32 group, uint32 sub_group);
		virtual ~Invasion();

		void Clear();
		void GenerateRespawnLocation();
		void AddMonsters();

		MonsterData * monsterGet(Monster* mMonster) const
		{
			MonsterDataMap::const_iterator it = this->monsters.find(mMonster);

			return (it != this->monsters.end() ? it->second: nullptr);
		}

		MonsterDataMap GetMonsterMap() const { return this->monsters; }
	private:
		DECLARE_ENUM(uint32, ID);
		DECLARE_ENUM(uint32, Group);
		DECLARE_ENUM(uint32, SubGroup);
		MonsterDataMap monsters;
		DECLARE_ENUM(coord_type, X);
		DECLARE_ENUM(coord_type, Y);
};

typedef std::vector<Invasion*> InvasionList;

class InvasionData
{
	friend class InvasionMgr;

	public:
		explicit InvasionData(uint32 id, uint32 time, int32 notify);
		virtual ~InvasionData();

		void UpdateTime();

		void SetState_None();
		void SetState_Notify();
		void SetState_Start();
		void SetState_End();

		void ProcState_None();
		void ProcState_Notify();
		void ProcState_Start();
		void ProcState_End();

		void RunTime(uint32 time)
		{
			this->SetUpdateTick(GetTickCount());
			this->SetUpdateTime(time);
		}

		bool TimePassed() const
		{
			return (GetTickCount() - this->GetUpdateTick()) > this->GetUpdateTime();
		}

		Invasion* GetInvasion(uint32 id, uint32 group, uint32 sub_group) const
		{
			for ( InvasionList::const_iterator it = this->invasion_list.begin(); it != this->invasion_list.end(); ++it )
			{
				if ( !*it )
					continue;

				if ( (*it)->GetID() == id &&
					 (*it)->GetGroup() == group &&
					 (*it)->GetSubGroup() == sub_group )
				{
					return *it;
				}
			}

			return nullptr;
		}
	private:
		DECLARE(uint32, id);
		DECLARE(uint32, time);
		DECLARE(int32, notify);
		DECLARE_ENUM(uint32, UpdateTick);
		DECLARE_ENUM(uint32, UpdateTime);
		DECLARE_ENUM(InvasionState, State);

		InvasionList invasion_list;
};

typedef std::map<uint32, InvasionData*> InvasionDataMap;

class InvasionMgr
{
	SingletonInstance(InvasionMgr);

	public:
		InvasionMgr();
		virtual ~InvasionMgr();

		void ClearData();

		void LoadInvasionData();
		void LoadInvasionGroup();
		
		void LaunchInvasion(uint32 invasion, uint32 time, int32 notify);

		InvasionDataDB * GetInvasionDataDB(uint32 invasion) const;
		InvasionGroupDB const* GetInvasionGroupDB(uint32 invasion, uint32 group, uint32 sub_group) const;
		bool IsInvasionActive(uint32 invasion) const;

		void UpdateTime();
		void StopInvasion(uint32 invasion);
		void StopInvasion();

		InvasionData* GetInvasionData(uint32 invasion) const
		{
			InvasionDataMap::const_iterator it = this->m_invasion.find(invasion);

			if ( it != this->m_invasion.end() )
				return it->second;

			return nullptr;
		}
	private:
		InvasionDataMapDB m_invasion_map;

		InvasionDataMap m_invasion;
};

#endif