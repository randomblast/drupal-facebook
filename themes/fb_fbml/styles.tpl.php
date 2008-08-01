
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
  padding: 0px;
  }
  div.node-header, .comment-header {
  background-color: #f7f7f7;
  border-bottom: 1px solid #d8dfea;
  border-top: 1px solid #3b5998;
  padding: 5px 5px 5px 10px;
  overflow: hidden;
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
  padding: 8px 10px 0px 10px;
  font-size: 12px;
  }
  .content {
  clear: both;
  }

  .content p {
  margin: 0px 0 8px 0;
  font-size: 12px;
  }

  .node-teaser .footer, .node, .footer, .comment .footer {
  border-top: 1px solid #dddddd;
  font-size: 9px;
  margin: 0px 0 20px 0;
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
  margin: 0;
  white-space: nowrap;
  }

  ul.links li.first, .header .block-menu ul li:first-child {
  border: none;
  padding-left: 0px;
  }
  
  .header .block-menu .title { display: none;}
  .header ul.links, .header .block-menu, .header .breadcrumb {
  font-size: 10px;
  margin: 0;
  }
  
  .header ul.links, .header .block-menu {
  float: right;
}
  
  .header .block .content {clear: none;}
  
  .header .breadcrumb { float: left;}
  
  #canvas-header h1 {
    float: left;
	clear: left;
	padding: 10px 0 0 0;
  }
  #canvas-header img {
    vertical-align: middle;
    margin-right: 10px;
  }
  #canvas-header {
    margin: 0px;
    padding: 10px 10px 10px 20px;
	/*border-bottom: 1px solid #cccccc; IE will not render this */
	overflow: hidden;
  }
  
  /* for IE */
  #end-canvas-header {clear: both;}

  .admin-sidebar {
  padding: 2em 0;
  background: #fdd;
  }
  
  .form_item {
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

  /* emulate facebook blue buttons */
  .form-submit { 
    background-color:#3B5998;
	border-color:#D9DFEA rgb(14, 31, 91) rgb(14, 31, 91) rgb(217, 223, 234);
	border-style:solid;
	border-width:1px;
	color:#FFFFFF;
	font-size:11px;
	padding: 1px 8px;
	text-align:center;
	margin: 10px 10px 10px 0;
	}

  fieldset {
    margin: 0 0 10px 0;
	padding: 0px 10px;
  }
  fieldset.collapsible legend {
    font-weight: bold;
  }

  /* Give some links the facebook button look */
  a.fb_button {
	background:#526EA6 url(http://www.facebook.com/images/pandemic/white_arrow_on_blue.gif) no-repeat scroll right center;
	border-color:#145C9A rgb(14, 31, 91) rgb(14, 31, 91) rgb(20, 92, 154);
	border-style:solid;
	border-width:1px;
    color:#FFFFFF;
    font-weight:bold;
    padding:3px 24px 5px 15px;
  }
  a.fb_button:hover {
    background-color:#40578A;
	text-decoration:none;
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
#content-main {
  float: left;
  padding: 20px 20px 20px 20px;
  border-top: 1px solid #cccccc;
}
.with-sidebar-right #content-main {width: 436px;}
/* prevent form fields from being too wide. */
.with-sidebar-right #content-main input,
.with-sidebar-right #content-main textarea { max-width: 390px; }

#sidebar-right {
  float: right;
  width: 170px;
  padding: 0 0 0 0;
  background-color: #f7f7f7;
  border-top: 1px solid #cccccc;
}

#sidebar-right .block .title { 
  margin: 0 5px 0 5px;
  padding: 3px 5px 4px 5px;
  background-color: #e9e9e9;
  text-align: left;
 }

#sidebar-right .block .content {
  padding: 0 0 0 10px;
  margin: 0 0 10px 0;
}

#sidebar-right .content-wrap { 
  background-color: transparent;
  background-image: url(http://www.facebook.com/images/newsfeed_line.gif);
  background-repeat: repeat-y;
  background-attachment: scroll;
  float: left;
 }

div.clear {clear: both;}

/*
 * menu styles
 * 
 */

ul.menu, .item-list ul {
  margin: 0;
  padding: 0;
}

ul.menu ul, .item-list ul ul {
  margin-left: 1em;
}

ul.menu li, .item-list ul li, li.leaf {
  margin: 0;
  padding: 0;
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
  padding: 0 .75em 0 .75em;
}

/* forums */
td.topics, td.posts {text-align: center;}
th {padding: 0 2px;}

/* make tabs align with header and content padding */
.left_tabs {padding-left: 20px;}

/* sub-tabs */
.secondary {
 border: none;
 height: 1em;
  list-style: none;
 }
.secondary li {
  float: left;
 margin: 0 0 0 20px;
}
.secondary li.active {
  font-weight: bold;
}
/* pager */
.pager {
  width: 100%;
 padding: 10px 0;
 }

.pager a, .pager-list strong {margin-right: 1em;}

.block-fb_devel {
  border: solid 1px #e2c822;
  background: #fff9d7;
  margin: 10px 0;
 padding: 5px;
 }
</style>
