/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerDB.cpp"
*
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// LOAD FROM DB /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::LoadDB(QueryHolder holder)
{
	this->LoadDBItem(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ITEM));
	this->LoadDBSkill(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_SKILL));
	this->LoadDBGameOption(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GAME_OPTION));
	this->LoadDBQuestEvo(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_QUEST_EVO));
	this->LoadDBFriendList(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_FRIEND_LIST));
	this->LoadDBMailList(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MAIL_LIST));
	this->LoadDBGuild(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GUILD));
	this->LoadDBGen(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GEN), false);
	this->LoadDBGenKill(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GEN_KILL));
	this->LoadDBHelper(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_HELPER));
	this->LoadDBBuff(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_BUFF));
	this->LoadDBBuff(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_BUFF));
	this->LoadDBRestriction(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_RESTRICTION));
	this->LoadDBStatistics(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_STATISTICS));
	this->LoadDBItemDelay(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ITEM_DELAY));
	this->GetWarehouse()->LoadDBData(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_WAREHOUSE_DATA));
	this->GetWarehouse()->LoadDBItemList(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_WAREHOUSE_ITEMS));
	this->LoadDBCashShopItemList(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_CASH_SHOP_LIST));
	this->LoadDBCashShopCoin(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_CREDITS));
	this->LoadDBChatBlock(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_CHAT_BLOCK));
	
	this->LoadDBMuun(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MUUN));
	this->LoadDBEventInventory(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_EVENT_INVENTORY));
	this->LoadDBPentagramInfo(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_PENTAGRAM));
	this->LoadDBGremoryCase(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_CHARACTER), ITEM_BOX_GREMORY_CASE_INV);
	this->LoadDBGremoryCase(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_ACCOUNT), ITEM_BOX_GREMORY_CASE_ACC);
	this->LoadDBGremoryCase(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_MOBILE), ITEM_BOX_GREMORY_CASE_MOBILE);
	this->LoadDBGremoryCase(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_PERSONAL_STORE), ITEM_BOX_GREMORY_CASE_PS);
	this->LoadDBItemRecovery(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ITEM_RECOVERY));
	this->LoadDBItemSell(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_ITEM_SELL));
	this->LoadDBHuntingRecord(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_HUNTING_RECORD));
	this->LoadDBLabyrinth(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_LABYRINTH));
	this->LoadDBLabyrinthZone(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_LABYRINTH_ZONE));
	this->LoadDBLabyrinthLevel(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_LABYRINTH_LEVEL));
	this->LoadDBEventCount(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_EVENT_COUNT));
	this->LoadDBMiniBomb(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MINI_BOMB));
	this->LoadDBMiniBombGrid(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MINI_BOMB_GRID));
	this->LoadDBMuRoomy(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MU_ROOMY));
	this->LoadDBMuRoomyDeck(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MU_ROOMY_DECK));
	this->LoadDBWarpFavoriteList(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_WARP_FAVORITE_LIST));
	this->LoadDBQuestGuided(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_GUIDED_QUEST));
	this->LoadDBJewelBingo(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_JEWEL_BINGO));
	this->LoadDBJewelBingoGrid(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_JEWEL_BINGO_GRID));
	this->LoadDBDate(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_DATE));
	this->LoadDBMajesticTree(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MAJESTIC_TREE));
	this->LoadDBMajesticStats(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MAJESTIC_STATS));
	this->LoadDBNumericBaseball(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_NUMERIC_BASEBALL));
	this->LoadDBMixRecovery(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_RECOVERY_MIX));
	this->LoadDBQuestMU(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_QUEST_MU));
	this->LoadMonsterSoul(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_MONSTER_SOUL));
	this->LoadDBItemPersonalStore(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOAD_PERSONAL_STORE));

	for ( QuestEvolutionMap::const_iterator itr = sQuestMgr->m_quest_evolution.begin(); itr != sQuestMgr->m_quest_evolution.end(); ++itr )
	{
		QuestEvolution const* pQuest = itr->second;

		if ( pQuest )
		{
			QuestEvolutionStatus & quest =  this->quest_evolution[pQuest->GetID()];

			if ( quest.GetState() == QUEST_EVOLUTION_STATE_NONE )
			{
				quest.SetID(pQuest->GetID());
				quest.SetState(QUEST_EVOLUTION_STATE_CLEAR);
			}
		}
	}

	if ( this->IsAdministrator() )
	{
		if ( this->GetChangeUP(0) == 1 )
		{
			this->QuestEvolutionSetState(0, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(1, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(2, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(3, QUEST_EVOLUTION_STATE_COMPLETE);
		}

		if ( this->IsMaster() )
		{
			this->QuestEvolutionSetState(4, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(5, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(6, QUEST_EVOLUTION_STATE_COMPLETE);
		}

		if ( this->IsMajestic() )
		{
			this->QuestEvolutionSetState(7, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(8, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(9, QUEST_EVOLUTION_STATE_COMPLETE);
		}
	}
	else
	{
		if ( this->IsMaster() )
		{
			this->QuestEvolutionSetState(0, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(1, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(2, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(3, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(4, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(5, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(6, QUEST_EVOLUTION_STATE_COMPLETE);
		}

		if ( this->IsMajestic() )
		{
			this->QuestEvolutionSetState(0, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(1, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(2, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(3, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(4, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(5, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(6, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(7, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(8, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(9, QUEST_EVOLUTION_STATE_COMPLETE);
		}
	}

	if ( CharacterBaseData* pCharacterData = sCharacterBase->GetCharacterBase(this->GetClass()) )
	{
		for ( int32 i = 0; i < MAX_STAT_TYPE; ++i )
		{
			if ( this->GetStat(i) < pCharacterData->GetStat(i) )
			{
				this->SetStat(i, pCharacterData->GetStat(i));
			}
		}
	}

	this->CheckAddInfinityArrow();
	this->WearedItemApplySkill();
	this->MailDBUpdate();

	this->CalculateMajesticStatPoints();

	this->CheckSkill();
}

void Player::LoadDBInfo(PreparedQueryResult result)
{
	if ( !result )
	{
		this->CloseSocket();
		return;
	}

	uint32 account_id = 0;

	FieldReader reader(result->Fetch());

	this->SetGUID(reader.GetUInt32());
	this->SetSlot(reader.GetUInt8());
	this->SetAuthority(reader.GetUInt8());
	this->SetDBClass(reader.GetUInt8());
	this->SetName(reader.GetCString());

	this->GetLevelData(LEVEL_DATA_NORMAL)->SetLevel(reader.GetInt16());
	this->GetLevelData(LEVEL_DATA_MASTER)->SetLevel(reader.GetInt16());
	this->GetLevelData(LEVEL_DATA_NORMAL)->SetExperience(reader.GetInt64());
	this->GetLevelData(LEVEL_DATA_MASTER)->SetExperience(reader.GetInt64());
	this->GetLevelData(LEVEL_DATA_NORMAL)->SetPoints(reader.GetInt32());
	this->GetLevelData(LEVEL_DATA_MASTER)->SetPoints(reader.GetInt32());

	for (uint8 i = 0; i < MAX_STAT_TYPE; ++i)
	{
		this->SetStat(i, reader.GetInt32());
	}

	this->SetWorldId(reader.GetUInt16());
	this->SetX(reader.GetInt16());
	this->SetY(reader.GetInt16());
	this->SetDirection(reader.GetUInt8());
	this->MoneySet(reader.GetUInt32());

	POWER_LOOP(i)
	{
		this->PowerSet(i, reader.GetInt32());
	}

	this->SetFruitPointsAdd(reader.GetInt32());
	this->SetFruitPointsDec(reader.GetInt32());

	this->GetInventory()->SetExpanded(reader.GetUInt8());

	this->SetMuteTime(reader.GetInt64());

	this->SetAdministratorFlag(reader.GetUInt32());

	this->SetPKLevel(reader.GetUInt8());
	this->SetPKCount(reader.GetInt32());
	this->SetPKPoints(reader.GetInt32());
	this->SetFirstTime(reader.GetBool());
	this->SetSantaClausGiftDate(reader.GetInt64());
	this->GetPersonalStore()->SetName(reader.GetBase64String().c_str());
	this->GetPersonalStore()->SetOff(false);

	this->SetGoblinPoints(reader.GetUInt32());
	this->SetKickTime(reader.GetInt64());
	
	account_id = reader.GetUInt32();

	this->SetPostCount(reader.GetInt32());
	this->SetPostDay(reader.GetUInt8());
	this->SetPostMonth(reader.GetUInt8());

	this->SetRuudMoney(reader.GetUInt32());

	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetLevel(reader.GetInt16());
	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetExperience(reader.GetInt64());
	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(reader.GetInt32());
	this->SetCreateDate(reader.GetInt64());
	this->SetHuntingRecordVisible(reader.GetUInt8() == 1 ? true : false);

	if (account_id != this->GetAccountData()->GetGUID())
	{
		this->CloseSocket();
		return;
	}

	if ( this->GetKickTime() <= 0 )
	{
		this->SetKickTime(time(nullptr));
	}
}

void Player::LoadDBItem(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		if ( inventory_range(fields[ITEM_DB_DATA_SLOT].GetUInt32()) && sItemMgr->IsItem(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) && !Item::IsEventArchangelWeapon(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) )
		{
			this->GetInventory()->LoadDBItem(fields, this->GetGUID(), ITEM_BOX_INVENTORY);
		}
		else
		{
			sLog->outError(LOG_PLAYER, "%s Wrong Inventory Item [%u / %u]", this->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
		}
	}
	while(result->NextRow());
}
	
void Player::LoadDBSkill(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		uint8 type = fields[0].GetUInt8();

		if ( type == 0 || type == 1 )
		{
			if ( sSkillMgr->IsSkill(fields[1].GetUInt16()) )
			{
				if ( type == 0 )
				{
					this->MagicAdd(fields[1].GetUInt16(), fields[2].GetUInt8());
				}
				else if ( type == 1 )
				{
					this->AddSkillTreeSkill(fields[1].GetUInt16(), fields[2].GetUInt8());
				}
			}
		}
//		else if ( type >= 2 )
//		{
//			this->AddMajesticSkillTreeSkill(0, type - 2, fields[1].GetUInt16(), fields[2].GetUInt8());
//		}
	}
	while(result->NextRow());
}

void Player::LoadDBGameOption(PreparedQueryResult result)
{
	if ( !result )
		return;

	FieldReader reader(result->Fetch());

	this->GetGameOption()->SetOption(reader.GetUInt8());
	this->GetGameOption()->SetChatWindow(reader.GetUInt8());
	this->GetGameOption()->SetKeyQ(reader.GetUInt8());
	this->GetGameOption()->SetKeyW(reader.GetUInt8());
	this->GetGameOption()->SetKeyE(reader.GetUInt8());
	this->GetGameOption()->SetKeyR(reader.GetUInt8());
	this->GetGameOption()->SetQWERLevel(reader.GetInt32());

	for ( uint8 i = 0; i < SKILL_KEY_COUNT; ++i )
	{
		this->GetGameOption()->SetSkillKey(i, reader.GetUInt16());
	}

	this->GetGameOption()->SetExtraFlag(reader.GetUInt64());
	this->GetGameOption()->SetChangeSkin(reader.GetUInt8());
	this->GetGameOption()->SetAdditionalOptions(reader.GetUInt16());

	for (int32 i = 0; i < BUTTON_KEY_COUNT; ++i)
	{
		this->GetGameOption()->SetButtonLink(i, reader.GetUInt8());
	}
}
	
void Player::LoadDBQuestEvo(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		QuestEvolutionStatus & add_quest = this->quest_evolution[fields[0].GetUInt8()];
		add_quest.SetID(fields[0].GetUInt8());
		add_quest.SetState(fields[1].GetUInt8());

		for ( uint8 i = 0; i < QUEST_EVOLUTION_MONSTER_COUNT; ++i )
			add_quest.SetMonsterCount(i, fields[2 + i].GetInt32());

		if ( add_quest.GetState() > QUEST_EVOLUTION_STATE_CLEAR )
			add_quest.SetState(QUEST_EVOLUTION_STATE_CLEAR);
	}
	while(result->NextRow());
}

void Player::LoadDBFriendList(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		this->AddFriend(fields[0].GetCString(), fields[2].GetBool() ? fields[1].GetUInt16() : -1);
	}
	while(result->NextRow());
}

void Player::LoadDBMailList(PreparedQueryResult result)
{
	if ( !result )
		return;

	MailData Mail;

	do
	{
		Field* fields = result->Fetch();
		
		Mail.Reset();

		Mail.SetFrom(fields[0].GetCString());
		Mail.SetSubject(fields[1].GetBase64String().c_str());
		Mail.SetText(fields[2].GetBase64String().c_str());
		Mail.SetDirection(fields[3].GetUInt8());
		Mail.SetAction(fields[4].GetUInt8());

		const char* data = fields[5].GetCString();

		if ( data )
		{
			Tokens tokens(data, ' ');

			for (uint8 i = 0; i < MAX_PREVIEW_DATA; ++i)
			{
				if (tokens.size() > i)
				{
					Mail.SetPhoto(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}

		Mail.SetWindowGUID(fields[6].GetUInt32());
		Mail.SetDate(fields[7].GetInt64());
		Mail.SetStatus(fields[8].GetUInt8());
		Mail.SetID(fields[9].GetUInt32());
		Mail.SetInUse(true);

		this->MailAdd(Mail);
	}
	while(result->NextRow());

	this->MailDBUpdate();
}

void Player::LoadDBGuild(PreparedQueryResult result)
{
	if ( !result )
	{
		return;
	}

	Field* fields = result->Fetch();

	this->SetGuildID(fields[0].GetUInt32());

	Guild* pGuild = sGuildMgr->GetGuild(this->GetGuildID());

	if ( !pGuild )
	{
		this->SetGuildID(0);
	}
	else
	{
		if ( !pGuild->GetMemberData(this->GetGUID()) )
		{
			this->SetGuildID(0);
		}
	}
}

void Player::LoadDBGen(PreparedQueryResult result, bool update)
{
	if ( update )
	{
		if (!Player::IsPlayerBasicState(this, false, false))
		{
			return;
		}
	}

	if ( !result )
		return;

	Field* fields = result->Fetch();

	this->GetGen()->SetFamily(fields[0].GetUInt8());
	this->GetGen()->SetLevel(fields[1].GetUInt8());
	this->GetGen()->SetContribution(fields[2].GetInt32());
	this->GetGen()->SetRewardDate(fields[3].GetInt64());
	this->GetGen()->SetJoinDate(fields[4].GetInt64());
	this->GetGen()->SetLeftDate(fields[5].GetInt64());
	this->GetGen()->SetRanking(fields[6].GetInt32());

	if (update)
	{
		this->GenUpdateLevel();
	}
}

void Player::LoadDBGenKill(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		GenVictimData & gen_victim = this->m_gen_victim_map[fields[0].GetUInt32()];
		gen_victim.count.set(fields[1].GetUInt32());
		gen_victim.last_hit.set(fields[2].GetInt64());
	}
	while(result->NextRow());
}

void Player::LoadDBHelper(PreparedQueryResult result)
{
	if ( !result )
		return;

	Field* fields = result->Fetch();

	this->GetHelper()->SetOptionFlags(0, fields[0].GetUInt32());
	this->GetHelper()->SetOptionFlags(1, fields[1].GetUInt32());
	this->GetHelper()->SetItemPickFlags(fields[2].GetUInt8());
	this->GetHelper()->SetHuntingRange(fields[3].GetUInt8());
	this->GetHelper()->SetPickRange(fields[4].GetUInt8());
	this->GetHelper()->SetMovementTime(fields[5].GetUInt16());
	this->GetHelper()->SetAttackSkill(0, fields[6].GetUInt16());
	this->GetHelper()->SetAttackSkill(1, fields[7].GetUInt16());
	this->GetHelper()->SetAttackSkill(2, fields[8].GetUInt16());
	this->GetHelper()->SetDelay(0, fields[9].GetUInt16());
	this->GetHelper()->SetDelay(1, fields[10].GetUInt16());
	this->GetHelper()->SetBuffSkill(0, fields[11].GetUInt16());
	this->GetHelper()->SetBuffSkill(1, fields[12].GetUInt16());
	this->GetHelper()->SetBuffSkill(2, fields[13].GetUInt16());
	this->GetHelper()->SetTimeSpaceCasting(fields[14].GetUInt16());
	this->GetHelper()->SetAutoPotPercent(fields[15].GetUInt8());
	this->GetHelper()->SetAutoHealPercent(fields[16].GetUInt8());
	this->GetHelper()->SetPartyHealPercent(fields[17].GetUInt8());
	this->GetHelper()->SetDrainLifePercent(fields[18].GetUInt8());

	Tokenizer tokens(fields[19].GetBase64String(), ';');

    for ( int32 i = 0; i < tokens.size(); ++i )
	{
		if ( i < PLAYER_HELPER_MAX_ITEMS )
		{
			strncpy(this->GetHelper()->item_name[i], tokens[i], strlen(tokens[i]) > HELPER_ITEM_LENGTH ? HELPER_ITEM_LENGTH: strlen(tokens[i]));
		}
	}

	this->GetHelper()->SetItemBuff(0, fields[20].GetUInt16());
	this->GetHelper()->SetItemBuff(1, fields[21].GetUInt16());
	this->GetHelper()->SetItemBuff(2, fields[22].GetUInt16());

	this->GetHelper()->SetLoaded(true);
	
	this->HelperSettingsUpdate();
}

void Player::LoadDBBuff(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();
		uint16 buff_id = fields[0].GetUInt16();
		uint8 effect_1 = (fields[1].GetUInt8());
		float value_1 = static_cast<float>(fields[2].GetInt32());
		uint8 effect_2 = (fields[3].GetUInt8());
		float value_2 = static_cast<float>(fields[4].GetInt32());
		uint8 effect_3 = (fields[5].GetUInt8());
		float value_3 = static_cast<float>(fields[6].GetInt32());
		time_t duration = fields[7].GetInt64();
		uint8 flags = fields[8].GetUInt8();

		if ( sGameServer->IsItemBuffCheck() )
		{
			auto pData = sSkillMgr->GetBuff(buff_id);

			if (pData && sItemMgr->IsItem(pData->GetItem()))
			{
				effect_1 = pData->GetEffect(0);
				effect_2 = pData->GetEffect(1);
				effect_3 = 0;

				value_1 = pData->GetValue(0);
				value_2 = pData->GetValue(1);
				value_3 = 0;
			}
		}
		
		this->AddBuff(buff_id, BuffEffect(effect_1, value_1),BuffEffect(effect_2, value_2),BuffEffect(effect_3, value_3), duration, flags);
	}
	while(result->NextRow());
}

void Player::LoadDBRestriction(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		uint32 i = fields[0].GetUInt32();

		if ( i >= PlayerAction::PLAYER_ACTION_MAX )
			continue;
			
		this->GetRestriction(i)->SetTime(fields[1].GetInt64());
		this->GetRestriction(i)->SetAdmin(fields[2].GetString());
		this->GetRestriction(i)->SetReason(fields[3].GetString());
	}
	while(result->NextRow());
}

void Player::LoadDBStatistics(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		if ( fields[0].GetUInt32() < STATISTIC_MAX )
		{
			this->GetStatisticData(fields[0].GetUInt32())->SetCount(fields[1].GetUInt64());
		}
	}
	while(result->NextRow());
}

void Player::LoadDBItemDelay(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();
			
		ItemDelayData & item_delay = this->m_item_delay[fields[0].GetUInt16()];
		item_delay.SetDate(fields[1].GetInt64());
	}
	while(result->NextRow());
}

void Player::LoadDBMuun(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		if ( MUUN_INVENTORY_RANGE(fields[ITEM_DB_DATA_SLOT].GetUInt32()) && (sItemMgr->GetItemInventoryType(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) == ITEM_INVENTORY_TYPE_MUUN) && sItemMgr->IsItem(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) )
		{
			this->GetMuunInventory()->LoadDBItem(fields, this->GetGUID(), ITEM_BOX_MUUN);
		}
		else
		{
			sLog->outError(LOG_PLAYER, "%s Wrong Muun Item [%u / %u]", this->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
		}
	}
	while(result->NextRow());
}

void Player::LoadDBEventInventory(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		if ( EVENT_INVENTORY_RANGE(fields[ITEM_DB_DATA_SLOT].GetUInt32()) && (sItemMgr->GetItemInventoryType(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) == ITEM_INVENTORY_TYPE_EVENT) )
		{
			this->GetEventInventory()->LoadDBItem(fields, this->GetGUID(), ITEM_BOX_EVENT_INVENTORY);
		}
		else
		{
			sLog->outError(LOG_PLAYER, "%s Wrong Event Item [%u / %u]", this->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
		}
	}
	while(result->NextRow());
}

void Player::LoadDBPentagramInfo(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();
		int32 loop = 1;

		uint8 type = fields[loop++].GetUInt8();
		uint8 index = fields[loop++].GetUInt8();

		if (index >= MAX_PENTAGRAM_JEWEL_INFO)
		{
			continue;
		}

		if (type != PENTAGRAM_JEWEL_TYPE_INVENTORY && type != PENTAGRAM_JEWEL_TYPE_WAREHOUSE)
		{
			continue;
		}

		PentagramJewelInfo* pPentagramJewelInfoStr = (type == PENTAGRAM_JEWEL_TYPE_INVENTORY) ? this->GetPentagramJewelInfoInventoryPtr() : this->GetPentagramJewelInfoWarehousePtr();

		if (pPentagramJewelInfoStr[index].GetIndex() != uint8(-1))
		{
			continue;
		}

		pPentagramJewelInfoStr[index].SetIndex(index);
		pPentagramJewelInfoStr[index].SetType(type);
		pPentagramJewelInfoStr[index].SetAttribute(fields[loop++].GetUInt8());
		pPentagramJewelInfoStr[index].SetItemSection(fields[loop++].GetUInt8());
		pPentagramJewelInfoStr[index].SetItemType(fields[loop++].GetUInt16());
		pPentagramJewelInfoStr[index].SetItemLevel(fields[loop++].GetUInt8());

		SOCKET_SLOT_LOOP(i)
		{
			pPentagramJewelInfoStr[index].GetRankData(i)->SetIndex(fields[loop++].GetUInt8());
			pPentagramJewelInfoStr[index].GetRankData(i)->SetLevel(fields[loop++].GetUInt8());
		}

		pPentagramJewelInfoStr[index].SetSerialServer(fields[loop++].GetUInt16());
		pPentagramJewelInfoStr[index].SetSerial(fields[loop++].GetUInt32());
		pPentagramJewelInfoStr[index].SetDBFlag(fields[loop++].GetUInt32());
		pPentagramJewelInfoStr[index].SetData(0, fields[loop++].GetInt32());
		pPentagramJewelInfoStr[index].SetData(1, fields[loop++].GetInt32());
		pPentagramJewelInfoStr[index].SetData(2, fields[loop++].GetInt32());
	}
	while(result->NextRow());
}

void Player::LoadDBCashShopItemList(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			CashShopItemData* pItem = new CashShopItemData;
			pItem->SetProduct(fields[0].GetUInt32());
			pItem->SetOption(fields[1].GetUInt32());
			pItem->SetSerial(fields[2].GetUInt32());
			pItem->SetSerialCashShop(fields[3].GetUInt32());
			pItem->SetServer(fields[4].GetUInt32());
			pItem->SetDate(fields[6].GetInt64());

			uint8 gift = fields[5].GetUInt8();

			this->CashShopInsertItem(gift, pItem, false);
		}
		while(result->NextRow());
	}
}

void Player::LoadDBCashShopCoin(PreparedQueryResult result)
{
	this->SetAccountDataLoaded(true);

	this->GetAccountData()->SetVIPStatus(-1);
	this->GetAccountData()->SetVIPDuration(0);
	this->GetWarehouse()->SetExpanded(0);
	this->GetWarehouse()->SetExpandedTime(0);
	this->GetAccountData()->SetSpecialCharacter(SPECIAL_CHARACTER_SUMMONER);
	this->SetCredits(0);

	if ( !result )
	{
		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CREATE);
		stmt->setUInt32(0, this->GetAccountData()->GetGUID());
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
		return;
	}

	Field* fields = result->Fetch();

	this->GetAccountData()->SetVIPStatus(fields[0].GetInt32());
	this->GetAccountData()->SetVIPDuration(fields[1].GetInt64());
	this->GetWarehouse()->SetExpanded(fields[2].GetUInt8());
	this->GetWarehouse()->SetExpandedTime(fields[3].GetInt64());
	this->GetAccountData()->SetSpecialCharacter(fields[4].GetUInt16());
	this->SetCredits(fields[5].GetUInt32());
	uint32 web_credits = fields[6].GetUInt32();

	this->SetCashShopDiscountWC(fields[7].GetUInt8());
	this->SetCashShopDiscountGP(fields[8].GetUInt8());
	this->SetCashShopDiscountDate(fields[9].GetInt64());

	if (this->GetCashShopDiscountWC() > 100)
	{
		this->SetCashShopDiscountWC(100);
	}

	if (this->GetCashShopDiscountGP() > 100)
	{
		this->SetCashShopDiscountGP(100);
	}

	this->IncreaseCredits(web_credits);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_COIN_CLEAR);
	stmt->setUInt32(0, web_credits);
	stmt->setUInt32(1, this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	this->SaveDBAccount(trans);

	MuDatabase.CommitTransaction(trans);

	sLog->outInfo(LOG_CASHSHOP, "LoadDBCashShopCoin() - %s Success: [%u, %u]",
		this->BuildLog().c_str(), this->GetCredits(), this->GetGoblinPoints());
}

void Player::LoadDBChatBlock(PreparedQueryResult result)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();
			
		this->ChatBlockAdd(fields[0].GetString().c_str());
	}
	while(result->NextRow());
}

void Player::LoadDBGremoryCase(PreparedQueryResult result, ItemBox box)
{
	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		if ( sItemMgr->IsItem(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) )
		{
			switch (box)
			{
			case ITEM_BOX_GREMORY_CASE_INV:
				this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->LoadDBItem(fields, this->GetGUID(), box);
				break;
			case ITEM_BOX_GREMORY_CASE_ACC:
				this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->LoadDBItem(fields, this->GetAccountData()->GetGUID(), box);
				break;
			case ITEM_BOX_GREMORY_CASE_MOBILE:
				this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->LoadDBItem(fields, this->GetGUID(), box);
				break;
			case ITEM_BOX_GREMORY_CASE_PS:
				this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->LoadDBItem(fields, this->GetGUID(), box);
				break;
			}
		}
		else
		{
			sLog->outError(LOG_PLAYER, "%s Wrong Gremory Case Item [%u / %u]", this->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
		}
	}
	while(result->NextRow());
}

void Player::LoadDBLabyrinth(PreparedQueryResult result)
{
	if ( !result )
	{
		return;
	}

	Field* fields = result->Fetch();
	
	this->GetLabyrinthData()->SetStage(fields[0].GetUInt8());
	this->GetLabyrinthData()->SetID(fields[1].GetUInt8());
	this->GetLabyrinthData()->SetLevel(fields[2].GetUInt16());
	this->GetLabyrinthData()->SetStatus(fields[3].GetUInt8());
	this->GetLabyrinthData()->SetKilledMonsters(fields[4].GetInt32());
	this->GetLabyrinthData()->SetEarnedExperience(fields[5].GetInt64());
	this->GetLabyrinthData()->SetCompletedMissions(fields[6].GetInt32());
	this->GetLabyrinthData()->SetDate(fields[7].GetInt64());
	this->GetLabyrinthData()->SetGoblinState(fields[8].GetUInt8());
	this->GetLabyrinthData()->SetDayFirst(fields[9].GetUInt8());
}
	
void Player::LoadDBLabyrinthZone(PreparedQueryResult result)
{
	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		int32 loop = 0;

		PlayerLabyrinthGate * pGate = new PlayerLabyrinthGate;
		pGate->SetIndex(fields[loop++].GetUInt8());
		pGate->SetSubIndex(fields[loop++].GetUInt8());

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			pGate->SetMissionID(i, fields[loop++].GetUInt8());
		}

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			pGate->SetMissionCount(i, fields[loop++].GetInt32());
		}

		this->GetLabyrinthData()->m_GateList.push_back(pGate);
	} while (result->NextRow());
}

void Player::LoadDBLabyrinthLevel(PreparedQueryResult result)
{
	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		this->GetLabyrinthData()->m_LevelMap[fields[0].GetUInt16()] = fields[1].GetInt32();
	} while (result->NextRow());
}

void Player::LoadDBEventCount(PreparedQueryResult result)
{
	if ( !result )
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		PlayerEventEnterCount * pData = new PlayerEventEnterCount;
		pData->SetEventID(fields[0].GetUInt8());
		pData->SetCount(fields[1].GetUInt8());
		pData->SetDay(fields[2].GetUInt8());

		this->event_enter_count_map[pData->GetEventID()] = pData;
	}
	while(result->NextRow());
}

void Player::LoadDBItemRecovery(PreparedQueryResult result)
{
	if ( !result )
	{
		return;
	}

	bool full = false;

	SQLTransaction trans = MuDatabase.BeginTransaction();
	
	do
	{
		Field* fields = result->Fetch();

		int32 loop = 0;

		Item item = Item();
		item.SetItem(fields[loop++].GetUInt16());
		item.SetLevel(fields[loop++].GetUInt8());
		item.SetDurability(fields[loop++].GetUInt8());
		item.SetSkill(fields[loop++].GetUInt8());
		item.SetLuck(fields[loop++].GetUInt8());
		item.SetOption(fields[loop++].GetUInt8());
		item.SetExe(fields[loop++].GetUInt8());
		item.SetAncient(fields[loop++].GetUInt8());
		item.Set380(fields[loop++].GetUInt8());
		item.SetHarmony(fields[loop++].GetUInt8());

		SOCKET_SLOT_LOOP(i)
		{
			item.SetSocket(i, fields[loop++].GetUInt16());
		}

		item.SetSocketBonus(fields[loop++].GetUInt8());
		item.raw.data_extra[0] = fields[loop++].GetInt32();
		item.raw.data_extra[1] = fields[loop++].GetInt32();
		item.raw.data_extra[2] = fields[loop++].GetInt32();
		item.SetExpireDate(time(nullptr) + (WEEK * 2));

		item.SetSerialServer(fields[loop++].GetUInt16());
		item.SetSerial(fields[loop++].GetUInt32());
		item.SetSerialShop(fields[loop++].GetUInt32());
		item.SetDBFlag(fields[loop++].GetUInt32());
		
		uint32 id = fields[loop++].GetUInt32();

		if ( this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetEmptySlot() != uint8(-1) )
		{
			sItemMgr->ItemSerialCreateGremoryCase(this, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT, 1);

			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_DELETE);
			stmt->setUInt32(0, this->GetGUID());
			stmt->setUInt32(1, id);
			trans->Append(stmt);
		}
		else
		{
			full = true;
			break;
		}
	}
	while(result->NextRow());

	MuDatabase.CommitTransaction(trans);

	if ( full )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Gremory Case is full. You have some items in queue.");
	}
}

void Player::LoadDBItemSell(PreparedQueryResult result)
{
	if ( !result )
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		if ( fields[ITEM_DB_DATA_SLOT].GetUInt32() < MAX_ITEM_SOLD && sItemMgr->IsItem(fields[ITEM_DB_DATA_ENTRY].GetUInt16()) )
		{
			this->GetNpcSell()->LoadDBItem(fields, this->GetGUID(), ITEM_BOX_SELL_NPC);
		}
		else
		{
			sLog->outError(LOG_PLAYER, "%s Wrong Item Sell Item [%u / %u]", this->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
		}
	}
	while(result->NextRow());
}

void Player::LoadDBHuntingRecord(PreparedQueryResult result)
{
	if (!result)
	{
		return;
	}

	Custom::SystemTimer cur_time = Custom::SystemTimer();

	do
	{
		Field* fields = result->Fetch();
		int32 loop = 0;
		uint16 world = fields[loop++].GetUInt16();

		HuntingRecordData* pData = new HuntingRecordData;
		pData->SetYear(fields[loop++].GetUInt16());
		pData->SetMonth(fields[loop++].GetUInt8());
		pData->SetDay(fields[loop++].GetUInt8());
		pData->SetLevel(fields[loop++].GetUInt32());
		pData->SetDuration(fields[loop++].GetUInt32());
		pData->SetDamage(fields[loop++].GetUInt64());
		pData->SetElementalDamage(fields[loop++].GetUInt64());
		pData->SetHealing(fields[loop++].GetUInt32());
		pData->SetKilledCount(fields[loop++].GetUInt32());
		pData->SetEarnedExperience(fields[loop++].GetUInt64());

		if (sGameServer->IsHuntingRecordClear() && cur_time.GetMonth() != pData->GetMonth())
		{
			delete pData;
			continue;
		}

		this->hunting_record_data[world].push_back(pData);
	} while (result->NextRow());
}

void Player::LoadDBMiniBomb(PreparedQueryResult result)
{
	if ( result )
	{
		Field* fields = result->Fetch();

		this->SetMiniBombState(fields[0].GetUInt8());
		this->SetMiniBombCurrentScore(fields[1].GetUInt16());
		this->SetMiniBombTotalScore(fields[2].GetInt32());
		this->SetMiniBombDate(fields[3].GetInt64());
		
		if ( time(nullptr) > (this->GetMiniBombDate() + WEEK) )
		{
			this->SetMiniBombState(MINI_BOMB_GAME_START);
			this->SetMiniBombCurrentScore(0);
			this->SetMiniBombTotalScore(0);
			this->SetMiniBombDate(time(nullptr));
		}
	}
}

void Player::LoadDBMiniBombGrid(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 cell = fields[0].GetUInt8();

			if ( cell < MINI_BOMB_SIZE )
			{
				this->GetMiniBombData(cell)->SetValue(fields[1].GetUInt8());
				this->GetMiniBombData(cell)->SetStatus(fields[2].GetUInt8());
			}
		}
		while(result->NextRow());
	}
}

void Player::LoadDBMuRoomy(PreparedQueryResult result)
{
	if ( result )
	{
		Field* fields = result->Fetch();
	
		this->GetMuRoomyInfo()->SetPlayCard(fields[0].GetBool());
		this->GetMuRoomyInfo()->SetType(fields[1].GetUInt8());
		this->GetMuRoomyInfo()->SetScore(fields[2].GetUInt16());
		this->GetMuRoomyInfo()->SetCardDeckCount(fields[3].GetUInt8());
		this->GetMuRoomyInfo()->SetSpecialCardDeckCount(fields[4].GetUInt8());
		this->GetMuRoomyInfo()->SetDate(fields[5].GetInt64());
	}
}

void Player::LoadDBMuRoomyDeck(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 card_type = fields[0].GetUInt8();
			uint8 card_slot = fields[1].GetUInt8();
			uint8 play_slot = fields[5].GetUInt8();

			switch ( card_type )
			{
			case MU_ROOMY_CARD_TYPE_NORMAL:
				{
					if ( MU_ROOMY_CARD_SLOT_RANGE(card_slot) )
					{
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetColor(fields[2].GetUInt8());
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetNumber(fields[3].GetUInt8());
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetState(fields[4].GetUInt8());
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetSlot(play_slot);

						if ( play_slot < MU_ROOMY_MAX_SLOT )
						{
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardSlot(card_slot);
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardType(card_type);
						}
					}
				} break;

			case MU_ROOMY_CARD_TYPE_SPECIAL:
				{
					if ( MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
					{
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetColor(fields[2].GetUInt8());
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetNumber(fields[3].GetUInt8());
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetState(fields[4].GetUInt8());
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetSlot(play_slot);

						if ( play_slot < MU_ROOMY_MAX_SLOT )
						{
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardSlot(card_slot);
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardType(card_type);
						}
					}
				} break;
			}
		}
		while(result->NextRow());
	}

	if ( time(nullptr) > (this->GetMuRoomyInfo()->GetDate() + WEEK) )
	{
		this->GetMuRoomyInfo()->Init();
	}
}

