MuRoomy::MuRoomy()
{
	this->SetActive(true);
}

MuRoomy::~MuRoomy()
{
	LIST_CLEAR(MuRoomyRewardList::iterator, this->reward_list);
}

void MuRoomy::LoadReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mu Roomy Reward...");

	LIST_CLEAR(MuRoomyRewardList::iterator, this->reward_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_mu_roomy_reward");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			MuRoomyReward * add_item = new MuRoomyReward;

			add_item->SetType(fields[0].GetUInt8());
			add_item->SetScoreMin(fields[1].GetUInt16());
			add_item->SetScoreMax(fields[2].GetUInt16());
			add_item->SetItem(fields[3].GetUInt16());
			add_item->SetLevel(fields[4].GetUInt8());

			this->reward_list.push_back(add_item);

			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mu roomy reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MuRoomy::Open(Player* pPlayer)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	if (pPlayer->GetInterfaceState()->GetID() != InterfaceData::None && !sShopMgr->IsShop(pPlayer->GetInterfaceState()->GetID()))
	{
		return;
	}

	if (pPlayer->GetPersonalStore()->IsBusy())
	{
		pPlayer->SendMessageBox(0, "Mu Roomy", "Can't play game while personal store is open.");
		return;
	}

	PMSG_EVENT_INVENTORY_OPEN_SEND pMsg;
	pMsg.id = EVENT_INVENTORY_TYPE_MU_ROOMY;
	pMsg.result = 1;
	pMsg.data = sGameServer->IsMuRoomySpecialEnabled() ? 1 : 0;

	if (!pPlayer->GetMuRoomyInfo()->IsPlayCard())
	{
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	pMsg.result = 2;
	pPlayer->SEND_PCT(pMsg);

	MU_ROOMY_START_REQUEST pMsgStart;
	pMsgStart.type = pPlayer->GetMuRoomyInfo()->GetType();
	this->Start(pPlayer, (uint8*)&pMsgStart);
}

void MuRoomy::Start(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsActive() || !sGameServer->IsMuRoomyEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetTransaction() == TRANSACTION_TYPE_BEGIN )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendMessageBox(0, "AUTH", "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MU_ROOMY_START_REQUEST, lpMsg, Packet, 0);

	MU_ROOMY_START_RESULT pMsg;

	bool bNewGame = false;

	if ( pPlayer->GetMuRoomyInfo()->IsPlayCard() )
	{
		if ( pPlayer->GetMuRoomyInfo()->GetType() != lpMsg->type )
		{
			return;
		}
	}
	else
	{
		if ( !sGameServer->IsMuRoomyEnabled() || !sMuRoomy->IsActive() )
		{
			return;
		}

		if ( lpMsg->type && !sGameServer->IsMuRoomySpecialEnabled() )
		{
			return;
		}

		if ( sGameServer->IsMuRoomyTicketRequired() )
		{
			if ( lpMsg->type )
			{
				if ( pPlayer->ItemFind(ITEMGET(14, 277), 0, false) == uint8(-1) )
				{
					this->SendMessage(pPlayer, 12, 0);
					return;
				}
			}

			if ( pPlayer->ItemFind(ITEMGET(14, 216), 0, true) == uint8(-1) )
			{
				this->SendMessage(pPlayer, 0, 0);
				return;
			}

			if ( lpMsg->type )
			{
				pPlayer->ItemFind(ITEMGET(14, 277), 0, true);
			}
		}

		pPlayer->GetMuRoomyInfo()->Init();
		pPlayer->GetMuRoomyInfo()->SetPlayCard(true);
		pPlayer->GetMuRoomyInfo()->SetType(lpMsg->type);

		this->SetCardDeck(pPlayer);

		bNewGame = true;
	}

	this->SetPlayCardInfo(pPlayer, pMsg.card_info);
	
	pMsg.card_count = pPlayer->GetMuRoomyInfo()->GetCardDeckCount();
	pMsg.special_card_count = pPlayer->GetMuRoomyInfo()->GetSpecialCardDeckCount();
	pMsg.type = pPlayer->GetMuRoomyInfo()->GetType();
	pMsg.scoreH = SET_NUMBERH(pPlayer->GetMuRoomyInfo()->GetScore());
	pMsg.scoreL = SET_NUMBERL(pPlayer->GetMuRoomyInfo()->GetScore());
	pMsg.slot_status[8] = pPlayer->GetMuRoomyInfo()->GetType() ? 0 : 1;
	pMsg.slot_status[9] = pPlayer->GetMuRoomyInfo()->GetType() ? 0 : 1;
	pPlayer->SEND_PCT(pMsg);

	if ( bNewGame )
	{
		this->SendMessage(pPlayer, 1, 0);
	}
}

void MuRoomy::Reveal(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsActive() || !sGameServer->IsMuRoomyEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MU_ROOMY_REVEAL_CARD_REQUEST, lpMsg, Packet, 0);

	if ( !pPlayer->GetMuRoomyInfo()->GetCardDeckCount() )
	{
		this->SendMessage(pPlayer, 2, 0);
		return;
	}

	uint8 btSlotNum = this->GetTempCardShowSlot(pPlayer);

	if ( btSlotNum == uint8(-1) )
	{
		this->SendMessage(pPlayer, 2, 0);
		return;
	}

	if ( btSlotNum == uint8(-2) )
	{
		this->SendMessage(pPlayer, 3, 0);
		return;
	}
	
	MU_ROOMY_REVEAL_CARD_RESULT pMsg;
	this->FillEmptySlotCard(pPlayer, pMsg.card_info);
	pMsg.card_count = pPlayer->GetMuRoomyInfo()->GetCardDeckCount();
	pMsg.special_card_count = pPlayer->GetMuRoomyInfo()->GetSpecialCardDeckCount();
	pPlayer->SEND_PCT(pMsg);
}

