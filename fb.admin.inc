<?php

function fb_admin_page() {
  $apps = fb_get_all_apps();

  if (count($apps)) {
    $header = array(t('Title'), t('About'), t('Canvas'), t('Local Settings'), t('Remote Settings'));
    
    foreach ($apps as $fb_app) {
      // Get known properties
      if (function_exists('fb_app_get_app_properties')) {
        fb_app_get_app_properties($fb_app);
      }
      $row = array();
      // Title.
      $row[] = l($fb_app->title, 'node/' . $fb_app->nid);
      
      // About.
      $row[] = l($fb_app->id, 'http://www.facebook.com/apps/application.php?id=' . $fb_app->id);
      
      // Canvas Page.
      if ($fb_app->canvas) {
        $row[] = l($fb_app->canvas, 'http://apps.facebook.com/' . $fb_app->canvas);
      }
      else {
        $row[] = t('n/a');
      }
      // Local Settings
      $row[] = l(t('edit'), 'node/' . $fb_app->nid . '/edit');

      // Remote Settings
      $row[] = l($fb_app->id, 'http://www.facebook.com/developers/editapp.php?app_id=' . $fb_app->id);
      
      $rows[] = $row;
    }

    $output .= theme('table', $header, $rows);

  }
  return $output;
}

