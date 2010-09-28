
// This function called by facebook's javascript when it is loaded.
window.fbAsyncInit = function() {
  //debugger; // debug
  // @TODO - make these settings completely customizable.
  var settings = {xfbml: true};
  if (Drupal.settings.fb.apikey) {
    settings.apiKey = Drupal.settings.fb.apikey;
    settings.status = true;
    settings.cookie = true;
  }
  
  FB.init(settings);

  // Async function to complete init.
  FB.getLoginStatus(function(response) {
    var status = {'session' : response.session, 'response': response};
    jQuery.event.trigger('fb_init', status);  // Trigger event for third-party modules.

    FB_JS.sessionChange(response);

    // Use FB.Event to detect Connect login/logout.
    FB.Event.subscribe('auth.sessionChange', FB_JS.sessionChange);
    
    // Other events that may be of interest...
    //FB.Event.subscribe('auth.login', FB_JS.debugHandler);
    //FB.Event.subscribe('auth.logout', FB_JS.debugHandler);
    //FB.Event.subscribe('auth.statusChange', FB_JS.debugHandler);
    //FB.Event.subscribe('auth.sessionChange', FB_JS.debugHandler);

  });
};

FB_JS = function(){};

/**
 * Helper parses URL params.
 *
 * http://jquery-howto.blogspot.com/2009/09/get-url-parameters-values-with-jquery.html
 */
FB_JS.getUrlVars = function(href)
{
  var vars = [], hash;
  var hashes = href.slice(href.indexOf('?') + 1).split('&');
  for(var i = 0; i < hashes.length; i++)
  {
    hash = hashes[i].split('=');
    vars[hash[0]] = hash[1];
    if (hash[0] != 'fb_js_fbu')
      vars.push(hashes[i]); // i.e. "foo=bar"
  }
  return vars;
}

/**
 * Reload the current page, whether on canvas page or facebook connect.
 */
FB_JS.reload = function(destination) {
  // Determine fbu.
  var session = FB.getSession();
  var fbu;
  if (session != null)
    fbu = session.uid;
  else
    fbu = 0;

  // Avoid infinite reloads
  var vars = FB_JS.getUrlVars(window.location.href);
  if (vars.fb_js_fbu === fbu) {
    return; // Do not reload (again)
  }

  // Determine where to send user.
  if (typeof(destination) != 'undefined' && destination) {
    // Use destination passed in.
  }
  else if (typeof(Drupal.settings.fb.reload_url) != 'undefined') {
    destination = Drupal.settings.fb.reload_url;
  }
  else {
    destination = window.location.href;
  }
  
  // Split and parse destination
  var path;
  if (destination.indexOf('?') == -1) {
    vars = [];
    path = destination;
  }
  else {
    vars = FB_JS.getUrlVars(destination);
    path = destination.substr(0, destination.indexOf('?'));
  }
  
  // Add fb_js_fbu to params before reload.
  if (fbu) {
    vars.push('fb_js_fbu=' + fbu);
  }

  // Use window.top for iframe canvas pages.
  destination = path + '?' + vars.join('&');
  window.top.location = destination;
};

// Facebook pseudo-event handlers.
FB_JS.sessionChange = function(response) {
  var status = {'changed': false, 'fbu': null, 'session': response.session, 'response' : response};
  
  if (response.session) {
    status.fbu = response.session.uid;
    if (Drupal.settings.fb.fbu != status.fbu) {
      // A user has logged in.
      status.changed = true;
    }
  }
  else if (Drupal.settings.fb.fbu) {
    // A user has logged out.
    status.changed = true;
    
    // Sometimes Facebook's invalid cookies are left around.  Let's try to clean up their crap.
    // @TODO - delete cookie only if it exists.
    FB_JS.deleteCookie('fbs_' + Drupal.settings.fb.apikey, '/', '');
  }
  
  if (status.changed) {
    // fbu has changed since server built the page.
    jQuery.event.trigger('fb_session_change', status);

    // Remember the fbu.
    Drupal.settings.fb.fbu = status.fbu;
  }
  
};

// Helper function for developers.
FB_JS.debugHandler = function(response) {
  debugger;
};

// JQuery pseudo-event handler.
FB_JS.sessionChangeHandler = function(context, status) {
  // Pass data to ajax event.
  var data = {
    'event_type': 'session_change'
  };
  
  if (status.session) {
    data.fbu = status.session.uid;
    // Suppress facebook-controlled session.
    data.fb_session_handoff = true;
  }
  FB_JS.ajaxEvent(data.event_type, data);
  // No need to call window.location.reload().  It will be called from ajaxEvent, if needed.
};

// click handler
FB_JS.logoutHandler = function(event) {
  if (typeof(FB) != 'undefined') {
    FB.logout(function () {
      //debugger;
    });
  }
};

