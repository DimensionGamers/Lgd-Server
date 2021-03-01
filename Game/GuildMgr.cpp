/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GuildMgr.cpp"
*
*/

CGuildMgr::CGuildMgr()
{

}

CGuildMgr::~CGuildMgr()
{
	MAP_CLEAR(GuildMap::iterator, this->m_GuildMap);
}

void CGuildMgr::LoadGuild()
{
	uint32 oldMSTime = getMSTime();

	sLog->outInfo(LOG_DEFAULT, "Loading guilds...");

	PreparedQueryResult result = MuDatabase.Query(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_SELECT));

	uint32 count = 0;

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			Guild* pGuild = new Guild;
			pGuild->SetID(fields[0].GetUInt32());
			pGuild->SetName(fields[1].GetBase64String().c_str());
			
			const char* data = fields[2].GetCString();

			if ( data )
			{
				Tokens tokens(data, ' ');

				for ( uint8 i = 0; i < MAX_GUILD_EMBLEM_LENGTH; ++i )
					pGuild->emblem[i] = cast(uint8, atoi(tokens[i]));
			}

			pGuild->SetNotice(fields[3].GetBase64String().c_str());
			pGuild->SetAlliance(fields[4].GetUInt32());
			pGuild->SetHostil(fields[5].GetUInt32());
			pGuild->SetScore(fields[6].GetUInt32());
			pGuild->SetCastleSiegeMarks(fields[7].GetUInt32());
			pGuild->SetRegisteredInCaslteSiege(fields[8].GetUInt32() ? true: false);

			this->AddGuild(pGuild);

			++count;
		}
		while (result->NextRow());
	}

	this->LoadGuildMembers();

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u guilds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CGuildMgr::LoadGuildMembers()
{
	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_GUILD_MEMBER_SELECT);

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			uint32 guild = fields[0].GetUInt32();

			Guild* pGuild = this->GetGuild(guild);

			if (!pGuild)
			{
				continue;
			}

			uint8 slot = fields[1].GetUInt8();

			if (slot >= MAX_GUILD_MEMBER)
			{
				continue;
			}

			pGuild->GetMember(slot)->SetID(fields[2].GetUInt32());
			pGuild->GetMember(slot)->SetRanking(fields[3].GetUInt8());
			pGuild->GetMember(slot)->SetName(fields[4].GetCString());
			pGuild->GetMember(slot)->SetServer(fields[5].GetBool() ? fields[6].GetUInt16() : -1);

			sLog->outInfo(LOG_GUILD, "[ LOAD ] Added Guild [%u - %s] Member [%u - %u - %s]",
				pGuild->GetID(), pGuild->GetName(), slot, pGuild->GetMember(slot)->GetID(), pGuild->GetMember(slot)->GetName());
		} while (result->NextRow());
	}
}

void CGuildMgr::AddGuild(Guild* guild)
{
    this->m_GuildMap[guild->GetID()] = guild;

	sLog->outInfo(LOG_GUILD, "%s -- [%u] %s -- Count: %u", __FUNCTION__, guild->GetID(), guild->GetName(), this->m_GuildMap.size());
}

void CGuildMgr::RemoveGuild(uint32 guildId)
{
	if (guildId == sCastleSiege->GetCastleOwner())
	{
		sCastleSiege->SetCastleOwner(0);
		sCastleSiege->SetOccupied(false);
	}

	GuildMap::const_iterator itr = m_GuildMap.find(guildId);

	if (itr != m_GuildMap.end())
	{
		itr->second->DisbandNotify();

		delete itr->second;
		m_GuildMap.erase(itr);
	}
}

Guild* CGuildMgr::GetGuild(uint32 guildId)
{
    GuildMap::const_iterator itr = m_GuildMap.find(guildId);

    if (itr != m_GuildMap.end())
	{
		return itr->second;
	}

    return nullptr;
}

Guild* CGuildMgr::GetGuild(std::string const& name)
{
    for ( GuildMap::const_iterator itr = m_GuildMap.begin(); itr != m_GuildMap.end(); ++itr )
	{
		if ( !strcmp(itr->second->GetName(), name.c_str()) )
		{
			return itr->second;
		}
	}

    return nullptr;
}

void CGuildMgr::UpdateTime()
{
	for ( GuildMap::iterator it = this->m_GuildMap.begin(); it != this->m_GuildMap.end(); ++it )
	{
		if ( !it->second )
			continue;

		it->second->UpdateTime();
	}
}

void CGuildMgr::SendMsgToGuildAlliance(uint32 alliance, const char * name, const char * notice)
{
	for ( GuildMap::const_iterator it = this->m_GuildMap.begin(); it != this->m_GuildMap.end(); ++it )
	{
		if ( !it->second )
			continue;

		if ( it->second->GetAlliance() != alliance )
			continue;

		it->second->SendChatToMembers(name, notice);
	}
}

