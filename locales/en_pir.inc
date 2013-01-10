<?php
// The Pirate Locale, arrrr

// Roles
$en_pir["seer"]="spyglass pirate";
$en_pir["wolf"]="rebel";
$en_pir["cursed"]="suspicious pirate";
$en_pir["drunk"]="drunk pirate";
$en_pir["harlot"]="pirate of love";
$en_pir["traitor"]="secret rebel";
$en_pir["gunner"]="technilogical pirate";
$en_pir["crow"]="rebel spy";
$en_pir["angel"]="guardian pirate";
$en_pir["detective"]="detective pirate";
$en_pir["villager"]="pirate";

$en_pir["seer_plural"]="spyglass pirates";
$en_pir["wolf_plural"]="rebels";
$en_pir["cursed_plural"]="suspicious pirates";
$en_pir["drunk_plural"]="drunk pirates";
$en_pir["harlot_plural"]="pirates of love";
$en_pir["traitor_plural"]="secret rebels";
$en_pir["gunner_plural"]="technilogical pirates";
$en_pir["crow_plural"]="rebel spies";
$en_pir["angel_plural"]="guardian pirates";
$en_pir["detective_plural"]="detective pirates";
$en_pir["villager_plural"]="pirates";

// Before Game
$en_pir["game_start"]="<bold>%answer1%</bold> has started a game of Werewolf - Pirate Edition. Type \"!join\" to join. Type \"!start\" to start the game. Type \"!wait\" to increase join wait time.";
$en_pir["game_wait"]="Mate, please wait at least %answer1% more seconds.";
$en_pir["game_required"]="%answer1%: Four or more fellow pirates are required to play.";
$en_pir["game_welcome"]="%answer1%: Welcome, Matey, to Werewolf - Pirate Edition, the popular detective/social party game (a theme of Mafia).";

// Night time
$en_pir["night_first"]="It is now nighttime aboard the ship. All players check for PMs from me, yer head pirate, for instructions. If you did not receive one, simply sit back, relax, have a beer with your mateys, and wait patiently for morning.";
$en_pir["night_turn"]="Day lasted <bold>%answer1%</bold>. It is now nighttime aboard the ship. All players check for PMs from me, yer head pirate, for instructions. If you did not receive one, simply sit back, relax, have a beer with your mateys, and wait patiently for morning.";
$en_pir["night_victim"]="The mob drags a protesting <bold>%answer1%</bold> to walk the plank. S/He is eaten by a shark, in typical dramatic pirate fashion. It is discovered through a note they left in their cabin that (s)he was a <bold>%answer2%</bold>.";

// Day time turn
$en_pir["day_turn"]="Night lasted <bold>%answer1%</bold>. It is now daytime. The pirates awake, thankful for surviving the night, and search the ship for any victims of the rebellion... ";
$en_pir["day_victim"]="The dead body of <bold>%answer1%</bold>, a <bold>%answer2%</bold>, is found. Those remaining have a drink in their honour.";
$en_pir["day_nokill"]="The body of a young parrot pet is found. All pirates, however, have survived.";
$en_pir["day_harlot"]="The rebels' selected victim was a pirate of love, but she wasn't home.";
$en_pir["day_harlot_dead"]="<bold>%answer1%</bold>, a <bold>pirate of love</bold>, made the unfortunate mistake of visiting a rebel's house last night and is now dead.";
$en_pir["day_angel"]="<bold>%answer1%</bold> was attacked by the rebels last night, but luckily, the guardian pirate protected him/her.";
$en_pir["day_angel_dead"]="<bold>%answer1%</bold>, a <bold>guardian pirate</bold> made the unfortunate mistake of guarding a rebel last night, attempted to escape, but failed and is now dead.";
$en_pir["day_vote"]="The villagers must now vote for whom to toss overboard. Use \"!vote <nick>\" to cast your vote. %answer1% votes are required to toss.";

// Maintenance
$en_pir["idle_warn"]="%answer1%: <bold>Matey, you've been in bed for a while. Please say something soon or you might be declared dead. (Or as having a really bad hangover.)</bold>";
$en_pir["idle_die"]="<bold>auror</bold> didn't get out of bed for a very long time. S/He is declared dead. Appears (s)he was a <bold>seer</bold>.";

