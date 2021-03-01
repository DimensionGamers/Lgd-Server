SystemMessage::SystemMessage()
{

}

SystemMessage::~SystemMessage()
{
	MAP_CLEAR(SystemMessageMap::iterator, this->message_map);
}

void SystemMessage::Init()
{
	sLog->outInfo(LOG_DEFAULT, "Loading System Messages...");

	MAP_CLEAR(SystemMessageMap::iterator, this->message_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM system_message");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			SystemMessageData *add_message = new SystemMessageData;
			add_message->SetID(fields[0].GetUInt32());
			add_message->SetLanguage(fields[1].GetUInt8());
			add_message->SetText(fields[2].GetString());

			this->message_map[add_message->GetID()] = add_message;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u system messages", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

std::string SystemMessage::Get(uint32 id) const
{
	SystemMessageMap::const_iterator it = this->message_map.find(id);

	if ( it != this->message_map.end() )
	{
		return it->second->GetText();
	}
	else
	{
		return ">>> System Message Error <<<";
	}
}