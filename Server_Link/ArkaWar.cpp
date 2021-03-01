ArkaWar::ArkaWar()
{

}

ArkaWar::~ArkaWar()
{
	this->Clear();
}

bool ArkaWarGuildSort(ArkaWarGuild const& pGuild01, ArkaWarGuild const& pGuild02)
{
	if ( pGuild01.GetSigns() == pGuild02.GetSigns() )
	{
		return pGuild01.GetRegisterTime() < pGuild02.GetRegisterTime();
	}
	else
	{
		return pGuild01.GetSigns() >= pGuild02.GetSigns();
	}
}

void ArkaWar::GetArkaWarGuildSort(std::vector<ArkaWarGuild> & guild_list, int32 min_member)
{
	for ( ArkaWarGuildMap::const_iterator it = this->arka_war_guild.begin(); it != this->arka_war_guild.end(); ++it )
	{
		if ( it->second->GetMemberCount() < min_member )
		{
			continue;
		}

		ArkaWarGuild add_guild;
		add_guild.SetID(it->second->GetID());
		add_guild.SetSigns(it->second->GetSigns());
		add_guild.SetMemberCount(it->second->GetMemberCount());
		add_guild.SetRegisterTime(it->second->GetRegisterTime());
		guild_list.push_back(add_guild);
	}

	std::sort(guild_list.begin(), guild_list.end(), ArkaWarGuildSort);
}

void ArkaWar::MasterRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_MASTER_REGISTER, lpMsg, Packet, 0);

	SL_ARKA_WAR_MASTER_REGISTER pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;

	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(lpMsg->guild);

	if ( it != this->arka_war_guild.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	ArkaWarGuild * add_guild = new ArkaWarGuild;
	add_guild->SetID(lpMsg->guild);
	add_guild->SetSigns(0);
	add_guild->SetMemberCount(1);
	add_guild->SetAdmited(false);
	add_guild->SetRegisterTime(time(nullptr));
	this->arka_war_guild[lpMsg->guild] = add_guild;

	ArkaWarMember * add_member = new ArkaWarMember;
	add_member->SetID(lpMsg->player.guid);
	add_member->SetName(lpMsg->player.name);
	add_member->SetGuild(lpMsg->guild);

	this->arka_war_member[add_member->GetID()] = add_member;

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}
	
void ArkaWar::MemberRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_MEMBER_REGISTER, lpMsg, Packet, 0);

	SL_ARKA_WAR_MEMBER_REGISTER pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;

	ArkaWarMemberMap::const_iterator it = this->arka_war_member.find(lpMsg->player.guid);

	if ( it != this->arka_war_member.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	ArkaWarGuildMap::iterator itr = this->arka_war_guild.find(lpMsg->guild);

	if ( itr == this->arka_war_guild.end() )
	{
		pMsg.result = 2;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	if ( itr->second->GetMemberCount() > lpMsg->max_member )
	{
		pMsg.result = 3;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	itr->second->IncreaseMemberCount(1);

	ArkaWarMember * add_member = new ArkaWarMember;
	add_member->SetID(lpMsg->player.guid);
	add_member->SetName(lpMsg->player.name);
	add_member->SetGuild(lpMsg->guild);

	this->arka_war_member[add_member->GetID()] = add_member;

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}
	
void ArkaWar::SignRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_SIGN_REGISTER, lpMsg, Packet, 0);

	SL_ARKA_WAR_SIGN_REGISTER pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;
	pMsg.result = lpMsg->result;

	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(lpMsg->guild);

	if ( it == this->arka_war_guild.end() )
	{
		pMsg.result = 2;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	if ( lpMsg->result == 1 )
	{
		it->second->IncreaseSigns(lpMsg->signs);
	}

	pMsg.signs = it->second->GetSigns();

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}
	
void ArkaWar::Enter(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_ENTER, lpMsg, Packet, 0);

	SL_ARKA_WAR_ENTER pMsg;
	pMsg.player = lpMsg->player;

	ArkaWarMemberMap::const_iterator it = this->arka_war_member.find(lpMsg->player.guid);

	if ( it == this->arka_war_member.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void ArkaWar::MemberCount(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_MEMBER_COUNT, lpMsg, Packet, 0);

	SL_ARKA_WAR_MEMBER_COUNT pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;

	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(lpMsg->guild);

	if ( it == this->arka_war_guild.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	pMsg.count = it->second->GetMemberCount();
	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void ArkaWar::GuildRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_GUILD_REQUEST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(SL_ARKA_WAR_GUILD_HEAD, head, buffer, 0);
	POINTER_PCT(SL_ARKA_WAR_GUILD_BODY, body, buffer, sizeof(SL_ARKA_WAR_GUILD_HEAD));
	head->count = 0;

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_ARKA_WAR_SELECT);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			body[head->count].id = fields[0].GetUInt8();
			body[head->count].guild = fields[1].GetUInt32();
			body[head->count].attribute = fields[2].GetUInt8();
			++head->count;
		}
		while(result->NextRow());
	}

	head->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_REQUEST, sizeof(SL_ARKA_WAR_GUILD_HEAD) + (head->count * sizeof(SL_ARKA_WAR_GUILD_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}
	
void ArkaWar::GuildSave(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_GUILD_HEAD, head, Packet, 0);
	POINTER_PCT(SL_ARKA_WAR_GUILD_BODY, body, Packet, sizeof(SL_ARKA_WAR_GUILD_HEAD));

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ARKA_WAR_DELETE);
	trans->Append(stmt);

	for ( int32 i = 0; i < head->count; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ARKA_WAR_INSERT);
		stmt->setUInt8(0, body[i].id);
		stmt->setUInt32(1, body[i].guild);
		stmt->setUInt8(2, body[i].attribute);
		trans->Append(stmt);
	}

	MuDatabase.CommitTransaction(trans);

	head->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_GUILD_REQUEST, sizeof(SL_ARKA_WAR_GUILD_HEAD) + (head->count * sizeof(SL_ARKA_WAR_GUILD_BODY)));
	sServerSocketMgr.SendPacketAll(Packet, head->h.get_size());
}

