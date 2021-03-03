/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GameServer.h"
*
*/

#ifndef GAME_SERVER_H
#define GAME_SERVER_H

struct filter_data
{
	DECLARE_PROPERTY_STRING(Word);
	DECLARE_PROPERTY_STRING(WordLower);
	DECLARE_PROPERTY_STRING(WordUpper);
	DECLARE(uint8, flags);
	DECLARE(uint8, action);
	DECLARE_PROPERTY_STRING(Replace);
};

typedef std::vector<filter_data*> FilterList;

struct notice_data
{
	DECLARE_PROPERTY_STRING(Notice);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, World);
	DECLARE_FLAG(uint8, Flag);
	DECLARE_ENUM(uint32, Tick);
	DECLARE_ENUM(uint32, Time);
	DECLARE_ENUM(uint16, Group);
	DECLARE_ENUM(uint16, SubGroup);
};

typedef std::vector<notice_data*> NoticeList;

struct GoblinPointData
{
	DECLARE_ENUM(int16, MonsterLevelMin);
	DECLARE_ENUM(int16, MonsterLevelMax);
	DECLARE_ENUM(int16, PlayerLevelMin);
	DECLARE_ENUM(int16, PlayerLevelMax);
};

typedef std::vector<GoblinPointData*> GoblinPointDataList;

enum FilterFlags
{
	FILTER_FLAG_NONE			= 0,
	FILTER_FLAG_TALK			= 1 << 0,
	FILTER_FLAG_DB				= 1 << 1,
	FILTER_FLAG_CHARACTER_NAME	= 1 << 2,
};

enum FilterAction
{
	FILTER_ACTION_NONE			= 0,
	FILTER_ACTION_REPLACE		= 1,
	FILTER_ACTION_CLEAR			= 2,
	FILTER_ACTION_REJECT		= 3,
};

enum NoticeFlags
{
	NOTICE_FLAG_NONE			= 0,
	NOTICE_FLAG_PK				= 1 << 0,
	NOTICE_FLAG_ADMIN			= 1 << 1,
	NOTICE_FLAG_HERO			= 1 << 2,
};

enum GameServerSignal
{
	GAMESERVER_SIGNAL_SHUTDOWN,
	GAMESERVER_SIGNAL_SHOP,
	GAMESERVER_SIGNAL_QUEST,
	GAMESERVER_SIGNAL_EVENT_MANAGER,
	GAMESERVER_SIGNAL_MONSTER,
	GAMESERVER_SIGNAL_CONFIG,
	GAMESERVER_SIGNAL_ITEM,
	GAMESERVER_SIGNAL_CHARACTER,
	GAMESERVER_SIGNAL_SKILL,
	GAMESERVER_SIGNAL_TELEPORT,
	GAMESERVER_SIGNAL_CASH_SHOP,
	GAMESERVER_SIGNAL_MIX,
	GAMESERVER_SIGNAL_WORLD,
	GAMESERVER_SIGNAL_ACTIVE,
	GAMESERVER_SIGNAL_PENTAGRAM,
};

struct NonPKTimeData
{
	DECLARE_ENUM(uint8, StartHour);
	DECLARE_ENUM(uint8, StartMinute);
	DECLARE_ENUM(uint8, EndHour);
	DECLARE_ENUM(uint8, EndMinute);

	int32 GetStartSecond() const
	{
		return this->GetStartHour() * HOUR + this->GetStartMinute() * MINUTE;
	}

	int32 GetEndSecond() const
	{
		return this->GetEndHour() * HOUR + this->GetEndMinute() * MINUTE;
	}
};

typedef std::vector<NonPKTimeData*> NonPKTimeList;

void LOG_ITEM_DROP(Player* pPlayer, Item * item, std::string const& comment = "");
void LOG_ITEM_LOOT(Player* pPlayer, Item * item, std::string const& comment = "");
void LOG_ITEM_SELL(Player* pPlayer, Item * item, std::string const& comment = "");
void LOG_ITEM_BUY(Player* pPlayer, Item * item, std::string const& comment = "");
void LOG_JEWEL_DROP(Player* pPlayer, uint16 world, int16 x, int16 y, Item const* item);

typedef std::map<std::string, int32> PostManageMACMap;

typedef std::set<std::string> MACMuteList;

struct MACRestrictionData
{
	MACRestrictionData()
	{
		this->ResetTime(0);
		this->SetApplied(false);
	}

	bool TimePassed(uint32 restriction) const
	{
		if ( !this->GetTime(restriction) )
			return false;

		return GetTickCount() > this->GetTime(restriction);
	}

	DECLARE_PROPERTY_ARRAY(uint32, Time, PlayerAction::PLAYER_ACTION_MAX);
	DECLARE_ARRAY_BOOL(Applied, PlayerAction::PLAYER_ACTION_MAX);
};

typedef std::map<std::string, MACRestrictionData> MACRestrictionDataMap;

struct notification
{
	DECLARE_STRING_FIXED(Text, 100);
};

typedef std::list<notification*> NotificationList;

struct OffsetData
{
	DECLARE_ENUM(uint32, Offset);
	DECLARE_ENUM(uint8, Value);
	DECLARE_ENUM(uint8, Fix);
};

typedef std::map<uint32, OffsetData*> OffsetDataMap;

struct OffsetFPS
{
	DECLARE_ENUM(uint32, Offset);
	DECLARE_ENUM(uint8, Mod);
	DECLARE_ENUM(uint8, Original);
};

typedef std::map<uint32, OffsetFPS*> OffsetFPSMap;

typedef std::set<std::string> CheatScanWhiteList;

bool IsServerIN(std::string const& data);

struct OfflineAttackWorld
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, LevelMin);
	DECLARE_ENUM(int16, LevelMax);
};

typedef std::map<uint16, OfflineAttackWorld*> OfflineAttackWorldMap;

struct OfflineAttackZone
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
	DECLARE_BOOL(Enabled);
};

typedef std::vector<OfflineAttackZone*> OfflineAttackZoneList;

enum AntiHackID
{
	ANTIHACK_ID_BYTE_OFFSET				= 1,
	ANTIHACK_ID_DOUBLE_OFFSET			= 2,
	ANTIHACK_ID_HOOKED_FUNCTION			= 3,
	ANTIHACK_ID_FRAME_COUNT				= 5,
	ANTIHACK_ID_CHEAT_SCAN				= 8,
	ANTIHACK_ID_WALK_SPEED				= 9,
	ANTIHACK_ID_WRONG_PATH				= 10,
	ANTIHACK_ID_WALK_WRONG_WORLD		= 11,
	ANTIHACK_ID_WALK_PATH_COUNT			= 12,
	ANTIHACK_ID_WALK_PATH_TABLE			= 13,
	ANTIHACK_ID_WALK_PATH_BLOCKED		= 14,
	ANTIHACK_ID_COMBO_CHECK				= 15,
	ANTIHACK_ID_ATTACK_TIME				= 16,
	ANTIHACK_ID_MACRO_POTION			= 17,
	ANTIHACK_ID_MACRO_SKILL				= 18,
	ANTIHACK_ID_KICK					= 19,
	ANTIHACK_ID_ATTACK_RANGE			= 20,
};

typedef std::vector<uint16> TeleportBuffList;

class GameServer
{
	SingletonInstance(GameServer);

public:
	GameServer();
	virtual ~GameServer();

	void LoadCommonSettings();
	void LoadCashShopSettings();
	void LoadBaseData();
	void LoadFilter();
	void LoadNotice();
	void LoadGoblinPoint();
	void LoadNonPKTime();
	bool IsGoblinPointLevelAllowed(Player* pPlayer, Monster* pMonster);
	void NonPKTimeUpdate();
	bool FilterText(std::string & text, uint8 flags);
	void Update();
	void UpdateNotice();
	void UpdateShutdown();
	void ProcessSignal(uint16 signal, int32 data_1, int32 data_2, int32 data_3);
	void UpdateAccountsDisconnect();
	void PostManageUpdate();
	void PostManageUpdate(Player* pPlayer);
	bool CanPost(Player* pPlayer, std::string const& message);

