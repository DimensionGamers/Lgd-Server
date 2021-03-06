/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "GameServerPCH.h"
*
*/
#ifndef GAME_PCH_H
#define GAME_PCH_H

//#define USING_STD_THREAD
#define USING_SINGLE_VIEWPORT
//#define USING_GLOBAL_VIEWPORT
//#define USING_NORMAL_STAND_ATTRIBUTE

/*
#pragma warning(disable: 4711)
#pragma warning(disable: 4738)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 4365) // conversion from 'int' to 'uint8', signed/unsigned mismatch
#pragma warning(disable: 4625) // copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4706) // assignment within conditional expression
#pragma warning(disable: 4061) // enumerator 'MYSQL_TYPE_SET' in switch of enum 'enum_field_types' is not explicitly handled by a case label
#pragma warning(disable: 4062) // enumerator 'MAIN_OBJECT_NONE' in switch of enum 'MainObjectType' is not handled
#pragma warning(disable: 4481) // nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4571) // Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning(disable: 4263) // member function does not override any base class virtual member function
#pragma warning(disable: 4264) // no override available for virtual member function from base 'ACE_Svc_Handler<_ACE_PEER_STREAM,_ACE_SYNCH>'; function is hidden
#pragma warning(disable: 4619) // #pragma warning : there is no warning number '4284'
#pragma warning(disable: 4986) // 'operator new[]': exception specification does not match previous declaration
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
#pragma warning(disable: 4121) // alignment of a member was sensitive to packing*/
//#pragma warning(disable: 4710)
//#pragma warning(disable: 4100)
//#pragma warning(disable: 4706)
#pragma warning(disable: 4800)
#pragma warning(disable: 4150)

//#define TRY_ENABLED

#ifdef TRY_ENABLED
#define TRY_START \
	try \
	{

#define TRY_END \
	} \
	catch(...) \
	{ \
		sLog->outError(LOG_CRASH, "%s %s %d",__FUNCTION__,__FILE__,__LINE__); \
	}

#define TRY_END_RETURN(value) \
	} \
	catch(...) \
	{ \
		sLog->outError(LOG_CRASH, "%s %s %d",__FUNCTION__,__FILE__,__LINE__); \
		return value; \
	}
#else
#define TRY_START {}
#define TRY_END {}
#define TRY_END_RETURN(value) {}
#endif

class Object;
class Player;
class Monster;
class Unit;
class WorldItem;
class World;
class Skill;
class Item;
class Party;
class WorldSocket;
class Invasion;
class Shop;
class Guild;
class GamePacket;
class ChatHandler;
class MixHandler;
struct AIData;
//struct MonsterAI;
struct GateData;
struct TeleportData;
struct skill_template;
struct monster_ai_unit;
struct monster_ai_automata;
struct monster_ai_state;
struct monster_ai_element;
struct CrownBasicAI;
struct CrownSwitchBasicAI;

struct item_template;

struct GenRewardData;
struct CharacterBaseData;
class LoginQueryHolder;
class WarehouseQueryHolder;
class CharacterListQueryHolder;
struct QuestEvolution;
struct QuestEvolutionData;
struct buff_template;
struct JewelTypeData;
struct ResetData;

class MuRoomyInfo;

#include "CommonPCH.h"

//#include "..\Dependencies\cryptopp800\cryptlib.h"
//#include "..\Dependencies\cryptopp800\rijndael.h"

