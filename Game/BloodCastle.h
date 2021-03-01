/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "BloodCastle.h"
*
*/
#ifndef BLOOD_CASTLE_H
#define BLOOD_CASTLE_H

struct BloodCastleSetting
{
	DECLARE_ENUM(int32, MaxPlayers);
	DECLARE_BOOL(PKCheck);
	DECLARE_ENUM(uint8, PKLevelMax);
	DECLARE_ENUM(int32, MonsterPerUser);
	DECLARE_ENUM(int32, MonsterLimit);
	DECLARE_ENUM(int32, BossPerUser);
	DECLARE_ENUM(int32, BossLimit);
	DECLARE_ENUM(int32, DoorLife);
	DECLARE_ENUM(int32, StatueLife);
	DECLARE_ENUM(int32, BonusDoorCleared);
	DECLARE_ENUM(int32, BonusStatueCleared);
	DECLARE_ENUM(int32, BonusWeaponDelivered);
	DECLARE_ENUM(int32, BonusRemainTime);
	DECLARE_ENUM(int32, ScoreQuestFinished);
	DECLARE_ENUM(int32, ScoreQuestFinishedParty);
	DECLARE_ENUM(int32, ScoreSurvive);
	DECLARE_ENUM(int32, ScoreDie);
	DECLARE_ENUM(int32, ScoreQuestFail);
	DECLARE_ENUM(uint32, ZenRewardDie);
	DECLARE_ENUM(uint32, ZenRewardSurvive);
	DECLARE_BOOL(RemoveElfSoldierBuff);
};

struct BloodCastlePlayer: public EventGroundPlayer
{
	private:
		void Init()
		{
			this->SetExperience(0);
			this->SetComplete(false);
			this->SetReward(0);
			this->SetDoorDestroyed(false);
			this->SetStatueDestroyed(false);
			this->SetCanGetReward(false);
			this->SetKillScore(0);
			this->SetReconnected(false);
			this->GetReconnectedTime()->Reset();
		}

		DECLARE_PROPERTY(int32, Experience);
		DECLARE_BOOL(Complete);
		DECLARE_ENUM(uint8, Reward);
		DECLARE_BOOL(DoorDestroyed);
		DECLARE_BOOL(StatueDestroyed);
		DECLARE_BOOL(CanGetReward);
		DECLARE_PROPERTY(int32, KillScore);
};

class BloodCastle: public EventGroundCommon
{
	friend class CBloodCastleMgr;

	public:
		BloodCastle();
		virtual ~BloodCastle();

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

		void ProcSubState_MonsterKill();
		void ProcSubState_DoorStandBy();
		void ProcSubState_DoorKill();
		void ProcSubState_BossKill();
		void ProcSubState_StatueKill();
		void ProcSubState_DeliverWeapon();

		void UpdatePlayerStatus();
		void UpdateStatus(uint8 status);
		void SendStandByStart();
		void SendStart();
		void SendMonsterTerminated();
		void SendBossTerminated();
		void ProcessMonsterKilled(Player* pKiller, bool boss);
		void ProcessDoorDestroyed(Player* pKiller, Monster* pGate);
		void ProcessStatueDestroyed(Player* pKiller, Monster* pStatue);

		void AddMonster(BloodCastleMonsterType type);
		void ClearMonster();

		void ReleaseBlockEntrance(bool block);
		void ReleaseBlockBridge(bool block);
		void ReleaseBlockDoor(bool block);
		void ReleaseBlockBossZone(bool block);
		void ReleaseBlockAltar(bool block);

		void CalculateMonsterKill();
		void AddDeliverWeaponExperience();
		void AddRemainTimeExperience();
		bool CheckPlayerComplete(Player* mPlayer);
		bool CanGetReward(Player* mPlayer);
		void SetPlayerComplete(Player* mPlayer);
		void SendFail(Player* mPlayer, bool die = false);
		void SendFail();
		void SendReward();
		void SendMessageCompleted();
		void SendNotice(NoticeType id, const char* msg, ...);

		DECLARE_STRUCT(BloodCastleSetting, Settings);

		void MonsterAddCount()
		{
			this->IncreaseMonsterCurrent(1);
			this->LimitMaxMonsterCurrent(this->GetMonsterMax());
		}

		void SendWorldStatus(Player* pPlayer);

		void UpdateWeaponStatus();
		void SetWeaponStatus(Player* pPlayer);
	private:
		DECLARE_PROPERTY(uint8, SubState);
		DECLARE_BOOL(DoorKilled);
		DECLARE_BOOL(StatueKilled);
		DECLARE_PROPERTY_PTR(Player, WeaponOwner);
		DECLARE_STRING_FIXED(WeaponOwnerName, MAX_CHARACTER_LENGTH + 1);
		DECLARE_ENUM(coord_type, WeaponOwnerX);
		DECLARE_ENUM(coord_type, WeaponOwnerY);
		
		DECLARE_PROPERTY(uint8, WeaponLevel);
		DECLARE_PROPERTY(uint32, Duration);
		DECLARE_PROPERTY(int32, MonsterMax);
		DECLARE_PROPERTY(int32, MonsterCurrent);
		DECLARE_STRUCT(TickTimer, DoorOpenTime);
};

class CBloodCastleMgr: public EventCommonMgr
{
	SingletonInstance(CBloodCastleMgr);

	public:
		CBloodCastleMgr();
		virtual ~CBloodCastleMgr();

		void LoadBloodCastleSetting();

		void StartGround(uint32 duration);
		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		DECLARE_ARRAY_STRUCT(BloodCastle, Ground, MAX_BLOOD_CASTLE_GROUND);

		void EnterRequest(Player * pPlayer, uint8 * Packet);
		void EnterResult(Player * pPlayer, uint8 result);

		bool IsIn(Player* pPlayer);
		bool HaveTicket(Player* pPlayer, uint8 ground) { return EventCommonMgr::HaveTicket(pPlayer, ITEMGET(13, 18), ITEMGET(13, 47), ground); }
};

#endif