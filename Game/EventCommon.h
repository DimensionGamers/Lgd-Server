#ifndef EVENT_COMMON_H
#define EVENT_COMMON_H

typedef bool (*WORLD_RANGE)(uint16);

#define FOR_EACH_GROUND(EVENT) \
for ( int32 i = 0; i < MAX_##EVENT##_GROUND; ++i ) \
		this->GetGround(i)

#define LOG_EVENT_STATE() \
	this->Log("%s", __FUNCTION__);

enum EventMgrState
{
	EVENT_MGR_STATE_NONE,
	EVENT_MGR_STATE_NOTIFY,
	EVENT_MGR_STATE_OPEN,
	EVENT_MGR_STATE_PLAYING,
	EVENT_MGR_STATE_MAX
};

#define EVENT_MGR_OPEN_TIME			5

struct EventCommonGroundData
{
	uint16 world;
	uint16 gate;
};

struct EventCommonGroundLevel
{
	int16 min;
	int16 max;
};

class EventCommon
{
	public:
		EventCommon();
		virtual ~EventCommon();

		DECLARE_ENUM(uint32, UpdateTick);
		DECLARE_ENUM(uint32, UpdateTime);
		DECLARE_ENUM(int32, CurrentMinute);
		DECLARE_ENUM(uint32, SecondTick);
		DECLARE_ENUM(uint8, State);
		DECLARE_ENUM(int32, NotifyTime);

		void RunTime(uint32 time)
		{
			this->SetUpdateTick(GetTickCount());
			this->SetUpdateTime(time);
			this->SetCurrentMinute(-1);
		}

		bool SecondPassed()
		{
			if ( (GetTickCount() - this->GetSecondTick()) > IN_MILLISECONDS )
			{
				this->SetSecondTick(GetTickCount());
				return true;
			}

			return false;
		}

		bool TimePassed(bool reset = false)
		{
			if ( (GetTickCount() - this->GetUpdateTick()) > this->GetUpdateTime() )
			{
				if ( reset )
					this->SetUpdateTick(GetTickCount());
				return true;
			}

			return false;
		}
		
		void SendRemainMinutes(std::string const& message, int32 period, int32 max_minute = -1);
		virtual void SendMessageID(uint8 id);

		virtual void Update() {}
		virtual void UpdateTime();

		bool IsMessageSent(uint8 id)
		{
			MessageSentMap::const_iterator it = this->message_sent.find(id);

			if ( it != this->message_sent.end() )
			{
				return it->second;
			}

			SetMessageSent(id, false);
			return false;
		}

		void SetMessageSent(uint8 id, bool state)
		{
			message_sent[id] = state;
		}

		uint32 GetRemain() const
		{
			uint32 cur_time = GetTickCount();
			uint32 diff = cur_time - this->GetUpdateTick();

			if ( diff < this->GetUpdateTime() )
			{
				diff = this->GetUpdateTime() - diff;
			}
			else
			{
				diff = 0;
			}

			return diff;
		}

		bool IsMessageElapsed(uint8 id)
		{
			if ( !this->IsMessageSent(id) && (this->GetRemain() <= (30 * IN_MILLISECONDS)) )
			{
				this->SetMessageSent(id, true);
				return true;
			}

			return false;
		}

		bool IsMinuteElapsed(int32 period)
		{
			if ( !this->GetRemain() || (this->GetRemain() / (IN_MILLISECONDS * MINUTE)) == this->GetCurrentMinute() )
				return false;

			this->SetCurrentMinute(this->GetRemain() / (IN_MILLISECONDS * MINUTE));

			if ( (this->GetCurrentMinute() % period) != 0 )
				return false;

			if ( (this->GetCurrentMinute() + 1) <= 0 )
				return false;

			return true;
		}

		void InitMessageSent()
		{
			message_sent.clear();
		}
	private:
		typedef std::map<uint8, bool> MessageSentMap;
		MessageSentMap message_sent;
};

class EventCommonMgr: public EventCommon
{
	public:
		EventCommonMgr(int32 max_ground)
		{
			this->SetMaxGround(max_ground);
		}

		virtual ~EventCommonMgr()
		{
		}

		virtual void Start(uint32 duration, int32 notify)
		{
			if ( this->GetState() != EVENT_MGR_STATE_NONE )
				return;

			this->SetNotifyTime(notify);
			this->GetNotificationTime()->Reset();

			if ( notify > 0 )
			{
				this->SetState_Notify();
			}
			else
			{
				this->SetState_Open();
			}

			this->StartGround(duration);
		}

		virtual void StartGround(uint32 duration) { }

		void UpdateTime()
		{
			switch(this->GetState())
			{
			case EVENT_MGR_STATE_NONE:		{			this->ProcState_None();			} break;
			case EVENT_MGR_STATE_NOTIFY:	{			this->ProcState_Notify();		} break;
			case EVENT_MGR_STATE_OPEN:		{			this->ProcState_Open();			} break;
			case EVENT_MGR_STATE_PLAYING:	{			this->ProcState_Playing();		} break;
			}

			this->Update();
		}

		bool HaveTicket(Item const* item, uint16 entrance, uint16 ticket, uint8 ground);
		bool HaveTicket(Player* pPlayer, uint16 entrance, uint16 ticket, uint8 ground = uint8(-1));
		virtual bool HaveTicket(Player* pPlayer, uint8 ground) { return false; }
		void ConsumeTicket(Player* pPlayer, uint16 entrance, uint16 ticket, uint8 slot);

		CREATE_STATE_VIRTUAL(None);
		CREATE_STATE_VIRTUAL(Notify);
		CREATE_STATE_VIRTUAL(Open);
		CREATE_STATE_VIRTUAL(Playing);

	private:
		DECLARE_ENUM(int32, MaxGround);
		DECLARE_STRUCT(TickTimer, NotificationTime);
};

struct DevilSquarePlayer;
struct BloodCastlePlayer;
struct ChaosCastlePlayer;
struct IllusionTemplePlayer;
struct DoubleGoerPlayer;

#define EventCastTo(type, name) \
	type * To##name() { return reinterpret_cast<type*>(this); } \
	type const* To##name() const { return (type const*)((type*)this); }

struct EventGroundPlayer
{
	public:
		void Reset()
		{
			this->SetPlayer(nullptr);
			this->SetGUID(0);
			this->SetReconnected(false);
			this->GetReconnectedTime()->Reset();
			Init();
		}

		EventCastTo(DevilSquarePlayer, DS);
		EventCastTo(BloodCastlePlayer, BC);
		EventCastTo(ChaosCastlePlayer, CC);
		EventCastTo(IllusionTemplePlayer, IT);
		EventCastTo(DoubleGoerPlayer, DG);
	private:
		virtual void Init() { }

		DECLARE_PROPERTY_PTR(Player, Player);
		DECLARE_ENUM(uint32, GUID);
		DECLARE_BOOL(Reconnected);
		DECLARE_STRUCT(TickTimer, ReconnectedTime);
};

class EventGroundCommon: public EventCommon
{
	public:
		EventGroundCommon(std::string const& name, std::string const& filter, WORLD_RANGE world_range, EventID Event)
		{
			this->SetName(name);
			this->SetLogFilter(filter);
			this->InitPlayer();
			this->world_range = world_range;
			this->SetEvent(Event);
			this->SetEnterGate(0);
			this->SetRespawnGate(0);
			this->SetWorldID(-1);
		}

		virtual ~EventGroundCommon()
		{
			this->ErasePlayer();
		}

		void InitPlayer()
		{
			
		}

		void ErasePlayer()
		{
			LIST_CLEAR(EventGroundPlayerList::iterator, player_list);
		}

		EventGroundPlayer* GetPlayer(uint32 position)
		{
			return player_list[position];
		}

		uint32 GetPlayerSize() const
		{
			return player_list.size();
		}

		int32 GetPlayersCount(bool guid) 
		{
			int32 count = 0;

			for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
			{
				if (!this->GetPlayer(i))
				{
					continue;
				}

				if ( guid )
				{
					if ( !this->GetPlayer(i)->GetGUID() )
						continue;
				}
				else
				{
					if ( !this->GetPlayer(i)->GetPlayer() )
						continue;
				}

				++count;
			}

			return count;
		}

		int32 GetPlayersCountByPC(Player* pPlayer);

		template <typename TYPE> void InitPlayers(uint32 count)
		{
			for ( uint32 i = 0; i < count; ++i )
			{
				player_list.push_back(new TYPE);
			}

			for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
			{
				this->GetPlayer(i)->Reset();
			}
		}

		void SendMessageID(uint8 id);
		bool AddPlayer(Player * pPlayer);
		void MovePlayersOut(uint16 gate);
		bool IsIn(Player* pPlayer);
		void UpdatePlayer(bool playing = true);
		void RemovePlayer(Player* pPlayer);
		 
		void SendNotice(NoticeType type, const char * message, ...);
		void SendPacket(uint8 * Packet, uint16 size);
		
		void Log(const char * log, ...);
	private:
		DECLARE_PROPERTY_STRING(LogFilter);
		DECLARE_PROPERTY_STRING(Name);
		DECLARE_ENUM(uint8, Ground);
		DECLARE_ENUM(uint32, Duration);
		DECLARE_ENUM(EventID, Event);
		DECLARE_ENUM(uint16, EnterGate);
		DECLARE_ENUM(uint16, RespawnGate);
		DECLARE_ENUM(uint16, WorldID);

		typedef std::vector<EventGroundPlayer*> EventGroundPlayerList;
		EventGroundPlayerList player_list;
		WORLD_RANGE world_range;
};

#endif