void Player::LoadDBWarpFavoriteList(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 slot = fields[0].GetUInt8();

			if ( slot < WARP_FAVORITE_COUNT )
			{
				this->GetWarpFavoriteList(slot)->SetData(fields[1].GetUInt16());
				this->GetWarpFavoriteList(slot)->SetWarpID(fields[2].GetUInt16());
			}
		}
		while(result->NextRow());
	}
}

void Player::LoadDBQuestGuided(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			PlayerQuestGuided * pData = new PlayerQuestGuided;
			pData->SetID(fields[0].GetUInt16());
			pData->SetCount(fields[1].GetInt32());
			pData->SetState(fields[2].GetUInt8());
			pData->SetDate(fields[3].GetInt64());

			this->quest_guided_map[pData->GetID()] = pData;

			if ( this->GetQuestGuidedID() <= pData->GetID() )
			{
				this->SetQuestGuidedID(pData->GetID());
			}
		}
		while(result->NextRow());
	}
}

void Player::LoadDBJewelBingo(PreparedQueryResult result)
{
	if ( result )
	{
		Field* fields = result->Fetch();
	
		this->SetJewelBingoState(fields[0].GetUInt8());
		this->SetJewelBingoBox(fields[1].GetUInt8());
		this->SetJewelBingoCount(fields[2].GetUInt8());
		this->SetJewelBingoCurrentJewel(fields[3].GetUInt8());
		this->SetJewelBingoScore1(fields[4].GetInt32());
		this->SetJewelBingoScore2(fields[5].GetInt32());
		this->SetJewelBingoScore3(fields[6].GetInt32());
		this->SetJewelBingoDate(fields[7].GetInt64());
	}
}
	
