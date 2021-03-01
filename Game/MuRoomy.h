#ifndef MU_ROOMY_H
#define MU_ROOMY_H

#pragma once

#define MU_ROOMY_MAX_CARD						24
#define MU_ROOMY_MAX_CARD_SPECIAL				3

#define MU_ROOMY_MAX_CARD_COLOR					3
#define MU_ROOMY_MAX_CARD_NUMBER				8

#define MU_ROOMY_MAX_SPECIAL_CARD_COLOR			4
#define MU_ROOMY_MAX_SPECIAL_CARD_NUMBER		11
#define MU_ROOMY_SPECIAL_CARD_DUPLICATE			10
#define MU_ROOMY_SPECIAL_CARD_COMBINE			11

#define MU_ROOMY_MAX_SLOT						10

#define MU_ROOMY_CARD_SLOT_RANGE(slot)			(slot < 0 ? false: slot >= MU_ROOMY_MAX_CARD ? false: true)
#define MU_ROOMY_SPECIALCARD_SLOT_RANGE(slot)	(slot < 0 ? false: slot >= MU_ROOMY_MAX_CARD_SPECIAL ? false: true)

enum
{
	MU_ROOMY_CARD_STATE_NONE				= 0,
	MU_ROOMY_CARD_STATE_ACTIVE				= 1,
	MU_ROOMY_CARD_STATE_REMOVED				= 2,
	MU_ROOMY_CARD_STATE_MATCHED				= 3,

	MU_ROOMY_CARD_TYPE_NONE					= 0,
	MU_ROOMY_CARD_TYPE_NORMAL				= 1,
	MU_ROOMY_CARD_TYPE_SPECIAL				= 2,
};

#pragma pack(1)
struct MU_ROOMY_CARD_INFO
{
	MU_ROOMY_CARD_INFO()
	{
		this->color = 0;
		this->number = 0;
		this->slot = -1;
	}

	uint8 color;
	uint8 number;
	uint8 slot;
};

struct MU_ROOMY_START_REQUEST
{
	MU_ROOMY_START_REQUEST()
	{
		this->h.set(0x4D, 0x10, sizeof(MU_ROOMY_START_REQUEST));
		this->type = 0;
	}

	PBMSG_HEAD2 h;
	uint8 type;
};

struct MU_ROOMY_START_RESULT
{
	MU_ROOMY_START_RESULT()
	{
		this->h.set(0x4D, 0x10, sizeof(MU_ROOMY_START_RESULT));
		this->scoreH = 0;
		this->scoreL = 0;
		this->card_count = 0;
		this->special_card_count = 0;
		this->unk = 0;
		this->type = 0;
		memset(this->slot_status, 0, MU_ROOMY_MAX_SLOT);
	};

	PBMSG_HEAD2 h;
	uint8 scoreH;
	uint8 scoreL;
	uint8 card_count;
	uint8 special_card_count;
	uint8 unk;
	uint8 type;
	uint8 slot_status[MU_ROOMY_MAX_SLOT];
	MU_ROOMY_CARD_INFO card_info[6];
};

struct MU_ROOMY_REVEAL_CARD_REQUEST
{
	PBMSG_HEAD2 h;
};

struct MU_ROOMY_REVEAL_CARD_RESULT
{
	MU_ROOMY_REVEAL_CARD_RESULT()
	{
		this->h.set(0x4D, 0x11, sizeof(MU_ROOMY_REVEAL_CARD_RESULT));
		this->card_count = 0;
		this->special_card_count = 0;
	};

	PBMSG_HEAD2 h;
	MU_ROOMY_CARD_INFO card_info[6];
	uint8 card_count;
	uint8 special_card_count;
};

struct MU_ROOMY_CARD_MOVE_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 source_slot;
	uint8 target_slot;
};

struct MU_ROOMY_CARD_MOVE_RESULT
{
	MU_ROOMY_CARD_MOVE_RESULT()
	{
		this->h.set(0x4D, 0x12, sizeof(MU_ROOMY_CARD_MOVE_RESULT));
		this->source_slot = -1;
		this->target_slot = -1;
		this->color = -1;
		this->number = -1;
	};

	PBMSG_HEAD2 h;
	uint8 source_slot;
	uint8 target_slot;
	uint8 color;
	uint8 number;
};

struct MU_ROOMY_CARD_REMOVE_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 slot;
};

struct MU_ROOMY_CARD_REMOVE_RESULT
{
	MU_ROOMY_CARD_REMOVE_RESULT(uint8 slot)
	{
		this->h.set(0x4D, 0x13, sizeof(MU_ROOMY_CARD_REMOVE_RESULT));
		this->slot = slot;
	}

