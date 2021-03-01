/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Guild.cpp"
*
*/

Guild::Guild()
{
	this->SetID(0);
	this->SetAlliance(0);
	this->SetHostil(0);
	this->SetLifeStone(nullptr);

	this->ResetName();
	memset(this->emblem, 0, MAX_GUILD_EMBLEM_LENGTH);
	this->ResetNotice();

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		this->GetMember(i)->Reset();
	}

	this->SetScore(0);

	this->SetRegisteredInCaslteSiege(false);
	this->SetCastleSiegeMarks(0);
	this->GetWarData()->Reset();

	this->SetUpdateTick(0);
}

Guild::~Guild()
{

}

void Guild::UpdateAlliance(uint32 value)
{
	this->SetAlliance(value);

	Guild* pAlliance = sGuildMgr->GetGuild(this->GetAlliance());

	if ( !pAlliance )
	{
		this->SendNoticeToMembers(NOTICE_GLOBAL, "Alliance finished.");
	}
	else
	{
		this->SendNoticeArgToMembers(NOTICE_GLOBAL, "Joined %s alliance", pAlliance->GetName());
	}

	sLog->outInfo(LOG_GUILD, "[%u][%s] - Alliance changed to %u", 
		this->GetID(), this->GetName(), value);
}
	
void Guild::UpdateHostil(uint32 value)
{
	this->SetHostil(value);

	Guild* pHostil = sGuildMgr->GetGuild(this->GetHostil());

	if ( !pHostil )
	{
		this->SendNoticeToMembers(NOTICE_GLOBAL, "Hostility finished.");
	}
	else
	{
		this->SendNoticeArgToMembers(NOTICE_GLOBAL, "Hostility against %s started.", pHostil->GetName());
	}

	sLog->outInfo(LOG_GUILD, "[%u][%s] - Hostil changed to %u", 
		this->GetID(), this->GetName(), value);
}

uint8 Guild::GetTotalMembers()
{
	uint8 count = 0;

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !this->GetMember(i)->GetID() )
			continue;

		++count;
	}

	return count;
}

uint8 Guild::GetMemberRanking(Player const* pPlayer) const
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( this->GetMember(i)->GetID() != pPlayer->GetGUID() )
			continue;

		return this->GetMember(i)->GetRanking();
	}

	return GUILD_RANK_NONE;
}

void Guild::GetRankedMembersCount(int32 & asistant_count, int32 & battle_master_count)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !this->GetMember(i)->GetID() )
			continue;

		if ( this->GetMember(i)->GetRanking() == GUILD_RANK_ASISTANT )
		{
			asistant_count++;
		}
		if ( this->GetMember(i)->GetRanking() == GUILD_RANK_BATTLE_MASTER )
		{
			battle_master_count++;
		}
	}
}

void Guild::UpdateTime()
{
	this->UpdateLifeStone();
	this->UpdateRegisteredInSiege();

	Player* pPlayer = nullptr;

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !(pPlayer = this->GetMember(i)->GetPlayer()) )
			continue;

		if ( pPlayer->GetGuildID() == this->GetID() )
			continue;

		this->GetMember(i)->SetPlayer(nullptr);
	}

	if ( this->GetWarData()->GetUpdateTime()->Elapsed(IN_MILLISECONDS) )
	{
		for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
		{
			if ( !(pPlayer = this->GetMember(i)->GetPlayer()) )
				continue;

			if ( this->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE && 
				 this->GetWarData()->GetType() == GUILD_WAR_TYPE_NORMAL && 
				 this->GetWarData()->GetEnemy() )
			{
				if ( !this->GetMember(i)->IsWarInvolved() ) // Si no estoy activo en la Guild War reviso si debo estarlo
				{
					if ( pPlayer->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_WAR) )
					{
						sGuildWarMgr->SendStart(pPlayer, this->GetWarData()->GetEnemy()->GetName(), GUILD_WAR_TYPE_NORMAL, this->GetWarData()->GetTeam());
						sGuildWarMgr->SendScore(pPlayer, this->GetWarData()->GetScore(), this->GetWarData()->GetEnemy()->GetWarData()->GetScore());

						this->GetMember(i)->SetWarInvolved(true);
					}
				}
				else if ( !pPlayer->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_WAR) ) // Reviso si estoy en un mapa donde no se permita guild war
				{
					this->GetMember(i)->SetWarInvolved(false);
					sGuildWarMgr->SendEnd(pPlayer, this->GetName(), 7);
				}
			}
		}

		if ( this->GetWarData()->GetState() == GUILD_WAR_STATE_DECLARE )
		{
			if ( this->GetWarData()->TimeOut() )
			{
				this->GetWarData()->Reset();
			}
		}
		else if ( this->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE && this->GetWarData()->GetType() == GUILD_WAR_TYPE_NORMAL && this->GetWarData()->GetEnemy() )
		{
			pPlayer = this->GetMember(0)->GetPlayer();

			if ( !pPlayer || !pPlayer->Object::IsPlaying() ) // Guild Master OFF
			{
				sGuildWarMgr->EndWar(this, this->GetWarData()->GetEnemy(), BATTLE_SOCCER_END_REASON_LEFT);
			}
		}
	}
}

