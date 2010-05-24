<title>SinusCurve - (c) Harvie 2oo6</title>

<style>
  body { background-color: black; color: lime; font-family: Arial, Tahoma; }
  #hide { border: 2px solid; color: white; margin: 10px; padding: 10px; }
  #sipka {float: right;}
  pre { line-height: 20%; letter-spacing: 0%; word-spacing: 0%; }
@media print {
  #hide { border: none; height: 0px; overflow: hidden; color: red; }
  body { background-color: white; color: black; font-family: Arial, Tahoma;}
}
</style>

<h2>f:Y = sin(X)</h2>

<div id="hide">
<form action="#" method="get">
<b>Settings:</b><br /><br />
X-Min: <input type="text" name="start" value="0"> = First X<br />
X-Max:<input type="text" name="reset" value="<? echo(2*PI()); ?>"> = Last X<br />
Step: <input type="text" name="step" value="0.07"> = Increase X by this every line.<br /><br />

Y-Offset: <input type="text" name="offset" value="1.1"> = Displacement on Y (0 = half sinewave; 1.1 = whole sinewave)<br />
Width:<input type="text" name="width" value="35"> = Y Zoom<br />
Zoom: <input type="text" name="zoom" value="1"> = X Zoom (Every line will be printed this much times.)<br /><br />

Outline: <input type="text" name="outline" value="+"> = Curve outline (try: "#")<br />
Inlay: <input type="text" name="inlay" value=" "> = Curve inlay (try: ":")<br /><br />

<i>
Tips:<br />
Press CTRL+A<br />
Press CTRL and try to roll MouseWheeeel...<br />
Look at source code of this webpage ;)<br />
</i><br />

<input type="submit" value=".: DRAW :.">
</form>
</div>
<pre><?php
//This will draw the ASCII "neverending" sinus curve.
//Writen by (c) Harvie in 2oo6

//Settings:
$start = 0;     //0 //First X
$step = 0.07;    //0.1 //Increase X on every line by this number
$reset = (2*PI()); //When X >= reset, then it will be reseted to zero
$zoom = 1;      //1 //This will repeat every line few times
$offset = 1.1;    //1.1 //1 //0 -> Zero offset will draw only half of sin curve.
$width = 35;     //20 //35 //40 //This will stretch every line.
$sleep = "0";   //15000 - Wait between lines in microseconds
$line = (" ");     // " " // ":" //chr ( 176 ) //This is the string, that will be repeated from display start to the curve (curve inlay).
$endline = ("+<br />\n"); // "#<br />\n" // ".<br />\n" //chr ( 219 )."<br />\n" //This is the string, that will be printed at end of every line (curve outline).

//Read settings from form:
if ($_GET["start"] != "") {
  $start = $_GET["start"];}
if ($_GET["reset"] != "") {
  $reset = $_GET["reset"];}
if ($_GET["step"] != "") {
  $step = $_GET["step"];}

if ($_GET["offset"] != "") {
  $offset = $_GET["offset"];}
if ($_GET["width"] != "") {
  $width = $_GET["width"];}
if ($_GET["zoom"] != "") {
  $zoom = $_GET["zoom"];}

if ($_GET["outline"] != "") {
  $endline = ($_GET["outline"]."<br />\n");}
if ($_GET["inlay"] != "") {
  $line = $_GET["inlay"];}

//Code:
echo ("[SinusCurve]-[c][Harvie][2oo6]<br />\n<br />\n");
echo (" |                                                                            y<br />\n");
echo ("-+-----------------------------------------------------------------------------><span id=\"sipka\">-------> Y ></span><br />\n");
echo ("x|<br />\n");
echo (" V<br /><br />\n<small>\n");

//sleep(2);

$pos = $start;
while(1) {

  //usleep($sleep); //Wait in microseconds
  
  $znaku = ((sin($pos) + $offset) * $width);
  
  $zoom2 = $zoom;
  while ($zoom2 > 0) {
  
    $znaku2 = round($znaku); //It looks better after round ;)
    while ($znaku2 > 0) {
      echo $line;
      $znaku2 = ($znaku2 - 1);
    }
    //echo(" ".$pos); //Debug
    echo ($endline);
    
    $zoom2 = ($zoom2 - 1);
  }
  
  $pos = ($pos + $step);
  if ($pos >= $reset) {
    $pos = 0;
    die();
  }

}
?></pre></small>
