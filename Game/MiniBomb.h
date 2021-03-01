#ifndef MINI_BOMB_H
#define MINI_BOMB_H

enum
{
	MINI_BOMB_GAME_START			= 1,
	MINI_BOMB_GAME_CURRENT			= 2,
	MINI_BOMB_GAME_RESULT			= 3,

	MINI_BOMB_CELL_STATUS_HIDE		= 0,
	MINI_BOMB_CELL_STATUS_REVEALED	= 1,
	MINI_BOMB_CELL_STATUS_MARKED	= 2,

	MINI_BOMB_CELL_VALUE_EMPTY		= 0,
	MINI_BOMB_CELL_VALUE_NUMBER_1	= 1,
	MINI_BOMB_CELL_VALUE_NUMBER_2	= 2,
	MINI_BOMB_CELL_VALUE_NUMBER_3	= 3,
	MINI_BOMB_CELL_VALUE_NUMBER_4	= 4,
	MINI_BOMB_CELL_VALUE_NUMBER_5	= 5,
	MINI_BOMB_CELL_VALUE_NUMBER_6	= 6,
	MINI_BOMB_CELL_VALUE_NUMBER_7	= 7,
	MINI_BOMB_CELL_VALUE_NUMBER_8	= 8,
	MINI_BOMB_CELL_VALUE_MINE		= 9,
	MINI_BOMB_CELL_VALUE_MINE_ERR	= 10,
	MINI_BOMB_CELL_VALUE_MINE_BOOM	= 11,
	//MINI_BOMB_CELL_MARKED			= 254,
	//MINI_BOMB_CELL_NONE			= 255,
};

#pragma pack(1)
struct MINI_BOMB_OPEN_REQUEST
{
	PBMSG_HEAD2 h;
};

struct MINI_BOMB_OPEN_BODY
{
	uint8 cell;
	uint8 type;
};

struct MINI_BOMB_OPEN_HEAD
{
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 remain_bombs;
	uint8 count;
	uint8 junk;
	uint16 current_score;
};

struct MINI_BOMB_START_REQUEST
{
	PBMSG_HEAD2 h;
};

struct MINI_BOMB_START_RESULT
{
	MINI_BOMB_START_RESULT(uint8 result)
	{
		this->h.set(0x4D, 0x23, sizeof(MINI_BOMB_START_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;

	/*
	0 = Start
	1 = Insufficient Card
	*/
};

struct MINI_BOMB_REVEAL_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 cell;
};

struct MINI_BOMB_REVEAL_BODY
{
	uint8 cell;
	uint8 type;
};

struct MINI_BOMB_REVEAL_HEAD
{
	MINI_BOMB_REVEAL_HEAD()
	{
		this->h.set(0x4D, 0x24, sizeof(MINI_BOMB_REVEAL_HEAD));
	}

	PBMSG_HEAD2 h;
	uint8 cell;
	uint8 scoreH;
	uint8 scoreL;
	uint8 count;
};

struct MINI_BOMB_MARK_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 cell;
};

struct MINI_BOMB_MARK_RESULT
{
	MINI_BOMB_MARK_RESULT(uint8 result)
	{
		this->h.set(0x4D, 0x25, sizeof(MINI_BOMB_MARK_RESULT));
		this->cell = 0;
		this->result = result;
		this->remain_bombs = 0;
	}

