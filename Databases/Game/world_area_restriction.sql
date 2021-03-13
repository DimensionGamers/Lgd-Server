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

 Date: 13/03/2021 19:16:54
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for world_area_restriction
-- ----------------------------
DROP TABLE IF EXISTS `world_area_restriction`;
CREATE TABLE `world_area_restriction`  (
  `world` smallint(5) UNSIGNED NOT NULL,
  `x` smallint(6) NOT NULL DEFAULT 0,
  `y` smallint(6) NOT NULL DEFAULT 0,
  `range` tinyint(3) UNSIGNED NOT NULL,
  `flags` tinyint(3) UNSIGNED NOT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of world_area_restriction
-- ----------------------------
INSERT INTO `world_area_restriction` VALUES (2, 209, 30, 1, 1);
INSERT INTO `world_area_restriction` VALUES (2, 219, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (3, 171, 104, 1, 1);
INSERT INTO `world_area_restriction` VALUES (11, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (12, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (13, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (14, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (15, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (16, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (17, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (52, 10, 9, 1, 1);
INSERT INTO `world_area_restriction` VALUES (79, 202, 54, 41, 8);

SET FOREIGN_KEY_CHECKS = 1;