// Utilities
$en_pir["help"]="(notice) Commands: <bold>help, simple, back, quit, stats, away, ping, start, retract, goat, shoot, votes, lynch, coin, wait, join, rules, admins</bold>";
$en_pir["ping"]="PING! %answer1%";
$en_pir["coin_toss"]="<bold>%answer1%</bold> tosses a coin into the air...";
$en_pir["coin_land"]="The coin lands on <bold>%answer1%</bold>";
$en_pir["away"]="(notice) You are now marked as away.";
$en_pir["back"]="(notice) You are no longer marked as away.";
$en_pir["stats"]="%answer1%: <bold>%answer2%</bold> pirates: %answer3%";
$en_pir["stats_none"]="%answer1%: There are no pirates on the ship.";
$en_pir["stats_roles"]="%answer1%: There is %answer2%";
$en_pir["wait"]="<bold>%answer1%</bold> increased the wait time by %answer2% seconds.";
$en_pir["wait_reached"]="Limit has already been reached for extending the wait time.";
$en_pir["rules_part1"]="##phpwolf channel rules: 1) Be nice to others. 2) Do not share information after death. 3) No bots allowed. 4) Do not play with clones.";
$en_pir["rules_part2"]="5) Do not quit unless you need to leave. 6) No swearing and keep it family-friendly. 7) Do not paste PM's from the bot during the game. 8) Use common sense. 9) Waiting for timeouts is discouraged.";
//$en_pir["votes"]="%answer1%: njstein: 1 (gluxon), gluxon: 2 (Nascar1996 rwg), FoolsTP: 1 (Mithadon)";
$en_pir["votes"]="%answer1%: %answer2%";
$en_pir["votes_none"]="%answer1%: No votes yet.";
$en_pir["votes_status"]="%answer1%: <bold>%answer2%</bold> players, <bold>%answer3%</bold> votes required to lynch, <bold>%answer4%</bold> players available to vote.";
$en_pir["votes_night"]="You can only toss someone overboard during the day, matey!";
$en_pir["goat"]="<bold>%answer1%</bold>'s parrot flies by and bites <bold>%answer2%</bold>";

// Game commands
$en_pir["join"]="<bold>%answer1%</bold> has joined the ship.";
$en_pir["quit"]="<bold>%answer1%</bold> died of an overdose of alcohol. S/He was a <bold>%answer2%</bold>.";
$en_pir["lynch"]="<bold>%answer1%</bold> votes for <bold>%answer2%</bold>.";
$en_pir["lynch_night"]="Tossing overboard is only allowed during the day. Please wait patiently for morning.";
$en_pir["lynch_resting"]="%answer1%: You were shot by the technilogical pirate with his fancy new gun, thus you are unable to vote for the day.";
$en_pir["retract"]="<bold>%answer1%</bold> retracted his/her vote.";
$en_pir["retract_not"]="You haven't voted yet.";

// Notices
$en_pir["game_none"]="No game is currently running.";
$en_pir["ratelimit"]="This command is rate-limited. Please wait a while before using it again.";
$en_pir["join_already_playing"]="You're already playing!";
$en_pir["start_ongoing"]="Werewolf is already in play.";
$en_pir["not_playing"]="<bold>%answer1%</bold> is currently not playing.";
$en_pir["not_playing_self"]="You're not currently playing.";
$en_pir["not_admin"]="You are not an admin.";
$en_pir["away"]="You are now marked as away.";
$en_pir["not_away"]="You are not marked as away.";
$en_pir["back"]="You are no longer marked as away.";
$en_pir["parameters"]="Not enough parameters.";

// End Game
$en_pir["win_villagers"]="Game over! All the rebels are dead! The villagers chop them up, BBQ them, and have a hearty meal.";
$en_pir["win_wolves"]="Game over! There are the same number of rebels as pirates. The rebels kill everyone and win.";
$en_pir["end_times"]="Game lasted <bold>%answer1%</bold>. <bold>%answer2%</bold> was day. <bold>%answer3%</bold> was night.";
//The wolves are always morirarty and Nascar1996
$en_pir["end_roles"]="The %answer1% was <bold>%answer2%</bold>.";
$en_pir["end_roles_plural"]="The %answer1% were <bold>%answer2%</bold>.";

// Directions
$en_pir["seer_assign"]="You are a <bold>spyglass pirate</bold>. It is your job to detect the rebels, you may use your spyglass once per night. Use \"see <nick>\" to see the role of a player.";
$en_pir["seer_see"]="You see a pirate; and you see that <bold>%answer1%</bold> is a <bold>%answer2%</bold>!";
$en_pir["seer_see_not"]="Only a seer may use this command";
$en_pir["seer_seen"]="You may only see one pirate per round.";
$en_pir["seer_see_self"]="You can't see yourself without a mirror. You don't have a mirror.";
$en_pir["seer_see_day"]="You may only see pirates at night.";

$en_pir["wolf_assign"]="You are a <bold>rebel</bold>. It is your job to kill all the pirates. Use \"kill <nick>\" to kill a pirate.";
$en_pir["wolf_multiple"]="Also, if you PM me, your message will be relayed to other rebels.";
$en_pir["wolf_kill"]="You have selected <bold>%answer1%</bold> to be killed.";
$en_pir["wolf_relay"]="<bold>%answer1%</bold> says: %answer2%";

$en_pir["drunk_assign"]="You have been drinking too much! For a pirate, anyway. You are the <bold>drunk pirate</bold>.";

$en_pir["harlot_assign"]="You are a <bold>pirate of love</bold>. You may spend the night with one pirate per round. If you visit a victim of a rebel, or visit a rebel, you will die. Use !visit to visit a player.";
$en_pir["harlot_visit"]="You are spending the night with <bold>%answer1%</bold>. Have a good time!";
$en_pir["harlot_visit_not"]="Only a pirate of love may use this command.";
$en_pir["harlot_visit_self"]="You have chosen to stay in your hammock for the night.";
$en_pir["harlot_visited"]="You are already spending the night with <bold>%answer1%</bold>.";
$en_pir["harlot_day"]="You may only visit someone at night.";
$en_pir["harlot_dead"]="<bold>%answer1%<bold>, a <bold>harlot<bold>, made the unfortunate mistake of visiting the victim's house last night and is now dead.";

