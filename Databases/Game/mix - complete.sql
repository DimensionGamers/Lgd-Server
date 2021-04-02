/*
 Navicat Premium Data Transfer

 Source Server         : Local
 Source Server Type    : MySQL
 Source Server Version : 50731
 Source Host           : 127.0.0.1:3306
 Source Schema         : server_legend_game

 Target Server Type    : MySQL
 Target Server Version : 50731
 File Encoding         : 65001

 Date: 02/04/2021 10:36:50
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for mix_guardian
-- ----------------------------
DROP TABLE IF EXISTS `mix_guardian`;
CREATE TABLE `mix_guardian`  (
  `type` tinyint(3) UNSIGNED NOT NULL,
  `name` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `id` tinyint(3) UNSIGNED NOT NULL,
  `main_item_type` tinyint(4) NOT NULL,
  `main_item_index` smallint(6) NOT NULL,
  `main_item_level` tinyint(3) UNSIGNED NOT NULL,
  `main_item_durability` tinyint(3) UNSIGNED NOT NULL,
  `sub_type_1` tinyint(4) NOT NULL DEFAULT -1,
  `sub_index_1` smallint(6) NOT NULL DEFAULT -1,
  `sub_amount_1` int(11) NOT NULL DEFAULT -1,
  `sub_type_2` tinyint(4) NOT NULL DEFAULT -1,
  `sub_index_2` smallint(6) NOT NULL DEFAULT -1,
  `sub_amount_2` int(11) NOT NULL DEFAULT -1,
  `sub_type_3` tinyint(4) NOT NULL DEFAULT -1,
  `sub_index_3` smallint(6) NOT NULL DEFAULT -1,
  `sub_amount_3` int(11) NOT NULL DEFAULT -1,
  `sub_type_4` tinyint(4) NOT NULL DEFAULT -1,
  `sub_index_4` smallint(6) NOT NULL DEFAULT -1,
  `sub_amount_4` int(11) NOT NULL DEFAULT -1,
  `sub_type_5` tinyint(4) NOT NULL DEFAULT -1,
  `sub_index_5` smallint(6) NOT NULL DEFAULT -1,
  `sub_amount_5` int(11) NOT NULL DEFAULT -1,
  `price` int(11) NOT NULL DEFAULT 0,
  `rate` int(11) NOT NULL DEFAULT 0,
  `result_item_type` tinyint(4) NOT NULL,
  `result_item_index` smallint(6) NOT NULL,
  `result_level` tinyint(3) UNSIGNED NOT NULL,
  PRIMARY KEY (`type`, `id`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of mix_guardian
-- ----------------------------
INSERT INTO `mix_guardian` VALUES (0, 'Dark Horse', 0, 13, 31, 0, 1, 14, 13, 5, 14, 14, 5, 14, 22, 1, 12, 15, 1, -1, -1, -1, 5000000, 1000000, 13, 4, 0);
INSERT INTO `mix_guardian` VALUES (0, 'Dark Raven', 1, 13, 31, 1, 1, 14, 13, 2, 14, 14, 2, 14, 22, 1, 12, 15, 1, -1, -1, -1, 1000000, 1000000, 13, 5, 0);
INSERT INTO `mix_guardian` VALUES (1, 'Ghost Horse', 0, 14, 477, 0, 1, 14, 466, 10, 14, 13, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1000000, 1000000, 12, 471, 0);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 1', 0, -1, -1, 0, 0, 14, 467, 1, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 150000, 1000000, -1, -1, 0);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 2', 1, -1, -1, 0, 0, 14, 467, 3, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 300000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 3', 2, -1, -1, 0, 0, 14, 467, 5, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 450000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 4', 3, -1, -1, 0, 0, 14, 467, 7, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 600000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 5', 4, -1, -1, 0, 0, 14, 467, 9, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 750000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 6', 5, -1, -1, 0, 0, 14, 467, 11, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 900000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 7', 6, -1, -1, 0, 0, 14, 467, 13, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1050000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 8', 7, -1, -1, 0, 0, 14, 467, 15, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1200000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 9', 8, -1, -1, 0, 0, 14, 467, 15, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1350000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 10', 9, -1, -1, 0, 0, 14, 467, 15, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1500000, 1000000, -1, -1, 1);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 11', 10, -1, -1, 0, 0, 14, 468, 1, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1750000, 1000000, -1, -1, 2);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 12', 11, -1, -1, 0, 0, 14, 468, 2, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2000000, 1000000, -1, -1, 2);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 13', 12, -1, -1, 0, 0, 14, 468, 3, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2250000, 1000000, -1, -1, 2);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 14', 13, -1, -1, 0, 0, 14, 468, 4, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2500000, 1000000, -1, -1, 2);
INSERT INTO `mix_guardian` VALUES (2, 'Guardian Enhancement 15', 14, -1, -1, 0, 0, 14, 468, 5, 14, 16, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3000000, 1000000, -1, -1, 2);
INSERT INTO `mix_guardian` VALUES (4, 'Ice Dragon', 0, 12, 471, 11, 1, 14, 498, 1, 14, 13, 20, 14, 14, 20, 12, 15, 1, 14, 22, 1, 0, 1000000, 12, 483, 0);
INSERT INTO `mix_guardian` VALUES (4, 'Ice Dragon', 1, 12, 471, 12, 1, 14, 498, 1, 14, 13, 20, 14, 14, 20, 12, 15, 1, 14, 22, 1, 0, 1000000, 12, 483, 0);
INSERT INTO `mix_guardian` VALUES (4, 'Ice Dragon', 2, 12, 471, 13, 1, 14, 498, 1, 14, 13, 20, 14, 14, 20, 12, 15, 1, 14, 22, 1, 0, 1000000, 12, 483, 0);
INSERT INTO `mix_guardian` VALUES (4, 'Ice Dragon', 3, 12, 471, 14, 1, 14, 498, 1, 14, 13, 20, 14, 14, 20, 12, 15, 1, 14, 22, 1, 0, 1000000, 12, 483, 0);
INSERT INTO `mix_guardian` VALUES (4, 'Ice Dragon', 4, 12, 471, 15, 1, 14, 498, 1, 14, 13, 20, 14, 14, 20, 12, 15, 1, 14, 22, 1, 0, 1000000, 12, 483, 0);

-- ----------------------------
-- Table structure for mix_jewel_template
-- ----------------------------
DROP TABLE IF EXISTS `mix_jewel_template`;
CREATE TABLE `mix_jewel_template`  (
  `id` int(10) UNSIGNED NOT NULL,
  `type` tinyint(3) UNSIGNED NOT NULL,
  `index` smallint(5) UNSIGNED NOT NULL,
  `level` tinyint(3) UNSIGNED NOT NULL,
  `count` int(10) UNSIGNED NOT NULL,
  `mix_money` int(10) UNSIGNED NOT NULL,
  `unmix_money` int(10) UNSIGNED NOT NULL,
  `pack_type` tinyint(3) UNSIGNED NOT NULL,
  `pack_index` smallint(5) UNSIGNED NOT NULL,
  `flags` int(10) UNSIGNED NOT NULL,
  PRIMARY KEY (`id`, `type`, `index`, `level`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of mix_jewel_template
-- ----------------------------
INSERT INTO `mix_jewel_template` VALUES (0, 14, 13, 0, 10, 500000, 500000, 12, 30, 3);
INSERT INTO `mix_jewel_template` VALUES (0, 14, 13, 1, 20, 1000000, 1000000, 12, 30, 3);
INSERT INTO `mix_jewel_template` VALUES (0, 14, 13, 2, 30, 1500000, 1500000, 12, 30, 3);
INSERT INTO `mix_jewel_template` VALUES (1, 14, 14, 0, 10, 500000, 500000, 12, 31, 3);
INSERT INTO `mix_jewel_template` VALUES (1, 14, 14, 1, 20, 1000000, 1000000, 12, 31, 3);
INSERT INTO `mix_jewel_template` VALUES (1, 14, 14, 2, 30, 1500000, 1500000, 12, 31, 3);
INSERT INTO `mix_jewel_template` VALUES (2, 14, 16, 0, 10, 500000, 500000, 12, 136, 3);
INSERT INTO `mix_jewel_template` VALUES (2, 14, 16, 1, 20, 1000000, 1000000, 12, 136, 3);
INSERT INTO `mix_jewel_template` VALUES (2, 14, 16, 2, 30, 1500000, 1500000, 12, 136, 3);
INSERT INTO `mix_jewel_template` VALUES (3, 14, 22, 0, 10, 500000, 500000, 12, 137, 3);
INSERT INTO `mix_jewel_template` VALUES (3, 14, 22, 1, 20, 1000000, 1000000, 12, 137, 3);
INSERT INTO `mix_jewel_template` VALUES (3, 14, 22, 2, 30, 1500000, 1500000, 12, 137, 3);
INSERT INTO `mix_jewel_template` VALUES (4, 14, 31, 0, 10, 500000, 500000, 12, 138, 3);
INSERT INTO `mix_jewel_template` VALUES (4, 14, 31, 1, 20, 1000000, 1000000, 12, 138, 3);
INSERT INTO `mix_jewel_template` VALUES (4, 14, 31, 2, 30, 1500000, 1500000, 12, 138, 3);
INSERT INTO `mix_jewel_template` VALUES (5, 14, 41, 0, 10, 500000, 500000, 12, 139, 3);
INSERT INTO `mix_jewel_template` VALUES (5, 14, 41, 1, 20, 1000000, 1000000, 12, 139, 3);
INSERT INTO `mix_jewel_template` VALUES (5, 14, 41, 2, 30, 1500000, 1500000, 12, 139, 3);
INSERT INTO `mix_jewel_template` VALUES (6, 14, 42, 0, 10, 500000, 500000, 12, 140, 3);
INSERT INTO `mix_jewel_template` VALUES (6, 14, 42, 1, 20, 1000000, 1000000, 12, 140, 3);
INSERT INTO `mix_jewel_template` VALUES (6, 14, 42, 2, 30, 1500000, 1500000, 12, 140, 3);
INSERT INTO `mix_jewel_template` VALUES (7, 12, 15, 0, 10, 500000, 500000, 12, 141, 3);
INSERT INTO `mix_jewel_template` VALUES (7, 12, 15, 1, 20, 1000000, 1000000, 12, 141, 3);
INSERT INTO `mix_jewel_template` VALUES (7, 12, 15, 2, 30, 1500000, 1500000, 12, 141, 3);
INSERT INTO `mix_jewel_template` VALUES (8, 14, 43, 0, 10, 500000, 500000, 12, 142, 3);
INSERT INTO `mix_jewel_template` VALUES (8, 14, 43, 1, 20, 1000000, 1000000, 12, 142, 3);
INSERT INTO `mix_jewel_template` VALUES (8, 14, 43, 2, 30, 1500000, 1500000, 12, 142, 3);
INSERT INTO `mix_jewel_template` VALUES (9, 14, 44, 0, 10, 500000, 500000, 12, 143, 3);
INSERT INTO `mix_jewel_template` VALUES (9, 14, 44, 1, 20, 1000000, 1000000, 12, 143, 3);
INSERT INTO `mix_jewel_template` VALUES (9, 14, 44, 2, 30, 1500000, 1500000, 12, 143, 3);

-- ----------------------------
-- Table structure for mix_lucky_template
-- ----------------------------
DROP TABLE IF EXISTS `mix_lucky_template`;
CREATE TABLE `mix_lucky_template`  (
  `ticket_type` tinyint(3) UNSIGNED NOT NULL,
  `ticket_index` smallint(5) UNSIGNED NOT NULL,
  `ticket_level` tinyint(3) UNSIGNED NOT NULL,
  `item_type` tinyint(3) UNSIGNED NOT NULL,
  `item_index` smallint(5) UNSIGNED NOT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of mix_lucky_template
-- ----------------------------
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 62);
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 63);
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 64);
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 65);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 62);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 63);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 64);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 65);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 62);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 63);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 64);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 65);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 62);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 63);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 64);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 65);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 62);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 63);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 64);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 65);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 66);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 67);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 68);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 69);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 70);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 72);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 66);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 67);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 68);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 69);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 70);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 71);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 72);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 66);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 67);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 68);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 69);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 70);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 71);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 72);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 66);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 67);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 68);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 69);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 70);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 71);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 66);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 67);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 68);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 69);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 70);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 71);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 72);
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 96);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 96);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 96);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 96);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 96);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 97);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 97);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 97);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 97);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 97);
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 124);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 124);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 124);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 124);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 124);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 125);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 125);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 125);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 125);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 125);
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 186);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 186);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 186);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 186);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 186);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 187);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 187);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 187);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 187);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 187);
INSERT INTO `mix_lucky_template` VALUES (13, 137, 0, 7, 218);
INSERT INTO `mix_lucky_template` VALUES (13, 135, 0, 8, 218);
INSERT INTO `mix_lucky_template` VALUES (13, 136, 0, 9, 218);
INSERT INTO `mix_lucky_template` VALUES (13, 138, 0, 10, 218);
INSERT INTO `mix_lucky_template` VALUES (13, 139, 0, 11, 218);
INSERT INTO `mix_lucky_template` VALUES (13, 142, 0, 7, 219);
INSERT INTO `mix_lucky_template` VALUES (13, 140, 0, 8, 219);
INSERT INTO `mix_lucky_template` VALUES (13, 141, 0, 9, 219);
INSERT INTO `mix_lucky_template` VALUES (13, 143, 0, 10, 219);
INSERT INTO `mix_lucky_template` VALUES (13, 144, 0, 11, 219);

-- ----------------------------
-- Table structure for mix_spellstone
-- ----------------------------
DROP TABLE IF EXISTS `mix_spellstone`;
CREATE TABLE `mix_spellstone`  (
  `type` tinyint(3) UNSIGNED NOT NULL,
  `count` smallint(5) UNSIGNED NOT NULL,
  `rate_1` smallint(5) UNSIGNED NOT NULL,
  `rate_2` smallint(5) UNSIGNED NOT NULL,
  `rate_3` smallint(5) UNSIGNED NOT NULL,
  `rate_4` smallint(5) UNSIGNED NOT NULL,
  `rate_5` smallint(5) UNSIGNED NOT NULL,
  `rate_6` smallint(5) UNSIGNED NOT NULL,
  `rate_7` smallint(5) UNSIGNED NOT NULL,
  `rate_8` smallint(5) UNSIGNED NOT NULL,
  `rate_9` smallint(5) UNSIGNED NOT NULL,
  `rate_10` smallint(5) UNSIGNED NOT NULL,
  `rate_11` smallint(5) UNSIGNED NOT NULL,
  `rate_12` smallint(5) UNSIGNED NOT NULL,
  `rate_13` smallint(5) UNSIGNED NOT NULL,
  `rate_14` smallint(5) UNSIGNED NOT NULL,
  `rate_15` smallint(5) UNSIGNED NOT NULL,
  PRIMARY KEY (`type`, `count`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of mix_spellstone
-- ----------------------------
INSERT INTO `mix_spellstone` VALUES (0, 0, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (0, 4, 8500, 1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (0, 14, 0, 0, 5500, 4500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (0, 22, 0, 0, 0, 1000, 8500, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (0, 30, 0, 0, 0, 0, 0, 1000, 8500, 500, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (0, 40, 0, 0, 0, 0, 0, 0, 0, 1000, 8500, 500, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 8000, 1000, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 0, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 24, 0, 9500, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 54, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 84, 0, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 119, 0, 0, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 150, 0, 0, 0, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 184, 0, 0, 0, 0, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 210, 0, 0, 0, 0, 0, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (1, 238, 0, 0, 0, 0, 0, 0, 0, 0, 9500, 500, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (2, 0, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (2, 24, 0, 9500, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (2, 54, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (2, 89, 0, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (2, 115, 0, 0, 0, 500, 9000, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `mix_spellstone` VALUES (2, 145, 0, 0, 0, 0, 0, 9500, 500, 0, 0, 0, 0, 0, 0, 0, 0);

SET FOREIGN_KEY_CHECKS = 1;
