/*
*
* Copyright (C) 2014 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerDef.h"
*
*/
#ifndef PLAYER_DEF_H
#define PLAYER_DEF_H

#define FRIEND_MAX						50

#define ITEM_DB_DATA_ID					0
#define ITEM_DB_DATA_SLOT				2
#define ITEM_DB_DATA_ENTRY				3

#define MAIL_MAX						150
#define NPC_INTERACT_MAX_DISTANCE		5

#define FRIEND_LOOP(loop) for ( int32 loop = 0; loop < FRIEND_MAX; ++loop )
#define MAIL_LOOP(loop) for ( int32 loop = 0; loop < MAIL_MAX; ++loop )

#define KICK_PLAYER(player, reason) if ( player ) player->kick(reason, __FUNCTION__);

#define MAX_PERSONAL_SHOP_MONEY 999999999

#define MoveItem(to, from) MoveItemTo##to##From##from(uint8 source, uint8 source_slot, uint8 target, uint8 target_slot, uint8 * item_info)
#define CallMoveItem(to, from) MoveItemTo##to##From##from(lpMsg->source, lpMsg->source_slot, lpMsg->target, lpMsg->target_slot, lpMsg->source_item_info)
#define MoveItemSuccess(to, from) \
	this->Get##from##()->DeleteItem(source_slot); \
	this->Get##to##()->ItemSet(target_slot, 1); \
	this->Get##to##()->GetItem(target_slot)->ConvertToBuffer(item_info);

enum CloseSocketTimes
{
	CLOSE_SOCKET_TIME_CONNECT,
	CLOSE_SOCKET_TIME_SERVER_QUIT,
	CLOSE_SOCKET_TIME_GAME_CLOSE,

	CLOSE_SOCKET_TIME_COUNT
};

enum
{
	ALE_SPEED,
	ALE_DAMAGE,
	ALE_MAX,
};

namespace PlayerAction
{
	enum
	{
		PLAYER_ACTION_TRADE,
		PLAYER_ACTION_TRADE_ANSWER,
		PLAYER_ACTION_PARTY,
		PLAYER_ACTION_DUEL,
		PLAYER_ACTION_ITEM_DROP,
		PLAYER_ACTION_ITEM_LOOT,
		PLAYER_ACTION_ITEM_USE,
		PLAYER_ACTION_ITEM_MOVE,
		PLAYER_ACTION_ITEM_BUY_NPC,
		PLAYER_ACTION_ITEM_SELL_NPC,
		PLAYER_ACTION_OPEN_PERSONAL_STORE,
		PLAYER_ACTION_BUY_PERSONAL_STORE,
		PLAYER_ACTION_TELEPORT,
		PLAYER_ACTION_GAIN_EXPERIENCE,
		PLAYER_ACTION_NPC_INTERACTION,
		PLAYER_ACTION_ATTACK_REQUEST,
		PLAYER_ACTION_MAGIC_REQUEST,
		PLAYER_ACTION_CANCEL_MAGIC,
		PLAYER_ACTION_WHISPER,
		PLAYER_ACTION_SPEAK,
		PLAYER_ACTION_CASH_SHOP_USE,
		PLAYER_ACTION_GUILD_JOIN,
		PLAYER_ACTION_RESET,
		PLAYER_ACTION_GUILD_CREATE,
		PLAYER_ACTION_POST,
		PLAYER_ACTION_GEN_CHAT,
		PLAYER_ACTION_PVP,
		PLAYER_ACTION_MAX
	};

	static const std::string Name[PLAYER_ACTION_MAX] =
	{
		"Trade",
		"Trade",
		"Party",
		"Duel",
		"Item Drop",
		"Item Loot",
		"Item Use",
		"Item Move",
		"Item Buy Shop",
		"Item Sell Shop",
		"Open Personal Shop",
		"Item Buy Personal Shop",
		"Teleport",
		"Gain Exp",
		"Npc Interaction",
		"Normal Attack",
		"Magic Attack",
		"Cancel Magic",
		"Whisper",
		"Speak",
		"Use Cash Shop",
		"Guild Join",
		"Reset",
		"Guild Create",
		"Post",
		"Gen Chat",
		"PVP"
	};
};

struct RestrictionData
{
	DECLARE_ENUM(time_t, Time);
	DECLARE_PROPERTY_STRING(Admin);
	DECLARE_PROPERTY_STRING(Reason);

	bool IsActive() const { return time(nullptr) < this->GetTime(); }

	void Reset()
	{
		this->SetTime(0);
		this->ResetAdmin();
		this->ResetReason();
	}
};

enum
{
	STAT_FRUIT_ENERGY,
	STAT_FRUIT_VITALITY,
	STAT_FRUIT_AGILITY,
	STAT_FRUIT_STRENGTH,
	STAT_FRUIT_LEADERSHIP,
	STAT_FRUIT_MAX
};

enum
{
	STAT_FRUIT_RESULT_ADDED					= 0,
	STAT_FRUIT_RESULT_ADD_FAILED			= 1,
	STAT_FRUIT_RESULT_ADD_ANYMORE			= 2,
	STAT_FRUIT_RESULT_DECREASED				= 3,
	STAT_FRUIT_RESULT_DECREASE_FAILED		= 4,
	STAT_FRUIT_RESULT_DECREASE_ANYMORE		= 5,
	STAT_FRUIT_RESULT_EX_DECREASE_SUCCESS	= 6,
	STAT_FRUIT_RESULT_DECREASE_EQUIPMENT	= 16,

	STAT_FRUIT_RESULT_POINT_AT_MAXIMUM		= 33,
	STAT_FRUIT_RESULT_POINT_AT_MAXIMUM_2	= 37,

	STAT_FRUIT_RESULT_DECREASE_DEFAULT_STAT	= 38,

	/*
	0 = Added
	1 = Fruit stat add failed.
	2 = Create / This stat cannot be %s anymore. => en %s va Create
	3 = Decreased
	4 = Fruit stat decrease failed.
	5 = Decrease / This stat cannot be %s anymore. => en %s va Decrease
	6 = XShop Decrease Success
	7 = Fruit stat decrease failed.
	8 = Decrease / This stat cannot be %s anymore. => en %s va Decrease
	16 = To decrease the fruit, weapons, armors and others must be removed.
	17 = No text... Solamente agrega stats
	18 = No text... Solamente resta stats
	33 = Impossible since the usable fruit points are at maximum.
	37 = Impossible since the usable fruit points are at maximum.
	38 = Cannot be decreased under the default stat value.
	*/
};

enum PKStatus
{
	PK_STATUS_HERO,
	PK_STATUS_HERO_LVL_2,
	PK_STATUS_HERO_LVL_1,
	PK_STATUS_COMMONER,
	PK_STATUS_MURDER_LVL_1,
	PK_STATUS_MURDER_LVL_2,
	PK_STATUS_MURDER_LVL_3,
	PK_STATUS_MAX
};

enum PlayerLoginQueryIndex
{
	PLAYER_LOGIN_QUERY_LOAD,
	PLAYER_LOGIN_QUERY_LOAD_ITEM,
	PLAYER_LOGIN_QUERY_LOAD_SKILL,
	PLAYER_LOGIN_QUERY_LOAD_GAME_OPTION,
	PLAYER_LOGIN_QUERY_LOAD_QUEST_EVO,
	PLAYER_LOGIN_QUERY_LOAD_FRIEND_LIST,
	PLAYER_LOGIN_QUERY_LOAD_MAIL_LIST,
	PLAYER_LOGIN_QUERY_LOAD_GUILD,
	PLAYER_LOGIN_QUERY_LOAD_GEN,
	PLAYER_LOGIN_QUERY_LOAD_GEN_KILL,
	PLAYER_LOGIN_QUERY_LOAD_HELPER,
	PLAYER_LOGIN_QUERY_LOAD_BUFF,
	PLAYER_LOGIN_QUERY_LOAD_BLOCK_LIST,
	PLAYER_LOGIN_QUERY_LOAD_RESTRICTION,
	PLAYER_LOGIN_QUERY_LOAD_STATISTICS,
	PLAYER_LOGIN_QUERY_LOAD_ITEM_DELAY,
	PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_BUFF,
	PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_WAREHOUSE_DATA,
	PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_WAREHOUSE_ITEMS,
	PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_CASH_SHOP_LIST,
	PLAYER_LOGIN_QUERY_LOAD_ACCOUNT_CREDITS,
	PLAYER_LOGIN_QUERY_LOAD_MUUN,
	PLAYER_LOGIN_QUERY_LOAD_EVENT_INVENTORY,
	PLAYER_LOGIN_QUERY_LOAD_PENTAGRAM,
	PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_CHARACTER,
	PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_ACCOUNT,
	PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_MOBILE,
	PLAYER_LOGIN_QUERY_LOAD_GREMORY_CASE_PERSONAL_STORE,
	PLAYER_LOGIN_QUERY_LOAD_PERSONAL_STORE,
	PLAYER_LOGIN_QUERY_LOAD_CHAT_BLOCK,

