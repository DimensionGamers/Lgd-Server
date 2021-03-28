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

 Date: 19/03/2021 11:30:23
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_base
-- ----------------------------
DROP TABLE IF EXISTS `character_base`;
CREATE TABLE `character_base`  (
  `class` tinyint(3) UNSIGNED NOT NULL,
  `strenght` int(10) UNSIGNED NOT NULL,
  `agility` int(10) UNSIGNED NOT NULL,
  `vitality` int(10) UNSIGNED NOT NULL,
  `energy` int(10) UNSIGNED NOT NULL,
  `leadership` int(10) UNSIGNED NOT NULL,
  `life` int(11) NOT NULL DEFAULT 0,
  `mana` int(11) NOT NULL DEFAULT 0,
  `level_to_life` float unsigned NOT NULL,
  `level_to_mana` float unsigned NOT NULL,
  `vitality_to_life` float unsigned NOT NULL,
  `energy_to_mana` float unsigned NOT NULL,
  `world` smallint(5) UNSIGNED NOT NULL,
  `level` smallint(6) NOT NULL DEFAULT 1,
  `points` int(10) UNSIGNED NOT NULL,
  `automatic_life_recovery` float unsigned NOT NULL,
  `automatic_mana_recovery` float unsigned NOT NULL,
  `automatic_shield_recovery` float unsigned NOT NULL,
  `automatic_stamina_recovery` float unsigned NOT NULL,
  `normal_levelup_points` int(11) NOT NULL DEFAULT 5,
  `plus_levelup_points` int(11) NOT NULL DEFAULT 1,
  `master_levelup_points` int(11) NOT NULL DEFAULT 1,
  `majestic_level_up_points` int(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`class`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_base
-- ----------------------------
INSERT INTO `character_base` VALUES (0, 18, 18, 15, 30, 0, 60, 60, 1, 2, 2, 2, 0, 1, 0, 3, 3.7, 0, 6, 5, 1, 1, 1);
INSERT INTO `character_base` VALUES (1, 28, 20, 25, 10, 0, 110, 20, 2, 0.5, 3, 1, 0, 1, 0, 3, 3.7, 0, 8, 5, 1, 1, 1);
INSERT INTO `character_base` VALUES (2, 22, 25, 20, 15, 0, 80, 30, 1, 1.5, 2, 1.5, 3, 1, 0, 3, 3.7, 0, 6, 5, 1, 1, 1);
INSERT INTO `character_base` VALUES (3, 26, 26, 26, 26, 0, 110, 60, 1, 1, 2, 2, 0, 1, 0, 3, 3.7, 0, 6, 7, 0, 1, 1);
INSERT INTO `character_base` VALUES (4, 26, 20, 20, 15, 25, 90, 40, 1.5, 1, 2, 1.5, 0, 1, 0, 3, 3.7, 0, 6, 7, 0, 1, 1);
INSERT INTO `character_base` VALUES (5, 21, 21, 18, 23, 0, 70, 20, 1, 1.5, 2, 1.7, 51, 1, 0, 3, 3.7, 0, 6, 5, 1, 1, 1);
INSERT INTO `character_base` VALUES (6, 32, 27, 25, 20, 0, 100, 40, 1.3, 1, 2, 1.3, 0, 1, 0, 3, 3.7, 0, 6, 7, 0, 1, 1);
INSERT INTO `character_base` VALUES (7, 30, 30, 25, 24, 0, 110, 40, 2, 1, 2, 1, 0, 1, 0, 3, 3.7, 0, 6, 7, 0, 1, 1);
INSERT INTO `character_base` VALUES (8, 13, 18, 14, 40, 0, 60, 60, 1.2, 1.8, 2, 1.5, 3, 1, 0, 3, 3.7, 0, 3, 5, 1, 1, 1);
INSERT INTO `character_base` VALUES (9, 28, 30, 15, 10, 0, 130, 10, 1.5, 1, 3, 1, 0, 1, 0, 3, 3.7, 0, 3, 5, 1, 1, 1);
INSERT INTO `character_base` VALUES (10, 20, 18, 20, 25, 0, 100, 80, 1.5, 1.5, 2, 2, 51, 1, 0, 3, 3.7, 0, 3, 5, 1, 1, 1);

-- ----------------------------
-- Table structure for character_bonus
-- ----------------------------
DROP TABLE IF EXISTS `character_bonus`;
CREATE TABLE `character_bonus`  (
  `server` smallint(5) UNSIGNED NOT NULL,
  `min_level` smallint(6) NOT NULL DEFAULT 0,
  `max_level` smallint(6) NOT NULL DEFAULT 0,
  `experience_rate` smallint(6) NOT NULL DEFAULT 0,
  `drop_rate` smallint(6) NOT NULL DEFAULT 0,
  PRIMARY KEY (`server`, `min_level`, `max_level`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Table structure for character_experience_adjust
-- ----------------------------
DROP TABLE IF EXISTS `character_experience_adjust`;
CREATE TABLE `character_experience_adjust`  (
  `min_level` smallint(6) NOT NULL DEFAULT 0,
  `max_level` smallint(6) NOT NULL DEFAULT 0,
  `experience_rate` smallint(6) NOT NULL DEFAULT 0
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Table structure for character_experience_bonus
-- ----------------------------
DROP TABLE IF EXISTS `character_experience_bonus`;
CREATE TABLE `character_experience_bonus`  (
  `race_1` tinyint(3) UNSIGNED NOT NULL,
  `race_2` tinyint(3) UNSIGNED NOT NULL,
  `race_3` tinyint(3) UNSIGNED NOT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_experience_bonus
-- ----------------------------
INSERT INTO `character_experience_bonus` VALUES (0, 1, 2);
INSERT INTO `character_experience_bonus` VALUES (3, 4, 5);
INSERT INTO `character_experience_bonus` VALUES (0, 2, 7);
INSERT INTO `character_experience_bonus` VALUES (4, 5, 6);
INSERT INTO `character_experience_bonus` VALUES (1, 3, 8);
INSERT INTO `character_experience_bonus` VALUES (1, 5, 9);
INSERT INTO `character_experience_bonus` VALUES (10, 8, 7);

-- ----------------------------
-- Table structure for character_experience_table
-- ----------------------------
DROP TABLE IF EXISTS `character_experience_table`;
CREATE TABLE `character_experience_table`  (
  `level_min` smallint(6) NOT NULL,
  `level_max` smallint(6) NOT NULL,
  `experience_rate` int(11) NOT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Table structure for character_formula
-- ----------------------------
DROP TABLE IF EXISTS `character_formula`;
CREATE TABLE `character_formula`  (
  `stat_id` tinyint(3) UNSIGNED NOT NULL,
  `race` tinyint(3) UNSIGNED NOT NULL,
  `level_mul` float NOT NULL DEFAULT 0,
  `level_div` float NOT NULL DEFAULT 1,
  `strength_mul` float NOT NULL DEFAULT 0,
  `strength_div` float NOT NULL DEFAULT 1,
  `agility_mul` float NOT NULL DEFAULT 0,
  `agility_div` float NOT NULL DEFAULT 1,
  `vitality_mul` float NOT NULL DEFAULT 0,
  `vitality_div` float NOT NULL DEFAULT 1,
  `energy_mul` float NOT NULL DEFAULT 0,
  `energy_div` float NOT NULL DEFAULT 1,
  `leadership_mul` float NOT NULL DEFAULT 0,
  `leadership_div` float NOT NULL DEFAULT 1,
  PRIMARY KEY (`stat_id`, `race`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_formula
-- ----------------------------
INSERT INTO `character_formula` VALUES (0, 0, 0, 1, 1, 8, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (0, 1, 0, 1, 1, 6, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (0, 2, 0, 1, 1, 7, 1, 7, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (0, 3, 0, 1, 1, 6, 0, 1, 0, 1, 1, 12, 0, 1);
INSERT INTO `character_formula` VALUES (0, 4, 0, 1, 1, 7, 0, 1, 0, 1, 1, 14, 0, 1);
INSERT INTO `character_formula` VALUES (0, 5, 0, 1, 1, 7, 1, 7, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (0, 6, 0, 1, 1, 7, 0, 1, 1, 15, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 0, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 2, 0, 1, 1, 4, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 3, 0, 1, 1, 4, 0, 1, 0, 1, 1, 8, 0, 1);
INSERT INTO `character_formula` VALUES (1, 4, 0, 1, 1, 5, 0, 1, 0, 1, 1, 10, 0, 1);
INSERT INTO `character_formula` VALUES (1, 5, 0, 1, 1, 4, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 6, 0, 1, 1, 5, 0, 1, 1, 12, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 0, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 1, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 2, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 3, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 4, 5, 1, 1, 4, 6, 2, 0, 1, 0, 1, 1, 10);
INSERT INTO `character_formula` VALUES (2, 5, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 6, 3, 1, 1, 6, 5, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 0, 3, 1, 0, 1, 4, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 1, 3, 1, 0, 1, 4.5, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 2, 3, 1, 0, 1, 0.6, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 3, 3, 1, 0, 1, 3.5, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 4, 3, 1, 0, 1, 4, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 5, 3, 1, 0, 1, 3.5, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 6, 2.6, 1, 0, 1, 3.6, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 0, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 1, 0, 1, 0, 1, 1, 15, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 2, 0, 1, 0, 1, 1, 50, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 3, 0, 1, 0, 1, 1, 15, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 4, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 5, 0, 1, 0, 1, 1, 20, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 6, 0, 1, 0, 1, 1, 9, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 0, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 1, 0, 1, 0, 1, 1, 15, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 2, 0, 1, 0, 1, 1, 50, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 3, 0, 1, 0, 1, 1, 15, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 4, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 5, 0, 1, 0, 1, 1, 20, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 6, 0, 1, 0, 1, 1, 9, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 0, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 1, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 2, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 3, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 4, 0, 1, 0, 1, 1, 7, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 5, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 6, 0, 1, 0, 1, 1, 8, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 0, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 1, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 2, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 3, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 4, 0, 1, 0, 1, 1, 7, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 5, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 6, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 0, 2, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 1, 2, 1, 0, 1, 1, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 2, 2, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 3, 2, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 4, 2, 1, 0, 1, 1, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 5, 2, 1, 0, 1, 1, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 6, 1.5, 1, 0, 1, 0.2, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (9, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (9, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (9, 2, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (9, 3, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (9, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (9, 5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (9, 6, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (10, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (10, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (10, 2, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (10, 3, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (10, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (10, 5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (10, 6, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (11, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (11, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (11, 2, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (11, 3, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (11, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (11, 5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 18, 0, 1);
INSERT INTO `character_formula` VALUES (11, 6, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (12, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (12, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (12, 2, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (12, 3, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (12, 4, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (12, 5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 10, 0, 1);
INSERT INTO `character_formula` VALUES (12, 6, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (13, 0, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (13, 1, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (13, 2, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (13, 3, 0, 1, 0, 1, 1, 5, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (13, 4, 0, 1, 0, 1, 1, 7, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (13, 5, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (13, 6, 0, 1, 0, 1, 1, 8, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (14, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (15, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 6, 0, 1);
INSERT INTO `character_formula` VALUES (14, 1, 0, 1, 1, 6, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (15, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (14, 2, 0, 1, 1, 14, 1, 8, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (15, 2, 0, 1, 1, 8, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (14, 3, 0, 1, 1, 10, 0, 1, 0, 1, 1, 14, 0, 1);
INSERT INTO `character_formula` VALUES (15, 3, 0, 1, 1, 6, 0, 1, 0, 1, 1, 8, 0, 1);
INSERT INTO `character_formula` VALUES (14, 4, 0, 1, 1, 10, 0, 1, 0, 1, 1, 14, 0, 1);
INSERT INTO `character_formula` VALUES (15, 4, 0, 1, 1, 6, 0, 1, 0, 1, 1, 10, 0, 1);
INSERT INTO `character_formula` VALUES (14, 5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 10, 0, 1);
INSERT INTO `character_formula` VALUES (15, 5, 0, 1, 0, 1, 0, 1, 0, 1, 1, 6, 0, 1);
INSERT INTO `character_formula` VALUES (14, 6, 0, 1, 1, 10, 0, 1, 1, 20, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (15, 6, 0, 1, 1, 6, 0, 1, 1, 15, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 0, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 1, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 2, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 3, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 4, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 5, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 6, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 0, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 1, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 2, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 3, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 4, 0, 1, 0, 1, 1, 7, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 5, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 6, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (0, 7, 0, 1, 1, 8, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 7, 0, 1, 1, 4, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 7, 5, 1, 1, 4, 5, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 7, 3, 1, 0, 1, 2.5, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 7, 0, 1, 0, 1, 1, 20, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 7, 0, 1, 0, 1, 1, 20, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 7, 0, 1, 0, 1, 1, 7, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 7, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 7, 2, 1, 0, 1, 1, 5, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (9, 7, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (10, 7, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (11, 7, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (12, 7, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (13, 7, 0, 1, 0, 1, 1, 8, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (14, 7, 0, 1, 1, 5, 1, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (15, 7, 0, 1, 1, 3, 1, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 7, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 7, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (18, 0, 0, 1, 1, 3, 1, 5, 1, 5, 1, 7, 0, 1);
INSERT INTO `character_formula` VALUES (18, 1, 0, 1, 1, 5, 1, 5, 1, 7, 1, 3, 0, 1);
INSERT INTO `character_formula` VALUES (18, 2, 0, 1, 1, 5, 1, 3, 1, 7, 1, 5, 0, 1);
INSERT INTO `character_formula` VALUES (18, 3, 0, 1, 1, 3, 1, 5, 1, 5, 1, 7, 0, 1);
INSERT INTO `character_formula` VALUES (18, 4, 0, 1, 1, 5, 1, 5, 1, 7, 1, 3, 1, 3);
INSERT INTO `character_formula` VALUES (18, 5, 0, 1, 1, 5, 1, 5, 1, 7, 1, 3, 0, 1);
INSERT INTO `character_formula` VALUES (18, 6, 0, 1, 1, 3, 1, 5, 1, 5, 1, 7, 0, 1);
INSERT INTO `character_formula` VALUES (18, 7, 0, 1, 1, 3, 1, 5, 1, 5, 1, 7, 0, 1);
INSERT INTO `character_formula` VALUES (19, 1, 0, 1, 1.5, 1, 1, 1, 0, 1, 1, 1, 0, 1);
INSERT INTO `character_formula` VALUES (0, 8, 0, 1, 1, 8, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 8, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 8, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 8, 3, 1, 0, 1, 4, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 8, 0, 1, 0, 1, 1, 12, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 8, 0, 1, 0, 1, 1, 12, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 8, 0, 1, 0, 1, 1, 5, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 8, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 8, 2, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (9, 8, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (10, 8, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (11, 8, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (12, 8, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (13, 8, 0, 1, 0, 1, 1, 5, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (14, 8, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (15, 8, 0, 1, 0, 1, 0, 1, 0, 1, 1, 6, 0, 1);
INSERT INTO `character_formula` VALUES (16, 8, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 8, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (18, 8, 0, 1, 1, 3, 1, 5, 1, 5, 1, 7, 0, 1);
INSERT INTO `character_formula` VALUES (0, 9, 0, 1, 1, 9, 1, 20, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 9, 0, 1, 1, 5, 1, 14, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 9, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 9, 3, 1, 0, 1, 2.5, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 9, 0, 1, 0, 1, 1, 12, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 9, 0, 1, 0, 1, 1, 12, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 9, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 9, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 9, 2, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (9, 9, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (10, 9, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (11, 9, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (12, 9, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (13, 9, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (14, 9, 0, 1, 1, 6, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (15, 9, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (16, 9, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 9, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (18, 9, 0, 1, 1, 5, 1, 5, 1, 7, 1, 3, 0, 1);
INSERT INTO `character_formula` VALUES (0, 10, 0, 1, 1, 8, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (1, 10, 0, 1, 1, 5, 0, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (2, 10, 5, 1, 1, 4, 3, 2, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (3, 10, 3, 1, 0, 1, 3, 1, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (4, 10, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (5, 10, 0, 1, 0, 1, 1, 10, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (6, 10, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (7, 10, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (8, 10, 2, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (9, 10, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (10, 10, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (11, 10, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (12, 10, 0, 1, 0, 1, 0, 1, 0, 1, 1, 4, 0, 1);
INSERT INTO `character_formula` VALUES (13, 10, 0, 1, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (14, 10, 0, 1, 0, 1, 0, 1, 0, 1, 1, 9, 0, 1);
INSERT INTO `character_formula` VALUES (15, 10, 0, 1, 0, 1, 0, 1, 0, 1, 1, 6, 0, 1);
INSERT INTO `character_formula` VALUES (16, 10, 2.5, 1, 1, 8, 1, 6, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (17, 10, 0, 1, 0, 1, 1, 3, 0, 1, 0, 1, 0, 1);
INSERT INTO `character_formula` VALUES (18, 10, 0, 1, 1, 3, 1, 5, 1, 5, 1, 7, 0, 1);

-- ----------------------------
-- Table structure for character_helper
-- ----------------------------
DROP TABLE IF EXISTS `character_helper`;
CREATE TABLE `character_helper`  (
  `race` tinyint(3) UNSIGNED NOT NULL,
  `stage` int(10) UNSIGNED NOT NULL,
  `duration` int(10) UNSIGNED NOT NULL,
  `cost` int(10) UNSIGNED NOT NULL,
  `cost_interval` int(10) UNSIGNED NOT NULL,
  `next_stage` int(11) NOT NULL DEFAULT -1,
  PRIMARY KEY (`race`, `stage`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_helper
-- ----------------------------
INSERT INTO `character_helper` VALUES (0, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (0, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (0, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (0, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (0, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (1, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (1, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (1, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (1, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (1, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (2, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (2, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (2, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (2, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (2, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (3, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (3, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (3, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (3, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (3, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (4, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (4, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (4, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (4, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (4, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (5, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (5, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (5, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (5, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (5, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (6, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (6, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (6, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (6, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (6, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (7, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (7, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (7, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (7, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (7, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (8, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (8, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (8, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (8, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (8, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (9, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (9, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (9, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (9, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (9, 5, 5760, 120, 5, -1);
INSERT INTO `character_helper` VALUES (10, 1, 200, 20, 5, 2);
INSERT INTO `character_helper` VALUES (10, 2, 200, 50, 5, 3);
INSERT INTO `character_helper` VALUES (10, 3, 200, 80, 5, 4);
INSERT INTO `character_helper` VALUES (10, 4, 200, 100, 5, 5);
INSERT INTO `character_helper` VALUES (10, 5, 5760, 120, 5, -1);

-- ----------------------------
-- Table structure for character_item
-- ----------------------------
DROP TABLE IF EXISTS `character_item`;
CREATE TABLE `character_item`  (
  `class` tinyint(3) UNSIGNED NOT NULL,
  `slot` tinyint(3) UNSIGNED NOT NULL,
  `type` tinyint(3) UNSIGNED NOT NULL,
  `index` smallint(5) UNSIGNED NOT NULL,
  `level` tinyint(3) UNSIGNED NOT NULL,
  `durability` tinyint(3) UNSIGNED NOT NULL,
  `skill` tinyint(3) UNSIGNED NOT NULL,
  `luck` tinyint(3) UNSIGNED NOT NULL,
  `option` tinyint(3) UNSIGNED NOT NULL,
  `exe` tinyint(3) UNSIGNED NOT NULL,
  `ancient` tinyint(3) UNSIGNED NOT NULL,
  `duration` bigint(20) NOT NULL DEFAULT 0
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_item
-- ----------------------------
INSERT INTO `character_item` VALUES (1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 1, 4, 15, 0, 255, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (0, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (0, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (0, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (0, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (1, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (1, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (1, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (1, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (1, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (2, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (2, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (2, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (2, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (2, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (3, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (3, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (3, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (3, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (3, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (4, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (4, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (4, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (4, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (4, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (5, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (5, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (5, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (5, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (5, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (6, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (6, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (6, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (6, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (6, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (0, 7, 12, 133, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (1, 7, 12, 134, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (2, 7, 12, 132, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (3, 7, 12, 134, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (4, 7, 12, 130, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (5, 7, 12, 131, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (6, 7, 12, 135, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (0, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (7, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (7, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (7, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (7, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (7, 7, 12, 278, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (7, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 3, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (7, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 5, 5, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 2, 7, 3, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 3, 8, 3, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 4, 9, 3, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 5, 10, 3, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 6, 11, 3, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (0, 255, 15, 8, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 2, 7, 1, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 3, 8, 1, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 4, 9, 1, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 5, 10, 1, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 6, 11, 1, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (1, 255, 12, 7, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 2, 7, 13, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 3, 8, 13, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 4, 9, 13, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 5, 10, 13, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (2, 6, 11, 13, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 3, 8, 15, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 4, 9, 15, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 5, 10, 15, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (3, 6, 11, 15, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 2, 7, 26, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 3, 8, 26, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 4, 9, 26, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 5, 10, 26, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (4, 6, 11, 26, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 2, 7, 40, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 3, 8, 40, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 4, 9, 40, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 5, 10, 40, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 6, 11, 40, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (5, 255, 15, 19, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 255, 0, 32, 5, 0, 1, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 2, 7, 59, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 3, 8, 59, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 4, 9, 59, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (6, 6, 11, 59, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (8, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (8, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (8, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (8, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (8, 7, 12, 133, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (8, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 2, 28, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 2, 7, 118, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 3, 8, 118, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 4, 9, 118, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 5, 10, 118, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 6, 11, 118, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (8, 255, 15, 8, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 0, 0, 66, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 1, 0, 66, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 2, 7, 174, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 3, 8, 174, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 4, 9, 174, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 5, 10, 174, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 6, 11, 174, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 7, 12, 134, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (9, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (9, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (9, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (9, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (9, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (9, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 12, 481, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (9, 255, 12, 476, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 0, 4, 42, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 1, 4, 42, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 2, 7, 208, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 3, 8, 208, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 4, 9, 208, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 5, 10, 208, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 6, 11, 208, 5, 0, 0, 1, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 7, 12, 133, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (10, 255, 13, 76, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (10, 255, 13, 80, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (10, 255, 13, 107, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (10, 255, 13, 122, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (10, 255, 13, 123, 0, 255, 0, 0, 0, 0, 0, 432000);
INSERT INTO `character_item` VALUES (10, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 3, 0, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 6, 0, 30, 0, 0, 0, 0, 0, 0);
INSERT INTO `character_item` VALUES (10, 255, 14, 286, 0, 0, 0, 0, 0, 0, 0, 0);

-- ----------------------------
-- Table structure for character_max_level_reward
-- ----------------------------
DROP TABLE IF EXISTS `character_max_level_reward`;
CREATE TABLE `character_max_level_reward`  (
  `class` tinyint(3) UNSIGNED NOT NULL,
  `max_level` smallint(5) UNSIGNED NOT NULL,
  `type` tinyint(3) UNSIGNED NOT NULL,
  `index` smallint(5) UNSIGNED NOT NULL,
  `level` tinyint(3) UNSIGNED NOT NULL,
  `durability` tinyint(3) UNSIGNED NOT NULL,
  `skill` tinyint(3) UNSIGNED NOT NULL,
  `luck` tinyint(3) UNSIGNED NOT NULL,
  `option` tinyint(3) UNSIGNED NOT NULL,
  `excellent` tinyint(3) UNSIGNED NOT NULL,
  `ancient` tinyint(3) UNSIGNED NOT NULL,
  `harmony` tinyint(3) UNSIGNED NOT NULL,
  `option_380` tinyint(3) UNSIGNED NOT NULL,
  `socket_1` smallint(5) UNSIGNED NOT NULL,
  `socket_2` smallint(5) UNSIGNED NOT NULL,
  `socket_3` smallint(5) UNSIGNED NOT NULL,
  `socket_4` smallint(5) UNSIGNED NOT NULL,
  `socket_5` smallint(5) UNSIGNED NOT NULL,
  `socket_bonus` tinyint(3) UNSIGNED NOT NULL,
  `flags` int(10) UNSIGNED NOT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Table structure for character_skill
-- ----------------------------
DROP TABLE IF EXISTS `character_skill`;
CREATE TABLE `character_skill`  (
  `class` tinyint(3) UNSIGNED NOT NULL,
  `skill` smallint(5) UNSIGNED NOT NULL,
  PRIMARY KEY (`class`, `skill`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_skill
-- ----------------------------
INSERT INTO `character_skill` VALUES (0, 17);
INSERT INTO `character_skill` VALUES (0, 45);
INSERT INTO `character_skill` VALUES (0, 67);
INSERT INTO `character_skill` VALUES (0, 68);
INSERT INTO `character_skill` VALUES (0, 69);
INSERT INTO `character_skill` VALUES (0, 70);
INSERT INTO `character_skill` VALUES (0, 71);
INSERT INTO `character_skill` VALUES (0, 72);
INSERT INTO `character_skill` VALUES (1, 44);
INSERT INTO `character_skill` VALUES (1, 67);
INSERT INTO `character_skill` VALUES (1, 68);
INSERT INTO `character_skill` VALUES (1, 69);
INSERT INTO `character_skill` VALUES (1, 70);
INSERT INTO `character_skill` VALUES (1, 71);
INSERT INTO `character_skill` VALUES (1, 72);
INSERT INTO `character_skill` VALUES (2, 46);
INSERT INTO `character_skill` VALUES (2, 67);
INSERT INTO `character_skill` VALUES (2, 68);
INSERT INTO `character_skill` VALUES (2, 69);
INSERT INTO `character_skill` VALUES (2, 70);
INSERT INTO `character_skill` VALUES (2, 71);
INSERT INTO `character_skill` VALUES (2, 72);
INSERT INTO `character_skill` VALUES (3, 57);
INSERT INTO `character_skill` VALUES (3, 67);
INSERT INTO `character_skill` VALUES (3, 68);
INSERT INTO `character_skill` VALUES (3, 69);
INSERT INTO `character_skill` VALUES (3, 70);
INSERT INTO `character_skill` VALUES (3, 71);
INSERT INTO `character_skill` VALUES (3, 72);
INSERT INTO `character_skill` VALUES (3, 73);
INSERT INTO `character_skill` VALUES (4, 60);
INSERT INTO `character_skill` VALUES (4, 67);
INSERT INTO `character_skill` VALUES (4, 68);
INSERT INTO `character_skill` VALUES (4, 69);
INSERT INTO `character_skill` VALUES (4, 70);
INSERT INTO `character_skill` VALUES (4, 71);
INSERT INTO `character_skill` VALUES (4, 72);
INSERT INTO `character_skill` VALUES (4, 74);
INSERT INTO `character_skill` VALUES (5, 45);
INSERT INTO `character_skill` VALUES (5, 67);
INSERT INTO `character_skill` VALUES (5, 68);
INSERT INTO `character_skill` VALUES (5, 69);
INSERT INTO `character_skill` VALUES (5, 70);
INSERT INTO `character_skill` VALUES (5, 71);
INSERT INTO `character_skill` VALUES (5, 72);
INSERT INTO `character_skill` VALUES (6, 67);
INSERT INTO `character_skill` VALUES (6, 68);
INSERT INTO `character_skill` VALUES (6, 69);
INSERT INTO `character_skill` VALUES (6, 70);
INSERT INTO `character_skill` VALUES (6, 71);
INSERT INTO `character_skill` VALUES (6, 72);
INSERT INTO `character_skill` VALUES (6, 269);
INSERT INTO `character_skill` VALUES (7, 67);
INSERT INTO `character_skill` VALUES (7, 68);
INSERT INTO `character_skill` VALUES (7, 69);
INSERT INTO `character_skill` VALUES (7, 70);
INSERT INTO `character_skill` VALUES (7, 71);
INSERT INTO `character_skill` VALUES (7, 72);
INSERT INTO `character_skill` VALUES (7, 275);
INSERT INTO `character_skill` VALUES (8, 17);
INSERT INTO `character_skill` VALUES (8, 45);
INSERT INTO `character_skill` VALUES (8, 67);
INSERT INTO `character_skill` VALUES (8, 68);
INSERT INTO `character_skill` VALUES (8, 69);
INSERT INTO `character_skill` VALUES (8, 70);
INSERT INTO `character_skill` VALUES (8, 71);
INSERT INTO `character_skill` VALUES (8, 72);
INSERT INTO `character_skill` VALUES (9, 67);
INSERT INTO `character_skill` VALUES (9, 68);
INSERT INTO `character_skill` VALUES (9, 69);
INSERT INTO `character_skill` VALUES (9, 70);
INSERT INTO `character_skill` VALUES (9, 71);
INSERT INTO `character_skill` VALUES (9, 72);

-- ----------------------------
-- Table structure for character_specialization
-- ----------------------------
DROP TABLE IF EXISTS `character_specialization`;
CREATE TABLE `character_specialization`  (
  `race` tinyint(3) UNSIGNED NOT NULL,
  `stat` tinyint(3) UNSIGNED NOT NULL,
  `required_stat` tinyint(3) UNSIGNED NOT NULL,
  `required_stat_min` int(11) NOT NULL DEFAULT 0,
  `required_stat_max` int(11) NOT NULL DEFAULT 0,
  `percent_min` tinyint(3) UNSIGNED NOT NULL,
  `percent_max` tinyint(3) UNSIGNED NOT NULL,
  `required_stat_limit` int(11) NOT NULL DEFAULT 0
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_specialization
-- ----------------------------
INSERT INTO `character_specialization` VALUES (0, 7, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (0, 7, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (0, 7, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (0, 8, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (0, 8, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (0, 8, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (0, 2, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (0, 2, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (0, 2, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (0, 3, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (0, 3, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (0, 3, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (1, 0, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (1, 0, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (1, 0, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (1, 1, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (1, 1, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (1, 1, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (1, 4, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (1, 4, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (1, 4, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (2, 0, 1, 1, 1800, 0, 10, 1800);
INSERT INTO `character_specialization` VALUES (2, 0, 1, 1801, 2100, 10, 30, 2100);
INSERT INTO `character_specialization` VALUES (2, 0, 1, 2101, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (2, 1, 1, 1, 1800, 0, 10, 1800);
INSERT INTO `character_specialization` VALUES (2, 1, 1, 1801, 2100, 10, 30, 2100);
INSERT INTO `character_specialization` VALUES (2, 1, 1, 2101, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (2, 5, 1, 1, 1800, 0, 10, 1800);
INSERT INTO `character_specialization` VALUES (2, 5, 1, 1801, 2100, 10, 30, 2100);
INSERT INTO `character_specialization` VALUES (2, 5, 1, 2101, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (2, 6, 1, 1, 1800, 0, 10, 1800);
INSERT INTO `character_specialization` VALUES (2, 6, 1, 1801, 2100, 10, 30, 2100);
INSERT INTO `character_specialization` VALUES (2, 6, 1, 2101, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (3, 0, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (3, 0, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (3, 0, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (3, 1, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (3, 1, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (3, 1, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (3, 7, 3, 1, 1800, 0, 10, 1800);
INSERT INTO `character_specialization` VALUES (3, 7, 3, 1801, 2500, 10, 30, 2500);
INSERT INTO `character_specialization` VALUES (3, 7, 3, 2501, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (3, 8, 3, 1, 1800, 0, 10, 1800);
INSERT INTO `character_specialization` VALUES (3, 8, 3, 1801, 2500, 10, 30, 2500);
INSERT INTO `character_specialization` VALUES (3, 8, 3, 2501, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (4, 0, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (4, 0, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (4, 0, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (4, 1, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (4, 1, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (4, 1, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (4, 4, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (4, 4, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (4, 4, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (5, 7, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (5, 7, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (5, 7, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (5, 8, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (5, 8, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (5, 8, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (5, 9, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (5, 9, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (5, 9, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (5, 10, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (5, 10, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (5, 10, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (6, 0, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (6, 0, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (6, 0, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (6, 1, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (6, 1, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (6, 1, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (7, 0, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (7, 0, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (7, 0, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (7, 1, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (7, 1, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (7, 1, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (7, 4, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (7, 4, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (7, 4, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (8, 7, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (8, 7, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (8, 7, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (8, 8, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (8, 8, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (8, 8, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (8, 2, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (8, 2, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (8, 2, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (8, 3, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (8, 3, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (8, 3, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (9, 0, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (9, 0, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (9, 0, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (9, 1, 0, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (9, 1, 0, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (9, 1, 0, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (9, 5, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (9, 5, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (9, 5, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (10, 7, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (10, 7, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (10, 7, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (10, 8, 3, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (10, 8, 3, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (10, 8, 3, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (10, 2, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (10, 2, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (10, 2, 1, 2001, -1, 30, 45, 3000);
INSERT INTO `character_specialization` VALUES (10, 3, 1, 1, 1500, 0, 10, 1500);
INSERT INTO `character_specialization` VALUES (10, 3, 1, 1501, 2000, 10, 30, 2000);
INSERT INTO `character_specialization` VALUES (10, 3, 1, 2001, -1, 30, 45, 3000);

-- ----------------------------
-- Table structure for character_stat_fruit
-- ----------------------------
DROP TABLE IF EXISTS `character_stat_fruit`;
CREATE TABLE `character_stat_fruit`  (
  `class` tinyint(3) UNSIGNED NOT NULL,
  `min_level` smallint(6) NOT NULL DEFAULT 0,
  `max_level` smallint(6) NOT NULL DEFAULT 0,
  `total_add_points` smallint(5) UNSIGNED NOT NULL,
  `total_minus_points` smallint(5) UNSIGNED NOT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of character_stat_fruit
-- ----------------------------
INSERT INTO `character_stat_fruit` VALUES (0, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (0, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (0, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (0, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (0, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (0, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (0, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (0, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (0, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (0, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (0, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (0, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (0, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (0, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (0, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (0, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (0, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (0, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (0, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (0, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (0, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (0, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (0, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (0, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (0, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (0, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (0, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (0, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (0, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (0, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (0, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (0, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (0, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (0, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (0, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (0, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (0, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (0, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (0, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (0, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (1, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (1, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (1, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (1, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (1, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (1, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (1, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (1, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (1, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (1, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (1, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (1, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (1, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (1, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (1, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (1, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (1, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (1, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (1, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (1, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (1, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (1, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (1, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (1, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (1, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (1, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (1, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (1, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (1, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (1, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (1, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (1, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (1, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (1, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (1, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (1, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (1, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (1, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (1, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (1, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (2, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (2, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (2, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (2, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (2, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (2, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (2, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (2, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (2, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (2, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (2, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (2, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (2, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (2, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (2, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (2, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (2, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (2, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (2, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (2, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (2, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (2, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (2, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (2, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (2, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (2, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (2, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (2, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (2, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (2, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (2, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (2, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (2, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (2, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (2, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (2, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (2, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (2, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (2, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (2, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (3, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (3, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (3, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (3, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (3, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (3, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (3, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (3, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (3, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (3, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (3, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (3, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (3, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (3, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (3, 141, 150, 32, 32);
INSERT INTO `character_stat_fruit` VALUES (3, 151, 160, 34, 34);
INSERT INTO `character_stat_fruit` VALUES (3, 161, 170, 36, 36);
INSERT INTO `character_stat_fruit` VALUES (3, 171, 180, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (3, 181, 190, 40, 40);
INSERT INTO `character_stat_fruit` VALUES (3, 191, 200, 42, 42);
INSERT INTO `character_stat_fruit` VALUES (3, 201, 210, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (3, 211, 220, 46, 46);
INSERT INTO `character_stat_fruit` VALUES (3, 221, 230, 49, 49);
INSERT INTO `character_stat_fruit` VALUES (3, 231, 240, 52, 52);
INSERT INTO `character_stat_fruit` VALUES (3, 241, 250, 55, 55);
INSERT INTO `character_stat_fruit` VALUES (3, 251, 260, 58, 58);
INSERT INTO `character_stat_fruit` VALUES (3, 261, 270, 61, 61);
INSERT INTO `character_stat_fruit` VALUES (3, 271, 280, 64, 64);
INSERT INTO `character_stat_fruit` VALUES (3, 281, 290, 67, 67);
INSERT INTO `character_stat_fruit` VALUES (3, 291, 300, 70, 70);
INSERT INTO `character_stat_fruit` VALUES (3, 301, 310, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (3, 311, 320, 76, 76);
INSERT INTO `character_stat_fruit` VALUES (3, 321, 330, 79, 79);
INSERT INTO `character_stat_fruit` VALUES (3, 331, 340, 82, 82);
INSERT INTO `character_stat_fruit` VALUES (3, 341, 350, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (3, 351, 360, 88, 88);
INSERT INTO `character_stat_fruit` VALUES (3, 361, 370, 91, 91);
INSERT INTO `character_stat_fruit` VALUES (3, 371, 380, 94, 94);
INSERT INTO `character_stat_fruit` VALUES (3, 381, 390, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (3, 391, 400, 100, 100);
INSERT INTO `character_stat_fruit` VALUES (4, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (4, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (4, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (4, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (4, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (4, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (4, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (4, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (4, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (4, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (4, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (4, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (4, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (4, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (4, 141, 150, 32, 32);
INSERT INTO `character_stat_fruit` VALUES (4, 151, 160, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (4, 161, 170, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (4, 171, 180, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (4, 181, 190, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (4, 191, 200, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (4, 201, 210, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (4, 211, 220, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (4, 221, 230, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (4, 231, 240, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (4, 241, 250, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (4, 251, 260, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (4, 261, 270, 68, 68);
INSERT INTO `character_stat_fruit` VALUES (4, 271, 280, 71, 71);
INSERT INTO `character_stat_fruit` VALUES (4, 281, 290, 74, 74);
INSERT INTO `character_stat_fruit` VALUES (4, 291, 300, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (4, 301, 310, 80, 80);
INSERT INTO `character_stat_fruit` VALUES (4, 311, 320, 83, 83);
INSERT INTO `character_stat_fruit` VALUES (4, 321, 330, 87, 87);
INSERT INTO `character_stat_fruit` VALUES (4, 331, 340, 91, 91);
INSERT INTO `character_stat_fruit` VALUES (4, 341, 350, 95, 95);
INSERT INTO `character_stat_fruit` VALUES (4, 351, 360, 99, 99);
INSERT INTO `character_stat_fruit` VALUES (4, 361, 370, 103, 103);
INSERT INTO `character_stat_fruit` VALUES (4, 371, 380, 107, 107);
INSERT INTO `character_stat_fruit` VALUES (4, 381, 390, 111, 111);
INSERT INTO `character_stat_fruit` VALUES (4, 391, 400, 115, 115);
INSERT INTO `character_stat_fruit` VALUES (5, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (5, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (5, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (5, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (5, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (5, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (5, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (5, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (5, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (5, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (5, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (5, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (5, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (5, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (5, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (5, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (5, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (5, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (5, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (5, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (5, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (5, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (5, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (5, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (5, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (5, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (5, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (5, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (5, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (5, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (5, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (5, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (5, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (5, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (5, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (5, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (5, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (5, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (5, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (5, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (6, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (6, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (6, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (6, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (6, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (6, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (6, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (6, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (6, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (6, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (6, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (6, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (6, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (6, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (6, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (6, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (6, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (6, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (6, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (6, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (6, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (6, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (6, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (6, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (6, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (6, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (6, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (6, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (6, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (6, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (6, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (6, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (6, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (6, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (6, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (6, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (6, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (6, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (6, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (6, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (7, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (7, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (7, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (7, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (7, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (7, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (7, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (7, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (7, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (7, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (7, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (7, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (7, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (7, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (7, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (7, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (7, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (7, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (7, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (7, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (7, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (7, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (7, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (7, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (7, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (7, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (7, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (7, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (7, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (7, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (7, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (7, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (7, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (7, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (7, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (7, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (7, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (7, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (7, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (7, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (8, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (8, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (8, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (8, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (8, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (8, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (8, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (8, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (8, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (8, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (8, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (8, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (8, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (8, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (8, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (8, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (8, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (8, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (8, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (8, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (8, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (8, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (8, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (8, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (8, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (8, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (8, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (8, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (8, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (8, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (8, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (8, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (8, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (8, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (8, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (8, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (8, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (8, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (8, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (8, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (9, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (9, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (9, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (9, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (9, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (9, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (9, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (9, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (9, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (9, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (9, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (9, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (9, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (9, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (9, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (9, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (9, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (9, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (9, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (9, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (9, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (9, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (9, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (9, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (9, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (9, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (9, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (9, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (9, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (9, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (9, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (9, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (9, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (9, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (9, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (9, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (9, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (9, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (9, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (9, 391, 400, 127, 127);
INSERT INTO `character_stat_fruit` VALUES (10, 1, 10, 4, 4);
INSERT INTO `character_stat_fruit` VALUES (10, 11, 20, 6, 6);
INSERT INTO `character_stat_fruit` VALUES (10, 21, 30, 8, 8);
INSERT INTO `character_stat_fruit` VALUES (10, 31, 40, 10, 10);
INSERT INTO `character_stat_fruit` VALUES (10, 41, 50, 12, 12);
INSERT INTO `character_stat_fruit` VALUES (10, 51, 60, 14, 14);
INSERT INTO `character_stat_fruit` VALUES (10, 61, 70, 16, 16);
INSERT INTO `character_stat_fruit` VALUES (10, 71, 80, 18, 18);
INSERT INTO `character_stat_fruit` VALUES (10, 81, 90, 20, 20);
INSERT INTO `character_stat_fruit` VALUES (10, 91, 100, 22, 22);
INSERT INTO `character_stat_fruit` VALUES (10, 101, 110, 24, 24);
INSERT INTO `character_stat_fruit` VALUES (10, 111, 120, 26, 26);
INSERT INTO `character_stat_fruit` VALUES (10, 121, 130, 28, 28);
INSERT INTO `character_stat_fruit` VALUES (10, 131, 140, 30, 30);
INSERT INTO `character_stat_fruit` VALUES (10, 141, 150, 35, 35);
INSERT INTO `character_stat_fruit` VALUES (10, 151, 160, 38, 38);
INSERT INTO `character_stat_fruit` VALUES (10, 161, 170, 41, 41);
INSERT INTO `character_stat_fruit` VALUES (10, 171, 180, 44, 44);
INSERT INTO `character_stat_fruit` VALUES (10, 181, 190, 47, 47);
INSERT INTO `character_stat_fruit` VALUES (10, 191, 200, 50, 50);
INSERT INTO `character_stat_fruit` VALUES (10, 201, 210, 53, 53);
INSERT INTO `character_stat_fruit` VALUES (10, 211, 220, 56, 56);
INSERT INTO `character_stat_fruit` VALUES (10, 221, 230, 59, 59);
INSERT INTO `character_stat_fruit` VALUES (10, 231, 240, 62, 62);
INSERT INTO `character_stat_fruit` VALUES (10, 241, 250, 65, 65);
INSERT INTO `character_stat_fruit` VALUES (10, 251, 260, 69, 69);
INSERT INTO `character_stat_fruit` VALUES (10, 261, 270, 73, 73);
INSERT INTO `character_stat_fruit` VALUES (10, 271, 280, 77, 77);
INSERT INTO `character_stat_fruit` VALUES (10, 281, 290, 81, 81);
INSERT INTO `character_stat_fruit` VALUES (10, 291, 300, 85, 85);
INSERT INTO `character_stat_fruit` VALUES (10, 301, 310, 89, 89);
INSERT INTO `character_stat_fruit` VALUES (10, 311, 320, 93, 93);
INSERT INTO `character_stat_fruit` VALUES (10, 321, 330, 97, 97);
INSERT INTO `character_stat_fruit` VALUES (10, 331, 340, 101, 101);
INSERT INTO `character_stat_fruit` VALUES (10, 341, 350, 105, 105);
INSERT INTO `character_stat_fruit` VALUES (10, 351, 360, 109, 109);
INSERT INTO `character_stat_fruit` VALUES (10, 361, 370, 113, 113);
INSERT INTO `character_stat_fruit` VALUES (10, 371, 380, 117, 117);
INSERT INTO `character_stat_fruit` VALUES (10, 381, 390, 122, 122);
INSERT INTO `character_stat_fruit` VALUES (10, 391, 400, 127, 127);

SET FOREIGN_KEY_CHECKS = 1;
