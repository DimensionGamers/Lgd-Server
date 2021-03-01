/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ServerLink.cpp"
*
*/

std::unordered_map<uint8, LoginHandler<ServerLink>> ServerLink::InitHandlers()
{
    std::unordered_map<uint8, LoginHandler<ServerLink>> handlers;

	handlers[HEADCODE_SERVER_LINK_ON_CONNECT]							= { "ON_CONNECT", sizeof(SL_ON_CONNECT), &ServerLink::HandleHeadcodeOnConnect };

    handlers[HEADCODE_SERVER_LINK_GUILD_CHAT]							= { "GUILD_CHAT", sizeof(SL_CHAT_PACKET), &ServerLink::HandleHeadcodeGuildChat };
	handlers[HEADCODE_SERVER_LINK_ALLIANCE_CHAT]						= { "ALLIANCE_CHAT", sizeof(SL_CHAT_PACKET), &ServerLink::HandleHeadcodeAllianceChat };
	handlers[HEADCODE_SERVER_LINK_GUILD_NOTICE]							= { "GUILD_NOTICE", sizeof(SL_CHAT_PACKET), &ServerLink::HandleHeadcodeGuildNotice };
	handlers[HEADCODE_SERVER_LINK_POST]									= { "POST", sizeof(SL_CHAT_PACKET), &ServerLink::HandleHeadcodePost };
	handlers[HEADCODE_SERVER_LINK_SIMPLE_MESSAGE]						= { "SIMPLE_MESSAGE", sizeof(SL_CHAT_PACKET), &ServerLink::SimpleMessageResult };
	handlers[HEADCODE_SERVER_LINK_WHISPER]								= { "WHISPER", sizeof(SL_WHISPER), &ServerLink::WhisperResult };

	handlers[HEADCODE_SERVER_LINK_GUILD_ADD]							= { "GUILD_ADD", sizeof(SL_GUILD_ADD), &ServerLink::GuildCreateResult };
	handlers[HEADCODE_SERVER_LINK_GUILD_REMOVE]							= { "GUILD_REMOVE", sizeof(SL_GUILD_DEL), &ServerLink::GuildDeleteResult };
	handlers[HEADCODE_SERVER_LINK_GUILD_MEMBER_ADD]						= { "GUILD_MEMBER_ADD", sizeof(SL_GUILD_MEMBER_ADD), &ServerLink::GuildMemberAddResult };
	handlers[HEADCODE_SERVER_LINK_GUILD_MEMBER_DEL]						= { "GUILD_MEMBER_DEL", sizeof(SL_GUILD_MEMBER_DEL), &ServerLink::GuildMemberDelResult };
	handlers[HEADCODE_SERVER_LINK_GUILD_MEMBER_STATUS]					= { "GUILD_MEMBER_STATUS", sizeof(SL_GUILD_MEMBER_STATUS), &ServerLink::GuildMemberStatusResult };
	handlers[HEADCODE_SERVER_LINK_GUILD_RELATIONSHIP]					= { "GUILD_RELATIONSHIP", sizeof(SL_GUILD_RELATIONSHIP), &ServerLink::GuildRelationshipResult };
	handlers[HEADCODE_SERVER_LINK_GUILD_SCORE]							= { "GUILD_SCORE", sizeof(SL_GUILD_SCORE), &ServerLink::GuildScoreResult };

	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_DATA]				= { "CASTLE_SIEGE_LOAD_DATA", sizeof(SL_CASTLE_SIEGE_LOAD_DATA_RESULT), &ServerLink::CastleSiegeLoadDataResult };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_NPC]				= { "CASTLE_SIEGE_LOAD_NPC", sizeof(SL_CASTLE_SIEGE_LOAD_NPC_HEAD), &ServerLink::CastleSiegeLoadNpcResult };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_REGISTERED_GUILD]	= { "CASTLE_SIEGE_LOAD_REGISTERED_GUILD", sizeof(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD), &ServerLink::CastleSiegeLoadRegisteredGuildResult };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD]		= { "CASTLE_SIEGE_REGISTERED_GUILD", sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD), &ServerLink::CastleSiegeRegisteredGuildResult };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD_ALL]	= { "CASTLE_SIEGE_REGISTERED_GUILD_ALL", sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD), &ServerLink::CastleSiegeRegisteredGuildAllResult };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTER_GUILD]			= { "CASTLE_SIEGE_REGISTER_GUILD", sizeof(SL_CASTLE_SIEGE_REGISTER_GUILD), &ServerLink::HandleHeadcodeCastleSiegeRegisterGuild };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_TAX_RATE]			= { "CASTLE_SIEGE_SAVE_TAX_RATE", sizeof(SL_CASTLE_SIEGE_SAVE_TAX_RATE), &ServerLink::HandleHeadcodeCastleSiegeTaxRate };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_MONEY]				= { "CASTLE_SIEGE_SAVE_MONEY", sizeof(SL_CASTLE_SIEGE_SAVE_MONEY), &ServerLink::HandleHeadcodeCastleSiegeMoney };
	handlers[HEADCODE_SERVER_LINK_CASTLE_SIEGE_SAVE_OWNER_STATUS]		= { "CASTLE_SIEGE_SAVE_OWNER_STATUS", sizeof(SL_CASTLE_SIEGE_SAVE_OWNER_STATUS), &ServerLink::HandleHeadcodeCastleSiegeUpdateOwner };

	handlers[HEADCODE_SERVER_LINK_NOTICE]								= { "NOTICE", sizeof(SL_NOTICE), &ServerLink::HandleHeadcodeNotice };
	handlers[HEADCODE_SERVER_LINK_CHARACTER_ON_OFF]						= { "CHARACTER_ON_OFF", sizeof(SL_CHARACTER_ON_OFF), &ServerLink::HandleHeadcodeCharacterOnOff };
	handlers[HEADCODE_SERVER_LINK_CHARACTER_CREATE]						= { "CHARACTER_CREATE", sizeof(SL_CHARACTER_CREATE_RESULT), &ServerLink::CharacterCreateResult };
	handlers[HEADCODE_SERVER_LINK_CHARACTER_DELETE]						= { "CHARACTER_DELETE", sizeof(SL_CHARACTER_DELETE_RESULT), &ServerLink::CharacterDeleteResult };

	handlers[HEADCODE_SERVER_LINK_ADMIN_COMMAND]						= { "ADMIN_COMMAND", sizeof(SL_ADMIN_COMMAND), &ServerLink::AdminCommandResult };

	handlers[HEADCODE_SERVER_LINK_SIGNAL]								= { "SIGNAL", sizeof(SL_SIGNAL), &ServerLink::Signal };

	handlers[HEADCODE_SERVER_LINK_CRYWOLF_REQUEST]						= { "CRYWOLF_REQUEST", sizeof(SL_CRYWOLF_REQUEST), &ServerLink::CrywolfDataResult };

	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_LIST]					= { "GUILD_MATCHING_LIST", sizeof(SL_GUILD_MATCHING_LIST_HEAD), &ServerLink::GuildMatchingList };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_INSERT]				= { "GUILD_MATCHING_INSERT", sizeof(SL_GUILD_MATCHING_INSERT_SEND), &ServerLink::GuildMatchingInsert };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_CANCEL]				= { "GUILD_MATCHING_CANCEL", sizeof(SL_GUILD_MATCHING_CANCEL_SEND), &ServerLink::GuildMatchingCancel };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INSERT]			= { "GUILD_MATCHING_JOIN_INSERT", sizeof(SL_GUILD_MATCHING_JOIN_INSERT_SEND), &ServerLink::GuildMatchingJoinInsert };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_CANCEL]			= { "GUILD_MATCHING_JOIN_CANCEL", sizeof(SL_GUILD_MATCHING_JOIN_CANCEL_SEND), &ServerLink::GuildMatchingJoinCancel };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_ACCEPT]			= { "GUILD_MATCHING_JOIN_ACCEPT", sizeof(SL_GUILD_MATCHING_JOIN_ACCEPT_SEND), &ServerLink::GuildMatchingJoinAccept };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_LIST]				= { "GUILD_MATCHING_JOIN_LIST", sizeof(SL_GUILD_MATCHING_JOIN_LIST_HEAD), &ServerLink::GuildMatchingJoinList };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_JOIN_INFO]				= { "GUILD_MATCHING_JOIN_INFO", sizeof(SL_GUILD_MATCHING_JOIN_INFO_SEND), &ServerLink::GuildMatchingJoinInfo };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_NOTIFY]				= { "GUILD_MATCHING_NOTIFY", sizeof(SL_GUILD_MATCHING_NOTIFY_SEND), &ServerLink::GuildMatchingNotify };
	handlers[HEADCODE_SERVER_LINK_GUILD_MATCHING_MASTER_NOTIFY]			= { "GUILD_MATCHING_MASTER_NOTIFY", sizeof(SL_GUILD_MATCHING_NOTIFY_MASTER_SEND), &ServerLink::GuildMatchingMasterNotify };

	handlers[HEADCODE_SERVER_LINK_EVENT_NOTIFICATION]					= { "EVENT_NOTIFICATION", sizeof(SL_EVENT_NOTIFICATION), &ServerLink::EventNotification };

	handlers[HEADCODE_SERVER_LINK_EVENT_STATE_UPDATE]					= { "EVENT_STATE_UPDATE", sizeof(SL_EVENT_STATE_UPDATE), &ServerLink::EventStateUpdate };

	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_MASTER_REGISTER]				= { "ARKA_WAR_MASTER_REGISTER", sizeof(SL_ARKA_WAR_MASTER_REGISTER), &ServerLink::ArkaWarMasterRegister };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_REGISTER]				= { "ARKA_WAR_MEMBER_REGISTER", sizeof(SL_ARKA_WAR_MEMBER_REGISTER), &ServerLink::ArkaWarMemberRegister };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REGISTER]				= { "ARKA_WAR_SIGN_REGISTER", sizeof(SL_ARKA_WAR_SIGN_REGISTER), &ServerLink::ArkaWarSignRegister };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_ENTER]						= { "ARKA_WAR_ENTER", sizeof(SL_ARKA_WAR_ENTER), &ServerLink::ArkaWarEnter };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_LIST]						= { "ARKA_WAR_LIST", sizeof(SL_ARKA_WAR_LIST_HEAD), &ServerLink::ArkaWarList };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_MEMBER_COUNT]				= { "ARKA_WAR_MEMBER_COUNT", sizeof(SL_ARKA_WAR_MEMBER_COUNT), &ServerLink::ArkaWarMemberCount };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_REQUEST]				= { "ARKA_WAR_GUILD_REQUEST", sizeof(SL_ARKA_WAR_GUILD_HEAD), &ServerLink::ArkaWarGuildRequest };
	handlers[HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REQUEST]				= { "ARKA_WAR_SIGN_REQUEST", sizeof(SL_ARKA_WAR_SIGN_REQUEST_HEAD), &ServerLink::ArkaWarSignOfLordCheckResult };

	handlers[HEADCODE_SERVER_LINK_CHARACTER_SIGNAL]						= { "CHARACTER_SIGNAL", sizeof(SL_CHARACTER_SIGNAL), &ServerLink::CharacterSignal };

    return handlers;
}

