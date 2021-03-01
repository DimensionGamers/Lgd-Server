/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerFriend.cpp"
*
*/

bool Player::IsFriend(char const* name) const
{
	std::string lower_name = name;
	strToLower(lower_name);

	FRIEND_LOOP(i)
	{
		std::string lower_tmp = this->GetFriend(i)->GetName();
		strToLower(lower_tmp);

		if (lower_name == lower_tmp)
		{
			return true;
		}
	}

	return false;
}

bool Player::AddFriend(const char* name, uint16 server)
{
	if ( this->IsFriend(name) )
		return false;

	FRIEND_LOOP(i)
	{
		if ( !strlen(this->GetFriend(i)->GetName()) )
		{
			this->GetFriend(i)->SetName(name);
			this->GetFriend(i)->SetServer(server);
			return true;
		}
	}

	return false;
}

void Player::FriendAddRequest(uint8 * Packet)
{
	POINTER_PCT_LOG(FRIEND_ADD, lpMsg, Packet, 0);

	if ( !Player::IsPlayerBasicState(this) )
	{
		this->FriendAddResult(FRIEND_ADD_NOT_FOUNDED, lpMsg->name);
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->FriendAddResult(FRIEND_ADD_NOT_FOUNDED, lpMsg->name);
		return;
	}

	if ( !sGameServer->IsFriendEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Friend system is disabled.");
		this->FriendAddResult(FRIEND_ADD_NOT_FOUNDED, lpMsg->name);
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, lpMsg->name, MAX_CHARACTER_LENGTH);
	memset(this->requested_friend, 0, MAX_CHARACTER_LENGTH + 1);
	strcpy(this->requested_friend, name);

	if ( this->IsFriend(this->requested_friend) )
	{
		this->FriendAddResult(FRIEND_ADD_ALREADY, this->requested_friend);
		return;
	}

	if ( this->FriendGetCount() >= FRIEND_MAX )
	{
		this->FriendAddResult(FRIEND_ADD_FULL, this->requested_friend);
		return;
	}

	if ( this->GetLevel() < sGameServer->GetFriendAddMinLevel() )
	{
		this->FriendAddResult(FRIEND_ADD_LOWLVL, this->requested_friend);
		return;
	}
	
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_FRIEND_ADD_REQUEST);
	stmt->setString(0, this->requested_friend);

	this->_friendAddCallBack = MuDatabase.AsyncQuery(stmt);
}

void Player::FriendAddCallBack(PreparedQueryResult result)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	if ( !result )
	{
		this->FriendAddResult(FRIEND_ADD_NOT_FOUNDED, this->requested_friend);
		return;
	}

	Field* fields = result->Fetch();

	uint32 tmp_id = fields[0].GetUInt32();
	bool online = fields[1].GetBool();
	uint16 server_code = fields[2].GetUInt16();
	uint8 authority = fields[3].GetUInt8();

	if ( !tmp_id )
	{
		this->FriendAddResult(FRIEND_ADD_NOT_FOUNDED, this->requested_friend);
		return;
	}

	if ( tmp_id == this->GetGUID() )
	{
		this->FriendAddResult(FRIEND_ADD_SELF, this->requested_friend);
		return;
	}

	if ( authority == AUTHORITY_CODE_ADMINISTRATOR && !this->IsAdministrator() )
	{
		this->FriendAddResult(FRIEND_ADD_NOT_FOUNDED, this->requested_friend);
		return;
	}

	if ( this->AddFriend(this->requested_friend, online ? server_code: -1) )
	{
		this->FriendAddResult(FRIEND_ADDED, this->requested_friend, online ? server_code: -1);
	}
	else
	{
		this->FriendAddResult(FRIEND_ADD_NOT_FOUNDED, this->requested_friend);
	}
}

