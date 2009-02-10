<?php

// We need to make the session_name unique to this app and we need to do it
// after settings.php is included and before drupal includes it's session.inc.
// Drupal gives us no easy way to do just this, however it does give us a way
// to override the entire session.inc file.  So, in fb_settings.inc, we use
// the session_inc variable to include this file instead of the normal
// session.inc.  At the end of this file, we include that original file.

if ($nid = _fb_settings_parse(FB_SETTINGS_APP_NID)) {
  // Here, we are in canvas page, either fbml or iframe.
  
  // Session name taking into account the fb_app.  This is intended to
  // support links in iframes without the target parameter.  
  // (That needs testing.)
  $sess = session_name();
  session_name("fb{$nid}{$sess}");
  
  if (isset($_REQUEST['fb_sig_session_key'])) {
    // If facebook provides a session key, us it.  Allows us to share
    // a session between FBML and iframe, and when forms are submitted
    // from FBML canvas pages.
    session_id("fb_{$nid}_" . $_REQUEST['fb_sig_session_key']);
  }
  else if (variable_get('fb_session_cookieless_iframe', FALSE) && 
	   ($sess_key = _fb_settings_parse(FB_SETTINGS_SESSION_KEY))) {
    // Iframes use cookie-less sessions.  The session key is embedded in the URL.
    // Prepend "fb_{nid}_" so that regular drupal sessions are not subject to a potential security hole.
    session_id("fb_{$nid}_" . $sess_key);
  }
  
  // Force url() to include the cookie-less session when in iframe
  if (variable_get('fb_session_cookieless_iframe', FALSE) &&
      isset($_REQUEST['fb_sig_in_iframe']) &&
      $_REQUEST['fb_sig_in_iframe']) {
    fb_settings(FB_SETTINGS_SESSION_KEY, $_REQUEST['fb_sig_session_key']);
  }
  
  // requests from facebook (FBML canvas pages) will not have cookies.
  // We want Drupal's session.inc to work properly, as if the session
  // came via cookie.
  if (!isset($_COOKIE[session_name()])) {
    if (!$_COOKIE || !count($_COOKIE))
      // Remember that cookies are actually disabled, some apps will want to display a message and/or redirect in this case.
      $_COOKIE['_fb_cookie_fake'] = TRUE;
    $_COOKIE[session_name()] = session_id();
  }
}
else {
  // Here, we are in a normal page request, possibly using Facebook Connect
  // If fbConnect, we want to use another session id, so that if the
  // user logs out of facebook, they are also logged out of drupal.
  
  $apikey = NULL;
  // Discover APIKEY by inspecting cookies.
  foreach ($_COOKIE as $key => $value) {
    if (strpos($key, 'fbsetting_') === 0) {
      $apikey = substr($key, 10); // strlen('fbsetting');
    }
  }

  if ($apikey && isset($_COOKIE[$apikey . '_session_key'])) {
    // We're logged into Facebook Connect.

    // Use globals to remember some values, for fb_connect.module to use.
    $GLOBALS['fb_connect_apikey'] = $apikey;
    $GLOBALS['fb_connect_old_session_name'] = session_name();
    $GLOBALS['fb_connect_old_session_id'] = session_id();

    // Now rename the session id, so the Facebook Connect session is distinct from the original drupal session.
    $sess = session_name();
    session_name("fb_connect_{$sess}");
    session_id('fb_connect_' . $_COOKIE[$apikey . '_session_key']);
  }
}

// Finally, include the logic of Drupal's session.inc
require_once('includes/session.inc');

?>