	void MACMute(std::string data, int32 operation, Player* pPlayer);
	bool IsMACMuted(Player* pPlayer);

	void ApplyMACRestriction(std::string const& data, int32 operation, uint32 restriction, uint32 time, Player* pPlayer);
	bool IsMACRestriction(uint32 restriction, Player* pPlayer);
	void UpdateMACRestriction();

	void LoadNotification();
	void SendNotification();
	void SendNotification(Player* pPlayer);

	void LoadOffsetData();
	void SendOffsetData(Player* pPlayer = nullptr);

	void LoadOffsetFPS();
	void SendOffsetFPS(Player* pPlayer = nullptr);

	void LoadCheatScanWhiteList();
	bool IsCheatScanWhiteList(std::string const& process_name) const;

	void LoadOfflineAttackWorld();
	void LoadOfflineAttackZone();

	bool SerialCheck(Player* pPlayer, uint16 server, uint32 serial, std::string const& item_name, uint8 type);
	void SerialCheckCallBack(PreparedQueryResult result);
	void LogAntiHack(Player* pPlayer, uint8 type, const char* msg, ...);

	void ProcessTrack(Player* pPlayer);
	void ProcessTrack(Player* pPlayer, uint8 * Packet);

	bool IsOfflineAttackWorld(Player* pPlayer, uint16 world_id) const;
	bool IsOfflineAttackZone(Player* pPlayer, uint16 world_id, int16 x, int16 y) const;

	bool IsDiscountWC() const;
	bool IsDiscountGP() const;

	void LogDamagePVP(Player* pPlayer01, Player* pPlayer02, int32 hp_damage, int32 sd_damage, uint16 skill);

	PostManageMACMap post_manage_mac_map;
	MACMuteList mac_mute_list;
	MACRestrictionDataMap mac_restriction_map;
	NotificationList notification_list;
	OffsetDataMap offset_data_map;
	OffsetFPSMap offset_fps_map;
	CheatScanWhiteList cheat_scan_white_list;

	OfflineAttackWorldMap offline_attack_world_map;
	OfflineAttackZoneList m_OfflineAttackZoneList;

	DECLARE_BOOL(GlobalMute);

	DECLARE_ENUM(uint16, NetworkPort);
	DECLARE_STRING_FIXED(NetworkIP, 16);
	DECLARE_ENUM(uint16, DefaultWorld);
	DECLARE_ENUM(int16, DefaultX);
	DECLARE_ENUM(int16, DefaultY);
	DECLARE_FLAG(uint8, Flag);

	DECLARE_ENUM(uint8, ServerMode);
	DECLARE_ENUM(uint8, ServerType);

	DECLARE_PROPERTY(int32, ShutdownTime);

	DECLARE_ENUM(int32, MaxConnectionsPerIP);
	DECLARE_BOOL(Shutdown);
	DECLARE_ENUM(uint32, AccountsDisconnectTick);

	DECLARE_STRUCT(TickTimer, SerialCheckTime);
	
	// Server Settings
	DECLARE_ENUM(uint16, ServerCode);
	uint16 GetServerGroup() const { return this->GetServerCode() / MAX_SERVER_PER_GROUP; }
	DECLARE_PROPERTY_ARRAY(uint8, GameServerVersion, 5);
	DECLARE_STRING_FIXED(GameServerSerial, 17);
	DECLARE_BOOL(PacketRecvLog);
	DECLARE_BOOL(PacketSendLog);
	DECLARE_PROPERTY_STRING(WelcomeMessage);
	DECLARE_BOOL(Test);
	DECLARE_ENUM(uint32, TestKickTime);
	DECLARE_ENUM(uint32, TestKey);
	DECLARE_BOOL(Debug);


	// Account Settings
	DECLARE_BOOL(SecurityCode);
	DECLARE_ENUM(uint32, AccountDisconnectTime);

	DECLARE_ENUM(int32, WrongSerialCount);
	DECLARE_ENUM(int32, WrongSerialTime);
	DECLARE_ENUM(int32, WrongDecryptCount);
	DECLARE_ENUM(int32, WrongDecryptTime);

	DECLARE_BOOL(ItemDropLogDB);
	DECLARE_BOOL(ItemLootLogDB);
	DECLARE_BOOL(ItemBuyLogDB);
	DECLARE_BOOL(ItemSellLogDB);
	DECLARE_BOOL(LogDBTrade);
	DECLARE_BOOL(LogDBPersonalStore);
		
	DECLARE_ENUM(int32, NormalExperience);
	DECLARE_ENUM(int32, MasterExperience);
	DECLARE_ENUM(int32, MajesticExperience);
	DECLARE_ENUM(int32, NormalZen);
	DECLARE_ENUM(int32, MasterZen);
	DECLARE_ENUM(int32, MajesticZen);

	DECLARE_BOOL(MultiAttackPenetration);
	DECLARE_BOOL(MultiAttackFireScream);
	DECLARE_BOOL(ViewportFrustrum);
	DECLARE_BOOL(NormalMonsterMovement);
		
	DECLARE_ENUM(int16, ElfSoldierMaxLevel);
	DECLARE_BOOL(ElfSoldierUpdate);
	DECLARE_BOOL(ElfSoldierRemoveOnDie);
	
	DECLARE_BOOL(CashShopEnabled);
	DECLARE_ENUM(uint16, CashShopItemListNumber);
	DECLARE_ENUM(uint16, CashShopItemListMonth);
	DECLARE_ENUM(uint16, CashShopItemListYear);
	DECLARE_ENUM(uint16, CashShopBannerNumber);
	DECLARE_ENUM(uint16, CashShopBannerMonth);
	DECLARE_ENUM(uint16, CashShopBannerYear);
	DECLARE_STRING_FIXED(CashShopItemIP, 16);
	DECLARE_STRING_FIXED(CashShopItemDir, 32);
	DECLARE_STRING_FIXED(CashShopItemDirTest, 32);
	DECLARE_STRING_FIXED(CashShopBannerIP, 16);
	DECLARE_STRING_FIXED(CashShopBannerDir, 32);
	DECLARE_STRING_FIXED(CashShopBannerDirTest, 32);
	DECLARE_ENUM(uint32, CashShopCoinUpdateTime);
	DECLARE_ENUM(uint32, CashShopGiftUpdateTime);
	DECLARE_BOOL(CashShopUseJoin);
	DECLARE_BOOL(CashShopSafeZone);
	DECLARE_ENUM(uint8, CashShopDiscountWC);
	DECLARE_ENUM(uint8, CashShopDiscountGP);
	DECLARE_ENUM(time_t, CashShopDiscountDate);
	
	DECLARE_ENUM(uint32, ConnectCheckTimeDelay);
	DECLARE_BOOL(ConnectCheckResetOnAction);
	DECLARE_ENUM(uint32, ServerMoveKickTime);
	DECLARE_ENUM(uint8, MaxLoggingAttempts);
	DECLARE_ENUM(int32, MagicGladiatorCreate);
	DECLARE_ENUM(int32, DarkLordCreate);
	DECLARE_ENUM(int32, RageFighterCreate);
	DECLARE_ENUM(int32, GrowLancerCreate);

	// Account Warehouse
	DECLARE_BOOL(WarehouseLock);
	DECLARE_ENUM(uint32, WarehouseMaxMoney);
	DECLARE_BOOL(WarehouseSaveMoney);
	DECLARE_BOOL(WarehouseGetMoney);

	DECLARE_ENUM(uint32, AutoSaveTime);
	DECLARE_ENUM(uint32, AutoSaveOfflineTime);
	
