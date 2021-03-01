#ifndef GREMORY_CASE_H
#define GREMORY_CASE_H

namespace GremoryCaseReward
{
	enum
	{
		NONE = 0,
		CHAOS_CASTLE = 1,
		BLOOD_CASTLE = 2,
		ILLUSION_TEMPLE = 3,
		DOPPELGANGER = 4,
		CHAOS_CASTLE_SURVIVAL = 5,
		TORMENTED_SQUARE = 6,
		ILLUSION_TEMPLE_2 = 7,
		CHAOS_CASTLE_SURVIVAL_2 = 8,
		TORMENTED_SQUARE_2 = 9,
		CHAOS_CASTLE_BATTLE = 10,
		TORMENTED_SQUARE_BATTLE = 11,
		EVOMON_REWARD = 12,
		DEVIL_SQUARE = 13,
		IMPERIAL_FORTRESS = 14,
		HYESUNG = 15,
		PROVIDE_ELEMENTAL_COMBINATION_SUCCESS_ITEM = 16,
		EVENT = 17,
		LABYRINTH_OF_DIMENSIONS = 18,
		QUEST = 19,
		MOBILE = 20,
		GM_REWARD = 100,
	};
};

#pragma pack(1)
struct GREMORY_CASE_ITEM_LIST_HEAD : C2_HEADER_SUB
{
	uint8 count;
};

struct GREMORY_CASE_ITEM_LIST_BODY
{
	/*+0*/ uint8 inventory;
	/*+1*/ uint8 reward_type;
	/*+2*/ uint8 unk1;
	/*+3*/ uint8 unk2;
	/*+4*/ uint32 serial;
	/*+8*/ uint8 item_info[MAX_ITEM_INFO];
	/*+20*/ uint32 slot;
	/*+24*/ uint32 time;
	/*+28*/ uint32 unk3;
	/*+32*/ char unk4[12];
};

struct GREMORY_CASE_ITEM_ADD : C1_HEADER_SUB
{
	GREMORY_CASE_ITEM_ADD()
	{
		this->Set(HEADCODE_GREMORY_CASE, SUBCODE_GREMORY_CASE_ITEM_ADD, sizeof(GREMORY_CASE_ITEM_ADD));
	}

	GREMORY_CASE_ITEM_LIST_BODY item;
};

struct GREMORY_CASE_LIST : C1_HEADER_SUB
{
	GREMORY_CASE_LIST(uint8 result)
	{
		this->Set(HEADCODE_GREMORY_CASE, SUBCODE_GREMORY_CASE_OPEN, sizeof(GREMORY_CASE_LIST));
		this->result = result;
	}
	uint8 result;

	/*
	0 = OK
	1 = Failed to load information
	*/
};

struct GREMORY_CASE_OPEN : C1_HEADER_SUB
{
	uint8 type;
};

struct GREMORY_CASE_OPEN_RESULT : C1_HEADER_SUB
{
	GREMORY_CASE_OPEN_RESULT(uint8 result)
	{
		this->Set(0xCD, 0x06, sizeof(GREMORY_CASE_OPEN_RESULT));
		this->result = result;
	}

	uint8 result;
};

struct GREMOERY_CASE_ITEM_USE_RESULT : C1_HEADER_SUB
{
	GREMOERY_CASE_ITEM_USE_RESULT()
	{
		this->Set(HEADCODE_GREMORY_CASE, SUBCODE_GREMORY_CASE_ITEM_USE, sizeof(GREMOERY_CASE_ITEM_USE_RESULT));
		this->result = 0;
		this->inventory = 0;
		this->item = 0;
		this->slot = 0;
		this->serial = 0;
		this->unk2 = 0;
		this->unk3 = 0;
	}
	/*+4*/ uint8 result;
	/*+5*/ uint8 inventory;
	/*+6*/ uint16 item;
	/*+8*/ uint16 unk2;
	/*+10*/ uint16 unk3;
	/*+12*/ uint32 serial;
	/*+16*/ uint32 slot;
	/*
	result 0 -> Item received
	result 1 -> There has been an error
	result 2 -> There has been an error
	result 3 -> A database error has ocurred
	result 4 -> Not enought space. Please check free space in your inventory
	result 5 -> Closing Gremory Case window as it cannot be used
	*/
};

struct GREMOERY_CASE_ITEM_REMOVE : C1_HEADER_SUB
{
	GREMOERY_CASE_ITEM_REMOVE()
	{
		this->Set(HEADCODE_GREMORY_CASE, SUBCODE_GREMORY_CASE_ITEM_REMOVE, sizeof(GREMOERY_CASE_ITEM_REMOVE));
		this->inventory = 0;
		this->unk1 = 0;
		this->item = 0;
		this->slot = 0;
		this->serial = 0;
		this->unk2 = 0;
		this->unk3 = 0;
	}

	/*+4*/ uint8 inventory;
	/*+5*/ uint8 unk1;
	/*+6*/ uint16 item;
	/*+8*/ uint16 unk2;
	/*+10*/ uint16 unk3;
	/*+12*/ uint32 serial;
	/*+16*/ uint32 slot;
};

struct GREMORY_CASE_NOTIFY : C1_HEADER_SUB
{
	GREMORY_CASE_NOTIFY(uint8 result)
	{
		this->Set(HEADCODE_GREMORY_CASE, SUBCODE_GREMORY_CASE_NOTIFY, sizeof(GREMORY_CASE_NOTIFY));
		this->result = result;
	}
	uint8 result;

	/*
	result 1 -> There's an item that is about to expire in your Gremory Case.
	result 2 -> There's not enough space in your Gremory Case. Please check.
	result 3 -> Gremory Case is full. If you recieve
	*/
};

struct GREMORY_CASE_ITEM_USE : C1_HEADER_SUB
{
	uint16 item;
	uint8 unk1;
	uint8 unk2;
	uint32 serial;
	uint32 slot;
	uint8 inventory;
};
#pragma pack()

class GremoryCase
{
	SingletonInstance(GremoryCase);

	public:
		GremoryCase();
		virtual ~GremoryCase();

		void OpenRequest(Player* pPlayer, uint8 * Packet);
		void OpenResult(Player* pPlayer, uint8 result);

		void ItemAdd(Player* pPlayer, uint8 inventory, uint8 slot);
		void ItemDel(Player* pPlayer, uint8 inventory, uint8 slot);
		
		void ItemUseRequest(Player* pPlayer, uint8 * Packet);
		void ItemUseResult(Player* pPlayer, uint8 result, uint8 inventory, uint8 slot);

		void ItemListRequest(Player* pPlayer, uint8 * Packet);
		void ItemListSend(Player* pPlayer, uint8 inventory);

		void SendNotification(Player* pPlayer, uint8 result);

		void Update(Player* pPlayer);
};

#endif