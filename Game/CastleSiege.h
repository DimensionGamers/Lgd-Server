/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "CastleSiege.h"
*
*/
#ifndef CASTLE_SIEGE_H
#define CASTLE_SIEGE_H

class CastleSiegeStateTime
{
	friend class CCastleSiege;

	public:
		void Reset()
		{
			this->day.set(0);
			this->hour.set(0);
			this->minute.set(0);
			this->SetStartSecond(0);
			this->SetEndSecond(0);
			this->SetDuration(0);
			this->SetInUse(false);
		}

		void SetMSec()
		{
			this->SetStartSecond((this->day.get() * DAY) + (this->hour.get() * HOUR) + (this->minute.get() * MINUTE));
			this->SetInUse(true);
		}

	private:
		DECLARE(uint8, day);
		DECLARE(uint8, hour);
		DECLARE(uint8, minute);
		DECLARE_ENUM(uint32, StartSecond);
		DECLARE_ENUM(uint32, EndSecond);
		DECLARE_ENUM(int32, Duration);
		DECLARE_BOOL(InUse);
};

class CastleSiegeNpcData
{
	public:
		explicit CastleSiegeNpcData()
		{
			this->monster.set(0);
			this->id.set(0);
			this->store_in_db.set(0);
			this->side.set(0);
			this->defense_level.set(0);
			this->regen_level.set(0);
			this->life_level.set(0);
			this->x.set(0);
			this->y.set(0);
			this->direction.set(0);
			this->current_life.set(0);
			this->SetActive(false);
		}

		DECLARE(uint16, monster);
		DECLARE(uint8, id);
		DECLARE(uint8, store_in_db);
		DECLARE(uint8, side);
		DECLARE(uint8, defense_level);
		DECLARE(uint8, regen_level);
		DECLARE(uint8, life_level);
		DECLARE(coord_type, x);
		DECLARE(coord_type, y);
		DECLARE(uint8, direction);
		DECLARE(int32, current_life);
		DECLARE_BOOL(Active);

}; typedef std::vector<CastleSiegeNpcData*> CastleSiegeNpcDataList;

class CastleSiegeNpcDefense
{
	friend class CCastleSiege;

	public:
		void Reset()
		{
			this->jog_count.set(0);
			this->required_zen.set(0);
			this->defense.set(0);
		}

		DECLARE(uint8, jog_count);
		DECLARE(uint32, required_zen);
		DECLARE(int32, defense);
};

class CastleSiegeNpcLife
{
	friend class CCastleSiege;

	public:
		void Reset()
		{
			this->jog_count.set(0);
			this->required_zen.set(0);
			this->life.set(0);
		}

		DECLARE(uint8, jog_count);
		DECLARE(uint32, required_zen);
		DECLARE(int32, life);
};

class CastleSiegeNpcRegen
{
	friend class CCastleSiege;

	public:
		void Reset()
		{
			this->jog_count.set(0);
			this->required_zen.set(0);
		}

		DECLARE(uint8, jog_count);
		DECLARE(uint32, required_zen);
};

class CastleSiegeGuildData
{
	public:
		CastleSiegeGuildData()
		{
			this->Reset();
		}
		virtual ~CastleSiegeGuildData()
		{

		}

		void Reset()
		{
			this->guid.set(0);
			this->marks.set(0);
			this->register_id.set(0);
			this->level.set(0);
			this->members.set(0);
			this->total_score.set(0);
			this->score.set(0);
			this->side.set(0);
		}

		DECLARE(uint32, guid);
		DECLARE(int32, marks);
		DECLARE(int32, register_id);
		DECLARE(int32, level);
		DECLARE(uint8, members);
		DECLARE(int64, total_score);
		DECLARE(int32, score);
		DECLARE(uint8, side);
};

class CastleSiegeGuildFinal
{
	public:
		explicit CastleSiegeGuildFinal() { this->Reset(); }
		virtual ~CastleSiegeGuildFinal() { }

		void Reset()
		{
			this->SetGuild(0);
			this->SetScore(0);
			this->SetSide(0);
		}

		DECLARE_ENUM(uint32, Guild);
		DECLARE_ENUM(int32, Score);
		DECLARE_ENUM(uint8, Side);
};

struct CastleSiegeMiniMapCoord
{
	uint8 x;
	uint8 y;
};

