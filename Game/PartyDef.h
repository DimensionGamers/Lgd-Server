/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PartyDef.h"
*
*/

#ifndef PARTYDEF_H
#define PARTYDEF_H

#define MAX_PARTY_MEMBERS							5
#define MAX_PARTY_MONSTER_KILL_DISTANCE				15
#define MAX_PARTY_SKILL_EFFECT_DISTANCE				10
#define MAX_PARTY_QUEST_SUPPORT_UPDATE_DISTANCE		25
#define MAX_PARTY_GEN_KILL_DISTANCE					25
#define PARTY_NULL									uint16(-1)

enum PartyMemberStatus
{
	PARTY_USER_FLAG_NONE,
	PARTY_USER_FLAG_PLAYING,
	PARTY_USER_FLAG_OFFLINE,
	MAX_PARTY_USER_FLAG,
};

enum PartyStatus
{
	PARTY_NONE,
	PARTY_STANDBY,
	PARTY_ACTIVE,
	PARTY_DELETE,
	MAX_PARTY_STATUS
};

#define PARTY_MEMBER_LOOP \
	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i ) \
		if ( this->GetMember(i)->GetStatus() == PARTY_USER_FLAG_PLAYING ) \
			PLAYER_POINTER(this->GetMember(i)->GetPlayer())

#define PARTY_LOOP(loop) for ( uint8 loop = 0; loop < MAX_PARTY_MEMBERS; ++loop )

#endif