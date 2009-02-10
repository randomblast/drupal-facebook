

function fb_connect_login_onclick() {
    FB.Facebook.get_sessionState().waitUntilReady(function() {
        var fbu = FB.Facebook.apiClient.get_session() ?
            FB.Facebook.apiClient.get_session().uid :
            0;
	
	//alert('waitUntilReady callback was called, fbu via javascript is ' + fbu);
	if (fbu) {
	    //window.location.reload();
	}
    }); 
    FB.Connect.requireSession();
}

function fb_connect_logout_onclick() {
    FB.Facebook.get_sessionState().waitUntilReady(function() {
        var fbu = FB.Facebook.apiClient.get_session() ?
            FB.Facebook.apiClient.get_session().uid :
            0;
	
	FB.Connect.logoutAndRedirect(Drupal.settings.fb_connect.front_url);
    });
    FB.Connect.requireSession();
}

function fb_connect_onready() {
    FB.Facebook.get_sessionState().waitUntilReady(function(session) {
        var is_loggedin = session ? true : false;
	
	var fbu = FB.Facebook.apiClient.get_session() ?
            FB.Facebook.apiClient.get_session().uid :
            0;
	
	// Sanity check
	if (fbu != Drupal.settings.fb_connect.fbu) {
	    // alert('fb_connect_ready... fbu is ' + fbu + ', fb_connect.fbu is ' + Drupal.settings.fb_connect.fbu + ' is_loggedin is ' + is_loggedin);
	}
    });
}

function fb_connect_init() {
    if (Drupal.jsEnabled && Drupal.settings.fb_connect.enable_login) {
	$(document).ready(fb_connect_onready);
    }
}