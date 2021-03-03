/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Player.cpp"
*
*/

Player::PlayerSaveTransaction::PlayerSaveTransaction(Player* pPlayer, uint8 operation): TransactionCallBack()
{
	this->SetPlayer(pPlayer);
	this->SetOperation(operation);
}

Player::PlayerSaveTransaction::~PlayerSaveTransaction()
{

}

void Player::PlayerSaveTransaction::Finished()
{
	if ( Player* pPlayer = this->GetPlayer() )
	{
		switch ( this->GetOperation() )
		{
		case 0: // Regular Save
			{
			} break;

		case 1: // Server Move Save
			{
				if ( pPlayer->IsServerQuit() && pPlayer->IsPlaying() )
				{
					pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Character saved. Moving to server.");

					SERVER_CHANGE_REQUEST pMsg;
					pMsg.server = pPlayer->GetDestServer() / MAX_SERVER_PER_GROUP; // pMsg.wMapSvrCode = g_MapServerManager.GetMapSvrGroup();
					pMsg.port = pPlayer->GetDestPort();
					memcpy(pMsg.ip, pPlayer->GetDestIP(), MAX_ACCOUNT_IP_LENGTH);
					pMsg.auth[0] = pPlayer->GetDestAuth(0);
					pMsg.auth[1] = pPlayer->GetDestAuth(1);
					pMsg.auth[2] = pPlayer->GetDestAuth(2);
					pMsg.auth[3] = pPlayer->GetDestAuth(3);

					pPlayer->SEND_PCT(pMsg);

					if ( pPlayer->GetPartyID() != PARTY_NULL )
					{
						pPlayer->PartyDelete();
					}

					sLog->outInfo(LOG_PLAYER, "[ SERVER MOVE SUCCESS ] %s --- Server: %d [%d - %d/%d]", pPlayer->BuildLog().c_str(),
						pPlayer->GetDestServer(), pPlayer->GetDestWorld(), pPlayer->GetDestX(), pPlayer->GetDestY());
				}
			} break;

		case 2: // Character Logout
			{
				if ( pPlayer->GetAccountData()->GetCloseAuthorization() == 1 && pPlayer->GetAccountData()->GetCloseType() == CLOSE_FLAG_SELECT_CHAR )
				{
					pPlayer->GetAccountData()->SetCloseAuthorization(2);
				}
			} break;

		case 3: // Personal Store Off
			{
				if ( pPlayer->GetPersonalStore()->IsEnding() )
				{
					pPlayer->GetPersonalStore()->Clear();
				}
			} break;
		}
	}
}

void Player::PlayerSaveTransaction::Cancelled()
{
	if ( Player* pPlayer = this->GetPlayer() )
	{
		switch ( this->GetOperation() )
		{
		case 0: // Regular Save
			{
			} break;

		case 1: // Server Move Save
			{
				if ( pPlayer->IsServerQuit() && pPlayer->IsPlaying() )
				{
					pPlayer->SetServerQuit(false);
				}

				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Character not saved. Failed to move server.");
			} break;

		case 2: // Character Logout
			{
				pPlayer->LogoutResult(-1);
				pPlayer->Close(0, CLOSE_FLAG_NONE);
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Error while saving character. Contact administrators.");
			} break;

		case 3: // Personal Stoer Off
			{

			} break;
		}
	}
}

void Player::ProcessQueryCallbacks()
{
	QueryCallBackProcess(_friendAddCallBack, FriendAddCallBack);
	QueryCallBackProcess(_mailAddCallBack, FriendMailRequestCallBack);
	QueryCallBackProcess(_mailUpdateCallBack, MailNewUpdateCallBack);
	QueryCallBackProcess(_genRankingCallBack, LoadDBGen);
	QueryCallBackProcess(CallBackCashShopCoin, CashShopCoinCallBack);
	QueryCallBackProcess(CallBackCashShopGift, CashShopGiftCallBack);
	QueryCallBackProcess(CallBackCashShopGiftSend, CashShopGiftSendCallBack);
	QueryCallBackProcess(_charListCallBack, CharacterListRequestCallBack);
	QueryCallBackProcess(_charSelectCallBack_1, CharacterSelectCallBack);
	QueryCallBackProcess(_charSelectCallBack_2, CharacterLoginHandle);
}

class CharacterListQueryHolder: public SQLQueryHolder
{
    private:
        uint32 account_id;
    public:
        CharacterListQueryHolder(uint32 account_id)
            : account_id(account_id) { }
        uint32 GetAccountID() const { return this->account_id; }
        bool Initialize();
};

bool CharacterListQueryHolder::Initialize()
{
	this->SetSize(CHARACTER_LIST_QUERY_MAX);

	bool res = true;

	auto stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LIST_ACCOUNT_DATA);
	stmt->setUInt32(0, this->GetAccountID());
	res &= SetPreparedQuery(CHARACTER_LIST_QUERY_ACCOUNT_DATA, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LIST_CHARACTER);
	stmt->setUInt32(0, this->GetAccountID());
	stmt->setUInt8(1, MAX_CHARACTER_PER_ACCOUNT);
	res &= SetPreparedQuery(CHARACTER_LIST_QUERY_CHARACTER_DATA, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LIST_CHARACTER_ITEM);
	stmt->setUInt32(0, this->GetAccountID());
	stmt->setUInt8(1, ITEM_BOX_INVENTORY);
	stmt->setUInt8(2, PLAYER_MAX_EQUIPMENT);
	res &= SetPreparedQuery(CHARACTER_LIST_QUERY_CHARACTER_ITEM_DATA, stmt);

	return res;
}

class LoginQueryHolder: public SQLQueryHolder
{
    private:
        uint32 m_accountId;
        uint32 guid;
    public:
        LoginQueryHolder(uint32 accountId, uint32 _guid)
            : m_accountId(accountId), guid(_guid) { }
        uint32 GetGUID() const { return this->guid; }
        uint32 GetAccountId() const { return this->m_accountId; }
        bool Initialize();
};

#define EASY_HOLDER(statement, query, holder) \
	stmt = MuDatabase.GetPreparedStatement(query); \
	stmt->setUInt32(0, this->GetGUID()); \
	res &= SetPreparedQuery(holder, stmt);

bool LoginQueryHolder::Initialize()
{
	this->SetSize(MAX_PLAYER_LOGIN_QUERY);

	bool res = true;

	auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_SELECT_INFO);
	stmt->setUInt32(0, this->GetAccountId());
	stmt->setUInt32(1, this->GetGUID());
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_INVENTORY);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ITEM, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_MUUN);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_MUUN, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_EVENT_INVENTORY);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_EVENT_INVENTORY, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, PENTAGRAM_JEWEL_TYPE_INVENTORY);
	stmt->setUInt32(2, this->GetAccountId());
	stmt->setUInt8(3, PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_PENTAGRAM, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_GREMORY_CASE_INV);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_CHARACTER, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetAccountId());
	stmt->setUInt8(1, ITEM_BOX_GREMORY_CASE_ACC);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_ACCOUNT, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_GREMORY_CASE_MOBILE);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_MOBILE, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_GREMORY_CASE_PS);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_PERSONAL_STORE, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_PERSONAL_STORE);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_PERSONAL_STORE, stmt);

	EASY_HOLDER(stmt, CHARACTER_SELECT_SKILL, PLAYER_LOGIN_QUERY_LOAD_SKILL);
	EASY_HOLDER(stmt, CHARACTER_SELECT_GAME_OPTION, PLAYER_LOGIN_QUERY_LOAD_GAME_OPTION);
	EASY_HOLDER(stmt, CHARACTER_SELECT_QUEST_EVOLUTION, PLAYER_LOGIN_QUERY_LOAD_QUEST_EVO);
	EASY_HOLDER(stmt, CHARACTER_SELECT_FRIEND, PLAYER_LOGIN_QUERY_LOAD_FRIEND_LIST);
	EASY_HOLDER(stmt, CHARACTER_SELECT_MAIL, PLAYER_LOGIN_QUERY_LOAD_MAIL_LIST);
	EASY_HOLDER(stmt, CHARACTER_SELECT_GUILD, PLAYER_LOGIN_QUERY_LOAD_GUILD);
	EASY_HOLDER(stmt, CHARACTER_SELECT_GEN, PLAYER_LOGIN_QUERY_LOAD_GEN);
	EASY_HOLDER(stmt, CHARACTER_SELECT_GEN_KILL, PLAYER_LOGIN_QUERY_LOAD_GEN_KILL);
	EASY_HOLDER(stmt, CHARACTER_SELECT_HELPER, PLAYER_LOGIN_QUERY_LOAD_HELPER);
	EASY_HOLDER(stmt, CHARACTER_SELECT_BUFF, PLAYER_LOGIN_QUERY_LOAD_BUFF);
	EASY_HOLDER(stmt, CHARACTER_SELECT_RESTRICTION, PLAYER_LOGIN_QUERY_LOAD_RESTRICTION);
	EASY_HOLDER(stmt, CHARACTER_SELECT_STATISTICS, PLAYER_LOGIN_QUERY_LOAD_STATISTICS);
	EASY_HOLDER(stmt, CHARACTER_SELECT_ITEM_DELAY, PLAYER_LOGIN_QUERY_LOAD_ITEM_DELAY);
	EASY_HOLDER(stmt, CHARACTER_SELECT_CHAT_BLOCK, PLAYER_LOGIN_QUERY_LOAD_CHAT_BLOCK);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_ITEM_RECOVERY_SELECT, PLAYER_LOGIN_QUERY_LOAD_ITEM_RECOVERY);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_SELECT, PLAYER_LOGIN_QUERY_LOAD_HUNTING_RECORD);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_LABYRINTH_SELECT, PLAYER_LOGIN_QUERY_LOAD_LABYRINTH);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_SELECT, PLAYER_LOGIN_QUERY_LOAD_LABYRINTH_ZONE);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_SELECT, PLAYER_LOGIN_QUERY_LOAD_LABYRINTH_LEVEL);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_SELECT, PLAYER_LOGIN_QUERY_LOAD_EVENT_COUNT);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_MINI_BOMB_SELECT, PLAYER_LOGIN_QUERY_LOAD_MINI_BOMB);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_SELECT, PLAYER_LOGIN_QUERY_LOAD_MINI_BOMB_GRID);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_MU_ROOMY_SELECT, PLAYER_LOGIN_QUERY_LOAD_MU_ROOMY);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_SELECT, PLAYER_LOGIN_QUERY_LOAD_MU_ROOMY_DECK);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_SELECT, PLAYER_LOGIN_QUERY_LOAD_WARP_FAVORITE_LIST);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_SELECT, PLAYER_LOGIN_QUERY_LOAD_GUIDED_QUEST);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_SELECT, PLAYER_LOGIN_QUERY_LOAD_JEWEL_BINGO);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_SELECT, PLAYER_LOGIN_QUERY_LOAD_JEWEL_BINGO_GRID);

	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_DATE_SELECT, PLAYER_LOGIN_QUERY_LOAD_DATE);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_SELECT, PLAYER_LOGIN_QUERY_LOAD_MAJESTIC_TREE);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_SELECT, PLAYER_LOGIN_QUERY_LOAD_MAJESTIC_STATS);

	EASY_HOLDER(stmt, QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_SELECT, PLAYER_LOGIN_QUERY_LOAD_NUMERIC_BASEBALL);

	EASY_HOLDER(stmt, QUERY_MUDATABASE_QUEST_MU_SELECT, PLAYER_LOGIN_QUERY_LOAD_QUEST_MU);
	EASY_HOLDER(stmt, QUERY_MUDATABASE_MONSTER_SOUL_SELECT, PLAYER_LOGIN_QUERY_LOAD_MONSTER_SOUL);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_SELL_NPC);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ITEM_SELL, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, ITEM_BOX_MIX_RECOVERY);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_RECOVERY_MIX, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_SELECT_BUFF);
	stmt->setUInt32(0, this->GetAccountId());
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_BUFF, stmt);

	stmt = MuDatabase.GetPreparedStatement(ACCOUNT_WAREHOUSE_DATA_SELECT);
	stmt->setUInt32(0, this->GetAccountId());
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_WAREHOUSE_DATA, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_ITEM_DATA_SELECT);
	stmt->setUInt32(0, this->GetAccountId());
	stmt->setUInt8(1, ITEM_BOX_WAREHOUSE);
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_WAREHOUSE_ITEMS, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_REQUEST);
	stmt->setUInt32(0, this->GetAccountId());
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_CASH_SHOP_LIST, stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_COIN_REQUEST);
	stmt->setUInt32(0, this->GetAccountId());
	res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_CREDITS, stmt);

	return res;
}

Player::Player(uint16 entry): Unit(entry)
{
	this->m_Socket = nullptr;

	this->SetInventory(new InventoryScript(this));
	this->SetTransactionInventory(new InventoryScript(this));
	this->SetWarehouse(new WarehouseScript(this));
	this->SetTradeInventory(new TradeScript(this));
	this->SetMixInventory(new StoreScript(this, chaos_box_size, chaos_box_size));
	this->SetMixRecoveryInventory(new StoreScript(this, max_shop_item, max_shop_item, ITEM_BOX_MIX_RECOVERY));
	this->SetType(OBJECT_TYPE_PLAYER);
	this->SetSendType(MAIN_OBJECT_PLAYER);
	this->SetMuunInventory(new MuunScript(this));
	this->SetEventInventory(new EventInventoryScript(this));
	this->SetTransactionEventInventory(new EventInventoryScript(this));
	this->SetGremoryCase(GREMORY_CASE_TYPE_CHARACTER, new GremoryCaseScript(this, ITEM_BOX_GREMORY_CASE_INV, GREMORY_CASE_INVENTORY_SIZE));
	this->SetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT, new GremoryCaseScript(this, ITEM_BOX_GREMORY_CASE_ACC, GREMORY_CASE_INVENTORY_SIZE));
	this->SetGremoryCase(GREMORY_CASE_TYPE_MOBILE, new GremoryCaseScript(this, ITEM_BOX_GREMORY_CASE_MOBILE, GREMORY_CASE_INVENTORY_SIZE));
	this->SetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE, new GremoryCaseScript(this, ITEM_BOX_GREMORY_CASE_PS, GREMORY_CASE_PERSONAL_STORE_SIZE));
	this->SetNpcSell(new NpcSellScript(this));
	this->SetPersonalStore(new PersonalStoreScript(this, ITEM_BOX_PERSONAL_STORE, PERSONAL_STORE_SELL_MAX + (PERSONAL_STORE_SELL_MAX * PERSONAL_STORE_BUNDLE_MAX)));

	this->SetTimeout(0);

	this->ClearAccount();
	this->ClearCharacter();
}

Player::~Player()
{
	this->ClearAccount();
	this->ClearCharacter();
	delete this->GetInventory();
	delete this->GetTransactionInventory();
	delete this->GetWarehouse();
	delete this->GetTradeInventory();
	delete this->GetMixInventory();
	delete this->GetMixRecoveryInventory();
	this->EraseMuunInventory();
	this->EraseEventInventory();
	this->EraseTransactionEventInventory();
	this->EraseGremoryCase();
	this->EraseNpcSell();
	delete this->GetPersonalStore();

	WorldPacket* packet;
	while(!_recvPacket.empty() && _recvPacket.next(packet))
	{
		if ( packet )
			delete packet;
	}
}

void Player::ClearAccount()
{
	this->CloseSocket();

	this->GetAccountData()->SetGUID(0);
	this->GetAccountData()->ResetAccount();
	this->GetAccountData()->SetLoginAttempts(0);
	this->GetAccountData()->ResetIP();
	this->GetAccountData()->ResetMac();

	this->Close(0, CLOSE_FLAG_NONE);
	this->GetAccountData()->SetCloseGame(false);

	memset(this->GetAccountData()->secure_code, 0, MAX_SECURE_CODE_LENGTH + 1);
	this->GetAccountData()->SetSpecialCharacter(0);

	this->GetWarehouse()->Clear();

	this->GetAccountData()->SetVIPStatus(-1);
	this->GetAccountData()->SetVIPDuration(0);

	this->ResetAccountSafe();
	this->ResetPasswordSafe();

	this->ResetCharacterHandleData();
	
	this->SetCredits(0);

	this->SetWrongSerialCount(0);
	this->SetWrongDecryptCount(0);

	this->SetDestServer(-1);
	this->SetDestWorld(-1);
	this->SetDestX(0);
	this->SetDestY(0);
	this->SetDestPort(-1);
	this->ResetDestIP();
	this->ResetDestAuth(0);
	this->SetServerQuit(false);
	this->SetLoginSent(false);
	this->SetServerMove(false);
	this->SetServerMoveFromCharacterSelect(false);

	this->GetAccountData()->SetFacebookStatus(0);

	this->ResetCloseSocketTime(GetTickCount());

	this->GetTimer(PLAYER_TIMER_PACKET)->Start();
	this->GetTimer(PLAYER_TIMER_WRONG_SERIAL)->Start();
	this->GetTimer(PLAYER_TIMER_WRONG_DECRYPT)->Start();

	this->GetAccountData()->ResetGameServerSerial();

	this->SetMSTime(0);

	this->SetStartServer(-1);

	this->GetAccountData()->SetGoldenChannel(0);
	this->GetAccountData()->SetDiskSerial(0);

	//this->SetWhiteList(false);

	this->SetCharacterListRequested(false);

	this->SetAuthorizationEnabled(false);

	this->SetAutoLogin(false);
	this->SetAutoLoginAccount(0);
	this->SetAutoLoginCharacter(0);
	this->SetAutoLoginType(0);
	this->SetAutoLoginSent(false);
}

uint8 Player::GetCharacterEmptySlot()
{
	for ( uint8 i = 0; i < MAX_CHARACTER_PER_ACCOUNT; ++i )
	{
		if ( strlen(this->GetCharacterList(i)->GetName()) <= 0 )
		{
			return i;
		}
	}

	return -1;
}

uint8 Player::GetCharacterListIndex(const char * name)
{
	for ( uint8 i = 0; i < MAX_CHARACTER_PER_ACCOUNT; ++i )
	{
		if ( !strcmp(this->GetCharacterList(i)->GetName(), name) )
		{
			return i;
		}
	}

	return -1;
}

void Player::ResetCharacterHandleData()
{
	this->GetAccountData()->SetCharacterHandleAction(CHARACTER_HANDLE_ACTION_NONE);
	this->GetAccountData()->SetCurrentCharacterIndex(-1);
}

bool Player::IsPlaying() const
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		return false;

	if ( this->GetAccountData()->GetCloseType() != CLOSE_FLAG_NONE )
		return false;

	return true;
}

void Player::GameClose(bool close)
{
	this->InitCloseSocketTime(CLOSE_SOCKET_TIME_GAME_CLOSE);
	this->GetAccountData()->SetCloseGame(close);

	if ( close )
		this->LogoutEventRemove();
}

void Player::Close(int8 count, CloseType type)
{
	this->GetAccountData()->SetCloseCount(count);
	this->GetAccountData()->SetCloseType(type);
	this->GetAccountData()->SetCloseAuthorization(0);

	if ( type == CLOSE_FLAG_SELECT_CHAR )
	{
		this->GetAccountData()->SetCloseAuthorization(1);
		this->SaveCharacter(2);
	}
}

void Player::SaveDBAccount(SQLTransaction& trans)
{
	if (!this->IsAccountDataLoaded())
	{
		return;
	}

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DATA_UPDATE);
	stmt->setInt32(0, this->GetAccountData()->GetVIPStatus());
	stmt->setInt64(1, this->GetAccountData()->GetVIPDuration());
	stmt->setUInt8(2, this->GetWarehouse()->GetExpanded());
	stmt->setInt64(3, this->GetWarehouse()->GetExpandedTime());
	stmt->setUInt16(4, this->GetAccountData()->GetSpecialCharacter());
	stmt->setUInt32(5, this->GetCredits());
	stmt->setUInt32(6, this->GetAccountData()->GetGUID());
	trans->Append(stmt);
}

void Player::LoginBoxSend()
{
	JOIN_ACCOUNT_SEND pMsg(this->GetEntry(), 1, sGameServer->GetGameServerVersions());
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::LoginRequest(uint8 * Packet, bool autologin)
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_CONNECTED )
	{
		sLog->outError(LOG_PLAYER, "[ LOGIN REQUEST ] %s Wrong Connect Status", this->BuildLog().c_str());
		this->LoginResult(LOGIN_RESULT_ERROR);
		this->SetAutoLogin(false);
		this->CloseSocket();
		return;
	}

	if ( this->IsLoginSent() )
	{
		return;
	}

	if ( !autologin )
	{
		POINTER_PCT_LOG(LOGIN_REQUEST, lpMsg, Packet, 0);

		if (sGameServer->IsLoginDiskSerialMacCheck())
		{
			if (lpMsg->disk_serial == 0 || !memcmp(lpMsg->mac, "00:00:00:00:00:00", 17))
			{
				sLog->outError(LOG_PLAYER, "[ LOGIN REQUEST ] %s Wrong DiskSerial [%u] / MAC [%s]", this->BuildLog().c_str(), lpMsg->disk_serial, lpMsg->mac);

				this->LoginResult(LOGIN_RESULT_WRONG_SERIAL);
				this->CloseSocket();
				return;
			}
		}

		this->GetAccountData()->IncreaseLoginAttempts(1);
		this->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);

		if ( memcmp(lpMsg->client_version, sGameServer->GetGameServerVersions(), 5) )
		{
			sLog->outError(LOG_PLAYER, "[ LOGIN REQUEST ] %s Wrong Version [%d/%d/%d/%d/%d] / [%d/%d/%d/%d/%d]", this->BuildLog().c_str(),
				lpMsg->client_version[0], lpMsg->client_version[1], lpMsg->client_version[2], lpMsg->client_version[3], lpMsg->client_version[4],
				sGameServer->GetGameServerVersion(0), sGameServer->GetGameServerVersion(1), sGameServer->GetGameServerVersion(2), 
				sGameServer->GetGameServerVersion(3), sGameServer->GetGameServerVersion(4));

			this->LoginResult(LOGIN_RESULT_WRONG_SERIAL);
			this->CloseSocket();
			return;
		}

		STRING_SAFE_COPY(client_serial_tmp, 17, lpMsg->client_serial, 16);
	
		if ( memcmp(client_serial_tmp, sGameServer->GetGameServerSerial(), 16) )
		{
			sLog->outError(LOG_PLAYER, "[ LOGIN REQUEST ] %s Wrong Serial [%s / %s]", this->BuildLog().c_str(),
				 client_serial_tmp, sGameServer->GetGameServerSerial());
			this->LoginResult(LOGIN_RESULT_WRONG_SERIAL);
			this->CloseSocket();
			return;
		}

		STRING_SAFE_COPY(account_temp, MAX_ACCOUNT_LENGTH + 1, lpMsg->account, MAX_ACCOUNT_LENGTH);
		STRING_SAFE_COPY(password_temp, MAX_PASSWORD_LENGTH + 1, lpMsg->password, MAX_PASSWORD_LENGTH);

		Xor::Byte3(account_temp, MAX_ACCOUNT_LENGTH);
		Xor::Byte3(password_temp, MAX_PASSWORD_LENGTH);

		sLog->outInfo(LOG_PLAYER, "[ LOGIN REQUEST ] %s on [%s]", this->BuildLog().c_str(), account_temp);

		/*if ( sObjectMgr->AccountOnlineCheck(account_temp) )
		{
			sLog->outWarn(LOG_PLAYER, "[ LOGIN REQUEST ] %s Account [%s] Already Connected", this->BuildLog().c_str(), account_temp);
			this->LoginResult(LOGIN_RESULT_ONLINE);
			return;
		}*/

		this->CopyAccountSafe(account_temp);
		this->SetAccountSafeToLower();
		this->SetPasswordSafe(password_temp);
		this->GetAccountData()->SetMac(lpMsg->mac);
		this->GetAccountData()->SetGameServerSerial((const char*)lpMsg->client_serial, 16);
		this->GetAccountData()->SetDiskSerial(lpMsg->disk_serial);
	}

	//STRING_SAFE(account, MAX_ACCOUNT_LENGTH + 1);
	//sprintf(account, "%s", account_temp);

	this->SetDestServer(-1);
	this->SetDestWorld(-1);
	this->SetDestX(0);
	this->SetDestY(0);
	this->SetDestPort(-1);
	this->ResetDestIP();
	this->ResetDestAuth(0);
	this->SetServerMove(false);
	this->SetLoginSent(true);
	
	sAuthServer->PlayerLoginRequest(this, PLAYER_LOGIN_STEP_OK);
}

void Player::LoginResult(uint8 result)
{
	if ( this->GetAccountData()->GetLoginAttempts() >= sGameServer->GetMaxLoggingAttempts() && result != LOGIN_RESULT_SUCCESS )
	{
		result = LOGIN_RESULT_THREE_TIMES;
	}

	LOGIN_RESULT pMsg(result);
	this->sendPacket(MAKE_PCT(pMsg));

	if ( result == LOGIN_RESULT_THREE_TIMES )
	{
		this->CloseSocket();
	}
}

