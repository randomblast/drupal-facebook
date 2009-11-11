<?php

/**
 * Filter handler for friends of the current user
 */
class fb_views_handler_filter_friends extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is friend of the current user');
  }
  
  function query() {
    $fb_app = $GLOBALS['fb_app']; // Set on Canvas and Connect pages.
    
    $uid = $GLOBALS['user']->uid;
    
    if ($fb_app && $uid) {
      $fbu = fb_get_fbu($uid, $fb_app);
      
      if ($fbu) {
        $friends = fb_get_friends($fbu, $fb_app);
        
        if (is_array($friends) && count($friends)) {
          $this->ensure_my_table();
          $this->query->add_where(0, $this->table_alias . '.fbu IN (%s)', implode(',', $friends));
        }
      }
    }
  }
}