void MuRoomy::Move(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsActive() || !sGameServer->IsMuRoomyEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MU_ROOMY_CARD_MOVE_REQUEST, lpMsg, Packet, 0);

	uint8 source_slot = lpMsg->source_slot;
	uint8 target_slot = lpMsg->target_slot;

	if ( source_slot < 0 || source_slot >= 10 )
	{
		return;
	}

	if ( target_slot < 0 || target_slot >= 10 )
	{
		return;
	}

	if ( source_slot == target_slot )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->GetType() )
	{
		if ( source_slot == 8 || source_slot == 9 )
		{
			return;
		}

		if ( target_slot == 8 || target_slot == 9 )
		{
			return;
		}
	}
	else
	{
		if ( target_slot == 8 )
		{
			return;
		}

		/*if ( source_slot == 9 && (target_slot < 5 || target_slot > 7) )
		{
			return;
		}*/

		if ( source_slot == 8 && target_slot != 9 )
		{
			return;
		}

		/*if ( target_slot == 9 && source_slot != 8 )
		{
			return;
		}*/
	}

	MU_ROOMY_CARD_MOVE_RESULT pMsg;
	pMsg.source_slot = source_slot;
	pMsg.target_slot = target_slot;

	uint8 source_card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(source_slot)->GetCardSlot();
	uint8 source_card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(source_slot)->GetCardType();

	uint8 target_card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(target_slot)->GetCardSlot();
	uint8 target_card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(target_slot)->GetCardType();

	if ( source_card_type == MU_ROOMY_CARD_TYPE_NONE || target_card_type != MU_ROOMY_CARD_TYPE_NONE )
	{
		pMsg.source_slot = -1;
	}
	else if ( source_card_type == MU_ROOMY_CARD_TYPE_SPECIAL && !MU_ROOMY_SPECIALCARD_SLOT_RANGE(source_card_slot) )
	{
		pMsg.source_slot = -1;
	}
	else if ( source_card_type == MU_ROOMY_CARD_TYPE_NORMAL && !MU_ROOMY_CARD_SLOT_RANGE(source_card_slot) )
	{
		pMsg.source_slot = -1;
	}
	else if ( source_card_type == MU_ROOMY_CARD_TYPE_SPECIAL && (target_slot >= 0 && target_slot <= 4) )
	{
		pMsg.source_slot = -1;
	}
	else if ( source_card_type == MU_ROOMY_CARD_TYPE_SPECIAL && (target_slot >= 5 && target_slot <= 7) )
	{
		if ( pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(source_card_slot)->GetNumber() == MU_ROOMY_SPECIAL_CARD_DUPLICATE ||
			 pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(source_card_slot)->GetNumber() == MU_ROOMY_SPECIAL_CARD_COMBINE )
		{
			pMsg.source_slot = -1;
		}
	}
	else if ( source_card_type == MU_ROOMY_CARD_TYPE_NORMAL && (target_slot >= 8) )
	{
		pMsg.source_slot = -1;
	}

	if ( pMsg.source_slot != uint8(-1) )
	{
		MuRoomyCardInfo * pMuRoomyCardInfo = source_card_type == MU_ROOMY_CARD_TYPE_SPECIAL ? pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(source_card_slot) : pPlayer->GetMuRoomyInfo()->GetCardInfo(source_card_slot);

		if ( pMuRoomyCardInfo )
		{
			pPlayer->GetMuRoomyInfo()->GetSlotInfo(source_slot)->Init();

			pPlayer->GetMuRoomyInfo()->GetSlotInfo(target_slot)->SetCardSlot(source_card_slot);
			pPlayer->GetMuRoomyInfo()->GetSlotInfo(target_slot)->SetCardType(source_card_type);

			pMuRoomyCardInfo->SetSlot(target_slot);
								
			pMsg.color = pMuRoomyCardInfo->GetColor();
			pMsg.number = pMuRoomyCardInfo->GetNumber();

			pMsg.source_slot = source_slot;
		}
	}

	pPlayer->SEND_PCT(pMsg);
}

