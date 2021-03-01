#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

namespace Custom
{
	class SystemTimer
	{
		public:
			explicit SystemTimer()
			{
				GetLocalTime(&time);
			}

			uint32 getInMinutes()
			{
				return (time.wHour * MINUTE + time.wMinute);
			}

			uint32 getInSeconds()
			{
				return (time.wHour * HOUR + time.wMinute * MINUTE + time.wSecond);
			}

			int32 getInMSeconds()
			{
				return (time.wHour * HOUR + time.wMinute * MINUTE + time.wSecond) * IN_MILLISECONDS;
			}

			uint16 GetDayOfWeek() const { return time.wDayOfWeek; }
			uint16 GetDay() const { return time.wDay; }
			uint16 GetMonth() const { return time.wMonth; }
			uint16 GetYear() const { return time.wYear; }
			uint16 GetHour() const { return time.wHour; }
			uint16 GetMinute() const { return time.wMinute; }

			uint8 GetDayOfWeekMod()
			{
				uint8 day = 0;

				switch ( this->GetDayOfWeek() )
				{
				case MONDAY: { day = 0; } break;
				case TUESDAY: { day = 1; } break;
				case WEDNESDAY: { day = 2; } break;
				case THURSDAY: { day = 3; } break;
				case FRIDAY: { day = 4; } break;
				case SATURDAY: { day = 5; } break;
				case SUNDAY: { day = 6; } break;
				}

				return day;
			}

			uint32 GetConvertedToMinute() const
			{
				return this->GetMinute() + (this->GetHour() * MINUTE) + (this->GetDay() * HOUR) + (this->GetMonth() * DAY);
			}
		private:
			SYSTEMTIME time;
	};
};

#endif