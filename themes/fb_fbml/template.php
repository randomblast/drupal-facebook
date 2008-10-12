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
    if (function_exists('fb_canvas_is_iframe') && fb_canvas_is_iframe()) {
      // Iframe in a canvas
      $vars['template_file'] = 'iframe';
    }
    else {
      // FBML canvas page
      
      // Add our own stylesheet
      drupal_add_css(path_to_theme() . '/styles_fbml.css', 'theme', 'fbml');
      
      // http://wiki.developers.facebook.com/index.php/Include_files
      // Facebook now allows external references to stylesheets, but not
      // using the normal syntax, we don't use the normal
      // drupal_get_css() here.
      $css = drupal_add_css();
      
      // Only include stylesheets for FBML
      foreach ($css['fbml'] as $type => $files) {
	foreach ($files as $file => $preprocess) {
	  $url = base_path() . $file;
	  if (file_exists($file)) {
	    // Refresh Facebook's cache anytime file changes.
	    $url .= '?v=' . filemtime($file);
	  }
	  // preprocess ignored
	  if ($type == 'module') {
	    $module_css .= '<link rel="stylesheet" type="text/css" media="screen" href="'.$url."\" />\n";
	  }
	  else if ($type == 'theme') {
	    $theme_css .= '<link rel="stylesheet" type="text/css" media="screen" href="'.$url."\" />\n";
	  }
	}
      }
      $vars['styles'] = $module_css . $theme_css;
      
      // Include only Facebook aware javascript.
      $vars['fbjs'] = drupal_get_js('fbml');
      
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
	$body_classes[] = 'with-both-sidebars';
      }
      else if ($vars['sidebar_right']) {
	$body_classes[] = 'with-sidebar-right';
      }
      else if ($vars['sidebar_left']) {
	$body_classes[] = 'with-sidebar-left';
      }
      
      // new facebook pages are wider
      if ($_REQUEST['fb_sig_in_new_facebook'])
	$body_classes[] = 'in-new-facebook';
      
      $vars['body_classes'] = implode(' ', $body_classes);
      
    }
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
		   'iframe_header' => t('Iframe Header'),
		   'iframe_footer' => t('Iframe Footer'),
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
    if ($secondary = menu_secondary_local_tasks()) {
	  $output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
    }
  }
  else 
    $output = theme_menu_local_tasks();
  
  return $output;
  
}

// collapsing fieldsets
function phptemplate_fieldset($element) {
  global $fb;
  if (($fb && $fb->in_fb_canvas()) ||
      (function_exists('fb_canvas_is_fbml') && fb_canvas_is_fbml())) {
    
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
    $output .= '<div class="fieldset-content" ' . drupal_attributes($contentattrs) . '>';
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