<?=$styles?>
<div class="page-wrap <?=$body_classes?>">
<div class=header>
<?=$breadcrumb?>
<?=$header?>
<h1><?=$title?></h1>
</div>
<?=$tabs?>
<div class="content-wrap">
<div class="middle">
<?=$messages?>
<?=$content?>
</div>
<?php if ($sidebar_right):?>
<div class="sidebar_right">
<?=$sidebar_right?>
<?=$admin /* Administrator only sidebar */?>
</div>
<?php endif; ?>
<div class="clear"></div>
</div>
</div>


