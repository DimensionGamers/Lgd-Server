void MuEditorDatabaseConnection::DoPrepareStatements()
{
	if (!m_reconnecting)
		m_stmts.resize(MAX_MUEDITOR_STATEMENTS);
	/******************************************************************************************
											ITEM
	******************************************************************************************/

	PrepareStatement(QUERY_EDITOR_DELETE_ITEM, "TRUNCATE TABLE item_bmd_tmp", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_SAVE_ITEM, "INSERT INTO item_bmd_tmp (`type`, `index`, `path`, filename, `name`, unk1, kind, unk2, two_hand, level, slot, unk3, skill, `x`, `y`, damage_min, damage_max, "
	"defense_success,  defense, defense_magic, attack_speed, walk_speed, durability, durability_magic, power, unk4, required_str, required_agi, required_ene, required_vit, required_cmd, required_lvl, "
	"value, unk5, unk6, price, ancient_attribute, allow_dw, allow_dk, allow_elf, allow_mg, allow_dl, allow_sum, allow_rf, allow_gl, allow_rw, allow_sl, "
	"res_1, res_2, res_3, res_4, res_5, res_6, res_7, flag1, flag2, flag3, flag4, flag5, expensive_item, flag7, max_stack, pc_point, unk7) "
	"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_LOAD_ITEM, "SELECT * FROM item_bmd", CONNECTION_SYNCH);

	//PrepareStatement(QUERY_EDITOR_INSERT_ITEM, "", CONNECTION_ASYNC);
	PrepareStatement(QUERY_EDITOR_UPDATE_ITEM, "UPDATE item_bmd AS a INNER JOIN item_bmd_tmp AS b ON a.`type` = b.`type` AND a.`index` = b.`index` SET "
	"a.path = b.path, a.filename = b.filename, a.unk1 = b.unk1, a.kind = b.kind, a.unk2 = b.unk2, a.two_hand = b.two_hand, a.level = b.level, a.slot = b.slot, a.unk3 = b.unk3, a.skill = b.skill, "
	"a.`x` = b.`x`, a.`y` = b.`y`, a.damage_min = b.damage_min, a.damage_max = b.damage_max, a.defense_success = b.defense_success, a.defense = b.defense, a.defense_magic = b.defense_magic, a.attack_speed = b.attack_speed, "
	"a.walk_speed = b.walk_speed, a.durability = b.durability, a.durability_magic = b.durability_magic, a.power = b.power, a.unk4 = b.unk4, a.required_str = b.required_str, a.required_agi = b.required_agi, "
	"a.required_ene = b.required_ene, a.required_vit = b.required_vit, a.required_cmd = b.required_cmd, a.required_lvl = b.required_lvl, a.value = b.value, a.unk5 = b.unk5, a.unk6 = b.unk6, a.price = b.price, "
	"a.ancient_attribute = b.ancient_attribute, a.allow_dw = b.allow_dw, a.allow_dk = b.allow_dk, a.allow_elf = b.allow_elf, a.allow_mg = b.allow_mg, a.allow_dl = b.allow_dl, a.allow_sum = b.allow_sum, a.allow_rf = b.allow_rf, a.allow_gl = b.allow_gl, a.allow_rw = b.allow_rw, a.allow_sl = b.allow_sl, "
	"a.res_1 = b.res_1, a.res_2 = b.res_2, a.res_3 = b.res_3, a.res_4 = b.res_4, a.res_5 = b.res_5, a.res_6 = b.res_6, a.res_7 = b.res_7, a.flag1 = b.flag1, a.flag2 = b.flag2, a.flag3 = b.flag3, a.flag4 = b.flag4, a.flag5 = b.flag5, "
	"a.expensive_item = b.expensive_item, a.flag7 = b.flag7, a.max_stack = b.max_stack, a.pc_point = b.pc_point, a.unk7 = b.unk7 WHERE "
	"a.path != b.path OR a.filename != b.filename OR a.unk1 != b.unk1 OR a.kind != b.kind OR a.unk2 != b.unk2 OR a.two_hand != b.two_hand OR a.level != b.level OR a.slot != b.slot OR "
	"a.unk3 != b.unk3 OR a.skill != b.skill OR a.`x` != b.`x` OR a.`y` != b.`y` OR a.damage_min != b.damage_min OR a.damage_max != b.damage_max OR a.defense_success != b.defense_success OR "
	"a.defense != b.defense OR a.defense_magic != b.defense_magic OR a.attack_speed != b.attack_speed OR a.walk_speed != b.walk_speed OR a.durability != b.durability OR a.durability_magic != b.durability_magic OR "
	"a.power != b.power OR a.unk4 != b.unk4 OR a.required_str != b.required_str OR a.required_agi != b.required_agi OR a.required_ene != b.required_ene OR a.required_vit != b.required_vit OR "
	"a.required_cmd != b.required_cmd OR a.required_lvl != b.required_lvl OR a.value != b.value OR a.unk5 != b.unk5 OR a.unk6 != b.unk6 OR a.price != b.price OR a.ancient_attribute != b.ancient_attribute OR "
	"a.allow_dw != b.allow_dw OR a.allow_dk != b.allow_dk OR a.allow_elf != b.allow_elf OR a.allow_mg != b.allow_mg OR a.allow_dl != b.allow_dl OR a.allow_sum != b.allow_sum OR a.allow_rf != b.allow_rf OR "
	"a.allow_gl != b.allow_gl OR a.allow_rw != b.allow_rw OR a.allow_sl != b.allow_sl OR a.res_1 != b.res_1 OR a.res_2 != b.res_2 OR a.res_3 != b.res_3 OR a.res_4 != b.res_4 OR a.res_5 != b.res_5 OR a.res_6 != b.res_6 OR a.res_7 != b.res_7 OR "
	"a.flag1 != b.flag1 OR a.flag2 != b.flag2 OR a.flag3 != b.flag3 OR a.flag4 != b.flag4 OR a.flag5 != b.flag5 OR a.expensive_item != b.expensive_item OR a.flag7 != b.flag7 OR a.max_stack != b.max_stack OR a.pc_point != b.pc_point OR "
	"a.unk7 != b.unk7", CONNECTION_ASYNC);


	/******************************************************************************************
										ITEM_TOOL_TIP
	******************************************************************************************/
	PrepareStatement(QUERY_EDITOR_DELETE_ITEM_TOOL_TIP, "TRUNCATE TABLE item_tool_tip_tmp", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_SAVE_ITEM_TOOL_TIP, "INSERT INTO item_tool_tip_tmp (`type`, `index`, `name`, name_color, data1, data2, level_tooltip, "
	"text_1, color_1, text_2, color_2, text_3, color_3, text_4, color_4, text_5, color_5, text_6, color_6, text_7, color_7, text_8, color_8, text_9, color_9, text_10, color_10, text_11, color_11, text_12, color_12) "
	"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_LOAD_ITEM_TOOL_TIP, "SELECT * FROM item_tool_tip", CONNECTION_SYNCH);

	/******************************************************************************************
									ITEM_LEVEL_TOOL_TIP
	******************************************************************************************/
	PrepareStatement(QUERY_EDITOR_DELETE_ITEM_LEVEL_TOOL_TIP, "TRUNCATE TABLE item_level_tool_tip_tmp", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_SAVE_ITEM_LEVEL_TOOL_TIP, "INSERT INTO item_level_tool_tip_tmp (`id`, `name`, "
	"text_1, color_1, text_2, color_2, text_3, color_3, text_4, color_4, text_5, color_5, text_6, color_6, text_7, color_7, text_8, color_8, text_9, color_9, text_10, color_10, text_11, color_11, text_12, color_12) "
	"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_LOAD_ITEM_LEVEL_TOOL_TIP, "SELECT * FROM item_level_tool_tip", CONNECTION_SYNCH);

	/******************************************************************************************
									ITEM_TOOL_TIP_TEXT
	******************************************************************************************/
	PrepareStatement(QUERY_EDITOR_DELETE_ITEM_TOOL_TIP_TEXT, "TRUNCATE TABLE item_tool_tip_text_tmp", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_SAVE_ITEM_TOOL_TIP_TEXT, "INSERT INTO item_tool_tip_text_tmp (`id`, `text`, `value`) VALUES (?, ?, ?)", CONNECTION_ASYNC);

	PrepareStatement(QUERY_EDITOR_LOAD_ITEM_TOOL_TIP_TEXT, "SELECT * FROM item_tool_tip_text", CONNECTION_SYNCH);
}