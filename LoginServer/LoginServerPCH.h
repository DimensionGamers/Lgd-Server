#ifndef CONNECT_SERVER_PCH
#define CONNECT_SERVER_PCH

#include "CommonPCH.h"

class AuthSocket;

#include "ServerList.h"
#include "Main.h"

#include "AuthSocket.h"
#include "AuthSocketMgr.h"

#include "LoginQueue.h"

#define sServer SingNull(CServerList)
#define sMain SingNull(MainApplication)
#define sClientMgr SingNull(ClientMgr)
#define sAuthSocketMgr AuthSocketMgr::Instance()
#define sLoginQueue SingNull(LoginQueue)

#endif