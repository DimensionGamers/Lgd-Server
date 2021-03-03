/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerCharacterBase.h"
*
*/

#ifndef PLAYER_CHARACTER_BASE_H
#define PLAYER_CHARACTER_BASE_H

struct CharacterBaseData
{
	DECLARE_PROPERTY_ARRAY(uint32, Stat, MAX_STAT_TYPE);

	DECLARE_ENUM(int32, Life);
	DECLARE_ENUM(int32, Mana);
	DECLARE_FLOAT(LevelToLife);
	DECLARE_FLOAT(LevelToMana);
	DECLARE_FLOAT(VitalityToLife);
	DECLARE_FLOAT(EnergyToMana);

	DECLARE_ENUM(uint16, World);

	DECLARE_ENUM(int16, Level);
	DECLARE_ENUM(int32, Points);

	DECLARE_PROPERTY_ARRAY(float, AutomaticPowerRecovery, POWER_MAX);

	DECLARE_ENUM(uint32, LevelUpPoints);
	DECLARE_ENUM(uint32, PlusLevelUpPoints);
	DECLARE_ENUM(uint32, MasterLevelUpPoints);
	DECLARE_ENUM(uint32, MajesticLevelUpPoints);
};

struct character_helper
{
	DECLARE_ENUM(int32, Stage);
	DECLARE_ENUM(uint32, Duration);
	DECLARE_ENUM(uint32, Cost);
	DECLARE_ENUM(uint32, CostInterval);
	DECLARE_ENUM(int32, NextStage);
};

struct character_formula
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, Class);
	DECLARE_FLOAT(LevelMul);
	DECLARE_FLOAT(LevelDiv);
	DECLARE_PROPERTY_ARRAY(float, StatMul, MAX_STAT_TYPE);
	DECLARE_PROPERTY_ARRAY(float, StatDiv, MAX_STAT_TYPE);
};

struct character_stat_fruit
{
	DECLARE_ENUM(int16, MinLevel);
	DECLARE_ENUM(int16, MaxLevel);
	DECLARE_ENUM(uint16, TotalAddPoints);
	DECLARE_ENUM(uint16, TotalMinusPoints);
};

struct character_item
{
	DECLARE_ENUM(uint8, Slot);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint8, Durability);
	DECLARE_ENUM(uint8, Skill);
	DECLARE_ENUM(uint8, Luck);
	DECLARE_ENUM(uint8, Option);
	DECLARE_ENUM(uint8, Excellent);
	DECLARE_ENUM(uint8, Ancient);
	DECLARE_ENUM(time_t, Duration);

	uint16 GetItem() const { return ITEMGET(this->GetType(), this->GetIndex()); }
};

struct CharacterExperience
{
	DECLARE_ENUM(uint16, Level);
	DECLARE_ENUM(int64, Experience);
	DECLARE_ENUM(int64, AccumulatedExperience);
};

struct character_specialization
{
	DECLARE_ENUM(uint8, Race);
	DECLARE_ENUM(uint8, Stat);
	DECLARE_ENUM(uint8, RequiredStat);
	DECLARE_ENUM(int32, RequiredStatMin);
	DECLARE_ENUM(int32, RequiredStatMax);
	DECLARE_ENUM(uint8, PercentMin);
	DECLARE_ENUM(uint8, PercentMax);
	DECLARE_ENUM(int32, RequiredStatLimit);
};

struct character_experience_adjust
{
	DECLARE_ENUM(int16, LevelMin);
	DECLARE_ENUM(int16, LevelMax);
	DECLARE_ENUM(int16, ExperienceRate);
};

struct character_experience_bonus
{
	DECLARE_PROPERTY_ARRAY(uint8, Race, 3);
};

struct character_bonus
{
	DECLARE_ENUM(int16, LevelMin);
	DECLARE_ENUM(int16, LevelMax);
	DECLARE_ENUM(int16, ExperienceRate);
	DECLARE_ENUM(int16, DropRate);
};

struct CharacterMaxLevelReward
{
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(int16, MaxLevel);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint8, Durability);
	DECLARE_ENUM(uint8, Skill);
	DECLARE_ENUM(uint8, Luck);
	DECLARE_ENUM(uint8, Option);
	DECLARE_ENUM(uint8, Excellent);
	DECLARE_ENUM(uint8, Ancient);
	DECLARE_ENUM(uint8, Harmony);
	DECLARE_ENUM(uint8, Option380);
	DECLARE_PROPERTY_ARRAY(SocketDataType, Socket, MAX_SOCKET_SLOT);
	DECLARE_ENUM(uint8, SocketBonus);
	DECLARE_ENUM(uint32, Flags);
};

