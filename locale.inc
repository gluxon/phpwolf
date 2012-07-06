<?php
// The English locale

// Roles
$en_US["seer"]="seer";
$en_US["wolf"]="wolf";
$en_US["wolf_plural"]="wolves";
$en_US["cursed"]="cursed villager";
$en_US["drunk"]="village drunk";
$en_US["harlot"]="harlot";
$en_US["traitor"]="traitor";
$en_US["gunner"]="gunner";
$en_US["angel"]="guardian angel";
$en_US["detective"]="detective";

// Before Game
$en_US["game_start"]="<bold>%nick%</bold> has started a game of Werewolf. Type \"!join\" to join. Type \"!start\" to start the game. Type \"!wait\" to increase join wait time.";
$en_US["game_wait"]="Please wait at least %answer% more seconds.";
$en_US["game_required"]="%nick%: Four or more players are required to play.";

$en_US["game_welcome"]="%answer%: Welcome to Werewolf, the popular detective/social party game (a theme of Mafia).";

// Night time
$en_US["night_first"]="It is now nighttime. All players check for PMs from me for instructions. If you did not receive one, simply sit back, relax, and wait patiently for morning.";

// Maintenance
$en_US["idle_warn"]="%nick%: <bold>You have been idling for a while. Please say something soon or you might be declared dead.</bold>";
$en_US["idle_die"]="<bold>auror</bold> didn't get out of bed for a very long time. S/He is declared dead. Appears (s)he was a <bold>seer</bold>.";

// Utilities
$en_US["help"]="(notice) Commands: <bold>help, simple, back, quit, stats, away, ping, start, retract, goat, shoot, votes, lynch, coin, wait, join, rules, admins</bold>";
$en_US["ping"]="PING! %answer%";
$en_US["coin_toss"]="<bold>%nick%</bold> tosses a coin into the air...";
$en_US["coin_land"]="The coin lands on <bold>%answer%</bold>";
$en_US["away"]="(notice) You are now marked as away.";
$en_US["back"]="(notice) You are no longer marked as away.";
$en_US["stats"]="%nick%: <bold>%answer%</bold> players: %answer2%";
$en_US["stats_none"]="%nick%: There are no players.";
$en_US["wait"]="<bold>%nick%</bold> increased the wait time by %answer% seconds.";
$en_US["wait_limit"]="Limit has already been reached for extending the wait time.";

// Game commands
$en_US["join"]="<bold>%nick%</bold> has joined the game.";
$en_US["quit"]="<bold>%nick%</bold> died of an unknown disease. S/He was a <bold>%answer%</bold>.";

// Notices
$en_US["game_none"]="(notice) No game is currently running.";
$en_US["ratelimit"]="(notice) This command is rate-limited. Please wait a while before using it again.";
$en_US["join_already_playing"]="(notice) You're already playing!";
$en_US["start_ongoing"]="(notice) Werewolf is already in play.";
$en_US["not_playing"]="<bold>%nick%</bold> is currently not playing.";

// End Game
$en_US["win_villagers"]="Game over! All the wolves are dead! The villagers chop them up, BBQ them, and have a hearty meal.";
$en_US["win_wolves"]="Game over! There are the same number of wolves as villagers. The wolves eat everyone and win.";
$en_US["end_times"]="Game lasted <bold>12:56</bold>. <bold>08:39</bold> was day. <bold>04:17</bold> was night.";
//The wolves were moriarty and Nascar1996
$en_US["end_roles"]="The %answer% was <bold>%answer2%</bold>.";

// Directions
$en_US["seer_assign"]="You are a <bold>seer</bold>. It is your job to detect the wolves, you may have a vision once per night. Use \"see <nick>\" to see the role of a player.";
$en_US["seer_list"]="Players: Arti_, genii-around, Grovyle";

$en_US["wolf_assign"]="You are a <bold>wolf</bold>. It is your job to kill all the villagers. Use \"kill <nick>\" to kill a villager.";
$en_US["wolf_group"]="Also, if you PM me, your message will be relayed to other wolves.";
$en_US["wolf_list"]="<bold>Players:</bold> auror (wolf), ishanyx, Mithadon, MrBenC, sheep, Snuupy, Surfcash";
$en_US["wolf_kill"]="You have selected <bold>Snuupy</bold> to be killed.";
$en_US["wolf_relay"]="<bold>auror</bold> says: kill snu";

$en_US["drunk_assign"]="You have been drinking too much! You are the <bold>village drunk</bold>.";

$en_US["harlot_assign"]="You are a <bold>harlot</bold>. You may spend the night with one person per round. If you visit a victim of a wolf, or visit a wolf, you will die. Use !visit to visit a player.";
$en_US["harlot_visit"]="You are spending the night with <bold>moriarty</bold>. Have a good time!";

$en_US["gunner_assign"]="You are a <bold>gunner</bold>. You hold a gun that shoots special silver bullets. You may only use it during the day. If you shoot a wolf, (s)he will die instantly, but if you shoot a villager, that villager will likely survive. There is also the possibility of the gun blowing up, and killing you with it, so take caution. You get 2 bullets.";

// Events
$en_US["traitor_wolf"]="<bold>The villagers, during their celebrations, are frightened as they hear a loud howl. The wolves are not gone!</bold>";
?>