uint8 CGuildMgr::GetRelationShip(Guild * Guild1, Guild * Guild2, bool alliance)
{
	if ( !Guild1 || !Guild2 )
	{
		return GUILD_RELATIONSHIP_NONE;
	}

	if ( Guild1->GetAlliance() && Guild2->GetAlliance() && Guild1->GetAlliance() == Guild2->GetAlliance() )
	{
		return GUILD_RELATIONSHIP_UNION;
	}

	if ( Guild1->GetHostil() == Guild2->GetID() || Guild2->GetHostil() == Guild1->GetID() )
	{
		return GUILD_RELATIONSHIP_RIVAL;
	}

	if ( !alliance )
		return GUILD_RELATIONSHIP_NONE;

	for ( GuildMap::const_iterator it = this->m_GuildMap.begin(); it != this->m_GuildMap.end(); ++it )
	{
		if ( !it->second )
			continue;

		if ( Guild1->GetAlliance() && Guild1->GetAlliance() == it->second->GetAlliance() && Guild1 != it->second )
		{
			if ( it->second->GetHostil() == Guild2->GetID() || Guild2->GetHostil() == it->second->GetID() )
			{
				return GUILD_RELATIONSHIP_RIVAL;
			}
		}

		if ( Guild2->GetAlliance() && Guild2->GetAlliance() == it->second->GetAlliance() && Guild2 != it->second )
		{
			if ( it->second->GetHostil() == Guild1->GetID() || Guild1->GetHostil() == it->second->GetID() )
			{
				return GUILD_RELATIONSHIP_RIVAL;
			}
		}
	}

	return GUILD_RELATIONSHIP_NONE;
}

uint8 CGuildMgr::GetInvolvedGuilds(uint32 alliance)
{
	if ( !alliance )
		return sGameServer->guild_alliance_max.get();

	uint8 count = 0;

	for ( GuildMap::const_iterator it = this->m_GuildMap.begin(); it != this->m_GuildMap.end(); ++it )
	{
		if ( !it->second )
			continue;

		if ( it->second->GetAlliance() != alliance )
			continue;

		++count;
	}

	return count;
}

void CGuildMgr::SendAllianceList(Player* pPlayer)
{
	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
		return;

	Guild* pAlliance = this->GetGuild(pGuild->GetAlliance());

	if ( !pAlliance )
		return;

	uint8 buffer[10000];
	POINTER_PCT(GUILD_ALLIANCE_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(GUILD_ALLIANCE_LIST_BODY, body, buffer, sizeof(GUILD_ALLIANCE_LIST_HEAD));
	head->count = 0;
	head->master_hostil = 0; // head->master_hostil = pAlliance->GetHostil();
	head->master = 0; // head->master = pAlliance->GetID();
	head->result = 1;

	for ( GuildMap::const_iterator it = this->m_GuildMap.begin(); it != this->m_GuildMap.end(); ++it )
	{
		if ( !it->second )
			continue;

		if ( !it->second->GetAlliance() || it->second->GetAlliance() != pAlliance->GetID() )
			continue;

		body[head->count].members = it->second->GetTotalMembers();
		memcpy(body[head->count].emblem, it->second->emblem, MAX_GUILD_EMBLEM_LENGTH);
		memcpy(body[head->count].name, it->second->GetName(), MAX_GUILD_NAME_LENGTH);

		++head->count;
	}

	if ( !head->count )
		return;
	
	head->h.set(HEADCODE_GUILD_RELATIONSHIP_LIST, sizeof(GUILD_ALLIANCE_LIST_HEAD) + sizeof(GUILD_ALLIANCE_LIST_BODY) * head->count);
	
	pPlayer->sendPacket(buffer, head->h.get_size());
}

void CGuildMgr::AddCastleSiegeGuildList(uint32 guild, int32 score, uint8 side)
{
	for ( GuildMap::const_iterator it = this->m_GuildMap.begin(); it != this->m_GuildMap.end(); ++it )
	{
		if ( !it->second )
			continue;

		if ( !it->second->GetAlliance() && it->first == guild && guild == sCastleSiege->GetCastleOwner() )
		{
			sCastleSiege->AddGuildToFinalList(it->second->GetID(), it->second->GetID() == guild ? score: 0, side);
		}

		if ( it->second->GetAlliance() == guild )
		{
			sCastleSiege->AddGuildToFinalList(it->second->GetID(), it->second->GetID() == guild ? score: 0, side);
		}
	}
}

void CGuildMgr::SendGuildNotice(Player* pPlayer)
{
	Guild* pGuild = pPlayer->GuildGet();

	if ( pGuild )
	{
		pGuild->SendNotice(pPlayer);
	}
}

void CGuildMgr::AddScore(Guild* pGuild, int32 score)
{
	if ( !pGuild || score == 0 )
	{
		return;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();
	
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_SCORE_UPDATE);
	stmt->setUInt32(pGuild->GetID());
	stmt->setInt64(score);
	stmt->setInt64(score);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void CGuildMgr::AddAllianceScore(uint32 alliance, int32 score)
{
	if (!alliance || score == 0)
	{
		return;
	}

	for (GuildMap::const_iterator it = this->m_GuildMap.begin(); it != this->m_GuildMap.end(); ++it)
	{
		Guild * pGuild = it->second;

		if (!pGuild)
		{
			continue;
		}

		if (pGuild->GetAlliance() == alliance && pGuild->GetID() != alliance)
		{
			this->AddScore(pGuild, score);
		}
	}
}