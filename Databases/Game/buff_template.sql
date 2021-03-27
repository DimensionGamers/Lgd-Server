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

 Date: 19/03/2021 11:06:34
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for buff_template
-- ----------------------------
DROP TABLE IF EXISTS `buff_template`;
CREATE TABLE `buff_template`  (
  `buff` smallint(5) UNSIGNED NOT NULL,
  `group` smallint(5) UNSIGNED NOT NULL,
  `item_type` smallint(6) NOT NULL DEFAULT -1,
  `item_index` smallint(6) NOT NULL DEFAULT -1,
  `name` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `description` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL,
  `send` tinyint(3) UNSIGNED NOT NULL,
  `debuff` tinyint(3) UNSIGNED NOT NULL,
  `effect_1` tinyint(3) UNSIGNED NOT NULL,
  `value_1` float NOT NULL DEFAULT 0,
  `effect_2` tinyint(3) UNSIGNED NOT NULL,
  `value_2` float NOT NULL DEFAULT 0,
  `duration` int(10) UNSIGNED NOT NULL,
  PRIMARY KEY (`buff`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of buff_template
-- ----------------------------
INSERT INTO `buff_template` VALUES (1, 1, -1, -1, 'Increase Attack Power', 'Attack power increases.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (2, 2, -1, -1, 'Increase Defense', 'Defensibility increases.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (3, 3, -1, -1, 'Shadow Phantom effect', 'Attack power and Defensibility increases', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (4, 4, -1, -1, 'Soul Barrier', 'Manna absorbs the damage.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (5, 5, -1, -1, 'Critical Damage increase', 'Critical damage increases.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (6, 6, -1, -1, 'Infinity Arrow', 'Arrows do not become consumed.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (7, 7, -1, -1, 'AG recovery speed increase', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (8, 8, -1, -1, 'Swell Life', 'Health stat increases to maximum.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (9, 9, -1, -1, 'Swell Mana', 'Mana stat increases to maximum.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (10, 10, -1, -1, 'Potion of Blessing', 'Attack power +20%% increase', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (11, 11, -1, -1, 'Potion of Soul', 'AG recovery speed increase +8/Lightning and Ice resistance increase/attack speed increase +20', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (12, 12, -1, -1, 'Abolish Magic', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (13, 13, -1, -1, 'Gate Open/Close Status', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (14, 14, -1, -1, 'Defense Guild', 'Watching over the castle.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (15, 14, -1, -1, 'Attack Guild 1', 'In the process of attacking the castle', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (16, 14, -1, -1, 'Attack Guild 2', 'In the process of attacking the castle', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (17, 14, -1, -1, 'Attack Guild 3', 'In the process of attacking the castle', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (18, 18, -1, -1, 'Invisibility', 'Transparency Status', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (19, 19, -1, -1, 'Dark Rod Battle Skill', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (20, 20, -1, -1, 'Crown Registration Status', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (21, 21, -1, -1, 'Altar of Wolf contract enabled', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (22, 21, -1, -1, 'Altar of Wolf contract disabled', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (23, 21, -1, -1, 'Altar of Wolf contract Status', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (24, 21, -1, -1, 'Altar of Wolf contract attempt', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (25, 21, -1, -1, 'Altar of Wolf besiegement Status', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (26, 21, -1, -1, 'Altar of Wolf hero contract Status', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (27, 27, -1, -1, 'Cry Wolf NPC transparency', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (28, 28, -1, -1, 'GM', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (29, 29, -1, -1, 'GM Buff - Stat', 'Increase HP +500 / Increase Mana +500', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (30, 30, -1, -1, 'GM Buff - Damage & Defense', 'Increase Damage +100 / Increase Defense +100 / Increase Defense Rate +500', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (31, 31, -1, -1, 'GM Buff - Experience', 'Increase Experience +50%%', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (32, 32, -1, -1, 'Temple - Spell of Quickness', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (33, 32, -1, -1, 'Temple - Spell of Sublimation', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (34, 32, -1, -1, 'Spell of Protection', 'Damage absorbed at 100%%.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (35, 432, 14, 46, 'Jack O\'Lantern Blessings', 'Attack speed increase +10', 1, 0, 1, 10, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (36, 435, 14, 47, 'Jack O\'Lantern Wrath', 'Attack power increase +25', 1, 0, 2, 40, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (37, 435, 14, 48, 'Jack O\'Lantern Cry', 'Defensibility increase +100', 1, 0, 3, 100, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (38, 432, 14, 49, 'Jack O\'Lantern Food', 'Maximum Life increase +500', 1, 0, 4, 700, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (39, 432, 14, 50, 'Jack O\'Lantern Drink', 'Maximum Mana increase +500', 1, 0, 5, 700, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (40, 40, 13, 43, 'Seal of Ascension', 'EXP gain increase 150%%', 1, 0, 6, 20, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (41, 41, 13, 44, 'Seal of Wealth', 'EXP gain increase 200%%/Item gain increase 150%%', 1, 0, 6, 20, 7, 50, 0);
INSERT INTO `buff_template` VALUES (42, 42, 13, 45, 'Seal of Sustenance', 'EXP gain disabled', 1, 0, 8, 0, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (43, 43, 13, 59, 'Seal of Mobility', 'Mobility enabled', 1, 0, 16, 0, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (44, 44, 14, 72, 'Scroll of Quickness', 'Attack speed increase +15', 1, 0, 1, 20, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (45, 45, 14, 73, 'Scroll of Defense', 'Defensibility increase +60', 1, 0, 3, 120, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (46, 46, 14, 74, 'Scroll of Wrath', 'Attack power increase +30', 1, 0, 14, 60, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (47, 47, 14, 75, 'Scroll of Wizardry', 'Wizardry increase +30', 1, 0, 15, 60, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (48, 48, 14, 76, 'Scroll of Health', 'HP increase +200', 1, 0, 4, 400, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (49, 49, 14, 77, 'Scroll of Mana', 'Mana increase +300', 1, 0, 5, 600, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (50, 50, 14, 78, 'Elixir of Strength', 'Strength Status +50', 1, 0, 9, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (51, 50, 14, 79, 'Elixir of Agility', 'Agility Status +50', 1, 0, 10, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (52, 50, 14, 80, 'Elixir of Health', 'Health Status +50', 1, 0, 11, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (53, 50, 14, 81, 'Elixir of Energy', 'Energy Status +50', 1, 0, 12, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (54, 50, 14, 82, 'Elixir of Control', 'Control Status +50', 1, 0, 13, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (55, 55, -1, -1, 'Poison', 'Poisoned status of condition', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (56, 56, -1, -1, 'Ice', 'Mobility speed decreases.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (57, 57, -1, -1, 'Ice Arrow', 'Mobility disabled', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (58, 58, -1, -1, 'Defensibility reduction', 'Random defensibility decreases at 10%%.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (59, 59, -1, -1, 'Attack power reduction', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (60, 60, -1, -1, 'Wizardry increase reduction', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (61, 61, -1, -1, 'Stun', 'Mobility disabled', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (62, 62, -1, -1, 'Magic Defense', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (63, 63, -1, -1, 'Monster Magic Attack Resistance', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (64, 64, -1, -1, 'Monster Chemistry Attack Resistance', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (65, 65, -1, -1, 'Spell of Restriction', 'Mobility disabled', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (66, 66, -1, -1, 'Cry Wolf Divine Protection Status', 'Cry Wolf Divine Protection Status', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (67, 66, -1, -1, 'Cry Wolf Divine Protection Status', 'Cry Wolf Divine Protection Status', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (68, 66, -1, -1, 'Cry Wolf Divine Protection Status', 'Cry Wolf Divine Protection Status', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (69, 66, -1, -1, 'Cry Wolf Divine Protection Status', 'Cry Wolf Divine Protection Status', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (70, 66, -1, -1, 'Cry Wolf Divine Protection Status', 'Cry Wolf Divine Protection Status', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (71, 71, -1, -1, 'Damage Reflection', 'Partial damage reflected out.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (72, 72, -1, -1, 'Sleep', 'Short period of nap', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (73, 73, -1, -1, 'Blind', 'Shortened view and the attack accuracy decreases.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (74, 74, -1, -1, 'Requiem', 'Summoner Neil attack', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (75, 75, -1, -1, 'Explosion', 'Summoner Shamut attack', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (76, 76, -1, -1, 'Weakness', 'Attack power reduction', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (77, 77, -1, -1, 'Innovation', 'Defensibility reduction', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (78, 432, 14, 85, 'Cherry Blossom Wine', 'Maximum Mana increase +700', 1, 0, 5, 700, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (79, 432, 14, 86, 'Cherry Blossom Rice Cake', 'Maximum Life increase +700', 1, 0, 4, 700, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (80, 435, 14, 87, 'Cherry Blossom Flower Petal', 'Attack power increase +40', 1, 0, 2, 40, 0, 10, 1800);
INSERT INTO `buff_template` VALUES (81, 81, -1, -1, 'Berserker', 'Curse spell, Wizardry, Attack power and Maximum Mana increases, Defense and Maximum Life decreases', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (82, 82, -1, -1, 'Expansion of Wizardry', 'Minimum Wizardry increases.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (83, 83, -1, -1, 'Flame Strike', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (84, 84, -1, -1, 'Gigantic Storm', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (85, 85, -1, -1, 'Lightning Shock', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (86, 86, -1, -1, 'Cold', 'Detrimental cold reduces /the mobility speed.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (87, 87, 13, 62, 'Seal of Healing', 'EXP gain increase 150%%/Automatic Life recovery at 5%%', 1, 0, 6, 10, 53, 5, 1800);
INSERT INTO `buff_template` VALUES (88, 88, 13, 63, 'Seal of Divinity', 'Item gain increase at 150%%/Automatic Mana recovery at 5%%', 1, 0, 7, 10, 54, 5, 1800);
INSERT INTO `buff_template` VALUES (89, 89, 14, 97, 'Scroll of Battle', 'Critical damage increase at 20%%', 1, 0, 55, 25, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (90, 90, 14, 98, 'Scroll of Strength', 'Excellent damage increase at 20%%', 1, 0, 56, 25, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (91, 91, -1, -1, 'Blessing of Christmas', 'Attack power and Defensibility increases', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (92, 91, -1, -1, 'Santa Healing', 'Maximum Life increase +500', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (93, 91, -1, -1, 'Santa Protection', 'Maximum Mana increase +500', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (94, 91, -1, -1, 'Santa Strengthener', 'Attack power +30', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (95, 91, -1, -1, 'Santa Defense', 'Defensibility +100', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (96, 91, -1, -1, 'Santa Quickness', 'Attack speed +15', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (97, 91, -1, -1, 'Santa Fortune', 'AG recovery speed increase +10', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (98, 98, -1, -1, 'Watch', 'You may enter and watch a duel in Arena.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (99, 99, 13, 81, 'Talisman of Guardian', 'No penalty for dying.', 1, 0, 0, 0, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (100, 100, 13, 82, 'Talisman of Item Protection', 'Keeps item durable', 1, 0, 0, 0, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (101, 40, 13, 93, 'Master Seal of Ascension', 'EXP gain increase 150%%', 1, 0, 6, 20, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (102, 41, 13, 94, 'Master Seal of Wealth', 'EXP gain increase 200%%/Item gain increase 150%%', 1, 0, 6, 20, 7, 50, 1800);
INSERT INTO `buff_template` VALUES (103, 103, 13, 95, 'Gladiator\'s Honor', 'Increases EXP by 20%% /Rare Item drop rate/can be used in Vulcanus only.', 1, 0, 0, 0, 0, 0, 3600);
INSERT INTO `buff_template` VALUES (104, 104, 13, 96, 'Seal of Strength', 'Increases Attack Rate and Defense Rate\'', 1, 0, 2, 50, 3, 60, 1800);
INSERT INTO `buff_template` VALUES (105, 105, -1, -1, 'Ascension', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (106, 106, -1, -1, 'PK Penalty', 'Warp Command Window access restricted for five minutes.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (107, 107, -1, -1, 'You feel tired.', '50%% EXP Gain /50%% Item Drop Rate ', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (108, 107, -1, -1, 'You feel exhausted.', '50%% EXP Gain /0%% Item Drop Rate ', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (112, 112, 13, 103, 'Party EXP Bonus Item', 'Up to 170% EXP gain, depending on the number of members in the party.', 1, 0, 68, 10, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (113, 113, 13, 104, 'Max AG Boost Aura', 'Increases your max AG.', 1, 0, 66, 1, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (114, 114, 13, 105, 'Max SD Boost Aura', 'Increases your max SD.', 1, 0, 67, 5, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (115, 115, 13, 117, 'Minimum Vitality', '100%% EXP Gain/100%% Item Drop Rate/Stops stamina from diminishing for a fixed duration of time.', 1, 0, 69, 100, 0, 0, 21600);
INSERT INTO `buff_template` VALUES (116, 115, 13, 118, 'Low Vitality', '100%% EXP Gain/100%% Item Drop Rate/Stops stamina from diminishing for a fixed duration of time.', 1, 0, 69, 100, 0, 0, 86400);
INSERT INTO `buff_template` VALUES (117, 115, 13, 119, 'Medium Vitality', '100%% EXP Gain/100%% Item Drop Rate/Stops stamina from diminishing for a fixed duration of time.', 1, 0, 69, 100, 0, 0, 604800);
INSERT INTO `buff_template` VALUES (118, 115, 13, 120, 'High Vitality', '100%% EXP Gain/100%% Item Drop Rate/Stops stamina from diminishing for a fixed duration of time.', 1, 0, 69, 100, 0, 0, 2592000);
INSERT INTO `buff_template` VALUES (119, 119, 13, 116, 'Seal of Wealth', 'Increases Item Drop Rate to 150%%.', 1, 0, 7, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (120, 120, -1, -1, 'Punishment for Illicit Software Use', '50%% EXP Gain /0%% Item Drop Rate ', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (121, 121, 14, 140, 'Scroll of Healing', '+3%% HP Regen/+100 HP', 1, 0, 53, 3, 4, 100, 1800);
INSERT INTO `buff_template` VALUES (129, 129, -1, -1, 'Ignore Enemy\'s Defense', 'Increases chance to ignore enemy\'s defense and inflict damage.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (130, 130, -1, -1, 'Increase Health', 'Increases health stat.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (131, 131, -1, -1, 'Increase Defense Rate', 'Increases maximum defend rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (132, 132, -1, -1, 'Defense success rate decrease', 'Decreases defense success rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (135, 8, -1, -1, 'Swell Life', 'Increases max life and Mana.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (136, 8, -1, -1, 'Swell Life', 'Increases max life, Mana, and AG.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (138, 82, -1, -1, 'Expansion of Wizardry', 'Increases min Wizardry and max Wizardry.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (139, 82, -1, -1, 'Expansion of Wizardry', 'Increases min Wizardry, max Wizardry,/ and critical damage rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (140, 4, -1, -1, 'Soul Barrier', 'Uses Mana to absorb damage,/ and increases max Mana.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (143, 6, -1, -1, 'Infinity Arrow', 'Arrows are not consumed,/ and increases your damage.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (148, 5, -1, -1, 'Critical Damage increase', 'Increases critical damage and critical damage rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (149, 5, -1, -1, 'Critical Damage increase', 'Increases critical damage and critical damage rate,/ and also increases excellent damage rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (150, 81, -1, -1, 'Berserker', 'Curse spell, Wizardry, Attack power and Maximum Mana increases, Defense and Maximum Life decreases', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (151, 81, -1, -1, 'Berserker', 'Curse spell, Wizardry, Attack power and Maximum Mana increases, Defense and Maximum Life decreases', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (152, 81, -1, -1, 'Berserker', 'Curse spell, Wizardry, Attack power and Maximum Mana increases, Defense and Maximum Life decreases', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (153, 131, -1, -1, 'Increase Defense Success Rate', 'Increases defense success rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (154, 131, -1, -1, 'Increase Defense Success Rate', 'Increases maximum defense and defense success rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (155, 130, -1, -1, 'Increase HP', 'Increases HP.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (156, 129, -1, -1, 'Ignore Enemy\'s Defense', 'Increases the chance of ignoring Enemy DEF. If successful, enemy a chance to take a Penetrate Armor.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (164, 58, -1, -1, 'Decrease Defense Rate', 'Decreases defense rate.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (125, 124, 13, 131, 'Maple Charm', 'Maximum Mana +50.', 0, 0, 5, 50, 0, 0, 0);
INSERT INTO `buff_template` VALUES (124, 124, 13, 130, 'Oak Charm', 'Maximum Life +50.', 0, 0, 4, 50, 0, 0, 0);
INSERT INTO `buff_template` VALUES (123, 122, 13, 129, 'Goat Figurine', 'Excellent Damage +10%.', 0, 0, 56, 10, 0, 0, 0);
INSERT INTO `buff_template` VALUES (122, 122, 13, 128, 'Hawk Figurine', 'Critical Damage +10%.', 0, 0, 55, 10, 0, 0, 0);
INSERT INTO `buff_template` VALUES (126, 124, 13, 132, 'Golden Oak Charm', 'Maximum Life +150 / Maximum SD +50.', 0, 0, 4, 100, 70, 500, 0);
INSERT INTO `buff_template` VALUES (127, 124, 13, 133, 'Golden Maple Charm', 'Maximum Mana +150 / Maximum AG +50.', 0, 0, 5, 150, 69, 50, 0);
INSERT INTO `buff_template` VALUES (128, 128, 13, 134, 'Worn Horse Shoe', 'Item gain increase +20%.', 0, 0, 7, 20, 0, 0, 0);
INSERT INTO `buff_template` VALUES (134, 134, -1, -1, 'Iron Defense', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (137, 137, -1, -1, 'Death Stab Enhanced', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (141, 141, -1, -1, 'Frozen Stab Mastered', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (142, 142, -1, -1, 'Bless', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (144, 73, -1, -1, 'Blind Improved', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (145, 145, -1, -1, 'Drain Life Enhanced', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (146, 146, -1, -1, 'Ice Storm Enhanced', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (147, 147, -1, -1, 'Earth Prison', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (157, 157, -1, -1, 'Dragon Lore Enhanced', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (158, 158, -1, -1, 'Chain Driver Enhanced', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (159, 159, -1, -1, 'Poison Arrow', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (160, 159, -1, -1, 'Poison Arrow Improved', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (161, 142, -1, -1, 'Bless Improved', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (162, 76, -1, -1, 'Lesser Damage Improved', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (163, 77, -1, -1, 'Lesser Defense Improved', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (165, 134, -1, -1, 'Iron Defense Improved', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (166, 166, -1, -1, 'Blood Howling', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (167, 166, -1, -1, 'Blood Howling Improved', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (174, 174, -1, -1, 'Pentagram Jewel Half SD', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (175, 175, -1, -1, 'Pentagram Jewel Half MP', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (176, 176, -1, -1, 'Pentagram Jewel Half Speed', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (177, 177, -1, -1, 'Pentagram Jewel Half HP', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (178, 178, -1, -1, 'Pentagram Jewel Stun', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (186, 186, -1, -1, 'Pentagram Jewel Slow', '', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (190, 190, 13, 177, 'Talisman of Ascension 1', '', 1, 0, 6, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (191, 190, 13, 178, 'Talisman of Ascension 2', '', 1, 0, 6, 100, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (192, 190, 13, 179, 'Talisman of Ascension 3', '', 1, 0, 6, 200, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (193, 193, 13, 180, 'Seal of Ascension', '', 1, 0, 6, 100, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (194, 193, 13, 181, 'Master Seal of Ascension', '', 1, 0, 6, 100, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (195, 195, 14, 224, 'Blessing of Light', '', 1, 0, 6, 200, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (196, 45, 14, 225, 'Master Scroll of Defense', '', 1, 0, 3, 120, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (197, 47, 14, 226, 'Master Scroll of Magic Damage', '', 1, 0, 15, 60, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (198, 48, 14, 227, 'Master Scroll of Life', '', 1, 0, 4, 400, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (199, 49, 14, 228, 'Master Scroll of Mana', '', 1, 0, 5, 600, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (200, 46, 14, 229, 'Master Scroll of Damage', '', 1, 0, 14, 60, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (201, 121, 14, 230, 'Master Scroll of Healing', '', 1, 0, 24, 4, 4, 200, 1800);
INSERT INTO `buff_template` VALUES (202, 89, 14, 231, 'Master Scroll of Battle', '', 1, 0, 55, 25, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (203, 89, 14, 232, 'Master Scroll of Strength', '', 1, 0, 56, 25, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (204, 44, 14, 235, 'Master Scroll of Quick', '', 1, 0, 1, 20, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (205, 205, 13, 2, 'Horn of Uniria', 'Summons Uniria to mount.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (206, 205, 13, 3, 'Horn of Dinorant', 'Summons Dinorant to mount.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (207, 205, 13, 4, 'Horn of Dark Horse', 'Summons Dark Horse to mount.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (208, 205, 13, 37, 'Horn of Fenrir', 'Summons Fenrir to mount.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (209, 209, -1, -1, 'Paralyze', 'Reduces your Mvmt SP by 50% by paralyzing your legs.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (210, 209, -1, -1, 'Paralyze', 'Reduces your Mvmt SP by 50% by paralyzing your legs, and gives continual DMG.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (211, 211, -1, -1, 'Internet Cafe', 'Extra XP/Chaos Castle in PC Cafe/Access to Kalima/Accum. Gob. Pts.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (212, 195, 14, 263, 'Bless of Light (Low Grade)', 'Receiving the blessing for a short period of time, you acquire 50% of basic EXP additionally.', 1, 0, 6, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (213, 195, 14, 264, 'Bless of Light (Middle Grade)', 'Receiving the blessing for a short period of time, you acquire 100% of basic EXP additionally.', 1, 0, 6, 100, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (214, 214, -1, -1, 'Evasion', 'Increases your block chance and makes you immune to immobilizing effect', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (215, 211, -1, -1, 'Internet Cafe', 'Extra XP/Chaos Castle in PC Cafe/Acesss to Kalima/Accum. Gob. Pts./Stam. Sys. Disabled', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (216, 216, -1, -1, 'Circle Shield', 'Chance to decrease opponent\'s AG by 10', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (217, 217, -1, -1, 'Obsidian', 'Increases Skill Damage by %d', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (218, 218, -1, -1, 'Wrath', 'Decreases Defense by 30%, increases Damage by 50%', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (219, 219, -1, -1, 'Burst', 'Increases chance of True Damage', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (220, 217, -1, -1, 'Obsidian PowUp', 'Increases Skill Damage by %d', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (221, 216, -1, -1, 'Circle Shield PowUp', 'Chance to increase AG reduce effect by %d on hit.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (222, 216, -1, -1, 'Circle Shield Mastery', 'Chance to increase AG reduce effect by %d on hit.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (223, 218, -1, -1, 'Wrath PowUp', 'Decreases Defense by 30%, increases Damage by 50%', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (224, 218, -1, -1, 'Wrath Proficiency', 'Decreases Defense by 30%, increases Damage by 50%', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (225, 218, -1, -1, 'Wrath Mastery', 'Increases damage by 50%', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (226, 219, -1, -1, 'Burst PowUp', 'Increases chance of True Damage', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (227, 227, -1, -1, 'Awakening Success Effect', 'This is Evomon Awakening Success Effect.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (228, 228, -1, -1, 'Awakening Failure Effect', 'This is Evomon Awakening Failure Effect', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (229, 229, -1, -1, 'Nars - Berserker', 'Increases Damage.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (230, 230, -1, -1, 'Nars - Clone', 'Summons a clone.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (231, 231, -1, -1, 'Nars - Spin', 'Spins the target.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (237, 237, 13, 476, ' ', ' ', 1, 0, 6, 50, 0, 0, 0);
INSERT INTO `buff_template` VALUES (238, 238, -1, -1, 'Bastion', 'Elemental Damage -90%.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (239, 239, -1, -1, 'Hemorrhage', 'Bleeding Damage for every second.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (240, 240, -1, -1, 'Paralysis', 'Movement speed and regeneration effect will be reduced. / (reduced effectiveness of Potions)', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (241, 241, -1, -1, 'Bondage', 'Cannot move while Earth Power is holding the target\'s leg.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (242, 242, -1, -1, 'Blindness', 'Blinds the target and reduces its Attack Success Rate by %d%.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (243, 243, -1, -1, 'Immune I', 'Becomes immune to Elemental Damage.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (244, 243, -1, -1, 'Immune II', 'Becomes immune to every Debuff Effect.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (245, 245, -1, -1, 'Widened I', 'Greatly increases the Elemental Damage and Elemental Attack Rate temporarily.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (246, 246, -1, -1, 'Archangel\'s will', 'Attack damage and skill damage is increased.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (247, 247, -1, -1, 'Ferea Party EXP buff', 'Up to 10% EXP gain is added on basic EXP/5% additional EXP per party member.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (179, 179, -1, -1, 'Fire Tower', 'ATK+25/EXP+20%/when fire equipped ATK&DEF+5%/set item drop chance(excludes Debenter)', 1, 0, 100, 25, 6, 20, 0);
INSERT INTO `buff_template` VALUES (180, 180, -1, -1, 'Water Tower', 'DEF +50/EXP +20%/when Water equipped, ATK & DEF +5%/set item drop chance(excludes Debenter)', 1, 0, 3, 50, 6, 20, 0);
INSERT INTO `buff_template` VALUES (181, 181, -1, -1, 'Wind Tower', 'CRIT+5%/EXP+20%/when Earth equipped ATK&DEF+5%/set item drop chance(excludes Debenter)', 1, 0, 55, 5, 6, 20, 0);
INSERT INTO `buff_template` VALUES (182, 182, -1, -1, 'Earth Tower', 'ATK+30/EXP+20%/when Wind equipped, ATK&DEF+5%/set item drop chance(excludes Debenter)', 1, 0, 2, 30, 6, 20, 0);
INSERT INTO `buff_template` VALUES (183, 183, -1, -1, 'Darkness Tower', 'DMG Rec.-10%/EXP+20%/when Darkness equipped, ATK&DEF+5%/set item drop chance(excludes Debenter)', 1, 0, 76, 10, 6, 20, 0);
INSERT INTO `buff_template` VALUES (184, 184, -1, -1, 'Resurrection Penalty', 'Movement speed decreased for a certain period of time.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (185, 185, -1, -1, 'Vault Expansion', 'Vault Expansion', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (232, 232, 13, 5, 'Horn of Dark Spirit', 'Summon Dark Spirit', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (248, 248, -1, -1, 'Blessing of Lunedil', 'Experience Rise Stage 1: 5%', 1, 0, 6, 5, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (249, 248, -1, -1, 'Blessing of Lunedil', 'Experience Rise Stage 2: 10%', 1, 0, 6, 10, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (250, 248, -1, -1, 'Blessing of Lunedil', 'Experience Rise Stage 3: 15%', 1, 0, 6, 15, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (251, 248, -1, -1, 'Blessing of Lunedil', 'Experience Rise Stage 4: 20%', 1, 0, 6, 20, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (252, 248, -1, -1, 'Blessing of Lunedil', 'Experience Rise Stage 5: 25%', 1, 0, 6, 25, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (253, 195, 14, 371, 'Bless of Light (Greater)', 'Receiving the blessing for a short period of time, you acquire 200% of basic EXP additionally.', 1, 0, 6, 200, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (254, 195, 14, 372, 'Bless of Light (Middle Grade)', 'Receiving the blessing for a short period of time, you acquire 100% of basic EXP additionally.', 1, 0, 6, 100, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (255, 195, 14, 373, 'Bless of Light (Low Grade)', 'Receiving the blessing for a short period of time, you acquire 50% of basic EXP additionally.', 1, 0, 6, 50, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (264, 256, -1, -1, 'Swell Life Enhancement', 'Increases Swell Life HP', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (265, 257, -1, -1, 'Mana Shield Enhancement', 'Increases Mana Shield Damage Reduction', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (266, 258, -1, -1, 'Berserker Enhancement', 'Increases Berserker Attack Power', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (267, 259, -1, -1, 'Berserker Enhancement', 'Eliminated Berserker Defense Reduction', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (268, 260, -1, -1, 'Increase Stamina Enhancement', 'Increase Stamina Effect', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (269, 261, -1, -1, 'Wrath Enhancement', 'Increases Wrath Attack Power', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (270, 262, -1, -1, 'Wrath Enhancement', 'Wrath defense reduction effect reduced', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (271, 263, -1, -1, 'Decrease SD ratio', 'Decrease SD ratio', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (272, 264, -1, -1, 'SD rise', 'SD rise', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (273, 265, -1, -1, 'Poison', 'Poison', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (274, 266, -1, -1, 'Freezing', 'Freezing', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (275, 267, -1, -1, 'Bleeding', 'Bleeding', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (276, 268, -1, -1, 'Shock', 'Shock', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (277, 269, -1, -1, 'Addicted', 'Addicted', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (278, 270, -1, -1, 'Freezing', 'Freezing', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (279, 271, -1, -1, 'Excessive bleeding', 'Excessive bleeding', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (280, 272, -1, -1, 'Shock', 'Shock', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (281, 273, -1, -1, 'Increased SD reduction', 'Increased SD reduction', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (282, 274, -1, -1, 'Increases maximum HP', 'Increases maximum HP', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (283, 275, -1, -1, '4th stats', '4th stats', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (284, 276, -1, -1, 'Increases basic defense', 'Increases basic defense', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (285, 277, -1, -1, 'Increases 4th wings defense', 'Increases 4th wings defense', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (286, 278, -1, -1, 'Increases attack power', 'Increases attack power', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (287, 279, -1, -1, 'Increases wizardy', 'Increases wizardy', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (288, 280, -1, -1, '4th stats', '4th stats', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (289, 281, -1, -1, 'Increases skill damage', 'Increases skill damage', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (290, 282, -1, -1, 'Increases 4th wings damage', 'Increases 4th wings damage', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (291, 283, -1, -1, 'Increases attack power/wizardy', 'Increases attack power/wizardy', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (256, 432, 14, 432, '[Increase]HP Elixir', 'Increases maximum HP +700', 1, 0, 4, 700, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (257, 432, 14, 433, '[Increase]Mana Elixir', 'Increases maximum Mana +700', 1, 0, 5, 700, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (258, 432, 14, 434, '[Increase]Speed Elixir', 'Increases attack speed +10', 1, 0, 1, 10, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (259, 435, 14, 435, '[Enhance]Attack Elixir', 'Increases attack power +40', 1, 0, 14, 40, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (260, 435, 14, 436, '[Enhance]Wizardry Elixir', 'Increases wizardry +40', 1, 0, 15, 40, 17, 40, 1800);
INSERT INTO `buff_template` VALUES (261, 435, 14, 437, '[Enhance]Defense Elixir', 'Increases defense +100', 1, 0, 3, 100, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (262, 435, 14, 438, '[Enhance]Critical Elixir', 'Increases critical damage +85', 1, 0, 57, 85, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (263, 435, 14, 439, '[Enhance]Excellent Elixir', 'Increases excellent damage +85', 1, 0, 58, 85, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (292, 292, 14, 453, 'Switch Mode Scroll', 'You can use switch mode with CTRL + F.', 1, 0, 0, 0, 0, 0, 1800);
INSERT INTO `buff_template` VALUES (293, 293, -1, -1, 'Burst', 'Increases damage and increases skill MP consumption.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (294, 294, -1, -1, 'Haste', 'Increases attack speed and increases skill AG consumption.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (295, 293, -1, -1, 'Burst Enhnanced', 'Increases damage and increases skill MP consumption.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (296, 293, -1, -1, 'Burst Mastery', 'Increases damage and increases skill MP consumption.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (298, 294, -1, -1, 'Haste Enhnanced', 'Increases attack speed and increases skill AG consumption.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (299, 294, -1, -1, 'Haste Mastery', 'Increases attack speed and increases skill AG consumption.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (301, 81, -1, -1, 'Darkness', 'Increases curse/defense, decreases maximum HP', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (302, 81, -1, -1, 'Darkness', 'Increases curse/defense, decreases maximum HP', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (303, 81, -1, -1, 'Darkness', 'Increases curse/defense', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (304, 76, -1, -1, 'Weakness Mastery', 'Decreases damage.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (305, 77, -1, -1, 'Innovation Mastery', 'Decreases defense.', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (306, 306, -1, -1, 'Deathside', 'Deathside attacks the enemy target with the character.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (307, 307, 12, 471, 'Guardian Horse', 'Guardian ride item', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (308, 308, -1, -1, 'Increases speed', 'Guardian ride skill', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (309, 309, -1, -1, 'Elite Monster - Bleeding', 'Elite Monster - Bleeding', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (310, 310, -1, -1, 'Elite Monster - Poison', 'Elite Monster - Poison', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (311, 311, -1, -1, 'Elite Monster - Decreases Attack Damage', 'Elite Monster - Decreases Attack Damage', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (312, 312, -1, -1, 'Elite Monster - Decreases Attack Speed', 'Elite Monster - Decreases Attack Speed', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (313, 313, -1, -1, 'Elite Monster - Decreases Defense', 'Elite Monster - Decreases Defense', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (314, 283, -1, -1, 'Increases wizardry/curse', 'Increases wizardry/curse', 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (315, 297, -1, -1, 'Bat Flock', 'You take damage every second from bats', 1, 1, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (316, 298, -1, -1, 'Detection', 'Temporarily detects the location of nearby creatures / (results remain for 1 minute).', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (317, 299, -1, -1, 'Demolish', 'Ignores the enemy\'s defense by %d when attacking.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (318, 298, -1, -1, 'Detection Enhancement', 'Temporarily detects the location of nearby creatures / (results last for 1 minute).', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (319, 299, -1, -1, 'Demolish Enhancement', 'Ignores the enemy\'s defense by %d when attacking.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (320, 299, -1, -1, 'Demolish Mastery', 'Ignores the enemy\'s defense by %d when attacking.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (321, 300, -1, -1, 'Demolish - Decreases Skill Damage', '', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (322, 290, 12, 483, 'Ice Dragon Mount', 'Guardian Mount Item', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (323, 290, 12, 484, 'Ice Dragon(Rare) Mount', 'Guardian Mount Item', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (324, 301, 255, 255, 'Sword\'s Wrath', 'Rage mode. Increases the range and damage of blow type attack skills.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (325, 302, 255, 255, 'Strong Belief', 'Belief mode. Stat efficiency is changed and you can use Solid Protection skills.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (326, 303, 255, 255, 'Solid Protection', 'Increases the damage of the party member, absorbs the party member HP, and takes part of the damage instead.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (327, 301, 255, 255, 'Sword\'s Wrath Enhancement', 'Rage mode. Increases the range and damage of blow type attack skills.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (328, 301, 255, 255, 'Sword\'s Wrath Mastery', 'Rage mode. Increases the range and damage of blow type attack skills.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (329, 303, 255, 255, 'Solid Protection Enhancement', 'Increases the damage of the party member, absorbs the party member HP, and takes part of the damage instead.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (330, 303, 255, 255, 'Solid Protection Skill', 'Increases the damage of the party member, absorbs the party member HP, and takes part of the damage instead.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (331, 303, 255, 255, 'Solid Protection Mastery', 'Increases the damage of the party member, absorbs the party member HP, and takes part of the damage instead.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (332, 303, 255, 255, 'Solid Protection Mastery', 'Increases the damage of the party member, absorbs the party member HP, and takes part of the damage instead.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (333, 304, 255, 255, 'Solid Protection Effect', 'Dark Knight Protection Effect', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (334, 302, 255, 255, 'Strong Belief Skill Enhancement', 'Belief mode. Stat efficiency is changed and you can use Solid Protection skills.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (335, 303, 255, 255, 'Solid Protection Skill Enhancement', 'Increases the damage of the party member, absorbs the party member HP, and takes part of the damage instead.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (336, 302, 255, 255, 'Strong Belief Enhancement', 'Belief mode. Stat efficiency is changed and you can use Solid Protection skills.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (337, 305, -1, -1, 'Increases attack succcess rate - Party Member', 'Increases party members attack success rate.', 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `buff_template` VALUES (338, 305, -1, -1, 'Increases attack succcess rate - Caster', 'Increases caster attack success rate.', 1, 0, 0, 0, 0, 0, 0);

SET FOREIGN_KEY_CHECKS = 1;
