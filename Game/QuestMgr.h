/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "QuestMgr.h"
*
*/

#ifndef QUESTMGR_H
#define QUESTMGR_H

struct QuestEvolutionData
{
	DECLARE_ENUM(uint8, QuestID);
	DECLARE_ENUM(uint8, ObjectiveType);
	DECLARE_ENUM(uint8, ItemType);
	DECLARE_ENUM(uint16, ItemIndex);
	DECLARE_ENUM(uint8, ItemLevel);
	DECLARE_ENUM(uint16, Count);
	DECLARE_ENUM(int16, MonsterID);
	DECLARE_ENUM(int16, MonsterMinLevel);
	DECLARE_ENUM(int16, MonsterMaxLevel);
	DECLARE_ENUM(uint8, MonsterSlot);
	DECLARE_ENUM(uint16, DropRate);
	DECLARE_ENUM(uint16, RewardType);
	DECLARE_ENUM(uint16, RewardSubType);
	DECLARE_ENUM(uint16, RewardCount);
	DECLARE_ENUM(int16, ConditionIndex);
	DECLARE_ENUM(uint16, ContextBeforeReg);
	DECLARE_ENUM(uint16, ContextAfterReg);
	DECLARE_ENUM(uint16, ContextCompleteQuest);
	DECLARE_ENUM(uint16, ContextClearQuest);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredClass, Character::MAX_CLASS);

	uint16 GetItem() const { return ITEMGET(this->GetItemType(),this->GetItemIndex()); }
};

struct QuestEvolutionCondition
{
	DECLARE_ENUM(uint8, QuestID);
	DECLARE_ENUM(int32, ConditionID);
	DECLARE_ENUM(uint8, RequiredQuestID);
	DECLARE_ENUM(uint32, MinLevel);
	DECLARE_ENUM(uint32, MaxLevel);
	DECLARE_ENUM(uint32, RequiredZen);
	DECLARE_ENUM(uint8, ContextStart);
};

typedef std::vector<QuestEvolutionData*> QuestEvolutionDataList;
typedef std::vector<QuestEvolutionCondition*> QuestEvolutionConditionList;

struct QuestEvolution
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, NPC);
	DECLARE_ENUM(uint8, Party);
	DECLARE_PROPERTY_STRING(Name);

	QuestEvolutionDataList data;
	QuestEvolutionConditionList condition;
};

struct QuestEvolutionReward
{
	DECLARE_ENUM(uint8, QuestID);
	DECLARE_FLAG(uint32, ClassFlag);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint8, Durability);
	DECLARE_ENUM(uint8, Skill);
	DECLARE_ENUM(uint8, Luck);
	DECLARE_ENUM(uint8, Option);
	DECLARE_ENUM(uint8, Excellent);
	DECLARE_ENUM(uint8, Ancient);
	DECLARE_PROPERTY_ARRAY(SocketDataType, Socket, MAX_SOCKET_SLOT);
	DECLARE_ENUM(uint8, SocketBonus);

	uint16 GetItem() const { return ITEMGET(this->GetType(), this->GetIndex()); }
};

struct QuestGuidedCondition
{
	DECLARE_ENUM(uint16, TemplateType);
	DECLARE_ENUM(uint16, Chapter);
	DECLARE_ENUM(uint16, Category);
	DECLARE_ENUM(uint16, Importance);
	DECLARE_ENUM(uint16, StartType);
	DECLARE_ENUM(uint16, StartSubType);
	DECLARE_ENUM(uint16, LevelMin);
	DECLARE_ENUM(uint16, QuestRepeatCycle);
	DECLARE_ENUM(uint16, PrecedentQuest);
	DECLARE_ENUM(uint16, QuestStartItemType);
	DECLARE_ENUM(uint16, QuestStartItemIndex);
	DECLARE_ENUM(uint8, QuestCheckGens);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredClass, Character::MAX_CLASS);
	DECLARE_ENUM(uint16, StartText);
	DECLARE_ENUM(uint16, StartErrorText);
};

