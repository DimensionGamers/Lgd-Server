/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GameServer.cpp"
*
*/

bool IsServerIN(std::string const& data)
{
	if ( data.empty() )
	{
		return false;
	}

	int32 server = 0;
	std::istringstream stream(data.c_str());

    while (stream)
    {
        stream >> server;

		if ( server == sGameServer->GetServerCode() )
		{
			return true;
		}
    }

	return false;
}

GameServer::GameServer()
{
	this->SetShutdown(false);
	this->SetPKTimeAllowed(true);
	this->SetConnectionDisabled(false);
	this->SetAccountsDisconnectTick(GetTickCount());
	this->SetPostManageMACDay(0);
	this->SetGlobalMute(false);
	this->SetActiveStatus(false);
	this->SetInactiveTick(MyGetTickCount());
	this->SetInactiveNotificationTick(MyGetTickCount());
	this->SetFlag(0);
	this->SetServerMode(SERVER_TYPE_PVP);
}

GameServer::~GameServer()
{
	LIST_CLEAR(FilterList::iterator, this->filter_list);
	LIST_CLEAR(NoticeList::iterator, this->notice_list);
	LIST_CLEAR(GoblinPointDataList::iterator, this->goblin_point_list);
	LIST_CLEAR(NonPKTimeList::iterator, this->non_pk_time_list);
	LIST_CLEAR(NotificationList::iterator, this->notification_list);
	MAP_CLEAR(OffsetDataMap::iterator, this->offset_data_map);
	MAP_CLEAR(OffsetFPSMap::iterator, this->offset_fps_map);

	MAP_CLEAR(OfflineAttackWorldMap::iterator, this->offline_attack_world_map);
	LIST_CLEAR(OfflineAttackZoneList::iterator, this->m_OfflineAttackZoneList);

	this->post_manage_mac_map.clear();
	this->mac_mute_list.clear();
	this->mac_restriction_map.clear();
}

