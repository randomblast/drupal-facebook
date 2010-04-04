<?php
// $Id$
/**
 * @file
 * Admin pages and forms for user settings.
 * 
 */

// TODO: add pages to view data in the fb_user_app table.

/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_user_admin_settings() {
  $options = array(0 => t('<none>')) +
    fb_admin_get_app_options(FALSE);
  if (count($options) == 1) {
    $message = t('You must create an app first!');
    drupal_set_message($message, 'error');
    return array('help' => array('#value' => $message));
  }

  $form[FB_USER_VAR_STATS] = array(
    '#type' => 'checkbox',
    '#title' => t('Record Statistics'),
    '#description' => t('If checked, maintain data about the most recent visit of each user who has authorized an app.  Enable if you want this sort of record kept.  Enable also if you want to run cron actions for each user of an app.  Disable for better performance.'),
    '#default_value' => variable_get(FB_USER_VAR_STATS, TRUE),
  );

  return system_settings_form($form);
}