void Player::LogoutRequest(uint8 * Packet)
{
	if ( this->GetAccountData()->GetCloseCount() > 0 )
	{
		sLog->outWarn(LOG_PLAYER, "[ LOGOUT REQUEST ] %s - Already Request Logout", this->BuildLog().c_str());
		return;
	}

	if ( this->GetConnectStatus() == CONNECT_STATUS_PLAYING )
	{
		if ( this->IsInSelfDefense() )
		{
			this->LogoutResult(-1);
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to logout while self defense is active.");
			return;
		}

		this->CancelAllActivities();
	}

	POINTER_PCT_LOG(LOGOUT_REQUEST, lpMsg, Packet, 0);

	switch ( lpMsg->flag )
	{
	case CLOSE_FLAG_EXIT_GAME:
		{
			sLog->outInfo(LOG_PLAYER, "[ LOGOUT REQUEST ] %s - Exit Game", this->BuildLog().c_str());
		} break;
	case CLOSE_FLAG_SELECT_CHAR:
		{
			sLog->outInfo(LOG_PLAYER, "[ LOGOUT REQUEST ] %s - Select Character", this->BuildLog().c_str());
		} break;
	case CLOSE_FLAG_SELECT_SERVER:
		{
			sLog->outInfo(LOG_PLAYER, "[ LOGOUT REQUEST ] %s - Select Server", this->BuildLog().c_str());
		} break;
	default:
		{
			sLog->outError(LOG_PLAYER, "[ LOGOUT REQUEST ] %s - Flag %d", this->BuildLog().c_str(), lpMsg->flag);
		} break;
	}

	if ( lpMsg->flag >= MAX_CLOSE_FLAG )
	{
		lpMsg->flag = CLOSE_FLAG_EXIT_GAME;
	}

	this->Close(6, CloseType(lpMsg->flag));
}

void Player::LogoutResult(uint8 flag)
{
	LOGOUT_RESULT pMsg(flag);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::LogoutTimeCheck()
{
	if ( this->GetAccountData()->IsCloseGame() )
		return;

	if ( !this->IsSecondPassed() )
		return;

	if ( this->GetAccountData()->GetCloseCount() > 0 )
	{
		if ( this->GetAccountData()->GetCloseCount() == 1 )
		{
			switch ( this->GetAccountData()->GetCloseType() )
			{
			case CLOSE_FLAG_EXIT_GAME:
				{
					this->GameClose();
					this->LogoutResult(CLOSE_FLAG_EXIT_GAME);
					this->GetAccountData()->SetCloseCount(0);
				} break;

			case CLOSE_FLAG_SELECT_CHAR:
				{
					if ( this->GetAccountData()->GetCloseAuthorization() == 2 && this->GetConnectStatus() == CONNECT_STATUS_PLAYING )
					{
						this->SetCharacterListRequested(false);
						sServerLink->CharacterOnOff(this, 0);
						this->SetConnectStatus(CONNECT_STATUS_LOGGED);
						sObjectMgr->RemoveSessionCharacter(this);
						this->GameClose();

						this->SendClearBuff();

						this->LogoutResult(CLOSE_FLAG_SELECT_CHAR);
						this->GetAccountData()->SetCloseCount(0);
					}
				} break;

			case CLOSE_FLAG_SELECT_SERVER:
				{
					this->GameClose();

					if ( this->GetConnectStatus() == CONNECT_STATUS_PLAYING )
					{
						this->SendClearBuff();
					}

					this->LogoutResult(CLOSE_FLAG_SELECT_SERVER);
					this->GetAccountData()->SetCloseCount(0);
				} break;
			}
		}
		else
		{
			this->GetAccountData()->ReduceCloseCount(1);
			this->SendNotice(NOTICE_NORMAL_BLUE, "Left %d second(s) to exit game.", this->GetAccountData()->GetCloseCount());
		}
	}
}

void Player::Ping(uint8 * Packet)
{
	if ( this->Object::IsPlaying() )
	{
		this->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);

		POINTER_PCT(PMSG_CLIENTTIME, lpMsg, Packet, 0);

		if ( sGameServer->IsAttackSpeedCheck() )
		{
			int32 att_speed = this->GetIntData(UNIT_INT_ATTACK_SPEED);
			int32 mc_speed = this->GetIntData(UNIT_INT_MAGIC_SPEED);

			if ( att_speed < lpMsg->AttackSpeed || mc_speed < lpMsg->MagicSpeed )
			{
				int32 attack_speed_diff = lpMsg->AttackSpeed - att_speed;
				int32 magic_speed_diff = lpMsg->MagicSpeed - mc_speed;

				if ( attack_speed_diff < 0 )
				{
					attack_speed_diff = 0;
				}

				if ( magic_speed_diff < 0 )
				{
					magic_speed_diff = 0;
				}

				if ( attack_speed_diff >= sGameServer->GetAttackSpeedRange() || magic_speed_diff >= sGameServer->GetAttackSpeedRange() )
				{
					sLog->outError("antihack", "%s ClientHack Detected : Editing AttackSpeed [%d][%d] [%d][%d]", 
						this->BuildLog().c_str(), 
						att_speed, lpMsg->AttackSpeed, 
						mc_speed, lpMsg->MagicSpeed);

					this->IncreaseCounter(PLAYER_COUNTER_ATTACK_SPEED, 1);
				
					if ( this->GetCounter(PLAYER_COUNTER_ATTACK_SPEED) > sGameServer->GetAttackSpeedCount() )
					{
						if ( sGameServer->IsAttackSpeedBan() )
						{
							this->SetAuthority(1);
						}

						if ( sGameServer->IsAttackSpeedKick() )
						{
							KICK_PLAYER(this, "Attack Speed");

							sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Attack Speed");
						}
					}
				}
				else
				{
					this->SetCounter(PLAYER_COUNTER_ATTACK_SPEED, 0);
				}
			}
			else
			{
				this->SetCounter(PLAYER_COUNTER_ATTACK_SPEED, 0);
			}
		}
	}
}

void Player::Ping()
{
	if ( sGameServer->IsConnectCheckResetOnAction() )
		this->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);
}

bool Player::PacketTimeCheck()
{
	return this->GetTimer(PLAYER_TIMER_PACKET)->Elapsed(300);
}

void Player::CharacterListRequest()
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_LOGGED )
	{
		this->CloseSocket();
		return;
	}

	if ( this->IsCharacterListRequested() )
	{
		return;
	}

	this->SetCharacterListRequested(true);

	this->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);
	this->GameClose(false);

	for ( uint8 i = 0; i < MAX_CHARACTER_PER_ACCOUNT; ++i )
	{
		this->GetCharacterList(i)->Reset();
	}

	CharacterListQueryHolder* holder = new CharacterListQueryHolder(this->GetAccountData()->GetGUID());
    if (!holder->Initialize())
    {
        delete holder;
		this->CloseSocket();
        return;
    }

	this->_charListCallBack = MuDatabase.DelayQueryHolder(holder);
}

void Player::CharacterListRequestCallBack(QueryHolder holder)
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_LOGGED )
	{
		this->CloseSocket();
		return;
	}

	for ( int32 i = 0; i < MAX_CHARACTER_PER_ACCOUNT; ++i )
	{
		this->GetCharacterList(i)->Reset();
	}

	this->SetAutoLoginInfo(0);

	int32 max_level = 0;
	int32 online_count = 0;
	int32 characters = 0;
	
	PreparedQueryResult character_result = holder->GetPreparedResult(CHARACTER_LIST_QUERY_CHARACTER_DATA);

	if ( character_result )
	{
		do
		{
			Field* fields = character_result->Fetch();

			this->GetCharacterList(characters)->SetSlot(fields[7].GetUInt8());
			this->GetCharacterList(characters)->SetGUID(fields[0].GetUInt32());
			this->GetCharacterList(characters)->SetName(fields[1].GetString().c_str(), fields[1].GetString().size());
			this->GetCharacterList(characters)->SetLevelNormal(fields[2].GetInt16());
			this->GetCharacterList(characters)->SetLevelMaster(fields[3].GetInt16());
			this->GetCharacterList(characters)->SetLevelMajestic(fields[4].GetInt16());
			this->GetCharacterList(characters)->SetAuthority(fields[5].GetUInt8());
			this->GetCharacterList(characters)->SetRace(fields[6].GetUInt8());
			this->GetCharacterList(characters)->SetKickTime(fields[10].GetInt64());
			this->GetCharacterList(characters)->SetGuildRanking(fields[9].GetUInt32() ? fields[8].GetUInt8() : 0xFF);
			this->GetCharacterList(characters)->SetPKLevel(fields[11].GetUInt8());
			this->GetCharacterList(characters)->SetCreateDate(fields[13].GetInt64());

			if ( fields[12].GetUInt8() == 1 )
			{
				++online_count;
			}

			++characters;

			if (characters >= MAX_CHARACTER_PER_ACCOUNT)
			{
				break;
			}
		}
		while(character_result->NextRow());
	}

	if ( sGameServer->IsCharacterOnlineOnLogin() && online_count > 0 )
	{
		this->CloseSocket();
		return;
	}

	PreparedQueryResult account_data_result = holder->GetPreparedResult(CHARACTER_LIST_QUERY_ACCOUNT_DATA);

	if ( account_data_result )
	{
		Field* fields = account_data_result->Fetch();

		this->GetAccountData()->SetVIPStatus(fields[0].GetInt32());
		this->GetAccountData()->SetVIPDuration(fields[1].GetInt64());
		this->GetWarehouse()->SetExpanded(fields[2].GetUInt8());
		this->GetWarehouse()->SetExpandedTime(fields[3].GetInt64());
		this->GetAccountData()->SetSpecialCharacter(fields[4].GetUInt16());
	}
	else
	{
		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CREATE);
		stmt->setUInt32(0, this->GetAccountData()->GetGUID());
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}
		
	this->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_SUMMONER);
	this->GetAccountData()->SetCharacterHandleAction(CHARACTER_HANDLE_ACTION_NONE);

	PreparedQueryResult character_item_result = holder->GetPreparedResult(CHARACTER_LIST_QUERY_CHARACTER_ITEM_DATA);

	if ( character_item_result )
	{
		do
		{
			Field* fields = character_item_result->Fetch();

			uint32 id = fields[0].GetUInt32();
			uint32 slot = fields[1].GetUInt32();

			if ( id )
			{
				for ( uint8 i = 0; i < MAX_CHARACTER_PER_ACCOUNT; ++i )
				{
					if ( this->GetCharacterList(i)->GetGUID() == id)
					{
						this->GetCharacterList(i)->GetItem(slot)->SetItem(fields[2].GetUInt16());
						this->GetCharacterList(i)->GetItem(slot)->SetLevel(fields[3].GetUInt8());
						this->GetCharacterList(i)->GetItem(slot)->SetExcellent(fields[4].GetUInt8());
						this->GetCharacterList(i)->GetItem(slot)->SetAncient(fields[5].GetUInt8());
					}
				}
			}
		}
		while(character_item_result->NextRow());
	}

	uint8 buffer[8192];
	POINTER_PCT(CHARACTER_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(CHARACTER_LIST_BODY, body, buffer, sizeof(CHARACTER_LIST_HEAD));
	head->count = 0;
	head->unk2 = 0;
	head->max_character = MAX_CHARACTER_PER_ACCOUNT;
	head->special_character = 5;
	
	head->expanded_warehouse = this->GetWarehouse()->GetExpanded();

	int32 index;
	int32 levelindex;

	for ( uint8 i = 0; i < MAX_CHARACTER_PER_ACCOUNT; ++i )
	{
		if ( !this->GetCharacterList(i)->GetGUID() )
		{
			continue;
		}

		memcpy(body[head->count].name, this->GetCharacterList(i)->GetName(), MAX_CHARACTER_LENGTH);
		body[head->count].index = i;
		body[head->count].level = this->GetCharacterList(i)->GetLevelNormal() + this->GetCharacterList(i)->GetLevelMaster() + this->GetCharacterList(i)->GetLevelMajestic();
		body[head->count].pk_level = this->GetCharacterList(i)->GetPKLevel();

		body[head->count].authority = this->GetCharacterList(i)->GetAuthority();
		body[head->count].guild_rank = this->GetCharacterList(i)->GetGuildRanking();
		body[head->count].unk37 = 0;

		if (body[head->count].level >= max_level)
			max_level = body[head->count].level;

		if ( this->GetCharacterList(i)->GetKickTime() > 0 )
		{
			if (!body[head->count].authority && time(nullptr) < this->GetCharacterList(i)->GetKickTime())
			{
				body[head->count].authority = 1;
			}
		}

		if (sGameServer->IsTimeCheck())
		{
			if (this->GetCharacterList(i)->GetCreateDate() > 0 && sGameServer->GetTimeCheckTime() > 0)
			{
				if (!body[head->count].authority && time(nullptr) > (this->GetCharacterList(i)->GetCreateDate() + sGameServer->GetTimeCheckTime()))
				{
					body[head->count].authority = 1;
				}
			}

			if (sGameServer->GetTimeCheckLevelMin() != -1 && body[head->count].level < sGameServer->GetTimeCheckLevelMin())
			{
				if (!body[head->count].authority)
				{
					body[head->count].authority = 1;

					this->SendNotice(NOTICE_NORMAL_BLUE, "You have characters that can't join this server.");
					this->SendNotice(NOTICE_NORMAL_BLUE, "Only characters higher than lvl %d", sGameServer->GetTimeCheckLevelMin() - 1);
				}
			}

			if (sGameServer->GetTimeCheckLevelMax() != -1 && body[head->count].level > sGameServer->GetTimeCheckLevelMax())
			{
				if (!body[head->count].authority)
				{
					body[head->count].authority = 1;

					this->SendNotice(NOTICE_NORMAL_BLUE, "You have characters that can't join this server.");
					this->SendNotice(NOTICE_NORMAL_BLUE, "Only characters lower than lvl %d", sGameServer->GetTimeCheckLevelMax() + 1);
				}
			}
		}

		memset(body[head->count].preview, 0, MAX_PREVIEW_DATA);

		body[head->count].preview[CS_CLASS] = this->GetCharacterList(i)->GetRace() & ~7;

		if ( ((this->GetCharacterList(i)->GetRace() & 0x07) & 0x01) != 0 )
		{
			body[head->count].preview[CS_CLASS] |= 8;
		}

		if ( ((this->GetCharacterList(i)->GetRace() & 0x07) & 0x02) != 0 )
		{
			body[head->count].preview[CS_CLASS] |= 4;
		}

		if ( ((this->GetCharacterList(i)->GetRace() & 0x07) & 0x04) != 0 )
		{
			body[head->count].preview[CS_CLASS] |= 2;
		}

		this->GetCharacterList(i)->GetItem(0)->SetItem(this->GetCharacterList(i)->GetItem(0)->GetItem());
		this->GetCharacterList(i)->GetItem(1)->SetItem(this->GetCharacterList(i)->GetItem(1)->GetItem());
		this->GetCharacterList(i)->GetItem(2)->SetItem(this->GetCharacterList(i)->GetItem(2)->GetItem() % MAX_SUBTYPE_ITEMS);
		this->GetCharacterList(i)->GetItem(3)->SetItem(this->GetCharacterList(i)->GetItem(3)->GetItem() % MAX_SUBTYPE_ITEMS);
		this->GetCharacterList(i)->GetItem(4)->SetItem(this->GetCharacterList(i)->GetItem(4)->GetItem() % MAX_SUBTYPE_ITEMS);
		this->GetCharacterList(i)->GetItem(5)->SetItem(this->GetCharacterList(i)->GetItem(5)->GetItem() % MAX_SUBTYPE_ITEMS);
		this->GetCharacterList(i)->GetItem(6)->SetItem(this->GetCharacterList(i)->GetItem(6)->GetItem() % MAX_SUBTYPE_ITEMS);

		body[head->count].preview[9] = 0;

		MAKE_WEAPON_01(body[head->count].preview, this->GetCharacterList(i)->GetItem(0)->GetItem());
		MAKE_WEAPON_02(body[head->count].preview, this->GetCharacterList(i)->GetItem(1)->GetItem());
		MAKE_HELMET(body[head->count].preview, this->GetCharacterList(i)->GetItem(2)->GetItem());
		MAKE_ARMOR(body[head->count].preview, this->GetCharacterList(i)->GetItem(3)->GetItem());
		MAKE_PANTS(body[head->count].preview, this->GetCharacterList(i)->GetItem(4)->GetItem());
		MAKE_GLOVES(body[head->count].preview, this->GetCharacterList(i)->GetItem(5)->GetItem());
		MAKE_BOOTS(body[head->count].preview, this->GetCharacterList(i)->GetItem(6)->GetItem());

		index = 0;

		//v2 = ((*(_BYTE *)(a1 + 15) & 0x1C) >> 2) | 8 * (*(_BYTE *)(a1 + 8) & 7) | 16 * (*(_BYTE *)(a1 + 4) & 0xC);

		//v2 = ((preview[16] & 0x1C) >> 2) | 8 * (preview[9] & 7) | 16 * (preview[5] & 0xC);
		//					preview[16]						preview[9]						preview[5]


		/*
		5 -> Max 12
		9 -> Max 7
		16 -> Max 28
		*/

		///- Set Wings
		switch (this->GetCharacterList(i)->GetItem(7)->GetItem())
		{
		case ITEMGET(12, 0):
		case ITEMGET(12, 1):
		case ITEMGET(12, 2):
			{
				body[head->count].preview[16] |= 0x04 * ((this->GetCharacterList(i)->GetItem(7)->GetItem() - ITEMGET(12, 0)) + 1);
			} break;

		case ITEMGET(12, 3): // ELF Lvl 2
		case ITEMGET(12, 424): // ELF Lvl 2
			{
				body[head->count].preview[16] |= 16;
			} break;

		case ITEMGET(12, 4): // DW Lvl 2
		case ITEMGET(12, 422): // DW Lvl 2
			{
				body[head->count].preview[16] |= 20;
			} break;

		case ITEMGET(12, 5): // DK Lvl 2
		case ITEMGET(12, 423): // DK Lvl 2
			{
				body[head->count].preview[16] |= 24;
			} break;

		case ITEMGET(12, 6): // MG Lvl 2
		case ITEMGET(12, 425): // MG Lvl 2
			{
				body[head->count].preview[16] |= 28;
			} break;

		case ITEMGET(12, 36): // DK Lvl 3
		case ITEMGET(12, 431): // DK Lvl 3
			{
				body[head->count].preview[16] |= 0x00;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 37): // DW Lvl 3
		case ITEMGET(12, 430): // DW Lvl 3
			{
				body[head->count].preview[16] |= 0x04;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 38): // ELF Lvl 3
		case ITEMGET(12, 432): // ELF Lvl 3
			{
				body[head->count].preview[16] |= 0x08;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 39): // MG Lvl 3
		case ITEMGET(12, 433): // MG Lvl 3
			{
				body[head->count].preview[16] |= 12;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 40): // DL Lvl 3
		case ITEMGET(12, 434): // DL Lvl 3
			{
				body[head->count].preview[16] |= 16;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 41): // SUM Lvl 1
			{
				body[head->count].preview[16] |= 20;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 42): // SUM Lvl 2
		case ITEMGET(12, 427): // SUM Lvl 2
			{
				body[head->count].preview[16] |= 24;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 43): // SUM Lvl 3
		case ITEMGET(12, 435): // SUM Lvl 3
			{
				body[head->count].preview[16] |= 28;
				body[head->count].preview[9] |= 1;
			} break;

		case ITEMGET(12, 49): // RF Lvl 2
		case ITEMGET(12, 428): // RF Lvl 2
			{
				body[head->count].preview[16] |= 0;
				body[head->count].preview[9] |= 2;
			} break;

		case ITEMGET(12, 50): // RF Lvl 3
		case ITEMGET(12, 436): // RF Lvl 3
			{
				body[head->count].preview[16] |= 0x04;
				body[head->count].preview[9] |= 2;
			} break;

		case ITEMGET(12, 130): // Small Cape of Lord
		case ITEMGET(12, 131): // Small Wings of Misery
		case ITEMGET(12, 132): // Small Wings of Elf
		case ITEMGET(12, 133): // Small Wings of Heaven
		case ITEMGET(12, 134): // Small Wings of Satan
		case ITEMGET(12, 135): // Small Cloak of Warrior
		{
			body[head->count].preview[16] |= 4 * ((this->GetCharacterList(i)->GetItem(7)->GetItem() - ITEMGET(12, 130) + 2));
			body[head->count].preview[9] |= 2;
		} break;

		case ITEMGET(12, 262):
		case ITEMGET(12, 263):
		case ITEMGET(12, 264):
		case ITEMGET(12, 265):
		case ITEMGET(12, 267):
		{
			body[head->count].preview[16] |= 4 * (this->GetCharacterList(i)->GetItem(7)->GetItem() - ITEMGET(12, 262));
			body[head->count].preview[9] |= 3;
		} break;

		case ITEMGET(13, 30): // DL Lvl 2
		case ITEMGET(12, 426): // DL Lvl 2
			{
				body[head->count].preview[16] |= 4 * 6;
				body[head->count].preview[9] |= 3;
			} break;

		case ITEMGET(12, 269): // GL Lvl 2
		case ITEMGET(12, 429): // GL Lvl 2
			{
				body[head->count].preview[16] |= 4 * 7;
				body[head->count].preview[9] |= 3;
			} break;

		case ITEMGET(12, 270): // GL Lvl 3
		case ITEMGET(12, 437): // GL Lvl 3
			{
				if (this->GetCharacterList(i)->GetItem(7)->GetLevel() == 15)
				{
					body[head->count].preview[16] |= 8;
					body[head->count].preview[9] |= 4;
				}
				else
				{
					body[head->count].preview[16] |= 0;
					body[head->count].preview[9] |= 4;
				}
			} break;

		case ITEMGET(12, 278):
			{
				body[head->count].preview[16] |= 4;
				body[head->count].preview[9] |= 4;
			} break;

		case ITEMGET(12, 266):
		case ITEMGET(12, 268):
			{
				body[head->count].preview[16] |= 12;
				body[head->count].preview[9] |= 4;
			} break;
		
		case ITEMGET(12, 279):
		case ITEMGET(12, 284):
			{
				body[head->count].preview[16] |= 16;
				body[head->count].preview[9] |= 4;
			} break;

		case ITEMGET(12, 280):
		case ITEMGET(12, 285):
			{
				body[head->count].preview[16] |= 20;
				body[head->count].preview[9] |= 4;
			} break;

		case ITEMGET(12, 281):
		case ITEMGET(12, 286):
			{
				body[head->count].preview[16] |= 24;
				body[head->count].preview[9] |= 4;
			} break;

		case ITEMGET(12, 282):
		case ITEMGET(12, 287):
			{
				body[head->count].preview[16] |= 28;
				body[head->count].preview[9] |= 4;
			} break;

		case ITEMGET(12, 414):
			{
				body[head->count].preview[16] |= 0;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 415):
			{
				body[head->count].preview[16] |= 4;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 416):
			{
				body[head->count].preview[16] |= 8;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 417):
			{
				body[head->count].preview[16] |= 12;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 418):
			{
				body[head->count].preview[16] |= 16;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 419):
			{
				body[head->count].preview[16] |= 20;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 420):
			{
				body[head->count].preview[16] |= 24;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 421):
			{
				body[head->count].preview[16] |= 28;
				body[head->count].preview[9] |= 5;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;
			
		case ITEMGET(12, 438):
			{
				body[head->count].preview[16] |= 0;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 439):
			{
				body[head->count].preview[16] |= 4;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 440):
			{
				body[head->count].preview[16] |= 8;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 441):
			{
				body[head->count].preview[16] |= 12;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 442):
			{
				body[head->count].preview[16] |= 16;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 443):
			{
				body[head->count].preview[16] |= 20;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 444):
			{
				body[head->count].preview[16] |= 24;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 445):
			{
				body[head->count].preview[16] |= 28;
				body[head->count].preview[9] |= 6;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 467):
		case ITEMGET(12, 468):
			{
				body[head->count].preview[16] |= 0;
				body[head->count].preview[9] |= 7;
				//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
			} break;

		case ITEMGET(12, 469):
		{
			body[head->count].preview[16] |= 4;
			body[head->count].preview[9] |= 7;
			//body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
		} break;

		case ITEMGET(12, 472):
		case ITEMGET(12, 473):
		{
								 body[head->count].preview[16] |= 16;
								 body[head->count].preview[9] |= 7;
								 //body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
		} break;

		case ITEMGET(12, 474):
		{
								 body[head->count].preview[16] |= 20;
								 body[head->count].preview[9] |= 7;
								 //body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
		} break;

		case ITEMGET(12, 480):
		{
								 body[head->count].preview[16] |= 12;
								 body[head->count].preview[9] |= 7;
								 //body[head->count].preview[17] = this->GetCharacterList(i)->GetItem(7)->GetLevel();
		} break;

		default:
			{
				index |= 0;
			} break;
		}

		body[head->count].preview[18] = this->GetCharacterList(i)->GetItem(7)->GetLevel();

		// Pets Set
		switch ( this->GetCharacterList(i)->GetItem(8)->GetItem() )
		{
		case ITEMGET(13, 0):
		case ITEMGET(13, 1):
			{
				index |= CS_SET_HELPER(this->GetCharacterList(i)->GetItem(8)->GetItem() % MAX_SUBTYPE_ITEMS);
			} break;

		// Panda Pet
		case ITEMGET(13, 80): { body[head->count].preview[16] |= 0xE0; } break;
		// Unicorn
		case ITEMGET(13, 106): { body[head->count].preview[16] |= 0xA0; } break;
		// Rudolf Pet
		case ITEMGET(13, 67): { body[head->count].preview[16] |= 0x80; } break;
		// Skeleton Pet
		case ITEMGET(13, 123): { body[head->count].preview[16] |= 0x60; } break;
		// Spirit of Guardian
		case ITEMGET(13, 65): { body[head->count].preview[16] |= 0x40; } break;
		// Demon Pet
		case ITEMGET(13, 64): { body[head->count].preview[16] |= 0x20; } break;

		default:
			{
				index |= 3;
			} break;
		}

		body[head->count].preview[5] |= index;

		levelindex = CS_SET_SMALLLEVEL_RH(sItemMgr->LevelSmallConvert(this->GetCharacterList(i)->GetItem(0)->GetLevel()));
		levelindex |= CS_SET_SMALLLEVEL_LH(sItemMgr->LevelSmallConvert(this->GetCharacterList(i)->GetItem(1)->GetLevel()));
		levelindex |= CS_SET_SMALLLEVEL_HELMET(sItemMgr->LevelSmallConvert(this->GetCharacterList(i)->GetItem(2)->GetLevel()));
		levelindex |= CS_SET_SMALLLEVEL_ARMOR(sItemMgr->LevelSmallConvert(this->GetCharacterList(i)->GetItem(3)->GetLevel()));
		levelindex |= CS_SET_SMALLLEVEL_PANTS(sItemMgr->LevelSmallConvert(this->GetCharacterList(i)->GetItem(4)->GetLevel()));
		levelindex |= CS_SET_SMALLLEVEL_GLOVES(sItemMgr->LevelSmallConvert(this->GetCharacterList(i)->GetItem(5)->GetLevel()));
		levelindex |= CS_SET_SMALLLEVEL_BOOTS(sItemMgr->LevelSmallConvert(this->GetCharacterList(i)->GetItem(6)->GetLevel()));

		body[head->count].preview[CS_LEVEL1] = CS_SET_SMALLLEVEL1(levelindex);
		body[head->count].preview[CS_LEVEL2] = CS_SET_SMALLLEVEL2(levelindex);
		body[head->count].preview[CS_LEVEL3] = CS_SET_SMALLLEVEL3(levelindex);

		// Set Excellent Items
		body[head->count].preview[CS_EXE_EFFECT] = 0;
		if ( this->GetCharacterList(i)->GetItem(2)->GetExcellent() ) body[head->count].preview[CS_EXE_EFFECT] |= 0x80;
		if ( this->GetCharacterList(i)->GetItem(3)->GetExcellent() ) body[head->count].preview[CS_EXE_EFFECT] |= 0x40;
		if ( this->GetCharacterList(i)->GetItem(4)->GetExcellent() ) body[head->count].preview[CS_EXE_EFFECT] |= 0x20;
		if ( this->GetCharacterList(i)->GetItem(5)->GetExcellent() ) body[head->count].preview[CS_EXE_EFFECT] |= 0x10;
		if ( this->GetCharacterList(i)->GetItem(6)->GetExcellent() ) body[head->count].preview[CS_EXE_EFFECT] |= 0x08;
		if ( this->GetCharacterList(i)->GetItem(0)->GetExcellent() ) body[head->count].preview[CS_EXE_EFFECT] |= 0x04;
		if ( this->GetCharacterList(i)->GetItem(1)->GetExcellent() ) body[head->count].preview[CS_EXE_EFFECT] |= 0x02;

		// Set Set Items
		body[head->count].preview[CS_ANC_EFFECT] = 0;
		if ( this->GetCharacterList(i)->GetItem(2)->GetAncient() ) body[head->count].preview[CS_ANC_EFFECT] |= 0x80;
		if ( this->GetCharacterList(i)->GetItem(3)->GetAncient() ) body[head->count].preview[CS_ANC_EFFECT] |= 0x40;
		if ( this->GetCharacterList(i)->GetItem(4)->GetAncient() ) body[head->count].preview[CS_ANC_EFFECT] |= 0x20;
		if ( this->GetCharacterList(i)->GetItem(5)->GetAncient() ) body[head->count].preview[CS_ANC_EFFECT] |= 0x10;
		if ( this->GetCharacterList(i)->GetItem(6)->GetAncient() ) body[head->count].preview[CS_ANC_EFFECT] |= 0x08;
		if ( this->GetCharacterList(i)->GetItem(0)->GetAncient() ) body[head->count].preview[CS_ANC_EFFECT] |= 0x04;
		if ( this->GetCharacterList(i)->GetItem(1)->GetAncient() ) body[head->count].preview[CS_ANC_EFFECT] |= 0x02;

		//if ( full_ancient )
		//{
		//	body[head->count].preview[CS_ANC_EFFECT] |= 0x01;
		//}

		head->count++;
	}

	if ( max_level >= sGameServer->GetRageFighterCreate() )
	{
		this->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_RAGE_FIGHTER);
	}

	if ( max_level >= sGameServer->GetGrowLancerCreate() )
	{
		this->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_GROW_LANCER);
	}

	if ( max_level >= sGameServer->GetMagicGladiatorCreate() )
	{
		this->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_MAGIC_GLADIATOR);
	}

	if ( max_level >= sGameServer->GetDarkLordCreate() )
	{
		this->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_DARK_LORD);
	}

	/*
	1 = RF
	2 = RF & GL
	3 = RF & GL & MG
	4 = RF & GL & MG & DL
	*/

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_LIST, sizeof(CHARACTER_LIST_HEAD) + (head->count * sizeof(CHARACTER_LIST_BODY)));

	this->sendPacket(buffer, head->h.get_size());
}

