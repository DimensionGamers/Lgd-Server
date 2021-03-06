SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for monster_event
-- ----------------------------
DROP TABLE IF EXISTS `monster_event`;
CREATE TABLE `monster_event`  (
  `server` smallint(5) UNSIGNED NOT NULL,
  `id` smallint(5) UNSIGNED NOT NULL,
  `world` smallint(5) UNSIGNED NOT NULL,
  `x1` smallint(6) NOT NULL,
  `y1` smallint(6) NOT NULL,
  `x2` smallint(6) NOT NULL DEFAULT -1,
  `y2` smallint(6) NOT NULL DEFAULT -1,
  `direction` tinyint(4) NOT NULL DEFAULT -1,
  `spawn_delay` int(10) UNSIGNED NOT NULL,
  `spawn_distance` tinyint(3) UNSIGNED NOT NULL,
  `respawn_time` int(10) UNSIGNED NOT NULL,
  `respawn_id` int(10) UNSIGNED NOT NULL,
  `move_distance` tinyint(3) UNSIGNED NOT NULL,
  `event_id` tinyint(4) NOT NULL DEFAULT -1,
  `data_1` int(11) NOT NULL DEFAULT 0,
  `data_2` int(11) NOT NULL DEFAULT 0,
  `data_3` int(11) NOT NULL DEFAULT 0,
  `data_4` int(11) NOT NULL DEFAULT 0,
  `data_5` int(11) NOT NULL DEFAULT 0,
  `npc_function` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `itembag` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `script_name` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `ai_group` int(10) UNSIGNED NOT NULL,
  `ai_group_member` int(10) UNSIGNED NOT NULL,
  `count` int(11) NOT NULL DEFAULT 1
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