	DECLARE_PROPERTY_ARRAY(int16, MaxLevel, LEVEL_DATA_MAX);
	DECLARE_ENUM(int16, MinMonsterLevelMaster);
	DECLARE_ENUM(int16, MinMonsterLevelMajestic);

	// Character Settings
	DECLARE_ENUM(uint32, MaxCharacterZen);
	
	DECLARE_ARRAY_BOOL(AutoRecuperationEnabled, POWER_MAX);
	DECLARE_PROPERTY_ARRAY(int16, AutoRecuperationMaxLevel, POWER_MAX);
	DECLARE_PROPERTY_ARRAY(uint8, AutoRecuperationCount, POWER_MAX);
	DECLARE_PROPERTY_ARRAY(uint8, AutoRecuperationCountRest, POWER_MAX);

	DECLARE_ENUM(int32, SDAttackSuccessRate);
	DECLARE(uint8, shield_combo_miss_init);
	DECLARE(uint8, shield_damage_to_sd_rate);
	
	DECLARE_ENUM(int16, PlayerPVPMinLevel);
	DECLARE_BOOL(PlayerSelfDefenseEnabled);
	DECLARE_ENUM(uint32, PlayerSelfDefenseTime);
	DECLARE_ENUM(int32, PlayerMurderCount);
	DECLARE_ENUM(int32, PlayerMurderTime);
	DECLARE_ENUM(int32, PlayerMurderLimitCount);
	DECLARE_ENUM(int32, PlayerHeroCount);
	DECLARE_ENUM(int32, PlayerHeroTime);
	DECLARE_BOOL(PlayerPKItemDrop);
	DECLARE_ENUM(uint8, PlayerPKItemDropMinLevel);
	DECLARE_BOOL(PlayerPKItemDropExcellent);
	DECLARE_BOOL(PlayerPKItemDropAncient);
	DECLARE_BOOL(PlayerPKItemDropHarmony);
	DECLARE_BOOL(PlayerPKItemDrop380);
	DECLARE_ENUM(uint8, PlayerPKItemDropItemMaxLevel);
	DECLARE_ENUM(int32, PlayerPKWarpCostIncrease);
	
	DECLARE_PROPERTY_ARRAY(int32, MaxStat, MAX_STAT_TYPE);

	DECLARE_BOOL(PersonalStoreEnabled);
	DECLARE_BOOL(PersonalStoreOff);
	DECLARE_ENUM(int16, PersonalStoreMinLevel);
	DECLARE_BOOL(PersonalStoreDisconnect);
	DECLARE_ENUM(uint32, PersonalStoreOffDuration);
	DECLARE_ENUM(int32, PersonalStoreOffCount);
	DECLARE_ENUM(int32, PersonalStoreOffTotalCount);
	
	DECLARE_BOOL(CharacterRepairButtonEnabled);
	DECLARE_ENUM(int16, CharacterRepairButtonMinLevel);
	
	DECLARE_BOOL(HarmonyEnabled);
	DECLARE(uint8, harmony_refine_rate);
	DECLARE(uint8, harmony_strengthen_rate);
	DECLARE(uint8, harmony_smelt_normal_rate);
	DECLARE(uint8, harmony_smelt_exe_rate);
	DECLARE(uint8, harmony_strengthen_update_normal_rate);
	DECLARE(uint8, harmony_strengthen_update_exe_rate);
	DECLARE_BOOL(HarmonyOnAncient);

	DECLARE(uint8, option_380_enabled);
	DECLARE(uint32, option_380_price);
	DECLARE(uint8, option_380_rate_grade_1);
	DECLARE(uint8, option_380_rate_grade_2);
	DECLARE(uint8, option_380_rate_grade_3);

	DECLARE_BOOL(PartyEnabled);
	DECLARE(uint32, party_max_wait_interval);
	DECLARE(int16, party_max_level_dif);
	DECLARE_ENUM(uint32, PartyMemberOfflineInterval);
	DECLARE_BOOL(PartyLeaderChange);
	DECLARE(uint8, party_allow_dif_gens);
	DECLARE(uint8, party_in_battlezone);
	DECLARE(uint8, party_allow_pk);
	DECLARE_PROPERTY_ARRAY(int32, PartyExperience, MAX_PARTY_MEMBERS);
	DECLARE_PROPERTY_ARRAY(int32, PartyBonusExperience, MAX_PARTY_MEMBERS);
	DECLARE_BOOL(PartyMatchingEnabled);
	DECLARE_BOOL(PartyMatchingLeaderChange);
	DECLARE_BOOL(PartyMoveEnabled);
	DECLARE_ENUM(uint32, PartyMoveCost);

	DECLARE(uint32, item_duration_time);
	DECLARE(uint32, looting_time);
	DECLARE(uint16, item_drop_rate_normal);
	DECLARE(uint16, item_drop_rate_exe);
	DECLARE(uint8, item_drop_skill_rate_normal);
	DECLARE(uint8, item_drop_luck_rate_normal);
	DECLARE_PROPERTY_ARRAY(int32, ItemDropSocketRate, MAX_SOCKET_SLOT);
	DECLARE(uint8, item_drop_luck_rate_exe);
	DECLARE_PROPERTY_ARRAY(int32, ItemDropSocketRateExe, MAX_SOCKET_SLOT);
	DECLARE_RANGE(uint8, ItemDropExeOption);

	DECLARE_ENUM(int32, FenrirRepairRate);

	DECLARE_BOOL(JewelOfSoulEnabled);
	DECLARE_ENUM(int32, JewelOfSoulRate);
	DECLARE_ENUM(int32, JewelOfSoulRateLuck);

	DECLARE_BOOL(JewelOfLifeEnabled);
	DECLARE(int32, jewel_of_life_max_rate);
	DECLARE(int32, jewel_of_life_rate_normal);
	DECLARE(int32, jewel_of_life_rate_socket);
	DECLARE(int32, jewel_of_life_rate_exe);
	DECLARE(int32, jewel_of_life_rate_ancient);
	DECLARE_ENUM(uint8, JewelOfLifeMax);
	DECLARE_BOOL(JewelOfLifeReset);
	DECLARE_PROPERTY_STRING(JewelOfLifeResetWarning);
	DECLARE_ENUM(int32, JewelOfLifeResetRate);

	DECLARE_BOOL(SantaVillageEnabled);
	DECLARE_BOOL(SantaVillageSantaClausEnabled);
	DECLARE_ENUM(uint32, SantaVillageSantaClausGiftTime);
	DECLARE_ENUM(uint8, SantaVillageSeason);
	DECLARE_PROPERTY_STRING(SantaVillageSantaClausItemBag);
	DECLARE_BOOL(SantaVillageCheckSeason);

	DECLARE_PROPERTY_STRING(PersonalMerchantShop);

	DECLARE(uint8, trade_enabled);
	DECLARE(uint32, trade_max_wait_interval);
	DECLARE_ENUM(uint32, TradeMaxZen);

	DECLARE(uint8, guild_enabled);
	DECLARE(int16, guild_create_min_level);
	DECLARE(uint8, guild_create_gens);
	DECLARE(uint8, guild_join_gens);
	DECLARE(uint32, guild_max_wait_interval);
	DECLARE(uint8, guild_delete);
	DECLARE(uint8, guild_alliance_min_members);
	DECLARE(uint8, guild_alliance_gens);
	DECLARE(uint8, guild_alliance_max);
	DECLARE(uint8, guild_hostil_event_kill);
	DECLARE_BOOL(GuildAllianceCastleSiegeCheck);

	DECLARE_BOOL(GuildWarEnabled);
	DECLARE_ENUM(uint32, GuildWarMaxWaitInterval);
	DECLARE_ENUM(int32, GuildWarMax);
	DECLARE_ENUM(uint32, GuildWarIntervalTime);
	DECLARE_ENUM(int32, GuildWarMaxPoints);
	DECLARE_ENUM(int32, GuildWarKillPoints);
	DECLARE_ENUM(int32, GuildWarKillAssistantPoints);
	DECLARE_ENUM(int32, GuildWarKillBattlePoints);
	DECLARE_ENUM(int32, GuildWarKillMasterPoints);

