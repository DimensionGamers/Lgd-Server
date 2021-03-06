/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterMgr.h"
*
*/

#ifndef MONSTER_TEMPLATE_H
#define MONSTER_TEMPLATE_H

struct MonsterTemplate
{
	uint16 Id;
	std::string Name;
	uint16 Model;
	float Size;
	uint8 Type;
	int16 Level;
	int32 Stat[POWER_MAX];
	int32 DamageMin;
	int32 DamageMax;
	int32 CriticalDamageRate;
	int32 CriticalDamageAdd;
	int32 ExcellentDamageRate;
	int32 ExcellentDamageAdd;
	int32 AttackSuccessRate;
	int32 Defense;
	int32 DefenseSuccessRate;
	uint32 MovementRange;
	uint32 MovementSpeed;
	uint32 AttackRange;
	int32 AttackSpeed;
	uint32 ViewRange;
	uint8 Resistance[Element::MAX];
	uint32 RespawnTimeMin;
	uint32 RespawnTimeMax;
	int32 ItemRate;
	int32 ZenRate;
	int32 ItemMaxLevel;
	float StatRecovery[POWER_MAX];
	uint32 StatRecoveryTime[POWER_MAX];
	uint8 ElementalAttribute;
	int32 ElementalDefense;
	int32 ElementalDamageMin;
	int32 ElementalDamageMax;
	int32 ElementalAttackSuccessRate;
	int32 ElementalDefenseSuccessRate;
	std::string ScriptName;
	uint8 RadianceImmune;
	int32 DebuffResistance;
	int32 DebuffDefense;
	uint8 CriticalDamageResistance;
	uint8 ExcellentDamageResistance;
	uint8 DamageAbsrob;
	bool IsElite;
	bool IsCustom;
};

struct monster_skill_special
{
	DECLARE_ENUM(uint16, Monster);
	DECLARE_ENUM(int16, Type);
	DECLARE_ENUM(uint16, Skill);
};

struct monster
{
	DECLARE_ENUM(uint16, GUID);
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
	DECLARE_ENUM(uint8, Direction);
	DECLARE_ENUM(uint32, SpawnDelay);
	DECLARE_ENUM(uint8, SpawnDistance);
	DECLARE_ENUM(uint32, RespawnTimeMin);
	DECLARE_ENUM(uint32, RespawnTimeMax);
	DECLARE_ENUM(uint32, RespawnID);
	DECLARE_ENUM(uint8, MoveDistance);
	DECLARE_PROPERTY_STRING(NpcFunction);
	DECLARE_PROPERTY_STRING(ItemBag);
	DECLARE_PROPERTY_STRING(ScriptName);
	DECLARE_ENUM(uint8, ElementalAttribute);
};

struct MonsterEvent
{
	uint16 MonsterId;
	uint16 MapId;
	int16 X1;
	int16 Y1;
	int16 X2;
	int16 Y2;
	uint8 Direction;
	uint32 SpawnDelay;
	uint8 SpawnDistance;
	uint32 RespawnTime;
	uint32 RespawnId;
	uint8 MovementDistance;
	uint8 EventId;
	std::string NpcFunction;
	std::string ItemBag;
	std::string ScriptName;
	uint32 AIGroup;
	uint32 AIGroupMember;

	union
	{
		struct
		{
			int32 ground;
			int32 type;
		} blood_castle;

		struct
		{
			int32 ground;
			int32 despawn_time;
			int32 boss;
		} devil_square;

		struct
		{
			int32 ground;
		} chaos_castle;

		struct
		{
			int32 day;
			int32 type;
			int32 stage;
			int32 id;
		} imperial_fortress;

		struct
		{
			int32 id;
			int32 group;
			int32 sub_group;
			int32 boss;
			int32 attack_percent;
		} invasion;

		struct
		{
			int32 score;
		} crywolf;

		struct
		{
			int32 state;
			int32 sub_state;
		} kanturu;

		struct
		{
			int32 ground;
			int32 zone;
			int32 despawn_time;
			int32 boss;
		} doppelganger;

		struct
		{
			int32 id;
		} castle_siege;

		struct
		{
			int32 ground;
			int32 stage;
			int32 boss;
		} tormented_square;

		struct
		{
			int32 stage;
			int32 boss;
			int32 score;
		} tormented_square_survival;

		struct
		{
			int32 stage;
			int32 type;
			int32 despawn_time;
		} castle_deep;

		struct
		{
			int32 id;
		} world_boss;

		struct
		{
			int32 id;
			int32 type;
		} dungeon;

		struct
		{
			int32 data[5];
		} raw;
	};
};