void Player::LoadDBJewelBingoGrid(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 type = fields[0].GetUInt8();
			uint8 slot = fields[1].GetUInt8();
			uint8 value = fields[2].GetUInt8();

			if ( type >= 3 )
			{
				continue;
			}
			
			switch ( type )
			{
			case 0:
				{
					if ( slot < JEWEL_BINGO_GRID_SIZE )
					{
						this->GetJewelBingoGrid(slot)->SetType(type);
						this->GetJewelBingoGrid(slot)->SetSlot(slot);
						this->GetJewelBingoGrid(slot)->SetValue(value);
					}
				} break;

			case 1:
				{
					if ( slot < JEWEL_BINGO_JEWEL_COUNT )
					{
						this->GetJewelBingoJewel(slot)->SetType(type);
						this->GetJewelBingoJewel(slot)->SetSlot(slot);
						this->GetJewelBingoJewel(slot)->SetValue(value);
					}
				} break;

			case 2:
				{
					if ( slot < JEWEL_BINGO_MATCHING_SIZE )
					{
						this->GetJewelBingoMatching(slot)->SetType(type);
						this->GetJewelBingoMatching(slot)->SetSlot(slot);
						this->GetJewelBingoMatching(slot)->SetValue(value);
					}
				} break;
			}
		}
		while(result->NextRow());
	}

	if ( time(nullptr) > (this->GetJewelBingoDate() + WEEK) )
	{
		this->SetJewelBingoState(0);
		this->SetJewelBingoDate(0);
	}
}

