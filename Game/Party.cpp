/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Party.cpp"
*
*/

Party::Party()
{
	this->Reset();
}

void Party::UpdateDB()
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_PARTY_DELETE);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt16(this->GetID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_PARTY_DELETE_MEMBER);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt16(this->GetID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_PARTY_INSERT);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt16(this->GetID());
	trans->Append(stmt);

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() == PARTY_USER_FLAG_NONE)
			continue;

		stmt = MuDatabase.GetPreparedStatement(QUERY_PARTY_INSERT_MEMBER);
		stmt->setUInt16(sGameServer->GetServerCode());
		stmt->setUInt16(this->GetID());
		stmt->setUInt32(this->GetMember(i)->GetGUID());
		stmt->setUInt8(i);
		trans->Append(stmt);
	}

	MuDatabase.CommitTransaction(trans);
}

void Party::DeleteDB()
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_PARTY_DELETE);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt16(this->GetID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_PARTY_DELETE_MEMBER);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt16(this->GetID());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void Party::Update()
{
	if (this->GetStatus() != PARTY_ACTIVE)
	{
		return;
	}

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		auto pMember = this->GetMember(i);
		if (!pMember)
		{
			continue;
		}

		switch (pMember->GetStatus())
		{
		case PARTY_USER_FLAG_OFFLINE:
		{
			if (pMember->GetLeftTime()->Elapsed(sGameServer->GetPartyMemberOfflineInterval()))
			{
				this->DeleteMember(i);
				i = 0;
			}
		} break;

		case PARTY_USER_FLAG_PLAYING:
		{
			auto pPlayer = pMember->GetPlayer();

			if (!pPlayer || !pPlayer->Object::IsPlaying())
			{
				pMember->SetPlayer(nullptr);
				pMember->SetStatus(PARTY_USER_FLAG_OFFLINE);
				pMember->GetLeftTime()->Start();
				this->ListAllMembers();
				continue;
			}

			if (!pMember->GetAccount() || !pMember->GetGUID())
			{
				this->DeleteMember(i);
				i = 0;
				continue;
			}

			if (pPlayer->IsWorldFlag(WORLD_FLAG_BATTLE_ZONE) && pPlayer->GetGen()->GetFamily() != this->GetMember(0)->GetGenFamily())
			{
				this->DeleteMember(i);
				i = 0;
				continue;
			}

			pMember->SetGUID(pPlayer->GetGUID());
			pMember->SetName(pPlayer->GetName());
			pMember->SetGenFamily(pPlayer->GetGen()->GetFamily());
			pMember->SetClass(pPlayer->GetDBClass());
			pMember->SetLevel(pPlayer->GetTotalLevel());
		} break;
		}
	}

	if (this->GetMemberCount() <= 0)
	{
		this->SetStatus(PARTY_DELETE);
		return;
	}
	else if (this->GetMemberCount() <= 1 && !this->GetMatching()->IsEnabled())
	{
		this->SetStatus(PARTY_DELETE);
		return;
	}

	this->UpdateData();
}

void Party::Reset()
{
	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		this->GetMember(i)->Reset();
	}

	this->SetStatus(PARTY_NONE);

	this->GetMatching()->Reset();
}

void Party::MoveMembers()
{
	for (uint8 i = 0; i < MAX_PARTY_MEMBERS - 1; ++i)
	{
		if (!this->GetMember(i)->GetPlayer() && !this->GetMember(i)->GetAccount())
		{
			this->GetMember(i)->Set(this->GetMember(i + 1));
			this->GetMember(i + 1)->Reset();
		}
	}
}

uint8 Party::GetMemberCount() const
{
	uint8 count = 0;

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() == PARTY_USER_FLAG_NONE)
		{
			continue;
		}

		++count;
	}

	return count;
}

bool Party::IsMember(uint32 guid)
{
	if ( !guid )
		return false;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetGUID() == guid )
		{
			return true;
		}
	}

	return false;
}

uint8 Party::GetMemberPosition(Player * pPlayer) const
{
	if (!pPlayer)
	{
		return -1;
	}

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetAccount() == pPlayer->GetAccountData()->GetGUID() &&
			this->GetMember(i)->GetGUID() == pPlayer->GetGUID())
		{
			return i;
		}
	}

	return -1;
}

