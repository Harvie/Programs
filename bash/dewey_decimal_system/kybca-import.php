#!/usr/bin/php
<?php
//You will need JSON PHP extension for this script...
/* This script have to be included to nodes.php
 * somewhere after all othere needed includes and initializations
 * like that:
 * require('kybca-import.php'); die('hotovo!');
 */

$dds=json_decode(file_get_contents('dds.json'),true);
//die(print_r($dds));

function aaddnode($params) {
	//$_POST['code']=true;
	$params['node_creator']=UBIK_ID;
	return nodes::addNode($params);
	//return(rand(3,100000));
}

function apostrof($txt) {
	return str_replace("'",'',$txt); //kyberia is broken... screw you!!!
}

function recursive_add($tree,$id,$depth) {
	foreach($tree as $key => $value) {
		$new=$key;
		while((!is_numeric($new)) || $new < 2) $new=aaddNode(array(
			'node_parent' => $id, 'node_name' => apostrof($key), 'node_content' => apostrof($key),
			'template_id' => 2, 'node_system_access' => $depth>1?'public':'moderated', 'node_external_access' => 'yes'
		));
		$tree[$new]=$tree[$key]; unset($tree[$key]); //rename key to new
		$tree[$new] = recursive_add($tree[$new],$new,$depth+1);
	}
	//foreach($tree as $key => $value) $tree[$key] = recursive_add($tree[$key],$key);
	return $tree;
}

$dds = recursive_add($dds['dds'],4738157,0);
echo '<pre>'; print_r($dds); echo '</pre>';
