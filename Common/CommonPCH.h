#ifndef COMMON_PCH_H
#define COMMON_PCH_H

#define UPDATE_GET_TICK_COUNT			0

#define USING_OLD_CONFIG

#pragma warning(disable: 4355)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <istream>
#include <algorithm>
#include <float.h>
#include <iterator>
#include <stdexcept>
#include <limits.h>
#include <random>
#include <locale>
#include <stdarg.h>
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <concurrent_queue.h>
#include <concurrent_vector.h>
#include <stdint.h>
#include <memory>
#include <exception>
#include <type_traits>
#include <functional>

#include <future>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <chrono>
#include <thread>
#include <boost/format.hpp>

#include <new>
#include <emmintrin.h>                 // Define SSE2 intrinsics

#include <sys/types.h>

#ifdef _WIN32
  #include <winsock2.h>
#endif

#include <mysql.h>
#include <mysqld_error.h>
#include <errmsg.h>

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

class Config;

#include "CompilerDefs.h"
#include "Define.h"

#if COMPILER == COMPILER_INTEL
#include <ext/hash_map>
#elif COMPILER == COMPILER_GNU && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 3)
#include <tr1/unordered_map>
#elif COMPILER == COMPILER_GNU && __GNUC__ >= 3
#include <ext/hash_map>
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1500 && _HAS_TR1    // VC9.0 and later
#include <unordered_map>
#else
#include <hash_map>
#endif

#include "Sha256.h"

#include "randomc.h"                   // Define integer types etc
#include "SFMT.h"
#include "UnorderedMapSet.h"
#include "Errors.h"
#include "Common.h"
#include "Timer.h"
#include "core.h"
#include "checked.h"
#include "SingletonInc.h"
#include "MD5KEY.h"
#include "MD5.h"

#include "MiniDump.h"

#include "Util.h"
#include "PacketDefine.h"
#include "CustomPacket.h"
#include "base64.h"
#include "LargeRandom.h"
#include "EncDec.h"
#include "FreeImage.h"
#include "RandomValueMgr.h"
#include "Readscript.h"
#include "ServerLinkPacket.h"
#include "ConnectServerPacket.h"
#include "LoginServerPacket.h"
#include "SystemTimer.h"
#include "VarTemplate.h"
#include "TickTimer.h"
#include "LockedQueue.h"

#include "Config.h"
#include "Appender.h"
#include "AppenderConsole.h"
#include "AppenderFile.h"
#include "Logger.h"
#include "LogOperation.h"
#include "Log.h"
#include "ProcessPriority.h"

extern Config* sConfig;
extern Config* sConfigCommon;

#define sLog Log::instance()

#include "ProducerConsumerQueue.h"
#include "Field.h"
#include "FieldReader.h"
#include "QueryResult.h"
#include "SQLOperation.h"
#include "TransactionCallBack.h"
#include "Transaction.h"
#include "MySQLConnection.h"
#include "PreparedStatement.h"
#include "QueryHolder.h"
#include "MySQLThreading.h"
#include "DatabaseWorker.h"
#include "MuDatabase.h"
#include "GameServerDatabase.h"
#include "MuLogDatabase.h"
#include "LoginDatabase.h"
#include "MuEditorDatabase.h"
#include "AdhocStatement.h"
#include "Callback.h"
#include "DatabaseWorkerPool.h"
#include "DatabaseEnv.h"

#include "MessageBuffer.h"
#include "ByteConverter.h"
#include "ByteBuffer.h"
#include "Socket.h"
#include "NetworkThread.h"
#include "AsyncAcceptor.h"
#include "SocketMgr.h"
#include "WorldPacket.h"
#include "TCPClient.h"

#include "Settings.h"
#include "App.h"
#include "Security.h"

enum ShutdownExitCode
{
    SHUTDOWN_EXIT_CODE = 0,
    ERROR_EXIT_CODE    = 1,
    RESTART_EXIT_CODE  = 2,
};


#define sCustomUtil CCustomUtil::instance()
#define sEncDec SingNull(EncodeDecode)
#define sOpcodeMgr SingNull(OpcodeMgr)
#define sSecurity SingNull(Security)
#define sMiniDump SingNull(MiniDump)

#endif