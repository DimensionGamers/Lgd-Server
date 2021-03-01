Guild::Guild()
{
	this->SetID(0);
	this->SetAlliance(0);
	this->SetHostil(0);
	this->SetScore(0);
	this->SetRegisteredMarks(0);
	this->ResetName();
	memset(this->emblem, 0, MAX_GUILD_EMBLEM_LENGTH);
	this->ResetNotice();

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		this->GetMember(i)->Reset();
	}

	this->SetRemove(false);
	this->SetCastleSiegeRegistered(false);

	this->ResetMatching();
}

Guild::~Guild()
{
}

void Guild::Create(uint32 id, const char * name, uint8 * emblem, bool db)
{
	this->SetID(id);
	this->SetName(name);
	memcpy(this->emblem, emblem, MAX_GUILD_EMBLEM_LENGTH);

	if ( db )
	{
		std::ostringstream ss;
		for ( uint8 i = 0; i < MAX_GUILD_EMBLEM_LENGTH; ++i )
			ss << (uint32)emblem[i] << " ";

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement *stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_INSERT);
		stmt->setUInt32(0, this->GetID());
		stmt->setString(1, base64::encode((uint8*)this->GetName(), MAX_GUILD_NAME_LENGTH));
		stmt->setString(2, ss.str());
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}
}

void Guild::AddMember(uint32 id, const char * name, uint8 ranking, uint8 slot, uint16 server, bool db)
{
	this->GetMember(slot)->SetID(id);
	this->GetMember(slot)->SetName(name);
	this->GetMember(slot)->SetRanking(ranking);
	this->GetMember(slot)->SetServer(server);

	if ( db )
	{
		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_INSERT_MEMBER);
		stmt->setUInt8(0, slot);
		stmt->setUInt32(1, this->GetID());
		stmt->setUInt32(2, id);
		stmt->setUInt8(3, ranking);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}
}

uint8 Guild::GetEmptySlot() const
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !this->GetMember(i)->GetID() )
			return i;
	}

	return 0;
}

uint8 Guild::GetMemberSlot(const char * name) const
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( memcmp(this->GetMember(i)->GetName(), name, MAX_CHARACTER_LENGTH) )
		{
			continue;
		}

		return i;
	}

	return -1;
}

uint8 Guild::GetTotalMembers() const
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

std::string Guild::BuildLog() const
{
	std::ostringstream stream;

	stream << "[" << this->GetID() << " - " << this->GetName() << "]";
	stream << " - ";
	stream << "Master [" << this->GetMember(0)->GetName() << "]";

	return stream.str();
}

void Guild::UpdateRelationshipDB()
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement*stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_ALLIANCE);
	stmt->setUInt32(0, this->GetAlliance());
	stmt->setUInt32(1, this->GetID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_HOSTIL);
	stmt->setUInt32(0, this->GetHostil());
	stmt->setUInt32(1, this->GetID());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

GuildMember* Guild::GetMemberData(uint32 id)
{
	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( this->GetMember(i)->GetID() == id )
			return this->GetMember(i);
	}

	return nullptr;
}

void Guild::ResetMatching()
{
	this->SetMatchingEnabled(false);
	this->ResetMatchingText();
	this->SetMatchingInterestType(0);
	this->SetMatchingLevelRange(0);
	this->SetMatchingClassType(0);
	this->SetMatchingBoardNumber(0);
	this->SetMatchingMasterClass(0);
	this->SetMatchingMasterLevel(0);
	this->SetMatchingGensType(0);
}

GuildMgr::GuildMgr()
{
	
}

GuildMgr::~GuildMgr()
{
	MAP_CLEAR(GuildMap::iterator, guild_map);
}

