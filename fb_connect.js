
Drupal.behaviors.fb_connect = function(context) {
  // Respond to connected events
  var events = $(document).data('events');
  if (!events || !events.fb_connect_status) {
    $(document).bind('fb_connect_status', FB_Connect.statusHandle);
  }

  // Sanity check.  Not sure when this can happen, but if it does we can't expect anything to work.
  if (typeof FB_RequireFeatures == 'undefined') {
    return;
  }
    
  // Tell Facebook to parse any XFBML elements found in the context.
  FB_RequireFeatures(['XFBML'], function() {
      $(context).each(function() {
          var elem = $(this).get(0);
          //alert('fb_connect: ' + elem + $(elem).html()); // debug
          FB.XFBML.Host.parseDomElement(elem);
          
      });
      // Respect fb_connect classes on new content.
      $('.fb_connect_show', context).show();
      $('.fb_connect_hide', context).hide();
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
          var popup;
          //alert('Clicked!  Will show ' + this.fbml_popup); // debug
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
  
};


// The following functions help us keep track of when a user is logged into Facebook Connect.
FB_Connect = function(){};

// Global to keep track of connected state.  Better than facebook's 'reloadIfSessionStateChanged' option.
FB_Connect.fbu = null; // null, not 0

FB_Connect.statusHandle = function(e, data) {
  if (data.changed) {
    // Status has changed, user has logged in or logged out.
    var destination = Drupal.settings.fb_connect.destination;
    if (destination) {
      window.location.href = destination;
    }
    else if (data.fbu) {
      // User has connected.
      FB_Connect.sessionStart(function(data) {
	  //alert("reloading because user connected.");
	  window.location.reload();
	});
    }
    else {
      // Refresh page for not-connected user.
      FB_Connect.sessionEnd(function(data) {
	  //alert("reloading because user disconnected.");
	  
	  // User has disconnected.
	  // Sometimes bogus cookies are left behind, here we try to clean up.
	  FB_Connect.purgeCookies();
	  
	  window.location.reload();
	});
    }
  }
};

FB_Connect.sessionStart = function(callback) {
  if (Drupal.settings.fb_connect.session_start_url) {
    var data = {
      'fb_session_no': 1, // allow drupal to control session.
      'apikey': FB.Facebook.apiKey
    };
    $.post(Drupal.settings.fb_connect.session_start_url, data, callback);
  }
  else {
    callback();
  }
};

FB_Connect.sessionEnd = function(callback) {
  if (Drupal.settings.fb_connect.session_end_url) {
    var data = {
      //'fb_session_no': 0, // TBD: are the facebook cookies still set when we get here?
      'apikey': FB.Facebook.apiKey
    };
    $.post(Drupal.settings.fb_connect.session_end_url, data, callback);
  }
  else {
    callback();
  }
};

// TODO: for some reason, when you start connected, then log out of facebook and log in as another user, this routine still thinks you are the first user, the next time a page is loaded.
FB_Connect.on_connected = function(fbu) {
  var status = {'changed': false, 'fbu': fbu};
  if ((FB_Connect.fbu === 0 || Drupal.settings.fb_connect.fbu != fbu) &&
      Drupal.settings.fb_connect.in_iframe != 1) {
    status.changed = true;
  }
  FB_Connect.fbu = fbu;
  $.event.trigger('fb_connect_status', status);
};


FB_Connect.on_not_connected = function() {
  var status = {'changed': false, 'fbu': 0};
  if ((FB_Connect.fbu > 0 || Drupal.settings.fb_connect.fbu > 0) &&
      Drupal.settings.fb_connect.in_iframe != 1){
    // This code will not be reached if fb_connect_logout_onclick (below) calls logoutAndRedirect.
    // We've gone from connected to not connected.
    status.changed = true;
  }
  FB_Connect.fbu = 0;
  $.event.trigger('fb_connect_status', status);
};

FB_Connect.login_onclick = function() {
  // This will execute before the user fills out the login form.
  // More important is FB_Connect.on_connected, above.
};

FB_Connect.logout_onclick = function() {
  FB.ensureInit(function() {
      FB.Connect.logoutAndRedirect(Drupal.settings.fb_connect.front_url);
    });
};


FB_Connect.execute_javascript = function() {
  var url = '/fb_connect/js';
  $.post(url, null, function(eval_me) {
      if (eval_me) {
        eval(eval_me);
      }
    });
};



// This function called after fbConnect is initialized
FB_Connect.init = function() {
  $.event.trigger('fb_connect_init', status);
};


/**
 * Facebook leaves bogus cookies.  We must clean up their mess.
 * Thanks to http://techpatterns.com/downloads/javascript_cookies.php for example code.
 * TODO: make this more efficient by learning the apikey some other way.
 */
FB_Connect.purgeCookies = function() {
  var cookies = { };
  
  if (document.cookie && document.cookie != '') {
    var split = document.cookie.split(';');
    for (var i = 0; i < split.length; i++) {
      var name_value = split[i].split("=");
      name_value[0] = name_value[0].replace(/^ /, '');
      var pos = name_value[0].indexOf('_session_key');
      if (pos != -1) {
        // Looks like a facebook cookie.
        var apikey = name_value[0].substring(0, pos);
        
        // Try to delete all the crud.
        FB_Connect.deleteCookie(apikey, '/', '');
        FB_Connect.deleteCookie(apikey + '_user', '/', '');
        FB_Connect.deleteCookie(apikey + '_ss', '/', '');
        FB_Connect.deleteCookie(apikey + '_session_key', '/', '');
        FB_Connect.deleteCookie(apikey + '_expires', '/', '');
        FB_Connect.deleteCookie('fb_setting_' + apikey, '/', '');
      }
    }
  }
};
  
// Delete a cookie.
FB_Connect.deleteCookie = function( name, path, domain ) {
  document.cookie = name + "=" +
  ( ( path ) ? ";path=" + path : "") +
  ( ( domain ) ? ";domain=" + domain : "" ) +
  ";expires=Thu, 01-Jan-1970 00:00:01 GMT";
};
  