<?php
// $Id$
/**
 * @file
 * Admin pages and forms for facebook applications.
 * 
 */

include drupal_get_path('module', 'fb') . '/fb.admin.inc';

/**
 * Implementation of hook_fb_admin().
 */
function fb_app_fb_admin($op, $data, &$return) {
  $fb = isset($data['fb']) ? $data['fb'] : NULL;
  $fb_app = isset($data['fb_app']) ? $data['fb_app'] : NULL;
  
  if ($op == FB_ADMIN_OP_SET_PROPERTIES) {
    // URLs for Facebook events we support.
    $url = url(FB_SETTINGS_APP_LABEL . '/' . $fb_app->label . '/' . FB_APP_PATH_EVENT, array('absolute' => TRUE)) . '/';
    $return['uninstall_url'] = $url . FB_APP_EVENT_POST_REMOVE;
    $return['authorize_url'] = $url . FB_APP_EVENT_POST_AUTHORIZE;
  }
  elseif ($op == FB_ADMIN_OP_LIST_PROPERTIES) {
    $return[t('Application Name')] = 'application_name';
    $return[t('About URL')] = 'about_url';
    $return[t('Post-Authorize Callback URL')] = 'authorize_url';
    $return[t('Post-Remove Callback URL')] = 'uninstall_url';
    // Learn canvas name regardless of whether fb_canvas is enabled.
    $return[t('Canvas Name')] = 'canvas_name';
  }
  elseif (($op == FB_ADMIN_OP_LOCAL_LINKS) && isset($fb_app->fba_id)) {
    // Path to edit this app.
    $return[t('edit')] = FB_PATH_ADMIN_APPS . '/' . $fb_app->label . '/fb_app';
    // TODO!!! Support delete, and maybe view.
  }
}

/**
 * Builds the form used to edit an application.
 *
 * This form supports both create and edit.
 */
function fb_app_edit_form($form_state, $fb_app = NULL) {
  // If app is managed by this module, it has fba_id.
  if (isset($fb_app) && !$fb_app->fba_id) {
    drupal_set_message(t('Application %label not found.', array(
          '%label' => $label)),
      'warning');
    drupal_not_found();
    exit();
  }
  
  if (!isset($fb_app)) {
    // Defaults for new app.
    $fb_app = (object) array(
      'label' => NULL,
      'apikey' => NULL,
      'canvas' => NULL,
      'fba_id' => NULL,
      'id' => NULL,
      'status' => 1,
      'data' => serialize(array('fb_app' => array('set_app_props' => TRUE))),
    );
    //drupal_set_title(t('Create Facebook Application'));
  }
  else {
    //drupal_set_title(t('Edit %label', array('%label' => $fb_app->label)));
  }
  
  $form['#fb_app'] = $fb_app; // Similar to #node 
  
  //$form['#node'] = (object) array('fb_app' => $fb_app);  // deprecated! backward compatibility!
  
  if (!$fb_app->label) {
    // Helpful link
    // http://wiki.developers.facebook.com/index.php/How_To_Write_A_Good_Connect_Plugin
    $helptext = '<ol>
<li>Visit the Facebook application creation page: <a target="_blank" href="http://www.facebook.com/developers/createapp.php">http://www.facebook.com/developers/createapp.php</a>.</li>
<li>Enter a descriptive name in the Application Name field. Users will see this when signing up for your site.</li>
<li>Accept the Facebook Terms of Service.</li>
<li>If building a Canvas Page App, specify a Canvas Path.</li>
<li>Upload icon and logo images. The icon appears in News Feed stories and the logo appears in the Connect dialog when the user connects with your site.</li>
<li>Click Submit.</li>
<li>Copy the displayed API Key, Application Secret and App ID into this form.</li>
</ol>';

    $form['helptext'] = array(
      '#type' => 'markup',
      '#value' => t($helptext),
      '#weight' => -10,
    );
  }
  
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#default_value' => $fb_app->label,
    '#description' => t('A short name for this application.  Use letters and numerals only (no spaces, etc). <br/>This label will be used by code to customize the behavior of this app.<br/>Working with multiple copies of a site (i.e. development, staging, production)?  Use the <strong>same label</strong> on all servers!  Apikey, secret and ID usually change from server to server, but <strong>the label remains the same</strong>.'),
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $fb_app->status,
    '#description' => t('Uncheck if this server no longer hosts this application.'),
  );

  // ID, apikey and secret are shown on facebook.  User copies and pastes values.
  $form['id'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook App ID'),
    '#required' => FALSE,
    '#default_value' => $fb_app->id,
    '#description' => t('Facebook will generate this value when you create the application.'),
  );

  $form['apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#required' => TRUE,
    '#default_value' => $fb_app->apikey,
    '#description' => t('Facebook will generate this value when you create the application.'),
  );
  $form['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#required' => TRUE,
    '#default_value' => $fb_app->secret,
    '#description' => t('Facebook will generate this value when you create the application.'),
  );
  
  
  // fb_app_data is a placeholder where other modules can attach settings.
  $form['fb_app_data'] = array('#tree' => TRUE);  
  
  // Add our own fields to fb_app_data.  Other modules use hook_form_alter to do this.

  $data = fb_get_app_data($fb_app);
  $form['fb_app_data']['fb_app']['set_app_props'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set Application Properties Automatically'),
    '#default_value' => $data['fb_app']['set_app_props'],
    '#description' => t('Automatically update Facebook settings for this application.  Disable this if you have customized your callback URL, for example.  Also disable if another Drupal instance hosts the same application (i.e. with shared subdomain).'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 99,
  );
  return $form;
}