	PLAYER_LOGIN_QUERY_LOAD_ITEM_RECOVERY,
	PLAYER_LOGIN_QUERY_LOAD_ITEM_SELL,

	PLAYER_LOGIN_QUERY_LOAD_HUNTING_RECORD,
	PLAYER_LOGIN_QUERY_LOAD_LABYRINTH,
	PLAYER_LOGIN_QUERY_LOAD_LABYRINTH_ZONE,
	PLAYER_LOGIN_QUERY_LOAD_LABYRINTH_LEVEL,
	PLAYER_LOGIN_QUERY_LOAD_EVENT_COUNT,
	PLAYER_LOGIN_QUERY_LOAD_MINI_BOMB,
	PLAYER_LOGIN_QUERY_LOAD_MINI_BOMB_GRID,
	PLAYER_LOGIN_QUERY_LOAD_MU_ROOMY,
	PLAYER_LOGIN_QUERY_LOAD_MU_ROOMY_DECK,
	PLAYER_LOGIN_QUERY_LOAD_WARP_FAVORITE_LIST,
	PLAYER_LOGIN_QUERY_LOAD_GUIDED_QUEST,

	PLAYER_LOGIN_QUERY_LOAD_JEWEL_BINGO,
	PLAYER_LOGIN_QUERY_LOAD_JEWEL_BINGO_GRID,

	PLAYER_LOGIN_QUERY_LOAD_DATE,

	PLAYER_LOGIN_QUERY_LOAD_MAJESTIC_TREE,
	PLAYER_LOGIN_QUERY_LOAD_MAJESTIC_STATS,

	PLAYER_LOGIN_QUERY_LOAD_NUMERIC_BASEBALL,
	PLAYER_LOGIN_QUERY_LOAD_RECOVERY_MIX,
	PLAYER_LOGIN_QUERY_LOAD_QUEST_MU,
	PLAYER_LOGIN_QUERY_LOAD_MONSTER_SOUL,

    MAX_PLAYER_LOGIN_QUERY
};

enum CharacterListQueryIndex
{
	CHARACTER_LIST_QUERY_ACCOUNT_DATA,
	CHARACTER_LIST_QUERY_CHARACTER_DATA,
	CHARACTER_LIST_QUERY_CHARACTER_ITEM_DATA,
	CHARACTER_LIST_QUERY_MAX
};

enum PlayerGuildQueryIndex
{
	PLAYER_GUILD_QUERY_DATA,
	PLAYER_GUILD_QUERY_ITEM,
	PLAYER_GUILD_QUERY_MAX
};

enum FriendAddResults
{
	FRIEND_ADD_NOT_FOUNDED,
	FRIEND_ADDED,
	FRIEND_ADD_UNK,
	FRIEND_ADD_FULL,
	FRIEND_ADD_ALREADY,
	FRIEND_ADD_SELF,
	FRIEND_ADD_LOWLVL
};

enum FriendMailResults
{
	FRIEND_MAIL_SEND_ERROR,
	FRIEND_MAIL_SEND_SUCCESS,
	FRIEND_MAIL_SEND_TARGET_FULL_BOX,
	FRIEND_MAIL_SEND_TARGET_NOT_EXIST,
	FRIEND_MAIL_SEND_SELF,
	FRIEND_MAIL_SEND_UNK,
	FRIEND_MAIL_SEND_TARGET_LOWLVL,
	FRIEND_MAIL_SEND_SHORT_ZEN
};

enum
{
	FRIEND_STATUS_NONE,
	FRIEND_STATUS_STANDBY,
	FRIEND_STATUS_ACCEPT,
	FRIEND_STATUS_REJECT,
	FRIEND_STATUS_MAX
};

enum
{
	FRIEND_MAIL_STATE_NEW,
	FRIEND_MAIL_STATE_CLOSED,
	FRIEND_MAIL_STATE_OPEN,
	MAX_FRIEND_MAIL_STATE
};

enum
{
	PARTY_REQUEST_FAILED,			// Result = 0 -> Creating party has failed
	PARTY_REQUEST_CANCELED,			// Result = 1 -> Your request has been denied
	PARTY_REQUEST_FULL,				// Result = 2 -> Party is full
	PARTY_REQUEST_TARGET_OFF,		// Result = 3 -> The user has left the game
	PARTY_REQUEST_TARGET_IN_PARTY,	// Result = 4 -> The user is already in other party
	PARTY_REQUEST_LEFT,				// Result = 5 -> You have just left the party
	PARTY_REQUEST_DIF_GENS,			// Result = 6 -> You cannot make a party with a different Gens
	PARTY_REQUEST_BATTLE_ZONE,		// Result = 7 -> You cannot form party in a battle zone
	PARTY_REQUEST_BATTLE_ZONE_2		// Result = 8 -> Partys are not activated in battle zone
};

enum TradeResultType
{
	TRADE_RESULT_CANCEL,
	TRADE_OK,
	TRADE_INV_OR_ZEN_FULL,
	TRADE_OVER_TIME_AUTO_CANCEL,
	TRADE_NOT_ALLOWED_ITEM
};

enum GuildCreateResult
{
	GUILD_CREATE_NULL,
	GUILD_CREATE_SUCCESS,
	GUILD_CREATE_ERROR,
	GUILD_CREATE_ALREADY_EXIST,
};

enum CharacterFlag
{
	CHARACTER_FLAG_REQUEST			= 1 << 0,
	CHARACTER_FLAG_POST				= 1 << 1,
};

enum TransactionType
{
	TRANSACTION_TYPE_NONE,
	TRANSACTION_TYPE_BEGIN,
	TRANSACTION_TYPE_COMMIT,
	TRANSACTION_TYPE_ROLLBACK
};

// Interface
namespace InterfaceData
{
	//enum InterfaceType
	//{
		static const std::string None					= "interface_none";
		static const std::string Shop					= "interface_shop";
		static const std::string Warehouse				= "interface_warehouse";
		static const std::string ChaosMachine			= "interface_chaos_machine";
		static const std::string BloodCastleEntrance	= "interface_blood_castle_entrance";
		static const std::string DevilSquareEntrance	= "interface_devil_square_entrance";
		static const std::string Party					= "interface_party";
		static const std::string Trade					= "interface_trade";
		static const std::string Guild					= "interface_guild";
		static const std::string GuildRequest			= "interface_guild_request";
		static const std::string GuildAlliance			= "interface_guild_alliance";
		static const std::string Duel					= "interface_duel";
		static const std::string DuelGate				= "interface_duel_gate";
		static const std::string PetTrainer				= "interface_pet_trainer";
		static const std::string QuestSupportGiver		= "interface_quest_support_giver";
		static const std::string QuestEvolutionGiver	= "interface_quest_evolution_giver";
		static const std::string MixJewel				= "interface_mix_jewel";
		static const std::string CashShop				= "interface_cash_shop";
		static const std::string HarmonyRefine			= "interface_harmony_refine";
		static const std::string HarmonySmelt			= "interface_harmony_smelt";
		static const std::string HarmonyRestore			= "interface_harmony_restore";
		static const std::string Lucky					= "interface_lucky";
		static const std::string SeedCreation			= "interface_seed_creation";
		static const std::string SeedResearch			= "interface_seed_research";
		static const std::string ChaosCard				= "interface_chaos_card";
		static const std::string CastleSiegeLever		= "interface_castle_siege_lever";
		static const std::string LordMix				= "interface_lord_mix";
		static const std::string LandOfTrialsGuard		= "interface_land_of_trials_guard";
		static const std::string CastleGuard			= "interface_castle_guard";
		static const std::string CastleMachine			= "interface_castle_siege_machine";
		static const std::string PersonalStoreOpen		= "interface_personal_store_open";
		static const std::string IllusionTempleEntrance	= "interface_illusion_temple_entrance";
		static const std::string GuildWarRequest		= "interface_guild_war_request";
		static const std::string ArcaBattleLesnar		= "interface_arca_battle_lesnar";
		static const std::string ArkaWarSignRegister	= "interface_arka_war_sign_register";
		static const std::string ArkaWarExchange		= "interface_arka_war_exchange";
		static const std::string Adniel					= "interface_adniel";
		static const std::string Mining					= "interface_mining";
		static const std::string ChaosCastleSurvivalUI	= "interface_chaos_castle_mait";
		static const std::string AcheronEntrance		= "interface_acheron_entrance";
		static const std::string CentTown				= "interface_cent_town";
		static const std::string CentEntrance			= "interface_cent_entrance";
		static const std::string MossMerchant			= "interface_moss_merchant";
		static const std::string MixRecovery			= "interface_mix_recovery";
	//};
}