void Player::LoadDBDate(PreparedQueryResult result)
{
	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint32 id = fields[0].GetUInt32();
			time_t date = fields[1].GetInt64();
			
			if ( id >= PLAYER_DATE_MAX )
			{
				continue;
			}

			this->SetDate(id, date);
		}
		while(result->NextRow());
	}
}

void Player::LoadDBMajesticTree(PreparedQueryResult result)
{
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			this->AddMajesticSkillTreeSkill(fields[0].GetUInt8(), fields[1].GetUInt8(), fields[2].GetUInt16(), fields[3].GetUInt8());
		} while (result->NextRow());
	}
}

void Player::LoadDBMajesticStats(PreparedQueryResult result)
{
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 id = fields[0].GetUInt8();

			if (id < MAX_SKILL_TREE_MAJESTIC_PASSIVE)
			{
				this->SetMajesticStat(id, fields[1].GetUInt16());
			}
		} while (result->NextRow());
	}
}

void Player::LoadDBNumericBaseball(PreparedQueryResult result)
{
	if (result)
	{
		Field* fields = result->Fetch();

		this->SetNumericBaseballState(fields[0].GetUInt8());
		this->SetNumericBaseballScore(fields[1].GetInt32());

		for (int32 i = 0; i < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++i)
		{
			this->SetNumericBaseballNumber(i, fields[2 +  i].GetUInt8());
		}

		const char* numbers = fields[5].GetCString();

		if (numbers)
		{
			Tokens tokens(numbers, ' ');

			for (int32 i = 0; i < (NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW); ++i)
			{
				if (tokens.size() > i)
				{
					this->SetNumericBaseballUsedNumber(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}

		const char* strikes = fields[6].GetCString();

		if (strikes)
		{
			Tokens tokens(strikes, ' ');

			for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
			{
				if (tokens.size() > i)
				{
					this->SetNumericBaseballStrike(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}

		const char* balls = fields[7].GetCString();

		if (balls)
		{
			Tokens tokens(balls, ' ');

			for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
			{
				if (tokens.size() > i)
				{
					this->SetNumericBaseballBall(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}
	}
}

void Player::LoadDBMixRecovery(PreparedQueryResult result)
{
	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		if (shop_range(fields[ITEM_DB_DATA_SLOT].GetUInt32()) && sItemMgr->IsItem(fields[ITEM_DB_DATA_ENTRY].GetUInt16()))
		{
			this->GetMixRecoveryInventory()->LoadDBItem(fields, this->GetGUID(), ITEM_BOX_MIX_RECOVERY);
		}
		else
		{
			sLog->outError(LOG_PLAYER, "%s Wrong Mix Recovery Item [%u / %u]", this->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
		}
	} while (result->NextRow());
}

void Player::LoadDBQuestMU(PreparedQueryResult result)
{
	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		QuestMUData* pData = new QuestMUData;
		pData->SetID(fields[0].GetUInt16());
		pData->SetState(fields[1].GetUInt8());
		pData->SetObjective(fields[2].GetUInt16());
		pData->SetDate(fields[3].GetInt64());

		this->m_QuestMUDataMap[pData->GetID()] = pData;
	} while (result->NextRow());
}

void Player::LoadMonsterSoul(PreparedQueryResult result)
{
	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		uint32 type = fields[0].GetUInt32();

		MonsterSoulData* pData = this->GetMonsterSoulData(type);

		if (!pData)
		{
			pData = new MonsterSoulData;
			pData->SetType(type);
			this->m_MonsterSoulDataMap[type] = pData;
		}

		pData->m_AmountMap[fields[1].GetUInt32()] = fields[2].GetInt32();
	} while (result->NextRow());
}

void Player::LoadDBItemPersonalStore(PreparedQueryResult result)
{
	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		if (sItemMgr->IsItem(fields[ITEM_DB_DATA_ENTRY].GetUInt16()))
		{
			this->GetPersonalStore()->LoadDBItem(fields, this->GetGUID(), ITEM_BOX_PERSONAL_STORE);
		}
		else
		{
			sLog->outError(LOG_PLAYER, "%s Wrong Personal Store Item [%u / %u]", this->BuildLog().c_str(), fields[ITEM_DB_DATA_SLOT].GetUInt32(), fields[ITEM_DB_DATA_ENTRY].GetUInt16());
		}
	} while (result->NextRow());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// SAVE TO DB /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::SaveCharacter(uint8 operation)
{
	if ( !this->GetGUID() || this->IsServerQuit() )
		return;

	this->GetTimer(PLAYER_TIMER_SAVE_DB)->Start();

	SQLTransaction trans = MuDatabase.BeginTransaction();

	this->SaveDBInfo(trans);
	this->SaveDBItem(trans);
	this->SaveDBSkill(trans);
	//this->SaveDBGameOption(trans);
	this->SaveDBQuestEvo(trans);
	this->SaveDBFriendList(trans);
	this->SaveDBMailList(trans);
	this->SaveDBGen(trans);
	this->SaveDBGenKill(trans);
	this->SaveDBHelper(trans);
	this->SaveDBBuff(trans);
	this->SaveDBRestriction(trans);
	this->SaveDBStatistics(trans);
	this->SaveDBItemDelay(trans);
	this->SaveDBCashShopItem(trans);
	this->SaveDBAccount(trans);
	this->SaveDBPentagramInfo(trans);
	this->SaveDBChatBlock(trans);

	this->SaveDBHuntingRecord(trans);
	this->SaveDBLabyrinth(trans);
	this->SaveDBLabyrinthZone(trans);
	this->SaveDBLabyrinthLevel(trans);
	this->SaveDBEventCount(trans);
	this->SaveDBMiniBomb(trans);
	this->SaveDBMuRoomy(trans);
	this->SaveDBWarpFavoriteList(trans);
	this->SaveDBQuestGuided(trans);
	this->SaveDBJewelBingo(trans);
	this->SaveDBDate(trans);
	this->SaveDBMajesticTree(trans);
	this->SaveDBMajesticStats(trans);
	this->SaveDBNumericBaseball(trans);
	this->SaveDBQuestMU(trans);
	this->SaveDBMonsterSoul(trans);

	SQLTransactionCallBack save_callback(new PlayerSaveTransaction(this, operation));

	MuDatabase.CommitTransaction(trans, save_callback);
}

void Player::SaveDBInfo(SQLTransaction& trans)
{
	PreparedStatement * stmt = MuDatabase.GetPreparedStatement(CHARACTER_UPDATE_INFO);

	stmt->setUInt8(this->GetAuthority());
	stmt->setUInt8(this->GetDBClass());
	stmt->setString(this->GetName());
	stmt->setInt16(this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	stmt->setInt16(this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel());
	stmt->setInt64(this->GetLevelData(LEVEL_DATA_NORMAL)->GetExperience());
	stmt->setInt64(this->GetLevelData(LEVEL_DATA_MASTER)->GetExperience());
	stmt->setInt32(this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints());
	stmt->setInt32(this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints());

	for (uint8 i = 0; i < MAX_STAT_TYPE; ++i)
	{
		stmt->setUInt32(this->GetStat(i));
	}

	stmt->setUInt16(this->GetWorldId());
	stmt->setInt16(this->GetX());
	stmt->setInt16(this->GetY());
	stmt->setUInt8(this->GetDirection());
	stmt->setUInt32(this->MoneyGet());

	for (uint8 i = 0; i < POWER_MAX; ++i)
	{
		stmt->setInt32(this->PowerGet(i));
	}

	stmt->setInt32(this->GetFruitPointsAdd());
	stmt->setInt32(this->GetFruitPointsDec());
	stmt->setUInt8(this->GetInventory()->GetExpanded());
	stmt->setInt64(this->GetMuteTime());
	stmt->setUInt32(this->GetAdministratorFlag());
	stmt->setUInt8(this->GetPKLevel());
	stmt->setInt32(this->GetPKCount());
	stmt->setInt32(this->GetPKPoints());
	stmt->setUInt8(this->IsFirstTime());
	stmt->setInt64(this->GetSantaClausGiftDate());
	stmt->setString(base64::encode((uint8*)this->GetPersonalStore()->GetName(), PERSONAL_STORE_NAME_LENGTH));
	stmt->setBool(this->GetPersonalStore()->IsOpen() && this->GetPersonalStore()->IsOff());
	stmt->setUInt32(this->GetGoblinPoints());
	stmt->setInt64(time(nullptr));
	stmt->setInt64(this->GetKickTime());
	stmt->setInt32(this->GetPostCount());
	stmt->setUInt8(this->GetPostDay());
	stmt->setUInt8(this->GetPostMonth());
	stmt->setUInt32(this->GetRuudMoney());
	stmt->setInt16(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel());
	stmt->setInt64(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetExperience());
	stmt->setInt32(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints());
	stmt->setUInt8(this->IsHuntingRecordVisible() ? 1 : 0);
	stmt->setUInt32(this->GetGUID());

	trans->Append(stmt);

	this->GetWarehouse()->SaveDBData(trans);
}

void Player::SaveDBItem(SQLTransaction& trans)
{
	this->SaveDBItem(trans, ITEM_SAVE_RULE_REMOVE);
	this->SaveDBItem(trans, ITEM_SAVE_RULE_ADD);
}
	
void Player::SaveDBSkill(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_SKILL);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == 0 )
			continue;

		if ( this->GetSkill(i)->IsWeapon() )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_SKILL);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(0);
		stmt->setUInt16(this->GetSkill(i)->GetSkill());
		stmt->setUInt8(this->GetSkill(i)->GetLevel());
		
		trans->Append(stmt);
	}

	for ( PlayerSkillTreeMap::const_iterator it = this->skill_tree_map.begin(); it != this->skill_tree_map.end(); ++it )
	{
		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_SKILL);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(1);
		stmt->setUInt16(it->second->GetSkill());
		stmt->setUInt8(it->second->GetLevel());
		
		trans->Append(stmt);
	}
}

void Player::SaveDBGameOption(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GAMEOPTION);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_GAME_OPTION);
	stmt->setUInt32(this->GetGUID());
	stmt->setUInt8(this->GetGameOption()->GetOption());
	stmt->setUInt8(this->GetGameOption()->GetChatWindow());
	stmt->setUInt8(this->GetGameOption()->GetKeyQ());
	stmt->setUInt8(this->GetGameOption()->GetKeyW());
	stmt->setUInt8(this->GetGameOption()->GetKeyE());
	stmt->setUInt8(this->GetGameOption()->GetKeyR());
	stmt->setInt32(this->GetGameOption()->GetQWERLevel());

	for (int32 i = 0; i < SKILL_KEY_COUNT; ++i)
	{
		stmt->setUInt16(this->GetGameOption()->GetSkillKey(i));
	}

	stmt->setUInt64(this->GetGameOption()->GetExtraFlag());
	stmt->setUInt8(this->GetGameOption()->GetChangeSkin());
	stmt->setUInt16(this->GetGameOption()->GetAdditionalOptions());

	for (int32 i = 0; i < BUTTON_KEY_COUNT; ++i)
	{
		stmt->setUInt8(this->GetGameOption()->GetButtonLink(i));
	}

	trans->Append(stmt);
}
	
void Player::SaveDBQuestEvo(SQLTransaction& trans)
{
	PreparedStatement *stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_QUEST_EVOLUTION);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( QuestEvolutionStatusMap::const_iterator it = this->quest_evolution.begin(); it != this->quest_evolution.end(); ++it )
	{
		if ( it->second.GetState() == QUEST_EVOLUTION_STATE_NONE )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_QUEST_EVOLUTION);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, it->second.GetID());
		stmt->setUInt8(2, it->second.GetState());

		for ( uint8 i = 0; i < QUEST_EVOLUTION_MONSTER_COUNT; ++i )
			stmt->setInt32(3 + i, it->second.GetMonsterCount(i));

		trans->Append(stmt);
	}
}

