<?php
// The English locale

// Roles
$en_US["seer"]="seer";
$en_US["wolf"]="wolf";
$en_US["cursed"]="cursed villager";
$en_US["drunk"]="village drunk";
$en_US["harlot"]="harlot";
$en_US["traitor"]="traitor";
$en_US["gunner"]="gunner";
$en_US["crow"]="crow";
$en_US["angel"]="guardian angel";
$en_US["detective"]="detective";
$en_US["villager"]="villager";

$en_US["seer_plural"]="seers";
$en_US["wolf_plural"]="wolves";
$en_US["cursed_plural"]="cursed villagers";
$en_US["drunk_plural"]="drunk villagers";
$en_US["harlot_plural"]="harlots";
$en_US["traitor_plural"]="traitors";
$en_US["gunner_plural"]="gunners";
$en_US["crow_plural"]="crows";
$en_US["angel_plural"]="guardian angels";
$en_US["detective_plural"]="detectives";
$en_US["villager_plural"]="villagers";

// Before Game
$en_US["game_start"]="<bold>%nick%</bold> has started a game of Werewolf. Type \"!join\" to join. Type \"!start\" to start the game. Type \"!wait\" to increase join wait time.";
$en_US["game_wait"]="Please wait at least %answer% more seconds.";
$en_US["game_required"]="%nick%: Four or more players are required to play.";
$en_US["game_welcome"]="%answer%: Welcome to Werewolf, the popular detective/social party game (a theme of Mafia).";

// Night time
$en_US["night_first"]="It is now nighttime. All players check for PMs from me for instructions. If you did not receive one, simply sit back, relax, and wait patiently for morning.";
$en_US["night_turn"]="Day lasted <bold>%answer%</bold>. It is now nighttime. All players check for PMs from me for instructions. If you did not receive one, simply sit back, relax, and wait patiently for morning.";
$en_US["night_victim"]="The mob drags a protesting <bold>%answer%</bold> to the hanging tree. S/He succumbs to the will of the horde, and is hanged. It is discovered (s)he was a <bold>%answer2%</bold>.";

// Day time turn
$en_US["day_turn"]="Night lasted <bold>%answer%</bold>. It is now daytime. The villagers awake, thankful for surviving the night, and search the village... ";
$en_US["day_victim"]="The dead body of <bold>%answer%</bold>, a <bold>%answer2%</bold>, is found. Those remaining mourn his/her death.";
$en_US["day_nokill"]="The body of a young penguin pet is found. All villagers, however, have survived.";
$en_US["day_harlot"]="The wolves' selected victim was a harlot, but she wasn't home.";
$en_US["day_harlot_dead"]="<bold>%answer%</bold>, a <bold>harlot</bold>, made the unfortunate mistake of visiting a wolf's house last night and is now dead.";
$en_US["day_angel"]="<bold>%answer%</bold> was attacked by the wolves last night, but luckily, the guardian angel protected him/her.";
$en_US["day_angel_dead"]="<bold>%answer%</bold>, a <bold>guardian angel</bold> made the unfortunate mistake of guarding a wolf last night, attempted to escape, but failed and is now dead.";
$en_US["day_vote"]="The villagers must now vote for whom to lynch. Use \"!lynch <nick>\" to cast your vote. %answer% votes are required to lynch.";

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
$en_US["stats_roles"]="%nick%: There is %answer%";
$en_US["wait"]="<bold>%nick%</bold> increased the wait time by %answer% seconds.";
$en_US["wait_limit"]="Limit has already been reached for extending the wait time.";
$en_US["rules_part1"]="##phpwolf channel rules: 1) Be nice to others. 2) Do not share information after death. 3) No bots allowed. 4) Do not play with clones.";
$en_US["rules_part2"]="5) Do not quit unless you need to leave. 6) No swearing and keep it family-friendly. 7) Do not paste PM's from the bot during the game. 8) Use common sense. 9) Waiting for timeouts is discouraged.";
//$en_US["votes"]="%nick%: njstein: 1 (gluxon), gluxon: 2 (Nascar1996 rwg), FoolsTP: 1 (Mithadon)";
$en_US["votes"]="%nick%: %answer%";
$en_US["votes_required"]="%nick%: <bold>%answer%</bold> players, <bold>%answer2%</bold> votes required to lynch, <bold>%answer%</bold> players available to vote.";
$en_US["votes_night"]="Voting is only during the day.";
$en_US["goat"]="<bold>%nick%</bold>'s goat walks by and kicks <bold>%answer%</bold>";

