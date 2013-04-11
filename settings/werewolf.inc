<?php
// Times (in seconds)
$wolf['game_wait'] = 60;
$wolf['game_add_wait'] = 20;
$wolf['game_add_wait_limit'] = 2;
$wolf['wait_ping'] = 60 * 5;
$wolf['wait_stats'] = 60 * 1/2;
$wolf['length_night'] = 60 * 2;
$wolf['length_day'] = 60 * 15;
$wolf['night_warn'] = 60 * 1.5;
$wolf['day_warn'] = 60 * 13;

$wolf['min_players'] = 4;
$wolf['max_players'] = 30;

$wolf['part_grace_time'] = 7; // seconds
$wolf['quit_grace_time'] = 60 * 1/2;

$wolf['roles'] = array(
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PLAYERS:           SEER    WOLF   CURSED   DRUNK  HARLOT  TRAITOR  GUNNER   CROW   ANGEL DETECTIVE GHOST   //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      4    => array(   1   ,   1   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ), //
      6    => array(   1   ,   1   ,   1   ,   1   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ), //
      8    => array(   1   ,   2   ,   1   ,   1   ,   1   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ), //
      10   => array(   1   ,   2   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   0   ,   0   ,   0   ), //
<<<<<<< HEAD
      11   => array(   1   ,   2   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   0   ,   0   ), //
      15   => array(   1   ,   3   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ,   1   ), //
      22   => array(   1   ,   4   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ,   2   ), //
      29   => array(   1   ,   5   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ,   2   )  //
=======
      11   => array(   1   ,   2   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   0   ,   1   ), //
      15   => array(   1   ,   3   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ,   1   ), //
      22   => array(   1   ,   4   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ,   1   ), //
      29   => array(   1   ,   5   ,   1   ,   1   ,   1   ,   1   ,   1   ,   0   ,   1   ,   1   ,   1   )  //
>>>>>>> 82c48ee69f05469d569f82cd8ddb548ec48cf05f
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
);

$wolf['shots_multiplier'] = .12; // ceil($shots_multiplier * count($players) )

// Syntax: 1/7 = array(1,7)

//                               HIT          MISS         SUICIDE
$wolf['gun_shot']       = array( array(5,7) , array(1,7) , array(1,7) );
$wolf['gun_shot_drunk'] = array( array(2,7) , array(4,7) , array(1,7) );
$wolf['manslaughter']   = array(1,5); // ACCIDENTAL HEADSHOT (FATAL)

// Not sure what this is -> GUNNER_KILLS_WOLF_AT_NIGHT_CHANCE = 0
$wolf['guardian_angel_dies'] = array(1,2);
$wolf['detective_revealed']  = array(2,5);
?>
