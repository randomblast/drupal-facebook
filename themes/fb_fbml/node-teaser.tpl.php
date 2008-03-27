<div class="node-teaser<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
<div class="node-header">
<?php if ($picture) {
  print $picture;
  }?>
  <h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2>
   <span class="about"><?php print $about; ?></span>
  <span class="submitted"><?php print $submitted?></span>
  <span class="taxonomy"><?php print $terms?></span>
</div>
<div class="content"><?php print $content?></div>
<div class="footer">
  <?php if ($links) { ?><div class="links"><?php print $links?></div><?php }; ?>
</div>

</div>