struct CharacterExperienceTable
{
	DECLARE_ENUM(int16, LevelMin);
	DECLARE_ENUM(int16, LevelMax);
	DECLARE_ENUM(int32, ExperienceRate);
};

typedef std::vector<uint16> CharacterSkillBaseList;
typedef std::map<int32, character_helper*> CharacterHelperMap;
typedef std::list<character_formula*> CharacterFormulaList;
typedef std::vector<character_stat_fruit*> CharacterStatFruitList;
typedef std::vector<character_item*> CharacterItemList;

typedef std::map<uint16, CharacterExperience*> CharacterExperienceMap;

typedef std::list<character_specialization*> CharacterSpecializationList;

typedef std::list<character_experience_adjust*> CharacterExperienceAdjust;
typedef std::list<character_experience_bonus*> CharacterExperienceBonus;
typedef std::list<character_bonus*> CharacterBonusList;
typedef std::vector<CharacterMaxLevelReward*> CharacterMaxLevelRewardList;
typedef std::vector<CharacterExperienceTable*> CharacterExperienceTableList;

class CharacterBaseMgr
{
	SingletonInstance(CharacterBaseMgr);

	private:
		CharacterBaseData char_base[Character::MAX_CLASS];
		CharacterHelperMap m_character_helper[Character::MAX_CLASS];
		CharacterFormulaList m_CharacterFormulaList;
		CharacterStatFruitList m_character_stat_fruit[Character::MAX_CLASS];
		CharacterItemList m_character_item[Character::MAX_CLASS];
		CharacterSpecializationList m_character_specialization[Character::MAX_CLASS];

		CharacterExperienceMap m_character_experience[LEVEL_DATA_MAX];

		CharacterExperienceAdjust m_character_experience_adjust;
		CharacterExperienceBonus m_character_experience_bonus;
		CharacterBonusList m_character_bonus_list;
		CharacterMaxLevelRewardList m_CharacterMaxLevelRewardList;
		CharacterExperienceTableList m_CharacterExperienceTableList;

	public:
		CharacterBaseMgr();
		virtual ~CharacterBaseMgr();

		void LoadCharacterBase();
		void LoadCharacterBaseSkill();
		void LoadCharacterExperienceNormal();
		void LoadCharacterExperienceMaster();
		void LoadCharacterExperienceMajestic();

		void LoadCharacterHelper();
		void LoadCharacterFormula();
		void LoadCharacterStatFruit();
		void LoadCharacterItem();
		void LoadCharacterSpecialization();
		void LoadCharacterExperienceAdjust();
		void LoadCharacterExperienceBonus();
		void LoadCharacterBonus();
		void LoadCharacterMaxLevelReward();
		void LoadCharacterExperienceTable();

		CharacterBaseData * GetCharacterBase(uint8 character_class);

		int64 GetCharacterExperience(uint8 type, int16 level, bool accumulated = false) const;
		void FixCharacterExperience(Player* pPlayer);

		character_helper * GetHelperStage(uint8 race, int32 stage) const;

		CharacterSkillBaseList char_skill_base[Character::MAX_CLASS];

		uint16 GetStatFruitMaxAdd(uint8 race, int16 level);
		uint16 GetStatFruitMaxDec(uint8 race, int16 level);

		void GenerateCharacterItemList(Player* pPlayer);

		void CalculateStatFormula(Player* pPlayer);

		void CalculateSpecialization(Player* pPlayer);
		void CalculateSpecialization(Player* pPlayer, uint8 stat);
		void ApplySpecialization(Player* pPlayer, uint8 stat, float percent);

		void ApplyExperienceAdjust(Player* pPlayer, int64 & experience);
		bool IsExperienceBonus(bool const* members) const;

		int32 GetBonus(Player* pPlayer, bool drop = false);

		void LevelUpReward(Player* pPlayer, int16 level);

		CharacterExperienceTable const* GetCharacterExperienceTable(Player* pPlayer) const;
};

#define sCharacterBase SingNull(CharacterBaseMgr)

#endif