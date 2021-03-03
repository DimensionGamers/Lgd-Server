/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ChaosCastle.h"
*
*/
#ifndef CHAOS_CASTLE_H
#define CHAOS_CASTLE_H

struct ChaosCastleSetting
{
	DECLARE_ENUM(int32, MinPlayers);
	DECLARE_ENUM(int32, MaxPlayers);
	DECLARE_BOOL(PKCheck);
	DECLARE_ENUM(uint8, PKLevelMax);
	DECLARE_ENUM(uint32, RequiredZen);
	DECLARE_ENUM(int32, ExperienceUser);
	DECLARE_ENUM(int32, ExperienceMonster);
	DECLARE_BOOL(RemoveElfSoldierBuff);
};

struct ChaosCastlePlayer: public EventGroundPlayer
{
	private:
		void Init()
		{
			this->SetMonsterKill(0);
			this->SetPlayerKill(0);
		}

		DECLARE_PROPERTY(int32, MonsterKill);
		DECLARE_PROPERTY(int32, PlayerKill);
};

class ChaosCastle: public EventGroundCommon
{
	friend class CChaosCastleMgr;

	public:
		ChaosCastle();
		virtual ~ChaosCastle();

		void Update();

		void SetState_None();
		void SetState_Open();
		void SetState_StandBy();
		void SetState_Playing();
		void SetState_Playend();

		void ProcState_None();
		void ProcState_Open();
		void ProcState_StandBy();
		void ProcState_Playing();
		void ProcState_Playend();

		void UpdatePlayerStatus();

		void RefundMoney();
		void SendStart();
		void SendState(uint8 state);

		void KillMonster() { this->ReduceMonsterCount(1); }
		void KillMonster(Player* pPlayer);
		void KillPlayer(Player* pPlayer);

		void AddMonster();

		void RestoreCastleZone();
		void UnsafetyCastleZone();
		void SendSafetyCastleZone(bool apply);
		void BlowObjects(int16 x, int16 y);
		void BlowObjectsFromPoint(Unit* mUnit, int16 x, int16 y);
		void KillPlayersOnDieTile();
		void UpdateTraps();
		void SetTrapHollow(uint8 trap);
		void DeleteItemsOnTrap(uint8 trap);
		void RewardExperience(ChaosCastlePlayer const* player, bool winner);
		ChaosCastlePlayer* GetWinner(bool strict = true);
		void SendFailToAll(ChaosCastlePlayer const* player);
		void GiveItemToWinner(Player* pPlayer, int32 score);
		DECLARE_STRUCT(ChaosCastleSetting, Settings);
		ChaosCastleItemDataList item_list;

		DECLARE_ARRAY_BOOL(TrapTriggered, MAX_CHAOS_CASTLE_TRAP_STEP);
		//DECLARE_ENUM(uint8, TrapStatus);
		DECLARE_PROPERTY(int32, MonsterCount);
		DECLARE_PROPERTY(int32, MonsterItemDrop);
		DECLARE_PROPERTY(int32, MonsterItemDropCounter);
};

class CChaosCastleMgr: public EventCommonMgr
{
	SingletonInstance(CChaosCastleMgr);

	public:
		CChaosCastleMgr();
		~CChaosCastleMgr();

		void LoadChaosCastleSetting();

		void StartGround(uint32 duration);
		void Update();
	
		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		DECLARE_ARRAY_STRUCT(ChaosCastle, Ground, MAX_CHAOS_CASTLE_GROUND);
		
		void EnterRequest(Player * pPlayer, uint8 * Packet);
		void EnterResult(Player * pPlayer, uint8 result);
		bool EnterAllowed(Player * pPlayer);
		bool IsIn(Player* pPlayer);
};

#endif