// Helper to pass events via AJAX.
// A list of javascript functions to be evaluated is returned.
FB_JS.ajaxEvent = function(event_type, data) {
  if (Drupal.settings.fb.ajax_event_url) {

    // Include session, in the format faceboook's PHP sdk expects it.
    // @TODO - pass this only if fbs_APIKEY cookie is not set.
    data.session = JSON.stringify(FB.getSession());
    // Other values to always include.
    data.apikey = FB._apiKey;
    data.fb_controls = Drupal.settings.fb.controls;

    jQuery.post(Drupal.settings.fb.ajax_event_url + '/' + event_type, data,
		function(js_array, textStatus, XMLHttpRequest) {
		  //debugger; // debug
		  for (var i = 0; i < js_array.length; i++) {
		    eval(js_array[i]);
		  }
		}, 'json');
  }
};

// Delete a cookie.
FB_JS.deleteCookie = function( name, path, domain ) {
  document.cookie = name + "=" +
  ( ( path ) ? ";path=" + path : "") +
  ( ( domain ) ? ";domain=" + domain : "" ) +
  ";expires=Thu, 01-Jan-1970 00:00:01 GMT";
};

// Test the FB settings to see if we are still truly connected to facebook.
FB_JS.sessionSanityCheck = function() {
  if (!Drupal.settings.fb.checkSemaphore) {
    Drupal.settings.fb.checkSemaphore=true;
    FB.api('/me', function(response) {
      if (response.id != Drupal.settings.fb.fbu) {
	// We are no longer connected.
	var status = {'changed': true, 'fbu': null, 'check_failed': true};
	jQuery.event.trigger('fb_session_change', status);
      }
      Drupal.settings.fb.checkSemaphore=null;
    });
  }
};


//// FBML popup helpers.  Does this code belong here in fb.js?

/**
 * Move new dialogs to visible part of screen.
 **/
FB_JS.centerPopups = function() {
  var scrollTop = $(window).scrollTop();
  $('.fb_dialog:not(.fb_popup_centered)').each(function() {
    var offset = $(this).offset();
    if (offset.left == 0) {
      // This is some facebook cruft that cannot be centered.
    }
    else if (offset.top < 0) {
      // Not yet visible, don't center.
    }
    else if (offset.top < scrollTop) {
      $(this).css('top', offset.top + scrollTop + 'px');
      $(this).addClass('fb_popup_centered'); // Don't move this dialog again.
    }
  });
};

FB_JS.enablePopups = function(context) {
  // Support for easy fbml popup markup which degrades when javascript not enabled.
  // Markup is subject to change.  Currently...
  // <div class=fb_fbml_popup_wrap><a title="POPUP TITLE">LINK MARKUP</a><div class=fb_fbml_popup><fb:SOME FBML>...</fb:SOME FBML></div></div>
  $('.fb_fbml_popup:not(.fb_fbml_popup-processed)', context).addClass('fb_fbml_popup-processed').prev().each(
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
	    
	    // http://forum.developers.facebook.net/viewtopic.php?pid=243983
	    var size = FB.UIServer.Methods["fbml.dialog"].size;
	    if (this.fbml_popup_width) {
	      size.width=this.fbml_popup_width;
	    }
	    if (this.fbml_popup_height) {
	      size.height=this.fbml_popup_height;
	    }
	    FB.UIServer.Methods['fbml.dialog'].size = size;
	    
	    // http://forum.developers.facebook.net/viewtopic.php?id=74743
	    var markup = this.fbml_popup;
	    if ($(this).attr('title')) {
	      markup = '<fb:header icon="true" decoration="add_border">' + $(this).attr('title') + '</fb:header>' + this.fbml_popup;
	    }
	    var dialog = {
	      method: 'fbml.dialog', // triple-secret undocumented feature.
	      display: 'dialog',
	      fbml: markup,
	      width: this.fbml_popup_width,
	      height: this.fbml_popup_height
	    };
	    var popup = FB.ui(dialog, function (response) {
	      console.log(response);
	    });
	    
	    // Start a timer to keep popups centered.
	    // @TODO - avoid starting timer more than once.
	    window.setInterval(FB_JS.centerPopups, 500);
	    
            e.preventDefault();      
          })
    .parent().show();
};


Drupal.behaviors.fb = function(context) {
  // Respond to our jquery pseudo-events
  var events = jQuery(document).data('events');
  if (!events || !events.fb_session_change) {
    jQuery(document).bind('fb_session_change', FB_JS.sessionChangeHandler);
  }
  
  if (typeof(FB) == 'undefined') {
    // Include facebook's javascript.  @TODO - determine locale dynamically.
    jQuery.getScript(Drupal.settings.fb.js_sdk_url);
  }
  else {
    $(context).each(function() {
      var elem = $(this).get(0);
      FB.XFBML.parse(elem);
    });

    FB_JS.sessionSanityCheck();
  }

  // Markup with class .fb_show should be visible if javascript is enabled.  .fb_hide should be hidden.
  jQuery('.fb_hide', context).hide();
  jQuery('.fb_show', context).show();

  // Logout of facebook when logging out of drupal
  jQuery("a[href^='" + Drupal.settings.basePath + "logout']", context).click(FB_JS.logoutHandler);

  // Support markup for dialog boxes.
  FB_JS.enablePopups(context);
};

