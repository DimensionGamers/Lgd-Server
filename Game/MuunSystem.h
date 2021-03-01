#ifndef MUUN_SYSTEM_H
#define MUUN_SYSTEM_H

#pragma once
#define MUUN_EVOLUTION_STONE			ITEMGET(16, 211)
#define MUUN_POWER_STONE				ITEMGET(16, 240)
#define MUUN_WIZARDY_STONE				ITEMGET(16, 239)

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_MUUN_ITEM_GET_RECV
{
	PMSG_MUUN_ITEM_GET_RECV()
	{
		this->h.set(0x4E, 0x00, sizeof(PMSG_MUUN_ITEM_GET_RECV));
	}

	PBMSG_HEAD2 h; // C3:4E:00
	uint8 index[2];
};

struct PMSG_MUUN_ITEM_USE_RECV
{
	PBMSG_HEAD2 h; // C1:4E:08
	uint8 source_slot;
	uint8 target_slot;
	uint8 type;
};
#pragma pack(1)
struct PMSG_MUUN_ITEM_SELL_RECV
{
	PBMSG_HEAD2 h; // C3:4E:09
	uint8 slot;
};
#pragma pack()
struct MUUN_MOUNT_RECV
{
	MUUN_MOUNT_RECV()
	{
		this->h.set(0x4E, 0x14, sizeof(MUUN_MOUNT_RECV));
		this->item = -1;
	}
	PBMSG_HEAD2 h;
	uint16 item;
};

struct MUUN_EXCHANGE_RECV
{
	PBMSG_HEAD2 h;
	uint8 type;
};

#pragma pack(1)
struct MUUN_SLOT_DISPLAY : C1_HEADER_SUB
{
	uint8 slot;
};
#pragma pack()

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//
#pragma pack(1)
struct PMSG_MUUN_ITEM_GET_SEND
{
	PMSG_MUUN_ITEM_GET_SEND()
	{
		this->h.set(0x4E, 0x00, sizeof(PMSG_MUUN_ITEM_GET_SEND));
	}

	PBMSG_HEAD2 h; // C3:4E:00
	uint8 junk1;
	uint8 result;
	uint8 junk2;
	uint8 item_info[MAX_ITEM_INFO];
};

struct PMSG_MUUN_ITEM_LIST_HEAD
{
	PWMSG_HEAD2 h; // C4:4E:02
	uint8 count;
};

struct PMSG_MUUN_ITEM_LIST_BODY
{
	uint8 slot;
	uint8 item_info[MAX_ITEM_INFO];
};

struct PMSG_MUUN_ITEM_DUR_SEND
{
	PMSG_MUUN_ITEM_DUR_SEND()
	{
		this->h.set(0x4E, 0x03, sizeof(PMSG_MUUN_ITEM_DUR_SEND));
	}

	PBMSG_HEAD2 h; // C1:4E:03
	uint8 slot;
	uint8 dur;
};

struct PMSG_MUUN_ITEM_DELETE_SEND
{
	PMSG_MUUN_ITEM_DELETE_SEND()
	{
		this->h.set(0x4E, 0x04, sizeof(PMSG_MUUN_ITEM_DELETE_SEND));
	}

	PBMSG_HEAD2 h; // C1:4E:04
	uint8 slot;
};

struct PMSG_MUUN_ITEM_MODIFY_SEND
{
	PMSG_MUUN_ITEM_MODIFY_SEND()
	{
		this->h.set(0x4E, 0x05, sizeof(PMSG_MUUN_ITEM_MODIFY_SEND));
	}

	PBMSG_HEAD2 h; // C1:4E:05
	uint8 flag;
	uint8 junk1;
	uint8 slot;
	uint8 junk2;
	uint8 item_info[MAX_ITEM_INFO];
};

struct PMSG_MUUN_ITEM_CHANGE_SEND
{
	PMSG_MUUN_ITEM_CHANGE_SEND()
	{
		this->h.set(0x4E, 0x06, sizeof(PMSG_MUUN_ITEM_CHANGE_SEND));
	}

	PBMSG_HEAD2 h; // C1:4E:06
	uint8 index[2];
	uint8 item_info[MAX_ITEM_INFO];
};

struct PMSG_MUUN_ITEM_STATUS_SEND
{
	PMSG_MUUN_ITEM_STATUS_SEND()
	{
		this->h.set(0x4E, 0x07, sizeof(PMSG_MUUN_ITEM_STATUS_SEND));
	}

	PBMSG_HEAD2 h; // C1:4E:07
	BYTE slot;
	BYTE status;
};

struct PMSG_MUUN_ITEM_USE_SEND
{
	PMSG_MUUN_ITEM_USE_SEND()
	{
		this->h.set(0x4E, 0x08, sizeof(PMSG_MUUN_ITEM_USE_SEND));
	}

