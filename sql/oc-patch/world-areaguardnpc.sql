-- Table structure for table `npc_areaguard`

CREATE TABLE IF NOT EXISTS `npc_areaguard` (
  `creature_entry` mediumint(8) unsigned NOT NULL,
  `guard_type` tinyint(3) unsigned NOT NULL default '0' COMMENT '0=All,1=Team,2=SecLvl,3=PlrLvl,4=Guild,5=PlrGuid',
  `trigger_distance` float NOT NULL default '65' COMMENT 'In Feet - Max is 65',
  `req_value` tinyint(3) unsigned NOT NULL default '0' COMMENT 'For Team 1=Ally,2=Horde',
  `tele_map` mediumint(8) unsigned NOT NULL default '1' COMMENT 'Dflt tele dest is Ratchet',
  `tele_x` float NOT NULL default '-955.726',
  `tele_y` float NOT NULL default '-3747.16',
  `tele_z` float NOT NULL default '5.47817',
  `tele_o` float NOT NULL default '1.6369',
  `comment` varchar(40) NOT NULL,
  PRIMARY KEY  (`creature_entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='NPC Area Guard';

-- Dumping data for table `npc_areaguard`

INSERT INTO `npc_areaguard` (`creature_entry`, `guard_type`, `trigger_distance`, `req_value`, `tele_map`, `tele_x`, `tele_y`, `tele_z`, `tele_o`, `comment`) VALUES
(92015, 0, 65, 0, 1, -955.726, -3747.16, 5.47817, 1.6369, 'Teleport all non-GM players to Ratchet');

DELETE FROM `creature_template` WHERE `entry` = 92015;

INSERT INTO `creature_template` (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(92015, 0, 10458, 0, 0, 0, 'Guard', '', NULL, 0, 70, 70, 12000, 12000, 4000, 4000, 12000, 35, 35, 129, 8, 1, 0, 2000, 50000, 0, 0, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 20, 20, 20, 20, 20, 20, 0, 0, 0, 0, 0, 521411, 521411, '', 0, 3, 0, 1, 0, 0, 0, 'npc_areaguard');

