#ifndef TICK_TIMER_H
#define TICK_TIMER_H

class TickTimer
{
	public:
		TickTimer();
		virtual ~TickTimer();

		void Reset();
		void Start();
		void Start(uint32 delay);

		bool Elapsed();
		bool Elapsed(uint32 delay);
		bool ExElapsed(uint32 delay);

		uint32 GetElapsed() const;
		uint32 GetRemain() const;
		bool Finished() const;
	private:
		DECLARE_ENUM(TCType, Timer);
		DECLARE_ENUM(uint32, Delay);
};

struct Timer
{
	DECLARE_PROPERTY(TCType, Tick);
	DECLARE_PROPERTY(int32, RemainMsec);

	Timer() { this->Reset(); }
	virtual ~Timer() {}

	void Reset()
	{
		this->SetTick(0);
		this->SetRemainMsec(0);
	}

	void Start(int32 remain_msec)
	{
		this->SetTick(MyGetTickCount());
		this->SetRemainMsec(remain_msec);
	}

	bool Elapsed(int32 msec)
	{
		int32 iTICK_MSEC = int32(MyGetTickCount() - this->GetTick());

		if ( iTICK_MSEC >= msec )
		{
			this->ReduceRemainMsec(iTICK_MSEC);
			this->SetTick(MyGetTickCount());
			return true;
		}

		return false;
	}

	bool Finised() const { return this->GetRemainMsec() <= 0; }
};

#endif