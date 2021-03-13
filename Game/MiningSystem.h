#ifndef MINING_MGR_H
#define MINING_MGR_H

#pragma once

#define MAX_MINING_STAGE 5
#define MAX_MINING_JEWEL_PACK 40

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//
#pragma pack(1)
struct MINING_START_REQUEST : C1_HEADER_SUB
{
	uint16 index;
	uint16 monster_index;
	uint16 monster_class;
	uint8 map;
	uint8 stage;
};

struct MINING_SUCCESS_REQUEST : C1_HEADER_SUB
{
	uint16 index;
	uint16 monster_index;
	uint16 monster_class;
	uint8 map;
	uint8 stage;
	uint8 result;
};

struct MINING_FAILURE_REQUEST : C1_HEADER_SUB
{
	uint16 index;
	uint16 monster_index;
	uint8 map;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct MINING_START_SEND : C1_HEADER_SUB
{
	MINING_START_SEND()
	{
		this->Set(HEADCODE_MINING_SYSTEM, 0x00, sizeof(MINING_START_SEND));
	}

	uint16 index;
	uint16 MonsterIndex;
	uint16 MonsterClass;
	uint8 map;
	uint8 stage;
	uint32 value;
	uint32 result;
};

struct MINING_SUCCESS_SEND : C1_HEADER_SUB
{
	MINING_SUCCESS_SEND()
	{
		this->Set(HEADCODE_MINING_SYSTEM, 0x01, sizeof(MINING_SUCCESS_SEND));
	}

	uint16 index;
	uint16 MonsterIndex;
	uint16 MonsterClass;
	uint8 map;
	uint8 stage;
	uint32 value;
};

struct MINING_CANCEL_SEND : C1_HEADER_SUB
{
	MINING_CANCEL_SEND()
	{
		this->Set(HEADCODE_MINING_SYSTEM, 0x02, sizeof(MINING_CANCEL_SEND));
	}

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