void GameServer::LoadCommonSettings()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Common Settings...");
	uint32 oldMSTime = getMSTime();

	sMain->Load();
	
	this->SetMaxConnectionsPerIP(sMain->GetInt32("Network.MaxConnectionPerIP", 0));

	std::string temp_client_version = sMain->GetString("GameServer.Version", "");
	char temp_client_version_buff[8];
	memset(temp_client_version_buff, 0, 8);
	memcpy(temp_client_version_buff, temp_client_version.c_str(), temp_client_version.size() > 8 ? 8 : temp_client_version.size());

	Tokens tokens(temp_client_version_buff, '.');

	this->SetGameServerVersion(0, tokens[0][0]);
	this->SetGameServerVersion(1, tokens[1][0]);
	this->SetGameServerVersion(2, tokens[1][1]);
	this->SetGameServerVersion(3, tokens[2][0]);
	this->SetGameServerVersion(4, tokens[2][1]);


	/*char * cvstr = strtok(temp_client_version_buff, ".");

	this->SetGameServerVersion(0, cvstr[0]);
	cvstr = strtok(nullptr, ".");
	this->SetGameServerVersion(1, cvstr[0]);
	this->SetGameServerVersion(2, cvstr[1]);
	cvstr = strtok(nullptr, ".");
	this->SetGameServerVersion(3, cvstr[0]);
	this->SetGameServerVersion(4, cvstr[1]);*/

	this->ResetGameServerSerial();
	this->SetGameServerSerial(sMain->GetString("GameServer.Serial", "").c_str());

	this->SetPacketRecvLog(sMain->GetBool("GameServer.LogRecvPacket", false));
	this->SetPacketSendLog(sMain->GetBool("GameServer.LogSendPacket", false));
	this->SetWelcomeMessage(sMain->GetString("GameServer.WelcomeMessage", ""));
	this->SetTest(sMain->GetBool("GameServer.TestEnabled", false));
	this->SetTestKickTime(sMain->GetInt32("GameServer.TestKickTime", 0) * MINUTE * IN_MILLISECONDS);
	this->SetTestKey(sMain->GetUInt32("GameServer.TestKey", 0));
	this->SetDebug(sMain->GetBool("GameServer.Debug", false));

	if (!this->IsTestEnabled())
	{
		this->SetTest(false);
		this->SetTestKey(0);
	}
	
	this->SetSecurityCode(sMain->GetBool("GameServer.SecurityCode", false));
	this->SetWrongSerialCount(sMain->GetInt32("GameServer.WrongSerialCount", 0));
	this->SetWrongSerialTime(sMain->GetInt32("GameServer.WrongSerialTime", 0) * IN_MILLISECONDS);
	this->SetWrongDecryptCount(sMain->GetInt32("GameServer.WrongDecryptCount", 0));
	this->SetWrongDecryptTime(sMain->GetInt32("GameServer.WrongDecryptTime", 0) * IN_MILLISECONDS);

	this->SetItemDropLogDB(sMain->GetBool("GameServer.ItemDropLogDB", false));
	this->SetItemLootLogDB(sMain->GetBool("GameServer.ItemLootLogDB", false));
	this->SetItemBuyLogDB(sMain->GetBool("GameServer.ItemBuyLogDB", false));
	this->SetItemSellLogDB(sMain->GetBool("GameServer.ItemSellLogDB", false));
	this->SetLogDBTrade(sMain->GetBool("GameServer.LogDB.Trade", false));
	this->SetLogDBPersonalStore(sMain->GetBool("GameServer.LogDB.PersonalStore", false));
		
	this->SetAccountDisconnectTime(sMain->GetInt32("Account.DisconnectTime", 0) * IN_MILLISECONDS);
	this->SetConnectCheckTimeDelay(sMain->GetInt32("Account.ConnectCheckTime", 0) * IN_MILLISECONDS);
	this->SetConnectCheckResetOnAction(sMain->GetBool("Account.ConnectCheckResetOnAction", false));
	this->SetServerMoveKickTime(sMain->GetInt32("Account.ServerMoveKickTime", 0) * IN_MILLISECONDS);
	this->SetMaxLoggingAttempts(sMain->GetInt32("Account.MaxLoginAttempts", 0));
	this->SetMagicGladiatorCreate(sMain->GetInt32("Account.MagicGladiatorCreate", 0));
	this->SetDarkLordCreate(sMain->GetInt32("Account.DarkLordCreate", 0));
	this->SetRageFighterCreate(sMain->GetInt32("Account.RageFighterCreate", 0));
	this->SetGrowLancerCreate(sMain->GetInt32("Account.GrowLancerCreate", 0));
	this->SetWarehouseLock(sMain->GetBool("Account.Warehouse.Lock", false));
	this->SetWarehouseMaxMoney(sMain->GetInt32("Account.Warehouse.MaxMoney", 0));
	this->SetWarehouseSaveMoney(sMain->GetBool("Account.Warehouse.SaveMoney", false));
	this->SetWarehouseGetMoney(sMain->GetBool("Account.Warehouse.GetMoney", false));

	this->SetAutoSaveTime(sMain->GetInt32("Character.AutosaveTime", 0) * IN_MILLISECONDS);
	this->SetAutoSaveOfflineTime(sMain->GetTime("Character.AutoSaveOfflineTime", 0) * IN_MILLISECONDS);

	this->SetNormalExperience(sMain->GetInt32("GameServer.NormalExperienceRate", 0));
	this->SetMasterExperience(sMain->GetInt32("GameServer.MasterExperienceRate", 0));
	this->SetMajesticExperience(sMain->GetInt32("GameServer.MajesticExperienceRate", 0));

	this->SetNormalZen(sMain->GetInt32("GameServer.NormalZenRate", 0));
	this->SetMasterZen(sMain->GetInt32("GameServer.MasterZenRate", 0));
	this->SetMajesticZen(sMain->GetInt32("GameServer.MajesticZenRate", 0));

	this->SetElfSoldierMaxLevel(sMain->GetInt32("GameServer.ElfSoldierMaxLevel", 0));
	this->SetElfSoldierUpdate(sMain->GetBool("GameServer.ElfSoldierUpdate", false));
	this->SetElfSoldierRemoveOnDie(sMain->GetBool("GameServer.ElfSoldierRemoveOnDie", false));

	this->SetViewportFrustrum(sMain->GetBool("GameServer.ViewportFrustrum",false));
	this->SetNormalMonsterMovement(sMain->GetBool("GameServer.NormalMonsterMovement",false));

	this->SetMultiAttackPenetration(sMain->GetBool("Character.MultiAttackPenetration", false));
	this->SetMultiAttackFireScream(sMain->GetBool("Character.MultiAttackFireScream", false));

	this->SetMaxCharacterZen(sMain->GetInt32("Character.MaxZen", 0));
	this->SetMaxStat(STRENGTH, sMain->GetInt32("Character.MaxStrength", 0));
	this->SetMaxStat(AGILITY, sMain->GetInt32("Character.MaxAgility", 0));
	this->SetMaxStat(VITALITY, sMain->GetInt32("Character.MaxVitality", 0));
	this->SetMaxStat(ENERGY, sMain->GetInt32("Character.MaxEnergy", 0));
	this->SetMaxStat(LEADERSHIP, sMain->GetInt32("Character.MaxLeadership", 0));

	/*for ( uint8 i = 0; i < MAX_STAT_TYPE; ++i )
	{
		if ( this->GetMaxStat(i) > 0xFFFF )
		{
			this->SetMaxStat(i, 0xFFFF);
		}
	}*/

	this->SetMaxLevel(LEVEL_DATA_NORMAL, sMain->GetInt32("Character.MaxLevelNormal", 0));
	this->SetMaxLevel(LEVEL_DATA_MASTER, sMain->GetInt32("Character.MaxLevelMaster", 0));
	this->SetMaxLevel(LEVEL_DATA_MAJESTIC, sMain->GetInt32("Character.MaxLevelMajestic", 0));
	this->SetMinMonsterLevelMaster(sMain->GetInt32("Character.MinMonsterLevelMaster", 0));
	this->SetMinMonsterLevelMajestic(sMain->GetInt32("Character.MinMonsterLevelMajestic", 0));
		
	this->SetAutoRecuperationEnabled(POWER_LIFE, sMain->GetBool("Character.HPAutorecuperationEnabled", false));
	this->SetAutoRecuperationEnabled(POWER_MANA, sMain->GetBool("Character.MPAutorecuperationEnabled", false));
	this->SetAutoRecuperationEnabled(POWER_SHIELD, sMain->GetBool("Character.SDAutorecuperationEnabled", false));
	this->SetAutoRecuperationEnabled(POWER_STAMINA, sMain->GetBool("Character.AGAutorecuperationEnabled", false));

	this->SetAutoRecuperationMaxLevel(POWER_LIFE, sMain->GetInt32("Character.HPAutorecuperationMaxLevel", 0));
	this->SetAutoRecuperationMaxLevel(POWER_MANA, sMain->GetInt32("Character.MPAutorecuperationMaxLevel", 0));
	this->SetAutoRecuperationMaxLevel(POWER_SHIELD, sMain->GetInt32("Character.SDAutorecuperationMaxLevel", 0));
	this->SetAutoRecuperationMaxLevel(POWER_STAMINA, sMain->GetInt32("Character.AGAutorecuperationMaxLevel", 0));

	this->SetAutoRecuperationCount(POWER_LIFE, sMain->GetInt32("Character.HPAutorecuperationCount", 0));
	this->SetAutoRecuperationCount(POWER_MANA, sMain->GetInt32("Character.MPAutorecuperationCount", 0));
	this->SetAutoRecuperationCount(POWER_SHIELD, sMain->GetInt32("Character.SDAutorecuperationCount", 0));
	this->SetAutoRecuperationCount(POWER_STAMINA, sMain->GetInt32("Character.AGAutorecuperationCount", 0));

	this->SetAutoRecuperationCountRest(POWER_LIFE, sMain->GetInt32("Character.HPAutorecuperationCountRest", 0));
	this->SetAutoRecuperationCountRest(POWER_MANA, sMain->GetInt32("Character.MPAutorecuperationCountRest", 0));
	this->SetAutoRecuperationCountRest(POWER_SHIELD, sMain->GetInt32("Character.SDAutorecuperationCountRest", 0));
	this->SetAutoRecuperationCountRest(POWER_STAMINA, sMain->GetInt32("Character.AGAutorecuperationCountRest", 0));

	this->SetSDAttackSuccessRate(sMain->GetInt32("Character.SD.AttackSuccessRate", 0));
	this->shield_combo_miss_init.set(sMain->GetInt32("Character.SD.ComboMissInit", 0));
	this->shield_damage_to_sd_rate.set(sMain->GetInt32("Character.SD.DamageToShield", 0));

	this->SetPlayerPVPMinLevel(sMain->GetInt32("Character.PVPMinLevel", 0));
	this->SetPlayerSelfDefenseEnabled(sMain->GetBool("Character.SelfDefenseEnabled", false));
	this->SetPlayerSelfDefenseTime(sMain->GetInt32("Character.SelfDefenseTime", 0) * IN_MILLISECONDS);
	this->SetPlayerMurderCount(sMain->GetInt32("Character.MurderCount", 0));
	this->SetPlayerMurderTime(sMain->GetInt32("Character.MurderTime", 0));
	this->SetPlayerMurderLimitCount(sMain->GetInt32("Character.MurderLimitCount", 0));
	this->SetPlayerHeroCount(sMain->GetInt32("Character.HeroCount", 0));
	this->SetPlayerHeroTime(sMain->GetInt32("Character.HeroTime", 0));
	this->SetPlayerPKItemDrop(sMain->GetBool("Character.PKItemDrop", false));
	this->SetPlayerPKItemDropMinLevel(sMain->GetInt32("Character.PKItemDropMinLevel", 0));
	this->SetPlayerPKItemDropExcellent(sMain->GetBool("Character.PKItemDropExcellent", false));
	this->SetPlayerPKItemDropAncient(sMain->GetBool("Character.PKItemDropAncient", false));
	this->SetPlayerPKItemDropHarmony(sMain->GetBool("Character.PKItemDropHarmony", false));
	this->SetPlayerPKItemDrop380(sMain->GetBool("Character.PKItemDrop380", false));
	this->SetPlayerPKItemDropItemMaxLevel(sMain->GetInt32("Character.PKItemDropItemMaxLevel", 0));
	this->SetPlayerPKWarpCostIncrease(sMain->GetInt32("Character.PKWarpCostIncrease", 0));

	this->SetPersonalStoreEnabled(sMain->GetBool("PersonalStore.Enabled", false));
	this->SetPersonalStoreOff(sMain->GetBool("PersonalStore.Off", false));
	this->SetPersonalStoreMinLevel(sMain->GetInt32("PersonalStore.MinLevel", 0));
	this->SetPersonalStoreDisconnect(sMain->GetBool("PersonalStore.Disconnect", false));
	this->SetPersonalStoreOffDuration(sMain->GetInt32("PersonalStore.OffDuration", 0) * HOUR * IN_MILLISECONDS);
	this->SetPersonalStoreOffCount(sMain->GetInt32("PersonalStore.OffCount", 0));
	this->SetPersonalStoreOffTotalCount(sMain->GetInt32("PersonalStore.OffTotalCount", 0));
		
	this->SetCharacterRepairButtonEnabled(sMain->GetBool("Character.RepairButton.Enabled", false));
	this->SetCharacterRepairButtonMinLevel(sMain->GetInt32("Character.RepairButton.MinLevel", 0));

	this->SetHarmonyEnabled(sMain->GetBool("Harmony.Enabled", false));
	this->harmony_refine_rate.set(sMain->GetInt32("Harmony.RefineRate", 0));
	this->harmony_strengthen_rate.set(sMain->GetInt32("Harmony.StrengthenRate", 0));
	this->harmony_smelt_normal_rate.set(sMain->GetInt32("Harmony.SmeltNormalRate", 0));
	this->harmony_smelt_exe_rate.set(sMain->GetInt32("Harmony.SmeltExeRate", 0));
	this->harmony_strengthen_update_normal_rate.set(sMain->GetInt32("Harmony.StrengthenUpdateNormalRate", 0));
	this->harmony_strengthen_update_exe_rate.set(sMain->GetInt32("Harmony.StrengthenUpdateExeRate", 0));
	this->SetHarmonyOnAncient(sMain->GetBool("Harmony.OnAncient", false));

	this->option_380_enabled.set(sMain->GetInt32("Option380.Enabled", 0));
	this->option_380_price.set(sMain->GetInt32("Option380.Price", 0));
	this->option_380_rate_grade_1.set(sMain->GetInt32("Option380.RateGrade1", 0));
	this->option_380_rate_grade_2.set(sMain->GetInt32("Option380.RateGrade2", 0));
	this->option_380_rate_grade_3.set(sMain->GetInt32("Option380.RateGrade3", 0));

	this->SetPartyEnabled(sMain->GetBool("Party.Enabled", false));
	this->party_max_wait_interval.set(sMain->GetInt32("Party.MaxWaitInterval",0) * IN_MILLISECONDS);
	this->party_max_level_dif.set(sMain->GetInt32("Party.MaxLevelDif", 0));
	this->SetPartyMemberOfflineInterval(sMain->GetInt32("Party.MemberOfflineInterval", 1) * IN_MILLISECONDS);
	this->SetPartyLeaderChange(sMain->GetBool("Party.LeaderChange", false));
	this->party_allow_dif_gens.set(sMain->GetInt32("Party.AllowDifGens", 0));
	this->party_in_battlezone.set(sMain->GetInt32("Party.InBattlezone", 0));
	this->party_allow_pk.set(sMain->GetInt32("Party.AllowPK", 0));

	this->SetPartyExperience(0, 100);
	this->SetPartyExperience(1, sMain->GetInt32("Party.Experience2", 0));
	this->SetPartyExperience(2, sMain->GetInt32("Party.Experience3", 0));
	this->SetPartyExperience(3, sMain->GetInt32("Party.Experience4", 0));
	this->SetPartyExperience(4, sMain->GetInt32("Party.Experience5", 0));
	this->SetPartyBonusExperience(0, 100);
	this->SetPartyBonusExperience(1, sMain->GetInt32("Party.BonusExperience2", 0));
	this->SetPartyBonusExperience(2, sMain->GetInt32("Party.BonusExperience3", 0));
	this->SetPartyBonusExperience(3, sMain->GetInt32("Party.BonusExperience4", 0));
	this->SetPartyBonusExperience(4, sMain->GetInt32("Party.BonusExperience5", 0));

	this->SetPartyMatchingEnabled(sMain->GetBool("Party.MatchingEnabled", false));
	this->SetPartyMatchingLeaderChange(sMain->GetBool("Party.MatchingLeaderChange", false));
	this->SetPartyMoveEnabled(sMain->GetBool("Party.MoveEnabled", false));
	this->SetPartyMoveCost(sMain->GetInt32("Party.MoveCost", 0));

	
	this->item_duration_time.set(sMain->GetInt32("Common.ItemDurationTime",0) * IN_MILLISECONDS);
	this->looting_time.set(sMain->GetInt32("Common.LootingTime",0) * IN_MILLISECONDS);
	this->item_drop_rate_normal.set(sMain->GetInt32("Common.ItemDrop.RateNormal", 0));
	this->item_drop_rate_exe.set(sMain->GetInt32("Common.ItemDrop.RateExe", 0));
	this->item_drop_skill_rate_normal.set(sMain->GetInt32("Common.ItemDrop.SkillRateNormal", 0));
	this->item_drop_luck_rate_normal.set(sMain->GetInt32("Common.ItemDrop.LuckRateNormal", 0));
	this->item_drop_luck_rate_exe.set(sMain->GetInt32("Common.ItemDrop.LuckRateExe", 0));

	this->SetItemDropSocketRate(0, sMain->GetInt32("Common.ItemDrop.Socket1RateNormal", 0));
	this->SetItemDropSocketRate(1, sMain->GetInt32("Common.ItemDrop.Socket2RateNormal", 0));
	this->SetItemDropSocketRate(2, sMain->GetInt32("Common.ItemDrop.Socket3RateNormal", 0));
	this->SetItemDropSocketRate(3, sMain->GetInt32("Common.ItemDrop.Socket4RateNormal", 0));
	this->SetItemDropSocketRate(4, sMain->GetInt32("Common.ItemDrop.Socket5RateNormal", 0));

	this->SetItemDropSocketRateExe(0, sMain->GetInt32("Common.ItemDrop.Socket1RateExe", 0));
	this->SetItemDropSocketRateExe(1, sMain->GetInt32("Common.ItemDrop.Socket2RateExe", 0));
	this->SetItemDropSocketRateExe(2, sMain->GetInt32("Common.ItemDrop.Socket3RateExe", 0));
	this->SetItemDropSocketRateExe(3, sMain->GetInt32("Common.ItemDrop.Socket4RateExe", 0));
	this->SetItemDropSocketRateExe(4, sMain->GetInt32("Common.ItemDrop.Socket5RateExe", 0));

	this->SetItemDropExeOptionMin(sMain->GetInt32("Common.ItemDrop.ExeMinOption", 1));
	this->SetItemDropExeOptionMax(sMain->GetInt32("Common.ItemDrop.ExeMaxOption", 2));

	this->SetFenrirRepairRate(sMain->GetInt32("Common.FenrirRepairRate", 0));

	this->SetJewelOfSoulEnabled(sMain->GetBool("Common.JewelOfSoul.Enabled", false));
	this->SetJewelOfSoulRate(sMain->GetInt32("Common.JewelOfSoul.Rate", 0));
	this->SetJewelOfSoulRateLuck(sMain->GetInt32("Common.JewelOfSoul.RateLuck", 0));

	this->SetJewelOfLifeEnabled(sMain->GetBool("Common.JewelOfLife.Enabled", false));
	this->jewel_of_life_max_rate.set(sMain->GetInt32("Common.JewelOfLife.MaxRate", 0));
	this->jewel_of_life_rate_normal.set(sMain->GetInt32("Common.JewelOfLife.RateNormal", 0));
	this->jewel_of_life_rate_socket.set(sMain->GetInt32("Common.JewelOfLife.RateSocket", 0));
	this->jewel_of_life_rate_exe.set(sMain->GetInt32("Common.JewelOfLife.RateExe", 0));
	this->jewel_of_life_rate_ancient.set(sMain->GetInt32("Common.JewelOfLife.RateAncient", 0));
	this->SetJewelOfLifeMax(sMain->GetInt32("Common.JewelOfLife.Max", 0));
	this->SetJewelOfLifeReset(sMain->GetBool("Common.JewelOfLife.Reset", false));
	this->SetJewelOfLifeResetWarning(sMain->GetString("Common.JewelOfLife.ResetWarning", ""));
	this->SetJewelOfLifeResetRate(sMain->GetInt32("Common.JewelOfLife.ResetRate", 0));

	this->SetSantaVillageEnabled(sMain->GetBool("SantaVillage.Enabled", false));
	this->SetSantaVillageSantaClausEnabled(sMain->GetBool("SantaVillage.SantaClaus.Enabled", false));
	this->SetSantaVillageSantaClausGiftTime(sMain->GetInt32("SantaVillage.SantaClaus.GiftTime", 0));
	this->SetSantaVillageSeason(sMain->GetInt32("SantaVillage.Season", 0));
	this->SetSantaVillageSantaClausItemBag(sMain->GetString("SantaVillage.SantaClaus.ItemBag", ""));
	this->SetSantaVillageCheckSeason(sMain->GetBool("SantaVillage.CheckSeason", false));

	this->SetPersonalMerchantShop(sMain->GetString("Common.PersonalMerchantShop", ""));

	this->trade_enabled.set(sMain->GetInt32("Trade.Enabled", 0));
	this->trade_max_wait_interval.set(sMain->GetInt32("Trade.MaxWaitInterval", 0) * IN_MILLISECONDS);
	this->SetTradeMaxZen(sMain->GetInt32("Trade.MaxZen", 0));

	this->guild_enabled.set(sMain->GetInt32("Guild.Enabled", 0));
	this->guild_create_min_level.set(sMain->GetInt32("Guild.CreateMinLevel", 0));
	this->guild_create_gens.set(sMain->GetInt32("Guild.CreateGens", 0));
	this->guild_join_gens.set(sMain->GetInt32("Guild.JoinGens", 0));
	this->guild_max_wait_interval.set(sMain->GetInt32("Guild.MaxWaitInterval", 0) * IN_MILLISECONDS);
	this->guild_delete.set(sMain->GetInt32("Guild.Delete", 0));
	this->guild_alliance_min_members.set(sMain->GetInt32("Guild.Alliance.MinMembers", 0));
	this->guild_alliance_gens.set(sMain->GetInt32("Guild.Alliance.Gens", 0));
	this->guild_alliance_max.set(sMain->GetInt32("Guild.Alliance.Max", 0));
	this->guild_hostil_event_kill.set(sMain->GetInt32("Guild.Hostil.EventKill", 0));
	this->SetGuildAllianceCastleSiegeCheck(sMain->GetBool("Guild.Alliance.CastleSiegeCheck", false));

	this->SetGuildWarEnabled(sMain->GetBool("Guild.War.Enabled", false));
	this->SetGuildWarMaxWaitInterval(sMain->GetInt32("Guild.War.MaxWaitInterval", 0) * IN_MILLISECONDS);
	this->SetGuildWarMax(sMain->GetInt32("Guild.War.Max", 0));
	this->SetGuildWarIntervalTime(sMain->GetInt32("Guild.War.IntervalTime", 0) * IN_MILLISECONDS);
	this->SetGuildWarMaxPoints(sMain->GetInt32("Guild.War.MaxPoints", 0));
	this->SetGuildWarKillPoints(sMain->GetInt32("Guild.War.KillPoints", 0));
	this->SetGuildWarKillAssistantPoints(sMain->GetInt32("Guild.War.KillAssistantPoints", 0));
	this->SetGuildWarKillBattlePoints(sMain->GetInt32("Guild.War.KillBattlePoints", 0));
	this->SetGuildWarKillMasterPoints(sMain->GetInt32("Guild.War.KillMasterPoints", 0));

	this->SetBattleSoccerEnabled(sMain->GetBool("Guild.BattleSoccer.Enabled", false));
	this->SetBattleSoccerDuration(sMain->GetInt32("Guild.BattleSoccer.Duration", 0) * MINUTE * IN_MILLISECONDS);
	this->SetBattleSoccerGoalPoints(sMain->GetInt32("Guild.BattleSoccer.GoalPoints", 0));
	this->SetBattleSoccerMaxPoints(sMain->GetInt32("Guild.BattleSoccer.MaxScore", 0));
	this->SetBattleSoccerKillPoints(sMain->GetInt32("Guild.BattleSoccer.KillPoints", 0));
	this->SetBattleSoccerKillAssistantPoints(sMain->GetInt32("Guild.BattleSoccer.KillAssistantPoints", 0));
	this->SetBattleSoccerKillBattlePoints(sMain->GetInt32("Guild.BattleSoccer.KillBattlePoints", 0));
	this->SetBattleSoccerKillMasterPoints(sMain->GetInt32("Guild.BattleSoccer.KillMasterPoints", 0));

	this->SetDuelEnabled(sMain->GetBool("Duel.Enabled", false));
	this->dueling_min_level.set(sMain->GetInt32("Duel.MinLevel", 0));
	this->SetDuelRequiredZen(sMain->GetInt32("Duel.RequiredZen", 0));
	this->dueling_max_wait_interval.set(sMain->GetInt32("Duel.MaxWaitInterval", 0) * IN_MILLISECONDS);
	this->SetDuelDuration(sMain->GetInt32("Duel.Duration", 0) * MINUTE * IN_MILLISECONDS);
	this->SetDuelRoomStandByTime(sMain->GetInt32("Duel.StandByTime", 0) * IN_MILLISECONDS);
	this->SetDuelingDamage(sMain->GetInt32("Duel.Damage", 0));
	this->SetDuelRoundStandby(sMain->GetInt32("Duel.RoundStandby", 0));
	this->SetDuelClassic(sMain->GetBool("Duel.Classic", false));
	this->SetDuelBuffDisabled(sMain->GetBool("Duel.BuffDisabled", false));
	this->SetDuelPentagramDisabled(sMain->GetBool("Duel.PentagramDisabled", false));
	this->SetDuelMuunDisabled(sMain->GetBool("Duel.MuunDisabled", false));
	this->SetDuelTestLevel(sMain->GetInt32("Duel.TestLevel", 0));

	this->SetFriendEnabled(sMain->GetBool("Friend.Enabled", false));
	this->SetFriendAddMinLevel(sMain->GetInt32("Friend.AddMinLevel", 0));
	this->SetFriendMailEnabled(sMain->GetBool("Friend.MailEnabled", false));
	this->SetFriendMailCost(sMain->GetInt32("Friend.MailCost", 0));
	this->SetFriendMailUpdateInterval(sMain->GetInt32("Friend.MailUpdateInterval", 0) * IN_MILLISECONDS);

	this->SetGenJoinMinLevel(sMain->GetInt32("Gen.JoinMinLevel", 0));
	this->SetGenChangeTime(sMain->GetInt32("Gen.ChangeFamilyTime", 0));
	this->SetGenPenalization(sMain->GetBool("Gen.Penalization", false));
	this->SetGenPenalizationCount(sMain->GetInt32("Gen.PenalizationCount", 0));
	this->SetGenPenalizationTime(sMain->GetInt32("Gen.PenalizationTime",0) * IN_MILLISECONDS);

	this->kundun_refill_time.set(sMain->GetInt32("Kundun.RefillTime", 0) * IN_MILLISECONDS);
	this->kundun_refill_hp.set(sMain->GetInt32("Kundun.RefillHp", 0));
	this->kundun_refill_hp_sec.set(sMain->GetInt32("Kundun.RefillHpSec", 0));

	this->SetSkillFireScreamExplosionRate(sMain->GetInt32("Skill.FireScreamExplosionRate", 0));

	this->SetHelperPotionEnabled(sMain->GetBool("Helper.PotionUseEnabled", false));
	this->SetHelperRepairEnabled(sMain->GetBool("Helper.RepairEnabled", false));
	this->SetHelperLootEnabled(sMain->GetBool("Helper.LootEnabled", false));
	this->SetHelperAutoLootEnabled(sMain->GetBool("Helper.AutoLootEnabled", false));
	
	this->SetBloodCastleEnabled(sMain->GetBool("BloodCastle.Enabled", false));
	this->SetBloodCastleDeliverWeaponTime(sMain->GetInt32("BloodCastle.DeliverWeaponTime", 0) * IN_MILLISECONDS);
	this->SetBloodCastleSkillRushCheck(sMain->GetBool("BloodCastle.SkillRushCheck", false));
	this->SetBloodCastleRewardScore(sMain->GetInt32("BloodCastle.RewardScore", 0));
	this->SetChaosCastleEnabled(sMain->GetBool("ChaosCastle.Enabled", false));
	this->SetChaosCastleSurvivalOfTheFittestEnabled(sMain->GetBool("ChaosCastle.SurvivalOfTheFittestEnabled", false));
	this->SetChaosCastleSurvivalMaxPerPC(sMain->GetInt32("ChaosCastle.SurvivalMaxPerPC", 0));
	this->SetDevilSquareEnabled(sMain->GetBool("DevilSquare.Enabled", false));

	this->SetDungeonRaceEnabled(IsServerIN(sMain->GetString("DungeonRace.Enabled", "999")));
	this->SetDungeonRaceReward(sMain->GetString("DungeonRace.Reward", ""));

	this->SetLosttowerRaceEnabled(IsServerIN(sMain->GetString("LosttowerRace.Enabled", "999")));
	this->SetLosttowerRaceReward(sMain->GetString("LosttowerRace.Reward", ""));

	this->SetRaklionEnabled(sMain->GetBool("Raklion.Enabled", false));
	this->SetRaklionSuccessTimeMin(sMain->GetInt32("Raklion.SuccessTimeMin", 0) * MINUTE * IN_MILLISECONDS);
	this->SetRaklionSuccessTimeMax(sMain->GetInt32("Raklion.SuccessTimeMax", 0) * MINUTE * IN_MILLISECONDS);
	this->SetRaklionFailTimeMin(sMain->GetInt32("Raklion.FailTimeMin", 0) * MINUTE * IN_MILLISECONDS);
	this->SetRaklionFailTimeMax(sMain->GetInt32("Raklion.FailTimeMax", 0) * MINUTE * IN_MILLISECONDS);
	
	this->SetKanturuEnabled(sMain->GetBool("Kanturu.Enabled", false));
	this->kanturu_max_players.set(sMain->GetInt32("Kanturu.MaxPlayers", 0));
	this->kanturu_ice_storm_rate.set(sMain->GetInt32("Kanturu.Maya.IceStormRate", 0));
	this->kanturu_skill_time.set(sMain->GetInt32("Kanturu.Maya.SkillTime", 0));

	this->SetCrywolfEnabled(sMain->GetBool("Crywolf.Enabled", false));

	this->SetCrywolfBenefitApply(sMain->GetBool("Crywolf.Benefit.Apply", false));
	this->SetCrywolfBenefitPlusRate(sMain->GetInt32("Crywolf.Benefit.PlusRate", 0));
	this->SetCrywolfBenefitMonsterHP(sMain->GetInt32("Crywolf.Benefit.MonsterHp", 0));
	this->SetCrywolfBenefitKundunRefill(sMain->GetBool("Crywolf.Benefit.KundunRefill", false));

	this->SetCrywolfPenaltyApply(sMain->GetBool("Crywolf.Penalty.Apply", false));
	this->SetCrywolfPenaltyJewelDrop(sMain->GetInt32("Crywolf.Penalty.DropGem", 0));
	this->SetCrywolfPenaltyExperienceGain(sMain->GetInt32("Crywolf.Penalty.ExpGain", 0));

	this->crywolf_altar_min_level.set(sMain->GetInt32("Crywolf.Altar.MinLevel", 0));
	this->crywolf_altar_max_contract.set(sMain->GetInt32("Crywolf.Altar.MaxContract", 0));
	this->crywolf_altar_contract_valid_time.set(sMain->GetInt32("Crywolf.Altar.ContractValidationTime",0) * IN_MILLISECONDS);
	this->crywolf_altar_contract_last_time.set(sMain->GetInt32("Crywolf.Altar.ContractWaitTime",0) * IN_MILLISECONDS);
	this->SetCrywolfAltarScore(sMain->GetInt32("Crywolf.Altar.Score", 0));

	this->crywolf_aichange_time.set(sMain->GetInt32("Crywolf.AIChange.Time",0) * IN_MILLISECONDS);
	this->crywolf_balgass_start_time.set(sMain->GetInt32("Crywolf.Balgass.StartTime",0) * IN_MILLISECONDS);

	this->SetCastleSiegeEnabled(sMain->GetBool("CastleSiege.Enabled", false));
	this->SetCastleSiegeRegisterEnabled(sMain->GetBool("CastleSiege.RegisterEnabled", false));
	this->SetCastleSiegeRegisterSignEnabled(sMain->GetBool("CastleSiege.RegisterSignEnabled", false));
	this->SetCastleSiegeMachineEnabled(sMain->GetBool("CastleSiege.MachineEnabled", false));
	this->SetCastleSiegeCrownTime(sMain->GetInt32("CastleSiege.CrownTime", 0));
	this->SetCastleSiegeLordMixMax(sMain->GetInt32("CastleSiege.LordMixMax", 0));
	this->SetCastleSiegeRegisterMinLevel(sMain->GetInt32("CastleSiege.RegisterMinLevel", 0));
	this->SetCastleSiegeRegisterMinMembers(sMain->GetInt32("CastleSiege.RegisterMinMembers", 0));
	this->SetCastleSiegeReduceDamageSameSide(sMain->GetInt32("CastleSiege.ReduceDamageSameSide", 0));
	this->SetCastleSiegeReduceDamageDifSide(sMain->GetInt32("CastleSiege.ReduceDamageDifSide", 0));
	this->SetCastleSiegeParticipantRewardMinutes(sMain->GetTime("CastleSiege.ParticipantRewardMinutes", 0));

	this->SetDoppelgangerEnabled(sMain->GetBool("Doppelganger.Enabled", false));
	this->SetDoppelgangerPKCheck(sMain->GetBool("Doppelganger.PKCheck", false));
	this->SetDoppelgangerPKLevelMax(sMain->GetInt32("Doppelganger.PKLevelMax", 0));
	this->SetDoppelgangerMonsterSpeed(sMain->GetInt32("Doppelganger.MonsterSpeed", 0));

	this->SetImperialFortressEnabled(sMain->GetBool("ImperialFortress.Enabled", false));
	this->SetImperialFortressPKCheck(sMain->GetBool("ImperialFortress.PKCheck", false));
	this->SetImperialFortressPKLevelMax(sMain->GetInt32("ImperialFortress.PKLevelMax", 0));
	this->SetImperialFortressInParty(sMain->GetBool("ImperialFortress.InParty", false));
	this->SetImperialFortressStandbyTime(sMain->GetTime("ImperialFortress.StandbyTime", 0) * IN_MILLISECONDS);
	this->SetImperialFortressPlayTime(sMain->GetTime("ImperialFortress.PlayTime", 0) * IN_MILLISECONDS);
	this->SetImperialFortressEndTime(sMain->GetTime("ImperialFortress.EndTime", 0) * IN_MILLISECONDS);
	this->SetImperialFortressDay(sMain->GetInt32("ImperialFortress.Day", -1));
	this->SetImperialFortressTimeZone(0, sMain->GetInt32("ImperialFortress.TimeZone1", 0) * IN_MILLISECONDS);
	this->SetImperialFortressTimeZone(1, sMain->GetInt32("ImperialFortress.TimeZone2", 0) * IN_MILLISECONDS);
	this->SetImperialFortressTimeZone(2, sMain->GetInt32("ImperialFortress.TimeZone3", 0) * IN_MILLISECONDS);
	this->SetImperialFortressTimeZone(3, sMain->GetInt32("ImperialFortress.TimeZone4", 0) * IN_MILLISECONDS);

	this->SetImperialFortressWalkCheck(sMain->GetBool("ImperialFortress.WalkCheck", false));
	this->SetImperialFortressTeleportCheck(sMain->GetBool("ImperialFortress.TeleportCheck", false));
	this->SetImperialFortressMoveToPrevious(sMain->GetBool("ImperialFortress.MoveToPrevious", false));

	this->happy_hour_experience_add.set(sMain->GetInt32("HappyHour.ExperienceAdd", 0));
	this->happy_hour_drop_add.set(sMain->GetInt32("HappyHour.DropAdd", 0));

	this->SetCommandPostEnabled(sMain->GetBool("Command.Post.Enabled", false));
	this->SetCommandPostMinLevel(sMain->GetInt32("Command.Post.MinLevel", 0));
	this->SetCommandPostCost(sMain->GetInt32("Command.Post.Cost", 0));
	this->SetCommandPostDelay(sMain->GetInt32("Command.Post.Delay", 0));
	this->SetCommandPostType(sMain->GetInt32("Command.Post.Type", 0));
	this->SetCommandPostHead(sMain->GetString("Command.Post.Head", "$name: [POST][$server]: $message"));

	this->SetCommandAddStatEnabled(sMain->GetBool("Command.AddStat.Enabled", false));
	this->SetCommandAddStatMinLevel(sMain->GetInt32("Command.AddStat.MinLevel", 0));
	this->SetCommandAddStatCost(sMain->GetInt32("Command.AddStat.Cost", 0));

	this->SetScrambleEnabled(IsServerIN(sMain->GetString("Scramble.Enabled", "999")));
	this->SetScrambleTotalWordCount(sMain->GetInt32("Scramble.TotalWordCount", 0));
	this->SetScrambleRepeatWordCount(sMain->GetInt32("Scramble.RepeatWordCount", 0));
	this->SetScrambleReward(sMain->GetString("Scramble.Reward", ""));

	this->SetGoblinPointEnabled(sMain->GetBool("GoblinPoint.Enabled", false));
	this->SetGoblinPointKillTime(sMain->GetInt32("GoblinPoint.KillTime", 0) * IN_MILLISECONDS);
	this->SetGoblinPointRequiredTime(sMain->GetInt32("GoblinPoint.RequiredTime", 0));
	this->SetGoblinPointRequiredTimeOffline(sMain->GetInt32("GoblinPoint.RequiredTimeOffline", 0));

	this->SetEventInventoryEnabled(sMain->GetBool("EventInventory.Enabled", false));

	this->SetMuRoomyEnabled(sMain->GetBool("MuRoomy.Enabled", false));
	this->SetMuRoomySpecialEnabled(sMain->GetBool("MuRoomy.SpecialEnabled", false));
	this->SetMuRoomyTicketRequired(sMain->GetBool("MuRoomy.TicketRequired", false));
	this->SetMuRoomyRewardMoneyAmmount(sMain->GetInt32("MuRoomy.RewardMoneyAmount", 0));
	
	this->SetJoinItemsWithExpireTime(sMain->GetBool("GameServer.JoinItemsWithExpireTime", false));
	this->SetPetComboExperience(sMain->GetInt32("GameServer.PetComboExperience", 0));

	this->SetGateHandle(sMain->GetBool("GameServer.GateHandle", false));

	this->SetBarracksMoveEnabled(sMain->GetBool("Barracks.MoveEnabled", false));
	this->SetBarracksMoveMinLevel(sMain->GetInt32("Barracks.MoveMinLevel", 0));
	this->SetBarracksMoveCost(sMain->GetInt32("Barracks.MoveCost", 0));
	this->SetBarracksMoveParty(sMain->GetBool("Barracks.MoveParty", false));

	this->SetRefugeMoveEnabled(sMain->GetBool("Refuge.MoveEnabled", false));
	this->SetRefugeMoveMinLevel(sMain->GetInt32("Refuge.MoveMinLevel", 0));
	this->SetRefugeMoveCost(sMain->GetInt32("Refuge.MoveCost", 0));
	this->SetRefugeMoveParty(sMain->GetBool("Refuge.MoveParty", false));

	this->SetTransactionSharedDuplicatedSerialDelete(sMain->GetBool("Transaction.SharedDuplicatedSerialDelete", false));
	this->SetTransactionDuplicatedSerialDelete(sMain->GetBool("Transaction.DuplicatedSerialDelete", false));

	this->SetTransferEnabled(sMain->GetBool("Transfer.Enabled", false));
	this->SetTransferServer(sMain->GetInt32("Transfer.Server", -1));
	this->SetTransferPartyServer(sMain->GetInt32("Transfer.PartyServer", -1));

	if ( this->GetTransferServer() == uint16(-1) )
	{
		this->SetTransferServer(this->GetServerCode());
	}

	if ( this->GetTransferPartyServer() == uint16(-1) )
	{
		this->SetTransferPartyServer(this->GetServerCode());
	}

	if ( !this->IsTransferEnabled() )
	{
		this->SetTransferServer(this->GetServerCode());
		this->SetTransferPartyServer(this->GetServerCode());
	}

	this->SetSpawnMonsterTime(sMain->GetInt32("GameServer.SpawnMonsterTime", 0) * IN_MILLISECONDS);

	this->SetAttackSafeEnabled(sMain->GetBool("GameServer.AttackSafeEnabled", false));
	this->SetAttackSafeCount(sMain->GetInt32("GameServer.AttackSafeCount", 0));
	this->SetAttackSafeTime(sMain->GetInt32("GameServer.AttackSafeTime", 0) * IN_MILLISECONDS);
	this->SetAttackSafeKick(sMain->GetBool("GameServer.AttackSafeKick", false));
	this->SetAttackSpeedSendTime(sMain->GetInt32("GameServer.AttackSpeedSendTime", 0) * IN_MILLISECONDS);

	this->SetSkillEnergyCheck(sMain->GetBool("Skill.EnergyCheck", false));
	this->SetSkillLeadershipCheck(sMain->GetBool("Skill.LeadershipCheck", false));

	this->SetGameCloseTime(sMain->GetInt32("Account.GameCloseTime", 0) * IN_MILLISECONDS);

	this->SetRecvPacketQueueMax(sMain->GetInt32("GameServer.RecvPacketQueueMax", 0));
	this->SetRecvPacketQueueKick(sMain->GetBool("GameServer.RecvPacketQueueKick", false));

	this->SetConnectServerReconnect(sMain->GetBool("GameServer.ConnectServerReconnectEnabled", false));
	this->SetConnectServerReconnecTime(sMain->GetInt32("GameServer.ConnectServerReconnectTime", 0) * IN_MILLISECONDS);
	this->SetAuthServerReconnect(sMain->GetBool("GameServer.AuthServerReconnectEnabled", false));
	this->SetAuthServerReconnecTime(sMain->GetInt32("GameServer.AuthServerReconnectTime", 0) * IN_MILLISECONDS);
	this->SetServerLinkServerReconnect(sMain->GetBool("GameServer.ServerLinkServerReconnectEnabled", false));
	this->SetServerLinkServerReconnecTime(sMain->GetInt32("GameServer.ServerLinkServerReconnectTime", 0) * IN_MILLISECONDS);

	this->SetCharacterDieExperienceDecrease(sMain->GetBool("Character.DieExperienceDecrease", false));
	this->SetCharacterDieZenDecrease(sMain->GetBool("Character.DieZenDecrease", false));

	this->SetSkillManaShieldLimit(sMain->GetInt32("Skill.ManaShieldLimit", 0));
	this->SetSkillManaShieldCastleSiegeLimit(sMain->GetInt32("Skill.ManaShieldCastleSiegeLimit", 0));

	this->SetKickEnabled(sMain->GetBool("Kick.Enabled", false));
	this->SetKickType(sMain->GetInt32("Kick.Type", 0));

	this->SetMonsterStopAction(sMain->GetBool("GameServer.MonsterStopAction", false));
	this->SetMonsterViewportControl(sMain->GetBool("GameServer.MonsterViewportControl", false));

	this->SetMasterCommand(sMain->GetBool("Command.Master", false));

	this->SetFixPowersOnBuff(sMain->GetBool("Skill.FixPowersOnBuff", false));

	this->SetRefreshEnabled(sMain->GetBool("Command.Refresh.Enabled", false));
	this->SetRefreshTime(sMain->GetInt32("Command.Refresh.Time", 0) * IN_MILLISECONDS);

	this->SetHostilCancelCommand(sMain->GetBool("GameServer.HostilCancelCommand", false));

	this->SetPostManageEnabled(sMain->GetBool("GameServer.PostManager.Enabled", false));
	this->SetPostManageCount(sMain->GetInt32("GameServer.PostManager.Count", 0));

	this->SetPostManageMACEnabled(sMain->GetBool("GameServer.PostManagerMAC.Enabled", false));
	this->SetPostManageMACCount(sMain->GetInt32("GameServer.PostManagerMAC.Count", 0));

	this->SetAntiSpamEnabled(sMain->GetBool("GameServer.AntiSpam.Enabled", false));
	this->SetAntiSpamCount(sMain->GetInt32("GameServer.AntiSpam.Count", 0));
	this->SetAntiSpamTime(sMain->GetInt32("GameServer.AntiSpam.Time", 0) * IN_MILLISECONDS);
	this->SetAntiSpamMuteTime(sMain->GetInt32("GameServer.AntiSpam.MuteTime", 0));

	this->SetDobleFrustrum(sMain->GetBool("GameServer.DobleFrustrum", false));

	this->SetSkillEarthPrisonStunRate(sMain->GetInt32("Skill.EarthPrisonStunRate", 0));

	this->SetSkillSleepLimit(sMain->GetInt32("Skill.SleepLimit", 70));
	this->SetSkillSleepLimitTime(sMain->GetInt32("Skill.SleepLimitTime", 70));

	this->SetSkillBlindAdd(sMain->GetInt32("Skill.BlindAdd", 0));
	this->SetSkillBlindDiv(sMain->GetInt32("Skill.BlindDiv", 0));
	this->SetSkillBlindValue(sMain->GetInt32("Skill.BlindValue", 0));
	this->SetSkillBlindTime(sMain->GetInt32("Skill.BlindTime", 0));

	this->SetSkillIronDefenseValue1(sMain->GetInt32("Skill.IronDefenseValue1", 0));
	this->SetSkillIronDefenseValue2(sMain->GetInt32("Skill.IronDefenseValue2", 0));
	this->SetSkillIronDefenseDuration(sMain->GetInt32("Skill.IronDefenseDuration", 0));

	this->SetSkillBloodHowlingRate(sMain->GetInt32("Skill.BloodHowlingRate", 0));
	this->SetSkillBloodHowlingDamage(sMain->GetInt32("Skill.BloodHowlingDamage", 0));
	this->SetSkillBloodHowlingDuration(sMain->GetInt32("Skill.BloodHowlingDuration", 0));

	this->SetSkillCircleShieldDuration(sMain->GetInt32("Skill.CircleShieldDuration", 0));
	this->SetSkillCircleShieldReduceAGRate(sMain->GetInt32("Skill.CircleShieldReduceAGRate", 0));
	this->SetSkillCircleShieldReduceAG(sMain->GetInt32("Skill.CircleShieldReduceAG", 0));
	this->SetSkillMagicPinExplosionRate(sMain->GetInt32("Skill.MagicPinExplosionRate", 0));
	this->SetSkillMagicPinExplosionDamage(sMain->GetInt32("Skill.MagicPinExplosionDamage", 0));

	this->SetRadianceBleedingRate(sMain->GetInt32("Radiance.BleedingRate", 5));
	this->SetRadianceBleedingDuration(sMain->GetInt32("Radiance.BleedingDuration", 10));
	this->SetRadianceParalysisRate(sMain->GetInt32("Radiance.ParalysisRate", 5));
	this->SetRadianceParalysisEffect(sMain->GetInt32("Radiance.ParalysisEffect", 90));
	this->SetRadianceBindingDuration(sMain->GetInt32("Radiance.BindingDuration", 5));
	this->SetRadianceBlindingRate(sMain->GetInt32("Radiance.BlindingRate", 5));
	this->SetRadianceBlindingDuration(sMain->GetInt32("Radiance.BlindingDuration", 10));
	
	this->SetSkillBastionRate(sMain->GetInt32("Skill.BastionRate", 0));
	this->SetSkillBastionSD(sMain->GetInt32("Skill.BastionSD", 0));
	this->SetSkillBastionReduction(sMain->GetInt32("Skill.BastionReduction", 0));
	this->SetSkillPunishRatePVM(sMain->GetInt32("Skill.PunishRatePVM", 0));
	this->SetSkillPunishRatePVP(sMain->GetInt32("Skill.PunishRatePVP", 0));
	this->SetSkillPunishFrequencyPVM(sMain->GetUInt32("Skill.PunishFrequencyPVM", 0));
	this->SetSkillPunishFrequencyPVP(sMain->GetUInt32("Skill.PunishFrequencyPVP", 0));
	this->SetSkillPunishFirstHit(sMain->GetBool("Skill.PunishFirstHit", false));
	this->SetSkillPunishMaxDamage(sMain->GetUInt32("Skill.PunishMaxDamage", 0));

	this->SetSkillAbsorbHPRate(sMain->GetInt32("Skill.AbsorbHPRate", 0));
	this->SetSkillAbsorbSDRate(sMain->GetInt32("Skill.AbsorbSDRate", 0));

	this->SetSkillIllusionDuration(sMain->GetInt32("Skill.IllusionDuration", 0));

	this->SetSkillArchangelWillDamage(sMain->GetInt32("Skill.ArchangelWillDamage", 0));
	this->SetSkillArchangelWillSkillDamage(sMain->GetInt32("Skill.ArchangelWillSkillDamage", 0));
	this->SetSkillArchangelWillDuration(sMain->GetInt32("Skill.ArchangelWillDuration", 0));

	this->SetSkillPoisonArrowDebuffRate(sMain->GetInt32("Skill.PoisonArrowDebuffRate", 0));
	this->SetSkillCureBuffRate(sMain->GetInt32("Skill.CureBuffRate", 0));
	this->SetSkillParalizeDebuffRate(sMain->GetInt32("Skill.ParalizeDebuffRate", 0));
	this->SetSkillSplashDamagePercent(sMain->GetInt32("Skill.SplashDamagePercent", 0));

	this->SetSkillElementalImmuneI(sMain->GetInt32("Skill.ElementalImmuneI", 0));
	this->SetSkillElementalImmuneII(sMain->GetInt32("Skill.ElementalImmuneII", 0));
	this->SetSkillElementalBerserkerI(sMain->GetInt32("Skill.ElementalBerserkerI", 0));
	this->SetSkillElementalBerserkerIDamage(sMain->GetInt32("Skill.ElementalBerserkerIDamage", 0));
	this->SetSkillElementalBerserkerIDefense(sMain->GetInt32("Skill.ElementalBerserkerIDefense", 0));

	this->SetTormentedSquareEnabled(sMain->GetBool("TormentedSquare.Enabled", false));
	this->SetTormentedSquareSurvivalEnabled(sMain->GetBool("TormentedSquareSurvival.Enabled", false));

	this->SetArkaWarEnabled(IsServerIN(sMain->GetString("ArkaWar.Server", "9999")));
	this->SetArkaWarRegisterMinMembers(sMain->GetInt32("ArkaWar.RegisterMinMembers", 0));
	this->SetArkaWarRegisterMaxMembers(sMain->GetInt32("ArkaWar.RegisterMaxMembers", 0));
	this->SetArkaWarMinGuilds(sMain->GetInt32("ArkaWar.MinGuilds", 0));
	this->SetArkaWarDamageReductionAlly(sMain->GetInt32("ArkaWar.DamageReductionAlly", 0));
	this->SetArkaWarDamageReductionEnemy(sMain->GetInt32("ArkaWar.DamageReductionEnemy", 0));

	this->SetProtectorOfAcheronEnabled(sMain->GetBool("ProtectorOfAcheron.Enabled", false));

	this->SetMossMerchantEnabled(sMain->GetBool("MossMerchant.Enabled", false));
	this->MossMerchantItemBag[MOSS_ITEM_TYPE_SWORD] = sMain->GetString("MossMerchant.ItemBag.Sword", "");
	this->MossMerchantItemBag[MOSS_ITEM_TYPE_STAFF] = sMain->GetString("MossMerchant.ItemBag.Staff", "");
	this->MossMerchantItemBag[MOSS_ITEM_TYPE_BOW] = sMain->GetString("MossMerchant.ItemBag.Bow", "");
	this->MossMerchantItemBag[MOSS_ITEM_TYPE_SCEPTER] = sMain->GetString("MossMerchant.ItemBag.Scepter", "");
	this->MossMerchantItemBag[MOSS_ITEM_TYPE_STICK] = sMain->GetString("MossMerchant.ItemBag.Stick", "");

	this->SetMuunDurabilityRate(sMain->GetInt32("Muun.DurabilityRate", 0));
	this->SetMuunAttackEnabled(sMain->GetBool("Muun.Attack.Enabled", false));
	this->SetMuunAttackRange(sMain->GetInt32("Muun.Attack.Range", 0));
	this->SetMuunAttackDelay(sMain->GetInt32("Muun.Attack.Delay", 0));
	this->SetMuunAttackHitDelay(sMain->GetInt32("Muun.Attack.HitDelay", 0));
	this->SetMuunEvolveBonus(sMain->GetBool("Muun.EvolveBonus", false));
	this->SetMuunEvolveBonusTime(sMain->GetInt64("Muun.EvolveBonusTime", 0));
	this->SetMuunMountTime(sMain->GetInt32("Muun.MountTime", 0));

	this->SetMaxExpandedInventory(sMain->GetInt32("Character.MaxExpandedInventory", 0));

	this->SetDamageModPVP(Character::DARK_WIZARD, sMain->GetInt32("Character.DWDamageModPVP", 0));
	this->SetDamageModPVP(Character::DARK_KNIGHT, sMain->GetInt32("Character.DKDamageModPVP", 0));
	this->SetDamageModPVP(Character::FAIRY_ELF, sMain->GetInt32("Character.FEDamageModPVP", 0));
	this->SetDamageModPVP(Character::MAGIC_GLADIATOR, sMain->GetInt32("Character.MGDamageModPVP", 0));
	this->SetDamageModPVP(Character::DARK_LORD, sMain->GetInt32("Character.DLDamageModPVP", 0));
	this->SetDamageModPVP(Character::SUMMONER, sMain->GetInt32("Character.SMDamageModPVP", 0));
	this->SetDamageModPVP(Character::RAGE_FIGHTER, sMain->GetInt32("Character.RFDamageModPVP", 0));
	this->SetDamageModPVP(Character::GROW_LANCER, sMain->GetInt32("Character.GLDamageModPVP", 0));
	this->SetDamageModPVP(Character::RUNE_WIZARD, sMain->GetInt32("Character.RWDamageModPVP", 0));
	this->SetDamageModPVP(Character::SLAYER, sMain->GetInt32("Character.SLDamageModPVP", 0));
	this->SetDamageModPVP(Character::GUN_CRUSHER, sMain->GetInt32("Character.GCDamageModPVP", 100));

	this->SetDamageModPVE(Character::DARK_WIZARD, sMain->GetInt32("Character.DWDamageModPVE", 0));
	this->SetDamageModPVE(Character::DARK_KNIGHT, sMain->GetInt32("Character.DKDamageModPVE", 0));
	this->SetDamageModPVE(Character::FAIRY_ELF, sMain->GetInt32("Character.FEDamageModPVE", 0));
	this->SetDamageModPVE(Character::MAGIC_GLADIATOR, sMain->GetInt32("Character.MGDamageModPVE", 0));
	this->SetDamageModPVE(Character::DARK_LORD, sMain->GetInt32("Character.DLDamageModPVE", 0));
	this->SetDamageModPVE(Character::SUMMONER, sMain->GetInt32("Character.SMDamageModPVE", 0));
	this->SetDamageModPVE(Character::RAGE_FIGHTER, sMain->GetInt32("Character.RFDamageModPVE", 0));
	this->SetDamageModPVE(Character::GROW_LANCER, sMain->GetInt32("Character.GLDamageModPVE", 0));
	this->SetDamageModPVE(Character::RUNE_WIZARD, sMain->GetInt32("Character.RWDamageModPVE", 0));
	this->SetDamageModPVE(Character::SLAYER, sMain->GetInt32("Character.SLDamageModPVE", 0));
	this->SetDamageModPVE(Character::GUN_CRUSHER, sMain->GetInt32("Character.GCDamageModPVE", 100));

	this->SetOfflineAttack(sMain->GetBool("Character.OfflineAttack", false));
	this->SetOfflineMove(sMain->GetBool("Character.OfflineMove", false));
	this->SetOfflineAttackMinLevel(sMain->GetInt32("Character.OfflineAttackMinLevel", 0));
	this->SetOfflineAttackMaxLevel(sMain->GetInt32("Character.OfflineAttackMaxLevel", 0));
	this->SetOfflineCloseWhenWrongLevel(sMain->GetBool("Character.OfflineCloseWhenWrongLevel", false));
	this->SetOfflineAttackTime(sMain->GetInt32("Character.OfflineAttackTime", 0) * IN_MILLISECONDS);
	this->SetOfflineAttackCount(sMain->GetInt32("Character.OfflineAttackCount", 0));
	this->SetOfflineAttackOnDisconnect(sMain->GetBool("Character.OfflineAttackOnDisconnect", false));
	this->SetOfflineDisconnectTime(sMain->GetInt32("Character.OfflineDisconnectTime", 0) * IN_MILLISECONDS);
	this->SetOfflineSkillTime(sMain->GetInt32("Character.OfflineSkillTime", 0));
	this->SetOfflineDieDisconnectTime(sMain->GetInt32("Character.OfflineDieDisconnectTime", 0) * MINUTE * IN_MILLISECONDS);
	this->SetOfflineAttackZenLoot(sMain->GetBool("Character.OfflineAttackZenLoot", false));
	this->SetOfflineAttackDetailAutoloot(sMain->GetBool("Character.OfflineAttackDetailAutoloot", false));
	this->SetOfflineAttackBuffItem(sMain->GetBool("Character.OfflineAttackBuffItem", false));
	this->SetOfflineAttackGoblinPointsGain(sMain->GetBool("Character.OfflineAttackGoblinPointsGain", false));
	this->SetOfflineAttackFlag(sMain->GetUInt8("Character.OfflineAttackFlag", 0));
	this->SetOfflineAttackMonsterAICheck(sMain->GetBool("Character.OfflineAttackMonsterAICheck", false));
	this->SetOfflineAttackIPCount(sMain->GetInt32("Character.OfflineAttackIPCount", 0));

	this->SetPVPEnabled(sMain->GetBool("Character.PVPEnabled", false));

	this->SetSessionTimeout(sMain->GetInt32("GameServer.SessionTimeout", 0) * IN_MILLISECONDS);

	this->SetMultiAttackSkillCheck(sMain->GetBool("GameServer.MultiAttackSkillCheck", false));

	this->SetActive(sMain->GetBool("GameServer.Active", false));
	this->SetInactiveTime(sMain->GetInt32("GameServer.InactiveTime", 0) * IN_MILLISECONDS);

	this->SetAdministratorAuthorization(sMain->GetBool("GameServer.AdministratorAuthorization", false));
	this->SetCharacterOnlineOnLogin(sMain->GetBool("GameServer.CharacterOnlineOnLogin", false));

	this->SetAntiHackSkillHeadcodeCheck(sMain->GetBool("AntiHack.SkillHeadcodeCheck", false));
	this->SetAntiHackCoordinateAttackCheck(sMain->GetBool("AntiHack.CoordinateAttackCheck", false));
	this->SetAntiHackWalkCheck1(sMain->GetBool("AntiHack.WalkCheck1", false));
	this->SetAntiHackWalkCheckDistance1(sMain->GetInt32("AntiHack.WalkCheckDistance1", false));
	this->SetAntiHackWalkCheckError1(sMain->GetInt32("AntiHack.WalkCheckError1", false));
	this->SetAntiHackWalkCheck2(sMain->GetBool("AntiHack.WalkCheck2", false));
	this->SetAntiHackWalkCheck3(sMain->GetBool("AntiHack.WalkCheck3", false));
	this->SetAntiHackWalkCheck4(sMain->GetBool("AntiHack.WalkCheck4", false));
	this->SetAntiHackWalkCheck5(sMain->GetBool("AntiHack.WalkCheck5", false));
	this->SetAntiHackCoordinateVerify(sMain->GetBool("AntiHack.CoordinateVerify", false));
	this->SetAntiHackCoordinateVerifyTime(sMain->GetInt32("AntiHack.CoordinateVerifyTime", 0) * IN_MILLISECONDS);
	
	this->SetSkillBlessStrenghtenerFix(sMain->GetBool("GameServer.SkillBlessStrenghtenerFix", false));

	this->SetEvomonEnabled(sMain->GetBool("Evomon.Enabled", false));
	this->SetEvomonPartyReward(sMain->GetBool("Evomon.PartyReward", false));
	this->SetEvomonPartySpecialReward(sMain->GetBool("Evomon.PartySpecialReward", false));
	this->SetEvomonSend(sMain->GetBool("Evomon.Send", false));

	this->SetLuckyItemFix(sMain->GetBool("LuckyItem.Fix", false));
	this->SetLuckyItemFixCount(sMain->GetInt32("LuckyItem.FixCount", 0));

	this->SetChaosMachineSafeItemMove(sMain->GetBool("GameServer.ChaosMachineSafeItemMove", false));

	this->SetFereaBossHPRecoveryTime(sMain->GetInt32("GameServer.FereaBossHPRecoveryTime", 0) * IN_MILLISECONDS);
	this->SetFereaBossHPRecoveryCount(sMain->GetInt32("GameServer.FereaBossHPRecoveryCount", 0));

	this->SetDarkRavenAutorepair(sMain->GetBool("GameServer.DarkRavenAutorepair", false));
	this->SetItemBuffCheck(sMain->GetBool("GameServer.ItemBuffCheck", false));
	this->SetMonsterTeleportOutOfRange(sMain->GetBool("GameServer.MonsterTeleportOutOfRange", false));
	this->SetMonsterAttackWallCheck(sMain->GetBool("GameServer.MonsterAttackWallCheck", false));
	this->SetReflectFix(sMain->GetBool("GameServer.ReflectFix", false));
	this->SetFullInventoryToGremoryCase(sMain->GetBool("GameServer.FullInventoryToGremoryCase", false));
	this->SetGremoryCaseFreeSpaceCheck(sMain->GetBool("GremoryCase.FreeSpaceCheck", true));

	this->SetAntiDupeBoxEnabled(sMain->GetBool("GameServer.AntiDupeBoxEnabled", false));
	this->SetAntiDupeBoxCheck(IsServerIN(sMain->GetString("GameServer.AntiDupeBoxServer", "9999")));
	this->SetAntiDupeBoxCheckTime(sMain->GetTime("GameServer.AntiDupeBoxCheckTime", 0) * IN_MILLISECONDS);
	this->SetAntiDupeBoxRemove(sMain->GetBool("GameServer.AntiDupeBoxRemove", false));
	this->SetAntiDupeBlock(sMain->GetBool("GameServer.AntiDupeBlock", false));
	this->SetAntiDupeKick(sMain->GetBool("GameServer.AntiDupeKick", false));
	this->SetTransactionRollbackCurrency(sMain->GetBool("GameServer.TransactionRollbackCurrency", false));

	this->SetTempDualWeaponDmg1(sMain->GetInt32("GameServer.TempDualWeaponDmg1", 0));
	this->SetTempDualWeaponDmg2(sMain->GetInt32("GameServer.TempDualWeaponDmg2", 0));

	this->SetAttackSpeedCheck(sMain->GetBool("GameServer.AttackSpeedCheck", false));
	this->SetAttackSpeedCount(sMain->GetInt32("GameServer.AttackSpeedCount", false));
	this->SetAttackSpeedRange(sMain->GetInt32("GameServer.AttackSpeedRange", false));
	this->SetAttackSpeedKick(sMain->GetBool("GameServer.AttackSpeedKick", false));
	this->SetAttackSpeedBan(sMain->GetBool("GameServer.AttackSpeedBan", false));

	this->SetLastManStandingEnabled(IsServerIN(sMain->GetString("LastManStanding.Server", "9999")));
	this->SetLastManStandingDuration(sMain->GetTime("LastManStanding.Duration", 0) * IN_MILLISECONDS);
	this->SetLastManStandingKillScore(sMain->GetInt32("LastManStanding.KillScore", 0));
	this->SetLastManStandingWinScore(sMain->GetInt32("LastManStanding.WinScore", 0));
	this->SetLastManStandingMaxPerPC(sMain->GetInt32("LastManStanding.MaxPerPC", 0));
	this->SetLastManStandingDamageReduction(sMain->GetInt32("LastManStanding.DamageReduction", 0));
	this->SetLastManStandingMinLevel(sMain->GetInt32("LastManStanding.MinLevel", 0));
	this->SetLastManStandingClassic(sMain->GetBool("LastManStanding.Classic", false));
	this->SetLastManStandingHideCharacter(sMain->GetBool("LastManStanding.HideCharacter", false));
	this->SetLastManStandingLevelRange(sMain->GetInt32("LastManStanding.LevelRange", 0));

	this->SetPKBossEnabled(IsServerIN(sMain->GetString("GameServer.PKBossServer", "9999")));
	this->SetPKBossTime(sMain->GetTime("GameServer.PKBossTime", 0) * IN_MILLISECONDS);
	this->SetAttackDebuffFirstHit(sMain->GetBool("GameServer.AttackDebuffFirstHit", false));
	this->SetTradeInterfaceCancelBan(sMain->GetBool("GameServer.TradeInterfaceCancelBan", false));

	this->SetServerWarpTransactionCheck1(sMain->GetBool("GameServer.ServerWarpTransactionCheck1", false));
	this->SetServerWarpTransactionCheck2(sMain->GetBool("GameServer.ServerWarpTransactionCheck2", false));
	this->SetServerWarpTransactionCheck3(sMain->GetBool("GameServer.ServerWarpTransactionCheck3", false));

	this->SetDupeCompleteBan(sMain->GetBool("GameServer.DupeCompleteBan", false));

	this->SetPersonalStoreLockSamePC(sMain->GetBool("GameServer.PersonalStoreLockSamePC", false));

	this->SetWalkSpeedDistance(sMain->GetInt32("GameServer.WalkSpeedDistance", 0));
	this->SetWalkSpeedCount(sMain->GetInt32("GameServer.WalkSpeedCount", 0));
	this->SetWalkSpeedFix(sMain->GetBool("GameServer.WalkSpeedFix", false));
	this->SetWalkSpeedKick(sMain->GetBool("GameServer.WalkSpeedKick", false));
	this->SetWalkSpeedBan(sMain->GetBool("GameServer.WalkSpeedBan", false));

	this->SetHackCheckEnabled(sMain->GetBool("GameServer.HackCheckEnabled", false));
	this->SetHackCheckResetTime(sMain->GetTime("GameServer.HackCheckResetTime", 0) * IN_MILLISECONDS);
	this->SetHackCheckTime(sMain->GetTime("GameServer.HackCheckTime", 0) * IN_MILLISECONDS);
	this->SetHackCheckLoopTime(sMain->GetTime("GameServer.HackCheckLoopTime", 0) * IN_MILLISECONDS);
	this->SetHackCheckCount(sMain->GetInt32("GameServer.HackCheckCount", 0));
	this->SetHackCheckFrameCount(sMain->GetInt32("GameServer.HackCheckFrameCount", 0));
	this->SetHackCheckKick(sMain->GetBool("GameServer.HackCheckKick", false));
	this->SetHackCheckBan(sMain->GetBool("GameServer.HackCheckBan", false));
	this->SetHackCheckFullBan(sMain->GetBool("GameServer.HackCheckFullBan", false));

	this->SetNixiesLakeEnabled(IsServerIN(sMain->GetString("NixiesLake.Server", "9999")));
	this->SetNixiesLakeStandbyTime(sMain->GetTimeMS("NixiesLake.StandbyTime", 0));
	this->SetNixiesLakeClosedTime(sMain->GetTimeMS("NixiesLake.ClosedTime", 0));
	this->SetNixiesLakeBattleTime(sMain->GetTimeMS("NixiesLake.BattleTime", 0));
	this->SetNixiesLakeFree(sMain->GetBool("NixiesLake.Free", false));
	this->SetNixiesLakeBossRecoveryStandyTime(sMain->GetTimeMS("NixiesLake.BossRecoveryStandyTime", 0));
	this->SetNixiesLakeBossRecoveryPercent(sMain->GetInt32("NixiesLake.BossRecoveryPercent", 0));
	this->SetNixiesLakeBossRecoveryTime(sMain->GetTimeMS("NixiesLake.BossRecoveryTime", 0));
	this->SetNixiesLakeItemBag1(sMain->GetString("NixiesLake.ItemBag1", ""));
	this->SetNixiesLakeItemBag2(sMain->GetString("NixiesLake.ItemBag2", ""));
	this->SetNixiesLakeItemBag3(sMain->GetString("NixiesLake.ItemBag3", ""));
	this->SetNixiesLakeItemBag4(sMain->GetString("NixiesLake.ItemBag4", ""));
	this->SetNixiesLakeItemBag5(sMain->GetString("NixiesLake.ItemBag5", ""));

	this->SetLabyrinthOfDimensionsEnabled(IsServerIN(sMain->GetString("LabyrinthOfDimensions.Server", "9999")));
	this->SetLabyrinthOfDimensionsRequiredMaster(sMain->GetBool("LabyrinthOfDimensions.RequiredMaster", false));
	this->SetLabyrinthOfDimensionsRequiredLevel(sMain->GetInt32("LabyrinthOfDimensions.RequiredLevel", 0));
	this->SetLabyrinthOfDimensionsGoblinScore(sMain->GetInt32("LabyrinthOfDimensions.GoblinScore", 0));
	this->SetLabyrinthOfDimensionsGoblinStage(sMain->GetInt32("LabyrinthOfDimensions.GoblinStage", 0));
	this->SetLabyrinthOfDimensionsMoveOnFail(sMain->GetBool("LabyrinthOfDimensions.MoveOnFail", false));
	this->SetLabyrinthOfDimensionsMissionTime(sMain->GetTimeMS("LabyrinthOfDimensions.MissionTime", 0));
	this->SetLabyrinthOfDimensionsDayFirstScore(sMain->GetInt32("LabyrinthOfDimensions.DayFirstScore", 0));
	this->SetLabyrinthOfDimensionsStateDuration(sMain->GetInt64("LabyrinthOfDimensions.StateDuration", 0));
	this->SetLabyrinthOfDimensionsTimeScore(sMain->GetInt32("LabyrinthOfDimensions.TimeScore", 0));

	this->SetLabyrinthOfDimensionsMove(LABYRINTH_OF_DIMENSIONS_RANK_BRONZE, sMain->GetInt32("LabyrinthOfDimensions.MoveBronze", 0));
	this->SetLabyrinthOfDimensionsMove(LABYRINTH_OF_DIMENSIONS_RANK_SILVER, sMain->GetInt32("LabyrinthOfDimensions.MoveSilver", 0));
	this->SetLabyrinthOfDimensionsMove(LABYRINTH_OF_DIMENSIONS_RANK_GOLD, sMain->GetInt32("LabyrinthOfDimensions.MoveGold", 0));
	this->SetLabyrinthOfDimensionsMove(LABYRINTH_OF_DIMENSIONS_RANK_LEGEND, sMain->GetInt32("LabyrinthOfDimensions.MoveLegend", 0));

	this->SetExcellentDamageRateLimit(sMain->GetInt32("GameServer.ExcellentDamageRateLimit", 0));
	this->SetCriticalDamageRateLimit(sMain->GetInt32("GameServer.CriticalDamageRateLimit", 0));
	this->SetDecreaseDamageRateLimit(sMain->GetInt32("GameServer.DecreaseDamageRateLimit", 0));
	this->SetReflectDamageRateLimit(sMain->GetInt32("GameServer.ReflectDamageRateLimit", 0));
	this->SetIgnoreDefenseRateLimit(sMain->GetInt32("GameServer.IgnoreDefenseRateLimit", 0));
	this->SetShieldDefenseRateLimit(sMain->GetInt32("GameServer.ShieldDefenseRateLimit", 0));

	this->SetMaxSocketWear(sMain->GetInt32("GameServer.MaxSocketWear", 0));

	this->SetAntiFloodCount(sMain->GetInt32("GameServer.AntiFloodCount", 0));
	this->SetAntiFloodKick(sMain->GetBool("GameServer.AntiFloodKick", 0));

	this->SetWelcomeNotice(sMain->GetBool("GameServer.WelcomeNotice", false));

	this->SetCheatScanEnabled(sMain->GetBool("CheatScan.Enabled", false));
	this->SetCheatScanKick(sMain->GetBool("CheatScan.Kick", false));
	this->SetCheatScanBan(sMain->GetBool("CheatScan.Ban", false));
	this->SetCheatScanCount(sMain->GetInt32("CheatScan.Count", 0));
	this->SetCheatScanTime(sMain->GetTime("CheatScan.Time", 0) * IN_MILLISECONDS);

	this->SetCastleDeepEnabled(sMain->GetBool("CastleDeep.Enabled", false));

	this->SetWorldBossEnabled(sMain->GetBool("WorldBoss.Enabled", false));
	this->SetWorldBossRequiredPlayer(sMain->GetInt32("WorldBoss.RequiredPlayer", 0));
	this->SetWorldBossStartNotification(sMain->GetInt32("WorldBoss.StartNotification", 0));
	this->SetWorldBossGuildPK(sMain->GetBool("WorldBoss.GuildPK", false));
	this->SetWorldBossPKGAP(sMain->GetInt32("WorldBoss.PKGAP", 0));
	this->SetWorldBossPKRange(sMain->GetInt32("WorldBoss.PKRange", 0));
	this->SetWorldBossRequiredPointsToAttack(sMain->GetInt32("WorldBoss.RequiredPointsToAttack", 0));
	this->SetWorldBossIncreaseDamagePerPoint(sMain->GetInt32("WorldBoss.IncreaseDamagePerPoint", 0));
	this->SetWorldBossMaxPKPoints(sMain->GetInt32("WorldBoss.MaxPKPoints", 0));
	this->SetWorldBossNormalRespawn(sMain->GetBool("WorldBoss.NormalRespawn", false));
	this->SetWorldBossItemBag(sMain->GetString("WorldBoss.ItemBag", ""));
	this->SetWorldBossAOERange(sMain->GetInt32("WorldBoss.AOERange", 0));

	this->SetSkillTeleportStateTime(TELEPORT_NONE, sMain->GetInt32("SkillTeleport.StateTime0", 0));
	this->SetSkillTeleportStateTime(TELEPORT_STANDBY, sMain->GetInt32("SkillTeleport.StateTime1", 0));
	this->SetSkillTeleportStateTime(TELEPORT_READY, sMain->GetInt32("SkillTeleport.StateTime2", 0));
	this->SetSkillTeleportStateTime(TELEPORT_UNK, sMain->GetInt32("SkillTeleport.StateTime3", 0));

	this->SetSkillTeleportStateAttack(TELEPORT_NONE, sMain->GetBool("SkillTeleport.StateAttack0", false));
	this->SetSkillTeleportStateAttack(TELEPORT_STANDBY, sMain->GetBool("SkillTeleport.StateAttack1", false));
	this->SetSkillTeleportStateAttack(TELEPORT_READY, sMain->GetBool("SkillTeleport.StateAttack2", false));
	this->SetSkillTeleportStateAttack(TELEPORT_UNK, sMain->GetBool("SkillTeleport.StateAttack3", false));

	this->SetSkillTeleportStateRemoveVP(TELEPORT_NONE, sMain->GetBool("SkillTeleport.StateRemoveVP0", false));
	this->SetSkillTeleportStateRemoveVP(TELEPORT_STANDBY, sMain->GetBool("SkillTeleport.StateRemoveVP1", false));
	this->SetSkillTeleportStateRemoveVP(TELEPORT_READY, sMain->GetBool("SkillTeleport.StateRemoveVP2", false));
	this->SetSkillTeleportStateRemoveVP(TELEPORT_UNK, sMain->GetBool("SkillTeleport.StateRemoveVP3", false));

	this->SetMiniBombEnabled(sMain->GetBool("MiniBomb.Enabled", false));
	this->SetMiniBombRequiredTicket(sMain->GetBool("MiniBomb.RequiredTicket", false));
	this->SetMiniBombReward(sMain->GetBool("MiniBomb.Reward", false));
	this->SetMiniBombRevealScore(sMain->GetInt32("MiniBomb.RevealScore", 0));
	this->SetMiniBombFoundedBombsScore(sMain->GetInt32("MiniBomb.FoundedBombsScore", 0));
	this->SetMiniBombWrongBombsScore(sMain->GetInt32("MiniBomb.WrongBombsScore", 0));
	this->SetMiniBombFailScore(sMain->GetInt32("MiniBomb.FailScore", 0));
	this->SetMiniBombClearScore(sMain->GetInt32("MiniBomb.ClearScore", 0));

	this->SetMaxAttackSpeed(Character::DARK_WIZARD, sMain->GetInt32("Character.MaxAttackSpeedDW", 0));
	this->SetMaxAttackSpeed(Character::DARK_KNIGHT, sMain->GetInt32("Character.MaxAttackSpeedDK", 0));
	this->SetMaxAttackSpeed(Character::FAIRY_ELF, sMain->GetInt32("Character.MaxAttackSpeedELF", 0));
	this->SetMaxAttackSpeed(Character::MAGIC_GLADIATOR, sMain->GetInt32("Character.MaxAttackSpeedMG", 0));
	this->SetMaxAttackSpeed(Character::DARK_LORD, sMain->GetInt32("Character.MaxAttackSpeedDL", 0));
	this->SetMaxAttackSpeed(Character::SUMMONER, sMain->GetInt32("Character.MaxAttackSpeedSUM", 0));
	this->SetMaxAttackSpeed(Character::RAGE_FIGHTER, sMain->GetInt32("Character.MaxAttackSpeedRF", 0));
	this->SetMaxAttackSpeed(Character::GROW_LANCER, sMain->GetInt32("Character.MaxAttackSpeedGL", 0));
	this->SetMaxAttackSpeed(Character::RUNE_WIZARD, sMain->GetInt32("Character.MaxAttackSpeedRW", 0));
	this->SetMaxAttackSpeed(Character::SLAYER, sMain->GetInt32("Character.MaxAttackSpeedSL", 0));
	this->SetMaxAttackSpeed(Character::GUN_CRUSHER, sMain->GetInt32("Character.MaxAttackSpeedGC", 280));

	this->SetJewelBingoEnabled(sMain->GetBool("JewelBingo.Enabled", false));
	this->SetJewelBingoRequiredTicket(sMain->GetBool("JewelBingo.RequiredTicket", false));
	this->SetJewelBingoReward(sMain->GetBool("JewelBingo.Reward", false));
	this->SetJewelBingoScoreH(sMain->GetInt32("JewelBingo.ScoreH", 0));
	this->SetJewelBingoScoreV(sMain->GetInt32("JewelBingo.ScoreV", 0));
	this->SetJewelBingoScoreD(sMain->GetInt32("JewelBingo.ScoreD", 0));
	this->SetJewelBingoScoreSpecialH(sMain->GetInt32("JewelBingo.ScoreSpecialH", 0));
	this->SetJewelBingoScoreSpecialV(sMain->GetInt32("JewelBingo.ScoreSpecialV", 0));
	this->SetJewelBingoScoreNotMatched(sMain->GetInt32("JewelBingo.ScoreNotMatched", 0));

	this->SetSecretShopWarpTime(sMain->GetTime("SecretShop.WarpTime", 0) * IN_MILLISECONDS);
	this->SetSecretShopList(sMain->GetString("SecretShop.List", ""));

	this->SetBossDebuffBlow(sMain->GetInt32("Skill.BossDebuffBlow", 0));

	this->SetAutoLoginEnabled(sMain->GetBool("GameServer.AutoLoginEnabled", false));
	this->SetAutoLoginRandomTime(sMain->GetInt32("GameServer.AutoLoginRandomTime", 0));
	this->SetAutoLoginTime(sMain->GetInt32("GameServer.AutoLoginTime", 0));

	this->SetGuildScoreCastleSiege(sMain->GetInt32("GuildScore.CastleSiege", 0));
	this->SetGuildScoreCastleSiegeMembers(sMain->GetInt32("GuildScore.CastleSiegeMembers", 0));
	this->SetGuildScoreArkaWar(sMain->GetInt32("GuildScore.ArkaWar", 0));

	this->SetHelperBuffItemUseCheck(sMain->GetBool("Helper.BuffItemUseCheck", false));

	this->SetHuntingRecordClear(sMain->GetBool("HuntingRecord.Clear", false));

	this->SetCommandGoldEnabled(sMain->GetBool("Command.Gold.Enabled", false));
	this->SetCommandGoldMaxDays(sMain->GetInt32("Command.Gold.MaxDays", 0));
	this->SetCommandGoldPrice(sMain->GetInt32("Command.Gold.Price", 0));

	this->SetSealedBloodAngelExcellentMin(sMain->GetUInt8("Mix.SealedBloodAngelExcellentMin", 0));
	this->SetSealedBloodAngelExcellentMax(sMain->GetUInt8("Mix.SealedBloodAngelExcellentMax", 0));

	this->SetWhisperLog(sMain->GetBool("GameServer.WhisperLog", false));
	this->SetAttackStunCheck(sMain->GetBool("GameServer.AttackStunCheck", false));
	this->SetDuplicatedCharacterCheck(sMain->GetBool("GameServer.DuplicatedCharacterCheck", false));

	this->SetMixSpellStonePriceDivision(SPIRIT_STONE_LESSER, sMain->GetUInt32("Mix.LesserSpellStonePriceDivision", 0));
	this->SetMixSpellStonePriceDivision(SPIRIT_STONE_MEDIUM, sMain->GetUInt32("Mix.MediumSpellStonePriceDivision", 0));
	this->SetMixSpellStonePriceDivision(SPIRIT_STONE_GREATER, sMain->GetUInt32("Mix.GreaterSpellStonePriceDivision", 0));
	this->SetMixSpellStoneRate(SPIRIT_STONE_LESSER, sMain->GetUInt8("Mix.LesserSpellStoneRate", 0));
	this->SetMixSpellStoneRate(SPIRIT_STONE_MEDIUM, sMain->GetUInt8("Mix.MediumSpellStoneRate", 0));
	this->SetMixSpellStoneRate(SPIRIT_STONE_GREATER, sMain->GetUInt8("Mix.GreaterSpellStoneRate", 0));

	this->SetTimeCheck(sMain->GetBool("GameServer.TimeCheck", false));
	this->SetTimeCheckTime(sMain->GetUInt32("GameServer.TimeCheckTime", 0));
	this->SetTimeCheckLevelMin(sMain->GetInt16("GameServer.TimeCheckLevelMin", 0));
	this->SetTimeCheckLevelMax(sMain->GetInt16("GameServer.TimeCheckLevelMax", 0));

	this->SetItemSplitEnabled(sMain->GetBool("GameServer.ItemSplitEnabled", false));

	this->SetSwampOfDarknessEnabled(sMain->GetBool("SwampOfDarkness.Enabled", false));
	this->SetSwampOfDarknessBossAppearTime(sMain->GetTimeMS("SwampOfDarkness.BossAppearTime", 0));
	this->SetSwampOfDarknessBattleTime(sMain->GetTimeMS("SwampOfDarkness.BattleTime", 0));
	this->SetSwampOfDarknessClosedTimeMin(sMain->GetTimeMS("SwampOfDarkness.ClosedTimeMin", 0));
	this->SetSwampOfDarknessClosedTimeMax(sMain->GetTimeMS("SwampOfDarkness.ClosedTimeMax", 0));
	this->SetSwampOfDarknessMaxPlayers(sMain->GetInt32("SwampOfDarkness.MaxPlayers", 0));

	this->SetComboCheckEnabled(sMain->GetBool("ComboCheck.Enabled", false));
	this->SetComboCheckTimeDiff(sMain->GetUInt32("ComboCheck.TimeDiff", 0));
	this->SetComboCheckCount(sMain->GetInt32("ComboCheck.Count", 0));
	this->SetComboCheckDatabaseLog(sMain->GetBool("ComboCheck.DatabaseLog", false));
	this->SetComboCheckStop(sMain->GetBool("ComboCheck.Stop", false));
	this->SetComboCheckKick(sMain->GetBool("ComboCheck.Kick", false));

	this->SetSkillCheckEnabled(sMain->GetBool("GameServer.SkillCheckEnabled", false));
	this->SetLuckyItemMixCheck(sMain->GetBool("GameServer.LuckyItemMixCheck", false));
	this->SetDebuffStopCheck(sMain->GetBool("GameServer.DebuffStopCheck", false));

	this->SetLoginDiskSerialMacCheck(sMain->GetBool("GameServer.LoginDiskSerialMacCheck", false));
	this->SetEventKillRankingEnabled(sMain->GetBool("Event.KillRankingEnabled", false));
	this->SetEventTimeRankingEnabled(sMain->GetBool("Event.TimeRankingEnabled", false));

	this->SetElfSkillWeaponCheck(sMain->GetBool("GameServer.ElfSkillWeaponCheck", false));
	this->SetPentagramErrtelFix(sMain->GetBool("GameServer.PentagramErrtelFix", false));
	this->SetStackExpirableItemsOnLoot(sMain->GetBool("GameServer.StackExpirableItemsOnLoot", false));

	this->SetBossStackCheck(sMain->GetBool("GameServer.BossStackCheck", false));

	this->SetNumericBaseballEnabled(sMain->GetBool("NumericBaseball.Enabled", false));
	this->SetNumericBaseballRequiredTicket(sMain->GetBool("NumericBaseball.RequiredTicket", false));
	this->SetNumericBaseballReward(sMain->GetBool("NumericBaseball.Reward", false));
	this->SetNumericBaseballScoreStrike(sMain->GetInt32("NumericBaseball.ScoreStrike", 0));
	this->SetNumericBaseballScoreBall(sMain->GetInt32("NumericBaseball.ScoreBall", 0));
	this->SetNumericBaseballScore(0, sMain->GetInt32("NumericBaseball.Score1", 0));
	this->SetNumericBaseballScore(1, sMain->GetInt32("NumericBaseball.Score2", 0));
	this->SetNumericBaseballScore(2, sMain->GetInt32("NumericBaseball.Score3", 0));
	this->SetNumericBaseballScore(3, sMain->GetInt32("NumericBaseball.Score4", 0));
	this->SetNumericBaseballScore(4, sMain->GetInt32("NumericBaseball.Score5", 0));

	this->SetChannelChangeEnabled(sMain->GetBool("GameServer.ChannelChangeEnabled", false));


	this->SetSkillMagicArrowProjectiles(sMain->GetUInt8("Skill.MagicArrowProjectiles", 0));
	this->SetSkillMagicArrowSplashDamage(sMain->GetInt32("Skill.MagicArrowSplashDamage", 0));
	this->SetSkillMagicArrowSplashTargets(sMain->GetInt32("Skill.MagicArrowSplashTargets", 0));
	this->SetSkillMagicArrowSplashDistance(sMain->GetInt32("Skill.MagicArrowSplashDistance", 0));

	this->SetSkillPlasmaBallMoveDistance(sMain->GetUInt8("Skill.PlasmaBallMoveDistance", 0));
	this->SetSkillPlasmaBallAttackDistance(sMain->GetUInt8("Skill.PlasmaBallAttackDistance", 0));
	this->SetSkillPlasmaBallMoveSpeed(sMain->GetFloat("Skill.PlasmaBallMoveSpeed", 0));
	this->SetSkillPlasmaBallAttackCount(sMain->GetUInt8("Skill.PlasmaBallAttackCount", 0));
	this->SetSkillPlasmaBallAttackSpeed(sMain->GetUInt8("Skill.PlasmaBallAttackSpeed", 0));
	this->SetSkillPlasmaBallTargetCount(sMain->GetUInt8("Skill.PlasmaBallTargetCount", 0));

	this->SetSkillBurstRWEffect1Div(sMain->GetInt32("Skill.BurstRWEffect1Div", 0));
	this->SetSkillBurstRWEffect2Div(sMain->GetInt32("Skill.BurstRWEffect2Div", 0));
	this->SetSkillBurstRWDurationAdd(sMain->GetInt32("Skill.BurstRWDurationAdd", 0));
	this->SetSkillBurstRWDurationDiv(sMain->GetInt32("Skill.BurstRWDurationDiv", 0));

	this->SetSkillHasteRWEffect1Div(sMain->GetInt32("Skill.HasteRWEffect1Div", 0));
	this->SetSkillHasteRWEffect2Div(sMain->GetInt32("Skill.HasteRWEffect2Div", 0));
	this->SetSkillHasteRWDurationAdd(sMain->GetInt32("Skill.HasteRWDurationAdd", 0));
	this->SetSkillHasteRWDurationDiv(sMain->GetInt32("Skill.HasteRWDurationDiv", 0));

	this->SetSkillDarkPlasmaAttackDistance(sMain->GetUInt8("Skill.DarkPlasmaAttackDistance", 8));
	this->SetSkillDarkPlasmaAttackSpeed(sMain->GetUInt8("Skill.DarkPlasmaAttackSpeed", 5));
	this->SetSkillDarkPlasmaTargetCount(sMain->GetUInt8("Skill.DarkPlasmaTargetCount", 5));

	this->SetAntiMacroEnabled(sMain->GetBool("AntiMacro.Enabled", 0));
	this->SetAntiMacroTime(sMain->GetUInt32("AntiMacro.Time", 0));
	this->SetAntiMacroComboEnabled(sMain->GetBool("AntiMacro.ComboEnabled", 0));
	this->SetAntiMacroComboTime(sMain->GetUInt32("AntiMacro.ComboTime", 0));

	this->SetMultiAttackCount(sMain->GetInt32("Skill.MultiAttackCount", 0));
	this->SetMultiAttackTime(sMain->GetTimeMS("Skill.MultiAttackTime", 0));
	this->SetMultiAttackNew(sMain->GetBool("Skill.MultiAttackNew", false));

	this->SetShutdownCheck(sMain->GetBool("GameServer.ShutdownCheck", false));

	this->SetDungeonEnabled(sMain->GetBool("Dungeon.Enabled", false));
	this->SetDungeonMinParty(sMain->GetInt32("Dungeon.MinParty", 0));
	this->SetDungeonWaitTime(sMain->GetInt64("Dungeon.WaitTime", 0));
	this->SetDungeonDuration(sMain->GetTimeMS("Dungeon.Duration", 0));
	this->SetDungeonSilverChestMonster(sMain->GetInt32("Dungeon.SilverChestMonster", 0));
	this->SetDungeonMaxPerPC(sMain->GetInt32("Dungeon.MaxPerPC", 0));
	this->SetDungeonEndGate(sMain->GetUInt16("Dungeon.EndGate", 0));
	this->SetDungeonGAP(sMain->GetInt32("Dungeon.GAP", 0));
	this->SetDungeonSimpleTrapTime(sMain->GetTimeMS("Dungeon.SimpleTrapTime", 0));
	this->SetDungeonSimpleTrapDamage(sMain->GetInt32("Dungeon.SimpleTrapDamage", 0));

	this->SetMixRecoveryEnabled(sMain->GetBool("MixRecovery.Enabled", false));
	this->SetMixRecoveryPriceWC(sMain->GetInt32("MixRecovery.PriceWC", 0));
	this->SetMixRecoveryPriceBless(sMain->GetInt32("MixRecovery.PriceBless", 0));
	this->SetMixRecoveryTime(sMain->GetInt32("MixRecovery.Time", 0));
	this->SetMixRecoveryFailTime(sMain->GetInt32("MixRecovery.FailTime", 0));
	this->SetMixRecoveryWarning(sMain->GetBool("MixRecovery.Warning", false));

	this->SetInventoryMountTime(sMain->GetInt32("GameServer.InventoryMountTime", 0));

	this->SetQuestMUEnabled(sMain->GetBool("QuestMU.Enabled", false));

	this->SetGuardianDisassembleItemBag(sMain->GetString("Guardian.DisassembleItemBag", ""));
	this->SetMonsterSoulPurchaseAmount(sMain->GetInt32("MonsterSoul.PurchaseAmount", 0));
	this->SetMonsterSoulDurationTime(sMain->GetInt32("MonsterSoul.DurationTime", 0));

	this->SetSkillDetectionRange(sMain->GetInt32("Skill.DetectionRange", 0));

	this->SetConnectTimeKick(sMain->GetTimeMS("GameServer.ConnectTimeKick", 0));

	this->SetItemUpgradeSuccessRate(0, sMain->GetUInt8("ItemUpgrade.SuccessRate10", 60));
	this->SetItemUpgradeSuccessRate(1, sMain->GetUInt8("ItemUpgrade.SuccessRate11", 60));
	this->SetItemUpgradeSuccessRate(2, sMain->GetUInt8("ItemUpgrade.SuccessRate12", 55));
	this->SetItemUpgradeSuccessRate(3, sMain->GetUInt8("ItemUpgrade.SuccessRate13", 55));
	this->SetItemUpgradeSuccessRate(4, sMain->GetUInt8("ItemUpgrade.SuccessRate14", 50));
	this->SetItemUpgradeSuccessRate(5, sMain->GetUInt8("ItemUpgrade.SuccessRate15", 50));

	this->SetCustomBossSummonTime(sMain->GetTimeMS("CustomBoss.SummonTime", 0));
	this->SetCustomBossSummonCount(sMain->GetInt32("CustomBoss.SummonCount", 0));
	this->SetCustomBossSummonDespawnTime(sMain->GetTimeMS("CustomBoss.SummonDespawnTime", 0));

	this->SetSpecialMapCharacterCount(sMain->GetInt32("SpecialMap.CharacterCount", 0));
	this->SetSpecialMapPVP(sMain->GetBool("SpecialMap.PVP", false));

	this->SetLogDamagePVP(sMain->GetBool("Log.DamagePVP", false));
	this->SetLogDamagePVPToDB(sMain->GetBool("Log.DamagePVPToDB", false));

	this->SetErrtelToRecovery(sMain->GetBool("MixRecovery.ErrtelOnFail", false));
	this->SetErrtelRemoveFailCount(sMain->GetInt32("MixRecovery.ErrtelRemoveFailCount", 5));
	this->SetErrtelMixFailCount(sMain->GetInt32("MixRecovery.ErrtelMixFailCount", 1));

	this->SetAntiHackSkillLogCount(sMain->GetInt32("AntiHack.SkillLogCount", 3));
	this->SetAntiHackSkillCount(sMain->GetInt32("AntiHack.SkillCount", 0));
	this->SetAntiHackSkillKick(sMain->GetBool("AntiHack.SkillKick", false));

	this->SetAntiHackSkillCountContinue(sMain->GetInt32("AntiHack.SkillCountContinue", 2));
	this->SetAntiHackSkillContinue(sMain->GetBool("AntiHack.SkillContinue", false));

	this->SetMajesticSkillDamageNormal(sMain->GetBool("MajesticSkillDamageNormal", true));
	this->SetMajesticAddDamageNormal(sMain->GetBool("MajesticAddDamageNormal", true));

	this->SetSkillSleepReuseTime(sMain->GetUInt32("Skill.SleepReuseTime", 1000));

	this->SetOnlyBaseStatForEquipment(sMain->GetBool("OnlyBaseStatForEquipment", true));

	this->SetAttackRangeTolerance(sMain->GetInt32("AttackRangeTolerance", 1));

	this->SetSkillReflectLimit(sMain->GetInt32("Skill.ReflectLimit", 25));

	this->SetTeleportFix(sMain->GetBool("TeleportFix", true));
	this->SetGLElementalDamageFix(sMain->GetBool("GLElementalDamageFix", true));

	SplitToVector(this->m_TeleportBuffRestrict, sMain->GetString("TeleportRestrictBuff", "61 72 146 147"));
	SplitToVector(this->m_TeleportBuffCoordinate, sMain->GetString("TeleportCoordinateBuff", "57 241"));

	this->SetRareIceDragonRate(sMain->GetUInt8("RareIceDragonRate", 5));

	this->SetAttackTimeNullKick(sMain->GetBool("AttackTime.NullKick", true));

	this->SetPlayerWalkSpeed(sMain->GetInt32("PlayerWalkSpeed", 400));

	if ( this->IsActive() != this->IsActiveStatus() )
	{
		this->SetActiveStatus(this->IsActive());
		this->SetInactiveTick(MyGetTickCount());
		this->SetInactiveNotificationTick(MyGetTickCount());
	}

	sLog->outInfo(LOG_DEFAULT, "Common Settings loaded in %u msec...", GetMSTimeDiffToNow(oldMSTime));
}