struct GameOption
{
	DECLARE_ENUM(uint8, Option);
	DECLARE_ENUM(uint8, ChatWindow);
	DECLARE_ENUM(uint8, KeyQ);
	DECLARE_ENUM(uint8, KeyW);
	DECLARE_ENUM(uint8, KeyE);
	DECLARE_ENUM(uint8, KeyR);
	DECLARE_ENUM(int32, QWERLevel);
	DECLARE_PROPERTY_ARRAY(uint16, SkillKey, SKILL_KEY_COUNT);
	DECLARE_ENUM(uint8, ChangeSkin);
	DECLARE_FLAG(uint64, ExtraFlag);
	DECLARE_ENUM(uint16, AdditionalOptions);
	DECLARE_PROPERTY_ARRAY(uint8, ButtonLink, BUTTON_KEY_COUNT);

	void Reset()
	{
		this->SetOption(0);
		this->SetChatWindow(0);
		this->SetKeyQ(0);
		this->SetKeyW(0);
		this->SetKeyE(0);
		this->SetKeyR(0);
		this->SetQWERLevel(0);

		this->ResetSkillKey(-1);
		this->SetChangeSkin(1);
		this->SetExtraFlag(0);
		this->SetAdditionalOptions(0);
		this->ResetButtonLink(0);
	}
};

struct QuestEvolutionStatus
{
	QuestEvolutionStatus()
	{
		this->SetID(0);
		this->ResetMonsterCount(0);
		this->SetState(QUEST_EVOLUTION_STATE_NONE);
	}
	uint8 GetStateByte() const
	{
		return (this->GetState() & QUEST_EVOLUTION_STATE_CLEAR) << ((this->GetID() % 4) * 2);
	}

	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint8, State);
	DECLARE_PROPERTY_ARRAY(int32, MonsterCount, QUEST_EVOLUTION_MONSTER_COUNT);
};

enum PersonalStoreOffState
{
	PERSONAL_STORE_OFF_STATE_NONE,
	PERSONAL_STORE_OFF_STATE_CONFIRM,
	PERSONAL_STORE_OFF_STATE_OK,
};

struct InterfaceState
{
	void Set(std::string const& id, Unit* Target, uint8 state = 0, uint32 interval = 0)
	{
		this->SetID(id);
		this->SetTarget(Target);
		this->SetState(state);

		if ( interval )
			this->SetTime(GetTickCount() + interval);
		else
			this->SetTime(0);

		this->SetType(0);
	}
	void Reset()
	{
		this->SetID(InterfaceData::None);
		this->SetState(0);
		this->SetTarget(nullptr);
		this->SetTime(0);
		this->SetType(0);
	}

	const char * GetName() const
	{
		return GetID().c_str();
	}
	bool IsCommonMix()
	{
		return this->GetID() == InterfaceData::ChaosMachine ||
			   this->GetID() == InterfaceData::HarmonyRefine ||
			   this->GetID() == InterfaceData::HarmonySmelt ||
			   this->GetID() == InterfaceData::HarmonyRestore ||
			   this->GetID() == InterfaceData::SeedCreation ||
			   this->GetID() == InterfaceData::SeedResearch ||
			   this->GetID() == InterfaceData::ChaosCard ||
			   this->GetID() == InterfaceData::Lucky ||
			   this->GetID() == InterfaceData::LordMix ||
			   this->GetID() == InterfaceData::ArkaWarExchange ||
			   this->GetID() == InterfaceData::ArkaWarSignRegister;
	}
	
	DECLARE_PROPERTY_STRING(ID);
	DECLARE_ENUM(uint8, State);
	DECLARE_PROPERTY_PTR(Unit, Target);
	DECLARE_ENUM(uint8, Type);
	
	DECLARE_ENUM(uint32, Time);
};

struct FriendData
{
	void Reset()
	{
		this->ResetName();
		this->SetServer(-1);
	}

	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(uint16, Server);
};

typedef std::list<FriendData*> FriendList;

struct MailData
{
	MailData()
	{
		this->Reset();
	}

	void Reset()
	{
		this->ResetFrom();
		this->ResetSubject();
		this->ResetText();
		this->SetDirection(0);
		this->SetAction(0);
		this->ResetPhoto(0);
		this->SetWindowGUID(0);
		this->SetDate(0);
		this->SetStatus(0);
		this->SetID(0);
		this->SetInUse(false);
	}

	DECLARE_STRING_FIXED(From, (MAX_CHARACTER_LENGTH + 1));
	DECLARE_STRING_FIXED(Subject, (MAX_MAIL_SUBJECT_LENGTH + 1));
	DECLARE_STRING_FIXED(Text, (MAIL_MESSAGE_LENGTH + 1));
	DECLARE_ENUM(uint8, Direction);
	DECLARE_ENUM(uint8, Action);
	DECLARE_PROPERTY_ARRAY(uint8, Photo, MAX_PREVIEW_DATA);
	DECLARE_ENUM(uint32, WindowGUID);
	DECLARE_ENUM(time_t, Date);
	DECLARE_ENUM(uint8, Status);
	DECLARE_ENUM(uint32, ID);
	DECLARE_BOOL(InUse);
};

typedef std::list<uint32> MailDataList;

enum AdminFlag
{
	ADMIN_FLAG_NONE			= 0,
	ADMIN_FLAG_PARTY		= 1 << 0,
	ADMIN_FLAG_TRADE		= 1 << 1,
	ADMIN_FLAG_GLOBAL		= 1 << 2,
	ADMIN_FLAG_TRACE		= 1 << 3,
	ADMIN_FLAG_KICK			= 1 << 4,
	ADMIN_FLAG_EVENTS		= 1 << 5,
	ADMIN_FLAG_MUTE			= 1 << 6,
	ADMIN_FLAG_ITEM			= 1 << 7,
	ADMIN_FLAG_ITEM_EVENT	= 1 << 8,
	ADMIN_FLAG_BAN			= 1 << 9,
	ADMIN_FLAG_POST			= 1 << 10,
	ADMIN_FLAG_TALK			= 1 << 11,
	ADMIN_FLAG_MOVE			= 1 << 12,
	ADMIN_FLAG_RELOAD		= 1 << 13,
	ADMIN_FLAG_MONSTER		= 1 << 14,
	ADMIN_FLAG_ZEN			= 1 << 15,
	ADMIN_FLAG_GUILD_TALK	= 1 << 16,
	ADMIN_FLAG_RESTRICTION	= 1 << 17,
	ADMIN_FLAG_SHUTDOWN		= 1 << 18,
	ADMIN_FLAG_PANEL		= 1 << 19,
	ADMIN_FLAG_BUFF			= 1 << 20,
	ADMIN_FLAG_MASIVE		= 1 << 21,

	ADMIN_FLAG_ALL			= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7 | 1 << 8 |
							  1 << 9 | 1 << 10| 1 << 11| 1 << 12| 1 << 13| 1 << 14| 1 << 15| 1 << 16| 1 << 17|
							  1 << 18| 1 << 19| 1 << 20 | 1 << 21
};

