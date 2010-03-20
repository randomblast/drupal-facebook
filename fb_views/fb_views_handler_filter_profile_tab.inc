<?php

/**
 * Filter handler for profile tab user
 */
class fb_views_handler_filter_profile_tab extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is profile tab user');
  }
  
  function query() {
    $fb_app = $GLOBALS['_fb_app']; // Set on Canvas and Connect pages.
    
    $profile_fbu = $_REQUEST['fb_sig_profile_id']; // Set by facebook.
    
    if ($fb_app && $profile_fbu) {
      list($module, $authname) = _fb_user_get_authmap($fb_app, $profile_fbu);
      if ($authname) {
        $profile_account = user_external_load($authname);
        $this->ensure_my_table();
        $this->query->add_where(0, $this->table_alias . ".authname='%s'", $authname);
      }
    }
  }
}