void GameServer::LoadCashShopSettings()
{
	this->SetCashShopEnabled(sMain->GetBool("CashShop.Enabled", false));
	this->SetCashShopItemListNumber(sMain->GetInt32("CashShop.ItemListNumber", 0));
	this->SetCashShopItemListMonth(sMain->GetInt32("CashShop.ItemListMonth", 0));
	this->SetCashShopItemListYear(sMain->GetInt32("CashShop.ItemListYear", 0));
	this->SetCashShopBannerNumber(sMain->GetInt32("CashShop.BannerNumber", 0));
	this->SetCashShopBannerMonth(sMain->GetInt32("CashShop.BannerMonth", 0));
	this->SetCashShopBannerYear(sMain->GetInt32("CashShop.BannerYear", 0));
	this->SetCashShopItemIP(sMain->GetString("CashShop.ItemIp", "").c_str());
	this->SetCashShopItemDir(sMain->GetString("CashShop.ItemDir", "").c_str());
	this->SetCashShopItemDirTest(sMain->GetString("CashShop.ItemDirTest", "").c_str());
	this->SetCashShopBannerIP(sMain->GetString("CashShop.BannerIp", "").c_str());
	this->SetCashShopBannerDir(sMain->GetString("CashShop.BannerDir", "").c_str());
	this->SetCashShopBannerDirTest(sMain->GetString("CashShop.BannerDirTest", "").c_str());
	this->SetCashShopCoinUpdateTime(sMain->GetInt32("CashShop.CoinUpdateTime", 0) * IN_MILLISECONDS);
	this->SetCashShopGiftUpdateTime(sMain->GetInt32("CashShop.GiftUpdateTime", 0) * IN_MILLISECONDS);
	this->SetCashShopUseJoin(sMain->GetBool("CashShop.UseJoin", false));
	this->SetCashShopSafeZone(sMain->GetBool("CashShop.SafeZone", false));
	this->SetCashShopDiscountWC(sMain->GetUInt8("CashShop.DiscountWC", 0));
	this->SetCashShopDiscountGP(sMain->GetUInt8("CashShop.DiscountGP", 0));
	this->SetCashShopDiscountDate(sMain->GetInt64("CashShop.DiscountDate", 0));
}

