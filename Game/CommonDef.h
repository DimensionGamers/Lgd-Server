/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "CommonDef.h"
*
*/

#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#define INIT_MEMBER_ARRAY(name, ct) \
	Unit* name[MAX_PARTY_MEMBERS]; \
	uint8 ct = 0; \
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i ) \
	{ \
		name[i] = nullptr; \
	}

inline uint32 _ARGB(uint8 a, uint8 r, uint8 g, uint8 b)
{
	return a*16777216 + b*65536 + g*256 + r;
};

enum GateFlag
{
	GATE_FLAG_NORMAL,
	GATE_FLAG_IN,
	GATE_FLAG_OUT,
	MAX_GATE_FLAG
};

#define MAX_VIEWPORT			120

#define BUFF_EFFECT_MAX			5
#define MAX_BUFF_DATA			2

#define BUFF_EFFECT_LOOP(loop) for ( int32 loop = 0; loop < BUFF_EFFECT_MAX; ++loop )

#define WARP_FAVORITE_COUNT		5

#define MAX_WALK_PATH			15

#define LIST_ERASE(it, list) \
	delete *it;\
	it = list.erase(it);

#define Vector(a,b,c,d) {(d)[0]=a;(d)[1]=b;(d)[2]=c;}
#define MAX_ARRAY_FRUSTRUM				4
#define DotProduct(x,y) ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])

#define	Q_PI         	3.14159265358979323846f

#define FIX_COORD(coord) \
	limitmin(coord, 0) \
	limitmax(coord, 255)

#define FIX_COORD_D(coord) \
	if ( coord < 0 ) coord.set(0); \
	if ( coord > 255 ) coord.set(255);

#define IN_RANGE(pObject1, pObject2, distance) \
	(Util::Distance(pObject1->GetX(), pObject1->GetY(), pObject2->GetX(), pObject2->GetY()) <= distance)

#define DISTANCE(pObject1, pObject2) \
	Util::Distance(pObject1->GetX(), pObject1->GetY(), pObject2->GetX(), pObject2->GetY())

#define MONSTER_AI(pMonsterPtr) \
	if ( pMonsterPtr->GetAI() ) \
		pMonsterPtr->GetAI()

#define MONSTER_AI_NAME(pMonsterPtr, name, script) \
	if ( pMonsterPtr->AIGroup(name) ) \
		((script*)pMonsterPtr->AIGroup(name))

#define PLAYER_POINTER(pPlayerPtr) \
	if ( pPlayerPtr && pPlayerPtr->IsPlayer() ) \
		pPlayerPtr->ToPlayer()

#define MONSTER_POINTER(pMonsterPtr) \
	if ( pMonsterPtr && pMonsterPtr->IsCreature() ) \
		pMonsterPtr->ToCreature()

#define ENTRY(pUnitPtr) (pUnitPtr ? pUnitPtr->GetEntry(): -1)

bool BC_MAP_RANGE(uint16 world);
bool DS_MAP_RANGE(uint16 world);
bool CC_MAP_RANGE(uint16 world);
bool IT_MAP_RANGE(uint16 world);
bool DG_MAP_RANGE(uint16 world);
bool IF_MAP_RANGE(uint16 world);
bool KL_MAP_RANGE(uint16 world);
bool CR_MAP_RANGE(uint16 world);
bool TS_MAP_RANGE(uint16 world);
bool AW_MAP_RANGE(uint16 world);
bool KUBERA_MAP_RANGE(uint16 world);

bool BC_MAP_RANGE(World * world);
bool DS_MAP_RANGE(World * world);
bool CC_MAP_RANGE(World * world);
bool IT_MAP_RANGE(World * world);
bool DG_MAP_RANGE(World * world);
bool IF_MAP_RANGE(World * world);
bool KL_MAP_RANGE(World * world);
bool CR_MAP_RANGE(World * world);
bool TS_MAP_RANGE(World * world);
bool AW_MAP_RANGE(World * world);
bool KUBERA_MAP_RANGE(World * world);

#define MAKE_PCT(packet) (uint8*)&packet, packet.h.get_size()
#define SEND_PCT(packet) sendPacket(MAKE_PCT(packet));
#define SEND_PCT_VP(packet) sendPacket_viewport(MAKE_PCT(packet));

#define normal_inventory_size				64

#define inventory_size						239

#define MUUN_INVENTORY_SIZE 103
#define MUUN_INVENTORY_WEAR_SIZE 3

#define MUUN_INVENTORY_RANGE(x) (((x)<0)?false:((x)>=MUUN_INVENTORY_SIZE)?false:true)
#define MUUN_INVENTORY_WEAR_RANGE(x) (((x)<0)?false:((x)>=MUUN_INVENTORY_WEAR_SIZE)?false:true)
#define MUUN_INVENTORY_BASE_RANGE(x) (((x)<MUUN_INVENTORY_WEAR_SIZE)?false:((x)>=MUUN_INVENTORY_SIZE)?false:true)

#define EVENT_INVENTORY_SIZE 32
#define EVENT_INVENTORY_RANGE(x) (((x)<0)?false:((x)>=EVENT_INVENTORY_SIZE)?false:true)
#define EVENT_INVENTORY_LOOP(x) for ( int32 x = 0; x < EVENT_INVENTORY_SIZE; ++x )

#define INVENTORY_EXT1_SIZE			108
#define INVENTORY_EXT2_SIZE			140
#define INVENTORY_EXT3_SIZE			172
#define INVENTORY_EXT4_SIZE			204

