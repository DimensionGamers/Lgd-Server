CharacterCreateQueue::CharacterCreateQueue()
{

}

CharacterCreateQueue::~CharacterCreateQueue()
{
	CharacterCreateDataQueue.clear();
}

void CharacterCreateQueue::Queue(CharacterCreateDataPtr data)
{
	CharacterCreateDataQueue.add(data);
}

void CharacterCreateQueue::Update()
{
	CharacterCreateDataPtr data;

	while ( !this->CharacterCreateDataQueue.empty() && this->CharacterCreateDataQueue.next(data) )
	{
		this->Process(data);
	}
}

void CharacterCreateQueue::Process(CharacterCreateDataPtr data)
{
	SL_CHARACTER_CREATE_RESULT pMsg;
	pMsg.result = CHARACTER_CREATE_RESULT_ERROR;
	pMsg.entry = data->entry;
	pMsg.h.server = data->server;
	pMsg.ms_time = data->ms_time;
	pMsg.character_class = data->character_class;
	memcpy(pMsg.character_name, data->character_name, MAX_CHARACTER_LENGTH + 1);
	pMsg.slot = data->slot;
	pMsg.level = data->level;
	
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_SELECT_GUID_BY_NAME);
	stmt->setString(0, data->character_name);

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( !result )
	{
		SQLTransaction trans = MuDatabase.BeginTransaction();
	
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_CREATE);
		
		stmt->setUInt32(0, data->account_id);
		stmt->setUInt8(1, data->character_class);
		stmt->setString(2, data->character_name);
		stmt->setUInt32(3, data->strength);
		stmt->setUInt32(4, data->agility);
		stmt->setUInt32(5, data->vitality);
		stmt->setUInt32(6, data->energy);
		stmt->setUInt32(7, data->leadership);
		stmt->setInt32(8, data->life);
		stmt->setInt32(9, data->mana);
		stmt->setUInt32(10, data->points);
		stmt->setUInt32(11, data->level);
		stmt->setUInt16(12, data->world);
		stmt->setInt16(13, 130);
		stmt->setInt16(14, 130);
		stmt->setUInt8(15, data->slot);

		trans->Append(stmt);
	
		MuDatabase.CommitTransaction(trans);

		pMsg.result = CHARACTER_CREATE_RESULT_SUCCESS;
	}

	if ( data->socket && data->socket->IsOpen() )
		data->socket->QueuePacket((uint8*)&pMsg, pMsg.h.size);
}