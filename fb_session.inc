<?php

// We need to make the session_name unique to this app and we need to do it
// after settings.php is included and before drupal includes it's session.inc.
// Drupal gives us no easy way to do just this, however it does give us a way
// to override the entire session.inc file.  So, in fb_settings.inc, we use
// the session_inc variable to include this file instead of the normal
// session.inc.  At the end of this file, we include that original file.

if ($nid = _fb_settings_parse(FB_SETTINGS_APP_NID)) {
  // Session name taking into account the fb_app.  This is intended to
  // support links in iframes without the target parameter.  This
  // needs testing and possibly will not work.
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
  // Leave default session_name
}

// Important: edit the path where includes/session.inc is included so
// that it works for your system.
require_once('includes/session.inc');

?>