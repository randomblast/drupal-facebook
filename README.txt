Drupal for Facebook
-------------------

More information: 
http://www.drupalforfacebook.org, http://drupal.org/project/fb

Primary author and maintainer: Dave Cohen (http://www.dave-cohen.com/contact)

Branch: HEAD (version 2.x for Drupal 6.x)

Detailed documentation is available online
http://drupal.org/node/195035.  This file is probably more recent than
the online documentation!

If upgrading from a previous dev build or version, read the upgrade instructions:
http://drupal.org/node/761886

To install:
   
- Make sure you have an up-to-date PHP client from facebook.  You may
  download a tarball,
  http://svn.facebook.com/svnroot/platform/clients/packages/facebook-platform.tar.gz,
  or do this with subversion: 
  svn co http://svn.facebook.com/svnroot/platform/clients/php/trunk

- If you extract facebook-platform.tar.gz into your modules/fb
  directory, it should be found automatically.  If this is not the
  case, set a drupal variable, 'fb_api_file', to the location of
  facebook.php.  For example, if you checked the libs out of
  subversion, add 

  $conf['fb_api_file'] = 'sites/all/modules/fb/facebook-platform/facebook.php';

  near the end of your settings.php.
  If you downloaded the bundle, this might work:

  $conf['fb_api_file'] = 'sites/all/modules/fb/facebook-platform/php/facebook.php'; 

  Customize the path above as needed for your installation.

- If you intend to support canvas pages, install a Facebook-aware
  theme into one of Drupal's themes directories.  One example theme is
  provided in the 'themes' directory of this module, but Drupal will
  not find it there.  So, you must copy the entire theme directory to
  sites/all/themes, or another of Drupal's themes directories.  Use a
  symbolic link, rather than copy, if you intend to make no changes
  and you want updates to be easier.  Visit Site Building >> Themes so
  that Drupal will detect the new theme.

- If using Facebook Connect, your theme needs the following attribute
  in it's <html> tag: xmlns:fb="http://www.facebook.com/2008/fbml" See
  http://www.drupalforfacebook.org/node/1106.  This also applies to
  themes used for iframe canvas pages.

- Edit your settings.php file (sites/default/settings.php, depending
  on your install) to include fb_settings.inc (in this directory).
  For example, add this at the very end of your settings.php:

  require_once "sites/all/modules/fb/fb_settings.inc";

  (Or whatever path is appropriate, could be
  "profiles/custom/modules/fb/fb_settings.inc")

  See the end of this file for other configuration you may add to
  settings.php.

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

If you see FBML Error (line 62): illegal tag "body" under "fb:canvas",
or many "CSS Errors", visit Administer >> Site Building >> Themes.
Make sure you see fb_fbml in the list.  Enable the fb_fbml theme.

If you _still_ see CSS Errors or HTML Errors, most likely the fb_fbml
theme is still not being used.  This can happen if the theme is
initialized before fb.module is initialized.  To avoid this, don't
have any code that initializes a theme at the top level of your
.module files, put that code in hook_init instead.  And if still
necessary, lower the weight of the fb module in your system table.
See http://drupal.org/node/187868

If you see "The page you requested was not found."  Make sure the
canvas page you specified agrees exactly with the canvas page assigned
by facebook.  Note also that facebook will make all letters lower case
even if you typed them upper.

The Facebook client libraries has bugs if you do not have the JSON
extension for PHP installed, see
http://us.php.net/manual/en/json.requirements.php.  And tell facebook
their client libs suck at
http://bugs.developers.facebook.com/show_bug.cgi?id=4351.

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
//$conf['fb_debug'] = TRUE; // Output from facebook's client libs.
//$conf['fb_api_file'] = 'facebook-platform/php/facebook.php'; // default is 'facebook-platform/php/facebook.php'

/**
 * Enable Drupal for Facebook.  
 * Sets up custom_url_rewrite and session handling required for 
 * canvas pages and Facebook Connect.
 */

// More effiecent connect session discovery.
$conf['fb_connect_primary_apikey'] = '123.....XYZ'; // Your app's apikey goes here.

include "sites/all/modules/fb/fb_settings.inc";


?>
