-- Table structure for table `npc_tokens`

CREATE TABLE IF NOT EXISTS `npc_tokens` (
  `type` tinyint(3) unsigned NOT NULL default '0' COMMENT 'Valid: 0-Gold, 1-Level, 2-Honor, 3-Arena, 4-MaxSkill',
  `min_level` tinyint(3) unsigned NOT NULL default '1',
  `max_level` tinyint(3) unsigned NOT NULL default '70',
  `curr_item_id` mediumint(8) unsigned NOT NULL default '29434' COMMENT 'Currency - Dflt: Badge of Justice',
  `curr_cost` tinyint(3) unsigned NOT NULL default '1',
  `count_granted` mediumint(8) unsigned NOT NULL default '1',
  UNIQUE KEY `type` (`type`,`min_level`,`max_level`,`curr_item_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='NPC Tokens';

-- Data for table `npc_tokens`

INSERT INTO `npc_tokens` (`type`, `min_level`, `max_level`, `curr_item_id`, `curr_cost`, `count_granted`) VALUES
(0, 1, 70, 29434, 1, 500),
(1, 1, 70, 29434, 1, 1),
(2, 1, 70, 29434, 1, 500),
(3, 1, 70, 29434, 1, 100),
(4, 1, 70, 29434, 1, 1);

