GuildCreateQueue::GuildCreateQueue()
{

}

GuildCreateQueue::~GuildCreateQueue()
{
	GuildCreateDataQueue.clear();
}

void GuildCreateQueue::Queue(GuildCreateDataPtr data)
{
	GuildCreateDataQueue.add(data);
}

void GuildCreateQueue::Update()
{
	GuildCreateDataPtr data;

	while ( !this->GuildCreateDataQueue.empty() && this->GuildCreateDataQueue.next(data) )
	{
		this->Process(data);
	}
}

void GuildCreateQueue::Process(GuildCreateDataPtr data)
{
	SL_GUILD_ADD pMsg;
	pMsg.h.server = data->server;
	pMsg.player = data->player;
	memcpy(pMsg.name, data->name, MAX_GUILD_NAME_LENGTH + 1);
	memcpy(pMsg.emblem, data->emblem, MAX_GUILD_EMBLEM_LENGTH);

	if ( sGuildMgr->GetGuild(data->name) != nullptr )
	{
		pMsg.result = 0;
		if ( data->socket && data->socket->IsOpen() )
			data->socket->QueuePacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	if ( sGuildMgr->GetGuild(data->player.guid) != nullptr )
	{
		pMsg.result = 0;

		if ( data->socket && data->socket->IsOpen() )
			data->socket->QueuePacket((uint8*)&pMsg, pMsg.h.size);
		return;
	}

	Guild * pGuild = new Guild;
	pGuild->Create(data->player.guid, data->name, data->emblem, true);
	pGuild->AddMember(data->player.guid, data->player.name, GUILD_RANK_MASTER, 0, data->server, true);
		
	sGuildMgr->AddGuild(pGuild);

	sLog->outInfo("guild", "Server: %d - Created Guild %s",
		data->server, pGuild->BuildLog().c_str());

	pMsg.result = 1;
	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}