std::unordered_map<uint8, LoginHandler<ServerLink>> const Handlers = ServerLink::InitHandlers();

ServerLink::ServerLink(): TCPClientMgr("ServerLink")
{
	
}

void ServerLinkSocket::OnStart()
{
	sLog->outInfo("root", "CONNECTED SERVERLINK");

	sServerLink->IncreaseConnectCount(1);
	sServerLink->SetStopped(false);

	SL_ON_CONNECT pMsg(sGameServer->GetServerCode());
	this->QueuePacket(MAKE_PCT(pMsg));
}

void ServerLinkSocket::OnStop()
{
	sLog->outInfo("root", "DISCONNECTED SERVERLINK");

	sServerLink->GetReconnectTime()->Start();
	sServerLink->SetStopped(true);
}

void ServerLinkSocket::ReadHandler()
{
	if ( this->IsStopped() )
		return;

	MessageBuffer& packet = GetReadBuffer();
    while ( packet.GetActiveSize() > 0 )
    {
		uint8 cmd = packet.GetReadPointer()[0];
        auto itr = Handlers.find(cmd);
        if (itr == Handlers.end())
        {
            packet.Reset();
            break;
        }

        uint16 size = itr->second.packetSize;
        if (packet.GetActiveSize() < size)
            break;

		CUSTOM_PACKET_HEAD * head = (CUSTOM_PACKET_HEAD*)packet.GetReadPointer();

		sServerLink->AddPacket(new WorldPacket(cmd, packet.GetReadPointer(), head->size));	

		packet.ReadCompleted(head->size);
    }
}

