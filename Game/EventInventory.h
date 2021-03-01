#ifndef EVENT_INVENTORY_H
#define EVENT_INVENTORY_H

enum EventInventoryFlag
{
	EVENT_INVENTORY_TYPE_NONE			= 0,
	EVENT_INVENTORY_TYPE_BATTLE_CORE	= 1,
	EVENT_INVENTORY_TYPE_EGG			= 2,
	EVENT_INVENTORY_TYPE_EVOMON			= 3,
	EVENT_INVENTORY_TYPE_LUNAR_RABBIT	= 4,
	EVENT_INVENTORY_TYPE_CHRISTMAS		= 5,
	EVENT_INVENTORY_TYPE_NEW_YEAR		= 6,
	EVENT_INVENTORY_TYPE_MU_ROOMY		= 7,
	EVENT_INVENTORY_TYPE_FIND_BOMB		= 8,
	EVENT_INVENTORY_TYPE_JEWEL_BINGO	= 9,
	EVENT_INVENTORY_TYPE_BLOSSOM		= 10,
	EVENT_INVENTORY_TYPE_BASEBALL		= 11,
};

#pragma once

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_EVENT_ITEM_GET_RECV
{
	PMSG_EVENT_ITEM_GET_RECV()
	{
		this->h.set(0x4D, 0x00, sizeof(PMSG_EVENT_ITEM_GET_RECV));
	}
	PBMSG_HEAD2 h; // C3:4D:00
	uint8 index[2];
};

struct PMSG_EVENT_ITEM_DROP_RECV
{
	PBMSG_HEAD2 h; // C3:4D:01
	uint8 x;
	uint8 y;
	uint8 slot;
};

struct PMSG_EVENT_INVENTORY_OPEN_RECV
{
	PBMSG_HEAD2 h; // C3:4D:0F
	uint8 id;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//
#pragma pack(1)
struct PMSG_EVENT_ITEM_GET_SEND
{
	PMSG_EVENT_ITEM_GET_SEND()
	{
		this->h.set(0x4D, 0x00, sizeof(PMSG_EVENT_ITEM_GET_SEND));
	}

	PBMSG_HEAD2 h; // C3:4D:00
	uint8 junk1;
	uint8 result;
	uint8 indexH;
	uint8 junk2;
	uint8 indexL;
	uint8 item_info[MAX_ITEM_INFO];

	void SetIndex(uint16 index)
	{
		this->indexH = SET_NUMBERH(index);
		this->indexL = SET_NUMBERL(index);
	}
};

struct PMSG_EVENT_ITEM_DROP_SEND
{
	PMSG_EVENT_ITEM_DROP_SEND()
	{
		this->h.set(0x4D, 0x01, sizeof(PMSG_EVENT_ITEM_DROP_SEND));
	}

	PBMSG_HEAD2 h; // C1:4D:01
	uint8 result;
	uint8 slot;
};

struct PMSG_EVENT_ITEM_LIST_HEAD
{
	PWMSG_HEAD2 h; // C4:4D:02
	uint8 count;
};

struct PMSG_EVENT_ITEM_LIST_BODY
{
	uint8 slot;
	uint8 item_info[MAX_ITEM_INFO];
};

struct PMSG_EVENT_ITEM_DUR_SEND
{
	PMSG_EVENT_ITEM_DUR_SEND()
	{
		this->h.set(0x4D, 0x03, sizeof(PMSG_EVENT_ITEM_DUR_SEND));
	}

	PBMSG_HEAD2 h; // C1:4D:03
	uint8 slot;
	uint8 dur;
};

struct PMSG_EVENT_ITEM_DELETE_SEND
{
	PMSG_EVENT_ITEM_DELETE_SEND()
	{
		this->h.set(0x4D, 0x04, sizeof(PMSG_EVENT_ITEM_DELETE_SEND));
	}

	PBMSG_HEAD2 h; // C1:4D:04
	uint8 slot;
};

struct PMSG_EVENT_ITEM_MODIFY_SEND
{
	PMSG_EVENT_ITEM_MODIFY_SEND()
	{
		this->h.set(0x4D, 0x05, sizeof(PMSG_EVENT_ITEM_MODIFY_SEND));
	}

	PBMSG_HEAD2 h; // C1:4D:05
	uint8 junk1;
	uint8 slot;
	uint8 junk2;
	uint8 item_info[MAX_ITEM_INFO];
};

struct PMSG_EVENT_ITEM_CHANGE_SEND
{
	PMSG_EVENT_ITEM_CHANGE_SEND()
	{
		this->h.set(0x4D, 0x06, sizeof(PMSG_EVENT_ITEM_CHANGE_SEND));
	}

	PBMSG_HEAD2 h; // C1:4D:06
	uint8 junk1;
	uint8 slot;
	uint8 junk2;
	uint8 item_info[MAX_ITEM_INFO];
};

struct PMSG_EVENT_INVENTORY_OPEN_SEND
{
	PMSG_EVENT_INVENTORY_OPEN_SEND()
	{
		this->h.set(0x4D, 0x0F, sizeof(PMSG_EVENT_INVENTORY_OPEN_SEND));
		this->result = 0;
		this->id = 0;
		memset(this->time, 0, 4);
		this->data = 0;
	}

	PBMSG_HEAD2 h; // C1:4D:0F
	uint8 result;
	uint8 id;
	uint8 time[4];
	uint8 data;
};

struct PMSG_EVENT_INVENTORY_ENABLE_SEND
{
	PMSG_EVENT_INVENTORY_ENABLE_SEND()
	{
		this->h.set(0x4D, 0x18, sizeof(PMSG_EVENT_INVENTORY_ENABLE_SEND));
	}

	PBMSG_HEAD2 h; // C1:4D:18
	uint32 result;
};
#pragma pack()
//**********************************************//
//**********************************************//
//**********************************************//

class EventInventory
{
	SingletonInstance(EventInventory);

	public:
		EventInventory();
		virtual ~EventInventory();

		bool EventInventoryInsertItemStack(Player* pPlayer, Item * pItem);
		bool EventInventoryAddItemStack(Player* pPlayer, uint8 source_slot, uint8 target_slot);
		bool CGEventItemGetRecv(Player* pPlayer, uint8 * Packet);
		void CGEventItemDropRecv(Player* pPlayer, uint8 * Packet);
		void CGEventInventoryOpenRecv(Player* pPlayer, uint8 * Packet);
		void GCEventItemListSend(Player* pPlayer);
		void GCEventItemDurSend(Player* pPlayer, uint8 slot, uint8 dur);
		void GCEventItemDeleteSend(Player* pPlayer, uint8 slot);
		void GCEventItemModifySend(Player* pPlayer, uint8 slot);
		//void GCEventItemChangeSend(Player* pPlayer, uint8 slot);
		void GCEventInventoryEnableSend(Player* pPlayer);
		void GCEventInventoryExpireSend(Player* pPlayer);
		void GCEventInventoryExpireSend(Player* pPlayer, uint8 slot);

		void DecreaseItemDurabilityByUse(Player* pPlayer, uint8 slot, float durability);
};

#endif