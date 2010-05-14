
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
  FB.Event.subscribe('auth.sessionChange', FB_JS.sessionChange);
  jQuery.event.trigger('fb_init');  // Trigger event so that other code can now execute.
};

FB_JS = function(){};

// Facebook pseudo-event handler.
FB_JS.sessionChange = function(response) {
  var status = {'changed': true, 'fbu': response.session.uid, 'session': response.session, 'response' : response};
  jQuery.event.trigger('fb_session_change', status);
};

// JQuery pseudo-event handler.
FB_JS.sessionChangeHandler = function() {
  //debugger;
  window.location.reload();
};

Drupal.behaviors.fb = function(context) {
  if (typeof(FB) == 'undefined') {
    // Include facebook's javascript.
    jQuery.getScript('http://connect.facebook.net/en_US/all.js');
  }
  
  // Respond to connected events
  var events = jQuery(document).data('events');
  if (!events || !events.fb_session_change) {
    jQuery(document).bind('fb_session_change', FB_JS.sessionChangeHandler);
  }

  // Markup with class .fb_show should be visible if javascript is enabled.  .fb_hide should be hidden
  jQuery('.fb_hide', context).hide();
  jQuery('.fb_show', context).show();
};