struct monster_ai_element
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, State);
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint8, Rate);
	DECLARE_ENUM(uint32, Delay);
	DECLARE_ENUM(int32, Target);
	DECLARE_ENUM(int16, X);
	DECLARE_ENUM(int16, Y);
};

struct monster_ai_state
{
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, Priority);
	DECLARE_ENUM(uint8, CurrentState);
	DECLARE_ENUM(uint8, NextState);
	DECLARE_ENUM(int32, TransitionType);
	DECLARE_ENUM(uint8, TransitionRate);
	DECLARE_ENUM(int32, TransitionValueType);
	DECLARE_ENUM(int32, TransitionValue);
	DECLARE_ENUM(int32, Delay);
};

struct monster_ai_automata
{
	DECLARE_PROPERTY(int32, ID);
	monster_ai_state m_AIState[MAX_AI_STATE][MAX_AI_PRIORITY];
	int32 m_AIStatCount[MAX_AI_STATE];

	explicit monster_ai_automata()
	{
		for ( int32 i = 0; i < MAX_AI_STATE; ++i )
			this->m_AIStatCount[i] = 0;
	}
};

struct monster_ai_unit
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint32, Delay);
	DECLARE_PTR(monster_ai_automata, Automata);
	
	DECLARE_ARRAY_STRUCT_PTR(monster_ai_element, AIClass, MONSTER_AI_ELEMENT_MAX);
};

struct AIData
{
	DECLARE_ENUM(uint32, Group);
	DECLARE_ENUM(uint32, ID);
	DECLARE_PROPERTY_ARRAY(uint32, AIOrder, MONSTER_MAX_AI_ORDER);
	DECLARE_PROPERTY_ARRAY(uint32, AITime, MONSTER_MAX_AI_ORDER);
};

struct monster_respawn_location
{
	DECLARE_ENUM(uint32, Group);
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
	DECLARE_ENUM(int8, Direction);
	DECLARE_ENUM(int32, Instance);
	DECLARE_ENUM(uint8, Rate);
};

struct monster_equipment
{
	DECLARE(uint8, slot);
	DECLARE(uint8, type);
	DECLARE(uint16, index);
	DECLARE(uint8, level);
	DECLARE(uint8, excellent_ancient);
};

struct monster_respawn
{
	DECLARE_ENUM(uint16, GUID);
	DECLARE_ENUM(time_t, Date);
};

struct monster_exclusive
{
	DECLARE_ENUM(uint16, Server);
	DECLARE_ENUM(uint16, GUID);
	DECLARE_ENUM(uint16, AvailableServer);
};

struct MonsterItem
{
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
};

typedef std::vector<uint16> MonsterSkillVector;
typedef UNORDERED_MAP<uint16, MonsterSkillVector> MonsterSkillMap;
typedef std::vector<monster_skill_special*> MonsterSkillSpecialList;
typedef UNORDERED_MAP<uint16, MonsterSkillSpecialList> MonsterSkillSpecialMap;

typedef std::vector<AIData*> MonsterAIGroupList;
typedef std::map<uint32, MonsterAIGroupList> MonsterAIGroupMap;

typedef std::map<uint16, monster*> MonsterMap;

typedef std::vector<MonsterItem*> MonsterItemList;
typedef std::map<uint16, MonsterItemList> MonsterItemMap;

typedef std::vector<monster_respawn_location*> MonsterRespawnLocationList;
typedef std::map<uint32, MonsterRespawnLocationList> MonsterRespawnLocationMap;

typedef std::vector<monster_equipment*> MonsterEquipmentList;
typedef std::map<uint16, MonsterEquipmentList> MonsterEquipmentMap;

typedef std::map<uint32, monster_ai_unit*> MonsterAIUnitMap;
typedef std::map<uint32, monster_ai_element*> MonsterAIElementMap;
typedef std::map<uint32, monster_ai_automata*> MonsterAIAutomataMap;

typedef std::map<uint16, monster_respawn*> MonsterRespawnMap;

typedef std::vector<monster_exclusive*> MonsterExclusiveList;

typedef std::unordered_multimap<uint8, MonsterEvent*>::iterator MonsterEventItr;

class MonsterManager
{
	SingletonInstance(MonsterManager);

	public:
		MonsterManager();
		virtual ~MonsterManager();

		void LoadMonsterTemplate();
		void LoadMonsterItems();
		void LoadMonsterSkill();
		void LoadMonsterSkillSpecial();
		void LoadMonsterAIElement();
		void LoadMonsterAIAutomata();
		void LoadMonsterAIUnit();
		void LoadMonsterAIGroup();
		void LoadMonsterRespawnLocation();
		void LoadMonsterEquipment();
		void LoadMonster();
		void LoadMonster(uint16 guid);
		void LoadMonsterEvent();
		void LoadMonsterRespawn();
		void LoadMonsterExclusive();
	
