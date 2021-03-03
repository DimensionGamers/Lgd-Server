#ifndef ARCA_WAR_H
#define ARCA_WAR_H

/*
	0 = Success
	1 = You cannot participate, only Guild Master can register
	2 = Only guilds with 10 members can participate
	3 = Maximum guilds already registered
	4 = You have already registered
	5 = Registration period has ended
	6 = Registering Guild Master to participate in Arca War is only available up to top 6 of Ranking

	7 = You cannot participate in Arka War. Your are not a guild member of registered guild
	8 = Member already registered
	9 = Maximum member count 20
	10 = Registration period ended

	11 = You cannot participate in the Arka War. Please register first
	12 = The Arka war is not going on this moment. Please enter during the arka war
	13 = Only registered guild members can register for this except guild master
	14 = Guild members can only request participation during the registration period

	15 = Only guild members with at least Battle Master can register Sign of Lord
	16 = Registration not possible, only guilds with 10 members or more can participate
	17 = Registration not possible, the number of guild members exceed the limit
	*/

enum ArkaWarResult
{
	ARKA_WAR_RESULT_REGISTER_SUCCESS						= 0,
	ARKA_WAR_RESULT_MASTER_REGISTER_ONLY_GUILD_MASTER		= 1,
	ARKA_WAR_RESULT_MASTER_REGISTER_LOW_GUILD_MEMBERS		= 2,
	ARKA_WAR_RESULT_MASTER_REGISTER_MAXIMUM_GUILDS			= 3,
	ARKA_WAR_RESULT_MASTER_REGISTER_ALREADY_REGISTERED		= 4,
	ARKA_WAR_RESULT_MASTER_REGISTER_PERIOD_ENDED			= 5,
	ARKA_WAR_RESULT_MASTER_REGISTER_ONLY_TOP6				= 6,
	ARKA_WAR_RESULT_MEMBER_REGISTER_NOT_IN_GUILD			= 7,
	ARKA_WAR_RESULT_MEMBER_REGISTER_ALREADY_REGISTERED		= 8,
	ARKA_WAR_RESULT_MEMBER_REGISTER_MAXIMUM_MEMBERS			= 9,
	ARKA_WAR_RESULT_MEMBER_REGISTER_PERIOD_ENDED			= 10,
	ARKA_WAR_RESULT_MEMBER_REGISTER_ONLY_MEMBERS			= 13,
	ARKA_WAR_RESULT_MEMBER_REGISTER_ONLY_DURING_PERIOD		= 14,
	ARKA_WAR_RESULT_ENTER_NOT_REGISTERED					= 11,
	ARKA_WAR_RESULT_ENTER_PERIOD_ENDED						= 12,
	ARKA_WAR_RESULT_SIGN_REGISTER_NOT_ALLOWED				= 15,
	ARKA_WAR_RESULT_SIGN_REGISTER_LOW_MEMBERS				= 16,
	ARKA_WAR_RESULT_SIGN_REGISTER_HIGH_MEMBERS				= 17,
};

enum ArkaWarState
{
	ARKA_WAR_STATE_NONE				= 1,
	ARKA_WAR_STATE_CLOSED			= 2,
	ARKA_WAR_STATE_MASTER_REGISTER	= 3,
	ARKA_WAR_STATE_MEMBER_REGISTER	= 4,
	ARKA_WAR_STATE_READY			= 5,
	ARKA_WAR_STATE_STANDBY			= 6,
	ARKA_WAR_STATE_PLAYING			= 7,
	ARKA_WAR_STATE_RESULT			= 8,
	ARKA_WAR_STATE_CLOSE			= 9,
	ARKA_WAR_STATE_END				= 10,
	ARKA_WAR_STATE_MAX,
};

//static const int32 ArkaWarStateTime[ARKA_WAR_STATE_MAX] = { 0, 0, 0, 600, 600, 180, 300, 1800, 600, 30, 30 };
//static const int32 ArkaWarStateTime[ARKA_WAR_STATE_MAX] = { 0, 0, 0, 120, 120, 120, 120, 1800, 120, 30, 30 };

