#ifndef CASTLE_DEEP_H
#define CASTLE_DEEP_H

enum CastleDeepState
{
	CASTLE_DEEP_STATE_NONE,
	CASTLE_DEEP_STATE_NOTIFY,
	CASTLE_DEEP_STATE_PLAYING
};

struct CastleDeepStage
{
	DECLARE_ENUM(int32, Stage);
	DECLARE_ENUM(uint16, Rate);
};

typedef std::map<int32, CastleDeepStage*> CastleDeepStageMap;

class CastleDeep
{
	SingletonInstance(CastleDeep);
	public:
		CastleDeep();
		virtual ~CastleDeep();

		void Load();

		void Start(int32 notify);
		void Update();
		
		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Playing);

		DECLARE_ENUM(CastleDeepState, State);
		DECLARE_ENUM(int32, Stage);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_ENUM(int32, CurrentMinute);
		CastleDeepStageMap stage_map;

		void GetRandomStage();
		void AddMonster();

		void SendRemainMinutes(std::string const& message, int32 period, int32 max_minute = -1);

		bool IsMinuteElapsed(int32 period)
		{
			if ( !this->GetTime()->GetRemain() || (this->GetTime()->GetRemain() / (IN_MILLISECONDS * MINUTE)) == this->GetCurrentMinute() )
				return false;

			this->SetCurrentMinute(this->GetTime()->GetRemain() / (IN_MILLISECONDS * MINUTE));

			if ( (this->GetCurrentMinute() % period) != 0 )
				return false;

			if ( (this->GetCurrentMinute() + 1) <= 0 )
				return false;

			return true;
		}
};

#endif