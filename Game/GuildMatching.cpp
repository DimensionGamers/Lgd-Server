GuildMatching::GuildMatching()
{

}

GuildMatching::~GuildMatching()
{
	
}

void GuildMatching::CGGuildMatchingListRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_LIST, lpMsg, Packet, 0);

	this->GDGuildMatchingListSend(pPlayer, lpMsg->page);
}

void GuildMatching::CGGuildMatchingListSearchRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_LIST_SEARCH, lpMsg, Packet, 0);

	this->GDGuildMatchingListSearchSend(pPlayer, lpMsg->page, lpMsg->search_word);
}

void GuildMatching::CGGuildMatchingInsertRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, true, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_REGISTER, lpMsg, Packet, 0);

	Guild* pGuild = pPlayer->GuildGet();

	if (!pGuild)
	{
		this->InsertResult(pPlayer, 0xFFFFFFFC);
		return;
	}

	if (pGuild->GetID() != pPlayer->GetGUID())
	{
		this->InsertResult(pPlayer, 0xFFFFFFFC);
		return;
	}

	SL_GUILD_MATCHING_INSERT_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	memcpy(pMsg.text, lpMsg->text, GUILD_MATCHING_TEXT_LENGTH + 1);
	pMsg.GuildID = pPlayer->GetGuildID();
	pMsg.GuildMasterLevel = pPlayer->GetTotalLevel();
	pMsg.GuildMasterClass = pPlayer->GetMatchingClass();
	pMsg.InterestType = lpMsg->interest_type;
	pMsg.LevelRange = lpMsg->level_range;
	pMsg.ClassType = lpMsg->GetClass();
	pMsg.GensType = pPlayer->GetGen()->GetFamily();

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::InsertResult(Player* pPlayer, uint32 result)
{
	if (!pPlayer)
	{
		return;
	}

	GUILD_MATCHING_REGISTER_RESULT pMsg(result);
	pPlayer->SendPacket(&pMsg);
}

void GuildMatching::CGGuildMatchingCancelRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, true, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_CANCEL_REGISTER, lpMsg, Packet, 0);

	Guild* pGuild = pPlayer->GuildGet();

	if (!pGuild)
	{
		this->CancelResult(pPlayer, 0xFFFFFFFE);
		return;
	}

	if (pGuild->GetID() != pPlayer->GetGUID())
	{
		this->CancelResult(pPlayer, 0xFFFFFFFE);
		return;
	}

	SL_GUILD_MATCHING_CANCEL_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.GuildID = pGuild->GetID();

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::CancelResult(Player* pPlayer, uint32 result)
{
	if (!pPlayer)
	{
		return;
	}

	GUILD_MATCHING_CANCEL_REGISTER_RESULT pMsg(result);
	pPlayer->SendPacket(&pMsg);
}

void GuildMatching::CGGuildMatchingJoinInsertRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, true, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_JOIN, lpMsg, Packet, 0);

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild_id);

	if (!pGuild)
	{
		this->JoinInsertResult(pPlayer, 0xFFFFFFFC);
		return;
	}

	if (pPlayer->GuildGet() != nullptr)
	{
		this->JoinInsertResult(pPlayer, 0xFFFFFFFD);
		return;
	}

	SL_GUILD_MATCHING_JOIN_INSERT_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.GuildID = pGuild->GetID();
	pMsg.Class = pPlayer->GetClass();
	pMsg.Level = pPlayer->GetTotalLevel();

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::JoinInsertResult(Player* pPlayer, uint32 result)
{
	if (!pPlayer)
	{
		return;
	}

	GUILD_MATCHING_JOIN_RESULT pMsg(result);
	pPlayer->SendPacket(&pMsg);
}

void GuildMatching::CGGuildMatchingJoinCancelRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, true, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_JOIN_CANCEL, lpMsg, Packet, 0);

	this->GDGuildMatchingJoinCancelSend(pPlayer, 0);
}

void GuildMatching::CGGuildMatchingJoinAcceptRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_JOIN_ACCEPT, lpMsg, Packet, 0);

	if (lpMsg->type != 0)
	{
		if (pPlayer->GetInterfaceState()->GetID() != InterfaceData::None)
		{
			pPlayer->GuildJoinResult(6);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}

		Guild* pGuild = pPlayer->GuildGet();

		if (!pGuild)
		{
			pPlayer->GuildJoinResult(4);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}

		if (pGuild->GetID() != pPlayer->GetGUID())
		{
			pPlayer->GuildJoinResult(4);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}

		if (pGuild->GetWarData()->GetState() != GUILD_WAR_STATE_NONE)
		{
			pPlayer->GuildJoinResult(6);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}

		if (sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START)
		{
			pPlayer->GuildJoinResult(6);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}

		if (sArkaWar->GetState() > ARKA_WAR_STATE_CLOSED && sArkaWar->GetState() < ARKA_WAR_STATE_CLOSE)
		{
			pPlayer->GuildJoinResult(GUILD_ANS_ARKA_WAR_STATE);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}

		if (pPlayer->GetTotalLevel() < 6)
		{
			pPlayer->GuildJoinResult(7);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}

		if (pGuild->GetTotalMembers() >= pPlayer->GuildGetMaxGuildMember())
		{
			pPlayer->GuildJoinResult(2);
			this->JoinAcceptResult(pPlayer, lpMsg->type, 0xFFFFFFF9, lpMsg->name);
			return;
		}
	}

	SL_GUILD_MATCHING_JOIN_ACCEPT_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.type = lpMsg->type;
	pMsg.GuildID = pPlayer->GetGuildID();
	memcpy(pMsg.MemberName, lpMsg->name, MAX_CHARACTER_LENGTH + 1);

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::JoinAcceptResult(Player* pPlayer, uint32 type, uint32 result, const char * name)
{
	if (!pPlayer)
	{
		return;
	}

	GUILD_MATCHING_JOIN_ACCEPT_RESULT pMsg(type, result, name);
	pPlayer->SendPacket(&pMsg);
}