#define MAX_ARKA_WAR_AURA			3
#define MAX_ARKA_WAR_AURA_STATE		2
#define MAX_ARKA_WAR_GUILD			6
#define MAX_ARKA_WAR_ZONES			5
#define ARKA_WAR_KILL_OBELISK_SCORE	50
#define ARKA_WAR_KILL_MONSTER_SCORE	5
#define ARKA_WAR_KILL_PLAYER_SCORE	10
#define ARKA_WAR_KILL_TROPHY		ITEMGET(13, 147)

#define ARKA_WAR_REWARD_GENS_BOSS	30
#define ARKA_WAR_REWARD_GENS_USER1	5	//Target level == User level
#define ARKA_WAR_REWARD_GENS_USER2	7	//Target level - User level >= 51
#define ARKA_WAR_REWARD_GENS_USER3	6	//Target level - User level >= 31 && <= 50
#define ARKA_WAR_REWARD_GENS_USER4	6	//Target level - User level >= 11 && <= 30
#define ARKA_WAR_REWARD_GENS_USER5	5	//Target level - User level >= 0 && <= 10
#define ARKA_WAR_REWARD_GENS_USER6	1	//User level - Target level >= 51
#define ARKA_WAR_REWARD_GENS_USER7	2	//User level - Target level >= 31 && <= 50
#define ARKA_WAR_REWARD_GENS_USER8	3	//User level - Target level >= 11 && <= 30
#define ARKA_WAR_REWARD_GENS_USER9	5	//User level - Target level >= 0 && <= 10

/*
Gates a zonas -> Alkmaar
Gates a zonas -> Ubaid
Gates a zonas -> Debenter
Gates a zonas -> Uruk Mountain
Gates a zonas -> Nars
*/
static const struct ArkaWarZoneBasicData
{
	uint16 world;
	uint16 gate;
} g_ArkaWarZoneBasicData[MAX_ARKA_WAR_ZONES] =
{
	{WORLD_ACHERON, 418},
	{WORLD_ACHERON, 422},
	{WORLD_DEVENTER, 435},
	{WORLD_URUK_MOUNTAIN, 450},
	{WORLD_NARS, 487}
};

static const struct ArkaWarZoneBattleData
{
	uint16 world;
	uint16 gate;
	std::string name;
} g_ArkaWarZoneBattleData[MAX_ARKA_WAR_ZONES] =
{
	{WORLD_ACHERON_BATTLE, 427, "Alkmar"},
	{WORLD_ACHERON_BATTLE, 431, "Ubaid"},
	{WORLD_DEVENTER_BATTLE, 439, "Debenter"},
	{-1, -1, ""},
	{-1, -1, ""}
};

struct ArkaWarObeliskAuraBasicData
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X);
	DECLARE_ENUM(int16, Y);

	ArkaWarObeliskAuraBasicData()
	{
		this->SetWorld(-1);
		this->SetX(-1);
		this->SetY(-1);
	}
};

struct ArkaWarObeliskBasicData
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X);
	DECLARE_ENUM(int16, Y);
	DECLARE_ENUM(int32, Life);
	DECLARE_ENUM(int32, OccupyLife);
	DECLARE_ARRAY_STRUCT(ArkaWarObeliskAuraBasicData, AuraBasicData, MAX_ARKA_WAR_AURA);
	
	ArkaWarObeliskBasicData()
	{
		this->SetWorld(-1);
		this->SetX(-1);
		this->SetY(-1);
		this->SetLife(0);
		this->SetOccupyLife(0);
	}
};

struct ArkaWarPlayer
{
	DECLARE_ENUM(uint32, GUID);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_BOOL(BootyDrop);
	DECLARE_PROPERTY(uint32, BootyCount);
	DECLARE_PROPERTY(uint32, ContributePoints);
	DECLARE_PROPERTY(uint32, KillPoints);