struct QuestGuidedData
{
	DECLARE_ENUM(uint16, TemplateType);
	DECLARE_ENUM(uint16, ObjectiveType); ///- Type 2 = MOB
	DECLARE_ENUM(uint16, QuestMainType); ///- MonsterID
	DECLARE_ENUM(uint16, QuestSubType);
	DECLARE_ENUM(uint16, Count);
	DECLARE_ENUM(int16, MonsterMaxLevel);
	DECLARE_ENUM(uint16, DropRate);
	DECLARE_ENUM(uint16, GateNumber);
	DECLARE_ENUM(world_type, World);
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
	DECLARE_ENUM(uint32, RewardExp);
	DECLARE_ENUM(uint32, RewardZen);
	DECLARE_ENUM(uint8, RewardItemType01);
	DECLARE_ENUM(uint16, RewardItemIndex01);
	DECLARE_ENUM(int32, RewardItemCount01);
	DECLARE_ENUM(uint8, RewardItemType02);
	DECLARE_ENUM(uint16, RewardItemIndex02);
	DECLARE_ENUM(int32, RewardItemCount02);
	DECLARE_ENUM(uint8, RewardItemType03);
	DECLARE_ENUM(uint16, RewardItemIndex03);
	DECLARE_ENUM(int32, RewardItemCount03);
	DECLARE_ENUM(uint16, RewardType);
	DECLARE_ENUM(uint16, RewardSubType);
	DECLARE_ENUM(uint16, RewardNumber);
	DECLARE_ENUM(uint16, ProgressText);
	DECLARE_ENUM(uint16, CompleteText);

	uint16 GetRequiredItem() const { return ITEMGET(this->GetQuestMainType(), this->GetQuestSubType()); }

	uint16 GetRewardItem1() const { return ITEMGET(this->GetRewardItemType01(), this->GetRewardItemIndex01()); }
	uint16 GetRewardItem2() const { return ITEMGET(this->GetRewardItemType02(), this->GetRewardItemIndex02()); }
	uint16 GetRewardItem3() const { return ITEMGET(this->GetRewardItemType03(), this->GetRewardItemIndex03()); }
};

struct QuestGuided
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_STRUCT(QuestGuidedCondition, Condition);
	DECLARE_STRUCT(QuestGuidedData, Data);

	bool IsClassAllowed(uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const
	{
		if ( !this->GetCondition()->GetRequiredClass(race) )
			return false;

		if ( this->GetCondition()->GetRequiredClass(race) == 2 && change_up_1 == 0 )
			return false;

		if ( this->GetCondition()->GetRequiredClass(race) == 3 && change_up_2 == 0 )
			return false;

		if ( this->GetCondition()->GetRequiredClass(race) == 4 && change_up_3 == 0 )
			return false;

		return true;
	}
};

