#ifndef DUNGEON_RACE_H
#define DUNGEON_RACE_H

class DungeonRace: public CommonRace
{
	SingletonInstance(DungeonRace);

	public:
		DungeonRace();
		virtual ~DungeonRace();
		void LoadData();
		void Update();
};

#endif