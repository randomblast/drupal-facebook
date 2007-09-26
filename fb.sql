CREATE TABLE fb_cache_filter ( 
cid varchar(255) NOT NULL default '', 
data longblob, 
expire int NOT NULL default '0', 
created int NOT NULL default '0', 
headers text, 
PRIMARY KEY (cid), 
INDEX expire (expire) 
) /*!40100 DEFAULT CHARACTER SET UTF8 */;
