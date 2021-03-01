#ifndef MINING_MGR_H
#define MINING_MGR_H

#pragma once

#define MAX_MINING_STAGE 5
#define MAX_MINING_JEWEL_PACK 40

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//
#pragma pack(1)
struct PMSG_MINING_START_RECV
{
	PBMSG_HEAD2 h; // C1:4C:00
	uint16 index;
	uint16 monster_index;
	uint16 monster_class;
	uint8 map;
	uint8 stage;
};

struct PMSG_MINING_SUCCESS_RECV
{
	PBMSG_HEAD2 h; // C1:4C:01
	uint16 index;
	uint16 monster_index;
	uint16 monster_class;
	uint8 map;
	uint8 stage;
	uint8 result;
};

struct PMSG_MINING_FAILURE_RECV
{
	PBMSG_HEAD2 h; // C1:4C:03
	uint16 index;
	uint16 monster_index;
	uint8 map;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_MINING_START_SEND
{
	PMSG_MINING_START_SEND()
	{
		this->h.set(HEADCODE_MINING_SYSTEM, 0x00, sizeof(PMSG_MINING_START_SEND));
	}

	PBMSG_HEAD2 h;
	uint16 index;
	uint16 MonsterIndex;
	uint16 MonsterClass;
	uint8 map;
	uint8 stage;
	uint32 value;
	uint32 result;
};

struct PMSG_MINING_SUCCESS_SEND
{
	PMSG_MINING_SUCCESS_SEND()
	{
		this->h.set(HEADCODE_MINING_SYSTEM, 0x01, sizeof(PMSG_MINING_SUCCESS_SEND));
	}

	PBMSG_HEAD2 h;
	uint16 index;
	uint16 MonsterIndex;
	uint16 MonsterClass;
	uint8 map;
	uint8 stage;
	uint32 value;
};

struct PMSG_MINING_CANCEL
{
	PMSG_MINING_CANCEL()
	{
		this->h.set(HEADCODE_MINING_SYSTEM, 0x02, sizeof(PMSG_MINING_CANCEL));
	}

	PBMSG_HEAD2 h;
	uint16 index;
	uint16 MonsterIndex;
	uint16 MonsterClass;
	uint8 map;
	uint8 stage;
};
#pragma pack()
//**********************************************//
//**********************************************//
//**********************************************//

struct MiningInfo
{
	DECLARE_ENUM(uint16, Index);
	DECLARE_ENUM(uint8, Stage);
	DECLARE_ENUM(uint8, Value);
	DECLARE_ENUM(uint16, ItemRate);
	DECLARE_ENUM(uint16, ItemIndex);
	DECLARE_ENUM(uint16, ItemCount);
	DECLARE_ENUM(uint16, BonusItemRate);
	DECLARE_ENUM(uint16, BonusItemIndex);
	DECLARE_ENUM(uint16, BonusItemCount);
};

typedef std::vector<MiningInfo*> MiningInfoList;

class MiningSystem
{
	SingletonInstance(MiningSystem);

	public:
		MiningSystem();
		virtual ~MiningSystem();

		void Load();

		MiningInfo const* GetMiningInfo(uint16 index, uint8 stage) const;
		void CreateMiningReward(Player* pPlayer, uint16 item, int32 count);
		void CGMiningStartRecv(Player* pPlayer, uint8 * Packet);
		void CGMiningSuccessRecv(Player* pPlayer, uint8 * Packet);
		void CGMiningFailureRecv(Player* pPlayer, uint8 * Packet);
		void GCMiningStartSend(Player* pPlayer, uint16 monster_index, uint16 monster_class, uint8 map, uint8 stage, uint32 value, uint8 result);
		void GCMiningSuccessSend(Player* pPlayer, uint16 monster_index, uint16 monster_class, uint8 map, uint8 stage, uint8 result);
		void GCMiningFailSend(Player* pPlayer, uint16 monster_index, uint16 monster_class, uint8 map, uint8 stage);

		void Restore(Player* pPlayer);
		void UpdateMining(Unit* pUnit);
	private:
		MiningInfoList m_MiningInfo;
};

#endif