$en_pir["traitor_assign"]="You are a <bold>secret rebel</bold>. You are exactly like a pirate and not even a spyglass pirate can see your true identity. Only detective pirates can.";
$en_pir["traitor_transform"]="<bold>The pirates, during their celebrations, are frightened as they hear a gun fire. The rebels are not gone!</bold>";
$en_pir["traitor_notify"]="You have become a rebel!";
$en_pir["traitor_avenge"]="It is up to you to avenge your fallen leaders!";

$en_pir["gunner_assign"]="You hold a technilogical gun that shoots special rebel-detecting bullets. You may only use it during the day. If you shoot a rebel, (s)he will die instantly, but if you shoot a pirate, that pirate will likely survive. There is also the possibility of the gun blowing up, as it is a beta version, and killing you with it, so take caution. You get %answer1% bullets.";
$en_pir["gunner_shoot"]="<bold>%answer1%</bold> shoots <bold>%answer2%</bold> with a rebel-detecing bullet!";
$en_pir["gunner_shoot_wolf"]="<bold>%answer1%</bold> is a wolf, and is dying from the rebel-detecting bullet.";
$en_pir["gunner_shoot_villager"]="<bold>%answer1%</bold> is a pirate and is injured but will have a full recovery. S/He will be resting for the day.";
$en_pir["gunner_shoot_not"]="You don't have a gun.";
$en_pir["gunner_shoot_self"]="You are holding it the wrong way.";
$en_pir["gunner_nobullets"]="You don't have any more bullets.";
$en_pir["gunner_shoot_manslaughter"]="<bold>%answer1%</bold> is a not a rebel but was accidentally fatally injured.";
$en_pir["gunner_reveal"]="Appears (s)he was a <bold>%answer1%</bold>";
$en_pir["gunner_miss"]="<bold>%answer1%</bold> is a lousy shooter. S/He missed!";
$en_pir["gunner_suicide"]="<bold>%answer1%</bold> shouldn't use beta versions. The technilogical gun exploded and killed him/her!";
$en_pir["gunner_night"]="Shooting is only allowed during the day. Please wait patiently for morning.";

$en_pir["angel_assign"]="You are a <bold>guardian pirate</bold>. It is your job to protect the pirates. If you guard a rebel, there is a 50/50 chance of you dying, if you guard a victim, they will live. Use !guard to guard a player.";
$en_pir["angel_guard"]="You are protecting <bold>%answer1%</bold> tonight. Farewell!";
$en_pir["angel_guard_notify"]="You can sleep well tonight, for a guardian pirate is protecting you.";
$en_pir["angel_guard_not"]="Only a guardian pirate may use this command.";
$en_pir["angel_guard_self"]="You may not guard yourself.";
$en_pir["angel_guarded"]="You are already protecting <bold>%answer1%.</bold>";
$en_pir["angel_day"]="You may only protect people at night.";

$en_pir["detective_assign"]="You are a <bold>detective pirate</bold>.";
$en_pir["detective_assign2"]="It is your job to determine all the rebels and secret rebels. Your job is during the day, and you can see the true identity of all users, even secret rebels.";
$en_pir["detective_assign3"]="But, each time you use your ability, you risk a 2/5 chance of having your identity revealed to the rebels. So be careful. Use \"!id\" to identify any player during the day.";
$en_pir["detective_id"]="The results of your investigation have returned. <bold>%answer1%</bold> is a... %answer2%!";
$en_pir["detective_id_not"]="Only a detective pirate may use this command.";
$en_pir["detective_id_self"]="You may not id yourself."; // NOT OFFICIAL
$en_pir["detective_ided"]="You may only investigate one person per round.";
$en_pir["detective_night"]="You may only investigate people during the day.";
$en_pir["detective_revealed"]="<bold>%answer1%</bold> accidentally drops a paper. The paper reveals that (s)he is the detective pirate!";

$en_pir["werecrow_assign"]="You are a <bold>rebel spy</bold>. You are able to spy at night, much as the seer does. Use \"kill <nick>\" to kill a a villager. Alternatively, you can use \"observe <nick>\" to check if someone is in bed or not. Observing will prevent you from participating in a killing.";
$en_pir["werecrow_observe"]="You train your spyglass on <bold>%answer1%</bold>'s house. You will return after collecting your observations when day begins.";
$en_pir["werecrow_observed"]="You are already spying on  to <bold>%answer1%</bold>'s house.";
$en_pir["werecrow_observe_self"]="Instead of doing that, you should probably go kill someone.";
$en_pir["werecrow_observe_wolf"]="Flying to another rebel's house is a waste of time.";
$en_pir["werecrow_observe_day"]="You may only spy at night.";
$en_pir["werecrow_not"]="Only a rebel spy may use this command.";

$en_pir["players_list"]="Pirates: %answer1%";
?>

