#ifndef KANTURU_DEF_H
#define KANTURU_DEF_H

#define KANTURU_AI_NAME							"kanturu_ai"
#define KANTURU_MAYA_AI_NAME					"kanturu_maya_ai"
#define KANTURU_MAYA_HAND_AI_NAME				"kanturu_maya_hand_ai"
#define KANTURU_NIGHTMARE_NAME					"kanturu_nightmare_ai"

#define KANTURU_GATE_BATTLE						133
#define KANTURU_GATE_NIGHTMARE					134
#define KANTURU_GATE_TOWER						135
#define KANTURU_GATE_TOWER_OUT					136
#define KANTURU_GATE_OUT						137

enum KanturuState
{
	KANTURU_STATE_NONE							= 0,
	KANTURU_STATE_BATTLE_STANDBY				= 1,
	KANTURU_STATE_BATTLE_OF_MAYA				= 2,
	KANTURU_STATE_BATTLE_OF_NIGHTMARE			= 3,
	KANTURU_STATE_TOWER_OF_REFINEMENT			= 4,
	KANTURU_STATE_END							= 5,
	KANTURU_STATE_MAX
};

enum KanturuSubState
{
	KANTURU_STANBY_NONE							= 0,
	KANTURU_STANBY_START						= 1,
	KANTURU_STANBY_NOTIFY						= 2,
	KANTURU_STANBY_END							= 3,
	KANTURU_STANBY_MAX,

	KANTURU_MAYA_NONE							= 0,
	KANTURU_MAYA_STANDBY_1						= 1,
	KANTURU_MAYA_NOTIFY_1						= 2,
	KANTURU_MAYA_START_1						= 3,
	KANTURU_MAYA_1								= 4,
	KANTURU_MAYA_END_1							= 5,
	KANTURU_MAYA_ENDCYCLE_1						= 6,
	KANTURU_MAYA_STANDBY_2						= 7,
	KANTURU_MAYA_START_2						= 8,
	KANTURU_MAYA_2								= 9,
	KANTURU_MAYA_END_2							= 10,
	KANTURU_MAYA_ENDCYCLE_2						= 11,
	KANTURU_MAYA_STANDBY_3						= 12,
	KANTURU_MAYA_START_3						= 13,
	KANTURU_MAYA_3								= 14,
	KANTURU_MAYA_END_3							= 15,
	KANTURU_MAYA_ENDCYCLE_3						= 16,
	KANTURU_MAYA_END							= 17,
	KANTURU_MAYA_ENDCYCLE						= 18,
	KANTURU_MAYA_MAX,

	KANTURU_MAYA_SCENE_NONE						= -1,
	KANTURU_MAYA_SCENE_0						= 0,
	KANTURU_MAYA_SCENE_1						= 1,
	KANTURU_MAYA_SCENE_2						= 2,
	KANTURU_MAYA_SCENE_MAX,

	KANTURU_NIGHTMARE_NONE						= 0,
	KANTURU_NIGHTMARE_IDLE						= 1,
	KANTURU_NIGHTMARE_NOTIFY					= 2,
	KANTURU_NIGHTMARE_START						= 3,
	KANTURU_NIGHTMARE_END						= 4,
	KANTURU_NIGHTMARE_ENDCYCLE					= 5,
	KANTURU_NIGHTMARE_MAX,

	KANTURU_TOWER_OF_REFINEMENT_NONE			= 0,
	KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION	= 1,
	KANTURU_TOWER_OF_REFINEMENT_NOTIFY_1		= 2,
	KANTURU_TOWER_OF_REFINEMENT_CLOSE			= 3,
	KANTURU_TOWER_OF_REFINEMENT_NOTIFY_2		= 4,
	KANTURU_TOWER_OF_REFINEMENT_END				= 5,
	KANTURU_TOWER_OF_REFINEMENT_MAX,
};

class KanturuStateInfo
{
	public:
		KanturuStateInfo()
		{
			this->ResetTimeInfo();
		}
		virtual ~KanturuStateInfo() { }

		void ResetTimeInfo()
		{
			this->SetStateInfo(0);
			this->SetCondition(0);
			this->SetValue(0);
			this->SetAppliedTime(0);
		}
	
		void Start()
		{
			if ( this->GetCondition() > 0 )
			{
				this->SetAppliedTime(GetTickCount());
			}
		}

		bool Passed() const
		{
			return this->GetRemainTime() <= 0;
		}

		int32 GetRemainTime() const
		{
			int32 remain_time = 0;

			if ( this->GetCondition() == 1 )
			{
				remain_time = this->GetValue() - this->GetElapsedTime();

				if ( remain_time <= 0 )
				{
					return 0;
				}

				remain_time /= IN_MILLISECONDS;
			}

			return remain_time;
		}

		int32 GetElapsedTime() const
		{
			return GetTickCount() - this->GetAppliedTime();
		}
	private:
		DECLARE_ENUM(int32, StateInfo);
		DECLARE_ENUM(int32, Condition);
		DECLARE_ENUM(int32, Value);
		DECLARE_ENUM(uint32, AppliedTime);
};

#endif