	DECLARE_BOOL(BattleSoccerEnabled);
	DECLARE_ENUM(uint32, BattleSoccerDuration);
	DECLARE_ENUM(int32, BattleSoccerGoalPoints);
	DECLARE_ENUM(int32, BattleSoccerMaxPoints);
	DECLARE_ENUM(int32, BattleSoccerKillPoints);
	DECLARE_ENUM(int32, BattleSoccerKillAssistantPoints);
	DECLARE_ENUM(int32, BattleSoccerKillBattlePoints);
	DECLARE_ENUM(int32, BattleSoccerKillMasterPoints);

	DECLARE_BOOL(DuelEnabled);
	DECLARE(int16, dueling_min_level);
	DECLARE_ENUM(uint32, DuelRequiredZen);
	DECLARE(int32, dueling_max_wait_interval);
	DECLARE_ENUM(uint32, DuelDuration);
	DECLARE_ENUM(uint32, DuelRoomStandByTime);
	DECLARE_ENUM(int32, DuelingDamage);
	DECLARE_ENUM(int32, DuelRoundStandby);
	DECLARE_BOOL(DuelClassic);
	DECLARE_BOOL(DuelBuffDisabled);
	DECLARE_BOOL(DuelPentagramDisabled);
	DECLARE_BOOL(DuelMuunDisabled);
	DECLARE_ENUM(int32, DuelTestLevel);

	DECLARE_BOOL(FriendEnabled);
	DECLARE_ENUM(int16, FriendAddMinLevel);
	DECLARE_BOOL(FriendMailEnabled);
	DECLARE_ENUM(uint32, FriendMailCost);
	DECLARE_ENUM(uint32, FriendMailUpdateInterval);

	DECLARE_ENUM(int16, GenJoinMinLevel);
	DECLARE_ENUM(int32, GenChangeTime);
	DECLARE_BOOL(GenPenalization);
	DECLARE_ENUM(uint32, GenPenalizationCount);
	DECLARE_ENUM(uint32, GenPenalizationTime);

	DECLARE(uint32, kundun_refill_time);
	DECLARE(uint32, kundun_refill_hp);
	DECLARE(uint32, kundun_refill_hp_sec);

	DECLARE_ENUM(int32, SkillFireScreamExplosionRate);

	DECLARE_BOOL(HelperPotionEnabled);
	DECLARE_BOOL(HelperRepairEnabled);
	DECLARE_BOOL(HelperLootEnabled);
	DECLARE_BOOL(HelperAutoLootEnabled);

	// EVENT
	DECLARE_BOOL(BloodCastleEnabled);
	DECLARE_ENUM(uint32, BloodCastleDeliverWeaponTime);
	DECLARE_BOOL(BloodCastleSkillRushCheck);
	DECLARE_ENUM(int32, BloodCastleRewardScore);
	
	DECLARE_BOOL(ChaosCastleEnabled);
	DECLARE_BOOL(ChaosCastleSurvivalOfTheFittestEnabled);
	DECLARE_ENUM(int32, ChaosCastleSurvivalMaxPerPC);

	DECLARE_BOOL(DevilSquareEnabled);

	DECLARE_BOOL(DungeonRaceEnabled);
	DECLARE_PROPERTY_STRING(DungeonRaceReward);
	DECLARE_BOOL(LosttowerRaceEnabled);
	DECLARE_PROPERTY_STRING(LosttowerRaceReward);

	DECLARE_BOOL(RaklionEnabled);
	DECLARE_ENUM(uint32, RaklionSuccessTimeMin);
	DECLARE_ENUM(uint32, RaklionSuccessTimeMax);
	DECLARE_ENUM(uint32, RaklionFailTimeMin);
	DECLARE_ENUM(uint32, RaklionFailTimeMax);
	
	DECLARE_BOOL(KanturuEnabled);
	DECLARE(int32, kanturu_max_players);
	DECLARE(int32, kanturu_ice_storm_rate);
	DECLARE(int32, kanturu_skill_time);

	DECLARE_BOOL(CrywolfEnabled);

	DECLARE_BOOL(CrywolfBenefitApply);
	DECLARE_ENUM(uint8, CrywolfBenefitPlusRate);
	DECLARE_ENUM(uint8, CrywolfBenefitMonsterHP);
	DECLARE_BOOL(CrywolfBenefitKundunRefill);

	DECLARE_BOOL(CrywolfPenaltyApply);
	DECLARE_ENUM(uint8, CrywolfPenaltyJewelDrop);
	DECLARE_ENUM(uint8, CrywolfPenaltyExperienceGain);

	DECLARE(int16, crywolf_altar_min_level);
	DECLARE(uint16, crywolf_altar_max_contract);
	DECLARE(uint32, crywolf_altar_contract_valid_time);
	DECLARE(uint32, crywolf_altar_contract_last_time);
	DECLARE_PROPERTY(int32, CrywolfAltarScore);

	DECLARE(uint32, crywolf_aichange_time);
	DECLARE(uint32, crywolf_balgass_start_time);

	DECLARE_BOOL(CastleSiegeEnabled);
	DECLARE_BOOL(CastleSiegeRegisterEnabled);
	DECLARE_BOOL(CastleSiegeRegisterSignEnabled);
	DECLARE_BOOL(CastleSiegeMachineEnabled);
	DECLARE_ENUM(int32, CastleSiegeCrownTime);
	DECLARE_ENUM(int32, CastleSiegeLordMixMax);
	DECLARE_ENUM(int16, CastleSiegeRegisterMinLevel);
	DECLARE_ENUM(uint8, CastleSiegeRegisterMinMembers);
	DECLARE_ENUM(int32, CastleSiegeReduceDamageSameSide);
	DECLARE_ENUM(int32, CastleSiegeReduceDamageDifSide);
	DECLARE_ENUM(int32, CastleSiegeParticipantRewardMinutes);
	
	DECLARE_BOOL(DoppelgangerEnabled);
	DECLARE_BOOL(DoppelgangerPKCheck);
	DECLARE_ENUM(uint8, DoppelgangerPKLevelMax);
	DECLARE_ENUM(uint32, DoppelgangerMonsterSpeed);

	DECLARE_BOOL(ImperialFortressEnabled);
	DECLARE_BOOL(ImperialFortressPKCheck);
	DECLARE_ENUM(uint8, ImperialFortressPKLevelMax);
	DECLARE_BOOL(ImperialFortressInParty);
	
	DECLARE_ENUM(uint32, ImperialFortressStandbyTime);
	DECLARE_ENUM(uint32, ImperialFortressPlayTime);
	DECLARE_ENUM(uint32, ImperialFortressEndTime);

	DECLARE_ENUM(uint8, ImperialFortressDay);
	DECLARE_PROPERTY_ARRAY(uint32, ImperialFortressTimeZone, IMPERIAL_FORTRESS_MAX_ZONES);

	DECLARE_BOOL(ImperialFortressWalkCheck);
	DECLARE_BOOL(ImperialFortressTeleportCheck);
	DECLARE_BOOL(ImperialFortressMoveToPrevious);

	DECLARE(int32, happy_hour_experience_add);
	DECLARE(int32, happy_hour_drop_add);

	DECLARE_BOOL(CommandPostEnabled);
	DECLARE_ENUM(int16, CommandPostMinLevel);
	DECLARE_ENUM(uint32, CommandPostCost);
	DECLARE_ENUM(uint32, CommandPostDelay);
	DECLARE_ENUM(uint8, CommandPostType);
	DECLARE_PROPERTY_STRING(CommandPostHead);

	DECLARE_BOOL(CommandAddStatEnabled);
	DECLARE_ENUM(int16, CommandAddStatMinLevel);
	DECLARE_ENUM(uint32, CommandAddStatCost);

