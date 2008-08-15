/**
 * FBML pages cannot include all of jquery or Drupal's normal javascript
 * functions.  But here we emulate just enough to get basic things like
 * settings to work.
 */
	var Drupal = Drupal || {};

	/**
	 * Extends the current object with the parameter. Works recursively.
	 */
	Drupal.extend = function(obj) {
	  for (var i in obj) {
		if (this[i]) {
		  Drupal.extend.apply(this[i], [obj[i]]);
		}
		else {
		  this[i] = obj[i];
		}
	  }
	};
