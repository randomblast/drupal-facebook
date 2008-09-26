Drupal for Facebook
-------------------

More information: http://www.drupalforfacebook.org, http://drupal.org/project/fb

Primary author and maintainer: Dave Cohen (http://www.dave-cohen.com/contact)

Version: HEAD (version 2.x for Drupal 5.x)
Please read http://drupal.org/node/288721#version and confirm that
you are using the correct version.

IMPORTANT: THIS VERSION IS ONLY PARTIALLY CONVERTED FOR THE NEW API.
USE THIS VERSION ONLY IF YOU ARE CAPABLE OF FIXING ISSUES AS YOU
ENCOUNTER THEM AND SUBMITTING PATCHES TO THE ISSUE QUEUE.  OTHERWISE,
FOR THE TIME-BEING, USE VERSION 1.x.

Installation and setup documentation now available online
http://drupal.org/node/195035.  (Read this document first, as it
contains the most up-to-date information.  Then consult the online
documentation for more detailed instructions.)

To install or upgrade from an earlier version of Drupal for Facebook:
   
- This version requires a newer version of the Facebook API.  As of
  this writing, you can get this via Subversion at
  http://svn.facebook.com/svnroot/platform/clients/php/branches/redesign-changes

- Once you have the new API client, place it somewhere where it can be
  loaded, and set a drupal variable, 'fb_api_file', to the location of
  facebook.php.  For example, first checkout redesign-changes in the
  directory where this file is located.  Then, add 
  $conf['fb_api_file'] = 'redesign-changes/facebook.php';
  near the end of your settings.php file.
  You will get a cryptic error message until you done this correctly.

- Install a Facebook-aware theme into one of Drupal's themes
  directories.  One example theme is provided in the 'themes'
  directory of this module, but Drupal will not find it there.  So,
  you must copy the entire theme directory to sites/all/themes, or
  another of Drupal's themes directories.  Use a symbolic link, rather
  than copy, if you intend to make no changes and you want updates to
  be easier.  Visit Site Building >> Themes so that Drupal will detect
  the new theme.

- Edit your settings.php file (sites/default/settings.php, depending
  on your install) to include fb_settings.inc (in this directory).  For
  example, add this at the very end of your settings.php:

  require_once "sites/all/modules/fb/fb_settings.inc";

  (Or whatever path is appropriate, could be
  "profiles/custom/modules/fb/fb_settings.inc")

- Enable the Facebook modules via the drupal admin pages, as usual.
  You must enable at least fb.module and fb_app.module.  You will
  probably want to enable fb_user and more of the modules as your App
  needs them.

- You must enable clean URLs.  If you don't, some links that drupal
  creates will not work properly on canvas pages.

- Create an application on Facebook, currently at
  http://www.facebook.com/developers/editapp.php?new.  Fill in just
  the minimum required to get an apikey and secret.

- Go to Create Content >> Facebook Application.  Use the apikey and
  secret that Facebook has shown you.  If you have any trouble with
  the other fields, use Facebook's documentation to figure it out.
  Once you submit your changes, you'll be shown important information,
  like a callback URL and so on.  Return to the application settings
  form on Facebook, and fill in all the values Drupal for Facebook has
  shown you.

- If upgrading from an earlier Drupal for Facebook, revisit all of the
  Facebook Application nodes.  Edit each one, look over all the
  settings as many have changed, especially the choices under user
  management.  You'll have to submit your changes in order for your
  application to work properly with the new Facebook API.

- This patch is required for Drupal 5.x: http://drupal.org/node/241878

Troubleshooting:
---------------

Reread this file and follow instructions carefully.

Enable the devel module, http://drupal.org/project/devel.

Enable the fb_devel module and add the block it provides to the footer
of your Facebook theme.

If you see FBML Error (line 62): illegal tag "body" under "fb:canvas",
or many "CSS Errors", visit ?q=admin/build/themes.  Make sure you see
fb_fbml in the list.  It is not necessary to enable fb_fml, however
you must hit submit (even with no changes) just to get Drupal to
refresh its list of where the themes are located.

If you _still_ see CSS Errors or HTML Errors, most likely the fb_fbml
theme is still not being used.  This can happen if the theme is
initialized before fb.module is initialized.  To avoid this, don't
have any code directly in your .module files, put that code in
hook_init instead.  And if still necessary, lower the weight of the fb
module in your system table.  See http://drupal.org/node/187868

If you see "The page you requested was not found."  Make sure the
canvas page you specified agrees exactly with the canvas page assigned
by facebook.  Note also that facebook will make all letters lower case
even if you typed them upper.

Bug reports and feature requests may be submitted.  
Here's an idea: check the issue queue before you submit
(http://drupal.org/project/issues/fb).  

If you do submit an issue, start the description with "I read the
README.txt from start to finish," and you will get a faster, more
thoughtful response.
