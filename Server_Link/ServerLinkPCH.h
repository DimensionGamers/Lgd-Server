#ifndef SERVERLINK_PCH_H
#define SERVERLINK_PCH_H

#include "CommonPCH.h"

class ServerSocket;

#include "Main.h"
#include "Character.h"
#include "ServerSocket.h"
#include "ServerSocketMgr.h"

#include "CharacterCreateQueue.h"
#include "GuildCreateQueue.h"
#include "GuildMgr.h"

#include "CastleSiege.h"
#include "Crywolf.h"

#include "GuildMatching.h"
#include "ArkaWar.h"

#define sMain MainApplication::instance()
#define sServerSocketMgr ServerSocketMgr::Instance()
#define sCharacterCreateQueue SingNull(CharacterCreateQueue)
#define sGuildCreateQueue SingNull(GuildCreateQueue)
#define sCharacterMgr SingNull(CharacterMgr)
#define sGuildMgr SingNull(GuildMgr)
#define sCastleSiege SingNull(CastleSiege)
#define sCrywolf SingNull(Crywolf)
#define sGuildMatching SingNull(GuildMatching)
#define sArkaWar SingNull(ArkaWar)

#endif