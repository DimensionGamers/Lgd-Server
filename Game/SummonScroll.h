#ifndef SUMMON_SCROLL_H
#define SUMMON_SCROLL_H

#pragma once

#define MAX_SUMMON_SCROLL_MONSTER_GROUP			20

struct SummonScrollInfo
{
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint16, Item);
};

struct SummonScrollMonsterInfo
{
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Group);
	DECLARE_ENUM(uint16, MonsterClass);
	DECLARE_ENUM(uint16, CreateRate);
};

typedef std::vector<SummonScrollInfo*> SummonScrollInfoList;
typedef std::vector<SummonScrollMonsterInfo*> SummonScrollMonsterInfoList;

class SummonScroll
{
	SingletonInstance(SummonScroll);

	public:
		SummonScroll();
		virtual ~SummonScroll();
		void Load();
		void LoadMonster();

		bool CheckSummonScroll(uint16 item) const;
		SummonScrollInfo const* GetSummonScrollInfo(uint16 item) const;
		bool CreateSummonScrollMonster(Player* pPlayer, uint16 item, uint16 map, int16 x, int16 y);

	private:
		SummonScrollInfoList m_SummonScrollInfo;
		SummonScrollMonsterInfoList m_SummonScrollMonsterInfo;
};

#endif