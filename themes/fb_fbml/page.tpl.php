<?php print $styles; /* Styles for FBML pages */ ?>
<?php print $fbjs; /* Javascript for FBML pages */ ?>
<fb:title><?php print $title?></fb:title>

<div class="page-wrap <?php print $body_classes?>">
<div id="canvas-header" class="header">
<?php print $breadcrumb; ?>
<?php print $header; ?>
<?php
	if ($logo || $site_name) {
	  print '<h1><a href="'. url('<front>') .'" title="'. $site_name .'">';
	  if ($logo) {
		print '<img src="'. check_url($logo) .'" alt="'. $site_name .'" id="logo" />';
	  }
	  print $site_name .'</a>';
	  if (!$site_name)
		print $title;
	  print '</h1>';
	}
?>
<?php if ($site_name): ?>
  <h1><?php print $title; ?></h1>
<?php endif;?>
<div id="end-canvas-header"><!-- IE needs help --></div>
</div>
<?php print $tabs; ?>
<div id="content-wrap" class="content-wrap">
<div id="content-main" class="content-main">
	<?php print $messages; ?>
<?php print $content; ?>
<?php if ($content_footer):?>
<div id="content-footer" class="content-footer">
   <?php print $content_footer; ?>
</div>
<?php endif; ?>
</div>
<?php if ($right):?>
<div id="sidebar-right" class="sidebar-right">
   <?php print $right; ?>
<?php print $admin /* Administrator only sidebar */?>
</div>
<?php endif; ?>
<div class="clear"></div>
</div>
<?php if ($canvas_footer):?>
<div id="canvas-footer" class="canvas-footer">
   <?php print $canvas_footer; ?>
</div>
<?php endif; ?>
</div>
