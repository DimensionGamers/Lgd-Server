#ifndef CONNECT_SERVER_PCH
#define CONNECT_SERVER_PCH

#include "CommonPCH.h"

class MainSocket;
class GSSocket;

#include "ServerList.h"
#include "Main.h"

#include "MainSocket.h"
#include "MainSocketMgr.h"

#include "GSSocket.h"
#include "GSSocketMgr.h"

#define sServer SingNull(CServerList)
#define sMain SingNull(MainApplication)
#define sClientMgr SingNull(ClientMgr)
#define sMainSocketMgr MainSocketMgr::Instance()
#define sGSSocketMgr GSSocketMgr::Instance()

#endif