void MuRoomy::Remove(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsActive() || !sGameServer->IsMuRoomyEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}
		
	POINTER_PCT_LOG(MU_ROOMY_CARD_REMOVE_REQUEST, lpMsg, Packet, 0);

	uint8 source_slot = lpMsg->slot;

	if ( source_slot >= MU_ROOMY_MAX_SLOT )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->GetType() )
	{
		if ( source_slot == 8 || source_slot == 9 )
		{
			return;
		}
	}
	else
	{
		if ( source_slot == 8 )
		{
			return;
		}
	}

	uint8 Ret = this->CardSlotRemove(pPlayer, source_slot);

	if ( Ret == uint8(-1) )
	{
		return;
	}

	MU_ROOMY_CARD_REMOVE_RESULT pMsg(source_slot);
	pPlayer->SEND_PCT(pMsg);

	this->SendMessage(pPlayer, 4, 0);
}

void MuRoomy::Match(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsActive() || !sGameServer->IsMuRoomyEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MU_ROOMY_MATCH_REQUEST, lpMsg, Packet, 0);

	int32 score = 0;
	
	uint8 btRet = this->CardMatchCheck(pPlayer, score, false);

	if ( btRet )
	{
		this->SendMessage(pPlayer, 6, 0);
	}
	else
	{
		pPlayer->GetMuRoomyInfo()->IncreaseScore(score);

		MU_ROOMY_MATCH_RESULT pMsg;
		pMsg.scoreH = SET_NUMBERH(score);
		pMsg.scoreL = SET_NUMBERL(score);
		pMsg.total_scoreH = SET_NUMBERH(pPlayer->GetMuRoomyInfo()->GetScore());
		pMsg.total_scoreL = SET_NUMBERL(pPlayer->GetMuRoomyInfo()->GetScore());
		pPlayer->SEND_PCT(pMsg);

		this->SendMessage(pPlayer, 5, 0);

		this->GenerateSpecialCard(pPlayer);
	}

	this->SendCardList(pPlayer);
}

void MuRoomy::End(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsActive() || !sGameServer->IsMuRoomyEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MU_ROOMY_END_REQUEST, lpMsg, Packet, 0);

	MuRoomyReward const* pMuRoomyReward = this->GetReward(pPlayer);

	if ( !pMuRoomyReward )
	{
		if ( pPlayer->MoneyReachMaximum(sGameServer->GetMuRoomyRewardMoneyAmmount()) )
		{
			this->SendMessage(pPlayer, 11, 0);
			return;
		}

		this->SendMessage(pPlayer, 10, 0);
		pPlayer->MoneyAdd(sGameServer->GetMuRoomyRewardMoneyAmmount());
	}
	else
	{
		Item item(pMuRoomyReward->GetItem(), pMuRoomyReward->GetLevel());
		item.SetExpireDate(time(nullptr) + DAY);

		sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
		this->SendMessage(pPlayer, 9, pMuRoomyReward->GetItem());
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_MU_ROOMY);
	stmt->setUInt32(0, pPlayer->GetGUID());
	stmt->setUInt8(1, pPlayer->GetMuRoomyInfo()->GetType());
	stmt->setUInt16(2, pPlayer->GetMuRoomyInfo()->GetScore());
	stmt->setUInt8(3, pPlayer->GetMuRoomyInfo()->GetCardDeckCount());
	stmt->setUInt8(4, pPlayer->GetMuRoomyInfo()->GetSpecialCardDeckCount());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	MU_ROOMY_END_RESULT pMsg(1);
	pPlayer->SEND_PCT(pMsg);

	this->SendMessage(pPlayer, 8, 0);
	pPlayer->GetMuRoomyInfo()->Init();
}

