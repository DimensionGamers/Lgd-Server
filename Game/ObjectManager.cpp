/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ObjectManager.cpp"
*
*/

uint16 g_StartMonsters = 0;
uint16 g_EndMonsters = 0;
uint16 g_StartSummonMonsters = 0;
uint16 g_EndSummonMonsters = 0;
uint16 g_StartPlayers = 0;
uint16 g_EndPlayers = 0;
uint16 g_MaxObjects = 0;
uint16 g_CurrentPlayer = 0;
uint16 g_CurrentSummonMonster = 0;

int32 g_SiegeLordMixCount = 0;
uint16 g_SiegeLordMixDay = 0;

bool BC_MAP_RANGE(uint16 world)
{
	return world == WORLD_BLOOD_CASTLE_1 ||
		   world == WORLD_BLOOD_CASTLE_2 ||
		   world == WORLD_BLOOD_CASTLE_3 ||
		   world == WORLD_BLOOD_CASTLE_4 ||
		   world == WORLD_BLOOD_CASTLE_5 ||
		   world == WORLD_BLOOD_CASTLE_6 ||
		   world == WORLD_BLOOD_CASTLE_7 ||
		   world == WORLD_BLOOD_CASTLE_8;
}

bool DS_MAP_RANGE(uint16 world) 
{ 
	return (world == WORLD_DEVIL_SQUARE_1 ||
		    world == WORLD_DEVIL_SQUARE_2 ||
		    world == WORLD_DEVIL_SQUARE_3 ||
		    world == WORLD_DEVIL_SQUARE_4 ||
		    world == WORLD_DEVIL_SQUARE_5 ||
		    world == WORLD_DEVIL_SQUARE_6 ||
		    world == WORLD_DEVIL_SQUARE_7);
}

bool CC_MAP_RANGE(uint16 world)
{ 
	return (world == WORLD_CHAOS_CASTLE_1 ||
		    world == WORLD_CHAOS_CASTLE_2 ||
		    world == WORLD_CHAOS_CASTLE_3 ||
		    world == WORLD_CHAOS_CASTLE_4 ||
		    world == WORLD_CHAOS_CASTLE_5 ||
		    world == WORLD_CHAOS_CASTLE_6 ||
		    world == WORLD_CHAOS_CASTLE_7 ||
			world == WORLD_CHAOS_CASTLE_SURVIVAL);
}

bool IT_MAP_RANGE(uint16 world)
{
	return world == WORLD_ILLUSION_TEMPLE_1 ||
		   world == WORLD_ILLUSION_TEMPLE_2 ||
		   world == WORLD_ILLUSION_TEMPLE_3 ||
		   world == WORLD_ILLUSION_TEMPLE_4 ||
		   world == WORLD_ILLUSION_TEMPLE_5 ||
		   world == WORLD_ILLUSION_TEMPLE_6;
}

bool DG_MAP_RANGE(uint16 world)
{
	return world == WORLD_DOPPELGANGER_1 ||
		   world == WORLD_DOPPELGANGER_2 ||
		   world == WORLD_DOPPELGANGER_3 ||
		   world == WORLD_DOPPELGANGER_4 ||
		   world == WORLD_DOPPELGANGER_5 ||
		   world == WORLD_DOPPELGANGER_6 ||
		   world == WORLD_DOPPELGANGER_7 ||
		   world == WORLD_DOPPELGANGER_8 ||
		   world == WORLD_DOPPELGANGER_9;
}

bool IF_MAP_RANGE(uint16 world) 
{
	return world == WORLD_IMPERIAL_FORTRESS_1 ||
		   world == WORLD_IMPERIAL_FORTRESS_2 ||
		   world == WORLD_IMPERIAL_FORTRESS_3 ||
		   world == WORLD_IMPERIAL_FORTRESS_4;
}

bool KL_MAP_RANGE(uint16 world)
{ 
	return (world == WORLD_KALIMA_1 ||
		    world == WORLD_KALIMA_2 ||
		    world == WORLD_KALIMA_3 ||
		    world == WORLD_KALIMA_4 ||
		    world == WORLD_KALIMA_5 ||
		    world == WORLD_KALIMA_6 ||
		    world == WORLD_KALIMA_7);
}

bool CR_MAP_RANGE(uint16 world)
{
	return (world == WORLD_CRYWOLF_FIRST_ZONE ||
		    world == WORLD_CRYWOLF_SECOND_ZONE );
}

bool TS_MAP_RANGE(uint16 world)
{
	return (world == WORLD_TORMENTED_SQUARE ||
		    world == WORLD_TORMENTED_SQUARE_1 ||
			world == WORLD_TORMENTED_SQUARE_2 ||
			world == WORLD_TORMENTED_SQUARE_3 ||
			world == WORLD_TORMENTED_SQUARE_4 );
}

