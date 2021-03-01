#ifndef DATABASEENV_H
#define DATABASEENV_H

#define _LIKE_           "LIKE"
#define _TABLE_SIM_      "`"
#define _CONCAT3_(A, B, C) "CONCAT( " A ", " B ", " C " )"
#define _OFFSET_         "LIMIT %d, 1"

extern MuDatabaseWorkerPool MuDatabase;
extern GameServerDatabaseWorkerPool GameServerDatabase;
extern MuLogDatabaseWorkerPool MuLogDatabase;
extern LoginDatabaseWorkerPool LoginDatabase;
extern MuEditorDatabaseWorkerPool EditorDatabase;

#endif