void MuRoomy::SpecialMatch(Player* pPlayer, uint8 * Packet)
{
	if ( !this->IsActive() || !sGameServer->IsMuRoomyEnabled() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MU_ROOMY_SPECIAL_MATCH_REQUEST, lpMsg, Packet, 0);

	int32 score = 0;
	
	uint8 btRet = this->CardMatchCheck(pPlayer, score, true);

	if ( btRet )
	{
		this->SendMessage(pPlayer, 6, 0);
	}
	else
	{
		pPlayer->GetMuRoomyInfo()->IncreaseScore(score);

		MU_ROOMY_MATCH_RESULT pMsg;
		pMsg.scoreH = SET_NUMBERH(score);
		pMsg.scoreL = SET_NUMBERL(score);
		pMsg.total_scoreH = SET_NUMBERH(pPlayer->GetMuRoomyInfo()->GetScore());
		pMsg.total_scoreL = SET_NUMBERL(pPlayer->GetMuRoomyInfo()->GetScore());
		pPlayer->SEND_PCT(pMsg);

		this->SendMessage(pPlayer, 5, 0);

		this->GenerateSpecialCard(pPlayer);
	}

	this->SendCardList(pPlayer);
}

void MuRoomy::SendCardList(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}
	
	MU_ROOMY_CARD_LIST pMsg;
	this->SetPlayCardInfo(pPlayer, pMsg.card_info);
	pPlayer->SEND_PCT(pMsg);
}

void MuRoomy::SendMessage(Player* pPlayer, uint8 index, uint16 value)
{
	if ( !pPlayer )
	{
		return;
	}

	MU_ROOMY_MESSAGE pMsg(index, value);
	pPlayer->SEND_PCT(pMsg);
}

void MuRoomy::SetCardDeck(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( int32 i = 0; i < MU_ROOMY_MAX_SLOT; ++i )
	{
		pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->Init();
	}

	pPlayer->GetMuRoomyInfo()->SetCardDeckCount(MU_ROOMY_MAX_CARD);
	
	for ( int32 color = 0; color < MU_ROOMY_MAX_CARD_COLOR; ++color )
	{
		for ( int32 number = 0; number < MU_ROOMY_MAX_CARD_NUMBER; ++number )
		{
			while ( true )
			{
				int32 random_slot = Random(MU_ROOMY_MAX_CARD);

				if ( MU_ROOMY_CARD_SLOT_RANGE(random_slot) && pPlayer->GetMuRoomyInfo()->GetCardInfo(random_slot)->GetColor() == 0 )
				{
					pPlayer->GetMuRoomyInfo()->GetCardInfo(random_slot)->SetColor(color + 1);
					pPlayer->GetMuRoomyInfo()->GetCardInfo(random_slot)->SetNumber(number + 1);
					break;
				}
			}
		}
	}

	if ( pPlayer->GetMuRoomyInfo()->GetType() )
	{
		pPlayer->GetMuRoomyInfo()->SetSpecialCardDeckCount(MU_ROOMY_MAX_CARD_SPECIAL);

		for ( int32 i = 0; i < MU_ROOMY_MAX_CARD_SPECIAL; ++i )
		{
			pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(i)->SetColor(MU_ROOMY_MAX_SPECIAL_CARD_COLOR);
			pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(i)->SetNumber(Random(MU_ROOMY_MAX_SPECIAL_CARD_NUMBER) + 1);
		}

		this->GenerateSpecialCard(pPlayer);
	}
}

void MuRoomy::SetPlayCardInfo(Player* pPlayer, MU_ROOMY_CARD_INFO* pCardInfo)
{
	if ( !pPlayer )
	{
		return;
	}

	int32 count = 0;

	for ( int32 i = 0; i < MU_ROOMY_MAX_SLOT; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();
		uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardSlot();

		MuRoomyCardInfo * pRoomyCardInfo = nullptr;

		switch ( card_type )
		{
		case MU_ROOMY_CARD_TYPE_NONE:
			{
			} break;

		case MU_ROOMY_CARD_TYPE_NORMAL:
			{
				if ( MU_ROOMY_CARD_SLOT_RANGE(card_slot) )
				{
					pRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetCardInfo(card_slot);
				}
			} break;

		case MU_ROOMY_CARD_TYPE_SPECIAL:
			{
				if ( MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
				{
					pRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot);
				}
			} break;
		}

		if ( !pRoomyCardInfo )
		{
			continue;
		}

		if ( pRoomyCardInfo->GetState() != MU_ROOMY_CARD_STATE_ACTIVE )
		{
			continue;
		}

		if ( pRoomyCardInfo->GetSlot() != i )
		{
			continue;
		}

		pCardInfo[count].color = pRoomyCardInfo->GetColor();
		pCardInfo[count].number = pRoomyCardInfo->GetNumber();
		pCardInfo[count].slot = pRoomyCardInfo->GetSlot();
		++count;

		if ( count >= 6 )
		{
			break;
		}
	}
}