#define GREMORY_CASE_INVENTORY_SIZE			50
#define GREMORY_CASE_PERSONAL_STORE_SIZE	150

enum GremoryCaseType
{
	GREMORY_CASE_TYPE_ACCOUNT,
	GREMORY_CASE_TYPE_CHARACTER,
	GREMORY_CASE_TYPE_MOBILE,
	GREMORY_CASE_TYPE_PERSONAL_STORE,
	GREMORY_CASE_TYPE_MAX
};

#define use_inventory_size					204
#define inventory_map_size					224
#define only_inventory_map_size				192
#define expanded_inventory_1_start			76
#define expanded_inventory_1_end			107
#define expanded_inventory_2_start			108
#define expanded_inventory_2_end			139
#define expanded_inventory_3_start			140
#define expanded_inventory_3_end			171
#define expanded_inventory_4_start			172
#define expanded_inventory_4_end			203
#define personal_store_start				204
#define personal_store_end					235

#define inventory_range(x)					(x < 0 || x >= inventory_size ? false : true)

#define main_inventory_range(x)				((x < 0 || x >= inventory_size) ? false : ((x >= 0 && x < PLAYER_MAX_EQUIPMENT) || x == PENTAGRAM_SLOT || x == EARRING_01 || x == EARRING_02) ? false : true)
#define INVENTORY_WEAR_RANGE(x)				((x < 0 || x >= inventory_size) ? false : ((x >= 0 && x < PLAYER_MAX_EQUIPMENT) || x == PENTAGRAM_SLOT || x == EARRING_01 || x == EARRING_02) ? true : false)

#define INVENTORY_MAIN_RANGE(x)				(((x)<PLAYER_MAX_EQUIPMENT)?false:((x)>=expanded_inventory_1_start)?false:true)
#define INVENTORY_EXT1_RANGE(x)				(((x)<expanded_inventory_1_start)?false:((x)>=expanded_inventory_2_start)?false:true)
#define INVENTORY_EXT2_RANGE(x)				(((x)<expanded_inventory_2_start)?false:((x)>=expanded_inventory_3_start)?false:true)
#define INVENTORY_EXT3_RANGE(x)				(((x)<expanded_inventory_3_start)?false:((x)>=expanded_inventory_4_start)?false:true)
#define INVENTORY_EXT4_RANGE(x)				(((x)<expanded_inventory_4_start)?false:((x)>=personal_store_start)?false:true)

#define WAREHOUSE_MAIN_RANGE(x)				(((x)<0)?false:((x)>=120)?false:true)
#define WAREHOUSE_EXT1_RANGE(x)				(((x)<120)?false:((x)>=warehouse_size)?false:true)

#define use_inventory_range(x)				(((x)<PLAYER_MAX_EQUIPMENT)?false:((x)>use_inventory_size-1)?false:true )
#define drop_inventory_range(x)				(((x)<0)?false:((x)>use_inventory_size-1)?false:true )
#define sell_inventory_range(x)				(((x)<0)?false:((x)>use_inventory_size-1)?false:true )
#define normal_inventory_range(x)			(((x)<0)?false:((x)>normal_inventory_size-PLAYER_MAX_EQUIPMENT-1)?false:true )
#define expanded_inventory_1_range(x)		(((x)<expanded_inventory_1_start)?false:((x)>expanded_inventory_1_end)?false:true )
#define expanded_inventory_2_range(x)		(((x)<expanded_inventory_2_start)?false:((x)>expanded_inventory_2_end)?false:true )
#define expanded_inventory_3_range(x)		(((x)<expanded_inventory_3_start)?false:((x)>expanded_inventory_3_end)?false:true )
#define expanded_inventory_4_range(x)		(((x)<expanded_inventory_4_start)?false:((x)>expanded_inventory_4_end)?false:true )
#define personal_store_range(x)				((x >= personal_store_start && x <= personal_store_end) ? true : false)
#define inventory_loop(var, start, end)		for ( uint8 var = start; var < end; ++var )
#define use_inventory_loop(var)				for ( uint8 var = PLAYER_MAX_EQUIPMENT; var < use_inventory_size; ++var )
#define wear_inventory_loop(var)			for ( uint8 var = 0; var < PLAYER_MAX_EQUIPMENT; ++var )
#define full_inventory_loop(var)			for ( uint8 var = 0; var < inventory_size; ++var )

#define trade_size							32
#define trade_range(x)						(((x)<0)?false:((x)>trade_size-1)?false:true )
#define trade_loop(loop)					for ( uint8 loop = 0; loop < trade_size; ++loop )

#define chaos_box_size						32
#define chaos_box_range(x)					(((x)<0)?false:((x)>chaos_box_size-1)?false:true )
#define chaos_box_loop(loop)				for ( uint8 loop = 0; loop < chaos_box_size; ++loop )

#define max_shop_item		120
#define shop_range(x)		(((x)<0)?false:((x)>max_shop_item-1)?false:true )

namespace Viewport
{
	enum
	{
		STATUS_NONE,
		STATUS_STANDBY,
		STATUS_READY,
		STATUS_DELETE,

		MAX_DISTANCE				= 15,
		MAX_ITEM					= 100,
	};
}

#define PARTY_MATCHING_TEXT_LENGHT	40
#define PARTY_MATCHING_PASSWORD_LENGTH	4
#define PARTY_MATCHING_SEARCH_LENGTH	10

enum RespawnType
{
	GAME_OBJECT_RESPAWN_NONE,
	GAME_OBJECT_RESPAWN_NORMAL,
	GAME_OBJECT_RESPAWN_DELETE
};