struct CastleSiegeMiniMapData
{
	explicit CastleSiegeMiniMapData(uint32 guild): pGuildMaster(nullptr), guild(guild)
	{
		this->Clear();
	}

	virtual ~CastleSiegeMiniMapData()
	{

	}

	void Clear()
	{
		this->points_count = 0;

		for ( int32 i = 0; i < CASTLE_SIEGE_MINI_MAP_POINT_MAX; i++ )
		{
			this->points[i].x = 0;
			this->points[i].y = 0;
		}
	}

	uint32 guild;
	Player* pGuildMaster;
	int32 points_count;
	CastleSiegeMiniMapCoord points[CASTLE_SIEGE_MINI_MAP_POINT_MAX];
};

struct CastleSiegeParticipant
{
	explicit CastleSiegeParticipant(uint32 id)
	{
		this->SetID(id);
		this->SetSeconds(0);
	}

	DECLARE_ENUM(uint32, ID);
	DECLARE_PROPERTY(int32, Seconds);
};

typedef std::map<uint32, CastleSiegeGuildFinal*> CastleSiegeGuildDataMap;
typedef std::vector<CastleSiegeGuildData> CastleSiegeGuildDataList;
typedef std::map<uint32, Player*> CastleSiegeMiniMapRequestMap;
typedef std::map<uint8, CastleSiegeMiniMapData*> CastleSiegeMiniMapDataMap;
typedef std::map<uint32, CastleSiegeParticipant*> CastleSiegeParticipantMap;

class CCastleSiege: public EventMassive
{
	SingletonInstance(CCastleSiege);

	public:
		CCastleSiege();
		virtual ~CCastleSiege();

		void LoadData();
		void ClearData();
		uint8 GetNextState(uint8 state);
		void CalculateCurrentState();

		Monster* GetMonster(uint16 npc, uint8 id) const;

		void LoadNPC(uint8 * Packet);
		void VerifyNPCExist();
		CastleSiegeNpcData * GetNpcData(uint16 npc, uint8 id);
		void SaveDBNpc();
		void CreateDBNpc();
		void CreateNonDBNpc();
		void ClearNonDBNpc();
		bool AddDBNpc(CastleSiegeNpcData* NpcData);
		void BlockReleaseCastleGate(coord_type x, coord_type y, bool block);
		void CloseCastleGate();

		CrownBasicAI* GetCrown() const;
		void SetCrownState(uint8 state);
		Player * GetCrownUser();
		void SetCrownAvailable(bool value);
		bool IsCrownAvailable() const;
		Player * GetCrownSwitchUser(uint8 Switch);
		CrownSwitchBasicAI* GetCrownSwitch(uint8 Switch) const;
		bool IsCrownUser(uint8 Switch);
		void ResetCrownUser();
		void ResetCrownSwitchUser(uint8 Switch);

		void LockCrown();
		void UnlockCrown();

		void RespawnPlayers(bool defense);
		void GetRespawnCoord(coord_type & x, coord_type & y, uint8 type);

		void UpdateTime();

		bool CastleOwnerMember(Player* mPlayer) const;
		bool CastleOwnerUnionMember(Player* mPlayer) const;
		uint8 GetTaxRateChaos(Player* mPlayer) const;
		void SendTaxRate(Player* mPlayer, uint8 type);
		int32 GetTaxRateStore(Player* mPlayer) const;
		int32 GetTaxRateHunt(Player* pPlayer) const;

		void AddTributeMoney(int32 money);
		void DeleteGuildRegister();
		void LoadGuildRegister();
		void LoadGuildRegisterResult(uint8 * packet);
		void AddGuildToFinalList(uint32 guild, int32 score, uint8 side);
		void ResetPlayerJoinSide();
		void ResetPlayerJoinSide(Player* pPlayer);
		void SetPlayerJoinSide(bool delete_life_stone);
		void CheckMiddleWinner();
		void ChangeWinnerGuild(uint8 side);
		bool ExistAttackerGuild();
		bool CheckResult();
		void SaveDBTaxRate();
		void UpdateDBMoney(int32 money, uint8 type, Player* pPlayer = nullptr);
		void ResetTaxRate();
		void SaveDBCastleOwner();
						
		bool GuardianStatueExist();

		void ChangeState(uint8 state, bool command = false, int32 duration = 0);

