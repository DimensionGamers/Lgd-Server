#ifndef NUMERIC_BASEBALL_H
#define NUMERIC_BASEBALL_H

#pragma pack(1)
struct NUMERIC_BASEBALL_START : public C1_HEADER_SUB
{
	NUMERIC_BASEBALL_START()
	{
		this->Set(0x4D, 0x30, sizeof(NUMERIC_BASEBALL_START));
		this->result = 0;
	}

	uint8 result;
};

struct NUMERIC_BASEBALL_NUMBER_SELECT : public C1_HEADER_SUB
{
	uint8 numbers[NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW];
};

struct NUMERIC_BASEBALL_0x4D_0x31 : public C1_HEADER_SUB
{
	NUMERIC_BASEBALL_0x4D_0x31()
	{
		this->Set(0x4D, 0x31, sizeof(NUMERIC_BASEBALL_0x4D_0x31));
		this->result = 0;
	}

	uint8 result;
};

struct NUMERIC_BASEBALL_OPEN : public C1_HEADER_SUB
{
	NUMERIC_BASEBALL_OPEN()
	{
		this->Set(0x4D, 0x33, sizeof(NUMERIC_BASEBALL_OPEN));
		this->result = 0;
		this->junk = 0;
		this->score = 0;
		memset(this->strikes, 0, NUMERIC_BASEBALL_ROWS);
		memset(this->ball, 0, NUMERIC_BASEBALL_ROWS);
		memset(this->numbers, 0xFF, NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW);
	}

	/*
	0
	1
	2
	*/

	uint8 result;
	uint8 junk;
	uint16 score;
	uint8 strikes[NUMERIC_BASEBALL_ROWS];
	uint8 ball[NUMERIC_BASEBALL_ROWS];
	uint8 numbers[NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW];
};

struct NUMERIC_BASEBALL_RESULT : public C1_HEADER_SUB
{
	NUMERIC_BASEBALL_RESULT()
	{
		this->Set(0x4D, 0x34, sizeof(NUMERIC_BASEBALL_RESULT));
		this->result = 0;
		memset(this->strikes, 0, NUMERIC_BASEBALL_ROWS);
		memset(this->ball, 0, NUMERIC_BASEBALL_ROWS);
		memset(this->numbers, 0, NUMERIC_BASEBALL_NUMBERS_PER_ROW);
		this->data4 = 0;
		this->junk = 0;
		this->score = 0;
	}

	uint8 result;
	uint8 strikes[NUMERIC_BASEBALL_ROWS];
	uint8 ball[NUMERIC_BASEBALL_ROWS];
	uint8 numbers[NUMERIC_BASEBALL_NUMBERS_PER_ROW];
	uint8 data4;
	uint8 junk;
	uint16 score;
};
#pragma pack()

struct NumericBaseballReward
{
	DECLARE_ENUM(int32, ScoreMin);
	DECLARE_ENUM(int32, ScoreMax);
	DECLARE_ENUM(uint16, Item);
};

typedef std::vector<NumericBaseballReward*> NumericBaseballRewardList;

class NumericBaseball
{
	SingletonInstance(NumericBaseball);

	public:
		NumericBaseball();
		virtual ~NumericBaseball();

		void LoadReward();

		void Open(Player* pPlayer);
		void Start(Player* pPlayer, uint8 * Packet);
		void SelectNumber(Player* pPlayer, uint8 * Packet);

		void SendState(Player* pPlayer, uint8 result);
		void SendPlayInfo(Player* pPlayer);
		void SendPlayResult(Player* pPlayer, uint8 result);

		NumericBaseballReward const* GetReward(Player* pPlayer) const;
		
		DECLARE_BOOL(Active);
		NumericBaseballRewardList m_NumericBaseballRewardList;
};

#endif