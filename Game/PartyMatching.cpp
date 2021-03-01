PartyMatching::PartyMatching()
{

}

PartyMatching::~PartyMatching()
{

}

void PartyMatching::HandleRegister(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	PARTY_MATCHING_REGISTER_RESULT pMsg;
	
	if (pPlayer->GetInterfaceState()->GetID() == InterfaceData::Party)
	{
		pMsg.result = 0xFFFFFFFE;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	if (pPlayer->GetPartyMatchingID() != PARTY_NULL)
	{
		pMsg.result = 0xFFFFFFFE;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	Party * pParty = pPlayer->GetParty();

	if (pParty)
	{
		if (pParty->GetLeader() != pPlayer)
		{
			pMsg.result = 0xFFFFFFFD;
			pPlayer->SendPacket(&pMsg);
			return;
		}

		if (pParty->GetMatching()->IsEnabled())
		{
			pMsg.result = 0xFFFFFFFE;
			pPlayer->SendPacket(&pMsg);
			return;
		}
	}
	else
	{
		pParty = sPartyMgr->Create(pPlayer);
	}

	if (!pParty)
	{
		pMsg.result = 0xFFFFFFFD;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	POINTER_PCT_CHECK(PARTY_MATCHING_REGISTER, lpMsg, Packet, 0);

	pParty->GetMatching()->SetEnabled(true);
	pParty->GetMatching()->SetText(lpMsg->text, PARTY_MATCHING_TEXT_LENGHT + 1);
	pParty->GetMatching()->SetPassword(lpMsg->password, PARTY_MATCHING_PASSWORD_LENGTH + 1);
	pParty->GetMatching()->SetMinLevel(lpMsg->min_level);
	pParty->GetMatching()->SetMaxLevel(lpMsg->max_level);
	pParty->GetMatching()->SetAutoAccept(lpMsg->auto_accept);
	pParty->GetMatching()->SetUsePassword(lpMsg->use_password);
	pParty->GetMatching()->SetRequiredElf(lpMsg->required_elf);

	sLog->outInfo(LOG_PARTY, "%s -- Party: [ID: %u, Text: %s, Pass: %s, Lvl: [%u/%u], AutoAccept: %u - UsePassword: %u - RequiredElf:%u] Player: %s",
		__FUNCTION__, pParty->GetID(), pParty->GetMatching()->GetText(), pParty->GetMatching()->GetPassword(), pParty->GetMatching()->GetMinLevel(), pParty->GetMatching()->GetMaxLevel(),
		pParty->GetMatching()->GetAutoAccept(), pParty->GetMatching()->GetUsePassword(), pParty->GetMatching()->GetRequiredElf(), pPlayer->BuildLog().c_str());

	pPlayer->SendPacket(&pMsg);
}

void PartyMatching::HandleSearch(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PARTY_MATCHING_SEARCH, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(PARTY_MATCHING_SEARCH_HEAD, head, buffer, 0);
	POINTER_PCT(PARTY_MATCHING_SEARCH_BODY, body, buffer, sizeof(PARTY_MATCHING_SEARCH_HEAD));
	head->count = 0;
	head->result = 0;
	head->page = lpMsg->page;
	head->page_max = 1;

	uint32 PageCount = 0;

	for (uint32 i = 0; i < sPartyMgr->GetPartyDataSize(); ++i)
	{
		Party * pParty = sPartyMgr->GetPartyData(i);

		if (!pParty)
		{
			continue;
		}

		if (pParty->GetStatus() != PARTY_ACTIVE)
		{
			continue;
		}

		if (!pParty->GetMatching()->IsEnabled())
		{
			continue;
		}

		if ((lpMsg->search_flags & 1) == 1)
		{
			if (pParty->GetMemberCount() >= MAX_PARTY_MEMBERS)
			{
				continue;
			}
		}

		//if ( (lpMsg->search_flags & 1) == 1 && strstr(pParty->GetMatchingText(), lpMsg->search_word) )
		if ((lpMsg->search_flags & 2) == 2 && FindString<std::string>(pParty->GetMatching()->GetText(), lpMsg->search_word) == size_t(-1))
		{
			continue;
		}

		if (head->count < 6 && (PageCount / 6) == (lpMsg->page - 1))
		{
			auto & data = body[head->count++];

			memcpy(data.text, pParty->GetMatching()->GetText(), PARTY_MATCHING_TEXT_LENGHT + 1);
			
			data.min_level = pParty->GetMatching()->GetMinLevel();
			data.max_level = pParty->GetMatching()->GetMaxLevel();
			data.members_count = pParty->GetMemberCount();
			data.use_password = pParty->GetMatching()->GetUsePassword();
			data.gens = sGameServer->party_allow_dif_gens.get() ? pPlayer->GetGen()->GetFamily() : pParty->GetMember(0)->GetGenFamily();

			for (int32 n = 0; n < pParty->GetMemberCount(); ++n)
			{
				auto & member = data.member[n];
				auto * pMember = pParty->GetMember(n);

				memset(member.name, 0, MAX_CHARACTER_LENGTH + 1);
				memcpy(member.name, pMember->GetName(), MAX_CHARACTER_LENGTH);

				member.race = pMember->GetMatchingClass();
				member.level = pMember->GetLevel();
			}
		}

		PageCount++;
	}

	int32 total_page = 0;

	if (PageCount > 0)
	{
		total_page = PageCount / 6;

		if (PageCount % 6 > 0)
		{
			total_page++;
		}

		if (PageCount < 6)
		{
			total_page = 1;
		}
	}

	head->page_max = total_page;
	head->Set(0xEF, 0x01, sizeof(PARTY_MATCHING_SEARCH_HEAD)+(head->count * sizeof(PARTY_MATCHING_SEARCH_BODY)));

	pPlayer->sendPacket(buffer);
}

void PartyMatching::HandleJoin(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	PARTY_MATCHING_JOIN_RESULT pMsg;

	if (pPlayer->GetInterfaceState()->GetID() == InterfaceData::Party)
	{
		pMsg.result = 0xFFFFFFFC;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	if (pPlayer->GetPartyMatchingID() != PARTY_NULL)
	{
		pMsg.result = 0xFFFFFFFC;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	if (pPlayer->GetPartyID() != PARTY_NULL)
	{
		pMsg.result = 0xFFFFFFFA;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	POINTER_PCT_CHECK(PARTY_MATCHING_JOIN, lpMsg, Packet, 0);

	Party * pParty = nullptr;

	if (lpMsg->random == 0)
	{
		if (!(pParty = this->FindPartyMatchingInfo(lpMsg->leader_name)))
		{
			pMsg.result = 0xFFFFFFFE;
			pPlayer->SendPacket(&pMsg);
			return;
		}
	}
	else
	{
		if (!(pParty = this->FindPartyMatchingInfo(pPlayer)))
		{
			pMsg.result = 0xFFFFFFFD;
			pPlayer->SendPacket(&pMsg);
			return;
		}
	}

	if (pParty->GetMatching()->GetUsePassword() && memcmp(pParty->GetMatching()->GetPassword(), lpMsg->password, PARTY_MATCHING_PASSWORD_LENGTH))
	{
		pMsg.result = 0xFFFFFFFF;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	pPlayer->SetPartyMatchingID(pParty->GetID());
	pPlayer->SetPartyMatchingAssist(lpMsg->automatic_helper_assist != 0);

	pMsg.result = 0;
	pMsg.use_password = pParty->GetMatching()->GetUsePassword();
	pMsg.gens = sGameServer->party_allow_dif_gens.get() ? pPlayer->GetGen()->GetFamily() : pParty->GetMember(0)->GetGenFamily();
	memcpy(pMsg.name, pParty->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH);
	memcpy(pMsg.text, pParty->GetMatching()->GetText(), PARTY_MATCHING_TEXT_LENGHT);
	pPlayer->SendPacket(&pMsg);

	Player * pLeader = pParty->GetLeader();

	if (pParty->GetMatching()->GetAutoAccept() == 0)
	{
		if (pLeader)
		{
			PARTY_MATCHING_JOIN_NOTIFY pMsg2;
			pLeader->SendPacket(&pMsg2);
		}
	}
	else
	{
		this->Notify(pParty, pPlayer, 1);
	}
}

void PartyMatching::HandleJoinData(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PARTY_MATCHING_JOIN_DATA, lpMsg, Packet, 0);

	PARTY_MATCHING_JOIN_DATA_RESULT pMsg;
	pMsg.result = 0xFFFFFFFF;
	pMsg.server = (sGameServer->GetServerCode() % 20) + 1;

	Party* pParty = sPartyMgr->Get(pPlayer->GetPartyMatchingID());

	if (pParty && (pParty->GetStatus() == PARTY_ACTIVE) && pParty->GetMatching()->IsEnabled())
	{
		pMsg.result = 0;
		pMsg.use_password = pParty->GetMatching()->GetUsePassword();
		pMsg.gens = sGameServer->party_allow_dif_gens.get() ? pPlayer->GetGen()->GetFamily() : pParty->GetMember(0)->GetGenFamily();
		memcpy(pMsg.name, pParty->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH);
		memcpy(pMsg.text, pParty->GetMatching()->GetText(), PARTY_MATCHING_TEXT_LENGHT);
		pMsg.data = 1;
	}
	else
	{
		pPlayer->SetPartyMatchingID(PARTY_NULL);
		pPlayer->SetPartyMatchingAssist(false);
	}

	pPlayer->SendPacket(&pMsg);
}

void PartyMatching::HandleJoinList(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PARTY_MATCHING_JOIN_LIST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(PARTY_MATCHING_JOIN_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(PARTY_MATCHING_JOIN_LIST_BODY, body, buffer, sizeof(PARTY_MATCHING_JOIN_LIST_HEAD));
	head->count = 0;
	head->result = 0;

	Party* pParty = pPlayer->GetParty();

	if (pParty && (pParty->GetLeader() == pPlayer))
	{
		PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
		for (PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr)
		{
			Player* pPlayer01 = itr->second;

			if (!pPlayer01)
			{
				continue;
			}

			if (pPlayer01->GetPartyMatchingID() == pParty->GetID())
			{
				auto & data = body[head->count++];

				data.race = pPlayer01->GetMatchingClass();
				data.level = pPlayer01->GetTotalLevel();
				memset(data.name, 0, MAX_CHARACTER_LENGTH + 1);
				memcpy(data.name, pPlayer01->GetName(), MAX_CHARACTER_LENGTH);
				data.data = 0;
				data.junk[0] = 0;
				data.junk[1] = 0;
				data.junk[2] = 0;
			}
		}
	}

	head->Set(0xEF, 0x04, sizeof(PARTY_MATCHING_JOIN_LIST_HEAD)+(head->count * sizeof(PARTY_MATCHING_JOIN_LIST_BODY)));

	pPlayer->sendPacket(buffer);
}

void PartyMatching::HandleAccept(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PARTY_MATCHING_ACCEPT, lpMsg, Packet, 0);

	PARTY_MATCHING_ACCEPT_RESULT pMsg;
	memcpy(pMsg.name, lpMsg->name, MAX_CHARACTER_LENGTH);
	pMsg.type = lpMsg->type;
	pMsg.flag = 0;
	pMsg.result = 0;

	Party * pParty = pPlayer->GetParty();

	if (lpMsg->type != 0)
	{
		if (!pParty || (pParty->GetLeader() != pPlayer) || !pParty->GetMatching()->IsEnabled())
		{
			pMsg.result = 0xFFFFFFFF;
			pPlayer->SendPacket(&pMsg);
			return;
		}

		if (pParty->GetMemberCount() >= MAX_PARTY_MEMBERS)
		{
			pMsg.result = 0xFFFFFFFE;
			pPlayer->SendPacket(&pMsg);
			return;
		}
	}

	if (!this->IsPartyMatchingJoin(lpMsg->name))
	{
		pMsg.result = 0xFFFFFFFF;
		pPlayer->SendPacket(&pMsg);
		return;
	}

	pPlayer->SendPacket(&pMsg);

	if (Player* pMember = sObjectMgr->FindPlayerByName(lpMsg->name))
	{
		this->Notify(pParty, pMember, ((lpMsg->type == 0) ? 2 : 1));

		//PMSG_PARTY_MATCHING_JOIN_LIST_RECV pMsg1;
		//this->JoinListRecv(pPlayer, (uint8*)&pMsg1);
	}
}

void PartyMatching::HandleCancelRegister(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PARTY_MATCHING_CANCEL, lpMsg, Packet, 0);

	this->CancelRegister(pPlayer, lpMsg->type, 0);
}

void PartyMatching::Notify(Party* pParty, Player* pPlayer, uint8 result)
{
	if (!pPlayer)
	{
		return;
	}

	if (result == 1 && pParty)
	{
		if (!pParty->AddMember(pPlayer, true))
		{
			pPlayer->PartyRequestResult(PARTY_REQUEST_FULL);
		}
	}

	pPlayer->SetPartyMatchingID(PARTY_NULL);
	pPlayer->SetPartyMatchingAssist(false);
}

void PartyMatching::CancelRegister(Player* pPlayer, uint8 type, uint8 flag)
{
	if (!pPlayer)
	{
		return;
	}

	PARTY_MATCHING_CANCEL_RESULT pMsg;
	pMsg.type = type;
	pMsg.result = 0;

	if (type == 0)
	{
		Party* pParty = pPlayer->GetParty();

		if (!pParty || pParty->GetLeader() != pPlayer || !pParty->GetMatching()->IsEnabled())
		{
			pMsg.result = 0xFFFFFFFF;
		}
		else
		{
			this->InfoRemove(pParty->GetID());

			pParty->GetMatching()->SetEnabled(false);
		}
	}
	else
	{
		if (pPlayer->GetPartyMatchingID() == PARTY_NULL)
		{
			pMsg.result = 0xFFFFFFFF;
		}
		else
		{
			pPlayer->SetPartyMatchingID(PARTY_NULL);
			pPlayer->SetPartyMatchingAssist(false);
		}
	}

	if (flag == 0)
	{
		pPlayer->SendPacket(&pMsg);
	}
}

void PartyMatching::InfoRemove(uint16 id)
{
	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for (PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		if (pPlayer->GetPartyMatchingID() == id)
		{
			pPlayer->SetPartyMatchingID(PARTY_NULL);
			pPlayer->SetPartyMatchingAssist(false);
		}
	}
}
	
Party * PartyMatching::FindPartyMatchingInfo(const char * name)
{
	for (uint32 i = 0; i < sPartyMgr->GetPartyDataSize(); ++i)
	{
		Party * pParty = sPartyMgr->GetPartyData(i);

		if (!pParty)
		{
			continue;
		}

		if (pParty->GetStatus() != PARTY_ACTIVE)
		{
			continue;
		}

		if (!pParty->GetMatching()->IsEnabled())
		{
			continue;
		}

		if (!memcmp(name, pParty->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH))
		{
			return pParty;
		}
	}

	return nullptr;
}

Party * PartyMatching::FindPartyMatchingInfo(Player* pPlayer)
{
	RandomValue<Party*> m_RandomValue(nullptr);

	for (uint32 i = 0; i < sPartyMgr->GetPartyDataSize(); ++i)
	{
		Party * pParty = sPartyMgr->GetPartyData(i);

		if (!pParty)
		{
			continue;
		}

		if (pParty->GetStatus() != PARTY_ACTIVE)
		{
			continue;
		}

		if (!pParty->GetMatching()->IsEnabled())
		{
			continue;
		}

		if (pParty->GetMatching()->GetUsePassword() == 1)
		{
			continue;
		}

		if (!sGameServer->party_allow_dif_gens && (pParty->GetMember(0)->GetGenFamily() != pPlayer->GetGen()->GetFamily()))
		{
			continue;
		}

		if (pParty->GetMemberCount() >= MAX_PARTY_MEMBERS)
		{
			continue;
		}

		if (pPlayer->GetTotalLevel() >= pParty->GetMatching()->GetMinLevel() && pPlayer->GetTotalLevel() <= pParty->GetMatching()->GetMaxLevel())
		{
			m_RandomValue.AddValue(pParty, 0);
		}
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

bool PartyMatching::IsPartyMatchingJoin(const char * name) const
{
	return sObjectMgr->FindPlayerByName(name) != nullptr;
}