void ArkaWar::SignRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_SIGN_REQUEST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(SL_ARKA_WAR_SIGN_REQUEST_HEAD, head, buffer, 0);
	POINTER_PCT(SL_ARKA_WAR_SIGN_REQUEST_BODY, body, buffer, sizeof(SL_ARKA_WAR_SIGN_REQUEST_HEAD));
	head->count = 0;
	head->player = lpMsg->player;

	std::vector<ArkaWarGuild> ArkaWarGuildList;
	this->GetArkaWarGuildSort(ArkaWarGuildList, 0);

	for ( std::vector<ArkaWarGuild>::const_iterator it = ArkaWarGuildList.begin(); it != ArkaWarGuildList.end(); ++it )
	{
		body[head->count].guild = it->GetID();
		body[head->count].signs = it->GetSigns();
		++head->count;
	}

	head->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_SIGN_REQUEST, sizeof(SL_ARKA_WAR_SIGN_REQUEST_HEAD) + (head->count * sizeof(SL_ARKA_WAR_SIGN_REQUEST_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}

void ArkaWar::GuildRemove(uint32 id)
{
	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(id);

	if ( it != this->arka_war_guild.end() )
	{
		this->MemberGuildRemove(id);

		delete it->second;
		this->arka_war_guild.erase(it);
	}
}

void ArkaWar::MemberGuildRemove(uint32 id)
{
	for ( ArkaWarMemberMap::const_iterator it = this->arka_war_member.begin(); it != this->arka_war_member.end(); )
	{
		if ( it->second->GetGuild() == id )
		{
			delete it->second;
			this->arka_war_member.erase(it++);
		}
		else
		{
			++it;
		}
	}
}
	
void ArkaWar::MemberRemove(uint32 id)
{
	ArkaWarMemberMap::const_iterator it = this->arka_war_member.find(id);

	if ( it != this->arka_war_member.end() )
	{
		ArkaWarGuildMap::const_iterator it2 = this->arka_war_guild.find(it->second->GetGuild());

		if ( it2 != this->arka_war_guild.end() )
		{
			it2->second->ReduceMemberCount(1);
		}

		delete it->second;
		this->arka_war_member.erase(it);
	}
}

void ArkaWar::GetGuildList(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_ARKA_WAR_LIST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(SL_ARKA_WAR_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SL_ARKA_WAR_LIST_GUILD_BODY, body, buffer, sizeof(SL_ARKA_WAR_LIST_HEAD));
	head->count = 0;
	head->type = 0;

	std::vector<ArkaWarGuild> ArkaWarGuildList;
	this->GetArkaWarGuildSort(ArkaWarGuildList, lpMsg->min_member);

	int32 guild_count = ArkaWarGuildList.size();

	if ( guild_count > lpMsg->max_guild )
	{
		guild_count = lpMsg->max_guild;
	}

	for ( int32 i = 0; i < guild_count; ++i )
	{
		ArkaWarGuildMap::iterator it = this->arka_war_guild.find(ArkaWarGuildList[i].GetID());

		if ( it != this->arka_war_guild.end() )
		{
			it->second->SetAdmited(true);
		}
	}

	for ( ArkaWarGuildMap::const_iterator it = this->arka_war_guild.begin(); it != this->arka_war_guild.end(); )
	{
		if ( !it->second->IsAdmited() )
		{
			this->MemberGuildRemove(it->second->GetID());
			delete it->second;
			this->arka_war_guild.erase(it++);
		}
		else
		{
			++it;
		}
	}

	for ( ArkaWarGuildMap::const_iterator it = this->arka_war_guild.begin(); it != this->arka_war_guild.end(); ++it )
	{
		body[head->count].guild = it->second->GetID();
		body[head->count].members = it->second->GetMemberCount();
		++head->count;
	}

	head->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_LIST, sizeof(SL_ARKA_WAR_LIST_HEAD) + (head->count * sizeof(SL_ARKA_WAR_LIST_GUILD_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}
	
void ArkaWar::GetMemberList(std::shared_ptr<ServerSocket> socket)
{
	uint8 buffer[8192];
	POINTER_PCT(SL_ARKA_WAR_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SL_ARKA_WAR_LIST_MEMBER_BODY, body, buffer, sizeof(SL_ARKA_WAR_LIST_HEAD));
	head->count = 0;
	head->type = 1;

	for ( ArkaWarMemberMap::const_iterator it = this->arka_war_member.begin(); it != this->arka_war_member.end(); ++it )
	{
		body[head->count].id = it->second->GetID();
		memcpy(body[head->count].name, it->second->GetName(), MAX_CHARACTER_LENGTH + 1);
		body[head->count].guild = it->second->GetGuild();
		++head->count;
	}

	head->h.set(HEADCODE_SERVER_LINK_ARKA_WAR_LIST, sizeof(SL_ARKA_WAR_LIST_HEAD) + (head->count * sizeof(SL_ARKA_WAR_LIST_MEMBER_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}

void ArkaWar::Clear()
{
	MAP_CLEAR(ArkaWarGuildMap::iterator, this->arka_war_guild);
	MAP_CLEAR(ArkaWarMemberMap::iterator, this->arka_war_member);
}