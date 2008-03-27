<?php print $styles; ?>
<div class="page-wrap <?php print $body_classes?>">
<div class=header>
	<?php print $breadcrumb; ?>
<?php print $header; ?>
<h1><?php print $title; ?></h1>
</div>
<?php print $tabs; ?>
<div class="content-wrap">
<div class="middle">
	<?php print $messages; ?>
<?php print $content; ?>
</div>
<?php if ($sidebar_right):?>
<div class="sidebar_right">
   <?php print $sidebar_right; ?>
<?php print $admin /* Administrator only sidebar */?>
</div>
<?php endif; ?>
<div class="clear"></div>
</div>
</div>