void Guild::UpdateRelationship()
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !this->GetMember(i)->GetID() )
			continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( pPlayer && pPlayer->Object::IsPlaying() )
		{
			pPlayer->GuildUpdateViewportList1();
			pPlayer->GuildUpdateViewportList2();
		}
	}

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetGuildID() != this->GetID() )
			continue;

		pPlayer->ViewportCreate(VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
	}
}

void Guild::UpdateLifeStone()
{
	Monster* pMonster = this->GetLifeStone();

	if ( !pMonster )
		return;

	if ( pMonster->IsSpawning() || pMonster->GetDeadStatus() == DEAD_READY )
		return;

	if ( !pMonster->IsPlaying() || !pMonster->IsLive() || !pMonster->IsAIGroup(CS_LIFE_STONE_AI) )
	{
		this->SetLifeStone(nullptr);
		return;
	}
}

void Guild::Create(uint32 id, const char * name, uint8 * emblem)
{
	this->SetID(id);
	this->ResetName();
	this->SetName(name, MAX_GUILD_NAME_LENGTH);
	memcpy(this->emblem, emblem, MAX_GUILD_EMBLEM_LENGTH);
}

void Guild::DisbandNotify()
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !this->GetMember(i)->GetID() )
			continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( pPlayer && pPlayer->Object::IsPlaying() )
		{
			pPlayer->SetGuildID(0);
			pPlayer->GuildUpdateViewportDel(i == 0);
			pPlayer->GuildMemberDelResult(1);
			pPlayer->SendNotice(NOTICE_GLOBAL, "Guild has been disbanded");
		}

		this->GetMember(i)->Reset();
	}
}

void Guild::AddMember(uint32 id, const char * name, uint8 ranking, uint8 slot, Player* pPlayer, uint16 server)
{
	this->GetMember(slot)->SetID(id);
	this->GetMember(slot)->SetName(name);
	this->GetMember(slot)->SetRanking(ranking);
	this->GetMember(slot)->SetServer(server);
	this->GetMember(slot)->SetPlayer(pPlayer);

	if ( pPlayer )
	{
		pPlayer->SetGuildID(this->GetID());
	}
}

void Guild::DelMember(uint32 id)
{
	for (uint8 i = 0; i < MAX_GUILD_MEMBER; ++i)
	{
		if (this->GetMember(i)->GetID() != id)
		{
			continue;
		}

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if (pPlayer && pPlayer->Object::IsPlaying())
		{
			pPlayer->SetGuildID(0);
			pPlayer->GuildUpdateViewportDel((i == 0));
			pPlayer->GuildMemberDelResult(1);
		}

		this->GetMember(i)->Reset();
	}
}

void Guild::SendChatToMembers(const char * name, const char * msg)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !this->GetMember(i)->GetPlayer() || !this->GetMember(i)->GetPlayer()->Object::IsPlaying() )
			continue;

		this->GetMember(i)->GetPlayer()->SendNoticeNormal(name, msg);
	}
}

void Guild::SendNoticeToMembers(NoticeType type, const char * msg)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer || !pPlayer->Object::IsPlaying() )
			continue;

		pPlayer->SendNoticeNormal(type, msg);
	}
}

void Guild::SendNoticeArgToMembers(NoticeType type, const char * msg, ...)
{
	ARG(buffer, msg);

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer || !pPlayer->Object::IsPlaying() )
			continue;

		pPlayer->SendNoticeNormal(type, buffer);
	}
}

