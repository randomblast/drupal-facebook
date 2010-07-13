
// This function called by facebook's javascript when it is loaded.
window.fbAsyncInit = function() {
  //debugger; // debug
  var settings = {xfbml: true};
  if (Drupal.settings.fb.apikey) {
    settings.apiKey = Drupal.settings.fb.apikey;
    settings.status = true;
    settings.cookie = true;
  }
  FB.init(settings);
  FB.Event.subscribe('auth.sessionChange', FB_JS.sessionChange); // Catches login via connect button.
  jQuery.event.trigger('fb_init');  // Trigger event so that other code can now execute.
};

FB_JS = function(){};

// Facebook pseudo-event handler.
FB_JS.sessionChange = function(response) {
  var status = {'changed': true, 'session': response.session, 'response' : response};
  if (response.session) {
    status.fbu = response.session.uid;
  }

  jQuery.event.trigger('fb_session_change', status);
};

FB_JS.initHandler = function() {
  var session = FB.getLoginStatus(function(response) {
    var status = {'session' : response.session, 'response': response};
    if (response.session) {
      status.fbu = response.session.uid;
      if (Drupal.settings.fb.fbu != status.fbu) {
        status.changed = true;
      }
    }
    else if (Drupal.settings.fb.fbu) {
      status.changed = true;
    }
    if (status.changed) {
      jQuery.event.trigger('fb_session_change', status);
    }
  });
};

// JQuery pseudo-event handler.
FB_JS.sessionChangeHandler = function(context, status) {
  //debugger;
  var data = {
    'fb_session_no': new Boolean(status.session), // Suppress facebook-controlled session.
    'apikey': FB._apiKey,
    'event_type': 'session_change',
    'event_data': status
  };
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
    jQuery.post(Drupal.settings.fb.ajax_event_url + '/' + event_type, data,
		function(js_array, textStatus, XMLHttpRequest) {
		  //debugger;
		  for (var i = 0; i < js_array.length; i++) {
		    eval(js_array[i]);
		  }
		}, 'json');
  }
};

Drupal.behaviors.fb = function(context) {
  if (typeof(FB) == 'undefined') {
    // Include facebook's javascript.  @TODO - determine locale dynamically.
    jQuery.getScript(Drupal.settings.fb.js_sdk_url);
  }
  else {
    $(context).each(function() {
      var elem = $(this).get(0);
      //alert('fb_connect: ' + elem + $(elem).html()); // debug
      FB.XFBML.parse(elem);
    });
  }
  // Respond to connected events
  var events = jQuery(document).data('events');
  if (!events || !events.fb_session_change) {
    jQuery(document).bind('fb_session_change', FB_JS.sessionChangeHandler);
  }
  if (!events || !events.fb_init) {
    jQuery(document).bind('fb_init', FB_JS.initHandler);
  }
  
  // Markup with class .fb_show should be visible if javascript is enabled.  .fb_hide should be hidden.
  jQuery('.fb_hide', context).hide();
  jQuery('.fb_show', context).show();

  // Logout of facebook when logging out of drupal
  jQuery("a[href^='" + Drupal.settings.basePath + "logout']", context).click(FB_JS.logoutHandler);
};
