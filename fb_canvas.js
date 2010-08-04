
FB_Canvas = function(){};

/**
 * Called after Facebook javascript has initialized.  Global FB will be set.
 */
FB_Canvas.initHandler = function() {
  // @TODO - call setAutoResize only if iframe is resizeable.
  FB.Canvas.setAutoResize();
};

/**
 * Enable canvas page specific javascript on this page.
 */
Drupal.behaviors.fb_canvas = function(context) {
  jQuery(document).bind('fb_init', FB_Canvas.initHandler);
};