bool Player::CanCreateCharacter(uint8 race)
{
	return	(race == DB_CLASS_DARK_WIZARD) ||
			(race == DB_CLASS_DARK_KNIGHT) ||
			(race == DB_CLASS_FAIRY_ELF) ||
			(race == DB_CLASS_SUMMONER && this->GetAccountData()->IsSpecialCharacter(SPECIAL_CHARACTER_SUMMONER)) ||
		    (race == DB_CLASS_RAGE_FIGHTER && this->GetAccountData()->IsSpecialCharacter(SPECIAL_CHARACTER_RAGE_FIGHTER)) ||
		    (race == DB_CLASS_MAGIC_GLADIATOR && this->GetAccountData()->IsSpecialCharacter(SPECIAL_CHARACTER_MAGIC_GLADIATOR)) ||
		    (race == DB_CLASS_DARK_LORD && this->GetAccountData()->IsSpecialCharacter(SPECIAL_CHARACTER_DARK_LORD)) ||
			(race == DB_CLASS_GROW_LANCER && this->GetAccountData()->IsSpecialCharacter(SPECIAL_CHARACTER_GROW_LANCER)) ||
			(race == DB_CLASS_RUNE_WIZARD) ||
			(race == DB_CLASS_SLAYER) ||
			(race == DB_CLASS_GUNNER_1);
}

void Player::CharacterCreate(uint8 * Packet)
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_LOGGED )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER CREATE ] %s Not Logged", this->BuildLog().c_str());
		this->CloseSocket();
		return;
	}

	if ( this->GetAccountData()->GetCharacterHandleAction() != CHARACTER_HANDLE_ACTION_NONE )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER CREATE ] %s Character Handle Action %d", this->BuildLog().c_str(), this->GetAccountData()->GetCharacterHandleAction());
		//this->CloseSocket();
		return;
	}

	POINTER_PCT_LOG(CHARACTER_CREATE_REQUEST, lpMsg, Packet, 0);

	this->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);

	if ( !Character::Allowed(lpMsg->character_class) )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER CREATE ] %s Invalid character class %d", this->BuildLog().c_str(), lpMsg->character_class);
		this->CharacterCreateResult(CHARACTER_CREATE_RESULT_ERROR);
		return;
	}

	if ( !this->CanCreateCharacter(lpMsg->character_class) )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER CREATE ] %s Not allowed to create character class %d", this->BuildLog().c_str(), lpMsg->character_class);
		this->CharacterCreateResult(CHARACTER_CREATE_RESULT_ERROR);
		return;
	}

	CharacterBaseData const* info = sCharacterBase->GetCharacterBase(lpMsg->character_class >> 4);

	if ( !info )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER CREATE ] %s No default character info for class %d", this->BuildLog().c_str(), lpMsg->character_class);
		this->CharacterCreateResult(CHARACTER_CREATE_RESULT_ERROR);
		return;
	}

	STRING_SAFE_COPY(character_name, MAX_CHARACTER_LENGTH + 1, lpMsg->character_name, MAX_CHARACTER_LENGTH);
	STRING_SAFE(character, MAX_CHARACTER_LENGTH + 1);
	strcpy(character, character_name);

	for ( int32 i = 0; i < MAX_CHARACTER_LENGTH; ++i )
	{
		if ( character[i] && !isalnum(character[i]) )
		{
			sLog->outError(LOG_PLAYER, "[ CHARACTER CREATE ] %s / Character Name %s has wrong digits", this->BuildLog().c_str(), character);
			this->CharacterCreateResult(CHARACTER_CREATE_RESULT_ERROR);
			return;
		}
	}

	std::string character_name_string = character;

	if ( !sGameServer->FilterText(character_name_string, FILTER_FLAG_CHARACTER_NAME) )
	{
		this->CharacterCreateResult(CHARACTER_CREATE_RESULT_ERROR);
		return;
	}

	uint8 slot = this->GetCharacterEmptySlot();

	if ( slot == uint8(-1) )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER CREATE ] %s No slot available", this->BuildLog().c_str());
		this->CharacterCreateResult(CHARACTER_CREATE_RESULT_CANT_MORE);
		return;
	}

	this->GetAccountData()->SetCharacterHandleAction(CHARACTER_HANDLE_ACTION_CREATE);
	sServerLink->CharacterCreateRequest(this, lpMsg->character_class, character, slot, info);
}

void Player::CharacterCreateResult(uint8 result)
{
	CHARACTER_CREATE_RESULT pMsg(result);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::CharacterDelete(uint8 * Packet)
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_LOGGED )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER DELETE ] %s Not Logged.", this->BuildLog().c_str());
		this->CloseSocket();
		return;
	}

	if ( this->GetAccountData()->GetCharacterHandleAction() != CHARACTER_HANDLE_ACTION_NONE )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER DELETE ] %s Character Handle Action %d.", this->BuildLog().c_str(), this->GetAccountData()->GetCharacterHandleAction());
		//this->CloseSocket();
		return;
	}

	POINTER_PCT_LOG(CHARACTER_DELETE_REQUEST, lpMsg, Packet, 0);

	this->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);

	STRING_SAFE_COPY(secure_code, MAX_SECURE_CODE_LENGTH + 1, lpMsg->secure_code, MAX_SECURE_CODE_LENGTH);

	if ( !this->SecureCodeCheck(secure_code) )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER DELETE ] %s Wrong Secure Code.", this->BuildLog().c_str());
		this->CharacterDeleteResult(CHARACTER_DELETE_RESULT_WRONG_SECURITY_CODE);
		return;
	}

	STRING_SAFE_COPY(character_name, MAX_CHARACTER_LENGTH + 1, lpMsg->character_name, MAX_CHARACTER_LENGTH);
	STRING_SAFE(character, MAX_CHARACTER_LENGTH + 1);
	strcpy(character, character_name);

	this->GetAccountData()->SetCurrentCharacterIndex(this->GetCharacterListIndex(character));
	
	if ( this->GetAccountData()->GetCurrentCharacterIndex() == uint8(-1) )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER DELETE ] %s Character %s is not in character list.", this->BuildLog().c_str(), character);
		this->CloseSocket();
		return;
	}

	sLog->outInfo(LOG_PLAYER, "[ CHARACTER DELETE ] %s Character %s.", this->BuildLog().c_str(), character);

	this->GetAccountData()->SetCharacterHandleAction(CHARACTER_HANDLE_ACTION_DELETE);

	sServerLink->CharacterDeleteRequest(this, character);
}

void Player::CharacterDeleteResult(uint8 result)
{
	CHARACTER_DELETE_RESULT pMsg(result);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::CharacterNameCheck(uint8 * Packet)
{
	POINTER_PCT_LOG(CHARACTER_NAME_CHECK_REQUEST, lpMsg, Packet, 0);
	CHARACTER_NAME_CHECK_RESULT pMsg(lpMsg->character_name,0);
	this->sendPacket(MAKE_PCT(pMsg));
}
	
void Player::CharacterSelect(uint8 * Packet, bool server_move)
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_LOGGED )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s Not Logged.", this->BuildLog().c_str());
		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	if ( this->GetAccountData()->GetCharacterHandleAction() != CHARACTER_HANDLE_ACTION_NONE )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s Character Handle Action %d.", this->BuildLog().c_str(), this->GetAccountData()->GetCharacterHandleAction());
		this->SendJoinInfo(1);
		//this->CloseSocket();
		return;
	}

	POINTER_PCT_LOG(CHARACTER_SELECT_REQUEST, lpMsg, Packet, 0);
	STRING_SAFE_COPY(character_name, MAX_CHARACTER_LENGTH + 1, lpMsg->character_name, MAX_CHARACTER_LENGTH);

	this->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);

	if ( sObjectMgr->CharacterOnlineCheck(character_name) )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s Character %s already connected.",
			this->BuildLog().c_str(), character_name);

		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	this->GetAccountData()->SetCharacterHandleAction(CHARACTER_HANDLE_ACTION_SELECT);

	sLog->outInfo(LOG_PLAYER, "[ CHARACTER SELECT ] %s Character %s - %u selected ( Step One ).",
			this->BuildLog().c_str(), character_name, lpMsg->index);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_SELECT_QUERY);
	stmt->setString(0, character_name);
	stmt->setUInt32(1, this->GetAccountData()->GetGUID());

	this->_charSelectCallBack_1 = MuDatabase.AsyncQuery(stmt);
}

void Player::CharacterSelectCallBack(PreparedQueryResult result)
{
	if ( !result )
	{
		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	Field* fields = result->Fetch();

	uint32 guid = fields[0].GetUInt32();

	LoginQueryHolder * holder = new LoginQueryHolder(this->GetAccountData()->GetGUID(), guid);

    if (!holder->Initialize())
    {
        delete holder;

		this->SendJoinInfo(1);
		this->CloseSocket();
		sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] Error during Holder Creation %s - %u.", this->BuildLog().c_str(), guid);
        return;
    }

	sLog->outInfo(LOG_PLAYER, "[ CHARACTER SELECT ] %s Character selected ( Step Two ).",
			this->BuildLog().c_str());

	this->_charSelectCallBack_2 = MuDatabase.DelayQueryHolder(holder);
}

void Player::CharacterLoginHandle(QueryHolder holder)
{
	if ( this->GetConnectStatus() != CONNECT_STATUS_LOGGED )
	{
		this->SetAutoLogin(false);
		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	this->ClearCharacter();

	this->LoadDBInfo(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD));

	if ( this->GetAuthority() == AUTHORITY_CODE_BANNED )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s Banned.", this->BuildLog().c_str());

		this->SetAutoLogin(false);
		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	if ( !Character::AllowedDB(this->GetDBClass()) )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s has wrong db class %u.", this->BuildLog().c_str(), this->GetDBClass());

		this->SetAutoLogin(false);
		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	this->SetClass(this->GetDBClass() >> 4);
	this->SetChangeUP(0, this->GetDBClass() & 0x07 & 0x01);
	this->SetChangeUP(1, (this->GetDBClass() & 0x07 & 0x02) >> 1);
	this->SetChangeUP(2, (this->GetDBClass() & 0x07 & 0x04) >> 2);

	if (sGameServer->IsTimeCheck())
	{
		if (this->GetCreateDate() > 0 && sGameServer->GetTimeCheckTime() > 0)
		{
			if (this->GetAuthority() == AUTHORITY_CODE_NORMAL && time(nullptr) > (this->GetCreateDate() + sGameServer->GetTimeCheckTime()))
			{
				sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s Time Check.", this->BuildLog().c_str());

				this->SetAutoLogin(false);
				this->SendJoinInfo(1);
				this->CloseSocket();
				return;
			}
		}

		if (sGameServer->GetTimeCheckLevelMin() != -1 && this->GetTotalLevel() < sGameServer->GetTimeCheckLevelMin())
		{
			if (this->GetAuthority() == AUTHORITY_CODE_NORMAL)
			{
				sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s Time Check Min Level.", this->BuildLog().c_str());

				this->SetAutoLogin(false);
				this->SendJoinInfo(1);
				this->CloseSocket();
				return;
			}
		}

		if (sGameServer->GetTimeCheckLevelMax() != -1 && this->GetTotalLevel() > sGameServer->GetTimeCheckLevelMax())
		{
			if (this->GetAuthority() == AUTHORITY_CODE_NORMAL)
			{
				sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s Time Check Max Level.", this->BuildLog().c_str());

				this->SetAutoLogin(false);
				this->SendJoinInfo(1);
				this->CloseSocket();
				return;
			}
		}
	}

	this->LoadDefaultSkill();
	this->LoadDB(holder);

	if ( !this->CharacterSelectWorldSuccess() )
	{
		sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s has wrong world %d.", this->BuildLog().c_str(), this->GetWorldId());

		this->SetAutoLogin(false);
		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	if ( this->GetKickTime() > time(nullptr) )
	{
		sLog->outInfo(LOG_PLAYER, "[ CHARACTER SELECT ] %s is kicked.", this->BuildLog().c_str());

		this->SetAutoLogin(false);
		this->SendJoinInfo(1);
		this->CloseSocket();
		return;
	}

	if ( this->GetPKLevel() >= PK_STATUS_MAX )
	{
		this->SetPKLevel(PK_STATUS_MURDER_LVL_3);
	}

	if ( this->GetPKLevel() == PK_STATUS_COMMONER )
	{
		this->SetPKCount(0);
		this->SetPKPoints(0);
	}

	if ( this->GetInventory()->GetExpanded() > sGameServer->GetMaxExpandedInventory() )
	{
		this->GetInventory()->SetExpanded(sGameServer->GetMaxExpandedInventory());
	}

	this->SetRespawnTime(4 * IN_MILLISECONDS);

	this->RemoveBuff(BUFF_DUEL_WATCHER);
	this->RemoveBuff(BUFF_TRANSPARENCY);
	this->RemoveBuff(BUFF_ADMIN);

	this->StateInfoSend(BUFF_DUEL_WATCHER, 0);
	this->StateInfoSend(BUFF_TRANSPARENCY, 0);

	if ( this->IsAdministrator() )
	{
		this->AddBuff(BUFF_ADMIN, 0, BUFF_FLAG_CONSTANT);
		this->AddBuff(BUFF_INVISIBILITY, 0, BUFF_FLAG_CONSTANT);
		this->AddAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE);

		if ( sGameServer->IsAdministratorAuthorization() )
		{
			this->SetAuthorizationEnabled(false);
		}
	}

	sCharacterBase->FixCharacterExperience(this);

	this->CalculateNextExperience();
	this->CalculateCharacter();
	
	if ( this->IsFirstTime() )
	{
		POWER_LOOP(i)
		{
			this->PowerSet(i, this->PowerGetTotal(i));
		}
	}

	this->SetDeadStatus(DEAD_NONE);
	this->SetState(OBJECT_STATE_STAND_BY);
	this->SetLive(true);

	if ( this->PowerGet(POWER_LIFE) <= 0 )
	{
		this->PowerSet(POWER_LIFE, 1);
		this->GenerateRespawnLocation(true);
	}

	this->SetTX(this->GetX());
	this->SetTY(this->GetY());

	this->UpdateAutorecuperation();
	
	this->Close(0, CLOSE_FLAG_NONE);
	this->SetInstance(-1);
	this->SetLastItemUpdate(sItemMgr->GetLastItemUpdate());

	this->AddToWorld();

	if ( this->IsFirstTime() )
	{
		sCharacterBase->GenerateCharacterItemList(this);
	}

	this->AssignMount();

	this->PreviewMake();

	this->SetFirstTime(false);

	if ( !this->IsServerMove() )
	{
		uint16 server_move = sWorldMgr->AllowMoveToWorld(this, this->GetWorldId());

		if ( server_move != sGameServer->GetServerCode() )
		{
			/*if ( server_move == uint16(-1) )
			{
				sLog->outError(LOG_PLAYER, "[ CHARACTER SELECT ] %s is in wrong server. World %d.", this->BuildLog().c_str(), this->GetWorldId());

				this->SendJoinInfo(1);
				this->CloseSocket();
				return;
			}*/

			if ( this->IsAutoLogin() )
			{
				KICK_PLAYER(this, "AUTOLOGIN - WRONG MAP");
			}
			else
			{
				this->SetServerMoveFromCharacterSelect(true);
				sAuthServer->PlayerServerMoveRequest(this, server_move, this->GetWorldId(), this->GetX(), this->GetY());
			}

			this->SetAutoLogin(false);
			return;
		}
	}
	else
	{
		this->SetDestServer(-1);
		this->SetDestWorld(-1);
		this->SetDestX(0);
		this->SetDestY(0);
		this->SetDestPort(-1);
		this->ResetDestIP();
		this->ResetDestAuth(0);
		this->SetServerQuit(false);
	}

	this->CharacterSelectMapCheck();

	this->SendJoinInfo();

	this->EnterWorldSuccess(nullptr);

	this->SendWeather();
	this->RestoreEventAnimation();

	this->SetServerMove(false);
	this->SetServerMoveFromCharacterSelect(false);
}

