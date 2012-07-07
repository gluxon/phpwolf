<?php
class wolf extends irc {

	// Start random module
	private $random;

	// List of players
	private $players, $players_original;

	// List of people in the channel (for !ping)
	private $people;

	// $ongoing == true when game is running
	private $ongoing;

	// Time first person joined
	private $game_wait;
	
	private $wait_extended;

	public function __construct() {
		$this->random = new random();
		$this->players = array();
		$this->people = array();

		$this->ongoing = false;
		$wait_extended = 0;
	}

	/**
	* Check if the string is a command
	*
	* @param mixed $string string to check
	* @param mixed $command the command the string should be
	* @return boolean yes or no answer
	*/
	public function isCommand ($string, $command) {
		//if ( strtolower($string) == '.' . $command || strtolower($string) == '!' . $command ) {
		if ( strtolower($string) == '!' . $command ) {
			return true;
		}
		return false;
	}

	/**
	 * Decode a locale string
	 * 
	 * @param mixed $string Locale string
	 * @param mixed $nick Nickname to replace %nick%
	 * @param mixed $answer Answer to replace %answer%
	 */
	public function decode($string, $nick=null, $answer=null, $answer2=null) {
		$return = str_replace("<bold>", chr(2), $string);
		$return = str_replace("</bold>", chr(2), $return);

		if ($nick != null) {
			$return = str_replace("%nick%", $nick, $return);
		}
		if ($answer != null) {
			$return = str_replace("%answer%", $answer, $return);
		}
		if ($answer2 != null) {
			$return = str_replace("%answer2%", $answer2, $return);
		}

		return $return;
	}

	/**
	* Check if the user has permission to run a command
	*
	* @param mixed $nick Nickname of player
	* @param mixed $host Hostname of player
	* @param mixed $command Command
	* @return bollean True or false
	*/
	public function hasPermission($nick, $host, $command) {
		global $user, $admins;
	
		switch($command) {

			case 'help':
				return true;
				break;
	
			case 'simple':
				return true;
				break;
	
			case 'back':
				return true;
				break;
	
			case 'quit':
				return true;
				break;
	
			case 'stats':
				return true;
				break;
	
			case 'away':
				return true;
				break;
	
			case 'ping':
				return true;
				break;
	
			case 'start':
				return true;
				break;
	
			case 'retract':
				return true;
				break;
	
			case 'goat':
				return true;
				break;
	
			case 'shoot':
				return true;
				break;
	
			case 'votes':
				return true;
				break;
	
			case 'lynch':
				return true;
				break;
	
			case 'coin':
				return true;
				break;
	
			case 'wait':
				return true;
				break;
	
			case 'join':
				return true;
				break;
	
			case 'rules':
				return true;
				break;
	
			case 'admins':
				return true;
				break;

			case 'msg':
				if (in_array($nick . "!" . $host, $admins)) {
					return true;
				}
			break;

			case 'fstart':
				if (in_array($nick . "!" . $host, $admins)) {
					return true;
				}
			break;

			case 'fstop':
				if (in_array($nick . "!" . $host, $admins)) {
					return true;
				}
			break;

		}
	
		return false;
	}

	/**
	 * Nickname auto-complete
	 */
	public function nickAutocomplete($nick) {
		return $nick;
	}

