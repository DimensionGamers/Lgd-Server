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

void MuDatabaseConnection::DoPrepareStatements()
{
	if (!m_reconnecting)
		m_stmts.resize(MU_DATABASE_STATEMENT_MAX);

	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CREATE, "INSERT IGNORE INTO account_data (account_id) VALUES (?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_DATA_UPDATE, "UPDATE account_data SET vip_status = ?, vip_duration = ?, "
		"expanded_warehouse = ?, expanded_warehouse_time = ?, special_character = ?, credits = ? WHERE account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CHARACTER_UPDATE, "UPDATE account_data AS i SET i.current_character = ?, i.current_type = ?, "
		"i.current_ip = ?, i.current_mac = ?, i.current_diskserial = ?, i.current_server = ? WHERE i.account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CHARACTER_SELECT, "SELECT a.vip_status, a.vip_duration, a.expanded_warehouse, a.expanded_warehouse_time, a.special_character, a.credits, a.web_credits, a.account_id, "
		"a.current_character, a.current_type, a.current_ip, a.current_mac, a.current_diskserial FROM account_data a WHERE a.current_type != 0 AND a.current_server = ?", CONNECTION_SYNCH);

	PrepareStatement(ACCOUNT_WAREHOUSE_DATA_SELECT, "SELECT a.money, a.password FROM account_warehouse a WHERE a.account_id = ?", CONNECTION_ASYNC);
	PrepareStatement(ACCOUNT_WAREHOUSE_DATA_REPLACE, "REPLACE INTO account_warehouse (account_id, money, password) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_SELECT_GUID_BY_NAME, "SELECT c.guid, g.char_id FROM character_info c LEFT JOIN guild_members g ON g.char_id = c.guid WHERE c.name = ?", CONNECTION_BOTH);

	PrepareStatement(CHARACTER_SELECT_QUERY, "SELECT c.guid FROM character_info c WHERE c.name = ? AND c.account_id = ?", CONNECTION_ASYNC);
	
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_CREATE, "INSERT INTO character_info "
			"(account_id, race, name, strength, agility, vitality, energy, leadership, life, mana, points, level, world, world_x, world_y, slot) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_DELETE_BUFF, "DELETE FROM account_buff WHERE account_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_SELECT_BUFF, "SELECT a.buff, a.effect_1, a.value_1, a.effect_2, a.value_2, a.effect_3, a.value_3, a.duration, a.flags FROM account_buff a WHERE a.account_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_INSERT_BUFF, "INSERT IGNORE INTO account_buff (account_id, buff, effect_1, value_1, effect_2, value_2, effect_3, value_3, duration, flags) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_DELETE_INFO, "DELETE FROM character_info WHERE guid = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_SKILL, "DELETE FROM character_skill WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_QUEST_EVOLUTION, "DELETE FROM character_quest_evo WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_MAIL, "DELETE FROM character_mail WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_INVENTORY, "DELETE FROM item_data WHERE id = ? AND box = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_GENS, "DELETE FROM character_gens WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_GENS_KILL, "DELETE FROM character_gens_kill WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_GAMEOPTION, "DELETE FROM character_gameoption WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_FRIEND, "DELETE FROM character_friend WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_HELPER, "DELETE FROM character_helper WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_RESTRICTION, "DELETE FROM character_restriction WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_STATISTICS, "DELETE FROM character_statistics WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_BUFF, "DELETE FROM character_buff WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_ITEM_DELAY, "DELETE FROM character_item_delay WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_DELETE_CHAT_BLOCK, "DELETE FROM character_chat_block WHERE char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_INFO, "SELECT c.guid, c.slot, c.authority, c.race, c.name, c.level, c.level_master, c.experience, c.experience_master, "
		"c.points, c.points_master, c.strength, c.agility, c.vitality, c.energy, c.leadership, c.world, c.world_x, c.world_y, c.direction, c.money, c.life, c.mana, c.shield, c.stamina, "
		"c.add_fruit_points, c.dec_fruit_points, c.expanded_inventory, c.mute_time, c.admin_flags, c.pk_level, c.pk_count, c.pk_points, c.first_time, "
		"c.santa_claus_gift, c.personal_store_name, "
		"c.goblin_points, c.kick_time, c.account_id, c.post_count, c.post_day, c.post_month, c.ruud_money, c.level_majestic, c.experience_majestic, c.points_majestic, UNIX_TIMESTAMP(c.create_date), c.hunting_log_visible "
		"FROM character_info c WHERE c.account_id = ? AND c.guid = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_SKILL, "SELECT c.type, c.skill, c.skill_level FROM character_skill c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_GAME_OPTION, "SELECT c.game_option, c.chat_window, c.q_key, c.w_key, c.e_key, c.r_key, c.qwer_level, "
		"c.skill_bind_0, c.skill_bind_1, c.skill_bind_2, c.skill_bind_3, c.skill_bind_4, c.skill_bind_5, c.skill_bind_6, c.skill_bind_7, c.skill_bind_8, c.skill_bind_9, c.extra_data, c.change_skin, c.additional_options, "
		"c.button_bind_1, c.button_bind_2, c.button_bind_3, c.button_bind_4, c.button_bind_5 FROM character_gameoption c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_QUEST_EVOLUTION, "SELECT c.id, c.state, c.kill_count_1, c.kill_count_2, c.kill_count_3, c.kill_count_4, c.kill_count_5 "
		"FROM character_quest_evo c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_FRIEND, "SELECT c.friend_name, i.server_code, i.online "
		"FROM character_friend c LEFT JOIN character_info i ON c.friend_name = i.name WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_MAIL, "SELECT c.from_id, c.subject, c.message, c.direction, c.action, c.photo, c.window_guid, c.date, c.opened, c.id "
		"FROM character_mail c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_GUILD, "SELECT g.guild_id FROM guild_members g WHERE g.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_GEN, "SELECT c.family, c.level, c.contribution, c.reward_date, c.join_date, c.left_date, h.ranking "
		"FROM character_gens AS c LEFT JOIN gen_ranking AS h ON c.char_id = h.char_id WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_GEN_KILL, "SELECT c.killed_id, c.count, c.date FROM character_gens_kill c WHERE c.char_id = ?",CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_HELPER, "SELECT c.option_flag_1, c.option_flag_2, c.item_pick_flag, c.hunting_range, c.item_pick_range, c.distance, c.attack_skill_1, c.attack_sec_skill_1, c.attack_sec_skill_2, "
		"c.attack_sec_delay_1, c.attack_sec_delay_2, c.buff_skill_1, c.buff_skill_2, c.buff_skill_3, c.time_space_casting, c.percent_autopot, c.percent_autoheal, c.percent_partyheal, c.percent_drainlife, "
		"c.item_list, c.buff_item_1, c.buff_item_2, c.buff_item_3 FROM character_helper c WHERE c.char_id = ?",CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_BUFF, "SELECT c.buff, c.effect_1, c.value_1, c.effect_2, c.value_2, c.effect_3, c.value_3, c.duration, c.flags "
		"FROM character_buff c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_RESTRICTION, "SELECT c.restriction, c.time, c.admin_name, c.reason FROM character_restriction c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_STATISTICS, "SELECT c.statistic_id, c.count FROM character_statistics c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_ITEM_DELAY, "SELECT c.item, c.date FROM character_item_delay c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_SELECT_CHAT_BLOCK, "SELECT c.blocked FROM character_chat_block c WHERE c.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_UPDATE_INFO, "UPDATE character_info SET "
			"authority = ?, race = ?, name = ?, level = ?, level_master = ?, experience = ?, experience_master = ?, "
			"points = ?, points_master = ?, strength = ?, agility = ?, vitality = ?, energy = ?, leadership = ?, "
			"world = ?, world_x = ?, world_y = ?, "
			"direction = ?, money = ?, life = ?, mana = ?, shield = ?, stamina = ?, add_fruit_points = ?, dec_fruit_points = ?, "
			"expanded_inventory = ?, mute_time = ?, admin_flags = ?, pk_level = ?, pk_count = ?, pk_points = ?, first_time = ?, "
			"santa_claus_gift = ?, personal_store_name = ?, personal_store_open = ?, "
			"goblin_points = ?, last_use = ?, kick_time = ?, post_count = ?, post_day = ?, post_month = ?, ruud_money = ?, level_majestic = ?, experience_majestic = ?, points_majestic = ?, hunting_log_visible = ? "
			"WHERE guid = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_SKILL, "INSERT IGNORE INTO character_skill (char_id, type, skill, skill_level) VALUES "
			"(?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_GAME_OPTION, "INSERT IGNORE INTO character_gameoption "
		"(char_id, game_option, chat_window, q_key, w_key, e_key, r_key, qwer_level, "
		"skill_bind_0, skill_bind_1, skill_bind_2, skill_bind_3, skill_bind_4, skill_bind_5, skill_bind_6, skill_bind_7, "
		"skill_bind_8, skill_bind_9, extra_data, change_skin, additional_options, "
		"button_bind_1, button_bind_2, button_bind_3, button_bind_4, button_bind_5) VALUES "
		"(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_QUEST_EVOLUTION, "INSERT IGNORE INTO character_quest_evo "
			"(char_id, id, state, kill_count_1, kill_count_2, kill_count_3, kill_count_4, kill_count_5) VALUES "
			"(?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_FRIEND, "INSERT IGNORE INTO character_friend (char_id, friend_name) VALUES (?, ?) ", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_MAIL, "INSERT IGNORE INTO character_mail "
			"(char_id, from_id, subject, message, direction, action, photo, window_guid, date, opened) VALUES "
			"(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_GEN, "INSERT IGNORE INTO character_gens (char_id, family, character_gens.level, contribution, reward_date, join_date, left_date) VALUES "
		"(?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_GEN_KILL, "INSERT IGNORE INTO character_gens_kill (char_id, killed_id, count, date) VALUES (?, ?, ?, ?) ", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_HELPER, "INSERT IGNORE INTO character_helper "
		"(char_id, option_flag_1, option_flag_2, item_pick_flag, hunting_range, item_pick_range, distance, "
		"attack_skill_1, attack_sec_skill_1, attack_sec_skill_2, attack_sec_delay_1, attack_sec_delay_2, "
		"buff_skill_1, buff_skill_2, buff_skill_3, time_space_casting, "
		"percent_autopot, percent_autoheal, percent_partyheal, percent_drainlife, item_list, buff_item_1, buff_item_2, buff_item_3) VALUES "
		"(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_INSERT_BUFF, "INSERT IGNORE INTO character_buff (char_id, buff, effect_1, value_1, effect_2, value_2, effect_3, value_3, duration, flags) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_INSERT_RESTRICTION, "INSERT IGNORE INTO character_restriction (char_id, restriction, time, admin_name, reason) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_CHARACTER_STATISTICS_REMOVE, "DELETE FROM character_statistics WHERE char_id = ? AND statistic_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_CHARACTER_STATISTICS_INSERT, "INSERT IGNORE INTO character_statistics (char_id, statistic_id, count) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_INSERT_ITEM_DELAY, "INSERT IGNORE INTO character_item_delay (char_id, item, date) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_INSERT_CHAT_BLOCK, "INSERT IGNORE INTO character_chat_block (char_id, blocked) VALUES (?, ?)", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_MAIL_NEW_UPDATE, "SELECT c.from_id, c.subject, c.message, c.direction, c.action, c.photo, c.window_guid, c.date, c.opened, c.id "
		"FROM character_mail c WHERE c.char_id = ? AND c.opened = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_MAIL_UPDATE, "UPDATE character_mail SET opened = ? WHERE id = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_MAIL_DELETE_SINGLE, "DELETE FROM character_mail WHERE id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_PARTY_DELETE, "DELETE FROM party WHERE server = ? AND guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_PARTY_INSERT, "INSERT IGNORE INTO party (server, guid) VALUES (?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_PARTY_DELETE_MEMBER, "DELETE FROM party_member WHERE server = ? AND party = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_PARTY_INSERT_MEMBER, "INSERT IGNORE INTO party_member (server, party, member, position) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_GUILD_DELETE, "DELETE FROM guild_list WHERE guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_INSERT, "INSERT IGNORE INTO guild_list (guid, name, emblem) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_DELETE_MEMBER, "DELETE FROM guild_members WHERE guild_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_INSERT_MEMBER, "INSERT IGNORE INTO guild_members (id, guild_id, char_id, ranking) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_UPDATE_NOTICE, "UPDATE guild_list SET notice = ? WHERE guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_UPDATE_MEMBER_RANKING, "UPDATE guild_members SET ranking = ? WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_DELETE_MEMBER_ID, "DELETE FROM guild_members WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_UPDATE_ALLIANCE, "UPDATE guild_list SET alliance = ? WHERE guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_UPDATE_HOSTIL, "UPDATE guild_list SET hostil = ? WHERE guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_SELECT_REGISTERED, "SELECT 1 FROM castle_siege_registered_guild WHERE guild = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_GUILD_UPDATE_SCORE, "UPDATE guild_list SET score = ? WHERE guid = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_UPDATE_ONLINE, "UPDATE character_info SET online = ?, server_code = ? WHERE guid = ?", CONNECTION_ASYNC);

	PrepareStatement(CHARACTER_FRIEND_ADD_REQUEST, "SELECT guid, online, server_code, authority FROM character_info WHERE name = ?", CONNECTION_ASYNC);
	PrepareStatement(CHARACTER_MAIL_ADD_REQUEST, "SELECT guid, account_id, level FROM character_info WHERE name = ?", CONNECTION_BOTH);

	PrepareStatement(QUERY_CASH_SHOP_COIN_REQUEST, "SELECT a.vip_status, a.vip_duration, a.expanded_warehouse, a.expanded_warehouse_time, a.special_character, a.credits, a.web_credits, a.cash_shop_discount_wc, a.cash_shop_discount_gp, UNIX_TIMESTAMP(a.cash_shop_discount_date) FROM account_data a WHERE a.account_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_CASH_SHOP_COIN_CLEAR, "UPDATE account_data SET web_credits = web_credits - ? WHERE account_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_CASH_SHOP_COIN_ADD, "UPDATE account_data AS i SET i.web_credits = i.web_credits + ? WHERE i.account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_CASH_SHOP_ITEM_DELETE, "DELETE FROM account_cash_shop_item WHERE account_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_CASH_SHOP_ITEM_INSERT, "INSERT IGNORE INTO account_cash_shop_item (account_id, account_cash_shop_item.product, account_cash_shop_item.option, serial, serial_cash_shop, server, gift, date) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_CASH_SHOP_ITEM_REQUEST, "SELECT a.product, a.option, a.serial, a.serial_cash_shop, a.server, a.gift, a.date "
		"FROM account_cash_shop_item a WHERE a.account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_CASH_SHOP_ITEM_GIFT_REQUEST, "SELECT a.product, a.option, a.serial, a.serial_cash_shop, a.server, a.date "
		"FROM account_cash_shop_gift a WHERE a.account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_CASH_SHOP_ITEM_GIFT_REMOVE, "DELETE FROM account_cash_shop_gift WHERE serial = ? AND serial_cash_shop = ? AND server = ? AND account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_CASH_SHOP_ITEM_GIFT_INSERT, "INSERT INTO account_cash_shop_gift (account_id, product, account_cash_shop_gift.option, serial, serial_cash_shop, server, date) "
		"VALUES (?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_ITEM_DATA_SELECT, "SELECT i.id, i.box, i.slot, i.entry, i.server, i.serial, i.serial_cash_shop, i.level, i.durability, i.durability_state, i.skill, i.luck, i.option, "
			"i.excellent, i.ancient, i.option_380, i.harmony, i.socket_1, i.socket_2, i.socket_3, i.socket_4, i.socket_5, i.socket_bonus, i.locked, i.data_1, i.data_2, i.data_3, "
			"i.personal_store_price, i.personal_store_job, i.personal_store_jos, i.expire_date, i.flags "
			"FROM item_data i WHERE i.id = ? AND i.box = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_ITEM_DATA_INSERT, "REPLACE INTO item_data "
			"(id, box, slot, entry, server, serial, serial_cash_shop, item_data.level, durability, durability_state, skill, luck, item_data.option, "
			"excellent, ancient, option_380, harmony, socket_1, socket_2, socket_3, socket_4, socket_5, socket_bonus, locked, data_1, data_2, data_3, "
			"personal_store_price, personal_store_job, personal_store_jos, expire_date, flags) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	/*PrepareStatement(QUERY_ITEM_DATA_INSERT, "INSERT INTO item_data "
			"(id, box, slot, entry, server, serial, serial_cash_shop, item_data.level, durability, durability_state, skill, luck, item_data.option, "
			"excellent, ancient, option_380, harmony, socket_1, socket_2, socket_3, socket_4, socket_5, socket_bonus, locked, data_1, data_2, data_3, "
			"personal_store_price, personal_store_job, personal_store_jos, expire_date) "
			"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);*/

	PrepareStatement(QUERY_ITEM_DATA_DELETE, "DELETE FROM item_data WHERE id = ? AND box = ? AND slot = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_ITEM_DATA_DELETE_BY_SERIAL, "DELETE i FROM item_data AS i WHERE i.server = ? and i.serial = ? and i.serial_cash_shop = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_CHARACTER_EXIST, "SELECT 1 FROM character_info WHERE name = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_AUTOLOGIN, "SELECT guid, account_id FROM character_info "
		"WHERE personal_store_open = 1 AND online = 0 AND authority != 1 AND server_code = ? LIMIT 1", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ACCOUNT_AUTOLOGIN_DISCONNECT, "UPDATE character_info SET personal_store_open = 0 WHERE guid = ?", CONNECTION_ASYNC);


	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_LOAD_DATA, "SELECT c.owner, c.status, c.tax_hunt, c.tax_chaos, c.tax_store, c.hunt_allowed, c.money FROM castle_siege_data c", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CREATE_DATA, "INSERT INTO castle_siege_data (owner, status) VALUES (?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_LOAD_NPC, "SELECT c.npc, c.id, c.defense_level, c.regen_level, c.life_level, c.life FROM castle_siege_npc c", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CLEAR_NPC, "DELETE FROM castle_siege_npc", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_INSERT_NPC, "INSERT INTO castle_siege_npc (npc, id, defense_level, regen_level, life_level, life) VALUES (?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CLEAR_GUILD, "TRUNCATE TABLE castle_siege_guild", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_CLEAR_REGISTERED_GUILD, "TRUNCATE TABLE castle_siege_registered_guild", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_DATA, "UPDATE castle_siege_data SET owner = ?, status = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_DATA_MONEY, "UPDATE castle_siege_data SET money = money + ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_RESET_DATA_MONEY, "UPDATE castle_siege_data SET money = 0", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_DATA_TAX, "UPDATE castle_siege_data SET tax_hunt = ?, tax_chaos = ?, tax_store = ?, hunt_allowed = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_INSERT_GUILD, "INSERT INTO castle_siege_guild (guild, side, score) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_INSERT_REGISTERED_GUILD, "INSERT INTO castle_siege_registered_guild (guild) VALUES (?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_DELETE_REGISTERED_GUILD, "DELETE FROM castle_siege_registered_guild WHERE guild = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD, "UPDATE castle_siege_registered_guild SET marks = ? WHERE guild = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_SELECT_REGISTERED_GUILD_ALL, "SELECT c.guild, c.marks, c.register_id FROM castle_siege_registered_guild c", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_SELECT_REGISTERED_GUILD_ID, "SELECT c.marks, c.register_id FROM castle_siege_registered_guild c WHERE c.guild = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_CASTLE_SIEGE_SELECT_REGISTERED_GUILD, "SELECT guild, marks, register_id, "
		"a.guid, a.level, a.level_master, b.guid "
		"FROM castle_siege_registered_guild "
		"LEFT JOIN character_info AS a ON a.guid = castle_siege_registered_guild.guild "
		"LEFT JOIN guild_list AS b ON b.guid = castle_siege_registered_guild.guild", CONNECTION_SYNCH);


	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LIST_ACCOUNT_DATA, "SELECT a.vip_status, a.vip_duration, a.expanded_warehouse, a.expanded_warehouse_time, a.special_character "
		"FROM account_data a WHERE a.account_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LIST_CHARACTER, "SELECT c.guid, c.name, c.level, c.level_master, c.level_majestic, c.authority, c.race, c.slot, g.ranking, g.guild_id, c.kick_time, "
		"c.pk_level, c.online, UNIX_TIMESTAMP(c.create_date) "
		"FROM character_info c LEFT JOIN guild_members g ON c.guid = g.char_id WHERE c.account_id = ? AND c.slot < ? ORDER BY c.last_use DESC", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LIST_CHARACTER_ITEM, "SELECT i.id, i.slot, i.entry, i.level, i.excellent, i.ancient "
		"FROM item_data i "
		"WHERE i.id IN(SELECT c.guid FROM character_info c WHERE c.account_id = ?) AND i.box = ? AND i.slot < ? AND i.id != 0", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_GUILD_SELECT, "SELECT i.guid, i.name, i.emblem, i.notice, i.alliance, i.hostil, i.score, h.marks, h.guild "
		"FROM guild_list i LEFT JOIN castle_siege_registered_guild h ON i.guid = h.guild "
		"ORDER BY i.guid ASC", CONNECTION_SYNCH);

	PrepareStatement(QUERY_MUDATABASE_GUILD_MEMBER_SELECT, "SELECT i.guild_id, i.id, i.char_id, i.ranking, "
			"h.name, h.online, h.server_code FROM guild_members i LEFT JOIN character_info h ON h.guid = i.char_id", CONNECTION_SYNCH);

	PrepareStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_SELECT, "SELECT guid, authority, admin_flags FROM character_info WHERE character_info.name = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_KICK, "UPDATE character_info SET kick_time = ? WHERE guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_MUTE, "UPDATE character_info SET mute_time = ? WHERE guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_RESTRICTION, "REPLACE INTO character_restriction (char_id, restriction, time, admin_name, reason) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_BAN, "UPDATE character_info SET authority = ? WHERE guid = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_SELECT_LOCATION, "SELECT guid, character_info.name, world, world_x, world_y, direction, online, server_code FROM character_info WHERE character_info.name = ?", CONNECTION_BOTH);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_UPDATE_ON_SERVER_START, "UPDATE character_info SET online = 0 WHERE server_code = ?", CONNECTION_BOTH);

	/* SERVER SIGNAL */
	PrepareStatement(QUERY_MUDATABASE_SIGNAL_SELECT, "SELECT s.server, s.signal, s.data_1, s.data_2, s.data_3, s.id FROM server_signal s", CONNECTION_BOTH);
	PrepareStatement(QUERY_MUDATABASE_SIGNAL_DELETE, "DELETE FROM server_signal WHERE id = ?", CONNECTION_ASYNC);

	/* CRYWOLF */
	PrepareStatement(QUERY_MUDATABASE_CRYWOLF_SELECT, "SELECT c.state FROM crywolf_data c", CONNECTION_BOTH);
	PrepareStatement(QUERY_MUDATABASE_CRYWOLF_CREATE, "INSERT INTO crywolf_data (state) VALUES (?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CRYWOLF_UPDATE, "UPDATE crywolf_data SET state = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_PARTY_SELECT, "SELECT guid FROM party WHERE server = ? ORDER BY guid ASC", CONNECTION_SYNCH);

	PrepareStatement(QUERY_MUDATABASE_PARTY_MEMBER_SELECT, "SELECT a.member, a.position, c.name, c.guid, c.account_id, g.family, c.race, c.level + c.level_master + c.level_majestic as total_level "
		"FROM party_member a "
		"INNER JOIN character_info c ON c.guid = a.member "
		"LEFT JOIN character_gens g ON g.char_id = a.member "
		"WHERE a.server = ? AND a.party = ?", CONNECTION_SYNCH);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_KICK_SELECT, "SELECT 1 FROM character_kick a INNER JOIN character_info b ON a.char_id = b.guid AND b.name = ? WHERE a.character_name = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_KICK_DELETE, "DELETE FROM character_kick WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_KICK_VERIFY, "SELECT 1 FROM character_kick WHERE char_id = ? AND character_name = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_KICK_ADD, "INSERT IGNORE INTO character_kick (char_id, character_name) VALUES (?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_KICK_REMOVE, "DELETE FROM character_kick WHERE char_id = ? AND character_name = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_DELETE, "DELETE FROM item_pentagram_data WHERE `id` = ? AND `type` = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_INSERT, "INSERT IGNORE INTO item_pentagram_data (`id`, `type`, `index`, attribute, item_type, item_index, item_level, "
		"option_index_rank_1, option_level_rank_1, option_index_rank_2, option_level_rank_2, option_index_rank_3, option_level_rank_3, "
		"option_index_rank_4, option_level_rank_4, option_index_rank_5, option_level_rank_5, serial_server, `serial`, `flags`, `data_1`, `data_2`, `data_3`) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_SELECT, "SELECT * FROM item_pentagram_data AS i WHERE (i.id = ? AND i.type = ?) OR (i.id = ? AND i.type = ?)", CONNECTION_ASYNC);
	
	PrepareStatement(QUERY_MUDATABASE_GUILD_MATCHING_SELECT, "SELECT a.guild_id, a.text, a.interest_type, a.level_range, a.class_type, a.board_number, "
		"b.level, b.level_master, b.race, c.family "
		"FROM guild_matching a "
		"INNER JOIN character_info b ON a.guild_id = b.guid "
		"LEFT JOIN character_gens c ON c.char_id = b.guid", CONNECTION_SYNCH);

	PrepareStatement(QUERY_MUDATABASE_GUILD_MATCHING_INSERT, "INSERT INTO guild_matching (guild_id, text, interest_type, level_range, class_type, board_number) "
		"VALUES (?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_GUILD_MATCHING_DELETE, "DELETE FROM guild_matching WHERE guild_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ARKA_WAR_SELECT, "SELECT a.id, a.guild, a.attribute FROM arka_war_data a", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_ARKA_WAR_DELETE, "TRUNCATE TABLE arka_war_data", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_ARKA_WAR_INSERT, "INSERT INTO arka_war_data (id, guild, attribute) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_SERIAL_CHECK_INSERT, "INSERT INTO serial_check (serial_check.server, serial_check.serial, serial_check.type, account_id, serial_check.ip, mac, disk_serial) VALUES (?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_SCORE, "INSERT INTO character_score (char_id, type, `level`, score) VALUES (?, ?, ?, ?) ON DUPLICATE KEY UPDATE score = score + ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_DELETE_SCORE, "DELETE FROM character_score WHERE char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING, "INSERT INTO event_ranking (event_id, event_ground, char_id, score) VALUES (?, ?, ?, ?) ON DUPLICATE KEY UPDATE score = score + ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_DELETE, "DELETE FROM event_ranking WHERE char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ITEM_RECOVERY_SELECT, "SELECT a.item, a.level, a.durability, a.skill, a.luck, a.option, a.excellent, a.ancient, a.option_380, a.harmony, "
		"a.socket_1, a.socket_2, a.socket_3, a.socket_4, a.socket_5, a.socket_bonus, a.data_1, a.data_2, a.data_3, a.serial_server, a.serial, a.serial_cash_shop, a.flags, a.id FROM item_recovery a WHERE a.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_ITEM_RECOVERY_DELETE, "DELETE FROM item_recovery WHERE char_id = ? AND id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_ITEM_RECOVERY_INSERT, "INSERT IGNORE INTO item_recovery (char_id, item, `level`, durability, skill, luck, `option`, excellent, ancient, harmony, option_380, "
		"socket_1, socket_2, socket_3, socket_4, socket_5, socket_bonus, flags) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_SELECT, "SELECT a.world, a.year, a.month, a.day, a.level, a.duration, a.damage, a.elemental_damage, a.healing, a.killed_count, a.earned_experience FROM character_hunting_record a WHERE a.char_id = ? ORDER BY a.year ASC, a.month ASC, a.day ASC", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_DELETE, "DELETE FROM character_hunting_record WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_INSERT, "INSERT IGNORE INTO character_hunting_record (char_id, world, year, month, day, level, duration, damage, elemental_damage, healing, killed_count, earned_experience) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_MONSTER_RESPAWN_SELECT, "SELECT a.guid, a.date FROM monster_respawn a WHERE a.server = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_MONSTER_RESPAWN_DELETE, "DELETE FROM monster_respawn WHERE monster_respawn.server = ? AND guid = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_MONSTER_RESPAWN_INSERT, "INSERT INTO monster_respawn (monster_respawn.server, guid, date) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_SELECT, "SELECT a.stage, a.id, a.level, a.status, a.killed_monsters, a.earned_experience, a.completed_missions, a.date, a.goblin_state, a.day_first FROM character_labyrinth AS a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_DELETE, "DELETE FROM character_labyrinth WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_INSERT, "INSERT IGNORE INTO character_labyrinth (char_id, `stage`, `id`, `level`, `status`, killed_monsters, earned_experience, completed_missions, `date`, `goblin_state`, `day_first`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_SELECT, "SELECT a.idx, a.subidx, a.mission_id_1, a.mission_id_2, a.mission_id_3, a.mission_id_4, a.mission_id_5, "
		"a.mission_count_1, a.mission_count_2, a.mission_count_3, a.mission_count_4, a.mission_count_5 "
		"FROM character_labyrinth_zone AS a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_DELETE, "DELETE FROM character_labyrinth_zone WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_INSERT, "INSERT IGNORE INTO character_labyrinth_zone (char_id, idx, subidx, mission_id_1, mission_id_2, mission_id_3, mission_id_4, mission_id_5, mission_count_1, mission_count_2, mission_count_3, mission_count_4, mission_count_5) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_SELECT, "SELECT a.level, a.amount FROM character_labyrinth_level AS a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_DELETE, "DELETE FROM character_labyrinth_level WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_INSERT, "INSERT IGNORE INTO character_labyrinth_level (char_id, `level`, `amount`) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_LABYRINTH_INSERT, "INSERT INTO event_ranking_labyrinth (char_id, `level`, `stage`, `index`, killed_monsters, earned_experience, completed_missions, `score`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_SELECT, "SELECT a.event_id, a.count, a.day FROM character_event_count a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_DELETE, "DELETE FROM character_event_count WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_INSERT, "INSERT IGNORE INTO character_event_count (char_id, event_id, `count`, `day`) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);
	
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_SELECT, "SELECT a.state, a.score, a.total_score, a.cdate FROM character_mini_bomb a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_DELETE, "DELETE FROM character_mini_bomb WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_INSERT, "INSERT IGNORE INTO character_mini_bomb (char_id, `state`, `score`, `total_score`, `cdate`) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_SELECT, "SELECT a.cell, a.value, a.status FROM character_mini_bomb_grid a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_DELETE, "DELETE FROM character_mini_bomb_grid WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_INSERT, "INSERT IGNORE INTO character_mini_bomb_grid (char_id, `cell`, `value`, `status`) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_MINI_BOMB, "INSERT INTO event_mini_bomb_ranking (char_id, `state`, `score`, bombs_founded, bombs_failed) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_SELECT, "SELECT a.playing, a.type, a.score, a.card_count, a.special_card_count, a.date FROM character_mu_roomy a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DELETE, "DELETE FROM character_mu_roomy WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_INSERT, "INSERT IGNORE INTO character_mu_roomy (char_id, `playing`, `type`, `score`, `card_count`, special_card_count, `date`) VALUES (?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_SELECT, "SELECT a.type, a.slot, a.color, a.number, a.state, a.play_slot FROM character_mu_roomy_deck a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_DELETE, "DELETE FROM character_mu_roomy_deck WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_INSERT, "INSERT IGNORE INTO character_mu_roomy_deck (char_id, `type`, `slot`, `color`, `number`, `state`, play_slot) VALUES (?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_MU_ROOMY, "INSERT INTO event_mu_roomy_ranking (char_id, `type`, `score`, remain_card, remain_special_card) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_SELECT, "SELECT a.slot, a.data, a.warp_id FROM character_warp_favorite_list a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_DELETE, "DELETE FROM character_warp_favorite_list WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_INSERT, "INSERT IGNORE INTO character_warp_favorite_list (char_id, `slot`, `data`, `warp_id`) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_SELECT, "SELECT a.quest, a.count, a.state, a.date FROM character_quest_guided a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_DELETE, "DELETE FROM character_quest_guided WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_INSERT, "INSERT IGNORE INTO character_quest_guided (char_id, `quest`, `count`, `state`, `date`) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_SELECT, "SELECT a.state, a.box, a.count, a.jewel, a.score1, a.score2, a.score3, a.date FROM character_jewel_bingo a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_DELETE, "DELETE FROM character_jewel_bingo WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_INSERT, "INSERT IGNORE INTO character_jewel_bingo (char_id, `state`, `box`, `count`, `jewel`, `score1`, `score2`, `score3`, `date`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
	
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_SELECT, "SELECT a.type, a.slot, a.value FROM character_jewel_bingo_grid a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_DELETE, "DELETE FROM character_jewel_bingo_grid WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_INSERT, "INSERT IGNORE INTO character_jewel_bingo_grid (char_id, `type`, `slot`, `value`) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_JEWEL_BINGO, "INSERT INTO event_jewel_bingo_ranking (char_id, `type`, `score1`, `score2`, `score3`) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_SCRAMBLE, "INSERT INTO event_scramble_ranking (char_id, `word`, `shuffle_word`) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_RACE, "INSERT INTO event_race_ranking (char_id, `race`, `start`) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_GUILD_SCORE_UPDATE, "INSERT INTO guild_score (guild_id, score) VALUES (?, ?) ON DUPLICATE KEY UPDATE score = score + ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_GUILD_SCORE_DELETE, "DELETE FROM guild_score WHERE guild_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_CASTLE_SIEGE, "INSERT IGNORE INTO event_castle_siege_ranking (guild_id, `guild_name`, `character_count`) VALUES (?, ?, ?)", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_ARKA_WAR, "INSERT IGNORE INTO event_arka_war_ranking (guild_id_01, `guild_name_01`, guild_id_02, `guild_name_02`, `character_count`) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_DELETE_INSERT, "INSERT IGNORE INTO character_deleted (account_id, char_id, `name`) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_DATE_SELECT, "SELECT i.id, i.date FROM character_date AS i WHERE i.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_DATE_DELETE, "DELETE FROM character_date WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_DATE_INSERT, "INSERT IGNORE INTO character_date (char_id, id, `date`) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_SELECT, "SELECT i.type, i.section, i.id, i.level FROM character_majestic_tree AS i WHERE i.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_DELETE, "DELETE FROM character_majestic_tree WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_INSERT, "INSERT IGNORE INTO character_majestic_tree (char_id, `type`, `section`, id, `level`) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_SELECT, "SELECT i.id, i.level FROM character_majestic_stats AS i WHERE i.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_DELETE, "DELETE FROM character_majestic_stats WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_INSERT, "INSERT IGNORE INTO character_majestic_stats (char_id, id, `level`) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_CASTLE_SIEGE_KILL, "INSERT IGNORE INTO event_castle_siege_kill (char_id, char_name, char_rank, `state`, player_data_01, guild_id_01, guild_name_01, "
		"killer_id, killer_name, killer_rank, player_data_02, guild_id_02, guild_name_02, world, world_x, world_y) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_ARKA_WAR_KILL, "INSERT IGNORE INTO event_arka_war_kill (char_id, char_name, char_rank, player_data_01, guild_id_01, guild_name_01, "
		"killer_id, killer_name, killer_rank, player_data_02, guild_id_02, guild_name_02, world, world_x, world_y) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_CASTLE_SIEGE_TIME, "INSERT IGNORE INTO event_castle_siege_time (char_id, char_name, char_rank, player_data, guild_id, guild_name, `type`, `time`) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_SELECT, "SELECT a.state, a.score, a.number_1, a.number_2, a.number_3, a.numbers, a.strikes, a.balls FROM character_numeric_baseball AS a WHERE a.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_DELETE, "DELETE FROM character_numeric_baseball WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_INSERT, "INSERT IGNORE INTO character_numeric_baseball (char_id, `state`, `score`, `number_1`, `number_2`, `number_3`, `numbers`, `strikes`, `balls`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_RANKING_NUMERIC_BASEBALL, "INSERT INTO event_numeric_baseball_ranking (char_id, `score`) VALUES (?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_SCHEDULE_SELECT, "SELECT i.state, UNIX_TIMESTAMP(i.start_date) FROM event_labyrinth_schedule AS i", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_SCHEDULE_UPDATE, "REPLACE INTO event_labyrinth_schedule (`server`, `state`, `start_date`) VALUES (?, ?, FROM_UNIXTIME(?))", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_SELECT, "SELECT i.category, i.char_id, i.char_name, i.char_class, i.score, i.league_score, i.time FROM event_labyrinth_league AS i", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_UPDATE, "INSERT INTO event_labyrinth_league (`category`, `char_id`, `char_name`, `char_class`, `score`, `league_score`, `time`) "
		"VALUES (?, ?, ?, ?, ?, ?, ?) "
		"ON DUPLICATE KEY UPDATE `char_name` = ?, `char_class` = ?, `score` = ?, `league_score` = ?, `time` = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_CATEGORY_UPDATE, "UPDATE event_labyrinth_league AS i SET i.category = ?, i.league_score = ?, i.time = ? WHERE i.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_EVENT_LABYRINTH_LEAGUE_DELETE, "TRUNCATE TABLE event_labyrinth_league", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_DUNGEON_INSTANCE_SELECT, "SELECT * FROM event_dungeon_instance", CONNECTION_SYNCH);
	PrepareStatement(QUERY_MUDATABASE_DUNGEON_INSTANCE_UPDATE, "INSERT INTO event_dungeon_instance (`char_id`, `id`, `time`) VALUES (?, ?, ?) "
		"ON DUPLICATE KEY UPDATE `id` = ?, `time` = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_DUNGEON_INSTANCE_DELETE, "DELETE FROM event_dungeon_instance WHERE char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_QUEST_MU_SELECT, "SELECT i.quest_id, i.state, i.objective, i.date FROM character_quest_mu AS i WHERE i.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_QUEST_MU_DELETE, "DELETE FROM character_quest_mu WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_QUEST_MU_INSERT, "INSERT IGNORE INTO character_quest_mu (char_id, quest_id, `state`, objective, `date`) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_MUDATABASE_MONSTER_SOUL_SELECT, "SELECT i.type, i.id, i.amount FROM character_monster_soul AS i WHERE i.char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_MONSTER_SOUL_DELETE, "DELETE FROM character_monster_soul WHERE char_id = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_MUDATABASE_MONSTER_SOUL_INSERT, "INSERT IGNORE INTO character_monster_soul (char_id, `type`, `id`, `amount`) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);


	///*******************************************************************************************************************************************************
	//CHAOS CASTLE SURVIVAL
	//*******************************************************************************************************************************************************/
	PrepareStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_INSERT, "INSERT INTO chaos_castle_survival_ranking (char_id, score) "
		"VALUES (?, ?) ON DUPLICATE KEY UPDATE score = score + ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_SELECT, "SELECT i.char_id, a.name, i.score FROM chaos_castle_survival_ranking AS i "
		"INNER JOIN character_info AS a ON a.guid = i.char_id ORDER BY i.score DESC", CONNECTION_SYNCH);
	PrepareStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_DELETE, "TRUNCATE TABLE chaos_castle_survival_ranking", CONNECTION_ASYNC);
	PrepareStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_REMOVE, "DELETE i FROM chaos_castle_survival_ranking as i WHERE i.char_id = ?", CONNECTION_ASYNC);

	PrepareStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_INIT, "UPDATE chaos_castle_survival_ranking AS i SET i.score = 0", CONNECTION_ASYNC);

	PrepareStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_HALL_OF_FAME_INSERT, "INSERT IGNORE INTO chaos_castle_survival_hall_of_fame (char_id, char_name, score) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	///*******************************************************************************************************************************************************
	//TORMENTED SQUARE SURVIVAL
	//*******************************************************************************************************************************************************/
	PrepareStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_RANKING_DELETE, "DELETE i FROM tormented_square_survival_ranking as i WHERE i.type = ?", CONNECTION_ASYNC);
	PrepareStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_RANKING_INSERT, "INSERT IGNORE INTO tormented_square_survival_ranking "
		"(`type`, ranking, char_id_1, char_name_1, char_id_2, char_name_2, score) VALUES (?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_TEAM_SELECT, "SELECT a.char_id_1, b.name, a.char_id_2, c.name, a.score "
		"FROM tormented_square_survival_team AS a "
		"INNER JOIN character_info AS b ON b.guid = a.char_id_1 "
		"INNER JOIN character_info AS c ON c.guid = a.char_id_2 "
		"WHERE a.type = ? ORDER BY a.score DESC, (b.level + b.level_master + b.level_majestic + c.level + c.level_master + c.level_majestic) ASC", CONNECTION_SYNCH);

	PrepareStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_TEAM_INSERT, "INSERT INTO tormented_square_survival_team (char_id_1, char_id_2, score, `type`) VALUES (?, ?, ?, ?) "
		"ON DUPLICATE KEY UPDATE score = score + ?, enter_count = enter_count + 1", CONNECTION_ASYNC);

	PrepareStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_HALL_OF_FAME_INSERT, "INSERT IGNORE INTO tormented_square_survival_hall_of_fame "
		"(char_id_1, char_name_1, char_id_2, char_name_2, score) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	///*******************************************************************************************************************************************************
	//COMMON
	//*******************************************************************************************************************************************************/
	PrepareStatement(QUERY_BATTLE_CORE_EVENT_STAGE_SELECT, "SELECT i.stage FROM event_stage i WHERE i.event_id = ?", CONNECTION_SYNCH);
	PrepareStatement(QUERY_BATTLE_CORE_EVENT_STAGE_UPDATE, "REPLACE INTO event_stage (event_id, `stage`) VALUES (?, ?)", CONNECTION_ASYNC);
}