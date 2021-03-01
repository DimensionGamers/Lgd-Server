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

void LoginDatabaseConnection::DoPrepareStatements()
{
	if (!m_reconnecting)
		m_stmts.resize(MAX_LOGIN_STATEMENTS);

	PrepareStatement(QUERY_LOGIN_GAMESERVER, "UPDATE server_list SET online = ? WHERE code = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_GAMESERVER_SELECT, "SELECT s.code, s.name, s.port, s.ip, s.flag, s.online, s.default_world, s.default_x, s.default_y, s.type FROM server_list s WHERE s.code = ?", CONNECTION_BOTH);

	/* ACCOUNTS */
	PrepareStatement(QUERY_LOGIN_ACCOUNT_LOGIN, "SELECT a.guid, a.blocked, a.security_code, b.online, c.account_id, a.facebook_status, a.golden_channel, "
		"d.account_id, e.account_id, e.block_date, a.secured, a.account, f.account_id, f.unban_date, g.disk_serial "
		"FROM accounts AS a "
		"LEFT JOIN accounts_status AS b ON a.guid = b.account_id AND b.server_group = ? "
		"LEFT JOIN accounts_allowed AS c ON a.guid = c.account_id AND c.server = ? "
		"LEFT JOIN accounts_validation AS d ON a.guid = d.account_id AND d.disk_serial = ? "
		"LEFT JOIN accounts_warning AS e ON a.guid = e.account_id AND e.disk_serial = ? "
		"LEFT JOIN accounts_banned AS f ON a.guid = f.account_id "
		"LEFT JOIN master_pc AS g ON g.disk_serial = ? AND g.mac = ? "
		"WHERE ((a.account = ? AND a.password = ?) OR (a.guid = ?) OR (a.account = ? AND g.disk_serial = ? AND g.mac = ?)) AND a.guid != 0", CONNECTION_BOTH);

	PrepareStatement(QUERY_LOGIN_ACCOUNT_STATUS_DELETE, "DELETE FROM accounts_status WHERE account_id = ? AND server_group = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_ACCOUNT_STATUS_UPDATE, "INSERT INTO accounts_status (account_id, server_group, current_server, start_server, dest_server, dest_world, dest_x, dest_y, warp_time, warp_auth_1, warp_auth_2, warp_auth_3, warp_auth_4, last_ip, last_mac, last_online, online, disk_serial) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_ACCOUNT_BLOCK, "UPDATE accounts SET blocked = ? WHERE guid = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_ACCOUNT_CLEAN, "UPDATE accounts_status SET online = 0", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_ACCOUNT_TYPE_UPDATE, "UPDATE accounts_status AS i SET i.type = ? WHERE i.account_id = ? AND i.server_group = ?", CONNECTION_ASYNC);

	/* ITEM SERIAL */
	PrepareStatement(QUERY_LOGIN_ITEM_SERIAL_SELECT, "SELECT i.server, i.serial, i.serial_shop FROM item_serial i WHERE i.server = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_LOGIN_ITEM_SERIAL_UPDATE, "REPLACE INTO item_serial (server, serial, serial_shop) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	/* ACCOUNTS SECURITY */
	PrepareStatement(QUERY_LOGIN_ACCOUNT_SECURITY_DELETE, "DELETE FROM accounts_security WHERE account_id = ? AND accounts_security.ip = ? AND mac = ? AND disk_serial = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_ACCOUNT_SECURITY_INSERT, "INSERT INTO accounts_security (account_id, account, ip, mac, disk_serial) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	/* CHARACTER NOTIFICATION */
	PrepareStatement(QUERY_LOGIN_CHARACTER_NOTIFICATION, "INSERT IGNORE INTO character_notification (server_group, char_name, facebook_id, notification_id, notification_data) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_ACCOUNTS_DISCONNECT_SELECT, "SELECT i.account_id, i.masive FROM accounts_disconnect i WHERE i.server = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_LOGIN_ACCOUNTS_DISCONNECT_DELETE, "DELETE FROM accounts_disconnect WHERE server = ? AND account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_BLOCK_MAC_SELECT, "SELECT 1 FROM block_mac i WHERE i.mac = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_LOGIN_BLOCK_DISK_SERIAL_SELECT, "SELECT 1 FROM block_diskserial i WHERE i.disk_serial = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_LOGIN_BLOCK_IP_SELECT, "SELECT 1 FROM block_ip i WHERE i.ip = ?", CONNECTION_SYNCH);

	PrepareStatement(QUERY_LOGIN_COMPLETE_BAN_ACCOUNT_1, "UPDATE accounts AS i SET i.blocked = 1 WHERE i.guid IN (SELECT a.account_id FROM accounts_security a WHERE a.disk_serial = ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_COMPLETE_BAN_ACCOUNT_2, "UPDATE accounts AS i SET i.blocked = 1 WHERE i.guid IN (SELECT a.account_id FROM accounts_security a WHERE a.mac = ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_COMPLETE_BAN_ACCOUNT_3, "UPDATE accounts AS i SET i.blocked = 1 WHERE i.guid IN (SELECT a.account_id FROM accounts_security a WHERE a.ip = ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_COMPLETE_BAN_INSERT_1, "INSERT IGNORE INTO block_diskserial (disk_serial, comment) VALUES (?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_COMPLETE_BAN_INSERT_2, "INSERT IGNORE INTO block_mac (mac, comment) VALUES (?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_COMPLETE_BAN_INSERT_3, "INSERT IGNORE INTO block_ip (ip, comment) VALUES (?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_ACCOUNTS_VALIDATION_INSERT, "INSERT IGNORE INTO accounts_validation (account_id, disk_serial) VALUES (?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_ACCOUNTS_WARNING_INSERT, "INSERT IGNORE INTO accounts_warning (account_id, disk_serial, block_date) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_LOGIN_ACCOUNTS_WARNING_DELETE, "DELETE FROM accounts_warning WHERE account_id = ? AND disk_serial = ?", CONNECTION_ASYNC);


	PrepareStatement(QUERY_LOGIN_GOLDEN_CHANNEL_SELECT, "SELECT i.golden_channel FROM accounts AS i WHERE i.guid = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_LOGIN_GOLDEN_CHANNEL_UPDATE, "UPDATE accounts AS i SET i.golden_channel = ? WHERE i.guid = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_LOGIN_ACCOUNTS_STATUS_COUNT_SELECT, "SELECT COUNT(i.account_id) FROM accounts_status AS i WHERE i.account_id = ?", CONNECTION_SYNCH);

	//PrepareStatement(QUERY_LOGIN_WHITE_LIST_SELECT, "SELECT 1 FROM white_list i WHERE i.ip = ? OR i.mac = ? OR i.disk_serial = ?", CONNECTION_ASYNC);

	//PrepareStatement(QUERY_LOGIN_MASTER_PC, "SELECT i.disk_serial FROM master_pc i WHERE i.disk_serial = ?", CONNECTION_SYNCH);
}