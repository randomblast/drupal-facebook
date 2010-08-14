<?php
// $Id$
/**
 * @file
 * Performs custom url rewriting for Drupal for Facebook.
 *
 * Historically, these modules prefixed all facebook callbacks with values
 * that could be parsed from the url.  In particular we learned which
 * application was being used.  And in advanced cases, mostly for iframe apps,
 * we learned the "page type" and session key as well.
 *
 * Now, we learn these by inspecting 'fb_sig' params (canvas pages) and
 * cookies (facebook connect).  So most users will not need these functions.
 * They are available here for compatability and to support those more
 * advanced cases where they may still be necessary.
 *
 * If your application needs these, include this file in your settings.php.
 * I.e. the end of your settings.php might look like:
 *
 * include 'sites/all/modules/fb/fb_url_rewrite.inc'; // This on only if necessary.
 * include 'sites/all/modules/fb/fb_settings.inc'; // Always include this.
 */
   
/**
 * Returns a list of the values which we prepend to paths when rewriting urls.
 */
function _fb_settings_url_rewrite_prefixes() {
  static $prefixes;
  if (!isset($prefixes)) {
    $prefixes = array(
      FB_SETTINGS_CB,
      FB_SETTINGS_CB_TYPE, 
      FB_SETTINGS_CB_SESSION,
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




//// URL Management

// We define our custom_url rewrite function here, and not in
// fb_canvas.module, because custom_url_rewrite_inbound() must be defined
// *before* drupal_init_path(), which is called during DRUPAL_BOOTSTRAP_PATH,
// *before modules are loaded.  Set $conf['fb_url_rewrite'] = FALSE in
// *settings.php to disable this behavior.  That is safe for sites
// *implementing facebook connect, but not canvas pages or profile tabs.

// Enable by default.
if (!isset($conf['fb_url_rewrite']))
  $conf['fb_url_rewrite'] = TRUE;

/**
 * Define custom_url_rewrite_inbound() if not defined already.
 */
if (!function_exists('custom_url_rewrite_inbound') && $conf['fb_url_rewrite']) {
  function custom_url_rewrite_inbound(&$result, $path, $path_language) {
    fb_url_inbound_alter($result, $path, $path_language);
  }
}

/**
 * Define custom_url_rewrite_outbound() if not defined already.
 */
if (!function_exists('custom_url_rewrite_outbound') && $conf['fb_url_rewrite']) {
  function custom_url_rewrite_outbound(&$path, &$options, $original_path) {
    if (function_exists('fb_canvas_url_outbound_alter'))
      fb_canvas_url_outbound_alter($path, $options, $original_path);
    fb_url_outbound_alter($path, $options, $original_path);
  }
}

/**
 * Implementation of hook_url_outbound_alter().
 */
function fb_url_outbound_alter(&$path, &$options, $original_path) {
  //dpm(func_get_args(), 'fb_settings_url_rewrite_outbound'); // debug
  $pre = '';
  
  // Prefix each known value to the URL
  foreach (_fb_settings_url_rewrite_prefixes() as $prefix) {
    if ($value = fb_settings($prefix))
      $pre .= $prefix . '/'. $value . '/';
  }
  $path  = $pre . $path;
  
  return $path;
}

/**
 * Implementation of hook_url_inbound_alter().
 *
 * Rewrite URLs for facebook canvas pages, and connect callbacks.
 * 
 */
function fb_url_inbound_alter(&$result, $path, $path_language) {
  //dpm(func_get_args(), 'fb_settings_url_rewrite_outbound'); // debug
  // See if this is a request for us.
  if (strpos($path, FB_SETTINGS_CB . '/') === 0) {
    // Too soon for arg() function.
    $args = explode('/', $path);
    while (count($args) && in_array($args[0], _fb_settings_url_rewrite_prefixes())) {
      $key = array_shift($args);
      $value = array_shift($args);
      fb_settings($key, $value); // Store for use later.
    }
    if (fb_settings(FB_SETTINGS_CB)) {
      if (count($args)) {
        $path = implode('/', $args); // remaining args
        $alias = drupal_lookup_path('source', $path, $path_language); //can't use drupal_get_normal_path, it calls custom_url_rewrite_inbound
        if ($alias) 
          $path = $alias;
      }
      else {
        // frontpage
        $path = variable_get('site_frontpage', 'node');
        $alias = drupal_lookup_path('source', $path, $path_language);
        if ($alias) 
          $path = $alias;
        $_REQUEST['destination'] = $path; //required workaround for compatibility with Global Redirect module, best practice?
      }
    }
  }
  else { //resolve aliases for non-fb-callbacks
    $alias = drupal_lookup_path('source', $path, $path_language);
    if ($alias) 
      $path = $alias;
  }

  $result = $path;
}