/*
ADMIN_FLAG_PARTY		= 1,
ADMIN_FLAG_TRADE		= 2,
ADMIN_FLAG_GLOBAL		= 4,
ADMIN_FLAG_TRACE		= 8,
ADMIN_FLAG_KICK			= 16,
ADMIN_FLAG_EVENTS		= 32,
ADMIN_FLAG_MUTE			= 64,
ADMIN_FLAG_ITEM			= 128,
ADMIN_FLAG_ITEM_EVENT	= 256,
ADMIN_FLAG_BAN			= 512,
ADMIN_FLAG_POST			= 1024,
ADMIN_FLAG_TALK			= 2048,
ADMIN_FLAG_MOVE			= 4096,
ADMIN_FLAG_RELOAD		= 8192,
ADMIN_FLAG_MONSTER		= 16384,
ADMIN_FLAG_ZEN			= 32768,
ADMIN_FLAG_GUILD_TALK	= 65536,
ADMIN_FLAG_RESTRICTION	= 131072,
ADMIN_FLAG_SHUTDOWN		= 262144,
ADMIN_FLAG_PANEL		= 524288,
ADMIN_FLAG_BUFF			= 1048576,
ADMIN_FLAG_MASIVE		= 2097152,
*/

#define DARK_SPIRIT_ATTACK_DISTANCE			7

enum DarkSpiritAttack
{
	DARK_SPIRIT_ATTACK_NORMAL,
	DARK_SPIRIT_ATTACK_DIVE,
	DARK_SPIRIT_ATTACK_MAX
};

enum DarkSpiritMode
{
	DARK_SPIRIT_MODE_NORMAL,
	DARK_SPIRIT_MODE_RANDOM,
	DARK_SPIRIT_MODE_WITH_MASTER,
	DARK_SPIRIT_MODE_TARGET,
	DARK_SPIRIT_MODE_MAX
};

struct DarkSpirit
{
	DECLARE_ENUM(DarkSpiritMode, Mode);

	DECLARE_PROPERTY(int32, AttackSpeed);
	DECLARE_PROPERTY(int32, AttackDamageMin);
	DECLARE_PROPERTY(int32, AttackDamageMax);
	DECLARE_ENUM(int32, AttackSuccessRate);
	DECLARE_PROPERTY(int32, CriticalDamageRate);
	DECLARE_PROPERTY(int32, ExcellentDamageRate);
	DECLARE_PROPERTY(int32, DoubleDamageRate);
	DECLARE_PROPERTY(int32, IgnoreDefenseRate);
	DECLARE_ENUM(uint32, AttackTick);
	
	DECLARE_PTR(Unit, Target);

	void Reset()
	{
		this->SetMode(DARK_SPIRIT_MODE_NORMAL);
		this->SetAttackSpeed(0);
		this->SetAttackDamageMin(0);
		this->SetAttackDamageMax(0);
		this->SetAttackSuccessRate(0);
		this->SetCriticalDamageRate(0);
		this->SetExcellentDamageRate(0);
		this->SetDoubleDamageRate(0);
		this->SetIgnoreDefenseRate(0);
		this->SetTarget(nullptr);
		this->SetAttackTick(GetTickCount());
	}
};

struct SelfDefenseData
{
	DECLARE_PROPERTY_PTR(Player, Player);
	DECLARE_ENUM(uint32, AttackTime);

	explicit SelfDefenseData(Player* pPlayer)
	{
		this->SetPlayer(pPlayer);
		this->SetAttackTime(GetTickCount());
	}
};

typedef std::unordered_map<uint32, SelfDefenseData*> SelfDefenseDataMap;

enum StatisticsTypes
{
	STATISTIC_KILLED_MONSTERS						= 0,
	STATISTIC_KILLED_PLAYERS						= 1,
	STATISTIC_COMPLETED_QUESTS						= 2,
	STATISTIC_DUEL_WINNED							= 3,
	STATISTIC_DUEL_LOOSED							= 4,
	//STATISTIC_TOTAL_HOURS_PLAYING					= 5,
	//STATISTIC_LIFE_POTIONS							= 6,
	//STATISTIC_MANA_POTIONS							= 7,
	//STATISTIC_SHIELD_POTIONS						= 8,
	//STATISTIC_ANTIDOTE_POTIONS						= 9,
	//STATISTIC_USED_JEWEL_OF_BLESS					= 10,
	//STATISTIC_USED_JEWEL_OF_SOUL					= 11,
	//STATISTIC_USED_JEWEL_OF_LIFE					= 12,
	//STATISTIC_USED_JEWEL_OF_HARMONY					= 13,
	//STATISTIC_USED_REFINE_STONE						= 14,
	//STATISTIC_USED_CREDIT_COIN						= 15,
	//STATISTIC_USED_ZEN_COIN							= 16,
	STATISTIC_BLOOD_CASTLE_POINTS					= 18,
	STATISTIC_DEVIL_SQUARE_POINTS					= 19,
	STATISTIC_CHAOS_CASTLE_POINTS					= 20,
	STATISTIC_ILLUSION_TEMPLE_POINTS				= 21,
	STATISTIC_DOUBLE_GOER_POINTS					= 22,
	STATISTIC_IMPERIAL_FORTRESS_POINTS				= 23,
	//STATISTIC_USED_JEWEL_OF_EXTENSION				= 24,
	//STATISTIC_USED_JEWEL_OF_ELEVATION				= 25,
	//STATISTIC_USED_JEWEL_OF_DEATH					= 26,
	//STATISTIC_USED_GOBLIN_POINT_COIN				= 27,
	//STATISTIC_USED_EVENT_POINT_COIN					= 28,
	STATISTIC_CRYWOLF_POINTS						= 29,
	STATISTIC_RAKLION_POINTS						= 30,
	//STATISTIC_SCRAMBLE_POINTS						= 31,
	//STATISTIC_DUNGEON_RACE_POINTS					= 32,
	//STATISTIC_LOSTTOWER_RACE_POINTS				= 33,
	STATISTIC_CASTLE_SIEGE_KILLS					= 34,
	STATISTIC_PK_KILLED								= 35,
	STATISTIC_EVOMON_SCORE							= 36,
	STATISTIC_WORLD_QUEST							= 37,
	STATISTIC_PVE_SCORE								= 38,
	STATISTIC_PVP_SCORE								= 39,
	STATISTIC_CHAOS_CASTLE_SURVIVAL_POINTS			= 40,
	STATISTIC_MINI_BOMB_SUCCESS						= 41,
	STATISTIC_MINI_BOMB_FAIL						= 42,
	STATISTIC_ARKA_WAR_KILLS						= 43,
	STATISTIC_ATTACK_TIME							= 44,
	STATISTIC_ATTACK_TIME_DATE						= 45,

	STATISTIC_MAX
};

#define STATISTIC_LOOP(var) for ( int32 var = 0; var < STATISTIC_MAX; ++var )

struct StatisticData
{
	void Reset()
	{
		this->SetCount(0);
	}

	DECLARE_PROPERTY(uint64, Count);
};

struct ItemDelayData
{
	ItemDelayData()
	{
		this->SetDate(0);
	}

	DECLARE_ENUM(time_t, Date);
};

typedef std::map<uint16, ItemDelayData> ItemDelayDataMap;

enum PlayerFieldData
{
	PLAYER_DATA_INCREASE_DAMAGE_TO_SD_RATE,
	PLAYER_DATA_DECREASE_DAMAGE_TO_SD_RATE,
	PLAYER_DATA_IGNORE_SD_RATE,

	PLAYER_DATA_MONSTER_DIE_GIVE_ZEN,
	PLAYER_DATA_MONSTER_DIE_HP,
	PLAYER_DATA_MONSTER_DIE_MP,
	PLAYER_DATA_MONSTER_DIE_SD,
	PLAYER_DATA_MONSTER_DIE_AG,
	PLAYER_DATA_SHIELD_REFILL_ON,

	PLAYER_DATA_HP_RECOVERY_ADD,
	PLAYER_DATA_MP_RECOVERY_ADD,
	PLAYER_DATA_AG_RECOVERY_ADD,
	PLAYER_DATA_SD_RECOVERY_ADD,

	PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_PVP,
	PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_PVM,

	PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_PVP,
	PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_PVM,

	PLAYER_DATA_INCREASE_DAMAGE_PVP,
	PLAYER_DATA_INCREASE_DAMAGE_PVM,