	PBMSG_HEAD2 h; // C1:4E:08
	uint8 result;
	uint8 type;
};

struct PMSG_MUUN_ITEM_SELL_SEND
{
	PMSG_MUUN_ITEM_SELL_SEND()
	{
		this->h.set(0x4E, 0x09, sizeof(PMSG_MUUN_ITEM_SELL_SEND));
	}

	PBMSG_HEAD2 h; // C1:4E:09
	uint32 result;
	uint32 money;
};

struct MUUN_EXCHANGE_SEND
{
	MUUN_EXCHANGE_SEND(uint8 result)
	{
		this->h.set(0x4E, 0x13, sizeof(MUUN_EXCHANGE_SEND));
		this->result = result;
	}
	PBMSG_HEAD2 h;
	uint8 result;
};

struct MUUN_ATTACK_SEND
{
	MUUN_ATTACK_SEND(uint16 skill, uint16 source, uint16 target)
	{
		this->h.set(0x4E, 0x12, sizeof(MUUN_ATTACK_SEND));
		this->SetSkill(skill);
		this->SetSource(source);
		this->SetTarget(target);
	}
	PBMSG_HEAD2 h;
	INDEX_DATA(Skill);
	INDEX_DATA(Source);
	INDEX_DATA(Target);
};

struct MUUN_EXPIRE_DATE_HEAD
{
	PWMSG_HEAD2 h;
	uint8 result;
	uint16 count;
};

struct MUUN_EXPIRE_DATE_BODY
{
	uint32 item;
	uint32 time;
	uint32 slot;
};

struct MUUN_EXPIRE_DATE
{
	MUUN_EXPIRE_DATE()
	{
		this->h.set(0x4E, 0x21, sizeof(MUUN_EXPIRE_DATE));
		this->item = 0;
		this->slot = 0;
		this->time = 0;
	}

	PBMSG_HEAD2 h;
	uint16 item;
	uint16 slot;
	uint32 time;
};

struct MUUN_EXPIRE_UPDATE
{
	MUUN_EXPIRE_UPDATE()
	{
		this->h.set(0x4E, 0x22, sizeof(MUUN_EXPIRE_UPDATE));
		this->slot = 0;
		this->expire = 0;
	}
	PBMSG_HEAD2 h;
	uint32 slot;
	uint16 expire;
};

struct MUUN_DISPLAY_SLOT_VIEWPORT : C1_HEADER_SUB
{
	MUUN_DISPLAY_SLOT_VIEWPORT()
	{
		this->Set(0x4E, 0x23, sizeof(MUUN_DISPLAY_SLOT_VIEWPORT));
		this->SetIndex(-1);
		this->slot = 0;
	}

	INDEX_DATA(Index);
	uint8 slot;
};

struct MUUN_MOUNT_HEAD
{
	PWMSG_HEAD2 h;
	uint8 count;
};

struct MUUN_MOUNT_BODY
{
	INDEX_DATA(Index);
	INDEX_DATA(Mount);
	uint8 junk[2];
};

struct MUUN_MOUNT_SINGLE
{
	MUUN_MOUNT_SINGLE()
	{
		this->h.set(0x4E, 0x14, sizeof(MUUN_MOUNT_SINGLE));
		this->count = 1;
		this->data = 0;
		this->junk = 0;
	}

	PWMSG_HEAD2 h;
	uint8 count;
	INDEX_DATA(Index);
	INDEX_DATA(Mount);
	uint8 data;
	uint8 junk;
};
#pragma pack()
//**********************************************//
//**********************************************//
//**********************************************//

struct MuunSystemInfo
{
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Rank);
	DECLARE_ENUM(uint8, OptionIndex);
	DECLARE_ENUM(uint8, SpecialOptionType);
	DECLARE_ENUM(int32, SpecialOptionValue);
	DECLARE_ENUM(uint16, EvolutionItemIndex);
};

struct MuunSystemOptionInfo
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_PROPERTY_ARRAY(int32, OptionValue, 5);
	DECLARE_ENUM(int32, MaxOptionValue);
	DECLARE_ENUM(uint16, MapZone);
	DECLARE_ENUM(int32, PlayTime);
	DECLARE_ENUM(int32, DayOfWeek);
	DECLARE_ENUM(int32, MinHour);
	DECLARE_ENUM(int32, MaxHour);
	DECLARE_ENUM(int32, MinLevel);
	DECLARE_ENUM(int32, MaxLevel);
	DECLARE_ENUM(int32, MinMasterLevel);
	DECLARE_ENUM(int32, MaxMasterLevel);
};