void Player::FriendAddResult(uint8 result, const char * name, uint8 state)
{
	FRIEND_ADD_RESULT pMsg(result, name, state);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::FriendDelRequest(uint8 * Packet)
{
	POINTER_PCT_LOG(FRIEND_DEL, lpMsg, Packet, 0);

	if ( !this->IsPlaying() || this->IsServerQuit() )
	{
		this->FriendDelResult(0, lpMsg->name);
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, lpMsg->name, MAX_CHARACTER_LENGTH);
	STRING_SAFE(delete_name, MAX_CHARACTER_LENGTH + 1);
	strcpy(delete_name, name);


	FRIEND_LOOP(i)
	{
		if ( !memcmp(this->GetFriend(i)->GetName(), delete_name, MAX_CHARACTER_LENGTH) )
		{
			this->GetFriend(i)->Reset();
			this->FriendDelResult(1, delete_name);
			return;
		}
	}

	this->FriendDelResult(1, delete_name);
}

void Player::FriendDelResult(uint8 result, char * name)
{
	FRIEND_DEL_RESULT pMsg(result, name);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::FriendOnOff(const char * name, uint16 server, bool online)
{
	FRIEND_LOOP(i)
	{
		if (!memcmp(this->GetFriend(i)->GetName(), name, MAX_CHARACTER_LENGTH))
		{
			this->GetFriend(i)->SetServer(server);
			this->FriendStateSend(name, server);
			break;
		}
	}
}

void Player::FriendStateSend(const char * name, uint16 server)
{
	FRIEND_STATE pMsg(name, server);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::FriendMailRequest(uint8 * Packet)
{
	POINTER_PCT(FRIEND_MAIL, lpMsg, Packet, 0);

	if ( !Player::IsPlayerBasicState(this) )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_ERROR, lpMsg->window_guid);
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !sGameServer->IsFriendMailEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Mail system is disabled.");
		this->FriendMailResult(FRIEND_MAIL_SEND_ERROR, lpMsg->window_guid);
		return;
	}

	if ( !this->MoneyHave(sGameServer->GetFriendMailCost()) )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_SHORT_ZEN, lpMsg->window_guid);
		return;
	}

	if ( lpMsg->mail_size <= 0 || lpMsg->mail_size > MAIL_MESSAGE_LENGTH )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_ERROR, lpMsg->window_guid);
		return;
	}

	uint16 size = sizeof(FRIEND_MAIL);
	size -= MAIL_MESSAGE_LENGTH;
	size += lpMsg->mail_size;

	if ( lpMsg->h.get_size() < size )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_ERROR, lpMsg->window_guid);
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, lpMsg->name, MAX_CHARACTER_LENGTH);
	STRING_SAFE_COPY(subject, MAIL_SUBJECT_LENGTH + 1, lpMsg->subject, MAIL_SUBJECT_LENGTH);
	STRING_SAFE_COPY(mail, MAIL_MESSAGE_LENGTH + 1, lpMsg->mail, lpMsg->mail_size);

	memset(this->requested_friend, 0, MAX_CHARACTER_LENGTH + 1);
	strcpy(this->requested_friend, name);

	memset(this->mail_data.subject, 0, MAIL_SUBJECT_LENGTH);
	strcpy(this->mail_data.subject, subject);

	memset(this->mail_data.mail, 0, MAIL_MESSAGE_LENGTH);
	strcpy(this->mail_data.mail, mail);

	this->mail_data.action = lpMsg->action;
	this->mail_data.direction = lpMsg->direction;
	this->mail_data.mail_size = lpMsg->mail_size;
	this->mail_data.window_guid = lpMsg->window_guid;
	memcpy(this->mail_data.name, name, MAX_CHARACTER_LENGTH);
	
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_ADD_REQUEST);
	stmt->setString(0, this->requested_friend);

	this->_mailAddCallBack = MuDatabase.AsyncQuery(stmt);
}

void Player::FriendMailRequestCallBack(PreparedQueryResult result)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	if ( !result )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_TARGET_NOT_EXIST, this->mail_data.window_guid);
		return;
	}

	STRING_SAFE_COPY(tmp_mail_data, MAIL_MESSAGE_LENGTH + 1, this->mail_data.mail, this->mail_data.mail_size);
	STRING_SAFE_COPY(tmp_subject_data, MAIL_SUBJECT_LENGTH + 1, this->mail_data.subject, MAIL_SUBJECT_LENGTH);

	Field* fields = result->Fetch();

	uint32 tmp_id = fields[0].GetUInt32();
	//uint32 tmp_acc = fields[1].GetUInt32();
	uint16 tmp_level = fields[2].GetUInt16();

	if ( !tmp_id )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_TARGET_NOT_EXIST, this->mail_data.window_guid);
		return;
	}

	if ( tmp_id == this->GetGUID() )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_SELF, this->mail_data.window_guid);
		return;
	}

	if ( tmp_level < 6 )
	{
		this->FriendMailResult(FRIEND_MAIL_SEND_TARGET_LOWLVL, this->mail_data.window_guid);
		return;
	}

	this->MoneyReduce(sGameServer->GetFriendMailCost());

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_MAIL);
	stmt->setUInt32(0, tmp_id);
	stmt->setString(1, this->GetName());
	stmt->setString(2, base64::encode((uint8*)tmp_subject_data, MAIL_SUBJECT_LENGTH));
	stmt->setString(3, base64::encode((uint8*)tmp_mail_data, MAIL_MESSAGE_LENGTH));
	stmt->setUInt8(4, this->mail_data.direction);
	stmt->setUInt8(5, this->mail_data.action);
	std::ostringstream ss;
	for ( uint8 i = 0; i < MAX_PREVIEW_DATA; i++ )
		ss << (uint32)this->preview[i].get() << " ";
	stmt->setString(6, ss.str());
	stmt->setUInt32(7, this->mail_data.window_guid);
	stmt->setInt64(8, time(nullptr));
	stmt->setUInt8(9, FRIEND_MAIL_STATE_NEW);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
	
	this->FriendMailResult(FRIEND_MAIL_SEND_SUCCESS, this->mail_data.window_guid);

	sServerLink->CharacterSignal(tmp_id, 0);
}