void Player::SaveDBFriendList(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_FRIEND);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	FRIEND_LOOP(i)
	{
		if ( !strlen(this->GetFriend(i)->GetName()) )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_FRIEND);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setString(1, this->GetFriend(i)->GetName());
		trans->Append(stmt);
	}
}

void Player::SaveDBMailList(SQLTransaction& trans)
{
	MAIL_LOOP(i)
	{
		if ( !this->GetMailData(i)->IsInUse() )
			continue;

		auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_UPDATE);
		stmt->setUInt8(0, this->GetMailData(i)->GetStatus());
		stmt->setUInt32(1, this->GetMailData(i)->GetID());

		trans->Append(stmt);
	}
}

void Player::SaveDBGen(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GENS);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_GEN);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetGen()->GetFamily());
	stmt->setUInt8(2, this->GetGen()->GetLevel());
	stmt->setInt32(3, this->GetGen()->GetContribution());
	stmt->setInt64(4, this->GetGen()->GetRewardDate());
	stmt->setInt64(5, this->GetGen()->GetJoinDate());
	stmt->setInt64(6, this->GetGen()->GetLeftDate());
	
	trans->Append(stmt);
}

void Player::SaveDBGenKill(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GENS_KILL);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( GenVictimDataMap::const_iterator it = this->m_gen_victim_map.begin(); it != this->m_gen_victim_map.end(); ++it )
	{
		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_GEN_KILL);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt32(1, it->first);
		stmt->setUInt32(2, it->second.count.get());
		stmt->setInt64(3, it->second.last_hit.get());

		trans->Append(stmt);
	}
}