void GuildMgr::LoadGuilds()
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
			pGuild->SetRegisteredMarks(fields[7].GetUInt32());
			pGuild->SetCastleSiegeRegistered(fields[8].GetUInt32() ? true: false);

			this->AddGuild(pGuild);

			sLog->outInfo("guild", "[ LOAD ] Added Guild [%u - %s]", pGuild->GetID(), pGuild->GetName());

			++count;
		}
		while (result->NextRow());
	}

	this->LoadGuildMembers();

	for (GuildMap::iterator it = this->guild_map.begin(); it != this->guild_map.end(); ++it)
	{
		Guild* pGuild = it->second;

		if (!pGuild)
		{
			continue;
		}

		pGuild->SetRemove(pGuild->GetTotalMembers() <= 0);
	}

	for ( GuildMap::iterator it = this->guild_map.begin(); it != this->guild_map.end(); )
	{
		Guild* pGuild = it->second;

		if (!pGuild)
		{
			this->guild_map.erase(it++);
			continue;
		}


		if (!pGuild->IsRemove())
		{
			++it;
			continue;
		}

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE_MEMBER);
		stmt->setUInt32(0, it->first);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE);
		stmt->setUInt32(0, it->first);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);

		sLog->outInfo("guild", "[ LOAD ] Removed Guild [%u - %s]", pGuild->GetID(), pGuild->GetName());

		delete it->second;
		this->guild_map.erase(it++);
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u guilds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
	sLog->outInfo(LOG_DEFAULT, " ");
}

void GuildMgr::LoadGuildMembers()
{
	/*for ( GuildMap::iterator it = this->guild_map.begin(); it != this->guild_map.end(); ++it )
	{
	Guild * pGuild = it->second;

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_MEMBER_SELECT);
	stmt->setUInt32(0, pGuild->GetID());

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
	{
	do
	{
	Field* fields = result->Fetch();

	uint8 slot = fields[0].GetUInt8();

	if ( slot >= MAX_GUILD_MEMBER )
	continue;

	pGuild->GetMember(slot)->SetID(fields[1].GetUInt32());
	pGuild->GetMember(slot)->SetRanking(fields[2].GetUInt8());
	pGuild->GetMember(slot)->SetName(fields[3].GetCString());
	pGuild->GetMember(slot)->SetServer(fields[4].GetBool() ? fields[5].GetUInt16(): -1);

	sLog->outInfo("guild", "[ LOAD ] Added Guild [%u - %s] Member [%d - %u - %s]",
	pGuild->GetID(), pGuild->GetName(), slot, pGuild->GetMember(slot)->GetID(), pGuild->GetMember(slot)->GetName());
	}
	while (result->NextRow());
	}
	else
	{
	it->second->SetRemove(true);
	}
	}*/

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

			sLog->outInfo("guild", "[ LOAD ] Added Guild [%u - %s] Member [%d - %u - %s]",
				pGuild->GetID(), pGuild->GetName(), slot, pGuild->GetMember(slot)->GetID(), pGuild->GetMember(slot)->GetName());
		} while (result->NextRow());
	}
}

Guild* GuildMgr::GetGuild(uint32 id) const
{
	GuildMap::const_iterator it = guild_map.find(id);

	return it != guild_map.end() ? it->second: nullptr;
}
	
Guild* GuildMgr::GetGuild(std::string name) const
{
	strToLower(name);

	for ( GuildMap::const_iterator it = guild_map.begin(); it != guild_map.end(); ++it )
	{
		std::string lower_name = it->second->GetName();
		strToLower(lower_name);

		if ( lower_name == name )
		{
			return it->second;
		}
	}

	return nullptr;
}

void GuildMgr::AddGuild(Guild* pGuild)
{
	this->guild_map[pGuild->GetID()] = pGuild;
}

void GuildMgr::RemoveGuild(uint32 id)
{
	GuildMap::const_iterator it = guild_map.find(id);

	if ( it != guild_map.end() )
	{
		delete it->second;
		guild_map.erase(it);
	}
}

uint8 GuildMgr::GetInvolvedGuilds(uint32 alliance) const
{
	if ( !alliance )
		return 0;

	uint8 count = 0;

	for ( GuildMap::const_iterator it = this->guild_map.begin(); it != this->guild_map.end(); ++it )
	{
		if ( !it->second )
			continue;

		if ( it->second->GetAlliance() != alliance )
			continue;

		++count;
	}

	return count;
}