	/**
	 * Run the corresponding command
	 *
	 * @param mixed $recipient A #room or user
	 * @param mixed $message Message to send
	 * @param mixed $nick Nickname of sender
	 * @param mixed $host Host of sender
	 */
	public function commands($recipient, $message, $nick, $host) {
		global $user, $lang, $setting;

		if ( strpbrk(" ", $message) ) {
			$ex_message = explode(" ", $message, 2);
			$command=$ex_message["0"];
			// This doesn't avoid double space
			if ( !empty($ex_message["1"]) && $ex_message["1"] != " ") {
				$message=$ex_message["1"];
			} else {
				$message="";
			}
		}
		else {
			$command=$message;
			$message="";
		}

		// !help
		if ( $this->isCommand($command, 'help') && $this->hasPermission($nick, $host, 'help') ) {
			$help = str_replace("<bold>", chr(2), $lang["help"]);
			$help = str_replace("</bold>", chr(2), $help);

			// PM this message
			$this->say($nick, $help);
		}

		// !simple
		else if ( $this->isCommand($command, 'simple') && $this->hasPermission($nick, $host, 'simple') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !back
		else if ( $this->isCommand($command, 'back') && $this->hasPermission($nick, $host, 'back') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !quit and !leave
		else if ( ( $this->isCommand($command, 'quit') || $this->isCommand($command, 'leave') ) && $this->hasPermission($nick, $host, 'quit') ) {
			$this->removePlayer($recipient, $nick);

			// Send quit message
			if (isset($this->$nick)) foreach ($this->$nick as $value) {
				if ($value != 'gunner' || $value != 'cursed') {
					$answer=$value;
				}
			}
			if (!isset($answer) && $this->ongoing) {
				$answer = "villager";
			} else {
				$answer = "person";
			}

			$quit = $this->decode($lang["quit"], $nick, $answer);
			$this->say($recipient, $quit);
		}

		// !stats
		else if ( $this->isCommand($command, 'stats') && $this->hasPermission($nick, $host, 'stats') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else {

				// Get amount of players
				$answer = count($this->players);
				if ($answer == 0) {
					// There are no users playing
					$game_none = $this->decode($lang["game_none"]);
					$this->say($nick, $game_none);
				} else {
					// Return list of users and amount
					$answer2 = "";
					foreach (array_keys($this->players) as $key) {
						$answer2 = $answer2 . $key . ', ';
					}
					$answer2 = substr($answer2, 0, -2);

					$stats = $this->decode($lang["stats"], $nick, $answer, $answer2);
					$this->say($recipient, $stats);
				}

			}
		}

		// !away
		else if ( $this->isCommand($command, 'away') && $this->hasPermission($nick, $host, 'away') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !ping
		else if ( $this->isCommand($command, 'ping') && $this->hasPermission($nick, $host, 'ping') ) {
			// add pm and ratelimit protection
			$list = "";
			foreach($this->people as $person) {
				$list = $list . $person . ' ';
			}
			$list = substr($list, 0, -1);
			$ping = $this->decode($lang["ping"], null, $list);
			$this->say($recipient, $ping);
		}

		// !start
		else if ( $this->isCommand($command, 'start') && $this->hasPermission($nick, $host, 'start') ) {
			$count = count($this->players);
			$time = time() - $this->game_wait;

			// There are no players
			if ($count == 0) {
				$game_none = $this->decode($lang["game_none"]);
				$this->say($nick, $game_none);
			} 

			// Time limit has not passed
			else if ( $time < $setting["game_wait"] ) {
				$game_wait = $this->decode($lang["game_wait"], null, $setting["game_wait"] - $time);
				$this->say($recipient, $game_wait);
			}

			// There are not a sufficient amount of players
			else if ($count < 4) {
				$game_required = $this->decode($lang["game_required"], $nick);
				$this->say($recipient, $game_required);
			}

			// Game has already started
			else if ($this->ongoing) {
				$start_ongoing = $this->decode($lang["start_ongoing"], $nick);
				$this->say($nick, $start_ongoing);
			}

			// Sucess!
			else {
				// Loop through players to generate list
				$answer = "";
				foreach($this->players as $key => &$value) {
					$answer = $answer . $key . ", ";
					$value = time();
				}
				$answer = substr($answer, 0, -2);
				$game_welcome = $this->decode($lang["game_welcome"], null, $answer);
				$this->say($recipient, $game_welcome);

				$this->ongoing = true;

				// Assign roles
				$this->assignRoles();
			}

		}

		// !retract
		else if ( $this->isCommand($command, 'retract') && $this->hasPermission($nick, $host, 'retract') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !goat
		else if ( $this->isCommand($command, 'goat') && $this->hasPermission($nick, $host, 'goat') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !shoot
		else if ( $this->isCommand($command, 'shoot') && $this->hasPermission($nick, $host, 'shoot') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !votes
		else if ( $this->isCommand($command, 'votes') && $this->hasPermission($nick, $host, 'votes') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !lynch
		else if ( $this->isCommand($command, 'lynch') && $this->hasPermission($nick, $host, 'lynch') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !coin
		else if ( $this->isCommand($command, 'coin') && $this->hasPermission($nick, $host, 'coin') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else {
				$coin_toss = $this->decode($lang["coin_toss"], $nick);
				$this->say($recipient, $coin_toss);

				$int = $this->random->getRandInt(1, 2);
				if ($int == 1) {
					$coin_land = $this->decode($lang["coin_land"], null, "heads");
					$this->say($recipient, $coin_land);
				} else if ($int == 2) {
					$coin_land = $this->decode($lang["coin_land"], null, "tails");
					$this->say($recipient, $coin_land);
				} else {
					$this->sary($recipient, "There as an error retrieving random.org results.");
				}
			}
		}

		// !wait
		else if ( $this->isCommand($command, 'wait') && $this->hasPermission($nick, $host, 'wait') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			}
			else if ($this->ongoing == false) {
				if ($this->wait_extended < $setting["game_add_wait_limit"]) {
					$this->wait_extended += 1;
					$this->game_wait += $setting["game_add_wait"];
					$wait = $this->decode($lang["wait"], $nick, $setting["game_add_wait"]);
					$this->say($recipient, $wait);
				} else {
					$wait_limit = $this->decode($lang["wait_limit"]);
					$this->say($recipient, $wait_limit);
				}
			} else {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->say($nick, $start_ongoing);
			}
		}

		// !join
		else if ( $this->isCommand($command, 'join') && $this->hasPermission($nick, $host, 'join') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ( isset($this->players[$nick]) ) {
				$join_already_playing = $this->decode($lang["join_already_playing"]);
				$this->say($nick, $join_already_playing);
			} else {
				$this->addPlayer($recipient, $nick);

				// Send join message
				$join = $this->decode($lang["join"], $nick);
				$this->say($recipient, $join);

				// 1 player for player already added
				if ( count($this->players) == 1 ) {
					$game_start = $this->decode($lang["game_start"], $nick);
					$this->say($recipient, $game_start);
					$this->game_wait = time();
				}
			}
			
		}

		// !rules
		else if ( $this->isCommand($command, 'rules') && $this->hasPermission($nick, $host, 'rules') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !admins
		else if ( $this->isCommand($command, 'admins') && $this->hasPermission($nick, $host, 'admins') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		/***** Admin commands *****/
		// !msg
		else if ( $this->isCommand($command, 'msg') && $this->hasPermission($nick, $host, 'msg') ) {
			$message=explode(" ", $message, 2);
			$this->say($message["0"], $message["1"]);
		}

		// !fstart
		else if ( $this->isCommand($command, 'fstart') && $this->hasPermission($nick, $host, 'fstart') ) {
			$this->game_wait = 0;
		}

		// !fstop
		else if ( $this->isCommand($command, 'fstop') && $this->hasPermission($nick, $host, 'fstop') ) {
			$this->endGame($recipient);
		}

	}

	/**
	 * Add a player to the game
	 * 
	 * @param mixed $recipient Channel
	 * @param mixed $nick Nickname of player
	 */
	public function addPlayer($recipient, $nick) {
		$this->players[$nick] = time();
		// Add Voice
		$this->sendData("MODE $recipient +v $nick");
	}

	/**
	 * Remove a player from the game
	 * 
	 * @param mixed $recipient Channel
	 * @param mixed $nick Nickname of player
	 */
	public function removePlayer($recipient, $nick) {
		unset($this->players[$nick]);
		// Remove Voice
		$this->sendData("MODE $recipient -v $nick");
	}

	/**
	 * Assign player roles
	 */
	public function assignRoles() {
		global $roles;
		$player_amount = count($this->players);
		foreach ($roles as $key => $value) {
			if ( $player_amount >= $key ) {
				$index = $key;
			} else {
				break;
			}
		}
		$game_roles = $roles[$index];

		// Reconstruct players array
		$i=0;
		foreach(array_keys($this->players) as $key) {
			$players_sorted[$i] = $key;
			$i++;
		}
		// Backup players for original
		$this->players_original = $players_sorted;

		foreach($game_roles as $role => $amount) {
			switch($role) {
				case 0: // Seer
					$role_name = 'seer';
				break;

				case 1: // Wolf
					$role_name = 'wolf';
				break;

				case 2: // Cursed
					$role_name = 'cursed';
				break;

				case 3: // Drunk
					$role_name = 'drunk';
				break;

				case 4: // Harlot
					$role_name = 'harlot';
				break;

				case 5: // Traitor
					$role_name = 'traitor';
				break;

				case 6: // Gunner
					$role_name = 'gunner';
				break;

				case 7: // Crow
					$role_name = 'crow';
				break;

				case 8: // Angel
					$role_name = 'angel';
				break;

				case 9: // Detective
					$role_name = 'detective';
				break;
			}
			echo "\n\n$role_name : ";
			for($i=1; $i <= $amount; $i++) {
				echo "$role $amount ";

				// Loop until role is given
				// NOTE: This will result in an infinite loop if settings are manipulated wrong

				while(true) {
					echo "Finding $role_name";
					$player_index = $this->random->getRandInt(0, $player_amount-1);
					
					if ( !isset($this->{$players_sorted[$player_index]}) ) {
						// Assign a role to a person without one
						if ( !isset($this->{$players_sorted[$player_index]}) ) {
							$this->{$players_sorted[$player_index]} = array($role_name);
							$this->say($players_sorted[$player_index], "You are a $role_name");
							echo ": $players_sorted[$player_index]\n\n";
							break;
						}
					} else {
						echo ": $players_sorted[$player_index]\n\n";
						// If the person is a seer, only give them gunner
						if ( in_array('seer', $this->{$players_sorted[$player_index]}) && $role_name != 'gunner' ) {
							continue;
						}
						// If the person is a drunk, only give them gunner
						if ( in_array('drunk', $this->{$players_sorted[$player_index]}) && $role_name != 'gunner' ) {
							continue;
						}
						// If the person is cursed, only give them traitor
						if ( in_array('cursed', $this->{$players_sorted[$player_index]}) && $role_name != 'traitor' ) {
							continue;
						}
						// If the person is already harlot, angel, or detective, do not give them another role
						if ( in_array('harlot', $this->{$players_sorted[$player_index]}) || in_array('angel', $this->{$players_sorted[$player_index]}) || in_array('detective', $this->{$players_sorted[$player_index]}) ) {
							continue;
						}
						// If the person is a wolf, they can't have anything else
						if ( in_array('wolf', $this->{$players_sorted[$player_index]}) ) {
							continue;
						}

						// Conditions satisfied, give another role!
						array_push($this->{$players_sorted[$player_index]}, $role_name);
						$this->say($players_sorted[$player_index], "You are a $role_name");
						break;
					}

				}
			}

		}
	}

	/**
	 * End the game
	 * 
	 * @param mixed $channel Channel game is operating in
	 * @param mixed $win 0 no win, 1 for villagers, 2 for wolves, 3 for all wolves dead
	 */
	public function endGame($channel, $win=0) {
		global $lang;

		// No game is running now
		$this->ongoing = false;

		// Generate list of players
		$players_list = "";
		foreach (array_keys($this->players) as $person) {
			$players_list = $players_list . $person . " ";
		}
		$players_list = substr($players_list, 0, -1);

		$players_list_split = SplitByEveryNthOccurrence($players_list, ' ', 3);
		foreach($players_list_split as $list) {
			$devoice = '-';
			for ($i=0; $i < count($this->players); $i++) {
				$devoice = $devoice . 'v';
			}
			$this->sendData("MODE $channel $devoice $list");
		}

		// Clear players
		unset($this->players);
		$this->players = array();

		foreach($this->players_original as $people) {
			if (isset($this->$people)) foreach($this->$people as $role) {
				if (!empty($role)) {
					$$role = $people;
				}
			}
		}

		$roles_message = "";
		if (isset($wolf)) {
			$message_wolf = $this->decode($lang["end_roles"], null, $lang["wolf"], $wolf);
			$roles_message = $roles_message . $message_wolf . " ";
		}
		if (isset($seer)) {
			$message_seer = $this->decode($lang["end_roles"], null, $lang["seer"], $seer);
			$roles_message = $roles_message . $message_seer . " ";
		}
		if (isset($harlot)) {
			$message_harlot = $this->decode($lang["end_roles"], null, $lang["harlot"], $harlot);
			$roles_message = $roles_message . $message_harlot . " ";
		}
		if (isset($traitor)) {
			$message_traitor = $this->decode($lang["end_roles"], null, $lang["traitor"], $traitor);
			$roles_message = $roles_message . $message_traitor . " ";
		}
		if (isset($cursed)) {
			$message_cursed = $this->decode($lang["end_roles"], null, $lang["cursed"], $cursed);
			$roles_message = $roles_message . $message_cursed . " ";
		}
		if (isset($gunner)) {
			$message_gunner = $this->decode($lang["end_roles"], null, $lang["gunner"], $gunner);
			$roles_message = $roles_message . $message_gunner . " ";
		}
		if (isset($angel)) {
			$message_angel = $this->decode($lang["end_roles"], null, $lang["angel"], $angel);
			$roles_message = $roles_message . $message_angel . " ";
		}
		if (isset($detective)) {
			$message_detective = $this->decode($lang["end_roles"], null, $lang["detective"], $detective);
			$roles_message = $roles_message . $message_detective . " ";
		}
		if (isset($drunk)) {
			$message_drunk = $this->decode($lang["end_roles"], null, $lang["drunk"], $drunk);
			$roles_message = $roles_message . $message_drunk . " ";
		}

		$roles_message = substr($roles_message, 0, -1);
		$this->say($channel, $roles_message);
		unset($this->players_original);

	}

	/**
	 * Run functions not invoked by messages
	 */
	public function runMaintenance() {
		
	}

	/**
	 * Runs the bot functions
	 *
	 * @param array $data explodeData() return
	 */
	public function run($socket, $data) {
		global $channel, $user;

		// Set the socket from the running instance
		$this->setSocket($socket);

		// Parse the data

		// Remove unneeded newline colon at beginning
		$smplf["0"]=substr($data["0"], 1);
 
		if ( strpos($smplf["0"], "!") ) {
			// Message from user
			$username=explode("!", $smplf["0"]);
			$nick=$username["0"];
			$host=$username["1"];
		} else {
			// Message from server
			$server = $smplf["0"];
		}

		if (!empty($data["1"])) {

			$event = $data["1"];
			switch($event) {
				case 'PRIVMSG':
					$recipient = $data["2"];
					$implode=$data;
					unset($implode["0"], $implode["1"], $implode["2"]);
					$implode["3"] = substr($implode["3"], 1);
	
					$message=implode(" ", $implode);
	
					$this->commands($recipient, $message, $nick, $host);
				break;
				case 'QUIT':
					// Remove from peoples list
					$key = array_search($nick, $this->people);
					unset($this->people[$key]);
				break;
				case 'PART':
					// Remove from peoples list
					$key = array_search($nick, $this->people);
					unset($this->people[$key]);
				break;
				case 'JOIN':
					// Add to peoples list
					if ($nick != $user["nick"]) {
						array_push($this->people, $nick);
					}
				break;
				case 'NICK':
					// Change nick in people list
					$implode = implode(' ', $data);
					$new_nick = explode(':', $implode, 3);
					$new_nick = $new_nick["2"];

					$key = array_search($nick, $this->people);
					$this->people[$key] = $new_nick;
				break;
				case '353': // RPL_NAMREPLY (list of people in channel)
					$implode = implode(' ', $data);
					$explode = explode(':', $implode, 3);
					$people=explode(' ', $explode["2"]);

					foreach($people as $value) {
						if ( strpbrk('@', $value) || strpbrk('+', $value) ) {
							$value = substr($value, 1);
						}
						if ($value != $user["nick"]) {
							array_push($this->people, $value);
						}
					}
				break;
			}

		}
	}

}
?>