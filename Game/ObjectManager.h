/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ObjectManager.h"
*
*/

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

extern uint16 g_StartMonsters;
extern uint16 g_EndMonsters;
extern uint16 g_StartSummonMonsters;
extern uint16 g_EndSummonMonsters;
extern uint16 g_StartPlayers;
extern uint16 g_EndPlayers;
extern uint16 g_MaxObjects;
extern uint16 g_CurrentPlayer;
extern uint16 g_CurrentSummonMonster;

extern int32 g_SiegeLordMixCount;
extern uint16 g_SiegeLordMixDay;

#define player_range(x)				(((x)<g_StartPlayers)?false:((x)>g_EndPlayers-1)?false:true )
#define monster_range(x)			(((x)<g_StartMonsters)?false:((x)>g_EndMonsters-1)?false:true )
#define summon_monster_range(x)		(((x)<g_StartSummonMonsters)?false:((x)>g_EndSummonMonsters-1)?false:true )


//						IP					Cuenta	Tiempo
typedef std::map<uint32, uint32> AccountBlockedTimeMap;
typedef std::map<std::string, AccountBlockedTimeMap> AccountBlockedMap;

typedef std::map<uint16, Object*> ObjectContainerMap;
extern ObjectContainerMap ObjectContainer;

typedef std::set<uint16> InvasionIndexList;
typedef std::map<uint16, InvasionIndexList> InvasionIndexWorld;
typedef std::map<uint8, InvasionIndexWorld> InvasionIndexMap;

typedef std::unordered_map<uint16, Object*> SessionMap;
typedef std::unordered_map<uint16, Player*> PlayerSessionMap;
typedef std::unordered_map<uint32, Player*> PlayerMap;

class CObjectMgr
{
	SingletonInstance(CObjectMgr);

	private:
		DECLARE_ENUM(uint16, MaxPlayers);
		DECLARE_ENUM(uint16, MaxMonsters);
		DECLARE_ENUM(uint16, MaxReservedMonsters);
		DECLARE_ENUM(uint16, ReservedPlayers);

		AccountBlockedMap m_account_blocked;
		InvasionIndexMap m_invasion_index;

		DECLARE_BOOL(RespawnMonster);
		DECLARE_ENUM(TCType, RespawnMonsterTime);

		DECLARE_ENUM(uint32, UpdateMovementTime);

		DECLARE_BOOL(AutoLogin);
		DECLARE_STRUCT(TickTimer, AutoLoginTime);
	public:
		CObjectMgr();
		~CObjectMgr();

		void Init();
		void LoadGameServerData();
		void SaveGameServerData();

		Player * PlayerAdd(std::shared_ptr<WorldSocket> socket, bool autologin);

		Monster * MonsterTryAdd(uint16 monster, uint16 world);

		Unit * GetUnit(uint16 guid);
		Player * FindPlayer(uint16 guid);
		Monster * FindMonster(uint16 guid);
		Monster* FindEmptySummon();
		void Update();
		bool AccountOnlineCheck(const char * account);
		bool CharacterOnlineCheck(const char * character);

		void sendPacket(uint8 * packet, uint16 size);
		void sendPacketWorld(uint16 world, uint8 * packet, uint16 size);

		void SendNoticeToGens(const char * name, const char * msg, uint8 family);
		void SendNoticeToAll(uint8 type, const char * msg, ...);
		void SendNoticeToAll(CustomMessageID type, const char * msg, ...);
		void SendEventNotification(uint8 type, const char * msg, ...);
		void SendNoticeToAdministrator(uint8 type, uint16 world, const char * msg, ...);
		void SendNoticeToAllNormal(uint8 type, const char * msg);
		void SendNoticeToMap(uint16 world, uint8 type, const char * msg, ...);
		void SendPostToAll(uint16 server, const char * name, const char * msg);

		void SendGuildMessageToAdmin(uint32 guild, const char * name, const char * msg);
		void SendAllianceMessageToAdmin(uint32 guild, const char * name, const char * msg);
		void SendGuildNoticeToAdmin(uint32 guild, const char * name, const char * msg);
		void PlayerOnlineUpdate(uint32 id, const char * name, uint16 server, bool online);