enum RespawnLocation
{
	MONSTER_RESPAWN_NORMAL,
	MONSTER_RESPAWN_ZONE,
};

enum ConnectStatus
{
	CONNECT_STATUS_NONE,
	CONNECT_STATUS_CONNECTED,
	CONNECT_STATUS_LOGGED,
	CONNECT_STATUS_PLAYING
};

namespace Element
{
	enum
	{
		ICE,
		POISON,
		LIGHTNING,
		FIRE,
		EARTH,
		WIND,
		WATER,
		MAX
	};
}

#define GET_MAX_RESISTANCE(x, y, z) ( ( ( ( (x) > (y) ) ? (x) : (y) ) > (z) ) ? ( ( (x) > (y) ) ? (x) : (y) ) : (z) )

#define CS_CLASS		0
#define CS_WEAPON1_TYPE	1
#define CS_WEAPON2_TYPE	2
#define CS_WEAPON1_DATA	12
#define CS_WEAPON2_DATA	13
#define CS_HELMET1		13
#define CS_HELMET2		9
#define CS_HELMET3		3
#define CS_ARMOR1		14
#define CS_ARMOR2		9
#define CS_ARMOR3		3
#define CS_PANTS1		14
#define CS_PANTS2		9
#define CS_PANTS3		4
#define CS_GLOVES1		15
#define CS_GLOVES2		9
#define CS_GLOVES3		4
#define CS_BOOTS1		15
#define CS_BOOTS2		9
#define CS_BOOTS3		5
#define CS_WINGS1		5
#define CS_WINGS2		9
#define CS_LEVEL1		6
#define CS_LEVEL2		7
#define CS_LEVEL3		8
#define CS_EXE_EFFECT	10
#define CS_ANC_EFFECT	11

#define CS_GET_CLASS(x) ( (((x)>>4)<<5)&(0xE0) )
#define CS_GET_CHANGEUP(x) ( ((x)&0x07) )
#define CS_GET_MASTERCLASS(x) ( (((x)>>1)<<3)&(0x08) )

#define CS_SET_CLASS(x) ( ((x)<<5)& 0xE0 )
#define CS_SET_CHANGEUP(x) ( ((x) << 4) & 0x10 )
#define CS_SET_MASTERCLASS(x) ( ((x) << 3) & 0x08 )

#define DBI_GET_TYPE(x)			( ((x)&((MAX_TYPE_ITEMS-1)<<8))>>4 )
#define DBI_GET_INDEX(x)		( (x) & 0xFF )

#define CS_SET_HELMET1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_HELMET2(x) ( ((x) & 0x10 ) << 3 )
#define CS_SET_HELMET3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_ARMOR1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_ARMOR2(x) ( ((x) & 0x10 ) << 2 )
#define CS_SET_ARMOR3(x) ( ((x) & 0x0F )      )

#define CS_SET_PANTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_PANTS2(x) ( ((x) & 0x10 ) << 1 )
#define CS_SET_PANTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_GLOVES1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_GLOVES2(x) ( ((x) & 0x10 )      )
#define CS_SET_GLOVES3(x) ( ((x) & 0x0F )      )

#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_WING1(x)  ( ((x) & 0x03 ) << 2 )
#define CS_SET_HELPER(x) ( ((x) & 0x03 )      )

#define CS_SET_SMALLLEVEL_RH(x)		( (x)       )
#define CS_SET_SMALLLEVEL_LH(x)		( (x) << 3  ) 
#define CS_SET_SMALLLEVEL_HELMET(x)	( (x) << 6  )
#define CS_SET_SMALLLEVEL_ARMOR(x)	( (x) << 9  )
#define CS_SET_SMALLLEVEL_PANTS(x)	( (x) << 12 )
#define CS_SET_SMALLLEVEL_GLOVES(x)	( (x) << 15 )
#define CS_SET_SMALLLEVEL_BOOTS(x)	( (x) << 18 )

#define CS_SET_SMALLLEVEL1(x)		( ((x) >> 16) & 0xFF )
#define CS_SET_SMALLLEVEL2(x)		( ((x) >> 8 ) & 0xFF )
#define CS_SET_SMALLLEVEL3(x)		(  (x)		  & 0xFF )

#define MAKE_WEAPON_01(charset, item) \
	charset[CS_WEAPON1_DATA] |= DBI_GET_TYPE(item); \
	charset[CS_WEAPON1_TYPE] = item % 256;

#define MAKE_WEAPON_02(charset, item) \
	charset[CS_WEAPON2_DATA] |= DBI_GET_TYPE(item); \
	charset[CS_WEAPON2_TYPE] = item % 256;

#define MAKE_HELMET(charset, item) \
	charset[CS_HELMET1] |= CS_SET_HELMET1(item); \
	charset[CS_HELMET2] |= CS_SET_HELMET2(item); \
	charset[CS_HELMET3] |= CS_SET_HELMET3(item);

#define MAKE_ARMOR(charset, item) \
	charset[CS_ARMOR1] |= CS_SET_ARMOR1(item); \
	charset[CS_ARMOR2] |= CS_SET_ARMOR2(item); \
	charset[CS_ARMOR3] |= CS_SET_ARMOR3(item);

#define MAKE_PANTS(charset, item) \
	charset[CS_PANTS1] |= CS_SET_PANTS1(item); \
	charset[CS_PANTS2] |= CS_SET_PANTS2(item); \
	charset[CS_PANTS3] |= CS_SET_PANTS3(item);

