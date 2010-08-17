
FB_Canvas = function(){};

/**
 * Called after Facebook javascript has initialized.  Global FB will be set.
 */
FB_Canvas.setAutoResize = function() {
  FB.Canvas.setAutoResize(true, 100); // time in ms, default 100.
};

/**
 * Enable canvas page specific javascript on this page.
 */
Drupal.behaviors.fb_canvas = function(context) {
  // Resize if body class includes fb_canvas-resizable.
  $('body.fb_canvas-resizable:not(.fb_canvas-processed)').each(function () {
    $(this).addClass('fb_canvas-processed');
    jQuery(document).bind('fb_init', FB_Canvas.setAutoResize);
  });
};