		void SpawnMonsters();
		bool IsMonsterExclusive(uint16 guid) const;

		void CreateItem(int32 level, int32 max_level);
		int32 GenerateItemLevel(item_template const* item_info, int32 level);
		bool InsertItem(int32 level, uint16 item, uint8 item_level);
		bool GenerateItem(Item & item, int32 level, bool exe);
		void ClearItem();
	public:
		MonsterTemplate const* GetMonsterTemplate(uint16 id) const;
		monster * GetMonsterData(uint16 index) const;

		MonsterSkillVector GetMonsterSkillMapBounds(uint16 monster) const
		{
			MonsterSkillMap::const_iterator it = this->monster_skill_map.find(monster);
			MonsterSkillVector result;

			if ( it != this->monster_skill_map.end() )
				result = it->second;

			return result;
		}

		MonsterSkillSpecialList GetMonsterSkillSpecial(uint16 monster) const
		{
			MonsterSkillSpecialMap::const_iterator it = this->monster_skill_special_map.find(monster);

			if ( it != this->monster_skill_special_map.end() )
				return it->second;

			MonsterSkillSpecialList empty;

			return empty;
		}

		MonsterAIGroupList GetMonsterAIGroup(uint32 group) const
		{
			MonsterAIGroupMap::const_iterator it = this->monster_ai_group_map.find(group);
			MonsterAIGroupList return_group;

			if ( it != this->monster_ai_group_map.end() )
				return_group = it->second;

			return return_group;
		}

		AIData const* GetMonsterAIData(uint32 group, uint32 member) const
		{
			MonsterAIGroupMap::const_iterator ai_list = this->monster_ai_group_map.find(group);

			if ( ai_list != this->monster_ai_group_map.end() )
			{
				for ( MonsterAIGroupList::const_iterator it = ai_list->second.begin(); it != ai_list->second.end(); ++it )
				{
					if ( (*it)->GetID() == member )
						return (*it);
				}
			}

			return nullptr;
		}

		monster_ai_element * GetMonsterAIElement(uint32 element) const
		{
			MonsterAIElementMap::const_iterator it = this->m_monster_ai_element.find(element);

			return it != this->m_monster_ai_element.end() ? it->second: nullptr;
		}

		monster_ai_unit * GetMonsterAIUnit(uint32 unit) const
		{
			MonsterAIUnitMap::const_iterator it = this->m_monster_ai_unit.find(unit);

			return it != this->m_monster_ai_unit.end() ? it->second: nullptr;
		}

		monster_ai_automata * GetMonsterAIAutomata(uint32 automata) const
		{
			MonsterAIAutomataMap::const_iterator it = this->m_monster_ai_automata.find(automata);

			return it != this->m_monster_ai_automata.end() ? it->second: nullptr;
		}

		void GenerateRespawnLocation(Monster* pMonster);
		monster_respawn_location const* GetRandomRespawnLocation(Monster* pMonster) const;
		bool TeleportToRandomLocation(Monster* pMonster, int32 attempts);
		void BuildMonsterCustomData(Monster* pMonster, Player* pPlayer);

		uint16 GetRandomSkillSpecial(Monster* pMonster, int16 type);

		std::pair<MonsterEventItr, MonsterEventItr> GetEventMonsters(uint8 eventId) { return _monsterEvent.equal_range(eventId); }
	private:
		std::unordered_map<uint16, MonsterTemplate> _monsterTemplate;
		MonsterSkillMap monster_skill_map;
		MonsterSkillSpecialMap monster_skill_special_map;
		MonsterMap monster_map;
		MonsterAIGroupMap monster_ai_group_map;
		MonsterItemMap m_monster_item_map;
		MonsterRespawnLocationMap m_monster_respawn_location_map;
		MonsterEquipmentMap m_monster_equipment_map;
		MonsterAIUnitMap m_monster_ai_unit;
		MonsterAIElementMap m_monster_ai_element;
		MonsterAIAutomataMap m_monster_ai_automata;
		std::unordered_multimap<uint8, MonsterEvent*> _monsterEvent;
		MonsterRespawnMap monster_respawn_map;
		MonsterExclusiveList monster_exclusive_list;

		DECLARE_ENUM(time_t, LastUpdate);
};

#define sMonsterManager MonsterManager::instance()

bool AllowItemDropInWorld(uint16 world, Item const& item);

#endif