	PBMSG_HEAD2 h;
	uint8 slot;
};

struct MU_ROOMY_MATCH_REQUEST
{
	PBMSG_HEAD2 h;
};

struct MU_ROOMY_MATCH_RESULT
{
	MU_ROOMY_MATCH_RESULT()
	{
		this->h.set(0x4D, 0x14, sizeof(MU_ROOMY_MATCH_RESULT));
		this->scoreH = 0;
		this->scoreL = 0;
		this->total_scoreH = 0;
		this->total_scoreL = 0;
		this->result = 1;
	}

	PBMSG_HEAD2 h;
	uint8 scoreH;
	uint8 scoreL;
	uint8 total_scoreH;
	uint8 total_scoreL;
	uint8 result;
};

struct MU_ROOMY_END_REQUEST
{
	PBMSG_HEAD2 h;
};

struct MU_ROOMY_END_RESULT
{
	MU_ROOMY_END_RESULT(uint8 result)
	{
		this->h.set(0x4D, 0x15, sizeof(MU_ROOMY_END_RESULT));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct MU_ROOMY_CARD_LIST
{
	MU_ROOMY_CARD_LIST()
	{
		this->h.set(0x4D, 0x16, sizeof(MU_ROOMY_CARD_LIST));
	}

	PBMSG_HEAD2 h;
	MU_ROOMY_CARD_INFO card_info[6];
};

struct MU_ROOMY_MESSAGE
{
	MU_ROOMY_MESSAGE(uint8 index, uint16 value)
	{
		this->h.set(0x4D, 0x17, sizeof(MU_ROOMY_MESSAGE));
		this->index = index;
		this->valueH = HIBYTE(value);
		this->valueL = LOBYTE(value);
	};

	PBMSG_HEAD2 h;
	uint8 index;
	uint8 valueH;
	uint8 valueL;
};

struct MU_ROOMY_SPECIAL_MATCH_REQUEST
{
	PBMSG_HEAD2 h;
};
#pragma pack()
/*
0A21DD28   83BD 08FBFFFF 15 CMP DWORD PTR SS:[EBP-4F8],15
0A21DD2F  ^0F84 EBB896F6    JE 00B89620
0A21DD35   E9 00F81000      JMP 0A32D53A

0A32D53A   83BD 08FBFFFF 16 CMP DWORD PTR SS:[EBP-4F8],16
0A32D541  ^0F84 E4C085F6    JE 00B8962B
0A32D547  ^E9 01A7F1FF      JMP 0A247C4D

0A247C4D   83BD 08FBFFFF 17 CMP DWORD PTR SS:[EBP-4F8],17
0A247C54  ^0F84 DC1994F6    JE 00B89636
0A247C5A   E9 DAD30700      JMP 0A2C5039

0A2C5039   83BD 08FBFFFF 18 CMP DWORD PTR SS:[EBP-4F8],18
0A2C5040  ^0F84 FB458CF6    JE 00B89641
0A2C5046  ^E9 4043FEFF      JMP 0A2A938B

0A31B85E   83BD 08FBFFFF 14 CMP DWORD PTR SS:[EBP-4F8],14
0A31B865  ^0F8F 4FDC86F6    JG 00B894BA
0A31B86B  ^E9 37F1EFFF      JMP 0A21A9A7

0A21A9A7   83BD 08FBFFFF 14 CMP DWORD PTR SS:[EBP-4F8],14
0A21A9AE  ^0F84 61EC96F6    JE 00B89615
0A21A9B4   E9 5BCA1500      JMP 0A377414

0A15E2A9   83BD 08FBFFFF 10 CMP DWORD PTR SS:[EBP-4F8],10
0A15E2B0  ^0F84 27B3A2F6    JE 00B895DD
0A15E2B6   E9 AD4C1400      JMP 0A2A2F68

0A2A2F68   83BD 08FBFFFF 11 CMP DWORD PTR SS:[EBP-4F8],11
0A2A2F6F  ^0F84 76668EF6    JE 00B895EB
0A2A2F75  ^E9 780EEBFF      JMP 0A153DF2

0A153DF2   83BD 08FBFFFF 12 CMP DWORD PTR SS:[EBP-4F8],12
0A153DF9  ^0F84 FA57A3F6    JE 00B895F9
0A153DFF   E9 43392000      JMP 0A357747

0A357747   83BD 08FBFFFF 13 CMP DWORD PTR SS:[EBP-4F8],13
0A35774E  ^0F84 B31E83F6    JE 00B89607
0A357754  ^E9 03E2F5FF      JMP 0A2B595C


*/

class MuRoomySlotInfo
{
	public:
		MuRoomySlotInfo()
		{
			this->Init();
		}

		void Init()
		{
			this->SetCardSlot(-1);
			this->SetCardType(MU_ROOMY_CARD_TYPE_NONE);
		}

		DECLARE_ENUM(uint8, CardSlot);
		DECLARE_ENUM(uint8, CardType);
};

class MuRoomyCardInfo
{
	public:
		MuRoomyCardInfo()
		{
			this->Init();
		}

		void Init()
		{
			this->SetColor(0);
			this->SetNumber(0);
			this->SetState(MU_ROOMY_CARD_STATE_NONE);
			this->SetSlot(-1);
		}

		DECLARE_ENUM(uint8, Color);
		DECLARE_ENUM(uint8, Number);
		DECLARE_ENUM(uint8, State);
		DECLARE_ENUM(uint8, Slot);
};

class MuRoomyInfo
{
	public:
		MuRoomyInfo()
		{
			this->Init();
		}

		virtual ~MuRoomyInfo()
		{

		}

		void Init()
		{
			this->SetScore(0);
			this->SetCardDeckCount(0);
			this->SetSpecialCardDeckCount(0);
			this->SetPlayCard(false);
			this->SetType(0);

			for ( int32 i = 0; i < MU_ROOMY_MAX_CARD; ++i )
			{
				this->GetCardInfo(i)->Init();
			}

			for ( int32 i = 0; i < MU_ROOMY_MAX_CARD_SPECIAL; ++i )
			{
				this->GetSpecialCardInfo(i)->Init();
			}

			for ( int32 i = 0; i < MU_ROOMY_MAX_SLOT; ++i )
			{
				this->GetSlotInfo(i)->Init();
			}

			this->SetDate(time(nullptr));
		}

		DECLARE_PROPERTY(uint16, Score);
		DECLARE_PROPERTY(uint8, CardDeckCount);
		DECLARE_PROPERTY(uint8, SpecialCardDeckCount);
		DECLARE_BOOL(PlayCard);
		DECLARE_ENUM(uint8, Type);
		DECLARE_ENUM(time_t, Date);
		DECLARE_ARRAY_STRUCT(MuRoomyCardInfo, CardInfo, MU_ROOMY_MAX_CARD);
		DECLARE_ARRAY_STRUCT(MuRoomyCardInfo, SpecialCardInfo, MU_ROOMY_MAX_CARD_SPECIAL);
		DECLARE_ARRAY_STRUCT(MuRoomySlotInfo, SlotInfo, MU_ROOMY_MAX_SLOT);
};

struct MuRoomyReward
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, ScoreMin);
	DECLARE_ENUM(uint16, ScoreMax);
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
};

