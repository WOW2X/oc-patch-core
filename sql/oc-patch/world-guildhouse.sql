--
-- Table structure for table `guild_houses`
--

CREATE TABLE IF NOT EXISTS `guild_houses` (
  `id` int(8) unsigned NOT NULL auto_increment,
  `guildId` bigint(20) NOT NULL default '0',
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `map` int(11) NOT NULL,
  `comment` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;

--
-- Dumping data for table `guild_houses`
--

INSERT INTO `guild_houses` (`id`, `guildId`, `x`, `y`, `z`, `map`, `comment`) VALUES
(1, 1, 16222, 16266, 14.2, 1, 'GM Island'),
(2, 0, -10711, 2483, 8, 1, 'Village on the Veiled Sea (Silithus)'),
(3, 0, -8323, -343, 146, 0, 'Elwynn Falls Camp (Elwynn Forest)'),
(4, 0, -1840, -4233, 2.14, 0, 'Arathi Village (Arathi Highlands, Forbidding Sea)'),
(5, 0, -6374, 1262, 7, 0, 'Harbor House (Elwynn Forest)'),
(6, 0, 4303, -2760, 16.8, 0, 'Quel''Thalas Tower');


