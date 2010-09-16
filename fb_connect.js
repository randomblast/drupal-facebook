// $Id$

/**
 * @file
 *
 * Javascript specific to facebook connect pages.  This means pages
 * which are not canvas pages, and where fb_connect.module has
 * initialized the facebook api.  The user may or may not have
 * authorized the app, this javascript will still be loaded.
 *
 * Note (!) much of the work done here is deprecated, and moved to fb.js
 * (where code works equally well on both connect pages and canvas
 * pages).  If your app needs the features here, please report your
 * use case to our issue queue (http://drupal.org/project/issues/fb),
 * otherwise these features may go away...
 */

Drupal.behaviors.fb_connect = function(context) {

  // Support for easy fbml popup markup which degrades when javascript not enabled.
  // Markup is subject to change.  Currently...
  // <div class=fb_connect_fbml_popup_wrap><a title="POPUP TITLE">LINK MARKUP</a><div class=fb_connect_fbml_popup><fb:SOME FBML>...</fb:SOME FBML><div></div>
  $('.fb_connect_fbml_popup:not(.fb_connect_fbml_popup-processed)', context).addClass('fb_connect_fbml_popup-processed').prev().each(
    function() {
      this.fbml_popup = $(this).next().html();
      this.fbml_popup_width = parseInt($(this).next().attr('width'));
      this.fbml_popup_height = parseInt($(this).next().attr('height'));
      //console.log("stored fbml_popup markup: " + this.fbml_popup); // debug
      $(this).next().remove(); // Remove FBML so facebook does not expand it.
    })
    // Handle clicks on the link element.
    .bind('click', 
          function (e) {
            var popup;
            //console.log('Clicked!  Will show ' + this.fbml_popup); // debug
            popup = new FB.UI.FBMLPopupDialog($(this).attr('title'), this.fbml_popup, true, true);
            if (this.fbml_popup_width) {
              popup.setContentWidth(this.fbml_popup_width);
            }
            if (this.fbml_popup_height) {
              popup.setContentHeight(this.fbml_popup_height);
            }
            popup.set_placement(FB.UI.PopupPlacement.topCenter);
            popup.show();
            e.preventDefault();      
          })
    .parent().show();
  
  // Respect fb_connect classes on new content.
  // These classes are deprecated.  Use .fb_show and .fb_hide instead.  See fb.js.
  $('.fb_connect_show', context).show();
  $('.fb_connect_hide', context).hide();
  
};


// The following functions help us keep track of when a user is logged into Facebook Connect.
FB_Connect = function(){};


  