/*
Navicat MySQL Data Transfer

Source Server         : db
Source Server Version : 80018
Source Host           : localhost:3306
Source Database       : dbo_log

Target Server Type    : MYSQL
Target Server Version : 80018
File Encoding         : 65001

Date: 2020-04-21 14:06:37
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for auctionhouse_log
-- ----------------------------
DROP TABLE IF EXISTS `auctionhouse_log`;
CREATE TABLE `auctionhouse_log` (
  `Seller` int(15) unsigned DEFAULT NULL,
  `Buyer` int(15) unsigned DEFAULT NULL,
  `Price` int(15) unsigned DEFAULT NULL,
  `ItemTblidx` int(15) unsigned DEFAULT NULL,
  `ItemID` bigint(30) unsigned DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auth_login_log
-- ----------------------------
DROP TABLE IF EXISTS `auth_login_log`;
CREATE TABLE `auth_login_log` (
  `AccountID` int(15) unsigned NOT NULL,
  `IP` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `AccountID` (`AccountID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for budokai
-- ----------------------------
DROP TABLE IF EXISTS `budokai`;
CREATE TABLE `budokai` (
  `SeasonCount` int(6) unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of budokais. 0 = 1, 10 = 11 ...',
  `DefaultOpenTime` int(15) unsigned DEFAULT '0',
  `RankPointInitialized` bit(1) DEFAULT b'0',
  `StateData_State` tinyint(3) unsigned DEFAULT '0' COMMENT 'Budokai State',
  `StateData_NextStepTime` int(15) unsigned DEFAULT '0' COMMENT 'budokai next step time',
  `IndividualStateData_State` tinyint(3) unsigned DEFAULT '0' COMMENT 'Solo match state',
  `IndividualStateData_NextStepTime` int(15) unsigned DEFAULT '0' COMMENT 'solo match next step time',
  `TeamStateData_State` tinyint(3) unsigned DEFAULT '0' COMMENT 'team match state',
  `TeamStateData_NextStepTime` int(15) unsigned DEFAULT '0' COMMENT 'Team match next state time'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for budokai_winners
-- ----------------------------
DROP TABLE IF EXISTS `budokai_winners`;
CREATE TABLE `budokai_winners` (
  `BudokaiNumber` int(10) unsigned DEFAULT '0',
  `Type` tinyint(1) DEFAULT '0' COMMENT '0 = junior, 1 = adult',
  `MatchType` tinyint(1) DEFAULT '0' COMMENT '0 = individual, 1 = team',
  `WinnerCharID1` int(15) unsigned DEFAULT '0',
  `WinnerCharID2` int(15) unsigned DEFAULT '0',
  `WinnerCharID3` int(15) unsigned DEFAULT '0',
  `WinnerCharID4` int(15) unsigned DEFAULT '0',
  `WinnerCharID5` int(15) unsigned DEFAULT '0',
  `Date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for change_char_name
-- ----------------------------
DROP TABLE IF EXISTS `change_char_name`;
CREATE TABLE `change_char_name` (
  `CharID` int(15) unsigned NOT NULL,
  `Name` varchar(17) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `newName` varchar(17) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for character_delete_log
-- ----------------------------
DROP TABLE IF EXISTS `character_delete_log`;
CREATE TABLE `character_delete_log` (
  `AccountID` int(15) unsigned NOT NULL,
  `CharID` int(15) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dynamic_field_count
-- ----------------------------
DROP TABLE IF EXISTS `dynamic_field_count`;
CREATE TABLE `dynamic_field_count` (
  `serverIndex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`serverIndex`),
  UNIQUE KEY `serverIndex` (`serverIndex`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='When the server restarts it will save & load the progress from this table.\r\nSince it takes many days to reach max count it is very important.';

-- ----------------------------
-- Table structure for founder_log
-- ----------------------------
DROP TABLE IF EXISTS `founder_log`;
CREATE TABLE `founder_log` (
  `Username` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `forumname` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `date` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for gm_log
-- ----------------------------
DROP TABLE IF EXISTS `gm_log`;
CREATE TABLE `gm_log` (
  `CharID` int(15) unsigned NOT NULL,
  `LogType` int(3) unsigned DEFAULT NULL,
  `String` text CHARACTER SET utf8 COLLATE utf8_general_ci,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for guild_name_change_log
-- ----------------------------
DROP TABLE IF EXISTS `guild_name_change_log`;
CREATE TABLE `guild_name_change_log` (
  `key` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `GuildID` int(15) unsigned DEFAULT NULL,
  `CurrentName` varchar(25) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `NewName` varchar(25) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`key`)
) ENGINE=InnoDB AUTO_INCREMENT=47 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for item_upgrade_log
-- ----------------------------
DROP TABLE IF EXISTS `item_upgrade_log`;
CREATE TABLE `item_upgrade_log` (
  `charId` int(15) unsigned NOT NULL,
  `IsSuccess` bit(1) NOT NULL,
  `itemId` bigint(20) unsigned NOT NULL,
  `itemTblidx` int(15) unsigned NOT NULL,
  `grade` int(3) NOT NULL,
  `newGrade` int(3) NOT NULL,
  `StoneItemId` bigint(20) unsigned NOT NULL,
  `StoneItemTblidx` int(15) unsigned NOT NULL,
  `CoreItemUse` bit(1) NOT NULL,
  `coreItemId` bigint(20) unsigned NOT NULL DEFAULT '0',
  `coreItemTblidx` int(15) unsigned NOT NULL,
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for mail_deleted
-- ----------------------------
DROP TABLE IF EXISTS `mail_deleted`;
CREATE TABLE `mail_deleted` (
  `id` int(10) unsigned NOT NULL,
  `CharID` int(10) unsigned NOT NULL,
  `SenderType` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `MailType` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `TextSize` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Text` varchar(127) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `Zenny` int(10) unsigned NOT NULL DEFAULT '0',
  `itemId` bigint(20) unsigned NOT NULL DEFAULT '0',
  `TargetName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `FromName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `IsAccept` tinyint(1) NOT NULL DEFAULT '0',
  `IsLock` tinyint(1) NOT NULL DEFAULT '0',
  `IsRead` tinyint(1) NOT NULL DEFAULT '0',
  `CreateTime` bigint(20) unsigned DEFAULT NULL,
  `EndTime` bigint(20) unsigned DEFAULT NULL,
  `RemainDay` tinyint(2) unsigned NOT NULL DEFAULT '1',
  `year` int(4) unsigned NOT NULL,
  `month` tinyint(2) unsigned NOT NULL,
  `day` tinyint(2) unsigned NOT NULL,
  `hour` tinyint(2) unsigned NOT NULL,
  `minute` tinyint(2) unsigned NOT NULL,
  `second` tinyint(2) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`,`CharID`) USING BTREE,
  UNIQUE KEY `id_2` (`id`) USING BTREE,
  KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for mute_log
-- ----------------------------
DROP TABLE IF EXISTS `mute_log`;
CREATE TABLE `mute_log` (
  `CharID` int(15) unsigned NOT NULL,
  `GmAccountID` int(15) unsigned DEFAULT NULL,
  `DurationInMinutes` int(15) unsigned DEFAULT NULL,
  `Reason` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `muteUntil` bigint(30) unsigned DEFAULT NULL,
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`CharID`),
  UNIQUE KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for privateshoplogs
-- ----------------------------
DROP TABLE IF EXISTS `privateshoplogs`;
CREATE TABLE `privateshoplogs` (
  `key` int(15) unsigned NOT NULL AUTO_INCREMENT,
  `SellerCharID` int(15) unsigned DEFAULT NULL,
  `BuyerCharID` int(15) unsigned DEFAULT NULL,
  `Zeni` int(15) unsigned DEFAULT NULL,
  `ItemCount` int(3) unsigned DEFAULT NULL,
  `ItemID_1` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_1` int(15) unsigned DEFAULT NULL,
  `ItemID_2` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_2` int(15) unsigned DEFAULT NULL,
  `ItemID_3` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_3` int(15) unsigned DEFAULT NULL,
  `ItemID_4` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_4` int(15) unsigned DEFAULT NULL,
  `ItemID_5` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_5` int(15) unsigned DEFAULT NULL,
  `ItemID_6` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_6` int(15) unsigned DEFAULT NULL,
  `ItemID_7` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_7` int(15) unsigned DEFAULT NULL,
  `ItemID_8` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_8` int(15) unsigned DEFAULT NULL,
  `ItemID_9` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_9` int(15) unsigned DEFAULT NULL,
  `ItemID_10` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_10` int(15) unsigned DEFAULT NULL,
  `ItemID_11` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_11` int(15) unsigned DEFAULT NULL,
  `ItemID_12` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_12` int(15) unsigned DEFAULT NULL,
  `HasIssues` int(1) DEFAULT NULL,
  `IssueReason` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`key`)
) ENGINE=InnoDB AUTO_INCREMENT=163 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for slot_machine_log
-- ----------------------------
DROP TABLE IF EXISTS `slot_machine_log`;
CREATE TABLE `slot_machine_log` (
  `accountid` int(15) unsigned DEFAULT NULL,
  `charid` int(15) unsigned DEFAULT NULL,
  `extractCount` int(3) unsigned DEFAULT NULL,
  `type` int(3) unsigned DEFAULT NULL,
  `coin` int(15) DEFAULT NULL,
  `currentPoints` int(15) unsigned DEFAULT NULL,
  `newPoints` int(15) unsigned DEFAULT NULL,
  `ProductId1` int(15) unsigned DEFAULT NULL,
  `ProductId2` int(15) unsigned DEFAULT NULL,
  `ProductId3` int(15) unsigned DEFAULT NULL,
  `ProductId4` int(15) unsigned DEFAULT NULL,
  `ProductId5` int(15) unsigned DEFAULT NULL,
  `ProductId6` int(15) unsigned DEFAULT NULL,
  `ProductId7` int(15) unsigned DEFAULT NULL,
  `ProductId8` int(15) unsigned DEFAULT NULL,
  `ProductId9` int(15) unsigned DEFAULT NULL,
  `ProductId10` int(15) unsigned DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for tradelogs
-- ----------------------------
DROP TABLE IF EXISTS `tradelogs`;
CREATE TABLE `tradelogs` (
  `key` int(15) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` int(15) unsigned DEFAULT NULL,
  `TargetCharID` int(15) DEFAULT NULL,
  `Zeni` int(15) unsigned DEFAULT NULL,
  `ItemCount` int(3) unsigned DEFAULT NULL,
  `ItemID_1` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_1` int(15) unsigned DEFAULT NULL,
  `ItemID_2` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_2` int(15) unsigned DEFAULT NULL,
  `ItemID_3` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_3` int(15) unsigned DEFAULT NULL,
  `ItemID_4` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_4` int(15) unsigned DEFAULT NULL,
  `ItemID_5` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_5` int(15) unsigned DEFAULT NULL,
  `ItemID_6` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_6` int(15) unsigned DEFAULT NULL,
  `ItemID_7` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_7` int(15) unsigned DEFAULT NULL,
  `ItemID_8` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_8` int(15) unsigned DEFAULT NULL,
  `ItemID_9` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_9` int(15) unsigned DEFAULT NULL,
  `ItemID_10` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_10` int(15) unsigned DEFAULT NULL,
  `ItemID_11` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_11` int(15) unsigned DEFAULT NULL,
  `ItemID_12` bigint(15) unsigned DEFAULT NULL,
  `ItemTblidx_12` int(15) unsigned DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`key`)
) ENGINE=InnoDB AUTO_INCREMENT=32837 DEFAULT CHARSET=utf8;
