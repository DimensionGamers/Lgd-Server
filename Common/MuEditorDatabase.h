#ifndef IMP_MU_EDITOR_H
#define IMP_MU_EDITOR_H

enum MuEditorDatabaseStatements
{
	QUERY_EDITOR_DELETE_ITEM,
	QUERY_EDITOR_SAVE_ITEM,
	QUERY_EDITOR_LOAD_ITEM,
	//QUERY_EDITOR_INSERT_ITEM,
	QUERY_EDITOR_UPDATE_ITEM,	


	QUERY_EDITOR_DELETE_ITEM_TOOL_TIP,
	QUERY_EDITOR_SAVE_ITEM_TOOL_TIP,
	QUERY_EDITOR_LOAD_ITEM_TOOL_TIP,

	QUERY_EDITOR_DELETE_ITEM_TOOL_TIP_TEXT,
	QUERY_EDITOR_SAVE_ITEM_TOOL_TIP_TEXT,
	QUERY_EDITOR_LOAD_ITEM_TOOL_TIP_TEXT,

	QUERY_EDITOR_DELETE_ITEM_LEVEL_TOOL_TIP,
	QUERY_EDITOR_SAVE_ITEM_LEVEL_TOOL_TIP,
	QUERY_EDITOR_LOAD_ITEM_LEVEL_TOOL_TIP,

	MAX_MUEDITOR_STATEMENTS
};

class MuEditorDatabaseConnection : public MySQLConnection
{
    public:
		typedef MuEditorDatabaseStatements Statements;

        //- Constructors for sync and async connections
        MuEditorDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo) { }
        MuEditorDatabaseConnection(ProducerConsumerQueue<SQLOperation*>* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo) { }

        //- Loads database type specific prepared statements
        void DoPrepareStatements() override;
};

typedef DatabaseWorkerPool<MuEditorDatabaseConnection> MuEditorDatabaseWorkerPool;

#endif