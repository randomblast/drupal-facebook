<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>" xmlns:fb="http://www.facebook.com/2008/fbml">
  <head><!-- fb_fbml/iframe.tpl.php -->
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<title><?php print $head_title; ?></title>
	<?php print $head; ?>
	<?php print $styles; ?>
	<?php print $scripts; ?>
  </head>
  <body class="<?php print $body_classes; ?>" >
    <div id="page">
	  <div id="header">
		        
          <?php if (isset($iframe_header)): ?>
          <div id="header-blocks">
            <?php print $iframe_header; ?>
          </div>
        <?php endif; ?>
        <!-- /header-blocks -->
        
      </div>
      <!-- /header -->
	  
      <div id="main">
        
		<div id="content">
          <?php if (!empty($tabs)): ?>
            <div class="tabs">
              <?php print $tabs; ?>
            </div>
          <?php endif; ?>
          <!-- /tabs -->
          
		  <?php if ($messages || $help): ?>
            <div id="content-header">
			  <?php print $messages; ?>
              <?php print $help; ?>
            </div>
          <?php endif; ?>
          <!-- /content-header -->
                    
		  <?php if (!empty($content_top)):?>
            <div id="content-top">
              <?php print $content_top; ?>
            </div>
          <?php endif; ?>
          <!-- /content-top -->
          
		  <?php if (!empty($content)):?>
            <div id="content-area">
              <?php print $content; ?>
            </div>
          <?php endif; ?>
          <!-- /content -->
          
		  <?php if (!empty($content_bottom)):?>
            <div id="content-bottom">
              <?php print $content_bottom; ?>
            </div>
          <?php endif; ?>
          <!-- /content-bottom -->
        </div>
      </div>
      <!-- /main -->
      
	  <div id="footer">
		<?php if ($iframe_footer): ?>
          <div id="footer-blocks">
            <?php print $iframe_footer; ?>
          </div>
        <?php endif; ?>
      </div>
      <!-- /footer -->
      
      <?php if (isset($closure_region)): ?>
        <div id="closure-blocks">
          <?php print $closure_region; ?>
        </div>
        <?php endif; ?>
      <?php if (isset($closure)) {print $closure;} ?>
    </div>
    <!-- /page -->
<!-- http://wiki.developers.facebook.com/index.php/JavaScript_Client_Library -->
<div id="FB_HiddenIFrameContainer" style="display:none; position:absolute; left:-100px; top:-100px; width:0px; height: 0px;"></div>
  </body>
</html>
