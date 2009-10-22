/**
 * Javascript helpers for Facebook Feeds.  Loaded by fb_feed.module.
 */
FB_Feed = function(){};

/**
 * Display a feed dialog on Facebook Connect pages, via http://wiki.developers.facebook.com/index.php/JS_API_M_FB.Connect.ShowFeedDialog.
 *
 * @param json is the json-encoded output of fb_feed_get_show_dialog_data().
 */
FB_Feed.show_feed_dialog = function(json) {
  var data_array = Drupal.parseJson(json);
  var len = data_array.length;
  for (var i=0; i < len; i++) {
    var data = data_array[i];
    if (data.bundle_id) {
      // TODO: make use of the callback parameter.
      FB.Connect.showFeedDialog(data.bundle_id, data.options.template_data, data.options.target_id, data.options.body_general, null, FB.RequireConnect.require, null, data.options.user_message_prompt, data.options.user_message);
    }
  }
};

