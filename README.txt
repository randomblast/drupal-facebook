This is an early development release of Drupal for Facebook.  Expect
things to change...

Installation and setup documentation now available online
http://drupal.org/node/195035.  (Read this document first, as it
contains the most up-to-date information.  Then consult the online
documentation for more detailed instructions.)

More information: http://www.drupalforfacebook.org, http://drupal.org/project/fb

by Dave Cohen (http://www.dave-cohen.com/contact)

To install:

- Link or move the contents of ./themes/ into a Drupal themes
  directory.  (The themes are so closely associated with the modules
  that it seemed appropriate to include them here, rather than
  download seperately.)

- Download the facebook-platform PHP code from developer.facebook.com.
  Extract it into the 'modules/fb' directory, so you have
  modules/fb/facebook-application/

wget http://developers.facebook.com/clientlibs/facebook-platform.tar.gz
tar xvzf facebook-platform.tar.gz
  
  Extract the client API for PHP4, and simplexml, as instructed by
  Facebook.  Or, if you want to use PHP5 API, set
  $conf['fb_use_php4_api'] = FALSE in your settings.php.  Note that
  you may encounter uncaught exceptions using the PHP5 API.

- Edit your settings.php file (sites/default/settings.php, depending
  on your install) to include settings.inc (in this directory).  For
  example:

require_once "profiles/custom/modules/fb/fb_settings.inc";

  (Or whatever path is appropriate, could be
  "sites/all/module/fb/fb_settings.inc")

- Enable the Facebook modules via the drupal admin pages, as usual.
  You must enable at least fb.module and fb_app.module.  You will
  probably want to enable fb_user and more of the modules as your App
  needs them.

- It is highly recommended that you enable clean URLs.  If you don't,
  some links that drupal creates will not work properly on canvas
  pages.

- Create a Facebook Application (under "create content").  You will
  have to create the application on Facebook.com first, to get the
  secret and api key fields.  Then fill those values into the form on
  Drupal.  It will suggest a callback URL for you to use.  Go back to
  the form on facebook and fill the callback URL field in.





Troubleshooting:
---------------

If you get an error along the lines of "FacebookRestClient" not found,
check the facebook-platform/client/facebook.php for an include like
so:

include_once $_SERVER['PHP_ROOT'].'/lib/api/client/php_1_1/facebookapi_php5_restlib.php';

You will need to either install faceboook-platform in this location, or patch the file to read more like:

require_once 'facebookapi_php5_restlib.php';



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