void GameServer::LoadBaseData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Base Data...");

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GAMESERVER_SELECT);
	stmt->setUInt16(0, this->GetServerCode());
	PreparedQueryResult result = LoginDatabase.Query(stmt);

	if ( result )
	{
		Field* fields = result->Fetch();
		this->SetDefaultWorld(fields[6].GetUInt16());
		this->SetDefaultX(fields[7].GetInt16());
		this->SetDefaultY(fields[8].GetInt16());
		this->SetServerMode(fields[9].GetUInt8());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded base data");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void GameServer::LoadFilter()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Filter...");

	LIST_CLEAR(FilterList::iterator, this->filter_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM filter_text");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			filter_data * add_word = new filter_data;

			add_word->SetWord(fields[0].GetString());
			add_word->SetWordLower(add_word->GetWord());
			add_word->SetWordUpper(add_word->GetWord());
			add_word->ConvertWordLowerToLower();
			add_word->ConvertWordUpperToUpper();
			add_word->flags.set(fields[1].GetUInt8());
			add_word->action.set(fields[2].GetUInt8());
			add_word->SetReplace(fields[3].GetString());

			this->filter_list.push_back(add_word);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u filter definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void GameServer::LoadNotice()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Notice...");

	LIST_CLEAR(NoticeList::iterator, this->notice_list);
	uint32 count = 0;

	PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_SRVCFG_NOTICE_SELECT);
	stmt->setUInt16(0, sGameServer->GetTransferServer());

	PreparedQueryResult result = GameServerDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			notice_data * add_notice = new notice_data;
			add_notice->SetNotice(fields[1].GetString());
			add_notice->SetType(fields[2].GetUInt8());
			add_notice->SetWorld(fields[3].GetUInt16());
			add_notice->SetFlag(fields[4].GetUInt8());
			add_notice->SetTime(fields[5].GetUInt32());
			//add_notice->SetGroup(fields[6].GetUInt16());
			//add_notice->SetSubGroup(fields[7].GetUInt16());

			this->notice_list.push_back(add_notice);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u notice definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool GameServer::FilterText(std::string & text, uint8 flags)
{
	StringLower(text_lower, text);
	StringUpper(text_upper, text);

	for ( FilterList::const_iterator it = this->filter_list.begin(); it != this->filter_list.end(); ++it )
	{
		if ( !((*it)->flags.get() & flags) )
			continue;

		switch ( (*it)->action.get() )
		{
		case FILTER_ACTION_REPLACE:
			{
				Util::ReplaceString(text, (*it)->GetWord(), (*it)->GetReplace());
			} break;

		case FILTER_ACTION_CLEAR:
			{
				Util::ReplaceString(text, (*it)->GetWord(), "");
			} break;

		case FILTER_ACTION_REJECT:
			{
				if ( text_lower == (*it)->GetWordLower() )
					return false;
			} break;
		}
	}

	return true;
}

