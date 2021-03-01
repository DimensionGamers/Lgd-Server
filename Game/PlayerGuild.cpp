/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerGuild.cpp"
*
*/

void Player::GuildCreateRequest(uint8 * Packet)
{
	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_GUILD_CREATE) )
		return;

	POINTER_PCT_LOG(GUILD_CREATE, lpMsg, Packet, 0);

	std::string guild_name = lpMsg->name;
	sGameServer->FilterText(guild_name, FILTER_FLAG_TALK | FILTER_FLAG_DB);

	if ( guild_name.empty() )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_GUILD_NAME_LENGTH + 1, guild_name.c_str(), guild_name.size() > MAX_GUILD_NAME_LENGTH ? MAX_GUILD_NAME_LENGTH : guild_name.size())

	sGuildMatching->GDGuildMatchingJoinCancelSend(this, 1);

	sServerLink->GuildCreateRequest(this, name, lpMsg->emblem);
}

void Player::GuildCreateResult(uint8 result, uint8 type)
{
	GUILD_CREATE_RESULT pMsg(result, type);
	this->SEND_PCT(pMsg);
}

void Player::GuildJoinRequest(uint8 * Packet)
{
	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_GUILD_JOIN) )
		return;

	POINTER_PCT_LOG(GUILD_JOIN_REQUEST, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if ( !this->CanInteract(pPlayer, true) )
	{
		this->GuildJoinResult(0x03);
		return;
	}

	if ( pPlayer->IsServerQuit() )
	{
		this->GuildJoinResult(0x03);
		return;
	}

	if ( !pPlayer->IsFlag(CHARACTER_FLAG_REQUEST) )
	{
		this->GuildJoinResult(0x03);
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None || this->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->GuildJoinResult(0x06);
		return;
	}

	if ( !this->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_COMMAND) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to send guild join request here.");
		this->GuildJoinResult(0x06);
		return;
	}

	if ( sArkaWar->GetState() > ARKA_WAR_STATE_CLOSED && sArkaWar->GetState() < ARKA_WAR_STATE_CLOSE )
	{
		this->GuildJoinResult(GUILD_ANS_ARKA_WAR_STATE);
		return;
	}

	if ( this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < 6 || pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < 6 )
	{
		this->GuildJoinResult(0x07);
		return;
	}

	if ( this->GetGuildID() != 0 )
	{
		this->GuildJoinResult(0x04);
		return;
	}

	Guild * pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->GuildJoinResult(0x05);
		return;
	}

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER )
	{
		this->GuildJoinResult(0x05);
		return;
	}

	if ( pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE )
	{
		this->GuildJoinResult(0x06);
		return;
	}

	if (sGameServer->guild_join_gens.get() && this->GetGen()->GetFamily() != pPlayer->GetGen()->GetFamily())
	{
		if (this->GetGen()->GetFamily() == 0)
			this->GuildJoinResult(GUILD_ANS_DIFFERENT_GENS);
		else if ( pPlayer->GetGen()->GetFamily() == 0 )
			this->GuildJoinResult(GUILD_ANS_MASTER_NOT_JOINED_GENS);
		else
			this->GuildJoinResult(GUILD_ANS_MASTER_NOT_JOINED_GENS_2);

		return;
	}

	if ( pGuild->GetTotalMembers() >= pPlayer->GuildGetMaxGuildMember() )
	{
		this->GuildJoinResult(0x02);
		return;
	}

	this->GetInterfaceState()->Set(InterfaceData::GuildRequest, pPlayer, 0, sGameServer->guild_max_wait_interval.get());
	pPlayer->GetInterfaceState()->Set(InterfaceData::GuildRequest, this, 0, sGameServer->guild_max_wait_interval.get());

	GUILD_JOIN_REQUEST_SEND pMsg(this->GetEntry());
	pPlayer->sendPacket(MAKE_PCT(pMsg));

	sLog->outInfo(LOG_GUILD, "[%s][%s] Requested [%s][%s] to join Guild [%u][%s]",
		this->GetAccountData()->GetAccount(), this->GetName(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), pGuild->GetID(), pGuild->GetName());
}
	