	explicit ArkaWarPlayer()
	{
		this->SetGUID(0);
		this->ResetName();
		this->SetBootyDrop(false);
		this->SetBootyCount(0);
		this->SetContributePoints(0);
		this->SetKillPoints(0);
	}
};

typedef std::map<uint32, ArkaWarPlayer*> ArkaWarPlayerMap;

struct ArkaWarGuild
{
	DECLARE_ENUM(uint32, ID);
	//DECLARE_ENUM(uint16, Gate);

	ArkaWarGuild()
	{
		this->Clear();
	}

	void Clear()
	{
		this->SetID(0);
		//this->SetGate(-1);
	}
};

struct ArkaWarOccupyGuild
{
	DECLARE_ENUM(uint32, Guild);
	DECLARE_ENUM(uint8, Attribute);

	ArkaWarOccupyGuild()
	{
		this->Clear();
	};

	void Clear()
	{
		this->SetGuild(0);
		this->SetAttribute(0);
	};
};

struct ObeliskAuraState
{
	DECLARE_ENUM(uint8, State);
	DECLARE_PTR(Player, Player);
	//DECLARE_PTR(Monster, Monster);
	DECLARE_ENUM(uint32, Guild);

	void Clear()
	{
		this->SetState(0);
		this->SetPlayer(nullptr);
		//this->SetMonster(nullptr);
		this->SetGuild(0);
	}
};

struct ObeliskState
{
	DECLARE_PTR(Monster, Monster);
	DECLARE_PROPERTY(uint8, AuraState);
	DECLARE_ENUM(uint32, AuraReleaseGuild);
	DECLARE_ENUM(uint8, State);
	DECLARE_ENUM(uint32, OccupyGuild);
	DECLARE_ENUM(uint8, Attribute);
	DECLARE_ENUM(uint16, MonsterID);
	DECLARE_ARRAY_STRUCT(ObeliskAuraState, AuraStateData, MAX_ARKA_WAR_AURA);
	DECLARE_BOOL(Active);
	
	ObeliskState()
	{
		this->Clear();
	}

	void Clear()
	{
		this->SetActive(false);
		this->SetMonster(nullptr);
		this->SetAuraState(0);
		this->SetAuraReleaseGuild(0);
		this->SetOccupyGuild(0);
		this->SetAttribute(0);
		this->SetMonsterID(-1);

		for ( int32 i = 0; i < MAX_ARKA_WAR_AURA; ++i )
		{
			this->GetAuraStateData(i)->Clear();
		}
	}
};

struct ArkaWarMonsterInfo
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int32, Count);
	DECLARE_ENUM(int32, RespawnTime); // Minutes

	ArkaWarMonsterInfo()
	{
		this->SetWorld(-1);
		this->SetCount(0);
		this->SetRespawnTime(0);
	};
};

struct ArkaWarMonsterRespawnArea
{
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
};

typedef std::list<ArkaWarMonsterRespawnArea*> ArkaWarMonsterRespawnAreaList;

typedef std::vector<uint16> ArkaWarMonsterList;

struct ArkaWarPlayerPosition
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
};

typedef std::list<ArkaWarPlayerPosition*> ArkaWarPlayerPositionList;

struct ArkaWarRewardExperience
{
	DECLARE_ENUM(uint32, KillPointsMin);
	DECLARE_ENUM(uint32, KillPointsMax);
	DECLARE_ENUM(int32, Experience);
};

typedef std::list<ArkaWarRewardExperience*> ArkaWarRewardExperienceList;

struct ArkaWarExchangeData
{
	DECLARE_ENUM(int32, TrophyMin);
	DECLARE_ENUM(int32, TrophyMax);
	DECLARE_ENUM(int32, SuccessRate);
	DECLARE_ENUM(int32, AddSuccessRate);
	DECLARE_PROPERTY_STRING(ItemBag);
};

