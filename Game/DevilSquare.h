/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "DevilSquare.h"
*
*/
#ifndef DEVIL_SQUARE_H
#define DEVIL_SQUARE_H

struct DevilSquareSetting
{
	DECLARE_ENUM(int32, Experience);
	DECLARE_ENUM(int32, MaxPlayers);
	DECLARE_BOOL(PKCheck);
	DECLARE_ENUM(uint8, PKLevelMax);
	DECLARE_BOOL(RemoveElfSoldierBuff);
};

struct devil_square_ranking
{
	DECLARE(int32, experience);
	DECLARE(uint32, zen);
};

struct DevilSquarePlayer: public EventGroundPlayer
{
	private:
		void Init()
		{
			this->SetScore(0);
		}

		DECLARE_PROPERTY(int32, Score);
};

typedef std::map<uint8, devil_square_ranking*> DevilSquareRankingMap;

class DevilSquare: public EventGroundCommon
{
	friend class DevilSquareMgr;

	public:
		DevilSquare();
		virtual ~DevilSquare();

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
		void SendRemainMinutes();

		void BubbleSort();
		void CalculatePlayerScore();
		
		devil_square_ranking * GetRankingData(uint8 position) const;

		void AddMonster();
		void ClearMonster();

		DevilSquarePlayer * FindPlayer(Player* pPlayer);

		DECLARE_STRUCT(DevilSquareSetting, Settings);

	private:
		DECLARE_STRUCT_PTR(DevilSquarePlayer, Ranking);
		DevilSquareRankingMap ranking;
};

class DevilSquareMgr: public EventCommonMgr
{
	SingletonInstance(DevilSquareMgr);

	public:
		DevilSquareMgr();
		virtual ~DevilSquareMgr();

		void LoadDevilSquareSetting();
		void LoadDevilSquareRanking();

		void StartGround(uint32 duration);
		void Update();
	
		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		DECLARE_ARRAY_STRUCT(DevilSquare, Ground, MAX_DEVIL_SQUARE_GROUND);

		void SendOpen();
		void SendStart();

		void EnterRequest(Player * pPlayer, uint8 * Packet);
		void EnterResult(Player * pPlayer, uint8 result);
		bool IsIn(Player* pPlayer);

		bool HaveTicket(Player* pPlayer, uint8 ground) { return EventCommonMgr::HaveTicket(pPlayer, ITEMGET(14, 19), ITEMGET(13, 46), ground); }
};

#endif