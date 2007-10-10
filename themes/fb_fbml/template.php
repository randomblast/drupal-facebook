<?php

function _phptemplate_variables($hook, $vars = array()) {
  global $fb_app;

  if ($hook == 'page') {
    $vars['styles'] = _phptemplate_callback('styles', $vars);

    // Enforce that only admins see admin block.  This can be done (more
    // cleanly?) elsewhere.  But we're doing it here to make sure.
    if (!user_access('access administration pages'))
      $vars['admin'] ='';
    else if ($vars['admin']) {
	  $vars['admin'] = "<div class=\"region admin_sidebar\">\n".$vars['admin'].
        "\n</div><!-- end admin sidebar-->";
	}

	// Change 'Home' in breadcrumbs
	$crumbs = drupal_get_breadcrumb();
	if (count($crumbs) && strpos($crumbs[0], t('Home'))) {
	  $crumbs[0] = l(t($fb_app->title), '');
	  $vars['breadcrumb'] = theme('breadcrumb', $crumbs);
	}
	
	// Style page differently depending on which sidebars are present.
	// Approach copied from Zen theme.
	// allows advanced theming based on context (home page, node of certain type, etc.)
	$body_classes = array();
	$body_classes[] = ($vars['is_front']) ? 'front' : 'not-front';
	$body_classes[] = ($vars['logged_in']) ? 'logged-in' : 'not-logged-in';
	if ($vars['sidebar_left'] && $vars['sidebar_right']) {
	  $body_classes[] = 'both-sidebars';
	}
	else if ($vars['sidebar_right']) {
	  $body_classes[] = 'sidebar-right';
	}
	else if ($vars['sidebar_left']) {
	  $body_classes[] = 'sidebar-left';
	}

	$vars['body_classes'] = implode(' ', $body_classes);

  }
  else if ($hook == 'node') {
    if ($vars['teaser']) {
      $vars['template_file'] = 'node-teaser';
    }

    // TODO: could move this to phptemplate engine.
    if (count($vars['about']))
      $vars['about'] = drupal_render($vars['about']);
    if (count($vars['children']))
      $vars['children'] = drupal_render($vars['children']);

    if ($vars['extra_style'])
      $vars['extra_style'] = drupal_render($vars['extra_style']);

    if ($vars['teaser'])
      $size = 'thumb';
    else
      $size = 'thumb'; // small is too big for now, change this when node header has been made larger to fit image.
    $vars['picture'] = '<div class="picture"><fb:profile-pic uid="'.fb_get_fbu($vars['uid']).'" linked="yes" size="'.$size.'" /></div>';
    
    //drupal_set_message("node vars: " . dprint_r($vars, 1));
  }
  return $vars;
}

function fb_fbml_regions() {
  $regions = array('admin' => t('Admin sidebar'),
				   'header' => t('Facebook Canvas Header'),
				   'right' => t('Facebook Canvas Right'),
				   );

  // Announce the profileFBML region only in admin pages.  If we
  // announce it every page, we build its content every page, which is
  // inefficient.  We'll only build the blocks in these regions when
  // generating special content, like profile FBML.
  if (arg(0) == 'admin' && arg(1) == 'build' && arg(2) == 'block') {
	$regions['profile_fbml'] = t('Facebook Profile FBML');
	$regions['ref_fbml'] = t('Facebook Ref Page FBML');	
  }
  return $regions;
}

// Facebook requires image URLs to be absolute.
// We need to tweak the URL to point directly to us, much like we do with form actions.
function phptemplate_image($path, $alt='', $title = '', $attributes = NULL, $getsize = TRUE) {
  if (!$getsize || (is_file($path) && (list($width, $height, $type, $image_attributes) = @getimagesize($path)))) {
    $attributes = drupal_attributes($attributes);
    $url = (url($path) == $path) ? $path : (base_path() . $path);
    return '<img src="'. fb_local_url(check_url($url)) .'" alt="'. check_plain($alt) .'" title="'. check_plain($title) .'" '. $image_attributes . $attributes .' />';
  }

}


//// tabs

function phptemplate_menu_local_tasks() {
  $local_tasks = menu_get_local_tasks();
  $pid = menu_get_active_nontask_item();
  $output = '';

  if (count($local_tasks[$pid]['children'])) {
	$output .= "<fb:tabs>\n";
    foreach ($local_tasks[$pid]['children'] as $mid) {
	  $item = menu_get_item($mid);
	  $selected = menu_in_active_trail($mid) ? "true" : "false";
	  $output .= "<fb:tab-item href=\"".url($item['path'], NULL, NULL, TRUE)."\" title=\"".$item['title']."\" selected=\"$selected\">".$item['title']."</fb:tab-item>\n";
    }
	$output .= "</fb:tabs>\n";
  }
  // TODO secondary local tasks
  
  return $output;
  
}

?>