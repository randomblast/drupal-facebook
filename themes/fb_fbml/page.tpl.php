<!-- begin <?php print __FILE__; ?> -->
<?php print $styles; /* Styles for FBML pages */ ?>
<?php if (isset($fbjs)) print $fbjs; /* Javascript for FBML pages */ ?>
<fb:title><?php print $title?></fb:title>

<div class="page-wrap <?php print $body_classes?>">
<div id="canvas-header" class="header">

<div id="logo-title-wrapper">
<?php print $breadcrumb; ?>

  <?php if ($logo || $site_name): ?>
    <h1><a href="<?php print url('<front>');?>" title="<?php print $site_name;?>">
    <?php if (isset($logo)):?>
      <img id="logo" src="<?php print check_url($logo);?>" alt="<?php print $site_name;?>" />
    <?php endif;?>
    <span id="site-name"><?php print $site_name;?></span>
    </a>
  <?php if ($site_slogan): ?>
    <span id="slogan"><?php print $site_slogan; ?></span>
  <?php endif; ?>
    </h1>
  <?php endif; /* $logo or $site_name */?>
</div><!-- /logo-title-wrapper -->

<?php print $header; ?>

<?php if ($search_box): ?>
  <div id="search-box">
    <?php print $search_box; ?>
  </div><!-- /search-box -->
<?php endif; ?>

<div id="end-canvas-header"><!-- IE needs help --></div>
</div><!-- /canvas-header -->

  <div id="preface">
    <div id="preface-wrapper" class="<?php print $prefaces_class; ?>" >
      <?php if ($mission): ?>
      <div id="mission"> 
      <?php print $mission; ?>
      </div>
      <?php endif; ?>

      <?php if ($preface_first): ?>
      <div id="preface-first" class="column">
      <?php print $preface_first; ?>
      </div><!-- /preface-first -->
      <?php endif; ?>

      <?php if ($preface_middle): ?>
      <div id="preface-middle" class="column">
      <?php print $preface_middle; ?>
      </div><!-- /preface-middle -->
      <?php endif; ?>

      <?php if ($preface_last): ?>
      <div id="preface-last" class="column">
      <?php print $preface_last; ?>
      </div><!-- /preface-last -->
      <?php endif; ?>

    </div><!-- /preface-wrapper -->
  </div><!-- /preface -->

<h1 id="page-title" class="clearfix"><?php print $title; ?></h1>

<?php print $tabs; ?>
<div id="content-wrap" class="content-wrap">
<div id="content-main" class="content-main">
<?php print $messages; ?>
<?php print $help; ?>
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
