MiniMap::MiniMap()
{
	
}

MiniMap::~MiniMap()
{
	for ( MiniMapInfoMap::iterator it = this->m_MiniMapInfo.begin(); it != this->m_MiniMapInfo.end(); ++it )
	{
		LIST_CLEAR(MiniMapInfoList::iterator, it->second);
	}

	this->m_MiniMapInfo.clear();
}

void MiniMap::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mini Map...");

	for ( MiniMapInfoMap::iterator it = this->m_MiniMapInfo.begin(); it != this->m_MiniMapInfo.end(); ++it )
	{
		LIST_CLEAR(MiniMapInfoList::iterator, it->second);
	}

	this->m_MiniMapInfo.clear();

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM mini_map i WHERE i.server = '%u'", sGameServer->GetTransferServer());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			MiniMapInfo * info = new MiniMapInfo;

			info->SetWorld(fields[i++].GetUInt16());
			info->SetIndex(fields[i++].GetUInt8());
			info->SetGroup(fields[i++].GetUInt8());
			info->SetType(fields[i++].GetUInt8());
			info->SetX(fields[i++].GetInt16());
			info->SetY(fields[i++].GetInt16());
			info->SetText(fields[i++].GetString().c_str());

			this->m_MiniMapInfo[info->GetWorld()].push_back(info);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mini map definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MiniMap::CGMiniMapStartPartyInfoRecv(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	pPlayer->SetMiniMapState(true);

	this->GCMiniMapInfoSend(pPlayer);
}

void MiniMap::CGMiniMapClosePartyInfoRecv(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	pPlayer->SetMiniMapState(false);
}

void MiniMap::CGMiniMapInfoRecv(Player* pPlayer, uint8 * Packet)
{
	if ( !pPlayer )
	{
		return;
	}

	
}

void MiniMap::GCMiniMapPartyInfoSend(Player* pPlayer)
{
	if (!pPlayer || !pPlayer->IsMiniMapState())
	{
		return;
	}

	Party* pParty = pPlayer->GetParty();

	if (!pParty)
	{
		return;
	}

	uint8 buffer[256];
	POINTER_PCT(MINI_MAP_PARTY_HEAD, head, buffer, 0);
	POINTER_PCT(MINI_MAP_PARTY_BODY, body, buffer, sizeof(MINI_MAP_PARTY_HEAD));
	head->count = 0;

	for (int32 n = 0; n < MAX_PARTY_MEMBERS; ++n)
	{
		if (pParty->GetMember(n)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		auto pMember = pParty->GetMember(n)->GetPlayer();
		if (!pMember)
		{
			continue;
		}

		auto & data = body[head->count++];

		memcpy(data.name, pMember->GetName(), MAX_CHARACTER_LENGTH + 1);
		data.map = pMember->GetDisplayWorld();
		data.x = pMember->GetX();
		data.y = pMember->GetY();
	}

	head->Set(0xE7, 0x01, sizeof(MINI_MAP_PARTY_HEAD)+(head->count * sizeof(MINI_MAP_PARTY_BODY)));
	pPlayer->sendPacket(buffer);
}

void MiniMap::GCMiniMapInfoSend(Player* pPlayer)
{
	if( pPlayer->GetMiniMapWorld() == pPlayer->GetWorldId() )
	{
		return;
	}

	pPlayer->SetMiniMapWorld(pPlayer->GetWorldId());

	MiniMapInfoMap::const_iterator itr = this->m_MiniMapInfo.find(pPlayer->GetWorldId());

	if ( itr == this->m_MiniMapInfo.end() )
	{
		return;
	}

	for( MiniMapInfoList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
	{
		PMSG_MINI_MAP_INFO_SEND pMsg;
		pMsg.index = (*it)->GetIndex();
		pMsg.group = (*it)->GetGroup();
		pMsg.type = (*it)->GetType();
		pMsg.flag = 0;
		pMsg.x = (*it)->GetX();
		pMsg.y = (*it)->GetY();
		memcpy(pMsg.text, (*it)->GetText(), 32);
		pMsg.text[32] = 0;


		pPlayer->SEND_PCT(pMsg);
	}
}