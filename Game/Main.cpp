/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Main.cpp"
*
*/

bool StartNetwork();
Config * sConfig = new Config;
Config * sConfigCommon = new Config;

boost::asio::io_service _ioService;
boost::asio::signal_set signals(_ioService, SIGINT, SIGTERM);

MainApplication::MainApplication(): AppConsole("GameServer", "GAME SERVER", MAIN_DB_FLAG_CONFIG | MAIN_DB_FLAG_MU_LOGIN, sConfigCommon)
{
	sGameServer->SetServerType(sConfig->GetIntDefault("GameServer.ServerType", SERVER_TYPE_NORMAL));
	sGameServer->SetServerCode(sConfig->GetIntDefault("GameServer.Code", 0));

	this->AddDBFlag(MAIN_DB_FLAG_MU);
	this->AddDBFlag(MAIN_DB_FLAG_MU_LOG);
}

MainApplication::~MainApplication()
{

}

int main(int argc, char *argv[])
{
	sLog->LoadFolder();

	sLog->LoadAppender("Console", APPENDER_CONSOLE, LOG_LEVEL_TRACE, APPENDER_FLAGS_NONE, "", "", "w");
	sLog->LoadAppender("GameServer", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\GameServer", "GameServer.log", "w");
	sLog->LoadAppender("Trade", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Trade", "Trade.log", "w");
	sLog->LoadAppender("CashShop", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "CashShop", "CashShop.log", "w");
	sLog->LoadAppender("Network", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\Network", "Network.log", "w");
	sLog->LoadAppender("ChaosMix", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "ChaosMix", "ChaosMix.log", "w");
	sLog->LoadAppender("Quest", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Quest", "Quest.log", "w");
	sLog->LoadAppender("BloodCastle", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\BloodCastle", "BloodCastle.log", "w");
	sLog->LoadAppender("DevilSquare", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\DevilSquare", "DevilSquare.log", "w");
	sLog->LoadAppender("ChaosCastle", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ChaosCastle", "ChaosCastle.log", "w");
	sLog->LoadAppender("Crywolf", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Crywolf", "Crywolf.log", "w");
	sLog->LoadAppender("Party", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Party", "Party.log", "w");
	sLog->LoadAppender("Invasion", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Invasion", "Invasion.log", "w");
	sLog->LoadAppender("Kanturu", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Kanturu", "Kanturu.log", "w");
	sLog->LoadAppender("CastleSiege", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\CastleSiege", "CastleSiege.log", "w");
	sLog->LoadAppender("Database", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\Database", "Database.log", "w");
	sLog->LoadAppender("Raklion", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Raklion", "Raklion.log", "w");
	sLog->LoadAppender("PersonalShop", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "PersonalShop", "PersonalShop.log", "w");
	sLog->LoadAppender("Doppelganger", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Doppelganger", "Doppelganger.log", "w");
	sLog->LoadAppender("Guild", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Guild", "Guild.log", "w");
	sLog->LoadAppender("Items", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Items", "Items.log", "w");
	sLog->LoadAppender("Gens", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Gens", "Gens.log", "w");
	sLog->LoadAppender("Skill", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Skill", "Skill.log", "w");
	sLog->LoadAppender("Chat", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Chat", "Chat.log", "w");
	sLog->LoadAppender("Player", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Player", "Player.log", "w");
	sLog->LoadAppender("Monster", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Monster", "Monster.log", "w");
	sLog->LoadAppender("Crash", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\Crash", "Crash.log", "w");
	sLog->LoadAppender("Scramble", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Scramble", "Scramble.log", "w");
	sLog->LoadAppender("Race", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Race", "Race.log", "w");
	sLog->LoadAppender("IllusionTemple", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\IllusionTemple", "IllusionTemple.log", "w");
	sLog->LoadAppender("Jewel", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Jewel", "Jewel.log", "w");
	sLog->LoadAppender("ImperialFortress", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ImperialFortress", "ImperialFortress.log", "w");
	sLog->LoadAppender("AntiHack", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "AntiHack", "AntiHack.log", "w");
	sLog->LoadAppender("ProtectorOfAcheron", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ProtectorOfAcheron", "ProtectorOfAcheron.log", "w");
	sLog->LoadAppender("TormentedSquare", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\TormentedSquare", "TormentedSquare.log", "w");
	sLog->LoadAppender("ArkaWar", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ArkaWar", "ArkaWar.log", "w");
	sLog->LoadAppender("NixiesLake", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\NixiesLake", "NixiesLake.log", "w");
	sLog->LoadAppender("LabyrinthDimension", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\LabyrinthDimension", "LabyrinthDimension.log", "w");
	sLog->LoadAppender("WorldBoss", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\WorldBoss", "WorldBoss.log", "w");
	sLog->LoadAppender("MuRoomy", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\MuRoomy", "MuRoomy.log", "w");
	sLog->LoadAppender("MiniBomb", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\MiniBomb", "MiniBomb.log", "w");
	sLog->LoadAppender("JewelBingo", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\JewelBingo", "JewelBingo.log", "w");
	sLog->LoadAppender("AutoLogin", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "AutoLogin", "AutoLogin.log", "w");
	sLog->LoadAppender("MossMerchant", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\MossMerchant", "MossMerchant.log", "w");
	sLog->LoadAppender("SwampOfDarkness", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\SwampOfDarkness", "SwampOfDarkness.log", "w");
	sLog->LoadAppender("NumericBaseball", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\NumericBaseball", "NumericBaseball.log", "w");
	sLog->LoadAppender("LastManStanding", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\LastManStanding", "LastManStanding.log", "w");
	sLog->LoadAppender("InstancedDungeon", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\InstancedDungeon", "InstancedDungeon.log", "w");
	sLog->LoadAppender("QuestMU", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "QuestMU", "QuestMU.log", "w");
	sLog->LoadAppender("DamagePvP", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "DamagePvP", "DamagePvP.log", "w");

	//sLog->LoadAppender("ConfigTemp", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(16), "ConfigTemp", "ConfigTemp.log", "w");

	sLog->LoadLogger("root", LOG_LEVEL_TRACE, " Console GameServer");
	sLog->LoadLogger("trade", LOG_LEVEL_TRACE, "Trade");
	sLog->LoadLogger("cashshop", LOG_LEVEL_TRACE, "CashShop");
	sLog->LoadLogger("network", LOG_LEVEL_TRACE, "Network");
	sLog->LoadLogger("chaosmix", LOG_LEVEL_TRACE, "ChaosMix");
	sLog->LoadLogger("quest", LOG_LEVEL_TRACE, "Quest");
	sLog->LoadLogger("bloodcastle", LOG_LEVEL_TRACE, "BloodCastle");
	sLog->LoadLogger("devilsquare", LOG_LEVEL_TRACE, "DevilSquare");
	sLog->LoadLogger("chaoscastle", LOG_LEVEL_TRACE, "ChaosCastle");
	sLog->LoadLogger("crywolf", LOG_LEVEL_TRACE, "Crywolf");
	sLog->LoadLogger("party", LOG_LEVEL_TRACE, "Party");
	sLog->LoadLogger("invasion", LOG_LEVEL_TRACE, "Invasion");
	sLog->LoadLogger("kanturu", LOG_LEVEL_TRACE, "Kanturu");
	sLog->LoadLogger("castlesiege", LOG_LEVEL_TRACE, "CastleSiege");
	sLog->LoadLogger("database", LOG_LEVEL_TRACE, "Database");
	sLog->LoadLogger("raklion", LOG_LEVEL_TRACE, "Raklion");
	sLog->LoadLogger("personalshop", LOG_LEVEL_TRACE, "PersonalShop");
	sLog->LoadLogger("doppelganger", LOG_LEVEL_TRACE, "Doppelganger");
	sLog->LoadLogger("guild", LOG_LEVEL_TRACE, "Guild");
	sLog->LoadLogger("items", LOG_LEVEL_TRACE, "Items");
	sLog->LoadLogger("gens", LOG_LEVEL_TRACE, "Gens");
	sLog->LoadLogger("skill", LOG_LEVEL_TRACE, "Skill");
	sLog->LoadLogger("chat", LOG_LEVEL_TRACE, "Chat");
	sLog->LoadLogger("player", LOG_LEVEL_TRACE, "Player");
	sLog->LoadLogger("monster", LOG_LEVEL_TRACE, "Monster");
	sLog->LoadLogger("crash", LOG_LEVEL_TRACE, "Crash");
	sLog->LoadLogger("scramble", LOG_LEVEL_TRACE, "Scramble");
	sLog->LoadLogger("race", LOG_LEVEL_TRACE, "Race");
	sLog->LoadLogger("illusiontemple", LOG_LEVEL_TRACE, "IllusionTemple");
	sLog->LoadLogger("jewel", LOG_LEVEL_TRACE, "Jewel");
	sLog->LoadLogger("imperial_fortress", LOG_LEVEL_TRACE, "ImperialFortress");
	sLog->LoadLogger("antihack", LOG_LEVEL_TRACE, "AntiHack");
	sLog->LoadLogger("protectorofacheron", LOG_LEVEL_TRACE, "ProtectorOfAcheron");
	sLog->LoadLogger("tormentedsquare", LOG_LEVEL_TRACE, "TormentedSquare");
	sLog->LoadLogger("arkawar", LOG_LEVEL_TRACE, "ArkaWar");
	sLog->LoadLogger("nixieslake", LOG_LEVEL_TRACE, "NixiesLake");
	sLog->LoadLogger("labyrinthdimension", LOG_LEVEL_TRACE, "LabyrinthDimension");
	sLog->LoadLogger("world_boss", LOG_LEVEL_TRACE, "WorldBoss");
	sLog->LoadLogger("mu_roomy", LOG_LEVEL_TRACE, "MuRoomy");
	sLog->LoadLogger("mini_bomb", LOG_LEVEL_TRACE, "MiniBomb");
	sLog->LoadLogger("jewel_bingo", LOG_LEVEL_TRACE, "JewelBingo");
	sLog->LoadLogger("auto_login", LOG_LEVEL_TRACE, "AutoLogin");
	sLog->LoadLogger("moss_merchant", LOG_LEVEL_TRACE, "MossMerchant");
	sLog->LoadLogger("swamp_of_darkness", LOG_LEVEL_TRACE, "SwampOfDarkness");
	sLog->LoadLogger("numeric_baseball", LOG_LEVEL_TRACE, "NumericBaseball");
	sLog->LoadLogger("last_man_standing", LOG_LEVEL_TRACE, "LastManStanding");
	sLog->LoadLogger("instanced_dungeon", LOG_LEVEL_TRACE, "InstancedDungeon");
	sLog->LoadLogger("quest_mu", LOG_LEVEL_TRACE, "QuestMU");
	sLog->LoadLogger("damage_pvp", LOG_LEVEL_TRACE, "DamagePvP");

	//sLog->LoadLogger("config_temp", LOG_LEVEL_TRACE, "ConfigTemp");

	//StartStackLogging();
	if ( !InitConfig("game_common.conf") )
	{
		system("PAUSE");
		return 1;
	}

	if ( !sMain->Start() )
		return 0;

	sMain->UpdateLoop();

	sMain->End();

	return sMain->GetExitCode();
}

void MainApplication::End()
{
	sObjectMgr->KickAll();

	sWorldSocketMgr.StopNetwork();
	sServerLink->Stop();
	sAuthServer->Stop();
	sConnectServer->Stop();
	
	AppConsole::End();
}

void MainApplication::Load()
{
	this->Init();

	QueryResult result1 = GameServerDatabase.Query("SELECT `key`, `value` FROM settings WHERE server_id = -1");

	if ( result1 )
	{
		do
		{
			Field* fields = result1->Fetch();

			this->Add(fields[0].GetString(), fields[1].GetString());
		}
		while(result1->NextRow());
	}

	QueryResult result2 = GameServerDatabase.PQuery("SELECT `key`, `value` FROM settings WHERE server_id = '%u'", sGameServer->GetServerCode());

	if ( result2 )
	{
		do
		{
			Field* fields = result2->Fetch();

			this->Add(fields[0].GetString(), fields[1].GetString());
		}
		while(result2->NextRow());
	}
}

void MainApplication::CreateSetting(std::string const& key, std::string const& default)
{
	if ( key.empty() )
	{
		return;
	}

	GameServerDatabase.PExecute("INSERT IGNORE INTO settings (server_id, `key`, `value`) VALUES ('%d', '%s', '%s')", 
		-1, key.c_str(), default.c_str());
}

bool MainApplication::Run()
{
	if ( !MixHandler::DuplicatedMixCheck() )
	{
		sLog->outError("root", "Duplicated MixID");
		return false;
	}

	

	sGameServer->LoadCommonSettings();
	sGameServer->LoadCashShopSettings();

	sMessage->Init();

	sFormulaMgr->Load();

	sWorldMgr->LoadWorldList();
	sWorldMgr->LoadWorldAIPath();
	sWorldMgr->LoadWorldBuff();
	sWorldMgr->LoadWorldAreaRestriction();
	sWorldMgr->LoadWorldServer();
	sWorldMgr->LoadWorldAttribute();
	sWorldMgr->LoadWorldExperience();
	sWorldMgr->LoadWorldAreaAttribute();
	sWorldMgr->LoadWorldFreePK();

	sObjectMgr->Init();

	sItemMgr->LoadItemTemplate();
	sItemMgr->LoadItemAddOption();
	sItemMgr->LoadItemSetType();
	sItemMgr->LoadItemSetOption();
	sItemMgr->LoadItemHarmony();
	sItemMgr->LoadItemHarmonyEnhancement();
	sItemMgr->LoadItemSocket();
	sItemMgr->LoadItemSocketSeed();
	sItemMgr->LoadItemSocketBonus();
	sItemMgr->LoadItemSocketPackage();
	sItemMgr->LoadItemSocketMix();
	sItemMgr->LoadItemSocketUpgrade();
	sItemMgr->LoadItemSocketSeedSphere();
	sItemMgr->LoadItemTransformation();
	sItemMgr->LoadItemDrop();
	sItemMgr->LoadItemLevelTemplate();
	sItemMgr->LoadItemMixLucky();
	sItemMgr->LoadItemPriceData();
	sItemMgr->LoadItemStack();
	sItemMgr->LoadItemQuest();
	sItemMgr->LoadItemPriceRuud();
	sItemMgr->LoadItemWingValues();
	sItemMgr->LoadItemReducedDrop();
	sItemMgr->LoadItemCreateData();
	sItemMgr->LoadItemFlagDB();
	sItemMgr->LoadItemWingGradedOption();
	sItemMgr->LoadItemWingSocketOption();
	sItemMgr->LoadItemWingSocketUpgrade();
	sItemMgr->LoadEarring();
	sItemMgr->LoadEarringOption();
	sItemMgr->LoadEarringOptionValue();
	sItemMgr->LoadEarringBonus();
	sItemMgr->LoadEarringUpgrade();
	sItemMgr->LoadMasteryBonus();
	sItemMgr->LoadMysteriousStone();
	sItemMgr->LoadGuardianOption();
	sItemMgr->LoadGuardianOptionValue();
	sItemMgr->LoadGuardianEliteOption();
	sItemMgr->LoadGuardianEliteOptionValue();
	sItemMgr->SetLastItemUpdate(time(nullptr));
	
	sSkillMgr->LoadSkillTemplate();
	sSkillMgr->LoadSkillLearn();
	sSkillMgr->LoadSkillTree();
	sSkillMgr->LoadSkillSpecialElement();
	sSkillMgr->LoadSkillSpecial();
	sSkillMgr->LoadBuffTemplate();
	sSkillMgr->LoadSkillRequire();
	sSkillMgr->LoadSkillTreeMajestic();
	sSkillMgr->LoadSkillTreeMajesticStat();
	sSkillMgr->LoadSkillAttackTime();

	sCharacterBase->LoadCharacterBase();
	sCharacterBase->LoadCharacterBaseSkill();
	sCharacterBase->LoadCharacterExperienceNormal();
	sCharacterBase->LoadCharacterExperienceMaster();
	sCharacterBase->LoadCharacterExperienceMajestic();
	sCharacterBase->LoadCharacterHelper();
	sCharacterBase->LoadCharacterFormula();
	sCharacterBase->LoadCharacterStatFruit();
	sCharacterBase->LoadCharacterItem();
	sCharacterBase->LoadCharacterSpecialization();
	sCharacterBase->LoadCharacterExperienceAdjust();
	sCharacterBase->LoadCharacterExperienceBonus();
	sCharacterBase->LoadCharacterBonus();
	sCharacterBase->LoadCharacterMaxLevelReward();
	sCharacterBase->LoadCharacterExperienceTable();

	sJewelMix->Load();

	sScriptAI->LoadScriptAI();

	sMonsterManager->LoadMonsterTemplate();
	sMonsterManager->LoadMonsterItems();
	sMonsterManager->LoadMonsterSkill();
	sMonsterManager->LoadMonsterSkillSpecial();
	sMonsterManager->LoadMonsterAIElement();
	sMonsterManager->LoadMonsterAIAutomata();
	sMonsterManager->LoadMonsterAIUnit();
	sMonsterManager->LoadMonsterAIGroup();
	sMonsterManager->LoadMonsterRespawnLocation();
	sMonsterManager->LoadMonsterEquipment();
	sMonsterManager->LoadMonsterExclusive();
	sMonsterManager->LoadMonster();
	sMonsterManager->LoadMonsterEvent();
	sMonsterManager->SetLastUpdate(time(nullptr));

	sTeleport->LoadGateTemplate();
	sTeleport->LoadTeleportTemplate();
	sTeleport->LoadMoveLevel();
	
	sVipMgr->LoadVipTemplate();

	sQuestMgr->LoadQuestEvolution();
	sQuestMgr->LoadQuestEvolutionData();
	sQuestMgr->LoadQuestEvolutionCondition();
	sQuestMgr->LoadQuestEvolutionItemReward();

	sQuestMgr->LoadQuestGuided();
	sQuestMgr->LoadQuestMU();

	sItemBagMgr->LoadItemBagTemplate();
	sItemBagMgr->LoadItemBagItems();
	sGenMgr->LoadGenReward();
	sGenMgr->LoadGenRanking();
	sGenMgr->LoadGenKillPoints();
	sEventMgr->LoadEventData();
	sEventMgr->LoadEventSeasonData();
	sEventMgr->LoadEventContributionReward();
	sEventMgr->LoadEventLevel();
	sEventMgr->LoadEventEnterCount();
	sBloodCastleMgr->LoadBloodCastleSetting();
	sDevilSquareMgr->LoadDevilSquareSetting();
	sDevilSquareMgr->LoadDevilSquareRanking();
	sChaosCastleMgr->LoadChaosCastleSetting();
	sChaosCastleSurvivalMgr->LoadChaosCastleSetting();
	sIllusionTempleMgr->LoadIllusionTempleSettings();
	sTormentedSquareSurvival->Load();
	sRaklion->LoadData();
	sKanturuMgr->Load();
	sImperialFortressMgr->LoadImperialFortressLevel();
	sImperialFortressMgr->LoadImperialFortressMonsterSettings();
	sImperialFortressMgr->LoadImperialFortressTraps();
	sDoppelganger->LoadLevel();
	sDoppelganger->LoadMonster();
	sInvasionMgr->LoadInvasionData();
	sInvasionMgr->LoadInvasionGroup();
	sCastleSiege->LoadData();
	sCrywolf->LoadData();
	sDungeonRace->LoadData();
	sLosttowerRace->LoadData();
	sCastleDeep->Load();
	sCashShopMgr->LoadItemList();
	sGameServer->LoadFilter();
	sGameServer->LoadNotice();
	sGameServer->LoadGoblinPoint();
	sGameServer->LoadNonPKTime();
	sGameServer->LoadNotification();
	sGameServer->LoadOffsetData();
	sGameServer->LoadOffsetFPS();
	sGameServer->LoadCheatScanWhiteList();
	sGameServer->LoadOfflineAttackWorld();
	sGameServer->LoadOfflineAttackZone();
	
	sScramble->LoadWordList();
	sMixMgr->LoadDisabled();
	sMixMgr->LoadSpellStone();
	sMixMgr->LoadGuardian();

	sPentagramSystem->LoadType();
	sPentagramSystem->LoadOption();
	sPentagramSystem->LoadJewelOption();
	sPentagramSystem->LoadJewelRemove();
	sPentagramSystem->LoadJewelUpgradeRank();
	sPentagramSystem->LoadJewelUpgradeLevel();

	sMuunSystem->Load();
	sMuunSystem->LoadOption();
	sMuunSystem->LoadEnergy();
	sMuunSystem->LoadExchange();

	sMiniMap->Load();

	sSummonScroll->Load();
	sSummonScroll->LoadMonster();
	sMiningSystem->Load();
	sProtectorOfAcheron->Load();
	sTormentedSquare->Load();
	sTormentedSquare->LoadRanking();
	
	sEvomon->Load();
	sEvomon->LoadReward();

	sArkaWar->Load();

	sLabyrinthDimensions->LoadInfo();
	sLabyrinthDimensions->LoadMission();
	sLabyrinthDimensions->LoadArea();
	sLabyrinthDimensions->LoadAreaData();
	sLabyrinthDimensions->LoadLevel();
	sLabyrinthDimensions->LoadMonster();
	sLabyrinthDimensions->LoadReward();

	sShopMgr->LoadShopTemplate();
	sShopMgr->LoadShopItems();

	sMossMerchant->Load();

	sDungeon->LoadInstance();
	sDungeon->LoadLevel();

	sMonsterSoul->LoadConverter();
	sMonsterSoul->LoadTransformation();
	sMonsterSoul->LoadReward();

/* CHARACTER DATABASE */
	sGuildMgr->LoadGuild();

	sPartyMgr->LoadPartyList();

	sMiniBomb->LoadReward();
	sMuRoomy->LoadReward();
	sJewelBingo->LoadReward();
	sNumericBaseball->LoadReward();

	sEventMgr->GetEventStage(EVENT_CHAOS_CASTLE_SURVIVAL);
	sEventMgr->GetEventStage(EVENT_TORMENTED_SQUARE_SURVIVAL);

	sChaosCastleSurvivalMgr->UpdateRanking();
	sLabyrinthDimensions->LoadRanking();
	sLabyrinthDimensions->LoadSchedule();

	sDungeon->LoadSavedInstance();

	sObjectMgr->LoadGameServerData();

	PreparedStatement* stmt = nullptr;

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_UPDATE_ON_SERVER_START);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	MuDatabase.Execute(stmt);

	stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GAMESERVER_SELECT);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	
	PreparedQueryResult result = LoginDatabase.Query(stmt);

	if ( !result )
	{
		sLog->outError("root", "Can't find Server Code %u info.", sGameServer->GetServerCode());
		return false;
	}

	Field* fields = result->Fetch();
	sGameServer->SetNetworkPort(fields[2].GetUInt16());
	sGameServer->SetNetworkIP(fields[3].GetCString());
	sGameServer->SetFlag(fields[4].GetUInt8());
	sGameServer->SetDefaultWorld(fields[6].GetUInt16());
	sGameServer->SetDefaultX(fields[7].GetInt16());
	sGameServer->SetDefaultY(fields[8].GetInt16());
	sGameServer->SetServerMode(fields[9].GetUInt8());

	if ( !sServerLink->Start() )
		return false;

	if ( !sAuthServer->Start() )
		return false;

	if ( !sConnectServer->Start() )
		return false;

	if ( !StartNetwork() )
		return false;

	sObjectMgr->SetRespawnMonsterTime(MyGetTickCount() + sGameServer->GetSpawnMonsterTime());
	sObjectMgr->SetRespawnMonster(true);

	m_timers[WUPDATE_WEATHERS].SetInterval(500);
	m_timers[WUPDATE_VIEWPORT].SetInterval(IN_MILLISECONDS);
	m_timers[WUPDATE_PINGDB].SetInterval(this->GetPingDB() * MINUTE * IN_MILLISECONDS);
	m_timers[WUPDATE_SHUTDOWN].SetInterval(1 * IN_MILLISECONDS);
	m_timers[WUPDATE_EVENT_ANIMATION].SetInterval(1 * IN_MILLISECONDS);

	return true;
}

bool StartNetwork()
{
    std::string worldListener = sMain->GetString("Network.BindIP", "0.0.0.0");

    return sWorldSocketMgr.StartNetwork(_ioService, worldListener, sGameServer->GetNetworkPort(), 1);
}

void MainApplication::Update(uint32 diff)
{
	for (int32 i = 0; i < WUPDATE_COUNT; ++i)
	{
		if (m_timers[i].GetCurrent() >= 0)
			m_timers[i].Update(diff);
		else
			m_timers[i].SetCurrent(0);
	}

	if (m_timers[WUPDATE_SHUTDOWN].Passed())
	{
		m_timers[WUPDATE_SHUTDOWN].Reset();

		sGameServer->UpdateShutdown();
	}

	sObjectMgr->UpdateSessions(diff);

	if (m_timers[WUPDATE_VIEWPORT].Passed())
	{
		m_timers[WUPDATE_VIEWPORT].Reset();

		sObjectMgr->UpdateSessionsViewport(diff);
	}

	if (m_timers[WUPDATE_WEATHERS].Passed())
	{
		m_timers[WUPDATE_WEATHERS].Reset();

		sWorldMgr->UpdateWeather();
		sWorldMgr->Update();
	}

	sObjectMgr->Update();

	if (m_timers[WUPDATE_EVENT_ANIMATION].Passed())
	{
		m_timers[WUPDATE_EVENT_ANIMATION].Reset();

		sObjectMgr->UpdateInvasionAnimation();
		sConnectServer->SendPlayerPercent();
		sAuthServer->SendPlayerPercent();
		sTeleport->Update();
		sPartyMgr->Update();
		sObjectMgr->SaveGameServerData();
		sDuelMgr->UpdateTime();
		sGuildWarMgr->Update();
		sGameServer->NonPKTimeUpdate();

		sGameServer->Update();
		sGameServer->PostManageUpdate();
		sGameServer->UpdateMACRestriction();

		sShopMgr->Update();

		sEventMgr->UpdateSeason();

		sQuestMgr->Update4thQuestInstance();

		sObjectMgr->CharacterAutoLogin();
	}
		
	sEventMgr->Update();

	if ( sGameServer->IsConnectServerReconnect() && sConnectServer->IsStopped() )
	{
		if ( sConnectServer->GetReconnectTime()->Elapsed(sGameServer->GetConnectServerReconnecTime()) )
		{
			sConnectServer->Start();
		}
	}

	sConnectServer->UpdatePacket();
	
	if ( sGameServer->IsAuthServerReconnect() && sAuthServer->IsStopped() )
	{
		if ( sAuthServer->GetReconnectTime()->Elapsed(sGameServer->GetAuthServerReconnecTime()) )
		{
			sAuthServer->Start();
		}
	}

	sAuthServer->UpdatePacket();

	if ( sGameServer->IsServerLinkServerReconnect() && sServerLink->IsStopped() )
	{
		if ( sServerLink->GetReconnectTime()->Elapsed(sGameServer->GetServerLinkServerReconnecTime()) )
		{
			sServerLink->Start();
		}
	}

	sServerLink->UpdatePacket();

	Custom::SystemTimer cur_time = Custom::SystemTimer();

	if ( cur_time.GetDay() != g_SiegeLordMixDay )
	{
		g_SiegeLordMixDay = cur_time.GetDay();
		g_SiegeLordMixCount = sGameServer->GetCastleSiegeLordMixMax();
	}
	
	if (m_timers[WUPDATE_PINGDB].Passed())
	{
		m_timers[WUPDATE_PINGDB].Reset();

		KeepDatabaseAlive();
	}
}