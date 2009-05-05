
// Global to keep track of connected state.  Better than facebook's 'reloadIfSessionStateChanged' option.
var fb_connect_fbu = null;

function fb_connect_on_connected(fbu) {
    //alert("fb_connect_on_connected " + fbu + " settings fbu is " + Drupal.settings.fb_connect.fbu + " fb_connect_fbu is " + fb_connect_fbu);
    if (fb_connect_fbu === 0) {
	// We've gone from not connected to connected.
	window.location.reload();
    }
    fb_connect_fbu = fbu;
}

function fb_connect_on_not_connected() {
    //alert("fb_connect_on_not_connected ");
    if (fb_connect_fbu > 0) {
	// This code will not be reached if fb_connect_logout_onclick (below) calls logoutAndRedirect.
	//alert("fb_connect_on_not_connected, reloading... ");
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

Drupal.behaviors.fb_connect = function() {
    // Support for easy fbml popup markup which degrades when javascript not enabled.
    // Markup is subject to change.  Currently...
    // <div class=fb_connect_fbml_popup><a title="POPUP TITLE">LINK MARKUP</a><fb:SOME FBML>...</fb:SOME FBML></div>
    $('div.fb_connect_fbml_popup').show().children().
	bind('click', function(e) {
	    popup = new FB.UI.FBMLPopupDialog($(this).attr('title'), $(this).next().html());
	    popup.set_placement(FB.UI.PopupPlacement.topCenter);
	    popup.show();
	    e.preventDefault();
	}).next().wrap('<div style="display: none;">');
}