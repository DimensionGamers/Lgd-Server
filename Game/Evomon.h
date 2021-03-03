#ifndef EVOMON_H
#define EVOMON_H

static const uint16 EvomonMonster[2] = { 681, 690 };
static const uint16 EvomonPortal[2] = { 704, 705 };

struct EvomonInfo
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint16, Monster);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(int32, Life);
	DECLARE_ENUM(int32, AttackDamageMin);
	DECLARE_ENUM(int32, AttackDamageMax);
	DECLARE_ENUM(int32, AttackSuccessRate);
	DECLARE_ENUM(int32, Defense);
	DECLARE_ENUM(int32, DefenseSuccessRate);
	DECLARE_ENUM(uint8, NextID);
	DECLARE_ENUM(uint16, NextIDRate);
	DECLARE_ENUM(uint16, SpecialRate);
	DECLARE_ENUM(uint16, SpecialMonster);
};

struct EvomonReward
{
	DECLARE_ENUM(int32, ScoreMin);
	DECLARE_ENUM(int32, ScoreMax);
	DECLARE_ENUM(uint8, RewardType);
	DECLARE_ENUM(uint16, RewardIndex);
	DECLARE_ENUM(int32, RewardCount);
};

typedef std::list<EvomonInfo*> EvomonInfoList;
typedef std::list<EvomonReward*> EvomonRewardList;

class Evomon
{
	SingletonInstance(Evomon);

	public:
		Evomon();
		virtual ~Evomon();

		void Load();
		void LoadReward();

		EvomonInfo const* GetEvomon(uint8 id, uint16 monster) const;
		int32 GetEvomonScore(uint8 id, uint16 monster) const;
		uint8 GetNextEvomon(uint8 id, uint16 monster) const;
		uint8 ScrollDrop(Player* pPlayer, Item const* pItem, uint8 x, uint8 y);
		void ScrollDropResult(Player* pPlayer, uint8 result, uint16 world = 0, int16 x = 0, int16 y = 0, uint32 duration = 0);
		void SendScore(Player* pPlayer, uint8 result, int32 score);
		void GiveReward(Player* pPlayer, int32 score);
		void GiveSpecialReward(Player* pPlayer);

		EvomonInfoList evomon_list;
		EvomonRewardList evomon_reward_list;

		DECLARE_BOOL(Active);
};

#endif