struct MuunSystemEnergy
{
	DECLARE_ENUM(uint8, Rating);
	DECLARE_PROPERTY_ARRAY(uint8, Value, 5);
	DECLARE_ENUM(uint8, EvolutionValue);
};

struct MuunSystemExchange
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, Count);
	DECLARE_ENUM(uint16, Muun);
};

typedef std::map<uint16, MuunSystemInfo*> MuunSystemInfoMap;
typedef std::map<uint8, MuunSystemOptionInfo*> MuunSystemOptionInfoMap;
typedef std::map<uint8, MuunSystemEnergy*> MuunSystemEnergyMap;
typedef std::map<uint8, MuunSystemExchange*> MuunSystemExchangeMap;

class MuunSystem
{
	SingletonInstance(MuunSystem);

	public:
		MuunSystem();
		virtual ~MuunSystem();

		void Load();
		void LoadOption();
		void LoadEnergy();
		void LoadExchange();

		bool IsMuunItem(uint16 item) const;
		bool IsMuunItem(Item const* pItem) const;
		bool IsMuunUtil(uint16 item) const;
		bool IsMuunUtil(Item const* pItem) const;

		MuunSystemInfo const* GetMuunSystemInfo(uint16 index) const;
		MuunSystemOptionInfo const* GetMuunSystemOptionInfo(uint8 index) const;
		MuunSystemEnergy const* GetMuunSystemEnergyInfo(uint8 rating) const;
		MuunSystemExchange const* GetMuunSystemExchangeInfo(uint8 id) const;

		bool CheckSpecialOption(Player* pPlayer, MuunSystemInfo const* lpInfo) const;

		bool CheckItemMoveToMuunInventory(Player* pPlayer, Item* pItem, uint8 slot);
		void GetOptionValue(Player* pPlayer, Item const* pItem, MuunSystemInfo const* lpInfo, MuunSystemOptionInfo const* lpOption, int32 & value);
		void GetMuunItemSocketOption(Item & item);
		bool GetMuunUtilSocketOption(Item & item);
		bool GetMuunUtilSocketOption(Item * item);
		bool CharacterUseMuunLevelUp(Player* pPlayer, uint8 source_slot, uint8 target_slot);
		bool CharacterUseEvolutionStone(Player* pPlayer, uint8 source_slot, uint8 target_slot);
		bool CharacterUseJewelOfLife(Player* pPlayer, uint8 source_slot, uint8 target_slot);
		bool CharacterMuunEnergyConverter(Player* pPlayer, uint8 source_slot, uint8 target_slot);
		void MuunSprite(Player* pPlayer, int damage);
		void CalcMuunOption(Player* pPlayer, bool flag);
		void InsertOption(Player* pPlayer, MuunSystemInfo const* lpInfo, MuunSystemOptionInfo const* lpOption, bool flag, Item const* pItem);
		bool CGMuunItemGetRecv(Player* pPlayer, uint8 * Packet);
		void CGMuunItemUseRecv(Player* pPlayer, uint8 * Packet);
		void CGMuunItemSellRecv(Player* pPlayer, uint8 * Packet);
		void CGMuunMountRecv(Player* pPlayer, uint8 * Packet, bool from_packet = false);
		void CGMuunExchangeRecv(Player* pPlayer, uint8 * Packet);
		void SelectDisplayMuun(Player* pPlayer, uint8 * Packet);

		void GCMuunItemListSend(Player* pPlayer);
		void GCMuunItemDurSend(Player* pPlayer, uint8 slot, uint8 dur);
		void GCMuunItemDeleteSend(Player* pPlayer, uint8 slot);
		void GCMuunItemModifySend(Player* pPlayer, uint8 slot);
		void GCMuunItemChangeSend(Player* pPlayer, uint8 slot, bool remove);
		void GCMuunItemStatusSend(Player* pPlayer, uint8 slot, uint8 status);
		void GCMuunExchangeSend(Player* pPlayer, uint8 result);
		void GCMuunItemExpireSend(Player* pPlayer);
		void GCMuunItemExpireSend(Player* pPlayer, uint8 slot);
		void GCMuunItemExpireUpdate(Player* pPlayer, uint8 slot);

		void MuunAttack(Player* pPlayer);
		void ProcessMuunAttack(Player* pPlayer, Unit* pTarget);

		void CreateMuunList(Player* pPlayer, int32 start_count);

		void SetMuunBonus(Item & item);
	private:
		MuunSystemInfoMap m_MuunSystemInfo;
		MuunSystemOptionInfoMap m_MuunSystemOptionInfo;
		MuunSystemEnergyMap m_MuunSystemEnergy;
		MuunSystemExchangeMap m_MuunSystemExchange;

		DECLARE_BOOL(Active);
};

#endif