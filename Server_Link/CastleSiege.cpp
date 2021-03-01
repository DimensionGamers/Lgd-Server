CastleSiege::CastleSiege()
{

}

CastleSiege::~CastleSiege()
{

}

void CastleSiege::LoadData(std::shared_ptr<ServerSocket> socket)
{
	PreparedQueryResult result = MuDatabase.Query(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_LOAD_DATA));

	SL_CASTLE_SIEGE_LOAD_DATA_RESULT pMsg;
	pMsg.owner = 0;
	pMsg.status = 0;
	pMsg.tax_hunt = 3;
	pMsg.tax_chaos = 3;
	pMsg.tax_store = 3;
	pMsg.hunt_allowed = false;
	pMsg.money = 0;

	if ( !result )
	{
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CREATE_DATA);
		stmt->setUInt32(0, 0);
		stmt->setUInt8(1, 0);
		MuDatabase.Execute(stmt);
	}
	else
	{
		Field* fields = result->Fetch();

		pMsg.owner = fields[0].GetUInt32();
		pMsg.status = fields[1].GetUInt8();
		pMsg.tax_hunt = fields[2].GetInt32();
		pMsg.tax_chaos = fields[3].GetUInt8();
		pMsg.tax_store = fields[4].GetUInt8();
		pMsg.hunt_allowed = fields[5].GetBool();
		pMsg.money = fields[6].GetInt64();
	}

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.size);
}

void CastleSiege::LoadNpc(std::shared_ptr<ServerSocket> socket)
{
	PreparedQueryResult result = MuDatabase.Query(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_LOAD_NPC));

	uint8 buffer[15000];
	POINTER_PCT(SL_CASTLE_SIEGE_LOAD_NPC_HEAD, head, buffer, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_LOAD_NPC_BODY, body, buffer, sizeof(SL_CASTLE_SIEGE_LOAD_NPC_HEAD));
	head->count = 0;

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			body[head->count].npc = fields[0].GetUInt16();
			body[head->count].id = fields[1].GetUInt8();
			body[head->count].defense_level = fields[2].GetUInt8();
			body[head->count].regen_level = fields[3].GetUInt8();
			body[head->count].life_level = fields[4].GetUInt8();
			body[head->count].life = fields[5].GetInt32();

			++head->count;
		}
		while (result->NextRow());
	}

	head->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_NPC, sizeof(SL_CASTLE_SIEGE_LOAD_NPC_HEAD) + (sizeof(SL_CASTLE_SIEGE_LOAD_NPC_BODY) * head->count));

	socket->QueuePacket(buffer, head->h.get_size());
}

void CastleSiege::LoadGuildRegister(std::shared_ptr<ServerSocket> socket)
{
	PreparedQueryResult result = MuDatabase.Query(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_SELECT_REGISTERED_GUILD));

	uint8 buffer[15000];
	POINTER_PCT(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD, head, buffer, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_BODY, body, buffer, sizeof(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD));
	head->count = 0;

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			if ( !fields[3].GetUInt32() || !fields[6].GetUInt32() )
				continue;

			Guild* pGuild = sGuildMgr->GetGuild(fields[0].GetUInt32());

			if ( !pGuild )
				continue;

			SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_BODY & add_guild = body[head->count];

			add_guild.guid = fields[0].GetUInt32();
			add_guild.marks = fields[1].GetInt32();
			add_guild.register_id = 0x100000 - fields[2].GetInt32();
			add_guild.level = fields[4].GetUInt16() + fields[5].GetUInt16();
			add_guild.members = pGuild->GetTotalMembers();
			add_guild.score = (add_guild.marks * 5) + add_guild.members + (add_guild.level / 4);
			add_guild.total_score = ((int64)(uint32)CASTLE_SIEGE_SCORE_MACRO(fields[2].GetInt32())) | (((int64)(uint32)CASTLE_SIEGE_SCORE_MACRO(add_guild.score))<<32);

			++head->count;
		}
		while(result->NextRow());
	}

	head->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_LOAD_REGISTERED_GUILD, sizeof(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD) + (sizeof(SL_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_BODY) * head->count));

	socket->QueuePacket(buffer, head->h.get_size());
}

void CastleSiege::SaveNpc(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_NPC_HEAD, head, packet, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_NPC_BODY, body, packet, sizeof(SL_CASTLE_SIEGE_SAVE_NPC_HEAD));

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CLEAR_NPC);
	trans->Append(stmt);

	for ( int32 i = 0; i < head->count; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_INSERT_NPC);
		stmt->setUInt16(0, body[i].npc);
		stmt->setUInt8(1, body[i].id);
		stmt->setUInt8(2, body[i].defense_level);
		stmt->setUInt8(3, body[i].regen_level);
		stmt->setUInt8(4, body[i].life_level);
		stmt->setInt32(5, body[i].life);
		trans->Append(stmt);
	}

	MuDatabase.CommitTransaction(trans);
}

void CastleSiege::ClearGuild()
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	trans->Append(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CLEAR_GUILD));
	trans->Append(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CLEAR_REGISTERED_GUILD));

	MuDatabase.CommitTransaction(trans);
}