bool AW_MAP_RANGE(uint16 world)
{
	return (world == WORLD_ACHERON_BATTLE ||
		    world == WORLD_DEVENTER_BATTLE ||
			world == WORLD_URUK_MOUNTAIN_BATTLE );	
}

bool KUBERA_MAP_RANGE(uint16 world)
{
	return (world == WORLD_KUBERA_MINE_1 ||
		world == WORLD_KUBERA_MINE_2 ||
		world == WORLD_KUBERA_MINE_3 ||
		world == WORLD_KUBERA_MINE_4 ||
		world == WORLD_KUBERA_MINE_5);
}

bool BC_MAP_RANGE(World * world)
{
	return !world ? false: BC_MAP_RANGE(world->GetEntry());
}

bool DS_MAP_RANGE(World * world)
{
	return !world ? false: DS_MAP_RANGE(world->GetEntry());
}

bool CC_MAP_RANGE(World * world)
{
	return !world ? false: CC_MAP_RANGE(world->GetEntry());
}

bool IT_MAP_RANGE(World * world)
{
	return !world ? false: IT_MAP_RANGE(world->GetEntry());
}

bool DG_MAP_RANGE(World * world)
{
	return !world ? false: DG_MAP_RANGE(world->GetEntry());
}

bool IF_MAP_RANGE(World * world)
{
	return !world ? false: IF_MAP_RANGE(world->GetEntry());
}

bool KL_MAP_RANGE(World * world)
{
	return !world ? false: KL_MAP_RANGE(world->GetEntry());
}

bool CR_MAP_RANGE(World * world)
{
	return !world ? false: CR_MAP_RANGE(world->GetEntry());
}

bool TS_MAP_RANGE(World * world)
{
	return !world ? false: TS_MAP_RANGE(world->GetEntry());
}

bool AW_MAP_RANGE(World * world)
{
	return !world ? false: AW_MAP_RANGE(world->GetEntry());
}

bool KUBERA_MAP_RANGE(World * world)
{
	return !world ? false : KUBERA_MAP_RANGE(world->GetEntry());
}

ObjectContainerMap ObjectContainer;

CObjectMgr::CObjectMgr()
{
	this->SetCurrentSerial(0);
	this->SetCurrentCashShopSerial(0);
	this->SetMaxMonsters(0);
	this->SetMaxReservedMonsters(0);
	this->SetMaxPlayers(0);
	this->SetReservedPlayers(0);
	this->SetRespawnMonster(false);
	this->SetRespawnMonsterTime(0);

	this->SetUpdateMovementTime(0);

	this->SetAutoLogin(false);
	this->GetAutoLoginTime()->Reset();

	InitializeCriticalSection(&this->PlayerAddCritical);
}

CObjectMgr::~CObjectMgr()
{
	MAP_CLEAR(ObjectContainerMap::iterator, ObjectContainer);

	this->m_account_blocked.clear();
	this->m_invasion_index.clear();

	addSessQueue.clear();
	m_sessions.clear();
	m_player_characters.clear();
	m_player_accounts.clear();
	m_player_sessions.clear();

	DeleteCriticalSection(&this->PlayerAddCritical);
}

void CObjectMgr::Init()
{
	uint32 oldMSTime = getMSTime();

	sLog->outInfo(LOG_DEFAULT, "Loading Objects...");

	this->SetMaxPlayers(sMain->GetInt32("GameServer.MaxPlayers", 1));
	this->SetReservedPlayers(sMain->GetInt32("GameServer.ReservedPlayers", 500));
	this->SetMaxMonsters(sMain->GetInt32("GameServer.MaxMonsters", 1));
	this->SetMaxReservedMonsters(sMain->GetInt32("GameServer.ReservedMonsters", 1));

	g_StartMonsters = 0;
	g_EndMonsters = this->GetMaxMonsters();

	g_StartSummonMonsters = g_EndMonsters;
	g_EndSummonMonsters = g_StartSummonMonsters + this->GetMaxReservedMonsters();
	g_StartPlayers = g_EndSummonMonsters;
	g_EndPlayers = g_StartPlayers + this->GetMaxPlayers();
	g_MaxObjects = this->GetMaxPlayers() + this->GetMaxMonsters() + this->GetMaxReservedMonsters();

	HASH_UNIT(i)
	{
		if ( monster_range(i) || summon_monster_range(i) )
		{
			ObjectContainer[i] = new Monster(i);
		}
		else if ( player_range(i) )
		{
			ObjectContainer[i] = new Player(i);
		}
	}

	g_CurrentPlayer = g_StartPlayers;
	g_CurrentSummonMonster = g_StartSummonMonsters;

	InitFrustrum();

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u Game Objects in %u ms", g_MaxObjects, GetMSTimeDiffToNow(oldMSTime));
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CObjectMgr::LoadGameServerData()
{
	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ITEM_SERIAL_SELECT);
	stmt->setUInt16(0, sGameServer->GetServerCode());

	PreparedQueryResult result = LoginDatabase.Query(stmt);
	
	if ( result )
	{
		Field* fields = result->Fetch();
		this->SetCurrentSerial(fields[1].GetUInt32());
		this->SetCurrentCashShopSerial(fields[2].GetUInt32());
	}
}
	