void Player::SaveDBHelper(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_HELPER);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	this->HelperSettingsUpdate();

	stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_HELPER);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt32(1, this->GetHelper()->GetOptionFlags(0));
	stmt->setUInt32(2, this->GetHelper()->GetOptionFlags(1));
	stmt->setUInt8(3, this->GetHelper()->GetItemPickFlags());
	stmt->setUInt8(4, this->GetHelper()->GetHuntingRange());
	stmt->setUInt8(5, this->GetHelper()->GetPickRange());
	stmt->setUInt16(6, this->GetHelper()->GetMovementTime());
	stmt->setUInt16(7, this->GetHelper()->GetAttackSkill(0));
	stmt->setUInt16(8, this->GetHelper()->GetAttackSkill(1));
	stmt->setUInt16(9, this->GetHelper()->GetAttackSkill(2));
	stmt->setUInt16(10, this->GetHelper()->GetDelay(0));
	stmt->setUInt16(11, this->GetHelper()->GetDelay(1));
	stmt->setUInt16(12, this->GetHelper()->GetBuffSkill(0));
	stmt->setUInt16(13, this->GetHelper()->GetBuffSkill(1));
	stmt->setUInt16(14, this->GetHelper()->GetBuffSkill(2));
	stmt->setUInt16(15, this->GetHelper()->GetTimeSpaceCasting());
	stmt->setUInt8(16, this->GetHelper()->GetAutoPotPercent());
	stmt->setUInt8(17, this->GetHelper()->GetAutoHealPercent());
	stmt->setUInt8(18, this->GetHelper()->GetPartyHealPercent());
	stmt->setUInt8(19, this->GetHelper()->GetDrainLifePercent());

	std::ostringstream sstream;

	for ( uint8 i = 0; i < PLAYER_HELPER_MAX_ITEMS; ++i )
	{
		if ( strlen(this->GetHelper()->item_name[i]) )
		{
			sstream << this->GetHelper()->item_name[i] << ";";
		}
	}

	stmt->setString(20, base64::encode((uint8*)sstream.str().c_str(), sstream.str().size()));

	stmt->setUInt16(21, this->GetHelper()->GetItemBuff(0));
	stmt->setUInt16(22, this->GetHelper()->GetItemBuff(1));
	stmt->setUInt16(23, this->GetHelper()->GetItemBuff(2));

	trans->Append(stmt);
}

void Player::SaveDBBuff(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_BUFF);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DELETE_BUFF);
	stmt->setUInt32(this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if (data->GetID() == 0)
			continue;

		if (!data->IsFlag(BUFF_FLAG_DB_STORE))
			continue;

		if (data->IsFlag(BUFF_FLAG_ACCOUNT))
		{
			stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_INSERT_BUFF);
			stmt->setUInt32(this->GetAccountData()->GetGUID());
		}
		else
		{
			stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_BUFF);
			stmt->setUInt32(this->GetGUID());
		}

		stmt->setUInt16(data->GetID());
		stmt->setUInt8(data->GetEffect(0)->GetEffect());
		stmt->setInt32(static_cast<int32>(data->GetEffect(0)->GetValue()));
		stmt->setUInt8(data->GetEffect(1)->GetEffect());
		stmt->setInt32(static_cast<int32>(data->GetEffect(1)->GetValue()));
		stmt->setUInt8(data->GetEffect(2)->GetEffect());
		stmt->setInt32(static_cast<int32>(data->GetEffect(2)->GetValue()));
		stmt->setInt64(data->GetDuration());
		stmt->setUInt8(data->GetFlag());

		trans->Append(stmt);
	}
}

void Player::SaveDBRestriction(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_RESTRICTION);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for ( int32 i = 0; i < PlayerAction::PLAYER_ACTION_MAX; i++ )
	{
		if ( !this->GetRestriction(i)->IsActive() )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_RESTRICTION);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt32(i);
		stmt->setInt64(this->GetRestriction(i)->GetTime());
		stmt->setString(this->GetRestriction(i)->GetAdmin());
		stmt->setString(this->GetRestriction(i)->GetReason());

		trans->Append(stmt);
	}
}

