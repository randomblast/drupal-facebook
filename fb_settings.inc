<?php
// The URL prefix will look something like fb_cb/NID.
// Or in complex cases fb_cb/NID/fb_cb_type/TYPE, or even fb_cb/NID/fb_sess/SESSION.
define('FB_SETTINGS_APP_NID', 'fb_cb');
define('FB_SETTINGS_PAGE_TYPE', 'fb_cb_type');
define('FB_SETTINGS_SESSION_KEY', 'fb_sess');

// NOTE: can't use variable_set() -- database.inc hasn't loaded yet, so db_query() in variable_set() fails
if (!is_array($conf))
  $conf = array();

// This allows us to test whether this file has been included.
$conf['fb_settings_check'] = TRUE;

// Ensure unique session for facebook app.
$conf['session_inc'] = dirname(__FILE__) . '/fb_session.inc';

if ($nid = _fb_settings_parse(FB_SETTINGS_APP_NID))
  // We're providing a canvas page or connect callback.

  if (isset($_REQUEST['fb_sig']) && isset($_REQUEST['fb_sig_in_canvas'])) {
  // It's a facebook callback to an FBML page.
  
  // Force clean URLs.  Necessary for canvas pages.
  $conf['clean_url'] = 1;
  
  // Force the admin theme to be our facebook theme.
  // TODO: find a way to make this configurable.  I.e. to support iframe apps.
  $conf['admin_theme'] = 'fb_fbml';
  
}

/**
 * Helper function to learn values prefixed to URL.
 */
function fb_settings($key, $value = NULL) {
  static $cache = array();
  if (isset($value)) {
    $cache[$key] = $value;
  }
  return isset($cache[$key]) ? $cache[$key] : NULL;
}

/**
 * Returns a list of the values which we prepend to paths when rewriting urls.
 */
function _fb_settings_url_rewrite_prefixes() {
  static $prefixes;
  if (!isset($prefixes)) {
    $prefixes = array(
      FB_SETTINGS_APP_NID, 
      FB_SETTINGS_PAGE_TYPE, 
      FB_SETTINGS_SESSION_KEY,
    );
  }
  return $prefixes;
}

/**
 * Parse a setting from the URL.  This may be called before
 * custom_url_rewrite, so we can't count on fb_settings() to return the value.
 * For internal use only (see fb_session.inc).
 */
function _fb_settings_parse($key) {
  if (isset($_GET['q'])) {
    $path = $_GET['q'];
    $pos = strpos($path, $key . '/');
    if ($pos !== FALSE) {
      // Too soon for arg() function.
      $args = explode('/', $path);
      $i = 0;
      while (isset($args[$i]) && isset($args[$i+1])) {
	if ($args[$i] == $key)
	  // Found the value we're interested in.
	  return $args[$i+1];
	$i = $i + 2;
      }
    }
  }
}

