/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerChat.cpp"
*
*/

void Player::ChatRequest(uint8 * Packet)
{
	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_SPEAK) )
		return;

	if ( this->IsMuted() )
	{
		return;
	}

	POINTER_PCT(PACKET_CHAT_DATA, lpMsg, Packet, 0);

	STRING_SAFE_COPY(message, 61, lpMsg->message, 60);

	if ( !strlen(message) )
	{
		return;
	}

	this->Ping();

	ChatType type = this->ChatGetType(message);

	if ( type == CHAT_POST )
	{
		sLog->outInfo(LOG_CHAT, "[ COMMAND ] %s Used Command: %s",
			this->BuildLog().c_str(), message);
	}

	std::string safe_message(message);
	sGameServer->FilterText(safe_message, FILTER_FLAG_TALK);

	this->ChatProcess(type, safe_message.c_str(), this->GetName());
}

void Player::ChatProcess(ChatType type, const char * msg, const char * whisper_name, bool log)
{
	switch(type)
	{
	case CHAT_NORMAL:
		{
			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
			{
				return;
			}

			if ( this->IsAdministrator() && !this->IsAdministratorFlag(ADMIN_FLAG_TALK) )
			{
				return;
			}

			//if ( this->IsAdministrator() )
			//{
				sLog->outInfo(LOG_CHAT, "[ NORMAL ] %s Say: [%s]", this->BuildLog().c_str(), msg);
			//}

			PACKET_CHAT_DATA pMsg(this->GetName(), msg);
			
			this->sendPacket(MAKE_PCT(pMsg));
			this->SendPacketViewportChat(MAKE_PCT(pMsg));
		} break;

	case CHAT_GLOBAL:
		{
			if ( !this->IsAuthorizationEnabled() )
			{
				return;
			}

			if ( !this->IsAdministrator() || !this->IsAdministratorFlag(ADMIN_FLAG_GLOBAL) )
			{
				return;
			}

			sLog->outInfo(LOG_CHAT, "[ GLOBAL ] %s Say: [%s]", this->BuildLog().c_str(), msg);

			if ( this->IsAdminPanelFlag(ADMIN_PANEL_GLOBAL_NAME) )
			{
				static const char global_admin_head[] = {"[GM][%s]: %s"};
				STRING_SAFE(global_admin_message, 100);
				sprintf_s(global_admin_message, global_admin_head, this->GetName(), &msg[1]);

				sServerLink->NoticeSendNormal(NOTICE_GLOBAL, global_admin_message);
			}
			else
			{
				sServerLink->NoticeSendNormal(NOTICE_GLOBAL, &msg[1]);
			}
		} break;

	case CHAT_PARTY:
		{
			Party* pParty = this->GetParty();

			if ( !pParty )
				return;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
			{
				return;
			}

			if ( this->IsAdministrator() )
			{
				sLog->outInfo(LOG_CHAT, "[ PARTY ] %s Say: [%s]", this->BuildLog().c_str(), msg);
			}

			pParty->SendNoticeNormal(this->GetName(), msg);
		} break;

	case CHAT_GUILD:
		{
			if ( !this->GetGuildID() )
				return;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
			{
				return;
			}

			if ( this->IsAdministrator() )
			{
				sLog->outInfo(LOG_CHAT, "[ GUILD ] %s Say: [%s]", this->BuildLog().c_str(), msg);
			}

			sServerLink->ChatSend(HEADCODE_SERVER_LINK_GUILD_CHAT, this, msg);
		} break;

	case CHAT_ALLIANCE:
		{
			if ( !this->GetGuildID() )
				return;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
			{
				return;
			}

			if ( this->IsAdministrator() )
			{
				sLog->outInfo(LOG_CHAT, "[ ALLIANCE ] %s Say: [%s]", this->BuildLog().c_str(), msg);
			}

			sServerLink->ChatSend(HEADCODE_SERVER_LINK_ALLIANCE_CHAT, this, msg);
		} break;

	case CHAT_GUILD_NOTICE:
		{
			if ( !this->GetGuildID() )
				return;

			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return;
			}

			if ( !strlen(&msg[2]) )
				return;

			Guild * pGuild = this->GuildGet();

			if ( !pGuild || pGuild->GetMemberRanking(this) != GUILD_RANK_MASTER )
				return;

			if ( this->IsAdministrator() )
			{
				sLog->outInfo(LOG_CHAT, "[ GUILD NOTICE ] %s Say: [%s]", this->BuildLog().c_str(), msg);
			}

			sServerLink->ChatSend(HEADCODE_SERVER_LINK_GUILD_NOTICE, this, &msg[2]);
		} break;

	case CHAT_GENS:
		{
			if ( !this->GetGen()->IsFamily() )
				return;

			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return;
			}

			if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_GEN_CHAT) )
				return;

			sLog->outInfo(LOG_CHAT, "[ GENS ] %s Say: [%s]", this->BuildLog().c_str(), msg);

			sObjectMgr->SendNoticeToGens(this->GetName(), msg, this->GetGen()->GetFamily());
		} break;

	case CHAT_POST:
		{
			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return;
			}

			if ( this->IsAdministrator() && !this->IsAdministratorFlag(ADMIN_FLAG_POST) )
				return;

			if ( !sGameServer->IsCommandPostEnabled() )
				return;

			if ( sGameServer->IsGlobalMute() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Post is disabled for the moment.");
				return;
			}

			if ( sGameServer->IsMACMuted(this) )
				return;

			if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_POST) )
				return;

			if ( this->GetTotalLevel() < sGameServer->GetCommandPostMinLevel() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "[ POST ]: Required level to use: %d.", sGameServer->GetCommandPostMinLevel());
				return;
			}

			if ( !this->MoneyHave(sGameServer->GetCommandPostCost()) )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "[ POST ]: Required zen to use: %u.", sGameServer->GetCommandPostCost());
				return;
			}

			if ( sScramble->GetState() != SCRAMBLE_STATE_START )
			{
				time_t diff = time(nullptr) - (this->GetPostTime() + sGameServer->GetCommandPostDelay());

				if ( diff < 0 )
				{
					this->SendNotice(CUSTOM_MESSAGE_ID_RED, "[ POST ]: Need to wait %s.", secsToTimeString(-diff).c_str());
					return;
				}
			}

			if ( log )
				sLog->outInfo(LOG_CHAT, "[ POST ] %s Say: [%s]", this->BuildLog().c_str(), msg);

			std::string converted_string = TrimBlankSpaces(&msg[1]);

			bool success = sScramble->CompareWord(this, converted_string.c_str());

			if ( !success )
				success = sGameServer->CanPost(this, converted_string);

			if ( success ) ///- Te permito postear si no superaste la cantidad máxima o si acertaste en scramble
			{
				this->MoneyReduce(sGameServer->GetCommandPostCost());
				this->SetPostTime(time(nullptr));

				sServerLink->ChatSend(HEADCODE_SERVER_LINK_POST, this, msg);
			}
		} break;

	case CHAT_COMMAND:
		{
			ChatHandler(this, whisper_name).ExecuteCommand(msg);
		} break;

	case CHAT_COMMAND_GM:
		{
			if ( !this->IsAdministrator() )
			{
				return;
			}

			if ( !this->IsAuthorizationEnabled() )
			{
				return;
			}

			ChatHandler(this, whisper_name).ExecuteCommandAdmin(msg);
		} break;
	}
}