void Player::SaveDBStatistics(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_STATISTICS);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	STATISTIC_LOOP(i)
	{
		if ( this->GetStatisticData(i)->GetCount() > 0 )
		{
			stmt = MuDatabase.GetPreparedStatement(QUERY_CHARACTER_STATISTICS_INSERT);
			stmt->setUInt32(this->GetGUID());
			stmt->setUInt32(i);
			stmt->setUInt64(this->GetStatisticData(i)->GetCount());
			trans->Append(stmt);
		}
	}
}

void Player::SaveDBItemDelay(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_ITEM_DELAY);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for ( ItemDelayDataMap::const_iterator it = this->m_item_delay.begin(); it != this->m_item_delay.end(); it++ )
	{
		if ( it->second.GetDate() <= 0 )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_ITEM_DELAY);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt16(it->first);
		stmt->setInt64(it->second.GetDate());

		trans->Append(stmt);
	}
}

void Player::SaveDBRegister(uint32 register_id, std::string register_data, Item const* item)
{
	/*PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CHARACTER_REGISTER_INSERT);

	stmt->setUInt32(0, this->GetGUID());
	stmt->setString(1, this->GetName());
	stmt->setUInt32(2, register_id);
	stmt->setString(3, register_data);
	stmt->setString(4, item ? item->BuildRegister(): "");
	stmt->setString(5, this->GetAccountData()->GetIP());

	MuDatabase.Execute(stmt);*/
}

void Player::SaveDBItem(SQLTransaction & trans, uint8 rules)
{
	this->GetInventory()->SaveDB(trans, rules);
	this->GetWarehouse()->SaveDB(trans, rules);
	this->GetMuunInventory()->SaveDB(trans, rules);
	this->GetEventInventory()->SaveDB(trans, rules);
	for (int32 i = 0; i < GREMORY_CASE_TYPE_MAX; ++i)
	{
		this->GetGremoryCase(i)->SaveDB(trans, rules);
	}
	this->GetNpcSell()->SaveDB(trans, rules);
	this->GetMixRecoveryInventory()->SaveDB(trans, rules);
	this->GetPersonalStore()->SaveDB(trans, rules);
}

void Player::SaveDBPentagramInfo(SQLTransaction & trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_DELETE);
	stmt->setUInt32(this->GetGUID());
	stmt->setUInt8(PENTAGRAM_JEWEL_TYPE_INVENTORY);
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_DELETE);
	stmt->setUInt32(this->GetAccountData()->GetGUID());
	stmt->setUInt8(PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
	trans->Append(stmt);

	PentagramJewelInfo const* pPentagramJewelInfo = nullptr;

	for (int32 i = 0; i < MAX_PENTAGRAM_JEWEL_INFO; ++i)
	{
		if (pPentagramJewelInfo = this->GetPentagramJewelInfoInventory(i))
		{
			if (pPentagramJewelInfo->GetIndex() != uint8(-1))
			{
				stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_INSERT);
				stmt->setUInt32(this->GetGUID());
				stmt->setUInt8(PENTAGRAM_JEWEL_TYPE_INVENTORY);
				stmt->setUInt8(pPentagramJewelInfo->GetIndex());
				stmt->setUInt8(pPentagramJewelInfo->GetAttribute());
				stmt->setUInt8(pPentagramJewelInfo->GetItemSection());
				stmt->setUInt16(pPentagramJewelInfo->GetItemType());
				stmt->setUInt8(pPentagramJewelInfo->GetItemLevel());
				
				SOCKET_SLOT_LOOP(n)
				{
					stmt->setUInt8(pPentagramJewelInfo->GetRankData(n)->GetIndex());
					stmt->setUInt8(pPentagramJewelInfo->GetRankData(n)->GetLevel());
				}

				stmt->setUInt16(pPentagramJewelInfo->GetSerialServer());
				stmt->setUInt32(pPentagramJewelInfo->GetSerial());
				stmt->setUInt32(pPentagramJewelInfo->GetDBFlag());
				stmt->setInt32(pPentagramJewelInfo->GetData(0));
				stmt->setInt32(pPentagramJewelInfo->GetData(1));
				stmt->setInt32(pPentagramJewelInfo->GetData(2));

				trans->Append(stmt);
			}
		}

		if (pPentagramJewelInfo = this->GetPentagramJewelInfoWarehouse(i))
		{
			if (pPentagramJewelInfo->GetIndex() != uint8(-1))
			{
				stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_INSERT);
				stmt->setUInt32(this->GetAccountData()->GetGUID());
				stmt->setUInt8(PENTAGRAM_JEWEL_TYPE_WAREHOUSE);
				stmt->setUInt8(pPentagramJewelInfo->GetIndex());
				stmt->setUInt8(pPentagramJewelInfo->GetAttribute());
				stmt->setUInt8(pPentagramJewelInfo->GetItemSection());
				stmt->setUInt16(pPentagramJewelInfo->GetItemType());
				stmt->setUInt8(pPentagramJewelInfo->GetItemLevel());

				SOCKET_SLOT_LOOP(n)
				{
					stmt->setUInt8(pPentagramJewelInfo->GetRankData(n)->GetIndex());
					stmt->setUInt8(pPentagramJewelInfo->GetRankData(n)->GetLevel());
				}

				stmt->setUInt16(pPentagramJewelInfo->GetSerialServer());
				stmt->setUInt32(pPentagramJewelInfo->GetSerial());
				stmt->setUInt32(pPentagramJewelInfo->GetDBFlag());
				stmt->setInt32(pPentagramJewelInfo->GetData(0));
				stmt->setInt32(pPentagramJewelInfo->GetData(1));
				stmt->setInt32(pPentagramJewelInfo->GetData(2));

				trans->Append(stmt);
			}
		}
	}
}

void Player::SaveDBChatBlock(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_CHAT_BLOCK);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for ( PlayerChatBlockList::const_iterator it = this->chat_block_list.begin(); it != this->chat_block_list.end(); ++it )
	{
		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_CHAT_BLOCK);
		stmt->setUInt32(this->GetGUID());
		stmt->setString(*it);

		trans->Append(stmt);
	}
}

void Player::SaveDBCashShopItem(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_DELETE);
	stmt->setUInt32(0, this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	for (CashShopItemDataList::const_iterator itr = this->m_CashShopItemList.begin(); itr != this->m_CashShopItemList.end(); ++itr)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_INSERT);
		stmt->setUInt32(this->GetAccountData()->GetGUID());
		stmt->setUInt32(pItemData->GetProduct());
		stmt->setUInt32(pItemData->GetOption());
		stmt->setUInt32(pItemData->GetSerial());
		stmt->setUInt32(pItemData->GetSerialCashShop());
		stmt->setUInt16(pItemData->GetServer());
		stmt->setUInt8(0);
		stmt->setInt64(pItemData->GetDate());
		trans->Append(stmt);
	}

	for (CashShopItemDataList::const_iterator itr = this->m_CashShopGiftList.begin(); itr != this->m_CashShopGiftList.end(); ++itr)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_INSERT);
		stmt->setUInt32(this->GetAccountData()->GetGUID());
		stmt->setUInt32(pItemData->GetProduct());
		stmt->setUInt32(pItemData->GetOption());
		stmt->setUInt32(pItemData->GetSerial());
		stmt->setUInt32(pItemData->GetSerialCashShop());
		stmt->setUInt16(pItemData->GetServer());
		stmt->setUInt8(1);
		stmt->setInt64(pItemData->GetDate());
		trans->Append(stmt);
	}
}

void Player::SaveDBHuntingRecord(SQLTransaction& trans)
{
	Custom::SystemTimer cur_time = Custom::SystemTimer();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( HuntingRecordDataMap::const_iterator itr = this->hunting_record_data.begin(); itr != this->hunting_record_data.end(); ++itr )
	{
		for ( HuntingRecordDataList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			HuntingRecordData const* pData = (*it);

			if ( !pData )
			{
				continue;
			}

			if ( sGameServer->IsHuntingRecordClear() && cur_time.GetMonth() != pData->GetMonth() )
			{
				continue;
			}

			stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_INSERT);
			stmt->setUInt32(this->GetGUID());
			stmt->setUInt16(itr->first);
			stmt->setUInt16(pData->GetYear());
			stmt->setUInt8(pData->GetMonth());
			stmt->setUInt8(pData->GetDay());
			stmt->setUInt32(pData->GetLevel());
			stmt->setUInt32(pData->GetDuration());
			stmt->setUInt64(pData->GetDamage());
			stmt->setUInt64(pData->GetElementalDamage());
			stmt->setUInt32(pData->GetHealing());
			stmt->setUInt32(pData->GetKilledCount());
			stmt->setUInt64(pData->GetEarnedExperience());

			trans->Append(stmt);
		}
	}
}

void Player::SaveDBLabyrinth(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_INSERT);
	stmt->setUInt32(this->GetGUID());
	stmt->setUInt8(this->GetLabyrinthData()->GetStage());
	stmt->setUInt8(this->GetLabyrinthData()->GetID());
	stmt->setUInt16(this->GetLabyrinthData()->GetLevel());
	stmt->setUInt8(this->GetLabyrinthData()->GetStatus());
	stmt->setInt32(this->GetLabyrinthData()->GetKilledMonsters());
	stmt->setInt64(this->GetLabyrinthData()->GetEarnedExperience());
	stmt->setInt32(this->GetLabyrinthData()->GetCompletedMissions());
	stmt->setInt64(this->GetLabyrinthData()->GetDate());
	stmt->setUInt8(this->GetLabyrinthData()->GetGoblinState());
	stmt->setUInt8(this->GetLabyrinthData()->GetDayFirst());

	trans->Append(stmt);
}
	
