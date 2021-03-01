#ifndef JEWEL_BINGO_H
#define JEWEL_BINGO_H

enum
{
	JEWEL_BINGO_STATE_NONE				= 0,
	JEWEL_BINGO_STATE_JEWEL_PLACE		= 1,
	JEWEL_BINGO_STATE_BOX_SELECT		= 2,
	JEWEL_BINGO_STATE_PLAYING			= 3,
	JEWEL_BINGO_STATE_REWARD			= 4,
};

enum
{
	JEWEL_BINGO_REWARD_LOWER			= 0,
	JEWEL_BINGO_REWARD_INTERMEDIATE		= 1,
	JEWEL_BINGO_REWARD_ADVANCED			= 2,
	JEWEL_BINGO_REWARD_PREMIUM			= 3,
};

#pragma pack(1)
struct JEWEL_BINGO_STATE
{
	JEWEL_BINGO_STATE(uint8 state)
	{
		this->h.set(0x4D, 0x2A, sizeof(JEWEL_BINGO_STATE));
		this->state = state;
	}

	PBMSG_HEAD2 h;
	uint8 state;

	/*
	result:
	0x00 = Open
	0x01 = State 1
	0x02 = State 2
	0x03 = Box Select
	0x04 = Playing
	0x05 = State 5
	0x06 = State 6
	0x0E = Insufficient Card Deck
	*/
};

struct JEWEL_BINGO_MOVE_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 type;
	uint8 jewel_type;
	uint8 target_slot;
};

struct JEWEL_BINGO_INFO
{
	JEWEL_BINGO_INFO()
	{
		this->h.set(0x4D, 0x2B, sizeof(JEWEL_BINGO_INFO));
		this->result = 0;
		memset(this->grid, 0, JEWEL_BINGO_GRID_SIZE);
		memset(this->current_jewel, 0, JEWEL_BINGO_JEWEL_COUNT);
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 grid[JEWEL_BINGO_GRID_SIZE];
	uint8 current_jewel[JEWEL_BINGO_JEWEL_COUNT];
};

struct JEWEL_BINGO_BOX_SELECT
{
	JEWEL_BINGO_BOX_SELECT()
	{
		this->h.set(0x4D, 0x2C, sizeof(JEWEL_BINGO_BOX_SELECT));
	}

	PBMSG_HEAD2 h;
};

struct JEWEL_BINGO_BOX_SELECT_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 box;
};

struct JEWEL_BINGO_JEWEL_SELECT
{
	PBMSG_HEAD2 h;
	uint8 jewel_type;
	uint8 slot;
};

struct JEWEL_BINGO_PLAY_INFO
{
	JEWEL_BINGO_PLAY_INFO()
	{
		this->h.set(0x4D, 0x2D, sizeof(JEWEL_BINGO_PLAY_INFO));
		this->result = 0;
		memset(this->grid, 0, JEWEL_BINGO_GRID_SIZE);
		memset(this->matching_jewel, 0, JEWEL_BINGO_MATCHING_SIZE);
		this->current_jewel = 0;
		this->jewel_count = 0;
		this->current_box = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 grid[JEWEL_BINGO_GRID_SIZE];
	uint8 matching_jewel[JEWEL_BINGO_MATCHING_SIZE];
	uint8 current_jewel;
	uint8 jewel_count;
	uint8 current_box;
};

struct JEWEL_BINGO_PLAY_RESULT
{
	JEWEL_BINGO_PLAY_RESULT()
	{
		this->h.set(0x4D, 0x2E, sizeof(JEWEL_BINGO_PLAY_RESULT));
		memset(this->grid, 0, JEWEL_BINGO_GRID_SIZE);
		memset(this->matching_jewel, 0, JEWEL_BINGO_MATCHING_SIZE);
		this->score1 = 0;
		this->score2 = 0;
		this->score3 = 0;
	}

	PBMSG_HEAD2 h;
	uint8 grid[JEWEL_BINGO_GRID_SIZE];
	uint8 matching_jewel[JEWEL_BINGO_MATCHING_SIZE];
	uint8 unk1;
	uint16 score1;
	uint16 score2;
	uint16 score3;
};

struct JEWEL_BINGO_REWARD_REQUEST
{
	PBMSG_HEAD2 h;
};
#pragma pack()



/*
0A332C5B   83BD E4FAFFFF 2A CMP DWORD PTR SS:[EBP-51C],2A
0A332C62  ^0F84 61D18CF6    JE 00BFFDC9
0A332C68   E9 C7B70000      JMP 0A33E434

0A33E434   83BD E4FAFFFF 2B CMP DWORD PTR SS:[EBP-51C],2B
0A33E43B  ^0F84 93198CF6    JE 00BFFDD4
0A33E441   E9 ACF00400      JMP 0A38D4F2

0A38D4F2   83BD E4FAFFFF 2C CMP DWORD PTR SS:[EBP-51C],2C
0A38D4F9  ^0F84 E02887F6    JE 00BFFDDF
0A38D4FF  ^E9 6224F2FF      JMP 0A2AF966

0A2AF966   83BD E4FAFFFF 2D CMP DWORD PTR SS:[EBP-51C],2D
0A2AF96D  ^0F84 770495F6    JE 00BFFDEA
0A2AF973   E9 94910700      JMP 0A328B0C

0A328B0C   83BD E4FAFFFF 2E CMP DWORD PTR SS:[EBP-51C],2E
0A328B13  ^0F84 DC728DF6    JE 00BFFDF5
0A328B19  ^E9 7490FCFF      JMP 0A2F1B92
*/
struct JewelBingoReward
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(int32, ScoreMin);
	DECLARE_ENUM(int32, ScoreMax);
	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
};

typedef std::vector<JewelBingoReward*> JewelBingoRewardList;

class JewelBingo
{
	SingletonInstance(JewelBingo);

	public:
		JewelBingo();
		virtual ~JewelBingo();

		void LoadReward();

		void Open(Player* pPlayer);
		void Start(Player* pPlayer, uint8 * Packet);
		void Move(Player* pPlayer, uint8 * Packet);
		void BoxSelect(Player* pPlayer, uint8 * Packet);
		void JewelSelect(Player* pPlayer, uint8 * Packet);
		void Reward(Player* pPlayer, uint8 * Packet);

		void SendState(Player* pPlayer, uint8 state);
		void SendJewelState(Player* pPlayer, uint8 result);
		void SendBoxSelect(Player* pPlayer);
		void SendPlayInfo(Player* pPlayer);
		void SendPlayResult(Player* pPlayer);
		void SortJewels(Player* pPlayer);
		void GenerateRandomJewel(Player* pPlayer);
		bool CheckMatching(Player* pPlayer, uint8 slot, bool result = false);
		bool CheckMatchingV(Player* pPlayer, uint8 slot, bool result = false);
		bool CheckMatchingH(Player* pPlayer, uint8 slot, bool result = false);
		bool CheckMatchingD1(Player* pPlayer, uint8 slot, bool result = false);
		bool CheckMatchingD2(Player* pPlayer, uint8 slot, bool result = false);
		void GenerateNotMatchedScore(Player* pPlayer);
		JewelBingoReward const* GetReward(Player* pPlayer) const;
		
		DECLARE_BOOL(Active);
		JewelBingoRewardList reward_list;
};

#endif