#define MAKE_GLOVES(charset, item) \
	charset[CS_GLOVES1] |= CS_SET_GLOVES1(item); \
	charset[CS_GLOVES2] |= CS_SET_GLOVES2(item); \
	charset[CS_GLOVES3] |= CS_SET_GLOVES3(item);

#define MAKE_BOOTS(charset, item) \
	charset[CS_BOOTS1] |= CS_SET_BOOTS1(item); \
	charset[CS_BOOTS2] |= CS_SET_BOOTS2(item); \
	charset[CS_BOOTS3] |= CS_SET_BOOTS3(item);

enum SerialCreateType
{
	serial_create_none,
	serial_create_map,
	serial_create_inventory,
	serial_create_chaos_box,
	serial_create_warehouse,
	serial_create_shop,
	serial_create_cash_shop,
	serial_create_pentagram_mix,
	serial_create_gremory_case,
	serial_create_ruud_shop,
	serial_create_inventory_box,
	serial_create_socket_mix,
};

enum ChatType
{
	CHAT_NORMAL,		// Chat Normal
	CHAT_GLOBAL,		// Chat de administrador
	CHAT_PARTY,
	CHAT_GUILD,
	CHAT_ALLIANCE,
	CHAT_GUILD_NOTICE,
	CHAT_GENS,
	CHAT_POST,
	CHAT_COMMAND,
	CHAT_COMMAND_GM,
};



#define KALIMA_LEVEL				7

static const int32 g_KalimaLevel[KALIMA_LEVEL][Character::MAX_CLASS] =
{
	40, 40, 40, 20, 20, 40, 20, 20, 40, 40, 40,
	131, 131, 131, 111, 111, 131, 111, 111, 131, 131,
	181, 181, 181, 161, 161, 181, 161, 161, 181, 181, 131,
	231, 231, 231, 211, 211, 231, 211, 211, 231, 231, 231,
	281, 281, 281, 261, 261, 281, 261, 261, 281, 281, 281,
	331, 331, 331, 311, 311, 331, 311, 311, 331, 331, 331,
	350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350,
};

enum GuildCommonResult
{
	GUILD_ANS_NOTEXIST_GUILD			= 0x10,
	GUILD_ANS_UNIONFAIL_BY_CASTLE		= 0x10,
	GUILD_ANS_NOTEXIST_PERMISSION		= 0x11,
	GUILD_ANS_NOTEXIST_EXTRA_STATUS		= 0x12,
	GUILD_ANS_NOTEXIST_EXTRA_TYPE		= 0x13,
	GUILD_ANS_EXIST_RELATIONSHIP_UNION	= 0x15,
	GUILD_ANS_EXIST_RELATIONSHIP_RIVAL	= 0x16,
	GUILD_ANS_EXIST_UNION				= 0x17,
	GUILD_ANS_EXIST_RIVAL				= 0x18,
	GUILD_ANS_NOTEXIST_UNION			= 0x19,
	GUILD_ANS_NOTEXIST_RIVAL			= 0x1a,
	GUIDL_ANS_NOT_UNION_MASTER			= 0x1b,
	GUILD_ANS_NOT_GUILD_RIVAL			= 0x1c,
	GUILD_ANS_CANNOT_BE_UNION_MASTER	= 0x1d,
	GUILD_ANS_EXCEED_MAX_UNION_MEMBER	= 0x1e,
	GUILD_ANS_CANCLE_REQUEST			= 0x20,
	GUILD_ANS_ASSIGN_ARKA_WAR_STATE		= 0x21,
	GUILD_ANS_MASTER_NOT_JOINED_GENS	= 0xA1,
	GUILD_ANS_MASTER_NOT_JOINED_GENS_2	= 0xA2,
	GUILD_ANS_DIFFERENT_GENS			= 0xA3,
	GUILD_ANS_ARKA_WAR_STATE			= 0xA4,
};

enum
{
	EVENT_MESSAGE_OPEN_DEVIL_SQUARE = 1,
	EVENT_MESSAGE_END_DEVIL_SQUARE = 2,

	EVENT_MESSAGE_OPEN_BLOOD_CASTLE = 3,
	EVENT_MESSAGE_START_BLOOD_CASTLE = 4,
	EVENT_MESSAGE_END_BLOOD_CASTLE = 5,
	EVENT_MESSAGE_CLEAR_BLOOD_CASTLE = 6,

	EVENT_MESSAGE_OPEN_CHAOS_CASTLE = 10,
	EVENT_MESSAGE_START_CHAOS_CASTLE = 11,
	EVENT_MESSAGE_END_CHAOS_CASTLE = 12,
	EVENT_MESSAGE_CLEAR_CHAOS_CASTLE = 13,

	EVENT_MESSAGE_DOPPELGANGER_START = 17,
	EVENT_MESSAGE_DOPPELGANGER_BOSS = 18,
	EVENT_MESSAGE_DOPPELGANGER_END = 19,

	EVENT_MESSAGE_OPEN_CHAOS_CASTLE_SURVIVAL = 20,
	EVENT_MESSAGE_START_CHAOS_CASTLE_SURVIVAL = 21,
	EVENT_MESSAGE_END_CHAOS_CASTLE_SURVIVAL = 22,
	EVENT_MESSAGE_CLEAR_CHAOS_CASTLE_SURVIVAL = 23,

	EVENT_MESSAGE_START_TORMENTED_SQUARE_SURVIVAL = 26,
};