void Player::SaveDBLabyrinthZone(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for (PlayerLabyrinthGateList::const_iterator it = this->GetLabyrinthData()->m_GateList.begin(); it != this->GetLabyrinthData()->m_GateList.end(); ++it)
	{
		PlayerLabyrinthGate const* pGate = (*it);

		if (!pGate)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(pGate->GetIndex());
		stmt->setUInt8(pGate->GetSubIndex());

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			stmt->setUInt8(pGate->GetMissionID(i));
		}

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			stmt->setInt32(pGate->GetMissionCount(i));
		}

		trans->Append(stmt);
	}
}

void Player::SaveDBLabyrinthLevel(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for (PlayerLabyrinthLevelMap::const_iterator it = this->GetLabyrinthData()->m_LevelMap.begin(); it != this->GetLabyrinthData()->m_LevelMap.end(); ++it)
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt16(1, it->first);
		stmt->setInt32(2, it->second);
		trans->Append(stmt);
	}
}

void Player::SaveDBEventCount(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( PlayerEventEnterCountMap::const_iterator itr = this->event_enter_count_map.begin(); itr != this->event_enter_count_map.end(); ++itr )
	{
		PlayerEventEnterCount const* pData = itr->second;

		if ( !pData )
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, pData->GetEventID());
		stmt->setUInt8(2, pData->GetCount());
		stmt->setUInt8(3, pData->GetDay());
		trans->Append(stmt);
	}
}

void Player::SaveDBMiniBomb(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetMiniBombState());
	stmt->setUInt16(2, this->GetMiniBombCurrentScore());
	stmt->setInt32(3, this->GetMiniBombTotalScore());
	stmt->setInt64(4, this->GetMiniBombDate());
	trans->Append(stmt);

	if ( this->GetMiniBombState() == MINI_BOMB_GAME_START )
	{
		return;
	}

	for ( int32 i = 0; i < MINI_BOMB_SIZE; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, i);
		stmt->setUInt8(2, this->GetMiniBombData(i)->GetValue());
		stmt->setUInt8(3, this->GetMiniBombData(i)->GetStatus());
		trans->Append(stmt);
	}
}

void Player::SaveDBMuRoomy(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetMuRoomyInfo()->IsPlayCard() ? 1 : 0);
	stmt->setUInt8(2, this->GetMuRoomyInfo()->GetType());
	stmt->setUInt16(3, this->GetMuRoomyInfo()->GetScore());
	stmt->setUInt8(4, this->GetMuRoomyInfo()->GetCardDeckCount());
	stmt->setUInt8(5, this->GetMuRoomyInfo()->GetSpecialCardDeckCount());
	stmt->setInt64(6, this->GetMuRoomyInfo()->GetDate());
	trans->Append(stmt);

	if ( !this->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	for ( int32 i = 0; i < MU_ROOMY_MAX_CARD; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, MU_ROOMY_CARD_TYPE_NORMAL);
		stmt->setUInt8(2, i);
		stmt->setUInt8(3, this->GetMuRoomyInfo()->GetCardInfo(i)->GetColor());
		stmt->setUInt8(4, this->GetMuRoomyInfo()->GetCardInfo(i)->GetNumber());
		stmt->setUInt8(5, this->GetMuRoomyInfo()->GetCardInfo(i)->GetState());
		stmt->setUInt8(6, this->GetMuRoomyInfo()->GetCardInfo(i)->GetSlot());
		trans->Append(stmt);
	}

	for ( int32 i = 0; i < MU_ROOMY_MAX_CARD_SPECIAL; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, MU_ROOMY_CARD_TYPE_SPECIAL);
		stmt->setUInt8(2, i);
		stmt->setUInt8(3, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetColor());
		stmt->setUInt8(4, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetNumber());
		stmt->setUInt8(5, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetState());
		stmt->setUInt8(6, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetSlot());
		trans->Append(stmt);
	}
}

void Player::SaveDBWarpFavoriteList(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( int32 i = 0; i < WARP_FAVORITE_COUNT; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, i);
		stmt->setUInt16(2, this->GetWarpFavoriteList(i)->GetData());
		stmt->setUInt16(3, this->GetWarpFavoriteList(i)->GetWarpID());
		trans->Append(stmt);
	}
}

void Player::SaveDBQuestGuided(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( PlayerQuestGuidedMap::const_iterator itr = this->quest_guided_map.begin(); itr != this->quest_guided_map.end(); ++itr )
	{
		PlayerQuestGuided const* pData = itr->second;

		if ( !pData )
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt16(1, pData->GetID());
		stmt->setInt32(2, pData->GetCount());
		stmt->setUInt8(3, pData->GetState());
		stmt->setInt64(4, pData->GetDate());
		trans->Append(stmt);
	}
}

void Player::SaveDBJewelBingo(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetJewelBingoState());
	stmt->setUInt8(2, this->GetJewelBingoBox());
	stmt->setUInt8(3, this->GetJewelBingoCount());
	stmt->setUInt8(4, this->GetJewelBingoCurrentJewel());
	stmt->setInt32(5, this->GetJewelBingoScore1());
	stmt->setInt32(6, this->GetJewelBingoScore2());
	stmt->setInt32(7, this->GetJewelBingoScore3());
	stmt->setInt64(8, this->GetJewelBingoDate());
	trans->Append(stmt);

	if ( !this->GetJewelBingoState() )
	{
		return;
	}

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, this->GetJewelBingoGrid(i)->GetType());
		stmt->setUInt8(2, this->GetJewelBingoGrid(i)->GetSlot());
		stmt->setUInt8(3, this->GetJewelBingoGrid(i)->GetValue());
		trans->Append(stmt);
	}

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, this->GetJewelBingoJewel(i)->GetType());
		stmt->setUInt8(2, this->GetJewelBingoJewel(i)->GetSlot());
		stmt->setUInt8(3, this->GetJewelBingoJewel(i)->GetValue());
		trans->Append(stmt);
	}

	for ( int32 i = 0; i < JEWEL_BINGO_MATCHING_SIZE; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, this->GetJewelBingoMatching(i)->GetType());
		stmt->setUInt8(2, this->GetJewelBingoMatching(i)->GetSlot());
		stmt->setUInt8(3, this->GetJewelBingoMatching(i)->GetValue());
		trans->Append(stmt);
	}
}

void Player::SaveDBDate(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_DATE_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for ( int32 i = 0; i < PLAYER_DATE_MAX; ++i )
	{
		if ( !this->GetDate(i) )
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_DATE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt32(i);
		stmt->setInt64(this->GetDate(i));
		trans->Append(stmt);
	}
}

void Player::SaveDBMajesticTree(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it)
	{
		MajesticSkill* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(0);
		stmt->setUInt8(pSkill->GetSection());
		stmt->setUInt16(pSkill->GetID());
		stmt->setUInt8(pSkill->GetLevel());

		trans->Append(stmt);
	}

	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_stat_list.begin(); it != this->majestic_skill_tree_stat_list.end(); ++it)
	{
		MajesticSkill* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(1);
		stmt->setUInt8(pSkill->GetSection());
		stmt->setUInt16(pSkill->GetID());
		stmt->setUInt8(pSkill->GetLevel());

		trans->Append(stmt);
	}
}

void Player::SaveDBMajesticStats(SQLTransaction& trans)
{
	PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(i);
		stmt->setUInt16(this->GetMajesticStat(i));

		trans->Append(stmt);
	}
}

void Player::SaveDBNumericBaseball(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetNumericBaseballState());
	stmt->setInt32(2, this->GetNumericBaseballScore());

	for (int32 i = 0; i < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++i)
	{
		stmt->setUInt8(3 + i, this->GetNumericBaseballNumber(i));
	}

	std::ostringstream numbers;
	for (int32 i = 0; i < (NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW); ++i)
	{
		numbers << (uint32)this->GetNumericBaseballUsedNumber(i) << " ";
	}

	stmt->setString(6, numbers.str());
	
	std::ostringstream strikes;
	for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
	{
		strikes << (uint32)this->GetNumericBaseballStrike(i) << " ";
	}

	stmt->setString(7, strikes.str());

	std::ostringstream balls;
	for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
	{
		balls << (uint32)this->GetNumericBaseballBall(i) << " ";
	}

	stmt->setString(8, balls.str());

	trans->Append(stmt);
}

void Player::SaveDBQuestMU(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_QUEST_MU_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_QUEST_MU_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt16(1, pData->GetID());
		stmt->setUInt8(2, pData->GetState());
		stmt->setUInt16(3, pData->GetObjective());
		stmt->setInt64(4, pData->GetDate());
				
		trans->Append(stmt);
	}
}

void Player::SaveDBMonsterSoul(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_SOUL_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for (MonsterSoulDataMap::const_iterator itr = this->m_MonsterSoulDataMap.begin(); itr != this->m_MonsterSoulDataMap.end(); ++itr)
	{
		MonsterSoulData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		for (MonsterSoulAmountDataMap::const_iterator it = pData->m_AmountMap.begin(); it != pData->m_AmountMap.end(); ++it)
		{
			stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_SOUL_INSERT);
			stmt->setUInt32(0, this->GetGUID());
			stmt->setUInt32(1, pData->GetType());
			stmt->setUInt32(2, it->first);
			stmt->setInt32(3, it->second);

			trans->Append(stmt);
		}
	}
}