void Player::FriendMailResult(uint8 result, uint32 window_guid)
{
	FRIEND_MAIL_RESULT pMsg(result, window_guid);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::FriendMailList()
{
	MAIL_LOOP(i)
	{
		if ( !this->GetMailData(i)->IsInUse() )
			continue;

		MailData const* Mail = this->GetMailData(i);

		this->FriendMailSend(i, Mail->GetFrom(), TimeToTimestampStr(Mail->GetDate()).c_str(), Mail->GetSubject(), Mail->GetStatus());
	}
}

void Player::FriendMailSend(uint16 id, const char * from, const char * date, const char * subject, uint8 status)
{
	FRIEND_MAIL_LIST pMsg(id, from, date, subject, status == FRIEND_MAIL_STATE_OPEN ? 1: 0);
	this->sendPacket(MAKE_PCT(pMsg));
}

MailData * Player::GetMail(uint16 id)
{
	if ( id >= MAIL_MAX )
		return nullptr;

	return this->GetMailData(id);
}

void Player::UpdateMailStatus(uint16 id, uint8 status)
{
	if ( id >= MAIL_MAX )
		return;

	if ( !this->GetMailData(id)->IsInUse() )
		return;

	this->GetMailData(id)->SetStatus(status);
}

bool Player::DeleteMail(uint16 id)
{
	if ( id >= MAIL_MAX )
		return false;

	if ( !this->GetMailData(id)->IsInUse() )
		return false;

	uint32 delete_id = this->GetMailData(id)->GetID();

	this->GetMailData(id)->Reset();

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_DELETE_SINGLE);
	stmt->setUInt32(0, delete_id);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	return true;
}

void Player::FriendMailReadRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT(FRIEND_MAIL_READ, lpMsg, Packet, 0);

	MailData * Mail = this->GetMail(lpMsg->id);

	if ( !Mail )
	{
		sLog->outError(LOG_PLAYER, "%s -- %s -- MailID: %u", __FUNCTION__, this->BuildLog().c_str(), lpMsg->id);
		return;
	}

	FRIEND_MAIL_READ_RESULT pMsg;
	pMsg.id = lpMsg->id;
	pMsg.direction = Mail->GetDirection();
	pMsg.Action = Mail->GetAction();
	pMsg.message_size = strlen(Mail->GetText());

	if ( pMsg.message_size >= MAIL_MESSAGE_LENGTH )
	{
		pMsg.message_size = MAIL_MESSAGE_LENGTH - 1;
	}

	memcpy(pMsg.message, Mail->GetText(), pMsg.message_size);
	Mail->CopyPhoto(pMsg.photo);
	pMsg.h.set_size((pMsg.h.get_size() - MAIL_MESSAGE_LENGTH) + pMsg.message_size);

	this->sendPacket((uint8*)&pMsg, pMsg.h.get_size());

	this->UpdateMailStatus(lpMsg->id, FRIEND_MAIL_STATE_OPEN);
}

void Player::FriendMailDeleteRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT(FRIEND_MAIL_DELETE, lpMsg, Packet, 0);

	if ( !this->DeleteMail(lpMsg->id) )
	{
		this->FriendMailDeleteResult(0, lpMsg->id);
		return;
	}

	this->FriendMailDeleteResult(1, lpMsg->id);
}

void Player::FriendMailDeleteResult(uint8 result, uint16 id)
{
	FRIEND_MAIL_DELETE_RESULT pMsg(result, id);
	this->sendPacket(MAKE_PCT(pMsg));
}

