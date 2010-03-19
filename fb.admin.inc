<?php
define('FB_ADMIN_HOOK', 'fb_admin');

define('FB_ADMIN_OP_SET_PROPERTIES', 'set_props'); // build props for admin.setAppProperties
define('FB_ADMIN_OP_LIST_PROPERTIES', 'list_props'); // list of known properties for admin.getAppProperties


// Pages which may or may not be supported by the app implementation.
define('FB_ADMIN_OP_LOCAL_PROPERTIES', 'fb_admin_props');
define('FB_ADMIN_LOCAL_EDIT', 'fb_admin_prop_edit');
define('FB_ADMIN_LOCAL_DELETE', 'fb_admin_prop_edit');
define('FB_ADMIN_LOCAL_VIEW', 'fb_admin_prop_view');

function fb_admin_page() {
  drupal_set_title(t('Drupal for Facebook'));

  $apps = fb_get_all_apps();

  if (count($apps)) {
    $header = array(t('Label'), t('About'), t('Canvas'), t('Local Settings'), t('Remote Settings'));
    
    foreach ($apps as $fb_app) {
      // Get properties from facebook.
      fb_admin_get_app_properties($fb_app);
      
      // Get properties from local modules.
      $local_props = fb_invoke(FB_ADMIN_OP_LOCAL_PROPERTIES, array('fb_app' => $fb_app), array(), FB_ADMIN_HOOK);
      
      $row = array();
      // Title.
      $row[] = $fb_app->label . ($fb_app->status ? '' : ' ' . t('(disabled!)'));
      
      // About.
      $row[] = l($fb_app->application_name, 'http://www.facebook.com/apps/application.php?id=' . $fb_app->id);
      
      // Canvas Page.
      if ($fb_app->canvas_name != $fb_app->canvas) {
        drupal_set_message(t('Canvas page for %label is out of sync!  Facebook believes it is %fbcanvas, while our database believes %canvas.  Edit and save the application to remedy this.', array('%label' => $fb_app->label, '%fbcanvas' => $fb_app->canvas_name, '%canvas' => $fb_app->canvas)), 'error');
      }
      if ($fb_app->canvas) {
        $row[] = l($fb_app->canvas, 'http://apps.facebook.com/' . $fb_app->canvas);
      }
      else {
        $row[] = t('n/a');
      }
      // Local Settings
      $links = array();
      if (isset($local_props[FB_ADMIN_LOCAL_EDIT])) {
        $links[] = array('title' => t('edit'), 'href' => $local_props[FB_ADMIN_LOCAL_EDIT]);
      }
      $row[] = theme('links', $links);
      
      
      // Remote Settings
      $row[] = l($fb_app->id, 'http://www.facebook.com/developers/editapp.php?app_id=' . $fb_app->id);
      
      $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
  }
  else {
    $message = t('You must create an app first!');
    drupal_set_message($message, 'error');
    $output = $message;
  }
  return $output;
}

/**
 * Get properties from Facebook.  Fills in the data that we need to
 * know by querying facebook.
 */
function fb_admin_get_app_properties(&$fb_app) {
  static $cache;
  static $props_map;
  if (!isset($cache)) {
    $cache = array();
    // http://wiki.developers.facebook.com/index.php/ApplicationProperties
    $props_map = array(
      t('About URL') => 'about_url',
      t('Application Name') => 'application_name',
      t('Edit URL') => 'edit_url',
    );
    $props_map = fb_invoke(FB_ADMIN_OP_LIST_PROPERTIES, array('fb_app' => $fb_app), $props_map, FB_ADMIN_HOOK);
  }
  
  if (!isset($cache[$fb_app->apikey])) {
    if ($fb = fb_api_init($fb_app, FB_FBU_CURRENT)) {
      try {
        $props = $fb->api_client->admin_getAppProperties(array_values($props_map));
        $cache[$fb_app->apikey] = $props;
      } catch (Exception $e) {
        fb_log_exception($e, t('Failed to get application properties (%label) from Facebook', array('%label' => $fb_app->label)));
      }
    }
  }
  else {
    $props = $cache[$fb_app->apikey];
  }
  
  // Update $fb_app with the values we got from facebook api.
  foreach ($props_map as $key) {
    if ($props[$key]) {
      $fb_app->$key = $props[$key];
    }
  }
}

function fb_admin_set_app_properties($fb_app) {
  $fb_app_data = fb_get_app_data($fb_app);
  if ($fb_app_data['fb_app']['set_app_props']) {
    $props = fb_invoke(FB_ADMIN_OP_SET_PROPERTIES, array('fb_app' => $fb_app), array(), FB_ADMIN_HOOK);
    if (count($props)) {
      if ($fb = fb_api_init($fb_app, FB_FBU_CURRENT)) {
        try {
          $fb->api_client->admin_setAppProperties($props);
        } catch (Exception $e) {
          fb_log_exception($e, t('Failed to set application properties on Facebook'));
        }
      }
    }
  }
}


/**
 * Convenience method to return a list of all known apps, suitable for
 * form elements.
 *
 */
function fb_admin_get_app_options($include_current = FALSE, $key = 'label') {
  $apps = fb_get_all_apps();
  $options = array();
  if ($include_current)
    $options[FB_APP_CURRENT] = t('<current>');
  foreach ($apps as $app) {
    if ($key == 'apikey') {
      $options[$app->apikey] = $app->label;
    }
    else {
      $options[$app->label] = $app->label;
    }
  }
  return $options;
}