void ServerLink::HandleHeadcodeOnConnect(uint8 * Packet)
{
	if ( this->GetConnectCount() == 1 )
	{
		//this->GuildListRequest();
		this->CastleSiegeLoadDataRequest();
		this->CastleSiegeLoadNpcRequest();
		this->CrywolfDataRequest();
		this->ArkaWarGuildRequest();
	}
	else
	{
		PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			this->CharacterOnOff(pPlayer, 1);
		}
	}
}

void ServerLink::HandleHeadcodeGuildChat(uint8 * packet)
{
	POINTER_PCT(SL_CHAT_PACKET, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->SendChatToMembers(lpMsg->name,lpMsg->msg);

	sObjectMgr->SendGuildMessageToAdmin(lpMsg->guild,lpMsg->name,lpMsg->msg);
}

void ServerLink::HandleHeadcodeAllianceChat(uint8 * packet)
{
	POINTER_PCT(SL_CHAT_PACKET, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	if ( !pGuild->GetAlliance() )
	{
		pGuild->SendChatToMembers(lpMsg->name,lpMsg->msg);
		sObjectMgr->SendAllianceMessageToAdmin(lpMsg->guild,lpMsg->name,lpMsg->msg);
	}
	else
	{
		sGuildMgr->SendMsgToGuildAlliance(pGuild->GetAlliance(),lpMsg->name,lpMsg->msg);
		sObjectMgr->SendAllianceMessageToAdmin(pGuild->GetAlliance(),lpMsg->name,lpMsg->msg);
	}
}

void ServerLink::HandleHeadcodeGuildNotice(uint8 * packet)
{
	POINTER_PCT(SL_CHAT_PACKET, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->ResetNotice();
	pGuild->SetNotice(lpMsg->msg);
	pGuild->SendNoticeToMembers(NOTICE_GUILD, lpMsg->msg);

	sObjectMgr->SendGuildNoticeToAdmin(lpMsg->guild,lpMsg->name,lpMsg->msg);
}

void ServerLink::HandleHeadcodePost(uint8 * packet)
{
	if ( sGameServer->IsGlobalMute() )
		return;

	POINTER_PCT(SL_CHAT_PACKET, lpMsg, packet, 0);

	sObjectMgr->SendPostToAll(lpMsg->h.server, lpMsg->name, lpMsg->msg);
}

void ServerLink::HandleHeadcodeCharacterOnOff(uint8 * packet)
{
	POINTER_PCT(SL_CHARACTER_ON_OFF, lpMsg, packet, 0);

	if ( lpMsg->status == 2 )
		return;

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( pGuild )
	{
		pGuild->UpdateMemberStatus(lpMsg->guid, lpMsg->server, lpMsg->name);
	}

	sObjectMgr->PlayerOnlineUpdate(lpMsg->guid, lpMsg->name, lpMsg->server, lpMsg->status);

	if (lpMsg->status == 1)
	{
		sChaosCastleSurvivalMgr->OnPlayerConnect(lpMsg->guid, lpMsg->name);
		sTormentedSquareSurvival->OnPlayerConnect(lpMsg->guid, lpMsg->name);
	}

	if (sGameServer->IsDuplicatedCharacterCheck())
	{
		if (lpMsg->h.server != sGameServer->GetServerCode() && lpMsg->status == 1 && lpMsg->guid > 0)
		{
			HASH_PLAYER(i)
			{
				Player* pPlayer = ObjectContainer[i]->ToPlayer();

				if (!pPlayer || pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING)
				{
					continue;
				}

				if (pPlayer->GetGUID() == lpMsg->guid || pPlayer->GetAccountData()->GetGUID() == lpMsg->account_id)
				{
					KICK_PLAYER(pPlayer, "Duplicated Character");

					sGameServer->LogAntiHack(pPlayer, ANTIHACK_ID_KICK, "Duplicated Character");
				}
			}
		}
	}
}

void ServerLink::HandleHeadcodeNotice(uint8 * packet)
{
	POINTER_PCT(SL_NOTICE, lpMsg, packet, 0);

	sObjectMgr->SendNoticeToAllNormal(lpMsg->type, lpMsg->notice);
}

void ServerLink::CastleSiegeLoadDataResult(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_LOAD_DATA_RESULT, lpMsg, packet, 0);

	sCastleSiege->SetCastleOwner(lpMsg->owner);
	sCastleSiege->SetOccupied(lpMsg->status);
	sCastleSiege->SetTaxRateHunt(lpMsg->tax_hunt);
	sCastleSiege->SetTaxRateChaos(lpMsg->tax_chaos);
	sCastleSiege->SetTaxRateStore(lpMsg->tax_store);
	sCastleSiege->SetHuntEnabled(lpMsg->hunt_allowed);
	sCastleSiege->IncreaseTributeMoney(lpMsg->money);
}

void ServerLink::CastleSiegeLoadNpcResult(uint8 * packet)
{
	sCastleSiege->LoadNPC(packet);
}

void ServerLink::CastleSiegeLoadRegisteredGuildResult(uint8 * packet)
{
	sCastleSiege->LoadGuildRegisterResult(packet);
}

void ServerLink::CastleSiegeRegisteredGuildResult(uint8 * packet)
{
	sCastleSiege->RegisterGuildInfoCallBack(packet);
}

void ServerLink::CastleSiegeRegisteredGuildAllResult(uint8 * packet)
{
	sCastleSiege->GuildRegisterListCallBack(packet);
}

void ServerLink::HandleHeadcodeCastleSiegeRegisterGuild(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTER_GUILD, lpMsg, packet, 0);

	if ( Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild) )
	{
		pGuild->SetRegisteredInCaslteSiege(lpMsg->status);
		pGuild->SetCastleSiegeMarks(lpMsg->mark);
	}
}

void ServerLink::HandleHeadcodeCastleSiegeTaxRate(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_TAX_RATE, lpMsg, packet, 0);

	sCastleSiege->SetTaxRate(lpMsg->tax_hunt, lpMsg->tax_chaos, lpMsg->tax_store, lpMsg->hunt_allowed);
}

void ServerLink::HandleHeadcodeCastleSiegeMoney(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_MONEY, lpMsg, packet, 0);

	if ( !lpMsg->type )
	{
		sCastleSiege->IncreaseTributeMoney(lpMsg->money);
	}
	else if ( lpMsg->type == 1 )
	{
		sCastleSiege->SetTributeMoney(0);
	}
	else if ( lpMsg->type == 2 )
	{
		sCastleSiege->ReduceTributeMoney(lpMsg->money);
	}

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
		return;

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry, lpMsg->ms_time, lpMsg->account_id, lpMsg->character_id);

	if ( !pPlayer )
		return;

	if ( pPlayer->IsCastleSiegeMoneyRemove() )
	{
		pPlayer->MoneyAdd(lpMsg->money);
		sCastleSiege->MoneyOutResult(pPlayer, 1, sCastleSiege->GetTributeMoney());
		pPlayer->SetCastleSiegeMoneyRemove(false);
	}
}