typedef std::list<ArkaWarExchangeData*> ArkaWarExchangeDataList;

struct ArkaWarObeliskData
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint16, Monster);
	DECLARE_ENUM(uint8, ElementalAttribute);
};

struct ArkaWarBuffData
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_ENUM(uint16, Buff);
	DECLARE_ENUM(uint8, ElementalAttribute);
};

typedef std::map<uint8, ArkaWarObeliskData*> ArkaWarObeliskDataMap;
typedef std::map<uint8, ArkaWarBuffData*> ArkaWarBuffDataMap;

#pragma pack(1)
struct ARKA_WAR_MASTER_REGISTER_RESULT
{
	ARKA_WAR_MASTER_REGISTER_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_MASTER_REGISTER_RESULT, sizeof(ARKA_WAR_MASTER_REGISTER_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct ARKA_WAR_MEMBER_REGISTER_RESULT
{
	ARKA_WAR_MEMBER_REGISTER_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_MEMBER_REGISTER_RESULT, sizeof(ARKA_WAR_MEMBER_REGISTER_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct ARKA_WAR_ENTER_REQUEST
{
	PBMSG_HEAD2 h;
	uint8 result;
};

struct ARKA_WAR_ENTER_RESULT
{
	ARKA_WAR_ENTER_RESULT(uint8 result, uint8 data)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_ENTER_RESULT, sizeof(ARKA_WAR_ENTER_RESULT));
		this->result = result;
		this->data = data;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 data;
};

struct ARKA_WAR_GUILD_MEMBER_RESULT
{
	ARKA_WAR_GUILD_MEMBER_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_VIEW_GUILD_MEMBER_RESULT, sizeof(ARKA_WAR_GUILD_MEMBER_RESULT));
		this->result = result;
		this->count = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 count;
};

struct ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT
{
	ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT(uint8 result)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT, sizeof(ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT));
		this->result = result;
	}

	PBMSG_HEAD2 h;
	uint8 result;
};

struct ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT_OK
{
	ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT_OK(uint32 signs)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_SIGN_OF_LORD_SEND, sizeof(ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT_OK));
		this->signs[0] = HIBYTE(HIWORD(signs));
		this->signs[1] = LOBYTE(HIWORD(signs));
		this->signs[2] = HIBYTE(LOWORD(signs));
		this->signs[3] = LOBYTE(LOWORD(signs));
	}

	PBMSG_HEAD2 h;
	uint8 signs[4];
};

struct ARKA_WAR_SIGN_OF_LORD_CHECK_BODY
{
	uint8 rank;
	char name[MAX_GUILD_NAME_LENGTH + 1];
	uint8 signs[4];
};

struct ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD
{
	ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_SIGN_OF_LORD_CHECK_RESULT, sizeof(ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD));
		this->rank = 0;
		this->count = 0;
		memset(this->signs, 0, 4);
	}

	PBMSG_HEAD2 h;
	uint8 rank;
	uint8 signs[4];
	uint8 count;
};

struct ARKA_WAR_OBELISK_DATA
{
	uint8 life;
	uint8 attribute;
};

struct ARKA_WAR_OBELISK_LIFE
{
	ARKA_WAR_OBELISK_LIFE()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_OBELISK_LIFE, sizeof(ARKA_WAR_OBELISK_LIFE));
	}

	PBMSG_HEAD2 h;
	ARKA_WAR_OBELISK_DATA data[MAX_ARKA_WAR_ZONES];
};

struct ARKA_WAR_OBELISK_STATE
{
	ARKA_WAR_OBELISK_STATE()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_OBELISK_STATE, sizeof(ARKA_WAR_OBELISK_STATE));
		this->attribute = 0;
		this->state = 0;
		this->aura_state = 0;
	}

	PBMSG_HEAD2 h;
	uint8 attribute;
    uint8 state;
    uint8 aura_state;
};