struct QuestMU
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint8, Server);
	DECLARE_ENUM(uint8, Day);
	DECLARE_ENUM(uint8, TemplateType1);
	DECLARE_ENUM(uint8, Chapter);
	DECLARE_ENUM(uint8, Category);
	DECLARE_ENUM(uint8, Importante);
	DECLARE_ENUM(uint8, StartType);
	DECLARE_ENUM(uint16, StartSubType);
	DECLARE_ENUM(uint16, LevelMin);
	DECLARE_ENUM(uint16, LevelMax);
	DECLARE_ENUM(int8, RepeatCycle);
	DECLARE_ENUM(int16, PrecedenceQuest);
	DECLARE_ENUM(int8, StartItemType);
	DECLARE_ENUM(int16, StartItemIndex);
	DECLARE_ENUM(int8, CheckGens);
	DECLARE_ENUM(int32, Zen);
	DECLARE_PROPERTY_ARRAY(uint8, RequiredClass, Character::MAX_CLASS);
	DECLARE_ENUM(uint8, AddClass);
	DECLARE_ENUM(uint8, TemplateType2);
	DECLARE_ENUM(uint8, ObjectiveType);
	DECLARE_ENUM(int16, ObjectiveMainType);
	DECLARE_ENUM(int16, ObjectiveMainSubType);
	DECLARE_ENUM(int16, TargetNumber);
	DECLARE_ENUM(int16, TargetMaxLevel);
	DECLARE_ENUM(int16, DropRate);
	DECLARE_ENUM(uint16, GateID);
	DECLARE_ENUM(uint16, MapID);
	DECLARE_ENUM(uint8, X);
	DECLARE_ENUM(uint8, Y);
	DECLARE_ENUM(int32, RewardExperience);
	DECLARE_ENUM(int32, RewardZen);
	DECLARE_ENUM(uint8, RewardGensPoints);
	DECLARE_PROPERTY_ARRAY(int8, RewardItemType, QUEST_MU_MAX_REWARD);
	DECLARE_PROPERTY_ARRAY(int16, RewardItemIndex, QUEST_MU_MAX_REWARD);
	DECLARE_PROPERTY_ARRAY(int16, RewardItemCount, QUEST_MU_MAX_REWARD);
	DECLARE_ENUM(int16, RewardType);
	DECLARE_ENUM(int16, RewardSubType);
	DECLARE_ENUM(int16, RewardNumber);

	bool IsLevel(uint16 level) const
	{
		return level >= this->GetLevelMin() && level <= this->GetLevelMax();
	}

	bool IsClass(uint8 character) const
	{
		return this->GetRequiredClass(character) != 0;
	}

	uint16 GetRewardItem(uint8 id) const
	{
		return ITEMGET(this->GetRewardItemType(id), this->GetRewardItemIndex(id));
	}

	uint16 GetObjectiveItem() const
	{
		return ITEMGET(this->GetObjectiveMainType(), this->GetObjectiveMainSubType());
	}
};

struct Quest4thInstanceData
{
	explicit Quest4thInstanceData(int32 instance)
	{
		this->SetInstance(instance);
		this->SetState(0);
		this->SetQuestID(0);
		this->SetPartyID(PARTY_NULL);
		this->GetTime()->Reset();
	}

	DECLARE_ENUM(int32, Instance);
	DECLARE_ENUM(uint8, State);
	DECLARE_ENUM(uint8, QuestID);
	DECLARE_ENUM(uint16, PartyID);
	DECLARE_STRUCT(TickTimer, Time);
};

typedef std::map<uint8, QuestEvolution*> QuestEvolutionMap;
typedef std::vector<QuestEvolutionReward*> QuestEvolutionRewardList;
typedef std::map<uint16, QuestGuided*> QuestGuidedMap;
typedef std::map<int32, Quest4thInstanceData*> Quest4thInstanceDataMap;
typedef std::map<uint16, QuestMU*> QuestMUMap;

class CQuestMgr
{
	SingletonInstance(CQuestMgr);

	public:
		CQuestMgr();
		virtual ~CQuestMgr();

		void LoadQuestEvolution();
		void LoadQuestEvolutionData();
		void LoadQuestEvolutionCondition();
		void LoadQuestEvolutionItemReward();
		void LoadQuestGuided();
		void LoadQuestMU();

		QuestEvolution const* GetQuest(uint8 quest_id) const;
		QuestEvolution * GetQuest(uint8 quest_id);

		QuestGuided * GetQuestGuided(uint16 quest);
		QuestGuided const* GetQuestGuided(uint16 quest) const;

		uint16 GetFinalQuestGuided(Player* pPlayer) const;

		void GiveQuestEvolutionItemReward(Player* pPlayer, uint8 quest_id);
	public:
		QuestEvolutionMap m_quest_evolution;
		QuestEvolutionRewardList m_QuestEvolutionRewardList;
		QuestMUMap m_QuestMUMap;

		QuestGuidedMap m_quest_guided_map;
		
		DECLARE_ENUM(int8, TestingDay);

		DECLARE_PROPERTY(int32, Current4thQuestInstance);
		Quest4thInstanceDataMap m_quest_4th_instance_data_map;

		Quest4thInstanceData * Get4thQuestInstance();
		Quest4thInstanceData const* GetQuest4thInstanceData(int32 instance) const;
		Quest4thInstanceData * GetQuest4thInstanceData(int32 instance);

		void Update4thQuestInstance();
		void Change4thQuestInstanceState(int32 instance, uint8 state, uint8 quest_id);

		QuestMU const* GetQuestMU(uint16 id) const;
};

#endif