enum EventID
{
	EVENT_BLOOD_CASTLE							= 0,
	EVENT_DEVIL_SQUARE							= 1,
	EVENT_CHAOS_CASTLE							= 2,
	EVENT_ILLUSION_TEMPLE						= 3,
	EVENT_CRYWOLF								= 4,
	EVENT_IMPERIAL_FORTRESS						= 5,
	EVENT_RAKLION								= 6,
	EVENT_KANTURU								= 7,
	EVENT_INVASION								= 8,
	EVENT_CASTLE_SIEGE							= 9,
	EVENT_HAPPY_HOUR							= 10,
	EVENT_SCRAMBLE								= 11,
	EVENT_DUNGEON_RACE							= 12,
	EVENT_LOSTTOWER_RACE						= 13,
	EVENT_DOPPELGANGER							= 14,
	EVENT_CHAOS_CASTLE_SURVIVAL					= 16,
	EVENT_PROTECTOR_OF_ACHERON					= 17,
	EVENT_TORMENTED_SQUARE						= 18,
	EVENT_ARKA_WAR								= 19,
	EVENT_LAST_MAN_STANDING						= 20,
	EVENT_NIXIES_LAKE							= 21,
	EVENT_LABYRINTH_OF_DIMENSIONS				= 22,
	EVENT_TORMENTED_SQUARE_SURVIVAL				= 23,
	EVENT_CASTLE_DEEP							= 24,
	EVENT_WORLD_BOSS							= 26,
	EVENT_SWAMP_OF_DARKNESS						= 27,
	EVENT_INSTANCED_DUNGEON						= 28,

	EVENT_ID_MAX,

	EVENT_NONE						= uint8(-1),
};

enum NoticeType
{
	NOTICE_GLOBAL,
	NOTICE_NORMAL_BLUE,
	NOTICE_GUILD,
	NOTICE_NORMAL_RED,
};

enum GenFamily
{
	GEN_NONE,
	GEN_DUPRIAN,
	GEN_VANERT,
	GEN_MAX
};

enum GenLevel
{
	GEN_LEVEL_NONE,
	GEN_LEVEL_GRAND_DUKE,
	GEN_LEVEL_DUKE,
	GEN_LEVEL_MARQUIS,
	GEN_LEVEL_COUNT,
	GEN_LEVEL_VISCOUNT,
	GEN_LEVEL_BARON,
	GEN_LEVEL_KNIGHT_COMMANDER,
	GEN_LEVEL_SUPERIOR_KNIGHT,
	GEN_LEVEL_KNIGHT,
	GEN_LEVEL_GUARD_PREFECT,
	GEN_LEVEL_OFFICER,
	GEN_LEVEL_LIEUTENANT,
	GEN_LEVEL_SERGEANT,
	GEN_LEVEL_PRIVATE,
	GEN_LEVEL_MAX
};

class GenData
{
	public:
		DECLARE_ENUM(uint8, Family);
		DECLARE_ENUM(uint8, Level);
		DECLARE_PROPERTY(int32, Contribution);
		DECLARE_ENUM(int32, Ranking);

		DECLARE_ENUM(time_t, RewardDate);
		DECLARE_ENUM(time_t, JoinDate);
		DECLARE_ENUM(time_t, LeftDate);

		bool IsFamily() const { return this->GetFamily() == GEN_DUPRIAN || this->GetFamily() == GEN_VANERT; };

		void Reset()
		{
			this->SetFamily(0);
			this->SetLevel(0);
			this->SetContribution(0);
			this->SetRanking(0);
			this->SetRewardDate(0);
			this->SetJoinDate(0);
			this->SetLeftDate(0);
		}
};

enum UnitIntFieldData
{
	UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN,
	UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX,
	UNIT_INT_ATTACK_DAMAGE_LEFT_MIN,
	UNIT_INT_ATTACK_DAMAGE_LEFT_MAX,

	UNIT_INT_PHYSICAL_DAMAGE_MIN,
	UNIT_INT_PHYSICAL_DAMAGE_MAX,

	UNIT_INT_ATTACK_RATE,
	UNIT_INT_ATTACK_RATE_PVP,
	UNIT_INT_ATTACK_SPEED,
	UNIT_INT_MAGIC_DAMAGE_MIN,
	UNIT_INT_MAGIC_DAMAGE_MAX,
	UNIT_INT_CURSE_DAMAGE_MIN,
	UNIT_INT_CURSE_DAMAGE_MAX,
	UNIT_INT_MAGIC_SPEED,
	UNIT_INT_DEFENSE,
	UNIT_INT_DEFENSE_ADD_PVP,
	UNIT_INT_DEFENSE_RATE,
	UNIT_INT_DEFENSE_RATE_PVP,
	UNIT_INT_CRITICAL_DAMAGE_ADD,
	UNIT_INT_EXCELLENT_DAMAGE_ADD,
	UNIT_INT_DOUBLE_DAMAGE_ADD,
	UNIT_INT_DAMAGE_DECREASE,
	UNIT_INT_DAMAGE_DECREASE_ADD,
	UNIT_INT_REFLECT_DAMAGE_ADD,
	UNIT_INT_SKILL_ADD_DAMAGE,
	UNIT_INT_DAMAGE_ADD,
	//UNIT_INT_DAMAGE_ADD_PVP,
	UNIT_INT_DEFENSE_MAGIC,
	UNIT_INT_MOVE_SPEED,

	UNIT_INT_ELEMENTAL_DEFENSE,
	UNIT_INT_ELEMENTAL_DAMAGE_MIN,
	UNIT_INT_ELEMENTAL_DAMAGE_MAX,
	UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE,
	UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE,

