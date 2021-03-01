/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ImperialFortress.h"
*
*/
#ifndef IMPERIAL_FORTRESS_H
#define IMPERIAL_FORTRESS_H

class ImperialFortressTrap
{
	public:
		friend class ImperialFortressMgr;

		ImperialFortressTrap()
		{
			this->SetX1(0);
			this->SetY1(0);
			this->SetX2(0);
			this->SetY2(0);
			this->SetElement(0);
			this->SetDamage(0.0f);
		}

		bool IsInTrap(coord_type x, coord_type y) const
		{
			return ( x >= this->GetX1() && x <= this->GetX2() &&
					 y >= this->GetY1() && y <= this->GetY2() );
		}

	private:
		DECLARE_ENUM(coord_type, X1);
		DECLARE_ENUM(coord_type, Y1);
		DECLARE_ENUM(coord_type, X2);
		DECLARE_ENUM(coord_type, Y2);
		DECLARE_ENUM(uint8, Element);
		DECLARE_FLOAT(Damage);
};

struct ImperialFortressMonsterLevel
{
	DECLARE(uint16, monster);
	DECLARE(int16, level);
	DECLARE(int32, power[POWER_MAX]);
	DECLARE(int32, attack_min);
	DECLARE(int32, attack_max);
	DECLARE(int32, attack_rate);
	DECLARE(int32, defense);
	DECLARE(int32, defense_rate);
	DECLARE_PROPERTY_STRING(ItemBag);
};

typedef std::vector<ImperialFortressMonsterLevel*> ImperialFortressMonsterLevelList;

struct ImperialFortressLevel
{
	ImperialFortressLevel()
	{
		this->monsters.clear();
	}
	DECLARE(uint32, guid);
	DECLARE_ENUM(int16, LevelMin);
	DECLARE_ENUM(int16, LevelMax);
	DECLARE(int32, experience);

	ImperialFortressMonsterLevel* GetMonster(uint16 monster) const
	{
		for ( ImperialFortressMonsterLevelList::const_iterator it = this->monsters.begin(); it != this->monsters.end(); ++it )
		{
			if ( (*it)->monster.Is(monster) )
			{
				return (*it);
			}
		}

		return nullptr;
	}

	ImperialFortressMonsterLevelList monsters;
};

typedef std::vector<ImperialFortressTrap*> ImperialFortressTrapList;
typedef std::map<uint8, ImperialFortressTrapList> ImperialFortressTrapMap;

typedef std::map<uint32,ImperialFortressLevel*> ImperialFortressLevelMap;

struct ImperialFortressCharacterInstance
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_ENUM(uint32, Instance);
};

typedef std::map<uint32, ImperialFortressInstance*> ImperialFortressMap;
typedef std::vector<ImperialFortressCharacterInstance*> ImperialFortressCharacterInstanceList;

class ImperialFortressMgr
{
	SingletonInstance(ImperialFortressMgr);
	friend class ImperialFortressZone;

	public:
		ImperialFortressMgr();
		virtual ~ImperialFortressMgr();

		void LoadImperialFortressLevel();
		void LoadImperialFortressMonsterSettings();
		void LoadImperialFortressTraps();

		void EnterRequest(Player* pPlayer);
		void EnterResult(Player* pPlayer, uint8 result, uint8 day = 0, uint8 state = 0, uint8 weather = 0, uint32 time = 0);
		uint32 EnterPartyLevel(Player* pPlayer);
		
		uint8 GetDayOfWeek();
		bool FindTicket(Player* pPlayer, uint8 day, bool consume);

		void Update();
		
		bool UpdatePlayerGate(Player* pPlayer, uint16 gate);

		ImperialFortressPlayer* FindPlayer(Player* pPlayer);
		ImperialFortressPlayer* FindPlayer(uint8 zone, Player* pPlayer);
		void UpdatePlayer(uint32 instance, Player* pPlayer);

		ImperialFortressLevel * GetLevel(uint32 guid) const;

		ImperialFortressInstance * GetInstance(uint32 id) const;
		ImperialFortressInstance * GetInstanceByParty(uint16 id) const;
		ImperialFortressInstance * GetInstanceByCharacter(uint32 id) const;
		ImperialFortressInstance * CreateInstance();

		void RemoveInstance(uint32 id);
		void RemoveInstanceByCharacter(uint32 id);

		void AddCharacterInstance(uint32 id, Player* pPlayer);
		
		ImperialFortressTrapList const& GetTrapList(uint8 day) const;

		void Stop();
	private:
		
		ImperialFortressLevelMap m_level;
		ImperialFortressTrapMap m_ImperialFortressTrapMap;

		DECLARE_PROPERTY(uint32, InstanceID);
		ImperialFortressMap m_ImperialFortressMap;
		ImperialFortressCharacterInstanceList m_ImperialFortressCharacterInstanceList;
};

#endif