void GuildMgr::CreateRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_ADD, lpMsg, Packet, 0);

	GuildCreateDataPtr add_data(new GuildCreateData(lpMsg->name, lpMsg->emblem));
	memcpy(&add_data->player, &lpMsg->player);
	add_data->server = lpMsg->h.server;
	add_data->socket = socket;

	sGuildCreateQueue->Queue(add_data);
}

void GuildMgr::RemoveRequest(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_DEL, lpMsg, Packet, 0);
	SL_GUILD_DEL pMsg(lpMsg->guild);
	pMsg.h.server = lpMsg->h.server;

	if ( Guild* pGuild = this->GetGuild(lpMsg->guild) )
	{
		sLog->outInfo("guild", "Server: %d - Removed Guild %s",
			lpMsg->h.server, pGuild->BuildLog().c_str());
	}

	this->RemoveGuild(lpMsg->guild);
	sArkaWar->GuildRemove(lpMsg->guild);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE_MEMBER);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_MATCHING_DELETE);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_SCORE_DELETE);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

    MuDatabase.CommitTransaction(trans);

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::MemberAddRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MEMBER_ADD, lpMsg, Packet, 0);
	SL_GUILD_MEMBER_ADD pMsg(lpMsg->guild);
	pMsg.h.server = lpMsg->h.server;
	memcpy(&pMsg.player, &lpMsg->player);
	memcpy(&pMsg.to_add, &lpMsg->to_add);

	Guild* pGuild = this->GetGuild(lpMsg->guild);

	if ( !pGuild )
	{
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	pMsg.slot = pGuild->GetEmptySlot();

	if ( pMsg.slot == 0 )
	{
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	uint32 add_guid = lpMsg->to_add.guid;
	uint16 server = lpMsg->result == 1 ? uint16(-1): lpMsg->h.server;
	
	if ( lpMsg->result == 1 )
	{
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_ADD_REQUEST);
		stmt->setString(0, lpMsg->to_add.name);

		PreparedQueryResult result = MuDatabase.Query(stmt);

		if ( result )
		{
			Field* fields = result->Fetch();
			add_guid = fields[0].GetUInt32();
		}
	}

	if ( !add_guid )
	{
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	if (pGuild->GetMemberData(add_guid))
	{
		pMsg.to_add.entry = -1;
		pMsg.player.entry = -1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	lpMsg->to_add.guid = add_guid;
	pMsg.to_add.guid = add_guid;

	pGuild->AddMember(lpMsg->to_add.guid, lpMsg->to_add.name, GUILD_RANK_NORMAL, pMsg.slot, server, true);

	pMsg.result = 1;
	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}
	
void GuildMgr::MemberDelRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MEMBER_DEL, lpMsg, Packet, 0);
	SL_GUILD_MEMBER_DEL pMsg;
	pMsg.h.server = lpMsg->h.server;
	pMsg.guild = lpMsg->guild;
	pMsg.id = lpMsg->id;

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( pGuild )
	{
		GuildMember * pGuildMember = pGuild->GetMemberData(lpMsg->id);

		if (pGuildMember)
		{
			SQLTransaction trans = MuDatabase.BeginTransaction();

			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE_MEMBER_ID);
			stmt->setUInt32(0, pGuildMember->GetID());
			trans->Append(stmt);

			MuDatabase.CommitTransaction(trans);
			
			CharacterDataPtr pCharacterData = sCharacterMgr->GetCharacterData(pGuildMember->GetID());

			if ( pCharacterData )
			{
				pCharacterData->SetGuild(0);
			}

			sArkaWar->MemberRemove(pGuildMember->GetID());

			pGuildMember->Reset();
		}
	}

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::MemberStatusRequest(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MEMBER_STATUS, lpMsg, Packet, 0);
	SL_GUILD_MEMBER_STATUS pMsg(lpMsg->guild, lpMsg->name, lpMsg->status);
	pMsg.h.server = lpMsg->h.server;

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( pGuild )
	{
		uint8 member_slot = pGuild->GetMemberSlot(lpMsg->name);

		if ( member_slot != uint8(-1) )
		{
			SQLTransaction trans = MuDatabase.BeginTransaction();

			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_MEMBER_RANKING);
			stmt->setUInt8(0, lpMsg->status);
			stmt->setUInt32(1, pGuild->GetMember(member_slot)->GetID());
			trans->Append(stmt);

			MuDatabase.CommitTransaction(trans);
			
			pGuild->GetMember(member_slot)->SetRanking(lpMsg->status);
		}
	}

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::RelationshipRequest(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_RELATIONSHIP, lpMsg, Packet, 0);

	Guild* pGuild01 = this->GetGuild(lpMsg->guild01);
	Guild* pGuild02 = this->GetGuild(lpMsg->guild02);

	SL_GUILD_RELATIONSHIP pMsg(lpMsg->guild01, lpMsg->guild02, lpMsg->type, lpMsg->operation);
	pMsg.h.server = lpMsg->h.server;

	if ( !pGuild01 || !pGuild02 )
	{
		sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	if ( lpMsg->operation ) // join
	{
		switch( lpMsg->type )
		{
		case GUILD_RELATIONSHIP_UNION:
			{
				pGuild01->SetAlliance(pGuild01->GetID());
				pGuild02->SetAlliance(pGuild01->GetID());
			} break;

		case GUILD_RELATIONSHIP_RIVAL:
			{
				pGuild01->SetHostil(pGuild02->GetID());
				pGuild02->SetHostil(pGuild01->GetID());
			} break;
		}
	}
	else // break
	{
		switch( lpMsg->type )
		{
		case GUILD_RELATIONSHIP_UNION:
			{
				if ( this->GetInvolvedGuilds(pGuild01->GetAlliance()) <= 2 )
				{
					pGuild01->SetAlliance(0);
				}

				pGuild02->SetAlliance(0);
			} break;

		case GUILD_RELATIONSHIP_RIVAL:
			{
				pGuild01->SetHostil(0);
				pGuild02->SetHostil(0);
			} break;
		}
	}

	pGuild01->UpdateRelationshipDB();
	pGuild02->UpdateRelationshipDB();

	pMsg.result = 1;
	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::Score(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_SCORE, lpMsg, Packet, 0);

	Guild * pGuild = this->GetGuild(lpMsg->guild);

	if ( pGuild )
	{
		pGuild->SetScore(pGuild->GetScore() + lpMsg->score);

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_SCORE);
		stmt->setInt32(0, pGuild->GetScore());
		stmt->setUInt32(1, lpMsg->guild);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}

	sServerSocketMgr.SendPacketAll(Packet, lpMsg->h.get_size());
}

void GuildMgr::Notice(uint8 * Packet)
{
	POINTER_PCT(SL_CHAT_PACKET, lpMsg, Packet, 0);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_NOTICE);
	stmt->setString(0, base64::encode((uint8*)lpMsg->msg, MAX_GUILD_NOTICE_LENGTH));
	stmt->setUInt32(1, lpMsg->guild);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
	
	sServerSocketMgr.SendPacketAll(Packet, lpMsg->h.get_size());
}

void GuildMgr::MemberOnlineOffline(CharacterDataPtr pCharacterData)
{
	if ( !pCharacterData )
		return;

	if ( !pCharacterData->GetID() )
		return;

	Guild* pGuild = this->GetGuild(pCharacterData->GetGuild());

	if ( !pGuild )
		return;

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( pGuild->GetMember(i)->GetID() == pCharacterData->GetID() )
		{
			pGuild->GetMember(i)->SetName(pCharacterData->GetName());
			pGuild->GetMember(i)->SetServer(pCharacterData->GetServer());
			break;
		}
	}
}