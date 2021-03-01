/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "SkillMgr.h"
*
*/

#ifndef SKILL_TEMPLATE_H
#define SKILL_TEMPLATE_H

struct skill_template
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE(uint16, level);
	DECLARE_ENUM(int32, Damage);
	DECLARE(int32, mana_cost);
	DECLARE(int32, stamina_cost);
	DECLARE_ENUM(uint8, EffectRange);
	DECLARE_ENUM(uint8, EffectRadio);
	DECLARE_ENUM(uint32, Cooldown);
	DECLARE_ENUM(int8, Type);
	DECLARE_ENUM(uint8, Attribute);
	DECLARE_ENUM(uint16, BuffIcon);
	DECLARE_ENUM(int16, RequiredLevel);
	DECLARE_PROPERTY_ARRAY(int32, RequiredStat, MAX_STAT_TYPE);
	DECLARE_ENUM(uint8, RequiredKillCount);
	DECLARE_PROPERTY_ARRAY(uint8, Status, 3);
	DECLARE_ENUM(uint16, BaseSkill);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredClass, Character::MAX_CLASS);
	DECLARE(uint8, animation);
	DECLARE_ENUM(uint8, ElementalAttribute);
	DECLARE_ENUM(uint16, ElementalBonusDamage);
	DECLARE_ENUM(uint16, LearnItem);
	DECLARE_ENUM(uint8, LearnItemLevel);
	DECLARE_ENUM(uint16, RequireSkill);
	DECLARE_ENUM(uint16, RequireMasterSkill);
	DECLARE_ENUM(uint8, RequireMasterLevel);
	DECLARE_ENUM(uint32, AttackDelay);
	DECLARE_ENUM(uint8, MountCheck);
	DECLARE_ENUM(uint8, PVMDamage);
	DECLARE_ENUM(uint8, PVPDamage);

	DECLARE_ENUM(int32, Stat1);
	DECLARE_ENUM(int32, StatValue1);
	DECLARE_ENUM(int32, Stat2);
	DECLARE_ENUM(int32, StatValue2);

	bool IsRequiredClass(uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const
	{
		if ( !this->GetRequiredClass(race) )
			return false;

		if ( this->GetRequiredClass(race) == 2 && change_up_1 == 0 )
			return false;

		if ( this->GetRequiredClass(race) == 3 && change_up_2 == 0 )
			return false;

		if ( this->GetRequiredClass(race) == 4 && change_up_3 == 0 )
			return false;

		return true;
	}

	bool IsValidStatus(uint8 status) const;

	bool IsValidKillCount(uint8 count) const
	{
		return count >= this->GetRequiredKillCount();
	}
};

struct SkillTree
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint8, Group);
	DECLARE_ENUM(uint8, RequiredPoints);
	DECLARE_ENUM(uint8, MaxLevel);
	DECLARE_ENUM(uint8, Link);
	DECLARE_PROPERTY_ARRAY(uint16, RequiredSkill, 2);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredSkillPoints, 2);
	DECLARE_ENUM(uint16, Skill);
	DECLARE_ENUM(uint16, RelatedSkill);
	DECLARE_ENUM(uint16, ReplaceSkill);
	DECLARE_ENUM(uint32, FormulaID);
	DECLARE_ENUM(uint8, Unk5);
	DECLARE_ENUM(uint8, Unk6);
	DECLARE_ENUM(uint8, Unk7);
	DECLARE_ENUM(uint8, Type);
	
	float GetValue(uint8 level);
	
	uint8 GetRank() const { return (((this->GetID() - 1) % 36) / 4); }
};

struct skill_special
{
	DECLARE(uint16,guid);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE(uint8,target);
	DECLARE(int32,scope_type);
	DECLARE(uint8,scope_value);
	DECLARE(int32,element[MONSTER_SKILL_ELEMENT_MAX]);
	DECLARE_ARRAY_STRUCT_PTR(skill_special_element const, Element, MONSTER_SKILL_ELEMENT_MAX); 
};

struct skill_special_element
{
	DECLARE(uint32, guid);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE(uint8, type);
	DECLARE(uint8, rate);
	DECLARE(int32, duration);
	DECLARE(uint8, calc_type);
	DECLARE(int32, calc_value);
	DECLARE(int32, magic);
};

struct buff_template
{
	DECLARE_ENUM(uint16, Buff);
	DECLARE_ENUM(uint16, Group);
	DECLARE_ENUM(uint16, Item);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_PROPERTY_STRING(Description);
	DECLARE_BOOL(Send);
	DECLARE_BOOL(Debuff);
	DECLARE_PROPERTY_ARRAY(uint8, Effect, MAX_BUFF_DATA);
	DECLARE_PROPERTY_ARRAY(float, Value, MAX_BUFF_DATA);
	DECLARE_ENUM(uint32, Duration);
};

struct skill_tree_majestic
{
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(uint8, Section);
	DECLARE_ENUM(uint8, UIGroup);
	DECLARE_ENUM(uint8, PositionTemp);
	DECLARE_ENUM(uint16, SkillID);
	DECLARE_ENUM(uint16, BuffID);
	DECLARE_ENUM(uint8, Rank);
	DECLARE_ENUM(uint16, FormulaID);
	DECLARE_PROPERTY_ARRAY(int32, LinkSlot, MAX_SKILL_TREE_MAJESTIC_LINK);
	DECLARE_PROPERTY_ARRAY(int32, LinkLine, MAX_SKILL_TREE_MAJESTIC_LINK);
	DECLARE_ENUM(uint8, RequiredPoints);
	DECLARE_ENUM(uint8, MaxPoints);
	DECLARE_ENUM(uint8, ActiveNextPoints);
	DECLARE_ENUM(uint16, ParentSkill1);
	DECLARE_ENUM(uint8, ParentSkillPoints1);
	DECLARE_ENUM(uint16, ParentSkill2);
	DECLARE_ENUM(uint8, ParentSkillPoints2);
	DECLARE_ENUM(uint16, ParentSkill3);
	DECLARE_ENUM(uint8, ParentSkillPoints3);