struct ARKA_WAR_OBELISK_INFO
{
	ARKA_WAR_OBELISK_INFO()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_OBELISK_INFO, sizeof(ARKA_WAR_OBELISK_INFO));
		this->attribute = 0;

		for ( int32 i = 0; i < MAX_ARKA_WAR_AURA; ++i )
		{
			this->aura_state[i] = 0;
		}
	}

	PBMSG_HEAD2 h;
	uint8 attribute;
    uint8 aura_state[MAX_ARKA_WAR_AURA];
};

struct ARKA_WAR_STATE
{
	ARKA_WAR_STATE(uint8 state)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_STATE, sizeof(ARKA_WAR_STATE));
		this->state = state;
	}

	PBMSG_HEAD2 h;
	uint8 state;
};

struct ARKA_WAR_JOIN_CANCEL
{
	ARKA_WAR_JOIN_CANCEL()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_JOIN_CANCEL, sizeof(ARKA_WAR_JOIN_CANCEL));
	}

	PBMSG_HEAD2 h;
};

struct ARKA_WAR_MEMBER_COUNT
{
	ARKA_WAR_MEMBER_COUNT()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_VIEW_GUILD_MEMBER_RESULT, sizeof(ARKA_WAR_MEMBER_COUNT));
		this->result = 0;
		this->count = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
	uint8 count;
};

struct ARKA_WAR_PLAYING_OBELISK_AURA
{
	uint32 x;
	uint32 y;
};

struct ARKA_WAR_PLAYING_OBELISK
{
	uint32 group;
	uint32 attribute;
    uint32 world;
    uint32 x;
    uint32 y;
    ARKA_WAR_PLAYING_OBELISK_AURA aura[MAX_ARKA_WAR_AURA];
};

struct ARKA_WAR_PLAYING_INFO
{
	ARKA_WAR_PLAYING_INFO()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_PLAYING_INFO, sizeof(ARKA_WAR_PLAYING_INFO));
		this->guild_group = 0;
		this->unk1 = 0;

		for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
		{
			this->obelisk[i].group = 0;
			this->obelisk[i].attribute = 0;
			this->obelisk[i].world = 0;
			this->obelisk[i].x = 0;
			this->obelisk[i].y = 0;

			for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
			{
				this->obelisk[i].aura[n].x = 0;
				this->obelisk[i].aura[n].y = 0;
			}
		}
	}

	PBMSG_HEAD2 h;
	uint16 guild_group;
	uint16 unk1;
	ARKA_WAR_PLAYING_OBELISK obelisk[MAX_ARKA_WAR_ZONES];
};

struct ARKA_WAR_PLAYER_POSITION_HEAD
{
	PWMSG_HEAD2 h; // C1:F8:48
	uint8 result;
	uint8 count;
	int32 guild;
	char master_name[MAX_CHARACTER_LENGTH + 1];
	uint32 guild_master_gate;
	uint8 master_x;
	uint8 master_y;
};

struct ARKA_WAR_PLAYER_POSITION_BODY 
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 status;
	uint8 x;
	uint8 y;
	uint16 guild_master_gate;
};

struct ARKA_WAR_OBELISK_GUILD_STATE
{
	ARKA_WAR_OBELISK_GUILD_STATE()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_OBELISK_GUILD_STATE, sizeof(ARKA_WAR_OBELISK_GUILD_STATE));
		this->zone = 0;
		this->attribute = 0;
		this->state = 0;
		this->guild_id = 0;
		memset(this->guild, 0, MAX_GUILD_NAME_LENGTH + 1);
	}

	PBMSG_HEAD2 h;
	uint32 zone;
	uint32 attribute;
	uint8 state; // 0 = None, 1 = In Progress, 2 = Captured
	uint32 guild_id;
	char guild[MAX_GUILD_NAME_LENGTH + 1];
};