void CObjectMgr::SaveGameServerData()
{
	SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ITEM_SERIAL_UPDATE);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, this->GetCurrentSerial());
	stmt->setUInt32(2, this->GetCurrentCashShopSerial());
	trans->Append(stmt);

	LoginDatabase.CommitTransaction(trans);
}

Unit * CObjectMgr::GetUnit(uint16 guid)
{
	Object* pObject = this->FindObject(guid);

	if ( !pObject )
		return nullptr;
		
	return pObject->ToUnit();
}

Player * CObjectMgr::FindPlayer(uint16 guid)
{
	Object* pObject = this->FindObject(guid);

	if ( !pObject )
		return nullptr;

	return pObject->ToPlayer();
}
	
Monster * CObjectMgr::FindMonster(uint16 guid)
{
	Object* pObject = this->FindObject(guid);

	if ( !pObject )
		return nullptr;

	return pObject->ToCreature();
}

Player * CObjectMgr::PlayerAdd(std::shared_ptr<WorldSocket> socket, bool autologin)
{
	int32 totalcount = 0;
	int32 count = 0;
	Player* pPlayer = nullptr;

	EnterCriticalSection(&this->PlayerAddCritical);
	
	count = g_CurrentPlayer;
	
	while ( true )
	{
		pPlayer = ObjectContainer[count]->ToPlayer();

		if ( pPlayer && pPlayer->GetConnectStatus() == CONNECT_STATUS_NONE && pPlayer->IsTimeout() )
		{
			pPlayer->ClearAccount();
			pPlayer->SetMSTime(GetTickCount());
			pPlayer->SetConnectStatus(CONNECT_STATUS_CONNECTED);
			pPlayer->CreateSocket(socket);
			pPlayer->GetAccountData()->SetIP(socket ? socket->GetRemoteIpAddress().to_string().c_str(): "");
			pPlayer->InitCloseSocketTime(CLOSE_SOCKET_TIME_CONNECT);
			pPlayer->SetStartServer(sGameServer->GetServerCode());
			pPlayer->SetAutoLogin(autologin);
			g_CurrentPlayer++;

			if ( g_CurrentPlayer >= g_EndPlayers )
				g_CurrentPlayer = g_StartPlayers;

			this->AddSession(pPlayer);

			LeaveCriticalSection(&this->PlayerAddCritical);
			return pPlayer;
		}

		count++;

		if ( count >= g_EndPlayers )
		{
			count = g_StartPlayers;
		}

		totalcount++;

		if ( totalcount >= this->GetMaxPlayers() )
		{
			break;
		}
	}

	LeaveCriticalSection(&this->PlayerAddCritical);
	return nullptr;
}

Monster * CObjectMgr::MonsterTryAdd(uint16 monster, uint16 world)
{
	Monster* pMonster = this->FindEmptySummon();

	if ( !pMonster )
	{
		sLog->outError(LOG_MONSTER, "MONSTER ADD ERROR");
		return nullptr;
	}

	pMonster->SetClass(monster);

	if ( !pMonster->SetTemplateInfo() )
		return nullptr;

	pMonster->SetWorldId(world);

	return pMonster;
}

Monster* CObjectMgr::FindEmptySummon()
{
	int32 totalcount = 0;
	int32 count = g_CurrentSummonMonster;
	
	while ( true )
	{
		Monster* pMonster = ObjectContainer[count]->ToCreature();

		if ( pMonster && pMonster->GetConnectStatus() == CONNECT_STATUS_NONE )
		{
			pMonster->reset();
			pMonster->SetConnectStatus(CONNECT_STATUS_CONNECTED);

			g_CurrentSummonMonster++;

			if ( g_CurrentSummonMonster >= g_EndSummonMonsters )
				g_CurrentSummonMonster = g_StartSummonMonsters;

			return pMonster;
		}

		count++;

		if ( count >= g_EndSummonMonsters )
		{
			count = g_StartSummonMonsters;
		}

		totalcount++;

		if ( totalcount >= this->GetMaxReservedMonsters() )
		{
			break;
		}
	}

	return nullptr;
}

void CObjectMgr::Update()
{
	if ( this->IsRespawnMonster() )
	{
		if (MyGetTickCount() > this->GetRespawnMonsterTime())
		{
			this->SetRespawnMonster(false);
			sMonsterManager->LoadMonsterExclusive();
			sMonsterManager->LoadMonster();
			sMonsterManager->LoadMonsterRespawn();
			sMonsterManager->SpawnMonsters();
		}
	}
}

