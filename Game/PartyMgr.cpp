/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PartyMgr.cpp"
*
*/
PartyMgr::PartyMgr()
{
	this->SetPartyLoop(0);
}

PartyMgr::~PartyMgr()
{
	this->ErasePartyData();
}

void PartyMgr::LoadPartyList()
{
	this->CreatePartyData(sObjectMgr->GetMaxPlayers());

	for ( uint16 i = 0; i < sObjectMgr->GetMaxPlayers(); ++i )
	{
		this->GetPartyData(i)->Reset();
		this->GetPartyData(i)->SetID(i);
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Party...");

	uint32 count = 0;

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_PARTY_SELECT);
	stmt->setUInt16(0, sGameServer->GetTransferPartyServer());

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
    {
        do
		{
			Field* fields = result->Fetch();

			uint16 id = fields[0].GetUInt16();

			if ( id >= sObjectMgr->GetMaxPlayers() )
			{
				continue;
			}

			this->GetPartyData(id)->SetStatus(PARTY_ACTIVE);
			++count;
		}
		while (result->NextRow());
    }

	for ( uint16 i = 0; i < sObjectMgr->GetMaxPlayers(); ++i )
	{
		if ( this->GetPartyData(i)->GetStatus() != PARTY_ACTIVE )
		{
			continue;
		}

		Party * mParty = this->GetPartyData(i);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_PARTY_MEMBER_SELECT);
		stmt->setUInt16(0, sGameServer->GetTransferPartyServer());
		stmt->setUInt16(1, mParty->GetID());

		PreparedQueryResult party_member_result = MuDatabase.Query(stmt);

		if ( party_member_result )
		{
			do
			{
				Field* fields = party_member_result->Fetch();

				if ( fields[3].GetUInt32() && fields[4].GetUInt32() )
				{
					uint8 position = fields[1].GetUInt8();

					mParty->GetMember(position)->Reset();
					mParty->GetMember(position)->SetGUID(fields[0].GetUInt32());
					mParty->GetMember(position)->SetAccount(fields[4].GetUInt32());
					mParty->GetMember(position)->SetName(fields[2].GetCString());
					mParty->GetMember(position)->SetGenFamily(fields[5].GetUInt8());
					mParty->GetMember(position)->SetClass(fields[6].GetUInt8());
					mParty->GetMember(position)->SetLevel(fields[7].GetUInt16());

					mParty->GetMember(position)->SetStatus(PARTY_USER_FLAG_PLAYING);
				}
			}
			while (party_member_result->NextRow());

			mParty->MoveMembers();
		}
	}

    sLog->outInfo(LOG_DEFAULT, ">> Loaded %u party", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void PartyMgr::Update()
{
	for ( uint32 i = 0; i < this->GetPartyDataSize(); ++i )
	{
		if ( this->GetPartyData(i)->GetStatus() == PARTY_DELETE )
		{
			this->GetPartyData(i)->Destroy();
			continue;
		}

		this->GetPartyData(i)->Update();
	}
}

void PartyMgr::Delete(uint16 id)
{
	if ( id >= this->GetPartyDataSize() )
	{
		return;
	}

	this->GetPartyData(id)->Destroy();
}

Party * PartyMgr::Get(uint16 id)
{
	if ( id >= this->GetPartyDataSize() )
	{
		return nullptr;
	}

	return this->GetPartyData(id);
}

Party * PartyMgr::GetEmpty()
{
	int32 totalcount = 0;
	
	while ( true )
	{
		if ( this->GetPartyData(this->GetPartyLoop())->GetStatus() == PARTY_NONE )
		{
			return this->GetPartyData(this->GetPartyLoop());
		}

		this->IncreasePartyLoop(1);

		if ( this->GetPartyLoop() >= sObjectMgr->GetMaxPlayers() )
		{
			this->SetPartyLoop(0);
		}

		++totalcount;

		if ( totalcount >= sObjectMgr->GetMaxPlayers() )
		{
			break;
		}
	}

	return nullptr;
}

void PartyMgr::OnLogin(Player * mPlayer)
{
	for ( uint32 i = 0; i < this->GetPartyDataSize(); ++i )
	{
		if ( this->GetPartyData(i)->GetStatus() != PARTY_ACTIVE )
		{
			continue;
		}

		if ( this->GetPartyData(i)->OnLogin(mPlayer) )
		{
			return;
		}
	}
}

Party* PartyMgr::GetPartyByLeader(const char * name)
{
	std::string leader_name;
	std::string check_name = name;

	strToLower(check_name);

	for ( uint32 i = 0; i < this->GetPartyDataSize(); ++i )
	{
		if ( this->GetPartyData(i)->GetStatus() != PARTY_ACTIVE )
		{
			continue;
		}

		leader_name = this->GetPartyData(i)->GetMember(0)->GetName();

		strToLower(leader_name);

		if ( leader_name == check_name )
		{
			return this->GetPartyData(i);
		}
	}

	return nullptr;
}

Party* PartyMgr::GetPartyByMember(const char * name)
{
	std::string leader_name;
	std::string check_name = name;

	strToLower(check_name);

	for ( uint32 i = 0; i < this->GetPartyDataSize(); ++i )
	{
		if ( this->GetPartyData(i)->GetStatus() != PARTY_ACTIVE )
		{
			continue;
		}

		for ( int32 h = 0; h < MAX_PARTY_MEMBERS; ++h )
		{
			if ( this->GetPartyData(i)->GetMember(h)->GetStatus() == PARTY_USER_FLAG_NONE )
			{
				continue;
			}

			leader_name = GetPartyData(i)->GetMember(h)->GetName();

			strToLower(leader_name);

			if ( leader_name == check_name )
			{
				return this->GetPartyData(i);
			}
		}
	}

	return nullptr;
}

Party* PartyMgr::Create(Player* pLeader, Player* pMember)
{
	if ( !pLeader || !pMember )
	{
		return nullptr;
	}

	Party * pParty = sPartyMgr->GetEmpty();

	if ( !pParty )
	{
		pLeader->PartyRequestResult(PARTY_REQUEST_FAILED);
		pMember->PartyRequestResult(PARTY_REQUEST_FAILED);
		return nullptr;
	}

	if ( !pParty->AddMember(pLeader) )
	{
		this->Delete(pParty->GetID());
		pLeader->PartyRequestResult(PARTY_REQUEST_FAILED);
		pMember->PartyRequestResult(PARTY_REQUEST_FAILED);
		return nullptr;
	}

	if ( !pParty->AddMember(pMember) )
	{
		this->Delete(pParty->GetID());
		pLeader->SetPartyID(-1);
		pLeader->PartyRequestResult(PARTY_REQUEST_FAILED);
		pMember->PartyRequestResult(PARTY_REQUEST_FAILED);
		return nullptr;
	}

	pParty->SetStatus(PARTY_ACTIVE);

	pParty->UpdateDB();

	return pParty;
}

Party* PartyMgr::Create(Player* pLeader)
{
	if ( !pLeader )
	{
		return nullptr;
	}

	Party * pParty = sPartyMgr->GetEmpty();

	if ( !pParty )
	{
		pLeader->PartyRequestResult(PARTY_REQUEST_FAILED);
		return nullptr;
	}

	if ( !pParty->AddMember(pLeader) )
	{
		this->Delete(pParty->GetID());
		pLeader->PartyRequestResult(PARTY_REQUEST_FAILED);
		return nullptr;
	}

	pParty->SetStatus(PARTY_ACTIVE);

	pParty->UpdateDB();

	return pParty;
}