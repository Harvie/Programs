<?php
$prom = 0;
$a = 5;
$b = 14;

echo $_POST["username"];
echo "<br />";

echo $_POST["domain"];
echo "<br />";

echo $_POST["password"];
echo "<br />";
echo "<br />";

echo "<title>HELL-O-WORLD!!!</title><button style=\"background-color: orange;\"><h1>Hellllo!</h1></button><br>";
echo "<br />";

echo <<<END

<form action="http://server202/users/mudrunka/index.php" method="post">
		<h2>Pøihlásit se uživatelským jménem <img src="/img/szn-logo-mini.gif" alt="Seznam.cz" /></h2>
		<p>
			
			
			<label for="name">Jméno:</label>
			<input type="text" id="name" name="username" value="" tabindex="1" />
			<select name="domain" tabindex="3">
				
	                <option value="seznam.cz" >@seznam.cz</option>

	            
	                <option value="email.cz" >@email.cz</option>
	            
	                <option value="post.cz" >@post.cz</option>
	            
			</select>
			
			
			
			</p>
		<p>
			<label for="password">Heslo:</label>
			<input type="password" id="password" name="password" tabindex="2" />

		</p>
		<p class="remember">
			<input type="checkbox" name="remember" id="remember" value="1" /><label for="remember">pøihlásit trvale na tomto poèítaèi</a></label> - <a href="http://napoveda.seznam.cz/cz/prihlaseni.html">Co to je?</a>
		</p>
		<p class="submit-row">
			<input type="submit" class="submit" value="Pøihlásit se" tabindex="4" />
			<input type="hidden" name="serviceId" value="email" />

			<input type="hidden" name="disableSSL" value="0" />
			<input type="hidden" name="forceSSL" value="0" />
			<input type="hidden" name="lang" value="cz" />
			<input type="hidden" name="template" value="html" />
			<input type="hidden" name="lang" value="cz" />
			<input type="hidden" name="loggedURL" value="http://email.seznam.cz/ticket" />
			<input type="hidden" name="returnURL" value="" />
			<input type="hidden" name="loginFormIsSSL" value="0" />
		</p>

	</form>

END;

echo '+ '. ($a + $b) ."<br />";
echo "- ". ($a - $b) ."<br>";
echo "/ ". ($b / $a) ."<br>";
echo "* ". ($a * $b) ."<br>";
echo "<br />";

$fp = FOpen ("data.txt", "a");  //a = doplnìní w = zápis r = ètení
FWrite ($fp, $_POST["password"]); // uloží obsah promìnné $data do souboru data2.txt

?>