	PLAYER_DATA_INCREASE_MAX_DAMAGE_PVP,
	PLAYER_DATA_INCREASE_MAX_DAMAGE_PVM,

	PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVP,
	PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVM,

	PLAYER_DATA_INCREASE_DEFENSE_PVP,
	PLAYER_DATA_INCREASE_DEFENSE_PVM,

	PLAYER_DATA_FENRIR_DAMAGE,

	PLAYER_DATA_SHIELD_PROTECTION_VALUE,

	PLAYER_DATA_MUUN_SKILL_ATTACK,
	PLAYER_DATA_MUUN_SKILL_ATTACK_PVE,
	PLAYER_DATA_MUUN_SKILL_ATTACK_COUNT,
	PLAYER_DATA_MUUN_SKILL_ATTACK_DELAY,

	PLAYER_DATA_380_PVP_DAMAGE,

	PLAYER_DATA_SHIELD_DEFENSE_INCREASE,
	PLAYER_DATA_SHIELD_DEFENSE_INCREASE_PERCENT,

	PLAYER_DATA_ADD_EXPERIENCE_RATE,

	PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVP,
	PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVM,

	PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVP,
	PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVM,

	PLAYER_DATA_MAJESTIC_SKILL_DAMAGE,

	PLAYER_DATA_ELEMENTAL_IMPRINT,

	PLAYER_DATA_MAX
};

enum PlayerFieldFloat
{
	PLAYER_FLOAT_HP_RECOVERY_RATE,
	PLAYER_FLOAT_MP_RECOVERY_RATE,
	PLAYER_FLOAT_AG_RECOVERY_RATE,
	PLAYER_FLOAT_SD_RECOVERY_RATE,

	PLAYER_FLOAT_MONSTER_DIE_HP,
	PLAYER_FLOAT_MONSTER_DIE_MP,
	PLAYER_FLOAT_MONSTER_DIE_SD,
	PLAYER_FLOAT_MONSTER_DIE_AG,
	
	PLAYER_FLOAT_MANA_REDUCTION,

	PLAYER_DATA_OFFENSIVE_TOTAL_HP_RECOVERY,
	PLAYER_DATA_OFFENSIVE_TOTAL_MP_RECOVERY,
	PLAYER_DATA_OFFENSIVE_TOTAL_SD_RECOVERY,
	PLAYER_DATA_OFFENSIVE_TOTAL_AG_RECOVERY,

	PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_RATE_PVP,
	PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_RATE_PVM,

	PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_RATE_PVP,
	PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_RATE_PVM,

	PLAYER_DATA_DECREASE_REPAIR_FEE,

	PLAYER_DATA_SHIELD_PROTECTION,
	PLAYER_DATA_SHIELD_BLOCK,
	PLAYER_DATA_WEAPON_BLOCK,

	PLAYER_DATA_ABSORB_HP,
	PLAYER_DATA_ABSORB_SD,

	PLAYER_DATA_INCREASES_MP_CONSUMPTION,
	PLAYER_DATA_INCREASES_AG_CONSUMPTION,

	PLAYER_FLOAT_MAX
};

struct CashShopItemData
{
	void Reset()
	{
		this->SetProduct(0);
		this->SetOption(0);
		this->SetSerial(0);
		this->SetSerialCashShop(0);
		this->SetServer(0);
		this->SetDate(0);
	}

	DECLARE_ENUM(uint32, Product);
	DECLARE_ENUM(uint32, Option);
	DECLARE_ENUM(uint32, Serial);
	DECLARE_ENUM(uint32, SerialCashShop);
	DECLARE_ENUM(uint16, Server);
	DECLARE_ENUM(time_t, Date);
};

typedef std::vector<CashShopItemData*> CashShopItemDataList;

enum HelperFlags
{
	// Item Pick
	ITEM_PICK_JEWEL						= 0x08,
	ITEM_PICK_ANCIENT					= 0x10,
	ITEM_PICK_EXE						= 0x20,
	ITEM_PICK_ZEN						= 0x40,
	ITEM_PICK_SELECTED					= 0x80,

	OPT_AUTOPOT							= 0x00000001,
	OPT_AUTOHEAL						= 0x00000002, // Solamente ELF, Se necesita skill Heal
	OPT_DRAIN_LIFE						= 0x00000004, // Solamente SUM, Se necesita skill DrainLife
	OPT_LONG_DISTANCE_COUNTER_ATTACK	= 0x00000008,
	OPT_ORIGINAL_POSITION				= 0x00000010,
	OPT_COMBO							= 0x00000020, // Solamente DK, Se necesita tener los 3 skills
	OPT_PARTY							= 0x00000040, // ELF, DW
	OPT_REFERENCE_OF_PARTY_HEAL			= 0x00000080, // ELF
	OPT_BUFF_DURATION_FOR_ALL_PARTY		= 0x00000100, // ELF, DW
	OPT_USE_DARK_SPIRIT					= 0x00000200, // Solamente DL
	OPT_BUFF_DURATION					= 0x00000400,
	OPT_SKILL_2_DELAY					= 0x00000800,
	OPT_SKILL_2_CONDITION				= 0x00001000,
	OPT_SKILL_2_MONSTER_ATTACKING_ME	= 0x00002000,
	OPT_SKILL_2_MORE_THAN_3_MONSTERS	= 0x00004000,
	OPT_SKILL_2_MORE_THAN_4_MONSTERS	= 0x00008000,
	OPT_SKILL_2_MORE_THAN_5_MONSTERS	= OPT_SKILL_2_MORE_THAN_3_MONSTERS | OPT_SKILL_2_MORE_THAN_4_MONSTERS, // 0x40 | 0x80
	OPT_SKILL_3_DELAY					= 0x00010000,
	OPT_SKILL_3_CONDITION				= 0x00020000,
	OPT_SKILL_3_MONSTER_ATTACKING_ME	= 0x00040000,
	OPT_SKILL_3_MORE_THAN_3_MONSTERS	= 0x00080000,
	OPT_SKILL_3_MORE_THAN_4_MONSTERS	= 0x00100000,
	OPT_SKILL_3_MORE_THAN_5_MONSTERS	= OPT_SKILL_3_MORE_THAN_3_MONSTERS | OPT_SKILL_3_MORE_THAN_4_MONSTERS, // 0x08 | 0x10
	OPT_REPAIR_ITEMS					= 0x00200000,
	OPT_PICK_ALL_NEAR_ITEMS				= 0x00400000,
	OPT_PICK_SELECTED_ITEMS				= 0x00800000,
	OPT_DARK_SPIRIT_AUTO_ATTACK			= 0x01000000, // Solamente DL
	OPT_DARK_SPIRIT_ATTACK_TOGETHER		= 0x02000000, // Solamente DL
	OPT_AUTO_ACCEPT_PARTY				= 0x04000000,
	OPT_AUTO_ACCEPT_GUILD_MEMBER		= 0x08000000,
	OPT_USE_ELITE_MANA_POTION			= 0x10000000,
	OPT_USE_SKILL_CLOSELY				= 0x20000000,
	OPT_USE_REGULAR_ATTACK				= 0x40000000,
	OPT_PICK_EVENT_ITEMS				= 0x80000000,

	// El DarkLord no usa Skill 3, Usa Dark Spirit

	HELPER_OPTION_BUFF_ITEM				= 0x00000001,
	HELPER_OPTION_CONCENTRATED_MONSTERS	= 0x00000020,
	HELPER_OPTION_REPAIR_MUUN			= 0x00000040,
	HELPER_OPTION_REPAIR_GUARDIAN		= 0x00000100,
};

struct GenVictimData
{
	GenVictimData()
	{
		this->count.set(1);
		this->last_hit.set(time(nullptr));
	}

	DECLARE(uint32, count);
	DECLARE(int64, last_hit);
};

typedef std::map<uint32, GenVictimData> GenVictimDataMap;

enum AccountBlockedStatus
{
	ACCOUNT_BLOCKED_NONE,
	ACCOUNT_BLOCKED_SECURE_CODE,
	ACCOUNT_BLOCKED_BANNED,
	ACCOUNT_BLOCKED_ACCELERATOR,
};

