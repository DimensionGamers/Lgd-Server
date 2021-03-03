MiniMap::MiniMap()
{
	
}

MiniMap::~MiniMap()
{
	for (auto it = _miniMapInfo.begin(); it != _miniMapInfo.end(); ++it)
	{
		CLEAR_LIST(it->second);
	}

	_miniMapInfo.clear();
}

void MiniMap::Load()
{
	sLog->outLoad(true, "Loading Mini Map...");

	for (auto it = _miniMapInfo.begin(); it != _miniMapInfo.end(); ++it)
	{
		CLEAR_LIST(it->second);
	}

	_miniMapInfo.clear();

	uint32 count = 0;

	auto result = GameServerDatabase.PQuery("SELECT * FROM mini_map i WHERE i.server = '%u'", sGameServer->GetTransferServer());
	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			MiniMapInfo * info = new MiniMapInfo;

			info->SetWorld(reader.GetUInt16());
			info->SetIndex(reader.GetUInt8());
			info->SetGroup(reader.GetUInt8());
			info->SetType(reader.GetUInt8());
			info->SetX(reader.GetInt16());
			info->SetY(reader.GetInt16());
			info->SetText(reader.GetString().c_str());

			_miniMapInfo[info->GetWorld()].push_back(info);
			count++;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u mini map definitions", count);
}

void MiniMap::CGMiniMapStartPartyInfoRecv(Player* player)
{
	if (!player)
		return;

	player->SetMiniMapState(true);

	GCMiniMapInfoSend(player);
}

void MiniMap::CGMiniMapClosePartyInfoRecv(Player* player)
{
	if (!player)
		return;

	player->SetMiniMapState(false);
}

void MiniMap::CGMiniMapInfoRecv(Player* player, uint8 * packet)
{
	if (!player)
		return;
}

void MiniMap::GCMiniMapPartyInfoSend(Player* player)
{
	if (!player || !player->IsMiniMapState())
		return;

	auto party = player->GetParty();
	if (!party)
		return;

	uint8 buffer[256];
	POINTER_PCT(MINI_MAP_PARTY_HEAD, head, buffer, 0);
	POINTER_PCT(MINI_MAP_PARTY_BODY, body, buffer, sizeof(MINI_MAP_PARTY_HEAD));
	head->count = 0;

	for (int32 n = 0; n < MAX_PARTY_MEMBERS; ++n)
	{
		if (party->GetMember(n)->GetStatus() != PARTY_USER_FLAG_PLAYING)
			continue;

		auto member = party->GetMember(n)->GetPlayer();
		if (!member)
			continue;

		auto & data = body[head->count++];

		memcpy(data.name, member->GetName(), MAX_CHARACTER_LENGTH + 1);
		data.map = member->GetDisplayWorld();
		data.x = member->GetX();
		data.y = member->GetY();
	}

	head->Set(0xE7, 0x01, sizeof(MINI_MAP_PARTY_HEAD)+(head->count * sizeof(MINI_MAP_PARTY_BODY)));
	player->sendPacket(buffer);
}

void MiniMap::GCMiniMapInfoSend(Player* player)
{
	if (player->GetMiniMapWorld() == player->GetWorldId())
		return;

	player->SetMiniMapWorld(player->GetWorldId());

	auto itr = _miniMapInfo.find(player->GetWorldId());
	if (itr == _miniMapInfo.end())
		return;

	for (auto & info : itr->second)
	{
		PMSG_MINI_MAP_INFO_SEND pMsg;
		pMsg.index = info->GetIndex();
		pMsg.group = info->GetGroup();
		pMsg.type = info->GetType();
		pMsg.flag = 0;
		pMsg.x = info->GetX();
		pMsg.y = info->GetY();
		memcpy(pMsg.text, info->GetText(), 32);
		pMsg.text[32] = 0;

		player->SendPacket(&pMsg);
	}
}