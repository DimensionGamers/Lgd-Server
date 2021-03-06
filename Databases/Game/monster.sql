SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for monster
-- ----------------------------
DROP TABLE IF EXISTS `monster`;
CREATE TABLE `monster`  (
  `server` smallint(5) UNSIGNED NOT NULL,
  `guid` smallint(5) UNSIGNED NOT NULL,
  `id` smallint(5) UNSIGNED NOT NULL COMMENT 'Monster ID defined in monster_template',
  `world` smallint(5) UNSIGNED NOT NULL COMMENT 'Monster World ID defined in world',
  `x1` smallint(6) NOT NULL COMMENT 'Monster World X 0 - 255',
  `y1` smallint(6) NOT NULL COMMENT 'Monster World Y 0 - 255',
  `x2` smallint(6) NOT NULL DEFAULT -1,
  `y2` smallint(6) NOT NULL DEFAULT -1,
  `direction` tinyint(4) NOT NULL DEFAULT -1 COMMENT 'Monster View Direction ( -1 = random )',
  `spawn_delay` int(10) UNSIGNED NOT NULL COMMENT 'Delay time to spawn monster',
  `spawn_distance` tinyint(3) UNSIGNED NOT NULL COMMENT 'Max spawn distance relative to start position',
  `respawn_time_min` int(10) UNSIGNED NOT NULL,
  `respawn_time_max` int(10) UNSIGNED NOT NULL,
  `respawn_id` int(10) UNSIGNED NOT NULL,
  `move_distance` tinyint(3) UNSIGNED NOT NULL,
  `npc_function` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `itembag` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `script_name` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `elemental_attribute` tinyint(3) UNSIGNED NOT NULL,
  `disabled` tinyint(3) UNSIGNED NOT NULL,
  PRIMARY KEY (`server`, `guid`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
