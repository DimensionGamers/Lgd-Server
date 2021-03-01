/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "IllusionTemple.h"
*
*/
#ifndef ILLUSION_TEMPLE_H
#define ILLUSION_TEMPLE_H

struct IllusionTempleSetting
{
	DECLARE_ENUM(int32, MinPlayers);
	DECLARE_ENUM(int32, MaxPlayers);
	DECLARE_BOOL(PKCheck);
	DECLARE_ENUM(uint8, PKLevelMax);

	DECLARE_ENUM(int32, DefaultExperience);
	DECLARE_ENUM(int32, RelicPointsExperience);
	DECLARE_ENUM(int32, AddKillPoints);
	DECLARE_ENUM(int32, MoveRelicsExperience);
	DECLARE_ARRAY_STRUCT(int32, PartyUserPoints, MAX_PARTY_MEMBERS);
};

struct IllusionTemplePlayer: public EventGroundPlayer
{
	private:
		void Init()
		{
		
		}

		DECLARE_ENUM(IllusionTempleTeam, Team);
};

class IllusionTemple: public EventGroundCommon
{
	friend class IllusionTempleMgr;

	public:
		IllusionTemple();
		virtual ~IllusionTemple();

		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Open);
		CREATE_STATE(StandBy);
		CREATE_STATE(Playing);
		CREATE_STATE(Playend);
	private:
		DECLARE_STRUCT(IllusionTempleSetting, Settings);
};

class IllusionTempleMgr: public EventCommonMgr
{
	SingletonInstance(IllusionTempleMgr);

	public:
		IllusionTempleMgr();
		virtual ~IllusionTempleMgr();

		void LoadIllusionTempleSettings();

		void StartGround(uint32 duration);
		void Update();

		CREATE_STATE(None);
		CREATE_STATE(Notify);
		CREATE_STATE(Open);
		CREATE_STATE(Playing);

		void EnterRequest(Player * pPlayer, uint8 * Packet);
		void EnterResult(Player * pPlayer, uint8 result);

		bool IsIn(Player* pPlayer);

		bool HaveTicket(Player* pPlayer, uint8 ground) { return EventCommonMgr::HaveTicket(pPlayer, ITEMGET(13, 51), ITEMGET(13, 61), ground); }
	private:
		DECLARE_ARRAY_STRUCT(IllusionTemple, Ground, MAX_ILLUSION_TEMPLE_GROUND);
};

#endif