uint8 MuRoomy::GetTempCardShowSlot(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return -1;
	}

	///- Retornar cuando hay cartas actualmente en juego
	for ( int32 i = 5; i < 8; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();

		if ( card_type != MU_ROOMY_CARD_TYPE_NONE )
		{
			return -2;
		}
	}

	int32 iSlotCnt = 0;
	///- Contar cuantos slots hay ocupados
	for ( int32 i = 0; i < 5; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();

		if ( card_type != MU_ROOMY_CARD_TYPE_NORMAL )
		{
			continue;
		}
		
		iSlotCnt++;
	}
	
	if ( iSlotCnt >= 5 )
	{
		return -1;
	}

	for ( int32 i = 0; i < 5; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();

		if ( card_type != MU_ROOMY_CARD_TYPE_NONE )
		{
			continue;
		}
		
		return i;
	}

	return -1;
}

uint8 MuRoomy::CardSlotRemove(Player* pPlayer, uint8 source_slot)
{
	if ( !pPlayer )
	{
		return -1;
	}

	uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(source_slot)->GetCardType();
	uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(source_slot)->GetCardSlot();

	MuRoomyCardInfo * pMuRoomyCardInfo = nullptr;

	switch ( card_type )
	{
	case MU_ROOMY_CARD_TYPE_NONE:
		{
		} break;

	case MU_ROOMY_CARD_TYPE_NORMAL:
		{
			if ( MU_ROOMY_CARD_SLOT_RANGE(card_slot) )
			{
				pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetCardInfo(card_slot);
			}
		} break;

	case MU_ROOMY_CARD_TYPE_SPECIAL:
		{
			if ( MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
			{
				pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot);
			}
		} break;
	}

	if ( !pMuRoomyCardInfo )
	{
		return -1;
	}

	if ( pMuRoomyCardInfo->GetState() != MU_ROOMY_CARD_STATE_ACTIVE )
	{
		return -1;
	}

	if ( pMuRoomyCardInfo->GetSlot() != source_slot )
	{
		return -1;
	}

	pMuRoomyCardInfo->SetSlot(-1);
	pMuRoomyCardInfo->SetState(MU_ROOMY_CARD_STATE_REMOVED);
	pPlayer->GetMuRoomyInfo()->GetSlotInfo(source_slot)->Init();

	if ( card_type == MU_ROOMY_CARD_TYPE_SPECIAL )
	{
		this->GenerateSpecialCard(pPlayer);
	}

	return card_slot;
}