void GuildMatching::CGGuildMatchingJoinListRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_JOIN_LIST, lpMsg, Packet, 0);

	GUILD_MATCHING_JOIN_LIST_HEAD pMsg;
	pMsg.count = 0;
	pMsg.result = 0xFFFFFFFE;

	Guild* pGuild = pPlayer->GuildGet();

	if (!pGuild)
	{
		pPlayer->SendPacket(&pMsg);
		return;
	}

	if (pGuild->GetID() != pPlayer->GetGUID())
	{
		pPlayer->SendPacket(&pMsg);
		return;
	}

	this->GDGuildMatchingJoinListSend(pPlayer);
}

void GuildMatching::CGGuildMatchingJoinInfoRecv(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(GUILD_MATCHING_JOIN_INFO, lpMsg, Packet, 0);

	this->GDGuildMatchingJoinInfoSend(pPlayer);
}

void GuildMatching::DGGuildMatchingListRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_LIST_HEAD, lpMsg, Packet, 0);
	POINTER_PCT(SL_GUILD_MATCHING_LIST_BODY, lpBody, Packet, sizeof(SL_GUILD_MATCHING_LIST_HEAD));

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player);

	if (!pPlayer)
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(GUILD_MATCHING_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(GUILD_MATCHING_LIST_BODY, body, buffer, sizeof(GUILD_MATCHING_LIST_HEAD));
	head->count = 0;
	head->current_page = lpMsg->CurPage;
	head->max_page = lpMsg->MaxPage;
	head->result = lpMsg->result;

	for (uint32 n = 0; n < lpMsg->count; ++n)
	{
		auto & data = body[head->count++];

		memcpy(data.text, lpBody[n].text, GUILD_MATCHING_TEXT_LENGTH + 1);
		memcpy(data.name, lpBody[n].name, MAX_CHARACTER_LENGTH + 1);
		memcpy(data.guild_name, lpBody[n].GuildName, MAX_GUILD_NAME_LENGTH + 1);
		data.members_count = lpBody[n].GuildMemberCount;
		data.master_class = lpBody[n].GuildMasterClass;
		data.interest_type = lpBody[n].InterestType;
		data.level_range = lpBody[n].LevelRange;
		data.class_type = lpBody[n].ClassType;
		data.master_level = lpBody[n].GuildMasterLevel;
		data.board_number = lpBody[n].BoardNumber;
		data.guild_id = lpBody[n].GuildNumber;
		data.gens = (!sGameServer->guild_join_gens ? pPlayer->GetGen()->GetFamily() : lpBody[n].GensType);
	}

	head->Set(0xED, 0x00, sizeof(GUILD_MATCHING_LIST_HEAD)+(head->count * sizeof(GUILD_MATCHING_LIST_BODY)));
	pPlayer->sendPacket(buffer);
}

void GuildMatching::DGGuildMatchingInsertRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_INSERT_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player);

	if (!pPlayer)
	{
		return;
	}

	//lpObj->UseGuildMatching = ((lpMsg->result==0)?1:lpObj->UseGuildMatching);

	this->InsertResult(pPlayer, lpMsg->result);
}

void GuildMatching::DGGuildMatchingCancelRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_CANCEL_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player);

	if (!pPlayer)
	{
		return;
	}

	//lpObj->UseGuildMatching = ((lpMsg->result==0)?0:lpObj->UseGuildMatching);

	this->CancelResult(pPlayer, lpMsg->result);
}

void GuildMatching::DGGuildMatchingJoinInsertRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_INSERT_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player);

	if (!pPlayer)
	{
		return;
	}

	//lpObj->UseGuildMatchingJoin = ((lpMsg->result==0)?1:lpObj->UseGuildMatching);

	this->JoinInsertResult(pPlayer, lpMsg->result);
}

void GuildMatching::DGGuildMatchingJoinCancelRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_CANCEL_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player);

	if (!pPlayer)
	{
		return;
	}

	//lpObj->UseGuildMatchingJoin = ((lpMsg->result==0)?0:lpObj->UseGuildMatching);

	GUILD_MATCHING_JOIN_CANCEL_RESULT pMsg;
	pMsg.result = lpMsg->result;

	if (lpMsg->flag == 0)
	{
		pPlayer->SendPacket(&pMsg);
	}
}

