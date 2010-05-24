<?php
///SETTINGS//////////////////////////////////////////////////////////////////////////////////////////////////////
//Login
$realm = 'secret_zone'; //This is used by browser to identify protected area and saving passwords (one_site+one_realm==one_user+one_password)
$user = 'root'; //User
$passwd = 'toor'; //Password
//Misc
$require_login = true; //Require login? (if false, no login needed) - WARNING!!!
$location = '401'; //Location after logout - 401 = default logout page (can be overridden by ?logout=[LOCATION])
//CopyLeft
$ver = '3.7.1';
$link = '<a href="https://harvie.ath.cx/">harvie.ath.cx</a>';
$banner = "Harvie's PHP HTTP-Auth script (v$ver)";
$hbanner = "<hr /><i>$banner\n$link</i>\n";
$cbanner = "<!-- $banner -->\n";
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MANUAL/////////////////////////////////////////////////////////////////////////////////////////////////////////
/* HOWTO
 * To each file, you want to lock add this line (at begin of first line):
 * <?php include('http_auth.php'); ?>
 * This file have to be php script (if it's html, simply rename it to .php)
 * Server have to run PHP (not CGI).
 * You need HTTP Basic auth enabled on server and in php.ini
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////CODE/////////////////////////////////////////////////////////////////////////////////////////////////////////
  function send_auth_headers($realm='') {
    Header('WWW-Authenticate: Basic realm="'.$realm.'"');
    Header('HTTP/1.0 401 Unauthorized');
  }
  
  function check_auth($PHP_AUTH_USER, $PHP_AUTH_PW) { //Check if login is succesfull (U can modify to use DB, or anything else)
	return (($PHP_AUTH_USER == $GLOBALS['user']) && ($PHP_AUTH_PW == $GLOBALS['passwd']));
  }
    
  function unauth() { //Do this when login fails
    $cbanner = $GLOBALS['cbanner'];
    $hbanner = $GLOBALS['hbanner'];
    die("$cbanner<title>401 - Forbidden</title>\n<h1>401 - Forbidden</h1>\n<a href=\"?\">Login...</a>\n$hbanner"); //Show warning and die
    die(); //Don't forget!!!
  }

//Back-Compatibility
if(isset($_SERVER['PHP_AUTH_USER']) && $_SERVER['PHP_AUTH_PW'] != '') $PHP_AUTH_USER = $_SERVER['PHP_AUTH_USER'];
if(isset($_SERVER['PHP_AUTH_PW']) && $_SERVER['PHP_AUTH_PW'] != '') $PHP_AUTH_PW = $_SERVER['PHP_AUTH_PW'];

//Logout
if(isset($_GET['logout'])) { //script.php?logout
  if(isset($PHP_AUTH_USER) || isset($PHP_AUTH_PW)) {
    Header('WWW-Authenticate: Basic realm="'.$realm.'"');
    Header('HTTP/1.0 401 Unauthorized');
  } else {
    if($_GET['logout'] != '') $location = $_GET['logout'];
    if(trim($location) != '401') Header('Location: '.$location);
    die("$cbanner<title>401 - Log out successfull</title>\n<h1>401 - Log out successfull</h1>\n<a href=\"?\">Continue...</a>\n$hbanner");
  }
}

if($require_login) {
  if(!isset($PHP_AUTH_USER)) { //Storno or first visit of page
    send_auth_headers($realm);
    unauth();
  } else { //Login sent
    
    if (check_auth($PHP_AUTH_USER, $PHP_AUTH_PW)) { //Login succesfull - probably do nothing
    } else { //Bad login
      send_auth_headers($realm);
      unauth();
    }
    
  }
}
//Rest of file will be displayed only if login is correct