void Player::GuildJoinAnswer(uint8 * Packet)
{
	if ( !this->PacketTimeCheck() || !this->IsPlaying() || this->IsServerQuit() )
		return;

	POINTER_PCT_LOG(GUILD_JOIN_ANSWER, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if ( !pPlayer || !pPlayer->IsPlaying() || pPlayer->IsServerQuit() )
	{
		this->GuildJoinResult(0x02);
		return;
	}

	if ( !Player::InterfaceSharedCheck(this, pPlayer, InterfaceData::GuildRequest, 0) )
	{
		this->GuildJoinResult(0x00);
		return;
	}

	Guild * pGuild = this->GuildGet();

	if ( !pGuild )
	{
		this->GuildJoinResult(0x00);
		return;
	}

	if ( lpMsg->result == 1 )
	{
		sGuildMatching->GDGuildMatchingJoinCancelSend(pPlayer, 1);

		sServerLink->GuildMemberAddRequest(this, pPlayer, pGuild->GetID());
	}
	else
	{
		pPlayer->GuildJoinResult(0x00);
	}
	
	this->GetInterfaceState()->Reset();
	pPlayer->GetInterfaceState()->Reset();
}

void Player::GuildJoinResult(uint8 result)
{
	GUILD_JOIN_RESULT pMsg(result);
	this->SEND_PCT(pMsg);
}

void Player::GuildMemberDelRequest(uint8 * Packet)
{
	if ( !this->PacketTimeCheck() || !this->IsPlaying() || this->IsServerQuit() )
		return;

	if ( !this->CanInteract() )
		return;

	Guild * pGuild = this->GuildGet();

	if ( !pGuild )
		return;

	if ( sArkaWar->GetState() > ARKA_WAR_STATE_CLOSED && sArkaWar->GetState() < ARKA_WAR_STATE_CLOSE )
	{
		this->GuildMemberDelResult(0x06);
		return;
	}

	POINTER_PCT_LOG(GUILD_MEMBER_DELETE_REQUEST, lpMsg, Packet, 0);

	STRING_SAFE_COPY(member_name, (MAX_CHARACTER_LENGTH + 1), lpMsg->name, MAX_CHARACTER_LENGTH);
	STRING_SAFE_COPY(secure_code, MAX_SECURE_CODE_LENGTH + 1, lpMsg->secure_code, MAX_SECURE_CODE_LENGTH);

	if ( !this->SecureCodeCheck(secure_code) )
	{
		this->GuildMemberDelResult(0);
		return;
	}

	if ( pGuild->GetID() == this->GetGUID() ) // Soy el Guild Master
	{
		if ( !memcmp(member_name, this->GetName(), MAX_CHARACTER_LENGTH) )
		{
			if ( !sGameServer->guild_delete )
			{
				this->GuildMemberDelResult(3);
				return;
			}

			if ( pGuild->IsRegisteredInCaslteSiege() )
			{
				this->GuildMemberDelResult(3);
				return;
			}

			if (pGuild->GetID() == sCastleSiege->GetCastleOwner())
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't remove Castle Siege owner guild.");
				this->GuildMemberDelResult(3);
				return;
			}

			if ( pGuild->GetWarData()->GetState() != GUILD_WAR_STATE_NONE )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't remove guild while in war.");
				this->GuildMemberDelResult(3);
				return;
			}

			if ( pGuild->GetAlliance() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't remove guild while in alliance.");
				this->GuildMemberDelResult(3);
				return;
			}

			if ( pGuild->GetHostil() )
			{
				sServerLink->GuildRelationshipRequest(pGuild->GetID(), pGuild->GetHostil(), GUILD_RELATIONSHIP_RIVAL, false);
			}

			sServerLink->GuildDeleteRequest(pGuild->GetID());
			this->GuildMemberDelResult(4);
		}
		else
		{
			GuildMember* pGuildMember = pGuild->FindMember(member_name);

			if (pGuildMember)
			{
				sServerLink->GuildMemberDelRequest(pGuild->GetID(), pGuildMember->GetID());
			}
		}
	}
	else
	{
		if ( memcmp(member_name, this->GetName(), MAX_CHARACTER_LENGTH) )
		{
			this->GuildMemberDelResult(0);
			return;
		}

		sServerLink->GuildMemberDelRequest(pGuild->GetID(), this->GetGUID());
	}
}

void Player::GuildMemberDelResult(uint8 result)
{
	GUILD_MEMBER_DELETE_RESULT pMsg(result);
	this->SEND_PCT(pMsg);
}

void Player::GuildNpcAnswer(uint8 * Packet)
{
	POINTER_PCT_LOG(GUILD_NPC_ANSWER, lpMsg, Packet, 0);

	if ( lpMsg->result == 1 )
	{
		if ( sGameServer->guild_create_min_level > this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() )
		{
			if ( this->GetInterfaceState()->GetID() == InterfaceData::Guild )
			{
				this->GetInterfaceState()->Reset();
			}

			return;
		}

		GUILD_NPC_ANSWER_RESULT pMsg;
		this->SEND_PCT(pMsg);
		return;
	}

	if ( this->GetInterfaceState()->GetID() == InterfaceData::Guild )
	{
		this->GetInterfaceState()->Reset();
	}
}