bool Party::AddMember(Player * pPlayer, bool update)
{
	if (!pPlayer)
	{
		return false;
	}

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		auto pMember = this->GetMember(i);

		if (pMember->GetStatus() != PARTY_USER_FLAG_NONE)
		{
			continue;
		}

		pMember->SetAccount(pPlayer->GetAccountData()->GetGUID());
		pMember->SetGUID(pPlayer->GetGUID());
		pMember->SetName(pPlayer->GetName());
		pMember->SetPlayer(pPlayer);
		pMember->SetGenFamily(pPlayer->GetGen()->GetFamily());
		pMember->SetStatus(PARTY_USER_FLAG_PLAYING);
		pMember->SetClass(pPlayer->GetDBClass());
		pMember->SetLevel(pPlayer->GetTotalLevel());

		pPlayer->SetPartyID(this->GetID());
		pPlayer->SetPartyMatchingID(PARTY_NULL);
		pPlayer->SetPartyMatchingAssist(false);

		if (update)
		{
			this->UpdateDB();
		}

		return true;
	}

	return false;
}

void Party::DeleteMember(uint8 position)
{
	if (position >= MAX_PARTY_MEMBERS)
	{
		return;
	}

	if (!sGameServer->IsPartyLeaderChange() && position == 0)
	{
		this->SetStatus(PARTY_DELETE);
		return;
	}

	auto pMember = this->GetMember(position);

	Player* pPlayer = pMember->GetPlayer();

	if (pPlayer && pPlayer->Object::IsPlaying() && pMember->GetStatus() == PARTY_USER_FLAG_PLAYING)
	{
		pPlayer->SetPartyID(-1);
		pPlayer->PartyDelete();
	}

	pMember->Reset();

	if (this->GetMemberCount() >= 1)
	{
		this->MoveMembers();
		this->UpdateDB();
		this->ListAllMembers();
	}
	else 
	{
		this->SetStatus(PARTY_DELETE);
	}
}

void Party::Destroy()
{
	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if (!pPlayer)
		{
			continue;
		}

		pPlayer->SetPartyID(PARTY_NULL);
		pPlayer->PartyDelete();

		this->GetMember(i)->Reset();
	}

	this->DeleteDB();
	this->Reset();

	sPartyMatching->InfoRemove(this->GetID());

	sLog->outInfo(LOG_PARTY, "%s -- %u", __FUNCTION__, this->GetID());
}

void Party::ListAllMembers()
{
	this->SendData();
	this->UpdateData();
}

void Party::UpdateData()
{
	this->SendStats();
	this->SendBuff();

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		auto pPlayer = this->GetMember(i)->GetPlayer();

		if (pPlayer)
		{
			sMiniMap->GCMiniMapPartyInfoSend(pPlayer);
		}
	}
}

void Party::SendData()
{
	uint8 buffer[255];
	POINTER_PCT(PARTY_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(PARTY_LIST_BODY, body, buffer, sizeof(PARTY_LIST_HEAD));
	head->count = 0;

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() == PARTY_USER_FLAG_NONE)
		{
			continue;
		}

		auto pPlayer = this->GetMember(i)->GetPlayer();
		auto & data = body[head->count++];

		data.number = i;
		memcpy(data.name, this->GetMember(i)->GetName(), MAX_CHARACTER_LENGTH);

		if (pPlayer)
		{
			data.life = pPlayer->PowerGet(POWER_LIFE);
			data.life_max = pPlayer->PowerGetTotal(POWER_LIFE);
			data.mana = pPlayer->PowerGet(POWER_MANA);
			data.mana_max = pPlayer->PowerGetTotal(POWER_MANA);
			data.world = pPlayer->GetDisplayWorld();
			data.world_x = pPlayer->GetX();
			data.world_y = pPlayer->GetY();
			data.helper = pPlayer->GetHelper()->IsStarted();
			data.server_code = (sGameServer->GetServerCode() % 20) + 1;
		}
		else
		{
			data.life = 0;
			data.life_max = 0;
			data.mana = 0;
			data.mana_max = 0;
			data.world = 0;
			data.world_x = 0;
			data.world_y = 0;
			data.helper = 0;
			data.server_code = 0;
		}

		data.assistant = this->GetMember(i)->IsAssisted();
		data.unk42 = 0;
		data.unk18 = 0;
		data.level = this->GetMember(i)->GetLevel();
	}

	head->Set(HEADCODE_PARTY_LIST, sizeof(PARTY_LIST_HEAD)+(head->count * sizeof(PARTY_LIST_BODY)));
	head->result = 0;

	PARTY_MEMBER_LOOP->sendPacket(buffer);
}