bool CObjectMgr::AccountOnlineCheck(const char * account)
{
	PlayerMap const& accounts = sObjectMgr->GetAllAccounts();
	for ( PlayerMap::const_iterator it = accounts.begin(); it != accounts.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( pPlayer->IsOffline() )
			continue;

		if ( !memcmp(pPlayer->GetAccountData()->GetAccount(), account, MAX_ACCOUNT_LENGTH) )
		{
			pPlayer->PlaySoundRequest(42);
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[Attention] Someone is trying to log on your account!!.");
			return true;
		}
	}

	return false;
}

bool CObjectMgr::CharacterOnlineCheck(const char * character)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !memcmp(pPlayer->GetName(), character, MAX_CHARACTER_LENGTH) )
			return true;
	}

	return false;
}

void CObjectMgr::sendPacket(uint8 * packet, uint16 size)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		pPlayer->sendPacket(packet, size);
	}
}

void CObjectMgr::sendPacketWorld(uint16 world, uint8 * packet, uint16 size)
{
	World* pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
		return;

	WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		pPlayer->sendPacket(packet, size);
	}
}

void CObjectMgr::SendNoticeToGens(const char * name, const char * msg, uint8 family)
{
	PACKET_CHAT_DATA pMsg(name, msg);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if (pPlayer->GetGen()->GetFamily() != family)
			continue;

		if (pPlayer->ChatBlockFind(name))
		{
			continue;
		}

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void CObjectMgr::SendNoticeToAll(uint8 type, const char * msg, ...)
{
	ARG(message, msg);

	PMSG_NOTICE pMsg;
	pMsg.type = type;
	memcpy(pMsg.Notice,message,strlen(message));
	pMsg.h.size += strlen(message);

	this->sendPacket(MAKE_PCT(pMsg));
}

void CObjectMgr::SendNoticeToAll(CustomMessageID type, const char * msg, ...)
{
	ARG(Buffer, msg);

	CUSTOM_MESSAGE pMsg(type, Buffer);
	this->sendPacket(MAKE_PCT(pMsg));
}

void CObjectMgr::SendEventNotification(uint8 type, const char * msg, ...)
{
	ARG(message, msg);

	PMSG_NOTICE pMsg;
	pMsg.type = type;
	strcpy(pMsg.Notice, message);
	pMsg.h.size += strlen(message);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void CObjectMgr::SendNoticeToAdministrator(uint8 type, uint16 world, const char * msg, ...)
{
	ARG(message, msg);

	PMSG_NOTICE pMsg;
	pMsg.type = type;
	memcpy(pMsg.Notice,message,strlen(message));
	pMsg.h.size += strlen(message);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsAdministrator() )
			continue;

		if ( pPlayer->GetWorldId() != world && world != static_cast<uint16>(-1) )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void CObjectMgr::SendNoticeToAllNormal(uint8 type, const char * msg)
{
	PMSG_NOTICE pMsg;
	pMsg.type = type;
	memcpy(pMsg.Notice,msg,strlen(msg));
	pMsg.h.size += strlen(msg);

	this->sendPacket(MAKE_PCT(pMsg));
}

void CObjectMgr::SendNoticeToMap(uint16 world, uint8 type, const char * msg, ...)
{
	ARG(message, msg);

	PMSG_NOTICE pMsg;
	pMsg.type = type;
	memcpy(pMsg.Notice,message,strlen(message));
	pMsg.h.size += strlen(message);

	this->sendPacketWorld(world, MAKE_PCT(pMsg));
}

void CObjectMgr::SendPostToAll(uint16 server, const char * name, const char * msg)
{
	CUSTOM_MESSAGE pMsg(sGameServer->GetCommandPostType(), "");
	STRING_SAFE(message, 4096);
	memset(pMsg.message, 0, MAX_CHAT_LENGTH);
	std::string converted_string = TrimBlankSpaces(&msg[1]);
	std::string post_head = sGameServer->GetCommandPostHead();
	
	Util::ReplaceString(post_head, "$name", name);
	Util::ReplaceString(post_head, "$server", std::to_string(server + 1));
	Util::ReplaceString(post_head, "$message", converted_string.c_str());

	sprintf(message, "%s", post_head.c_str());
	memcpy(pMsg.message, message, 99);

	std::string lower_name = name;
	strToLower(lower_name);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsFlag(CHARACTER_FLAG_POST) )
			continue;

		if ( pPlayer->ChatBlockFind(name) )
		{
			continue;
		}

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void CObjectMgr::SendGuildMessageToAdmin(uint32 guild, const char * name, const char * msg)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsAdministrator() || !pPlayer->IsAdministratorFlag(ADMIN_FLAG_GUILD_TALK) || pPlayer->TalkingGuild != guild )
			continue;

		pPlayer->SendNoticeNormal(name,msg);
	}
}
	
void CObjectMgr::SendAllianceMessageToAdmin(uint32 guild, const char * name, const char * msg)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsAdministrator() || !pPlayer->IsAdministratorFlag(ADMIN_FLAG_GUILD_TALK) || pPlayer->TalkingAlliance != guild )
			continue;

		pPlayer->SendNoticeNormal(name,msg);
	}
}