#include "CommonDef.h"
#include "Main.h"
#include "WorldDef.h"
#include "WorldPath.h"
#include "Object.h"
#include "BuffDef.h"
#include "QuestDef.h"
#include "LabyrinthDimensionsDef.h"
#include "PlayerDef.h"
#include "EventCommon.h"
#include "EventMassive.h"
#include "ItemBagDef.h"
#include "ItemDef.h"
#include "PentagramSystemDef.h"
#include "BloodCastleDef.h"
#include "DevilSquareDef.h"
#include "ChaosCastleDef.h"
#include "ImperialFortressDef.h"
#include "IllusionTempleDef.h"
#include "CrywolfDef.h"
#include "BattleSoccerDef.h"
#include "CastleSiegeDef.h"
#include "RaklionDef.h"
#include "KanturuDef.h"
#include "PartyDef.h"
#include "TormentedSquareDef.h"
#include "SkillDef.h"
#include "MonsterDef.h"
#include "MessageDef.h"
#include "CashShopDef.h"
#include "NpcHandler.h"
#include "MixHandler.h"
#include "Invasion.h"
#include "JewelMix.h"
#include "Item.h"
#include "Skill.h"
#include "WorldItem.h"
#include "WorldClass.h"
#include "MonsterThreat.h"
#include "Unit.h"
#include "Monster.h"
#include "ItemBag.h"
#include "GameServer.h"
#include "DuelMgr.h"
#include "Guild.h"
#include "ChatHandler.h"
#include "Party.h"
#include "PartyMgr.h"
#include "WorldManager.h"
#include "SkillMgr.h"
#include "ItemMgr.h"
#include "QuestMgr.h"
#include "GenMgr.h"
#include "GuildMgr.h"
#include "GuildWar.h"
#include "ShopMgr.h"
#include "TeleportManager.h"
#include "CashShop.h"
#include "CharacterBase.h"
#include "VIPMgr.h"
#include "StoreScript.h"
#include "TradeScript.h"
#include "InventoryScript.h"
#include "WarehouseScript.h"
#include "MuunScript.h"
#include "NPCSellScript.h"
#include "EventInventoryScript.h"
#include "System_Message.h"
#include "BloodCastle.h"
#include "CastleSiege.h"
#include "CastleDeep.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "ChaosCastleSurvival.h"
#include "Doppelganger.h"
#include "Crywolf.h"
#include "ImperialFortressInstance.h"
#include "ImperialFortress.h"
#include "BattleSoccer.h"
#include "Raklion.h"
#include "KanturuStandBy.h"
#include "KanturuNightmare.h"
#include "KanturuMaya.h"
#include "KanturuTower.h"
#include "Kanturu.h"
#include "IllusionTemple.h"
#include "HappyHour.h"
#include "SkillHandler.h"
#include "MonsterAIHandler.h"
#include "ScrambleDef.h"
#include "Scramble.h"
#include "CommonRace.h"
#include "DungeonRace.h"
#include "LosttowerRace.h"
#include "MixMgr.h"
#include "ArkaWar.h"
#include "PentagramSystem.h"
#include "MuunSystem.h"
#include "MiniMap.h"
#include "SummonScroll.h"
#include "MuRoomy.h"
#include "EventInventory.h"
#include "GremoryCaseScript.h"
#include "PersonalStoreScript.h"
#include "LastManStanding.h"
#include "SocketSystem.h"
#include "HuntingRecord.h"
#include "WorldBoss.h"

#include "GenPacket.h"
#include "EventPacket.h"
#include "QuestPacket.h"
#include "GuildPacket.h"
#include "ClientPacket.h"

#include "MonsterAI.h"
#include "ScriptAI.h"
#include "ai_basic.h"
#include "ai_summon_player.h"

#include "ObjectManager.h"

#include "MonsterManager.h"
#include "EventManager.h"

#include "spe.h"
#include "WorldSocket.h"
#include "WorldSocketMgr.h"
#include "ServerToServer.h"

#include "Player.h"
#include "MiningSystem.h"
#include "GuildMatching.h"
#include "PartyMatching.h"
#include "RuudSystem.h"
#include "ProtectorOfAcheron.h"
#include "TormentedSquare.h"
#include "TormentedSquareSurvival.h"
#include "GremoryCase.h"
#include "Evomon.h"
#include "MiniBomb.h"
#include "JewelBingo.h"
#include "NumericBaseball.h"
#include "MossMerchant.h"
#include "NixiesLake.h"
#include "LabyrinthDimensions.h"
#include "SwampOfDarkness.h"
#include "DungeonDef.h"
#include "DungeonInstance.h"
#include "Dungeon.h"
#include "FormulaData.h"
#include "MonsterSoul.h"

#include "GamePacket.h"

#define WM_GM_SERVER_LINK_MSG_PROC		0x403
#include "ServerLink.h"
#include "AuthServer.h"
#include "ConnectServer.h"