void Party::SendStats()
{
	uint8 buffer[255];
	POINTER_PCT(PARTY_LIFE_HEAD, head, buffer, 0);
	POINTER_PCT(PARTY_LIFE_BODY, body, buffer, sizeof(PARTY_LIFE_HEAD));
	head->count = 0;

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		auto pPlayer = this->GetMember(i)->GetPlayer();
		if (!pPlayer)
		{
			continue;
		}

		auto & data = body[head->count++];

		memcpy(data.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH + 1);

		data.life = pPlayer->PowerGetPercent(POWER_LIFE);
		data.mana = pPlayer->PowerGetPercent(POWER_MANA);
	}

	head->h.set(HEADCODE_PARTY_LIFE, sizeof(PARTY_LIFE_HEAD)+(head->count * sizeof(PARTY_LIFE_BODY)));

	PARTY_MEMBER_LOOP->sendPacket(buffer, head->h.get_size());
}

void Party::SendBuff()
{
	uint8 buffer[256];
	POINTER_PCT(PARTY_EFFECT_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(PARTY_EFFECT_LIST_BODY, body, buffer, sizeof(PARTY_EFFECT_LIST_HEAD));

	for (uint8 n = 0; n < MAX_PARTY_MEMBERS; ++n)
	{
		if (this->GetMember(n)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		auto pPlayer = this->GetMember(n)->GetPlayer();

		if (!pPlayer)
		{
			continue;
		}

		head->count = 0;
		memcpy(head->name, this->GetMember(n)->GetName(), MAX_CHARACTER_LENGTH + 1);

		BUFF_LOOP(h)
		{
			auto pBuffData = pPlayer->GetBuffData(h);

			if (!pBuffData)
			{
				continue;
			}

			if (pBuffData->GetID() == 0 || pBuffData->IsFlag(BUFF_FLAG_CONSTANT) || pBuffData->IsFlag(BUFF_FLAG_SHOP))
			{
				continue;
			}

			auto & data = body[head->count++];

			data.effect = pBuffData->GetID();
			data.count = pBuffData->GetRemainTime();
		}

		head->Set(HEADCODE_PARTY_BUFF, sizeof(PARTY_EFFECT_LIST_HEAD)+(head->count * sizeof(PARTY_EFFECT_LIST_BODY)));

		PARTY_MEMBER_LOOP->sendPacket(buffer);
	}
}

void Party::QuestMUObjectiveUpdate(QuestMUObjectiveType type, Unit* pUnit, int32 data)
{
	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
			continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if (!pPlayer)
			continue;

		if (!pPlayer->SameDimension(pUnit))
			continue;

		if (!IN_RANGE(pPlayer, pUnit, MAX_PARTY_QUEST_SUPPORT_UPDATE_DISTANCE))
			continue;

		pPlayer->QuestMUObjectiveUpdate(type, pUnit, data);
	}
}

void Party::QuestGuidedMonsterKill(Monster* pMonster)
{
	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !pPlayer->SameDimension(pMonster) )
			continue;

		if ( !IN_RANGE(pPlayer, pMonster, MAX_PARTY_QUEST_SUPPORT_UPDATE_DISTANCE) )
			continue;

		pPlayer->QuestGuidedMonsterKill(pMonster);
	}
}

void Party::GenKillReward(Player* pVictim)
{
	int16 high_level = this->GetPartyMemberHighestLevel(pVictim->GetWorldId(), pVictim->GetInstance(), pVictim->GetX(), pVictim->GetY(), MAX_PARTY_GEN_KILL_DISTANCE);
	uint8 count = this->GetPartyMembersWithinDistance(pVictim->GetWorldId(), pVictim->GetInstance(), pVictim->GetX(), pVictim->GetY(), MAX_PARTY_GEN_KILL_DISTANCE);

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !pPlayer->SameDimension(pVictim) )
			continue;

		if ( !IN_RANGE(pPlayer, pVictim, MAX_PARTY_GEN_KILL_DISTANCE) )
			continue;

		pPlayer->GenKillReward(pVictim, high_level, count);
	}

	pVictim->GenDeathPenalty(high_level, count);
}

uint8 Party::GetPartyMembersWithinDistance(uint16 world, int32 instance, int16 x, int16 y, uint8 distance)
{
	uint8 count = 0;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !pPlayer->SameDimension(world, instance) )
			continue;

		if ( Util::Distance(pPlayer->GetX(), pPlayer->GetY(), x, y) > distance )
			continue;

		count++;
	}

	return count;
}

