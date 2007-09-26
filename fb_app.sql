CREATE TABLE IF NOT EXISTS fb_app (
nid int(11) unsigned NOT NULL,
label varchar(128) NOT NULL,
apikey varchar(128) NOT NULL,
secret varchar(128) NOT NULL,
canvas varchar(128) NOT NULL,
require_login int(4) NOT NULL,
create_account int(4) NOT NULL,
unique_account int(4) NOT NULL,
rid int(10) unsigned NOT NULL,
data longtext,
PRIMARY KEY (nid),
UNIQUE KEY (apikey)
) /*!40100 DEFAULT CHARACTER SET UTF8 */;

CREATE TABLE IF NOT EXISTS fb_app_block (
nid int(11) unsigned NOT NULL,
delta varchar(32) NOT NULL,
format int(11) DEFAULT 0,
body longtext NOT NULL,
PRIMARY KEY (nid, delta)
) /*!40100 DEFAULT CHARACTER SET UTF8 */;

CREATE TABLE IF NOT EXISTS fb_app_user (
nid int(11) unsigned NOT NULL,
fbu int(11) unsigned NOT NULL,
added int(4) unsigned NOT NULL,
time_cron int(11) unsigned NOT NULL,
time_access int(11) unsigned NOT NULL,
session_key varchar(128) NOT NULL,
session_key_expires int(11) unsigned NOT NULL,
PRIMARY KEY (nid, fbu)
) /*!40100 DEFAULT CHARACTER SET UTF8 */;
