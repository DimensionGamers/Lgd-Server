#ifndef LOSTTOWER_RACE_H
#define LOSTTOWER_RACE_H

class LosttowerRace: public CommonRace
{
	SingletonInstance(LosttowerRace);

	public:
		LosttowerRace();
		virtual ~LosttowerRace();
		void LoadData();
		void Update();
};

#endif