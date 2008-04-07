<?php


function fb_fbml_page($content, $show_blocks = TRUE) {
  $output = phptemplate_page($content, $show_blocks);
  
  if (function_exists('fb_canvas_process')) {
	$output = fb_canvas_process($output);
  }
  return $output;
  
}

function _phptemplate_variables($hook, $vars = array()) {
  global $fb_app, $user;

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
	$body_classes[] = ($user->uid > 0) ? 'logged-in' : 'not-logged-in';
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
	if ($fbu = fb_get_fbu($vars['uid']))
	  $vars['picture'] = '<div class="picture"><fb:profile-pic uid="'.$fbu.'" linked="yes" size="'.$size.'" /></div>';
    
    //drupal_set_message("node vars: " . dprint_r($vars, 1));
  }
  return $vars;
}

function fb_fbml_regions() {
  $regions = array('admin' => t('Admin sidebar'),
				   'header' => t('Canvas Header'),
				   'right' => t('Canvas Right'),
				   'content_footer' => t('Content Footer'),
				   'canvas_footer' => t('Canvas Footer'),
				   );
  return $regions;
}

//// tabs

function phptemplate_menu_local_tasks() {
  global $fb;
  if ($fb && $fb->in_fb_canvas()) {
	$local_tasks = menu_get_local_tasks();
	$pid = menu_get_active_nontask_item();
	$output = '';
	
	if (count($local_tasks[$pid]['children'])) {
	  $output .= "<fb:tabs>\n";
	  foreach ($local_tasks[$pid]['children'] as $mid) {
		$item = menu_get_item($mid);
		$selected = menu_in_active_trail($mid) ? "true" : "false";
		$output .= "<fb:tab-item href=\"".url($item['path'], NULL, NULL, FALSE)."\" title=\"".$item['title']."\" selected=\"$selected\">".$item['title']."</fb:tab-item>\n";
	  }
	  $output .= "</fb:tabs>\n";
	}
	// TODO secondary local tasks
  }
  else 
	$output = theme_menu_local_tasks();

  return $output;
  
}

// collapsing fieldsets
function phptemplate_fieldset($element) {

  if ($fb && $fb->in_fb_canvas()) {
	
	//drupal_set_message("fb_fbml_fieldset" . dpr($element, 1));
	static $count = 0;
	
	if ($element['#collapsible']) {
	  $id = 'fbml_fieldset_' . $count++;
	  $linkattrs = array('clicktotoggle' => $id,
						 'href' => '#');
	  $contentattrs = array('id' => $id);
	  
	  if (!isset($element['#attributes']['class'])) {
		$element['#attributes']['class'] = '';
	  }
	  
	  $element['#attributes']['class'] .= ' collapsible';
	  if ($element['#collapsed']) {
		$element['#attributes']['class'] .= ' collapsed';
		$contentattrs['style'] = 'display:none';
	  }
	  $element['#title'] = '<a ' . drupal_attributes($linkattrs) .'>' . $element['#title'] . '</a>';
	}
	
	$output = '<fieldset ' . drupal_attributes($element['#attributes']) .'>';
	if ($element['#title']) {
	  $output .= '<legend>'. $element['#title'] .'</legend>';
	}
	$output .= '<div ' . drupal_attributes($contentattrs) . '>';
	if ($element['#description'])
	  $output .= '<div class="description">'. $element['#description'] .'</div>';
	$output .= $element['#children'] . $element['#value'];
	$output .= "</div></fieldset>\n";
	
  } 
  else
	$output = theme_fieldset($element);

	return $output;
}

?>