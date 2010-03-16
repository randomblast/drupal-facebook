<?php

/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_connect_admin_settings() {
  $options = array(0 => t('<none>')) +
    fb_get_app_options(FALSE);
  if (count($options) == 1) {
    drupal_set_message(t('Start by <a href=!href>creating a Facebook Application</a>.', array('!href' => url('node/add/fb-app'))),
                       'error');
  }

  $form[FB_CONNECT_VAR_PRIMARY] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Primary Connect Application'),
    '#description' => t('Inialize one application on every page.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_PRIMARY, NULL),
  );
  
  $form[FB_CONNECT_VAR_LOGIN] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Add Button to Login Form'),
    '#description' => t('Add a Facebook Connect button to Drupal\'s login form.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_LOGIN, NULL),
  );

  $form['theme_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme overrides'),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERNAME_1] = array(
    '#type' => 'checkbox',
    '#title' => t('Override default theme_username() function.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERNAME_1, TRUE),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERNAME_2] = array(
    '#type' => 'checkbox',
    '#title' => t('Override phptemplate_username() functions defined in themes.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERNAME_2, TRUE),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERPIC_1] = array(
    '#type' => 'checkbox',
    '#title' => t('Override default theme_user_picture() function.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERPIC_1, TRUE),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERPIC_2] = array(
    '#type' => 'checkbox',
    '#title' => t('Override phptemplate_user_picture() functions defined in themes.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERPIC_2, TRUE),
  );
  return system_settings_form($form);
}

?>