GuildMatching::GuildMatching()
{

}

GuildMatching::~GuildMatching()
{
	MAP_CLEAR(GuildMatchingJoinInfoMap::iterator, this->m_guild_matching_join_info_map);
}

void GuildMatching::LoadGuildMatching()
{
	uint32 oldMSTime = getMSTime();

	sLog->outInfo(LOG_DEFAULT, "Loading Guild Matching...");

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_GUILD_MATCHING_SELECT);

	uint32 count = 0;

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			Guild* pGuild = sGuildMgr->GetGuild(fields[0].GetUInt32());

			if ( pGuild )
			{
				pGuild->SetMatchingEnabled(true);
				pGuild->SetMatchingText(fields[1].GetBase64String().c_str());
				pGuild->SetMatchingInterestType(fields[2].GetUInt8());
				pGuild->SetMatchingLevelRange(fields[3].GetUInt8());
				pGuild->SetMatchingClassType(fields[4].GetUInt16());
				pGuild->SetMatchingBoardNumber(fields[5].GetUInt32());
				pGuild->SetMatchingMasterLevel(fields[6].GetUInt16() + fields[7].GetUInt16());
				pGuild->SetMatchingGensType(fields[9].GetUInt8());

				uint8 db_class = fields[8].GetUInt8();
				uint8 matching_class = db_class >> 4;
				uint8 change_up_1 = db_class & 0x07 & 0x01;
				uint8 change_up_2 = (db_class & 0x07 & 0x02) >> 1;
				uint8 change_up_3 = (db_class & 0x07 & 0x04) >> 2;
				
				if ( change_up_1 )
				{
					matching_class |= 0x10;
				}
				if ( change_up_2 )
				{
					matching_class |= 0x20;
				}
				if (change_up_3)
				{
					matching_class |= 0x40;
				}

				pGuild->SetMatchingMasterClass(matching_class);

				++count;
			}
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u guild matching in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
	sLog->outInfo(LOG_DEFAULT, " ");
}