// Game commands
$en_US["join"]="<bold>%nick%</bold> has joined the game.";
$en_US["quit"]="<bold>%nick%</bold> died of an unknown disease. S/He was a <bold>%answer%</bold>.";
$en_US["lynch"]="<bold>%nick%</bold> votes for <bold>%answer%</bold>.";
$en_US["lynch_night"]="Lynching is only allowed during the day. Please wait patiently for morning.";
$en_US["lynch_resting"]="%nick%: You are wounded and resting, thus you are unable to vote for the day.";
$en_US["retract"]="<bold>%nick%</bold> retracted his/her vote.";
$en_US["retract_not"]="You haven't voted yet.";

// Notices
$en_US["game_none"]="No game is currently running.";
$en_US["ratelimit"]="This command is rate-limited. Please wait a while before using it again.";
$en_US["join_already_playing"]="You're already playing!";
$en_US["start_ongoing"]="Werewolf is already in play.";
$en_US["not_playing"]="<bold>%answer%</bold> is currently not playing.";
$en_US["not_playing_self"]="You're not currently playing.";
$en_US["not_admin"]="You are not an admin.";
$en_US["away"]="You are now marked as away.";
$en_US["not_away"]="You are not marked as away.";
$en_US["back"]="You are no longer marked as away.";
$en_US["parameters"]="Not enough parameters.";

// End Game
$en_US["win_villagers"]="Game over! All the wolves are dead! The villagers chop them up, BBQ them, and have a hearty meal.";
$en_US["win_wolves"]="Game over! There are the same number of wolves as villagers. The wolves eat everyone and win.";
$en_US["end_times"]="Game lasted <bold>%answer%</bold>. <bold>%answer2%</bold> was day. <bold>%answer3%</bold> was night.";
//The wolves were moriarty and Nascar1996
$en_US["end_roles"]="The %answer% was <bold>%answer2%</bold>.";
$en_US["end_roles_plural"]="The %answer% were <bold>%answer2%</bold>.";

// Directions
$en_US["seer_assign"]="You are a <bold>seer</bold>. It is your job to detect the wolves, you may have a vision once per night. Use \"see <nick>\" to see the role of a player.";
$en_US["seer_see"]="You have a vision; in this vision, you see that <bold>%answer%</bold> is a <bold>%answer2%</bold>!";
$en_US["seer_see_not"]="Only a seer may use this command";
$en_US["seer_seen"]="You may only have one vision per round.";
$en_US["seer_see_day"]="You may only have visions at night.";

$en_US["wolf_assign"]="You are a <bold>wolf</bold>. It is your job to kill all the villagers. Use \"kill <nick>\" to kill a villager.";
$en_US["wolf_multiple"]="Also, if you PM me, your message will be relayed to other wolves.";
$en_US["wolf_kill"]="You have selected <bold>%answer%</bold> to be killed.";
$en_US["wolf_relay"]="<bold>%nick%</bold> says: %answer%";

$en_US["drunk_assign"]="You have been drinking too much! You are the <bold>village drunk</bold>.";

$en_US["harlot_assign"]="You are a <bold>harlot</bold>. You may spend the night with one person per round. If you visit a victim of a wolf, or visit a wolf, you will die. Use !visit to visit a player.";
$en_US["harlot_visit"]="You are spending the night with <bold>%answer%</bold>. Have a good time!";
$en_US["harlot_visit_not"]="Only a harlot may use this command.";
$en_US["harlot_visit_self"]="You have chosen to stay home for the night.";
$en_US["harlot_visited"]="You are already spending the night with <bold>%answer%</bold>.";
$en_US["harlot_day"]="You may only visit someone at night.";

