Crywolf::Crywolf()
{

}

Crywolf::~Crywolf()
{

}

void Crywolf::GetStatus(std::shared_ptr<ServerSocket> socket)
{
	uint8 status = 0;

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_CRYWOLF_SELECT);

	if ( result )
	{
		Field* fields = result->Fetch();

		status = fields[0].GetUInt8();
	}
	else
	{
		PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CRYWOLF_CREATE);
		stmt->setUInt8(0, status);
		MuDatabase.Execute(stmt);
	}

	SL_CRYWOLF_REQUEST pMsg;
	pMsg.status = status;

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void Crywolf::UpdateStatus(uint8 * Packet)
{
	POINTER_PCT(SL_CRYWOLF_SAVE, lpMsg, Packet, 0);

	PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CRYWOLF_UPDATE);
	stmt->setUInt8(0, lpMsg->status);
	MuDatabase.Execute(stmt);

	SL_CRYWOLF_REQUEST pMsg;
	pMsg.status = lpMsg->status;

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}