GuildMatchingJoinInfo* GuildMatching::GetGuildMatchingJoinInfo(uint32 id)
{
	GuildMatchingJoinInfoMap::const_iterator it = this->m_guild_matching_join_info_map.find(id);

	if ( it != this->m_guild_matching_join_info_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

GuildMatchingJoinInfo* GuildMatching::GetGuildMatchingJoinInfo(const char* name)
{
	for ( GuildMatchingJoinInfoMap::const_iterator it = this->m_guild_matching_join_info_map.begin(); it != this->m_guild_matching_join_info_map.end(); ++it )
	{
		if ( !memcmp(name, it->second->GetName(), MAX_CHARACTER_LENGTH) )
		{
			return it->second;
		}
	}

	return nullptr;
}

void GuildMatching::RemoveGuildMatchingJoinInfo(uint32 id)
{
	GuildMatchingJoinInfoMap::const_iterator it = this->m_guild_matching_join_info_map.find(id);

	if ( it != this->m_guild_matching_join_info_map.end() )
	{
		delete it->second;
		this->m_guild_matching_join_info_map.erase(it);
	}
}

void GuildMatching::RemoveGuildMatchingJoinInfo(const char* name)
{
	for ( GuildMatchingJoinInfoMap::const_iterator it = this->m_guild_matching_join_info_map.begin(); it != this->m_guild_matching_join_info_map.end(); )
	{
		if ( !memcmp(name, it->second->GetName(), MAX_CHARACTER_LENGTH) )
		{
			delete it->second;
			this->m_guild_matching_join_info_map.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void GuildMatching::RemoveGuildMatchingJoinInfoNotifyAll(uint32 id)
{
	for ( GuildMatchingJoinInfoMap::const_iterator it = this->m_guild_matching_join_info_map.begin(); it != this->m_guild_matching_join_info_map.end(); ++it )
	{
		if ( it->second->GetGuildID() == id )
		{
			this->DGGuildMatchingNotifySend(it->second->GetName(), it->second->GetGuildID(), 2);
		}
	}
}

void GuildMatching::GuildMatchingListRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_LIST_RECV, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(SL_GUILD_MATCHING_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SL_GUILD_MATCHING_LIST_BODY, body, buffer, sizeof(SL_GUILD_MATCHING_LIST_HEAD));
	head->player = lpMsg->player;
	head->result = 0;
	head->CurPage = lpMsg->page;
	head->MaxPage = 1;
	head->count = 0;

	uint32 PageCount = 0;

	for ( GuildMap::const_iterator itr = sGuildMgr->guild_map.begin(); itr != sGuildMgr->guild_map.end(); ++itr )
	{
		Guild* pGuild = itr->second;

		if ( !pGuild || !pGuild->IsMatchingEnabled() )
		{
			continue;
		}

		if( PageCount >= (((head->CurPage)-1)*9) && PageCount < ((head->CurPage)*9) )
		{
			memcpy(body[head->count].text, pGuild->GetMatchingText(), GUILD_MATCHING_TEXT_LENGTH + 1);
			memcpy(body[head->count].name, pGuild->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH + 1);
			memcpy(body[head->count].GuildName, pGuild->GetName(), MAX_GUILD_NAME_LENGTH + 1);
			body[head->count].GuildMemberCount = pGuild->GetTotalMembers();
			body[head->count].GuildMasterClass = pGuild->GetMatchingMasterClass();
			body[head->count].InterestType = pGuild->GetMatchingInterestType();
			body[head->count].LevelRange = pGuild->GetMatchingLevelRange();
			body[head->count].ClassType = pGuild->GetMatchingClassType();
			body[head->count].GuildMasterLevel = pGuild->GetMatchingMasterLevel();
			body[head->count].BoardNumber = pGuild->GetMatchingBoardNumber();
			body[head->count].GuildNumber = pGuild->GetID();
			body[head->count].GensType = pGuild->GetMatchingGensType();

			++head->count;
		}

		PageCount++;
	}

	head->MaxPage = ((PageCount==0)?1:(((PageCount-1)/9)+1));

	head->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST, sizeof(SL_GUILD_MATCHING_LIST_HEAD) + (head->count * sizeof(SL_GUILD_MATCHING_LIST_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}

void GuildMatching::GuildMatchingListSearchRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_LIST_SEARCH_RECV, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(SL_GUILD_MATCHING_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SL_GUILD_MATCHING_LIST_BODY, body, buffer, sizeof(SL_GUILD_MATCHING_LIST_HEAD));
	head->player = lpMsg->player;
	head->result = 0;
	head->CurPage = lpMsg->page;
	head->MaxPage = 1;
	head->count = 0;

	uint32 PageCount = 0;

	for ( GuildMap::const_iterator itr = sGuildMgr->guild_map.begin(); itr != sGuildMgr->guild_map.end(); ++itr )
	{
		Guild* pGuild = itr->second;

		if ( !pGuild || !pGuild->IsMatchingEnabled() )
		{
			continue;
		}

		//if( strstr(pGuild->GetMatchingText(), lpMsg->SearchWord) == 0 )
		if ( FindString<std::string>(pGuild->GetMatchingText(), lpMsg->SearchWord) != size_t(-1) )
		{
			if( PageCount >= (((head->CurPage)-1)*9) && PageCount < ((head->CurPage)*9) )
			{
				memcpy(body[head->count].text, pGuild->GetMatchingText(), GUILD_MATCHING_TEXT_LENGTH + 1);
				memcpy(body[head->count].name, pGuild->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH + 1);
				memcpy(body[head->count].GuildName, pGuild->GetName(), MAX_GUILD_NAME_LENGTH + 1);
				body[head->count].GuildMemberCount = pGuild->GetTotalMembers();
				body[head->count].GuildMasterClass = pGuild->GetMatchingMasterClass();
				body[head->count].InterestType = pGuild->GetMatchingInterestType();
				body[head->count].LevelRange = pGuild->GetMatchingLevelRange();
				body[head->count].ClassType = pGuild->GetMatchingClassType();
				body[head->count].GuildMasterLevel = pGuild->GetMatchingMasterLevel();
				body[head->count].BoardNumber = pGuild->GetMatchingBoardNumber();
				body[head->count].GuildNumber = pGuild->GetID();
				body[head->count].GensType = pGuild->GetMatchingGensType();

				++head->count;
			}

			PageCount++;
		}
	}

	head->MaxPage = ((PageCount==0)?1:(((PageCount-1)/9)+1));

	head->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST, sizeof(SL_GUILD_MATCHING_LIST_HEAD) + (head->count * sizeof(SL_GUILD_MATCHING_LIST_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}

void GuildMatching::GuildMatchingInsertRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_INSERT_RECV, lpMsg, Packet, 0);

	SL_GUILD_MATCHING_INSERT_SEND pMsg;
	pMsg.player = lpMsg->player;
	pMsg.result = 0;

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->GuildID);

	if ( !pGuild || pGuild->IsMatchingEnabled() )
	{
		pMsg.result = 0xFFFFFFFF;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	pGuild->SetMatchingEnabled(true);
	pGuild->SetMatchingText(lpMsg->text);
	pGuild->SetMatchingMasterClass(lpMsg->GuildMasterClass);
	pGuild->SetMatchingInterestType(lpMsg->InterestType);
	pGuild->SetMatchingLevelRange(lpMsg->LevelRange);
	pGuild->SetMatchingClassType(lpMsg->ClassType);
	pGuild->SetMatchingMasterLevel(lpMsg->GuildMasterLevel);
	pGuild->SetMatchingBoardNumber(0);
	pGuild->SetMatchingGensType(lpMsg->GensType);
	
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_MATCHING_INSERT);
	stmt->setUInt32(lpMsg->GuildID);
	stmt->setString(base64::encode((uint8*)lpMsg->text, GUILD_MATCHING_TEXT_LENGTH + 1));
	stmt->setUInt8(lpMsg->InterestType);
	stmt->setUInt8(lpMsg->LevelRange);
	stmt->setUInt16(lpMsg->ClassType);
	stmt->setUInt32(0);

	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMatching::GuildMatchingCancelRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_CANCEL_RECV, lpMsg, Packet, 0);
	SL_GUILD_MATCHING_CANCEL_SEND pMsg;
	pMsg.player = lpMsg->player;
	pMsg.result = 0;

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->GuildID);

	if ( !pGuild || !pGuild->IsMatchingEnabled() )
	{
		pMsg.result = 0xFFFFFFFF;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	pGuild->ResetMatching();

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_MATCHING_DELETE);
	stmt->setUInt32(pGuild->GetID());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());

	this->RemoveGuildMatchingJoinInfoNotifyAll(lpMsg->GuildID);
}

void GuildMatching::GuildMatchingJoinInsertRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_INSERT_RECV, lpMsg, Packet, 0);

	SL_GUILD_MATCHING_JOIN_INSERT_SEND pMsg;
	pMsg.player = lpMsg->player;
	pMsg.result = 0;

	if ( this->GetGuildMatchingJoinInfo(lpMsg->player.guid) != nullptr )
	{
		pMsg.result = 0xFFFFFFFF;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->GuildID);

	if ( !pGuild || !pGuild->IsMatchingEnabled() )
	{
		pMsg.result = 0xFFFFFFFF;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	GuildMatchingJoinInfo * pGuildMatchingJoinInfo = new GuildMatchingJoinInfo;
	pGuildMatchingJoinInfo->SetID(lpMsg->player.guid);
	pGuildMatchingJoinInfo->SetName(lpMsg->player.name);
	pGuildMatchingJoinInfo->SetGuildID(lpMsg->GuildID);
	pGuildMatchingJoinInfo->SetClass(lpMsg->Class);
	pGuildMatchingJoinInfo->SetLevel(lpMsg->Level);

	this->m_guild_matching_join_info_map[pGuildMatchingJoinInfo->GetID()] = pGuildMatchingJoinInfo;

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());

	this->DGGuildMatchingNotifyMasterSend(pGuild->GetMember(0)->GetName(), 0);
}

void GuildMatching::GuildMatchingJoinCancelRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_CANCEL_RECV, lpMsg, Packet, 0);
	SL_GUILD_MATCHING_JOIN_CANCEL_SEND pMsg;
	pMsg.player = lpMsg->player;
	pMsg.result = 0;
	pMsg.flag = lpMsg->flag;

	if ( this->GetGuildMatchingJoinInfo(lpMsg->player.name) == nullptr )
	{
		pMsg.result = 0xFFFFFFFF;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	this->RemoveGuildMatchingJoinInfo(lpMsg->player.name);

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMatching::GuildMatchingJoinAcceptRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_ACCEPT_RECV, lpMsg, Packet, 0);
	SL_GUILD_MATCHING_JOIN_ACCEPT_SEND pMsg;
	pMsg.player = lpMsg->player;
	pMsg.result = 0;
	pMsg.type = lpMsg->type;
	pMsg.GuildID = lpMsg->GuildID;
	memcpy(pMsg.MemberName, lpMsg->MemberName, MAX_CHARACTER_LENGTH + 1);

	if ( this->GetGuildMatchingJoinInfo(lpMsg->MemberName) == nullptr )
	{
		pMsg.result = 0xFFFFFFFF;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->GuildID);

	if ( !pGuild || !pGuild->IsMatchingEnabled() )
	{
		pMsg.result = 0xFFFFFFFF;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	if ( !sCharacterMgr->GetCharacterData(lpMsg->MemberName) )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		this->RemoveGuildMatchingJoinInfo(lpMsg->MemberName);
	}
	else
	{
		pMsg.result = 0;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		this->DGGuildMatchingNotifySend(lpMsg->MemberName, lpMsg->GuildID, ((lpMsg->type==0)?2:1));
	}
}

