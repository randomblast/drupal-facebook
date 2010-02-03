<?php

/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_connect_admin_settings() {
  $options = array(0 => t('<none>')) +
    fb_get_app_options(FALSE, 'apikey');
  $form['fb_connect_primary_apikey'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Primary Connect Application'),
    '#description' => t('Inialize one application on every page.'),
    '#default_value' => variable_get('fb_connect_primary_apikey', NULL),
  );

  $form['fb_connect_login_apikey'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Add Button to Login Form'),
    '#description' => t('Add a Facebook Connect button to Drupal\'s login form.'),
    '#default_value' => variable_get('fb_connect_login_apikey', NULL),
  );

  return system_settings_form($form);
}

?>