void Player::GuildListAll()
{
	Guild * pGuild = this->GuildGet();

	if ( !pGuild )
		return;

	uint8 buffer[sizeof(GUILD_LIST_ALL_HEAD) + sizeof(GUILD_LIST_ALL_BODY) * MAX_GUILD_MEMBER];
	POINTER_PCT(GUILD_LIST_ALL_HEAD, head, buffer, 0);
	POINTER_PCT(GUILD_LIST_ALL_BODY, body, buffer, sizeof(GUILD_LIST_ALL_HEAD));
	uint16 size = sizeof(GUILD_LIST_ALL_HEAD);
	head->count = 0;

	for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
	{
		if ( !pGuild->GetMember(i)->GetID() )
			continue;

		memcpy(body[head->count].name, pGuild->GetMember(i)->GetName(), MAX_CHARACTER_LENGTH);
		body[head->count].ranking = pGuild->GetMember(i)->GetRanking();
		body[head->count].Number = cast(uint8, pGuild->GetMember(i)->GetServer());
		body[head->count].server = pGuild->GetMember(i)->GetServer() & 0x7F;

		if ( pGuild->GetMember(i)->GetServer() != uint16(-1) )
			body[head->count].server |= 0x80;

		head->count++;
		size += sizeof(GUILD_LIST_ALL_BODY);
	}

	if ( head->count == 0 )
		return;

	memset(head->rival, 0, MAX_GUILD_NAME_LENGTH + 1);

	if ( Guild * rival = sGuildMgr->GetGuild(pGuild->GetHostil()) )
	{
		memcpy(head->rival, rival->GetName(), MAX_GUILD_NAME_LENGTH + 1);
	}

	head->h.set(0x52, size);
	head->result = 1;
	head->total_score = pGuild->GetScore();
	head->score = 0;

	this->sendPacket(buffer, size);
}

/*enum GUILD_TYPE
{
	G_TYPE_NONE = 0xff,
	G_TYPE_COMMON = 0x0,
	G_TYPE_GUARD = 0x1,
};*/

void Player::GuildUpdateViewport()
{
	Guild * pGuild = this->GuildGet();

	if ( !pGuild )
	{
		return;
	}

	GUILD_UPDATE_VIEWPORT_COMPLETE pMsg;
	pMsg.numberH = SET_NUMBERH(this->GetEntry()) & 0x7F;
	pMsg.numberL = SET_NUMBERL(this->GetEntry());

	if ( pGuild->GetMemberRanking(this) == GUILD_RANK_MASTER )
			pMsg.numberH |= 0x80;

	pMsg.guild_id = pGuild->GetID();
	pMsg.ranking = pGuild->GetMemberRanking(this);
	pMsg.type = GUILD_TYPE_TEST;
	pMsg.relationship = 0;
	pMsg.owner_status = sCastleSiege->CastleOwnerMember(this) || sCastleSiege->CastleOwnerUnionMember(this);
	
	this->sendPacket((uint8*)&pMsg, pMsg.h.get_size());

	if ( CC_MAP_RANGE(this->GetWorldId()) || !this->SendViewportData() )
		return;

	Object * pUnit;

	VIEWPORT_LOOP(pUnit)

		if ( !pUnit->IsPlayer() )
			continue;

		if ( this->IsGenPVP(pUnit->ToPlayer()) )
			continue;

		pMsg.relationship = this->GuildRelationShip(pUnit->ToPlayer());

		pUnit->sendPacket((uint8*)&pMsg, pMsg.h.get_size()); 

	VIEWPORT_CLOSE
}

void Player::GuildUpdateViewport(uint8 * Packet)
{
	if ( !this->IsPlaying() )
		return;

	POINTER_PCT_LOG(GUILD_VIEWPORT_REQUEST, lpMsg, Packet, 0);
	
	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->id);

	if ( !pGuild )
		return;

	GUILD_VIEWPORT pMsg;
	pMsg.id = pGuild->GetID();
	pMsg.type = 0;
	memcpy(pMsg.name, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);
	memset(pMsg.alliance, 0, MAX_GUILD_NAME_LENGTH);

	if ( Guild * pAlliance = sGuildMgr->GetGuild(pGuild->GetAlliance()) )
	{
		memcpy(pMsg.alliance, pAlliance->GetName(), MAX_GUILD_NAME_LENGTH);
	}
	
	memcpy(pMsg.emblem, pGuild->GetEmblem(), MAX_GUILD_EMBLEM_LENGTH);

	this->SEND_PCT(pMsg);
}