void ServerLink::HandleHeadcodeCastleSiegeUpdateOwner(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_OWNER_STATUS, lpMsg, packet, 0);

	sCastleSiege->SetCastleOwner(lpMsg->owner);
	sCastleSiege->SetOccupied(lpMsg->status);
}

void ServerLink::ChatSend(uint8 headcode, Player * pPlayer, const char * msg)
{
	SL_CHAT_PACKET pMsg(headcode, 0, pPlayer->GetName(), msg);
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.guild = pPlayer->GetGuildID();

	if ( pPlayer->IsAdministrator() && pPlayer->IsAdministratorFlag(ADMIN_FLAG_GUILD_TALK) && pPlayer->TalkingGuild != 0 && headcode == HEADCODE_SERVER_LINK_GUILD_CHAT )
	{
		pMsg.guild = pPlayer->TalkingGuild.get();
	}
	else if ( pPlayer->IsAdministrator() && pPlayer->IsAdministratorFlag(ADMIN_FLAG_GUILD_TALK) && pPlayer->TalkingAlliance != 0 && headcode == HEADCODE_SERVER_LINK_ALLIANCE_CHAT )
	{
		pMsg.guild = pPlayer->TalkingAlliance.get();
	}

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildCreateRequest(Player * pPlayer, const char * name, uint8 * emblem)
{
	SL_GUILD_ADD pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.player.entry = pPlayer->GetEntry();
	pMsg.player.ms_time = pPlayer->GetMSTime();
	pMsg.player.guid = pPlayer->GetGUID();
	pMsg.player.account_id = pPlayer->GetAccountData()->GetGUID();
	memcpy(pMsg.player.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH + 1);
	memcpy(pMsg.name, name, MAX_GUILD_NAME_LENGTH);
	memcpy(pMsg.emblem, emblem, MAX_GUILD_EMBLEM_LENGTH);
	
	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildCreateResult(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_ADD, lpMsg, Packet, 0);

	Player* pPlayer = lpMsg->h.server == sGameServer->GetServerCode() ? sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid) : nullptr;

	if ( lpMsg->result == 1 )
	{
		Guild * pGuild = new Guild;
		pGuild->Create(lpMsg->player.guid, lpMsg->name, lpMsg->emblem);
		pGuild->AddMember(lpMsg->player.guid, lpMsg->player.name, GUILD_RANK_MASTER, 0, pPlayer, lpMsg->h.server);

		sGuildMgr->AddGuild(pGuild);

		if ( pPlayer )
		{
			pPlayer->GuildCreateResult(GUILD_CREATE_SUCCESS);
			pPlayer->GuildUpdateViewport();
			pPlayer->GetInterfaceState()->Reset();

			sLog->outInfo(LOG_GUILD, "%s Created guild [%s]", 
				pPlayer->BuildLog().c_str(), pGuild->GetName());
		}
	}
	else
	{
		if ( pPlayer )
			pPlayer->GuildCreateResult(GUILD_CREATE_ALREADY_EXIST);
	}
}

void ServerLink::GuildDeleteRequest(uint32 guild)
{
	SL_GUILD_DEL pMsg(guild);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildDeleteResult(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_DEL, lpMsg, Packet, 0);

	sGuildMgr->RemoveGuild(lpMsg->guild);
}

void ServerLink::GuildMemberAddRequest(Player* pPlayer, Player* pToAdd, uint32 guild)
{
	SL_GUILD_MEMBER_ADD pMsg(guild);
	pMsg.h.server = sGameServer->GetServerCode();
	if ( pPlayer )
	{
		pMsg.player.entry = pPlayer->GetEntry();
		pMsg.player.ms_time = pPlayer->GetMSTime();
		pMsg.player.account_id = pPlayer->GetAccountData()->GetGUID();
		pMsg.player.guid = pPlayer->GetGUID();
		memcpy(pMsg.player.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH + 1);
	}

	pMsg.to_add.entry = pToAdd->GetEntry();
	pMsg.to_add.ms_time = pToAdd->GetMSTime();
	pMsg.to_add.account_id = pToAdd->GetAccountData()->GetGUID();
	pMsg.to_add.guid = pToAdd->GetGUID();
	memcpy(pMsg.to_add.name, pToAdd->GetName(), MAX_CHARACTER_LENGTH + 1);
	
	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildMemberAddRequest(const char * member_name, uint32 guild)
{
	SL_GUILD_MEMBER_ADD pMsg(guild);
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.result = 1;
	memcpy(pMsg.to_add.name, member_name, MAX_CHARACTER_LENGTH + 1);
	
	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildMemberAddResult(uint8 * packet)
{
	POINTER_PCT(SL_GUILD_MEMBER_ADD, lpMsg, packet, 0);

	Player* pPlayer = lpMsg->h.server == sGameServer->GetServerCode() ? sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid) : nullptr;
	Player* pToAdd = lpMsg->h.server == sGameServer->GetServerCode() ? sObjectMgr->FindPlayer(lpMsg->to_add.entry, lpMsg->to_add.ms_time, lpMsg->to_add.account_id, lpMsg->to_add.guid) : nullptr;

	if ( lpMsg->result == 0 )
	{
		if ( pPlayer )
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild is full.");
		}

		if ( pToAdd )
		{
			pToAdd->GuildJoinResult(0x00);
		}

		return;
	}

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
	{
		return;
	}

	pGuild->AddMember(lpMsg->to_add.guid, lpMsg->to_add.name, GUILD_RANK_NORMAL, lpMsg->slot, pToAdd, lpMsg->h.server);

	if ( pToAdd )
	{
		pToAdd->GuildJoinResult(0x01);
		pToAdd->GuildUpdateViewport();
	}
}

