#ifndef IMPERIAL_FORTRESS_INSTANCE_H
#define IMPERIAL_FORTRESS_INSTANCE_H

class ImperialFortressPlayer : public EventGroundPlayer
{
public:
	void Init()
	{
		this->SetZone(0);
		this->SetCompletedZones(0);
		this->SetWorld(-1);
		this->SetX(0);
		this->SetY(0);
	}

private:
	DECLARE_ENUM(uint8, Zone);
	DECLARE_PROPERTY(int32, CompletedZones);
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X);
	DECLARE_ENUM(int16, Y);
};

struct ImperialFortressReleaseZone
{
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
};

typedef std::vector<ImperialFortressReleaseZone*> ImperialFortressReleaseZoneList;

class ImperialFortressInstance
{
	public:
		ImperialFortressInstance();
		virtual ~ImperialFortressInstance();

		DECLARE_ENUM(uint32, ID);
		DECLARE_ENUM(uint32, ZoneLevel);
		DECLARE_ENUM(ImperialFortressState, State);
		DECLARE_ENUM(uint8, Day);
		DECLARE_ENUM(uint8, Weather);
		DECLARE_ENUM(uint8, Zone);
		DECLARE_ENUM(uint16, PartyID);
		DECLARE_PROPERTY(int16, MonsterCount);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, SecondTime);
		DECLARE_BOOL(ReadyToMove);
		DECLARE_BOOL(ReadyToRemove);

		DECLARE_ARRAY_STRUCT(ImperialFortressPlayer, Player, MAX_PARTY_MEMBERS);
		ImperialFortressReleaseZoneList m_ImperialFortressReleaseZoneList;

		void Update();
		void SetState_None();
		void SetState_StandBy();
		void SetState_Door1();
		void SetState_Fight();
		void SetState_Door2();
		void SetState_Door3();
		void SetState_Move();
		void SetState_End();

		void ProcState_None();
		void ProcState_StandBy();
		void ProcState_Playing();
		void ProcState_End();

		void SendTime();
		void SendFail();
		void SendFail(Player* pPlayer);
		void SendSuccess();

		void UpdatePlayers();
		void MoveOut();
		void TrapDamage();
		void UpdateTime();

		void AddMonster(ImperialFortressMonsterType type);
		void ClearMonster();

		void BlockReleaseZone(uint8 zone, bool block);
		void BlockReleaseZone(Monster* pMonster);

		ImperialFortressPlayer* FindPlayer(Player* pPlayer);

		bool IsZoneEmpty() const;
		bool IsZoneReleased(int16 x, int16 y) const;
};

#endif