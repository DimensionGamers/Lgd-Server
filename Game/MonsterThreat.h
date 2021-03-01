/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterThreat.h"
*
*/

#ifndef MONSTER_THREAT_H
#define MONSTER_THREAT_H

#define MONSTER_THREAT_VALUE_MAX			100000000000

enum MonsterThreatDamageType
{
	MONSTER_THREAT_DAMAGE_NORMAL,
	MONSTER_THREAT_DAMAGE_REFLECT,
	MONSTER_THREAT_DAMAGE_DEBUFF,
	MONSTER_THREAT_DAMAGE_ELEMENTAL,
	MONSTER_THREAT_DAMAGE_MAX
};

struct DamageData
{
	friend class MonsterThreat;

	explicit DamageData(uint32 guid, const char * name, MonsterThreatDamageType type, int64 value)
	{
		this->SetGuid(guid);
		this->ResetDamage(0);
		this->SetFirstHit(time(nullptr));
		this->AddDamage(name, type, value);
	}

	void AddDamage(const char * name, MonsterThreatDamageType type, int64 value)
	{
		this->ResetName();
		this->SetName(name);
		this->IncreaseDamage(type, value);
		this->SetLastAttackTime(GetTickCount());
	}

	int64 GetTotalDamage() const
	{
		int64 damage = 0;

		for ( int32 i = 0; i < MONSTER_THREAT_DAMAGE_MAX; ++i )
		{
			damage += this->GetDamage(i);
		}

		return damage;
	}
	
	DECLARE_ENUM(uint32, Guid);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_PROPERTY_ARRAY(int64, Damage, MONSTER_THREAT_DAMAGE_MAX);
	DECLARE_ENUM(uint32, LastAttackTime);
	DECLARE_ENUM(time_t, FirstHit);
};

struct ThreatData
{
	friend class MonsterThreat;

	explicit ThreatData(uint32 guid, int64 value, Player* pPlayer)
	{
		this->SetGuid(guid);
		this->SetThreat(value);
		this->SetPlayer(pPlayer);
	}

	void AddThreat(int64 value) 
	{ 
		this->IncreaseThreat(value);
		this->LimitMaxThreat(MONSTER_THREAT_VALUE_MAX);
	} 

	DECLARE_ENUM(uint32, Guid);
	DECLARE_PROPERTY(int64, Threat);
	DECLARE_PTR(Player, Player);
};

///- TODO: Rebuild a map con char id
typedef std::vector<DamageData*> DamageDataList;
typedef std::map<uint32, ThreatData*> ThreatDataMap;

class MonsterThreat
{
	public:
		friend class Monster;

		void Create(Monster* pOwner);
		void AddThreat(Player* pVictim, int64 threat);
		void AddDamage(Player* pVictim, MonsterThreatDamageType type, int64 damage);

		void Update();
		void UpdateThreat();

		void ClearThreat();
		void ClearDamage();

		DECLARE_PROPERTY_PTR(Monster, Owner);
		DECLARE_PROPERTY(uint32, MostHatedID);
		DECLARE_ENUM(int64, HighestThreat);

		DamageDataList const& GetDamageData() const { return damage_list; }
		ThreatDataMap const& GetThreatData() const { return m_ThreatMap; }

		bool IsInThreat(Unit* pTarget);
		bool IsUsingThreat() const;

		DamageData* GetDamageData(Player* pPlayer);
		DamageData* GetDamageData(uint32 char_id);

		ThreatData* GetThreatData(uint32 id);
	private:
		DamageDataList damage_list;
		ThreatDataMap m_ThreatMap;
};

#endif