void Player::GuildUpdateViewportDel(bool master)
{
	GUILD_VIEWPORT_DEL pMsg(this->GetEntry(), master);
	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Player::GuildUpdateViewportList1()
{
	uint8 buffer[8192];
	POINTER_PCT(GUILD_VIEWPORT_HEAD, head, buffer, 0);
	POINTER_PCT(GUILD_VIEWPORT_BODY, body, buffer, sizeof(GUILD_VIEWPORT_HEAD));
	head->count = 0;

	Object* pUnit;
	Guild* pGuild = nullptr;
	Guild* pGuildMine = this->GuildGet();

	VIEWPORT_LOOP(pUnit)

		if ( !pUnit->IsPlayer() )
			continue;

		if ( !(pGuild = pUnit->ToPlayer()->GuildGet()) )
			continue;

		body[head->count].guild = pGuild->GetID();
		body[head->count].SetIndex(pUnit->GetEntry());
		body[head->count].relationship = sGuildMgr->GetRelationShip(pGuildMine, pGuild);
		memset(body[head->count].alliance, 0, MAX_GUILD_NAME_LENGTH);

		if ( Guild* pAlliance = sGuildMgr->GetGuild(pGuild->GetAlliance()) )
		{
			memcpy(body[head->count].alliance, pAlliance->GetName(), MAX_GUILD_NAME_LENGTH);
		}
		
		++head->count;
	VIEWPORT_CLOSE

	if ( !head->count )
		return;

	head->h.set(HEADCODE_GUILD_UPDATE_VIEWPORT_COMPLETE, sizeof(GUILD_VIEWPORT_HEAD) + sizeof(GUILD_VIEWPORT_BODY) * head->count);
	this->sendPacket(buffer, head->h.get_size());
}

void Player::GuildUpdateViewportList2()
{
	Guild* pGuild = this->GuildGet();

	if ( !pGuild )
		return;

	uint8 buffer[sizeof(GUILD_VIEWPORT_HEAD) + sizeof(GUILD_VIEWPORT_BODY)];
	POINTER_PCT(GUILD_VIEWPORT_HEAD, head, buffer, 0);
	POINTER_PCT(GUILD_VIEWPORT_BODY, body, buffer, sizeof(GUILD_VIEWPORT_HEAD));
	head->count = 0;

	body[0].guild = pGuild->GetID();
	body[0].SetIndex(this->GetEntry());
	body[0].relationship = GUILD_RELATIONSHIP_NONE;
	memset(body[0].alliance, 0, MAX_GUILD_NAME_LENGTH);

	if ( Guild* pAlliance = sGuildMgr->GetGuild(pGuild->GetAlliance()) )
	{
		memcpy(body[0].alliance, pAlliance->GetName(), MAX_GUILD_NAME_LENGTH);
	}
	
	++head->count;

	head->h.set(HEADCODE_GUILD_UPDATE_VIEWPORT_COMPLETE, sizeof(GUILD_VIEWPORT_HEAD) + sizeof(GUILD_VIEWPORT_BODY));
	this->sendPacket(buffer, head->h.get_size());

	Object* pUnit;

	VIEWPORT_LOOP(pUnit)

		if ( !pUnit->IsPlayer() )
			continue;

		body[0].relationship = sGuildMgr->GetRelationShip(pGuild, pUnit->ToPlayer()->GuildGet());

		if ( body[0].relationship != 1 )
		{
			pUnit->sendPacket(buffer, head->h.get_size());
		}

	VIEWPORT_CLOSE
}

void Player::GuildAssignStatus(uint8 * Packet)
{
	if ( !this->PacketTimeCheck() || !this->IsPlaying() )
		return;

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(GUILD_ASSIGN_REQUEST, lpMsg, Packet, 0);

	Guild * pGuild = this->GuildGet();

	if ( !pGuild )
	{
		this->GuildAssignStatusResult(lpMsg->type, GUILD_ANS_NOTEXIST_GUILD, lpMsg->name);
		return;
	}
	
	if ( pGuild->GetMemberRanking(this) != GUILD_RANK_MASTER )
	{
		this->GuildAssignStatusResult(lpMsg->type, GUILD_ANS_NOTEXIST_PERMISSION, lpMsg->name);
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, lpMsg->name, MAX_CHARACTER_LENGTH);

	if ( !memcmp(this->GetName(), name, MAX_CHARACTER_LENGTH) )
	{
		this->GuildAssignStatusResult(lpMsg->type, GUILD_ANS_NOTEXIST_PERMISSION, lpMsg->name);
		return;
	}

	if ( sArkaWar->GetState() > ARKA_WAR_STATE_CLOSED && sArkaWar->GetState() < ARKA_WAR_STATE_CLOSE )
	{
		this->GuildAssignStatusResult(lpMsg->type, GUILD_ANS_ASSIGN_ARKA_WAR_STATE, lpMsg->name);
		return;
	}

	if ( lpMsg->type == 1 || lpMsg->type == 2 )
	{
		int32 asistant_count = 0;
		int32 battle_master_count = 0;

		pGuild->GetRankedMembersCount(asistant_count, battle_master_count);

		if ( lpMsg->status == GUILD_RANK_ASISTANT )
		{
			if ( asistant_count != 0 )
			{
				this->GuildAssignStatusResult(lpMsg->type, GUILD_ANS_NOTEXIST_EXTRA_STATUS, lpMsg->name);
				return;
			}
		}

		if ( lpMsg->status == GUILD_RANK_BATTLE_MASTER )
		{
			if ( battle_master_count >= ((this->GetLevel() / 200) +1) )
			{
				this->GuildAssignStatusResult(lpMsg->type, GUILD_ANS_NOTEXIST_EXTRA_STATUS, lpMsg->name);
				return;
			}
		}
	}
	
	if ( lpMsg->type == 1 || lpMsg->type == 2 || lpMsg->type == 3 )
	{
		sServerLink->GuildMemberStatusRequest(pGuild->GetID(), lpMsg->name, lpMsg->status);
	}
}

void Player::GuildAssignStatusResult(uint8 type, uint8 result, char * name)
{
	GUILD_ASSIGN_RESULT pMsg(type,result,name);
	this->SEND_PCT(pMsg);
}
	
void Player::GuildAssignType(uint8 * Packet)
{
	if ( !this->PacketTimeCheck() || !this->IsPlaying() )
		return;

	POINTER_PCT_LOG(GUILD_ASSIGN_TYPE_REQUEST, lpMsg, Packet, 0);

	Guild * pGuild = this->GuildGet();

	if ( !pGuild )
	{
		this->GuildAssignTypeResult(lpMsg->type, GUILD_ANS_NOTEXIST_GUILD);
		return;
	}

	if ( pGuild->GetMemberRanking(this) != GUILD_RANK_MASTER )
	{
		this->GuildAssignTypeResult(lpMsg->type, GUILD_ANS_NOTEXIST_PERMISSION);
		return;
	}

	// GDGuildReqAssignType(aIndex, aRecv->btGuildType);
}

void Player::GuildAssignTypeResult(uint8 type, uint8 result)
{
	GUILD_ASSIGN_TYPE_RESULT pMsg(type, result);
	this->SEND_PCT(pMsg);
}

void Player::GuildRelationShipRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(GUILD_RELATIONSHIP_REQUEST, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		this->GuildJoinResult(0x03);
		return;
	}

	if ( !this->SameDimension(pPlayer) )
	{
		this->GuildJoinResult(0x06);
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None || this->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->GuildJoinResult(0x06);
		return;
	}

	if ( !this->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_COMMAND) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to use guild commands here.");
		this->GuildJoinResult(0x06);
		return;
	}

	Guild * pGuild01 = this->GuildGet();
	Guild * pGuild02 = pPlayer->GuildGet();

	if ( !pGuild01 || !pGuild02 )
	{
		this->GuildJoinResult(0x05);
		return;
	}

	if ( pGuild01->GetID() != this->GetGUID() || pGuild02->GetID() != pPlayer->GetGUID() )
	{
		this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_NOTEXIST_PERMISSION, lpMsg->GetTarget());
		return;
	}
	///- CHANGELOG: 03/10
	if ( sArkaWar->GetState() > ARKA_WAR_STATE_CLOSED && sArkaWar->GetState() < ARKA_WAR_STATE_CLOSE )
	{
		this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_ASSIGN_ARKA_WAR_STATE, lpMsg->GetTarget());
		return;
	}

	///- CHANGELOG: 03/10
	if ( sCastleSiege->GetState() >= CASTLE_SIEGE_STATE_NOTIFY && sCastleSiege->GetState() <= CASTLE_SIEGE_STATE_START )
	{
		this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_UNIONFAIL_BY_CASTLE, lpMsg->GetTarget());
		return;
	}

	uint8 relation_ship = sGuildMgr->GetRelationShip(pGuild01, pGuild02);

	switch ( lpMsg->request_type )
	{
	case GUILD_RELATIONSHIP_OPERATION_JOIN:
		{
			if ( !pPlayer->IsFlag(CHARACTER_FLAG_REQUEST) )
			{
				this->GuildJoinResult(0x06);
				return;
			}

			switch ( lpMsg->relationship )
			{
			case GUILD_RELATIONSHIP_UNION:
				{
					if ( sGameServer->guild_alliance_gens.get() && this->GetGen()->GetFamily() != pPlayer->GetGen()->GetFamily() )
					{
						if ( !this->GetGen()->IsFamily() )
						{
							this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_MASTER_NOT_JOINED_GENS, lpMsg->GetTarget());
						}
						else if ( !pPlayer->GetGen()->IsFamily() )
						{
							this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_MASTER_NOT_JOINED_GENS_2, lpMsg->GetTarget());
						}
						else
						{
							this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_DIFFERENT_GENS, lpMsg->GetTarget());
						}

						return;
					}

					if ( sGameServer->guild_alliance_min_members > pGuild01->GetTotalMembers() ||
						 sGameServer->guild_alliance_min_members > pGuild02->GetTotalMembers() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_CANNOT_BE_UNION_MASTER, lpMsg->GetTarget());
						return;
					}

					if ( pGuild01->IsRegisteredInCaslteSiege() || pGuild02->IsRegisteredInCaslteSiege() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_UNIONFAIL_BY_CASTLE, lpMsg->GetTarget());
						return;
					}

					if ( pGuild01->GetAlliance() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_EXIST_UNION, lpMsg->GetTarget());
						return;
					}

					if ( pGuild02->GetAlliance() && pGuild02->GetAlliance() != pGuild02->GetID() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUIDL_ANS_NOT_UNION_MASTER, lpMsg->GetTarget());
						return;
					}

					if ( relation_ship == GUILD_RELATIONSHIP_RIVAL )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_EXIST_RELATIONSHIP_RIVAL, lpMsg->GetTarget());
						return;
					}

					if ( pGuild02->GetAlliance() )
					{
						uint8 involved_guilds = sGuildMgr->GetInvolvedGuilds(pGuild02->GetAlliance());

						if ( involved_guilds >= sGameServer->guild_alliance_max.get() )
						{
							this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_EXCEED_MAX_UNION_MEMBER, lpMsg->GetTarget());
							return;
						}
					}

					if (sGameServer->IsGuildAllianceCastleSiegeCheck() && sCastleSiege->GetCastleOwner() == pGuild01->GetID())
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_EXIST_UNION, lpMsg->GetTarget());
						return;
					}
				} break;

			case GUILD_RELATIONSHIP_RIVAL:
				{
					if ( pGuild01->GetHostil() || pGuild02->GetHostil() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_EXIST_RIVAL, lpMsg->GetTarget());
						return;
					}

					if ( relation_ship == GUILD_RELATIONSHIP_UNION )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_EXIST_RELATIONSHIP_UNION, lpMsg->GetTarget());
						return;
					}
				} break;
			}

			this->GetInterfaceState()->Set(InterfaceData::GuildAlliance, pPlayer, 0, sGameServer->guild_max_wait_interval.get());
			pPlayer->GetInterfaceState()->Set(InterfaceData::GuildAlliance, this, 0, sGameServer->guild_max_wait_interval.get());

			GUILD_RELATIONSHIP_REQUEST_SEND pMsg(lpMsg->relationship, lpMsg->request_type, this->GetEntry());
			pPlayer->sendPacket(MAKE_PCT(pMsg));
		} break;

	case GUILD_RELATIONSHIP_OPERATION_BREAKOFF:
		{
			switch ( lpMsg->relationship )
			{
			case GUILD_RELATIONSHIP_UNION:
				{
					///- CHANGELOG: 03/10
					if ( sCastleSiege->GetState() >= CASTLE_SIEGE_STATE_NOTIFY && sCastleSiege->GetState() <= CASTLE_SIEGE_STATE_START )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_UNIONFAIL_BY_CASTLE, lpMsg->GetTarget());
						return;
					}

					if ( !pGuild01->GetAlliance() || !pGuild02->GetAlliance() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_NOTEXIST_UNION, lpMsg->GetTarget());
						return;
					}

					if ( pGuild01->GetAlliance() != pGuild02->GetAlliance() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_NOTEXIST_UNION, lpMsg->GetTarget());
						return;
					}

					if ( pGuild01->IsRegisteredInCaslteSiege() || pGuild02->IsRegisteredInCaslteSiege() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_UNIONFAIL_BY_CASTLE, lpMsg->GetTarget());
						return;
					}
					
					if ( pGuild01->GetID() == pGuild01->GetAlliance() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_NOTEXIST_UNION, lpMsg->GetTarget());
						return;
					}

					if (sGameServer->IsGuildAllianceCastleSiegeCheck() && sCastleSiege->GetCastleOwner() == pGuild01->GetAlliance())
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_UNIONFAIL_BY_CASTLE, lpMsg->GetTarget());
						return;
					}
					
					/*Guild* pAllianceMaster = sGuildMgr->GetGuild(pGuild01->GetAlliance());

					if ( pAllianceMaster && pAllianceMaster->IsRegisteredInCaslteSiege() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_UNIONFAIL_BY_CASTLE, lpMsg->GetTarget());
						return;
					}*/
					
					sServerLink->GuildRelationshipRequest(pGuild01->GetAlliance(), pGuild01->GetID(), GUILD_RELATIONSHIP_UNION, false);
				} break;

			case GUILD_RELATIONSHIP_RIVAL:
				{
					if ( pGuild01->GetHostil() != pGuild02->GetID() || pGuild02->GetHostil() != pGuild01->GetID() )
					{
						this->GuildRelationShipResult(lpMsg->relationship, lpMsg->request_type, GUILD_ANS_NOT_GUILD_RIVAL, lpMsg->GetTarget());
						return;
					}

					this->GetInterfaceState()->Set(InterfaceData::GuildAlliance, pPlayer, 0, sGameServer->guild_max_wait_interval.get());
					pPlayer->GetInterfaceState()->Set(InterfaceData::GuildAlliance, this, 0, sGameServer->guild_max_wait_interval.get());

					GUILD_RELATIONSHIP_REQUEST_SEND pMsg(lpMsg->relationship, lpMsg->request_type, this->GetEntry());
					pPlayer->sendPacket(MAKE_PCT(pMsg));
					return;
				} break;
			}
		} break;
	}
}

