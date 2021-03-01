/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "DuelMgr.h"
*
*/

#ifndef DUELMGR_H
#define DUELMGR_H

enum DuelRoomStatus
{
	DUEL_ROOM_EMPTY,
	DUEL_ROOM_REQUESTED,
	DUEL_ROOM_STARTED,
	DUEL_ROOM_FINISHED,
};

static const struct DuelRoomInfo
{
	int32 dueler_gate[2];
	int32 watcher_gate;
} g_DuelRoomInfo[MAX_DUEL_ROOM] = {
	295, 296, 303,
	297, 298, 304,
	299, 300, 305,
	301, 302, 306
};

#define DUELING_SCORE				10

class DuelPlayer
{
	public:
		DECLARE_PTR(Player, Player);
		DECLARE_BOOL(Winner);
		DECLARE_PROPERTY(int32, Score);
		DECLARE_BOOL(Cancelled);

		void Reset()
		{
			this->SetPlayer(nullptr);
			this->SetWinner(false);
			this->SetScore(0);
			this->SetCancelled(false);
		}
};

class DuelRoom
{
	public:
		friend class DuelMgr;
		friend class Player;

		DuelRoom();

		void Clear();
		void UpdateTime();

		void StartTimer(uint32 time);

		void SetDuelers(Player* pDueler01, Player* pDueler02);
	
		void SetState_Empty();
		void SetState_Requested();
		void SetState_Started();
		void SetState_Finished();
	
		void ProcState_Requested();
		void ProcState_Started();
		void ProcState_Finished();

		void FinishedOutOfTime();
		void FinishedWinner(Player* pWinner, Player* pLooser);
		void FinishedLeftRoom(Player* pDesertor, bool cancelled);
		void UpdateSpectators();
		void UpdateDuelers();

		void CollectDuelersZen();
		void MoveDuelersIn();

		void AddSpectator(Player* pPlayer);
		void DelSpectator(Player* pPlayer);

		void SendScore();
		void SendLifeBarInit(Player* pPlayer);
		void SendLifeBarError(Player* pPlayer, uint8 result);
		void SendLifeBar();
		void SendSpectatorAdd(Player* pPlayer);
		void SendSpectatorDel(Player* pPlayer);
		void SendSpectatorList();

		bool IsInRoom(Player* pPlayer);

		uint8 SpectatorFreeSlot();

		void UpdateScore(Player* pPlayer);
		void DuelFinished(const char * winner, const char * looser);
		void DuelEnd(uint8 result = 0);
	private:
		DECLARE_ENUM(DuelRoomStatus, Status);
		DECLARE_ENUM(uint32, UpdateTick);
		DECLARE_ENUM(uint32, UpdateTime);

		bool TimePassed() const
		{
			return (GetTickCount() - this->GetUpdateTick()) > this->GetUpdateTime();
		}

		DECLARE_ARRAY_STRUCT_PTR(Player, Spectator, MAX_DUEL_SPECTATORS);

		DECLARE_ARRAY_STRUCT(DuelPlayer, Dueler, 2);
		DECLARE_PTR(Player, Winner);
		DECLARE_PTR(Player, Looser);

		DECLARE_ENUM(uint8, RoomID);
};

class DuelMgr
{
	SingletonInstance(DuelMgr);

	public:
		DuelMgr();
		DECLARE_ARRAY_STRUCT(DuelRoom, Room, MAX_DUEL_ROOM);

		void UpdateTime();

		DuelRoom * GetFreeRoom();

		void SendDuelRoomStatus(Player* pPlayer);
		void GenerateRespawn(Player* pPlayer, world_type & world, coord_type & x, coord_type & y);

		void ClassicScore(Player* pPlayer01, Player* pPlayer02);
		void ClassicEnd(Player* pPlayer01, Player* pPlayer02, uint8 result);
		void ClassicResult(Player* pWinner, Player* pLooser);
		void ClassicSendScore(Player* pPlayer01, Player* pPlayer02);
		void ClassicUpdate(Player* pPlayer);
		void ClassicCancel(Player* pPlayer01, Player* pPlayer02);
};

#endif