void GameServer::Update()
{
	this->UpdateNotice();
	this->UpdateAccountsDisconnect();
}

void GameServer::UpdateNotice()
{
	uint32 cur_time = GetTickCount();

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( NoticeList::iterator notice = this->notice_list.begin(); notice != this->notice_list.end(); ++notice )
	{
		notice_data * pNotice = *notice;

		if ( !pNotice )
			continue;

		if ( (cur_time - pNotice->GetTick()) < pNotice->GetTime() )
			continue;

		pNotice->SetTick(cur_time);

		for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
				continue;

			if ( pNotice->GetWorld() != pPlayer->GetWorldId() && pNotice->GetWorld() != world_type(-1) )
				continue;

			if ( pNotice->IsFlag(NOTICE_FLAG_PK) && !pPlayer->IsMurder() )
				continue;

			if ( pNotice->IsFlag(NOTICE_FLAG_ADMIN) && !pPlayer->IsAdministrator() )
				continue;

			if ( pNotice->IsFlag(NOTICE_FLAG_HERO) && !pPlayer->IsHero() )
				continue;

			pPlayer->SendNotice(NoticeType(pNotice->GetType()), pNotice->GetNotice().c_str());
		}
	}
}

void GameServer::UpdateShutdown()
{
	if ( this->IsConnectionDisabled() )
	{
		return;
	}

	if ( !this->IsActiveStatus() )
	{
		if ((MyGetTickCount() - this->GetInactiveNotificationTick()) > (MINUTE * IN_MILLISECONDS))
		{
			this->SetInactiveNotificationTick(MyGetTickCount());

			PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
			for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
			{
				Player* pPlayer = it->second;

				if ( !pPlayer )
				{
					continue;
				}

				pPlayer->SendNotice(NOTICE_GLOBAL, "[Announcement]: Game has been updated.");
				pPlayer->SendNotice(NOTICE_GLOBAL, "[Announcement]: Please select server again.");

				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[Announcement]: Game has been updated.");
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[Announcement]: Please select server again.");
			}
		}

		if ( (MyGetTickCount() - this->GetInactiveTick()) > this->GetInactiveTime() )
		{
			this->SetConnectionDisabled(true);
			return;
		}
	}
	
	if ( !this->IsShutdown() )
		return;

	this->ReduceShutdownTime(1);

	if ( this->GetShutdownTime() > 1 )
	{
		if ( (this->GetShutdownTime() <= 10) || ((this->GetShutdownTime() <= 60) && ((this->GetShutdownTime() % 10) == 0)) || ((this->GetShutdownTime() % 60) == 0) )
		{
			sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "Server Shutdown in %s", secsToTimeString(this->GetShutdownTime()).c_str());
			sLog->outInfo("root", "Server Shutdown in %s", secsToTimeString(this->GetShutdownTime()).c_str());
		}
	}
	else if ( this->GetShutdownTime() == 1 )
	{
		sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "Closing Server");
		sLog->outInfo("root", "Closing Server");
	}

	if ( this->GetShutdownTime() <= 0 )
	{
		this->SetConnectionDisabled(true);
		this->SetShutdown(false);
		this->SetShutdownTime(0);
		return;
	}
}

