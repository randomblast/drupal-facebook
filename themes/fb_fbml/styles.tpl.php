
<style type="text/css">
  .page-wrap {
  position: relative;
  }
  div.error {
  padding: 10px;
  border: solid 1px #dd3c10;
  background: #ffebe8;
  }
  div.messages {
  margin: 10px 10px 10px 10px;
  }
  div.node-teaser, div.node, .comment, .view {
  padding: 10px;
  }
  div.node-header, .comment-header {
  background-color: #f7f7f7;
  border-bottom: 1px solid #d8dfea;
  border-top: 1px solid #3b5998;
  padding: 4px 6px 5px 12px;
  }
  .comment-header {
  border: none;
  }

  .node-header span {
  font-size: 10px;
  display: block;
  }
  .node-header .picture {
  float: left;
  margin: 0 10px 0 0px;
  }
  .indented {padding-left: 10px;}
  .node-teaser .content, .node .content, .comment .content {
  padding: 12px;
  font-size: 12px;
  }
  .content {
  clear: both;
  }

  .content p {
  margin: 10px 0 0 0;
  font-size: 12px;
  }

  .node-teaser .footer, .node, .footer, .comment .footer {
  border-top: 1px solid #dddddd;
  font-size: 9px;
  margin: 0px 0 10px 0;
  padding: 5px 2px 5px 12px;
  }

  ul.links, .header .block-menu ul {
  list-style: none;
  display: inline;
  }
  
  ul.links li, .header .block-menu ul li {
  border-left: 1px solid #666666;
  float: left;
  padding: 0 5px 0 5px;
  }
  ul.links li.first, .header .block-menu ul li:first-child {
  border: none;
  padding-left: 0px;
  }
  
  .header .block-menu .title { display: none;}
  .header ul.links, .header .block-menu, .header .breadcrumb {
  font-size: 10px;
  margin: 10px 10px 5px 10px;
  }
  
  .header ul.links, .header .block-menu {
  float: right;
  }
  
  .header .block .content {clear: none;}
  
  .header .breadcrumb { float: left;}
  
  .header h1 {clear: both; margin: 10px 10px 20px 10px;}
  .header {
  border-bottom: 1px solid #cccccc;
  margin-bottom: 0px;
  }

  .admin_sidebar {
  position:absolute;
  right: 0px;
  top: 80px;
  width: 200px;
  z-index: 999;
  background: #eee;
  }
  
  form, .form_item {
  margin: 10px;
  }
  
  label {
  display: block;
  margin: 10px 0 0 0;
  }

  .form-required { color: red;}

  .form-item input.error, .form-item textarea.error, .form-item select.error {
  border: 1px solid #dd3c10;
  color: #494949;
  }

  fieldset {
  margin: 10px 0 0 0;
  }

  table {
  border-collapse: collapse;  
  }

  th {
  text-align: center;
  }

  td {
  margin: 0;
  padding: 0px;
  border: 1px solid #cccccc;
  }
  
  td.view-field {
  text-align: center;
  vertical-align: middle;
  padding: 2px 10px 2px 10px;
  }

  td.view-field-node-title, .fb_discussion_topic .title {
  text-align: left;
  font-size: 12px;
  font-weight: bold;
  }
  .odd {
  background-color: #eeeeee;
  }
  
  div.view {
  clear: both;
  }

  table.fb_discussion {
  clear: both;
  margin: 10px 0 10px 0;
  }
  table.fb_discussion td{
  padding: 10px;
  }

/**
 * Sidebars and such.
 * 
 * The right sidebar styles are based on Facebook's home page.
 * .sidebar_right is the actual sidebar div.  .sidebar-right is on the
 * overall page container.  Confusing.
 */
div.middle {float: left;}
.sidebar-right .middle {width: 460px;}
/* prevent form fields from being too wide, on Konqueror at least.  Hack. */
.sidebar-right .middle input { max-width: 430px; }

div.sidebar_right {
  float: right;
  width: 186px;
  padding: 0 0 0 0;
  background-color: #f7f7f7;
}

.sidebar_right .block .title { 
  margin: 0 5px 0 5px;
  padding: 3px 5px 4px 5px;
  background-color: #e9e9e9;
  text-align: left;
 }

.sidebar-right .content-wrap { 
  background-color: transparent;
  background-image: url(http://www.facebook.com/images/newsfeed_line.gif);
  background-repeat: repeat-y;
  background-attachment: scroll;
 }

div.clear {clear: both;}

/*
 * menu styles
 * 
 * Still refer to garland's images here.  Must be fixed.
 */

ul.menu, .item-list ul {
  margin: 0.35em 0 0 -0.5em;
  padding: 0;
}

ul.menu ul, .item-list ul ul {
  margin-left: 0em;
}

ul.menu li, .item-list ul li, li.leaf {
  margin: 0.15em 0 0.15em .5em;
}

ul.menu li, .item-list ul li, li.leaf {
  padding: 0 0 .2em 1.5em;
  list-style-type: none;
  list-style-image: none;
}

ul li.expanded {
}

ul li.collapsed {
}

ul li.leaf a, ul li.expanded a, ul li.collapsed a {
  display: block;
}

ul.inline li {
  background: none;
  margin: 0;
  padding: 0 1em 0 0;
}


</style>