enum SpecialCharacterFlag
{
	SPECIAL_CHARACTER_SUMMONER				= 1 << 0,
	SPECIAL_CHARACTER_DARK_LORD				= 1 << 1,
	SPECIAL_CHARACTER_MAGIC_GLADIATOR		= 1 << 2,
	SPECIAL_CHARACTER_RAGE_FIGHTER			= 1 << 3,
	SPECIAL_CHARACTER_GROW_LANCER			= 1 << 4,
};

#define MULTI_ATTACK_MAX_INFO		40
#define MULTI_ATTACK_SERIAL_SIZE	255

struct MultiAttackData
{
	DECLARE_ENUM(uint16, Index);
	DECLARE_PROPERTY_ARRAY(uint8, Serial, MULTI_ATTACK_SERIAL_SIZE);

	void Reset()
	{
		this->SetIndex(uint16(-1));
		this->ResetSerial(0);
	}
};

#define DURATION_MAGIC_KEY_MAX		60

struct DurationMagicKeyData
{
	bool IsValidDurationTime(uint8 magic_key)
	{
		if ( magic_key < DURATION_MAGIC_KEY_MAX && magic_key > 0 )
		{
			if ( (GetTickCount() - this->GetDurationTime(magic_key)) < 15000 )
				return true;
		}

		return false;
	}

	bool IsValidCount(uint8 magic_key)
	{
		if ( magic_key < DURATION_MAGIC_KEY_MAX && magic_key > 0 )
		{
			if ( this->GetCount(magic_key) >= 0 )
			{
				return true;
			}
		}

		return false;
	}

	void ApplyDurationTime(uint8 magic_key, uint32 current_tick)
	{
		if ( magic_key < DURATION_MAGIC_KEY_MAX && magic_key > 0 )
		{
			this->SetDurationTime(magic_key, current_tick);
			this->SetCount(magic_key, 7);
		}
	}

	uint32 GetValidDurationTime(uint8 magic_key)
	{
		if ( magic_key < DURATION_MAGIC_KEY_MAX && magic_key > 0 )
		{
			return this->GetDurationTime(magic_key);
		}

		return -77;
	}

	uint8 GetValidCount(uint8 magic_key)
	{
		if ( magic_key < DURATION_MAGIC_KEY_MAX && magic_key > 0 )
		{
			return this->GetCount(magic_key);
		}

		return -77;
	}

	void Reset()
	{
		this->ResetDurationTime(0);
		this->ResetCount(0);
	}

	DECLARE_PROPERTY_ARRAY(uint32, DurationTime, DURATION_MAGIC_KEY_MAX);
	DECLARE_PROPERTY_ARRAY(uint8, Count, DURATION_MAGIC_KEY_MAX);
};

struct LevelData
{
	DECLARE_PROPERTY(int16, Level);
	DECLARE_PROPERTY(int64, Experience);
	DECLARE_PROPERTY(int64, NextExperience);
	DECLARE_PROPERTY(int32, Points);

	void Reset()
	{
		this->SetLevel(0);
		this->SetExperience(0);
		this->SetNextExperience(0);
		this->SetPoints(0);
	}

	void Set(LevelData const* Data)
	{
		this->SetLevel(Data->GetLevel());
		this->SetExperience(Data->GetExperience());
		this->SetNextExperience(Data->GetNextExperience());
		this->SetPoints(Data->GetPoints());
	}
};

enum
{
	LEVEL_DATA_NORMAL,
	LEVEL_DATA_MASTER,
	LEVEL_DATA_MAJESTIC,
	LEVEL_DATA_MAX
};

enum CharacterHandleAction
{
	CHARACTER_HANDLE_ACTION_NONE,
	CHARACTER_HANDLE_ACTION_CREATE,
	CHARACTER_HANDLE_ACTION_DELETE,
	CHARACTER_HANDLE_ACTION_SELECT,
};

typedef std::map<uint32, uint8> CashShopOffersMap;

struct PentagramOption
{
	DECLARE_PROPERTY(int32, MulPentagramDamage);
	DECLARE_PROPERTY(int32, MulPentagramDefense);
	DECLARE_PROPERTY(int32, AddElementalCriticalDamageRate);
	DECLARE_PROPERTY(int32, AddElementalDefenseTransferRate);
	DECLARE_PROPERTY(int32, AddElementalAttackTransferRate);
	DECLARE_PROPERTY(int32, AddElementalAvoidanceRate);
	int32 AddElementalErrtelEffect[MAX_PENTAGRAM_JEWEL_TYPE][MAX_PENTAGRAM_JEWEL_RANK];
	
	void Reset()
	{
		this->SetMulPentagramDamage(0);
		this->SetMulPentagramDefense(0);
		this->SetAddElementalCriticalDamageRate(0);
		this->SetAddElementalDefenseTransferRate(0);
		this->SetAddElementalAttackTransferRate(0);
		this->SetAddElementalAvoidanceRate(0);

		for ( int32 i = 0; i < MAX_PENTAGRAM_JEWEL_TYPE; ++i )
		{
			for ( int32 n = 0; n < MAX_PENTAGRAM_JEWEL_RANK; ++n )
			{
				this->AddElementalErrtelEffect[i][n] = 0;
			}
		}
	}
};

struct PentagramJewelOption
{
	DECLARE_PROPERTY(int32, AddElementalDamage);
	DECLARE_PROPERTY(int32, AddElementalDamagePvP);
	DECLARE_PROPERTY(int32, AddElementalDamagePvM);
	DECLARE_PROPERTY(int32, MulElementalDamagePvP);
	DECLARE_PROPERTY(int32, MulElementalDamagePvM);
	DECLARE_PROPERTY(int32, ElementalDamageVsFire);
	DECLARE_PROPERTY(int32, ElementalDamageVsWater);
	DECLARE_PROPERTY(int32, ElementalDamageVsEarth);
	DECLARE_PROPERTY(int32, ElementalDamageVsWind);
	DECLARE_PROPERTY(int32, ElementalDamageVsDark);
	DECLARE_PROPERTY(int32, AddElementalDefense);
	DECLARE_PROPERTY(int32, AddElementalDefensePvP);
	DECLARE_PROPERTY(int32, AddElementalDefensePvM);
	DECLARE_PROPERTY(int32, ElementalDefenseVsFire);
	DECLARE_PROPERTY(int32, ElementalDefenseVsWater);
	DECLARE_PROPERTY(int32, ElementalDefenseVsEarth);
	DECLARE_PROPERTY(int32, ElementalDefenseVsWind);
	DECLARE_PROPERTY(int32, ElementalDefenseVsDark);
	DECLARE_PROPERTY(int32, AddElementalDamageReduction);
	DECLARE_PROPERTY(int32, AddElementalDamageReductionPvP);
	DECLARE_PROPERTY(int32, AddElementalDamageReductionPvM);
	DECLARE_PROPERTY(int32, AddElementalShieldAbsorbRate);
	DECLARE_PROPERTY(int32, AddElementalLifeAbsorbRate);
	DECLARE_PROPERTY(int32, ElementalBastionDuration);
	DECLARE_PROPERTY(int32, ElementalBleeding);
	DECLARE_PROPERTY(int32, ElementalParalysisRate);
	DECLARE_PROPERTY(int32, ElementalBindingRate);
	DECLARE_PROPERTY(int32, ElementalPunishRate);
	DECLARE_PROPERTY(int32, ElementalBlindness);

	void Reset()
	{
		this->SetAddElementalDamage(0);
		this->SetAddElementalDamagePvP(0);
		this->SetAddElementalDamagePvM(0);
		this->SetMulElementalDamagePvP(0);
		this->SetMulElementalDamagePvM(0);
		this->SetElementalDamageVsFire(0);
		this->SetElementalDamageVsWater(0);
		this->SetElementalDamageVsEarth(0);
		this->SetElementalDamageVsWind(0);
		this->SetElementalDamageVsDark(0);
		this->SetAddElementalDefense(0);
		this->SetAddElementalDefensePvP(0);
		this->SetAddElementalDefensePvM(0);
		this->SetElementalDefenseVsFire(0);
		this->SetElementalDefenseVsWater(0);
		this->SetElementalDefenseVsEarth(0);
		this->SetElementalDefenseVsWind(0);
		this->SetElementalDefenseVsDark(0);
		this->SetAddElementalDamageReduction(0);
		this->SetAddElementalDamageReductionPvP(0);
		this->SetAddElementalDamageReductionPvM(0);
		this->SetAddElementalShieldAbsorbRate(0);
		this->SetAddElementalLifeAbsorbRate(0);
		this->SetElementalBastionDuration(0);
		this->SetElementalBleeding(0);
		this->SetElementalParalysisRate(0);
		this->SetElementalBindingRate(0);
		this->SetElementalPunishRate(0);
		this->SetElementalBlindness(0);
	}
};

