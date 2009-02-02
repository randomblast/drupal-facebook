

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
	
	FB.Connect.logoutAndRedirect(Drupal.settings.fb_connect.logout_url);
    });
    FB.Connect.requireSession();
}

function fb_connect_onready() {
    FB.Facebook.get_sessionState().waitUntilReady(function(session) {
        var is_loggedin = session ? true : false;
	
	var fbu = FB.Facebook.apiClient.get_session() ?
            FB.Facebook.apiClient.get_session().uid :
            0;
	
        //alert('waitUntilReady callback, is_now_logged_in is ' + is_loggedin + ' and fbu is ' + fbu);
	
	if (is_loggedin && (!Drupal.settings.fb_connect.fbu)) {
	    //alert('fb_connect.fbu is ' + Drupal.settings.fb_connect.fbu + ' fbu is ' + fbu);
            window.location.reload();
	}
	else if (!is_loggedin && (Drupal.settings.fb_connect.fbu > 0)) {
            //window.location='$logout_url';
            FB.Connect.logoutAndRedirect(Drupal.settings.fb_connect.logout_url);
	}
    });
    
}

function fb_connect_init() {
    if (Drupal.jsEnabled && Drupal.settings.fb_connect.enable_login) {
	$(document).ready(fb_connect_onready);
    }
}