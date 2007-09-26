<?php

if (isset($_REQUEST['fb_sig'])) {
  // It's a facebook callback!

  // Can't rely on cookies for session data, use what fb passes in.
  // The following settings make drupal's session.inc work for us.
  // Would be cleaner, perhaps, to use our own session.inc, but we're lazy. ;)

  // The cookie domain and session name get munged by bootstrap.inc
  // AFTER we get called here.  So we have to be extra careful to use
  // the same values they end up with.  In other words, the slightest change
  // to bootstrap.inc could break this (as it did from drupal 5.1 to
  // drupal 5.2) and its a real pain to track down.
  global $cookie_domain;
  if (!$cookie_domain) {
    // If the $cookie_domain is empty, try to use the session.cookie_domain.
    $cookie_domain = ini_get('session.cookie_domain');
  }
  if ($cookie_domain) {
    // If the user specifies the cookie domain, also use it for session name.
    $session_name = $cookie_domain;
  }
  else {
    // Otherwise use $base_url as session name, without the protocol
    // to use the same session identifiers across http and https.
    list( , $session_name) = explode('://', $base_url, 2);
    // We try to set the cookie domain to the hostname.
    // We escape the hostname because it can be modified by a visitor.
    if (!empty($_SERVER['HTTP_HOST'])) {
      $cookie_domain = check_plain($_SERVER['HTTP_HOST']);
    }
  }
  // Strip leading periods, www., and port numbers from cookie domain.
  $cookie_domain = ltrim($cookie_domain, '.');
  if (strpos($cookie_domain, 'www.') === 0) {
    $cookie_domain = substr($cookie_domain, 4);
  }
  $cookie_domain = explode(':', $cookie_domain);
  $cookie_domain = '.'. $cookie_domain[0];
  
  // Now that we have the proper name of the cookie domain, we have to
  // recognize that facebook is not sending cookies.  So we use the fb
  // session key instead.

  if (isset($_REQUEST['fb_sig_session_key']))
    $sk = 'fb_' . $_REQUEST['fb_sig_session_key'];
  else
    $sk = 'fb_not_logged_in';

  //$cookie_domain = '.'.$sk.'.'.$cookie_domain; // ensure it's unique
  $session_name = $cookie_domain;
  $session_name_after_bootstrap = 'SESS'. md5($session_name); // This has to be exactly the same as bootstrap.inc!

  // Now finally, the line that will trick session.inc into working for us:
  $_COOKIE[$session_name_after_bootstrap] = $sk;

  //print("set cookie[" . $session_name_after_bootstrap . "] to $sk.");  // debug
  // We also need the session_id set to something, so that form token validation can work.
  session_id($sk);



  // Force clean URLs.  Looks better on facebook, and helps us know
  // what to expect Note that we set our facebook callback to end in
  // "?q=" so clean URLs will work in a facebook app whether or not
  // they work locally
  if (!is_array($conf))
	$conf = array();
  $conf['clean_url'] = 1;

  // Force the admin theme to be our facebook theme.
  // TODO: find a way to make this configurable.  I.e. to support iframe apps.
  $conf['admin_theme'] = 'fb_fbml';

  // Filters will behave differently on facebook.
  if (!is_array($db_prefix))
	$db_prefix = array();
  if (!isset($db_prefix['cache_filter']))
	$db_prefix['cache_filter'] = 'fb_';

  // Worth considering: use db_prefix to give facebook, or each app,
  // it's own set of users.

 }

// Perhaps change other ini session settings here???

function custom_url_rewrite_XXX($op, $result, $path, $path_language = NULL) {
  global $fb_app;
  static $invoked = false;
  
  //watchdog("settings.inc", "custom_url_rewrite($op, $result, $path)");
  if ($_REQUEST['fb_sig'] && !$invoked && $op == 'source') {
	//watchdog('custom_url_rewrite', "request is " . dpr($_REQUEST, 1));
	$fb_app = db_fetch_object(db_query("SELECT * FROM {fb_app} WHERE label='%s'",
									   arg(0)));
	if ($fb_app) {
	  $result = substr($path, strlen(arg(0))+1);
	  if (!$result)
		$result = variable_get('site_frontpage', 'node');
	}
	session_id($_REQUEST['fb_sig_session_key']);
	$invoked = true;
  }
  else if ($fb_app && $op == 'alias') {
	if ($path && $path != $fb_app->label)
	  $result = $fb_app->label . '/'. $path;
	else 
	  $result = $fb_app->label;
  }

  return $result;
}
?>
