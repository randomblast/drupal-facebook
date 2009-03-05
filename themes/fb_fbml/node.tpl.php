<?php
$class = "class=\"node node-$type";
if ($sticky)
  $class .= " sticky";
if (!$status)
  $class .= " node-unpublished";
$class .= "\"";
if ($extra_style)
  $style = "style = \"$extra_style\"";
?>
<div <?php print $class; ?> <?php print $style; ?>>

<?php if ($picture || $page == 0 || $submitted || $about || $terms) { ?>
<div class="node-header">
   <?php if ($picture && $submitted) {
  print $picture;
  }?>
<?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2><?php }; ?>
<span class="about"><?php print $about; ?></span>
<span class="submitted"><?php print $submitted?></span>
<span class="taxonomy"><?php print $terms?></span>
</div>
<?php } ?>
<div class="content"><?php print $content?></div>
<div class="footer">
  <?php if ($links) { ?><div class="links"><?php print $links?></div><?php }; ?>
</div>
<div class="children" id="children_<?php print $node->nid; ?>">
  <?php print $children; ?>
</div>

</div>
