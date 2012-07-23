<?php
class wolf extends irc {

	// Start random module
	private $random;

	// List of players
	private $players, $players_original;
	private $lynch, $away;

	// Game stores
	private $people, $channel, $ongoing;

	// Counters
	private $game_wait, $wait_extended, $last_ping;

	// Role variables
	public $role_names;
	private $roles_set;
	private $seer, $wolf, $cursed, $drunk, $harlot, $traitor, $gunner, $crow, $angel, $detective;

	// Seen, visited, killed variables (per night)
	private $seer_seen, $wolf_killed, $harlot_visited, $gunner_shot, $angel_guarded, $detective_ided;

	// Pre-generated end roles message
	private $roles_message;

	// Day and night
	private $isDay, $isNight;
	private $day_time, $night_time;
	private $current_day_start, $current_night_start;
	private $fday, $fnight;

	public function __construct() {
		$this->random = new random();
		$this->players = array();
		$this->people = array();
		$this->lynch = array();
		$this->away = array();

		$this->ongoing = false;
		$this->wait_extended = 0;
		$this->last_ping = 0;

		$this->isDay = null;
		$this->isNight = null;
		$this->day_time = 0;
		$this->night_time = 0;

		$this->fday = false;
		$this->fnight = false;

		$this->roles_message = '';

		$this->seer = array();
		$this->wolf = array();
		$this->cursed = array();
		$this->drunk = array();
		$this->harlot = array();
		$this->traitor = array();
		$this->gunner = array();
		$this->crow = array(); 
		$this->angel = array();
		$this->detective = array();
		$this->role_names = array("wolf", "seer", "cursed", "drunk", "harlot", "traitor", "gunner", "crow", "angel", "detective");

		$this->seer_seen = array();
		$this->wolf_killed = array();
		$this->harlot_visited = array();
		$this->gunner_shot = array();
		$this->angel_guarded = array();
		$this->detective_ided = array();
	}