void Player::CharacterSelectMapCheck()
{
	uint16 world_id = this->GetWorldId();
	int16 x = this->GetX();
	int16 y = this->GetY();
	CharacterBaseData const* base_data = sCharacterBase->GetCharacterBase(this->GetClass());
	bool force_update = false;
	bool get_from_world = true;

	if ( !base_data )
	{
		this->SetWorldId(-1);
		return;
	}

	if ( world_id == WORLD_SANTA_VILLAGE )
	{
		world_id = WORLD_DEVIAS;
	}
	else if ( BC_MAP_RANGE(world_id) )
	{
		if ( !sBloodCastleMgr->IsIn(this) )
		{
			this->GetValidCoordinates(BLOOD_CASTLE_GATE, world_id, x, y);
			get_from_world = false;
		}
	}
	else if ( DS_MAP_RANGE(world_id) )
	{
		if ( !sDevilSquareMgr->IsIn(this) )
		{
			this->GetValidCoordinates(DEVIL_SQUARE_GATE, world_id, x, y);
			get_from_world = false;
		}
	}
	else if ( CC_MAP_RANGE(world_id) )
	{
		this->GetValidCoordinates(CHAOS_CASTLE_GATE, world_id, x, y);
		get_from_world = false;
	}
	else if ( IT_MAP_RANGE(world_id) )
	{
		if ( !sIllusionTempleMgr->IsIn(this) )
		{
			this->GetValidCoordinates(267, world_id, x, y);
			get_from_world = false;
		}
	}
	else if ( DG_MAP_RANGE(world_id) )
	{
		if ( !sDoppelganger->IsIn(this) )
		{
			this->GetValidCoordinates(DOPPELGANGER_GATE, world_id, x, y);
			get_from_world = false;
		}
	}
	else if ( TS_MAP_RANGE(world_id) )
	{
		if ( !sTormentedSquare->IsIn(this) )
		{
			this->GetValidCoordinates(TORMENTED_SQUARE_GATE, world_id, x, y);
			get_from_world = false;
		}
	}
	else if ( IF_MAP_RANGE(world_id) )
	{
		this->GetValidCoordinates(503, world_id, x, y);
		get_from_world = false;
	}
	else if (world_id == WORLD_SPECIAL_EVENT_MAP)
	{
		this->GetValidCoordinates(503, world_id, x, y);
		get_from_world = false;
	}
	else if ( world_id == WORLD_RAKLION_BOSS )
	{
		world_id = WORLD_RAKLION;
	}
	else if ( world_id == WORLD_VULCANUS_ROOM )
	{
		this->GetValidCoordinates(17, world_id, x, y);
		get_from_world = false;
	}
	else if ( world_id == WORLD_KANTURU_BOSS )
	{
		world_id = WORLD_KANTURU_CORE;
	}
	else if ( world_id == WORLD_ACHERON_BATTLE || 
			  world_id == WORLD_DEVENTER_BATTLE || 
			  world_id == WORLD_URUK_MOUNTAIN_BATTLE )
	{
		if ( !this->IsAdministrator() )
		{
			if ( sProtectorOfAcheron->GetState() != PROTECTOR_OF_ACHERON_STATE_PLAYING && 
				 sArkaWar->GetState() != ARKA_WAR_STATE_READY &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_STANDBY &&
				 sArkaWar->GetState() != ARKA_WAR_STATE_PLAYING )
			{
				world_id = WORLD_NORIA;
			}
		}
	}
	else if ( world_id == WORLD_BALGASS_BARRACKS || world_id == WORLD_BALGASS_REFUGE )
	{
		if ( !this->IsServerMove() )
		{
			world_id = WORLD_BALGASS_BARRACKS;
			force_update = true;
		}
	}
	else if ( world_id == WORLD_STADIUM )
	{
		this->GetValidCoordinates(17, world_id, x, y);
		get_from_world = false;
	}
	else if ( world_id == WORLD_CASTLE_SIEGE )
	{
		if ( IsInDragonTower(x, y, x, y) || sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
		{
			world_id = WORLD_CASTLE_SIEGE;
			force_update = true;
		}
	}
	else if ( world_id == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE )
	{
		this->GetValidCoordinates(503, world_id, x, y);
		get_from_world = false;
	}
	else if ( world_id == WORLD_PLACE_OF_QUALIFICATION )
	{
		this->GetValidCoordinates(537, world_id, x, y);
		get_from_world = false;
	}
	else if (sDungeon->IsDungeon(world_id))
	{
		this->GetValidCoordinates(sGameServer->GetDungeonEndGate(), world_id, x, y);
		get_from_world = false;
	}
	
	if (sSwampOfDarkness->IsBossZone(this))
	{
		world_id = WORLD_SWAMP_OF_DARKNESS;
		force_update = true;
	}

	if (sNixiesLake->IsPlayerInSanctuary(this))
	{
		world_id = WORLD_NIXIES_LAKE;
		force_update = true;
	}

	if ( world_id != this->GetWorldId() || force_update )
	{
		World* pWorld = sWorldMgr->GetWorld(world_id);

		if ( pWorld )
		{
			if ( get_from_world )
			{
				pWorld->GetRespawn(world_id, x, y);
			}
			this->SetWorldId(world_id);
			this->SetX(x);
			this->SetY(y);
			this->SetTX(x);
			this->SetTY(y);
			this->SetWalkSpeedCount(0);
			this->GetACPathData()->Reset();

			//this->TeleportToLocation(world_id, x, y, this->GetDirection(), this->GetInstance());
		}
	}
}

bool Player::CharacterSelectWorldSuccess()
{
	CharacterBaseData const* base_data = sCharacterBase->GetCharacterBase(this->GetClass());

	if (!base_data)
	{
		this->SetWorldId(-1);
		return false;
	}

	if (this->IsServerMove())
	{
		if (sWorldMgr->AllowMoveToWorld(this, this->GetDestWorld()) == sGameServer->GetServerCode())
		{
			this->SetWorldId(this->GetDestWorld());
			this->SetX(this->GetDestX());
			this->SetY(this->GetDestY());
			this->SetTX(this->GetDestX());
			this->SetTY(this->GetDestY());
			this->SetWalkSpeedCount(0);
			this->GetACPathData()->Reset();
		}
		else
		{
			this->SetWorldId(-1);
		}
	}

	if (this->GetWorldId() == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
	{
		uint16 world_id = WORLD_LOREN_MARKET;
		int16 x = 0;
		int16 y = 0;

		this->GetValidCoordinates(503, world_id, x, y);

		if (World* pWorld = sWorldMgr->GetWorld(world_id))
		{
			this->SetWorldId(world_id);
			this->SetX(x);
			this->SetY(y);
			this->SetTX(x);
			this->SetTY(y);
			this->SetWalkSpeedCount(0);
			this->GetACPathData()->Reset();
		}
	}
	else if (this->GetWorldId() == WORLD_PLACE_OF_QUALIFICATION)
	{
		uint16 world_id = WORLD_LABYRINTH_OF_DIMENSIONS_SAFE;
		int16 x = 0;
		int16 y = 0;

		this->GetValidCoordinates(537, world_id, x, y);

		if (World* pWorld = sWorldMgr->GetWorld(world_id))
		{
			this->SetWorldId(world_id);
			this->SetX(x);
			this->SetY(y);
			this->SetTX(x);
			this->SetTY(y);
			this->SetWalkSpeedCount(0);
			this->GetACPathData()->Reset();
		}
	}

	if (!this->IsServerMove())
	{
		if (this->IsAdministrator())
		{
			uint16 world_id = WORLD_SILENT;

			int16 x = 0;
			int16 y = 0;

			if (World* pWorld = sWorldMgr->GetWorld(world_id))
			{
				pWorld->GetRespawn(world_id, x, y);
				this->SetWorldId(world_id);
				this->SetX(x);
				this->SetY(y);
				this->SetTX(x);
				this->SetTY(y);
				this->SetWalkSpeedCount(0);
				this->GetACPathData()->Reset();
				return true;
			}
			else
			{
				return false;
			}
		}

		if (this->GetWorldId() == WORLD_LAST_MAN_STANDING || this->GetWorldId() == WORLD_SPECIAL_EVENT_MAP)
		{
			uint16 world_id = WORLD_LOREN_MARKET;
			int16 x = 0;
			int16 y = 0;

			if (World* pWorld = sWorldMgr->GetWorld(world_id))
			{
				pWorld->GetRespawn(world_id, x, y);
				this->SetWorldId(world_id);
				this->SetX(x);
				this->SetY(y);
				this->SetTX(x);
				this->SetTY(y);
				this->SetWalkSpeedCount(0);
				this->GetACPathData()->Reset();
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < 6 || !this->GetWorld())
	{
		uint16 world_id = base_data->GetWorld();
		int16 x = 0;
		int16 y = 0;

		if (World* pWorld = sWorldMgr->GetWorld(world_id))
		{
			pWorld->GetRespawn(world_id, x, y);
			this->SetWorldId(world_id);
			this->SetX(x);
			this->SetY(y);
			this->SetTX(x);
			this->SetTY(y);
			this->SetWalkSpeedCount(0);
			this->GetACPathData()->Reset();
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Player::SendInventory(uint8 slot, uint8 flag, bool clear)
{
	if ( clear )
	{
		INVENTORY_LIST pMsg(slot, flag);
		this->SEND_PCT(pMsg);
	}
	else
	{
		INVENTORY_LIST_MODIFY pMsg(slot, nullptr);
		this->GetInventory()->GetItem(slot)->ConvertToBuffer(pMsg.item);
		this->SEND_PCT(pMsg);
	}

	this->CashShopSendExpireItem(slot);
}

void Player::SendEquipment(uint8 slot)
{
	auto pItem = this->GetInventory()->GetItem(slot);

	EQUIPMENT_SEND pMsg(this->GetEntry(), nullptr);
	pItem->ConvertToBuffer(pMsg.item);
	pMsg.item[1] = slot << 4;
	pMsg.item[1] |= sItemMgr->LevelSmallConvert(pItem->GetLevel()) & MAX_ITEM_LEVEL;
	pMsg.attribute = this->GetElementalAttribute();
	if (pItem->IsInventoryActive())
	{
		pMsg.item[6] = 1;
	}

	this->SEND_PCT_VP(pMsg);
}

void Player::SendItemDurability(uint8 slot, uint8 flag)
{
	ITEM_DURABILITY pMsg(slot, static_cast<uint8>(this->GetInventory()->GetItem(slot)->GetDurability()), flag);
	this->SEND_PCT(pMsg);
}

void Player::CharacterStatPointAdd(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	this->Ping();

	POINTER_PCT_LOG(STAT_POINT_ADD, lpMsg, Packet, 0);
	
	if ( lpMsg->stat_type >= MAX_STAT_TYPE )
	{
		return;
	}

	STAT_POINT_ADD_RESULT pMsg(0);

	if ( this->GetStat(lpMsg->stat_type) >= sGameServer->GetMaxStat(lpMsg->stat_type) )
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if ( lpMsg->stat_type == LEADERSHIP && this->GetClass() != Character::DARK_LORD )
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if ( this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints() )
	{
		this->IncreaseStat(lpMsg->stat_type, 1);
		this->GetLevelData(LEVEL_DATA_NORMAL)->ReducePoints(1);

		pMsg.result = 16 + lpMsg->stat_type;
		pMsg.MaxStamina = 0;
		pMsg.MaxShield = 0;
		pMsg.MaxLifeAndMana = 0;
		this->SEND_PCT(pMsg);
		
		this->CalculateCharacter();
	}
}

void Player::GameOptionUpdate(uint8 * Packet)
{
	if (this->IsGameOptionSaved())
	{
		return;
	}

	this->SetGameOptionSaved(true);

	POINTER_PCT_LOG(GAME_OPTION, lpMsg, Packet, 0);

	this->GetGameOption()->SetOption(lpMsg->game_option);
	this->GetGameOption()->SetChatWindow(lpMsg->chat_window);
	this->GetGameOption()->SetKeyQ(lpMsg->key_q);
	this->GetGameOption()->SetKeyW(lpMsg->key_w);
	this->GetGameOption()->SetKeyE(lpMsg->key_e);
	this->GetGameOption()->SetKeyR(lpMsg->key_r);
	this->GetGameOption()->SetQWERLevel(lpMsg->qwer_level);
	this->GetGameOption()->SetAdditionalOptions(lpMsg->additional_options);

	for (int32 i = 0; i < SKILL_KEY_COUNT; ++i)
	{
		this->GetGameOption()->SetSkillKey(i, MAKE_NUMBERW(lpMsg->skill_key[i * 2], lpMsg->skill_key[i * 2 + 1]));
	}

	for (int32 i = 0; i < BUTTON_KEY_COUNT; ++i)
	{
		this->GetGameOption()->SetButtonLink(i, lpMsg->linked_button[i]);
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	this->SaveDBGameOption(trans);

	MuDatabase.CommitTransaction(trans);
}

void Player::CharacterAction(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false, false) )
	{
		return;
	}

	POINTER_PCT(ACTION_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->h.get_size() > sizeof(ACTION_REQUEST) )
	{
		return;
	}

	this->SetDirection(lpMsg->dir);
	this->SetAction(lpMsg->action);

	switch ( lpMsg->action )
	{
	case OBJECT_ACTION_REST_1:
		{
			this->SetViewState(VIEW_STATE_ACTION_1);
			this->SetRest(lpMsg->action);
		} break;

	case OBJECT_ACTION_REST_2:
		{
			this->SetViewState(VIEW_STATE_ACTION_2);
			this->SetRest(lpMsg->action);
		} break;

	case OBJECT_ACTION_REST_3:
		{
			this->SetViewState(VIEW_STATE_ACTION_3);
			this->SetRest(lpMsg->action);
		} break;

	default:
	{
			   this->SetRest(0);
	} break;
	}

	this->ActionSend(lpMsg->GetTarget(), lpMsg->action, lpMsg->dir);
}

void Player::CharacterMove(uint8 * Packet)
{
	///- Checks básicos
	if ( !this->CommonMovementCheck() )
	{
		return;
	}
	///- Si sos Admin y no estás autorizado no podés caminar
	if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
	{
		this->CancelMovement();
		return;
	}

	World* pWorld = this->GetWorld();
	///- En caso de que esto ocurra no permitir caminar
	if ( !pWorld )
	{
		sLog->outError("antihack", "%s -- WRONG WORLD -- %s -- %s", __FUNCTION__, this->BuildLog().c_str(), this->BuildLocationLog().c_str());

		sGameServer->LogAntiHack(this, ANTIHACK_ID_WALK_WRONG_WORLD, "");

		this->CancelMovement();
		return;
	}
	
	

	POINTER_PCT(MOVE_REQUEST, lpMsg, Packet, 0);

	if (this->IsAdministrator())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "X: %d - Y: %d", lpMsg->x, lpMsg->y);
	}

	int32 count = lpMsg->path[0] & 0x0F;
	if (count <= 0)
	{
		return;
	}

	sMiningSystem->Restore(this);
	this->Ping();
	this->ClearPathData();
	this->LocationChange();

	this->SetRest(0);
	this->SetDirection(lpMsg->path[0] >> 4);
	this->GetPathData()->SetCount(count);
	///- Arriba lo limita a 15, pero por las dudas
	if ( this->GetPathData()->GetCount() > MAX_WALK_PATH )
	{
		if (!this->GetHelper()->IsStarted() || !this->GetHelper()->IsOffline())
		{
			sLog->outError("antihack", "%s -- PATH COUNT MAX -- %s -- %s", __FUNCTION__, this->BuildLog().c_str(), this->BuildLocationLog().c_str());
			sGameServer->LogAntiHack(this, ANTIHACK_ID_WALK_PATH_COUNT, "PATH COUNT MAX");
		}

		this->CancelMovement();
		return;
	}

	int16 start_x = lpMsg->x;
	int16 start_y = lpMsg->y;


	if (this->GetPathData()->GetCount() > 0 && !this->IsOffline() && (this->GetRegenStatus() == REGEN_NONE) && !this->IsTeleporting() && (sGameServer->GetWalkSpeedCount() > 0))
	{
		if (!this->GetACPathData()->IsLoaded())
		{
			this->GetACPathData()->SetLoaded(true);
			this->GetACPathData()->SetX(start_x);
			this->GetACPathData()->SetY(start_y);
		}
		else
		{
			auto walk_distance = Util::Distance(this->GetACPathData()->GetX(), this->GetACPathData()->GetY(), start_x, start_y);

			if (walk_distance > sGameServer->GetWalkSpeedDistance())
			{
				sLog->outError("antihack", "%s -- %s [%d / %d] [%d / %d] : Distance: %d - Count: %d", __FUNCTION__, this->BuildLog().c_str(),
					this->GetACPathData()->GetX(), this->GetACPathData()->GetY(), start_x, start_y, walk_distance, this->GetWalkSpeedCount());

				sGameServer->LogAntiHack(this, ANTIHACK_ID_WALK_SPEED, "WALK DISTANCE: [%d / %d] [%d / %d] : Distance: %d - Count: %d",
					this->GetACPathData()->GetX(), this->GetACPathData()->GetY(), start_x, start_y, walk_distance, this->GetWalkSpeedCount());

				this->IncreaseWalkSpeedCount(1);

				if (this->GetWalkSpeedCount() > sGameServer->GetWalkSpeedCount())
				{
					if (sGameServer->IsWalkSpeedBan())
					{
						this->SetAuthority(1);
					}

					if (sGameServer->IsWalkSpeedKick())
					{
						KICK_PLAYER(this, "Walk Speed");

						sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Walk Speed");
					}

					if (sGameServer->IsWalkSpeedFix())
					{
						this->CancelMovement();
						return;
					}
				}
			}
			else
			{
				this->SetWalkSpeedCount(0);
			}

			this->GetACPathData()->SetX(start_x);
			this->GetACPathData()->SetY(start_y);
		}
	}

	this->GetPathData()->GetPosition(0)->SetX(lpMsg->x);
	this->GetPathData()->GetPosition(0)->SetY(lpMsg->y);
	this->GetPathData()->GetPosition(0)->SetDir(this->GetDirection());
	this->GetPathData()->SetStartEnd(true);

	if ( this->GetPathData()->GetCount() > 0 )
	{
		this->GetPathData()->SetCurrent(1);
		this->GetPathData()->IncreaseCount(1);
	}

	int16 pathtable = 0;
	int16 ax = this->GetPathData()->GetPosition(0)->GetX();
	int16 ay = this->GetPathData()->GetPosition(0)->GetY();
	///- Genero el path
	for ( int32 n = 1; n < this->GetPathData()->GetCount(); ++n )
	{
		if( (n%2) == 1 )
		{
			pathtable = lpMsg->path[(n+1) / 2] >> 4;
		}
		else
		{
			pathtable = lpMsg->path[(n+1) / 2] & 0x0F;
		}

		if ( sGameServer->IsAntiHackWalkCheck5() && (pathtable < 0 || pathtable >= (Path::Max / 2)) )
		{
			if (!this->GetHelper()->IsStarted() || !this->GetHelper()->IsOffline())
			{
				sGameServer->LogAntiHack(this, ANTIHACK_ID_WALK_PATH_TABLE, "PATH TABLE %d", pathtable);
			}

			this->CancelMovement();
			return;
		}

		ax += Path::Table[pathtable*2];
		ay += Path::Table[pathtable*2+1];

		this->GetPathData()->GetPosition(n)->SetX(ax);
		this->GetPathData()->GetPosition(n)->SetY(ay);
		this->GetPathData()->GetPosition(n)->SetDir(pathtable);
		this->GetPathData()->GetPosition(n - 1)->SetDir(pathtable);
	}
	///- Este check habría que sacarlo o mejorarlo
	if (this->GetVehiculeType() == 0)
	{
		if (this->GetPathData()->GetCount() > 0)
		{
			WorldGrid const& attr = pWorld->GetGrid(this->GetPathData()->GetPosition(1)->GetX(), this->GetPathData()->GetPosition(1)->GetY());

			if (attr.IsLocked_1() || attr.IsLocked_2())
			{
				this->CancelMovement();
				return;
			}
		}
	}

	///- Esto no debería ocurrir ni siquiera con lag, solo en caso de Cheat o pérdida de Packets.
	if ( (sGameServer->GetWalkSpeedCount() > 0) && (this->GetRegenStatus() == REGEN_NONE) && !this->IsTeleporting() && !this->IsOffline())
	{
		auto walk_distance = Util::Distance(this->GetX(), this->GetY(), start_x, start_y);

		if (walk_distance > sGameServer->GetWalkSpeedDistance())
		{
			sLog->outError("antihack", "%s -- %s [%d / %d] [%d / %d] : Distance: %d - Count: %d", __FUNCTION__, this->BuildLog().c_str(),
				this->GetX(), this->GetY(), start_x, start_y, walk_distance, this->GetWalkSpeedCount());

			sGameServer->LogAntiHack(this, ANTIHACK_ID_WALK_SPEED, "WALK DISTANCE: [%d / %d] [%d / %d] : Distance: %d - Count: %d",
				this->GetX(), this->GetY(), start_x, start_y, walk_distance, this->GetWalkSpeedCount());

			this->IncreaseWalkSpeedCount(1);

			if ( this->GetWalkSpeedCount() > sGameServer->GetWalkSpeedCount() )
			{
				if ( sGameServer->IsWalkSpeedBan() )
				{
					this->SetAuthority(1);
				}

				if ( sGameServer->IsWalkSpeedKick() )
				{
					KICK_PLAYER(this, "Walk Speed");

					sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Walk Speed");
				}

				if ( sGameServer->IsWalkSpeedFix() )
				{
					this->CancelMovement();
					return;
				}
			}
		}
		else
		{
			this->SetWalkSpeedCount(0);
		}
	}

	if (this->GetVehiculeType() == 0)
	{
		///- Reviso la distancia entre cada coord.
		if (sGameServer->IsAntiHackWalkCheck1() && (this->GetRegenStatus() == REGEN_NONE) && !this->IsTeleporting())
		{
			int32 err_count = 0;

			for (int32 n = 0; n < this->GetPathData()->GetCount() - 1; ++n)
			{
				int32 distance = Util::Distance(this->GetPathData()->GetPosition(n)->GetX(),
					this->GetPathData()->GetPosition(n)->GetY(),
					this->GetPathData()->GetPosition(n + 1)->GetX(),
					this->GetPathData()->GetPosition(n + 1)->GetY());

				if (distance > sGameServer->GetAntiHackWalkCheckDistance1())
				{
					err_count++;
				}
			}

			if (err_count > sGameServer->GetAntiHackWalkCheckError1())
			{
				if (!this->GetHelper()->IsStarted() || !this->GetHelper()->IsOffline())
				{
					sGameServer->LogAntiHack(this, ANTIHACK_ID_WRONG_PATH, "Wrong Path %s", this->BuildPath().c_str());
				}

				this->CancelMovement();
				return;
			}
		}
	}

	if (this->GetVehiculeType() == 0)
	{
		///- Reviso que no haya ninguna zona bloqueada en el camino.
		if (sGameServer->IsAntiHackWalkCheck2())
		{
			for (int32 n = 0; n < this->GetPathData()->GetCount(); ++n)
			{
				WorldGrid const& attr = pWorld->GetGrid(this->GetPathData()->GetPosition(n)->GetX(), this->GetPathData()->GetPosition(n)->GetY());

				if (attr.IsLocked_1() || attr.IsLocked_2())
				{
					if (!this->GetHelper()->IsStarted() || !this->GetHelper()->IsOffline())
					{
						sGameServer->LogAntiHack(this, ANTIHACK_ID_WALK_PATH_BLOCKED, "Blocked Path %s", this->BuildPath().c_str());
					}

					this->CancelMovement();
					return;
				}
			}
		}
	}

	if (IF_MAP_RANGE(this->GetWorldId()) && sGameServer->IsImperialFortressWalkCheck())
	{
		ImperialFortressInstance * pInstance = sImperialFortressMgr->GetInstanceByCharacter(this->GetGUID());

		if (pInstance)
		{
			for (int32 n = 0; n < this->GetPathData()->GetCount(); ++n)
			{
				WorldGrid const& attr = pWorld->GetGrid(this->GetPathData()->GetPosition(n)->GetX(), this->GetPathData()->GetPosition(n)->GetY());

				if (attr.IsLocked_3() && !pInstance->IsZoneReleased(this->GetPathData()->GetPosition(n)->GetX(), this->GetPathData()->GetPosition(n)->GetY()))
				{
					this->CancelMovement();
					return;
				}
			}
		}
	}

	this->SetTX(ax);
	this->SetTY(ay);

	int16 nextX = this->GetX();
	int16 nextY = this->GetY();

	if ( this->GetWorldId() == WORLD_CASTLE_SIEGE )
	{
		///- Este es un check obsoleto.
		if ( sGameServer->IsAntiHackWalkCheck3() )
		{
			int32 result = 0;

			for( int32 i = 0; i < 256; ++i )
			{	
				if(nextX > this->GetTX()) { nextX -= 1; }
				if(nextX < this->GetTX()) { nextX += 1; }
				if(nextY > this->GetTY()) { nextY -= 1; }
				if(nextY < this->GetTY()) { nextY += 1; }

				WorldGrid const& attr = pWorld->GetGrid(nextX, nextY);

				if ( attr.IsLocked_3() )
				{
					result = 1;
					break;
				}

				if( this->GetTX() == nextX && this->GetTY() == nextY )
					break;
			}

			if ( result == 1 )
			{
				this->CancelMovement();
				return;
			}
		}
		///- Reviso que no haya ninguna zona bloqueada en el camino.
		if ( sGameServer->IsAntiHackWalkCheck4() )
		{
			for ( int32 n = 0; n < this->GetPathData()->GetCount(); ++n )
			{
				WorldGrid const& attr = pWorld->GetGrid(this->GetPathData()->GetPosition(n)->GetX(), this->GetPathData()->GetPosition(n)->GetY());

				if ( attr.IsLocked_3() )
				{
					this->CancelMovement();
					return;
				}
			}
		}
	}

	if ( sShopMgr->IsShop(this->GetInterfaceState()->GetID()) )
	{
		this->GetInterfaceState()->Reset();
	}

	if ( !this->GetHelper()->IsStarted() || !this->GetHelper()->IsOffline() )
	{
		MOVE_RESULT pMsg(this->GetEntry(), cast(uint8, ax), cast(uint8, ay), this->GetDirection()<<4);
		
		this->SEND_PCT(pMsg);
		this->SEND_PCT_VP(pMsg);
	}

	this->SetX(start_x);
	this->SetY(start_y);
	this->SetViewState(VIEW_STATE_NONE);
}

void Player::ItemMove(uint8 * Packet)
{
	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_MOVE) )
	{
		this->ItemMoveResult(uint8(-1));
		return;
	}

	this->Ping();

	POINTER_PCT_LOG(ITEM_MOVE, lpMsg, Packet, 0);

	if ( lpMsg->source >= max_move_item_flag || lpMsg->target >= max_move_item_flag )
	{
		this->ItemMoveResult(uint8(-1));
		return;
	}

	if ( lpMsg->source == lpMsg->target && lpMsg->source_slot == lpMsg->target_slot )
	{
		this->ItemMoveResult(-1);
		return;
	}

	if ( lpMsg->source == move_item_trade || lpMsg->target == move_item_trade )
	{
		if ( !Player::InterfaceCheck(this, InterfaceData::Trade, 1) )
		{
			this->ItemMoveResult(-1);
			return;
		}
	}

	//if ( lpMsg->source == move_item_warehouse || lpMsg->target == move_item_warehouse )
	if ( lpMsg->source == move_item_warehouse )
	{
		if ( !Player::InterfaceCheck(this, InterfaceData::Warehouse, 0) )
		{
			this->ItemMoveResult(-1);
			return;
		}

		if ( this->GetWarehouse()->IsLocked() )
		{
			this->ItemMoveResult(-1);
			return;
		}
	}

	if ( lpMsg->target == move_item_warehouse )
	{
		if ( !Player::InterfaceCheck(this, InterfaceData::Warehouse, 0) )
		{
			this->ItemMoveResult(-1);
			return;
		}
	}

	if ( lpMsg->source == move_item_personal_shop || lpMsg->target == move_item_personal_shop )
	{
		if ( this->GetPersonalStore()->IsBusy() )
		{
			this->ItemMoveResult(-1);
			return;
		}

		if ( this->GetInterfaceState()->GetID() != InterfaceData::None )
		{
			this->ItemMoveResult(-1);
			return;
		}
	}

	if ( lpMsg->source == 3 || (lpMsg->source >= 6 && lpMsg->source <= 20) || lpMsg->target == 3 || (lpMsg->target >= 6 && lpMsg->target <= 20) )
	{
		if ( !this->GetInterfaceState()->IsCommonMix() )
		{
			this->ItemMoveResult(-1);
			return;
		}

		if ( this->GetInterfaceState()->GetState() == 1 )
		{
			this->ItemMoveResult(-1);
			return;
		}
	}

	if ( lpMsg->source == move_item_pet_trainer || lpMsg->target == move_item_pet_trainer )
	{
		if ( !Player::InterfaceCheck(this, InterfaceData::PetTrainer, 0) )
		{
			this->ItemMoveResult(-1);
			return;
		}
	}

	uint8 result = -1;

	if ( lpMsg->target == move_item_inventory && lpMsg->source == move_item_inventory )
	{
		result = CallMoveItem(Inventory, Inventory);
	}
	else if ( lpMsg->target == move_item_trade && lpMsg->source == move_item_trade )
	{
		result = CallMoveItem(Trade, Trade);
	}
	else if ( lpMsg->target == move_item_trade && lpMsg->source == move_item_inventory )
	{
		result = CallMoveItem(Trade, Inventory);
	}
	else if ( lpMsg->target == move_item_inventory && lpMsg->source == move_item_trade )
	{
		result = CallMoveItem(Inventory, Trade);
	}
	else if ( lpMsg->target == move_item_warehouse && lpMsg->source == move_item_warehouse )
	{
		result = CallMoveItem(Warehouse, Warehouse);
	}
	else if ( lpMsg->target == move_item_warehouse && lpMsg->source == move_item_inventory )
	{
		result = CallMoveItem(Warehouse, Inventory);
	}
	else if ( lpMsg->target == move_item_inventory && lpMsg->source == move_item_warehouse )
	{
		result = CallMoveItem(Inventory, Warehouse);
	}
	else if ( (lpMsg->source == 3 || (lpMsg->source >= 5 && lpMsg->source <= 20)) && (lpMsg->target == 3 || (lpMsg->target >= 5 && lpMsg->target <= 20)) )
	{
		result = CallMoveItem(ChaosBox, ChaosBox);
	}
	else if ( (lpMsg->source == 3 || (lpMsg->source >= 5 && lpMsg->source <= 20)) && lpMsg->target == 0 ) // Chaos Box -> Inventory
	{
		result = CallMoveItem(Inventory, ChaosBox);
	}
	else if ( lpMsg->source == 0 && (lpMsg->target == 3 || (lpMsg->target >= 5 && lpMsg->target <= 20)) )
	{
		result = CallMoveItem(ChaosBox, Inventory);
	}
	else if ( lpMsg->source == 1 && lpMsg->target == move_item_event_inventory ) // Trade -> Event Inventory
	{
		result = CallMoveItem(EventInventory, Trade);
	}
	else if ( lpMsg->source == move_item_event_inventory && lpMsg->target == 1 ) // Event Inventory -> Trade
	{
		result = CallMoveItem(Trade, EventInventory);
	}
	else if ( lpMsg->source == move_item_event_inventory && lpMsg->target == move_item_event_inventory ) // Event Inventory -> Event Inventory
	{
		result = CallMoveItem(EventInventory, EventInventory);
	}
	else if ( lpMsg->source == move_item_muun_inventory && lpMsg->target == move_item_muun_inventory ) // Muun Inventory -> Muun Inventory
	{
		result = CallMoveItem(MuunInventory, MuunInventory);
	}
	else if ( (lpMsg->source == 3 || (lpMsg->source >= 5 && lpMsg->source <= 20)) && lpMsg->target == move_item_event_inventory ) // ChaosBox -> Event Inventory
	{
		result = CallMoveItem(EventInventory, ChaosBox);
	}
	else if ( lpMsg->source == move_item_event_inventory && (lpMsg->target == 3 || (lpMsg->target >= 5 && lpMsg->target <= 20)) ) // Event Inventory -> ChaosBox
	{
		result = CallMoveItem(ChaosBox, EventInventory);
	}
	else if ( lpMsg->source == 0 && lpMsg->target == move_item_event_inventory ) // Inventory -> Event Inventory
	{
		result = CallMoveItem(EventInventory, Inventory);
	}
	else if ( lpMsg->source == move_item_warehouse && lpMsg->target == move_item_event_inventory ) // Warehouse -> Event Inventory
	{
		result = CallMoveItem(EventInventory, Warehouse);
	}
	else if ( lpMsg->source == move_item_event_inventory && lpMsg->target == move_item_warehouse ) // Event Inventory -> Warehouse
	{
		result = CallMoveItem(Warehouse, EventInventory);
	}

	this->ItemMoveResult(result, lpMsg->target_slot, lpMsg->source_item_info);

	/*if( result == 0xFF && lpMsg->source == move_item_inventory && lpMsg->target == move_item_inventory )
	{
		if ( this->GetInventory()->GetItem(lpMsg->source_slot)->IsItem() )
		{
			this->SendItemDurability(lpMsg->source_slot, 0);
		}
	}*/

	if( lpMsg->target == move_item_inventory )
	{
		this->CashShopSendExpireItem(lpMsg->target_slot);
	}

	if ( lpMsg->target ==  move_item_muun_inventory )
	{
		sMuunSystem->GCMuunItemExpireSend(this, lpMsg->target_slot);
	}
}

void Player::ItemMoveResult(uint8 result, uint8 position, uint8 * iteminfo)
{
	ITEM_MOVE_RESULT pMsg(result,position,iteminfo);
	this->SEND_PCT(pMsg);
}

void Player::ItemDrop(uint8 * Packet, bool die)
{
	POINTER_PCT_LOG(ITEM_DROP, lpMsg, Packet, 0);

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_DROP) )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !die )
	{
		this->Ping();
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !die && !this->IsLive() )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !inventory_range(lpMsg->slot) )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( personal_store_range(lpMsg->slot) )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	Item const* pItem = this->GetInventory()->GetItem(lpMsg->slot);

	if ( !pItem->IsItem() )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( pItem->IsDBFlag(ITEM_DB_FLAG_DROP) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item can't be dropped.");
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !die && this->DuplicatedSerialFind(lpMsg->slot) )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !die && !this->GetInventory()->CanUseExpandedSlot(lpMsg->slot) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to drop items from this inventory.");
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	World* pWorld = this->GetWorld();

	if ( !sWorldMgr->IsItemDropAllowed(this, pWorld, lpMsg->px, lpMsg->py) )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !sItemMgr->IsItemFlag(pItem->GetItem(), pItem->GetLevel(), ITEM_FLAG_ALLOW_DROP) )
	{
		if ( !die )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item cannot be dropped.");
		}

		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( sPentagramSystem->IsPentagramItem(pItem->GetItem()) || pItem->GetMaxSocket() > 0 )
	{
		SOCKET_SLOT_LOOP(i)
		{
			if ( pItem->GetSocket(i) != SOCKET_SLOT_EMPTY &&
				 pItem->GetSocket(i) != SOCKET_SLOT_NONE )
			{
				this->ItemDropResult(0, lpMsg->slot);
				return;
			}
		}
	}

	if ( pItem->GetItem() == ITEMGET(14, 51) ||
		 pItem->GetItem() == ITEMGET(14, 63) ||
		 pItem->GetItem() == ITEMGET(14, 99) )
	{
		this->FireworksSend(3);
		this->ClearItem(lpMsg->slot);
		this->ItemDropResult(1, lpMsg->slot);
		return;
	}
	else if ( pItem->GetItem() == ITEMGET(13, 7) ) // Summon Contract
	{
		if ( this->CreateMercenary(pItem->GetLevel(), lpMsg->px, lpMsg->py) )
		{
			this->ClearItem(lpMsg->slot);
			this->ItemDropResult(1, lpMsg->slot);
		}
		else
		{
			this->ItemDropResult(0, lpMsg->slot);
		}

		return;
	}
	else if( sSummonScroll->CheckSummonScroll(pItem->GetItem()) )
	{
		if( !sSummonScroll->CreateSummonScrollMonster(this, pItem->GetItem(), this->GetWorldId(), lpMsg->px, lpMsg->py) )
		{
			this->ItemDropResult(0, lpMsg->slot);
		}
		else
		{
			this->ClearItem(lpMsg->slot);
			this->ItemDropResult(1, lpMsg->slot);
		}

		return;
	}
	else if ( pItem->GetItem() == ITEMGET(13,11) ) // Life Stone
	{
		if ( pItem->GetLevel() == 0 )
		{
			// Create Guardian
		}
		else if ( pItem->GetLevel() == 1 )
		{
			if ( this->CreateLifeStone() )
			{
				this->ClearItem(lpMsg->slot);
				this->ItemDropResult(1, lpMsg->slot);
				return;
			}
		}

		this->ItemDropResult(0, lpMsg->slot);
		return;
	}
	else if ( pItem->GetItem() == ITEMGET(14,28) ) // Kalima Normal Ticket
	{
		if ( this->CreateKalimaGate(pItem->GetLevel(), lpMsg->px, lpMsg->py) )
		{
			this->ClearItem(lpMsg->slot);
			this->ItemDropResult(1, lpMsg->slot);
		}
		else
		{
			this->ItemDropResult(0, lpMsg->slot);
		}

		return;
	}
	
	uint8 is_box_drop = sItemBagMgr->ExecuteItemBag(ITEM_BAG_ITEM, this, "", pItem->GetItem(), pItem->GetLevel(), lpMsg->px, lpMsg->py, pItem->GetSerialServer(), pItem->GetSerial(), pItem->GetName());

	if ( is_box_drop == ITEM_BAG_RESULT_SUCCESS )
	{
		this->ClearItem(lpMsg->slot);
		this->ItemDropResult(1, lpMsg->slot);
		return;
	}
	else if ( is_box_drop == ITEM_BAG_RESULT_LOW_LEVEL )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	is_box_drop = sItemBagMgr->ExecuteItemBag(ITEM_BAG_GROUP_ITEM, this, "", pItem->GetItem(), pItem->GetLevel(), lpMsg->px, lpMsg->py, pItem->GetSerialServer(), pItem->GetSerial(), pItem->GetName());

	if ( is_box_drop == ITEM_BAG_RESULT_SUCCESS )
	{
		this->ClearItem(lpMsg->slot);
		this->ItemDropResult(1, lpMsg->slot);
		return;
	}
	else if ( is_box_drop == ITEM_BAG_RESULT_LOW_LEVEL )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !die &&
		 (pItem->IsHarmonyUpgrade() || 
		 pItem->GetExe() || 
		 pItem->IsAncient() ||
		 pItem->Is380Option()) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item cannot be dropped.");
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( pItem->IsInventoryActive() )
	{
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( !die && (pItem->GetLevel() > 6 && pItem->GetItem() < ITEMGET(12, 0)) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item cannot be dropped.");
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( pItem->GetExpireDate() > 0 )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item cannot be dropped.");
		this->ItemDropResult(0, lpMsg->slot);
		return;
	}

	if ( pWorld->add_item(this->GetInventory()->item[lpMsg->slot], this, lpMsg->px, lpMsg->py) )
	{
		LOG_ITEM_DROP(this, this->GetInventory()->GetItem(lpMsg->slot), std::string(die ? "Killed by another player" : "Drag & Drop") + std::string(" - Location:") + std::to_string(lpMsg->px) + std::string("/") + std::to_string(lpMsg->py));

		this->GetInventory()->DeleteItem(lpMsg->slot);
		this->ItemDropResult(1, lpMsg->slot);
		this->WearedItemRemove(lpMsg->slot);
	}
	else
	{
		this->ItemDropResult(0, lpMsg->slot);
	}
}

