<?php

class fb_views_handler_profile_pic extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['fbu'] = 'fbu';
  }

  function render($values) {
    if ($fbu = $values->fb_user_app_fbu) {
      return '<div class="picture"><fb:profile-pic uid="'.$values->fb_user_app_fbu.'" linked="yes" size="thumb" /></div>';
    }
  }
}