		void SetState_Idle_1();
		void SetState_RegisterGuild();
		void SetState_Idle_2();
		void SetState_RegisterMark();
		void SetState_Idle_3();
		void SetState_Notify();
		void SetState_Ready();
		void SetState_Start();
		void SetState_End();
		void SetState_EndCycle();

		void ProcState_Idle_1();
		void ProcState_RegisterGuild();
		void ProcState_Idle_2();
		void ProcState_RegisterMark();
		void ProcState_Idle_3();
		void ProcState_Notify();
		void ProcState_Ready();
		void ProcState_Start();
		void ProcState_End();
		void ProcState_EndCycle();

		void SendCrownAccessState(Player* pPlayer, uint8 state);
		void SendNotifyProgress(uint8 state, const char * guild);
		void SendSwitchState(Player* pPlayer, Monster* pSwitch, uint16 SwitchUser, uint8 state);
		void SendSwitchInfo();
		void SendSwitchInfo(Monster* pMonster);
		void SendStartState(uint8 state);
		void SendStartState(Player* pPlayer, uint8 state);
		void SendJoinSide(Player* pPlayer);
		void SendLeftTime();
		void SendLeftTime(Player* pPlayer);
		void SendMiniMap(Player* pPlayer, uint8 result);
		void SendGateState(Player* pPlayer, uint8 result, uint16 index);
		
		void StateRequest(Player* pPlayer);

		void RegisterGuildRequest(Player* pPlayer);
		void RegisterGuildResult(Player* pPlayer, uint8 result, const char * guild = nullptr);

		void GiveupGuildRequest(Player* pPlayer);
		void GiveupGuildResult(Player* pPlayer, uint8 result, uint8 giveup, const char * guild = nullptr);

		void RegisterGuildInfoRequest(Player* pPlayer);
		void RegisterGuildInfoResult(Player* pPlayer, uint8 result, const char * guild, uint32 mark, uint8 ranking);
		void RegisterGuildInfoCallBack(uint8 * packet);

		void RegisterMarkRequest(Player* pPlayer, uint8 * Packet);
		void RegisterMarkResult(Player* pPlayer, uint8 result, const char * guild, uint32 mark);

		void NpcBuyRequest(Player* pPlayer, uint8 * Packet);
		void NpcBuyResult(Player* pPlayer, uint8 result, int32 npc, int32 id);

		void NpcRepairRequest(Player* pPlayer, uint8 * Packet);
		void NpcRepairResult(Player* pPlayer, uint8 result, int32 npc, int32 id, int32 hp, int32 max_hp);

		void NpcUpgradeRequest(Player* pPlayer, uint8 * Packet);
		void NpcUpgradeResult(Player* pPlayer, uint8 result, int32 npc, int32 id, int32 type, int32 value);

		void TaxMoneyRequest(Player* pPlayer);
		void TaxRateRequest(Player* pPlayer, uint8 * Packet);

		void EnableHuntRequest(Player* pPlayer, uint8 * Packet);
		void EnableHuntResult(Player* pPlayer, uint8 result, uint8 allow);

		void RequestGuildList(Player* pPlayer);
		void RequestCastleOwnerMark(Player* pPlayer);
		void LandOfTrialsMoveRequest(Player* pPlayer, uint8 * Packet);
		void LandOfTrialsMoveResult(Player* pPlayer, uint8 result);

		void RequestGateOperate(Player* pPlayer, uint8 * Packet);
		void RequestGateOperateResult(Player* pPlayer, uint8 result, uint8 operation, uint16 gate);

		void SetTaxRate(int32 tax_hunt, uint8 tax_chaos, uint8 tax_store, bool hunt_allowed);

		void NpcListRequest(Player* pPlayer, uint8 * Packet);

		void GuildRegisterListRequest(Player* pPlayer);
		void GuildRegisterListCallBack(uint8 * packet);

		void MoneyOutRequest(Player* pPlayer, uint8 * Packet);
		void MoneyOutResult(Player* pPlayer, uint8 result, int64 money);

		void MachineUseRequest(Player* pPlayer, uint8 * Packet);
		void MachineUseGetCoord(uint8 type, uint8 target, coord_type &x, coord_type &y);
		void MachineDamage(Player* pPlayer, uint8 * Packet);

