<title>PeriodiCalendar</Title>
<h1>PeriodiCal</h1>
<small>Calendar with period highlighting ability</small>
<?php if(!isset($_GET['last'])) { ?>
<form action="?" method="GET">
	Last: <input type="text" name="last" value="2010-01-20" title="YYYY-MM-DD date of last occurence" />
	Periodicity: <input type="text" name="period" value="28" title="Periodicity of cycle in days" />
	Duration: <input type="text" name="len" value="7" title="Days to highlight from beginning of cycle" />
	Months: <input type="text" name="months" value="12" title="How many months you want to draw" />
	<input type="submit" value="DRAW" />
</form>

<?php
	echo("\n<!-- OPEN SOURCE:\n");readfile(__FILE__);echo("\n-".'->');
} else {
//init
error_reporting(0);
$day = 60*60*24;
//get numbers
$now = time(); $now = mktime(12, 0, 0, date('n',$now), 1, date('Y',$now));
$last = date_parse_from_format('Y-m-d', $_GET['last']);
$last = mktime(12, 0, 0, $last['month'], $last['day'], $last['year']);
$period=28; if(isset($_GET['period'])) $period = $_GET['period'];
$length=7; if(isset($_GET['len'])) $length = $_GET['len'];
$months=12; if(isset($_GET['months'])) $months = $_GET['months'];

echo('<table border=1>');
$r = 0;
for($i=0;$i<$months;$i++) {
	$lm = $m = date('m',$now);
	$mo = date('M',$now);
	echo("<tr><td bgcolor='lightblue'><b>$m ($mo)</b></td>");
	while($m == $lm) {
		if(round(($now-$last)/$day)%($period) == 0) $r = $length;	
		$c = '';
		if($r>0) $c = '#FF0000';
		if($r==1 || $r==$length) $c = '#FF7777';
		$r--;
		echo("<td bgcolor='$c'>".date('d', $now).'</td>');
		$now += $day;
		$m = date('m',$now);
	}
	echo('</tr>');
}
echo('</table><small>You can bookmark this page it will get updated automatically each month.</small>');
}