void GuildMatching::GuildMatchingJoinListRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_LIST_RECV, lpMsg, Packet, 0);
	
	uint8 buffer[8192];
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_LIST_BODY, body, buffer, sizeof(SL_GUILD_MATCHING_JOIN_LIST_HEAD));
	head->player = lpMsg->player;
	head->count = 0;
	head->result = 0;
	
	for ( GuildMatchingJoinInfoMap::const_iterator it = this->m_guild_matching_join_info_map.begin(); it != this->m_guild_matching_join_info_map.end(); ++it )
	{
		GuildMatchingJoinInfo const* pData = it->second;

		if( pData && (pData->GetGuildID() == lpMsg->GuildID) )
		{
			memcpy(body[head->count].name, pData->GetName(), MAX_CHARACTER_LENGTH + 1);
			body[head->count].Class = pData->GetClass();
			body[head->count].Level = pData->GetLevel();

			++head->count;
		}
	}

	head->h.set(HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_LIST, sizeof(SL_GUILD_MATCHING_JOIN_LIST_HEAD) + (head->count * sizeof(SL_GUILD_MATCHING_JOIN_LIST_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}

void GuildMatching::GuildMatchingJoinInfoRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_INFO_RECV, lpMsg, Packet, 0);

	SL_GUILD_MATCHING_JOIN_INFO_SEND pMsg;
	pMsg.player = lpMsg->player;
	pMsg.result = 0;

	GuildMatchingJoinInfo* pGuildMatchingJoinInfo = this->GetGuildMatchingJoinInfo(lpMsg->player.name); 

	if ( !pGuildMatchingJoinInfo )
	{
		pMsg.result = 0xFFFFFFFE;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	Guild* pGuild = sGuildMgr->GetGuild(pGuildMatchingJoinInfo->GetGuildID());

	if ( !pGuild )
	{
		pMsg.result = 0xFFFFFFFE;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}
	
	memcpy(pMsg.GuildName, pGuild->GetName(), MAX_GUILD_NAME_LENGTH + 1);
	memcpy(pMsg.GuildMasterName, pGuild->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH + 1);

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMatching::GuildMatchingInsertSaveRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MATCHING_INSERT_SAVE_RECV, lpMsg, Packet, 0);

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->GuildID);

	if ( pGuild && pGuild->IsMatchingEnabled() )
	{
		pGuild->SetMatchingMasterClass(lpMsg->GuildMasterClass);
		pGuild->SetMatchingMasterLevel(lpMsg->GuildMasterLevel);
		pGuild->SetMatchingGensType(lpMsg->GensType);
	}
	
	GuildMatchingJoinInfo * pGuildMatchingJoinInfo = this->GetGuildMatchingJoinInfo(lpMsg->player.guid);

	if ( pGuildMatchingJoinInfo )
	{
		pGuildMatchingJoinInfo->SetClass(lpMsg->GuildMasterClass);
		pGuildMatchingJoinInfo->SetLevel(lpMsg->GuildMasterLevel);
		pGuildMatchingJoinInfo->SetName(lpMsg->player.name);
	}
}

void GuildMatching::DGGuildMatchingNotifySend(const char* name, uint32 guild, uint32 result)
{
	CharacterDataPtr pCharacterData = sCharacterMgr->GetCharacterData(name);

	if ( !pCharacterData )
	{
		return;
	}

	SL_GUILD_MATCHING_NOTIFY_SEND pMsg;
	pMsg.id = pCharacterData->GetID();
	pMsg.GuildID = guild;
	pMsg.result = result;

	if ( pCharacterData->socket && pCharacterData->socket->IsOpen() )
	{
		pCharacterData->socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
	}
}

void GuildMatching::DGGuildMatchingNotifyMasterSend(const char* name, uint32 result)
{
	CharacterDataPtr pCharacterData = sCharacterMgr->GetCharacterData(name);

	if ( !pCharacterData )
	{
		return;
	}

	SL_GUILD_MATCHING_NOTIFY_MASTER_SEND pMsg;
	pMsg.id = pCharacterData->GetID();
	pMsg.result = result;

	if ( pCharacterData->socket && pCharacterData->socket->IsOpen() )
	{
		pCharacterData->socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
	}
}
