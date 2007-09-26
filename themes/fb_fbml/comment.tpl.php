<div class="comment<?php if ($comment->status == COMMENT_NOT_PUBLISHED) print ' comment-unpublished'; ?>">
<div class="comment-header">
<?php if ($picture) {
  print $picture;
  } ?>
<h3 class="title"><?php print $title; ?></h3><?php if ($new != '') { ?><span class="new"><?php print $new; ?></span><?php } ?>
<div class="submitted"><?php print $submitted; ?></div>
</div><!-- end comment header -->
<div class="content"><?php print $content; ?></div>
<div class="footer">
<div class="links"><?php print $links; ?></div>
</div>
</div>
