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
 * Helper function called when handing off from drupal-controlled session to facebook-controlled session.
 */
function fb_session_destroy() {
  global $user;
  session_destroy(); // Destroy local drupal session, not session controlled by facebook cookies.
  $user = drupal_anonymous_user();
}