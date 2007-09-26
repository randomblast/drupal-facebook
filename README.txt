This is an early development release of Drupal for Facebook.  Expect
things to change...

More information: http://www.drupalforfacebook.org

by Dave Cohen (dave@dave-cohen.com)

To install:

- Download the facebook-platform PHP code from developer.facebook.com.
  Extract it into this directory, so you have modules/facebook/facebook-application/

- Edit your settings.php file (somewhere in the sites directory) to
include settings.inc (in this directory).  For example:

require_once "profiles/custom/modules/facebook/settings.inc";

- Enable the Facebook modules via the drupal admin pages, as usual.
  You must enable at least fb.module and fb_app.module.

- Create a Facebook Application (under "create content").
  You will have to create the application on Facebook.com first, to get the secret and api key fields.