void Player::GuildRelationShipResult(uint8 relation, uint8 type, uint8 result, uint16 target)
{
	GUILD_RELATIONSHIP_RESULT pMsg(relation, type, result, target);
	this->SEND_PCT(pMsg);
}

void Player::GuildRelationShipAnswer(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, true, false))
	{
		return;
	}

	POINTER_PCT_LOG(GUILD_RELATIONSHIP_RESULT, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		this->GuildJoinResult(0x03);
		return;
	}

	if ( !this->SameDimension(pPlayer) )
	{
		this->GuildJoinResult(0x06);
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::GuildAlliance || this->GetInterfaceState()->GetID() != InterfaceData::GuildAlliance )
	{
		this->GuildJoinResult(0x06);
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetTarget() != this || this->GetInterfaceState()->GetTarget() != pPlayer )
	{
		this->GuildJoinResult(0x06);
		return;
	}

	Guild * pGuild01 = this->GuildGet();
	Guild * pGuild02 = pPlayer->GuildGet();

	if ( !pGuild01 || !pGuild02 )
	{
		this->GuildJoinResult(0x05);
		return;
	}

	uint8 relation_ship = sGuildMgr->GetRelationShip(pGuild01, pGuild02);

	if ( lpMsg->result == 1 )
	{
		switch ( lpMsg->relation_ship )
		{
		case GUILD_RELATIONSHIP_UNION:
			{
				sServerLink->GuildRelationshipRequest(pGuild01->GetID(), pGuild02->GetID(), GUILD_RELATIONSHIP_UNION, true);
			} break;

		case GUILD_RELATIONSHIP_RIVAL:
			{
				sServerLink->GuildRelationshipRequest(pGuild01->GetID(), pGuild02->GetID(), GUILD_RELATIONSHIP_RIVAL, relation_ship == GUILD_RELATIONSHIP_RIVAL ? false: true);
			} break;
		}
	}
	else
	{
		pPlayer->GuildRelationShipResult(lpMsg->relation_ship, lpMsg->request_type, GUILD_ANS_CANCLE_REQUEST, lpMsg->GetTarget());
	}

	this->GetInterfaceState()->Reset();
	pPlayer->GetInterfaceState()->Reset();
}

