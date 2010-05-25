<?php
	$request = $_SERVER['REDIRECT_URL'];
	$host = $_SERVER['HTTP_HOST'];
	$proto = 'http';
	if(!(trim($_SERVER['HTTPS']) == '')) $proto = 'https';

	if(!eregi('www\\.', $host)) {
		header('Location: '.$proto.'://www.'.$host.$request);
	}

	$desc = trim(strtr($request, '/\\', '  '));
	$url = $desc.' site:'.$host;
	$cache_url = 'cache:'.$host.$request;

	$desc = htmlspecialchars($url);
	$cache_desc = htmlspecialchars($cache_url);

	$url = urlencode($url);
	$cache_url = urlencode($cache_url);

	$request = htmlspecialchars($request);
	$link = "<a href='http://google.com/search?q=$url'>$desc</a>";
	$link_cache = "<a href='http://google.com/search?q=$cache_url'>$cache_desc</a>";
?>
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html><head>
<title><?php echo($request); ?> - 404 Not Found</title>
<link rel="stylesheet" href="/_common/harvie.css" type="text/css" />
</head><body>
<table border="0" style="width:100%;"><tr>
	<td>
		<h1>404 Not Found</h1>
		<p>The requested URL <?php echo($request); ?> was not found on this server.</p>
		<p>You can try to google it: <?php echo($link); ?>.</p>
		<p>You can also try to lookup google cache: <?php echo($link_cache); ?>.</p>
		<p>Or you can visit my <a href="http://blog.harvie.cz/">blog</a>.</p>
	</td>
	<td>
		<h1>404 Nenalezeno</h1>
		<p>Požadovaná URL <?php echo($request); ?> nebyla na tomto serveru nalezena.</p>
		<p>Můžete ji zkusit vygooglovat: <?php echo($link); ?>.</p>
		<p>Můžete ji zkusit najít v google archivu: <?php echo($link_cache); ?>.</p>
		<p>Nebo můžete navštívit můj <a href="http://blog.harvie.cz/">blog</a>.</p>
	</td>
</tr></table>

<div style="text-align:center;"><h1>Big Brother's watching...</h1><img src="http://narcism.harvie.cz/1.jpg" /><br /><img src="http://img389.imageshack.us/img389/9341/resistaneisfutilern5.jpg" /></div>
<div style="text-align:right;"><pre>
     \./     
    (o o)    
oOOo-(_)-oOOo</div></pre>
<hr>
<address><?php echo($_SERVER['SERVER_SIGNATURE']); ?></address>

</body></html>