	DECLARE_BOOL(ScrambleEnabled);
	DECLARE_ENUM(int32, ScrambleTotalWordCount);
	DECLARE_ENUM(int32, ScrambleRepeatWordCount);
	DECLARE_PROPERTY_STRING(ScrambleReward);

	DECLARE_BOOL(GoblinPointEnabled);
	DECLARE_ENUM(int32, GoblinPointKillTime);
	DECLARE_ENUM(int32, GoblinPointRequiredTime);
	DECLARE_ENUM(int32, GoblinPointRequiredTimeOffline);

	DECLARE_BOOL(EventInventoryEnabled);

	DECLARE_BOOL(MuRoomyEnabled);
	DECLARE_BOOL(MuRoomySpecialEnabled);
	DECLARE_BOOL(MuRoomyTicketRequired);
	DECLARE_ENUM(uint32, MuRoomyRewardMoneyAmmount);

	DECLARE_BOOL(JoinItemsWithExpireTime);
	DECLARE_ENUM(int32, PetComboExperience);

	DECLARE_BOOL(GateHandle);

	DECLARE_BOOL(BarracksMoveEnabled);
	DECLARE_ENUM(int16, BarracksMoveMinLevel);
	DECLARE_ENUM(uint32, BarracksMoveCost);
	DECLARE_BOOL(BarracksMoveParty);

	DECLARE_BOOL(RefugeMoveEnabled);
	DECLARE_ENUM(int16, RefugeMoveMinLevel);
	DECLARE_ENUM(uint32, RefugeMoveCost);
	DECLARE_BOOL(RefugeMoveParty);

	FilterList filter_list;
	NoticeList notice_list;
	GoblinPointDataList goblin_point_list;
	NonPKTimeList non_pk_time_list;
	DECLARE_BOOL(PKTimeAllowed);

	DECLARE_BOOL(ConnectionDisabled);

	DECLARE_BOOL(TransactionSharedDuplicatedSerialDelete);
	DECLARE_BOOL(TransactionDuplicatedSerialDelete);

	DECLARE_BOOL(TransferEnabled);
	DECLARE_ENUM(uint16, TransferServer);
	DECLARE_ENUM(uint16, TransferPartyServer);

	DECLARE_ENUM(uint32, SpawnMonsterTime);

	DECLARE_BOOL(AttackSafeEnabled);
	DECLARE_ENUM(int32, AttackSafeCount);
	DECLARE_ENUM(uint32, AttackSafeTime);
	DECLARE_BOOL(AttackSafeKick);
	DECLARE_ENUM(uint32, AttackSpeedSendTime);
	
	DECLARE_BOOL(SkillEnergyCheck);
	DECLARE_BOOL(SkillLeadershipCheck);

	DECLARE_ENUM(uint32, GameCloseTime);

	DECLARE_ENUM(int32, RecvPacketQueueMax);
	DECLARE_BOOL(RecvPacketQueueKick);

	DECLARE_BOOL(ConnectServerReconnect);
	DECLARE_ENUM(uint32, ConnectServerReconnecTime);
	DECLARE_BOOL(AuthServerReconnect);
	DECLARE_ENUM(uint32, AuthServerReconnecTime);
	DECLARE_BOOL(ServerLinkServerReconnect);
	DECLARE_ENUM(uint32, ServerLinkServerReconnecTime);

	DECLARE_BOOL(CharacterDieExperienceDecrease);
	DECLARE_BOOL(CharacterDieZenDecrease);

	DECLARE_ENUM(int32, SkillManaShieldLimit);
	DECLARE_ENUM(int32, SkillManaShieldCastleSiegeLimit);

	DECLARE_BOOL(KickEnabled);
	DECLARE_ENUM(uint8, KickType);

	DECLARE_BOOL(MonsterStopAction);
	DECLARE_BOOL(MonsterViewportControl);
	DECLARE_BOOL(MasterCommand);

	DECLARE_BOOL(FixPowersOnBuff);

	DECLARE_BOOL(RefreshEnabled);
	DECLARE_ENUM(uint32, RefreshTime);

	DECLARE_BOOL(HostilCancelCommand);

	DECLARE_BOOL(PostManageEnabled);
	DECLARE_ENUM(int32, PostManageCount);

	DECLARE_BOOL(PostManageMACEnabled);
	DECLARE_ENUM(int32, PostManageMACCount);
	DECLARE_ENUM(uint8, PostManageMACDay);
	DECLARE_STRUCT(TickTimer, PostManageMACUpdate);

	DECLARE_BOOL(AntiSpamEnabled);
	DECLARE_ENUM(int32, AntiSpamCount);
	DECLARE_ENUM(uint32, AntiSpamTime);
	DECLARE_ENUM(uint32, AntiSpamMuteTime);

	DECLARE_BOOL(DobleFrustrum);

	DECLARE_ENUM(int32, SkillEarthPrisonStunRate);

	DECLARE_ENUM(int32, SkillSleepLimit);
	DECLARE_ENUM(int32, SkillSleepLimitTime);

	DECLARE_ENUM(int32, SkillBlindAdd);
	DECLARE_ENUM(int32, SkillBlindDiv);
	DECLARE_ENUM(int32, SkillBlindValue);
	DECLARE_ENUM(int32, SkillBlindTime);

	DECLARE_ENUM(int32, SkillIronDefenseValue1);
	DECLARE_ENUM(int32, SkillIronDefenseValue2);
	DECLARE_ENUM(int32, SkillIronDefenseDuration);

	DECLARE_ENUM(int32, SkillBloodHowlingRate);
	DECLARE_ENUM(int32, SkillBloodHowlingDamage);
	DECLARE_ENUM(int32, SkillBloodHowlingDuration);

	DECLARE_ENUM(int32, SkillCircleShieldDuration);
	DECLARE_ENUM(int32, SkillCircleShieldReduceAGRate);
	DECLARE_ENUM(int32, SkillCircleShieldReduceAG);
	DECLARE_ENUM(int32, SkillMagicPinExplosionRate);
	DECLARE_ENUM(int32, SkillMagicPinExplosionDamage);

	DECLARE_ENUM(int32, RadianceBleedingRate);
	DECLARE_ENUM(int32, RadianceBleedingDuration);
	DECLARE_ENUM(int32, RadianceParalysisRate);
	DECLARE_ENUM(int32, RadianceParalysisEffect);
	DECLARE_ENUM(int32, RadianceBindingDuration);
	DECLARE_ENUM(int32, RadianceBlindingRate);
	DECLARE_ENUM(int32, RadianceBlindingDuration);
	
	DECLARE_ENUM(int32, SkillBastionRate);
	DECLARE_ENUM(int32, SkillBastionSD);
	DECLARE_ENUM(int32, SkillBastionReduction);
	
	DECLARE_ENUM(int32, SkillPunishRatePVM);
	DECLARE_ENUM(int32, SkillPunishRatePVP);
	DECLARE_ENUM(uint32, SkillPunishFrequencyPVM);
	DECLARE_ENUM(uint32, SkillPunishFrequencyPVP);
	DECLARE_BOOL(SkillPunishFirstHit);
	DECLARE_ENUM(int32, SkillPunishMaxDamage);

	DECLARE_ENUM(int32, SkillAbsorbHPRate);
	DECLARE_ENUM(int32, SkillAbsorbSDRate);

	DECLARE_ENUM(int32, SkillIllusionDuration);

	DECLARE_ENUM(int32, SkillArchangelWillDamage);
	DECLARE_ENUM(int32, SkillArchangelWillSkillDamage);
	DECLARE_ENUM(int32, SkillArchangelWillDuration);

	DECLARE_ENUM(int32, SkillPoisonArrowDebuffRate);
	DECLARE_ENUM(int32, SkillCureBuffRate);
	DECLARE_ENUM(int32, SkillParalizeDebuffRate);

	DECLARE_ENUM(int32, SkillSplashDamagePercent);