	/**
	* Check if the string is a command
	*
	* @param mixed $string string to check
	* @param mixed $command the command the string should be
	* @return boolean yes or no answer
	*/
	public function isCommand ($string, $command, $no_operator=false) {
		//if ( strtolower($string) == '.' . $command || strtolower($string) == '!' . $command ) {
		if ( strtolower($string) == '!' . $command || ($no_operator && strtolower($string) == $command) ) {
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
	public function decode($string, $nick=null, $answer=null, $answer2=null, $answer3=null) {
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
		if ($answer3 != null) {
			$return = str_replace("%answer3%", $answer3, $return);
		}

		return $return;
	}

	/**
	 * Return a string with formated seconds
	 */
	public function formatSeconds($seconds) {
		$minutes = 0;
		while($seconds >= 60) {
			$seconds = $seconds - 60;
			$minutes++;
		}

		if (strlen($minutes) == 1) {
			$minutes = '0' . $minutes;
		}
		if (strlen($seconds) == 1) {
			$seconds = '0' . $seconds;
		}
		return $minutes . ':' . $seconds;
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
				
			case 'whoami':
				return true;
			break;

			case 'msg':
				if (in_array($host, $admins)) {
					return true;
				}
			break;

			case 'fstart':
				if (in_array($host, $admins)) {
					return true;
				}
			break;

			case 'fstop':
				if (in_array($host, $admins)) {
					return true;
				}
			break;

			case 'fday':
				if (in_array($host, $admins)) {
					return true;
				}
				break;

			case 'fnight':
				if (in_array($host, $admins)) {
					return true;
				}
			break;

			case 'fjoin':
				if (in_array($host, $admins)) {
					return true;
				}
			break;

			case 'fquit':
				if (in_array($host, $admins)) {
					return true;
				}
			break;
			
			case 'frestart':
				if (in_array($host, $admins)) {
					return true;
				}
			break;

			case 'nick':
				return true;
			break;

		}

		return false;
	}

	/**
	 * Nickname auto-complete
	 */
	public function nickAutocomplete($nick) {
		foreach(array_keys($this->players) as $player) {
			if ( stripos($player, $nick) === 0 ) {
				if (isset($return)) {
					// There are more than 1 person with that name, return original nick
					$return = $nick;
				} else {
					$return = $player;
				}
			}
		}
		if (!isset($return)) {
			$return = $nick;
		}
		return $return;
	}

	/**
	 * Check if the player is playing
	 */
	public function isPlaying($nick) {
		foreach(array_keys($this->players) as $player) {
			if ($player == $nick) {
				return true;
			}
		}
		return false;
	}

	public function getRoles($nick) {
		foreach($this->role_names as $role) {
			foreach($this->$role as $person) {
				if ($nick == $person) {
					if (!isset($return)) {
						$return = array($role);
					} else {
						array_push($return, $role);
					}
				}
			}
		}
		if (isset($return)) {
			return $return;
		} else {
			return false;
		}
	}

	public function getRoleByID($id) {
		switch($id) {
			case 0: // Seer
				return 'seer';
			break;
		
			case 1: // Wolf
				return 'wolf';
			break;
		
			case 2: // Cursed
				return 'cursed';
			break;

			case 3: // Drunk
				return 'drunk';
			break;
		
			case 4: // Harlot
				return 'harlot';
			break;
		
			case 5: // Traitor
				return 'traitor';
			break;
		
			case 6: // Gunner
				return 'gunner';
			break;
		
			case 7: // Crow
				return 'crow';
			break;
		
			case 8: // Angel
				return 'angel';
			break;
		
			case 9: // Detective
				return 'detective';
			break;

			default:
				return false;
		}
	}

	public function getRoleID($role) {
		switch($role) {
			case 'seer': // Seer
				return 0;
				break;

			case 'wolf': // Wolf
				return 1;
				break;
	
			case 'cursed': // Cursed
				return 2;
				break;
	
			case 'drunk': // Drunk
				return 3;
				break;
	
			case 'harlot': // Harlot
				return 4;
				break;
	
			case 'traitor': // Traitor
				return 5;
				break;
	
			case 'gunner': // Gunner
				return 6;
				break;
	
			case 'crow': // Crow
				return 7;
				break;
	
			case 'angel': // Angel
				return 8;
				break;
	
			case 'detective': // Detective
				return 9;
				break;
	
			default:
				return false;
		}
	}

	/**
	 * Return a list of all the players
	 * 
	 * @param mixed $nick Nick of player to not add
	 * @param boolean $wolf Add wolf role to other wolves
	 * @param boolean $reset Reset Times
	 * @param mixed List of players
	 */
	public function listPlayers($wolf = false, $reset = false, $nick = null) {
		global $lang;

		$list = "";
		if ($wolf) {
			foreach($this->players as $key => &$value) {
				if ($nick == null || $key != $nick) {
					$list = $list . $key;
					if ( in_array($key, $this->wolf) ) {
						$list = $list . ' (' . $lang["wolf"] . ')';
					} else if ( in_array($key, $this->traitor) ) {
						$list = $list . ' (' . $lang["traitor"] . ')';
					}
					if ($reset) {
						$value = time();
					}
					$list = $list . ', ';
				}
			}
		} else {
			foreach($this->players as $key => &$value) {
				if ($nick == null || $key != $nick) {
					$list = $list . $key . ', ';
					if ($reset) {
						$value = time();
					}
				}
			}
		}
		return substr($list, 0, -2);
	}

	public function relayToWolves($sender, $message) {
		global $lang;
		foreach($this->wolf as $wolf) {
			if ($wolf != $sender) {
				$wolf_relay = $this->decode($lang["wolf_relay"], $sender, $message);
				$this->say($wolf, $wolf_relay);
			}
		}
		foreach($this->traitor as $traitor) {
			if ($traitor != $sender) {
				$wolf_relay = $this->decode($lang["wolf_relay"], $sender, $message);
				$this->say($wolf, $wolf_relay);
			}
		}
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
		global $user, $lang, $setting, $limit, $irc;

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

		// !nick
		else if ( $this->isCommand($command, 'nick') && $this->hasPermission($nick, $host, 'nick') ) {
			$this->say($recipient, $this->nickAutocomplete($message));
		}

		// !simple
		else if ( $this->isCommand($command, 'simple') && $this->hasPermission($nick, $host, 'simple') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !back
		else if ( $this->isCommand($command, 'back') && $this->hasPermission($nick, $host, 'back') ) {
			if (in_array($nick, $this->away)) {
				$index = array_search($nick, $this->away);
				unset($this->away[$index]);

				$back = $this->decode($lang["back"]);
				$this->notice($nick, $back);
			} else {
				$not_away = $this->decode($lang["not_away"]);
				$this->notice($nick, $not_away);
			}
		}

		// !quit and !leave
		else if ( ( $this->isCommand($command, 'quit') || $this->isCommand($command, 'leave') ) && $this->hasPermission($nick, $host, 'quit') ) {
			// Send quit message
			$roles = $this->getRoles($nick);
			if ($roles === false) {
				if ($this->ongoing) {
					$answer = "villager";
				} else {
					$answer = "person";
				}
			} else {
				foreach ($roles as $value) {
					if ($value != 'gunner' || $value != 'cursed') {
						$answer=$value;
					}
				}
			}

			$this->removePlayer($recipient, $nick);

			$quit = $this->decode($lang["quit"], $nick, $answer);
			$this->say($recipient, $quit);
		}

		// !fquit and !fleave
		else if ( ( $this->isCommand($command, 'fquit') || $this->isCommand($command, 'fleave') ) && $this->hasPermission($nick, $host, 'fquit') ) {
			// Send quit message
			if ($this->isPlaying($message)) {
				$roles = $this->getRoles($message);
				if ($roles === false) {
					if ($this->ongoing) {
						$answer = "villager";
					} else {
						$answer = "person";
					}
				} else {
					foreach ($roles as $value) {
						if ($value != 'gunner' || $value != 'cursed') {
							$answer=$value;
						}
					}
				}

				$this->removePlayer($recipient, $message);

				$quit = $this->decode($lang["quit"], $message, $answer);
				$this->say($recipient, $quit);
			} else {
				// $message is not playing
			}
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
					$this->notice($nick, $game_none);
				} else {
					// Return list of users and amount
					$answer2 = $this->listPlayers();

					$stats = $this->decode($lang["stats"], $nick, $answer, $answer2);
					$this->say($recipient, $stats);
				}

				if ($this->ongoing) {
					$reply = '';
					$specials_total = 0;

					// TODO:
					// 1. Plural support for "There is.." = "There are.."

					foreach ($this->role_names as $role_name) {
						$role_id = $this->getRoleID($role_name);
						if ($this->roles_set[$role_id] > 0) {
							$amount = count($this->$role_name);

							$specials_total = $specials_total + $amount; // Add to total

							if ($amount == 0) {
								$reply = $reply . 'no ' . $lang[$role_name . '_plural'] . ', ';
							} else if ($amount == 1) {
								$reply = $reply . chr(2).$amount.chr(2) . ' ' . $lang[$role_name] . ', ';
							} else {
								$reply = $reply . chr(2).$amount.chr(2) . ' ' . $lang[$role_name . '_plural'] . ', ';
							}
						}
					}

					$player_total = count($this->players);
					$villagers_total = $player_total - $specials_total;

					if ($villagers_total == 0) { 
						$reply = substr($reply, 0, -2) . ' no ' . $lang["villager_plural"];
					} else if ($villagers_total == 1) {
						$reply = substr($reply, 0, -2) . ' and ' . chr(2).$villagers_total.chr(2) . ' ' . $lang["villager"] . '.';
					} else {
						$reply = substr($reply, 0, -2) . ' and ' . chr(2).$villagers_total.chr(2) . ' ' . $lang["villager_plural"] . '.';
					}

					$stats_roles = $this->decode($lang["stats_roles"], $nick, $reply);
					$this->say($recipient, $stats_roles);
				}
			
			}


		}

		// !away
		else if ( $this->isCommand($command, 'away') && $this->hasPermission($nick, $host, 'away') ) {
			if (in_array($nick, $this->away)) {
				$index = array_search($nick, $this->away);
				unset($this->away[$index]);

				$back = $this->decode($lang["back"]);
				$this->notice($nick, $back);
			} else {
				array_push($this->away, $nick);

				$away = $this->decode($lang["away"]);
				$this->notice($nick, $away);
			}
		}

		// !ping
		else if ( $this->isCommand($command, 'ping') && $this->hasPermission($nick, $host, 'ping') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			}
			else if ($this->ongoing == false) {
				if (time() - $this->last_ping > $limit["ping"]) {
					$this->last_ping = time();

					$list = implode(' ', $this->people);
					// This returns PING %answer% if $list is empty
					$ping = $this->decode($lang["ping"], null, $list);
					$this->say($recipient, $ping);
				} else {
					$ratelimit = $this->decode($lang["ratelimit"]);
					$this->notice($nick, $ratelimit);
				}
			} else {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			}

		}

