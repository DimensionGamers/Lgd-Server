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

 Date: 13/03/2021 19:16:49
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for world_area_attribute
-- ----------------------------
DROP TABLE IF EXISTS `world_area_attribute`;
CREATE TABLE `world_area_attribute`  (
  `world_id` smallint(5) UNSIGNED NOT NULL,
  `x1` smallint(6) NOT NULL DEFAULT -1,
  `y1` smallint(6) NOT NULL DEFAULT -1,
  `x2` smallint(6) NOT NULL DEFAULT -1,
  `y2` smallint(6) NOT NULL DEFAULT -1,
  `attribute` tinyint(3) UNSIGNED NOT NULL,
  `apply` tinyint(3) UNSIGNED NOT NULL,
  `send` tinyint(3) UNSIGNED NOT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of world_area_attribute
-- ----------------------------
INSERT INTO `world_area_attribute` VALUES (202, 132, 149, 135, 166, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (202, 154, 126, 170, 129, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (202, 132, 92, 135, 107, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (202, 96, 126, 112, 129, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (202, 113, 107, 153, 149, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (202, 120, 167, 153, 180, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (202, 171, 110, 187, 146, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (202, 80, 111, 95, 142, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (202, 110, 73, 147, 91, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (203, 1, 1, 255, 255, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (203, 164, 26, 178, 57, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (203, 209, 78, 240, 90, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (204, 19, 187, 77, 245, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (204, 78, 213, 99, 219, 12, 1, 1);
INSERT INTO `world_area_attribute` VALUES (204, 43, 166, 49, 186, 12, 1, 1);
INSERT INTO `world_area_attribute` VALUES (206, 1, 1, 255, 255, 1, 0, 1);
INSERT INTO `world_area_attribute` VALUES (30, 173, 212, 173, 212, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 174, 212, 174, 212, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 175, 212, 175, 212, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 176, 212, 176, 212, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 177, 212, 177, 212, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 178, 212, 178, 212, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 179, 212, 179, 212, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 174, 211, 174, 211, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 175, 211, 175, 211, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 176, 211, 176, 211, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 177, 211, 177, 211, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 178, 211, 178, 211, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 174, 213, 174, 213, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 175, 213, 175, 213, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 176, 213, 176, 213, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 177, 213, 177, 213, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 178, 213, 178, 213, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 167, 194, 167, 194, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 185, 194, 185, 194, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 176, 217, 176, 217, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 94, 227, 94, 227, 16, 1, 0);
INSERT INTO `world_area_attribute` VALUES (30, 94, 182, 94, 182, 16, 1, 0);
INSERT INTO `world_area_attribute` VALUES (30, 82, 130, 82, 130, 16, 1, 0);
INSERT INTO `world_area_attribute` VALUES (30, 107, 130, 107, 130, 16, 1, 0);
INSERT INTO `world_area_attribute` VALUES (30, 178, 215, 178, 215, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 175, 216, 175, 216, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (41, 127, 161, 128, 163, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (41, 123, 163, 125, 163, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (41, 125, 158, 126, 159, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (41, 129, 159, 129, 159, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (41, 127, 166, 127, 166, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (41, 122, 155, 142, 170, 4, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 175, 217, 175, 217, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 177, 217, 177, 217, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 175, 218, 175, 218, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 176, 218, 176, 218, 20, 1, 1);
INSERT INTO `world_area_attribute` VALUES (30, 177, 218, 177, 218, 20, 1, 1);

SET FOREIGN_KEY_CHECKS = 1;