void CObjectMgr::SendGuildNoticeToAdmin(uint32 guild, const char * name, const char * msg)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsAdministrator() || !pPlayer->IsAdministratorFlag(ADMIN_FLAG_GUILD_TALK) || pPlayer->TalkingGuild != guild )
			continue;

		pPlayer->SendNoticeNormal(NOTICE_GUILD, msg);
	}
}

void CObjectMgr::PlayerOnlineUpdate(uint32 id, const char * name, uint16 server, bool online)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetGUID() == id )
			continue;
		///- TODO: Armar sistema para Aceptar / Cancelar peticiones de amistad
		//pPlayer->FriendOnOff(name, server, online);
	}
}

Player* CObjectMgr::FindPlayerByName(const char * name)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( memcmp(pPlayer->GetName(), name, MAX_CHARACTER_LENGTH) )
			continue;

		return pPlayer;
	}

	return nullptr;
}

Player* CObjectMgr::FindPlayerByNameNoSensitive(const char * name)
{
	std::string name_lower = name;
	strToLower(name_lower);
	std::string name_tmp;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		name_tmp = pPlayer->GetName();

		strToLower(name_tmp);

		if ( name_tmp != name_lower )
			continue;

		return pPlayer;
	}

	return nullptr;
}

Player* CObjectMgr::FindPlayerByAccount(uint32 id)
{
	/// - No debería pasar, pero hay que revisar que no sea 0
	if ( !id )
		return nullptr;

	PlayerMap::const_iterator it = this->m_player_accounts.find(id);

	/// - Busco si el jugador está en la lista
	if ( it != this->m_player_accounts.end() )
	{
		return it->second;
	}

	return nullptr;
}

Player* CObjectMgr::FindPlayerByGUID(uint32 id)
{
	/// - No debería pasar, pero hay que revisar que no sea 0
	if ( !id )
	{
		return nullptr;
	}

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( pPlayer->GetGUID() != id )
		{
			continue;
		}

		return pPlayer;
	}

	return nullptr;
}

Player* CObjectMgr::FindPlayer(uint16 entry, uint32 ms_time, uint32 account_id, uint32 character_id)
{
	Player* pPlayer = this->FindPlayer(entry);

	if ( pPlayer )
	{
		if ( pPlayer->GetMSTime() != ms_time )
			return nullptr;

		if ( account_id )
		{
			if ( pPlayer->GetAccountData()->GetGUID() != account_id )
				return nullptr;
		}

		if ( character_id )
		{
			if ( pPlayer->GetGUID() != character_id )
			{
				return nullptr;
			}
		}
	}

	return pPlayer;
}

Player* CObjectMgr::FindPlayer(CUSTOM_PACKET_PLAYER const& player)
{
	Player* pPlayer = this->FindPlayer(player.entry);

	if ( pPlayer )
	{
		if ( pPlayer->GetMSTime() != player.ms_time )
			return nullptr;

		if ( player.account_id )
		{
			if ( pPlayer->GetAccountData()->GetGUID() != player.account_id )
				return nullptr;
		}

		if ( player.guid )
		{
			if ( pPlayer->GetGUID() != player.guid )
			{
				return nullptr;
			}
		}
	}

	return pPlayer;
}

std::string CObjectMgr::MakeText(std::string const& src, Unit* pSource, Unit* pTarget, uint32 time, uint32 data_1)
{
	std::string tmp = src;
	std::string str_time;
	std::string str_data_1;

	str_time = time;
	str_data_1 = data_1;

	if ( pSource )
	{
		Util::ReplaceString(tmp, "$name", pSource->GetName());

		if ( World* pWorld = pSource->GetWorld() )
			Util::ReplaceString(tmp, "$map", pWorld->GetCurrentStatusName());
		else
			Util::ReplaceString(tmp, "$map", "");

		if ( pSource->GetSummoner() && pSource->GetSummoner() != pSource )
			Util::ReplaceString(tmp, "$owner", pSource->GetSummoner()->GetName());
		else
			Util::ReplaceString(tmp, "$owner", "");

		Util::ReplaceString(tmp, "$level", std::to_string(pSource->GetLevel()));
	}
	else
	{
		Util::ReplaceString(tmp, "$name", "");
		Util::ReplaceString(tmp, "$map", "");
	}

	if ( pTarget )
	{
		Util::ReplaceString(tmp, "$tname", pTarget->GetName());

		if ( World* pWorld = pTarget->GetWorld() )
			Util::ReplaceString(tmp, "$tmap", pWorld->GetCurrentStatusName());
		else
			Util::ReplaceString(tmp, "$tmap", "");

		Util::ReplaceString(tmp, "$tlevel", std::to_string(pTarget->GetLevel()));
	}
	else
	{
		Util::ReplaceString(tmp, "$tname", "");
		Util::ReplaceString(tmp, "$tmap", "");
	}

	Util::ReplaceString(tmp, "$time", str_time);
	Util::ReplaceString(tmp, "$data_1", str_data_1);

	return tmp;
}