void GameServer::ProcessSignal(uint16 signal, int32 data_1, int32 data_2, int32 data_3)
{
	std::string const& data_string = std::to_string(int64(data_1)) + " " + std::to_string(int64(data_2)) + " " + std::to_string(int64(data_3));

	switch ( signal )
	{
	case GAMESERVER_SIGNAL_SHUTDOWN:
		{
			sGameServer->SetShutdownTime(data_1);
			sGameServer->SetShutdown(!sGameServer->IsShutdown());
		} break;

	case GAMESERVER_SIGNAL_SHOP:
		{
			ChatHandler(nullptr).ReloadShop(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_QUEST:
		{
			ChatHandler(nullptr).ReloadQuest(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_EVENT_MANAGER:
		{
			ChatHandler(nullptr).ReloadEvent(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_MONSTER:
		{
			ChatHandler(nullptr).ReloadMonster(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_CONFIG:
		{
			ChatHandler(nullptr).ReloadConfig(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_ITEM:
		{
			ChatHandler(nullptr).ReloadItem(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_CHARACTER:
		{
			ChatHandler(nullptr).ReloadCharacter(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_SKILL:
		{
			ChatHandler(nullptr).ReloadSkill(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_TELEPORT:
		{
			ChatHandler(nullptr).ReloadTeleport(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_CASH_SHOP:
		{
			ChatHandler(nullptr).ReloadCashShop(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_MIX:
		{
			ChatHandler(nullptr).ReloadMix(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_WORLD:
		{
			ChatHandler(nullptr).ReloadWorld(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_ACTIVE:
		{
			ChatHandler(nullptr).AdminCommandAcitve(data_string.c_str());
		} break;

	case GAMESERVER_SIGNAL_PENTAGRAM:
		{
			ChatHandler(nullptr).ReloadPentagram(data_string.c_str());
		} break;
	}
}

void GameServer::LoadGoblinPoint()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Goblin Point...");

	LIST_CLEAR(GoblinPointDataList::iterator, this->goblin_point_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM goblin_points_data");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			GoblinPointData * add_data = new GoblinPointData;
			add_data->SetMonsterLevelMin(fields[0].GetInt16());
			add_data->SetMonsterLevelMax(fields[1].GetInt16());
			add_data->SetPlayerLevelMin(fields[2].GetInt16());
			add_data->SetPlayerLevelMax(fields[3].GetInt16());

			this->goblin_point_list.push_back(add_data);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u goblin point definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void GameServer::LoadNonPKTime()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Non Pk Time...");

	LIST_CLEAR(NonPKTimeList::iterator, this->non_pk_time_list);
	uint32 count = 0;

	PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_SRVCFG_MISC_NON_PK_TIME_SELECT);
	stmt->setUInt16(0, sGameServer->GetTransferServer());

	PreparedQueryResult result = GameServerDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			NonPKTimeData * add_data = new NonPKTimeData;
			add_data->SetStartHour(fields[1].GetUInt8());
			add_data->SetStartMinute(fields[2].GetUInt8());
			add_data->SetEndHour(fields[3].GetUInt8());
			add_data->SetEndMinute(fields[4].GetUInt8());

			this->non_pk_time_list.push_back(add_data);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u non pk time definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool GameServer::IsGoblinPointLevelAllowed(Player* pPlayer, Monster* pMonster)
{
	if ( !pPlayer || !pMonster )
		return false;

	if ( this->goblin_point_list.empty() )
		return true;

	for ( GoblinPointDataList::const_iterator it = this->goblin_point_list.begin(); it != this->goblin_point_list.end(); ++it )
	{
		if (pMonster->GetLevel() >= (*it)->GetMonsterLevelMin() && pMonster->GetLevel() <= (*it)->GetMonsterLevelMax() && 
			pPlayer->GetTotalLevel() >= (*it)->GetPlayerLevelMin() && pPlayer->GetTotalLevel() <= (*it)->GetPlayerLevelMax())
		{
			return true;
		}
	}

	return false;
}

void GameServer::NonPKTimeUpdate()
{
	Custom::SystemTimer time = Custom::SystemTimer();

	int32 seconds = time.GetHour() * HOUR + time.GetMinute() * MINUTE;
	bool allowed = true;

	for ( NonPKTimeList::const_iterator it = this->non_pk_time_list.begin(); it != this->non_pk_time_list.end(); ++it )
	{
		if ( seconds >= (*it)->GetStartSecond() && seconds <= (*it)->GetEndSecond() )
		{
			allowed = false;
		}
	}

	this->SetPKTimeAllowed(allowed);
}

void LOG_ITEM_DROP(Player* pPlayer, Item * item, std::string const& comment)
{
	if (!sGameServer->IsItemDropLogDB())
	{
		return;
	}

	if (!pPlayer)
	{
		return;
	}

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_ITEM_DROP);

	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, pPlayer->GetAccountData()->GetGUID());
	stmt->setString(2, pPlayer->GetAccountData()->GetAccount());
	stmt->setUInt32(3, pPlayer->GetGUID());
	stmt->setString(4, pPlayer->GetName());
	stmt->setString(5, pPlayer->BuildLogDB());
	stmt->setString(6, item ? item->BuildRegisterDB() : "");
	stmt->setString(7, comment);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void LOG_ITEM_LOOT(Player* pPlayer, Item * item, std::string const& comment)
{
	if (!sGameServer->IsItemLootLogDB())
	{
		return;
	}

	if (!pPlayer)
	{
		return;
	}

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_ITEM_LOOT);

	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, pPlayer->GetAccountData()->GetGUID());
	stmt->setString(2, pPlayer->GetAccountData()->GetAccount());
	stmt->setUInt32(3, pPlayer->GetGUID());
	stmt->setString(4, pPlayer->GetName());
	stmt->setString(5, pPlayer->BuildLogDB());
	stmt->setString(6, item ? item->BuildRegisterDB() : "");
	stmt->setString(7, comment);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void LOG_ITEM_SELL(Player* pPlayer, Item * item, std::string const& comment)
{
	if (!sGameServer->IsItemSellLogDB())
	{
		return;
	}

	if (!pPlayer)
	{
		return;
	}

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_ITEM_SELL);

	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, pPlayer->GetAccountData()->GetGUID());
	stmt->setString(2, pPlayer->GetAccountData()->GetAccount());
	stmt->setUInt32(3, pPlayer->GetGUID());
	stmt->setString(4, pPlayer->GetName());
	stmt->setString(5, pPlayer->BuildLogDB());
	stmt->setString(6, item ? item->BuildRegisterDB() : "");
	stmt->setString(7, comment);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void LOG_ITEM_BUY(Player* pPlayer, Item * item, std::string const& comment)
{
	if (!sGameServer->IsItemBuyLogDB())
	{
		return;
	}

	if (!pPlayer)
	{
		return;
	}

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_ITEM_BUY);

	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, pPlayer->GetAccountData()->GetGUID());
	stmt->setString(2, pPlayer->GetAccountData()->GetAccount());
	stmt->setUInt32(3, pPlayer->GetGUID());
	stmt->setString(4, pPlayer->GetName());
	stmt->setString(5, pPlayer->BuildLogDB());
	stmt->setString(6, item ? item->BuildRegisterDB() : "");
	stmt->setString(7, comment);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void LOG_JEWEL_DROP(Player* pPlayer, world_type world, coord_type x, coord_type y, Item const* item)
{
	if ( item->GetItem() == JEWEL::BLESS ||
		 item->GetItem() == JEWEL::SOUL ||
		 item->GetItem() == JEWEL::CHAOS ||
		 item->GetItem() == JEWEL::LIFE ||
		 item->GetItem() == JEWEL::CREATION ||
		 item->GetItem() == JEWEL::DEATH ||
		 item->GetItem() == JEWEL::GUARDIAN ||
		 item->GetItem() == JEWEL::HARMONY ||
		 item->GetItem() == JEWEL::GEMSTONE ||
		 item->GetItem() == ITEMGET(13, 14) ||
		 item->GetItem() == ITEMGET(13, 31) ||
		 item->GetItem() == ITEMGET(13, 52) )
	{
		SQLTransaction trans = MuLogDatabase.BeginTransaction();

		PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_JEWEL_DROP);

		stmt->setUInt32(0, pPlayer ? pPlayer->GetGUID(): 0);
		stmt->setUInt32(1, pPlayer ? pPlayer->GetAccountData()->GetGUID(): 0);
		stmt->setUInt8(2, item->GetItemType());
		stmt->setUInt16(3, item->GetItemIndex());
		stmt->setString(4, item->GetName());
		stmt->setUInt16(5, world);
		stmt->setInt16(6, x);
		stmt->setInt16(7, y);
		trans->Append(stmt);

		MuLogDatabase.CommitTransaction(trans);
	}
}

void GameServer::UpdateAccountsDisconnect()
{
	if ( (GetTickCount() - this->GetAccountsDisconnectTick()) > this->GetAccountDisconnectTime() )
	{
		this->SetAccountsDisconnectTick(GetTickCount());

		PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNTS_DISCONNECT_SELECT);
		stmt->setUInt16(0, this->GetServerCode());

		PreparedQueryResult result = LoginDatabase.Query(stmt);

		if ( result )
		{
			SQLTransaction trans = LoginDatabase.BeginTransaction();
			Player* pPlayer = nullptr;

			do
			{
				Field* fields = result->Fetch();

				uint32 account_id = fields[0].GetUInt32();
				uint8 masive = fields[1].GetUInt8();

				if ( !masive )
				{
					pPlayer = sObjectMgr->FindPlayerByAccount(account_id);

					if ( !pPlayer ) // El jugador no existe en el servidor, puedo proceder a desconectarlo
					{
						sAuthServer->PlayerLogout(account_id);
					}
				}
				else
				{
					sAuthServer->PlayerAccountKick(account_id);
				}
				
				stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNTS_DISCONNECT_DELETE);
				stmt->setUInt16(0, this->GetServerCode());
				stmt->setUInt32(1, account_id);
				trans->Append(stmt);
				
			}
			while(result->NextRow());

			LoginDatabase.CommitTransaction(trans);
		}
	}
}

void GameServer::PostManageUpdate()
{
	if ( !this->IsPostManageMACEnabled() ) ///- Si el sistema de MAC no está habilitado, entonces no sigo
	{
		return;
	}

	if ( !this->GetPostManageMACUpdate()->Elapsed(IN_MILLISECONDS) ) ///- Si no transcurrió 1 segundo desde el último update, entonces no sigo
	{
		return;
	}

	Custom::SystemTimer time = Custom::SystemTimer();

	if ( time.GetDay() != this->GetPostManageMACDay() ) ///- Si el día actual es diferente al útlimo día verificado, entonces actualizo el día y restauro la lista
	{
		this->SetPostManageMACDay(time.GetDay());
		this->post_manage_mac_map.clear();
	}
}

void GameServer::PostManageUpdate(Player* pPlayer)
{
	if ( !pPlayer ) ///- No debería pasar, pero reviso que el puntero no sea nulo
	{
		return;
	}

	if ( !this->IsPostManageEnabled() ) ///- Si el sistema no está habilitado, entonces no sigo
	{
		return;
	}

	Custom::SystemTimer time = Custom::SystemTimer();
	///- Si las fechas son diferentes, entonces actualizo las fechas del jugador, reseto el contador del jugador y le envío un mensaje para notificarle
	if ( time.GetDay() != pPlayer->GetPostDay() || time.GetMonth() != pPlayer->GetPostMonth() )
	{
		pPlayer->SetPostDay(time.GetDay());
		pPlayer->SetPostMonth(time.GetMonth());
		pPlayer->SetPostCount(0);

		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Se ha restaurado el contador de post.");
	}
}

bool GameServer::CanPost(Player* pPlayer, std::string const& message)
{
	if ( !pPlayer ) ///- No debería pasar, pero reviso que el puntero no sea nulo
	{
		return false;
	}

	if ( this->IsAntiSpamEnabled() ) ///- Si el sistema de antispam está habilitado, entonces proceso el texto
	{
		if ( message == pPlayer->GetAntiSpamMessage() ) ///- En caso de que sea el mismo mensaje, sumo 1
		{
			pPlayer->IncreaseCounter(PLAYER_COUNTER_ANTI_SPAM, 1);
		}
		else ///- En caso de que sea otro texto, inicializo la variable
		{
			pPlayer->SetAntiSpamMessage(message);
			pPlayer->SetCounter(PLAYER_COUNTER_ANTI_SPAM, 1);
			pPlayer->GetTimer(PLAYER_TIMER_ANTI_SPAM)->Start();
		}

		if ( pPlayer->GetCounter(PLAYER_COUNTER_ANTI_SPAM) > this->GetAntiSpamCount() )
		{
			if ( !pPlayer->GetTimer(PLAYER_TIMER_ANTI_SPAM)->Elapsed(this->GetAntiSpamTime()) )
			{
				pPlayer->ChatMute(this->GetAntiSpamMuteTime());
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "[ AntiSpam ] Muted for %s.", secsToTimeString(this->GetAntiSpamMuteTime()).c_str());
				return false;
			}
			else
			{
				pPlayer->SetAntiSpamMessage(message);
				pPlayer->SetCounter(PLAYER_COUNTER_ANTI_SPAM, 1);
			}
		}
	}

	if ( !this->IsPostManageEnabled() ) ///- Si el sistema no está habilitado, entonces le permito postear
	{
		return true;
	}

	if ( pPlayer->GetPostCount() >= this->GetPostManageCount() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Has superado el límite de post diarios.");
		return false;
	}

	int32 & post_mac_count = this->post_manage_mac_map[pPlayer->GetAccountData()->GetMac()];

	if ( post_mac_count >= this->GetPostManageMACCount() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Has superado el límite de post diarios.");
		return false;
	}

	pPlayer->IncreasePostCount(1);
	post_mac_count++;

	int32 diff = this->GetPostManageCount() - pPlayer->GetPostCount();
	int32 diff_mac = this->GetPostManageMACCount() - post_mac_count;

	if ( diff > 0 )
	{
		if ( ((diff % 10) == 0) || (diff < 10) )
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Post restantes: %d", diff);
		}
	}

	return true;
}

void GameServer::MACMute(std::string data, int32 operation, Player* pPlayer)
{
	if ( operation == 0 || operation == 1 ) // Char Name
	{
		STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), data.c_str(), data.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH: data.size());

		Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(name);

		if ( !pTarget )
		{
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
			return;
		}

		if ( operation == 0 ) // Agregar
		{
			MACMuteList::const_iterator it = this->mac_mute_list.find(pTarget->GetAccountData()->GetMac());

			if ( it != this->mac_mute_list.end() )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s already muted.", pTarget->GetAccountData()->GetMac());
				return;
			}

			this->mac_mute_list.insert(pTarget->GetAccountData()->GetMac());
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s muted.", pTarget->GetAccountData()->GetMac());
		}
		else // Borrar
		{
			MACMuteList::const_iterator it = this->mac_mute_list.find(pTarget->GetAccountData()->GetMac());

			if ( it == this->mac_mute_list.end() )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s is not muted.", pTarget->GetAccountData()->GetMac());
				return;
			}

			this->mac_mute_list.erase(it);
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s removed from mute list.", pTarget->GetAccountData()->GetMac());
		}
	}
	else if ( operation == 2 || operation == 3 ) // MAC
	{
		STRING_SAFE_COPY(mac, (MAX_ACCOUNT_MAC_LENGTH + 1), data.c_str(), data.size() > MAX_ACCOUNT_MAC_LENGTH ? MAX_ACCOUNT_MAC_LENGTH: data.size());

		if ( operation == 2 ) // Agregar
		{
			MACMuteList::const_iterator it = this->mac_mute_list.find(mac);

			if ( it != this->mac_mute_list.end() )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s already muted.", mac);
				return;
			}

			this->mac_mute_list.insert(mac);
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s muted.", mac);
		}
		else // Borrar
		{
			MACMuteList::const_iterator it = this->mac_mute_list.find(mac);

			if ( it == this->mac_mute_list.end() )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s is not muted.", mac);
				return;
			}

			this->mac_mute_list.erase(it);
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s removed from mute list.", mac);
		}
	}
	else if ( operation == 4 ) // List
	{
		if ( pPlayer )
		{
			for ( MACMuteList::const_iterator it = this->mac_mute_list.begin(); it != this->mac_mute_list.end(); ++it )
			{
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC: %s", (*it).c_str());
			}
		}
	}
}

bool GameServer::IsMACMuted(Player* pPlayer)
{
	MACMuteList::const_iterator it = this->mac_mute_list.find(pPlayer->GetAccountData()->GetMac());

	if ( it != this->mac_mute_list.end() )
	{
		return true;
	}

	return false;
}

void GameServer::ApplyMACRestriction(std::string const& data, int32 operation, uint32 restriction, uint32 time, Player* pPlayer)
{
	if ( restriction >= PlayerAction::PLAYER_ACTION_MAX )
	{
		PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong restriction ID: %u", restriction);
		return;
	}

	time = time * IN_MILLISECONDS;

	if ( operation == 0 || operation == 1 ) // Char Name
	{
		STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), data.c_str(), data.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH: data.size());

		Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(name);

		if ( !pTarget )
		{
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
			return;
		}

		MACRestrictionData & restriction_data = this->mac_restriction_map[pTarget->GetAccountData()->GetMac()];

		if ( operation == 0 ) // Agregar
		{
			if ( restriction_data.IsApplied(restriction) )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s already have %s restriction.", 
					pTarget->GetAccountData()->GetMac(), PlayerAction::Name[restriction].c_str());

				return;
			}

			restriction_data.SetApplied(restriction, true);
			restriction_data.SetTime(restriction, time == 0 ? 0: (GetTickCount() + time));

			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s restricted for %s.", 
				pTarget->GetAccountData()->GetMac(), PlayerAction::Name[restriction].c_str());
		}
		else // Borrar
		{
			if ( !restriction_data.IsApplied(restriction) )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s is not restricted for %s.", 
					pTarget->GetAccountData()->GetMac(), PlayerAction::Name[restriction].c_str());

				return;
			}

			restriction_data.SetApplied(restriction, false);
			restriction_data.SetTime(restriction, 0);
			
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s removed from %s restriction.", 
				pTarget->GetAccountData()->GetMac(), PlayerAction::Name[restriction].c_str());
		}
	}
	else if ( operation == 2 || operation == 3 ) // MAC
	{
		STRING_SAFE_COPY(mac, (MAX_ACCOUNT_MAC_LENGTH + 1), data.c_str(), data.size() > MAX_ACCOUNT_MAC_LENGTH ? MAX_ACCOUNT_MAC_LENGTH: data.size());

		MACRestrictionData & restriction_data = this->mac_restriction_map[mac];

		if ( operation == 2 ) // Agregar
		{
			if ( restriction_data.IsApplied(restriction) )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s already have %s restriction.", 
					mac, PlayerAction::Name[restriction].c_str());

				return;
			}

			restriction_data.SetApplied(restriction, true);
			restriction_data.SetTime(restriction, time == 0 ? 0: (GetTickCount() + time));

			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s restricted for %s.", 
				mac, PlayerAction::Name[restriction].c_str());
		}
		else // Borrar
		{
			if ( !restriction_data.IsApplied(restriction) )
			{
				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "MAC %s is not restricted for %s.", 
					mac, PlayerAction::Name[restriction].c_str());

				return;
			}

			restriction_data.SetApplied(restriction, false);
			restriction_data.SetTime(restriction, 0);
			
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC %s removed from %s restriction.", 
				mac, PlayerAction::Name[restriction].c_str());
		}
	}
	else if ( operation == 4 ) // List
	{
		if ( pPlayer )
		{
			for ( MACRestrictionDataMap::const_iterator it = this->mac_restriction_map.begin(); it != this->mac_restriction_map.end(); ++it )
			{
				for ( int32 i = 0; i < PlayerAction::PLAYER_ACTION_MAX; ++i )
				{
					if ( !it->second.IsApplied(i) )
						continue;

					pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MAC: %s restricted for %s.", 
						it->first.c_str(), PlayerAction::Name[i].c_str());
				}
			}
		}
	}
}
	