void Guild::SendNotice(Player* pPlayer)
{
	if ( !strlen(this->GetNotice()) )
		return;

	pPlayer->SendNoticeNormal(NOTICE_GUILD, this->GetNotice());
}

void Guild::UpdateMemberRanking(const char * name, uint8 status)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !this->GetMember(i)->GetID() )
			continue;

		if ( memcmp(this->GetMember(i)->GetName(), name, MAX_CHARACTER_LENGTH) )
			continue;

		this->GetMember(i)->SetRanking(status);

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( pPlayer && pPlayer->Object::IsPlaying() )
		{
			pPlayer->GuildUpdateViewport();
		}
	}
}

void Guild::UpdateMemberRankingNotify(const char * name, uint8 status)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer || !pPlayer->Object::IsPlaying() )
			continue;

		if ( status == GUILD_RANK_ASISTANT )
		{
			pPlayer->SendNotice(NOTICE_GLOBAL, "%s's position will be changed to Assistant Guild Master.", name);
		}
		else if ( status == GUILD_RANK_BATTLE_MASTER )
		{
			pPlayer->SendNotice(NOTICE_GLOBAL, "%s's position will be changed to Battle Master.", name);
		}
		else
		{
			pPlayer->SendNotice(NOTICE_GLOBAL, "%s's position will be changed to Guild Member.", name);
		}
	}
}

void Guild::UpdateMemberStatus(uint32 guid, int32 server, const char * name)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( this->GetMember(i)->GetID() != guid )
			continue;

		this->GetMember(i)->SetName(name, MAX_CHARACTER_LENGTH + 1);
		this->GetMember(i)->SetServer(server);
		this->GetMember(i)->SetPlayer((server == -1 ? nullptr: sObjectMgr->FindPlayerByGUID(guid)));
		this->GetMember(i)->SetWarInvolved(false);
		return;
	}
}

bool Guild::LifeStoneTeleport(Player* pPlayer, coord_type & x, coord_type & y)
{
	if ( !this->GetLifeStone() || !this->GetLifeStone()->AIGroup(CS_LIFE_STONE_AI) )
		return false;

	LifeStoneBasicAI * pAI = AI_TO(LifeStoneBasicAI, this->GetLifeStone()->GetAI());

	if ( !pAI->IsCreated() )
		return false;

	x = this->GetLifeStone()->GetX();
	y = this->GetLifeStone()->GetY();

	return true;
}

void Guild::LifeStoneKill()
{
	if ( !this->GetLifeStone() )
		return;

	this->GetLifeStone()->Remove();
	this->SetLifeStone(nullptr);
}

void Guild::UpdateRegisteredInSiege()
{
	QueryCallBackProcess(_callbackRegisteredInSiege, CallBackRegisteredInSiege);

	if ( (GetTickCount() - this->GetUpdateTick()) > (MINUTE * IN_MILLISECONDS) )
	{
		this->SetUpdateTick(GetTickCount());

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_SELECT_REGISTERED);
		stmt->setUInt32(0, this->GetID());

		this->_callbackRegisteredInSiege = MuDatabase.AsyncQuery(stmt);
	}
}

void Guild::CallBackRegisteredInSiege(PreparedQueryResult result)
{
	this->SetRegisteredInCaslteSiege(!result ? false: true);
}

bool Guild::IsAllianceMaster()
{
	if ( !this->GetAlliance() )
		return false;

	return this->GetAlliance() == this->GetID();
}

GuildMember* Guild::GetMemberData(uint32 id)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( this->GetMember(i)->GetID() != id )
		{
			continue;
		}

		return this->GetMember(i);
	}

	return nullptr;
}

GuildMember* Guild::FindMember(const char* name, bool master)
{
	for (uint8 i = 0; i < MAX_GUILD_MEMBER; ++i)
	{
		if (!this->GetMember(i)->GetID())
		{
			continue;
		}

		if (this->GetMember(i)->GetRanking() == GUILD_RANK_MASTER && !master)
		{
			continue;
		}

		if (!memcmp(this->GetMember(i)->GetName(), name, MAX_CHARACTER_LENGTH))
		{
			return this->GetMember(i);
		}
	}

	return nullptr;
}