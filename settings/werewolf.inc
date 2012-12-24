<?php
// Times (in seconds)
$setting["game_wait"]=60;
$setting["game_add_wait"]=20;
$setting["game_add_wait_limit"]=2;
$limit["ping"]=60 * 5;
$limit["stats"]=60 * 1/2;
$length["night"]=60 * 2;
$length["day"]=60 * 15;

$roles = array(
////////////////////////////////////////////////////////////////////////////////////////////////////////
// PLAYERS:           SEER    WOLF   CURSED   DRUNK  HARLOT  TRAITOR  GUNNER   CROW   ANGEL DETECTIVE //
////////////////////////////////////////////////////////////////////////////////////////////////////////
      4    => array(   1   ,   1   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ), //
      6    => array(   1   ,   1   ,   1   ,   1   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ), //
      8    => array(   1   ,   2   ,   1   ,   1   ,   1   ,   0   ,   0   ,   0   ,   0   ,   0   ), //
      10   => array(   1   ,   2   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   0   ,   0   ), //
      11   => array(   1   ,   2   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   0   ), //
      15   => array(   1   ,   3   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ), //
      22   => array(   1   ,   4   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ), //
      29   => array(   1   ,   5   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   )  //
////////////////////////////////////////////////////////////////////////////////////////////////////////
);

$shots_multiplier = .12; // ceil($shots_multiplier * count($players) )

// Syntax: 1/7 = array(1,7)

//                               HIT         MISS        SUICIDE
$gun_shot =         array( array(5,7) , array(1,7) , array(1,7) );
$gun_shot_drunk =   array( array(2,7) , array(4,7) , array(1,7) );
$manslaughter =     array(1,5); // ACCIDENTAL HEADSHOT (FATAL)

// Not sure what this is -> GUNNER_KILLS_WOLF_AT_NIGHT_CHANCE = 0
$guardian_angel_dies = array(1,2);
$detective_revealed =  array(2,5);
?>
