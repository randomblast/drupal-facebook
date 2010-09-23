<?php
// $Id$
/**
 * @file
 * Admin pages and forms for Drupal for Facebook.
 * 
 */


define('FB_ADMIN_HOOK', 'fb_admin');

define('FB_ADMIN_OP_SET_PROPERTIES', 'set_props'); // build props for admin.setAppProperties
define('FB_ADMIN_OP_LIST_PROPERTIES', 'list_props'); // list of known properties for admin.getAppProperties


// Pages which may or may not be supported by the app implementation.
define('FB_ADMIN_OP_LOCAL_LINKS', 'fb_admin_links');

/**
 * Drupal page callback.
 */
function fb_admin_page() {
  $apps = fb_get_all_apps();
  $output = ''; // Avoid PHP notice.
  
  if (count($apps)) {
    $header = array(
      t('Label'), 
      t('About'), 
      t('Canvas'), 
      t('Local Operations'), 
      t('Remote Settings'),
    );
    
    foreach ($apps as $fb_app) {
      // Get properties from facebook.
      fb_admin_get_app_properties($fb_app);
      
      $row = array();
      // Title.
      $row[] = $fb_app->label . ($fb_app->status ? '' : ' ' . t('(<em>not enabled</em>)'));
      
      // About.
      $row[] = l($fb_app->application_name, 'http://www.facebook.com/apps/application.php?id=' . $fb_app->id);
      
      // Canvas Page.
      if (FALSE && $fb_app->canvas_name != $fb_app->canvas) { // @TODO: re-enable when app props are supported.
        drupal_set_message(t('Canvas page for %label is out of sync!  Facebook believes it is %fbcanvas, while our database believes %canvas.  Edit and save the application to remedy this.', array('%label' => $fb_app->label, '%fbcanvas' => $fb_app->canvas_name, '%canvas' => $fb_app->canvas)), 'error');
      }
      if ($fb_app->canvas) {
        $row[] = l($fb_app->canvas, 'http://apps.facebook.com/' . $fb_app->canvas);
      }
      else {
        $row[] = t('n/a');
      }
      // Local Ops
      $local_links =
        fb_invoke(FB_ADMIN_OP_LOCAL_LINKS,
                  array('fb_app' => $fb_app),
                  array(t('view') => FB_PATH_ADMIN_APPS . '/' . $fb_app->label,
                        t('set props') => FB_PATH_ADMIN_APPS . '/' . $fb_app->label . '/fb/set_props',
                  ),
                  FB_ADMIN_HOOK);
      
      $links = array();
      foreach ($local_links as $title => $href) {
        $links[] = array(
          'title' => $title,
          'href' => $href,
        );
      }
      $row[] = theme('links', $links);
      
      // Remote Settings
      $row[] = l($fb_app->id, 'http://www.facebook.com/developers/editapp.php?app_id=' . $fb_app->id);
      
      $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
  }
  else {
    $output = t('Start by creating an application.');
    if (!module_exists('fb_app')) {
      $output .= t('Ensure the Facebook Application module is enabled.');
    }
    else {
      $output .= t('Click <a href="!url">Add Application</a> to continue.',
                   array('!url' => url('admin/build/fb/fb_app_create')));
    }
  }
  return $output;
}

function fb_admin_app_page($fb_app = NULL) {
  fb_get_app_data($fb_app);
  fb_admin_get_app_properties($fb_app);
  $fb = fb_api_init($fb_app);

  // placeholder until theming done... XXX
  // Hide a couple things...
  unset($fb_app->secret); 
  unset($fb_app->data); 

  // Warn user if values have changed since last edit.
  if (FALSE) // @TODO: XXX re-enable this with facebook supports get app properties!
  foreach (array('canvas_name' => 'canvas', 'application_name' => 'title') as $prop => $key) {
    if (isset($fb_app->$prop) && $fb_app->$prop != $fb_app->$key) {
      drupal_set_message(t("The property %prop has been changed to %value on facebook.  Go to the Edit tab, confirm values are correct and hit Save button to syncronize the local values.",
                           array('%prop' => $prop, '%value' => $fb_app->$prop)), 'error');
    }
  }
  
  $props_map = array(
    t('Name') => 'name',
    t('Label') => 'label',
    t('API Key') => 'apikey',
    t('ID') => 'id',
  );
  $output = "<dl>\n";
  $props_map = fb_invoke(FB_ADMIN_OP_LIST_PROPERTIES, array('fb_app' => $fb_app), $props_map, FB_ADMIN_HOOK);
  foreach ($props_map as $name => $key) {
    if (isset($fb_app->$key)) {
      $output .= "<dt>$name</dt><dd>{$fb_app->$key}</dd>\n";
    }
  }
  $output .= "</dl>\n";
  //$output .=  '<pre>' . print_r($fb_app, 1) . '</pre>'; // debug
  
  return $output;
}

function fb_admin_page_title($fb_app) {
  return $fb_app->label;
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
    if ($fb = fb_api_init($fb_app)) {
      try {
        $props = fb_call_method($fb, 'admin.getAppProperties', array(
                                  'properties' => implode(',', array_values($props_map)),
                                ));
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

function fb_admin_set_properties_form(&$form_state, $fb_app) {
  $form['fba_id'] = array(
    '#type' => 'value',
    '#value' => $fb_app->fba_id,
  );

  $props = fb_invoke(FB_ADMIN_OP_SET_PROPERTIES, array('fb_app' => $fb_app), array(), FB_ADMIN_HOOK);

  $form['#fb_props'] = $props;
  
  $form['desc'] = array(
    '#type' => 'markup',
    '#value' => t('This will attempt to set the following application properties on facebook.com.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  // @TODO - beautify display of properties.
  $form['props'] = array(
    '#type' => 'markup',
    '#value' => print_r($props, 1),
    '#prefix' => '<pre>',
    '#suffix' => '</pre>',
  );

  return confirm_form($form,
                      t('Are you sure you set properties for %title?', array('%title' => $fb_app->title)),
                      isset($_GET['destination']) ? $_GET['destination'] : FB_PATH_ADMIN_APPS . '/' . $fb_app->label,
                      t('This action cannot be undone.'),
                      t('Set Properties'),
                      t('Cancel')
  );
}

/**
 * Button submit function.  Use has clicked delete, send them to confirm page.
 */
function fb_admin_set_properties_form_submit($form, &$form_state) {
  $fba_id = $form_state['values']['fba_id'];
  $fb_app = fb_get_app(array('fba_id' => $fba_id));
  $props = $form['#fb_props'];
  if ($fb_app && count($props)) {
    if ($fb = fb_api_init($fb_app)) {
      try {
        $result = fb_call_method($fb, 'admin.setAppProperties', array(
                                   'properties' => json_encode($props),
                                 ));
        // Success.
        $form_state['redirect'] = FB_PATH_ADMIN_APPS . '/' . $fb_app->label;
        
        drupal_set_message(t('Note that it may take several minutes for property changes to propigate to all facebook servers.'));
        if (fb_verbose()) {
          drupal_set_message(t('Set the following properties for %label application:<br/><pre>!props</pre>', array('%label' => $fb_app->label, '!props' => print_r($props, 1))));
          watchdog('fb_app', 'Set facebook app properties for %label.',
                   array('%label' => $fb_app->label,
                   ),
                   WATCHDOG_NOTICE,
                   l(t('view apps'), FB_PATH_ADMIN));
        }
      } catch (Exception $e) {
        drupal_set_message(t('Failed to set the following properties for %label application.  You may need to manually editing remote settings!<br/><pre>!props</pre>', array('%label' => $fb_app->label, '!props' => print_r($props, 1))), 'error');
        fb_log_exception($e, t('Failed to set application properties on Facebook'));
      }
    }
  }

}
