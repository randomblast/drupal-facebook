<?php
// $Id$
/**
 * @file
 * This file is a replacement for Drupal's session.inc.
 * Although not truly a replacement, as we include the default session.inc to do the heavy lifting.
 * In this file we set up some variables and functions to handle facebook-controlled sessions.
 */


// Default session handler functions.
require('includes/session.inc');

if (function_exists('fb_settings_session_helper')) {
  fb_settings_session_helper();
}

/**
 * In canvas pages, cookies do not work.  To enforce sessions, we "spoof" a
 * cookie (see fb_settings_session_helper()).  Sometimes drupal changes the
 * session id on us, we need to change it back, because the canvas page will
 * never honor the new value.
 *
 * This function is called from fb_exit();
 */
function fb_session_exit() {
  if ($id = fb_settings('fb_session_id_force')) {
    if ($id != session_id()) {
      // In case we are called after sess_write.
      //db_query("UPDATE {sessions} SET sid='%s' WHERE sid='%s'",
      //        array($id, session_id()));

      // We are called before sess_write.
      session_id($id);
    }
  }
}


