<?php
// Server configuration
$server["address"]="irc.freenode.net";
$server["port"]="6667";
$server["password"]="";

// Bot information configuration
$user["nick"]="phpwolf";
$user["mode"]="gluxon.com";
$user["unused"]="unused";
$user["realname"]="sherly";
$user["password"]="password";

// Default join channel
$channel["name"]="##phpwolf";

// Admins array
$admins = array("gluxon!~gluxon@unaffiliated/gluxon", "Snuupy!~Snuupy@unaffiliated/snuupy");

// Language
$lang=$en_US;

// Times (in seconds)
$setting["game_wait"]=0;
$setting["game_add_wait"]=20;
$setting["game_add_wait_limit"]=2;
$ping_wait=60 * 5;
$night=60 * 2;

$roles = array(
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// PLAYERS:           SEER    WOLF   CURSED   DRUNK   HARLOT  TRAITOR  GUNNER   CROW    ANGEL DETECTIVE  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
      4    => array(   1   ,   1   ,   0   ,   0   ,   0   ,    0   ,   0   ,   0    ,   0   ,   0   ), //
      6    => array(   1   ,   1   ,   1   ,   1   ,   0   ,    0   ,   0   ,   0    ,   0   ,   0   ), //
      8    => array(   1   ,   2   ,   1   ,   1   ,   1   ,    0   ,   0   ,   0    ,   0   ,   0   ), //
      10   => array(   1   ,   2   ,   1   ,   1   ,   1   ,    1   ,   1   ,   0    ,   0   ,   0   ), //
      11   => array(   1   ,   2   ,   1   ,   1   ,   1   ,    1   ,   1   ,   0    ,   1   ,   0   ), //
      15   => array(   1   ,   3   ,   1   ,   1   ,   1   ,    1   ,   1   ,   0    ,   1   ,   1   ), //
      22   => array(   1   ,   4   ,   1   ,   1   ,   1   ,    1   ,   1   ,   0    ,   1   ,   1   ), //
      29   => array(   1   ,   5   ,   1   ,   1   ,   1   ,    1   ,   1   ,   0    ,   1   ,   1   )  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
);

?>