bool GameServer::IsMACRestriction(uint32 restriction, Player* pPlayer)
{
	MACRestrictionDataMap::const_iterator it = this->mac_restriction_map.find(pPlayer->GetAccountData()->GetMac());

	if ( it == this->mac_restriction_map.end() )
	{
		return false;
	}

	if ( !it->second.IsApplied(restriction) )
	{
		return false;
	}

	pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You have %s restriction.", PlayerAction::Name[restriction].c_str());
	return true;
}

void GameServer::UpdateMACRestriction()
{
	for ( MACRestrictionDataMap::iterator it = this->mac_restriction_map.begin(); it != this->mac_restriction_map.end(); ++it )
	{
		for ( int32 i = 0; i < PlayerAction::PLAYER_ACTION_MAX; ++i )
		{
			if ( !it->second.IsApplied(i) )
				continue;

			if ( it->second.TimePassed(i) )
			{
				it->second.SetApplied(i, false);
				it->second.SetTime(i, 0);
			}
		}
	}
}

void GameServer::LoadNotification()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Notifications...");

	LIST_CLEAR(NotificationList::iterator, this->notification_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM notifications WHERE server_code = %u", this->GetServerCode());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 1;

			notification * add_data = new notification;
			add_data->SetText(fields[loop++].GetString().c_str());

			this->notification_list.push_back(add_data);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u notifications definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void GameServer::SendNotification()
{
	PlayerSessionMap const& player_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		this->SendNotification(pPlayer);
	}
}
	