void CObjectMgr::SendToWeb(const char * msg)
{
	std::string url = "localhost";
	std::string msg_string = &msg[1];
	
	HINTERNET hInternet = InternetOpen(url.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, nullptr,nullptr,0);

	if( hInternet )
	{
		HINTERNET hConnect = InternetConnect(hInternet, url.c_str(), INTERNET_DEFAULT_HTTP_PORT, nullptr,nullptr, INTERNET_SERVICE_HTTP,0,1u);

		if( hConnect ) 
		{
			std::istringstream string("Json/Process.php?parametro=" + msg_string);

			HINTERNET hRequest = HttpOpenRequest(hConnect, TEXT("GET"), string.str().c_str(), nullptr, nullptr, 0, INTERNET_FLAG_KEEP_CONNECTION, 1);

			if ( hRequest ) 
			{
				HttpSendRequest(hRequest, nullptr, 0, nullptr,0);
				InternetCloseHandle(hRequest);
			}

			InternetCloseHandle(hConnect);
		}

		InternetCloseHandle(hInternet);
	}
}

Monster* MONSTER_ADD_CONTINUE(monster const* data)
{
	Monster* mMonster = sObjectMgr->FindMonster(data->GetGUID() + g_StartMonsters);

	if ( !mMonster )
	{
		return nullptr;
	}

	mMonster->reset();
	mMonster->SetClass(data->GetID());
	mMonster->SetConnectStatus(CONNECT_STATUS_CONNECTED);

	if ( !mMonster->SetTemplateInfo() )
	{
		return nullptr;
	}

	mMonster->SetDBData(data);

	if ( !sWorldMgr->IsWorldAllowed(data->GetWorld()) )
	{
		sLog->outError(LOG_DEFAULT, "%s - Attempt to add monster %u (%u) at %u %d/%d", __FUNCTION__,
			mMonster->GetEntry(), mMonster->GetClass(), mMonster->GetWorldId(), mMonster->GetX(), mMonster->GetY());
		return nullptr;
	}

	return mMonster;
}

bool CObjectMgr::ExistKalimaGate(Player* pPlayer)
{
	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->IsAIGroup(std::string("kalima_gate_ai_1 kalima_gate_ai_2 kalima_gate_ai_3 kalima_gate_ai_4 kalima_gate_ai_5 kalima_gate_ai_6 kalima_gate_ai_7")) )
			continue;

		if ( pMonster->GetSummoner() != pPlayer )
			continue;

		return true;
	}

	return false;
}

void CObjectMgr::FireworksSend(uint16 world, int32 instance, int16 x, int16 y)
{
	World* pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
		return;

	FIREWORK_SEND pMsg(0, x, y);
	Player* pPlayer = nullptr;

	WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetInstance() != instance )
			continue;

		if ( Util::Distance(pPlayer->GetX(), pPlayer->GetY(), x, y) > 20 )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

Monster* CObjectMgr::FindAIGroupLeader(uint32 Group)
{
	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsLive() || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->GetGroupAI() != Group )
			continue;

		if ( pMonster->GetGroupMemberAI() )
			continue;

		return pMonster;
	}

	return nullptr;
}

void CObjectMgr::ChangeAIOrder(uint32 Group, uint8 order)
{
	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsLive() || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->GetGroupAI() != Group )
			continue;

		pMonster->ChangeAIOrder(order);
	}
}

int32 CObjectMgr::GetPlayerCount() const
{
	return this->m_player_sessions.size();
}

uint8 CObjectMgr::GetPlayerPercent(bool real) const
{
	int32 count = sObjectMgr->GetPlayerCount();
	int32 max_count = sObjectMgr->GetMaxPlayers() - sObjectMgr->GetReservedPlayers();
	
	if (real)
	{
		max_count = sObjectMgr->GetMaxPlayers();
	}

	if (count < 0)
	{
		count = 0;
	}

	if (max_count < 0)
	{
		max_count = 0;
	}

	if (count > max_count)
	{
		count = max_count;
	}

	int32 percent = count * 100 / max_count;

	if (percent > 100)
	{
		percent = 100;
	}

	if (percent < 0)
	{
		percent = 0;
	}

	return percent;
}

/// - Esta función no es seguro para usar std::map, queda normal
int32 CObjectMgr::GetIPConnected(std::string const& ip)
{
	int32 count = 0;

	HASH_PLAYER(i)
	{
		Player* pPlayer = ObjectContainer[i]->ToPlayer();

		if (!pPlayer || pPlayer->GetConnectStatus() == CONNECT_STATUS_NONE)
			continue;

		if (std::string(pPlayer->GetAccountData()->GetIP()) != ip)
			continue;

		if (pPlayer->IsPlaying() && pPlayer->IsOffline())
		{
			continue;
		}

		++count;
	}

	return count;
}

