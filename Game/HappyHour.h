#ifndef HAPPY_HOUR_H
#define HAPPY_HOUR_H

enum HappyHourState
{
	HAPPY_HOUR_STATE_NONE,
	HAPPY_HOUR_STATE_NOTIFY,
	HAPPY_HOUR_STATE_START,
};

class HappyHour: public EventCommon
{
	SingletonInstance(HappyHour);

	public:
		HappyHour();
		virtual ~HappyHour();

		void Start(int32 duration, int32 notify);
		void Update();

		void SetState_None();
		void SetState_Notify();
		void SetState_Playing();

		void ProcState_None();
		void ProcState_Notify();
		void ProcState_Playing();
	private:
		DECLARE_ENUM(uint32, Duration);
};

#endif