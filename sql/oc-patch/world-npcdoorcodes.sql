DELETE FROM `creature_template` WHERE `entry` IN ( 91020, 92021 );

INSERT INTO `creature_template` (`entry`, `heroic_entry`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `baseattacktime`, `rangeattacktime`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(91020, 0, 1985, 0, 1985, 0, 'Garble', 'Doorman', NULL, 0, 70, 70, 5000, 5000, 0, 0, 2865, 35, 35, 1, 1, 1, 1, 60, 165, 0, 97, 1500, 1500, 0, 0, 0, 0, 0, 0, 0, 50, 100, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 0, 0, 0, 'npc_door_codes'),
(91021, 0, 1985, 0, 1985, 0, 'Reinhard', 'Doorman', NULL, 0, 70, 70, 5000, 5000, 0, 0, 2865, 35, 35, 1, 1, 1, 1, 60, 165, 0, 97, 1500, 1500, 0, 0, 0, 0, 0, 0, 0, 50, 100, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 0, 0, 0, 'npc_door_codes');

DELETE FROM `gameobject_template` WHERE `entry` IN ( 600000, 600001 );

INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES
(600000, 0, 3811, 'Restricted Access Door 1', '', 14, 32, 0.75, 0, 0, 196608, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(600001, 0, 3811, 'Restricted Access Door 2', '', 14, 32, 0.75, 0, 0, 196608, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

CREATE TABLE IF NOT EXISTS `npc_door_codes` (
  `id` bigint(20) unsigned NOT NULL auto_increment COMMENT 'Code ID',
  `secret` varchar(20) NOT NULL COMMENT 'Secret Word',
  `creature_entry` mediumint(9) NOT NULL,
  `gameobject_entry` mediumint(9) NOT NULL,
  `opentimesec` smallint(5) unsigned NOT NULL default '5' COMMENT 'Seconds door stays open',
  `comments` text,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `creature_entry` (`creature_entry`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

INSERT INTO `npc_door_codes` (`id`, `secret`, `creature_entry`, `gameobject_entry`, `opentimesec`, `comments`) VALUES
(1, 'sesame', 91020, 600000, 2, 'Test Code 1'),
(2, 'howdy', 91021, 600001, 30, 'Test Code 2');