int32 Player::FriendGetCount()
{
	int32 count = 0;

	FRIEND_LOOP(i)
	{
		if ( strlen(this->GetFriend(i)->GetName()) )
		{
			++count;
		}
	}

	return count;
}

uint8 Player::MailGetCount() const
{
	uint8 count = 0;

	MAIL_LOOP(i)
	{
		if ( !this->GetMailData(i)->IsInUse() )
			continue;

		++count;
	}

	return count;
}

bool Player::MailAdd(MailData const& Mail)
{
	MAIL_LOOP(i)
	{
		if ( this->GetMailData(i)->IsInUse() )
			continue;

		this->SetMailData(i, Mail);
		return true;
	}

	this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Your inbox is full. Delete some mails to receive new.");
	return false;
}

void Player::MailNewUpdate()
{
	if ( this->IsMailQuery() )
	{
		return;
	}

	this->SetMailQuery(true);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_NEW_UPDATE);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, FRIEND_MAIL_STATE_NEW);
	this->_mailUpdateCallBack = MuDatabase.AsyncQuery(stmt);
}

void Player::MailNewUpdateCallBack(PreparedQueryResult result)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	this->SetMailQuery(false);

	if ( !result )
		return;

	MailData Mail;
	int32 count = 0;
	
	do
	{
		Field* fields = result->Fetch();

		Mail.Reset();

		Mail.SetFrom(fields[0].GetCString());
		Mail.SetSubject(fields[1].GetBase64String().c_str());
		Mail.SetText(fields[2].GetBase64String().c_str());
		Mail.SetDirection(fields[3].GetUInt8());
		Mail.SetAction(fields[4].GetUInt8());

		const char* data = fields[5].GetCString();

		if ( data )
		{
			Tokens tokens(data, ' ');

			for (uint8 i = 0; i < MAX_PREVIEW_DATA; i++)
			{
				if (tokens.size() > i)
				{
					Mail.SetPhoto(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}

		Mail.SetWindowGUID(fields[6].GetUInt32());
		Mail.SetDate(fields[7].GetInt64());
		Mail.SetStatus(fields[8].GetUInt8());
		Mail.SetID(fields[9].GetUInt32());
		Mail.SetInUse(true);

		if ( !this->MailAdd(Mail) )
			break;

		++count;
	}
	while(result->NextRow());

	if ( count > 0 )
	{
		this->MailDBUpdate();
		this->SendNotice(NOTICE_NORMAL_BLUE, "Mail received.");
		this->FriendMailList();
	}
}

void Player::MailDBUpdate()
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	MAIL_LOOP(i)
	{
		if ( this->GetMailData(i)->GetStatus() == FRIEND_MAIL_STATE_NEW )
		{
			this->GetMailData(i)->SetStatus(FRIEND_MAIL_STATE_CLOSED);

			PreparedStatement * stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_UPDATE);
			stmt->setUInt8(0, this->GetMailData(i)->GetStatus());
			stmt->setUInt32(1, this->GetMailData(i)->GetID());
			trans->Append(stmt);
		}
	}

	MuDatabase.CommitTransaction(trans);
}

void Player::FriendChatRoomCreateRequest(uint8 * Packet)
{
	POINTER_PCT(FRIEND_CHAT_ROOM_CREATE_REQUEST, lpMsg, Packet, 0);

	STRING_SAFE_COPY(safe_name, (MAX_CHARACTER_LENGTH + 1), lpMsg->name, MAX_CHARACTER_LENGTH);

	this->FriendChatRoomCreateResult(1, "127.0.0.1", 0, 0, 0, safe_name);
}

void Player::FriendChatRoomCreateResult(uint8 result, const char * ip, uint16 room, uint32 ticket, uint8 type, const char * name)
{
	FRIEND_CHAT_ROOM_CREATE_RESULT pMsg(result, ip, room, ticket, type, name);
	this->sendPacket(MAKE_PCT(pMsg));
}
	
void Player::FriendChatRoomInvitationRequest(uint8 * Packet)
{
	//POINTER_PCT(FRIEND_CHAT_ROOM_INVITATION_REQUEST, lpMsg, Packet, 0);
}

void Player::FriendChatRoomInvitationResult(uint8 result, uint32 window_guid)
{
	FRIEND_CHAT_ROOM_INVITATION_RESULT pMsg(result, window_guid);
	this->sendPacket(MAKE_PCT(pMsg));
}