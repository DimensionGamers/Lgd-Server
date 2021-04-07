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

 Date: 07/04/2021 10:09:16
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for muun
-- ----------------------------
DROP TABLE IF EXISTS `muun`;
CREATE TABLE `muun`  (
  `item` smallint(5) UNSIGNED NOT NULL,
  `rank` tinyint(3) UNSIGNED NOT NULL,
  `option` tinyint(3) UNSIGNED NOT NULL,
  `special_option_type` tinyint(3) UNSIGNED NOT NULL,
  `special_option_value` int(11) NOT NULL DEFAULT 0,
  `evolution_item` smallint(6) NOT NULL DEFAULT -1,
  PRIMARY KEY (`item`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of muun
-- ----------------------------
INSERT INTO `muun` VALUES (8192, 4, 42, 1, 20, 8193);
INSERT INTO `muun` VALUES (8193, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8194, 2, 38, 1, 5, 8195);
INSERT INTO `muun` VALUES (8195, 2, 38, 1, 5, -1);
INSERT INTO `muun` VALUES (8196, 2, 25, 3, 2, 8197);
INSERT INTO `muun` VALUES (8197, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8198, 4, 40, 1, 20, 8199);
INSERT INTO `muun` VALUES (8199, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8200, 2, 57, 1, 5, 8201);
INSERT INTO `muun` VALUES (8201, 2, 57, 1, 5, -1);
INSERT INTO `muun` VALUES (8202, 2, 28, 3, 2, 8203);
INSERT INTO `muun` VALUES (8203, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8204, 4, 42, 1, 20, 8205);
INSERT INTO `muun` VALUES (8205, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8206, 2, 38, 1, 5, 8207);
INSERT INTO `muun` VALUES (8207, 2, 38, 1, 5, -1);
INSERT INTO `muun` VALUES (8208, 2, 28, 3, 2, 8209);
INSERT INTO `muun` VALUES (8209, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8210, 4, 40, 1, 20, 8211);
INSERT INTO `muun` VALUES (8211, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8212, 2, 23, 1, 5, 8213);
INSERT INTO `muun` VALUES (8213, 2, 23, 1, 5, -1);
INSERT INTO `muun` VALUES (8214, 2, 54, 3, 2, 8215);
INSERT INTO `muun` VALUES (8215, 2, 54, 3, 2, -1);
INSERT INTO `muun` VALUES (8216, 4, 50, 1, 20, 8217);
INSERT INTO `muun` VALUES (8217, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8218, 2, 48, 1, 5, 8219);
INSERT INTO `muun` VALUES (8219, 2, 48, 1, 5, -1);
INSERT INTO `muun` VALUES (8220, 2, 54, 3, 2, 8221);
INSERT INTO `muun` VALUES (8221, 2, 54, 3, 2, -1);
INSERT INTO `muun` VALUES (8222, 3, 61, 1, 10, -1);
INSERT INTO `muun` VALUES (8223, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8224, 4, 40, 1, 20, 8225);
INSERT INTO `muun` VALUES (8225, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8226, 4, 42, 1, 20, 8227);
INSERT INTO `muun` VALUES (8227, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8228, 4, 50, 1, 20, 8229);
INSERT INTO `muun` VALUES (8229, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8230, 4, 42, 1, 20, 8231);
INSERT INTO `muun` VALUES (8231, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8232, 3, 68, 1, 10, -1);
INSERT INTO `muun` VALUES (8233, 4, 40, 1, 20, 8234);
INSERT INTO `muun` VALUES (8234, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8235, 2, 38, 1, 5, 8236);
INSERT INTO `muun` VALUES (8236, 2, 38, 1, 5, -1);
INSERT INTO `muun` VALUES (8237, 2, 25, 3, 3, 8238);
INSERT INTO `muun` VALUES (8238, 2, 25, 3, 3, -1);
INSERT INTO `muun` VALUES (8239, 4, 50, 1, 20, 8240);
INSERT INTO `muun` VALUES (8240, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8241, 4, 40, 1, 20, 8242);
INSERT INTO `muun` VALUES (8242, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8243, 4, 42, 1, 20, 8244);
INSERT INTO `muun` VALUES (8244, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8245, 4, 40, 1, 20, 8246);
INSERT INTO `muun` VALUES (8246, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8247, 4, 50, 1, 20, 8248);
INSERT INTO `muun` VALUES (8248, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8249, 4, 40, 1, 20, 8250);
INSERT INTO `muun` VALUES (8250, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8251, 4, 50, 1, 20, 8252);
INSERT INTO `muun` VALUES (8252, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8253, 4, 42, 1, 20, 8254);
INSERT INTO `muun` VALUES (8254, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8255, 4, 40, 1, 20, 8256);
INSERT INTO `muun` VALUES (8256, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8257, 4, 50, 1, 20, 8258);
INSERT INTO `muun` VALUES (8258, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8259, 4, 42, 1, 20, 8260);
INSERT INTO `muun` VALUES (8260, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8261, 4, 69, 1, 20, -1);
INSERT INTO `muun` VALUES (8262, 4, 72, 1, 20, -1);
INSERT INTO `muun` VALUES (8263, 4, 50, 1, 20, 8264);
INSERT INTO `muun` VALUES (8264, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8265, 2, 23, 1, 5, 8266);
INSERT INTO `muun` VALUES (8266, 2, 23, 1, 5, -1);
INSERT INTO `muun` VALUES (8267, 2, 39, 3, 2, 8268);
INSERT INTO `muun` VALUES (8268, 2, 39, 3, 2, -1);
INSERT INTO `muun` VALUES (8269, 4, 40, 1, 20, 8270);
INSERT INTO `muun` VALUES (8270, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8271, 2, 70, 1, 5, 8272);
INSERT INTO `muun` VALUES (8272, 2, 70, 1, 5, -1);
INSERT INTO `muun` VALUES (8273, 2, 25, 3, 2, 8274);
INSERT INTO `muun` VALUES (8274, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8275, 4, 42, 1, 20, 8276);
INSERT INTO `muun` VALUES (8276, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8277, 4, 50, 1, 20, 8278);
INSERT INTO `muun` VALUES (8278, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8279, 4, 42, 1, 20, 8280);
INSERT INTO `muun` VALUES (8280, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8281, 4, 40, 1, 20, 8282);
INSERT INTO `muun` VALUES (8282, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8283, 2, 71, 1, 5, 8284);
INSERT INTO `muun` VALUES (8284, 2, 71, 1, 5, -1);
INSERT INTO `muun` VALUES (8285, 2, 25, 3, 2, 8286);
INSERT INTO `muun` VALUES (8286, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8287, 4, 50, 1, 20, 8288);
INSERT INTO `muun` VALUES (8288, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8289, 4, 42, 1, 20, 8290);
INSERT INTO `muun` VALUES (8290, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8291, 4, 50, 1, 20, 8292);
INSERT INTO `muun` VALUES (8292, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8293, 4, 40, 1, 20, 8294);
INSERT INTO `muun` VALUES (8294, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8295, 4, 50, 1, 20, 8296);
INSERT INTO `muun` VALUES (8296, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8297, 4, 42, 1, 20, 8298);
INSERT INTO `muun` VALUES (8298, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8299, 4, 69, 1, 20, -1);
INSERT INTO `muun` VALUES (8300, 4, 72, 1, 20, -1);
INSERT INTO `muun` VALUES (8301, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8302, 4, 50, 1, 20, 8303);
INSERT INTO `muun` VALUES (8303, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8304, 4, 40, 1, 20, 8305);
INSERT INTO `muun` VALUES (8305, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8306, 4, 50, 1, 20, 8307);
INSERT INTO `muun` VALUES (8307, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8308, 4, 42, 1, 20, 8309);
INSERT INTO `muun` VALUES (8309, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8310, 4, 50, 1, 20, 8311);
INSERT INTO `muun` VALUES (8311, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8312, 4, 40, 1, 20, 8313);
INSERT INTO `muun` VALUES (8313, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8314, 4, 50, 1, 20, 8315);
INSERT INTO `muun` VALUES (8315, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8316, 4, 42, 1, 20, 8317);
INSERT INTO `muun` VALUES (8317, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8318, 4, 50, 1, 20, 8319);
INSERT INTO `muun` VALUES (8319, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8320, 4, 40, 1, 20, 8321);
INSERT INTO `muun` VALUES (8321, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8322, 4, 42, 1, 20, 8323);
INSERT INTO `muun` VALUES (8323, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8324, 4, 50, 1, 20, 8325);
INSERT INTO `muun` VALUES (8325, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8326, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8327, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8404, 3, 0, 1, 10, 8405);
INSERT INTO `muun` VALUES (8405, 3, 0, 1, 10, -1);
INSERT INTO `muun` VALUES (8406, 3, 9, 1, 15, 8407);
INSERT INTO `muun` VALUES (8407, 3, 9, 1, 15, -1);
INSERT INTO `muun` VALUES (8408, 2, 71, 1, 5, 8409);
INSERT INTO `muun` VALUES (8409, 2, 71, 1, 5, -1);
INSERT INTO `muun` VALUES (8410, 1, 2, 1, 5, 8411);
INSERT INTO `muun` VALUES (8411, 1, 2, 1, 5, -1);
INSERT INTO `muun` VALUES (8415, 4, 3, 1, 20, 8416);
INSERT INTO `muun` VALUES (8416, 4, 3, 1, 20, -1);
INSERT INTO `muun` VALUES (8417, 2, 4, 1, 5, 8418);
INSERT INTO `muun` VALUES (8418, 2, 4, 1, 5, -1);
INSERT INTO `muun` VALUES (8419, 2, 5, 1, 5, 8420);
INSERT INTO `muun` VALUES (8420, 2, 5, 1, 5, -1);
INSERT INTO `muun` VALUES (8421, 4, 14, 1, 20, 8422);
INSERT INTO `muun` VALUES (8422, 4, 14, 1, 20, -1);
INSERT INTO `muun` VALUES (8423, 3, 11, 1, 15, 8424);
INSERT INTO `muun` VALUES (8424, 3, 11, 1, 15, -1);
INSERT INTO `muun` VALUES (8425, 4, 6, 1, 20, 8426);
INSERT INTO `muun` VALUES (8426, 4, 6, 1, 20, -1);
INSERT INTO `muun` VALUES (8427, 2, 7, 1, 5, 8428);
INSERT INTO `muun` VALUES (8428, 2, 7, 1, 5, -1);
INSERT INTO `muun` VALUES (8429, 2, 8, 1, 5, 8430);
INSERT INTO `muun` VALUES (8430, 2, 8, 1, 5, -1);
INSERT INTO `muun` VALUES (8433, 4, 10, 1, 20, 8434);
INSERT INTO `muun` VALUES (8434, 4, 10, 1, 20, -1);
INSERT INTO `muun` VALUES (8435, 2, 54, 3, 2, 8436);
INSERT INTO `muun` VALUES (8436, 2, 54, 3, 2, -1);
INSERT INTO `muun` VALUES (8437, 2, 12, 1, 5, 8438);
INSERT INTO `muun` VALUES (8438, 2, 12, 1, 5, -1);
INSERT INTO `muun` VALUES (8444, 2, 15, 1, 5, 8445);
INSERT INTO `muun` VALUES (8445, 2, 15, 1, 5, -1);
INSERT INTO `muun` VALUES (8446, 2, 39, 3, 2, 8447);
INSERT INTO `muun` VALUES (8447, 2, 39, 3, 2, -1);
INSERT INTO `muun` VALUES (8448, 4, 40, 1, 20, 8449);
INSERT INTO `muun` VALUES (8449, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8450, 2, 23, 1, 5, 8451);
INSERT INTO `muun` VALUES (8451, 2, 23, 1, 5, -1);
INSERT INTO `muun` VALUES (8452, 2, 25, 3, 2, 8453);
INSERT INTO `muun` VALUES (8453, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8454, 4, 42, 1, 20, 8455);
INSERT INTO `muun` VALUES (8455, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8456, 2, 19, 1, 5, 8457);
INSERT INTO `muun` VALUES (8457, 2, 19, 1, 5, -1);
INSERT INTO `muun` VALUES (8458, 2, 25, 3, 2, 8459);
INSERT INTO `muun` VALUES (8459, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8462, 4, 50, 1, 20, 8463);
INSERT INTO `muun` VALUES (8463, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8464, 2, 21, 1, 5, 8465);
INSERT INTO `muun` VALUES (8465, 2, 21, 1, 5, -1);
INSERT INTO `muun` VALUES (8466, 2, 22, 3, 2, 8467);
INSERT INTO `muun` VALUES (8467, 2, 22, 3, 2, -1);
INSERT INTO `muun` VALUES (8470, 4, 26, 1, 20, 8471);
INSERT INTO `muun` VALUES (8471, 4, 26, 1, 20, -1);
INSERT INTO `muun` VALUES (8472, 2, 27, 1, 5, 8473);
INSERT INTO `muun` VALUES (8473, 2, 27, 1, 5, -1);
INSERT INTO `muun` VALUES (8474, 2, 28, 3, 2, 8475);
INSERT INTO `muun` VALUES (8475, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8476, 3, 29, 1, 10, 8477);
INSERT INTO `muun` VALUES (8477, 3, 29, 1, 10, -1);
INSERT INTO `muun` VALUES (8478, 3, 30, 1, 10, 8479);
INSERT INTO `muun` VALUES (8479, 3, 30, 1, 10, -1);
INSERT INTO `muun` VALUES (8480, 3, 31, 1, 10, 8481);
INSERT INTO `muun` VALUES (8481, 3, 31, 1, 10, -1);
INSERT INTO `muun` VALUES (8482, 4, 32, 1, 20, 8483);
INSERT INTO `muun` VALUES (8483, 4, 32, 1, 20, -1);
INSERT INTO `muun` VALUES (8484, 2, 33, 1, 5, 8485);
INSERT INTO `muun` VALUES (8485, 2, 33, 1, 5, -1);
INSERT INTO `muun` VALUES (8486, 2, 28, 3, 2, 8487);
INSERT INTO `muun` VALUES (8487, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8488, 4, 40, 1, 20, 8489);
INSERT INTO `muun` VALUES (8489, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8490, 2, 35, 1, 5, 8491);
INSERT INTO `muun` VALUES (8491, 2, 35, 1, 5, -1);
INSERT INTO `muun` VALUES (8492, 2, 25, 3, 2, 8493);
INSERT INTO `muun` VALUES (8493, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8494, 2, 28, 3, 2, 8495);
INSERT INTO `muun` VALUES (8495, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8496, 2, 36, 1, 5, 8497);
INSERT INTO `muun` VALUES (8497, 2, 36, 1, 5, -1);
INSERT INTO `muun` VALUES (8498, 4, 42, 1, 20, 8499);
INSERT INTO `muun` VALUES (8499, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8500, 4, 32, 1, 20, 8501);
INSERT INTO `muun` VALUES (8501, 4, 32, 1, 20, -1);
INSERT INTO `muun` VALUES (8502, 2, 37, 1, 5, 8503);
INSERT INTO `muun` VALUES (8503, 2, 37, 1, 5, -1);
INSERT INTO `muun` VALUES (8504, 2, 28, 3, 3, 8505);
INSERT INTO `muun` VALUES (8505, 2, 28, 3, 3, -1);
INSERT INTO `muun` VALUES (8506, 4, 40, 1, 20, 8507);
INSERT INTO `muun` VALUES (8507, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8508, 2, 38, 1, 5, 8509);
INSERT INTO `muun` VALUES (8509, 2, 38, 1, 5, -1);
INSERT INTO `muun` VALUES (8510, 2, 39, 3, 2, 8511);
INSERT INTO `muun` VALUES (8511, 2, 39, 3, 2, -1);
INSERT INTO `muun` VALUES (8512, 4, 42, 1, 20, 8513);
INSERT INTO `muun` VALUES (8513, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8514, 2, 44, 1, 5, 8515);
INSERT INTO `muun` VALUES (8515, 2, 44, 1, 5, -1);
INSERT INTO `muun` VALUES (8516, 2, 34, 3, 2, 8517);
INSERT INTO `muun` VALUES (8517, 2, 34, 3, 2, -1);
INSERT INTO `muun` VALUES (8518, 3, 45, 1, 15, 8519);
INSERT INTO `muun` VALUES (8519, 3, 45, 1, 15, -1);
INSERT INTO `muun` VALUES (8520, 3, 46, 1, 15, 8521);
INSERT INTO `muun` VALUES (8521, 3, 46, 1, 15, -1);
INSERT INTO `muun` VALUES (8522, 3, 47, 1, 15, 8523);
INSERT INTO `muun` VALUES (8523, 3, 47, 1, 15, -1);
INSERT INTO `muun` VALUES (8524, 4, 40, 1, 20, 8525);
INSERT INTO `muun` VALUES (8525, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8526, 2, 48, 1, 5, 8527);
INSERT INTO `muun` VALUES (8527, 2, 48, 1, 5, -1);
INSERT INTO `muun` VALUES (8528, 2, 25, 3, 3, 8529);
INSERT INTO `muun` VALUES (8529, 2, 25, 3, 3, -1);
INSERT INTO `muun` VALUES (8530, 4, 50, 1, 20, 8531);
INSERT INTO `muun` VALUES (8531, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8532, 2, 49, 1, 5, 8533);
INSERT INTO `muun` VALUES (8533, 2, 49, 1, 5, -1);
INSERT INTO `muun` VALUES (8534, 2, 39, 3, 2, 8535);
INSERT INTO `muun` VALUES (8535, 2, 39, 3, 2, -1);
INSERT INTO `muun` VALUES (8536, 4, 40, 1, 20, 8537);
INSERT INTO `muun` VALUES (8537, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8538, 2, 52, 1, 5, 8539);
INSERT INTO `muun` VALUES (8539, 2, 52, 1, 5, -1);
INSERT INTO `muun` VALUES (8540, 2, 28, 3, 2, 8541);
INSERT INTO `muun` VALUES (8541, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8601, 4, 50, 1, 20, 8602);
INSERT INTO `muun` VALUES (8602, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8603, 2, 53, 1, 5, 8604);
INSERT INTO `muun` VALUES (8604, 2, 53, 1, 5, -1);
INSERT INTO `muun` VALUES (8605, 2, 54, 3, 2, 8606);
INSERT INTO `muun` VALUES (8606, 2, 54, 3, 2, -1);
INSERT INTO `muun` VALUES (8607, 4, 50, 1, 20, 8608);
INSERT INTO `muun` VALUES (8608, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8609, 2, 55, 1, 5, 8610);
INSERT INTO `muun` VALUES (8610, 2, 55, 1, 5, -1);
INSERT INTO `muun` VALUES (8611, 2, 25, 3, 2, 8612);
INSERT INTO `muun` VALUES (8612, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8613, 4, 42, 1, 20, 8614);
INSERT INTO `muun` VALUES (8614, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8615, 2, 57, 1, 5, 8616);
INSERT INTO `muun` VALUES (8616, 2, 57, 1, 5, -1);
INSERT INTO `muun` VALUES (8617, 2, 39, 3, 2, 8618);
INSERT INTO `muun` VALUES (8618, 2, 39, 3, 2, -1);
INSERT INTO `muun` VALUES (8619, 4, 40, 1, 20, 8620);
INSERT INTO `muun` VALUES (8620, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8621, 2, 48, 1, 5, 8622);
INSERT INTO `muun` VALUES (8622, 2, 48, 1, 5, -1);
INSERT INTO `muun` VALUES (8623, 2, 25, 3, 2, 8624);
INSERT INTO `muun` VALUES (8624, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8625, 4, 42, 1, 20, 8626);
INSERT INTO `muun` VALUES (8626, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8627, 2, 44, 1, 5, 8628);
INSERT INTO `muun` VALUES (8628, 2, 44, 1, 5, -1);
INSERT INTO `muun` VALUES (8629, 2, 34, 3, 2, 8630);
INSERT INTO `muun` VALUES (8630, 2, 34, 3, 2, -1);
INSERT INTO `muun` VALUES (8631, 4, 50, 1, 20, 8632);
INSERT INTO `muun` VALUES (8632, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8633, 2, 36, 1, 5, 8634);
INSERT INTO `muun` VALUES (8634, 2, 36, 1, 5, -1);
INSERT INTO `muun` VALUES (8635, 2, 28, 3, 2, 8636);
INSERT INTO `muun` VALUES (8636, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8637, 4, 42, 1, 20, 8638);
INSERT INTO `muun` VALUES (8638, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8639, 2, 23, 1, 5, 8640);
INSERT INTO `muun` VALUES (8640, 2, 23, 1, 5, -1);
INSERT INTO `muun` VALUES (8641, 2, 25, 3, 2, 8642);
INSERT INTO `muun` VALUES (8642, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8643, 4, 50, 1, 20, 8644);
INSERT INTO `muun` VALUES (8644, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8645, 2, 7, 1, 5, 8646);
INSERT INTO `muun` VALUES (8646, 2, 7, 1, 5, -1);
INSERT INTO `muun` VALUES (8647, 2, 54, 3, 2, 8648);
INSERT INTO `muun` VALUES (8648, 2, 54, 3, 2, -1);
INSERT INTO `muun` VALUES (8649, 4, 50, 1, 20, 8650);
INSERT INTO `muun` VALUES (8650, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8651, 2, 23, 1, 5, 8652);
INSERT INTO `muun` VALUES (8652, 2, 23, 1, 5, -1);
INSERT INTO `muun` VALUES (8653, 2, 39, 3, 2, 8654);
INSERT INTO `muun` VALUES (8654, 2, 39, 3, 2, -1);
INSERT INTO `muun` VALUES (8655, 2, 7, 1, 5, -1);
INSERT INTO `muun` VALUES (8656, 2, 54, 3, 2, -1);
INSERT INTO `muun` VALUES (8657, 4, 60, 1, 20, -1);
INSERT INTO `muun` VALUES (8669, 4, 56, 2, 250, -1);
INSERT INTO `muun` VALUES (8672, 4, 42, 1, 20, 8673);
INSERT INTO `muun` VALUES (8673, 4, 43, 1, 20, -1);
INSERT INTO `muun` VALUES (8674, 2, 59, 1, 5, 8675);
INSERT INTO `muun` VALUES (8675, 2, 59, 1, 5, -1);
INSERT INTO `muun` VALUES (8676, 2, 54, 3, 2, 8677);
INSERT INTO `muun` VALUES (8677, 2, 54, 3, 2, -1);
INSERT INTO `muun` VALUES (8678, 4, 50, 1, 20, 8679);
INSERT INTO `muun` VALUES (8679, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8680, 2, 38, 1, 5, 8681);
INSERT INTO `muun` VALUES (8681, 2, 38, 1, 5, -1);
INSERT INTO `muun` VALUES (8682, 2, 25, 3, 2, 8683);
INSERT INTO `muun` VALUES (8683, 2, 25, 3, 2, -1);
INSERT INTO `muun` VALUES (8684, 4, 40, 1, 20, 8685);
INSERT INTO `muun` VALUES (8685, 4, 41, 1, 20, -1);
INSERT INTO `muun` VALUES (8686, 2, 49, 1, 5, 8687);
INSERT INTO `muun` VALUES (8687, 2, 49, 1, 5, -1);
INSERT INTO `muun` VALUES (8688, 2, 34, 3, 2, 8689);
INSERT INTO `muun` VALUES (8689, 2, 34, 3, 2, -1);
INSERT INTO `muun` VALUES (8690, 4, 50, 1, 20, 8691);
INSERT INTO `muun` VALUES (8691, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8692, 2, 57, 1, 5, 8693);
INSERT INTO `muun` VALUES (8693, 2, 57, 1, 5, -1);
INSERT INTO `muun` VALUES (8694, 2, 28, 3, 2, 8695);
INSERT INTO `muun` VALUES (8695, 2, 28, 3, 2, -1);
INSERT INTO `muun` VALUES (8328, 3, 73, 3, 2, 8329);
INSERT INTO `muun` VALUES (8329, 3, 73, 3, 2, -1);
INSERT INTO `muun` VALUES (8330, 4, 69, 1, 20, -1);
INSERT INTO `muun` VALUES (8331, 4, 72, 1, 20, -1);
INSERT INTO `muun` VALUES (8332, 4, 50, 1, 20, 8333);
INSERT INTO `muun` VALUES (8333, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8334, 4, 50, 1, 20, 8335);
INSERT INTO `muun` VALUES (8335, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8336, 4, 50, 1, 20, 8337);
INSERT INTO `muun` VALUES (8337, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8338, 4, 50, 1, 20, 8339);
INSERT INTO `muun` VALUES (8339, 4, 51, 1, 20, -1);
INSERT INTO `muun` VALUES (8340, 4, 50, 1, 20, 8341);
INSERT INTO `muun` VALUES (8341, 4, 51, 1, 20, -1);

-- ----------------------------
-- Table structure for muun_energy
-- ----------------------------
DROP TABLE IF EXISTS `muun_energy`;
CREATE TABLE `muun_energy`  (
  `rating` tinyint(3) UNSIGNED NOT NULL,
  `level_1` tinyint(3) UNSIGNED NOT NULL,
  `level_2` tinyint(3) UNSIGNED NOT NULL,
  `level_3` tinyint(3) UNSIGNED NOT NULL,
  `level_4` tinyint(3) UNSIGNED NOT NULL,
  `level_5` tinyint(3) UNSIGNED NOT NULL,
  `evolution` tinyint(3) UNSIGNED NOT NULL,
  PRIMARY KEY (`rating`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of muun_energy
-- ----------------------------
INSERT INTO `muun_energy` VALUES (1, 3, 6, 0, 0, 0, 8);
INSERT INTO `muun_energy` VALUES (2, 6, 12, 18, 0, 0, 24);
INSERT INTO `muun_energy` VALUES (3, 9, 18, 27, 36, 0, 48);
INSERT INTO `muun_energy` VALUES (4, 12, 24, 36, 48, 60, 80);

-- ----------------------------
-- Table structure for muun_exchange
-- ----------------------------
DROP TABLE IF EXISTS `muun_exchange`;
CREATE TABLE `muun_exchange`  (
  `id` tinyint(3) UNSIGNED NOT NULL,
  `count` tinyint(3) UNSIGNED NULL DEFAULT NULL,
  `muun` smallint(5) UNSIGNED NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of muun_exchange
-- ----------------------------
INSERT INTO `muun_exchange` VALUES (0, 3, 8192);
INSERT INTO `muun_exchange` VALUES (1, 3, 8194);
INSERT INTO `muun_exchange` VALUES (2, 3, 8196);
INSERT INTO `muun_exchange` VALUES (3, 3, 8198);
INSERT INTO `muun_exchange` VALUES (4, 3, 8200);
INSERT INTO `muun_exchange` VALUES (5, 3, 8202);
INSERT INTO `muun_exchange` VALUES (6, 3, 8204);
INSERT INTO `muun_exchange` VALUES (7, 3, 8206);
INSERT INTO `muun_exchange` VALUES (8, 3, 8208);
INSERT INTO `muun_exchange` VALUES (9, 3, 8210);
INSERT INTO `muun_exchange` VALUES (10, 3, 8212);
INSERT INTO `muun_exchange` VALUES (11, 3, 8214);
INSERT INTO `muun_exchange` VALUES (12, 3, 8216);
INSERT INTO `muun_exchange` VALUES (13, 3, 8218);
INSERT INTO `muun_exchange` VALUES (14, 3, 8220);
INSERT INTO `muun_exchange` VALUES (15, 3, 8224);
INSERT INTO `muun_exchange` VALUES (16, 3, 8226);
INSERT INTO `muun_exchange` VALUES (17, 3, 8228);
INSERT INTO `muun_exchange` VALUES (18, 3, 8230);
INSERT INTO `muun_exchange` VALUES (19, 3, 8404);
INSERT INTO `muun_exchange` VALUES (20, 3, 8406);
INSERT INTO `muun_exchange` VALUES (21, 3, 8408);
INSERT INTO `muun_exchange` VALUES (22, 3, 8410);
INSERT INTO `muun_exchange` VALUES (23, 3, 8415);
INSERT INTO `muun_exchange` VALUES (24, 3, 8417);
INSERT INTO `muun_exchange` VALUES (25, 3, 8419);
INSERT INTO `muun_exchange` VALUES (26, 3, 8421);
INSERT INTO `muun_exchange` VALUES (27, 3, 8423);
INSERT INTO `muun_exchange` VALUES (28, 3, 8425);
INSERT INTO `muun_exchange` VALUES (29, 3, 8427);
INSERT INTO `muun_exchange` VALUES (30, 3, 8429);
INSERT INTO `muun_exchange` VALUES (31, 3, 8433);
INSERT INTO `muun_exchange` VALUES (32, 3, 8435);
INSERT INTO `muun_exchange` VALUES (33, 3, 8437);
INSERT INTO `muun_exchange` VALUES (34, 3, 8444);
INSERT INTO `muun_exchange` VALUES (35, 3, 8446);
INSERT INTO `muun_exchange` VALUES (36, 3, 8448);
INSERT INTO `muun_exchange` VALUES (37, 3, 8450);
INSERT INTO `muun_exchange` VALUES (38, 3, 8452);
INSERT INTO `muun_exchange` VALUES (39, 3, 8454);
INSERT INTO `muun_exchange` VALUES (40, 3, 8456);
INSERT INTO `muun_exchange` VALUES (41, 3, 8458);
INSERT INTO `muun_exchange` VALUES (42, 3, 8462);
INSERT INTO `muun_exchange` VALUES (43, 3, 8464);
INSERT INTO `muun_exchange` VALUES (44, 3, 8466);
INSERT INTO `muun_exchange` VALUES (45, 3, 8470);
INSERT INTO `muun_exchange` VALUES (46, 3, 8472);
INSERT INTO `muun_exchange` VALUES (47, 3, 8474);
INSERT INTO `muun_exchange` VALUES (48, 3, 8482);
INSERT INTO `muun_exchange` VALUES (49, 3, 8484);
INSERT INTO `muun_exchange` VALUES (50, 3, 8486);
INSERT INTO `muun_exchange` VALUES (51, 3, 8488);
INSERT INTO `muun_exchange` VALUES (52, 3, 8490);
INSERT INTO `muun_exchange` VALUES (53, 3, 8492);
INSERT INTO `muun_exchange` VALUES (54, 3, 8494);
INSERT INTO `muun_exchange` VALUES (55, 3, 8496);
INSERT INTO `muun_exchange` VALUES (56, 3, 8498);
INSERT INTO `muun_exchange` VALUES (57, 3, 8500);
INSERT INTO `muun_exchange` VALUES (58, 3, 8502);
INSERT INTO `muun_exchange` VALUES (59, 3, 8504);
INSERT INTO `muun_exchange` VALUES (60, 3, 8506);
INSERT INTO `muun_exchange` VALUES (61, 3, 8508);
INSERT INTO `muun_exchange` VALUES (62, 3, 8510);
INSERT INTO `muun_exchange` VALUES (63, 3, 8512);
INSERT INTO `muun_exchange` VALUES (64, 3, 8514);
INSERT INTO `muun_exchange` VALUES (65, 3, 8516);
INSERT INTO `muun_exchange` VALUES (66, 3, 8518);
INSERT INTO `muun_exchange` VALUES (67, 3, 8520);
INSERT INTO `muun_exchange` VALUES (68, 3, 8522);
INSERT INTO `muun_exchange` VALUES (69, 3, 8524);
INSERT INTO `muun_exchange` VALUES (70, 3, 8526);
INSERT INTO `muun_exchange` VALUES (71, 3, 8528);
INSERT INTO `muun_exchange` VALUES (72, 3, 8530);
INSERT INTO `muun_exchange` VALUES (73, 3, 8532);
INSERT INTO `muun_exchange` VALUES (74, 3, 8534);
INSERT INTO `muun_exchange` VALUES (75, 3, 8536);
INSERT INTO `muun_exchange` VALUES (76, 3, 8538);
INSERT INTO `muun_exchange` VALUES (77, 3, 8540);
INSERT INTO `muun_exchange` VALUES (78, 3, 8601);
INSERT INTO `muun_exchange` VALUES (79, 3, 8603);
INSERT INTO `muun_exchange` VALUES (80, 3, 8605);
INSERT INTO `muun_exchange` VALUES (81, 3, 8607);
INSERT INTO `muun_exchange` VALUES (82, 3, 8609);
INSERT INTO `muun_exchange` VALUES (83, 3, 8611);
INSERT INTO `muun_exchange` VALUES (84, 3, 8613);
INSERT INTO `muun_exchange` VALUES (85, 3, 8615);
INSERT INTO `muun_exchange` VALUES (86, 3, 8617);
INSERT INTO `muun_exchange` VALUES (87, 3, 8619);
INSERT INTO `muun_exchange` VALUES (88, 3, 8621);
INSERT INTO `muun_exchange` VALUES (89, 3, 8623);
INSERT INTO `muun_exchange` VALUES (90, 3, 8625);
INSERT INTO `muun_exchange` VALUES (91, 3, 8627);
INSERT INTO `muun_exchange` VALUES (92, 3, 8629);
INSERT INTO `muun_exchange` VALUES (93, 3, 8631);
INSERT INTO `muun_exchange` VALUES (94, 3, 8633);
INSERT INTO `muun_exchange` VALUES (95, 3, 8635);
INSERT INTO `muun_exchange` VALUES (96, 3, 8637);
INSERT INTO `muun_exchange` VALUES (97, 3, 8639);
INSERT INTO `muun_exchange` VALUES (98, 3, 8641);
INSERT INTO `muun_exchange` VALUES (99, 3, 8643);
INSERT INTO `muun_exchange` VALUES (100, 3, 8645);
INSERT INTO `muun_exchange` VALUES (101, 3, 8647);
INSERT INTO `muun_exchange` VALUES (102, 3, 8649);
INSERT INTO `muun_exchange` VALUES (103, 3, 8651);
INSERT INTO `muun_exchange` VALUES (104, 3, 8653);
INSERT INTO `muun_exchange` VALUES (105, 3, 8672);
INSERT INTO `muun_exchange` VALUES (106, 3, 8674);
INSERT INTO `muun_exchange` VALUES (107, 3, 8676);
INSERT INTO `muun_exchange` VALUES (108, 3, 8678);
INSERT INTO `muun_exchange` VALUES (109, 3, 8680);
INSERT INTO `muun_exchange` VALUES (110, 3, 8682);
INSERT INTO `muun_exchange` VALUES (111, 3, 8684);
INSERT INTO `muun_exchange` VALUES (112, 3, 8686);
INSERT INTO `muun_exchange` VALUES (113, 3, 8688);
INSERT INTO `muun_exchange` VALUES (114, 3, 8690);
INSERT INTO `muun_exchange` VALUES (115, 3, 8692);
INSERT INTO `muun_exchange` VALUES (116, 3, 8694);

-- ----------------------------
-- Table structure for muun_option
-- ----------------------------
DROP TABLE IF EXISTS `muun_option`;
CREATE TABLE `muun_option`  (
  `index` tinyint(3) UNSIGNED NOT NULL,
  `option_value_1` int(11) NOT NULL DEFAULT 0,
  `option_value_2` int(11) NOT NULL DEFAULT 0,
  `option_value_3` int(11) NOT NULL DEFAULT 0,
  `option_value_4` int(11) NOT NULL DEFAULT 0,
  `option_value_5` int(11) NOT NULL DEFAULT 0,
  `max_option_value` int(11) NOT NULL DEFAULT 0,
  `world` smallint(6) NOT NULL DEFAULT -1,
  `play_time` int(11) NOT NULL DEFAULT -1,
  `day_of_week` int(11) NOT NULL DEFAULT -1,
  `start_hour` int(11) NOT NULL DEFAULT -1,
  `end_hour` int(11) NOT NULL DEFAULT -1,
  `min_level` int(11) NOT NULL DEFAULT -1,
  `max_level` int(11) NOT NULL DEFAULT -1,
  `min_master_level` int(11) NOT NULL DEFAULT -1,
  `max_master_level` int(11) NOT NULL DEFAULT -1,
  PRIMARY KEY (`index`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of muun_option
-- ----------------------------
INSERT INTO `muun_option` VALUES (0, 22, 26, 30, 34, 0, 38, -1, -1, -1, 18, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (1, 2, 4, 6, 0, 0, 8, 56, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (2, 2, 4, 0, 0, 0, 6, -1, -1, -1, -1, -1, 1, 200, -1, -1);
INSERT INTO `muun_option` VALUES (3, 14, 16, 18, 20, 22, 24, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (4, 4, 6, 8, 0, 0, 10, -1, -1, -1, 1, 12, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (5, 2, 4, 6, 0, 0, 8, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (6, 14, 16, 18, 20, 22, 24, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (7, 2, 4, 6, 0, 0, 8, -1, -1, -1, -1, -1, -1, -1, 201, -1);
INSERT INTO `muun_option` VALUES (8, 2, 4, 6, 0, 0, 8, -1, -1, -1, -1, -1, 1, 200, -1, -1);
INSERT INTO `muun_option` VALUES (9, 9, 12, 15, 18, 0, 21, -1, -1, 14, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (10, 23, 26, 29, 32, 35, 38, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (11, 6, 8, 10, 12, 0, 14, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (12, 2, 6, 10, 0, 0, 14, -1, -1, -1, -1, -1, 1, 200, -1, -1);
INSERT INTO `muun_option` VALUES (13, 4, 6, 8, 0, 0, 10, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (14, 14, 16, 18, 20, 22, 24, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (15, 2, 4, 6, 0, 0, 8, -1, -1, -1, -1, -1, 1, 300, -1, -1);
INSERT INTO `muun_option` VALUES (16, 4, 6, 8, 0, 0, 10, -1, -1, 14, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (17, 14, 16, 18, 20, 22, 24, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (18, 4, 6, 8, 0, 0, 10, -1, -1, -1, -1, -1, -1, -1, 201, -1);
INSERT INTO `muun_option` VALUES (19, 2, 4, 6, 0, 0, 8, -1, -1, 14, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (20, 9, 11, 13, 15, 17, 19, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (21, 2, 4, 6, 0, 0, 8, -1, -1, 14, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (22, 4, 6, 8, 0, 0, 10, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (23, 2, 4, 6, 0, 0, 8, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (24, 14, 16, 18, 20, 22, 24, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (25, 4, 6, 8, 0, 0, 10, -1, -1, -1, -1, -1, -1, -1, 201, -1);
INSERT INTO `muun_option` VALUES (26, 14, 16, 18, 20, 22, 24, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (27, 2, 4, 6, 0, 0, 8, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (28, 4, 6, 8, 0, 0, 10, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (29, 14, 16, 18, 20, 0, 22, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (30, 9, 11, 13, 15, 0, 17, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (31, 23, 26, 29, 32, 0, 35, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (32, 9, 11, 13, 15, 17, 19, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (33, 2, 4, 6, 0, 0, 8, -1, -1, -1, -1, -1, -1, -1, 201, -1);
INSERT INTO `muun_option` VALUES (34, 4, 6, 8, 0, 0, 10, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (35, 2, 4, 6, 0, 0, 8, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (36, 2, 4, 6, 0, 0, 8, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (37, 2, 4, 6, 0, 0, 8, -1, 300, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (38, 1, 2, 3, 0, 0, 4, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (39, 4, 6, 8, 0, 0, 10, -1, 300, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (40, 14, 16, 18, 20, 22, 24, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (41, 14, 16, 18, 20, 22, 24, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (42, 14, 16, 18, 20, 22, 24, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (43, 14, 16, 18, 20, 22, 24, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (44, 2, 6, 10, 0, 0, 14, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (45, 6, 8, 10, 12, 0, 14, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (46, 3, 5, 7, 9, 0, 11, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (47, 6, 8, 10, 12, 0, 14, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (48, 2, 4, 6, 0, 0, 8, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (49, 2, 4, 6, 0, 0, 8, -1, -1, -1, -1, -1, -1, -1, 201, -1);
INSERT INTO `muun_option` VALUES (50, 9, 11, 13, 15, 17, 19, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (51, 9, 11, 13, 15, 17, 19, -1, -1, 121, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (52, 2, 4, 6, 0, 0, 8, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (53, 2, 4, 6, 0, 0, 8, -1, -1, -1, -1, -1, 201, -1, -1, -1);
INSERT INTO `muun_option` VALUES (54, 4, 6, 8, 0, 0, 10, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (55, 1, 2, 3, 0, 0, 4, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (56, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (57, 2, 4, 6, 0, 0, 8, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (58, 2, 6, 10, 0, 0, 14, -1, -1, -1, 18, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (59, 2, 6, 10, 0, 0, 14, -1, -1, 113, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (60, 9, 11, 13, 15, 17, 19, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (61, 2, 6, 10, 0, 0, 14, -1, -1, -1, 13, 24, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (68, 3, 5, 7, 9, 0, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (69, 15, 15, 15, 15, 15, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (70, 2, 4, 6, 0, 0, 8, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (71, 2, 6, 10, 0, 0, 14, -1, -1, 30, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (72, 10, 10, 10, 10, 10, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1);
INSERT INTO `muun_option` VALUES (73, 25, 27, 29, 31, 0, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1);

SET FOREIGN_KEY_CHECKS = 1;