		Player* FindPlayerByName(const char * name);
		Player* FindPlayerByNameNoSensitive(const char * name);
		Player* FindPlayerByAccount(uint32 guid);
		Player* FindPlayerByGUID(uint32 id);
		Player* FindPlayer(uint16 entry, uint32 ms_time, uint32 account_id, uint32 character_id);
		Player* FindPlayer(CUSTOM_PACKET_PLAYER const& player);

		DECLARE_PROPERTY(uint32, CurrentSerial);
		DECLARE_PROPERTY(uint32, CurrentCashShopSerial);

		uint32 GenerateSerial()
		{
			this->IncreaseCurrentSerial(1);

			return this->GetCurrentSerial();
		}

		uint32 GenerateSerialCashShop()
		{
			this->IncreaseCurrentCashShopSerial(1);

			return this->GetCurrentCashShopSerial();
		}
		
		Object* FindObject(uint16 guid) { return guid < g_MaxObjects ? ObjectContainer[guid] : nullptr; }

		std::string MakeText(std::string const& src, Unit* pSource, Unit* pTarget, uint32 time = 0, uint32 data_1 = 0);

		void SendToWeb(const char * msg);

		bool ExistKalimaGate(Player* pPlayer);

		void FireworksSend(uint16 world, int32 instance, int16 x, int16 y);

		Monster* FindAIGroupLeader(uint32 Group);
		void ChangeAIOrder(uint32 Group, uint8 order);
		int32 GetPlayerCount() const;
		uint8 GetPlayerPercent(bool real = false) const;

		int32 GetIPConnected(std::string const& ip);

		void KickAll();

		void UpdateInvasionAnimation();
		size_t GetInvasionAnimationCount(uint8 animation, uint16 world) const;
		void UpdateInvasionAnimation(uint8 animation, uint16 index, uint16 world, bool add);
		void SendInvasionAnimation(Player* pPlayer, uint8 animation, size_t animation_count);

		void PersonalStoreDisconnect(const char * account);
		void KickCharactersByAccount(uint32 account_id);

		const PlayerSessionMap& GetAllCharacters() const { return m_player_characters; }
		const PlayerMap& GetAllAccounts() const { return m_player_accounts; }
		const PlayerSessionMap& GetAllPlayers() const { return m_player_sessions; }
		const SessionMap& GetAllSessions() const { return m_sessions; }
		/* PLAYER SESSION	*/

		SessionMap m_sessions;
		PlayerSessionMap m_player_sessions;
		PlayerSessionMap m_player_characters;
		PlayerMap m_player_accounts;

		// sessions that are added async
		void AddSession(Object* s);
        void AddSession_(Object* s);
        LockedQueue<Object*> addSessQueue;

		void UpdateSessions(uint32 diff);
		void UpdateSessionsViewport(uint32 diff);

		void AddSessionCharacter(Player* pPlayer);
		void AddSessionAccount(Player* pPlayer);

		void RemoveSessionCharacter(Player* pPlayer);
		void RemoveSessionAccount(Player* pPlayer);

		void RemoveSessionPlayer(Player* pPlayer);

		CRITICAL_SECTION PlayerAddCritical;

		int32 GetPlayerCountOnInstance(uint16 world_id, int32 instance) const;
		void SendPacketToInstance(uint16 world_id, int32 instance, uint8 * Packet, uint16 size);

		void CharacterAutoLogin();
};

#define sObjectMgr SingNull(CObjectMgr)

Monster* MONSTER_ADD_CONTINUE(monster const* data);

#define HASH_UNIT(iter) \
	for ( uint16 iter = 0; iter < g_MaxObjects; ++iter )

#define HASH_PLAYER(iter) \
	for ( uint16 iter = g_StartPlayers; iter < g_EndPlayers; ++iter )

#define HASH_MONSTER(iter) \
	for ( uint16 iter = g_StartMonsters; iter < g_EndMonsters; ++iter )

#define HASH_SUMMON(iter) \
	for ( uint16 iter = g_StartSummonMonsters; iter < g_EndSummonMonsters; ++iter )

#define HASH_GLOBAL_MONSTER(iter) \
	for ( uint16 iter = g_StartMonsters; iter < g_EndSummonMonsters; ++iter )

#endif