void CObjectMgr::KickAll()
{
	Player* pPlayer = nullptr;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		KICK_PLAYER(pPlayer, "All Kick");
	}
}

void ConvertAIGroupList(std::string const& ai_group, std::vector<int32> & ai_group_list)
{
	std::string name;
	std::istringstream stream(ai_group.c_str());
	int32 group_tmp = -1;

	while (stream)
    {
        stream >> name;
		group_tmp = atoi(name.c_str());
		ai_group_list.push_back(group_tmp);
        name = "";
    }
}

void CObjectMgr::UpdateInvasionAnimation()
{
	Player* pPlayer = nullptr;
	size_t animation_count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsLive() || pPlayer->GetRegenStatus() != REGEN_NONE || pPlayer->IsServerQuit() )
			continue;

		animation_count = this->GetInvasionAnimationCount(EVENT_STATE_GOLDEN_DRAGON, pPlayer->GetWorldId());

		if ( animation_count > 0 )
		{
			this->SendInvasionAnimation(pPlayer, EVENT_STATE_RED_DRAGON, 0);
			this->SendInvasionAnimation(pPlayer, EVENT_STATE_GOLDEN_DRAGON, animation_count);
		}
		else
		{
			animation_count = this->GetInvasionAnimationCount(EVENT_STATE_RED_DRAGON, pPlayer->GetWorldId());

			this->SendInvasionAnimation(pPlayer, EVENT_STATE_GOLDEN_DRAGON, 0);
			this->SendInvasionAnimation(pPlayer, EVENT_STATE_RED_DRAGON, animation_count);
		}
	}
}

size_t CObjectMgr::GetInvasionAnimationCount(uint8 animation, uint16 world) const
{
	InvasionIndexMap::const_iterator itr = this->m_invasion_index.find(animation);

	if ( itr == this->m_invasion_index.end() )
		return 0;

	InvasionIndexWorld::const_iterator it = itr->second.find(world);

	if ( it == itr->second.end() )
		return 0;

	return it->second.size();
}

void CObjectMgr::UpdateInvasionAnimation(uint8 animation, uint16 index, uint16 world, bool add)
{
	if ( animation == uint8(-1) )
		return;

	InvasionIndexWorld & index_world = this->m_invasion_index[animation];

	InvasionIndexList & index_list = index_world[world];

	if ( add )
	{
		index_list.insert(index);
	}
	else
	{
		index_list.erase(index);
	}
}

void CObjectMgr::SendInvasionAnimation(Player* pPlayer, uint8 animation, size_t animation_count)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( !pPlayer->IsEventAnimation(animation) && animation_count > 0 )
	{
		pPlayer->SetEventAnimation(animation, true);
		pPlayer->SendEventAnimation(animation, 1);
	}

	if ( pPlayer->IsEventAnimation(animation) && animation_count <= 0 )
	{
		pPlayer->SetEventAnimation(animation, false);
		pPlayer->SendEventAnimation(animation, 0);
	}
}

void CObjectMgr::PersonalStoreDisconnect(const char * account)
{
	if ( !sGameServer->IsPersonalStoreDisconnect() )
	{
		return;
	}

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !memcmp(pPlayer->GetAccountData()->GetAccount(), account, MAX_ACCOUNT_LENGTH) )
		{
			if ( pPlayer->GetPersonalStore()->IsOff() )
			{
				pPlayer->PersonalStoreClose(false);
			}

			if ( pPlayer->GetHelper()->IsOffline() )
			{
				pPlayer->HelperStop(0x01);
				pPlayer->GetHelper()->SetOffline(false);
			}

			break;
		}
	}
}

void CObjectMgr::KickCharactersByAccount(uint32 account_id)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for (PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Player* pPlayer = it->second;

		if (!pPlayer)
			continue;

		if (pPlayer->GetAccountData()->GetGUID() == account_id)
		{
			KICK_PLAYER(pPlayer, "KickCharactersByAccount");
		}
	}
}

void CObjectMgr::AddSession(Object* s)
{
    addSessQueue.add(s);
}

void CObjectMgr::AddSession_(Object* s)
{
	if ( !s )
		return;

    m_sessions[s->GetEntry()] = s;

	if ( Player* pPlayer = s->ToPlayer() )
	{
		m_player_sessions[s->GetEntry()] = pPlayer;
	}
}

void CObjectMgr::AddSessionCharacter(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	m_player_characters[pPlayer->GetEntry()] = pPlayer;
}
	
void CObjectMgr::AddSessionAccount(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	m_player_accounts[pPlayer->GetAccountData()->GetGUID()] = pPlayer;
}

void CObjectMgr::RemoveSessionCharacter(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	m_player_characters.erase(pPlayer->GetEntry());
}
	
void CObjectMgr::RemoveSessionAccount(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	m_player_accounts.erase(pPlayer->GetAccountData()->GetGUID());
}

void CObjectMgr::RemoveSessionPlayer(Player* pPlayer)
{
	if ( !pPlayer )
		return;

	m_player_sessions.erase(pPlayer->GetEntry());
}

