/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WORLDDATABASE_H
#define _WORLDDATABASE_H

enum WorldDatabaseStatements
{
	QUERY_SRVCFG_EVENT_MANAGER_SELECT,
	QUERY_SRVCFG_EVENT_SEASON_MANAGER_SELECT,
	QUERY_SRVCFG_MONSTER_SELECT,
	QUERY_SRVCFG_MONSTER_SELECT_GUID,
	QUERY_SRVCFG_MONSTER_RESPAWN_LOCATION_SELECT,
	QUERY_SRVCFG_NOTICE_SELECT,
	QUERY_SRVCFG_MISC_NON_PK_TIME_SELECT,
	QUERY_SRVCFG_MONSTER_DISABLE,
	QUERY_SRVCFG_MONSTER_ADD,
	QUERY_SRVCFG_MONSTER_MAX_GUID,

	QUERY_GAME_INSERT_QUEST,

	QUERY_SRVCFG_MAX
};

class GameServerDatabaseConnection : public MySQLConnection
{
	public:
		typedef WorldDatabaseStatements Statements;
  
        //- Constructors for sync and async connections
        GameServerDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo) { }
        GameServerDatabaseConnection(ProducerConsumerQueue<SQLOperation*>* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo) { }

        //- Loads database type specific prepared statements
        void DoPrepareStatements() override;
};

typedef DatabaseWorkerPool<GameServerDatabaseConnection> GameServerDatabaseWorkerPool;

#endif