		// !fping
		else if ( $this->isCommand($command, 'fping') && $this->hasPermission($nick, $host, 'fping') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			}
			else if ($this->ongoing == false) {
				$this->last_ping = time();
		
				$list = "";
				foreach($this->people as $person) {
					if ( !in_array($person, $this->away) && !isset($this->players[$person]) ) {
						$list = $list . $person . ' ';
					}
				}
				$list = substr($list, 0, -1);
				// This returns PING %answer% if $list is empty
				$ping = $this->decode($lang["ping"], null, $list);
				$this->say($recipient, $ping);
			} else {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			}
		
		}

		// !start
		else if ( $this->isCommand($command, 'start') && $this->hasPermission($nick, $host, 'start') ) {
			$count = count($this->players);
			$time = time() - $this->game_wait;

			// There are no players
			if ($count == 0) {
				$game_none = $this->decode($lang["game_none"]);
				$this->notice($nick, $game_none);
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
				$this->notice($nick, $start_ongoing);
			}

			// Sucess!
			else {
				// Loop through players to generate list
				$answer = $this->listPlayers(false, true);

				$game_welcome = $this->decode($lang["game_welcome"], null, $answer);
				$this->say($recipient, $game_welcome);

				$this->ongoing = true;
				$this->sendData('MODE ' . $recipient . ' +m');

				// Assign roles
				$this->assignRoles();

				// Send night message
				$night_first = $this->decode($lang["night_first"]);
				$this->say($recipient, $night_first);

				// Set booleans for.... THE SUN
				$this->isDay = false;
				$this->isNight = true;

				$this->current_night_start = time();

				// Pre-Generate end message
				foreach($this->role_names as $role) {

					if ( count($this->$role) > 0) {
						$role_list = '';
						$i = 0;
						$numItems = count($this->$role);

						foreach($this->$role as $person) {

							++$i;
							// Last element
							if ($numItems > 1) {
								if ($i == $numItems) {
									// Remove ', ', and replace with ' and ', then append $person
									$role_list = substr($role_list, 0, -2) . ' and ' . $person ;
								} else {
									$role_list = $role_list . $person . ', ';
								}
							}
						}

						if ($numItems > 1) {
							$this->roles_message = $this->roles_message . $this->decode($lang["end_roles_plural"], null, $lang[$role . '_plural'], $role_list) . ' ';
						} else {
							// This is a hackish workaround, $person is still the looped array since $numItems is 1
							$this->roles_message = $this->roles_message . $this->decode($lang["end_roles"], null, $lang[$role], $person) . ' ';
						}
					}
				}
				$this->roles_message = substr($this->roles_message, 0, -1); // remove trailing space

				// We do not want socket blocking for werewolf now
				$irc->setBlocking(false);

			}

		}

		// !retract
		else if ( $this->isCommand($command, 'retract') && $this->hasPermission($nick, $host, 'retract') ) {

			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ($this->ongoing == false) {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			} else if (!$this->isPlaying($nick)) {
				// This person is not playing, let's just ignore them
			} else if (!$this->isPlaying($victim)) {
				$not_playing = $this->decode($lang["not_playing"], null, $victim);
				$this->notice($nick, $not_playing);
			} else if ($this->isNight) {
				$this->say($nick, "Error, it is not day time");
			} else if (isset($this->lynch[$nick])) {
				unset($this->lynch[$nick]);
				$lynch = $this->decode($lang["lynch"], $nick, $victim);
				$this->say($recipient, $lynch);
			}
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
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if (!$this->ongoing) {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			} else {
				$defendant = array();
				foreach($this->lynch as $prosecutor => $defendor) {
					if (isset($defendants[$defendor])) {
						array_push($defendants[$$defendor], $prosecutor);
						$prosecution_amount[$defendor]++;
					} else {
						$defendants[$defendor] = array($prosecutor);
						$prosecution_amount[$defendor] = 1;
					}
				}
				$votes_list = '';
				foreach ($defendants as $defendor => $prosecutors) {
					$votes_list = $votes_list . $defendor . ': ' . $prosecution_amount[$defendor] . ' (' . implode(' ', $prosecutors) . ')' . ', ';
				}
				$votes_list = substr($votes_list, 0, -2);
				$votes = $this->decode($lang["votes"], $nick, $votes_list);
				$this->say($recipient, $votes);
			}
		}

		// !lynch
		else if ( $this->isCommand($command, 'lynch') || $this->isCommand($command, 'vote') && $this->hasPermission($nick, $host, 'lynch') ) {

			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ($this->ongoing) {
				$message = explode(' ', $message, 2);
				$victim = $this->nickAutocomplete($message["0"]);

				if (!$this->isPlaying($nick)) {
					// This person is not playing, let's just ignore them (and hope they go away)
				} else if (!$this->isPlaying($victim)) {
					$not_playing = $this->decode($lang["not_playing"], null, $victim);
					$this->notice($nick, $not_playing);
				} else if ($this->isNight) {
					$this->say($nick, "Error, it is not day time");
				} else {
					$this->lynch[$nick] = $victim;
					$lynch = $this->decode($lang["lynch"], $nick, $victim);
					$this->say($recipient, $lynch);
				}
			} else {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			}

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
					$this->say($recipient, "There as an error retrieving random.org results.");
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
				$this->notice($nick, $start_ongoing);
			}
		}

		// !join
		else if ( $this->isCommand($command, 'join') && $this->hasPermission($nick, $host, 'join') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ( isset($this->players[$nick]) ) {
				$join_already_playing = $this->decode($lang["join_already_playing"]);
				$this->notice($nick, $join_already_playing);
			} else {
				$this->addPlayer($recipient, $nick);

				// Send join message
				$join = $this->decode($lang["join"], $nick);
				$this->say($recipient, $join);

				// 1 player for player already added
				if ( count($this->players) == 1 ) {
					$this->channel = $recipient;
					$game_start = $this->decode($lang["game_start"], $nick);
					$this->say($recipient, $game_start);
					$this->game_wait = time();
				}
			}

		}

		

		// !rules
		else if ( $this->isCommand($command, 'rules') && $this->hasPermission($nick, $host, 'rules') ) {
			$rules_part1 = $this->decode($lang["rules_part1"]);
			$this->say($recipient, $rules_part1);
			$rules_part2 = $this->decode($lang["rules_part2"]);
			$this->say($recipient, $rules_part2);
		}

		// !admins
		else if ( $this->isCommand($command, 'admins') && $this->hasPermission($nick, $host, 'admins') ) {
			$this->say($recipient, "Snuupy is the man for that.");
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
			if ($this->ongoing) {
				$this->endGame($this->channel);
			}
		}

		// !fday
		else if ( $this->isCommand($command, 'fday') && $this->hasPermission($nick, $host, 'fday') ) {
			if ($this->isNight && $this->ongoing) {
				$this->fday = true;
			}
		}

		// !fnight
		else if ( $this->isCommand($command, 'fnight') && $this->hasPermission($nick, $host, 'fnight') ) {
			if ($this->isDay && $this->ongoing) {
				$this->fnight = true;
			}
		}

		// !fjoin
		else if ( $this->isCommand($command, 'fjoin') && $this->hasPermission($nick, $host, 'fjoin') ) {
			if ($recipient == $nick) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ( isset($this->players[$message]) ) {
				$join_already_playing = $this->decode($lang["join_already_playing"]);
				$this->notice($nick, $join_already_playing);
			} else {
				$this->addPlayer($recipient, $message);

				// Send join message
				$join = $this->decode($lang["join"], $message);
				$this->say($recipient, $join);
			
				// 1 player for player already added
				if ( count($this->players) == 1 ) {
					$this->channel = $recipient;
					$game_start = $this->decode($lang["game_start"], $message);
					$this->say($recipient, $game_start);
					$this->game_wait = time();
				}
			}
		}

		// !frestart
		else if ( $this->isCommand($command, 'frestart') && $this->hasPermission($nick, $host, 'frestart') ) {
			// Hack from -> http://debuggable.com/posts/restarting-a-php-command-line-script:4988968d-b9d0-4fe8-af30-05544834cda3
			//echo "\nphpwolf: RESTARTING";
			//die(exec('php ' . str_replace(' ', '\ ', getcwd()) . '/' . implode(' ', $GLOBALS['argv']) . ' > /dev/null &'));
		}

		/***** Private Role Commands *****/
		else if ( $this->isCommand($command, 'see', true) ) {
			if ($recipient == $this->channel || $this->isDay) {
				// ignore this, people are probably telling the seer to 'see' someone.
			} else if ( !in_array($nick, $this->seer) ) {
				$seer_see_not = $this->decode($lang["seer_see_not"]);
				$this->say($nick, $seer_see_not);
			} else if ( in_array($nick, $this->seer_seen) ) {
				$seer_seen = $this->decode($lang["seer_seen"]);
				$this->say($nick, $seer_seen);
			} else {
				$person = $this->nickAutocomplete($message);

				if ( $this->isPlaying($person) ) {

					// parse this person
					$person_roles = $this->getRoles($person);
					if ($person_roles == false) {
						$role = 'villager';
					} else {
						foreach($person_roles as $value) {
							if ($value == 'cursed') {
								$role = 'wolf';
							} else if ($value == 'traitor') {
								$role = 'villager';
							} else {
								$role = $value;
							}
						}
					}

					// Tell the seer who this guy is
					$seer_see = $this->decode($lang["seer_see"], null, $person, $lang[$role]);
					$this->say($nick, $seer_see);

					// Add to list of seers that have seen tonight
					array_push($this->seer_seen, $nick);
				} else {
					// This person wasn't playing
					$not_playing=$this->decode($lang["not_playing"], null, $person);
					$this->notice($nick, $not_playing);
				}
			}
		}

		else if ( $this->isCommand($command, 'kill', true) ) {
			if ($recipient == $this->channel || $this->isDay ) {
				// ignore this
			} else if ( !in_array($nick, $this->wolf) ) {
				$this->say($nick, "You are not a wolf. good thing too!");
			} else {
				$person = $this->nickAutocomplete($message);

				if ( $this->isPlaying($person) ) {

					// Check if the victim is a wolf
					$person_roles = $this->getRoles($person);
					if ($person_roles != false && in_array('wolf', $person_roles)) {
						$this->say($nick, "You can't kill a wolf!");
					} else {
						// This person has been selected for death.
						$this->wolf_killed[$nick] = $person;

						// Send Message
						$wolf_kill = $this->decode($lang["wolf_kill"], null, $person);
						$this->say($nick, $wolf_kill);
					}
				} else {
					// This person wasn't playing
					$not_playing=$this->decode($lang["not_playing"], null, $person);
					$this->notice($nick, $not_playing);
				}
			}
		}

		else if ( $this->isCommand($command, 'visit', true) ) {
			if ($recipient == $this->channel || $this->isDay ) {
				// ignore this
			} else if ( !in_array($nick, $this->harlot) ) {
				$this->say($nick, "You are not a harlot");
			} else if ( isset($this->harlot_visited[$nick])) {
				$this->say($nick, "You are already visiting blah");
			} else {
				$person = $this->nickAutocomplete($message);

				if ( $this->isPlaying($person) ) {

					// This person has been selected for death.
					$this->harlot_visited[$nick] = $person;

					// Send Message
					$harlot_visit = $this->decode($lang["harlot_visit"], null, $person);
					$this->say($nick, $harlot_visit);

					// Send Message to hostess
					$harlot_visit = $this->decode($lang["harlot_visit"], null, $nick);
					$this->say($person, $harlot_visit);

				} else {
					// This person wasn't playing
					$not_playing=$this->decode($lang["not_playing"], null, $person);
					$this->notice($nick, $not_playing);
				}
			}
		}

		// whoami (needs official locale)
		else if ( $this->isCommand($command, 'whoami', true) ) {
			if (!$this->ongoing) {
				//No game is currently running.
				$this->say($nick, $this->decode($lang["game_none"]));
			} else if ( !$this->isPlaying($nick) ) {
				//The person is not playing.
				$this->say($nick, "You are not currently playing. Only players in the game may request this command.");
			} else {
				$roles = $this->getRoles($nick);
				if ($roles === false) {
					$this->say($nick, "Ur a wullager.");
				} else if ( in_array("cursed", $roles) ) {
					// Don't tell the cursed that he's cursed; it defeats the purpose.
					$cursed_roles = $roles;
					$pos = array_search("cursed", $cursed_roles);
					unset($cursed_roles[$pos]);
					$this->say($nick, "You currently hold the role(s): " . implode(", ", $cursed_roles));
				} else {
					//Tell the player what roles they hold.
					$this->say($nick, "You currently hold the role(s): " . implode(", ", $cursed_roles));
				}
			}
		}

	}

	/**
	 * Add a player to the game
	 * 
	 * @param mixed $nick Nickname of player
	 */
	public function addPlayer($channel, $nick) {
		$this->players[$nick] = time();
		// Add Voice
		$this->sendData("MODE $channel +v $nick");
	}

	/**
	 * Remove a player from the game
	 * 
	 * @param mixed $nick Nickname of player
	 */
	public function removePlayer($channel, $nick) {
		unset($this->players[$nick]);
		// Remove Voice
		$this->sendData("MODE $channel -v $nick");

		foreach($this->role_names as $role) {
			foreach($this->$role as $key => &$person) {
				if ($nick == $person) {
					unset($this->{$role}[$key]);
				}
			}
		}
	}

	/**
	 * Assign player roles
	 */
	public function assignRoles() {
		global $roles;

		// Get the array of # of roles according to player amount
		$player_amount = count($this->players);
		foreach ($roles as $key => $value) {
			if ( $player_amount >= $key ) {
				$index = $key;
			} else {
				break;
			}
		}
		$game_roles = $roles[$index];
		$this->roles_set = $game_roles;

		// Reconstruct players array
		$i=0;
		foreach(array_keys($this->players) as $key) {
			$players_sorted[$i] = $key;
			$i++;
		}

		// Backup players for original
		$this->players_original = $players_sorted;

		// Start looping through each role and assign them
		foreach($game_roles as $role => $amount) {

			$role_name = $this->getRoleByID($role);
			echo "\n$role_name : ";

			// Assign each role the proper amount of times
			for($i=1; $i <= $amount; $i++) {

				// Loop until role is given to proper candidate
				// NOTE: This will result in an infinite loop if settings are manipulated wrong
				while(true) {
					echo "Finding $role_name";
					// Player index to receive role
					$player_index = $this->random->getRandInt(0, $player_amount-1);
					echo ": $players_sorted[$player_index]\n";

					// Do not assign if it's already assigned
					if ( in_array($players_sorted[$player_index], $this->$role_name) ) {
						continue;
					}
					// If the person is a seer, only give them gunner
					if ( in_array($players_sorted[$player_index], $this->seer) && $role_name != 'gunner' ) {
						continue;
					}
					// If the person is a drunk, only give them gunner
					if ( in_array($players_sorted[$player_index], $this->drunk) && $role_name != 'gunner' ) {
						continue;
					}
					// If the person is cursed, only give them traitor
					if ( in_array($players_sorted[$player_index], $this->cursed) && $role_name != 'traitor' ) {
						continue;
					}
					// If the person is already harlot, angel, or detective, do not give them another role
					if ( in_array($players_sorted[$player_index], $this->harlot) || in_array($players_sorted[$player_index], $this->angel) || in_array($players_sorted[$player_index], $this->detective) ) {
						continue;
					}
					// If the person is a wolf, they can't have anything else
					if ( in_array($players_sorted[$player_index], $this->wolf) ) {
						continue;
					}

					// Conditions satisfied, give another role!
					array_push($this->$role_name, $players_sorted[$player_index]);
					break;
				}
			}
		}
		// Roles have all been assigned, we can now send directions
		foreach ($this->role_names as $role) {
			if (count($this->$role) > 0) {
				foreach ($this->$role as $player) {
					$this->sendDirections($player, $role);
				}
			}
		}
	}

	/**
	 * Send the role directions to the assigned player
	 * 
	 * @param mixed $nick Nickname of player
	 * @param mixed $role Role player has been assigned
	 */
	public function sendDirections($nick, $role) {
		global $lang;

		switch($role) {
			case 'seer': // Seer
				// Role Assign
				$seer_assign = $this->decode($lang["seer_assign"]);
				$this->say($nick, $seer_assign);

				// List players
				$answer=$this->listPlayers(false, false, $nick);
				$players_list = $this->decode($lang["players_list"], null, $answer);
				$this->say($nick, $players_list);
			break;

			case 'wolf': // Wolf
				// Role Assign
				$wolf_assign = $this->decode($lang["wolf_assign"]);
				$this->say($nick, $wolf_assign);

				// Multiple wolves relay message
				if (count($this->wolf) + count($this->traitor) > 1) {
					$wolf_multiple = $this->decode($lang["wolf_multiple"]);
					$this->say($nick, $wolf_multiple);
				}

				// List players
				$answer=$this->listPlayers(true, false, $nick);
				$players_list = $this->decode($lang["players_list"], null, $answer);
				$this->say($nick, $players_list);
			break;

			case 'cursed': // Cursed
				// Do not say anything to the cursed
			break;

			case 'drunk': // Drunk
				// Role Assign
				$drunk_assign = $this->decode($lang["drunk_assign"]);
				$this->say($nick, $drunk_assign);
			break;

			case 'harlot': // Harlot
				// Role Assign
				$harlot_assign = $this->decode($lang["harlot_assign"]);
				$this->say($nick, $harlot_assign);

				// List players
				$answer=$this->listPlayers(false, false, $nick);
				$players_list = $this->decode($lang["players_list"], null, $answer);
				$this->say($nick, $players_list);
			break;

			case 'traitor': // Traitor
				// Role Assign
				$traitor_assign = $this->decode($lang["traitor_assign"]);
				$this->say($nick, $traitor_assign);

				// Multiple wolves relay message
				if (count($this->wolf) + count($this->traitor) > 1) {
					$wolf_multiple = $this->decode($lang["wolf_multiple"]);
					$this->say($nick, $wolf_multiple);
				}

				// List players
				$answer=$this->listPlayers(true, false, $nick);
				$players_list = $this->decode($lang["players_list"], null, $answer);
				$this->say($nick, $players_list);
			break;

			case 'gunner': // Gunner
				// Role Assign
				$gunner_assign = $this->decode($lang["gunner_assign"]);
				$this->say($nick, $gunner_assign);
			break;

			case 'crow': // Crow
				// There is no crow yet
			break;

			case 'angel': // Angel
				// Role Assign
				$angel_assign = $this->decode($lang["angel_assign"]);
				$this->say($nick, $angel_assign);

				// List players
				$answer=$this->listPlayers(false, false, $nick);
				$players_list = $this->decode($lang["players_list"], null, $answer);
				$this->say($nick, $players_list);
			break;

			case 'detective': // Detective
				// Role Assign
				$detective_assign = $this->decode($lang["detective_assign"]);
				$this->say($nick, $detective_assign);
			break;
		}
	}

	/**
	 * Turns the game into day mode
	 * 
	 * @param mixed $victim The victim wolves slected for death (NULL for none)
	 * @param mixed $lasted_time Time night lasted for
	 */
	public function turnDay($lasted_time, $victim = null) {
		global $lang;

		$this->night_time = $this->night_time + $lasted_time;
		$day_turn = $this->decode($lang["day_turn"], null, $this->formatSeconds($lasted_time));
		$this->say($this->channel, $day_turn);

		if ($victim == null) {
			$day_nokill = $this->decode($lang["day_nokill"]);
			$this->say($this->channel, $day_nokill);
		} else {
			// If the harlot is selected, and he/she is not home...
			if ( in_array($victim, $this->harlot) && isset($this->harlot_visited[$victim]) ) {
				$day_harlot = $this->decode($lang["day_harlot"]);
				$this->say($this->channel, $day_harlot);
			} else {
				$dead_guy_roles = $this->getRoles($victim);
				if ($dead_guy_roles == false) {
					$role_list = 'villager';
				} else {
					$role_list = '';
					foreach ($dead_guy_roles as $role) {
						if ($role != 'cursed' || $role != 'gunner') {
							$role_list = $role_list . $role . ', ';
						}
					}
					$role_list = substr($role_list, 0, -2);
				}
				
				$day_victim = $this->decode($lang["day_victim"], null, $victim, $role_list);
				$this->say($this->channel, $day_victim);
				$this->removePlayer($this->channel, $victim);
			}

			// If the harlot visits a wolf!
			foreach($this->harlot as $harlot) {
				if ( isset($this->harlot_visited[$harlot]) && in_array($this->harlot_visited[$harlot], $this->wolf) ) {
					$day_harlot_dead = $this->decode($lang["day_harlot_dead"], null, $harlot);
					$this->say($this->channel, $day_harlot_dead);
					$this->removePlayer($this->channel, $harlot);
				}
			}

		}

		// Night has ended, we can reset the night actions
		$this->seer_seen = array();
		$this->wolf_killed = array();
		$this->harlot_visited = array();
		$this->angel_guarded = array();

		// Check if game has finished
		// Only possible end scenario at begginning of day is wolves equaling villagers =>>
		// since wolves can't get eaten
		if (count($this->players) / 2 <= count($this->wolf)) {
			// Wolves equal villagers
			$this->endGame($this->channel, 2);
		} else {
			$amount = count($this->players);

			// If the number is odd, subtract 1
			if ($amount % 2 != 0) {
				$amount--;
			}
			// Lynch message, and how many required to kill a victim.
			$day_vote = $this->decode($lang["day_vote"], null, $amount / 2 + 1);
			$this->say($this->channel, $day_vote);

			// Set booleans for.... THE SUN
			$this->isDay = true;
			$this->isNight = false;

			$this->current_night_start = false;
			$this->current_day_start = time();
		}
	}

	/**
	 * Turns the game into day mode
	 *
	 * @param mixed $victim The lynch victim
	 * @param mixed $lasted_time Time day lasted for
	 */
	public function turnNight($lasted_time, $victim = null) {
		global $lang;

		// Day has ended, we can reset these actions
		// NOTE: Do not reset gunner shots
		$this->detective_ided = array();

		// Clear lynches
		$this->lynch = array();

		if ($victim == null) {
			$night_nokill = $this->decode($lang["night_nokill"]);
			$this->say($this->channel, $night_nokill);
		} else {
			$dead_guy_roles = $this->getRoles($victim);
			if ($dead_guy_roles == false) {
				$role_list = 'villager';
			} else {
				$role_list = '';
				foreach ($dead_guy_roles as $role) {
					if ($role != 'cursed' || $role != 'gunner') {
						$role_list = $role_list . $role . ', ';
					}
				}
				$role_list = substr($role_list, 0, -2);
			}

			$night_victim = $this->decode($lang["night_victim"], null, $victim, $role_list);
			$this->say($this->channel, $night_victim);
			$this->removePlayer($this->channel, $victim);
		}

		// Check if game has finished
		if (count($this->wolf) == 0 && count($this->traitor) == 0) {
			// All evildoers are dead
			$this->endGame($this->channel, 1);
		} else if (count($this->players) / 2 <= count($this->wolf) + count($this->traitor) ) {
			// Wolves equal villagers
			$this->endGame($this->channel, 2);
		} else {
			// No more wolves, traitor transforms
			if (count($this->wolf) == 0 && count($this->traitor) > 0) {
				$traitor_transform = $this->decode($lang["traitor_transform"]);
				$this->say($this->channel, $traitor_transform);

				// Transform traitor and tell them
				$lucky_traitor = reset($this->traitor);
				array_push($this->wolf, $lucky_traitor);
				$traitor_notify = $this->decode($lang["traitor_notify"]);
				$this->say($lucky_traitor, $traitor_notify);
				unset($this->traitor[key($this->traitor)]);
			}

			$this->day_time = $this->day_time + $lasted_time;
			$night_turn = $this->decode($lang["night_turn"], null, $this->formatSeconds($lasted_time));
			$this->say($this->channel, $night_turn);

			// Set booleans for.... THE SUN
			$this->isDay = false;
			$this->isNight = true;

			$this->current_day_start = null;
			$this->current_night_start = time();

			foreach($this->role_names as $role) {
				if (count($this->$role) > 0) {
					foreach($this->$role as $person) {
						$this->sendDirections($person, $role);
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
		global $lang, $irc;

		// Game stopped, we can start blocking the socket again
		$irc->setBlocking(true);

		// If !fstop is being used, time should be considered for current night/day time
		if ($this->isDay == true) {
			$lasted_time = time() - $this->current_day_start;
			$this->day_time += $lasted_time;
		} else {
			$lasted_time = time() - $this->current_night_start;
			$this->night_time += $lasted_time;
		}

		// No game is running now
		$this->ongoing = false;

		// Game over message here
		if ($win == 0) {
			// No win
		} else if ($win == 1) {
			$win_villagers = $this->decode($lang["win_villagers"]);
			$this->say($channel, $win_villagers);
		} else if ($win == 2) {
			$win_wolves = $this->decode($lang["win_wolves"]);
			$this->say($channel, $win_wolves);
		}

		// Time and portions
		$total = $this->day_time + $this->night_time;
		$end_times = $this->decode($lang["end_times"], null, $this->formatSeconds($total), $this->formatSeconds($this->day_time), $this->formatSeconds($this->night_time));
		$this->say($channel, $end_times);

		// Send previously generated roles message
		$this->say($channel, $this->roles_message);

		// Unmute the channel
		$this->sendData('MODE ' . $channel . ' -m');

		// Generate list of players
		$players_list = "";
		foreach(array_keys($this->players) as $person) {
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
		$this->players = array();
		$this->lynch = array();

		$wait_extended = 0;

		// Clear all player roles
		$this->seer = array();
		$this->wolf = array();
		$this->cursed = array();
		$this->drunk = array();
		$this->harlot = array();
		$this->traitor = array();
		$this->gunner = array();
		$this->crow = array(); 
		$this->angel = array();
		$this->detective = array();

		$this->seer_seen = array();
		$this->wolf_killed = array();
		$this->harlot_visited = array();
		$this->gunner_shot = array();
		$this->angel_guarded = array();
		$this->detective_ided = array();
		$this->roles_message = '';

		$this->roles_message = '';

		$this->isDay = null;
		$this->isNight = null;
		$this->day_time = 0;
		$this->night_time = 0;

		unset($this->players_original);
	}

	/**
	 * Run functions not invoked by messages
	 */
	public function runMaintenance() {
		global $lang, $length;

		if ($this->ongoing) {

			// Check if game has finished
			if (count($this->wolf) == 0 && count($this->traitor) == 0 ) {
				// All evildoers are dead
				$this->endGame($this->channel, 1);
			} else if (count($this->wolf) == 0 && count($this->traitor) > 0) {
				// No more wolves, traitor transforms
				$traitor_transform = $this->decode($lang["traitor_transform"]);
				$this->say($this->channel, $traitor_transform);

				// Transform traitor and tell them
				$lucky_traitor = reset($this->traitor);
				array_push($this->wolf, $lucky_traitor);
				$traitor_notify = $this->decode($lang["traitor_notify"]);
				$this->say($lucky_traitor, $traitor_notify);
				unset($this->traitor[key($this->traitor)]);
			} else if (count($this->players) / 2 <= count($this->wolf) + count($this->traitor) ) {
				// Wolves equal villagers
				$this->endGame($this->channel, 2);
			}

			if ($this->isNight) {
				// Has the time limit run out yet?
				$lasted_time = time() - $this->current_night_start;
				// If the time limit has run out, or someone ran fday
				if ($lasted_time >= $length["night"] || $this->fday) {
					$first_kill = reset($this->wolf_killed);
					if ( !empty($first_kill) ) {
						$this->turnDay($lasted_time, $first_kill);
					} else {
						$this->turnDay($lasted_time);
					}

					if ($this->fday == true) {
						$this->fday = false;
					}
				} else {

					$i_role = 0;
					$i_condition = 0;
					foreach($this->role_names as $role) {
						if (count($this->$role) > 0) {
							if ($role == 'seer') {
								if (count($this->seer_seen) == count($this->seer)) {
									$i_condition++;
								}
							} else if ($role == 'wolf') {
								if (count($this->wolf_killed) == count($this->wolf)) {
									$i_condition++;
								}
							} else if ($role == 'harlot') {
								if (count($this->harlot_visited) == count($this->harlot)) {
									$i_condition++;
								}
							} else if ($role == 'angel') {

							} else if ($role == 'detective') {

							}

							if ($role != 'drunk' && $role != 'cursed' && $role != 'traitor' && $role != 'gunner' && $role != 'detective') {
								$i_role++;
							}
						}
					}
					if ($i_role == $i_condition) {
						$this->turnDay( $lasted_time, reset($this->wolf_killed) );
					}
				}

			} else if ($this->isDay) {
				// Has the time limit run out yet?
				$lasted_time = time() - $this->current_day_start;
				// If the time limit has run out, or someone ran fday
				if ($lasted_time >= $length["day"] || $this->fnight) {
					$this->turnNight($lasted_time);

					if ($this->fnight) {
						$this->fnight = false;
					}
				} else {

					foreach($this->lynch as $lyncher => $lynchee) {
						if (isset($$lynchee)) {
							$$lynchee = $$lynchee +1;
						} else {
							$$lynchee = 1;
						}
					}
	
					$amount = count($this->players);
					// If the number is odd, subtract 1
					if ($amount % 2 != 0) {
						$amount--;
					}
					// Lynch message, and how many required to kill a victim.
					$amount = $amount / 2 + 1;
					foreach(array_keys($this->players) as $player) {
						if ( isset($$player) && $$player >= $amount) {
							$lasted_time = time() - $this->current_day_start;
							$this->turnNight($lasted_time, $player);
						}
					}

				}
			}

			// This function is being repeated called, sleep for a hundreth of a second
			usleep(10000);

		}

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

					// Wolf relay for game
					// If the game ongoing, recipient is phpwolf,          user is wolf or traitor,                         and there is more than one wolf
					if ($this->ongoing && $recipient == $user["nick"] && ( in_array($nick, $this->wolf) || in_array($nick, $this->traitor) ) && count($this->wolf) > 1) {
						$this->relayToWolves($nick, $message);
					}

					$this->commands($recipient, $message, $nick, $host);
				break;
				case 'KICK':
				case 'QUIT': // Alias PART
					$nick = $data["3"];
				case 'PART':
					// Remove from peoples list
					$key = array_search($nick, $this->people);
					unset($this->people[$key]);

					$recipient = $data["2"];

					// Quit update for game
					if (isset($this->players[$nick])) {
						$this->commands($recipient, "!quit", $nick, $host);
					}
					// Quit update for lynch
					if (isset($this->lynch[$nick])) {
						unset($this->lynch[$nick]);
					}
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

					// Nick update for game
					if (isset($this->players[$nick])) {
						$this->players[$new_nick] = $this->players[$nick];
						unset($this->players[$nick]);
					}
					// Nick update for lynch
					if (isset($this->lynch[$nick])) {
						$this->lynch[$new_nick] = $this->lynch[$nick];
						unset($this->lynch[$nick]);
					}
					// Nick update for away
					if (isset($this->away[$nick])) {
						$this->away[$new_nick] = $this->away[$nick];
						unset($this->away[$nick]);
					}
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