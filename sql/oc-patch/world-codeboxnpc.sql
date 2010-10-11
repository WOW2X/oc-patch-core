--
-- Table structure for table `npc_codes`
--

CREATE TABLE IF NOT EXISTS `npc_codes` (
  `npc_id` mediumint(10) NOT NULL default '0' COMMENT 'The NPC ID',
  `code` varchar(25) NOT NULL COMMENT 'The code you have to enter',
  `uses` tinyint(3) NOT NULL default '1' COMMENT 'Number of uses left for this code',
  `aura_id` mediumint(10) NOT NULL default '0' COMMENT 'The spell it will be applied',
  `item_id` mediumint(10) NOT NULL default '0' COMMENT 'The gift (item)',
  `quantity` tinyint(3) unsigned NOT NULL default '1' COMMENT 'The quantity of ITEMS',
  PRIMARY KEY  (`npc_id`,`code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `npc_codes`
--

INSERT INTO `npc_codes` (`npc_id`, `code`, `uses`, `aura_id`, `item_id`, `quantity`) VALUES
(2943, 'murlocsuit', 3, 0, 33079, 1),
(2943, 'surprise', -1, -1, 0, 1),
(16070, 'murlocblue', 5, 0, 20371, 1),
(16070, 'murlocpink', 5, 0, 22114, 1),
(16070, 'surprise', -1, -1, 0, 1),
(7951, 'murlocsuit', 3, 0, 33079, 1),
(7951, 'surprise', -1, -1, 0, 1),
(16076, 'murlocblue', 5, 0, 20371, 1),
(16076, 'murlocpink', 5, 0, 22114, 1),
(16076, 'surprise', -1, -1, 0, 1);


--
-- Table structure for table `npc_codes_text`
--

CREATE TABLE IF NOT EXISTS `npc_codes_text` (
  `npc_id` mediumint(10) NOT NULL COMMENT 'from npc_codes',
  `npc_text_id` mediumint(10) NOT NULL default '907' COMMENT 'from npc_text'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Reward Codes System';

--
-- Dumping data for table `npc_codes_text`
--

INSERT INTO `npc_codes_text` (`npc_id`, `npc_text_id`) VALUES
(7951, 13690),
(16076, 907),
(2943, 13690),
(16070, 907);

--
-- Dumping data for table `npc_text`
--

INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`) VALUES
(13690, 'What can I do for you?', '', 0, 1, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0);


UPDATE creature_template SET ScriptName = 'npc_codebox' WHERE entry IN (2943, 7951, 16070, 16076);


--
-- Table structure for table `npc_codes_aura_loot`
--

CREATE TABLE IF NOT EXISTS `npc_codes_aura_loot` (
  `group_id` mediumint(10) NOT NULL default '0' COMMENT 'Group that the auras belong to',
  `aura_id` mediumint(10) NOT NULL default '0' COMMENT 'GUID of aura',
  `name` varchar(25) default NULL COMMENT 'Name of aura',
  PRIMARY KEY  (`group_id`,`aura_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Dumping data for table `npc_codes_aura_loot`
--

INSERT INTO `npc_codes_aura_loot` (`group_id`, `aura_id`, `name`) VALUES
(1, 24708, 'male pirate costume'),
(1, 24709, 'female pirate costume'),
(1, 24710, 'male ninja costume'),
(1, 24711, 'female ninja costume'),
(1, 24713, 'leper gnome costume'),
(1, 24723, 'skeleton costume'),
(1, 24735, 'male ghost costume'),
(1, 24736, 'female ghost costume'),
(1, 24753, 'cannot cast');

--
-- Table structure for table `npc_codes_item_loot`
--

CREATE TABLE IF NOT EXISTS `npc_codes_item_loot` (
  `group_id` mediumint(10) NOT NULL default '0' COMMENT 'Group that the items belong to',
  `item_id` mediumint(10) NOT NULL default '0' COMMENT 'GUID of item',
  `name` varchar(25) default NULL COMMENT 'Name of Item',
  PRIMARY KEY  (`group_id`,`item_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

