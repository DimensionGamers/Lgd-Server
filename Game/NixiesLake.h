#ifndef NIXIES_LAKE_H
#define NIXIES_LAKE_H

#define MAX_NIXIES_LAKE_MAGIC_CIRCLE		3
#define NIXIES_LAKE_GATE					531
#define NIXIES_LAKE_END_GATE				522

#define NIXIES_LAKE_END_TIME				5 * MINUTE * IN_MILLISECONDS

enum NixiesLakeState
{
	NIXIES_LAKE_STATE_NONE,
	NIXIES_LAKE_STATE_OPEN,
	NIXIES_LAKE_STATE_STANDBY,
	NIXIES_LAKE_STATE_OCCUPY,
	NIXIES_LAKE_STATE_END,

	NIXIES_LAKE_STATE_MAX
};

static const struct NixiesLakeZone
{
	coord_type x;
	coord_type y;
} g_NixiesLakeZone[2] = {
	{10, 200},
	{50, 240},
};

#pragma pack(1)
struct NIXIES_LAKE_MAGIC_CIRCLE_BODY
{
	uint8 x;
	uint8 y;
};

struct NIXIES_LAKE_MAGIC_CIRCLE
{
	NIXIES_LAKE_MAGIC_CIRCLE()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_MAGIC_CIRCLE, sizeof(NIXIES_LAKE_MAGIC_CIRCLE));
		this->result = 0;
		for ( int32 i = 0; i < MAX_NIXIES_LAKE_MAGIC_CIRCLE; ++i )
		{
			this->data[i].x = 0;
			this->data[i].y = 0;
		}
	}

	PBMSG_HEAD2 h;
	uint8 result;
	NIXIES_LAKE_MAGIC_CIRCLE_BODY data[MAX_NIXIES_LAKE_MAGIC_CIRCLE];
};

struct NIXIES_LAKE_MEMBER_ENTER_REQUEST
{
	NIXIES_LAKE_MEMBER_ENTER_REQUEST()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_MEMBER_ENTER, sizeof(NIXIES_LAKE_MEMBER_ENTER_REQUEST));
	}
	PBMSG_HEAD2 h;
};

struct NIXIES_LAKE_MEMBER_ENTER_RESULT
{
	PBMSG_HEAD2 h;
	uint8 result;
};

struct NIXIES_LAKE_MEMBER_ENTER_NOTIFY
{
	NIXIES_LAKE_MEMBER_ENTER_NOTIFY(uint8 result)
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_MEMBER_NOTIFY, sizeof(NIXIES_LAKE_MEMBER_ENTER_NOTIFY));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct NIXIES_LAKE_ENTER_REQUEST
{
	PBMSG_HEAD2 h;
};

struct NIXIES_LAKE_ENTER_RESULT
{
	NIXIES_LAKE_ENTER_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_ENTER, sizeof(NIXIES_LAKE_ENTER_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct NIXIES_LAKE_LEAVE
{
	NIXIES_LAKE_LEAVE()
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_LEAVE, sizeof(NIXIES_LAKE_LEAVE));
	}
	PBMSG_HEAD2 h;
};

struct NIXIES_LAKE_SINGLE_ICE_EFFECT
{
	NIXIES_LAKE_SINGLE_ICE_EFFECT(uint16 index)
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_SINGLE_ICE_EFFECT, sizeof(NIXIES_LAKE_SINGLE_ICE_EFFECT));
		this->x = 0;
		this->y = 0;
		this->index = index;
	}
	PBMSG_HEAD2 h;
	uint8 x;
	uint8 y;
	uint16 index;
};

struct NIXIES_LAKE_UNK_2
{
	NIXIES_LAKE_UNK_2(uint8 result)
	{
		this->h.set(HEADCODE_NIXIES_LAKE, 0x08, sizeof(NIXIES_LAKE_UNK_2));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct NIXIES_LAKE_ICE_BREATH
{
	NIXIES_LAKE_ICE_BREATH(uint8 result, uint16 target)
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_ICE_BREATH, sizeof(NIXIES_LAKE_ICE_BREATH));
		this->result = result;
		this->unk = 0;
		this->target = target;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 unk;
	uint16 target;
};

struct NIXIES_LAKE_SINGLE_ICE_ATTACK
{
	NIXIES_LAKE_SINGLE_ICE_ATTACK(uint8 result, uint8 x, uint8 y)
	{
		this->h.set(HEADCODE_NIXIES_LAKE, SUBCODE_NIXIES_LAKE_SINGLE_ICE_ATTACK, sizeof(NIXIES_LAKE_SINGLE_ICE_ATTACK));
		this->result = result;
		this->x = x;
		this->y = y;
	}
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 unk;
	uint8 x;
	uint8 y;
	/*
	0 = prepare
	1 = attack
	*/
};

struct NIXIES_LAKE_ICE_BREATH_DAMAGE
{
	PBMSG_HEAD2 h;
	uint16 boss;
	uint16 target;
	uint8 data[2];
};
#pragma pack()

struct NixiesLakePlayer
{
	NixiesLakePlayer()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetID(0);
		this->GetTime()->Reset();
		this->SetState(0);
	}

	DECLARE_ENUM(uint32, ID);
	DECLARE_STRUCT(TickTimer, Time);
	DECLARE_ENUM(uint8, State);
};

class NixiesLake
{
	SingletonInstance(NixiesLake);

	public:
		NixiesLake();
		virtual ~NixiesLake();

		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result);
		void EnterConfirm(Player* pPlayer, uint8 * Packet);
		void MemberEnterRequest(Player* pPlayer);
		void MemberEnterConfirm(Player* pPlayer, uint8 * Packet);
		void MemberEnterNotify(uint8 result);

		bool EnterCheck(Player* pPlayer);
		bool FindEntranceItem(Player* pPlayer, bool remove);
		bool SetEnterState(Player* pPlayer, bool state, bool member);

		void IceBreathDamage(Player* pPlayer, uint8 * Packet);
				
		void ChangeState(NixiesLakeState state);
		void Update();
		void SetState_None();
		void SetState_Open();
		void SetState_Standby();
		void SetState_Occupy();
		void SetState_End();
		void ProcState_None();
		void ProcState_Open();
		void ProcState_Standby();
		void ProcState_Occupy();
		void ProcState_End();

		void CreateBoss();
		void ClearPlayer();
		bool IsPlayerInSanctuary(Player* pPlayer) const;
		bool IsInSanctuary(world_type world, coord_type x1, coord_type y1, coord_type x2, coord_type y2) const;
		bool IsPlayerAllowed(Player* pPlayer) const;
		void ReadyToMove();

		void SendPacket(uint8 * Packet, uint16 size);

		DECLARE_ENUM(NixiesLakeState, State);
		DECLARE_ARRAY_STRUCT(NixiesLakePlayer, Player, MAX_PARTY_MEMBERS);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, SecondTimer);
};

#endif