void GameServer::SendNotification(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( NotificationList::const_iterator it = this->notification_list.begin(); it != this->notification_list.end(); ++it )
	{
		pPlayer->SendNoticeNormal(CUSTOM_MESSAGE_ID_BLUE, (*it)->GetText());
	}
}

void GameServer::LoadOffsetData()
{
	sLog->outLoad(true, "Loading Offset Data...");

	MAP_CLEAR(OffsetDataMap::iterator, this->offset_data_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM offset_data i WHERE i.serial = '%s'", this->GetGameServerSerial());

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			OffsetData* add_data = new OffsetData;
			add_data->SetOffset(reader.GetUInt32());
			add_data->SetValue(reader.GetUInt8());
			add_data->SetFix(reader.GetUInt8());

			this->offset_data_map[add_data->GetOffset()] = add_data;
			count++;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u offset data definitions", count);
}
	
void GameServer::SendOffsetData(Player* pPlayer)
{
	uint8 buffer[8192];
	POINTER_PCT(HACK_CHECK_ADD_OFFSET_HEAD, head, buffer, 0);
	POINTER_PCT(HACK_CHECK_ADD_OFFSET_BODY, body, buffer, sizeof(HACK_CHECK_ADD_OFFSET_HEAD));
	head->count = 0;

	for (auto const& itr : this->offset_data_map)
	{
		const auto pData = itr.second;

		if (!pData)
		{
			continue;
		}

		body[head->count].offset = 0xFFFFFFFF - pData->GetOffset();
		body[head->count].value = 0xFF - pData->GetValue();
		body[head->count].fix = pData->GetFix();
		++head->count;
	}

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_HACK_CHECK_ADD_OFFSET, sizeof(HACK_CHECK_ADD_OFFSET_HEAD)+(head->count * sizeof(HACK_CHECK_ADD_OFFSET_BODY)));

	if (pPlayer)
	{
		pPlayer->sendPacket(buffer, head->h.get_size());
	}
	else
	{
		sObjectMgr->sendPacket(buffer, head->h.get_size());
	}
}

void GameServer::LoadOffsetFPS()
{
	sLog->outLoad(true, "Loading Offset FPS...");

	MAP_CLEAR(OffsetFPSMap::iterator, this->offset_fps_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM offset_fps i WHERE i.serial = '%s'", this->GetGameServerSerial());

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			OffsetFPS* add_data = new OffsetFPS;
			add_data->SetOffset(reader.GetUInt32());
			add_data->SetMod(reader.GetUInt8());
			add_data->SetOriginal(reader.GetUInt8());

			this->offset_fps_map[add_data->GetOffset()] = add_data;
			count++;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u offset fps definitions", count);
}

void GameServer::LoadCheatScanWhiteList()
{
	sLog->outLoad(true, "Loading Cheat Scan White List...");

	this->cheat_scan_white_list.clear();
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM cheat_scan_whitelist");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			this->cheat_scan_white_list.insert(reader.GetString());
			count++;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u cheat scan white list definitions", count);
}

bool GameServer::IsCheatScanWhiteList(std::string const& process_name) const
{
	CheatScanWhiteList::const_iterator it = this->cheat_scan_white_list.find(process_name);

	if (it != this->cheat_scan_white_list.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameServer::LoadOfflineAttackWorld()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Offline Attack World...");

	MAP_CLEAR(OfflineAttackWorldMap::iterator, this->offline_attack_world_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT i.world_id, i.level_min, i.level_max FROM offline_attack_world i WHERE i.server_id = '%u'", this->GetServerCode());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			OfflineAttackWorld * pData = new OfflineAttackWorld;
			pData->SetWorld(fields[0].GetUInt16());
			pData->SetLevelMin(fields[1].GetInt16());
			pData->SetLevelMax(fields[2].GetInt16());
						
			this->offline_attack_world_map[pData->GetWorld()] = pData;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u offline attack world definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void GameServer::LoadOfflineAttackZone()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Offline Attack Zone...");

	LIST_CLEAR(OfflineAttackZoneList::iterator, this->m_OfflineAttackZoneList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM offline_attack_zone");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			OfflineAttackZone * pData = new OfflineAttackZone;
			pData->SetWorld(fields[0].GetUInt16());
			pData->SetX1(fields[1].GetInt16());
			pData->SetY1(fields[2].GetInt16());
			pData->SetX2(fields[3].GetInt16());
			pData->SetY2(fields[4].GetInt16());
			pData->SetEnabled(fields[5].GetBool());

			this->m_OfflineAttackZoneList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u offline attack zone definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void GameServer::SendOffsetFPS(Player* pPlayer)
{
	uint8 buffer[8192];
	POINTER_PCT(HACK_CHECK_ADD_OFFSET_HEAD, head, buffer, 0);
	POINTER_PCT(HACK_CHECK_ADD_OFFSET_BODY, body, buffer, sizeof(HACK_CHECK_ADD_OFFSET_HEAD));
	head->count = 0;

	for ( OffsetFPSMap::const_iterator it = this->offset_fps_map.begin(); it != this->offset_fps_map.end(); ++it )
	{
		if ( !it->second )
		{
			continue;
		}

		body[head->count].offset = 0xFFFFFFFF - it->second->GetOffset();
		body[head->count].value = 0xFF - it->second->GetMod();
		body[head->count].fix = 0xFF - it->second->GetOriginal();
		++head->count;
	}

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_FPS_OFFSET, sizeof(HACK_CHECK_ADD_OFFSET_HEAD) + (head->count * sizeof(HACK_CHECK_ADD_OFFSET_BODY)));

	if ( pPlayer )
	{
		pPlayer->sendPacket(buffer, head->h.get_size());
	}
	else
	{
		sObjectMgr->sendPacket(buffer, head->h.get_size());
	}
}

bool GameServer::SerialCheck(Player* pPlayer, uint16 server, uint32 serial, std::string const& item_name, uint8 type)
{
	if (!pPlayer)
	{
		return false;
	}

	if (!sGameServer->IsAntiDupeBoxEnabled())
	{
		return true;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_SERIAL_CHECK_INSERT);
	stmt->setUInt16(server);
	stmt->setUInt32(serial);
	stmt->setUInt8(type);
	stmt->setUInt32(pPlayer->GetAccountData()->GetGUID());
	stmt->setString(pPlayer->GetAccountData()->GetIP());
	stmt->setString(pPlayer->GetAccountData()->GetMac());
	stmt->setUInt32(pPlayer->GetAccountData()->GetDiskSerial());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	return true;
}

void GameServer::LogAntiHack(Player* pPlayer, uint8 type, const char* msg, ...)
{
	if (!pPlayer)
	{
		return;
	}

	ARG(buffer, msg);

	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_ANTIHACK);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt32(pPlayer->GetAccountData()->GetGUID());
	stmt->setString(pPlayer->GetAccountData()->GetAccount());
	stmt->setUInt32(pPlayer->GetGUID());
	stmt->setString(pPlayer->GetName());
	stmt->setString(pPlayer->BuildLogDB());
	stmt->setString(pPlayer->BuildLocationLog(-1, -1, true));
	stmt->setUInt8(type);
	stmt->setString(buffer);
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void GameServer::ProcessTrack(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	SCAN_PROCESS_REQUEST pMsg;
	pPlayer->SEND_PCT(pMsg);
}
	
void GameServer::ProcessTrack(Player* pPlayer, uint8 * Packet)
{
	if ( !pPlayer )
	{
		return;
	}

	POINTER_PCT(SCAN_PROCESS_HEAD, head, Packet, 0);
	POINTER_PCT(SCAN_PROCESS_BODY, body, Packet, sizeof(SCAN_PROCESS_HEAD));

	for ( int32 i = 0; i < head->count; ++i )
	{
		STRING_SAFE_COPY(proc_name, 51, body[i].process_name, 50);

		sLog->outInfo("antihack", "%s -- %s -- %s", __FUNCTION__, pPlayer->BuildLog().c_str(), proc_name);
	}
}

bool GameServer::IsOfflineAttackWorld(Player* pPlayer, world_type world_id) const
{
	if (!pPlayer)
	{
		return false;
	}

	OfflineAttackWorldMap::const_iterator itr = this->offline_attack_world_map.find(world_id);

	if (itr != this->offline_attack_world_map.end())
	{
		OfflineAttackWorld const* pData = itr->second;

		if (pData)
		{
			if ((pPlayer->GetTotalLevel() >= pData->GetLevelMin()) && (pPlayer->GetTotalLevel() <= pData->GetLevelMax()))
			{
				return true;
			}
			else
			{
				pPlayer->SendMessageBox(1, "Offline Attack", "ERROR \n Can't use offline attack on this map. \n Level Range for this map is [%d / %d]", pData->GetLevelMin(), pData->GetLevelMax());
				return false;
			}
		}
	}

	return true;
}

bool GameServer::IsOfflineAttackZone(Player* pPlayer, world_type world_id, coord_type x, coord_type y) const
{
	if (!pPlayer)
	{
		return false;
	}

	bool founded = false;

	for (OfflineAttackZoneList::const_iterator itr = this->m_OfflineAttackZoneList.begin(); itr != this->m_OfflineAttackZoneList.end(); ++itr)
	{
		OfflineAttackZone const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetWorld() != world_id)
		{
			continue;
		}

		if (pData->IsEnabled())
		{
			founded = true;
		}

		if ((x >= pData->GetX1() && x <= pData->GetX2()) &&
			(y >= pData->GetY1() && y <= pData->GetY2()))
		{
			return pData->IsEnabled();
		}
	}

	return !founded;
}

bool GameServer::IsDiscountWC() const
{
	if (this->GetCashShopDiscountWC() <= 0)
	{
		return false;
	}

	return time(nullptr) < this->GetCashShopDiscountDate();
}

bool GameServer::IsDiscountGP() const
{
	if (this->GetCashShopDiscountGP() <= 0)
	{
		return false;
	}

	return time(nullptr) < this->GetCashShopDiscountDate();
}

void GameServer::LogDamagePVP(Player* pPlayer01, Player* pPlayer02, int32 hp_damage, int32 sd_damage, uint16 skill)
{
	if (!sGameServer->IsLogDamagePVP())
	{
		return;
	}

	if (!pPlayer01 || !pPlayer02)
	{
		return;
	}

	if (pPlayer01->GetWorldId() != WORLD_VULCANUS_ROOM)
	{
		return;
	}

	if (pPlayer01->GetTotalLevel() != sGameServer->GetDuelTestLevel() || pPlayer02->GetTotalLevel() != sGameServer->GetDuelTestLevel())
	{
		return;
	}

	sLog->outInfo("damage_pvp", "[%s] hit [%s] - HP: %d - SD: %d - Skill: %u", pPlayer01->BuildLog().c_str(), pPlayer02->BuildLog().c_str(), hp_damage, sd_damage, skill);

	if (sGameServer->IsLogDamagePVPToDB())
	{
		PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(LOG_STATEMENT_PVP);
		stmt->setUInt32(0, pPlayer01->GetGUID());
		stmt->setString(1, pPlayer01->GetName());
		stmt->setUInt8(2, pPlayer01->GetClass());
		stmt->setString(3, pPlayer01->BuildLogStat());
		stmt->setString(4, pPlayer01->BuildLogSet());
		stmt->setUInt32(5, pPlayer02->GetGUID());
		stmt->setString(6, pPlayer02->GetName());
		stmt->setUInt8(7, pPlayer02->GetClass());
		stmt->setString(8, pPlayer02->BuildLogStat());
		stmt->setString(9, pPlayer02->BuildLogSet());
		stmt->setUInt16(10, skill);
		stmt->setInt32(11, hp_damage);
		stmt->setInt32(12, sd_damage);
		MuLogDatabase.Execute(stmt);
	}
}