void Player::GuildRelationShipList()
{
	if ( !this->PacketTimeCheck() )
		return;

	if ( !this->IsPlaying() )
		return;

	sGuildMgr->SendAllianceList(this);
}

void Player::GuildAllianceKick(uint8 * Packet) // Solamente puede ser usado por el GMA para kickear miembros de la alianza
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	///- CHANGELOG: 03/10
	if ( sCastleSiege->GetState() >= CASTLE_SIEGE_STATE_NOTIFY && sCastleSiege->GetState() <= CASTLE_SIEGE_STATE_START )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_UNIONFAIL_BY_CASTLE, 0);
		return;
	}

	///- CHANGELOG: 03/10
	if ( sArkaWar->GetState() > ARKA_WAR_STATE_CLOSED && sArkaWar->GetState() < ARKA_WAR_STATE_CLOSE )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_ASSIGN_ARKA_WAR_STATE, 0);
		return;
	}

	Guild* pGuild = this->GuildGet();

	if ( !pGuild || !pGuild->GetAlliance() )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_NOTEXIST_UNION, 0);
		return;
	}

	if ( pGuild->GetMemberRanking(this) != GUILD_RANK_MASTER || pGuild->GetAlliance() != pGuild->GetID() )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_NOTEXIST_PERMISSION, 0);
		return;
	}

	POINTER_PCT_LOG(GUILD_ALLIANCE_KICK, lpMsg, Packet, 0);

	STRING_SAFE_COPY(target_guild, MAX_GUILD_NAME_LENGTH + 1, lpMsg->name, MAX_GUILD_NAME_LENGTH);

	Guild* pKick = sGuildMgr->GetGuild(target_guild);

	if ( !pKick )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_NOTEXIST_GUILD, 0);
		return;
	}

	if ( pKick->GetID() == pGuild->GetID() )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_NOTEXIST_PERMISSION, 0);
		return;
	}

	if ( !pKick->GetAlliance() || pKick->GetAlliance() != pGuild->GetAlliance() )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_NOTEXIST_UNION, 0);
		return;
	}

	if ( pGuild->IsRegisteredInCaslteSiege() || pKick->IsRegisteredInCaslteSiege() )
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_UNIONFAIL_BY_CASTLE, 0);
		return;
	}

	if (sGameServer->IsGuildAllianceCastleSiegeCheck() && sCastleSiege->GetCastleOwner() == pGuild->GetID())
	{
		this->GuildRelationShipResult(GUILD_RELATIONSHIP_UNION, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_ANS_UNIONFAIL_BY_CASTLE, 0);
		return;
	}

	sServerLink->GuildRelationshipRequest(pGuild->GetID(), pKick->GetID(), GUILD_RELATIONSHIP_UNION, false);
	GUILD_KICK_UNION pMsg(1, GUILD_RELATIONSHIP_OPERATION_BREAKOFF, GUILD_RELATIONSHIP_UNION);
	this->SEND_PCT(pMsg);
}

