<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
<meta http-equiv="content-type" content="text/html"; charset="utf-8">
<title>Redakce - (c) 2oo6 - Harvie</title>
<meta http-equiv="cache-control" content="no-cache" />

</head>

<body>

<div class="form">
<form action="edit.php" method="post">
		Login: <input type="text" name="user" size="25"><br>
		Password: <input type="password" name="password" size="25"><br>

		Soubor:
		<select name="file" size="10">

		<?
		$soubory=opendir('.'); 
		while (false!==($file = readdir($soubory))) 
		{ 
				if ($file != "." && $file != "..") 
				{ 
						echo "<option value=\"" .$file."\">".$file."</option>\n"; 
				} 
		}
		closedir($soubory); 
		?>
 
		</select>
		
		<input type="submit" value="EDIT">
		
</form>
</div>

</body>
</html>