	UNIT_INT_DEFENSE_BASE,
	UNIT_INT_COMBO_DAMAGE,

	UNIT_INT_DEBUFF_RESISTANCE,
	UNIT_INT_DEBUFF_DEFENSE,

	UNIT_INT_DEBUFF_BLEEDING_RESISTANCE,
	UNIT_INT_DEBUFF_POISON_RESISTANCE,
	UNIT_INT_DEBUFF_DAMAGE_REDUCTION_RESISTANCE,
	UNIT_INT_DEBUFF_ATTACK_SPEED_REDUCTION_RESISTANCE,
	UNIT_INT_DEBUFF_DEFENSE_REDUCTION_RESISTANCE,

	UNIT_INT_MAX
};

enum UnitFloatFieldData
{
	UNIT_FLOAT_REFLECT_DAMAGE_RATE,
	UNIT_FLOAT_CRITICAL_DAMAGE_RATE,
	UNIT_FLOAT_EXCELLENT_DAMAGE_RATE,
	UNIT_FLOAT_DOUBLE_DAMAGE_RATE,
	UNIT_FLOAT_TRIPLE_DAMAGE_RATE,
	UNIT_FLOAT_IGNORE_DEFENSE_RATE,
	UNIT_FLOAT_STUN_RATE,

	UNIT_FLOAT_TOTAL_REFLECT_DAMAGE,
	UNIT_FLOAT_TOTAL_LIFE_RECOVERY,
	UNIT_FLOAT_TOTAL_MANA_RECOVERY,
	UNIT_FLOAT_TOTAL_SHIELD_RECOVERY,
	UNIT_FLOAT_TOTAL_STAMINA_RECOVERY,

	UNIT_FLOAT_RESIST_STUN,
	UNIT_FLOAT_RESIST_IGNORE_DEFENSE,
	UNIT_FLOAT_RESIST_IGNORE_SD,
	UNIT_FLOAT_RESIST_DOUBLE_DAMAGE_RATE,
	UNIT_FLOAT_RESIST_EXCELLENT_DAMAGE_RATE,
	UNIT_FLOAT_RESIST_CRITICAL_DAMAGE_RATE,
	UNIT_FLOAT_RESIST_KNOCKBACK,

	UNIT_FLOAT_ELEMENTAL_DEFENSE_PERCENT,
	UNIT_FLOAT_ELEMENTAL_DAMAGE_PERCENT,
	UNIT_FLOAT_ELEMENTAL_DEFENSE_SUCCESS_RATE_PERCENT,
	UNIT_FLOAT_ELEMENTAL_ATTACK_SUCCESS_RATE_PERCENT,

	UNIT_FLOAT_MAX,
};

namespace Path
{
	static const int16 Max = 16;
	static const int16 Table[Max] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0 };

	inline void FixSearch(int16 & searchp)
	{
		searchp += 2;

		if ( searchp > Max - 1 )
		{
			searchp = 0;
		}
	}

	inline void FixCoordinate(coord_type & to_fix, coord_type to_compare, int32 distance)
	{
		if ( to_fix < to_compare )
		{
			to_fix -= distance;
		}

		if ( to_fix > to_compare )
		{
			to_fix += distance;
		}
	}
}

#define PATH_LOOP for ( int32 path_loop = 0; path_loop < ((Path::Max / 2) - 1); ++path_loop )

namespace Nova
{
	static const int16 Max = 13;
	static const int32 Damage[Max] = { 0, 20, 50, 99, 160, 225, 325, 425, 550, 700, 880, 1090, 1320 };
}

enum DelayedID
{
	DELAYED_NONE,
	DELAYED_ATTACK,
	DELAYED_MAGIC_ATTACK,
	DELAYED_MONSTER_DIE_PLUS,
	DELAYED_REFLECT,
	DELAYED_PUSH_BACK,
	DELAYED_PUSH_BACK_COUNT,
	DELAYED_DAMAGE,
	DELAYED_DARK_SPIRIT,
	DELAYED_BUFF_ADD,
	DELAYED_BUFF_DEL,
	DELAYED_MONSTER_DIE,
	DELAYED_MONSTER_HIT,
	DELAYED_LIFE_RECOVER,
	DELAYED_ATTACK_RANGE,
	DELAYED_MUUN_ATTACK,
	DELAYED_FEREA_MINE_CREATE,
	DELAYED_FEREA_MINE_EXPLODE,
	DELAYED_SWAMP_OF_DARKNESS_FLAME,
	DELAYED_SWAMP_OF_DARKNESS_DARK,
	DELAYED_SWAMP_OF_DARKNESS_PET,
	DELAYED_SWAMP_OF_DARKNESS_WATER,
	DELAYED_MULTI_SKILL,
	DELAYED_MULTI_SKILL_ANGLE,
};

enum TeleportStatus
{
	TELEPORT_NONE				= 0,
	TELEPORT_STANDBY			= 1,
	TELEPORT_READY				= 2,
	TELEPORT_UNK				= 3
};

#define TELEPORT_MAX		4

enum RegenStatus
{
	REGEN_NONE,
	REGEN_STANDBY,
	REGEN_READY,
	REGEN_STARTED,
};

enum DeadStatus
{
	DEAD_NONE,			// Proceso para objeto activo
	DEAD_STANDBY,		// Proceso para comenzar a revivir al objeto
	DEAD_READY,			// Proceso para calcular datos para revivir el objeto
};

enum ViewState
{
	VIEW_STATE_NONE,
	VIEW_STATE_TELEPORT,
	VIEW_STATE_ACTION_1,
	VIEW_STATE_ACTION_2,
	VIEW_STATE_ACTION_3,
	VIEW_STATE_MAX
};