bool Player::GuildIsAllianceMaster()
{
	Guild * pGuild = this->GuildGet();

	if ( !pGuild || pGuild->GetMemberRanking(this) != GUILD_RANK_MASTER )
		return false;

	if ( !pGuild->GetAlliance() )
		return true;

	if ( pGuild->GetAlliance() != pGuild->GetID() )
		return false;

	return true;
}

Guild* Player::GuildGet() const
{
	return !this->GetGuildID() ? nullptr: sGuildMgr->GetGuild(this->GetGuildID());
}

uint8 Player::GuildGetRanking() const
{
	Guild const* pGuild = this->GuildGet();

	if ( !pGuild )
		return GUILD_RANK_NONE;

	return pGuild->GetMemberRanking(this);
}

uint8 Player::GuildRelationShip(Player* pPlayer)
{
	Guild* pGuild1 = this->GuildGet();
	Guild* pGuild2 = pPlayer->GuildGet();

	if ( !pGuild1 || !pGuild2 )
		return GUILD_RELATIONSHIP_NONE;

	return sGuildMgr->GetRelationShip(pGuild1, pGuild2);
}

bool Player::IsInGuildWar() const
{
	Guild * pGuild = this->GuildGet();

	if ( pGuild )
	{
		if ( pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE )
		{
			if ( pGuild->GetWarData()->GetType() == GUILD_WAR_TYPE_SOCCER )
			{
				return sBattleSoccerMgr->IsInField(this);
			}
			else
			{
				if ( GuildMember* pMember = pGuild->GetMemberData(this->GetGUID()) )
				{
					return pMember->IsWarInvolved();
				}
			}
		}
	}

	return false;
}

