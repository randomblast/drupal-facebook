<?php
// $Id$
/**
 * @file
 * This file is to be included from your sites/.../settings.php file.
 * 
 * In this code we set up special session management and url
 * rewriting.  These things must be done before modules are loaded, so
 * the code is here instead of fb.module. And that is why this
 * must be include from settings.php.
 * 
 */


// Each of these are things we can learn and store in fb_settings().
// Historically, the 'CB' values were learned by url prefixing.  Now optional.
define('FB_SETTINGS_CB', 'fb_cb');
define('FB_SETTINGS_CB_TYPE', 'fb_cb_type');
define('FB_SETTINGS_CB_SESSION', 'fb_sess');

// These values learned via $_REQUEST['fb_sig_...'] parameters or Facebook
// Connect cookies.
define('FB_SETTINGS_SESSION_KEY', 'session_key');
define('FB_SETTINGS_APIKEY', 'apikey');
define('FB_SETTINGS_TYPE', 'type'); // page type not same as cb type
define('FB_SETTINGS_LABEL', 'label');
define('FB_SETTINGS_CHECK', 'fb_settings_check');

// Possible values for settings.
define('FB_SETTINGS_TYPE_CANVAS', 'canvas');
define('FB_SETTINGS_TYPE_CONNECT', 'connect');


/**
 * Helper function to remember values as we learn them.
 */
function fb_settings($key, $value = NULL) {
  static $cache = array();
  if (isset($value)) {
    $cache[$key] = $value;
  }
  return isset($cache[$key]) ? $cache[$key] : NULL;
}



// Can't use variable_set() -- database.inc hasn't loaded yet.
if (!is_array($conf))
  $conf = array();


// Force some variables for canvas pages.
if (isset($_REQUEST['fb_sig']) && 
    isset($_REQUEST['fb_sig_in_canvas'])) {
  // It's a facebook callback to an FBML page.

  // Force clean URLs.  Necessary for canvas pages.
  $conf['clean_url'] = 1;
  
  // Force the admin theme to be our facebook theme.
  // TODO: find a way to make this configurable.  I.e. to support iframe apps.
  $conf['admin_theme'] = 'fb_fbml';
}


/**
 * Forces the session_name and session_id to be appropriate when Facebook controls the session.
 * Call this function from custom session_inc files, before session_start() is called.
 */
function fb_settings_session_helper() {
  if (isset($_REQUEST['fb_sig_api_key'])) {
    // It's a canvas page or event callback.
    fb_settings(FB_SETTINGS_APIKEY, $_REQUEST['fb_sig_api_key']);
    fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CANVAS);
    
    if (isset($_REQUEST['fb_sig_session_key'])) {
      // User has authorized the app, facebook controls session.
      fb_settings(FB_SETTINGS_SESSION_KEY, $_REQUEST['fb_sig_session_key']);
    }
    elseif (isset($_REQUEST['_fb_fromhash'])) {
      // _fb_fromhash is a cryptically named parameter sometimes appended to URLs on canvas pages.
      // If present, it can be used as a session for users who have not even authorized the app.
      fb_settings(FB_SETTINGS_SESSION_KEY, $_REQUEST['_fb_fromhash']);
    }
  }
  elseif (variable_get('fb_session_cookieless_iframe', FALSE) &&
          function_exists('_fb_settings_parse') &&
          ($session_key = _fb_settings_parse(FB_SETTINGS_SESSION_KEY))) {
    // In special cases we embed the session key in the URL.
    // This is one way to force a session when a browser will not accept a cookie from an iframe.
    if ($label = _fb_settings_parse(FB_SETTINGS_CB)) {
      if ($apikey = db_result(db_query("SELECT apikey FROM {fb_app} WHERE label='%s'", array($label)))) {
        fb_settings(FB_SETTINGS_APIKEY, $apikey);
        fb_settings(FB_SETTINGS_LABEL, $label);
        fb_settings(FB_SETTINGS_SESSION_KEY, $session_key);
        fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CANVAS);
      }
    }
  }
  else {
    // Were not in a canvas page.
    // We might be in a facebook connect page.  We have to inspect cookies to make sure.

    $session_key = '';
    
    // TODO: make this more efficient.
    foreach ($_COOKIE as $key => $value) {
      if ($pos = strpos($key, '_session_key')) {
        $apikey = substr($key, 0, $pos);
        $session_key = $value;
        break;
      }
    }
    if (isset($apikey)) {
      fb_settings(FB_SETTINGS_APIKEY, $apikey);
      fb_settings(FB_SETTINGS_SESSION_KEY, $session_key);
      fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CONNECT);
    }
  }

  // By default Drupal will name the session based on the $cookie_domain.
  // When facebook controls the session, we need a different name.
  if (!isset($_REQUEST['fb_session_no']) && // (a way to override this)
      fb_settings(FB_SETTINGS_APIKEY) && fb_settings(FB_SETTINGS_TYPE)) {
    // Set session name differently for each app.
    //session_name('SESS' . fb_settings(FB_SETTINGS_TYPE) . md5(fb_settings(FB_SETTINGS_APIKEY))); // not sufficient!
    // In fb connect, one user may log out of facebook, and another log in using same browser.  We never get a logout event! For this case we must make session names different.
    session_name('SESS' . fb_settings(FB_SETTINGS_TYPE) . md5(fb_settings(FB_SETTINGS_APIKEY) . fb_settings(FB_SETTINGS_SESSION_KEY))); // unique to session, if known.
    
    if (fb_settings(FB_SETTINGS_TYPE) == FB_SETTINGS_TYPE_CANVAS && fb_settings(FB_SETTINGS_SESSION_KEY)) {
      // Spoof a cookie, and make it the same for both FBML and iframe canvas pages.
      $session_id = 'fb_canvas_' . md5(fb_settings(FB_SETTINGS_APIKEY) . fb_settings(FB_SETTINGS_SESSION_KEY));
      session_id($session_id);
      $_COOKIE[session_name()] = $session_id;
      fb_settings('fb_session_id_force', $session_id);
    }
  }

  // Also disable Drupal's caching, because a 'connected' user is not truly anonymous.
  if (fb_settings(FB_SETTINGS_SESSION_KEY)) {
    $GLOBALS['conf']['cache'] = 0;
  }
}

// We will use session names to ensure a unique session for
// facebook-controlled sessions.  That is we will keep our cookies seperate
// from normal drupal session cookies.  The trick is we must do this after
// conf_init() (where drupal sets the session name, during
// DRUPAL_BOOTSTRAP_CONFIGURATION), but before session_start() (during
// DRUPAL_BOOTSTRAP_SESSION).  The only way to do this is to define our own
// session.inc.

// Ensure unique session for facebook app.
if (!isset($conf['session_inc'])) {
  $conf['session_inc'] = dirname(__FILE__) . '/fb_session.inc';
}




//// URL rewriting.

// This is now optional.  If a site implements only facebook connect and not canvas pages, this is not required.
// This code WILL GO AWAY.  Its here now to make upgrades easier.

//include (dirname(__FILE__) . '/fb_url_rewrite.inc'); // will go away XX
