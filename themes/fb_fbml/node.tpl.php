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
<div <?=$class?> <?=$style?>>

<div class="node-header">
   <?php if ($picture && FALSE) { // disabling pic until styles are better.
  print $picture;
  }?>
<?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2><?php }; ?>
<span class="about"><?=$about?></span>
<span class="submitted"><?php print $submitted?></span>
<span class="taxonomy"><?php print $terms?></span>
</div>
<div class="content"><?php print $content?></div>
<div class="footer">
  <?php if ($links) { ?><div class="links"><?php print $links?></div><?php }; ?>
</div>
<div class="children" id="children_<?=$node->nid?>">
   <?=$children?>
</div>

</div>