uint8 MuRoomy::CardMatchCheck(Player* pPlayer, int32& score, bool special)
{
	if ( !pPlayer )
	{
		return -1;
	}

	///- Can't be more than 1 special card
	int32 special_count = 0;
	for ( int32 i = 0; i < MU_ROOMY_MAX_SLOT; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();

		if ( card_type == MU_ROOMY_CARD_TYPE_SPECIAL )
		{
			++special_count;
		}
	}

	if ( special_count > 1 )
	{
		return -1;
	}

	uint8 special_card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(9)->GetCardType();
	uint8 special_card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(9)->GetCardSlot();
	uint8 special_card = 0;
	uint8 special_card_number = 0;

	if ( special && special_card_type == MU_ROOMY_CARD_TYPE_SPECIAL && MU_ROOMY_SPECIALCARD_SLOT_RANGE(special_card_slot) )
	{
		special_card = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot)->GetNumber();
	}
	
	if ( special )
	{
		if ( special_card_type != MU_ROOMY_CARD_TYPE_SPECIAL )
		{
			return -1;
		}
	}
	else
	{
		if ( special_card_type == MU_ROOMY_CARD_TYPE_SPECIAL )
		{
			return -1;
		}
	}

	MuRoomyCardInfo CCardInfoTmp[4];

	int32 count = 0;
	
	for ( int32 i = 5; i < 8; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();
		uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardSlot();

		MuRoomyCardInfo * pMuRoomyCardInfo = nullptr;

		switch ( card_type )
		{
		case MU_ROOMY_CARD_TYPE_NONE:
			{
			} break;

		case MU_ROOMY_CARD_TYPE_NORMAL:
			{
				if ( MU_ROOMY_CARD_SLOT_RANGE(card_slot) )
				{
					pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetCardInfo(card_slot);
				}
			} break;

		case MU_ROOMY_CARD_TYPE_SPECIAL:
			{
				if ( MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
				{
					pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot);
				}
			} break;
		}

		if ( !pMuRoomyCardInfo )
		{
			continue;
		}

		if ( pMuRoomyCardInfo->GetState() != MU_ROOMY_CARD_STATE_ACTIVE )
		{
			continue;
		}

		if ( pMuRoomyCardInfo->GetSlot() != i )
		{
			continue;
		}

		if ( pMuRoomyCardInfo->GetNumber() == MU_ROOMY_SPECIAL_CARD_DUPLICATE ||
			 pMuRoomyCardInfo->GetNumber() == MU_ROOMY_SPECIAL_CARD_COMBINE )
		{
			continue;
		}

		CCardInfoTmp[count].SetColor(pMuRoomyCardInfo->GetColor());
		CCardInfoTmp[count].SetNumber(pMuRoomyCardInfo->GetNumber());
		CCardInfoTmp[count].SetState(pMuRoomyCardInfo->GetState());
		CCardInfoTmp[count].SetSlot(pMuRoomyCardInfo->GetSlot());

		if ( card_type == MU_ROOMY_CARD_TYPE_SPECIAL )
		{
			special_card_number = pMuRoomyCardInfo->GetNumber();
		}

		++count;
	}

	if ( special && special_card_type == MU_ROOMY_CARD_TYPE_SPECIAL && MU_ROOMY_SPECIALCARD_SLOT_RANGE(special_card_slot) )
	{
		MuRoomyCardInfo * pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot);

		if ( pMuRoomyCardInfo->GetState() == MU_ROOMY_CARD_STATE_ACTIVE )
		{
			CCardInfoTmp[3].SetColor(pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot)->GetColor());
			CCardInfoTmp[3].SetNumber(pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot)->GetNumber());
			CCardInfoTmp[3].SetState(pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot)->GetState());
			CCardInfoTmp[3].SetSlot(pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot)->GetSlot());
			++count;
		}
	}

	///- Must be 3 cards to normal match & 4 cards to special match
	if ( (!special && count != 3) || (special && count != 4) )
	{
		return -1;
	}

	if ( special && (special_card == MU_ROOMY_SPECIAL_CARD_DUPLICATE || special_card == MU_ROOMY_SPECIAL_CARD_COMBINE) )
	{
		CCardInfoTmp[3].Init();
		--count;
	}
	
	int32 nRetCardColor = this->IsCardSameColor(CCardInfoTmp, count);
	int32 nRetCardNum = this->IsCardNumber(CCardInfoTmp, count);

	if ( special && special_card == MU_ROOMY_SPECIAL_CARD_COMBINE && nRetCardColor == 1 && nRetCardNum == 3 )
	{
		nRetCardColor = 2;
	}

	bool match = false;

	if (nRetCardColor == 1 && nRetCardNum == 3) ///- Diff Color, Diff Numbers
	{
		score = 10 * (CCardInfoTmp[0].GetNumber() - 1) + 10;

		if ( special_card_number > 0 && special_card_number <= 9 )
		{
			score += 10;
		}
		if ( special && (special_card > 0 && special_card <= 9) )
		{
			score += 20;
		}

		match = true;
	}

	if (nRetCardColor == 2 && nRetCardNum == 3) ///- Same Color, Diff Numbers
	{
		score = 10 * (CCardInfoTmp[0].GetNumber() - 1) + 50;
		match = true;
	}

	if (nRetCardColor == 1 && nRetCardNum == 4) ///- Diff Color, Same Numbers
	{
		score = 10 * (CCardInfoTmp[0].GetNumber() - 1) + 20;
		match = true;
	}

	if ( special && special_card == MU_ROOMY_SPECIAL_CARD_DUPLICATE )
	{
		score *= 2;
	}

	///- If Match, remove cards
	if ( match )
	{
		for ( int32 i = 5; i < 8; ++i )
		{
			uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();
			uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardSlot();

			MuRoomyCardInfo * pMuRoomyCardInfo = nullptr;

			switch ( card_type )
			{
			case MU_ROOMY_CARD_TYPE_NONE:
				{
				} break;

			case MU_ROOMY_CARD_TYPE_NORMAL:
				{
					if ( MU_ROOMY_CARD_SLOT_RANGE(card_slot) )
					{
						pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetCardInfo(card_slot);
					}
				} break;

			case MU_ROOMY_CARD_TYPE_SPECIAL:
				{
					if ( MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
					{
						pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot);
					}
				} break;
			}

			if ( !pMuRoomyCardInfo )
			{
				continue;
			}

			if ( pMuRoomyCardInfo->GetState() != MU_ROOMY_CARD_STATE_ACTIVE )
			{
				continue;
			}

			if ( pMuRoomyCardInfo->GetSlot() != i )
			{
				continue;
			}

			pMuRoomyCardInfo->SetSlot(-1);
			pMuRoomyCardInfo->SetState(MU_ROOMY_CARD_STATE_MATCHED);
			pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->Init();
		}

		if ( special && special_card_type == MU_ROOMY_CARD_TYPE_SPECIAL && MU_ROOMY_SPECIALCARD_SLOT_RANGE(special_card_slot) )
		{
			pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot)->SetSlot(-1);
			pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(special_card_slot)->SetState(MU_ROOMY_CARD_STATE_MATCHED);
			pPlayer->GetMuRoomyInfo()->GetSlotInfo(9)->Init();
		}
		
		return 0;
	}
	///- If doesn't Match return cards
	int32 nSlotNum = 5;

	for ( int32 i = 5; i < 8; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();
		uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardSlot();

		switch ( card_type )
		{
		case MU_ROOMY_CARD_TYPE_NONE:
			{
			} break;

		case MU_ROOMY_CARD_TYPE_NORMAL:
			{
				if ( MU_ROOMY_CARD_SLOT_RANGE(card_slot) )
				{
					MuRoomyCardInfo * pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetCardInfo(card_slot);

					if ( pMuRoomyCardInfo && pMuRoomyCardInfo->GetState() == MU_ROOMY_CARD_STATE_ACTIVE && pMuRoomyCardInfo->GetSlot() == i )
					{
						for ( int32 n = 0; n < 5; ++n )
						{
							uint8 tmp_card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(n)->GetCardType();

							if ( tmp_card_type == MU_ROOMY_CARD_TYPE_NONE )
							{
								pPlayer->GetMuRoomyInfo()->GetSlotInfo(n)->SetCardSlot(card_slot);
								pPlayer->GetMuRoomyInfo()->GetSlotInfo(n)->SetCardType(card_type);
								pMuRoomyCardInfo->SetSlot(n);

								pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->Init();
								break;
							}
						}
					}
				}
			} break;

		case MU_ROOMY_CARD_TYPE_SPECIAL:
			{
				if ( MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
				{
					MuRoomyCardInfo * pMuRoomyCardInfo = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot);

					if ( pMuRoomyCardInfo && pMuRoomyCardInfo->GetState() == MU_ROOMY_CARD_STATE_ACTIVE && pMuRoomyCardInfo->GetSlot() == i )
					{
						uint8 tmp_card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(9)->GetCardType();

						if ( tmp_card_type == MU_ROOMY_CARD_TYPE_NONE )
						{
							pPlayer->GetMuRoomyInfo()->GetSlotInfo(9)->SetCardSlot(card_slot);
							pPlayer->GetMuRoomyInfo()->GetSlotInfo(9)->SetCardType(card_type);
							pMuRoomyCardInfo->SetSlot(9);

							pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->Init();
						}
					}
				}
			} break;
		}
	}

	score = 0;
	return -1;
}

