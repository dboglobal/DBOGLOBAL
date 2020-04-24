/*
Navicat MySQL Data Transfer

Source Server         : db
Source Server Version : 80018
Source Host           : localhost:3306
Source Database       : dbo_char

Target Server Type    : MYSQL
Target Server Version : 80018
File Encoding         : 65001

Date: 2020-04-21 14:06:18
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for auctionhouse
-- ----------------------------
DROP TABLE IF EXISTS `auctionhouse`;
CREATE TABLE `auctionhouse` (
  `id` bigint(20) unsigned NOT NULL,
  `CharID` int(10) unsigned NOT NULL DEFAULT '0',
  `TabType` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `ItemName` varchar(33) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `Seller` varchar(17) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `Price` int(10) unsigned NOT NULL,
  `ItemID` bigint(20) unsigned NOT NULL,
  `TimeStart` int(10) unsigned NOT NULL,
  `TimeEnd` int(10) unsigned DEFAULT NULL COMMENT 'time in seconds',
  `ItemLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `NeedClass` int(10) unsigned NOT NULL,
  `ItemType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`CharID`),
  UNIQUE KEY `id` (`id`) USING BTREE,
  KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for bannword
-- ----------------------------
DROP TABLE IF EXISTS `bannword`;
CREATE TABLE `bannword` (
  `id` int(5) NOT NULL AUTO_INCREMENT,
  `bannword` varchar(50) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for bind
-- ----------------------------
DROP TABLE IF EXISTS `bind`;
CREATE TABLE `bind` (
  `CharID` int(10) unsigned NOT NULL,
  `WorldID` int(10) unsigned NOT NULL DEFAULT '1',
  `BindObjectTblIdx` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `LocX` float(11,6) NOT NULL,
  `LocY` float(11,6) NOT NULL,
  `LocZ` float(11,6) NOT NULL,
  `DirX` float(11,6) NOT NULL,
  `DirY` float(11,6) NOT NULL,
  `DirZ` float(11,6) NOT NULL,
  `Type` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`CharID`),
  UNIQUE KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for buffs
-- ----------------------------
DROP TABLE IF EXISTS `buffs`;
CREATE TABLE `buffs` (
  `CharID` int(10) unsigned NOT NULL,
  `SourceTblidx` int(10) unsigned NOT NULL,
  `SourceType` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT '0 skill and 1 item',
  `BuffIndex` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `BuffGroup` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `InitialDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeRemaining` int(10) unsigned NOT NULL DEFAULT '0',
  `effectValue1` double(10,2) DEFAULT NULL,
  `effectValue2` double(10,2) DEFAULT NULL,
  `Argument1_0` int(10) unsigned DEFAULT NULL COMMENT 'commonConfigTblidx',
  `Argument1_1` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainTime',
  `Argument1_2` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainValue',
  `Argument2_0` int(10) unsigned DEFAULT NULL COMMENT 'commonConfigTblidx',
  `Argument2_1` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainTime',
  `Argument2_2` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainValue',
  PRIMARY KEY (`CharID`,`BuffIndex`),
  UNIQUE KEY `CharID` (`CharID`,`BuffIndex`) USING BTREE,
  KEY `CharID_2` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for characters
-- ----------------------------
DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `CharID` int(10) unsigned NOT NULL,
  `CharName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `AccountID` int(10) unsigned NOT NULL,
  `Level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `Exp` int(10) unsigned NOT NULL DEFAULT '0',
  `Race` tinyint(1) unsigned DEFAULT NULL,
  `Class` tinyint(2) unsigned DEFAULT NULL,
  `Gender` tinyint(1) unsigned DEFAULT NULL,
  `Face` tinyint(2) unsigned DEFAULT NULL,
  `Adult` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `Hair` tinyint(2) unsigned NOT NULL,
  `HairColor` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `SkinColor` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `Blood` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `CurLocX` float(11,6) NOT NULL DEFAULT '78.900002',
  `CurLocY` float(11,6) NOT NULL DEFAULT '46.950001',
  `CurLocZ` float(11,6) NOT NULL DEFAULT '168.350006',
  `CurDirX` float(11,6) NOT NULL DEFAULT '0.950000',
  `CurDirY` float(11,6) NOT NULL DEFAULT '0.000000',
  `CurDirZ` float(11,6) NOT NULL DEFAULT '0.300000',
  `WorldID` int(10) unsigned NOT NULL DEFAULT '1',
  `WorldTable` int(10) unsigned NOT NULL DEFAULT '1',
  `MapInfoIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `Money` int(10) unsigned NOT NULL DEFAULT '0',
  `MoneyBank` int(10) unsigned NOT NULL DEFAULT '0',
  `TutorialFlag` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0 = start tutorial / 1 = dont start tutorial',
  `TutorialHint` int(10) unsigned NOT NULL DEFAULT '0',
  `NameChange` tinyint(1) NOT NULL DEFAULT '0',
  `Reputation` int(10) unsigned NOT NULL DEFAULT '0',
  `MudosaPoint` int(10) unsigned NOT NULL DEFAULT '0',
  `SpPoint` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'skill points',
  `GameMaster` tinyint(1) NOT NULL DEFAULT '0',
  `GuildID` int(10) unsigned NOT NULL DEFAULT '0',
  `GuildName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `CurLP` int(10) NOT NULL DEFAULT '15000',
  `CurEP` smallint(5) unsigned NOT NULL DEFAULT '15000',
  `CurRP` smallint(5) unsigned NOT NULL DEFAULT '0',
  `CurAP` int(10) NOT NULL DEFAULT '450000',
  `MailIsAway` tinyint(1) NOT NULL DEFAULT '0',
  `SrvFarmID` int(3) unsigned NOT NULL DEFAULT '0',
  `DelCharTime` bigint(20) unsigned DEFAULT NULL COMMENT 'time(0) + 43200 = 12 hours',
  `Hoipoi_NormalStart` tinyint(1) NOT NULL DEFAULT '0',
  `Hoipoi_SpecialStart` tinyint(1) NOT NULL DEFAULT '0',
  `Hoipoi_Type` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `Hoipoi_MixLevel` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `Hoipoi_MixExp` int(10) unsigned NOT NULL DEFAULT '0',
  `Title` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `Mascot` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `RpBall` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `Netpy` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Netpy are points the user receives while staying online',
  `WaguPoint` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '',
  `IP` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '0.0.0.0' COMMENT 'the last IP in the char',
  `AirState` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '0 = off and 1 = on',
  `InvisibleCostume` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0 = false 1 = true',
  `PlayTime` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'play time in seconds',
  `SuperiorEffectType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `CreateTime` bigint(15) unsigned NOT NULL COMMENT 'time(0)',
  PRIMARY KEY (`CharID`,`CharName`,`SrvFarmID`),
  UNIQUE KEY `CharID` (`CharID`) USING BTREE,
  UNIQUE KEY `CharName` (`CharName`) USING BTREE,
  UNIQUE KEY `CharID_2` (`CharID`,`SrvFarmID`) USING BTREE,
  UNIQUE KEY `CharID_3` (`CharID`,`AccountID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for dojos
-- ----------------------------
DROP TABLE IF EXISTS `dojos`;
CREATE TABLE `dojos` (
  `GuildId` int(10) unsigned NOT NULL,
  `DojoTblidx` int(10) unsigned NOT NULL,
  `Level` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `PeaceStatus` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `PeacePoints` int(15) unsigned NOT NULL DEFAULT '0',
  `GuildName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `LeaderName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `Notice` varchar(64) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `ChallengeGuildId` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `SeedCharName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  PRIMARY KEY (`DojoTblidx`),
  UNIQUE KEY `DojoTblidx` (`DojoTblidx`) USING BTREE,
  UNIQUE KEY `GuildId` (`GuildId`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for friendlist
-- ----------------------------
DROP TABLE IF EXISTS `friendlist`;
CREATE TABLE `friendlist` (
  `user_id` int(10) unsigned NOT NULL,
  `friend_id` int(10) unsigned NOT NULL,
  `friend_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `blacklist` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_id`,`friend_id`),
  UNIQUE KEY `user_id` (`user_id`,`friend_id`) USING BTREE,
  KEY `user_id_2` (`user_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for guilds
-- ----------------------------
DROP TABLE IF EXISTS `guilds`;
CREATE TABLE `guilds` (
  `GuildID` int(10) unsigned NOT NULL,
  `GuildName` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `GuildMaster` int(10) unsigned NOT NULL,
  `GuildSecondMaster` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `GuildSecondMaster2` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `GuildSecondMaster3` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `GuildSecondMaster4` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `GuildReputation` int(10) unsigned NOT NULL DEFAULT '0',
  `GuildPointEver` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'max guild points ever received',
  `FunctionFlag` bigint(15) unsigned NOT NULL DEFAULT '7',
  `GuildDisbandTime` int(15) unsigned DEFAULT NULL,
  `MarkInColor` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `MarkInLine` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `MarkMain` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `MarkMainColor` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `MarkOutColor` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `MarkOutLine` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `NoticeBy` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `GuildNotice` varchar(257) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `DojoColor` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `GuildColor` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `DogiType` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `Zeni` int(10) unsigned DEFAULT '0' COMMENT 'Zeni inside guild bank',
  PRIMARY KEY (`GuildID`,`GuildName`),
  UNIQUE KEY `GuildID` (`GuildID`) USING BTREE,
  UNIQUE KEY `GuildName` (`GuildName`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for guild_members
-- ----------------------------
DROP TABLE IF EXISTS `guild_members`;
CREATE TABLE `guild_members` (
  `GuildID` int(14) unsigned NOT NULL,
  `CharID` int(14) unsigned NOT NULL,
  PRIMARY KEY (`GuildID`,`CharID`),
  UNIQUE KEY `GuildID` (`GuildID`,`CharID`) USING BTREE,
  UNIQUE KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for hoipoi_recipe
-- ----------------------------
DROP TABLE IF EXISTS `hoipoi_recipe`;
CREATE TABLE `hoipoi_recipe` (
  `CharID` int(10) unsigned NOT NULL,
  `RecipeTblidx` int(10) unsigned NOT NULL,
  `RecipeType` tinyint(3) unsigned NOT NULL DEFAULT '255',
  PRIMARY KEY (`CharID`,`RecipeTblidx`),
  UNIQUE KEY `CharID_2` (`CharID`,`RecipeTblidx`) USING BTREE,
  KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for htb_skills
-- ----------------------------
DROP TABLE IF EXISTS `htb_skills`;
CREATE TABLE `htb_skills` (
  `skill_id` int(10) unsigned NOT NULL DEFAULT '0',
  `owner_id` int(10) unsigned NOT NULL,
  `SlotID` smallint(3) unsigned NOT NULL DEFAULT '0',
  `TimeRemaining` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`skill_id`,`owner_id`),
  UNIQUE KEY `skill_id` (`skill_id`,`owner_id`) USING BTREE,
  UNIQUE KEY `owner_id` (`owner_id`,`SlotID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for items
-- ----------------------------
DROP TABLE IF EXISTS `items`;
CREATE TABLE `items` (
  `id` bigint(20) unsigned NOT NULL,
  `tblidx` int(10) unsigned NOT NULL DEFAULT '0',
  `owner_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'character id',
  `place` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `pos` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `count` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `rank` tinyint(2) NOT NULL DEFAULT '1',
  `durability` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `grade` tinyint(2) NOT NULL DEFAULT '0',
  `NeedToIdentify` tinyint(1) NOT NULL DEFAULT '0',
  `BattleAttribute` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `Maker` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '',
  `OptionTblidx` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT 'item_option_data tblidx',
  `OptionTblidx2` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT 'item_option_data tblidx',
  `OptionRandomId` smallint(5) unsigned NOT NULL DEFAULT '65535' COMMENT 'item_enchant tblidx',
  `OptionRandomVal` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `OptionRandomId2` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `OptionRandomVal2` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `OptionRandomId3` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `OptionRandomVal3` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `OptionRandomId4` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `OptionRandomVal4` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `OptionRandomId5` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `OptionRandomVal5` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `OptionRandomId6` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `OptionRandomVal6` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `OptionRandomId7` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `OptionRandomVal7` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `OptionRandomId8` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `OptionRandomVal8` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `UseStartTime` bigint(15) unsigned NOT NULL DEFAULT '0' COMMENT 'max duration time (3600 = 60 minutes)',
  `UseEndTime` bigint(15) unsigned NOT NULL DEFAULT '0' COMMENT 'current duration time (3600 = 60 minutes)',
  `RestrictState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DurationType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `AccountID` int(10) unsigned DEFAULT '0',
  `GuildID` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`id`,`owner_id`,`place`,`pos`),
  UNIQUE KEY `id` (`id`) USING BTREE,
  KEY `owner_id_idx` (`owner_id`) USING BTREE,
  KEY `item_vnum_index` (`tblidx`) USING BTREE,
  KEY `owner_id` (`owner_id`,`place`,`pos`) USING BTREE,
  KEY `owner_id_2` (`owner_id`,`place`) USING BTREE,
  KEY `AccountID` (`AccountID`) USING BTREE,
  KEY `GuildID` (`GuildID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for items_cd
-- ----------------------------
DROP TABLE IF EXISTS `items_cd`;
CREATE TABLE `items_cd` (
  `CharID` int(10) unsigned NOT NULL,
  `GroupIndex` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `CoolTime` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeRemaining` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`GroupIndex`,`CharID`),
  UNIQUE KEY `CharID` (`CharID`,`GroupIndex`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for mail
-- ----------------------------
DROP TABLE IF EXISTS `mail`;
CREATE TABLE `mail` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
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
) ENGINE=InnoDB AUTO_INCREMENT=5937 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for mascots
-- ----------------------------
DROP TABLE IF EXISTS `mascots`;
CREATE TABLE `mascots` (
  `CharID` int(15) unsigned NOT NULL,
  `SlotID` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT 'index',
  `MascotTblidx` int(10) unsigned NOT NULL,
  `CurVP` int(10) unsigned NOT NULL DEFAULT '100',
  `MaxVP` int(10) unsigned NOT NULL DEFAULT '100',
  `CurExp` int(10) unsigned NOT NULL DEFAULT '0',
  `skillTblidx0` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `skillTblidx1` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `skillTblidx2` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `skillTblidx3` int(10) unsigned NOT NULL DEFAULT '4294967295',
  PRIMARY KEY (`CharID`,`SlotID`),
  UNIQUE KEY `CharID` (`CharID`,`SlotID`) USING BTREE,
  KEY `CharID_2` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for portals
-- ----------------------------
DROP TABLE IF EXISTS `portals`;
CREATE TABLE `portals` (
  `CharID` int(10) unsigned NOT NULL,
  `Point` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`CharID`,`Point`),
  UNIQUE KEY `CharID_2` (`CharID`,`Point`) USING BTREE,
  KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for questitems
-- ----------------------------
DROP TABLE IF EXISTS `questitems`;
CREATE TABLE `questitems` (
  `CharID` int(10) unsigned NOT NULL,
  `tblidx` int(10) unsigned NOT NULL,
  `amount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `pos` tinyint(3) unsigned NOT NULL DEFAULT '255',
  KEY `CharID_2` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for quests
-- ----------------------------
DROP TABLE IF EXISTS `quests`;
CREATE TABLE `quests` (
  `CharID` int(10) unsigned NOT NULL,
  `QuestID` int(10) unsigned NOT NULL,
  `tcQuestInfo` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `taQuestInfo` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tgExcCGroup` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tcPreId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tcCurId` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `tcId` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `taId` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `evtUserData` int(10) unsigned NOT NULL DEFAULT '0',
  `evtUserData2` int(10) unsigned NOT NULL DEFAULT '0',
  `evtUserData3` int(10) unsigned NOT NULL DEFAULT '0',
  `evtUserData4` int(10) unsigned NOT NULL DEFAULT '0',
  `tcTimeInfo` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `taTimeInfo` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `TimeLeft` int(10) unsigned NOT NULL DEFAULT '0',
  `QState` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharID`,`QuestID`),
  UNIQUE KEY `CharID_2` (`CharID`,`QuestID`) USING BTREE,
  KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for quickslot
-- ----------------------------
DROP TABLE IF EXISTS `quickslot`;
CREATE TABLE `quickslot` (
  `CharID` int(10) unsigned NOT NULL DEFAULT '0',
  `Tblidx` int(10) unsigned NOT NULL DEFAULT '0',
  `Slot` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Item` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'item unique id',
  PRIMARY KEY (`CharID`,`Slot`),
  UNIQUE KEY `CharID` (`CharID`,`Slot`) USING BTREE,
  KEY `CharID_2` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for quick_teleport
-- ----------------------------
DROP TABLE IF EXISTS `quick_teleport`;
CREATE TABLE `quick_teleport` (
  `CharID` int(10) unsigned NOT NULL,
  `SlotNum` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `WorldTblidx` int(10) unsigned NOT NULL,
  `LocX` float(11,6) NOT NULL,
  `LocY` float(11,6) NOT NULL,
  `LocZ` float(11,6) NOT NULL,
  `MapNameTblidx` int(10) unsigned NOT NULL,
  `day` tinyint(2) unsigned NOT NULL,
  `hour` tinyint(2) unsigned NOT NULL,
  `minute` tinyint(2) unsigned NOT NULL,
  `month` tinyint(2) unsigned NOT NULL,
  `second` tinyint(2) unsigned NOT NULL,
  `year` int(4) unsigned NOT NULL,
  PRIMARY KEY (`CharID`,`SlotNum`),
  UNIQUE KEY `CharID` (`CharID`,`SlotNum`) USING BTREE,
  KEY `CharID_2` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for rank_battle
-- ----------------------------
DROP TABLE IF EXISTS `rank_battle`;
CREATE TABLE `rank_battle` (
  `CharID` int(10) unsigned NOT NULL,
  `Win` int(10) unsigned NOT NULL DEFAULT '0',
  `Draw` int(10) unsigned NOT NULL DEFAULT '0',
  `Lose` int(10) unsigned NOT NULL DEFAULT '0',
  `StraightKOWin` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxStraightKOWin` int(10) unsigned NOT NULL DEFAULT '0',
  `MaxStraightWin` int(10) unsigned NOT NULL DEFAULT '0',
  `StraightWin` int(10) unsigned NOT NULL DEFAULT '0',
  `Points` float(10,0) NOT NULL DEFAULT '0',
  PRIMARY KEY (`CharID`),
  UNIQUE KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for skills
-- ----------------------------
DROP TABLE IF EXISTS `skills`;
CREATE TABLE `skills` (
  `skill_id` int(10) unsigned NOT NULL DEFAULT '0',
  `owner_id` int(10) unsigned NOT NULL,
  `RpBonusAuto` tinyint(1) NOT NULL DEFAULT '0',
  `RpBonusType` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `SlotID` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'skillIndex',
  `TimeRemaining` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Skill CD Time',
  `Exp` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`owner_id`,`SlotID`),
  UNIQUE KEY `owner_id` (`owner_id`,`SlotID`) USING BTREE,
  KEY `owner_id_2` (`owner_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for titles
-- ----------------------------
DROP TABLE IF EXISTS `titles`;
CREATE TABLE `titles` (
  `CharID` int(10) unsigned NOT NULL,
  `TitleTblidx` int(10) unsigned NOT NULL,
  PRIMARY KEY (`CharID`,`TitleTblidx`),
  UNIQUE KEY `CharID_2` (`CharID`,`TitleTblidx`) USING BTREE,
  KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for warfog
-- ----------------------------
DROP TABLE IF EXISTS `warfog`;
CREATE TABLE `warfog` (
  `CharID` int(10) unsigned NOT NULL,
  `WarFog` int(10) unsigned NOT NULL,
  PRIMARY KEY (`CharID`,`WarFog`),
  UNIQUE KEY `CharID_2` (`CharID`,`WarFog`) USING BTREE,
  KEY `CharID` (`CharID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
