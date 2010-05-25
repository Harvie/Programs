<html>
<h1>TCP Service Checker</h1>
<form>
   Host: <input type="text" name="host"><br />
   Port: <input type="text" name="port"><br />
   Timeout: <input type="text" name="timeout" value="3"><br />
</form>

<pre><?php

@$fp = fsockopen ("127.0.0.1", 80, &$errno, &$errstr, 5);
   if (!$fp)
   { echo "<font color=\"#FF0000\"> UNRECHEABLE <br> ".$errno." - ".$errstr."</font>"; }
   else
   { echo "<font color=\"#00FF00\"> RECHEABLE </font>"; }

?></pre>

</html>
