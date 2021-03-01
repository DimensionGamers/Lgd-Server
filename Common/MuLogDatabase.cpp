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

void MuLogDatabaseConnection::DoPrepareStatements()
{
	if (!m_reconnecting)
		m_stmts.resize(MAX_MULOG_STATEMENTS);

	PrepareStatement(MULOG_STATEMENT_ITEM_DROP, "INSERT DELAYED INTO log_item_drop (server, account_id, account, char_id, char_name, player_data, item_data, `comment`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(MULOG_STATEMENT_ITEM_LOOT, "INSERT DELAYED INTO log_item_loot (server, account_id, account, char_id, char_name, player_data, item_data, `comment`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(MULOG_STATEMENT_ITEM_SELL, "INSERT DELAYED INTO log_item_sell (server, account_id, account, char_id, char_name, player_data, item_data, `comment`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(MULOG_STATEMENT_ITEM_BUY, "INSERT DELAYED INTO log_item_buy (server, account_id, account, char_id, char_name, player_data, item_data, `comment`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_QUERY_ITEM_BAG_SERIAL_INSERT, "INSERT DELAYED INTO item_bag_serial "
		"(serial, server, name, level, account_id, account, char_id, char_name, world, world_x, world_y, instance) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_QUERY_LEVEL_UP, "INSERT DELAYED INTO log_level_up (char_id, char_name, level_normal, level_master, level_majestic, world, world_x, world_y, event_ground) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_JEWEL_DROP, "INSERT DELAYED INTO log_jewel_drop (char_id, account_id, jewel_type, jewel_index, jewel_name, world, log_jewel_drop.x, log_jewel_drop.y) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_CASH_SHOP, "INSERT DELAYED INTO log_cash_shop (server_id, account_id, account, char_id, char_name, player_data, dest_char, "
		"package_id, product_id, option_id, product_name, price_type, price, gift) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_MIX, "INSERT DELAYED INTO log_mix (server, account_id, account, char_id, char_name, player_data, mix_id, mix_name, rate, cost, result, item_data_before, item_data_after) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_BOSS_KILL, "INSERT DELAYED INTO log_boss_kill (boss_index, boss_name, boss_id, character_id, character_name, damage, damage_reflect, damage_debuff, damage_elemental, log_boss_kill.server, first_hit) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_JEWEL_MIX, "INSERT DELAYED INTO log_jewel_mix (char_id, char_name, server, mix_type, mix_id, mix_level) VALUES (?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_ITEM_CREATE, "INSERT DELAYED INTO log_item_create (account_id, account, char_id, char_name, interface_id, world, world_x, world_y, server, serial, serial_shop, item, level, item_data) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_LOG_PERSONAL_STORE, "INSERT DELAYED INTO log_personal_store (server, account_id_01, account_01, char_id_01, char_name_01, player_data_01, account_id_02, account_02, char_id_02, char_name_02, player_data_02, offstore, item_data) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_LOG_TRADE, "INSERT DELAYED INTO log_trade (server, account_id_01, account_01, char_id_01, char_name_01, player_data_01, account_id_02, account_02, char_id_02, char_name_02, player_data_02, money, item_data) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_ANTIHACK, "INSERT DELAYED INTO antihack (server, account_id, account, char_id, char_name, player_data, player_location, type, description) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(MULOG_STATEMENT_ERRTEL_PENTAGRAM, "INSERT DELAYED INTO log_errtel_pentagram (server, account_id, account, char_id, char_name, player_data, "
		"type, blessing_spirit, result, pentagram_data, errtel_data) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(LOG_STATEMENT_MIX_RECOVERY, "INSERT DELAYED INTO mix_recovery (`server`, account_id, `account`, char_id, char_name, player_data, `type`, item_data) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(LOG_STATEMENT_REWARD_DUNGEON, "INSERT DELAYED INTO reward_dungeon (`server`, account_id, `account`, char_id, char_name, player_data, `type`, item_data) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(LOG_STATEMENT_REWARD_LABYRINTH_OF_DIMENSIONS, "INSERT DELAYED INTO reward_labyrinth_of_dimensions (`server`, char_id, char_name, `category`, `rank`, item_data) VALUES (?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(LOG_STATEMENT_PVP, "INSERT DELAYED INTO log_pvp (`char_id_1`, char_name_1, char_class_1, `char_stat_1`, `char_set_1`, "
		"`char_id_2`, char_name_2, char_class_2, `char_stat_2`, `char_set_2`, skill, hp_damage, sd_damage) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
}