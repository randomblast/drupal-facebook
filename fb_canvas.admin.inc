<?php


/**
 * Implementation of hook_fb_admin().
 */
function fb_canvas_fb_admin($op, $data, &$return) {
  $fb = isset($data['fb']) ? $data['fb'] : NULL;
  $fb_app = isset($data['fb_app']) ? $data['fb_app'] : NULL;

  if ($op == FB_ADMIN_OP_SET_PROPERTIES) {
    // Compute properties which we can set automatically.
    $callback_url = url('',array('absolute' => TRUE)) . FB_SETTINGS_APP_LABEL . '/' . $fb_app->label . '/';
    $return['callback_url'] = $callback_url;
  }
  else if ($op == FB_ADMIN_OP_LIST_PROPERTIES) {
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
    '#title' => t('Special Text Processing'),
    '#description' => t('Automatically re-write links in canvas pages?  Recommended you leave these items checked, unless you are quite sure your theme has been specially written to handle it.'),
  );
  $form['process_settings'][FB_CANVAS_VAR_PROCESS_FBML] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on all FBML canvas pages.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_FBML, TRUE),
  );
  $form['process_settings'][FB_CANVAS_VAR_PROCESS_FBML_FORM] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on FBML pages, when form submitted.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_FBML_FORM, TRUE),
    '#description' => t('Leave this <strong>checked</strong> unless you have made other modifications for form submissions on canvas pages.'),
  );
  $form['process_settings'][FB_CANVAS_VAR_PROCESS_IFRAME] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on all iframe canvas pages.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_IFRAME, TRUE),
  );
  return system_settings_form($form);
}



?>