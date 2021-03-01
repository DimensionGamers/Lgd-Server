#ifndef LAST_MAN_STANDING_H
#define LAST_MAN_STANDING_H

enum
{
	LAST_MAN_STANDING_STATE_NONE,
	LAST_MAN_STANDING_STATE_NOTIFY,
	LAST_MAN_STANDING_STATE_OPEN,
	LAST_MAN_STANDING_STATE_STANDBY,
	LAST_MAN_STANDING_STATE_PLAYING,
	LAST_MAN_STANDING_STATE_END,

	LAST_MAN_STANDING_STATE_MAX
};

enum LastManStandingType
{
	LAST_MAN_STANDING_TYPE_NORMAL,
	LAST_MAN_STANDING_TYPE_LEVEL,
	LAST_MAN_STANDING_TYPE_RACE,
	LAST_MAN_STANDING_TYPE_MAX
};

#define LAST_MAN_STANDING_GATE			1001

class LastManStanding
{
	SingletonInstance(LastManStanding);

	public:
		LastManStanding();
		virtual ~LastManStanding();

		void Start(int32 notify);
		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Standby);
		CREATE_STATE(Playing);
		CREATE_STATE(End);

		DECLARE_ENUM(uint8, State);
		DECLARE_ENUM(uint8, Type);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_ENUM(int32, CurrentMinute);
		DECLARE_ENUM(int32, CurrentSecond);
		DECLARE_ENUM(int32, NotifyTime);

		void MovePlayersOut();
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

		void PlayerKill(Player* pPlayer);
		void CheckWinner();

		void EnterRequest(Player* pPlayer);

		void SetInstance();
};

#endif