	DECLARE_ENUM(uint16, ParentID1);
	DECLARE_ENUM(uint16, ParentID2);
	DECLARE_ENUM(uint16, ParentID3);

	float GetValue(uint8 level);
};

struct skill_tree_majestic_stat
{
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint8, Section);
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint16, SkillID);
	DECLARE_PROPERTY_ARRAY(uint8, PointCondition, MAX_SKILL_TREE_MAJESTIC_STAT_STEP);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredPoints, MAX_SKILL_TREE_MAJESTIC_STAT_STEP);
	DECLARE_PROPERTY_ARRAY(uint8, MaxPoints, MAX_SKILL_TREE_MAJESTIC_STAT_STEP);
	DECLARE_PROPERTY_ARRAY(uint8, StartPoint, MAX_SKILL_TREE_MAJESTIC_STAT_GROUP);
	DECLARE_PROPERTY_ARRAY(uint8, EndPoint, MAX_SKILL_TREE_MAJESTIC_STAT_GROUP);
	DECLARE_PROPERTY_ARRAY(uint16, FormulaID, MAX_SKILL_TREE_MAJESTIC_STAT_GROUP);
		
	uint8 GetCurrentStep(uint8 level) const
	{
		uint8 step = 0;
		for (int32 i = 1; i < MAX_SKILL_TREE_MAJESTIC_STAT_STEP; ++i)
		{
			if (level >= this->GetMaxPoints(i - 1))
			{
				step = i;
			}
		}

		return step;
	}

	float GetValue(uint8 level);
};

struct SkillAttackTime
{
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint16, Skill);
	DECLARE_ENUM(uint8, Step);
	DECLARE_RANGE(int32, Speed);
	DECLARE_ENUM(uint32, Time);
	DECLARE_ENUM(uint32, RealTime);
};

typedef std::map<uint16, skill_template*> SkillTemplateMap;
typedef std::vector<SkillTree*> SkillTreeList;
typedef std::map<uint16, buff_template*> BuffTemplateMap;
typedef std::map<uint16, skill_special*> SkillSpecialMap;
typedef std::map<uint32, skill_special_element*> SkillSpecialElementMap;
typedef std::vector<skill_tree_majestic*> SkillTreeMajesticList;
typedef std::vector<skill_tree_majestic_stat*> SkillTreeMajesticStatList;
typedef std::vector<SkillAttackTime*> SkillAttackTimeList;

class SkillMgr
{
	SingletonInstance(SkillMgr);

	private:
		SkillTemplateMap skill_template_map;
		SkillTreeList m_SkillTreeList;
		SkillSpecialMap skill_special_map;
		SkillSpecialElementMap skill_special_element_map;
		BuffTemplateMap buff_template_map;
		SkillTreeMajesticList skill_tree_majestic_list;
		SkillTreeMajesticStatList skill_tree_majestic_stat_list;
		SkillAttackTimeList m_SkillAttackTimeList;

	public:
		SkillMgr();
		virtual ~SkillMgr();

		void LoadSkillTemplate();
		void LoadSkillLearn();
		void LoadSkillTree();
		void LoadSkillSpecial();
		void LoadSkillSpecialElement();
		void LoadBuffTemplate();
		void LoadSkillRequire();
		void LoadSkillTreeMajestic();
		void LoadSkillTreeMajesticStat();
		void LoadSkillAttackTime();
	
		bool IsSkill(uint16 skill) const;
		skill_template * GetSkill(uint16 skill);
		skill_template const* GetSkill(uint16 skill) const;
		
		skill_template const* GetSkillFromItem(uint16 item, uint8 level) const;
		SkillTree const* GetSkillTree(uint16 skill, uint8 character) const;
		SkillTree* GetSkillTree(uint16 skill, uint8 character);
		buff_template const* GetBuff(uint16 buff) const;
		buff_template const* GetBuffItem(uint16 item) const;
		skill_special const* GetSkillSpecial(uint16 guid) const;
		skill_special_element const* GetSkillSpecialElement(uint32 guid) const;

		uint8 GetSkillAttribute(uint16 skill) const;
		uint16 GetSkillBaseSkill(uint16 skill) const;
		int8 GetSkillType(uint16 skill) const;
		int32 GetSkillDamage(uint16 skill) const;
		uint16 GetSkillBuffIcon(uint16 skill) const;
		uint8 GetSkillElementalAttribute(uint16 skill) const;
		uint16 GetSkillElementalBonus(uint16 skill) const;
		bool IsSkillRequiredClass(uint16 skill, uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const;

		bool IsSkillRequire(Player* pPlayer, uint16 skill) const;

		skill_tree_majestic const* GetSkillTreeMajestic(uint8 race, uint8 section, uint16 id) const;
		skill_tree_majestic const* GetSkillTreeMajestic(uint8 race, uint16 skill) const;

		skill_tree_majestic * GetSkillTreeMajestic(uint8 race, uint8 section, uint16 id);
		skill_tree_majestic * GetSkillTreeMajestic(uint8 race, uint16 skill);

		skill_tree_majestic_stat const* GetSkillTreeMajesticStat(uint8 race, uint8 section, uint8 id) const;
		skill_tree_majestic_stat * GetSkillTreeMajesticStat(uint8 race, uint8 section, uint8 id);

		SkillAttackTime const* GetSkillAttackTime(Player* pPlayer, uint16 skill) const;
};

#endif