	DECLARE_ENUM(int32, SkillElementalImmuneI);
	DECLARE_ENUM(int32, SkillElementalImmuneII);
	DECLARE_ENUM(int32, SkillElementalBerserkerI);
	DECLARE_ENUM(int32, SkillElementalBerserkerIDamage);
	DECLARE_ENUM(int32, SkillElementalBerserkerIDefense);


	DECLARE_BOOL(TormentedSquareEnabled);
	DECLARE_BOOL(TormentedSquareSurvivalEnabled);

	DECLARE_BOOL(ArkaWarEnabled);
	DECLARE_ENUM(int32, ArkaWarRegisterMinMembers);
	DECLARE_ENUM(int32, ArkaWarRegisterMaxMembers);
	DECLARE_ENUM(int32, ArkaWarMinGuilds);
	DECLARE_ENUM(int32, ArkaWarDamageReductionAlly);
	DECLARE_ENUM(int32, ArkaWarDamageReductionEnemy);

	DECLARE_BOOL(ProtectorOfAcheronEnabled);

	DECLARE_BOOL(MossMerchantEnabled);
	std::string MossMerchantItemBag[MOSS_ITEM_TYPE_MAX];

	DECLARE_ENUM(int32, MuunDurabilityRate);
	DECLARE_BOOL(MuunAttackEnabled);
	DECLARE_ENUM(int32, MuunAttackRange);
	DECLARE_ENUM(int32, MuunAttackDelay);
	DECLARE_ENUM(int32, MuunAttackHitDelay);
	DECLARE_BOOL(MuunEvolveBonus);
	DECLARE_ENUM(int64, MuunEvolveBonusTime);
	DECLARE_ENUM(int32, MuunMountTime);

	DECLARE_ENUM(uint8, MaxExpandedInventory);

	DECLARE_PROPERTY_ARRAY(int32, DamageModPVP, Character::MAX_CLASS);
	DECLARE_PROPERTY_ARRAY(int32, DamageModPVE, Character::MAX_CLASS);

	DECLARE_BOOL(OfflineAttack);
	DECLARE_BOOL(OfflineMove);
	DECLARE_ENUM(int16, OfflineAttackMinLevel);
	DECLARE_ENUM(int16, OfflineAttackMaxLevel);
	DECLARE_BOOL(OfflineCloseWhenWrongLevel);
	DECLARE_ENUM(uint32, OfflineAttackTime);
	DECLARE_ENUM(uint32, OfflineAttackCount);
	DECLARE_BOOL(OfflineAttackOnDisconnect);
	DECLARE_ENUM(uint32, OfflineDisconnectTime);
	DECLARE_ENUM(uint32, OfflineSkillTime);
	DECLARE_ENUM(uint32, OfflineDieDisconnectTime);
	DECLARE_BOOL(OfflineAttackZenLoot);
	DECLARE_BOOL(OfflineAttackDetailAutoloot);
	DECLARE_BOOL(OfflineAttackBuffItem);
	DECLARE_BOOL(OfflineAttackGoblinPointsGain);
	DECLARE_ENUM(uint8, OfflineAttackFlag);
	DECLARE_BOOL(OfflineAttackMonsterAICheck);
	DECLARE_ENUM(int32, OfflineAttackIPCount);

	DECLARE_BOOL(PVPEnabled);

	DECLARE_ENUM(uint32, SessionTimeout);

	DECLARE_BOOL(MultiAttackSkillCheck);

	DECLARE_BOOL(Active);
	DECLARE_BOOL(ActiveStatus);
	DECLARE_ENUM(uint32, InactiveTime);
	DECLARE_ENUM(TCType, InactiveTick);
	DECLARE_ENUM(TCType, InactiveNotificationTick);

	DECLARE_BOOL(AdministratorAuthorization);
	DECLARE_BOOL(CharacterOnlineOnLogin);

	DECLARE_BOOL(AntiHackCoordinateAttackCheck);
	DECLARE_BOOL(AntiHackSkillHeadcodeCheck);
	DECLARE_BOOL(AntiHackWalkCheck1);
	DECLARE_ENUM(int32, AntiHackWalkCheckDistance1);
	DECLARE_ENUM(int32, AntiHackWalkCheckError1);
	DECLARE_BOOL(AntiHackWalkCheck2);
	DECLARE_BOOL(AntiHackWalkCheck3);
	DECLARE_BOOL(AntiHackWalkCheck4);
	DECLARE_BOOL(AntiHackWalkCheck5);

	DECLARE_BOOL(AntiHackCoordinateVerify);
	DECLARE_ENUM(uint32, AntiHackCoordinateVerifyTime);

	DECLARE_BOOL(SkillBlessStrenghtenerFix);

	DECLARE_BOOL(EvomonEnabled);
	DECLARE_BOOL(EvomonPartyReward);
	DECLARE_BOOL(EvomonPartySpecialReward);
	DECLARE_BOOL(EvomonSend);

	DECLARE_BOOL(LuckyItemFix);
	DECLARE_ENUM(int32, LuckyItemFixCount);

	DECLARE_BOOL(ChaosMachineSafeItemMove);

	DECLARE_ENUM(uint32, FereaBossHPRecoveryTime);
	DECLARE_ENUM(int32, FereaBossHPRecoveryCount);

	DECLARE_BOOL(DarkRavenAutorepair);
	DECLARE_BOOL(ItemBuffCheck);

	DECLARE_BOOL(MonsterTeleportOutOfRange);
	DECLARE_BOOL(MonsterAttackWallCheck);

	DECLARE_BOOL(ReflectFix);

	DECLARE_BOOL(FullInventoryToGremoryCase);
	DECLARE_BOOL(GremoryCaseFreeSpaceCheck);

	DECLARE_BOOL(AntiDupeBoxEnabled);
	DECLARE_BOOL(AntiDupeBoxCheck);
	DECLARE_ENUM(uint32, AntiDupeBoxCheckTime);
	DECLARE_BOOL(AntiDupeBoxRemove);
	DECLARE_BOOL(AntiDupeBlock);
	DECLARE_BOOL(AntiDupeKick);
	DECLARE_BOOL(TransactionRollbackCurrency);

	DECLARE_ENUM(int32, TempDualWeaponDmg1);
	DECLARE_ENUM(int32, TempDualWeaponDmg2);

	DECLARE_BOOL(AttackSpeedCheck);
	DECLARE_ENUM(int32, AttackSpeedCount);
	DECLARE_ENUM(int32, AttackSpeedRange);
	DECLARE_BOOL(AttackSpeedKick);
	DECLARE_BOOL(AttackSpeedBan);

	DECLARE_BOOL(LastManStandingEnabled);
	DECLARE_ENUM(int32, LastManStandingDuration);
	DECLARE_ENUM(int32, LastManStandingKillScore);
	DECLARE_ENUM(int32, LastManStandingWinScore);
	DECLARE_ENUM(int32, LastManStandingMaxPerPC);
	DECLARE_ENUM(int32, LastManStandingDamageReduction);
	DECLARE_ENUM(int16, LastManStandingMinLevel);
	DECLARE_BOOL(LastManStandingClassic);
	DECLARE_BOOL(LastManStandingHideCharacter);
	DECLARE_ENUM(int32, LastManStandingLevelRange);

	DECLARE_BOOL(PKBossEnabled);
	DECLARE_ENUM(uint32, PKBossTime);
	DECLARE_BOOL(AttackDebuffFirstHit);

	DECLARE_BOOL(TradeInterfaceCancelBan);

	DECLARE_BOOL(ServerWarpTransactionCheck1);
	DECLARE_BOOL(ServerWarpTransactionCheck2);
	DECLARE_BOOL(ServerWarpTransactionCheck3);

	DECLARE_BOOL(DupeCompleteBan);

	DECLARE_BOOL(PersonalStoreLockSamePC);