		// MINI MAP
		void MiniMapRequest(Player* pPlayer);
		void MiniMapClose(Player* pPlayer);
		void MiniMapAddPlayer(Player* pPlayer);
		void MiniMapDelPlayer(Player* pPlayer);
		void MiniMapCommandRequest(Player* pPlayer, uint8 * Packet);

		void MiniMapUpdate();

		bool CheckTeleportMagicAxisY(coord_type start_y, coord_type target_x, coord_type target_y) const;

		void UpdateStatus(Player* pPlayer);

		void RunTime(uint32 time)
		{
			this->SetUpdateTick(GetTickCount());
			this->SetUpdateTime(time);
			this->SetCurrentMinute(-1);
		}

		bool TimePassed() const
		{
			return (GetTickCount() - this->GetUpdateTick()) > this->GetUpdateTime();
		}

		uint32 GetRemain() const
		{
			uint32 cur_time = GetTickCount();
			uint32 diff = cur_time - this->GetUpdateTick();

			if ( diff < this->GetUpdateTime() )
			{
				diff = this->GetUpdateTime() - diff;
			}
			else
			{
				diff = 0;
			}

			return diff;
		}

		bool IsMinuteElapsed(int32 period)
		{
			if ( !this->GetRemain() || (this->GetRemain() / (IN_MILLISECONDS * MINUTE)) == this->GetCurrentMinute() )
				return false;

			this->SetCurrentMinute(this->GetRemain() / (IN_MILLISECONDS * MINUTE));

			if ( (this->GetCurrentMinute() % period) != 0 )
				return false;

			if ( (this->GetCurrentMinute() + 1) <= 0 )
				return false;

			return true;
		}

		void InsertRanking();
		void UpdateCharacterCount();

		void UpdateParticipants();
		void GiveParticipantsRewards();
	private:
		DECLARE_ENUM(uint8, State);
		DECLARE(bool, command);
		DECLARE_BOOL(ListLoaded);
		DECLARE_BOOL(NpcLoaded);
		DECLARE_BOOL(FirstRun);
		
		DECLARE_STRUCT(TickTimer, SaveNpcTime);
		DECLARE_STRUCT(TickTimer, MiniMapTime);
		DECLARE_STRUCT(TickTimer, LeftTime);
		DECLARE_STRUCT(TickTimer, UpdateSideTime);
		DECLARE_STRUCT(TickTimer, SwitchInfoTime);
		

		DECLARE_ENUM(uint8, TaxRateChaos);
		DECLARE_ENUM(uint8, TaxRateStore);
		DECLARE_PROPERTY(int64, TributeMoney);
		DECLARE_ENUM(int32, TaxRateHunt);
		DECLARE_BOOL(HuntEnabled);

		DECLARE_ARRAY_STRUCT(CastleSiegeStateTime, StateTimeData, CASTLE_SIEGE_STATE_MAX);
		CastleSiegeNpcDataList m_npc_data;
		CastleSiegeNpcDefense m_npc_defense[CASTLE_SIEGE_NPC_UPGRADE_MAX][CASTLE_SIEGE_NPC_MAX_DEFENSE_LEVEL];
		CastleSiegeNpcLife m_npc_life[CASTLE_SIEGE_NPC_UPGRADE_MAX][CASTLE_SIEGE_NPC_MAX_LIFE_LEVEL];
		CastleSiegeNpcRegen m_npc_regen[CASTLE_SIEGE_NPC_MAX_REGEN_LEVEL];
		DECLARE_PROPERTY_ARRAY(uint32, NpcBuyPrice, CASTLE_SIEGE_NPC_UPGRADE_MAX);

		DECLARE_ENUM(uint32, CastleOwner);
		DECLARE_ENUM(uint32, MiddleOwner);
		DECLARE_BOOL(Occupied);

		DECLARE_ARRAY_STRUCT(CastleSiegeGuildFinal, BasicGuildData, CASTLE_SIEGE_JOIN_SIDE_MAX);

		CastleSiegeGuildDataMap m_guild_data;
		CastleSiegeMiniMapRequestMap m_mini_map_request;
		CastleSiegeMiniMapDataMap m_mini_map_data;

		DECLARE_ENUM(uint32, UpdateTick);
		DECLARE_ENUM(uint32, UpdateTime);
		DECLARE_ENUM(int32, CurrentMinute);

		CastleSiegeParticipantMap m_CastleSiegeParticipantMap;
};

#endif