int16 Party::GetPartyMemberHighestLevel(uint16 world, int32 instance, int16 x, int16 y, uint8 distance)
{
	int16 high_level = 0;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !pPlayer->SameDimension(world, instance) )
			continue;

		if ( Util::Distance(pPlayer->GetX(), pPlayer->GetY(), x, y) > distance )
			continue;

		if ( pPlayer->GetTotalLevel() > high_level )
			high_level = pPlayer->GetTotalLevel();
	}

	return high_level;
}

void Party::IncreaseAggroAround(Player* pPlayer, Monster* pMonster, int64 value)
{
	SafeAssert(pPlayer, "pPlayer == nullptr");
	SafeAssert(pMonster, "pMonster == nullptr");

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pMember = this->GetMember(i)->GetPlayer();

		if ( !pMember )
			continue;

		if ( !pMember->SameDimension(pPlayer) )
			continue;

		if ( !pMonster->ViewportExist(pMember) )
			continue;

		pMonster->GetThreatManager()->AddThreat(pMember, value);
	}
}

uint8 Party::GetPKLevel() const
{
	uint8 high_level = PK_STATUS_COMMONER;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pPlayer = this->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetPKLevel() > high_level )
			high_level = pPlayer->GetPKLevel();
	}

	return high_level;
}

void Party::SendNotice(const char * name, const char * msg,...)
{
	ARG(buffer, msg);

	PARTY_MEMBER_LOOP->SendNotice(name, buffer);
}

void Party::SendNotice(NoticeType type, const char * notice, ...)
{
	ARG(buffer, notice);

	PARTY_MEMBER_LOOP->SendNotice(type, buffer);
}

void Party::SendNoticeNormal(const char * name, const char * msg)
{
	PARTY_MEMBER_LOOP->SendNoticeNormal(name, msg);
}

void Party::SendNoticeNormal(NoticeType type, const char * notice)
{
	PARTY_MEMBER_LOOP->SendNoticeNormal(type, notice);
}

void Party::SendNotice(CustomMessageID type, const char * notice, ...)
{
	ARG(buffer, notice);

	PARTY_MEMBER_LOOP->SendNotice(type, buffer);
}
	
void Party::SendNoticeNormal(CustomMessageID type, const char * notice)
{
	PARTY_MEMBER_LOOP->SendNoticeNormal(type, notice);
}

bool Party::OnLogin(Player* pPlayer)
{
	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetAccount() == pPlayer->GetAccountData()->GetGUID() )
		{
			if ( this->GetMember(i)->GetGUID() != pPlayer->GetGUID() )
			{
				this->DeleteMember(i);
			}
			else
			{
				this->GetMember(i)->SetPlayer(pPlayer);
				this->GetMember(i)->SetStatus(PARTY_USER_FLAG_PLAYING);
				pPlayer->SetPartyID(this->GetID());
				pPlayer->SetPartyMatchingID(PARTY_NULL);
				pPlayer->SetPartyMatchingAssist(false);

				this->ListAllMembers();
			}

			return true;
		}
	}

	return false;
}

void Party::SendKillDamage(Player* pKiller, Unit* pVictim, int32 damage, int32 shield_damage, uint16 damage_flag)
{
	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pMember = this->GetMember(i)->GetPlayer();

		if ( !pMember || pMember == pKiller )
			continue;

		if ( !pMember->SameDimension(pVictim) )
			continue;

		if ( !pMember->ViewportExist(pVictim) )
			continue;

		pMember->SendDamage(pVictim, damage, shield_damage, 0, damage_flag);
	}
}

bool Party::IsEvomonSummoned() const
{
	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		Player* pMember = this->GetMember(i)->GetPlayer();

		if ( !pMember )
		{
			continue;
		}

		/*if ( pMember->IsEvomon() )
		{
			return true;
		}*/

		if ( pMember->GetEvomonPortal() )
		{
			return true;
		}
	}

	return false;
}

PartyMemberData const* Party::FindMember(uint16 index) const
{
	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		Player* pMember = this->GetMember(i)->GetPlayer();

		if (!pMember)
		{
			continue;
		}

		if (pMember->GetEntry() == index)
		{
			return this->GetMember(i);
		}
	}

	return nullptr;
}

PartyMemberData * Party::FindMember(uint16 index)
{
	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		Player* pMember = this->GetMember(i)->GetPlayer();

		if (!pMember)
		{
			continue;
		}

		if (pMember->GetEntry() == index)
		{
			return this->GetMember(i);
		}
	}

	return nullptr;
}