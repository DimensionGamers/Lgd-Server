#ifndef IMP_MU_LOG_H
#define IMP_MU_LOG_H

enum MuLogDatabaseStatements
{
	MULOG_STATEMENT_ITEM_DROP,
	MULOG_STATEMENT_ITEM_LOOT,
	MULOG_STATEMENT_ITEM_SELL,
	MULOG_STATEMENT_ITEM_BUY,

	MULOG_QUERY_ITEM_BAG_SERIAL_INSERT,

	MULOG_QUERY_LEVEL_UP,

	MULOG_STATEMENT_JEWEL_DROP,

	MULOG_STATEMENT_CASH_SHOP,

	MULOG_STATEMENT_MIX,

	MULOG_BOSS_KILL,

	MULOG_STATEMENT_JEWEL_MIX,
	MULOG_STATEMENT_ITEM_CREATE,

	MULOG_STATEMENT_LOG_PERSONAL_STORE,
	MULOG_STATEMENT_LOG_TRADE,

	MULOG_STATEMENT_ANTIHACK,

	MULOG_STATEMENT_ERRTEL_PENTAGRAM,

	LOG_STATEMENT_MIX_RECOVERY,
	LOG_STATEMENT_REWARD_DUNGEON,
	LOG_STATEMENT_REWARD_LABYRINTH_OF_DIMENSIONS,

	LOG_STATEMENT_PVP,

	MAX_MULOG_STATEMENTS
};

class MuLogDatabaseConnection : public MySQLConnection
{
    public:
		typedef MuLogDatabaseStatements Statements;

        //- Constructors for sync and async connections
        MuLogDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo) { }
        MuLogDatabaseConnection(ProducerConsumerQueue<SQLOperation*>* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo) { }

        //- Loads database type specific prepared statements
        void DoPrepareStatements() override;
};

typedef DatabaseWorkerPool<MuLogDatabaseConnection> MuLogDatabaseWorkerPool;

#endif