typedef std::vector<MuRoomyReward*> MuRoomyRewardList;

class MuRoomy
{
	SingletonInstance(MuRoomy);

	public:
		MuRoomy();
		virtual ~MuRoomy();

		void LoadReward();

		void Open(Player* pPlayer);
		void Start(Player* pPlayer, uint8 * Packet);
		void Reveal(Player* pPlayer, uint8 * Packet);
		void Move(Player* pPlayer, uint8 * Packet);
		void Remove(Player* pPlayer, uint8 * Packet);
		void Match(Player* pPlayer, uint8 * Packet);
		void End(Player* pPlayer, uint8 * Packet);
		void SpecialMatch(Player* pPlayer, uint8 * Packet);

		void SendCardList(Player* pPlayer);
		void SendMessage(Player* pPlayer, uint8 index, uint16 value);

		void SetCardDeck(Player* pPlayer);
		void SetPlayCardInfo(Player* pPlayer, MU_ROOMY_CARD_INFO* pCardInfo);
		uint8 GetTempCardShowSlot(Player* pPlayer);

		uint8 CardSlotRemove(Player* pPlayer, uint8 source_slot);
		uint8 CardMatchCheck(Player* pPlayer, int32& score, bool special);
		void CardSortAscending(MuRoomyCardInfo * pMuRoomyCardInfo, int32 count);
		int IsCardSameColor(MuRoomyCardInfo * pMuRoomyCardInfo, int32 count);
		int IsCardNumber(MuRoomyCardInfo * pMuRoomyCardInfo, int32 count);
		void FillEmptySlotCard(Player* pPlayer, MU_ROOMY_CARD_INFO* CardInfo);

		void GenerateSpecialCard(Player* pPlayer);

		MuRoomyReward const* GetReward(Player* pPlayer) const;

		DECLARE_BOOL(Active);
		MuRoomyRewardList reward_list;
};

#endif