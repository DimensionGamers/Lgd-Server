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

 Date: 13/03/2021 19:17:16
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for shop_template
-- ----------------------------
DROP TABLE IF EXISTS `shop_template`;
CREATE TABLE `shop_template`  (
  `guid` tinyint(3) UNSIGNED NOT NULL,
  `name` text CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `max_pk_level` tinyint(3) UNSIGNED NOT NULL,
  `pk_text` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `flags` smallint(5) UNSIGNED NOT NULL,
  `type` tinyint(3) UNSIGNED NOT NULL,
  `max_buy_count` int(11) NOT NULL DEFAULT 0,
  `max_buy_type` tinyint(3) UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of shop_template
-- ----------------------------
INSERT INTO `shop_template` VALUES (3, 'lorencia_peddler', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (1, 'lorencia_bar', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (2, 'lorencia_mage', 4, 'I don\'t sell anything to people like you.', 5, 0, 0, 0);
INSERT INTO `shop_template` VALUES (0, 'lorencia_weapon', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (4, 'lorencia_merchant_near_river', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (5, 'lorencia_potion_girl', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (6, 'devias_bar', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (7, 'devias_wizard', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (8, 'devias_weapon', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (9, 'noria_weapon', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (10, 'noria_potion_fairy', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (11, 'alex', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (12, 'thomson_channel', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (13, 'shop_firework_girl', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (14, 'kalima_potion', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (15, 'siege_supplier', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (16, 'stadium_guard', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (17, 'elbeland_silvia', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (18, 'elbeland_leah', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (19, 'elbeland_marseille', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (20, 'loren_market_christine', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (21, 'kalrutan_merchant_1', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (22, 'kalrutan_merchant_2', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (23, 'shop_personal_merchant', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (24, 'shop_moss_merchant', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (25, 'shop_champion_chest', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (50, 'ruud_shop', 10, '', 0, 1, 0, 0);
INSERT INTO `shop_template` VALUES (26, 'shop_acheron_jin', 4, 'I don\'t sell anything to people like you.', 1, 0, 0, 0);
INSERT INTO `shop_template` VALUES (27, 'shop_battle_core_hp', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (28, 'shop_battle_core_sd', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (200, 'shop_anniversary_secret_1', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (201, 'shop_anniversary_secret_2', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (202, 'shop_anniversary_secret_3', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (203, 'shop_anniversary_secret_4', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (204, 'shop_anniversary_secret_5', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (101, 'shop_test_01', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (102, 'shop_test_02', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (103, 'shop_test_03', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (104, 'shop_test_04', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (105, 'shop_test_05', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (106, 'shop_test_06', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (107, 'shop_test_07', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (108, 'shop_test_08', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (109, 'shop_test_09', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (110, 'shop_test_10', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (111, 'shop_test_11', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (112, 'shop_test_12', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (113, 'shop_test_socket_dw', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (114, 'shop_test_socket_dk', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (115, 'shop_test_socket_elf', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (116, 'shop_test_socket_mg', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (117, 'shop_test_socket_dl', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (118, 'shop_test_socket_sum', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (119, 'shop_test_socket_rf', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (120, 'shop_test_socket_gl', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (121, 'shop_test_socket_rw', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (122, 'shop_test_socket_sl', 10, '', 0, 0, 0, 0);
INSERT INTO `shop_template` VALUES (123, 'shop_test_archangel', 10, '', 0, 0, 0, 0);

SET FOREIGN_KEY_CHECKS = 1;