	DECLARE_ENUM(int32, WalkSpeedDistance);
	DECLARE_ENUM(int32, WalkSpeedCount);
	DECLARE_BOOL(WalkSpeedFix);
	DECLARE_BOOL(WalkSpeedKick);
	DECLARE_BOOL(WalkSpeedBan);

	DECLARE_BOOL(HackCheckEnabled);
	DECLARE_ENUM(uint32, HackCheckResetTime);
	DECLARE_ENUM(uint32, HackCheckTime);
	DECLARE_ENUM(uint32, HackCheckLoopTime);
	DECLARE_ENUM(int32, HackCheckCount);
	DECLARE_ENUM(int32, HackCheckFrameCount);
	DECLARE_BOOL(HackCheckKick);
	DECLARE_BOOL(HackCheckBan);
	DECLARE_BOOL(HackCheckFullBan);
	DECLARE_BOOL(NixiesLakeEnabled);
	DECLARE_ENUM(uint32, NixiesLakeStandbyTime);
	DECLARE_ENUM(uint32, NixiesLakeClosedTime);
	DECLARE_ENUM(uint32, NixiesLakeBattleTime);
	DECLARE_BOOL(NixiesLakeFree);
	DECLARE_ENUM(uint32, NixiesLakeBossRecoveryStandyTime);
	DECLARE_ENUM(int32, NixiesLakeBossRecoveryPercent);
	DECLARE_ENUM(uint32, NixiesLakeBossRecoveryTime);
	DECLARE_PROPERTY_STRING(NixiesLakeItemBag1);
	DECLARE_PROPERTY_STRING(NixiesLakeItemBag2);
	DECLARE_PROPERTY_STRING(NixiesLakeItemBag3);
	DECLARE_PROPERTY_STRING(NixiesLakeItemBag4);
	DECLARE_PROPERTY_STRING(NixiesLakeItemBag5);
	
	DECLARE_BOOL(LabyrinthOfDimensionsEnabled);
	DECLARE_BOOL(LabyrinthOfDimensionsRequiredMaster);
	DECLARE_ENUM(int32, LabyrinthOfDimensionsRequiredLevel);
	DECLARE_ENUM(int32, LabyrinthOfDimensionsGoblinScore);
	DECLARE_ENUM(int32, LabyrinthOfDimensionsGoblinStage);
	DECLARE_BOOL(LabyrinthOfDimensionsMoveOnFail);
	DECLARE_ENUM(uint32, LabyrinthOfDimensionsMissionTime);
	DECLARE_ENUM(int32, LabyrinthOfDimensionsDayFirstScore);
	DECLARE_ENUM(int64, LabyrinthOfDimensionsStateDuration);
	DECLARE_ENUM(int32, LabyrinthOfDimensionsTimeScore);
	DECLARE_PROPERTY_ARRAY(int32, LabyrinthOfDimensionsMove, MAX_LABYRINTH_OF_DIMENSIONS_RANK);

	DECLARE_ENUM(int32, ExcellentDamageRateLimit);
	DECLARE_ENUM(int32, CriticalDamageRateLimit);
	DECLARE_ENUM(int32, DecreaseDamageRateLimit);
	DECLARE_ENUM(int32, ReflectDamageRateLimit);
	DECLARE_ENUM(int32, IgnoreDefenseRateLimit);
	DECLARE_ENUM(int32, ShieldDefenseRateLimit);

	DECLARE_ENUM(int32, MaxSocketWear);

	DECLARE_ENUM(int32, AntiFloodCount);
	DECLARE_BOOL(AntiFloodKick);

	DECLARE_BOOL(WelcomeNotice);

	DECLARE_BOOL(CheatScanEnabled);
	DECLARE_BOOL(CheatScanKick);
	DECLARE_BOOL(CheatScanBan);
	DECLARE_ENUM(int32, CheatScanCount);
	DECLARE_ENUM(uint32, CheatScanTime);

	DECLARE_BOOL(CastleDeepEnabled);

	DECLARE_BOOL(WorldBossEnabled);
	DECLARE_ENUM(int32, WorldBossRequiredPlayer);
	DECLARE_ENUM(uint8, WorldBossStartNotification);
	DECLARE_BOOL(WorldBossGuildPK);
	DECLARE_ENUM(int32, WorldBossPKGAP);
	DECLARE_ENUM(int32, WorldBossPKRange);
	DECLARE_ENUM(int32, WorldBossRequiredPointsToAttack);
	DECLARE_ENUM(int32, WorldBossIncreaseDamagePerPoint);
	DECLARE_ENUM(int32, WorldBossMaxPKPoints);
	DECLARE_BOOL(WorldBossNormalRespawn);
	DECLARE_PROPERTY_STRING(WorldBossItemBag);
	DECLARE_ENUM(int32, WorldBossAOERange);

	DECLARE_PROPERTY_ARRAY(uint32, SkillTeleportStateTime, TELEPORT_MAX);
	DECLARE_ARRAY_BOOL(SkillTeleportStateAttack, TELEPORT_MAX);
	DECLARE_ARRAY_BOOL(SkillTeleportStateRemoveVP, TELEPORT_MAX);

	DECLARE_BOOL(MiniBombEnabled);
	DECLARE_BOOL(MiniBombRequiredTicket);
	DECLARE_BOOL(MiniBombReward);
	DECLARE_ENUM(uint16, MiniBombRevealScore);
	DECLARE_ENUM(uint16, MiniBombFoundedBombsScore);
	DECLARE_ENUM(uint16, MiniBombWrongBombsScore);
	DECLARE_ENUM(uint16, MiniBombFailScore);
	DECLARE_ENUM(uint16, MiniBombClearScore);

	DECLARE_PROPERTY_ARRAY(int32, MaxAttackSpeed, Character::MAX_CLASS);

	DECLARE_BOOL(JewelBingoEnabled);
	DECLARE_BOOL(JewelBingoRequiredTicket);
	DECLARE_BOOL(JewelBingoReward);
	DECLARE_ENUM(int32, JewelBingoScoreH);
	DECLARE_ENUM(int32, JewelBingoScoreV);
	DECLARE_ENUM(int32, JewelBingoScoreD);
	DECLARE_ENUM(int32, JewelBingoScoreSpecialH);
	DECLARE_ENUM(int32, JewelBingoScoreSpecialV);
	DECLARE_ENUM(int32, JewelBingoScoreNotMatched);

	DECLARE_ENUM(uint32, SecretShopWarpTime);
	DECLARE_PROPERTY_STRING(SecretShopList);

	DECLARE_FLAG(uint8, BossDebuffBlow);

	DECLARE_BOOL(AutoLoginEnabled);
	DECLARE_ENUM(uint32, AutoLoginRandomTime);
	DECLARE_ENUM(uint32, AutoLoginTime);

	DECLARE_ENUM(int32, GuildScoreCastleSiege);
	DECLARE_ENUM(int32, GuildScoreCastleSiegeMembers);
	DECLARE_ENUM(int32, GuildScoreArkaWar);

	DECLARE_BOOL(HelperBuffItemUseCheck);

	DECLARE_BOOL(HuntingRecordClear);

	DECLARE_BOOL(CommandGoldEnabled);
	DECLARE_ENUM(int32, CommandGoldMaxDays);
	DECLARE_ENUM(int32, CommandGoldPrice);

	DECLARE_RANGE(uint8, SealedBloodAngelExcellent);

	DECLARE_BOOL(WhisperLog);
	DECLARE_BOOL(AttackStunCheck);

	DECLARE_BOOL(DuplicatedCharacterCheck);

	DECLARE_PROPERTY_ARRAY(uint32, MixSpellStonePriceDivision, MAX_SPIRIT_STONE);
	DECLARE_PROPERTY_ARRAY(uint8, MixSpellStoneRate, MAX_SPIRIT_STONE);
	
	DECLARE_BOOL(TimeCheck);
	DECLARE_ENUM(uint32, TimeCheckTime);
	DECLARE_RANGE(int16, TimeCheckLevel);

