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

 Date: 13/03/2021 19:16:37
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for world_template
-- ----------------------------
DROP TABLE IF EXISTS `world_template`;
CREATE TABLE `world_template`  (
  `entry` smallint(5) UNSIGNED NOT NULL,
  `display_entry` smallint(5) UNSIGNED NOT NULL,
  `state` tinyint(3) UNSIGNED NOT NULL,
  `file` text CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `name` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `spawn_gate` smallint(5) UNSIGNED NOT NULL,
  `experience_rate` int(10) UNSIGNED NOT NULL,
  `zen_rate` int(10) UNSIGNED NOT NULL,
  `flags` int(10) UNSIGNED NOT NULL,
  `comment` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  PRIMARY KEY (`entry`, `state`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of world_template
-- ----------------------------
INSERT INTO `world_template` VALUES (0, 0, 0, 'Terrain1.att', 'Lorencia', 17, 100, 100, 89518206, NULL);
INSERT INTO `world_template` VALUES (1, 1, 0, 'Terrain2.att', 'Dungeon', 17, 100, 100, 89518202, NULL);
INSERT INTO `world_template` VALUES (2, 2, 0, 'Terrain3.att', 'Devias', 22, 100, 100, 89518206, NULL);
INSERT INTO `world_template` VALUES (3, 3, 0, 'Terrain4.att', 'Noria', 27, 100, 100, 89518206, NULL);
INSERT INTO `world_template` VALUES (4, 4, 0, 'Terrain5.att', 'Losttower', 42, 100, 100, 89518202, NULL);
INSERT INTO `world_template` VALUES (6, 6, 0, 'Terrain7.att', 'Stadium', 50, 100, 100, 5337114, NULL);
INSERT INTO `world_template` VALUES (7, 7, 0, 'Terrain8.att', 'Atlans', 49, 100, 100, 89518202, NULL);
INSERT INTO `world_template` VALUES (8, 8, 0, 'Terrain9.att', 'Tarkan', 57, 100, 100, 89518202, NULL);
INSERT INTO `world_template` VALUES (9, 9, 0, 'Terrain10.att', 'Devil Square', 505, 100, 0, 221586, NULL);
INSERT INTO `world_template` VALUES (10, 10, 0, 'Terrain11.att', 'Icarus', 22, 100, 100, 18108538, NULL);
INSERT INTO `world_template` VALUES (11, 11, 0, 'Terrain12.att', 'Blood Castle 1', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (12, 12, 0, 'Terrain12.att', 'Blood Castle 2', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (13, 13, 0, 'Terrain12.att', 'Blood Castle 3', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (14, 14, 0, 'Terrain12.att', 'Blood Castle 4', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (15, 15, 0, 'Terrain12.att', 'Blood Castle 5', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (16, 16, 0, 'Terrain12.att', 'Blood Castle 6', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (17, 17, 0, 'Terrain12.att', 'Blood Castle 7', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (18, 18, 0, 'Terrain19.att', 'Chaos Castle 1', 503, 100, 100, 131456, NULL);
INSERT INTO `world_template` VALUES (19, 19, 0, 'Terrain19.att', 'Chaos Castle 2', 503, 100, 100, 131456, NULL);
INSERT INTO `world_template` VALUES (20, 20, 0, 'Terrain19.att', 'Chaos Castle 3', 503, 100, 100, 131456, NULL);
INSERT INTO `world_template` VALUES (21, 21, 0, 'Terrain19.att', 'Chaos Castle 4', 503, 100, 100, 131456, NULL);
INSERT INTO `world_template` VALUES (22, 22, 0, 'Terrain19.att', 'Chaos Castle 5', 503, 100, 100, 131456, NULL);
INSERT INTO `world_template` VALUES (23, 23, 0, 'Terrain19.att', 'Chaos Castle 6', 503, 100, 100, 131456, NULL);
INSERT INTO `world_template` VALUES (24, 24, 0, 'Terrain25.att', 'Kalima 1', 17, 100, 100, 18182266, NULL);
INSERT INTO `world_template` VALUES (25, 25, 0, 'Terrain25.att', 'Kalima 2', 17, 100, 100, 18182266, NULL);
INSERT INTO `world_template` VALUES (26, 26, 0, 'Terrain25.att', 'Kalima 3', 17, 100, 100, 18182266, NULL);
INSERT INTO `world_template` VALUES (27, 27, 0, 'Terrain25.att', 'Kalima 4', 17, 100, 100, 18182266, NULL);
INSERT INTO `world_template` VALUES (28, 28, 0, 'Terrain25.att', 'Kalima 5', 17, 100, 100, 18182266, NULL);
INSERT INTO `world_template` VALUES (29, 29, 0, 'Terrain25.att', 'Kalima 6', 17, 100, 100, 18182266, NULL);
INSERT INTO `world_template` VALUES (30, 30, 0, 'Terrain31.att', 'Valley Of Loren', 100, 100, 100, 1142866, NULL);
INSERT INTO `world_template` VALUES (31, 31, 0, 'Terrain32.att', 'Land Of Trials', 106, 100, 100, 1405042, NULL);
INSERT INTO `world_template` VALUES (32, 32, 0, 'Terrain33.att', 'Devil Square', 505, 100, 0, 221586, NULL);
INSERT INTO `world_template` VALUES (33, 33, 0, 'Terrain34.att', 'Aida', 119, 100, 100, 89518202, NULL);
INSERT INTO `world_template` VALUES (34, 34, 0, 'Terrain35_Peace.att', 'Crywolf', 118, 100, 100, 85323890, 'Crywolf Peace Status');
INSERT INTO `world_template` VALUES (35, 35, 0, 'Terrain36.att', 'CrywolfSecondZone', 118, 100, 100, 0, NULL);
INSERT INTO `world_template` VALUES (36, 36, 0, 'Terrain37.att', 'Kalima 7', 17, 100, 100, 152399994, NULL);
INSERT INTO `world_template` VALUES (37, 37, 0, 'Terrain38.att', 'Kanturu', 138, 100, 100, 89518202, NULL);
INSERT INTO `world_template` VALUES (38, 38, 0, 'Terrain39.att', 'Kanturu Core', 139, 100, 100, 85323898, NULL);
INSERT INTO `world_template` VALUES (39, 39, 0, 'Terrain40_Close.att', 'Kanturu Boss', 137, 100, 100, 90258, 'Kanturu Boss Close Status');
INSERT INTO `world_template` VALUES (40, 40, 0, 'Terrain41.att', 'Event Zone', 1000, 100, 100, 8250, NULL);
INSERT INTO `world_template` VALUES (41, 41, 0, 'Terrain42.att', 'Barracks', 256, 100, 100, 18215026, NULL);
INSERT INTO `world_template` VALUES (42, 42, 0, 'Terrain43.att', 'Refuge', 256, 100, 100, 18182258, NULL);
INSERT INTO `world_template` VALUES (45, 45, 0, 'Terrain47.att', 'Illusion Temple 1', 267, 100, 100, 384, NULL);
INSERT INTO `world_template` VALUES (46, 46, 0, 'Terrain47.att', 'Illusion Temple 2', 267, 100, 100, 384, NULL);
INSERT INTO `world_template` VALUES (47, 47, 0, 'Terrain47.att', 'Illusion Temple 3', 267, 100, 100, 384, NULL);
INSERT INTO `world_template` VALUES (48, 48, 0, 'Terrain47.att', 'Illusion Temple 4', 267, 100, 100, 384, NULL);
INSERT INTO `world_template` VALUES (49, 49, 0, 'Terrain47.att', 'Illusion Temple 5', 267, 100, 100, 384, NULL);
INSERT INTO `world_template` VALUES (50, 50, 0, 'Terrain47.att', 'Illusion Temple 6', 267, 100, 100, 384, NULL);
INSERT INTO `world_template` VALUES (51, 51, 0, 'Terrain52.att', 'Elbeland', 267, 100, 100, 89518206, NULL);
INSERT INTO `world_template` VALUES (52, 52, 0, 'Terrain12.att', 'Blood Castle 8', 504, 50, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (53, 53, 0, 'Terrain19.att', 'Chaos Castle 7', 503, 100, 100, 131456, NULL);
INSERT INTO `world_template` VALUES (56, 56, 0, 'Terrain57.att', 'Swamp Of Calmness', 273, 100, 100, 219541626, NULL);
INSERT INTO `world_template` VALUES (57, 57, 0, 'Terrain58.att', 'Raklion', 286, 100, 100, 85323898, NULL);
INSERT INTO `world_template` VALUES (58, 58, 0, 'Terrain59.att', 'Raklion Hatchery', 286, 100, 100, 135360656, NULL);
INSERT INTO `world_template` VALUES (62, 62, 0, 'Terrain63.att', 'XMas', 22, 100, 100, 32794, NULL);
INSERT INTO `world_template` VALUES (63, 63, 0, 'Terrain64.att', 'Vulcanus', 294, 100, 100, 89518203, NULL);
INSERT INTO `world_template` VALUES (64, 64, 0, 'Terrain65.att', 'Vulcanus Duel', 294, 100, 100, 65664, NULL);
INSERT INTO `world_template` VALUES (69, 69, 0, 'Terrain70.att', 'Imperial Fortress 1', 508, 100, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (70, 70, 0, 'Terrain71.att', 'Imperial Fortress 2', 508, 100, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (71, 71, 0, 'Terrain72.att', 'Imperial Fortress 3', 508, 100, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (72, 72, 0, 'Terrain73.att', 'Imperial Fortress 4', 508, 100, 100, 221586, NULL);
INSERT INTO `world_template` VALUES (79, 79, 0, 'Terrain80.att', 'Loren Market', 333, 100, 100, 303126, NULL);
INSERT INTO `world_template` VALUES (80, 80, 0, 'Terrain81.att', 'Kalrutan 1', 335, 100, 100, 89518202, NULL);
INSERT INTO `world_template` VALUES (81, 81, 0, 'Terrain82.att', 'Kalrutan 2', 335, 100, 100, 89485434, NULL);
INSERT INTO `world_template` VALUES (82, 82, 0, 'Terrain83.att', 'DoubleGoer 1', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (83, 83, 0, 'Terrain83.att', 'DoubleGoer 2', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (84, 84, 0, 'Terrain83.att', 'DoubleGoer 3', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (85, 85, 0, 'Terrain83.att', 'DoubleGoer 4', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (86, 86, 0, 'Terrain83.att', 'DoubleGoer 5', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (87, 87, 0, 'Terrain83.att', 'DoubleGoer 6', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (88, 88, 0, 'Terrain83.att', 'DoubleGoer 7', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (89, 89, 0, 'Terrain83.att', 'DoubleGoer 8', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (90, 90, 0, 'Terrain83.att', 'DoubleGoer 9', 507, 100, 100, 213392, NULL);
INSERT INTO `world_template` VALUES (34, 34, 1, 'Terrain35_Occupied.att', 'Crywolf', 118, 100, 100, 85323890, 'Crywolf Occupied Status');
INSERT INTO `world_template` VALUES (34, 34, 2, 'Terrain35_War.att', 'Crywolf', 118, 100, 100, 85323890, 'Crywolf War Status');
INSERT INTO `world_template` VALUES (39, 39, 1, 'Terrain40_Open.att', 'Kanturu Boss', 137, 100, 100, 90258, 'Kanturu Boss Open Status');
INSERT INTO `world_template` VALUES (91, 91, 0, 'Terrain92.att', 'Acheron 1', 417, 100, 100, 85323898, '');
INSERT INTO `world_template` VALUES (92, 92, 0, 'Terrain92.att', 'Acheron 2', 426, 100, 100, 1429618, '');
INSERT INTO `world_template` VALUES (95, 95, 0, 'Terrain96.att', 'DevEnter 1', 417, 100, 100, 85291130, '');
INSERT INTO `world_template` VALUES (96, 96, 0, 'Terrain96.att', 'DevEnter 2', 426, 100, 100, 1134706, '');
INSERT INTO `world_template` VALUES (97, 97, 0, 'Terrain19.att', 'Chaos Castle Final', 505, 100, 100, 131200, '');
INSERT INTO `world_template` VALUES (98, 98, 0, 'Terrain99.att', 'Illusion Temple Final 1', 333, 100, 100, 128, '');
INSERT INTO `world_template` VALUES (99, 99, 0, 'Terrain99.att', 'Illusion Temple Final 2', 333, 100, 100, 128, '');
INSERT INTO `world_template` VALUES (104, 104, 0, 'Terrain103.att', 'Tormented Square', 503, 100, 100, 221586, '');
INSERT INTO `world_template` VALUES (103, 103, 0, 'Terrain103.att', 'Tormented Square', 503, 100, 100, 221586, '');
INSERT INTO `world_template` VALUES (102, 102, 0, 'Terrain103.att', 'Tormented Square', 503, 100, 100, 221586, '');
INSERT INTO `world_template` VALUES (101, 101, 0, 'Terrain101.att', 'Uruk Mountain', 426, 100, 100, 1134706, '');
INSERT INTO `world_template` VALUES (100, 100, 0, 'Terrain101.att', 'Uruk Mountain', 417, 100, 100, 219508858, '');
INSERT INTO `world_template` VALUES (105, 105, 0, 'Terrain103.att', 'Tormented Square', 503, 100, 100, 221586, '');
INSERT INTO `world_template` VALUES (106, 106, 0, 'Terrain103.att', 'Tormented Square', 503, 100, 100, 221586, '');
INSERT INTO `world_template` VALUES (110, 110, 0, 'Terrain111.att', 'Nars', 417, 100, 100, 219508858, '');
INSERT INTO `world_template` VALUES (112, 112, 0, 'Terrain113.att', 'Ferea', 509, 100, 100, 219541626, '');
INSERT INTO `world_template` VALUES (200, 32, 0, 'Terrain33.att', 'Last Man Standing', 503, 100, 0, 131200, '');
INSERT INTO `world_template` VALUES (113, 113, 0, 'Terrain114.att', 'Nixies Lake', 522, 100, 100, 219275386, '');
INSERT INTO `world_template` VALUES (114, 114, 0, 'Terrain115.att', 'The labyrinth of Dimensions', 503, 100, 100, 221202, '');
INSERT INTO `world_template` VALUES (115, 115, 0, 'Terrain116.att', 'The labyrinth of Dimensions', 503, 50, 100, 221312, '');
INSERT INTO `world_template` VALUES (116, 116, 0, 'Terrain117.att', 'Deep Dungeon 1', 561, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (117, 117, 0, 'Terrain118.att', 'Deep Dungeon 2', 562, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (118, 118, 0, 'Terrain119.att', 'Deep Dungeon 3', 563, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (119, 119, 0, 'Terrain120.att', 'Deep Dungeon 4', 564, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (120, 120, 0, 'Terrain121.att', 'Deep Dungeon 5', 565, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (121, 121, 0, 'Terrain122.att', 'Place of Qualification', 537, 100, 100, 90128, '');
INSERT INTO `world_template` VALUES (201, 64, 0, 'Terrain65.att', 'Arena Ground 1', 503, 0, 0, 131200, '');
INSERT INTO `world_template` VALUES (202, 0, 0, 'Terrain1.att', 'Arena Ground 2', 503, 0, 0, 131200, '');
INSERT INTO `world_template` VALUES (203, 2, 0, 'Terrain3.att', 'Arena Ground 3', 503, 0, 0, 131200, '');
INSERT INTO `world_template` VALUES (204, 51, 0, 'Terrain52.att', 'Arena Ground 5', 503, 0, 0, 131200, '');
INSERT INTO `world_template` VALUES (205, 42, 0, 'Terrain43.att', 'Arena Ground 6', 503, 0, 0, 131200, '');
INSERT INTO `world_template` VALUES (206, 62, 0, 'Terrain63.att', 'Arena Ground 7', 503, 0, 0, 131200, '');
INSERT INTO `world_template` VALUES (122, 122, 0, 'Terrain123.att', 'Swamp of Darkness', 567, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (300, 1, 0, 'Terrain2.att', 'Dungeon Instance', 503, 100, 100, 73746, '');
INSERT INTO `world_template` VALUES (123, 123, 0, 'Terrain124.att', 'Kubera Mine', 591, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (124, 124, 0, 'Terrain124.att', 'Kubera Mine', 591, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (125, 125, 0, 'Terrain124.att', 'Kubera Mine', 591, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (126, 126, 0, 'Terrain124.att', 'Kubera Mine', 591, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (127, 127, 0, 'Terrain124.att', 'Kubera Mine', 591, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (128, 128, 0, 'Terrain129.att', 'Abyss Atlans 1', 617, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (129, 129, 0, 'Terrain130.att', 'Abyss Atlans 2', 618, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (130, 130, 0, 'Terrain131.att', 'Abyss Atlans 3', 619, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (131, 131, 0, 'Terrain132.att', 'Scorched Canyon', 620, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (207, 103, 0, 'Terrain103.att', 'Special Event Map', 503, 100, 100, 90386, 'Special Event Map');
INSERT INTO `world_template` VALUES (132, 132, 0, 'Terrain133.att', 'Scarlet Icarus', 631, 100, 100, 89518202, '');
INSERT INTO `world_template` VALUES (133, 133, 0, 'Terrain134.att', 'Temple of Arnil', 634, 100, 100, 89518202, '');

SET FOREIGN_KEY_CHECKS = 1;
