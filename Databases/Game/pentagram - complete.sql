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

 Date: 27/03/2021 21:27:15
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for pentagram_jewel_option
-- ----------------------------
DROP TABLE IF EXISTS `pentagram_jewel_option`;
CREATE TABLE `pentagram_jewel_option`  (
  `index` tinyint(3) UNSIGNED NOT NULL,
  `item_type` tinyint(3) UNSIGNED NOT NULL,
  `item_index` smallint(5) UNSIGNED NOT NULL,
  `rank_number` tinyint(3) UNSIGNED NOT NULL,
  `rank_option` tinyint(3) UNSIGNED NOT NULL,
  `rate` smallint(5) UNSIGNED NOT NULL,
  `unk` tinyint(3) UNSIGNED NOT NULL,
  `value_0` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_0` smallint(5) UNSIGNED NOT NULL,
  `value_1` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_1` smallint(5) UNSIGNED NOT NULL,
  `value_2` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_2` smallint(5) UNSIGNED NOT NULL,
  `value_3` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_3` smallint(5) UNSIGNED NOT NULL,
  `value_4` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_4` smallint(5) UNSIGNED NOT NULL,
  `value_5` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_5` smallint(5) UNSIGNED NOT NULL,
  `value_6` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_6` smallint(5) UNSIGNED NOT NULL,
  `value_7` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_7` smallint(5) UNSIGNED NOT NULL,
  `value_8` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_8` smallint(5) UNSIGNED NOT NULL,
  `value_9` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_9` smallint(5) UNSIGNED NOT NULL,
  `value_10` int(11) NOT NULL DEFAULT 0,
  `upgrade_rate_10` smallint(5) UNSIGNED NOT NULL,
  `name` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `variable_type` tinyint(3) UNSIGNED NOT NULL,
  PRIMARY KEY (`index`, `item_type`, `item_index`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of pentagram_jewel_option
-- ----------------------------
INSERT INTO `pentagram_jewel_option` VALUES (0, 12, 221, 1, 1, 10000, 101, 30, 0, 33, 9000, 38, 8500, 45, 8000, 54, 7500, 65, 7000, 78, 6500, 93, 6000, 113, 5500, 138, 5000, 168, 5000, 'Elemental Damage +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (10, 12, 221, 2, 1, 2000, 111, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (11, 12, 221, 2, 2, 2000, 112, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (12, 12, 221, 2, 3, 2000, 113, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (13, 12, 221, 2, 4, 2000, 114, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (14, 12, 221, 2, 5, 2000, 115, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (20, 12, 221, 3, 1, 5000, 121, 30, 0, 35, 8000, 43, 7500, 54, 7000, 66, 6500, 80, 6000, 97, 5500, 117, 5000, 142, 4500, 172, 4000, 207, 4000, 'Elemental Damage +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (21, 12, 221, 3, 2, 5000, 122, 30, 0, 35, 8000, 43, 7500, 54, 7000, 66, 6500, 80, 6000, 97, 5500, 117, 5000, 142, 4500, 172, 4000, 207, 4000, 'Elemental Damage +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (250, 12, 222, 1, 14, 10000, 150, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Elemental Damage +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (0, 12, 505, 1, 1, 10000, 101, 93, 0, 96, 6500, 101, 6500, 108, 6000, 117, 6000, 128, 5500, 141, 5000, 156, 5000, 176, 4500, 201, 4500, 231, 3000, 'Elemental Damage +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (10, 12, 505, 2, 1, 2000, 111, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (11, 12, 505, 2, 2, 2000, 112, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (12, 12, 505, 2, 3, 2000, 113, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (13, 12, 505, 2, 4, 2000, 114, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (14, 12, 505, 2, 5, 2000, 115, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (20, 12, 505, 3, 1, 5000, 121, 117, 0, 122, 6500, 130, 6500, 141, 6000, 153, 6000, 167, 5500, 184, 5000, 204, 5000, 229, 4500, 259, 4500, 294, 3000, 'Elemental Damage +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (21, 12, 505, 3, 2, 5000, 122, 117, 0, 122, 6500, 130, 6500, 141, 6000, 153, 6000, 167, 5500, 184, 5000, 204, 5000, 229, 4500, 259, 4500, 294, 3000, 'Elemental Damage +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (50, 12, 231, 1, 1, 10000, 201, 10, 0, 13, 9000, 17, 8500, 22, 8000, 28, 7500, 35, 7000, 43, 6500, 52, 6000, 62, 5500, 73, 5000, 85, 5000, 'Elemental Defense +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (60, 12, 231, 2, 1, 2000, 211, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (61, 12, 231, 2, 2, 2000, 212, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (62, 12, 231, 2, 3, 2000, 213, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (63, 12, 231, 2, 4, 2000, 214, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (64, 12, 231, 2, 5, 2000, 215, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (70, 12, 231, 3, 1, 5000, 221, 10, 0, 13, 8000, 17, 7500, 22, 7000, 28, 6500, 35, 6000, 43, 5500, 52, 5000, 62, 4500, 73, 4000, 85, 4000, 'Elemental Defense +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (71, 12, 231, 3, 2, 5000, 222, 10, 0, 13, 8000, 17, 7500, 22, 7000, 28, 6500, 35, 6000, 43, 5500, 52, 5000, 62, 4500, 73, 4000, 85, 4000, 'Elemental Defense +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (251, 12, 232, 1, 14, 10000, 250, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Elemental Defense +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (50, 12, 506, 1, 1, 10000, 201, 52, 0, 55, 6500, 59, 6500, 64, 6000, 70, 6000, 77, 5500, 85, 5000, 94, 5000, 104, 4500, 115, 4500, 127, 3000, 'Elemental Defense +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (60, 12, 506, 2, 1, 2000, 211, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (61, 12, 506, 2, 2, 2000, 212, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (62, 12, 506, 2, 3, 2000, 213, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (63, 12, 506, 2, 4, 2000, 214, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (64, 12, 506, 2, 5, 2000, 215, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (70, 12, 506, 3, 1, 5000, 221, 52, 0, 55, 6500, 59, 6500, 64, 6000, 70, 6000, 77, 5500, 85, 5000, 94, 5000, 104, 4500, 115, 4500, 127, 3000, 'Elemental Defense +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (71, 12, 506, 3, 2, 5000, 222, 52, 0, 55, 6500, 59, 6500, 64, 6000, 70, 6000, 77, 5500, 85, 5000, 94, 5000, 104, 4500, 115, 4500, 127, 3000, 'Elemental Defense +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (100, 12, 241, 1, 1, 10000, 255, 290, 0, 348, 9000, 406, 8500, 464, 8000, 522, 7500, 580, 7000, 697, 6500, 814, 6000, 931, 5500, 1048, 5000, 1165, 5000, 'Elemental Attack Success Rate +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (110, 12, 241, 2, 1, 2000, 255, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (111, 12, 241, 2, 2, 2000, 255, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (112, 12, 241, 2, 3, 2000, 255, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (113, 12, 241, 2, 4, 2000, 255, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (114, 12, 241, 2, 5, 2000, 255, 226, 0, 242, 8500, 258, 8000, 274, 7500, 290, 7000, 306, 6500, 337, 6000, 368, 5500, 399, 5000, 430, 4500, 461, 4500, 'Elemental Damage Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (120, 12, 241, 3, 1, 5000, 255, 30, 0, 35, 8000, 43, 7500, 54, 7000, 66, 6500, 80, 6000, 97, 5500, 117, 5000, 142, 4500, 172, 4000, 207, 4000, 'Elemental Damage +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (121, 12, 241, 3, 2, 5000, 255, 30, 0, 35, 8000, 43, 7500, 54, 7000, 66, 6500, 80, 6000, 97, 5500, 117, 5000, 142, 4500, 172, 4000, 207, 4000, 'Elemental Damage +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (252, 12, 242, 1, 14, 10000, 255, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Elemental Attack Success Rate +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (100, 12, 507, 1, 1, 10000, 255, 814, 0, 872, 6500, 930, 6500, 988, 6000, 1046, 6000, 1104, 5500, 1221, 5000, 1338, 5000, 1455, 4500, 1572, 4500, 1689, 3000, 'Elemental Attack Success Rate +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (110, 12, 507, 2, 1, 2000, 255, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (111, 12, 507, 2, 2, 2000, 255, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (112, 12, 507, 2, 3, 2000, 255, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (113, 12, 507, 2, 4, 2000, 255, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (114, 12, 507, 2, 5, 2000, 255, 368, 0, 384, 6500, 400, 6500, 416, 6000, 432, 6000, 448, 5500, 479, 5000, 510, 5000, 541, 4500, 572, 4500, 603, 3000, 'Elemental Damage Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (120, 12, 507, 3, 1, 5000, 255, 117, 0, 122, 6500, 130, 6500, 141, 6000, 153, 6000, 167, 5500, 184, 5000, 204, 5000, 229, 4500, 259, 4500, 294, 3000, 'Elemental Damage +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (121, 12, 507, 3, 2, 5000, 255, 117, 0, 122, 6500, 130, 6500, 141, 6000, 153, 6000, 167, 5500, 184, 5000, 204, 5000, 229, 4500, 259, 4500, 294, 3000, 'Elemental Damage +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (150, 12, 251, 1, 1, 10000, 255, 17, 0, 21, 9000, 25, 8500, 29, 8000, 33, 7500, 37, 7000, 44, 6500, 51, 6000, 58, 5500, 65, 5000, 72, 5000, 'Elemental Defense Success Rate +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (160, 12, 251, 2, 1, 2000, 255, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (161, 12, 251, 2, 2, 2000, 255, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (162, 12, 251, 2, 3, 2000, 255, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (163, 12, 251, 2, 4, 2000, 255, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (164, 12, 251, 2, 5, 2000, 255, 90, 0, 106, 8500, 122, 8000, 138, 7500, 154, 7000, 170, 6500, 201, 6000, 232, 5500, 263, 5000, 294, 4500, 325, 4500, 'Elemental Defense Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (170, 12, 251, 3, 1, 5000, 255, 10, 0, 13, 8000, 17, 7500, 22, 7000, 28, 6500, 35, 6000, 43, 5500, 52, 5000, 62, 4500, 73, 4000, 85, 4000, 'Elemental Defense +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (171, 12, 251, 3, 2, 5000, 255, 10, 0, 13, 8000, 17, 7500, 22, 7000, 28, 6500, 35, 6000, 43, 5500, 52, 5000, 62, 4500, 73, 4000, 85, 4000, 'Elemental Defense +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (253, 12, 252, 1, 14, 10000, 255, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Elemental Defense Success Rate +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (150, 12, 508, 1, 1, 10000, 255, 51, 0, 55, 6500, 59, 6500, 63, 6000, 67, 6000, 71, 5500, 78, 5000, 85, 5000, 92, 4500, 99, 4500, 106, 3000, 'Elemental Defense Success Rate +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (160, 12, 508, 2, 1, 2000, 255, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Fire Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (161, 12, 508, 2, 2, 2000, 255, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Water Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (162, 12, 508, 2, 3, 2000, 255, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Earth Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (163, 12, 508, 2, 4, 2000, 255, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Wind Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (164, 12, 508, 2, 5, 2000, 255, 232, 0, 248, 6500, 264, 6500, 280, 6000, 296, 6000, 312, 5500, 343, 5000, 374, 5000, 405, 4500, 436, 4500, 467, 3000, 'Elemental Defense Against Darkness Element +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (170, 12, 508, 3, 1, 5000, 255, 52, 0, 55, 6500, 59, 6500, 64, 6000, 70, 6000, 77, 5500, 85, 5000, 94, 5000, 104, 4500, 115, 4500, 127, 3000, 'Elemental Defense +%d (PvP)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (171, 12, 508, 3, 2, 5000, 255, 52, 0, 55, 6500, 59, 6500, 64, 6000, 70, 6000, 77, 5500, 85, 5000, 94, 5000, 104, 4500, 115, 4500, 127, 3000, 'Elemental Defense +%d (PvM)', 0);
INSERT INTO `pentagram_jewel_option` VALUES (200, 12, 261, 1, 1, 2495, 255, 33, 0, 48, 9000, 63, 8500, 78, 8000, 95, 7500, 112, 7000, 129, 6500, 148, 6000, 167, 5500, 186, 5000, 209, 5000, 'Elemental Damage (II) +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (201, 12, 261, 1, 2, 2495, 255, 13, 0, 21, 9000, 29, 8500, 37, 8000, 46, 7500, 55, 7000, 64, 6500, 74, 6000, 84, 5500, 94, 5000, 107, 5000, 'Elemental Defense (II) +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (202, 12, 261, 1, 3, 2495, 255, 150, 0, 173, 9000, 196, 8500, 219, 8000, 252, 7500, 285, 7000, 318, 6500, 361, 6000, 404, 5500, 447, 5000, 605, 5000, 'Elemental Attack Success Rate (II) +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (203, 12, 261, 1, 4, 2495, 255, 150, 0, 173, 9000, 196, 8500, 219, 8000, 252, 7500, 285, 7000, 318, 6500, 361, 6000, 404, 5500, 447, 5000, 605, 5000, 'Elemental Defense Success Rate (II) +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (204, 12, 261, 1, 5, 10, 255, 33, 0, 52, 9000, 71, 8500, 90, 8000, 120, 7500, 141, 7000, 162, 6500, 191, 6000, 213, 5500, 235, 5000, 261, 5000, 'Elemental Damage (III) +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (205, 12, 261, 1, 6, 10, 255, 13, 0, 23, 9000, 33, 8500, 43, 8000, 54, 7500, 65, 7000, 76, 6500, 90, 6000, 104, 5500, 118, 5000, 135, 5000, 'Elemental Defense (III) +%d', 0);
INSERT INTO `pentagram_jewel_option` VALUES (210, 12, 261, 2, 1, 4995, 255, 1, 0, 2, 8500, 3, 8000, 4, 7500, 5, 7000, 6, 6500, 7, 6000, 8, 5500, 9, 5000, 10, 4500, 11, 4500, 'Absorb SD - 30%% chance to absorb %d%% damage as SD', 1);
INSERT INTO `pentagram_jewel_option` VALUES (211, 12, 261, 2, 2, 4995, 255, 1, 0, 2, 8500, 3, 8000, 4, 7500, 5, 7000, 6, 6500, 7, 6000, 8, 5500, 9, 5000, 10, 4500, 11, 4500, 'Absorb HP - 30%% chance to absorb %d%% damage as HP', 1);
INSERT INTO `pentagram_jewel_option` VALUES (212, 12, 261, 2, 3, 10, 255, 3, 0, 4, 8500, 5, 8000, 6, 7500, 8, 7000, 10, 6500, 12, 6000, 15, 5500, 18, 5000, 21, 4500, 25, 4500, 'Bastion - Protection is activated for %d sec. by 50%% chance when SD is below 20%%', 0);
INSERT INTO `pentagram_jewel_option` VALUES (220, 12, 261, 3, 1, 3399, 255, 100, 0, 140, 8000, 180, 7500, 220, 7000, 260, 6500, 300, 6000, 340, 5500, 380, 5000, 420, 4500, 460, 4000, 500, 4000, 'Bleeding - A durable effect invoking %d damages to the target with certain chance when attackin', 0);
INSERT INTO `pentagram_jewel_option` VALUES (221, 12, 261, 3, 2, 1800, 255, 13, 0, 14, 8000, 15, 7500, 16, 7000, 18, 6500, 20, 6000, 22, 5500, 25, 5000, 28, 4500, 31, 4000, 35, 4000, 'Paralyzing - Chance to decrease target\'s Moving Speed and every Healing ability by 90%%.', 0);
INSERT INTO `pentagram_jewel_option` VALUES (222, 12, 261, 3, 3, 2800, 255, 13, 0, 14, 8000, 15, 7500, 16, 7000, 18, 6500, 20, 6000, 22, 5500, 25, 5000, 28, 4500, 31, 4000, 35, 4000, 'Binding - Immobilize the target by holding its leg with certain chance when attacking.', 0);
INSERT INTO `pentagram_jewel_option` VALUES (223, 12, 261, 3, 4, 1, 255, 6, 0, 7, 8000, 8, 7500, 9, 7000, 10, 6500, 11, 6000, 12, 5500, 13, 5000, 14, 4500, 15, 4000, 16, 4000, 'Punish- 30%% chance to inflict damage (%d%% of target\'s MAX Life) upon Critical Elemental Damage.', 1);
INSERT INTO `pentagram_jewel_option` VALUES (224, 12, 261, 3, 5, 2000, 255, 50, 0, 52, 8000, 54, 7500, 56, 7000, 59, 6500, 62, 6000, 65, 5500, 69, 5000, 73, 4500, 77, 4000, 85, 4000, 'Blinding - Chance to greatly decrease target\'s Attack Success Rate by %d%% when attacking.', 1);

-- ----------------------------
-- Table structure for pentagram_jewel_remove
-- ----------------------------
DROP TABLE IF EXISTS `pentagram_jewel_remove`;
CREATE TABLE `pentagram_jewel_remove`  (
  `index` tinyint(3) UNSIGNED NOT NULL,
  `mix_rate_1` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_2` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_3` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_4` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_5` smallint(5) UNSIGNED NOT NULL,
  PRIMARY KEY (`index`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of pentagram_jewel_remove
-- ----------------------------
INSERT INTO `pentagram_jewel_remove` VALUES (0, 10000, 9000, 8000, 8000, 8000);
INSERT INTO `pentagram_jewel_remove` VALUES (1, 10000, 9000, 8000, 8000, 8000);
INSERT INTO `pentagram_jewel_remove` VALUES (2, 10000, 9000, 8000, 8000, 8000);
INSERT INTO `pentagram_jewel_remove` VALUES (3, 9500, 9000, 8000, 8000, 8000);
INSERT INTO `pentagram_jewel_remove` VALUES (4, 9500, 8500, 7500, 7500, 7500);
INSERT INTO `pentagram_jewel_remove` VALUES (5, 9500, 8500, 7500, 7500, 7500);
INSERT INTO `pentagram_jewel_remove` VALUES (6, 9000, 8500, 7500, 7500, 7500);
INSERT INTO `pentagram_jewel_remove` VALUES (7, 9000, 8000, 7000, 7000, 7000);
INSERT INTO `pentagram_jewel_remove` VALUES (8, 9000, 8000, 7000, 7000, 7000);
INSERT INTO `pentagram_jewel_remove` VALUES (9, 8500, 7500, 7000, 7000, 7000);
INSERT INTO `pentagram_jewel_remove` VALUES (10, 8500, 7500, 7000, 7000, 7000);

-- ----------------------------
-- Table structure for pentagram_jewel_upgrade_level
-- ----------------------------
DROP TABLE IF EXISTS `pentagram_jewel_upgrade_level`;
CREATE TABLE `pentagram_jewel_upgrade_level`  (
  `index` tinyint(3) UNSIGNED NOT NULL,
  `mix_money` int(10) UNSIGNED NOT NULL,
  `mix_rate_1` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_2` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_3` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_4` smallint(5) UNSIGNED NOT NULL,
  `mix_rate_5` smallint(5) UNSIGNED NOT NULL,
  `elemental_rune_1` int(11) NOT NULL DEFAULT 0,
  `elemental_rune_2` int(11) NOT NULL DEFAULT 0,
  `elemental_rune_3` int(11) NOT NULL DEFAULT 0,
  `elemental_rune_4` int(11) NOT NULL DEFAULT 0,
  `elemental_rune_5` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`index`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of pentagram_jewel_upgrade_level
-- ----------------------------
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (0, 100000, 100, 100, 100, 100, 100, 1, 1, 1, 1, 1);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (1, 100000, 100, 100, 100, 100, 100, 1, 1, 1, 1, 1);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (2, 150000, 100, 100, 100, 100, 100, 2, 2, 2, 2, 2);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (3, 200000, 100, 100, 100, 100, 100, 2, 2, 2, 2, 2);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (4, 250000, 100, 100, 100, 100, 100, 3, 3, 3, 3, 3);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (5, 300000, 100, 100, 100, 100, 100, 3, 3, 3, 3, 3);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (6, 350000, 100, 100, 100, 100, 100, 4, 4, 4, 4, 4);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (7, 400000, 100, 100, 100, 100, 100, 5, 5, 5, 5, 5);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (8, 450000, 100, 100, 100, 100, 100, 6, 6, 6, 6, 6);
INSERT INTO `pentagram_jewel_upgrade_level` VALUES (9, 500000, 100, 100, 100, 100, 100, 7, 7, 7, 7, 7);

-- ----------------------------
-- Table structure for pentagram_jewel_upgrade_rank
-- ----------------------------
DROP TABLE IF EXISTS `pentagram_jewel_upgrade_rank`;
CREATE TABLE `pentagram_jewel_upgrade_rank`  (
  `index` tinyint(3) UNSIGNED NOT NULL,
  `mix_money` int(10) UNSIGNED NOT NULL,
  `mix_rate` smallint(5) UNSIGNED NOT NULL,
  PRIMARY KEY (`index`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of pentagram_jewel_upgrade_rank
-- ----------------------------
INSERT INTO `pentagram_jewel_upgrade_rank` VALUES (1, 100000, 10000);
INSERT INTO `pentagram_jewel_upgrade_rank` VALUES (2, 100000, 10000);
INSERT INTO `pentagram_jewel_upgrade_rank` VALUES (3, 150000, 10000);
INSERT INTO `pentagram_jewel_upgrade_rank` VALUES (4, 200000, 10000);

-- ----------------------------
-- Table structure for pentagram_option
-- ----------------------------
DROP TABLE IF EXISTS `pentagram_option`;
CREATE TABLE `pentagram_option`  (
  `index` tinyint(3) UNSIGNED NOT NULL,
  `name` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `value` smallint(5) UNSIGNED NOT NULL,
  `type_1` tinyint(4) NOT NULL DEFAULT -1,
  `rank_1` tinyint(4) NOT NULL DEFAULT -1,
  `level_1` tinyint(4) NOT NULL DEFAULT -1,
  `type_2` tinyint(4) NOT NULL DEFAULT -1,
  `rank_2` tinyint(4) NOT NULL DEFAULT -1,
  `level_2` tinyint(4) NOT NULL DEFAULT -1,
  `type_3` tinyint(4) NOT NULL DEFAULT -1,
  `rank_3` tinyint(4) NOT NULL DEFAULT -1,
  `level_3` tinyint(4) NOT NULL DEFAULT -1,
  `type_4` tinyint(4) NOT NULL DEFAULT -1,
  `rank_4` tinyint(4) NOT NULL DEFAULT -1,
  `level_4` tinyint(4) NOT NULL DEFAULT -1,
  `type_5` tinyint(4) NOT NULL DEFAULT -1,
  `rank_5` tinyint(4) NOT NULL DEFAULT -1,
  `level_5` tinyint(4) NOT NULL DEFAULT -1,
  PRIMARY KEY (`index`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of pentagram_option
-- ----------------------------
INSERT INTO `pentagram_option` VALUES (1, 'Pentagram Damage/Defense +5%%', 5, 0, 1, 0, 1, 1, 0, 2, 1, 0, 3, 1, 0, -1, -1, 0);
INSERT INTO `pentagram_option` VALUES (2, 'Pentagram Damage +10%%', 10, 0, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (3, 'Pentagram Defense +10%%', 10, 0, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (4, 'Elemental Critical Damage Rate +10%%', 10, 0, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (5, '가호의 에르텔 1랭크 옵션 효과 증가 +35', 35, 0, 1, 0, 1, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (21, 'Pentagram Damage +10%%', 10, 0, 2, 0, 2, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (22, 'Pentagram Defense +10%%', 10, 1, 2, 0, 3, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (23, 'Elemental Critical Damage Rate +5%%', 5, 1, 1, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (24, 'Elemental Critical Damage Rate +10%%', 10, 0, 2, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (25, 'Elemental Critical Damage Rate +25%%', 25, 0, 3, 0, 1, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (26, 'Elemental Attribute Defense +5%%', 5, 0, 3, 0, 1, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (27, 'Elemental Attribute Damage +1%%', 1, 0, 3, 0, 1, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (28, '%s, %s 속성 공격 완전 회피 확률 +5%%', 5, 1, 2, 0, 2, 1, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (29, 'Pentagram Damage +30', 30, 0, 3, 0, 2, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (30, 'Pentagram Damage +30', 30, 1, 3, 0, 3, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (31, '고결의 에르텔 1랭크 옵션 효과 증가 +20 (미사용 옵션)', 20, 2, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (32, '신성의 에르텔 1랭크 옵션 효과 증가 +20 (미사용 옵션)', 20, 3, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (33, '광휘의 에르텔 1랭크 옵션 효과 증가 +20 (미사용 옵션)', 20, 4, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (34, 'Pentagram Damage/Defense +5%%', 5, 0, 1, 7, 1, 1, 7, 4, 1, 7, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (41, 'Ignore Elemental Defense +5%%', 5, 1, 3, 6, 2, 3, 6, 3, 3, 6, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (42, 'Elemental Attribute Defense +5%%', 5, 0, 3, 5, 1, 3, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `pentagram_option` VALUES (43, 'Elemental Attribute Damage +1%%', 1, 0, 3, 5, 1, 3, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1);

-- ----------------------------
-- Table structure for pentagram_type
-- ----------------------------
DROP TABLE IF EXISTS `pentagram_type`;
CREATE TABLE `pentagram_type`  (
  `index` smallint(5) UNSIGNED NOT NULL,
  `name` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `rate` tinyint(3) UNSIGNED NOT NULL,
  `option_1` tinyint(3) UNSIGNED NOT NULL,
  `option_2` tinyint(3) UNSIGNED NOT NULL,
  `option_3` tinyint(3) UNSIGNED NOT NULL,
  `option_4` tinyint(3) UNSIGNED NOT NULL,
  `option_5` tinyint(3) UNSIGNED NOT NULL,
  `option_6` tinyint(3) UNSIGNED NOT NULL,
  `option_7` tinyint(3) UNSIGNED NOT NULL,
  PRIMARY KEY (`index`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of pentagram_type
-- ----------------------------
INSERT INTO `pentagram_type` VALUES (6344, 'Muren\'s Book of Magic', 1, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6345, 'Scroll of Etramu', 1, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6346, 'Lorencia Knights\' Iron Shield', 1, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6347, 'Iron Shield of the Magic', 1, 3, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6348, 'Hero Elixir', 1, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6349, 'Brave Hero Elixir', 1, 4, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6350, 'Gladiator\'s Dagger', 1, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6351, 'Merciless Gladiator\'s Dagger', 1, 2, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6361, 'Bead of Thoughtful Sage', 1, 5, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6352, 'Kundun\'s Madness Blade', 2, 26, 24, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6353, 'Kundun\'s Magic Spell Scroll', 2, 27, 24, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6354, 'Empire Guardians\' Stronghold', 2, 26, 28, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6355, 'Ancient Icarus Scroll', 2, 27, 28, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6362, 'Magic Scroll (attributable)', 2, 27, 24, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6450, 'Golden Line Harp of Lunidel', 2, 34, 21, 29, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6451, 'Precious Bead of Lemuria', 2, 34, 22, 30, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6452, 'Colorful Lira of Norwein', 2, 23, 24, 25, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6356, 'Arca\'s Prophecy', 3, 41, 24, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6357, 'Antonia\'s Sword', 3, 41, 42, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6358, 'Kundun\'s Seal Scroll', 3, 41, 43, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6359, 'Pentagram for Beginners', 1, 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `pentagram_type` VALUES (6360, 'Iron Shield (Bound)', 1, 1, 0, 0, 0, 0, 0, 0);

SET FOREIGN_KEY_CHECKS = 1;
