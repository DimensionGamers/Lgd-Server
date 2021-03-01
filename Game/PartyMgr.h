/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PartyMgr.h"
*
*/
#ifndef PARTY_HANDLER_H
#define PARTY_HANDLER_H

class PartyMgr
{
	SingletonInstance(PartyMgr);

	public:
		PartyMgr();
		virtual ~PartyMgr();

		void LoadPartyList();

		void Update();
		void Delete(uint16 id);

		Party * Get(uint16 id);
		Party * GetEmpty();

		void OnLogin(Player * mPlayer);

		DECLARE_PROPERTY(uint32, PartyLoop);

		Party* GetPartyByLeader(const char * name);
		Party* GetPartyByMember(const char * name);

		Party* Create(Player* pLeader, Player* pMember);
		Party* Create(Player* pLeader);

		DECLARE_STRUCT_PTR(Party, PartyData);
};

#endif