struct ARKA_WAR_NOTIFY_TEXT
{
	ARKA_WAR_NOTIFY_TEXT(uint8 id, int32 value)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_NOTIFY_TEXT, sizeof(ARKA_WAR_NOTIFY_TEXT));
		this->id = id;
		memset(this->unk, 0, 3);
		this->value = value;
	}

	PBMSG_HEAD2 h;
	uint8 id;
	uint8 unk[3];
	int32 value;
};
#pragma pack()
struct ARKA_WAR_RESULT_OBELISK
{
	uint16 obelisk;
    char guild[MAX_GUILD_NAME_LENGTH + 1];
};

struct ARKA_WAR_RESULT
{
	ARKA_WAR_RESULT()
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_RESULT, sizeof(ARKA_WAR_RESULT));
		this->result = 0;
		this->contribute_point = 0;
		this->kill_point = 0;
		this->reward_exp = 0;
		this->booty_count = 0;
	}

	PBMSG_HEAD2 h;
	uint8 result;
    uint32 contribute_point;
    uint32 kill_point;
    uint32 reward_exp;
    uint16 booty_count;
	ARKA_WAR_RESULT_OBELISK obelisk[MAX_ARKA_WAR_ZONES];
};
#pragma pack(1)
struct ARKA_WAR_OCCUPY_ZONE
{
	ARKA_WAR_OCCUPY_ZONE(const char * guild)
	{
		this->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_OCCUPY_ZONE, sizeof(ARKA_WAR_OCCUPY_ZONE));
		memcpy(this->guild, guild, MAX_GUILD_NAME_LENGTH + 1);
	}

	PBMSG_HEAD2 h;
	char guild[MAX_GUILD_NAME_LENGTH + 1];
};
#pragma pack()
class ArkaWar: public EventMassive
{
	SingletonInstance(ArkaWar);

	public:
		ArkaWar();
		~ArkaWar();

		void Load();

		void Start();
		void Update();
		void ChangeState(uint8 state, bool from_server_link = false);

		CREATE_STATE(None);
		CREATE_STATE(Closed);
		CREATE_STATE(MasterRegister);
		CREATE_STATE(MemberRegister);
		CREATE_STATE(Ready);
		CREATE_STATE(Standby);
		CREATE_STATE(Playing);
		CREATE_STATE(End);
		CREATE_STATE(Result);
		CREATE_STATE(Close);

		void MasterRegisterRequest(Player* pPlayer, uint8 * Packet);
		void MasterRegisterResult(Player* pPlayer, uint8 result);
		void MemberRegisterRequest(Player* pPlayer, uint8 * Packet);
		void MemberRegisterResult(Player* pPlayer, uint8 result);
		void EnterRequest(Player* pPlayer, uint8 * Packet);
		void EnterResult(Player* pPlayer, uint8 result, uint8 data = 0);
		void ViewGuildMemberRequest(Player* pPlayer, uint8 * Packet);
		void ViewGuildMemberResult(Player* pPlayer, uint8 result, uint8 count);
		void SignOfLordRegisterRequest(Player* pPlayer, uint8 * Packet);
		void SignOfLordRegisterResult(Player* pPlayer, uint8 result, uint32 signs = 0);
		void SignOfLordCheckRequest(Player* pPlayer, uint8 * Packet);
		void ExchangeOpen(Player* pPlayer, uint8 * Packet);

		void GuildList(uint8 * Packet);
		void MemberList(uint8 * Packet);

		void SendNoticeToAll(const char * msg, ...);
		void SendNoticeExToAll(uint8 type, uint32 color, uint8 count, uint16 delay, uint8 speed, const char * msg, ...);
		void SendOccupyZone(Player* pPlayer, uint16 gate);
		