#include <WinInet.h>
#pragma comment(lib, "WININET.LIB")


#define sMessage SystemMessage::instance()
#define sCashShopMgr CashShopMgr::instance()
#define sGenMgr CGenMgr::instance()
#define sJewelMix JewelMix::instance()
#define sDuelMgr DuelMgr::instance()
#define sPartyMgr PartyMgr::instance()
#define sItemBagMgr ItemBagMgr::instance()
#define sShopMgr ShopMgr::instance()
#define sVipMgr CVipMgr::instance()
#define sCrywolf Crywolf::instance()
#define sKanturuMgr KanturuMgr::instance()
#define sBattleSoccerMgr CBattleSoccerMgr::instance()
#define sBloodCastleMgr CBloodCastleMgr::instance()
#define sCastleSiege CCastleSiege::instance()
#define sChaosCastleMgr CChaosCastleMgr::instance()
#define sDevilSquareMgr DevilSquareMgr::instance()
#define sImperialFortressMgr ImperialFortressMgr::instance()
#define sDoppelganger SingNull(Doppelganger)
#define sTeleport TeleportManager::instance()
#define sIllusionTempleMgr SingNull(IllusionTempleMgr)
#define sEventMgr SingNull(CEventMgr)
#define sInvasionMgr SingNull(InvasionMgr)
#define sRaklion SingNull(Raklion)
#define sWorldMgr SingNull(WorldMgr)
#define sSkillMgr SingNull(SkillMgr)
#define sQuestMgr SingNull(CQuestMgr)
#define sItemMgr SingNull(CItemMgr)
#define sPetExperience SingNull(CPetItemExperience)
#define sHappyHour SingNull(HappyHour)
#define sLucky SingNull(LuckyEvent)
#define sScriptAI SingNull(ScriptAIMgr)
#define sScramble SingNull(Scramble)
#define sDungeonRace SingNull(DungeonRace)
#define sLosttowerRace SingNull(LosttowerRace)
#define sMixMgr SingNull(MixMgr)
#define sWorldSocketMgr WorldSocketMgr::Instance()
#define sGuildWarMgr SingNull(GuildWarMgr)
#define sAuthServer SingNull(AuthServer)
#define sConnectServer SingNull(ConnectServer)

#define sServerToServer SingNull(ServerToServer)
#define sArkaWar SingNull(ArkaWar)
#define sPentagramSystem SingNull(PentagramSystem)
#define sMuunSystem SingNull(MuunSystem)
#define sMiniMap SingNull(MiniMap)
#define sSummonScroll SingNull(SummonScroll)
#define sEventInventory SingNull(EventInventory)
#define sMiningSystem SingNull(MiningSystem)
#define sMuRoomy SingNull(MuRoomy)
#define sGuildMatching SingNull(GuildMatching)
#define sPartyMatching SingNull(PartyMatching)
#define sChaosCastleSurvivalMgr SingNull(ChaosCastleSurvivalMgr)
#define sRuudSystem SingNull(RuudSystem)
#define sProtectorOfAcheron SingNull(ProtectorOfAcheron)
#define sTormentedSquare SingNull(TormentedSquare)
#define sTormentedSquareSurvival SingNull(TormentedSquareSurvival)
#define sGremoryCase SingNull(GremoryCase)
#define sEvomon SingNull(Evomon)
#define sLastManStanding SingNull(LastManStanding)
#define sSocketSystem SingNull(SocketSystem)
#define sHuntingRecord SingNull(HuntingRecord)
#define sNixiesLake SingNull(NixiesLake)
#define sLabyrinthDimensions SingNull(LabyrinthDimensions)
#define sCastleDeep SingNull(CastleDeep)
#define sWorldBoss SingNull(WorldBoss)
#define sMiniBomb SingNull(MiniBomb)
#define sJewelBingo SingNull(JewelBingo)
#define sMossMerchant SingNull(MossMerchant)
#define sSwampOfDarkness SingNull(SwampOfDarkness)
#define sNumericBaseball SingNull(NumericBaseball)
#define sDungeon SingNull(Dungeon)
#define sFormulaMgr SingNull(FormulaMgr)
#define sMonsterSoul SingNull(MonsterSoul)

#endif