#!/usr/bin/php
<?php
$refresh = 1;
$battery = "/proc/acpi/battery/BAT1";
$critical = "echo Shutting down... - Low Battery";
$warning = "echo Warning!!! - Low Battery";

///CHECK_BATT
function check_battery($bat = "/proc/acpi/battery/BAT1", $critical = "echo Shutting down... - Low Battery", $warning = "echo Warning!!! - Low Battery") {
	$info = fopen($bat.'/info', 'r');
	while(!feof($info)) {
		$line = fgets($info);
		if(eregi('design capacity warning', $line)) {
			$line = explode(' ', $line);
			//print_r($line);
			$warncap = $line[3];
		}
		if(eregi('design capacity low', $line)) {
			$line = explode(' ', $line);
			//print_r($line);
			$lowcap = $line[7];
		}
	}
	fclose($info);
	
	$dischg = 'Chg';
	$state = fopen($bat.'/state', 'r');
	while(!feof($state)) {
		$line = fgets($state);
		if(eregi('discharging', $line)) {
			$dischg = 'Dischg';
		}
		if(eregi('remaining capacity', $line)) {
			$line = explode(' ', $line);
			//print_r($line);
			$remaincap = $line[7];
		}
	}
	fclose($state);
	
	echo("Remaining capacity $remaincap mAh (Critical: $lowcap mAh; Warning: $warncap mAh) $dischg        \r");
	if($dischg == 'Dischg') {
		if($remaincap < $lowcap) { echo("\n"); system($critical); }
		if($remaincap < $warncap) { echo("\n"); system($warning); }
	}
}

///LOOP
echo("Watching battery $battery\n");
while(1) {
	check_battery($battery, $critical, $warning);
	sleep($refresh);
}