void GuildMatching::DGGuildMatchingJoinAcceptRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_ACCEPT_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if( !pPlayer )
	{
		return;
	}
	
	if( lpMsg->result == 1 && lpMsg->type == 1 )
	{
		sServerLink->GuildMemberAddRequest(lpMsg->MemberName, lpMsg->GuildID);
	}

	this->JoinAcceptResult(pPlayer, lpMsg->type, (lpMsg->result == 1) ? 0 : lpMsg->result, lpMsg->MemberName);
}

void GuildMatching::DGGuildMatchingJoinListRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_LIST_HEAD, lpMsg, Packet, 0);
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_LIST_BODY, lpBody, Packet, sizeof(SL_GUILD_MATCHING_JOIN_LIST_HEAD));

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player);

	if (!pPlayer)
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(GUILD_MATCHING_JOIN_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(GUILD_MATCHING_JOIN_LIST_BODY, body, buffer, sizeof(GUILD_MATCHING_JOIN_LIST_HEAD));
	head->count = 0;
	head->result = lpMsg->result;

	for (uint32 n = 0; n < lpMsg->count; ++n)
	{
		auto & data = body[head->count++];

		memcpy(data.name, lpBody[n].name, MAX_CHARACTER_LENGTH + 1);

		data.race = lpBody[n].Class;
		data.level = lpBody[n].Level;
	}

	head->Set(0xED, 0x07, sizeof(GUILD_MATCHING_JOIN_LIST_HEAD)+(head->count * sizeof(GUILD_MATCHING_JOIN_LIST_BODY)));

	pPlayer->sendPacket(buffer);
}

void GuildMatching::DGGuildMatchingJoinInfoRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_JOIN_INFO_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player);

	if (!pPlayer)
	{
		return;
	}

	GUILD_MATCHING_JOIN_INFO_RESULT pMsg;
	memcpy(pMsg.master_name, lpMsg->GuildMasterName, MAX_CHARACTER_LENGTH + 1);
	memcpy(pMsg.guild_name, lpMsg->GuildName, MAX_GUILD_NAME_LENGTH + 1);
	pMsg.result = lpMsg->result;
	pPlayer->SendPacket(&pMsg);
}

void GuildMatching::DGGuildMatchingNotifyRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_NOTIFY_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayerByGUID(lpMsg->id);

	if (!pPlayer)
	{
		return;
	}

	if (lpMsg->result == 1)
	{
		sServerLink->GuildMemberAddRequest(nullptr, pPlayer, lpMsg->GuildID);
	}

	this->GDGuildMatchingJoinCancelSend(pPlayer, 1);

	GUILD_MATCHING_NOTIFY pMsg;
	pMsg.result = lpMsg->result;
	pPlayer->SendPacket(&pMsg);
}

void GuildMatching::DGGuildMatchingNotifyMasterRecv(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MATCHING_NOTIFY_MASTER_SEND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayerByGUID(lpMsg->id);

	if (!pPlayer)
	{
		return;
	}

	GUILD_MATCHING_NOTIFY_MASTER pMsg;
	pMsg.result = lpMsg->result;

	pPlayer->SendPacket(&pMsg);
}

void GuildMatching::GDGuildMatchingListSend(Player* pPlayer, uint32 page)
{
	SL_GUILD_MATCHING_LIST_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.page = page;
	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::GDGuildMatchingListSearchSend(Player* pPlayer, uint32 page, const char* SearchWord)
{
	SL_GUILD_MATCHING_LIST_SEARCH_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.page = page;
	memcpy(pMsg.SearchWord, SearchWord, 11);

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::GDGuildMatchingJoinCancelSend(Player* pPlayer, uint8 flag)
{
	SL_GUILD_MATCHING_JOIN_CANCEL_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.flag = flag;

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::GDGuildMatchingJoinListSend(Player* pPlayer)
{
	SL_GUILD_MATCHING_JOIN_LIST_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.GuildID = pPlayer->GetGuildID();

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::GDGuildMatchingJoinInfoSend(Player* pPlayer)
{
	SL_GUILD_MATCHING_JOIN_INFO_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}

void GuildMatching::Update(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	pPlayer->SetGuildMatchingUpdateTime(GetTickCount());

	Guild* pGuild = pPlayer->GuildGet();

	SL_GUILD_MATCHING_INSERT_SAVE_RECV pMsg;
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.GuildID = (pGuild && (pGuild->GetID() == pPlayer->GetGUID())) ? pGuild->GetID() : 0;
	pMsg.GensType = pPlayer->GetGen()->GetFamily();
	pMsg.GuildMasterLevel = pPlayer->GetTotalLevel();
	pMsg.GuildMasterClass = pPlayer->GetMatchingClass();

	sServerLink->SendPacket(MAKE_PCT(pMsg));
}