function fb_app_edit_form_validate($form, &$form_state) {
  $fb_app = (object)$form_state['values'];
  $fb_app->data = serialize($fb_app->fb_app_data);
  fb_admin_get_app_properties($fb_app);
  if (!$fb_app->application_name) {
    form_set_error('fb_app][apikey', t('Unable to get application properties.  Confirm both apikey and secret.'));
  }
}

function fb_app_edit_form_submit($form, &$form_state) {
  $fb_app = (object)$form_state['values'];
  $fb_app->data = serialize($fb_app->fb_app_data);
  
  // Get canvas_name, application_name from facebook.
  fb_admin_get_app_properties($fb_app);
  
  $orig_app = $form['#fb_app'];
  if ($orig_app->fba_id) {
    // Updating.
    
    db_query("UPDATE {fb_app} SET label='%s', status = %d, apikey='%s', secret='%s', id='%s', canvas='%s', title='%s',data='%s' WHERE fba_id = %d",
             $fb_app->label,
             $fb_app->status,
             $fb_app->apikey,
             $fb_app->secret,
             $fb_app->id,
             $fb_app->canvas_name, // From facebook, not form.
             $fb_app->application_name, // From facebook, not form.
             $fb_app->data,
             $orig_app->fba_id);
    
    watchdog('fb_app', 'Updated Facebook Application %label.',
             array('%label' => $fb_app->label,
             ),
             WATCHDOG_NOTICE,
             l(t('view apps'), FB_PATH_ADMIN_APPS));
    
    drupal_set_message(t('Saved changes to facebook application %title (%label).',
                         array('%title' => $fb_app->application_name,
                               '%label' => $fb_app->label))); 
  }
  else {
    // Inserting.
    db_query("INSERT INTO {fb_app} (label, status, apikey, secret, id, canvas, title, data) VALUES ('%s', %d, '%s', '%s', '%s', '%s', '%s', '%s')",
             $fb_app->label,
             $fb_app->status,
             $fb_app->apikey,
             $fb_app->secret,
             $fb_app->id,
             $fb_app->canvas_name, // From facebook, not form.
             $fb_app->application_name, // From facebook, not form.
             $fb_app->data);
    
    watchdog('fb_app', 'Created Facebook Application %label.',
             array('%label' => $fb_app->label,
             ),
             WATCHDOG_NOTICE,
             l(t('view apps'), FB_PATH_ADMIN_APPS));
    
    drupal_set_message(t('Created facebook application %title (%label).',
                         array('%label' => $fb_app->label,
                               '%title' => $fb_app->application_name)));
  }
  
  fb_app_set_app_properties($fb_app); // Set callback URL, etc.
  
  $form_state['redirect'] = FB_PATH_ADMIN;
}

/**
 * Sets callback URLs and other properties of a facebook app.  Calls the facebook 
 */
function fb_app_set_app_properties($fb_app) {
  $data = fb_get_app_data($fb_app);
  $fb_app_data = $data['fb_app'];
  $props = fb_invoke(FB_ADMIN_OP_SET_PROPERTIES, array('fb_app' => $fb_app), array(), FB_ADMIN_HOOK);
  if (count($props)) {
    if ($fb_app_data['set_app_props']) {
      if ($fb = fb_api_init($fb_app, FB_FBU_CURRENT)) {
        try {
          $fb->api_client->admin_setAppProperties($props);
          if (fb_verbose()) {
            drupal_set_message(t('Set the following properties for %label application:<br/><pre>!props</pre>', array('%label' => $fb_app->label, '!props' => print_r($props, 1))));
            watchdog('fb_app', 'Set facebook app properties for %label.',
                     array('%label' => $fb_app->label,
                     ),
                     WATCHDOG_NOTICE,
                     l(t('view apps'), FB_PATH_ADMIN));
          }
        } catch (Exception $e) {
          fb_log_exception($e, t('Failed to set application properties on Facebook'));
        }
      }
    }
    elseif (fb_verbose()) {
      drupal_set_message(t('The following recommended properties for %label application have <strong>not been set automatically</strong>:<br/><pre>!props</pre>', array('%label' => $fb_app->label, '!props' => print_r($props, 1))), 'warning');
    }
  }
}
