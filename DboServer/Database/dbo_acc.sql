/*
Navicat MySQL Data Transfer

Source Server         : db
Source Server Version : 80018
Source Host           : localhost:3306
Source Database       : dbo_acc

Target Server Type    : MYSQL
Target Server Version : 80018
File Encoding         : 65001

Date: 2020-04-21 14:05:53
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for accounts
-- ----------------------------
DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `AccountID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Username` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `Password_hash` varchar(32) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL COMMENT 'password in md5',
  `acc_status` enum('pending','block','active') CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT 'active',
  `email` varchar(80) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT 'test@mail.com',
  `mallpoints` int(10) unsigned NOT NULL DEFAULT '10000000',
  `reg_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_login` timestamp NULL DEFAULT NULL,
  `reg_ip` varchar(15) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `admin` tinyint(2) NOT NULL DEFAULT '0' COMMENT 'value from 0 to 10',
  `isGm` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0 = normal user / 1 = game master or people with who can login in testing phase',
  `lastServerFarmId` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT 'default: INVALID_SERVERFARMID ( 255 )',
  `founder` smallint(1) NOT NULL DEFAULT '0' COMMENT '0 = no founder / 1 = first founder / 2 = second / 3 = third',
  `founder_recv` smallint(1) NOT NULL DEFAULT '0' COMMENT '0 = founder not received / 1 = founder received',
  `last_ip` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '0.0.0.0',
  `del_char_pw` varchar(32) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '25f9e794323b453885f5181f1b624d0b',
  `PremiumSlots` tinyint(1) unsigned NOT NULL DEFAULT '4',
  `EventCoins` int(10) unsigned DEFAULT '0' COMMENT 'coins used to play on HLS event machine',
  `WaguCoins` int(10) unsigned DEFAULT '0' COMMENT 'coins used to play on HLS slot machine',
  `web_ip` varchar(15) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  PRIMARY KEY (`AccountID`,`Username`),
  UNIQUE KEY `AccountID` (`AccountID`) USING BTREE,
  UNIQUE KEY `Username` (`Username`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=812872 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for accounts_banned
-- ----------------------------
DROP TABLE IF EXISTS `accounts_banned`;
CREATE TABLE `accounts_banned` (
  `id` int(20) unsigned NOT NULL AUTO_INCREMENT,
  `GM_AccId` int(16) unsigned NOT NULL DEFAULT '0',
  `Banned_AccId` int(16) unsigned NOT NULL DEFAULT '0',
  `DateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `Reason` varchar(1024) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `Duration` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT 'Value in days. 255 = permanent',
  `Active` tinyint(1) NOT NULL DEFAULT '1' COMMENT '1 = true, 0 = false',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for cashshop_storage
-- ----------------------------
DROP TABLE IF EXISTS `cashshop_storage`;
CREATE TABLE `cashshop_storage` (
  `ProductId` int(20) unsigned NOT NULL AUTO_INCREMENT,
  `AccountID` int(10) unsigned NOT NULL,
  `HLSitemTblidx` int(10) unsigned NOT NULL,
  `StackCount` tinyint(3) unsigned NOT NULL,
  `giftCharId` int(10) unsigned DEFAULT NULL,
  `IsRead` tinyint(1) NOT NULL DEFAULT '0',
  `SenderName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `year` int(4) unsigned NOT NULL,
  `month` tinyint(2) unsigned NOT NULL,
  `day` tinyint(2) unsigned NOT NULL,
  `hour` tinyint(2) unsigned NOT NULL,
  `minute` tinyint(2) unsigned NOT NULL,
  `second` tinyint(2) unsigned NOT NULL,
  `millisecond` int(4) unsigned NOT NULL,
  `isMoved` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'BOOL. Did the player move the item to his inventory',
  `Buyer` int(10) unsigned DEFAULT '0' COMMENT 'account id from the buyer',
  `price` int(10) unsigned DEFAULT '0',
  `ItemID` bigint(20) unsigned DEFAULT '0',
  PRIMARY KEY (`ProductId`),
  UNIQUE KEY `ProductId` (`ProductId`,`AccountID`) USING BTREE,
  KEY `AccountID` (`AccountID`,`isMoved`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=463999 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for event_reward
-- ----------------------------
DROP TABLE IF EXISTS `event_reward`;
CREATE TABLE `event_reward` (
  `AccountID` int(15) unsigned DEFAULT NULL,
  `rewardTblidx` int(15) unsigned DEFAULT NULL,
  `CharID` int(15) unsigned DEFAULT '0',
  `CharName` varchar(15) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  UNIQUE KEY `AccountID` (`AccountID`,`rewardTblidx`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for shortcuts
-- ----------------------------
DROP TABLE IF EXISTS `shortcuts`;
CREATE TABLE `shortcuts` (
  `AccountID` int(10) unsigned NOT NULL,
  `ActionID` int(10) unsigned NOT NULL DEFAULT '0',
  `wKey` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`AccountID`,`ActionID`),
  UNIQUE KEY `AccountID` (`AccountID`,`ActionID`) USING BTREE,
  KEY `AccountID_2` (`AccountID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