typedef std::map<uint16, Skill*> PlayerSkillTreeMap;

struct MajesticSkill
{
	public:
		explicit MajesticSkill(uint8 section, uint16 id, uint8 level)
		{
			this->SetID(id);
			this->SetLevel(level);
			this->SetSection(section);
		}

	private:
		DECLARE_ENUM(uint16, ID);
		DECLARE_ENUM(uint8, Level);
		DECLARE_ENUM(uint8, Section);
};

typedef std::vector<MajesticSkill*> PlayerMajesticSkillTreeList;

enum
{
	MAJESTIC_POISONING_CIRCLE_RATE,
	MAJESTIC_CHILLING_CIRCLE_RATE,
	MAJESTIC_BLEEDING_CIRCLE_RATE,

	MAJESTIC_POISONING_CIRCLE_DURATION,
	MAJESTIC_CHILLING_CIRCLE_DURATION,
	MAJESTIC_BLEEDING_CIRCLE_DURATION,

	MAJESTIC_POISONING_CIRCLE_DEBUFF_UPGRADE_RATE,
	MAJESTIC_CHILLING_CIRCLE_DEBUFF_UPGRADE_RATE,
	MAJESTIC_BLEEDING_CIRCLE_DEBUFF_UPGRADE_RATE,

	MAJESTIC_POISONING_CIRCLE_1_TARGETS,
	MAJESTIC_CHILLING_CIRCLE_1_TARGETS,
	MAJESTIC_BLEEDING_CIRCLE_1_TARGETS,

	MAJESTIC_POISONING_CIRCLE_2_TARGETS,
	MAJESTIC_CHILLING_CIRCLE_2_TARGETS,
	MAJESTIC_BLEEDING_CIRCLE_2_TARGETS,

	MAJESTIC_POISONING_CIRCLE_RANGE,
	MAJESTIC_CHILLING_CIRCLE_RANGE,
	MAJESTIC_BLEEDING_CIRCLE_RANGE,

	MAJESTIC_DEBUFF_POISONING_EFFECT,
	MAJESTIC_DEBUFF_CHILLING_EFFECT,
	MAJESTIC_DEBUFF_BLEEDING_EFFECT,

	MAJESTIC_DEBUFF_DEADLY_POISONING_EFFECT,
	MAJESTIC_DEBUFF_FREEZING_EFFECT,
	MAJESTIC_DEBUFF_HEMORRHAGE_EFFECT,

	MAJESTIC_DEBUFF_POISONING_SUCCESS_RATE,
	MAJESTIC_DEBUFF_CHILLING_SUCCESS_RATE,
	MAJESTIC_DEBUFF_BLEEDING_SUCCESS_RATE,

	MAJESTIC_DEBUFF_DEADLY_POISONING_SUCCESS_RATE,
	MAJESTIC_DEBUFF_FREEZING_SUCCESS_RATE,
	MAJESTIC_DEBUFF_HEMORRHAGE_SUCCESS_RATE,
	/*
	Linked Skill Improve
	*/

	MAJESTIC_LINKED_SKILL_ADDITIONAL_DAMAGE,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT_RATE,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_ATTACK_SPEED,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_ATTACK_RANGE,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_SPLASH_DAMAGE,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_DISTANCE,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET,
	MAJESTIC_LINKED_SKILL_REMOVE_COOLDOWN,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_DAMAGE,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_MAGIC_DAMAGE,
	MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_CURSE_DAMAGE,

	MAJESTIC_LINKED_SKILL_SP_BASE_DEFENSE,
	MAJESTIC_LINKED_SKILL_SP_MAX_HP,
	MAJESTIC_LINKED_SKILL_SP_MEMBER_DAMAGE,
	MAJESTIC_LINKED_SKILL_SP_HP_TRANSFER,
	MAJESTIC_LINKED_SKILL_SP_DAMAGE_TRANSFER,
	MAJESTIC_LINKED_SKILL_SP_MEMBER_DEFENSE,
	MAJESTIC_LINKED_SKILL_SP_SHIELD_DEFENSE,
};

typedef std::map<uint8, float> MajesticDataSectionMap;
typedef std::map<uint8, MajesticDataSectionMap> MajesticDataMap;

typedef std::set<std::string> PlayerChatBlockList;

enum 
{
	CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MIN,
	CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MAX,
	CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE,
	CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE_PVP,
	CHARACTER_SPECIALIZATION_DEFENSE,
	CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE,
	CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE_PVP,
	CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MIN,
	CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MAX,
	CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MIN,
	CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MAX,
	CHARACTER_SPECIALIZATION_MAX
};

enum AddItemSend
{
	ADD_ITEM_SEND_OK,
	ADD_ITEM_SEND_NO,
};

#define MAX_ITEM_SOLD		5

typedef std::map<uint16, TickTimer> SkillCooldownMap;

struct HuntingRecordData
{
	HuntingRecordData()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetYear(0);
		this->SetMonth(0);
		this->SetDay(0);
		this->SetLevel(0);
		this->SetDuration(0);
		this->SetDamage(0);
		this->SetElementalDamage(0);
		this->SetHealing(0);
		this->SetKilledCount(0);
		this->SetEarnedExperience(0);
	}

	DECLARE_ENUM(uint16, Year);
	DECLARE_ENUM(uint8, Month);
	DECLARE_ENUM(uint8, Day);
	DECLARE_ENUM(uint32, Level);
	DECLARE_PROPERTY(uint32, Duration);
	DECLARE_PROPERTY(uint64, Damage);
	DECLARE_PROPERTY(uint64, ElementalDamage);
	DECLARE_PROPERTY(uint32, Healing);
	DECLARE_PROPERTY(uint32, KilledCount);
	DECLARE_PROPERTY(uint64, EarnedExperience);
};

typedef std::vector<HuntingRecordData*> HuntingRecordDataList;
typedef std::map<world_type, HuntingRecordDataList> HuntingRecordDataMap;

enum PlayerLoginStep
{
	PLAYER_LOGIN_STEP_ACCOUNT,
	PLAYER_LOGIN_STEP_CHARACTER,
	PLAYER_LOGIN_STEP_OK,
};

enum PlayerTimer
{
	PLAYER_TIMER_SAVE_DB,
	PLAYER_TIMER_GENS_UPDATE,
	PLAYER_TIMER_MUUN_ATTACK,
	PLAYER_TIMER_HELPER_LABYRINTH,
	PLAYER_TIMER_CHEAT_SCAN,
	PLAYER_TIMER_CHEAT_SCAN_INIT,
	PLAYER_TIMER_HACK_CHECK_INIT,
	PLAYER_TIMER_WALK_SPEED,
	PLAYER_TIMER_COORDINATE_ATTRIBUTE,
	PLAYER_TIMER_REFRESH,
	PLAYER_TIMER_QUEST_MU,
	PLAYER_TIMER_GOBLIN_POINT,
	PLAYER_TIMER_MAIL,
	PLAYER_TIMER_CASH_SHOP_COIN,
	PLAYER_TIMER_CASH_SHOP_GIFT,
	PLAYER_TIMER_WRONG_LOCATION,
	PLAYER_TIMER_TEST_MODE,
	PLAYER_TIMER_WRONG_SERIAL,
	PLAYER_TIMER_WRONG_DECRYPT,
	PLAYER_TIMER_SPEED_SEND,
	PLAYER_TIMER_CHAT,
	PLAYER_TIMER_ATTACK_SAFE,
	PLAYER_TIMER_PACKET,
	PLAYER_TIMER_ANTI_SPAM,
	PLAYER_TIMER_PLAY_TIME,
	PLAYER_TIMER_DUEL_TIME,
	PLAYER_TIMER_AUTOLOGIN,
	PLAYER_TIMER_COMBO_CHECK,
	PLAYER_TIMER_CASTLE_SIEGE_TIME,
	PLAYER_TIMER_MIX_RECOVERY,
	PLAYER_TIMER_MUUN_MOUNT,
	PLAYER_TIMER_NORMAL_MOUNT,
	PLAYER_TIMER_SLEEP_REUSE,
	PLAYER_TIMER_OFFATTACK_UPDATE,