	DECLARE_BOOL(ItemSplitEnabled);

	DECLARE_BOOL(SwampOfDarknessEnabled);
	DECLARE_ENUM(uint32, SwampOfDarknessBossAppearTime);
	DECLARE_ENUM(uint32, SwampOfDarknessBattleTime);
	DECLARE_RANGE(uint32, SwampOfDarknessClosedTime);
	DECLARE_ENUM(int32, SwampOfDarknessMaxPlayers);

	DECLARE_BOOL(ComboCheckEnabled);
	DECLARE_ENUM(uint32, ComboCheckTimeDiff);
	DECLARE_ENUM(int32, ComboCheckCount);
	DECLARE_BOOL(ComboCheckDatabaseLog);
	DECLARE_BOOL(ComboCheckStop);
	DECLARE_BOOL(ComboCheckKick);

	DECLARE_BOOL(SkillCheckEnabled);
	DECLARE_BOOL(LuckyItemMixCheck);
	DECLARE_BOOL(DebuffStopCheck);

	DECLARE_BOOL(LoginDiskSerialMacCheck);
	DECLARE_BOOL(EventKillRankingEnabled);
	DECLARE_BOOL(EventTimeRankingEnabled);

	DECLARE_BOOL(ElfSkillWeaponCheck);
	DECLARE_BOOL(PentagramErrtelFix);
	DECLARE_BOOL(StackExpirableItemsOnLoot);

	DECLARE_BOOL(BossStackCheck);

	DECLARE_BOOL(NumericBaseballEnabled);
	DECLARE_BOOL(NumericBaseballRequiredTicket);
	DECLARE_BOOL(NumericBaseballReward);
	DECLARE_ENUM(int32, NumericBaseballScoreStrike);
	DECLARE_ENUM(int32, NumericBaseballScoreBall);
	DECLARE_PROPERTY_ARRAY(int32, NumericBaseballScore, NUMERIC_BASEBALL_ROWS);

	DECLARE_BOOL(ChannelChangeEnabled);

	DECLARE_ENUM(uint8, SkillMagicArrowProjectiles);
	DECLARE_ENUM(int32, SkillMagicArrowSplashDamage);
	DECLARE_ENUM(int32, SkillMagicArrowSplashTargets);
	DECLARE_ENUM(int32, SkillMagicArrowSplashDistance);
	DECLARE_ENUM(uint8, SkillPlasmaBallMoveDistance);
	DECLARE_ENUM(uint8, SkillPlasmaBallAttackDistance);
	DECLARE_FLOAT(SkillPlasmaBallMoveSpeed);
	DECLARE_ENUM(int32, SkillPlasmaBallAttackCount);
	DECLARE_ENUM(uint8, SkillPlasmaBallAttackSpeed);
	DECLARE_ENUM(uint8, SkillPlasmaBallTargetCount);

	DECLARE_ENUM(int32, SkillBurstRWEffect1Div);
	DECLARE_ENUM(int32, SkillBurstRWEffect2Div);
	DECLARE_ENUM(int32, SkillBurstRWDurationAdd);
	DECLARE_ENUM(int32, SkillBurstRWDurationDiv);

	DECLARE_ENUM(int32, SkillHasteRWEffect1Div);
	DECLARE_ENUM(int32, SkillHasteRWEffect2Div);
	DECLARE_ENUM(int32, SkillHasteRWDurationAdd);
	DECLARE_ENUM(int32, SkillHasteRWDurationDiv);

	DECLARE_ENUM(uint8, SkillDarkPlasmaAttackDistance);
	DECLARE_ENUM(uint8, SkillDarkPlasmaAttackSpeed);
	DECLARE_ENUM(uint8, SkillDarkPlasmaTargetCount);

	DECLARE_BOOL(AntiMacroEnabled);
	DECLARE_ENUM(uint32, AntiMacroTime);
	DECLARE_BOOL(AntiMacroComboEnabled);
	DECLARE_ENUM(uint32, AntiMacroComboTime);

	DECLARE_ENUM(int32, MultiAttackCount);
	DECLARE_ENUM(uint32, MultiAttackTime);
	DECLARE_BOOL(MultiAttackNew);

	DECLARE_BOOL(ShutdownCheck);

	DECLARE_BOOL(DungeonEnabled);
	DECLARE_ENUM(int32, DungeonMinParty);
	DECLARE_ENUM(int64, DungeonWaitTime);
	DECLARE_ENUM(uint32, DungeonDuration);
	DECLARE_ENUM(int32, DungeonSilverChestMonster);
	DECLARE_ENUM(int32, DungeonMaxPerPC);
	DECLARE_ENUM(uint16, DungeonEndGate);
	DECLARE_ENUM(int32, DungeonGAP);
	DECLARE_ENUM(uint32, DungeonSimpleTrapTime);
	DECLARE_ENUM(int32, DungeonSimpleTrapDamage);

	DECLARE_BOOL(MixRecoveryEnabled);
	DECLARE_ENUM(int32, MixRecoveryPriceWC);
	DECLARE_ENUM(int32, MixRecoveryPriceBless);
	DECLARE_ENUM(int32, MixRecoveryTime);
	DECLARE_ENUM(int32, MixRecoveryFailTime);
	DECLARE_BOOL(MixRecoveryWarning);

	DECLARE_ENUM(int32, InventoryMountTime);

	DECLARE_BOOL(QuestMUEnabled);

	DECLARE_PROPERTY_STRING(GuardianDisassembleItemBag);

	DECLARE_ENUM(int32, MonsterSoulPurchaseAmount);
	DECLARE_ENUM(int32, MonsterSoulDurationTime);

	DECLARE_ENUM(int32, SkillDetectionRange);

	DECLARE_ENUM(uint32, ConnectTimeKick);

	DECLARE_PROPERTY_ARRAY(uint8, ItemUpgradeSuccessRate, 6);

	DECLARE_ENUM(uint32, CustomBossSummonTime);
	DECLARE_ENUM(int32, CustomBossSummonCount);
	DECLARE_ENUM(uint32, CustomBossSummonDespawnTime);

	DECLARE_ENUM(int32, SpecialMapCharacterCount);
	DECLARE_BOOL(SpecialMapPVP);

	DECLARE_BOOL(LogDamagePVP);
	DECLARE_BOOL(LogDamagePVPToDB);

	DECLARE_BOOL(ErrtelToRecovery);
	DECLARE_ENUM(int32, ErrtelRemoveFailCount);
	DECLARE_ENUM(int32, ErrtelMixFailCount);

	DECLARE_ENUM(int32, AntiHackSkillLogCount);
	DECLARE_ENUM(int32, AntiHackSkillCount);
	DECLARE_BOOL(AntiHackSkillKick);

	DECLARE_ENUM(int32, AntiHackSkillCountContinue);
	DECLARE_BOOL(AntiHackSkillContinue);

	DECLARE_BOOL(MajesticSkillDamageNormal);
	DECLARE_BOOL(MajesticAddDamageNormal);

	DECLARE_ENUM(uint32, SkillSleepReuseTime);

	DECLARE_BOOL(OnlyBaseStatForEquipment);

	DECLARE_ENUM(int32, AttackRangeTolerance);

	DECLARE_ENUM(int32, SkillReflectLimit);

	DECLARE_BOOL(TeleportFix);
	DECLARE_BOOL(GLElementalDamageFix);

	TeleportBuffList m_TeleportBuffRestrict;
	TeleportBuffList m_TeleportBuffCoordinate;

	DECLARE_ENUM(uint8, RareIceDragonRate);

	DECLARE_BOOL(AttackTimeNullKick);

	DECLARE_ENUM(int32, PlayerWalkSpeed);

	bool IsTestEnabled() const
	{
		return this->IsTest() && this->GetTestKey() == 2018;
	}
};

#define sGameServer GameServer::instance()

#endif