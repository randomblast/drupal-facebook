<?php

// We need to make the session_name unique to this app and we need to
// do it after settings.php is included and before drupal includes
// it's session.inc.  Drupal gives us no easy way to do this.

if ($nid = _fb_settings_parse_nid()) {
  // Session name taking into account the fb_app.  This is intended to
  // support links in iframes without the target parameter.  This
  // needs testing and possibly will not work.
  $sess = session_name();
  session_name("fb{$nid}{$sess}");
  
  if (isset($_REQUEST['fb_sig_session_key'])) {
	// If facebook provides a session key, us it.  Allows us to share
	// a session between FBML and iframe, and when forms are submitted
	// from FBML canvas pages.
	session_id("fb_{$nid}_".$_REQUEST['fb_sig_session_key']);
  }
}
else {
  // Leave default session_name
}

// Important: edit the path where includes/session.inc is included so
// that it works for your system.
require_once('includes/session.inc');