<?php

/**
 * Theme engine calls this preprocess "hook" before rendering a page.
 *
 * We change template for iframes, add styles and fbjs.
 */
function fb_fbml_preprocess_page(&$vars, $hook) {
  global $fb_app, $user;

  if (fb_is_iframe_canvas()) {
    // Iframe in a canvas
    $vars['template_file'] = 'iframe';
  }
  else {
    // FBML canvas page
    
    // Add our own stylesheet
    drupal_add_css(path_to_theme() . '/styles_fbml.css', 'theme', 'fbml');

    $vars['styles'] = drupal_get_css();

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
    // allows styling based on context (home page, node of certain type, etc.)
    $body_classes = array();
    $body_classes[] = ($vars['is_front']) ? 'front' : 'not-front';
    $body_classes[] = ($user->uid > 0) ? 'logged-in' : 'not-logged-in';
    if (isset($vars['left']) && isset($vars['right'])) {
      $body_classes[] = 'with-both-sidebars';
    }
    else if ($vars['right']) {
      $body_classes[] = 'with-sidebar-right';
    }
    else if ($vars['left']) {
      $body_classes[] = 'with-sidebar-left';
    }
    
    // new facebook pages are wider
    if ($_REQUEST['fb_sig_in_new_facebook'])
      $body_classes[] = 'in-new-facebook';
    
    // Regions
    $region_list = array(
    'prefaces' => array('preface_first', 'preface_middle', 'preface_last'), 
    //'postscripts' => array('postscript_first', 'postscript_middle', 'postscript_last')
    );
    foreach ($region_list as $sub_region_key => $sub_region_list) {
      $active_regions = array();
      foreach ($sub_region_list as $region_item) {
        if ($vars[$region_item]) {
          $active_regions[] = $region_item;
        }
      }
      $vars[$sub_region_key . '_class'] = $sub_region_key .'-'. strval(count($active_regions));
      $vars[$sub_region_key . '_count'] = count($active_regions);
    }

    $vars['body_classes'] = implode(' ', $body_classes);
  }
}

/**
 * Tell the theme engine which regions our theme supports.
 */
function fb_fbml_regionsXXX() {
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

//// override tabs at top of page.  We rely on a hack in fb_canvas.module to detect when we are theming the tabs.
function fb_fbml_menu_local_task($link, $active = FALSE) {
  global $_fb_canvas_state;
  
  if ($_fb_canvas_state == 'tabs') {
    if ($active) {
      $link = str_replace('selected="false"', 'selected="true"', $link);
    }
    return $link;
  } 
  else
    return theme_menu_local_task($link, $active);
}

function fb_fbml_menu_item_link($link) {
  global $_fb_canvas_state;
  
  if ($_fb_canvas_state == 'tabs') {
    // Theme an FBML tab
    $output = "<fb:tab-item href=\"" . 
      url($link['href']) . 
      (isset($link['title']) ? "\" title=\"" . $link['title'] : '') .
      "\" selected=\"false\">".$link['title']."</fb:tab-item>\n";
    return $output;
  }
  else
    return theme_menu_item_link($link);
}

function fb_fbml_menu_local_tasks() {
  global $fb;

  // menu.inc generates the local task (tab) markup in a convoluted and inflexible way.  Because of this we generate markup with <fb:tab-item ...> for the secondary links.  Here we use replacement to "fix" this.

  if ($fb && $fb->in_fb_canvas()) {
    $output = '';
    
    if ($primary = menu_primary_local_tasks()) {
      $output .= "<fb:tabs>\n". $primary ."</fb:tabs>\n";
    }
    
    if ($secondary = menu_secondary_local_tasks()) {
      // replace fb:tab-items with list items
      $pattern = '|<fb:tab-item href="([^"]*)" title="([^"]*)"([^>]*)>([^<]*)</fb:tab-item>|';
      $replace = '<li $3><a href="$1">$4</a></li>';
      $secondary = preg_replace($pattern, $replace, $secondary);
      $output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
    }
    
  }
  else {
    $output = theme_menu_local_tasks();
  }
  
  return $output;
}



// collapsing fieldsets
function fb_fbml_fieldset($element) {
  global $fb;
  if (($fb && $fb->in_fb_canvas()) ||
      fb_is_fbml_canvas()) {
    
    static $count = 0;
    
    if (isset($element['#collapsible']) && 
        $element['#collapsible']) {
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
    $output .= '<div class="fieldset-content" ' . 
      (isset($contentattrs) ? drupal_attributes($contentattrs) : '') . '>';
    if ($element['#description'])
      $output .= '<div class="description">'. $element['#description'] .'</div>';
    $output .= $element['#children'];
    if (isset($element['#value']))
      $output .= $element['#value'];
    $output .= "</div></fieldset>\n";
    
  } 
  else
    $output = theme_fieldset($element);
  
  return $output;
}

?>