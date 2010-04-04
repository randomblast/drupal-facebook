<?php
// $Id$
/**
 * @file
 * Admin pages and forms for canvas apps.
 * 
 */


/**
 * Implementation of hook_fb_admin().
 */
function fb_canvas_fb_admin($op, $data, &$return) {
  $fb = isset($data['fb']) ? $data['fb'] : NULL;
  $fb_app = isset($data['fb_app']) ? $data['fb_app'] : NULL;

  if ($op == FB_ADMIN_OP_SET_PROPERTIES) {
    // Compute properties which we can set automatically.
    if (function_exists('fb_url_inbound_alter')) {
      $callback_url = url('', array('absolute' => TRUE)) . FB_SETTINGS_CB . '/' . $fb_app->label . '/';
    }
    else {
      // Paving the way to make URL alters optional.
      $callback_url = url('', array('absolute' => TRUE));
    }
    $return['callback_url'] = $callback_url;
  }
  elseif ($op == FB_ADMIN_OP_LIST_PROPERTIES) {
    $return[t('Callback URL')] = 'callback_url';
  }
}


/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_canvas_admin_settings() {

  $form['process_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('URL processing'),
    '#description' => t('This option alters links, so that instead of referring directly to this server, they point to <em>apps.facebook.com/APP/...</em>.  While this impedes the performance of your server, it is recommended unless you are quite sure your theme and/or code has been specially written to handle this some other way.'),
  );
  $form['process_settings'][FB_CANVAS_VAR_PROCESS_FBML] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on all FBML canvas pages.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_FBML, TRUE),
  );
  $form['process_settings'][FB_CANVAS_VAR_PROCESS_IFRAME] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on iframe pages and iframes embedded in FBML.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_IFRAME, TRUE),
  );
  $form['form_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form handling'),
    '#description' => t('On canvas pages, forms can be submitted to a URL that starts <em>apps.facebook.com/APP/...</em>.  However, this allows facebook.com to observe all data posted by your users.  <em>And</em> prevents file uploads from working.  <em>And</em> prevents Drupal\'s #value fields such as those used in <em>captcha</em> from working properly.  By selecting this option, forms will instead submit directly to this server, and the user then redirected back to <em>apps.facebook.com/APP/...</em> where they should see the page they were intended to see.  (It\'s magic that should <em>just work</em>.)'),
  );
  $form['form_settings'][FB_CANVAS_VAR_PROCESS_FBML_FORM] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on FBML pages, when form submitted.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_FBML_FORM, TRUE),
    '#description' => t('Leave this <strong>checked</strong> unless you have made other modifications to support form submissions on canvas pages.'),
  );
  
  return system_settings_form($form);
}