void CastleSiege::InsertFinalGuild(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_HEAD, head, packet, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_BODY, body, packet, sizeof(SL_CASTLE_SIEGE_INSERT_FINAL_GUILD_HEAD));

	SQLTransaction trans = MuDatabase.BeginTransaction();

	trans->Append(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CLEAR_GUILD));

	for ( int32 i = 0; i < head->count; ++i )
	{
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_INSERT_GUILD);
		stmt->setUInt32(0, body[i].guild);
		stmt->setUInt8(1, body[i].side);
		stmt->setInt32(2, body[i].score);
		trans->Append(stmt);
	}

	MuDatabase.CommitTransaction(trans);
}

void CastleSiege::SaveTaxRate(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_TAX_RATE, lpMsg, packet, 0);

	PreparedStatement *stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_DATA_TAX);
	stmt->setInt32(0, lpMsg->tax_hunt);
	stmt->setUInt8(1, lpMsg->tax_chaos);
	stmt->setUInt8(2, lpMsg->tax_store);
	stmt->setBool(3, lpMsg->hunt_allowed);
	MuDatabase.Execute(stmt);

	sServerSocketMgr.SendPacketAll(packet, lpMsg->h.get_size());
}

void CastleSiege::UpdateMoney(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_MONEY, lpMsg, packet, 0);

	PreparedStatement *stmt = nullptr;

	if ( !lpMsg->type )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_DATA_MONEY);
		stmt->setInt64(0, lpMsg->money);
	}
	else if ( lpMsg->type == 1 )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_RESET_DATA_MONEY);
	}
	else if ( lpMsg->type == 2 )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_DATA_MONEY);
		stmt->setInt64(0, -lpMsg->money);
	}

	if ( stmt )
		MuDatabase.Execute(stmt);

	sServerSocketMgr.SendPacketAll(packet, lpMsg->h.get_size());
}

void CastleSiege::SaveOwnerStatus(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_SAVE_OWNER_STATUS, lpMsg, packet, 0);

	PreparedStatement *stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_DATA);
	stmt->setUInt32(0, lpMsg->owner);
	stmt->setBool(1, lpMsg->status ? true: false);
	MuDatabase.Execute(stmt);

	sServerSocketMgr.SendPacketAll(packet, lpMsg->h.get_size());
}

void CastleSiege::InsertRegisteredGuild(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_INSERT_REGISTERED_GUILD, lpMsg, packet, 0);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_INSERT_REGISTERED_GUILD);
	stmt->setUInt32(0, lpMsg->guild);
	MuDatabase.Execute(stmt);

	if ( Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild) )
	{
		pGuild->SetRegisteredMarks(0);
		pGuild->SetCastleSiegeRegistered(true);
	}
}

void CastleSiege::DeleteRegisteredGuild(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_DELETE_REGISTERED_GUILD, lpMsg, packet, 0);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_DELETE_REGISTERED_GUILD);
	stmt->setUInt32(0, lpMsg->guild);
	MuDatabase.Execute(stmt);

	if ( Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild) )
	{
		pGuild->SetRegisteredMarks(0);
		pGuild->SetCastleSiegeRegistered(false);
	}
}

void CastleSiege::UpdateRegisteredGuild(uint8 * packet)
{
	POINTER_PCT(SL_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD, lpMsg, packet, 0);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD);
	stmt->setUInt32(0, lpMsg->marks);
	stmt->setUInt32(1, lpMsg->guild);
	MuDatabase.Execute(stmt);

	if ( Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild) )
	{
		pGuild->SetRegisteredMarks(lpMsg->marks);
	}
}

void CastleSiege::RegisteredGuild(uint8 * packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_REQUEST, lpMsg, packet, 0);

	uint8 buffer[10000];
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD, head, buffer, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_BODY, body, buffer, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD));
	head->count = 0;
	head->entry = lpMsg->entry;

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_SELECT_REGISTERED_GUILD_ID);
	stmt->setUInt32(0, lpMsg->guild);

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
	{
		Field* fields = result->Fetch();

		body[head->count].guild = lpMsg->guild;
		body[head->count].marks = fields[0].GetUInt32();
		body[head->count].register_id = fields[1].GetUInt32();

		++head->count;
	}

	head->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD) + (sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_BODY) * head->count));

	socket->QueuePacket(buffer, head->h.get_size());
}
	
void CastleSiege::RegisteredGuildAll(uint8 * packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_ALL_REQUEST, lpMsg, packet, 0);

	uint8 buffer[10000];
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD, head, buffer, 0);
	POINTER_PCT(SL_CASTLE_SIEGE_REGISTERED_GUILD_BODY, body, buffer, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD));
	head->count = 0;
	head->entry = lpMsg->entry;

	PreparedQueryResult result = MuDatabase.Query(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CASTLE_SIEGE_SELECT_REGISTERED_GUILD_ALL));

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			body[head->count].guild = fields[0].GetUInt32();
			body[head->count].marks = fields[1].GetUInt32();
			body[head->count].register_id = fields[2].GetUInt32();

			++head->count;
		}
		while(result->NextRow());
	}

	head->h.set(HEADCODE_SERVER_LINK_CASTLE_SIEGE_REGISTERED_GUILD_ALL, sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_HEAD) + (sizeof(SL_CASTLE_SIEGE_REGISTERED_GUILD_BODY) * head->count));

	socket->QueuePacket(buffer, head->h.get_size());
}