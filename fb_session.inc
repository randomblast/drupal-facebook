<?php

/**
 * Here we override Drupal's session management.  Actually, we try not
 * to change things, unless we're servicing a facebook app.  We
 * include drupal's session.inc at the end of this file so that
 * default behavior will take place.  But first we detect whether
 * we're handling a facebook app and if so we do a few special things.
 *
 * We need to handle several cases: FBML canvas pages, iframe canvas
 * pages, and facebook connect pages.
 *
 * In the FBML case, the request comes from facebook, not the user's
 * browser.  So we can't set cookies.  Instead we look to $_REQUEST
 * for facebook's session key.  If found, we muck with $_COOKIE
 * variables so that session.inc, when we include it later, will do
 * reasonable things.
 *
 * One weak point is FBML canvas pages where the user is not logged
 * in.  In this case facebook gives us no session state info.  This is
 * difficult for drupal to deal with.  Perhaps the best option is to
 * require login for application pages.  A must for any pages which
 * require session to work.
 *
 * Iframe canvas pages are a tricky case.  Here, we can set cookies on
 * the browser.  On the first iframe request, facebook will provide
 * additional session state info.  On subsequest requests, if iframe
 * links to a local url (without target=_top) we won't have the
 * facebook params.
 *
 * In general with iframes, the user could also be visiting the
 * regular website.  We don't want iframe sessions to compete with
 * regular sessions, so we change the session_name.
 *
 * Some browsers (Safari) will not accept the cookies we assign to an
 * iframe.  Setting $conf[fb_session_cookieless_iframe] attempts to
 * work around this.
 *
 * For Facebook Connect, we have to honor facebook's session state
 * info, so that when a user logs out of facebook, they are also
 * logged out of their connect session.  Also, we want to preserve
 * previous session state.  So for example if a user is already logged
 * into Drupal, we'll know that after they hit the connect button.
 */

$orig_session_name = session_name();
if (isset($_COOKIE[$orig_session_name]))
  $orig_session_id = $_COOKIE[$orig_session_name];
else
  $orig_session_id = '';

$nid = _fb_settings_parse(FB_SETTINGS_APP_NID);

if ($nid && isset($_REQUEST['fb_sig_api_key'])) {
  // Canvas page or event callback
  
  // If facebook provides a session key, us it.  Allows us to share
  // a session between FBML and iframe, and when forms are submitted
  // from FBML canvas pages.
  $new_session_name = "fb_canvas_{$nid}_" . $orig_session_name;
  if (isset($_REQUEST['fb_sig_session_key']))
    $new_session_id = "fb_canvas_{$nid}_" . $_REQUEST['fb_sig_session_key'];
  else if ($orig_session_id) {
    // When user is logged into facebook, but not authorized app, cookies are honored.  (confirm this???)
    $new_session_id = "fb_canvas_{$nid}_" . $orig_session_id;
  }
  else {
    // If we have no session (user not logged into facebook) all such users will share one session!!!
    $new_session_id = "fb_canvas_{$nid}_shared_session";
  }
  
  // Force url() to include the cookie-less session when in iframe
  if (variable_get('fb_session_cookieless_iframe', FALSE) &&
      isset($_REQUEST['fb_sig_in_iframe']) &&
      $_REQUEST['fb_sig_in_iframe']) {
    fb_settings(FB_SETTINGS_SESSION_KEY, $_REQUEST['fb_sig_session_key']);
  }
 }
 else if ($nid && variable_get('fb_session_cookieless_iframe', FALSE) && 
          ($sess_key = _fb_settings_parse(FB_SETTINGS_SESSION_KEY))) {
   // using sessionless iframes
   // similar logic to clause above, using session key in url path
   $new_session_id = "fb_canvas_{$nid}_" . $sess_key;
   $new_session_name = "fb_canvas_{$nid}_" . $orig_session_name;   
 }
 else {
   // Try to learn session key from cookies (Facebook Connect)
   
   $apikey = NULL;
   // Discover APIKEY by inspecting cookies.
   // This could be made more efficient by looking only for the primary apikey.  I hesitate because some sites may need to support multiple connect apps. (I.e. one for the website and other for resizeable iframes in canvas pages)
   foreach ($_COOKIE as $key => $value) {
     if ($pos = strpos($key, '_session_key')) {
       $apikey = substr($key, 0, $pos);
     }
   }
   
   if ($apikey && isset($_COOKIE[$apikey . '_ss'])) {
     // We're logged into Facebook Connect.
     // If fbConnect, we want to use another session id, so that if the
     // user logs out of facebook, they are also logged out of drupal.
     
     // Use globals to remember some values, for fb_connect.module to use.
     $GLOBALS['fb_connect_apikey'] = $apikey;
     
     // Rename the session id, so the Facebook Connect session is distinct from the original drupal session.
     $new_session_id = 'fb_connect_' . $_COOKIE[$apikey . '_session_key'];
     
   }
 }

if (isset($new_session_name)) {
  session_name($new_session_name);
 }

if (isset($new_session_id)) {
  if (!variable_get('fb_session_long_keys', TRUE)) {
    // Facebook appends user id, time and expiry info which is not necessary for uniqueness.  Here we truncate that information to ensure the sid fits in sessions table.
    $new_session_id = substr($new_session_id, 0, 64);
  }
  if ($new_session_id != $orig_session_id) {
    session_id($new_session_id);
    if (isset($GLOBALS['fb_connect_apikey'])) {
      // We can preserve the session state when going into fbconnect
      db_query("DELETE FROM {sessions} WHERE sid='%s'", $new_session_id);
      db_query("UPDATE {sessions} SET sid = '%s' WHERE sid = '%s'", $new_session_id, $orig_session_id);
    }
    
    // If we've changed the session id, disable drupal's caching
    $GLOBALS['conf']['cache'] = 0;
  }
 }
 else {
   // No session from facebook, so make sure we're not using an out of date one.
   if (strpos($orig_session_id, 'fb_connect') === 0) {
     // Old fbconnect session can be deleted
     db_query("DELETE FROM {sessions} WHERE sid='%s'", $orig_session_id);
     session_id(md5(uniqid(microtime()) . $_SERVER['REMOTE_ADDR'] . $_SERVER['HTTP_USER_AGENT']));
   }
   else if (strpos($orig_session_id, 'fb_connect') === 0) {
     // Canvas session should not be deleted as it could be a user visiting both the website and an iframe app
     if (!$nid)
       session_id(md5(uniqid(microtime()) . $_SERVER['REMOTE_ADDR'] . $_SERVER['HTTP_USER_AGENT']));     
   }
 }


if ($nid && !isset($_COOKIE[session_name()])) {
  // requests from facebook (FBML canvas pages) will not have cookies.
  // We want Drupal's session.inc to work properly, as if the session
  // came via cookie.
  if (!$_COOKIE || !count($_COOKIE))
    // Remember that cookies are actually disabled, some apps will want to display a message and/or redirect in this case.
    $_COOKIE['_fb_cookie_fake'] = TRUE;
  $_COOKIE[session_name()] = session_id();
 }


// Finally, include the logic of Drupal's session.inc
include('includes/session.inc');

?>