	PBMSG_HEAD2 h;
	uint8 cell;
	uint8 result;
	uint8 remain_bombs;
};

struct MINI_BOMB_RESULT_BODY
{
	uint8 cell;
	uint8 type;
};

struct MINI_BOMB_RESULT_HEAD
{
	PBMSG_HEAD2 h;
	uint8 result;
	uint8 count;
	uint16 score;
	uint16 bombs_found;
	uint16 bombs_failure;
	uint16 stepped_on_bomb;
	uint16 clear;
	uint16 total_score;
};

struct MINI_BOMB_GIFT_REQUEST
{
	PBMSG_HEAD2 h;
};

struct MINI_BOMB_GIFT_RESULT
{
	MINI_BOMB_GIFT_RESULT(uint8 result)
	{
		this->h.set(0x4D, 0x28, sizeof(MINI_BOMB_GIFT_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct MINI_BOMB_CREATE_CELL
{
	MINI_BOMB_CREATE_CELL()
	{
		this->h.set(0x4D, 0x29, sizeof(MINI_BOMB_CREATE_CELL));
		this->y = 0;
		this->x = 0;
		this->effect = 0;
		memset(this->time, 0, 4);
	}

	PBMSG_HEAD2 h;
	uint8 time[4];
	uint8 x;
	uint8 y;
	uint8 effect;
};

/*

0A2C5039   83BD 08FBFFFF 18 CMP DWORD PTR SS:[EBP-4F8],18
0A2C5040  ^0F84 FB458CF6    JE 00B89641
0A2C5046  ^E9 4043FEFF      JMP 0A2A938B

0A247C4D   83BD 08FBFFFF 17 CMP DWORD PTR SS:[EBP-4F8],17
0A247C54  ^0F84 DC1994F6    JE 00B89636
0A247C5A   E9 DAD30700      JMP 0A2C5039

0A32D53A   83BD 08FBFFFF 16 CMP DWORD PTR SS:[EBP-4F8],16
0A32D541  ^0F84 E4C085F6    JE 00B8962B
0A32D547  ^E9 01A7F1FF      JMP 0A247C4D

0A21DD28   83BD 08FBFFFF 15 CMP DWORD PTR SS:[EBP-4F8],15
0A21DD2F  ^0F84 EBB896F6    JE 00B89620
0A21DD35   E9 00F81000      JMP 0A32D53A

	0A2A938B   83BD 08FBFFFF 23 CMP DWORD PTR SS:[EBP-4F8],23
0A2A9392  ^0F84 B4028EF6    JE 00B8964C
0A2A9398  ^E9 1CD3FBFF      JMP 0A2666B9

0A2B4021   83BD 08FBFFFF 24 CMP DWORD PTR SS:[EBP-4F8],24
0A2B4028  ^0F84 29568DF6    JE 00B89657
0A2B402E  ^E9 F59CF6FF      JMP 0A21DD28

0A23CD74   83BD 08FBFFFF 24 CMP DWORD PTR SS:[EBP-4F8],24
0A23CD7B  ^0F8F 95C794F6    JG 00B89516
0A23CD81   E9 9B720700      JMP 0A2B4021

0A375F65   83BD 08FBFFFF 25 CMP DWORD PTR SS:[EBP-4F8],25
0A375F6C  ^0F84 F03681F6    JE 00B89662
0A375F72  ^E9 2277F6FF      JMP 0A2DD699

0A2DD699   83BD 08FBFFFF 26 CMP DWORD PTR SS:[EBP-4F8],26
0A2DD6A0  ^0F84 C7BF8AF6    JE 00B8966D
0A2DD6A6  ^E9 D62AE8FF      JMP 0A160181

0A160181   83BD 08FBFFFF 27 CMP DWORD PTR SS:[EBP-4F8],27
0A160188  ^0F84 EA94A2F6    JE 00B89678
0A16018E   E9 CD381700      JMP 0A2D3A60

0A2D3A60   83BD 08FBFFFF 28 CMP DWORD PTR SS:[EBP-4F8],28
0A2D3A67  ^0F84 165C8BF6    JE 00B89683
0A2D3A6D   E9 86740500      JMP 0A32AEF8

0A32AEF8   83BD 08FBFFFF 29 CMP DWORD PTR SS:[EBP-4F8],29
0A32AEFF  ^0F84 89E785F6    JE 00B8968E
0A32AF05  ^E9 88B2E1FF      JMP 0A146192



	*/

#pragma pack()

struct MiniBombReward
{
	DECLARE_ENUM(uint16, ScoreMin);
	DECLARE_ENUM(uint16, ScoreMax);
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
};

typedef std::vector<MiniBombReward*> MiniBombRewardList;

class MiniBomb
{
	SingletonInstance(MiniBomb);

	public:
		MiniBomb();
		virtual ~MiniBomb();

		void LoadReward();

		void Open(Player* pPlayer, uint8 * Packet);
		void SendCell(Player* pPlayer, uint8 type);
		void Generate(Player* pPlayer, uint8 starting_cell);

		void Start(Player* pPlayer, uint8 * Packet);
		void StartResult(Player* pPlayer, uint8 result);

		void Reward(Player* pPlayer, uint8 * Packet);
		void RewardResult(Player* pPlayer, uint8 result);

		void Reveal(Player* pPlayer, uint8 * Packet);
		void RevealProcess(Player* pPlayer, uint8 cell, uint16 & score);
		bool IsRevealed(Player* pPlayer) const;
		bool IsGenerated(Player* pPlayer) const;

		void Mark(Player* pPlayer, uint8 * Packet);

		void Finish(Player* pPlayer, bool from_game);

		uint8 GetRemainBombs(Player* pPlayer) const;
		uint8 GetFoundedBombs(Player* pPlayer) const;
		uint8 GetWrongBombs(Player* pPlayer) const;

		MiniBombReward const* GetReward(Player* pPlayer) const;

		DECLARE_BOOL(Active);
		MiniBombRewardList reward_list;
};

#endif