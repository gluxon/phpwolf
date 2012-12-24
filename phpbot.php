<?php
// Stop PHP from killing itself
set_time_limit(0);

// Log error messages
ini_set('display_errors', 'On');
ini_set('log_errors', 'true');
ini_set('error_log', 'error_log');

// Where is everything?
define('INCLUDES_PATH', 'includes');
define('SETTINGS_PATH', 'settings');
define('MODULES_PATH', 'modules');
define('LOCALES_PATH', 'locales');

require("irc.inc"); // IRC Class

require(INCLUDES_PATH . "/functions.inc"); // Custom functions
require(INCLUDES_PATH . "/TokenBucket.inc"); // Token Bucket Algorithm
require(INCLUDES_PATH . "/random.inc"); // Random Functions
require(INCLUDES_PATH . "/locale.inc"); // Language class

require(MODULES_PATH . "/wolf.inc"); // Werewolf Game

require(SETTINGS_PATH . "/bot.inc"); // Settings

// Start IRC class
$irc = new irc();
$irc->connect($server["address"], $server["port"]);

// Send the password and user ident info
$irc->sendPassword($user["password"]);
$irc->sendIdent($user["nick"], $user["mode"], $user["unused"], $user["realname"]);

// Join the default channel
$irc->joinChannel($channel["name"]);

// Start wolf module
$wolf = new wolf();

// Continously read new lines
while (true) {

	// Read new line
	$data=$irc->getData(512);

	if (!empty($data)) {
		echo $data; 

		// Trim off line carriage and newline
		$data = str_replace("\r\n", '', $data);

		// Split the data into words
		$data = $irc->explodeData($data);

		// Play some Ping Pong!
		$irc->PingPong($data);

		// Run wolfbot module
		$wolf->run($irc->getSocket(), $data);

	} else {
		$wolf->runMaintenance();
	}

}
?>