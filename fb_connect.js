
// Global to keep track of connected state.  Better than facebook's 'reloadIfSessionStateChanged' option.
var fb_connect_fbu = null;

function fb_connect_on_connected(fbu) {
    //alert("fb_connect_on_connected " + fbu + " settings fbu is " + Drupal.settings.fb_connect.fbu + " fb_connect_fbu is " + fb_connect_fbu);
    if (fb_connect_fbu === 0 || Drupal.settings.fb_connect.fbu != fbu) {
	// We've gone from not connected to connected.
	window.location.reload();
    }
    fb_connect_fbu = fbu;
}

function fb_connect_on_not_connected() {
    //alert("fb_connect_on_not_connected, settings fbu is " + Drupal.settings.fb_connect.fbu);
    if (fb_connect_fbu > 0 || Drupal.settings.fb_connect.fbu > 0) {
	// This code will not be reached if fb_connect_logout_onclick (below) calls logoutAndRedirect.
	// alert("fb_connect_on_not_connected, reloading... ");
	// We've gone from connected to not connected.
	window.location.reload();
    }
    fb_connect_fbu = 0;
}

function fb_connect_login_onclick() {
    // This will execute before the user fills out the login form.
    // More important is fb_connect_on_connected, above.
}

function fb_connect_logout_onclick() {
    FB.ensureInit(function() {
	//alert('logout and redirect to ' + Drupal.settings.fb_connect.front_url);
	FB.Connect.logoutAndRedirect(Drupal.settings.fb_connect.front_url);
    });
}

// This function called after fbConnect is initialized
function fb_connect_init() {
    // Use show and hide to degrade gracefully when javascript not enabled.
    $('.fb_connect_show').show();
    $('.fb_connect_hide').hide();
}

Drupal.behaviors.fb_connect = function(context) {
  // Tell Facebook to parse any XFBML elements found in the context.
  FB_RequireFeatures(['XFBML'], function() {
      $(context).each(function() {
          if ($(this).html()) {
            var elem = $(this).get(0);
            //alert('fb_connect: ' + elem + $(elem).html()); // debug
            FB.XFBML.Host.parseDomElement(elem);
          }
        });
    });
  

  // Support for easy fbml popup markup which degrades when javascript not enabled.
  // Markup is subject to change.  Currently...
  // <div class=fb_connect_fbml_popup_wrap><a title="POPUP TITLE">LINK MARKUP</a><div class=fb_connect_fbml_popup><fb:SOME FBML>...</fb:SOME FBML><div></div>
  $('.fb_connect_fbml_popup:not(.fb_connect_fbml_popup-processed)', context).addClass('fb_connect_fbml_popup-processed').prev().each(
    function() {
      this.fbml_popup = $(this).next().html();
      this.fbml_popup_width = parseInt($(this).next().attr('width'));
      this.fbml_popup_height = parseInt($(this).next().attr('height'));
      //alert("stored fbml_popup markup: " + this.fbml_popup); // debug
      $(this).next().remove(); // Remove FBML so facebook does not expand it.
    })
  // Handle clicks on the link element.
  .bind('click', 
        function (e) {
          //alert('Clicked!  Will show ' + this.fbml_popup); // debug
          popup = new FB.UI.FBMLPopupDialog($(this).attr('title'), this.fbml_popup);
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
  
}