$en_US["traitor_assign"]="You are a <bold>traitor</bold>. You are exactly like a villager and not even a seer can see your true identity. Only detectives can.";
$en_US["traitor_transform"]="<bold>The villagers, during their celebrations, are frightened as they hear a loud howl. The wolves are not gone!</bold>";
$en_US["traitor_notify"]="HOOOOOOOOOWL. You have become... a wolf!";
$en_US["traitor_avenge"]="It is up to you to avenge your fallen leaders!";

$en_US["gunner_assign"]="You hold a gun that shoots special silver bullets. You may only use it during the day. If you shoot a wolf, (s)he will die instantly, but if you shoot a villager, that villager will likely survive. There is also the possibility of the gun blowing up, and killing you with it, so take caution. You get <bold>%answer%</bold> bullets.";
$en_US["gunner_shoot"]="<bold>%nick%</bold> shoots <bold>%answer%</bold> with a silver bullet!";
$en_US["gunner_shoot_wolf"]="<bold>%answer%</bold> is a wolf, and is dying from the silver bullet.";
$en_US["gunner_shoot_villager"]="<bold>%answer%</bold> is a villager and is injured but will have a full recovery. S/He will be resting for the day.";
$en_US["gunner_shoot_not"]="You don't have a gun.";
$en_US["gunner_shoot_self"]="You are holding it the wrong way.";
$en_US["gunner_nobullets"]="You don't have any more bullets.";
$en_US["gunner_shoot_manslaughter"]="<bold>%answer%</bold> is a not a wolf but was accidentally fatally injured.";
$en_US["gunner_reveal"]="Appears (s)he was a <bold>%answer%</bold>";
$en_US["gunner_miss"]="<bold>%nick%</bold> is a lousy shooter. S/He missed!";
$en_US["gunner_suicide"]="<bold>%nick%</bold> should clean his/her weapons more often. The gun exploded and killed him/her!";
$en_US["gunner_night"]="Shooting is only allowed during the day. Please wait patiently for morning.";

$en_US["angel_assign"]="You are a <bold>guardian angel</bold>. It is your job to protect the villagers. If you guard a wolf, there is a 50/50 chance of you dying, if you guard a victim, they will live. Use !guard to guard a player.";
$en_US["angel_guard"]="You are protecting <bold>%answer%</bold> tonight. Farewell!";
$en_US["angel_guard_notify"]="You can sleep well tonight, for a guardian angel is protecting you.";
$en_US["angel_guard_not"]="Only a guardian angel may use this command.";
$en_US["angel_guard_self"]="You may not guard yourself.";
$en_US["angel_guarded"]="You are already protecting <bold>%answer%.</bold>";
$en_US["angel_day"]="You may only protect people at night.";

$en_US["detective_assign"]="You are a <bold>detective</bold>.";
$en_US["detective_assign2"]="It is your job to determine all the wolves and traitors. Your job is during the day, and you can see the true identity of all users, even traitors.";
$en_US["detective_assign3"]="But, each time you use your ability, you risk a 2/5 chance of having your identity revealed to the wolves. So be careful. Use \"!id\" to identify any player during the day.";
$en_US["detective_id"]="The results of your investigation have returned. <bold>%answer%</bold> is a... <bold>%answer2%</bold>!";
$en_US["detective_id_not"]="Only a detective may use this command.";
$en_US["detective_ided"]="You may only investigate one person per round.";
$en_US["detective_night"]="You may only investigate people during the day.";
$en_US["detective_revealed"]="<bold>%nick%</bold> accidentally drops a paper. The paper reveals that (s)he is the detective!";

$en_US["players_list"]="Players: %answer%";
?>