void Player::ItemDropResult(uint8 result, uint8 slot)
{
	ITEM_DROP_RESULT pMsg(result,slot);
	this->sendPacket(MAKE_PCT(pMsg));
}

bool Player::ItemGet(uint8 * Packet, bool from_helper)
{
	if (!from_helper)
	{
		if (!this->PacketTimeCheck())
		{
			this->ItemGetResult();
			return false;
		}
	}

	if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_LOOT))
	{
		this->ItemGetResult();
		return false;
	}

	this->Ping();

	POINTER_PCT_LOG(ITEM_GET, lpMsg, Packet, 0);

	uint16 item_id = lpMsg->GetItem();

	World* pWorld = this->GetWorld();

	if (!pWorld)
	{
		this->ItemGetResult();
		return false;
	}

	WorldItem* world_item = pWorld->GetItem(item_id);

	if (!world_item)
	{
		this->ItemGetResult();
		return false;
	}

	if (!world_item->GiveCheck(this))
	{
		this->ItemGetResult();
		return false;
	}

	if (sItemMgr->GetItemInventoryType(world_item->item.GetItem()) != ITEM_INVENTORY_TYPE_NORMAL)
	{
		this->ItemGetResult();
		return false;
	}

	if (world_item->item.IsQuestItem() && world_item->GetOwner() != this)
	{
		this->ItemGetResult();
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(world_item->item.GetItem());

	if (!item_info)
	{
		this->ItemGetResult();
		return false;
	}

	if (item_info->GetMaxCount() != -1)
	{
		int32 count = this->GetInventory()->GetItemCount(world_item->item.GetItem(), world_item->item.GetLevel(), true);

		if (count >= item_info->GetMaxCount())
		{
			this->SendNotice(NOTICE_NORMAL_BLUE, "You can't get more %s", world_item->item.GetName());
			this->ItemGetResult();
			return false;
		}
	}

	if (item_info->IsQuest() && !this->CanLootQuestMUItem(world_item->item))
	{
		this->ItemGetResult();
		return false;
	}

	if (world_item->item.GetItem() == ITEMGET(14, 15))
	{
		world_item->Give();
		if (this->MoneyReachMaximum(world_item->item.zen.ammount))
		{
			this->MoneySet(sGameServer->GetMaxCharacterZen());
		}
		else
		{
			this->MoneyAdd(world_item->item.zen.ammount, false);
		}

		uint8 zen_info[12];
		zen_info[0] = SET_NUMBERH(SET_NUMBERHW(this->MoneyGet()));
		zen_info[1] = SET_NUMBERL(SET_NUMBERHW(this->MoneyGet()));
		zen_info[2] = SET_NUMBERH(SET_NUMBERLW(this->MoneyGet()));
		zen_info[3] = SET_NUMBERL(SET_NUMBERLW(this->MoneyGet()));
		this->ItemGetResult(uint8(-2), zen_info, world_item->GetEntry());
		return true;
	}
	else if (world_item->item.IsEventArchangelWeapon())
	{
		if (world_item->GetEventId() != EVENT_BLOOD_CASTLE)
		{
			this->ItemGetResult();
			return false;
		}

		if (world_item->GetEventId() != this->GetEventId() ||
			world_item->GetEventGround() != this->GetEventGround())
		{
			this->ItemGetResult();
			return false;
		}

		BloodCastle * pBloodCastle = sBloodCastleMgr->GetGround(world_item->GetEventGround());

		if (!pBloodCastle)
		{
			this->ItemGetResult();
			return false;
		}

		pBloodCastle->SetWeaponStatus(this);
		world_item->Give();
		return true;
	}

	if (this->ItemGetStack(world_item))
	{
		world_item->Give();
		this->ItemGetResult(uint8(-3), nullptr, world_item->GetEntry());
		return true;
	}

	uint8 result = 0xFF;
	bool expired_stack = false;

	if (sGameServer->IsStackExpirableItemsOnLoot() && world_item->item.GetExpireDate() != 0)
	{
		result = this->FindExpirableItem(world_item->item.GetItem(), world_item->item.GetLevel());

		if (result != 0xFF)
		{
			this->GetInventory()->GetItem(result)->SetExpireDate(this->GetInventory()->GetItem(result)->GetExpireDate() + (world_item->item.GetExpireDate() - time(nullptr)));
			this->GetInventory()->GetItem(result)->SetState(ITEM_CHANGED);
			expired_stack = true;
		}
	}

	if (result == 0xFF)
	{
		result = this->GetInventory()->AddItem(world_item->item);
	}

	if (result == 0xFF)
	{
		this->ItemGetResult();
		return false;
	}

	world_item->Give();
	uint8 buffer_info[MAX_ITEM_INFO];
	world_item->item.ConvertToBuffer(buffer_info);
	
	this->QuestMUObjectiveItemGet(world_item->item);
	LOG_ITEM_LOOT(this, &world_item->item, std::string("") + std::string(" - Location:") + std::to_string(world_item->GetX()) + std::string("/") + std::to_string(world_item->GetY()));

	if (!expired_stack)
	{
		this->ItemGetResult(result, buffer_info, world_item->GetEntry());
	}
	else
	{
		this->ItemGetResult();
	}

	this->PartyItemGetNotify(world_item);

	if ( this->GetInventory()->GetItem(result)->GetExpireDate() )
	{
		this->CashShopSendExpireItem(result);
	}

	return true;
}
	
void Player::ItemGetResult(uint8 result, uint8 * item_info, uint16 index)
{
	ITEM_GET_RESULT pMsg(result, item_info, index);
	this->sendPacket(MAKE_PCT(pMsg));
}

bool Player::ItemGetStack(WorldItem * pWorldItem)
{
	if ( !pWorldItem->item.IsItem() )
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(pWorldItem->item.GetItem());

	if ( !item_info )
	{
		return false;
	}

	if ( !item_info->GetStackData()->IsStackOnLoot() )
	{
		return false;
	}

	int32 MaxStack = item_info->GetStackData()->GetMaxStack();

	if ( MaxStack <= 0 )
	{
		return false;
	}

	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		if ( !this->GetInventory()->GetItem(i)->IsItem() )
		{
			continue;
		}

		if ( this->GetInventory()->GetItem(i)->GetItem() != pWorldItem->item.GetItem() ||
			 this->GetInventory()->GetItem(i)->GetLevel() != pWorldItem->item.GetLevel() ||
			 this->GetInventory()->GetItem(i)->GetSocketBonus() != pWorldItem->item.GetSocketBonus() )
		{
			continue;
		}

		if ( this->GetInventory()->GetItem(i)->GetDurability() < MaxStack )
		{
			int32 AddDur = (int32)(MaxStack - this->GetInventory()->GetItem(i)->GetDurability());

			AddDur = ((AddDur>pWorldItem->item.GetDurability())?(int32)pWorldItem->item.GetDurability():AddDur);

			pWorldItem->item.AlterDurability(0, AddDur);
			this->GetInventory()->GetItem(i)->AlterDurability(1, AddDur);

			this->QuestMUObjectiveItemGet(pWorldItem->item);
			
			if ( this->GetInventory()->GetItem(i)->GetDurability() == MaxStack && item_info->GetStackData()->GetConvertedItem() )
			{
				LOG_ITEM_LOOT(this, &pWorldItem->item, std::string("Stack item") + std::string(" - Location:") + std::to_string(pWorldItem->GetX()) + std::string("/") + std::to_string(pWorldItem->GetY()));

				this->ClearItem(i, 1);

				sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(item_info->GetStackData()->GetConvertedItem()->GetItem(), pWorldItem->item.GetLevel(), 1, 0, 0, 0, 0, 0, nullptr, pWorldItem->item.GetSocketBonus()));

				if( pWorldItem->item.GetDurability() < 1.0f )
				{
					return true;
				}
			}
			else
			{
				LOG_ITEM_LOOT(this, &pWorldItem->item, std::string("Stack item & convert") + std::string(" - Location:") + std::to_string(pWorldItem->GetX()) + std::string("/") + std::to_string(pWorldItem->GetY()));

				this->SendItemDurability(i, 0);

				if( pWorldItem->item.GetDurability() < 1.0f )
				{
					return true;
				}
			}
		}
	}

	return false;
}

