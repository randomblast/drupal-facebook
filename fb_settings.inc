<?php
  // The URL will look something like fb_cb/NID/fb_cb_type/TYPE
  // In the future, there may be additional parameters, like spoofing a session for anonymous visitors.
define('FB_SETTINGS_APP_NID', 'fb_cb');
define('FB_SETTINGS_PAGE_TYPE', 'fb_cb_type');
define('FB_SETTINGS_SESSION_KEY', 'fb_sess');

if (!is_array($conf))
  $conf = array();

// This allows us to test whether this file has been included
$conf['fb_settings_check'] = TRUE;

if ($nid = _fb_settings_parse(FB_SETTINGS_APP_NID))
  // Ensure unique session for facebook app.
  $conf['session_inc'] = dirname(__FILE__) . '/fb_session.inc';
										   
  if (isset($_REQUEST['fb_sig']) && isset($_REQUEST['fb_sig_in_canvas'])) {
  // It's a facebook callback to an FBML page.
  
  // Force clean URLs.  Looks better on facebook, and helps us know
  // what to expect Note that we set our facebook callback to end in
  // "?q=" so clean URLs will work in a facebook app whether or not
  // they work locally
  $conf['clean_url'] = 1;
  
  // Force the admin theme to be our facebook theme.
  // TODO: find a way to make this configurable.  I.e. to support iframe apps.
  $conf['admin_theme'] = 'fb_fbml';
  
  // Filters will behave differently on facebook, so we cannot share the cache_filter table.
  /*** NO LONGER NECESSARY XXX
  if (!is_array($db_prefix))
	if (isset($db_prefix)) 
	  $db_prefix = array('default' => $db_prefix);
	else
	  $db_prefix = array();
  if (!isset($db_prefix['cache_filter']))
	$db_prefix['cache_filter'] = 'fb_';
  ***/

  // Worth considering: use db_prefix to give facebook, or each app,
  // it's own set of users.
  
}

/**
 * Implementation of conf_url_rewrite
 * 
 * Defines the URL rewrite if not defined already.  If it is defined already,
 * or your using 118n modules, you should define a custom_url_rewrite in
 * settings.php which calls fb_canvas_url_rewrite in addition to any others
 * that need to be called.  The order will be important!
 */

if(!function_exists('custom_url_rewrite')) {
  function custom_url_rewrite($type, $path, $original) {
    return fb_settings_url_rewrite($type, $path, $original);
  }
}  


/**
 * Rewrite URLs for facebook canvas pages.
 * 
 * Ideally, this function would live in fb_canvas.module and only be
 * set when serving canvas pages.  However, it gets called before
 * modules are loaded.  So it must live here.
 */
function fb_settings_url_rewrite($type, $path, $original){
  //dpm(func_get_args(), 'fb_canvas_url_rewrite');
  //print("fb_canvas_url_rewrite($type, $path, $original)");
  $prefixes = array(FB_SETTINGS_APP_NID, FB_SETTINGS_PAGE_TYPE, FB_SETTINGS_SESSION_KEY);
  if ($type == 'source') {
    // See if this is a request for us.
    if (strpos($path, FB_SETTINGS_APP_NID . '/') === 0) {
	  // Too soon for arg() function.
      $args = explode('/', $path);
      while (count($args) && in_array($args[0], $prefixes)) {
        $key = array_shift($args);
        $value = array_shift($args);
        fb_settings($key, $value); // Store for use later.
      }
      if ($app_nid = fb_settings(FB_SETTINGS_APP_NID)) {
        if (count($args)) {
          $path = implode('/', $args); // remaining args
          $path = drupal_get_normal_path($path);
        }
        else
          // frontpage
          $path = drupal_get_normal_path(variable_get('site_frontpage', 'node'));
      }
    } 
  }
  else if ($type == 'alias') {
    $pre = '';
    
    // Prefix each known value to the URL
    foreach ($prefixes as $prefix) {
      if ($value = fb_settings($prefix))
	$pre .= $prefix . '/'. $value . '/';
    }
    $path  = $pre . $path;
  }
  
  return $path;  
}

function fb_settings($key, $value = NULL) {
  static $cache = array();
  if (isset($value)) {
    $cache[$key] = $value;
  }
  return $cache[$key];
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
	  // Found the value we're interested in
	  return $args[$i+1];
	$i = $i + 2;
      }
    }
  }
}


?>