void CObjectMgr::UpdateSessions(uint32 diff)
{
	///- Add new sessions
    Object* sess = nullptr;
    while (addSessQueue.next(sess))
        AddSession_ (sess);

	Object* pSession = nullptr;

	uint32 oldMSTime = getMSTime();
	static uint32 maxMSTime = 0;
	uint32 diffMSTime = 0;

	///- Then send an update signal to remaining ones
	for (SessionMap::iterator itr = m_sessions.begin(), next; itr != m_sessions.end(); itr = next)
    {
        next = itr;
        ++next;

		 ///- and remove not active sessions from the list
		pSession = itr->second;

		if (!pSession || pSession->GetConnectStatus() == CONNECT_STATUS_NONE || !pSession->Update(diff))
		{
			m_sessions.erase(itr);
		}
	}

	diffMSTime = GetMSTimeDiffToNow(oldMSTime);

	if ( diffMSTime > maxMSTime )
	{
		maxMSTime = diffMSTime;
		sLog->outInfo("root", "Updating Objets %u - Time: %u ms", m_sessions.size(), diffMSTime);
	}
}

void CObjectMgr::UpdateSessionsViewport(uint32 diff)
{
	Object* pSession = nullptr;
	uint32 oldMSTime = getMSTime();
	static uint32 maxMSTime = 0;
	uint32 diffMSTime = 0;

	for (SessionMap::iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
	{
		pSession = itr->second;

		if ( !pSession )
			continue;

		if ( pSession->GetConnectStatus() != CONNECT_STATUS_PLAYING )
			continue;

		if ( pSession->IsLive() )
		{
			pSession->UpdateStatus();
		}
	}

	for (SessionMap::iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
	{
		pSession = itr->second;

		if ( !pSession )
			continue;

		if ( pSession->GetConnectStatus() != CONNECT_STATUS_PLAYING )
			continue;

		if ( pSession->IsLive() )
		{
			pSession->ViewportUpdate(diff);
		}
	}

	diffMSTime = GetMSTimeDiffToNow(oldMSTime);

	if ( diffMSTime > maxMSTime )
	{
		maxMSTime = diffMSTime;
		sLog->outInfo("root", "Updating Viewport Objets %u - Time: %u ms", m_sessions.size(), diffMSTime);
	}
}

int32 CObjectMgr::GetPlayerCountOnInstance(uint16 world_id, int32 instance) const
{
	int32 count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr )
	{
		Player* pPlayer = itr->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != world_id )
		{
			continue;
		}

		if ( pPlayer->GetInstance() != instance )
		{
			continue;
		}

		++count;
	}

	return count;
}

void CObjectMgr::SendPacketToInstance(uint16 world_id, int32 instance, uint8 * Packet, uint16 size)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr )
	{
		Player* pPlayer = itr->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != world_id )
		{
			continue;
		}

		if ( pPlayer->GetInstance() != instance )
		{
			continue;
		}

		pPlayer->sendPacket(Packet, size);
	}
}

void CObjectMgr::CharacterAutoLogin()
{
	if ( !sGameServer->IsAutoLoginEnabled() )
	{
		return;
	}

	if ( this->IsAutoLogin() )
	{
		return;
	}

	if ( !this->GetAutoLoginTime()->Elapsed(5000) )
	{
		return;
	}

	this->SetAutoLogin(true);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CHARACTER_SELECT);
	stmt->setUInt16(0, sGameServer->GetServerCode());

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			Player* pPlayer = sObjectMgr->PlayerAdd(nullptr, true);

			if ( pPlayer )
			{
				pPlayer->SetAutoLogin(true);

				pPlayer->GetAccountData()->SetVIPStatus(fields[0].GetInt32());
				pPlayer->GetAccountData()->SetVIPDuration(fields[1].GetInt64());
				pPlayer->GetWarehouse()->SetExpanded(fields[2].GetUInt8());
				pPlayer->GetWarehouse()->SetExpandedTime(fields[3].GetInt64());
				pPlayer->GetAccountData()->SetSpecialCharacter(fields[4].GetUInt16());

				pPlayer->SetAutoLoginAccount(fields[7].GetUInt32());
				pPlayer->SetAutoLoginCharacter(fields[8].GetUInt32());
				pPlayer->SetAutoLoginType(fields[9].GetUInt8());

				pPlayer->GetAccountData()->SetIP(fields[10].GetCString());
				pPlayer->GetAccountData()->SetMac(fields[11].GetCString());
				pPlayer->GetAccountData()->SetDiskSerial(fields[12].GetUInt32());

				pPlayer->GetTimer(PLAYER_TIMER_AUTOLOGIN)->Start((sGameServer->GetAutoLoginRandomTime() + Random(sGameServer->GetAutoLoginTime())) * IN_MILLISECONDS);
			}
		}
		while(result->NextRow());
	}
}