void Player::ItemUse(uint8 * Packet)
{
	if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_USE))
	{
		this->ItemUseResult();
		return;
	}

	POINTER_PCT_LOG(USE_ITEM, lpMsg, Packet, 0);

	if (!use_inventory_range(lpMsg->source_slot))
	{
		return;
	}

	if (lpMsg->source_slot == lpMsg->target_slot)
	{
		return;
	}

	if (!this->GetInventory()->CanUseExpandedSlot(lpMsg->source_slot))
	{
		this->ItemUseResult();
		return;
	}

	Item * source_item = this->GetInventory()->GetItem(lpMsg->source_slot);
	
	if (!source_item->IsItem())
	{
		return;
	}

	Item * target_item = nullptr;

	if (source_item->GetExpireDate() != 0 && use_inventory_range(lpMsg->target_slot))
	{
		target_item = this->GetInventory()->GetItem(lpMsg->target_slot);
	}

	uint16 const item = source_item->GetItem();
	uint8 const level = source_item->GetLevel();
	uint32 use_delay = source_item->GetUseDelay();
	int64 use_date = time(nullptr);

	ItemDelayData & m_item_data = this->m_item_delay[item];

	if (use_delay > 0 && !this->IsAdministrator())
	{
		if (m_item_data.GetDate() > use_date)
		{
			this->SendNotice(NOTICE_NORMAL_BLUE, "Need to wait %s until you can use this item again.", secsToTimeString(m_item_data.GetDate() - use_date).c_str());
			this->ItemUseResult();
			return;
		}
	}

	if (source_item->GetExpireDate() != 0)
	{
		if (target_item && target_item->IsItem() && target_item->GetItem() == source_item->GetItem())
		{
			target_item->SetExpireDate(target_item->GetExpireDate() + source_item->GetRemainTime());

			this->ClearItem(lpMsg->source_slot);
			this->SendInventory(lpMsg->target_slot);
			this->CashShopSendExpireItem(lpMsg->target_slot);
			return;
		}
	}

	if (this->PotionUse(item, level, source_item->GetValue()))
	{
		this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
	}
	else if (this->ExperiencePotionUse(item, level))
	{
		this->ClearItem(lpMsg->source_slot);
	}
	else if (item == ITEMGET(14, 7)) // Siege Potion
	{
		if (level == 0)
		{
			this->AddBuff(BUFF_POTION_OF_BLESS, 120, 0, this);
		}
		else if (level == 1)
		{
			this->AddBuff(BUFF_POTION_OF_SOUL,
				BuffEffect(BUFF_OPTION_INCREASE_ATTACK_SPEED, 20),
				BuffEffect(BUFF_OPTION_INCREASE_AG_RECOVERY_ADD, 8), 30, 0, this);

			//this->SpecialItemUseSend(2, 30);
		}

		this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
	}
	else if (item == ITEMGET(14, 8)) // Antidote
	{
		this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);

		this->RemoveBuff(BUFF_POISON);
		this->RemoveBuff(BUFF_ICE);

		//this->UpdateStatistic(STATISTIC_ANTIDOTE_POTIONS, 1);
	}
	else if (item == ITEMGET(14, 9)) // Ale
	{
		if (source_item->GetLevel() == 0)
		{
			this->SetAleTime(ALE_SPEED, GetTickCount() + 60000);
			this->SetAleEffect(ALE_SPEED, 20);
			this->SetAleItem(ALE_SPEED, item);
			this->SetAleLevel(ALE_SPEED, source_item->GetLevel());
			//this->SpecialItemUseSend(ALE_SPEED, 60);
		}
		else if (source_item->GetLevel() == 1)
		{
			this->SetAleTime(ALE_SPEED, GetTickCount() + 180000);
			this->SetAleEffect(ALE_SPEED, 20);
			this->SetAleItem(ALE_SPEED, item);
			this->SetAleLevel(ALE_SPEED, source_item->GetLevel());
			//this->SpecialItemUseSend(ALE_SPEED, 180);
		}
		else if (source_item->GetLevel() == 2)
		{
			this->SetAleTime(ALE_DAMAGE, GetTickCount() + 60000);
			this->SetAleEffect(ALE_DAMAGE, 15);
			this->SetAleItem(ALE_DAMAGE, item);
			this->SetAleLevel(ALE_DAMAGE, source_item->GetLevel());
			//this->SpecialItemUseSend(ALE_DAMAGE, 60);
		}

		this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
		this->CalculateCharacter();
	}
	else if (item == ITEMGET(14, 20)) // Remedy of Love
	{
		if (source_item->GetLevel() == 0)
		{
			this->SetAleTime(ALE_DAMAGE, GetTickCount() + 60000);
			this->SetAleEffect(ALE_DAMAGE, 15);
			//this->SpecialItemUseSend(ALE_DAMAGE, 60);
			this->SetAleItem(ALE_DAMAGE, item);
			this->SetAleLevel(ALE_DAMAGE, source_item->GetLevel());
			this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
			this->CalculateCharacter();
		}
	}
	else if (item == ITEMGET(13, 15)) // Fruits
	{
		if (lpMsg->use_type == 0)
		{
			this->UsePlusStatFruit(lpMsg->source_slot);
		}
		else if (lpMsg->use_type == 1)
		{
			this->UseMinusStatFruit(lpMsg->source_slot);
		}
	}
	else if (item == ITEMGET(13, 54) ||
		item == ITEMGET(13, 55) ||
		item == ITEMGET(13, 56) ||
		item == ITEMGET(13, 57) ||
		item == ITEMGET(13, 58))
	{
		this->UseCashShopFruit(lpMsg->source_slot);
	}
	else if (item == ITEMGET(13, 60)) // Indulgence
	{
		if (this->UseIndulgence())
		{
			this->ClearItem(lpMsg->source_slot);
		}
	}
	else if (item == ITEMGET(14, 10)) // Town portal Scroll
	{
		this->TownPortalScrollUse(lpMsg->source_slot);
	}
	else if (item == JEWEL::BLESS) // Jewel of Bless
	{
		if (this->UpgradeItemUsingBless(lpMsg->source_slot, lpMsg->target_slot))
		{
			//this->UpdateStatistic(STATISTIC_USED_JEWEL_OF_BLESS, 1);
		}
	}
	else if (item == JEWEL::SOUL) // Jewel of Soul
	{
		if (this->UpgradeItemUsingSoul(lpMsg->source_slot, lpMsg->target_slot))
		{
			//this->UpdateStatistic(STATISTIC_USED_JEWEL_OF_SOUL, 1);
		}
	}
	else if (item == JEWEL::LIFE) // Jewel of Life
	{
		if (this->UpgradeItemUsingLife(lpMsg->source_slot, lpMsg->target_slot))
		{
			//this->UpdateStatistic(STATISTIC_USED_JEWEL_OF_LIFE, 1);
		}
	}
	else if (item == JEWEL::HARMONY) // Jewel of Harmony
	{
		if (this->UpgradeItemUsingHarmony(lpMsg->source_slot, lpMsg->target_slot))
		{
			//this->UpdateStatistic(STATISTIC_USED_JEWEL_OF_HARMONY, 1);
		}
	}
	else if (item == JEWEL::LOW_REFINE_STONE || // Lower Refine Stone
		item == JEWEL::HIGH_REFINE_STONE)  // Higher Refine Stone
	{
		if (this->UpgradeItemUsingRefineStone(lpMsg->source_slot, lpMsg->target_slot))
		{
			//this->UpdateStatistic(STATISTIC_USED_REFINE_STONE, 1);
		}
	}
	else if (item == JEWEL::EXTENSION)
	{
		if (this->UpgradeItemUsingExtension(lpMsg->source_slot, lpMsg->target_slot))
		{
			//this->UpdateStatistic(STATISTIC_USED_JEWEL_OF_EXTENSION, 1);
		}
	}
	else if (item == JEWEL::ELEVATION)
	{
		if (this->UpgradeItemUsingElevation(lpMsg->source_slot, lpMsg->target_slot))
		{
			//this->UpdateStatistic(STATISTIC_USED_JEWEL_OF_ELEVATION, 1);
		}
	}
	else if (item == ITEMGET(14, 209))
	{
		this->UpgradeItemUsingTradeableSeal(lpMsg->source_slot, lpMsg->target_slot);
	}
	else if (item == ITEMGET(14, 345) || item == ITEMGET(14, 420))
	{
		this->UpgradeItemUsingMysteriousStone(lpMsg->source_slot, lpMsg->target_slot);
	}
	else if (item == CREDIT_COIN)
	{
		if (level >= CREDIT_COIN_LEVEL_MIN && level <= CREDIT_COIN_LEVEL_MAX)
		{
			if (!sGameServer->SerialCheck(this, this->GetInventory()->GetItem(lpMsg->source_slot)->GetSerialServer(), this->GetInventory()->GetItem(lpMsg->source_slot)->GetSerial(), this->GetInventory()->GetItem(lpMsg->source_slot)->GetName(), 1))
			{
				if (sGameServer->IsAntiDupeBoxRemove())
				{
					this->ClearItem(lpMsg->source_slot);
				}
			}
			else
			{
				this->ClearItem(lpMsg->source_slot);
				this->CashShopCreditCoinUseRequest(level);
				//this->UpdateStatistic(STATISTIC_USED_CREDIT_COIN, 1);
			}
		}
		return;
	}
	/*else if ( item == PERSONAL_MERCHANT_ITEM )
	{
	if ( this->SummonPersonalMerchant() )
	{
	this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
	}
	}
	else if ( item == PERSONAL_WAREHOUSE_ITEM )
	{
	if ( this->SummonPersonalWarehouse() )
	{
	this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
	}
	}*/
	else if (item == ITEMGET(13, 66))
	{
		if (sGameServer->IsSantaVillageEnabled() && (sEventMgr->IsSeasonEventOn(sGameServer->GetSantaVillageSeason()) || !sGameServer->IsSantaVillageCheckSeason()))
		{
			this->TeleportToLocation(WORLD_SANTA_VILLAGE, 220, 20, this->GetDirection(), this->GetInstance());
			this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
		}
		else
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Santa Village is not avaliable.");
		}
	}
	else if (item == ITEMGET(13, 69))
	{
		if (this->CanEnterWorld(this->GetTalismanResurrection()->GetWorldId(), this->GetTalismanResurrection()->GetX(), this->GetTalismanResurrection()->GetY(), this->GetTalismanResurrection()->GetX(), this->GetTalismanResurrection()->GetY(), true))
		{
			if (this->GetTalismanResurrection()->IsWorldFlag(WORLD_FLAG_ALLOW_TALISMAN_MOVE))
			{
				this->ClearItem(lpMsg->source_slot);
				this->TeleportToLocation(this->GetTalismanResurrection()->GetWorldId(),
					this->GetTalismanResurrection()->GetX(), this->GetTalismanResurrection()->GetY(),
					this->GetTalismanResurrection()->GetDirection(), this->GetTalismanResurrection()->GetInstance());
				this->GetTalismanResurrection()->Reset();
				this->CommandSend(60, 0, 0);
			}
		}
	}
	else if (item == ITEMGET(13, 70))
	{
		if (this->GetInventory()->item[lpMsg->source_slot].GetDurability() > 1.0f)
		{
			if (this->IsWorldFlag(WORLD_FLAG_ALLOW_TALISMAN_MOVE))
			{
				this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
				this->GetTalismanMobility()->Set(this->GetLocation());
			}
			else
			{
				this->CommandSend(61, 0, 0);
			}
		}
		else
		{
			if (this->CanEnterWorld(this->GetTalismanMobility()->GetWorldId(), this->GetTalismanMobility()->GetX(), this->GetTalismanMobility()->GetY(), this->GetTalismanMobility()->GetX(), this->GetTalismanMobility()->GetY(), true))
			{
				this->ClearItem(lpMsg->source_slot);
				this->TeleportToLocation(this->GetTalismanMobility()->GetWorldId(),
					this->GetTalismanMobility()->GetX(), this->GetTalismanMobility()->GetY(),
					this->GetTalismanMobility()->GetDirection(), this->GetTalismanMobility()->GetInstance());
				this->GetTalismanMobility()->Reset();
			}

			this->CommandSend(61, 0, 0);
		}
	}
	else if (item == ITEMGET(13, 48)) // Kalima Special Ticket
	{
		uint8 level_entrance = this->GetKalimaLevelEntrance();

		if (level_entrance != uint8(-1))
		{
			level_entrance++;

			int16 tmp_x = this->GetX() + RANDOM(6) - 2;
			int16 tmp_y = this->GetY() + RANDOM(6) - 2;

			FIX_COORD(tmp_x);
			FIX_COORD(tmp_y);

			if (this->CreateKalimaGate(level_entrance, tmp_x, tmp_y))
			{
				this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1);
			}
		}
	}
	else if (item == ITEMGET(14, 121) ||
		item == ITEMGET(14, 122))
	{
		this->OpenSealedBoxAttempt(lpMsg->source_slot);
	}
	else if (item == ITEMGET(12, 446) ||
		item == ITEMGET(12, 447) ||
		item == ITEMGET(12, 448))
	{
		if (this->FillMagicSolution(lpMsg->source_slot, lpMsg->target_slot))
		{
			return;
		}
	}
	else if (item == ITEMGET(13, 509))
	{
		if (this->MajesticTreeReset())
		{
			this->ClearItem(lpMsg->source_slot);
		}
	}
	else if (item == ITEMGET(13, 152) ||
			 item == ITEMGET(13, 156))
	{
		if (this->MasterSkillReset(0))
		{
			this->ClearItem(lpMsg->source_slot);
		}
	}
	else if (item == ITEMGET(13, 153) ||
			 item == ITEMGET(13, 157))
	{
		if (this->MasterSkillReset(1))
		{
			this->ClearItem(lpMsg->source_slot);
		}
	}
	else if (item == ITEMGET(13, 154) ||
			 item == ITEMGET(13, 158))
	{
		if (this->MasterSkillReset(2))
		{
			this->ClearItem(lpMsg->source_slot);
		}
	}
	else if (item == ITEMGET(13, 155) ||
			 item == ITEMGET(13, 159))
	{
		if (level == 1 && this->MajesticTreeReset())
		{
			this->ClearItem(lpMsg->source_slot);
		}
		else if (level == 0 && this->MasterSkillReset(-1))
		{
			this->ClearItem(lpMsg->source_slot);
		}
	}
	else if (sMonsterSoul->SoulConverterUse(this, item, lpMsg->source_slot, lpMsg->use_type))
	{
		
	}
	else if (skill_template const* skill_info = sSkillMgr->GetSkillFromItem(item, level))
	{
		if (this->HasSkillRequiredStat(skill_info))
		{
			if (!this->MagicFindSkillTree(skill_info->GetID()))
			{
				int32 result = this->MagicAdd(skill_info->GetID(), level);

				if (result != -1)
				{
					this->SendSkill(result, this->GetSkill(result)->GetSkill(), this->GetSkill(result)->GetLevel());
					this->ClearItem(lpMsg->source_slot);
				}
			}
		}
	}
	else if (this->AddItemBuff(item, 0, BUFF_FLAG_DB_STORE, this, true, true))
	{
		this->DecreaseItemDurabilityByUse(lpMsg->source_slot, 1.0f);
	}

	if (use_delay > 0)
	{
		m_item_data.SetDate(use_date + static_cast<time_t>(use_delay));
	}

	this->ItemUseResult();
}

void Player::ItemUseResult()
{
	USE_ITEM_RESULT pMsg;
	this->sendPacket(MAKE_PCT(pMsg));
}

bool Player::DuplicatedSerialFind(uint8 slot)
{
	bool serial_duplicated = false;
	
	uint32 serial = this->GetInventory()->item[slot].GetSerial();
	uint16 code = this->GetInventory()->item[slot].GetSerialServer();

	full_inventory_loop(i)
	{
		if ( i == slot )
			continue;

		if ( !this->GetInventory()->GetItem(i)->IsItem() )
			continue;

		if ( serial != this->GetInventory()->GetItem(i)->GetSerial() )
			continue;

		if ( code != this->GetInventory()->GetItem(i)->GetSerialServer() )
			continue;

		serial_duplicated = true;

		this->ClearItem(i);
	}

	if ( serial_duplicated )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duplicated serial has been founded.");
	}

	return serial_duplicated;
}

void Player::TalkToNpc(uint8 * Packet)
{
	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_NPC_INTERACTION) )
	{
		return;
	}

	POINTER_PCT_LOG(TALK_TO_NPC, lpMsg, Packet, 0);

	Monster * Npc = sObjectMgr->FindMonster(lpMsg->GetIndex());

	if ( !Npc || !Npc->IsNpc() || !Npc->IsPlaying() || !Npc->IsLive() )
		return;

	if ( !IN_RANGE(this, Npc, NPC_INTERACT_MAX_DISTANCE) )
		return;

	if ( !this->ViewportExist(Npc) )
		return;

	if ( Npc->HasBuff(BUFF_INVISIBILITY) )
		return;

	if ( !this->CanInteract(Npc, true) )
		return;

	NpcHandler(this, Npc).OnTalk();
}

void Player::CloseInterface()
{
	if ( !this->Object::IsPlaying() )
	{
		return;
	}

	if ( this->GetInterfaceState()->IsCommonMix() )
	{
		return;
	}

	if ( this->GetInterfaceState()->GetID() == InterfaceData::None )
	{
		return;
	}

	if ( this->GetInterfaceState()->GetID() == InterfaceData::Trade ||
		 this->GetInterfaceState()->GetID() == InterfaceData::PersonalStoreOpen )
	{
		return;
	}

	sLog->outInfo(LOG_PLAYER, "Closing interface (%s - %d) -- %s",
		this->GetInterfaceState()->GetID().c_str(), this->GetInterfaceState()->GetState(), this->BuildLog().c_str());

	if ( this->GetInterfaceState()->GetID() == InterfaceData::Warehouse )
	{
		this->WarehouseClose();
		return;
	}
	else if ( this->GetInterfaceState()->GetID() == InterfaceData::PetTrainer )
	{
		this->GetMixInventory()->Clear();
	}
	else if ( sShopMgr->IsShop(this->GetInterfaceState()->GetID()) )
	{

	}
	
	this->GetInterfaceState()->Reset();
}