	PLAYER_TIMER_MAX
};

enum PlayerCounter
{
	PLAYER_COUNTER_CHEAT_SCAN,
	PLAYER_COUNTER_HACK_CHECK,
	PLAYER_COUNTER_ATTACK_SPEED,
	PLAYER_COUNTER_ATTACK_SAFE,
	PLAYER_COUNTER_ANTI_SPAM,
	PLAYER_COUNTER_WORLD_BOSS,
	PLAYER_COUNTER_COMBO_CHECK,

	PLAYER_COUNTER_MAX
};

enum PlayerDate
{
	PLAYER_DATE_LEVEL_UP,
	PLAYER_DATE_RESET,
	PLAYER_DATE_SANTA_CLAUS_GIFT,
	PLAYER_DATE_LABYRINTH,

	PLAYER_DATE_MAX
};

struct PlayerLabyrinthGate
{
	explicit PlayerLabyrinthGate()
	{
		this->SetIndex(0);
		this->SetSubIndex(0);
		this->ResetMissionID(-1);
		this->ResetMissionCount(0);
	}
	
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint8, SubIndex);
	DECLARE_PROPERTY_ARRAY(uint8, MissionID, MAX_LABYRINTH_PLAYER_MISSION);
	DECLARE_PROPERTY_ARRAY(int32, MissionCount, MAX_LABYRINTH_PLAYER_MISSION);
};

typedef std::vector<PlayerLabyrinthGate*> PlayerLabyrinthGateList;
typedef std::map<uint16, int32> PlayerLabyrinthLevelMap;

struct PlayerLabyrinthData
{
	DECLARE_PROPERTY(uint16, Level);
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY(uint8, Stage);
	DECLARE_ENUM(uint8, GateIndex);
	DECLARE_ENUM(uint8, GateSubIndex);

	DECLARE_PROPERTY(int32, KilledMonsters);
	DECLARE_PROPERTY(int64, EarnedExperience);
	DECLARE_PROPERTY(int32, CompletedMissions);
	DECLARE_PROPERTY(int32, Score);
	DECLARE_ENUM(time_t, Date);
	DECLARE_ENUM(uint8, Status);
	DECLARE_ENUM(uint8, GoblinState);
	DECLARE_ENUM(uint8, DayFirst);

	DECLARE_STRUCT(TickTimer, Time);
	DECLARE_BOOL(UsingTimer);
	
	PlayerLabyrinthGateList m_GateList;
	PlayerLabyrinthLevelMap m_LevelMap;

	void IncreaseLevel()
	{
		PlayerLabyrinthLevelMap::const_iterator itr = this->m_LevelMap.find(this->GetLevel());

		if (itr == this->m_LevelMap.end())
		{
			this->m_LevelMap[this->GetLevel()] = 1;
		}
		else
		{
			this->m_LevelMap[this->GetLevel()]++;
		}
	}

	int32 GetLevelCount(uint16 level) const
	{
		PlayerLabyrinthLevelMap::const_iterator itr = this->m_LevelMap.find(level);

		if (itr != this->m_LevelMap.end())
		{
			return itr->second;
		}

		return 0;
	}

	int32 GetGateCount() const
	{
		return this->m_GateList.size();
	}

	bool IsAnyLevelComplete() const
	{
		return !this->m_LevelMap.empty();
	}

	void StartTime(uint32 time)
	{
		this->SetUsingTimer(true);
		this->GetTime()->Start(time);
	}

	void InitTime()
	{
		this->SetUsingTimer(false);
		this->GetTime()->Reset();
	}

	void Init()
	{
		this->SetLevel(0);
		this->SetID(-1);
		this->SetStage(0);
		this->SetGateIndex(0);
		this->SetGateSubIndex(0);

		this->SetKilledMonsters(0);
		this->SetEarnedExperience(0);
		this->SetCompletedMissions(0);
		this->SetScore(0);
		this->SetDate(0);
		this->SetStatus(0);
		this->SetGoblinState(0);
		this->SetDayFirst(0);

		this->SetUsingTimer(false);
		this->GetTime()->Reset();

		LIST_CLEAR(PlayerLabyrinthGateList::iterator, this->m_GateList);
		this->m_LevelMap.clear();
	}

	void InitData()
	{
		this->SetLevel(0);
		this->SetID(-1);
		this->SetStage(0);
		this->SetGateIndex(0);
		this->SetGateSubIndex(0);

		this->SetKilledMonsters(0);
		this->SetEarnedExperience(0);
		this->SetCompletedMissions(0);
		this->SetScore(0);
		this->SetDate(0);
		this->SetStatus(0);
		this->SetGoblinState(0);

		this->SetUsingTimer(false);
		this->GetTime()->Reset();

		LIST_CLEAR(PlayerLabyrinthGateList::iterator, this->m_GateList);
	}
};


struct PlayerEventEnterCount
{
	DECLARE_ENUM(uint8, EventID);
	DECLARE_PROPERTY(uint8, Count);
	DECLARE_ENUM(uint8, Day);
};

typedef std::map<uint8, PlayerEventEnterCount*> PlayerEventEnterCountMap;

struct PlayerMineBomb
{
	void Reset()
	{
		this->SetValue(0);
		this->SetStatus(0);
	}

	DECLARE_ENUM(uint8, Value);
	DECLARE_ENUM(uint8, Status);
};

struct PlayerWarpFavoriteList
{
	void Reset()
	{
		this->SetData(-1);
		this->SetWarpID(-1);
	}

	DECLARE_ENUM(uint16, Data);
	DECLARE_ENUM(uint16, WarpID);
};

struct PlayerQuestGuided
{
	explicit PlayerQuestGuided()
	{
		this->SetID(0);
		this->SetCount(0);
		this->SetState(QUEST_GUIDED_STATE_NONE);
		this->SetDate(0);
	}

	DECLARE_ENUM(uint16, ID);
	DECLARE_PROPERTY(int32, Count);
	DECLARE_ENUM(uint8, State);
	DECLARE_ENUM(time_t, Date);
};

typedef std::map<uint16, PlayerQuestGuided*> PlayerQuestGuidedMap;

struct PlayerJewelBingo
{
	void Reset()
	{
		this->SetType(0);
		this->SetSlot(0);
		this->SetValue(-1);
	}

	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, Slot);
	DECLARE_PROPERTY(uint8, Value);
};

struct SkillTime
{
	SkillTime(coord_type x, coord_type y)
	{
		this->GetTime()->Start();
		this->SetCount(0);
		this->SetX(x);
		this->SetY(y);
	}

	DECLARE_STRUCT(TickTimer, Time);
	DECLARE_PROPERTY(int32, Count);
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
};

typedef std::map<uint16, SkillTime*> SkillTimeMap;

struct SkillCPS
{
	DECLARE_ENUM(uint32, Time);
	DECLARE_PROPERTY(int32, Count);
};

typedef std::map<uint16, SkillCPS*> SkillCPSMap;

struct AncientSetData
{
	AncientSetData()
	{
		this->SetID(0);
		this->SetParts(0);
		this->SetFull(false);
	}

	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY(int32, Parts);
	DECLARE_BOOL(Full);
};

typedef std::map<uint8, AncientSetData> AncientSetDataMap;

struct QuestMUData
{
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(uint8, State);
	DECLARE_PROPERTY(uint16, Objective);
	DECLARE_ENUM(time_t, Date);
};

typedef std::map<uint16, QuestMUData*> QuestMUDataMap;

typedef std::map<uint32, int32> MonsterSoulAmountDataMap;

struct MonsterSoulData
{
	DECLARE_ENUM(uint32, Type);

	MonsterSoulAmountDataMap m_AmountMap;
};

typedef std::map<uint32, MonsterSoulData*> MonsterSoulDataMap;

struct ACPathData
{
	DECLARE_BOOL(Loaded);
	DECLARE_ENUM(int32, X);
	DECLARE_ENUM(int32, Y);

	void Reset()
	{
		this->SetLoaded(false);
		this->SetX(0);
		this->SetY(0);
	}
};

#endif