void MuRoomy::CardSortAscending(MuRoomyCardInfo * pMuRoomyCardInfo, int32 count)
{
	MuRoomyCardInfo CCardInfoTmp;

	for ( int32 i = 0; i < (count - 1); ++i )
	{
		for ( int32 j = 0; j < count - i - 1; ++j )
		{
			if ( pMuRoomyCardInfo[j].GetNumber() > pMuRoomyCardInfo[j + 1].GetNumber() )
			{
				CCardInfoTmp.SetColor(pMuRoomyCardInfo[j].GetColor());
				CCardInfoTmp.SetNumber(pMuRoomyCardInfo[j].GetNumber());
				CCardInfoTmp.SetState(pMuRoomyCardInfo[j].GetState());
				CCardInfoTmp.SetSlot(pMuRoomyCardInfo[j].GetSlot());

				pMuRoomyCardInfo[j].SetColor(pMuRoomyCardInfo[j + 1].GetColor());
				pMuRoomyCardInfo[j].SetNumber(pMuRoomyCardInfo[j + 1].GetNumber());
				pMuRoomyCardInfo[j].SetState(pMuRoomyCardInfo[j + 1].GetState());
				pMuRoomyCardInfo[j].SetSlot(pMuRoomyCardInfo[j + 1].GetSlot());

				pMuRoomyCardInfo[j + 1].SetColor(CCardInfoTmp.GetColor());
				pMuRoomyCardInfo[j + 1].SetNumber(CCardInfoTmp.GetNumber());
				pMuRoomyCardInfo[j + 1].SetState(CCardInfoTmp.GetState());
				pMuRoomyCardInfo[j + 1].SetSlot(CCardInfoTmp.GetSlot());
			}
		}
	}
}

int MuRoomy::IsCardSameColor(MuRoomyCardInfo * pMuRoomyCardInfo, int32 count)
{
	for ( int32 i = 0; i < (count - 1); ++i )
	{
		//MU_ROOMY_MAX_SPECIAL_CARD_COLOR

		if ( pMuRoomyCardInfo[i].GetColor() != pMuRoomyCardInfo[i + 1].GetColor() )
		{
			return 1;
		}
	}

	return 2;
}