void ServerLink::GuildMemberDelRequest(uint32 guild, uint32 id)
{
	SL_GUILD_MEMBER_DEL pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.guild = guild;
	pMsg.id = id;
	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildMemberDelResult(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MEMBER_DEL, lpMsg, Packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if (!pGuild)
	{
		return;
	}

	pGuild->DelMember(lpMsg->id);
}

void ServerLink::GuildMemberStatusRequest(uint32 guild, const char * name, uint8 status)
{
	SL_GUILD_MEMBER_STATUS pMsg(guild, name, status);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildMemberStatusResult(uint8 * packet)
{
	POINTER_PCT(SL_GUILD_MEMBER_STATUS, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->UpdateMemberRanking(lpMsg->name, lpMsg->status);
	pGuild->UpdateMemberRankingNotify(lpMsg->name, lpMsg->status);
}

void ServerLink::GuildRelationshipRequest(uint32 guild01, uint32 guild02, uint8 type, bool operation)
{
	SL_GUILD_RELATIONSHIP pMsg(guild01, guild02, type, operation);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::GuildRelationshipResult(uint8 * packet)
{
	POINTER_PCT(SL_GUILD_RELATIONSHIP, lpMsg, packet, 0);

	if ( lpMsg->result )
	{
		Guild* pGuild01 = sGuildMgr->GetGuild(lpMsg->guild01);
		Guild* pGuild02 = sGuildMgr->GetGuild(lpMsg->guild02);

		if ( !pGuild01 || !pGuild02 )
		{
			return;
		}

		if ( lpMsg->operation ) // Join
		{
			switch ( lpMsg->type )
			{
			case GUILD_RELATIONSHIP_UNION:
				{
					pGuild01->UpdateAlliance(pGuild01->GetID());
					pGuild02->UpdateAlliance(pGuild01->GetID());
				} break;

			case GUILD_RELATIONSHIP_RIVAL:
				{
					pGuild01->UpdateHostil(pGuild02->GetID());
					pGuild02->UpdateHostil(pGuild01->GetID());
				} break;
			}
		}
		else // Break
		{
			switch ( lpMsg->type )
			{
			case GUILD_RELATIONSHIP_UNION:
				{
					if ( sGuildMgr->GetInvolvedGuilds(pGuild01->GetAlliance()) <= 2 )
					{
						pGuild01->UpdateAlliance(0);
					}

					pGuild02->UpdateAlliance(0);
				} break;

			case GUILD_RELATIONSHIP_RIVAL:
				{
					pGuild01->UpdateHostil(0);
					pGuild02->UpdateHostil(0);
				} break;
			}
		}

		pGuild01->UpdateRelationship();
		pGuild02->UpdateRelationship();
	}
}

void ServerLink::GuildScore(uint32 guild, int32 score)
{
	SL_GUILD_SCORE pMsg(guild, score);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::GuildScoreResult(uint8 * packet)
{
	POINTER_PCT(SL_GUILD_SCORE, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->IncreaseScore(lpMsg->score);
}

void ServerLink::CharacterOnOff(Player* pPlayer, uint8 status)
{
	if ( !pPlayer )
	{
		return;
	}

	SL_CHARACTER_ON_OFF pMsg(pPlayer->GetGuildID(), pPlayer->GetGUID(), status == 0 ? -1 : sGameServer->GetServerCode(), pPlayer->GetName(), status);
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.authority = pPlayer->GetAuthority();
	pMsg.admin_flags = pPlayer->GetAdminPanelFlag();
	pMsg.account_id = pPlayer->GetAccountData()->GetGUID();

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::NoticeSend(uint8 type, const char * notice, ...)
{
	ARG(notice_buffer, notice);

	SL_NOTICE pMsg(notice_buffer, type);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::NoticeSendNormal(uint8 type, const char * notice)
{
	SL_NOTICE pMsg(notice, type);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CastleSiegeLoadDataRequest()
{
	SL_CASTLE_SIEGE_LOAD_DATA_REQUEST pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CastleSiegeLoadNpcRequest()
{
	SL_CASTLE_SIEGE_LOAD_NPC_REQUEST pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CastleSiegeLoadRegisteredGuildRequest()
{
	SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CastleSiegeClearGuild()
{
	SL_CASTLE_SIEGE_CLEAR_GUILD pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CastleSiegeRegisteredGuildRequest(Player* pPlayer, Guild* pGuild)
{
	SL_CASTLE_SIEGE_REGISTERED_GUILD_REQUEST pMsg(pGuild->GetID(), pPlayer->GetEntry());

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CastleSiegeRegisteredGuildAllRequest(Player* pPlayer)
{
	SL_CASTLE_SIEGE_REGISTERED_GUILD_ALL_REQUEST pMsg(pPlayer->GetEntry());

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CastleSiegeRegisterGuild(uint32 guild, bool status, uint32 mark)
{
	SL_CASTLE_SIEGE_REGISTER_GUILD pMsg(guild, status, mark);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::CharacterCreateRequest(Player* pPlayer, uint8 race, const char * name, uint8 slot, CharacterBaseData const* info)
{
	SL_CHARACTER_CREATE_REQUEST pMsg;
	pMsg.entry = pPlayer->GetEntry();
	pMsg.ms_time = pPlayer->GetMSTime();
	pMsg.account_id = pPlayer->GetAccountData()->GetGUID();
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.character_class = race;
	memcpy(pMsg.character_name, name, MAX_CHARACTER_LENGTH);
	pMsg.strength = info->GetStat(STRENGTH);
	pMsg.agility = info->GetStat(AGILITY);
	pMsg.vitality = info->GetStat(VITALITY);
	pMsg.energy = info->GetStat(ENERGY);
	pMsg.leadership = info->GetStat(LEADERSHIP);
	pMsg.life = info->GetLife();
	pMsg.mana = info->GetMana();
	pMsg.points = info->GetPoints();
	pMsg.level = info->GetLevel();
	pMsg.world = info->GetWorld();
	pMsg.x = 130;
	pMsg.y = 130;
	pMsg.slot = slot;

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void ServerLink::CharacterCreateResult(uint8 * Packet)
{
	POINTER_PCT(SL_CHARACTER_CREATE_RESULT, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
		return;

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( !pPlayer )
		return;

	if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_LOGGED )
		return;

	if ( pPlayer->GetMSTime() != lpMsg->ms_time )
		return;

	if ( pPlayer->GetAccountData()->GetCharacterHandleAction() != CHARACTER_HANDLE_ACTION_CREATE )
		return;

	pPlayer->ResetCharacterHandleData();

	if ( lpMsg->result != CHARACTER_CREATE_RESULT_SUCCESS )
	{
		pPlayer->CharacterCreateResult(lpMsg->result);
		return;
	}

	pPlayer->SetCharacterListRequested(false);
	pPlayer->GetCharacterList(lpMsg->slot)->SetName(lpMsg->character_name, MAX_CHARACTER_LENGTH + 1);

	CHARACTER_CREATE_RESULT pMsg(CHARACTER_CREATE_RESULT_SUCCESS);
	pMsg.character_class = lpMsg->character_class;
	memcpy(pMsg.character_name, lpMsg->character_name, MAX_CHARACTER_LENGTH);
	pMsg.level = lpMsg->level;
	pMsg.position = lpMsg->slot;

	pPlayer->sendPacket(MAKE_PCT(pMsg));

	sLog->outInfo(LOG_PLAYER, "[ CHARACTER CREATE ] %s Created character [%s] class %d",
		pPlayer->BuildLog().c_str(), lpMsg->character_name, lpMsg->character_class);
}

void ServerLink::CharacterDeleteRequest(Player* pPlayer, const char * name)
{
	SL_CHARACTER_DELETE_REQUEST pMsg;
	pMsg.account_id = pPlayer->GetAccountData()->GetGUID();
	pMsg.entry = pPlayer->GetEntry();
	pMsg.ms_time = pPlayer->GetMSTime();
	pMsg.h.server = sGameServer->GetServerCode();
	memcpy(pMsg.character_name, name, MAX_CHARACTER_LENGTH);

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}
	
void ServerLink::CharacterDeleteResult(uint8 * Packet)
{
	POINTER_PCT(SL_CHARACTER_DELETE_RESULT, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
		return;

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( !pPlayer )
		return;

	if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_LOGGED )
		return;

	if ( pPlayer->GetMSTime() != lpMsg->ms_time )
		return;

	if ( pPlayer->GetAccountData()->GetCharacterHandleAction() != CHARACTER_HANDLE_ACTION_DELETE )
		return;

	if ( lpMsg->result != CHARACTER_DELETE_RESULT_SUCCESS )
	{
		if ( lpMsg->result != CHARACTER_DELETE_RESULT_WRONG_SECURITY_CODE )
		{
			//pPlayer->CloseSocket();
		}
		else
		{
			//pPlayer->CharacterDeleteResult(lpMsg->result);
		}

		pPlayer->CharacterDeleteResult(lpMsg->result);
	}
	else
	{
		if ( pPlayer->GetAccountData()->GetCurrentCharacterIndex() != uint8(-1) )
		{
			pPlayer->GetCharacterList(pPlayer->GetAccountData()->GetCurrentCharacterIndex())->Reset();
		}

		pPlayer->SetCharacterListRequested(false);
		pPlayer->CharacterDeleteResult(CHARACTER_DELETE_RESULT_SUCCESS);
	}

	pPlayer->ResetCharacterHandleData();
}

void ServerLink::AdminCommandRequest(Player* pPlayer, uint8 type, uint16 sub_type, const char * target_name, const char * target_account, time_t time)
{
	SL_ADMIN_COMMAND pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.type = type;
	pMsg.sub_type = sub_type;
	pMsg.entry = pPlayer->GetEntry();
	pMsg.guid = pPlayer->GetGUID();

	if ( target_name && strlen(target_name) > 0 )
		memcpy(pMsg.target_name, target_name, MAX_CHARACTER_LENGTH + 1);

	if ( target_account && strlen(target_account) > 0 )
		memcpy(pMsg.target_account, target_account, MAX_ACCOUNT_LENGTH + 1);

	pMsg.time = time;

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void ServerLink::AdminCommandResult(uint8 * Packet)
{
	POINTER_PCT(SL_ADMIN_COMMAND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( pPlayer && pPlayer->GetGUID() != lpMsg->guid )
		pPlayer = nullptr;

	if ( pPlayer && !pPlayer->IsAdministrator() )
		pPlayer = nullptr;

	if ( !lpMsg->result )
	{
		switch ( lpMsg->type )
		{
		case 0:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					if ( lpMsg->time > 0 )
					{
						pTarget->SetKickTime(::time(nullptr) + lpMsg->time);
					}

					KICK_PLAYER(pTarget, "Administrator Kick");
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Player %s kicked.", lpMsg->target_name);
			} break;

		case 1:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					pTarget->GetRestriction(lpMsg->sub_type)->SetTime(time(nullptr) + lpMsg->time);
					pTarget->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s restriction has been applied for %s", PlayerAction::Name[lpMsg->sub_type].c_str(), secsToTimeString(lpMsg->time).c_str());
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Applied %s restriction to %s", PlayerAction::Name[lpMsg->sub_type].c_str(), lpMsg->target_name);
			} break;

		case 2:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					pTarget->ChatMute(lpMsg->time);
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s Muted for %s", lpMsg->target_name, secsToTimeString(lpMsg->time).c_str());
			} break;

		case 3:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					pTarget->SetAuthority(1);
					KICK_PLAYER(pTarget, "Administrator Banned");

					sGameServer->LogAntiHack(pPlayer, ANTIHACK_ID_KICK, "Administrator Banned");
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Player %s banned.", lpMsg->target_name);
			} break;
		}
	}
	else
	{
		PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s.", lpMsg->target_name);
	}
}

void ServerLink::WhisperRequest(Player* pPlayer, const char * target_name, const char * message)
{
	SL_WHISPER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.entry = pPlayer->GetEntry();
	pMsg.guid = pPlayer->GetGUID();
	strcpy(pMsg.name, pPlayer->GetName());
	strcpy(pMsg.target_name, target_name);
	strcpy(pMsg.message, message);
	
	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}
	
void ServerLink::WhisperResult(uint8 * Packet)
{
	POINTER_PCT(SL_WHISPER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( pPlayer && pPlayer->GetGUID() != lpMsg->guid )
		pPlayer = nullptr;

	switch ( lpMsg->result )
	{
	case 0:
		{
			PACKET_CHAT_DATA pMsg(lpMsg->name, lpMsg->message);
			pMsg.h.headcode = HEADCODE_WHISPER;

			//PLAYER_POINTER(pPlayer)->sendPacket(MAKE_PCT(pMsg));

			Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name);

			if ( pTarget )
			{
				if ( !pTarget->ChatBlockFind(lpMsg->name) )
				{
					pTarget->sendPacket(MAKE_PCT(pMsg));
				}
			}
		} break;

	case 1:
	case 2:
		{
			PLAYER_POINTER(pPlayer)->WhisperOffline();
		} break;

	case 3:
		{
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not able to whisper yourself.");
		} break;
	}

	//sLog->outInfo(LOG_CHAT, "[ WHISPER ] %s - Say: [%s] To %s",
	//	this->BuildLog().c_str(), lpMsg->message, lpMsg->name);
}

void ServerLink::SimpleMessageRequest(const char * name, CustomMessageID type, const char * message, ...)
{
	ARG(buffer, message);

	SL_CHAT_PACKET pMsg(HEADCODE_SERVER_LINK_SIMPLE_MESSAGE, type, name, buffer);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}
	
void ServerLink::SimpleMessageResult(uint8 * Packet)
{
	POINTER_PCT(SL_CHAT_PACKET, lpMsg, Packet, 0);

	if ( Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->name) )
	{
		pPlayer->SendNotice(CustomMessageID(lpMsg->guild), lpMsg->msg);
	}
}

void ServerLink::Signal(uint8 * Packet)
{
	POINTER_PCT(SL_SIGNAL, lpMsg, Packet, 0);

	if ( lpMsg->h.server == uint16(-1) || lpMsg->h.server == sGameServer->GetServerCode() )
	{
		sGameServer->ProcessSignal(lpMsg->signal, lpMsg->data[0], lpMsg->data[1], lpMsg->data[2]);
	}
}

void ServerLink::CrywolfDataRequest()
{
	SL_CRYWOLF_REQUEST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::CrywolfDataResult(uint8 * Packet)
{
	POINTER_PCT(SL_CRYWOLF_REQUEST, lpMsg, Packet, 0);

	sCrywolf->SetOccupationState(lpMsg->status);

	sWorldMgr->statusChange(WORLD_CRYWOLF_FIRST_ZONE, sCrywolf->GetOccupationState());
}

void ServerLink::CrywolfDataSave()
{
	SL_CRYWOLF_SAVE pMsg(sCrywolf->GetOccupationState());
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::HandlePacketData(uint8 headcode, uint8 * packet)
{
    auto itr = Handlers.find(headcode);
    if (itr == Handlers.end())
        return;

	(*sServerLink.*Handlers.at(headcode).handler)(packet);
}

void ServerLink::GuildMatchingList(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingListRecv(Packet);
}

void ServerLink::GuildMatchingInsert(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingInsertRecv(Packet);
}
	
void ServerLink::GuildMatchingCancel(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingCancelRecv(Packet);
}
	
void ServerLink::GuildMatchingJoinInsert(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinInsertRecv(Packet);
}
	
void ServerLink::GuildMatchingJoinCancel(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinCancelRecv(Packet);
}
	
void ServerLink::GuildMatchingJoinAccept(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinAcceptRecv(Packet);
}
	
void ServerLink::GuildMatchingJoinList(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinListRecv(Packet);
}
	
void ServerLink::GuildMatchingJoinInfo(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinInfoRecv(Packet);
}
	
void ServerLink::GuildMatchingNotify(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingNotifyRecv(Packet);
}
	
void ServerLink::GuildMatchingMasterNotify(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingNotifyMasterRecv(Packet);
}

void ServerLink::EventNotification(uint8 event_id, uint8 open)
{
	SL_EVENT_NOTIFICATION pMsg(event_id, open);
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::EventNotification(uint8 * Packet)
{
	POINTER_PCT(SL_EVENT_NOTIFICATION, lpMsg, Packet, 0);

	PlayerSessionMap const& players = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = players.begin(); it != players.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->IsPlaying() || pPlayer->IsServerQuit() )
		{
			continue;
		}

		if ( pPlayer->GetEventNotification(lpMsg->event_id) != lpMsg->open )
		{
			pPlayer->SendEventNotification(lpMsg->event_id, lpMsg->open);

			pPlayer->SetEventNotification(lpMsg->event_id, lpMsg->open);
		}
	}
}

void ServerLink::EventStateUpdate(uint8 event_id, uint8 state, uint8 occupation_state)
{
	SL_EVENT_STATE_UPDATE pMsg(event_id, state, occupation_state);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::EventStateUpdate(uint8 * Packet)
{
	POINTER_PCT(SL_EVENT_STATE_UPDATE, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
	{
		switch ( lpMsg->event_id )
		{
		case EVENT_CASTLE_SIEGE:
			{
				sCastleSiege->SetState(lpMsg->state);
				sCastleSiege->SetOccupied(lpMsg->occupation_state);
			} break;

		case EVENT_CRYWOLF:
			{
				sCrywolf->SetState(lpMsg->state);
				sCrywolf->SetOccupationState(lpMsg->occupation_state);
			} break;

		case EVENT_ARKA_WAR:
			{
				sArkaWar->ChangeState(lpMsg->state, true);
			} break;

		case EVENT_PROTECTOR_OF_ACHERON:
			{
				sProtectorOfAcheron->SetState(lpMsg->state);
			} break;

		case EVENT_LAST_MAN_STANDING:
			{
				sLastManStanding->SetState(lpMsg->state);
			} break;
		}
	}
}

void ServerLink::ArkaWarMasterRegister(Player* pPlayer, uint32 guild)
{
	SL_ARKA_WAR_MASTER_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.guild = guild;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarMasterRegister(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_MASTER_REGISTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			sArkaWar->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_REGISTER_SUCCESS);
		} break;

	case 1:
		{
			sArkaWar->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_MASTER_REGISTER_ALREADY_REGISTERED);
		} break;
	}
}
	
void ServerLink::ArkaWarMemberRegister(Player* pPlayer)
{
	SL_ARKA_WAR_MEMBER_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.guild = pPlayer->GetGuildID();
	pMsg.max_member = sGameServer->GetArkaWarRegisterMaxMembers();

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarMemberRegister(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_MEMBER_REGISTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_REGISTER_SUCCESS);
		} break;

	case 1:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_ALREADY_REGISTERED);
		} break;

	case 2:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_NOT_IN_GUILD);
		} break;

	case 3:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_MAXIMUM_MEMBERS);
		} break;
	}
}
	
void ServerLink::ArkaWarSignRegister(Player* pPlayer, uint8 result, int32 count)
{
	SL_ARKA_WAR_SIGN_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.guild = pPlayer->GetGuildID();
	pMsg.result = result;
	pMsg.signs = count;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarSignRegister(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_SIGN_REGISTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
			{
				return;
			}

			pPlayer->GetInterfaceState()->Set(InterfaceData::ArkaWarSignRegister, nullptr);
			pPlayer->SetMixCompleted(false);
			pPlayer->GetMixInventory()->Clear();
									
			sArkaWar->SignOfLordRegisterResult(pPlayer, 0, lpMsg->signs);
		} break;

	case 1:
		{

		} break;

	case 2:
		{
			sArkaWar->SignOfLordRegisterResult(pPlayer, ARKA_WAR_RESULT_ENTER_NOT_REGISTERED);
		} break;
	}
}
	
void ServerLink::ArkaWarEnter(Player* pPlayer)
{
	SL_ARKA_WAR_ENTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarEnter(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_ENTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			//sArkaWar->EnterResult(pPlayer, 10);

			pPlayer->MoveToGate(426);
		} break;

	case 1:
		{
			sArkaWar->EnterResult(pPlayer, ARKA_WAR_RESULT_ENTER_NOT_REGISTERED);
		} break;
	}
}

void ServerLink::ArkaWarList()
{
	SL_ARKA_WAR_LIST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.min_member = sGameServer->GetArkaWarRegisterMinMembers();
	pMsg.max_guild = MAX_ARKA_WAR_GUILD;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarList(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_LIST_HEAD, head, Packet, 0);

	if ( head->type == 0 )
	{
		sArkaWar->GuildList(Packet);
	}
	else if ( head->type == 1 )
	{
		sArkaWar->MemberList(Packet);
	}
}
	
void ServerLink::ArkaWarClear()
{
	SL_ARKA_WAR_CLEAR pMsg;
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void ServerLink::ArkaWarMemberCount(Player* pPlayer)
{
	SL_ARKA_WAR_MEMBER_COUNT pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.guild = pPlayer->GetGuildID();
	pPlayer->BuildCustomPacketData(pMsg.player);

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarMemberCount(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_MEMBER_COUNT, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			sArkaWar->ViewGuildMemberResult(pPlayer, lpMsg->result, lpMsg->count);
		} break;

	case 1:
		{
			sArkaWar->ViewGuildMemberResult(pPlayer, 2, lpMsg->count);
		} break;

	case 2:
		{
			sArkaWar->ViewGuildMemberResult(pPlayer, 2, lpMsg->count);
		} break;
	}
}

void ServerLink::ArkaWarGuildRequest()
{
	SL_ARKA_WAR_GUILD_REQUEST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarGuildRequest(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_GUILD_HEAD, head, Packet, 0);
	POINTER_PCT(SL_ARKA_WAR_GUILD_BODY, body, Packet, sizeof(SL_ARKA_WAR_GUILD_HEAD));

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		sArkaWar->GetOccupyGuild(i)->Clear();
	}

	for ( int32 i = 0; i < head->count; ++i )
	{
		if ( body[i].id < MAX_ARKA_WAR_ZONES )
		{
			sArkaWar->GetOccupyGuild(body[i].id)->SetGuild(body[i].guild);
			sArkaWar->GetOccupyGuild(body[i].id)->SetAttribute(body[i].attribute);
		}
	}
}
	
void ServerLink::ArkaWarGuildSave()
{
	uint8 buffer[8192];
	POINTER_PCT(SL_ARKA_WAR_GUILD_HEAD, head, buffer, 0);
	POINTER_PCT(SL_ARKA_WAR_GUILD_BODY, body, buffer, sizeof(SL_ARKA_WAR_GUILD_HEAD));
	head->count = 0;

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( sArkaWar->GetOccupyGuild(i)->GetGuild() == 0 )
		{
			continue;
		}

		body[head->count].id = i;
		body[head->count].guild = sArkaWar->GetOccupyGuild(i)->GetGuild();
		body[head->count].attribute = sArkaWar->GetOccupyGuild(i)->GetAttribute();
		++head->count;
	}

	head->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_SAVE, sizeof(SL_ARKA_WAR_GUILD_HEAD) + (head->count * sizeof(SL_ARKA_WAR_GUILD_BODY)));
	this->SendPacket(buffer, head->h.get_size());
}

void ServerLink::ArkaWarSignOfLordCheckRequest(Player* pPlayer)
{
	SL_ARKA_WAR_SIGN_REQUEST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::ArkaWarSignOfLordCheckResult(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_SIGN_REQUEST_HEAD, lpHead, Packet, 0);
	POINTER_PCT(SL_ARKA_WAR_SIGN_REQUEST_BODY, lpBody, Packet, sizeof(SL_ARKA_WAR_SIGN_REQUEST_HEAD));

	Player* pPlayer = sObjectMgr->FindPlayer(lpHead->player.entry, lpHead->player.ms_time, lpHead->player.account_id, lpHead->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD, head, buffer, 0);
	POINTER_PCT(ARKA_WAR_SIGN_OF_LORD_CHECK_BODY, body, buffer, sizeof(ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD));
	head->count = 0;
	head->rank = 0;
	memset(head->signs, 0, 4);

	for ( int32 i = 0; i < lpHead->count; ++i )
	{
		if ( Guild * pGuild = sGuildMgr->GetGuild(lpBody[i].guild) )
		{
			if ( lpBody[i].guild == pPlayer->GetGuildID() )
			{
				head->rank = i + 1;
				head->signs[0] = HIBYTE(HIWORD(lpBody[i].signs));
				head->signs[1] = LOBYTE(HIWORD(lpBody[i].signs));
				head->signs[2] = HIBYTE(LOWORD(lpBody[i].signs));
				head->signs[3] = LOBYTE(LOWORD(lpBody[i].signs));
			}
			
			if (head->count < MAX_ARKA_WAR_GUILD)
			{
				body[head->count].rank = i + 1;
				body[head->count].signs[0] = HIBYTE(HIWORD(lpBody[i].signs));
				body[head->count].signs[1] = LOBYTE(HIWORD(lpBody[i].signs));
				body[head->count].signs[2] = HIBYTE(LOWORD(lpBody[i].signs));
				body[head->count].signs[3] = LOBYTE(LOWORD(lpBody[i].signs));
				memset(body[head->count].name, 0, MAX_GUILD_NAME_LENGTH + 1);
				memcpy(body[head->count].name, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);
				++head->count;
			}
		}
	}

	head->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_SIGN_OF_LORD_CHECK_RESULT, sizeof(ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD) + (sizeof(ARKA_WAR_SIGN_OF_LORD_CHECK_BODY) * head->count));
	pPlayer->sendPacket(buffer, head->h.get_size());	
}

void ServerLink::CharacterSignal(uint32 id, uint8 type)
{
	SL_CHARACTER_SIGNAL pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.id = id;
	pMsg.type = type;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void ServerLink::CharacterSignal(uint8 * Packet)
{
	POINTER_PCT(SL_CHARACTER_SIGNAL, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayerByGUID(lpMsg->id);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->type )
	{
	case 0:
		{
			pPlayer->GetTimer(PLAYER_TIMER_MAIL)->SetTimer(0);
		} break;

	case 1:
		{
			pPlayer->GetTimer(PLAYER_TIMER_CASH_SHOP_GIFT)->SetTimer(0);
		} break;
	}
}