enum DamageType
{
	DAMAGE_TYPE_NORMAL				= 0,
	DAMAGE_TYPE_IGNORE				= 1,
	DAMAGE_TYPE_EXE					= 2,
	DAMAGE_TYPE_CRITICAL			= 3,
	DAMAGE_TYPE_REFLECT				= 4,
	DAMAGE_TYPE_POISON				= 5,
	DAMAGE_TYPE_DEBUFF				= 6,
	DAMAGE_TYPE_WHITE				= 7,

	DAMAGE_PROP_RIGHT				= 0x10,
	DAMAGE_PROP_LEFT				= 0x20,
	DAMAGE_PROP_DOUBLE				= 0x40,
	DAMAGE_PROP_COMBO				= 0x80,
	DAMAGE_PROP_TRIPLE				= 0x100,

	MISS_TYPE_MISS					= 0,
	MISS_TYPE_RESIST				= 7, // Stun & Knockback Resist
	MISS_TYPE_BLOCK					= 8, // Shield Damage Block
	MISS_TYPE_PARRY					= 9,
	MISS_TYPE_ABSORB				= 10, // Shield Damage Absorb

	ELEMENTAL_DAMAGE_TYPE_PUNISH = 6,
	ELEMENTAL_DAMAGE_TYPE_CRITICAL = 7,
	ELEMENTAL_DAMAGE_TYPE_AVOID = 8,
};

enum BuffFlag
{
	BUFF_FLAG_NONE				= 0,
	BUFF_FLAG_CONSTANT			= 1 << 0,
	BUFF_FLAG_SHOP				= 1 << 1,
	BUFF_FLAG_DB_STORE			= 1 << 2,
	BUFF_FLAG_COMBAT			= 1 << 3,
	BUFF_FLAG_EXPIRE			= 1 << 4,
	BUFF_FLAG_ACCOUNT			= 1 << 5,

	BUFF_CLEAR_FLAG_CONSTANT	= 1 << 0,
	BUFF_CLEAR_FLAG_SHOP		= 1 << 1,
	BUFF_CLEAR_FLAG_COMBAT		= 1 << 2,

	BUFF_CLEAR_FLAG_ALL			= 1 << 0 | 1 << 1 | 1 << 2
};

#define VIEWPORT_LOOP(loop_object) \
	VIEWPORT_LOOP_OBJECT(this, loop_object)

