/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterMgr.h"
*
*/

#ifndef MONSTER_TEMPLATE_H
#define MONSTER_TEMPLATE_H

struct monster_template
{
	DECLARE(uint16, id);
	DECLARE_STRING_FIXED(Name, MAX_MONSTER_NAME_LENGTH);
	DECLARE(uint16, model);
	DECLARE(float, size);
	DECLARE(uint8, type);
	DECLARE(int16, min_level);
	DECLARE(int16, max_level);
	DECLARE(int32, power[POWER_MAX]);
	DECLARE(int32, attack_min_damage);
	DECLARE(int32, attack_max_damage);
	DECLARE(int32, magic_min_damage);
	DECLARE(int32, magic_max_damage);
	DECLARE(int32, critical_damage_rate);
	DECLARE(int32, critical_damage_add);
	DECLARE(int32, excellent_damage_rate);
	DECLARE(int32, excellent_damage_add);
	DECLARE(int32, attack_success);
	DECLARE(int32, defense);
	DECLARE(int32, defense_magic);
	DECLARE(int32, defense_success);
	DECLARE(uint32, move_range);
	DECLARE(uint32, move_speed);
	DECLARE(uint32, attack_range);
	DECLARE(int32, attack_speed);
	DECLARE(uint32, view_range);
	DECLARE_PROPERTY_ARRAY(uint8, Resistance, Element::MAX);
	DECLARE(uint32, respawn_time_min);
	DECLARE(uint32, respawn_time_max);
	DECLARE(int32, item_rate);
	DECLARE(int32, zen_rate);
	DECLARE(int32, item_max_level);
	float regen_power[POWER_MAX];
	DECLARE(uint32, regen_time[POWER_MAX]);
	DECLARE(uint8, faction);
	DECLARE(uint8, faction_level);
	DECLARE_ENUM(uint8, ElementalAttribute);
	DECLARE_ENUM(int32, ElementalPattern);
	DECLARE_ENUM(int32, ElementalDefense);
	DECLARE_ENUM(int32, ElementalDamageMin);
	DECLARE_ENUM(int32, ElementalDamageMax);
	DECLARE_ENUM(int32, ElementalAttackRate);
	DECLARE_ENUM(int32, ElementalDefenseRate);
	DECLARE_PROPERTY_STRING(ScriptName);
	DECLARE_ENUM(uint8, RadianceImmune);
	DECLARE_ENUM(int32, DebuffResistance);
	DECLARE_ENUM(int32, DebuffDefense);
	DECLARE_ENUM(uint8, CriticalDamageResistance);
	DECLARE_ENUM(uint8, ExcellentDamageResistance);
	DECLARE_ENUM(uint8, DamageAbsorb);
	DECLARE_BOOL(Elite);

	DECLARE(bool, custom);
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
	DECLARE_ENUM(uint8, Type);
	DECLARE_STRING_FIXED(Name, MAX_MONSTER_NAME_LENGTH);
	DECLARE_ENUM(world_type, World);
	DECLARE_ENUM(coord_type, X1);
	DECLARE_ENUM(coord_type, Y1);
	DECLARE_ENUM(coord_type, X2);
	DECLARE_ENUM(coord_type, Y2);
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

struct monster_event
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(world_type, World);
	DECLARE_ENUM(coord_type, X1);
	DECLARE_ENUM(coord_type, Y1);
	DECLARE_ENUM(coord_type, X2);
	DECLARE_ENUM(coord_type, Y2);
	DECLARE_ENUM(uint8, Direction);
	DECLARE_ENUM(uint32, SpawnDelay);
	DECLARE_ENUM(uint8, SpawnDistance);
	DECLARE_ENUM(uint32, RespawnTime);
	DECLARE_ENUM(uint32, RespawnID);
	DECLARE_ENUM(uint8, MoveDistance);
	DECLARE_ENUM(uint8, EventID);
	DECLARE_PROPERTY_STRING(NpcFunction);
	DECLARE_PROPERTY_STRING(ItemBag);
	DECLARE_PROPERTY_STRING(ScriptName);
	DECLARE_ENUM(uint32, AIGroup);
	DECLARE_ENUM(uint32, AIGroupMember);

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
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
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
	DECLARE_ENUM(world_type, World);
	DECLARE_ENUM(coord_type, X1);
	DECLARE_ENUM(coord_type, Y1);
	DECLARE_ENUM(coord_type, X2);
	DECLARE_ENUM(coord_type, Y2);
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

typedef std::map<uint16, monster_template*> MonsterTemplateMap;
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

typedef std::list<monster_event*> MonsterEventList;

typedef std::map<uint16, monster_respawn*> MonsterRespawnMap;

typedef std::vector<monster_exclusive*> MonsterExclusiveList;

class CMonsterMgr
{
	SingletonInstance(CMonsterMgr);

	public:
		MonsterTemplateMap map_template;
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
		MonsterEventList monster_event_list;
		MonsterRespawnMap monster_respawn_map;
		MonsterExclusiveList monster_exclusive_list;

		DECLARE_ENUM(time_t, LastUpdate);
	public:
		CMonsterMgr();
		virtual ~CMonsterMgr();

		void LoadMonsterTemplate();
		void LoadMonsterTemplateCustom();
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
		void LoadMonsterEvent(uint8 event_id);
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
		monster_template * GetMonsterTemplate(uint16 id) const;
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
};

bool AllowItemDropInWorld(world_type world, Item const& item);

#endif