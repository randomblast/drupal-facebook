Drupal for Facebook
-------------------

More information: 
http://www.drupalforfacebook.org, http://drupal.org/project/fb

Primary author and maintainer: Dave Cohen (http://www.dave-cohen.com/contact)

Branch: HEAD (version 3.x for Drupal 6.x)

Detailed documentation is available online
http://drupal.org/node/195035.  This file is probably more recent than
the online documentation!

If upgrading from a previous dev build or version, read the upgrade instructions:
http://drupal.org/node/761886

To install:
   
- Make sure you have an up-to-date PHP client from facebook.  
  Download from http://github.com/facebook/php-sdk.
  Extract the files, and place them in sites/all/libraries/facebook-php-sdk.

- To find the php-sdk in any other directory, edit your settings.php to include a line similar to this:

  $conf['fb_api_file'] = 'sites/all/libraries/facebook-php-sdk/src/facebook.php';
  (Customize the path above as needed for your installation.)

- If using Facebook Connect, your theme needs the following attribute
  in it's <html> tag: xmlns:fb="http://www.facebook.com/2008/fbml" See
  http://www.drupalforfacebook.org/node/1106.  This also applies to
  themes used for iframe canvas pages.

- If you want to support canvas pages, url rewriting is
  recommended.  Enable this by adding this line to settings.php:

  include "sites/all/modules/fb/fb_url_rewrite.inc";


- Edit your settings.php file (sites/default/settings.php, depending
  on your install) to include fb_settings.inc (in this directory).
  For example, add this at the very end of your settings.php:

  include "sites/all/modules/fb/fb_settings.inc";


- Go to Administer >> Site Building >> Modules and enable the Facebook
  modules.  Most of the modules under "Drupal for Facebook" should be
  enabled.  During development, the "Drupal for Facebook Devel" module
  is a must.

- You must enable clean URLs.  If you don't, some links that drupal
  creates will not work properly on canvas pages.

- Create an application on Facebook, currently at
  http://www.facebook.com/developers/editapp.php?new.  Fill in the
  minimum required to get an apikey and secret.  If supporting canvas
  pages, get a canvas name, too.

- Go to Administer >> Site Building >> Facebook Applications and click
  the Add Applicaiton tab.  Use the apikey and secret that Facebook
  has shown you.  If you have any trouble with the other fields, use
  Facebook's documentation to figure it out.  When you submit your
  changes, Drupal for Facebook will automatically set the callback URL
  and some other properties which help it work properly.

Troubleshooting:
---------------

Reread this file and follow instructions carefully.

Enable the devel module, http://drupal.org/project/devel.

Enable the "Drupal for Facebook Devel" module and add the block it
provides to the footer of your Facebook theme.

If you see "The page you requested was not found."  Make sure the
canvas page you specified agrees exactly with the canvas page assigned
by facebook.  Note also that facebook will make all letters lower case
even if you typed them upper.

Bug reports and feature requests may be submitted.  
Here's an idea: check the issue queue before you submit
http://drupal.org/project/issues/fb

If you do submit an issue, start the description with "I read the
README.txt from start to finish," and you will get a faster, more
thoughtful response.  Seriously, prove that you read this far.

Here is one way to set up your settings.php.  Add the PHP shown below
to the very end of your settings.php, and modify the paths accordingly
(i.e. where I use "sites/all/modules/fb", you might use
"profiles/custom/modules/fb").

<?php

/**
 * Drupal for Facebook settings.
 */

if (!is_array($conf))
  $conf = array();

$conf['fb_verbose'] = TRUE; // debug output
//$conf['fb_verbose'] = 'extreme'; // for verbosity fetishists.

// More efficient connect session discovery.
// Needed if supporting multiple apps.
$conf['fb_apikey'] = '123.....XYZ'; // Your connect app's apikey goes here.

// Enable URL rewriting (for canvas page apps).
include "sites/all/modules/fb/fb_url_rewrite.inc";

include "sites/all/modules/fb/fb_settings.inc"; // REQUIRED.

// end of settings.php
?>