void Player::ItemBuy(uint8 * Packet)
{
	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_BUY_NPC) )
	{
		this->ItemBuyResult();
		return;
	}

	this->Ping();

	POINTER_PCT_LOG(ITEM_BUY, lpMsg, Packet, 0);

	if ( !shop_range(lpMsg->slot) )
	{
		this->ItemBuyResult();
		return;
	}

	Monster* mNpc = this->GetInterfaceState()->GetTarget()->ToCreature();

	if ( !mNpc || !mNpc->IsPlaying() )
	{
		this->ItemBuyResult();
		return;
	}

	if  ( !this->SameDimension(mNpc) )
	{
		this->ItemBuyResult();
		return;
	}

	if (this->GetInterfaceState()->GetID() == InterfaceData::MixRecovery)
	{
		this->AttemptRecoveryMixItem(lpMsg->slot);
		return;
	}

	if (this->GetInterfaceState()->GetID() != mNpc->GetNpcFunction())
	{
		this->ItemBuyResult();
		return;
	}

	Shop const* shop = sShopMgr->GetShop(mNpc->GetNpcFunction());

	if ( !shop )
	{
		this->ItemBuyResult();
		return;
	}

	if ( shop->IsFlag(SHOP_FLAG_ADMINISTRATOR) && !this->IsAdministrator() )
	{
		this->ItemBuyResult();
		return;
	}

	if ( shop->IsFlag(SHOP_FLAG_GUILD_MASTER) && this->GuildGetRanking() != GUILD_RANK_MASTER )
	{
		this->ItemBuyResult();
		return;
	}

	if ( shop->IsFlag(SHOP_FLAG_GUILD_BATTLE) && this->GuildGetRanking() != GUILD_RANK_BATTLE_MASTER )
	{
		this->ItemBuyResult();
		return;
	}

	if ( shop->IsFlag(SHOP_FLAG_GUILD_ASSISTANT) && this->GuildGetRanking() != GUILD_RANK_ASISTANT )
	{
		this->ItemBuyResult();
		return;
	}

	if ( shop->GetItem(lpMsg->slot)->position.Is(uint8(-1)) )
	{
		this->ItemBuyResult();
		return;
	}

	if ( shop->GetType() == SHOP_TYPE_RUUD )
	{
		this->ItemBuyResult();
		return;
	}

	if ( !sShopMgr->EnableToBuy(this, shop) )
	{
		this->ItemBuyResult();
		return;
	}

	uint32 tax_rate = 0;
	uint32 total_price = shop->GetItem(lpMsg->slot)->GetPrice();

	if ( !total_price )
	{
		total_price = shop->GetItem(lpMsg->slot)->GetBuyPrice();
	}

	if ( shop->IsFlag(SHOP_FLAG_SIEGE_TAX) )
	{
		tax_rate = total_price * sCastleSiege->GetTaxRateStore(this) / 100;
	}

	total_price += tax_rate;

	switch ( shop->GetType() )
	{
	case SHOP_TYPE_ZEN:
		{
			if ( !this->MoneyHave(total_price) )
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_RUUD:
	case SHOP_TYPE_RUUD_MISC:
		{
			if ( this->GetRuudMoney() < total_price )
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_WCOIN:
		{
			if ( this->GetCredits() < total_price )
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_GOBLIN_POINTS:
		{
			if ( this->GetGoblinPoints() < total_price )
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_RARE_TICKET:
		{
			if ( (this->GetInventory()->GetItemCount(ITEMGET(14, 58), 0) < total_price) )
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_JEWEL_OF_BLESS:
		{
			if (this->GetInventory()->GetItemCount(JEWEL::BLESS, -1) < total_price)
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_JEWEL_OF_SOUL:
		{
			if (this->GetInventory()->GetItemCount(JEWEL::SOUL, -1) < total_price)
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_JEWEL_OF_CHAOS:
		{
			if (this->GetInventory()->GetItemCount(JEWEL::CHAOS, -1) < total_price)
			{
				this->ItemBuyResult();
				return;
			}
		} break;
	}
	
	if ( mNpc->GetAI() && mNpc->GetAI()->OnItemBuy(this, lpMsg->slot) )
	{
		this->ItemBuyResult();
		return;
	}

	switch ( sItemMgr->GetItemInventoryType(shop->GetItem(lpMsg->slot)->GetItem()) )
	{
	case ITEM_INVENTORY_TYPE_NORMAL:
		{
			if ( !this->GetInventory()->IsEmptySpace(shop->GetItem(lpMsg->slot)->GetItem()) )
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case ITEM_INVENTORY_TYPE_EVENT:
		{
			if ( !this->GetEventInventory()->IsEmptySpace(shop->GetItem(lpMsg->slot)->GetItem()) )
			{
				this->ItemBuyResult();
				return;
			}
		} break;

	case ITEM_INVENTORY_TYPE_MUUN:
		{
			if ( this->GetMuunInventory()->GetEmptySlot() == uint8(-1) )
			{
				this->ItemBuyResult();
				return;
			}
		} break;
	}
	
	sItemMgr->ItemSerialCreateShop(this, shop->GetID(), lpMsg->slot);
}

void Player::ItemBuyResult(uint8 result, uint8 * item_info)
{
	ITEM_BUY_RESULT pMsg(result,item_info);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::ItemSell(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ITEM_SELL_NPC) )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	this->Ping();

	POINTER_PCT_LOG(ITEM_SELL, lpMsg, Packet, 0);
	
	if ( !sell_inventory_range(lpMsg->slot) )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(lpMsg->slot) )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	Item * pItem = this->GetInventory()->GetItem(lpMsg->slot);

	if ( !pItem || !pItem->IsItem() )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	if ( pItem->IsDBFlag(ITEM_DB_FLAG_SELL) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item can't be sold.");
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	if ( this->DuplicatedSerialFind(lpMsg->slot) )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	if ( pItem->IsHarmonyUpgrade() ||
		 pItem->GetExpireDate() > 0 )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(pItem->GetItem());

	if ( !item_info )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}
	
	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_SELL) )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	if ( sPentagramSystem->IsPentagramItem(pItem->GetItem()) )
	{
		SOCKET_SLOT_LOOP(i)
		{
			if ( pItem->GetSocket(i) != SOCKET_SLOT_EMPTY &&
				 pItem->GetSocket(i) != SOCKET_SLOT_NONE )
			{
				this->ItemSellResult(0, this->MoneyGet());
				return;
			}
		}
	}

	uint32 sell_price = pItem->GetSellPrice();

	if ( this->MoneyReachMaximum(sell_price) )
	{
		this->ItemSellResult(0, this->MoneyGet());
		return;
	}

	if ( sGameServer->IsDebug() && this->IsAdministrator() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Sold item [%s]: %u", pItem->GetName(), sell_price);
	}

	Unit* pTarget = GetInterfaceState()->GetTarget();

	LOG_ITEM_SELL(this, pItem, "Sold item on NPC " + std::string(pTarget ? pTarget->GetName(): "") + " - Price " + std::to_string(long long(sell_price)));

	if (pItem->IsPentagramItem())
	{
		sPentagramSystem->DelAllPentagramJewelInfo(this, pItem, PENTAGRAM_JEWEL_TYPE_INVENTORY);
	}

	this->AddItemToSoldList(*pItem, sell_price);

	this->GetInventory()->DeleteItem(lpMsg->slot);
	this->MoneyAdd(sell_price, false);
	this->ItemSellResult(1, this->MoneyGet());
	this->WearedItemRemove(lpMsg->slot);
}

void Player::ItemSellResult(uint8 result, uint32 zen)
{
	ITEM_SELL_RESULT pMsg(result,zen);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::SendNotice(NoticeType type, const char * notice, ...)
{
	ARG(Buffer, notice);

	PMSG_NOTICE pMsg;
	pMsg.type = uint8(type);
	memcpy(pMsg.Notice, Buffer, strlen(Buffer));
	pMsg.h.size += strlen(Buffer);

	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::SendNotice(const char * name, const char * notice, ...)
{
	ARG(Buffer, notice);

	PACKET_CHAT_DATA pMsg(name, Buffer);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::SendNoticeNormal(NoticeType type, const char * notice)
{
	PMSG_NOTICE pMsg;
	pMsg.type = uint8(type);
	memcpy(pMsg.Notice, notice, strlen(notice));
	pMsg.h.size += strlen(notice);

	this->sendPacket(MAKE_PCT(pMsg));
}
	
void Player::SendNoticeNormal(const char * name, const char * notice)
{
	PACKET_CHAT_DATA pMsg(name, notice);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::SendNotice(CustomMessageID type, const char * notice, ...)
{
	ARG(Buffer, notice);

	CUSTOM_MESSAGE pMsg(type, Buffer);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::SendNoticeNormal(CustomMessageID type, const char * notice)
{
	CUSTOM_MESSAGE pMsg(type, notice);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::protocol_core(uint8 * packet_buffer, uint16 size, uint8 headcode)
{
	TRY_START
		if ( size > sizeof(FRIEND_MAIL) )
		{
			this->CloseSocket();
			return;
		}

		uint8 Packet[4096];
		memcpy(Packet, packet_buffer, size);

		if ( this->IsAdministrator() && sGameServer->IsDebug() )
		{
			sLog->outPacket("root", Packet, size, "ProtocolCore: ");
		}

		switch ( headcode )
		{
		case HEADCODE_ACCOUNT_DATA:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch ( subcode )
				{
				case SUBCODE_ACCOUNT_DATA_LOGIN:
					{
						this->LoginRequest(Packet);
					} break;

				case SUBCODE_ACCOUNT_DATA_LOGOUT:
					{
						this->LogoutRequest(Packet);
					} break;
				}
			} break;

		case HEADCODE_MISC_CHARACTER_DATA:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch ( subcode )
				{
				case SUBCODE_MISC_CHARACTER_DATA_LIST: { this->CharacterListRequest(); } break;
				case SUBCODE_MISC_CHARACTER_DATA_CREATE: { this->CharacterCreate(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_DELETE: { this->CharacterDelete(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_JOIN: { this->CharacterSelect(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_POINT_ADD: { this->CharacterStatPointAdd(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_ITEM: { } break;
				case SUBCODE_MISC_CHARACTER_DATA_LOAD_CONFIRM: { this->DataLoadingConfirm(); } break;
				case SUBCODE_MISC_CHARACTER_DATA_NAME_CHECK: { this->CharacterNameCheck(Packet); } break;
				case 0x21: { this->ChangeSkinOption(Packet); } break;
				case 0x26: { } break;
				case SUBCODE_MISC_CHARACTER_DATA_GAME_OPTION: { this->GameOptionUpdate(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_MASTER_SKILL_POINT_ADD: { this->MasterSkillPointAdd(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_SWITCH_SCROLL: { this->SwitchModeTurnOn(Packet); } break;
				//case SUBCODE_MISC_CHARACTER_DATA_RIGHT_CLICK_MOVE_ITEM: { this->RightClickMoveItemRequest(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_HACK_REPORT: { this->HackReport(Packet); } break;

				case SUBCODE_MISC_CHARACTER_DATA_ATTACK_SPEED: { this->AttackSpeedRecv(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_HACK_CHECK_PING: { this->HackCheckVerify(Packet, 0); } break;
				case SUBCODE_MISC_CHARACTER_DATA_HACK_CHECK_BYTE_MODIFIED: { this->HackCheckVerify(Packet, 1); } break;
				case SUBCODE_MISC_CHARACTER_DATA_HACK_CHECK_DOUBLE_MODIFIED: { this->HackCheckVerify(Packet, 2); } break;
				case SUBCODE_MISC_CHARACTER_DATA_HACK_CHECK_HOOKED_FUNCTION: { this->HackCheckVerify(Packet, 3); } break;
				case SUBCODE_MISC_CHARACTER_DATA_HACK_CHECK_FRAME_COUNT: { this->HackCheckVerify(Packet, 4); } break;
				case SUBCODE_MISC_CHARACTER_DATA_HACK_CHECK_LOOP_PING: { this->HackCheckVerify(Packet, 5); } break;

				case SUBCODE_MISC_CHARACTER_DATA_SCAN_PROCESS: { sGameServer->ProcessTrack(this, Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_CHEAT_SCAN_DETECT: { this->CheatScanDetect(Packet); } break;
				case SUBCODE_MISC_CHARACTER_DATA_MINIMIZE: { this->ClientMinimize(Packet); } break;

				case SUBCODE_MISC_CHARACTER_DATA_ANTI_MACRO: { this->AntiMacroDetect(Packet); } break;

				default: { sLog->outPacket("antihack", Packet, size, "CLIENT TO SERVER WRONG: %s", this->BuildLog().c_str()); } break;
				}
			} break;

		case HEADCODE_ATTACK_NORMAL:			{ this->NormalAttack(Packet); } break;
		case HEADCODE_NORMAL_MAGIC_ATTACK:		{ this->NormalMagicAttack(Packet); } break;
		case HEADCODE_CANCEL_MAGIC:				{ this->MagicCancel(Packet); } break;
		case HEADCODE_DURATION_MAGIC_ATTACK:	{ this->DurationMagicAttack(Packet); } break;
		case HEADCODE_ATTACK_MULTI_TARGET:		{ this->MultiTargetMagicAttack(Packet); } break;
		
		case HEADCODE_CHAT:						{ this->ChatRequest(Packet); } break;
		case HEADCODE_WHISPER:					{ this->WhisperRequest(Packet);  } break;
		//case HEADCODE_CHARACTER_READY:			{ this->EnterWorldSuccess(Packet); } break;
		case HEADCODE_TIME_CHECK:				{ this->Ping(Packet); } break;
		case HEADCODE_ACTION:					{ this->CharacterAction(Packet); } break;
		case HEADCODE_TELEPORT:					{ this->TeleportRequestSecond(Packet); } break;
		case HEADCODE_MOVE:						{ this->CharacterMove(Packet); } break;
		case HEADCODE_ITEM_GET:					{ this->ItemGet(Packet); } break;
		case HEADCODE_ITEM_DROP:				{ this->ItemDrop(Packet, false); } break;
		case HEADCODE_ITEM_MOVE:				{ this->ItemMove(Packet); } break;
		case HEADCODE_ITEM_USE:					{ this->ItemUse(Packet); } break;
		case HEADCODE_ITEM_DISASSEMBLE:			{ this->ItemDisassemble(Packet); } break;
		case HEADCODE_NPC_TALK:					{ this->TalkToNpc(Packet); } break;
		case HEADCODE_CLOSE_INTERFACE:			{ this->CloseInterface(); } break;
		case HEADCODE_ITEM_BUY:					{ this->ItemBuy(Packet); } break;
		case HEADCODE_ITEM_SELL:				{ this->ItemSell(Packet); } break;
		case HEADCODE_ITEM_REPAIR:				{ this->ItemRepairRequest(Packet); } break;

		case HEADCODE_TRADE_REQUEST:			{ this->TradeRequest(Packet); } break;
		case HEADCODE_TRADE_ANSWER:				{ this->TradeRequestAnswer(Packet); } break;
		case HEADCODE_TRADE_MONEY:				{ this->TradeZen(Packet); } break;
		case HEADCODE_TRADE_OK:					{ this->TradeButtonOK(Packet); } break;
		case HEADCODE_TRADE_CANCEL:				{ this->TradeCancelRequest(); } break;

		case HEADCODE_PARTY_REQUEST:			{ this->PartyRequest(Packet); } break;
		case HEADCODE_PARTY_ANSWER:				{ this->PartyRequestAnswer(Packet); } break;
		case HEADCODE_PARTY_DELETE:				{ this->HandleRemovePartyMember(Packet); } break;
		case 0x42: {} break; // Party Info Request
		case HEADCODE_EVENT_ENTER_COUNT: {} break; // Get Event Enter Count

		case HEADCODE_POSITION_SET:				{ this->PositionSet(Packet); } break;

		case HEADCODE_SKILL_NEW:				{ this->NewMagicAttack(Packet); } break;
		case HEADCODE_SKILL_DARK_SIDE:			{ this->DarkSideAttack(Packet); } break;

		case HEADCODE_GUILD_REQUEST:			{ this->GuildJoinRequest(Packet); } break;
		case HEADCODE_GUILD_ANSWER:				{ this->GuildJoinAnswer(Packet); } break;
		case HEADCODE_GUILD_LIST_ALL:			{ this->GuildListAll(); } break;
		case HEADCODE_GUILD_MEMBER_DEL:			{ this->GuildMemberDelRequest(Packet); } break;
		case HEADCODE_GUILD_NPC_ANSWER:			{ this->GuildNpcAnswer(Packet); } break;
		case HEADCODE_GUILD_CREATE:				{ this->GuildCreateRequest(Packet); } break;
		case HEADCODE_GUILD_WAR:				{ sGuildWarMgr->StartWarResult(this, Packet); } break;
		case HEADCODE_GUILD_UPDATE_VIEWPORT:	{ this->GuildUpdateViewport(Packet); } break;

		case HEADCODE_WAREHOUSE_MONEY:			{ this->WarehouseMoneyInOut(Packet); } break;
		case HEADCODE_WAREHOUSE_CLOSE:			{ this->WarehouseClose(); } break;
		case HEADCODE_WAREHOUSE_PASSWORD:		{ this->WarehousePasswordRequest(Packet); } break;

		case HEADCODE_MIX_BUTTON:				{ this->ChaosMixButton(Packet); } break;
		case HEADCODE_MIX_CLOSE:				{ this->ChaosMixClose(); } break;
		case HEADCODE_MASS_COMBINATION:			{ this->ChaosMixMassCombination(Packet); } break;
		case HEADCODE_MOVE_REQUEST:				{ this->TeleportRequest(Packet); } break;
		case HEADCODE_DEVIL_SQUARE_ENTER_REQUEST:{ sDevilSquareMgr->EnterRequest(this, Packet); } break;
		case HEADCODE_EVENT_REMAIN_TIME_REQUEST: { this->EventGetRemainTime(Packet); } break;
		case HEADCODE_BLOOD_CASTLE_ENTER_REQUEST:{ sBloodCastleMgr->EnterRequest(this, Packet); } break;

		case HEADCODE_QUEST_EVOLUTION_INFO: { this->QuestEvolutionSendInfo(); } break;
		case HEADCODE_QUEST_EVOLUTION_SET_STATE: { this->QuestEvolutionSetState(Packet); } break;

		case HEADCODE_PET_COMMAND:				{ this->DarkSpiritSetCommand(Packet); } break;
		case HEADCODE_PET_INFO:					{ this->PetInfoRequest(Packet); } break;

		case HEADCODE_DUEL:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case 0x01: { this->DuelRequest(Packet); } break;
				case 0x02: { this->DuelAnswer(Packet); } break;
				case 0x03: { this->DuelEndRequest(Packet); } break;
				case 0x07: { this->DuelSpectatorRequest(Packet); } break;
				case 0x09: { this->DuelSpectatorRemove(Packet); } break;
				}
			} break;

		case HEADCODE_HELPER_SETTINGS_SAVE:		{ this->HelperSaveSettings(Packet); } break;

		case HEADCODE_CHAOS_CASTLE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_CHAOS_CASTLE_ENTER_REQUEST: { sChaosCastleMgr->EnterRequest(this, Packet); } break;
				case SUBCODE_CHAOS_CASTLE_REPOSITION: { } break;
				case SUBCODE_CHAOS_CASTLE_SURVIVAL_DATA: { sChaosCastleSurvivalMgr->DataRequest(this, Packet); } break;
				case SUBCODE_CHAOS_CASTLE_SURVIVAL_ENTER: { sChaosCastleSurvivalMgr->EnterRequest(this, Packet); } break;
				case SUBCODE_CHAOS_CASTLE_SURVIVAL_POSITION: { sChaosCastleSurvivalMgr->PositionSet(this, Packet); } break;
				case SUBCODE_CHAOS_CASTLE_SURVIVAL_RANKING: { sChaosCastleSurvivalMgr->RankingRequest(this, Packet); } break;
				case SUBCODE_CHAOS_CASTLE_SURVIVAL_CLOSE: { sChaosCastleSurvivalMgr->Close(this, Packet); } break;
				}
			}

		case HEADCODE_TELEPORT_TARGET:			{ this->TeleportRequestTarget(Packet); } break;

		case HEADCODE_SERVER_MOVE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_SERVER_MOVE_RESULT:
					{
						sAuthServer->PlayerServerMoveAuth(this, Packet);
					} break;
				}
			} break;

		case HEADCODE_CASTLE_SIEGE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_CASTLE_SIEGE_STATE: { sCastleSiege->StateRequest(this); } break;
				case SUBCODE_CASTLE_SIEGE_REGISTER_GUILD: { sCastleSiege->RegisterGuildRequest(this); } break;
				case SUBCODE_CASTLE_SIEGE_GIVEUP_GUILD: { sCastleSiege->GiveupGuildRequest(this); } break;
				case SUBCODE_CASTLE_SIEGE_REGISTER_GUILD_INFO: { sCastleSiege->RegisterGuildInfoRequest(this); } break;
				case SUBCODE_CASTLE_SIEGE_REGISTER_MARK: { sCastleSiege->RegisterMarkRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_NPC_BUY: { sCastleSiege->NpcBuyRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_NPC_REPAIR: { sCastleSiege->NpcRepairRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_NPC_UPGRADE: { sCastleSiege->NpcUpgradeRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_TAX_MONEY_INFO: { sCastleSiege->TaxMoneyRequest(this); } break;
				case SUBCODE_CASTLE_SIEGE_TAX_RATE_INFO: { sCastleSiege->TaxRateRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_MONEY_OUT: { sCastleSiege->MoneyOutRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_GATE_OPERATE: { sCastleSiege->RequestGateOperate(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_MINI_MAP: { sCastleSiege->MiniMapRequest(this); } break;
				case SUBCODE_CASTLE_SIEGE_MINI_MAP_CLOSE: { sCastleSiege->MiniMapClose(this); } break;
				case SUBCODE_CASTLE_SIEGE_COMMAND_REQUEST: { sCastleSiege->MiniMapCommandRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_ENABLE_HUNT: { sCastleSiege->EnableHuntRequest(this, Packet); } break;
				}
			} break;

		case HEADCODE_CASTLE_SIEGE_NPC_LIST: { sCastleSiege->NpcListRequest(this, Packet); } break;
		case HEADCODE_CASTLE_SIEGE_GUILD_REGISTER_LIST: { sCastleSiege->GuildRegisterListRequest(this); } break;
		case HEADCODE_CASTLE_SIEGE_GUILD_LIST: { sCastleSiege->RequestGuildList(this); } break;

		case HEADCODE_CASTLE_SIEGE_MACHINE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_CASTLE_SIEGE_MACHINE_USE: { sCastleSiege->MachineUseRequest(this, Packet); } break;
				case SUBCODE_CASTLE_SIEGE_MACHINE_DAMAGE_VALUE: { sCastleSiege->MachineDamage(this, Packet); } break;
				}
			} break;

		case HEADCODE_CASTLE_SIEGE_MISC:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_CASTLE_SIEGE_MISC_OWNER_MARK: { sCastleSiege->RequestCastleOwnerMark(this); } break;
				case 0x05: { sCastleSiege->LandOfTrialsMoveRequest(this, Packet); } break;
				}
			} break;

		case HEADCODE_JEWEL_MIX:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_JEWEL_MIX: { sJewelMix->Mix(this, Packet); } break;
				case SUBCODE_JEWEL_UNMIX: { sJewelMix->UnMix(this, Packet); } break;
				}
			} break;

		case HEADCODE_CRYWOLF:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_CRYWOLF_STATE_INFO: { sCrywolf->sendStateInfo(this); } break;
				case SUBCODE_CRYWOLF_CONTRACT: { sCrywolf->AltarContractRequest(Packet, this); } break;
				case SUBCODE_CRYWOLF_PLUS: { sCrywolf->PlusChaosRateRequest(this); } break;
				}
			} break;

		case 0xBE:
			{
				this->HandleHelperPlusData(Packet);
			} break;

		case HEADCODE_EVENT_MISC:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_ILLUSION_TEMPLE_ENTER: { sIllusionTempleMgr->EnterRequest(this, Packet); } break;
				case 0x02: { } break; //CGReqUseIllusionTempleKillCntSkill((PMSG_USE_ILLUSIONTEMPLE_KILLCOUNT_SKILL *)aRecv, aIndex);
				case 0x05: { } break; //CGReqIllusionTempleDropReward((PMSG_ILLUSIONTEMPLE_DROP_REWARD *)aRecv, aIndex);
				case SUBCODE_DOUBLE_GOER_ENTER: { sDoppelganger->EnterRequest(this, Packet); } break;
				case SUBCODE_INVENTORY_ITEM_USE: { this->InventoryItemUse(Packet); } break;
				case SUBCODE_LOREN_MARKET_TELEPORT: { this->LorenMarketTeleport(); } break;
				case SUBCODE_HELPER_OPEN: { this->HelperOpenRequest(Packet); } break;
				case SUBCODE_PARTY_HELPER: { this->PartyHelper(Packet); } break;
				case SUBCODE_PARTY_SUMMON: { this->PartySummon(Packet); } break;
				case SUBCODE_PARTY_MOVE:{ this->PartyMove(Packet); }break;
				case SUBCODE_HELPER_PLUS_RUN: { this->HandleHelperPlusRun(Packet); } break;
				}
			} break;

		case HEADCODE_FRIEND_LIST_SEND:			{ this->SendFriendList(); } break;
		case HEADCODE_FRIEND_ADD:				{ this->FriendAddRequest(Packet); } break;
		case 0xC2:								{ } break;
		case HEADCODE_FRIEND_DEL:				{ this->FriendDelRequest(Packet); } break;
		case 0xC4:								{ } break;
		case HEADCODE_FRIEND_MAIL:				{ this->FriendMailRequest(Packet); } break;
		case HEADCODE_FRIEND_MAIL_READ:			{ this->FriendMailReadRequest(Packet); } break;
		case HEADCODE_FRIEND_MAIL_DELETE:		{ this->FriendMailDeleteRequest(Packet); } break;
		case HEADCODE_FRIEND_MAIL_LIST:			{ this->FriendMailList(); } break;
		case HEADCODE_CHAT_ROOM_CREATE:			{ this->FriendChatRoomCreateRequest(Packet); } break;
		case HEADCODE_CHAT_ROOM_INVITATION:		{ this->FriendChatRoomInvitationRequest(Packet); } break;

		case HEADCODE_NPC_INTERACTION:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_NPC_INTERACTION_WEREWOLF: { this->NpcInteractionBarrackTeleport(); } break;
				case SUBCODE_NPC_INTERACTION_GATE_KEEPER: { this->NpcInteractionRefugeTeleport(); } break;
				case SUBCODE_NPC_INTERACTION_SNOWMAN: { this->SnowmanMove(); } break;
				case SUBCODE_NPC_INTERACTION_SANTA_CLAUS: { this->SantaClausGift(); } break;
				case SUBCODE_NPC_INTERACTION_RUUD_BUY: { sRuudSystem->ItemBuyRequest(this, Packet); } break;
				case SUBCODE_NPC_INTERACTION_OPEN_BOX: { this->OpenBoxRequest(Packet); } break;
				}
			} break;

		case HEADCODE_KANTURU:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_KANTURU_NOTIFY_ENTRANCE: { sKanturuMgr->EntranceInfoNotify(this); } break;
				case SUBCODE_KANTURU_ENTER: { sKanturuMgr->EnterRequest(this); } break;
				}
			} break;

		case HEADCODE_CASH_SHOP:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch (subcode)
				{
				case SUBCODE_CASH_SHOP_COIN: { sCashShopMgr->CashShopCoinRequest(this); } break;
				case SUBCODE_CASH_SHOP_OPEN: { sCashShopMgr->CashShopOpenRequest(this, Packet); } break;
				case SUBCODE_CASH_SHOP_ITEM_BUY: { sCashShopMgr->CashShopItemBuyRequest(this, Packet); } break;
				case SUBCODE_CASH_SHOP_ITEM_GIFT: { sCashShopMgr->CashShopItemGiftRequest(this, Packet); } break;
				case SUBCODE_CASH_SHOP_ITEM_LIST: { sCashShopMgr->CashShopItemListRequest(this, Packet); } break;
				case SUBCODE_CASH_SHOP_ITEM_USE: { sCashShopMgr->CashShopItemUseRequest(this, Packet); } break;
				case SUBCODE_CASH_SHOP_ITEM_USE_NEW: { sCashShopMgr->CashShopItemUseRequestNew(this, Packet); }break;
				}
			} break;

		case HEADCODE_GUILD_ASSIGN_STATUS:		{ this->GuildAssignStatus(Packet); } break;
		case HEADCODE_GUILD_ASSIGN_TYPE:		{ this->GuildAssignType(Packet); } break;
		case HEADCODE_GUILD_RELATIONSHIP_REQUEST: { this->GuildRelationShipRequest(Packet); } break;
		case HEADCODE_GUILD_RELATIONSHIP_ANSWER: { this->GuildRelationShipAnswer(Packet); } break;
		case HEADCODE_GUILD_RELATIONSHIP_LIST: { this->GuildRelationShipList(); } break;
		case HEADCODE_GUILD_ALLIANCE_KICK: { this->GuildAllianceKick(Packet); } break;

		case 0xEC:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x00: { sPentagramSystem->CGPentagramJewelInsertRecv(this, Packet); } break;
				case 0x01: { sPentagramSystem->CGPentagramJewelRemoveRecv(this, Packet); } break;
				case 0x02: { sPentagramSystem->CGPentagramJewelRefineRecv(this, Packet); } break;
				case 0x03: { sPentagramSystem->CGPentagramJewelUpgradeRecv(this, Packet); } break;
				case 0x05: { sPentagramSystem->CGPentagramUpgradeRadianceSlot(this, Packet); } break;
				case 0x06: { this->WingSocketOptionAdd(Packet); } break;
				case 0x07: { this->WingSocketOptionUpgrade(Packet); } break;
				case 0x08: 
					{
						this->EarringUpgrade(Packet);
					} break;
					
				case 0x27: // TODO: Averiguar que es esto (Llega en login char)
					{
					} break;

				case 0x50:
					{
						sHuntingRecord->RecordRequest(this, Packet);
					} break;

				case 0x51:
					{
						sHuntingRecord->Close(this);
					} break;

				case 0x52:
					{
						sHuntingRecord->SetVisible(this, Packet);
					} break;

				case 0x54:
					{
						this->PlayerRightClick(Packet);
					} break;

				case 0x56:
					{
						this->ServerWarp(Packet);
					} break;

				case 0x57:
					{
						this->SendServerList();
					} break;

				case 0x61:
					{
						this->KuberaMineCart(Packet);
					} break;
				}
			} break;

		case 0x4E:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x00: { sMuunSystem->CGMuunItemGetRecv(this, Packet); } break;
				case 0x01: { } break;
				case 0x08: { sMuunSystem->CGMuunItemUseRecv(this, Packet); } break;
				case 0x09: { sMuunSystem->CGMuunItemSellRecv(this, Packet); } break;
				case 0x11: { sMuunSystem->CGMuunMountRecv(this, Packet, true); }break;
				case 0x13: { sMuunSystem->CGMuunExchangeRecv(this, Packet); } break;
				case 0x23: { sMuunSystem->SelectDisplayMuun(this, Packet); } break;
				}
			} break;

		case HEADCODE_GREMORY_CASE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case SUBCODE_GREMORY_CASE_ITEM_USE: { sGremoryCase->ItemUseRequest(this, Packet); } break;
				case SUBCODE_GREMORY_CASE_OPEN: { sGremoryCase->ItemListRequest(this, Packet); } break;
				}
			} break;

		case 0xE7:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x01: { sMiniMap->CGMiniMapStartPartyInfoRecv(this); } break;
				case 0x02: { sMiniMap->CGMiniMapClosePartyInfoRecv(this); } break;
				case 0x03: { sMiniMap->CGMiniMapInfoRecv(this, Packet); } break;
				}
			} break;

		case HEADCODE_MINING_SYSTEM:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x00: { sMiningSystem->CGMiningStartRecv(this, Packet); } break;
				case 0x01: { sMiningSystem->CGMiningSuccessRecv(this, Packet); } break;
				case 0x03: { sMiningSystem->CGMiningFailureRecv(this, Packet); } break;

				case 0x10: { this->PetTrainerMix1(Packet); } break; ///- Mix
				case 0x11: { this->PetTrainerMix2(Packet); } break; ///- Change Guardian Option
				}
			} break;

		case HEADCODE_BATTLE_CORE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case SUBCODE_BATTLE_CORE_GREMORY_CASE_OPEN: { sGremoryCase->OpenRequest(this, Packet); } break;
				default:
					{
						sLog->outPacket("root", Packet, size, "CLIENT TO SERVER WRONG: %s", this->BuildLog().c_str());
					} break;
				}
			} break;

		case 0x6F:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x00:
					{
						this->ItemSoldListView();
					} break;

				case 0x01:
					{
						this->ItemSoldListClose();
					} break;

				case 0x02:
					{
						this->ItemSoldRecover(Packet);
					} break;
				}
			} break;

		case 0x4D:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x00: { sEventInventory->CGEventItemGetRecv(this, Packet); } break;
				case 0x01: { sEventInventory->CGEventItemDropRecv(this, Packet); } break;
				case 0x0F: { sEventInventory->CGEventInventoryOpenRecv(this, Packet); } break;

				case 0x10: { sMuRoomy->Start(this, Packet); } break;
				case 0x11: { sMuRoomy->Reveal(this, Packet); } break;
				case 0x12: { sMuRoomy->Move(this, Packet); } break;
				case 0x13: { sMuRoomy->Remove(this, Packet); } break;
				case 0x14: { sMuRoomy->Match(this, Packet); } break;
				case 0x15: { sMuRoomy->End(this, Packet); } break;
				case 0x19: { sMuRoomy->SpecialMatch(this, Packet); } break;

				case 0x23: { sMiniBomb->Start(this, Packet); } break;
				case 0x24: { sMiniBomb->Reveal(this, Packet); } break;
				case 0x25: { sMiniBomb->Mark(this, Packet); } break;
				case 0x27: { sMiniBomb->Open(this, Packet); } break;
				case 0x28: { sMiniBomb->Reward(this, Packet); } break;

				case 0x2A: { sJewelBingo->Start(this, Packet); } break;
				case 0x2B: { sJewelBingo->Move(this, Packet); } break;
				case 0x2C: { sJewelBingo->BoxSelect(this, Packet); } break;
				case 0x2D: { sJewelBingo->JewelSelect(this, Packet); } break;
				case 0x2E: { sJewelBingo->Reward(this, Packet); } break;

				case 0x30: { sNumericBaseball->Start(this, Packet); } break;
				case 0x31: { sNumericBaseball->SelectNumber(this, Packet); } break;
				}
			} break;

		case 0xED:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x00: { sGuildMatching->CGGuildMatchingListRecv(this, Packet); } break;
				case 0x01: { sGuildMatching->CGGuildMatchingListSearchRecv(this, Packet); } break;
				case 0x02: { sGuildMatching->CGGuildMatchingInsertRecv(this, Packet); } break;
				case 0x03: { sGuildMatching->CGGuildMatchingCancelRecv(this, Packet); } break;
				case 0x04: { sGuildMatching->CGGuildMatchingJoinInsertRecv(this, Packet); } break;
				case 0x05: { sGuildMatching->CGGuildMatchingJoinCancelRecv(this, Packet); } break;
				case 0x06: { sGuildMatching->CGGuildMatchingJoinAcceptRecv(this, Packet); } break;
				case 0x07: { sGuildMatching->CGGuildMatchingJoinListRecv(this, Packet); } break;
				case 0x08: { sGuildMatching->CGGuildMatchingJoinInfoRecv(this, Packet); } break;
				}
			} break;

		case 0xEF:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x00: { sPartyMatching->HandleRegister(this, Packet); } break;
				case 0x01: { sPartyMatching->HandleSearch(this, Packet); } break;
				case 0x02: { sPartyMatching->HandleJoin(this, Packet); } break;
				case 0x03: { sPartyMatching->HandleJoinData(this, Packet); } break;
				case 0x04: { sPartyMatching->HandleJoinList(this, Packet); } break;
				case 0x05: { sPartyMatching->HandleAccept(this, Packet); } break;
				case 0x06: { sPartyMatching->HandleCancelRegister(this, Packet); } break;
				case 0x07: { this->PartyLeaderChange(Packet); }break;
				}
			} break;

		case 0x6E:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch( subcode )
				{
				case 0x01: { this->ChatBlockAdd(Packet); } break;
				case 0x02: { this->ChatBlockDelete(Packet); } break;
				}
			} break;

		case HEADCODE_TORMENTED_SQUARE_SURVIVAL:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch ( subcode )
				{
				case SUBCODE_TORMENTED_SQUARE_SURVIVAL_SCHEDULE:
					{
						sTormentedSquareSurvival->ScheduleRequest(this, Packet);
					} break;

				case SUBCODE_TORMENTED_SQUARE_SURVIVAL_ENTER_REQUEST:
					{
						sTormentedSquareSurvival->EnterRequest(this, Packet);
					} break;

				case SUBCODE_TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT:
					{
						sTormentedSquareSurvival->EnterAccept(this, Packet);
					} break;

				case SUBCODE_TORMENTED_SQUARE_SURVIVAL_RANKING:
					{
						sTormentedSquareSurvival->RankingRequest(this, Packet);
					} break;

				case 0x09:
					{
						
					} break;
				}
			} break;

		case HEADCODE_TORMENTED_SQUARE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch ( subcode )
				{
				case SUBCODE_TORMENTED_SQUARE_ENTER: { sTormentedSquare->EnterRequest(this, Packet); } break;
				case SUBCODE_TORMENTED_SQUARE_QUEUE: { sTormentedSquare->QueueResult(this, Packet); } break;
				}
			} break;

		case HEADCODE_MISC_EVENT:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch ( subcode )
				{
				case 0x08: ///- Select Reward Card
					{
					} break;

				case 0x09: ///- Close Reward Card Window
					{
					} break;

				case 0x20: ///- Start Quest Test (Part 1)
					{
						this->CentQuestStartRequest(Packet);
					} break;

				case 0x21:
					{
						this->CentQuestSpawnMonster(Packet);
					} break;

				case 0x22:
					{
						this->CentQuestMoveRequest(Packet);
					} break;
				}
			} break;

		case HEADCODE_QUEST_SUPPORT:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_QUEST_SUPPORT_BUFF: { this->ElfSoldierBuffUP(); } break;

				case SUBCODE_QUEST_GUIDED_COMPLETE: { this->QuestGuidedComplete(Packet); } break;
				case SUBCODE_QUEST_GUIDED_GIVE_UP: { this->QuestGuidedGiveUp(Packet); } break;
				case SUBCODE_QUEST_GUIDED_GIVE_UP_CHAPTER: { this->QuestGuidedGiveUpChapter(Packet); } break;
				case SUBCODE_QUEST_GUIDED_WARP: { this->QuestGuidedWarp(Packet); } break;
				case SUBCODE_QUEST_GUIDED_REACH_TARGET: { this->QuestGuidedReachZone(Packet); }break;

				case SUBCODE_QUEST_MU_NPC_TALK:
				{
					this->QuestMUNpcTalk(Packet);
				} break;

				case SUBCODE_QUEST_MU_ACCEPT:
				{
					this->QuestMUAccept(Packet);
				} break;

				case SUBCODE_QUEST_MU_GIVE_UP:
				{
					this->QuestMUGiveUp(Packet);
				} break;

				case SUBCODE_QUEST_MU_COMPLETE:
				{
					this->QuestMUComplete(Packet);
				} break;

				case SUBCODE_QUEST_MU_REACH_TARGET:
				{
					this->QuestMUReachTarget(Packet);
				} break;

				case SUBCODE_QUEST_MU_INFO:
				{
					this->QuestMUInfo(Packet);
				} break;

				case SUBCODE_QUEST_MU_MOVE:
				{
					this->QuestMUMove(Packet);
				} break;

				case SUBCODE_QUEST_MU_CLICK_COMPLETED:
				{
					this->QuestMUClickCompleted(Packet);
				} break;
				}
			} break;

		case HEADCODE_IMPERIAL_FORTRESS:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_IMPERIAL_FORTRESS_ENTER: { sImperialFortressMgr->EnterRequest(this); } break;
				}
			} break;

		case HEADCODE_GEN_SYSTEM:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
					case SUBCODE_GEN_SYSTEM_JOIN: { this->GenJoinRequest(Packet); } break;
					case SUBCODE_GEN_SYSTEM_DELETE: { this->GenDeleteRequest(); } break;
					case SUBCODE_GEN_SYSTEM_REWARD: { this->GenRewardRequest(Packet); } break;
					case SUBCODE_GEN_SYSTEM_INFO: { this->GenInfoRequest(); } break;

					case SUBCODE_WARP_ACHERON: { this->AcheronWarp(); } break;

					case SUBCODE_ARKA_WAR_MASTER_REGISTER: { sArkaWar->MasterRegisterRequest(this, Packet); } break;
					case SUBCODE_ARKA_WAR_MEMBER_REGISTER: { sArkaWar->MemberRegisterRequest(this, Packet); } break;
					case SUBCODE_ARKA_WAR_ENTER: { sArkaWar->EnterRequest(this, Packet); } break;
					case SUBCODE_ARKA_WAR_OPEN_TROPHY_EXCHANGE: { sArkaWar->ExchangeOpen(this, Packet); } break;
					case SUBCODE_ARKA_WAR_VIEW_GUILD_MEMBER: { sArkaWar->ViewGuildMemberRequest(this, Packet); } break;
					case SUBCODE_ARKA_WAR_SIGN_OF_LORD_REGISTER: { sArkaWar->SignOfLordRegisterRequest(this, Packet); } break;
					case SUBCODE_ARKA_WAR_SIGN_OF_LORD_CHECK: { sArkaWar->SignOfLordCheckRequest(this, Packet); } break;

					case SUBCODE_ENTER_AS_THE_PROTECTOR_OF_ACHERON: { sProtectorOfAcheron->EnterRequest(this, Packet); } break;
				}
			} break;

		case 0x70:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case 0x00:
				case 0x01:
					{
						this->CreateDarkAncientItem(Packet);
					} break;

				case 0x10:
					{
						sMossMerchant->OpenBox(this, Packet);
					} break;
				}
			} break;

		case 0x71:
			{
				sLog->outPacket("antihack", Packet, size, "CLIENT TO SERVER WRONG: %s", this->BuildLog().c_str());
			} break;

		case 0x78:
			{
				sSocketSystem->MixRequest(this, Packet);
			} break;

		case HEADCODE_NIXIES_LAKE: 
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_NIXIES_LAKE_ENTER:
					{
						sNixiesLake->EnterRequest(this, Packet);
					} break;
					
				case SUBCODE_NIXIES_LAKE_ENTER_CONFIRM:
					{
						sNixiesLake->EnterConfirm(this, Packet);
					} break;

				case SUBCODE_NIXIES_LAKE_MEMBER_ENTER:
					{
						sNixiesLake->MemberEnterConfirm(this, Packet);
					} break;

				case 0x10:
					{
						sNixiesLake->IceBreathDamage(this, Packet);
					} break;
				}
			} break;

		case HEADCODE_LABYRINTH_OF_DIMENSIONS:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch(subcode)
				{
				case SUBCODE_LABYRINTH_OF_DIMENSIONS_ENTRANCE:
					{
						sLabyrinthDimensions->EnterRequest(this, Packet);
					} break;

				case SUBCODE_LABYRINTH_OF_DIMENSIONS_ZONE_ENTER:
					{
						sLabyrinthDimensions->EnterConfirm(this, Packet);
					} break;

				case SUBCODE_LABYRINTH_OF_DIMENSIONS_GATE_ENTER:
					{
						sLabyrinthDimensions->GateEnter(this, Packet);
					} break;

				case SUBCODE_LABYRINTH_OF_DIMENSIONS_NEXT_LEVEL:
					{
						
					} break;

				case SUBCODE_LABYRINTH_OF_DIMENSIONS_ENTRANCE_LAYER:
					{
						sLabyrinthDimensions->LeagueInfo(this, Packet);
					} break;

				case SUBCODE_LABYRINTH_OF_DIMENSIONS_LEAVE:
					{
						sLabyrinthDimensions->Leave(this, Packet);
					} break;
				}
			} break;

		//case 0x6D:
		//	{
		//		MAKE_SUBCODE(subcode, Packet);

		//		switch(subcode)
		//		{
		//		case 0x01:
		//			{
		//				this->WarpFavoriteListSave(Packet);
		//			} break;
		//		}
		//	} break;

		case HEADCODE_MAJESTIC_SKILL_TREE:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch (subcode)
				{
				case SUBCODE_MAJESTIC_SKILL_TREE_ADD:
					{
						this->MajesticSkillTreeAdd(Packet);
					} break;

				case SUBCODE_MAJESTIC_SKILL_TREE_STAT_ADD:
					{
						this->MajesticSkillTreeAddStat(Packet);
					} break;

				case SUBCODE_MAJESTIC_SKILL_TREE_PASSIVE_ADD:
					{
						this->MajesticSkillTreeAddPassive(Packet);
					} break;
				}
			} break;

		case 0xE8:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch (subcode)
				{
				case 0x00:
					this->ItemSplit(Packet);
					break;

				case 0x01:
					this->ItemEnhancementRequest(Packet);
					break;
				}
			} break;

		case 0x59:
			{
				MAKE_SUBCODE(subcode, Packet);

				switch (subcode)
				{
				case 0x01:
					{
						this->PositionAttack(Packet);
					} break;

				case 0x04:
					{
						this->WarpFavoriteListSave(Packet);
					} break;
				}
			} break;

		case 0x7C:
		{
			MAKE_SUBCODE(subcode, Packet);

			switch (subcode)
			{
			case 0x00:
			{
				this->HandlePersonalStoreSearch(Packet);
			} break;

			case 0x01:
			{
				this->HandlePersonalStoreSearchItemList(Packet);
			} break;

			case 0x02:
			{
				this->HandlePersonalStoreView(Packet);
			} break;

			case 0x03:
			{
				this->HandlePersonalStoreSearchItem(Packet);
			} break;

			case 0x04:
			{
				this->HandlePersonalStoreSelectItem(Packet);
			} break;

			case 0x05:
			{
				this->HandlePersonalStorePurchase(Packet);
			} break;

			case 0x06:
			{
				this->HandlePersonalStoreSellList(Packet);
			} break;

			case 0x07:
			{
				this->HandlePersonalStoreSellItemView(Packet);
			} break;

			case 0x08:
			{
				this->HandlePersonalStoreSellItem(Packet);
			} break;

			case 0x09:
			{
				this->HandlePersonalStoreSellItemCancel(Packet);
			} break;

			case 0x0A:
			{
				this->HandlePersonalStoreState(Packet);
			} break;
			}
		} break;


		default:
			{
				sLog->outPacket("antihack", Packet, size, "CLIENT TO SERVER WRONG: %s", this->BuildLog().c_str());
			} break;
		}
		return;
	TRY_END
}