int MuRoomy::IsCardNumber(MuRoomyCardInfo * pMuRoomyCardInfo, int32 count)
{
	for ( int32 i = 0; ; ++i )
	{
		if ( i >= (count - 1) )
		{
			return 4;
		}

		if ( pMuRoomyCardInfo[i].GetNumber() != pMuRoomyCardInfo[i + 1].GetNumber() )
		{
			break;
		}
	}

	this->CardSortAscending(pMuRoomyCardInfo, count);

	for ( int32 j = 0; j < (count - 1); ++j )
	{
		if ( (pMuRoomyCardInfo[j + 1].GetNumber() - pMuRoomyCardInfo[j].GetNumber()) != 1 )
		{
			return 0;
		}
	}

	return 3;
}

void MuRoomy::FillEmptySlotCard(Player* pPlayer, MU_ROOMY_CARD_INFO* CardInfo)
{
	if ( !pPlayer )
	{
		return;
	}

	int32 count = 0;
	///- Hay que usar cartas hasta completar 5 slots
	for ( int32 i = 0; i < 5; ++i )
	{
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();
		uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardSlot();

		if ( card_type != MU_ROOMY_CARD_TYPE_NONE )
		{
			continue;
		}

		uint8 current_slot = pPlayer->GetMuRoomyInfo()->GetCardDeckCount() - 1;
		
		pPlayer->GetMuRoomyInfo()->GetCardInfo(current_slot)->SetSlot(i);
		pPlayer->GetMuRoomyInfo()->GetCardInfo(current_slot)->SetState(MU_ROOMY_CARD_STATE_ACTIVE);

		pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->SetCardSlot(current_slot);
		pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->SetCardType(MU_ROOMY_CARD_TYPE_NORMAL);
		
		CardInfo[count].color = pPlayer->GetMuRoomyInfo()->GetCardInfo(current_slot)->GetColor();
		CardInfo[count].number = pPlayer->GetMuRoomyInfo()->GetCardInfo(current_slot)->GetNumber();
		CardInfo[count].slot = pPlayer->GetMuRoomyInfo()->GetCardInfo(current_slot)->GetSlot();

		pPlayer->GetMuRoomyInfo()->ReduceCardDeckCount(1);

		++count;

		if ( !pPlayer->GetMuRoomyInfo()->GetCardDeckCount() )
		{
			break;
		}
	}
}

void MuRoomy::GenerateSpecialCard(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->GetType() )
	{
		return;
	}

	if ( !pPlayer->GetMuRoomyInfo()->GetSpecialCardDeckCount() )
	{
		return;
	}
	///- Don't generate special card if already exists
	for ( int32 i = 0; i < MU_ROOMY_MAX_SLOT; ++i )
	{
		uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardSlot();
		uint8 card_type = pPlayer->GetMuRoomyInfo()->GetSlotInfo(i)->GetCardType();

		if ( card_type == MU_ROOMY_CARD_TYPE_SPECIAL && MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
		{
			if ( pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->GetState() == MU_ROOMY_CARD_STATE_ACTIVE )
			{
				return;
			}
		}
	}

	uint8 card_slot = pPlayer->GetMuRoomyInfo()->GetSpecialCardDeckCount() - 1;

	pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetSlot(8);
	pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetState(MU_ROOMY_CARD_STATE_ACTIVE);

	pPlayer->GetMuRoomyInfo()->GetSlotInfo(8)->SetCardSlot(card_slot);
	pPlayer->GetMuRoomyInfo()->GetSlotInfo(8)->SetCardType(MU_ROOMY_CARD_TYPE_SPECIAL);

	pPlayer->GetMuRoomyInfo()->ReduceSpecialCardDeckCount(1);

	MU_ROOMY_REVEAL_CARD_RESULT pMsg;
	pMsg.card_info[5].slot = 8;
	pMsg.card_info[5].color = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->GetColor();
	pMsg.card_info[5].number = pPlayer->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->GetNumber();
	pMsg.card_count = pPlayer->GetMuRoomyInfo()->GetCardDeckCount();
	pMsg.special_card_count = pPlayer->GetMuRoomyInfo()->GetSpecialCardDeckCount();
	pPlayer->SEND_PCT(pMsg);
}

MuRoomyReward const* MuRoomy::GetReward(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return nullptr;
	}

	for ( MuRoomyRewardList::const_iterator itr = this->reward_list.begin(); itr != this->reward_list.end(); ++itr )
	{
		MuRoomyReward const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetType() != pPlayer->GetMuRoomyInfo()->GetType() )
		{
			continue;
		}

		if ( pPlayer->GetMuRoomyInfo()->GetScore() >= pData->GetScoreMin() && pPlayer->GetMuRoomyInfo()->GetScore() <= pData->GetScoreMax() )
		{
			return pData;
		}
	}

	return nullptr;
}