bool Player::IsInGuildWar(Player* pPlayer)
{
	if ( !pPlayer )
		return false;

	Guild * pGuild01 = this->GuildGet();
	Guild * pGuild02 = pPlayer->GuildGet();

	if ( !pGuild01 || !pGuild02 )
		return false;

	if ( pGuild01->GetWarData()->GetEnemy() == pGuild02 && 
		 pGuild02->GetWarData()->GetEnemy() == pGuild01 && 
		 pGuild01->GetWarData()->GetType() != GUILD_WAR_TYPE_NONE && 
		 pGuild02->GetWarData()->GetType() != GUILD_WAR_TYPE_NONE && 
		 pGuild01->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE && 
		 pGuild02->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE )
	{
		if ( pGuild01->GetWarData()->GetType() == GUILD_WAR_TYPE_NORMAL )
			return true;

		return sBattleSoccerMgr->IsInField(pPlayer) && sBattleSoccerMgr->IsInField(this);
	}

	return false;
}

int32 Player::GuildGetMaxGuildMember() const
{
	//int32 MaxGuildMember = this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() / 10;
	int32 MaxGuildMember = this->GetTotalLevel() / 10;

	if (this->GetClass() == Character::DARK_LORD)
	{
		MaxGuildMember += this->GetStat(LEADERSHIP) / 10;
	}

	if (MaxGuildMember > MAX_GUILD_MEMBER)
	{
		MaxGuildMember = MAX_GUILD_MEMBER;
	}

	return MaxGuildMember;
}