std::string Player::BuildLog() const
{
	std::ostringstream stream;

	stream << "[SRV " << sGameServer->GetServerCode() << " -- " << this->GetEntry() << " - ";

	if ( strlen(this->GetAccountData()->GetIP()) )
		stream << "IP: " << this->GetAccountData()->GetIP() << " - ";

	if ( strlen(this->GetAccountData()->GetMac()) )
		stream << "MAC: " << this->GetAccountData()->GetMac() << " - ";

	//stream << "DISK_SERIAL: " << this->GetAccountData()->GetDiskSerial() << " - ";

	if ( this->GetAccountData()->GetGUID() )
		stream << "Account: " << this->GetAccountData()->GetGUID() << "-" << this->GetAccountData()->GetAccount() << " - ";

	if ( this->GetGUID() )
		stream << "Character: " << this->GetGUID() << "-" << this->GetName();

	stream << "]";

	return stream.str();
}

std::string Player::BuildLogDB() const
{
	std::ostringstream stream;

	stream << "[{";

	stream << "\"server\":" << sGameServer->GetServerCode();
	stream << ",\"index\":" << this->GetEntry();
	stream << ",\"ip\":" << "\"" << this->GetAccountData()->GetIP() << "\"";
	stream << ",\"mac\":" << "\"" << this->GetAccountData()->GetMac() << "\"";
	stream << ",\"disk_serial\":" << this->GetAccountData()->GetDiskSerial();

	if ( this->GetAccountData()->GetGUID() )
	{
		stream << ",\"acc_id\":" << this->GetAccountData()->GetGUID();
		stream << ",\"acc\":" << "\"" << this->GetAccountData()->GetAccount() << "\"";

		if ( this->GetGUID() )
		{
			stream << ",\"char_id\":" << this->GetGUID();
			stream << ",\"char_name\":" << "\"" << this->GetName() << "\"";

			stream << ",\"world_id\":" << this->GetWorldId();
			stream << ",\"world_x\":" << this->GetX();
			stream << ",\"world_y\":" << this->GetY();
			stream << ",\"world_instance\":" << this->GetInstance();
		}
	}
	
	stream << "}]";

	return stream.str();
}

std::string Player::BuildLogStat() const
{
	std::ostringstream stream;

	stream << "[{";

	stream << "\"STR\":" << this->GetTotalStat(STRENGTH);
	stream << ",\"AGI\":" << this->GetTotalStat(AGILITY);
	stream << ",\"VIT\":" << this->GetTotalStat(VITALITY);
	stream << ",\"ENE\":" << this->GetTotalStat(ENERGY);
	stream << ",\"CMD\":" << this->GetTotalStat(LEADERSHIP);

	stream << ",\"HP\":" << this->PowerGetTotal(POWER_LIFE);
	stream << ",\"MP\":" << this->PowerGetTotal(POWER_MANA);
	stream << ",\"SD\":" << this->PowerGetTotal(POWER_SHIELD);
	stream << ",\"AG\":" << this->PowerGetTotal(POWER_STAMINA);

	stream << ",\"ATT_RIGHT_MIN\":" << this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN);
	stream << ",\"ATT_RIGHT_MAX\":" << this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX);
	stream << ",\"ATT_LEFT_MIN\":" << this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN);
	stream << ",\"ATT_LEFT_MAX\":" << this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX);
	stream << ",\"ATT_RATE\":" << this->GetIntData(UNIT_INT_ATTACK_RATE);
	stream << ",\"ATT_RATE_PVP\":" << this->GetIntData(UNIT_INT_ATTACK_RATE_PVP);
	stream << ",\"ATT_SPEED\":" << this->GetIntData(UNIT_INT_ATTACK_SPEED);
	stream << ",\"ATT_MAGIC_SPEED\":" << this->GetIntData(UNIT_INT_MAGIC_SPEED);
	stream << ",\"ATT_MAGIC_MIN\":" << this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN);
	stream << ",\"ATT_MAGIC_MAX\":" << this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX);
	stream << ",\"ATT_CURSE_MIN\":" << this->GetIntData(UNIT_INT_CURSE_DAMAGE_MIN);
	stream << ",\"ATT_CURSE_MAX\":" << this->GetIntData(UNIT_INT_CURSE_DAMAGE_MAX);
	stream << ",\"DEFENSE\":" << this->GetIntData(UNIT_INT_DEFENSE);
	stream << ",\"DEFENSE_MAGIC\":" << this->GetIntData(UNIT_INT_DEFENSE_MAGIC);
	stream << ",\"DEFENSE_ADD_PVP\":" << this->GetIntData(UNIT_INT_DEFENSE_ADD_PVP);
	stream << ",\"DEFENSE_RATE\":" << this->GetIntData(UNIT_INT_DEFENSE_RATE);
	stream << ",\"DEFENSE_RATE_PVP\":" << this->GetIntData(UNIT_INT_DEFENSE_RATE_PVP);
	stream << ",\"DEFENSE_BASE\":" << this->GetIntData(UNIT_INT_DEFENSE_BASE);
	stream << ",\"DAMAGE_ADD\":" << this->GetIntData(UNIT_INT_DAMAGE_ADD);
	stream << ",\"SKILL_DAMAGE_ADD\":" << this->GetIntData(UNIT_INT_SKILL_ADD_DAMAGE);
	stream << ",\"COMBO_DAMAGE\":" << this->GetIntData(UNIT_INT_COMBO_DAMAGE);

	stream << ",\"DAMAGE_DECREASE\":" << this->GetIntData(UNIT_INT_DAMAGE_DECREASE);
	stream << ",\"DAMAGE_DECREASE_ADD\":" << this->GetIntData(UNIT_INT_DAMAGE_DECREASE_ADD);

	stream << ",\"CRITICAL_DAMAGE_RATE\":" << this->GetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE);
	stream << ",\"CRITICAL_DAMAGE_ADD\":" << this->GetIntData(UNIT_INT_CRITICAL_DAMAGE_ADD);

	stream << ",\"EXCELLENT_DAMAGE_RATE\":" << this->GetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE);
	stream << ",\"EXCELLENT_DAMAGE_ADD\":" << this->GetIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD);

	stream << ",\"DOUBLE_DAMAGE_RATE\":" << this->GetFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE);
	stream << ",\"DOUBLE_DAMAGE_ADD\":" << this->GetIntData(UNIT_INT_DOUBLE_DAMAGE_ADD);

	stream << ",\"TRIPLE_DAMAGE_RATE\":" << this->GetFloatData(UNIT_FLOAT_TRIPLE_DAMAGE_RATE);

	stream << ",\"REFLECT_DAMAGE_RATE\":" << this->GetFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE);
	stream << ",\"REFLECT_DAMAGE_ADD\":" << this->GetIntData(UNIT_INT_REFLECT_DAMAGE_ADD);

	stream << ",\"IGNORE_RATE\":" << this->GetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE);
	stream << ",\"STUN_RATE\":" << this->GetFloatData(UNIT_FLOAT_STUN_RATE);

	stream << ",\"TOTAL_REFLECT\":" << this->GetFloatData(UNIT_FLOAT_TOTAL_REFLECT_DAMAGE);
	stream << ",\"TOTAL_HP_RECOVERY\":" << this->GetFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY);
	stream << ",\"TOTAL_MP_RECOVERY\":" << this->GetFloatData(UNIT_FLOAT_TOTAL_MANA_RECOVERY);
	stream << ",\"TOTAL_SD_RECOVERY\":" << this->GetFloatData(UNIT_FLOAT_TOTAL_SHIELD_RECOVERY);
	stream << ",\"TOTAL_AG_RECOVERY\":" << this->GetFloatData(UNIT_FLOAT_TOTAL_STAMINA_RECOVERY);

	stream << ",\"ELEMENTAL_DEFENSE\":" << this->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE);
	stream << ",\"ELEMENTAL_DAMAGE_MIN\":" << this->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN);
	stream << ",\"ELEMENTAL_DAMAGE_MAX\":" << this->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX);
	stream << ",\"ELEMENTAL_ATT_RATE\":" << this->GetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE);
	stream << ",\"ELEMENTAL_DEF_RATE\":" << this->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE);

	stream << ",\"ELEMENTAL_DEF_PERCENT\":" << this->GetFloatData(UNIT_FLOAT_ELEMENTAL_DEFENSE_PERCENT);
	stream << ",\"ELEMENTAL_ATT_PERCENT\":" << this->GetFloatData(UNIT_FLOAT_ELEMENTAL_DAMAGE_PERCENT);
	stream << ",\"ELEMENTAL_DEF_RATE_PERCENT\":" << this->GetFloatData(UNIT_FLOAT_ELEMENTAL_DEFENSE_SUCCESS_RATE_PERCENT);
	stream << ",\"ELEMENTAL_ATT_RATE_PERCENT\":" << this->GetFloatData(UNIT_FLOAT_ELEMENTAL_ATTACK_SUCCESS_RATE_PERCENT);

	stream << "}]";

	return stream.str();
}

std::string Player::BuildLogSet() const
{
	std::string item_data;

	wear_inventory_loop(i)
	{
		this->BuildLogSet(i, item_data);
	}

	this->BuildLogSet(236, item_data);
	this->BuildLogSet(237, item_data);
	this->BuildLogSet(238, item_data);

	use_inventory_loop(i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		if (pItem->IsMount() && pItem->IsInventoryActive())
		{
			this->BuildLogSet(i, item_data);
		}
	}
	
	if (!item_data.empty())
	{
		item_data = item_data + "]";
	}

	return item_data;
}

void Player::BuildLogSet(uint8 slot, std::string & item_data) const
{
	Item const* pItem = this->GetInventory()->GetItem(slot);
	if (pItem && pItem->IsItem())
	{
		if (item_data.empty())
		{
			item_data = "[";
		}
		else
		{
			item_data = item_data + ",";
		}

		item_data = item_data + pItem->BuildLogDB(slot, false);
	}
}

std::string Player::BuildLocationLog(int16 x, int16 y, bool to_db) const
{
	std::ostringstream stream("");

	if ( x == -1 )
		x = this->GetX();

	if ( y == -1 )
		y = this->GetY();

	if ( !to_db )
		stream << "Location:";

	if ( !to_db )
	{
		stream << "[" << this->GetWorldId() << " - " << x << "/" << y << "]";
	}
	else
	{
		stream << "[LOCATION]" << this->GetWorldId() << " - " << this->GetWorldName() << "- [" << x << "/" << y << "]" << "[LOCATION]";
	}
	
	return stream.str();
}

bool Player::SecureCodeCheck(const char * code) const
{
	if ( !sGameServer->IsSecurityCode() )
		return true;

	if ( !memcmp(this->GetAccountData()->secure_code, code, MAX_SECURE_CODE_LENGTH) )
		return true;

	return false;
}

void Player::PlaySoundRequest(int32 id, uint8 data1, uint8 data2)
{
	//CUSTOM_PLAY_SOUND pMsg(id, data1, data2);
	//this->SEND_PCT(pMsg);
}

void Player::BuildCustomPacketData(CUSTOM_PACKET_PLAYER & data)
{
	data.entry = this->GetEntry();
	data.account_id = this->GetAccountData()->GetGUID();
	data.guid = this->GetGUID();
	data.ms_time = this->GetMSTime();
	memset(data.name, 0, MAX_CHARACTER_LENGTH + 1);
	memcpy(data.name, this->GetName(), MAX_CHARACTER_LENGTH + 1);
}

void Player::HackReport(uint8 * Packet)
{
	POINTER_PCT(HACK_REPORT, lpMsg, Packet, 0);

	sLog->outPacket("antihack", Packet, lpMsg->h.get_size(), "[ HACK REPORT ] - %s", this->BuildLog().c_str());
}

bool Player::IsPlayerBasicState(Player* pPlayer, bool packet_time, bool live)
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( !pPlayer->IsPlaying() )
	{
		return false;
	}

	if ( live && !pPlayer->IsLive() )
	{
		return false;
	}

	if ( pPlayer->IsServerQuit() )
	{
		return false;
	}

	if ( packet_time && !pPlayer->PacketTimeCheck() )
	{
		return false;
	}

	return true;
}

bool Player::IsPlayerTransactionFree(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( pPlayer->GetPersonalStore()->IsBusy() )
	{
		return false;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return false;
	}

	if ( pPlayer->GetTransaction() == TRANSACTION_TYPE_BEGIN )
	{
		return false;
	}

	return true;
}

bool Player::IsTimeout() const
{
	if ( sGameServer->GetSessionTimeout() > 0 )
	{
		if ( (GetTickCount() - this->GetTimeout()) < sGameServer->GetSessionTimeout() )
		{
			return false;
		}
	}

	return true;
}

void Player::UpdateAccountStatusType(uint8 type)
{
	SL_ACCOUNT_CONNECT_TYPE pMsg(this->GetAccountData()->GetGUID(), type);
	pMsg.h.server = sGameServer->GetServerCode();
	sAuthServer->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void Player::GoldenChannelUpdate()
{
	if ( sGameServer->GetServerMode() == SERVER_TYPE_PVP ||
		 sGameServer->GetServerMode() == SERVER_TYPE_NO_PVP )
	{
		return;
	}

	if ( time(nullptr) > this->GetAccountData()->GetGoldenChannel() )
	{
		KICK_PLAYER(this, "Golden Channel End");
	}
}

void Player::CheatScanDetect(uint8 * Packet)
{
	if ( !sGameServer->IsCheatScanEnabled() )
	{
		return;
	}

	if ( this->GetTimer(PLAYER_TIMER_CHEAT_SCAN_INIT)->Elapsed(5 * MINUTE * IN_MILLISECONDS) )
	{
		this->SetCounter(PLAYER_COUNTER_CHEAT_SCAN, 0);
	}

	POINTER_PCT_LOG(CHEAT_SCAN_DETECT, lpMsg, Packet, 0);

	if ( sGameServer->IsCheatScanWhiteList(lpMsg->process_name) )
	{
		sLog->outInfo("antihack", "%s -- %s -- CHEAT_SCAN_DETECT WHITE LIST (%u - %u - %s - %s)", __FUNCTION__, this->BuildLog().c_str(), lpMsg->type, lpMsg->process_id, lpMsg->process_name, lpMsg->window_name);
		return;
	}

	this->IncreaseCounter(PLAYER_COUNTER_CHEAT_SCAN, 1);

	sLog->outInfo("antihack", "%s -- %s -- CHEAT_SCAN_DETECT (%u - %u - %s - %s)", __FUNCTION__, this->BuildLog().c_str(), lpMsg->type, lpMsg->process_id, lpMsg->process_name, lpMsg->window_name);

	if ( this->GetTimer(PLAYER_TIMER_CHEAT_SCAN)->Elapsed(sGameServer->GetCheatScanTime()) )
	{
		sGameServer->LogAntiHack(this, ANTIHACK_ID_CHEAT_SCAN, "CHEAT_SCAN_DETECT (%u - %u - %s - %s)", lpMsg->type, lpMsg->process_id, lpMsg->process_name, lpMsg->window_name);
	}

	if ( sGameServer->GetCheatScanCount() > 0 && (this->GetCounter(PLAYER_COUNTER_CHEAT_SCAN) >= sGameServer->GetCheatScanCount()) )
	{
		if ( sGameServer->IsCheatScanBan() )
		{
			this->SetAuthority(1);
		}

		if ( sGameServer->IsCheatScanKick() )
		{
			KICK_PLAYER(this, "Cheat Scan");

			sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Cheat Scan");
		}
	}
}

//void Player::WhiteListRequest()
//{
//	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_WHITE_LIST_SELECT);
//	stmt->setString(0, this->GetAccountData()->GetIP());
//	stmt->setString(1, this->GetAccountData()->GetMac());
//	stmt->setUInt32(2, this->GetAccountData()->GetDiskSerial());
//	this->CallBackWhiteList = LoginDatabase.AsyncQuery(stmt);
//}
//
//void Player::WhiteListCallBack(PreparedQueryResult result)
//{
//	if ( this->GetConnectStatus() != CONNECT_STATUS_LOGGED &&
//		 this->GetConnectStatus() != CONNECT_STATUS_PLAYING )
//	{
//		return;
//	}
//
//	if ( !result )
//	{
//		return;
//	}
//
//	this->SetWhiteList(true);
//}

void Player::AutoLoginProcess()
{
	if ( !sGameServer->IsAutoLoginEnabled() && this->IsAutoLogin() )
	{
		this->SetAutoLogin(false);
		return;
	}

	if ( !this->IsAutoLogin() )
	{
		return;
	}

	if ( this->IsAutoLoginSent() )
	{
		return;
	}

	if ( !this->GetTimer(PLAYER_TIMER_AUTOLOGIN)->Elapsed() )
	{
		return;
	}

	this->SetAutoLoginSent(true);

	sLog->outInfo("auto_login", "%s -- [%u - %u - %u]", __FUNCTION__, this->GetAutoLoginAccount(), this->GetAutoLoginCharacter(), this->GetAutoLoginType());

	this->LoginRequest(nullptr, true);
}

void Player::AutoLoginCharacterSelect()
{
	LoginQueryHolder* holder = new LoginQueryHolder(this->GetAccountData()->GetGUID(), this->GetAutoLoginCharacter());

    if (!holder->Initialize())
    {
        delete holder;
		this->SetAutoLogin(false);
		sLog->outError(LOG_PLAYER, "[ AUTOLOGIN CHARACTER SELECT ] Error during Holder Creation %s - %u.", this->BuildLog().c_str(), this->GetAutoLoginCharacter());
        return;
    }

	sLog->outInfo(LOG_PLAYER, "[ AUTOLOGIN CHARACTER SELECT ] %s Character selected ( Step Two ).",
			this->BuildLog().c_str());

	this->_charSelectCallBack_2 = MuDatabase.DelayQueryHolder(holder);
}

void Player::SetAutoLoginInfo(uint8 type)
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CHARACTER_UPDATE);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->IsAutoLogin() ? this->GetAutoLoginType() : type);
	stmt->setString(2, this->GetAccountData()->GetIP());
	stmt->setString(3, this->GetAccountData()->GetMac());
	stmt->setUInt32(4, this->GetAccountData()->GetDiskSerial());
	stmt->setUInt16(5, sGameServer->GetServerCode());
	stmt->setUInt32(6, this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}