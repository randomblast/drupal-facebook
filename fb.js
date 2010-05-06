
window.fbAsyncInit = function() {
  //debugger;
  var settings = {xfbml: true};
  if (Drupal.settings.fb.apikey) {
    settings.apiKey = Drupal.settings.fb.apikey;
    settings.status = true;
    settings.cookie = true;
  }
  FB.init(settings);
  FB.Event.subscribe('auth.sessionChange', FB_JS.sessionChange);
};

FB_JS = function(){};

FB_JS.sessionChange = function(response) {
  var status = {'changed': true, 'fbu': response.session.uid, 'session': response.session, 'response' : response};
  jQuery.event.trigger('fb_session_change', status);
};

FB_JS.sessionChangeHandler = function() {
  debugger;
  window.location.reload();
};

Drupal.behaviors.fb = function(context) {
  if (typeof(FB) == 'undefined') {
    // Include facebook's javascript.
    jQuery.getScript('http://connect.facebook.net/en_US/all.js');
  }
  
  // Respond to connected events
  var events = $(document).data('events');
  if (!events || !events.fb_connect_status) {
    $(document).bind('fb_session_change', FB_JS.sessionChangeHandler);
  }

};