ChatType Player::ChatGetType(const char * msg) const
{
	switch (msg[0])
	{
	case '!':
		{
			return CHAT_GLOBAL;
		} break;

	case '~':
		{
			return CHAT_PARTY;
		} break;

	case '@':
		{
			switch(msg[1])
			{
			case '@':
				{
					return CHAT_ALLIANCE;
				} break;
			case '>':
				{
					return CHAT_GUILD_NOTICE;
				} break;
			}

			return CHAT_GUILD;
		} break;

	case '$':
		{
			return CHAT_GENS;
		} break;

	case ';':
		{
			return CHAT_POST;
		} break;

	case '/':
		{
			return CHAT_COMMAND;
		} break;

	case '.':
		{
			return CHAT_COMMAND_GM;
		} break;
	}

	return CHAT_NORMAL;
}

void Player::WhisperRequest(uint8 * Packet)
{
	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_WHISPER) )
	{
		return;
	}

	if ( this->IsMuted() )
	{
		return;
	}

	POINTER_PCT(PACKET_CHAT_DATA, lpMsg, Packet, 0);

	STRING_SAFE_COPY(message, 61, lpMsg->message, 60);

	if ( !strlen(message) )
	{
		return;
	}

	this->Ping();

	ChatType type = this->ChatGetType(message);

	std::string safe_message = message;
	sGameServer->FilterText(safe_message, FILTER_FLAG_TALK);

	STRING_SAFE_COPY(name_tmp, (MAX_CHARACTER_LENGTH + 1), lpMsg->name, MAX_CHARACTER_LENGTH);
	STRING_SAFE(name, MAX_CHARACTER_LENGTH + 1);
	strcpy(name, name_tmp);

	if ( type != CHAT_NORMAL )
	{
		if ( type == CHAT_POST )
		{
			sLog->outInfo(LOG_CHAT, "[ COMMAND ] %s Used Command: %s",
				this->BuildLog().c_str(), message);
		}

		this->ChatProcess(type, safe_message.c_str(), name);
		return;
	}

	if ( this->IsAdministrator() )
	{
		if ( !this->IsAdminPanelFlag(ADMIN_PANEL_WHISPER) )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Whisper is disabled.");
			return;
		}
	}

	if (sGameServer->IsWhisperLog())
	{
		sLog->outInfo(LOG_CHAT, "%s -- %s -- %s -> %s", __FUNCTION__, this->BuildLog().c_str(), name, safe_message.c_str());
	}

	sServerLink->WhisperRequest(this, name, message);
}

void Player::WhisperOffline()
{
	SERVER_MESSAGE pMsg(0);
	this->sendPacket(MAKE_PCT(pMsg));
}

bool Player::IsMuted()
{
	if ( this->GetMuteTime() > time(nullptr) )
	{
		std::string timeStr = secsToTimeString(this->GetMuteTime() - time(nullptr));
		this->SendNotice(NOTICE_NORMAL_BLUE, "You must wait %s before speaking again.", timeStr.c_str());
		return true;
	}

	return false;
}