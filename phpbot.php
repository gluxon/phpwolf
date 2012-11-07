<?php
// Stop PHP from killing itself
set_time_limit(0);

// Custom Functions
require("functions.inc");

// Import locales
require("locale.inc");

// Import settings
require("settings.inc");

// Import IRC class
require("irc.inc");

// Import random module
require("random.inc");

// Import wolf module
require("wolf.inc");

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