		bool IsWinGuild(Player* pPlayer);
		ArkaWarMonsterRespawnArea const* GetRespawnArea(uint8 id) const;
		ArkaWarPlayerPosition const* GetRandomPosition(uint8 id) const;
		ArkaWarExchangeData const* GetExchangeData(int32 trophy_count) const;
		void SetRandomObeliskAttribute();
		void SetObelisk();
		void AddMonster();
		void SendPlayerPosition();
		void SendObeliskLife();
		void SendObeliskState();
		void CheckObeliskAuraPlayer();
		bool GetGatePosition(Player* pPlayer, uint16 gate, uint16 & world, int16 & x, int16 & y);
		bool GetGuildMasterPosition(Player* pPlayer, uint16 gate, uint16 & world, int16 & x, int16 & y);
		bool GetPosition(uint16 gate, uint16 & world, int16 & x, int16 & y);
		bool IsPkAllowed(Player* pPlayer01, Player* pPlayer02);
		void SendStateAll();
		void SendState(Player* pPlayer);
		void SendPlayInfo(Player* pPlayer);
		void SendResult();
		bool GetBoxPosition(uint16 world, int16 ax, int16 ay, int16 aw, int16 ah, int16 & mx, int16 & my);
		void PlayerKill(Player* pPlayer, Player* pVictim);
		void BootyItemGet(Player* pPlayer);
		int64 CalculateRewardExperience(Player* pPlayer, ArkaWarPlayer * pArkaWarPlayer);
		ArkaWarPlayer * GetPlayerInfo(uint32 id) const;
		ArkaWarPlayer * GetPlayerInfo(const char * name) const;
		ArkaWarGuild const* GetGuildInfo(uint32 id) const;
		bool ItemDrop(Player* pPlayer, Monster* pMonster);

		void WinnerBuffVerify(Player* pPlayer);

		void SendNotifyText(uint8 id, int32 value = 0);
		void SendNotifyTextObelisk(uint8 obelisk, uint8 id, uint32 guild);
		void SendObeliskReleased(uint8 obelisk, uint32 guild);
		void SendObeliskConquered(uint8 obelisk, uint32 guild);

		bool IsObeliskOccupied(uint32 guild) const;

		void SendObeliskGuildData(uint8 id);

		void InsertRanking();
		void UpdateCharacterCount();
	private:
		DECLARE_ENUM(uint8, State);
		DECLARE_STRUCT(TickTimer, Time);
		DECLARE_STRUCT(TickTimer, NotifyTime);
		DECLARE_STRUCT(TickTimer, PlayNotifyTime);
		DECLARE_STRUCT(TickTimer, PlayerPositionTime);
		DECLARE_PROPERTY(int32, CurrentNotifyTime);

		ArkaWarPlayerMap m_ArkaWarPlayer;
		DECLARE_ARRAY_STRUCT(ArkaWarGuild, ArkaWarGuild, MAX_ARKA_WAR_GUILD);
		DECLARE_ARRAY_STRUCT(ArkaWarOccupyGuild, OccupyGuild, MAX_ARKA_WAR_ZONES);
		DECLARE_ARRAY_STRUCT(ObeliskState, ObeliskState, MAX_ARKA_WAR_ZONES);
		DECLARE_ARRAY_STRUCT(ArkaWarObeliskBasicData, ObeliskBasicData, MAX_ARKA_WAR_ZONES);
		DECLARE_ARRAY_STRUCT(ArkaWarMonsterInfo, MonsterInfo, MAX_ARKA_WAR_ZONES);
		ArkaWarMonsterList monster_list[MAX_ARKA_WAR_ZONES];
		ArkaWarMonsterRespawnAreaList monster_respawn_area[MAX_ARKA_WAR_ZONES];
		ArkaWarPlayerPositionList player_position_list[MAX_ARKA_WAR_ZONES];
		ArkaWarRewardExperienceList reward_experience;
		ArkaWarExchangeDataList exchange_list;
		ArkaWarObeliskDataMap m_ArkaWarObeliskDataMap;
		ArkaWarBuffDataMap m_ArkaWarBuffDataMap;
};

#endif