#define VIEWPORT_LOOP_OBJECT(object, loop_object) \
	ViewportDataList const& oviewport = object->GetAllViewport(); \
	for ( ViewportDataList::const_iterator loop_id = oviewport.begin(); loop_id != oviewport.end(); ++loop_id ) {\
		ViewportData* data = loop_id->second; \
		if ( !data ) continue; \
		loop_object = data->GetObjectData(); \
		if ( data->GetStatus() != Viewport::STATUS_READY || !loop_object ) continue;

#define VIEWPORT_CLOSE }

enum ObjectAction
{
	OBJECT_ACTION_DIRECTION				= 122,
	OBJECT_ACTION_REST_1				= 128,
	OBJECT_ACTION_REST_2				= 129,
	OBJECT_ACTION_REST_3				= 130,
	OBJECT_ACTION_GO					= 148,
	OBJECT_ACTION_DANCE					= 157,

	OBJECT_ACTION_CHRISTMAS_1			= 160,
	OBJECT_ACTION_CHRISTMAS_2			= 161,
	OBJECT_ACTION_CHRISTMAS_3			= 162,
	OBJECT_ACTION_CHRISTMAS_4			= 163,
	OBJECT_ACTION_CHRISTMAS_5			= 164,
	OBJECT_ACTION_CHRISTMAS_6			= 165,
};

enum ViewportCreateFlag
{
	VIEWPORT_CREATE_FLAG_ME			= 1 << 0,
	VIEWPORT_CREATE_FLAG_GEN		= 1 << 1,
	VIEWPORT_CREATE_FLAG_GUILD		= 1 << 2,
	VIEWPORT_CREATE_FLAG_ONLY_ME	= 1 << 3,
	VIEWPORT_CREATE_FLAG_SIEGE		= 1 << 4,
};

enum ObjectState
{
	OBJECT_STATE_NONE			= 0,
	OBJECT_STATE_STAND_BY		= 1,
	OBJECT_STATE_READY			= 2,
	OBJECT_STATE_DEAD			= 4,
	OBJECT_STATE_RESPAWN		= 8,
	OBJECT_STATE_TELEPORTING	= 32,
};

enum DBDataState
{
	DB_DATA_UNCHANGED                               = 0,
    DB_DATA_CHANGED                                 = 1,
    DB_DATA_NEW                                     = 2,
    DB_DATA_REMOVED                                 = 3
};

void ConvertAIGroupList(std::string const& ai_group, std::vector<int32> & ai_group_list);

enum RegisterData
{
	REGISTER_ID_ITEM_SELL,
	REGISTER_ID_CHARACTER_RESET,
	REGISTER_ID_CHARACTER_MASTER_RESET,
};

#define CREATE_STATE(state) \
	void SetState_##state(); \
	void ProcState_##state();

#define CREATE_STATE_VIRTUAL(state) \
	virtual void SetState_##state() { } \
	virtual void ProcState_##state() { }

#define BUILD_STATE(name, type, state) void name::##type##State_##state()

#define PROCESS_STATE(type, state) type##State_##state()

enum GuildWarType
{
	GUILD_WAR_TYPE_NONE = -1,
	GUILD_WAR_TYPE_NORMAL,
	GUILD_WAR_TYPE_SOCCER,
};

enum GuildWarState
{
	GUILD_WAR_STATE_NONE,
	GUILD_WAR_STATE_DECLARE,
	GUILD_WAR_STATE_ACTIVE,
};

enum
{
	GUILD_WAR_TEAM_BLUE,
	GUILD_WAR_TEAM_RED,
	GUILD_WAR_TEAM_MAX
};

#define GUILD_WAR_TEAM_LOOP(loop) for ( int32 loop = 0; loop < GUILD_WAR_TEAM_MAX; ++loop )

#define EVENT_NOTIFICATION_TIME			2000

#define BOSS_INACTIVE_INSTANCE			1000000

/*
		1 = DK
		2 = ELF
		4 = DW
		8 = MG
		16 = DL
		32 = SUM
		64 = RF
		128 = GL
		*/

enum MatchingClassFlag
{
	MATCHING_DARK_KNIGHT		= 1 << 0,
	MATCHING_ELF				= 1 << 1,
	MATCHING_DARK_WIZARD		= 1 << 2,
	MATCHING_MAGIC_GLADIATOR	= 1 << 3,
	MATCHING_DARK_LORD			= 1 << 4,
	MATCHING_SUMMONER			= 1 << 5,
	MATCHING_RAGE_FIGHTER		= 1 << 6,
	MATCHING_GROW_LANCER		= 1 << 7,
	MATCHING_RUNE_WIZARD		= 1 << 8,
	MATCHING_SLAYER				= 1 << 9,
};

enum ServerType
{
	SERVER_TYPE_NORMAL,
	SERVER_TYPE_CASTLE_SIEGE,
	SERVER_TYPE_EVENT,
	SERVER_TYPE_ARKA_WAR,

	SERVER_TYPE_MAX
};

#define MINI_BOMB_SIZE_H			6
#define MINI_BOMB_SIZE_W			8
#define MINI_BOMB_SIZE				MINI_BOMB_SIZE_H * MINI_BOMB_SIZE_W
#define MINI_BOMB_TRAP_COUNT		11

#define JEWEL_BINGO_GRID_SIZE		25
#define JEWEL_BINGO_JEWEL_COUNT		6
#define JEWEL_BINGO_MATCHING_SIZE	12

#define NUMERIC_BASEBALL_ROWS				5
#define NUMERIC_BASEBALL_NUMBERS_PER_ROW	3
#define NUMERIC_BASEBALL_MAX_NUMBER			10

namespace StatFlag
{
	enum
	{
		HP = 1 << 0,
		SD = 1 << 1,
		MP = 1 << 2,
		AG = 1 << 3,
		MAX_HP = 1 << 4,
		MAX_SD = 1 << 5,
		MAX_MP = 1 << 6,
		MAX_AG = 1 << 7,
	};

	inline uint32 All()
	{
		return HP | SD | MP | AG | MAX_HP | MAX_SD | MAX_MP | MAX_AG;
	}

	inline uint32 Current()
	{
		return HP | SD | MP | AG;
	}

	inline uint32 Max()
	{
		return MAX_HP | MAX_SD | MAX_MP | MAX_AG;
	}
};

#define MAX_PENTAGRAM_JEWEL_INFO			250
#define MAX_PENTAGRAM_JEWEL_RANK			5
#define MAX_PENTAGRAM_JEWEL_LEVEL			11
#define MAX_PENTAGRAM_JEWEL_TYPE			5
#define MAX_PENTAGRAM_ELEMENTAL_ATTRIBUTE	5
#define MAX_PENTAGRAM_BONUS_OPTION		7

enum ePentagramJewelType
{
	PENTAGRAM_JEWEL_TYPE_INVENTORY = 0,
	PENTAGRAM_JEWEL_TYPE_WAREHOUSE = 1,
	PENTAGRAM_JEWEL_TYPE_TRADE = 2,
	PENTAGRAM_JEWEL_TYPE_PSHOP = 3,
};

enum PentagramElementalAttribute
{
	ELEMENTAL_ATTRIBUTE_NONE = 0,
	ELEMENTAL_ATTRIBUTE_FIRE = 1,
	ELEMENTAL_ATTRIBUTE_WATER = 2,
	ELEMENTAL_ATTRIBUTE_EARTH = 3,
	ELEMENTAL_ATTRIBUTE_WIND = 4,
	ELEMENTAL_ATTRIBUTE_DARK = 5,
	ELEMENTAL_ATTRIBUTE_MAX
};

enum ePentagramRefineChaosMixNumber
{
	CHAOS_MIX_PENTAGRAM_MITHRIL = 1,
	CHAOS_MIX_PENTAGRAM_SLOT_OF_RADIANCE = 2,
	CHAOS_MIX_PENTAGRAM_ERRTEL_ELEMENT_CHANGE = 3,
	CHAOS_MIX_PENTAGRAM_MAX,
};

enum ePentagramUpgradeChaosMixNumber
{
	CHAOS_MIX_PENTAGRAM_JEWEL_UPGRADE_LEVEL = 0,
	CHAOS_MIX_PENTAGRAM_JEWEL_UPGRADE_RANK = 1,
	CHAOS_MIX_PENTAGRAM_JEWEL_UPGRADE_MAX
};

#define PENTAGRAM_MIX_MAX_SLOT				28
#define PENTAGRAM_MIX_UPGRADE_MAX_SLOT		39

#endif