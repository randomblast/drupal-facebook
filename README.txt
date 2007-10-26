This is an early development release of Drupal for Facebook.  Expect
things to change...

More information: http://www.drupalforfacebook.org

by Dave Cohen (dave@dave-cohen.com)

To install:

- Link or move the contents of ./themes/ into a Drupal themes directory.
  (The themes are so closely associated with the modules that it seemed appropriate to include them here, rather than download seperately.)

- Download the facebook-platform PHP code from developer.facebook.com.
  Extract it into this directory, so you have modules/facebook/facebook-application/
  Make sure you extract the client API for PHP4, and simplexml, as instructed by Facebook.  Note that even if you are running PHP5, Drupal for Facebook uses the PHP4 client API, and will continue to use it until Drupal supports PHP5.

- Edit your settings.php file (somewhere in the sites directory) to
include settings.inc (in this directory).  For example:

require_once "profiles/custom/modules/fb/settings.inc";

- Enable the Facebook modules via the drupal admin pages, as usual.
  You must enable at least fb.module and fb_app.module.
  You will probably want to enable most of the others.

- It is highly recommended that you enable clean URLs.
  If you don't, some links that drupal creates will not work properly on canvas pages.

- Create a Facebook Application (under "create content").
  You will have to create the application on Facebook.com first, to get the secret and api key fields.  Then fill those values into the form on Drupal.  It will suggest a callback URL for you to use.  Go back to the form on facebook and fill the callback URL field in.





