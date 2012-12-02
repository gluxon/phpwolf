<?php
define('NO_OPERATOR', true);
define('NO_NICK', null);

class wolf extends irc {

	// Start random module
	private $random;

	// List of players
	private $players, $players_original;
	private $lynch, $away;

	// In-game stores
	private $people, $channel, $ongoing;

	// Counters and timeouts
	private $game_wait, $wait_extended, $last_stats, $last_ping;

	// Day and night
	private $isDay, $isNight;
	private $day_time, $night_time;
	private $current_day_start, $current_night_start;

	// Goat
	private $goat_switch;

	// Pre-generated and store end roles message
	private $roles_message;

	// Role variables
	public $role_names = array("wolf", "seer", "cursed", "drunk", "harlot", "traitor", "gunner", "crow", "angel", "detective"); // should be const, but constants can't be arrays
	private $seer, $wolf, $cursed, $drunk, $harlot, $traitor, $gunner, $crow, $angel, $detective;
	private $roles_set; // roles available corresponding to amount of players

	// Seen, visited, killed variables (per night/day)
	private $seer_seen, $wolf_killed, $harlot_visited, $gunner_shot, $gunner_bullets, $angel_guarded, $detective_ided;

	/**
	 * Constructor for werewolf, initialize variables with default values
	 */
	public function __construct() {
		// Inherited IRC Class doesn't run through constructor since we overrided it here
		// TokenBucket
		$this->TokenBucket = new TokenBucket(23, 1.73);

		$this->random = new random();

		$this->players = array();
		$this->lynch = array(); $this->away = array();

		$this->people = array(); $this->ongoing = false;

		$this->wait_extended = 0; $this->last_stats = 0; $this->last_ping = 0;

		$this->isDay = null; $this->isNight = null;
		$this->day_time = 0; $this->night_time = 0;

		$this->goat_switch = true;

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

		$this->seer_seen = array();
		$this->wolf_killed = array();
		$this->harlot_visited = array();
		$this->gunner_shot = array();
		$this->gunner_bullets = array();
		$this->angel_guarded = array();
		$this->detective_ided = array();
	}

	/**
	* Check if the string is a valid corresponding command
	*
	* @param mixed $string String to check
	* @param mixed $command The command the string should be
	* @param boolean $no_operator Set to true to remove operator requirement
	* @return boolean True or False answer
	*/
	public function isCommand ($string, $command, $no_operator=false) {
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
	 * @param mixed $answer Answer to replace %answer2%
	 * @param mixed $answer Answer to replace %answer3%
	 * @return string String with bolded words and correct answer values
	 */
	public function decode($string, $nick=null, $answer=null, $answer2=null, $answer3=null) {
		$return = str_replace("<bold>", chr(2), $string);
		$return = str_replace("</bold>", chr(2), $return);

		if (isset($nick))
			$return = str_replace("%nick%", $nick, $return);
		if (isset($answer))
			$return = str_replace("%answer%", $answer, $return);
		if (isset($answer2))
			$return = str_replace("%answer2%", $answer2, $return);
		if (isset($answer3))
			$return = str_replace("%answer3%", $answer3, $return);

		return $return;
	}

	/**
	 * Return a string with formated seconds
	 * 
	 * @param int Seconds
	 * @param string Formatted seconds ex: 05:32
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
	* @return boolean True or False answer
	*/
	public function isAdmin($host) {
		global $user, $admins;

		if (in_array($host, $admins)) {
			return true;
		}
		return false;
	}

	/**
	 * Nickname auto-complete
	 * 
	 * @param mixed $nick First part of desired nickname
	 * @return string Full nickname of player
	 */
	public function nickAutocomplete($nick) {
		foreach(array_keys($this->players) as $player) {
			// if $nick is at beginning (case-insensitive
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
			return $nick;
		}
		return $return;
	}

	/**
	 * Check if the player is playing
	 * 
	 * @param mixed $nick Nickname of potential player
	 * @return boolean True or False answer
	 */
	public function isPlaying($nick) {
		if (!empty($this->players[$nick])) {
			return true;
		}
		return false;
	}

	/**
	 * Get all the roles a player has
	 * 
	 * @param mixed $nick Nickname of player
	 * @return array Array containing roles of specified player
	 */
	public function getRoles($nick) {
		foreach($this->role_names as $role) {
			if (in_array($nick, $this->$role)) {
				if (!isset($return)) {
					$return = array($role);
				} else {
					array_push($return, $role);
				}
			}
		}
		if (isset($return)) {
			return $return;
		} else {
			return false;
		}
	}

	/**
	 * Get all the roles a player has and return it as a nice string
	 *
	 * @param mixed $nick Nickname of player
	 * @param boolean $show_cursed Show the cursed and gunner
	 * @param boolean $bold_roles Bold roles if true
	 * @return string A string containing a list of roles in sensible English.
	 */
	public function getRolesNice($nick, $bold_roles, $show_cursed=false, $show_gunner=false) {
		global $lang;

		$roles = $this->getRoles($nick);

		if ($roles === false) {
			$role_list = chr(2) .  $this->decode($lang["villager"]) . chr(2);
		} else {

			$role_amount = count($roles);
			if (in_array('cursed', $roles) && !$show_cursed)
				$role_amount--;
			if (in_array('gunner', $roles) && !$show_gunner)
				$role_amount--;

			$role_list = "";
			foreach($roles as $key => $role) {

				// Append "and" before the end of the list
				if ( $key + 1 == $role_amount && $role_amount > 1 ) {
					// Remove a space and comma if there is only two items
					if ($role_amount == 2) {
						$role_list = substr($role_list, 0, -2) . " ";
					}
					$role_list .= 'and ';
				}

				if ( ($role=='cursed' && !$show_cursed) || ($role=='gunner' && !$show_gunner) ) {
					if ( $role_amount <= 0 && empty($role_list) ) {
						$role_list .= chr(2) . $this->decode($lang["villager"]) . chr(2);
					}
				} else {
					$role_list .= chr(2) . $this->decode($lang[$role]) . chr(2);
				}

				if ( $key + 1 < $role_amount ) {
					$role_list .= ', ';
				}

			}

		}

		if ($bold_roles) {
			return $role_list;
		} else {
			return str_replace(chr(2), '', $role_list);
		}

	}

	/**
	 * Get the name of a role by id
	 * 
	 * @param int $id ID of role
	 * @return string Name of role
	 */
	public function getRoleByID($id) {
		switch($id) {
			case 0:
				return 'seer';
			case 1:
				return 'wolf';
			case 2:
				return 'cursed';
			case 3:
				return 'drunk';
			case 4:
				return 'harlot';
			case 5:
				return 'traitor';
			case 6:
				return 'gunner';
			case 7:
				return 'crow';
			case 8:
				return 'angel';
			case 9:
				return 'detective';
			default:
				return false;
		}
	}

	/**
	 * Get the ID of a role
	 * 
	 * This function is like an ENUM, but PHP does not support enum blocks
	 * 
	 * @param mixed $role Role name
	 * @return int ID of role
	 */
	public function getRoleID($role) {
		switch($role) {
			case 'seer':
				return 0;
			case 'wolf':
				return 1;
			case 'cursed':
				return 2;
			case 'drunk':
				return 3;
			case 'harlot':
				return 4;
			case 'traitor':
				return 5;
			case 'gunner':
				return 6;
			case 'crow':
				return 7;
			case 'angel':
				return 8;
			case 'detective':
				return 9;
			default:
				return false;
		}
	}

	/**
	 * Returns the reaction to a scenario
	 * 
	 * @param mixed $action The action that is performed
	 * @param mixed $reaction The reaction to the action
	 */
	public function chance($action) {
		global $gun_shot, $gun_shot_drunk, $manslaughter;
		global $guardian_angel_dies, $detective_revealed;

		switch ($action) {
			case 'gun_shot':
				$spin = $this->random->getRandInt(1, $gun_shot[0][1]);
				if ($spin <= $gun_shot[0][0]) {
					return 1;
				} else if ($spin <= $gun_shot[0][0] + $gun_shot[1][0]) {
					return 2;
				} else if ($spin <= $gun_shot[0][0] + $gun_shot[1][0] + $gun_shot[2][0]) {
					return 3;
				}
			break;

			case 'gun_shot_drunk':
				$spin = $this->random->getRandInt(1, $gun_shot_drunk[0][1]);
				if ($spin <= $gun_shot_drunk[0][0]) {
					return 1;
				} else if ($spin <= $gun_shot_drunk[0][0] + $gun_shot_drunk[1][0]) {
					return 2;
				} else if ($spin <= $gun_shot_drunk[0][0] + $gun_shot_drunk[1][0] + $gun_shot_drunk[2][0]) {
					return 3;
				}
			break;

			case 'manslaughter':
				$spin = $this->random->getRandInt(1, $manslaughter[1]);
				if ($spin <= $manslaughter[0]) {
					return true;
				} else {
					return false;
				}
			break;

			case 'guardian_angel_dies':
				$spin = $this->random->getRandInt(1, $guardian_angel_dies[1]);
				if ($spin <= $guardian_angel_dies[0]) {
					return true;
				} else {
					return false;
				}
			break;

			case 'detective_revealed':
				$spin = $this->random->getRandInt(1, $detective_revealed[1]);
				if ($spin <= $detective_revealed[0]) {
					return true;
				} else {
					return false;
				}
			break;

			default:
				return null;
		}
	}

	/**
	 * Return a list of all the players
	 * 
	 * 
	 * @param boolean $wolf Add wolf role to other wolves
	 * @param boolean $reset Reset Times
	 * @param mixed $nick Nick of player to not add
	 * @return mixed List of players
	 */
	public function listPlayers($wolf = false, $nick = null) {
		global $lang;

		$list = "";
		foreach(array_keys($this->players) as $player) {
			if ($player != $nick) {
				$list = $list . $player;

				// Display wolves as their role
				if ($wolf) {
					if ( in_array($player, $this->wolf) ) {
						$list = $list . ' (' . $lang["wolf"] . ')';
					} else if ( in_array($player, $this->traitor) ) {
						$list = $list . ' (' . $lang["traitor"] . ')';
					}
				}

				$list = $list . ', ';
			}
		}
		return substr($list, 0, -2);
	}

	public function resetPlayerTimeouts() {
		foreach($this->players as &$value) {
			$value = time();
		}
	}

	/**
	 * Send a message to the rest of the wolves
	 * 
	 * @param mixed $send Nickname of sender
	 * @param mixed $message Message being sent
	 */
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
		global $user, $lang, $setting, $limit, $length, $irc;

		// Remove double, extraneous whitespaces
		$message = preg_replace("/\s+/", " ", $message);
		if ( substr($message, -1, 1) == " " ) {
			$message = substr($message, 0, -1);
		}

		// Split into a command, and its parameters ($message)
		if ( strpbrk(" ", $message) ) {
			$ex_message = explode(" ", $message, 2);
			$command=$ex_message["0"];
			// This doesn't avoid double space
			if ( !empty($ex_message["1"]) ) {
				$message=$ex_message["1"];
			} else {
				$message="";
			}
		} else {
			$command=$message;
			$message="";
		}

		// !help
		if ( $this->isCommand($command, 'help') ) {
			$help = $this->decode($lang["help"]);
			$this->say($nick, $help);
		}

		// !simple
		else if ( $this->isCommand($command, 'simple') ) {
			$this->say($nick, "This feature on phpwolf is not yet available.");
		}

		// !back
		else if ( $this->isCommand($command, 'back') ) {
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
		else if ( ( $this->isCommand($command, 'quit') || $this->isCommand($command, 'leave') ) ) {
			if (!$this->isPlaying($nick)) {
				$not_playing_self=$this->decode($lang["not_playing_self"]);
				$this->notice($nick, $not_playing_self);
			} else {
				// Send quit message
				if ($this->ongoing) {
					$answer = $this->getRolesNice($nick, false, false, false);
				} else {
					$answer = "person";
				}

				$this->removePlayer($recipient, $nick);

				$quit = $this->decode($lang["quit"], $nick, $answer);
				$this->say($recipient, $quit);
			}
		}

		// !stats
		else if ( $this->isCommand($command, 'stats') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if (time() - $this->last_stats < $limit["stats"]) {
				$ratelimit = $this->decode($lang["ratelimit"]);
				$this->notice($nick, $ratelimit);
			} else {

				// Get amount of players
				$answer = count($this->players);
				if ($answer == 0) {
					// There are no users playing
					$game_none = $this->decode($lang["game_none"]);
					$this->notice($nick, $game_none);
				} else {
					// Ratelimit for stats
					$this->last_stats = time();

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
						// ignore gunner and cursed, do not count them in specials_total either
						if ($this->roles_set[$role_id] > 0 && $role_name != 'cursed' && $role_name != 'gunnder' ) {
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
		else if ( $this->isCommand($command, 'away') ) {
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
		else if ( $this->isCommand($command, 'ping') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ($this->ongoing) {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			} else if (time() - $this->last_ping < $limit["ping"]) {
				$ratelimit = $this->decode($lang["ratelimit"]);
				$this->notice($nick, $ratelimit);
			} else {
				$this->last_ping = time();

				$list = "";
				foreach($this->people as $person) {
					if ( !in_array($person, $this->away) && !isset($this->players[$person]) ) {
						$list = $list . $person . ' ';
					}
				}
				$list = substr($list, 0, -1);
				// This returns PING %answer% if $list is empty
				$ping = $this->decode($lang["ping"], NO_NICK, $list);
				$this->say($recipient, $ping);
			}
		}

		// !start
		else if ( $this->isCommand($command, 'start') ) {
			$count = count($this->players);
			$time = time() - $this->game_wait;

			// There are no players
			if ($count == 0) {
				$game_none = $this->decode($lang["game_none"]);
				$this->notice($nick, $game_none);
			} 

			// Time limit has not passed
			else if ( $time < $setting["game_wait"] ) {
				$game_wait = $this->decode($lang["game_wait"], NO_NICK, $setting["game_wait"] - $time);
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
				$answer = $this->listPlayers(false);

				// Reset Player timeouts at beginning of game
				$this->resetPlayerTimeouts();

				$game_welcome = $this->decode($lang["game_welcome"], NO_NICK, $answer);
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
							$this->roles_message = $this->roles_message . $this->decode($lang["end_roles_plural"], NO_NICK, $lang[$role . '_plural'], $role_list) . ' ';
						} else {
							// This is a hackish workaround, $person is still the looped array since $numItems is 1
							$this->roles_message = $this->roles_message . $this->decode($lang["end_roles"], NO_NICK, $lang[$role], $person) . ' ';
						}
					}
				}
				$this->roles_message = substr($this->roles_message, 0, -1); // remove trailing space

				// We do not want socket blocking for werewolf now
				$irc->setBlocking(false);

			}

		}

		// !retract
		else if ( $this->isCommand($command, 'retract') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ($this->ongoing == false) {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			} else if (!$this->isPlaying($nick)) {
				// This person is not playing, let's just ignore them
			} else if ($this->isNight) {
				$this->say($nick, "Error, it is not day time");
			} else if (isset($this->lynch[$nick])) {
				unset($this->lynch[$nick]);
				$retract = $this->decode($lang["retract"], $nick);
				$this->say($recipient, $retract);
			}
		}

		// !goat
		else if ( $this->isCommand($command, 'goat') ) {
			if ($this->ongoing == false){
				$game_none = $this->decode($lang["game_none"]);
				$this->say($recipient, $game_none);
			} else if (!$this->isPlaying($nick)) {
				$not_playing = $this->decode($lang["not_playing"], NO_NICK, $this->nickAutocomplete($message));
				$this->say($recipient, $not_playing);
			} else if ($this->isNight) {
				$this->say($nick, "Error, it is not day time");
			} else if ($this->goat_switch == false) {
				$this->say($recipient, "That can only be done once per day.");
			} else {
				$goat = $this->decode($lang["goat"], $nick, $this->nickAutocomplete($message));
				$this->say($recipient, $goat);
				$this->goat_switch = false;
			}
		}

		// !votes
		else if ( $this->isCommand($command, 'votes') ) {
			if ($recipient == $user["nick"]) {
				// ignore
			} else if (!$this->ongoing) {
				$game_none = $this->decode($lang["game_none"]);
				$this->notice($nick, $game_none);
			} else if ($this->isNight) {
				$votes_night = $this->decode($lang["votes_night"]);
				$this->notice($nick, $votes_night);
			} else if ( count($this->lynch) == 0 ) {
				$votes_none = $this->decode($lang["votes_none"], $nick);
				$this->say($recipient, $votes_none);
			} else {
				foreach($this->lynch as $prosecutor => $defendor) {
					if (isset($defendants[$defendor])) {
						array_push($defendants[$defendor], $prosecutor);
					} else {
						$defendants[$defendor] = array($prosecutor);
					}
				}
				$votes_list = '';
				foreach ($defendants as $defendor => $prosecutors) {
					$votes_list = $votes_list . $defendor . ': ' . count($prosecutors) . ' (' . implode(' ', $prosecutors) . ')' . ', ';
				}
				$votes_list = substr($votes_list, 0, -2);

				if ($votes_list != false) {
					$votes = $this->decode($lang["votes"], $nick, $votes_list);
					$this->say($recipient, $votes);

					// players available to vote is subtracted from unexistant idle mechanism
					$votes_required = floor(count($this->players) / 2) + 1 - count($this->gunner_shot);
					$votes_available = count($this->players);

					$votes_status = $this->decode($lang["votes_status"], $nick, count($this->players), $votes_required, $votes_available);
					$this->say($recipient, $votes_status);

				} else {
					// no votes yet
				}
			}
		}

		// !lynch
		else if ( $this->isCommand($command, 'lynch') || $this->isCommand($command, 'vote') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if (!$this->ongoing) {
				$game_none = $this->decode($lang["game_none"]);
				$this->notice($nick, $game_none);
			} else {
				$message = explode(' ', $message, 2);
				$victim = $this->nickAutocomplete($message["0"]);

				if (!$this->isPlaying($nick)) {
					// This person is not playing, let's just ignore them (and hope they go away)
				} else if (!$this->isPlaying($victim)) {
					$not_playing = $this->decode($lang["not_playing"], NO_NICK, $victim);
					$this->notice($nick, $not_playing);
				} else if ($this->isNight) {
					$lynch_night = $this->decode($lang["lynch_night"], NO_NICK, $victim);
					$this->notice($nick, $lynch_night);
				} else if ( in_array($nick, $this->gunner_shot) ) {
					$lynch_resting = $this->decode($lang["lynch_resting"], $nick);
					$this->say($recipient, $lynch_resting);
				} else {
					$this->lynch[$nick] = $victim;
					$lynch = $this->decode($lang["lynch"], $nick, $victim);
					$this->say($recipient, $lynch);
				}
			}
		}

		// !coin
		else if ( $this->isCommand($command, 'coin') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else {
				$coin_toss = $this->decode($lang["coin_toss"], $nick);
				$this->say($recipient, $coin_toss);

				$int = $this->random->getRandInt(1, 2);
				if ($int == 1) {
					$coin_land = $this->decode($lang["coin_land"], NO_NICK, "heads");
					$this->say($recipient, $coin_land);
				} else if ($int == 2) {
					$coin_land = $this->decode($lang["coin_land"], NO_NICK, "tails");
					$this->say($recipient, $coin_land);
				} else {
					$this->say($recipient, "There as an error retrieving random.org results.");
				}
			}
		}

		// !wait
		else if ( $this->isCommand($command, 'wait') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ($this->ongoing == false) {
				if ($this->wait_extended < $setting["game_add_wait_limit"]) {

					$this->wait_extended += 1;
					$this->game_wait += $setting["game_add_wait"];

					$wait = $this->decode($lang["wait"], $nick, $setting["game_add_wait"]);
					$this->say($recipient, $wait);
				} else {
					$wait_reached = $this->decode($lang["wait_reached"]);
					$this->say($recipient, $wait_reached);
				}
			} else {
				$start_ongoing = $this->decode($lang["start_ongoing"]);
				$this->notice($nick, $start_ongoing);
			}
		}

		// !join
		else if ( $this->isCommand($command, 'join') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ($this->ongoing) {
				$start_ongoing = $this->decode($lang["start_ongoing"], $nick);
				$this->notice($nick, $start_ongoing);
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
		else if ( $this->isCommand($command, 'rules') ) {
			$rules_part1 = $this->decode($lang["rules_part1"]);
			$this->say($recipient, $rules_part1);
			$rules_part2 = $this->decode($lang["rules_part2"]);
			$this->say($recipient, $rules_part2);
		}

		// !admins
		else if ( $this->isCommand($command, 'admins') ) {
			$this->say($recipient, "Snuupy is the man for that.");
		}

		/***** Admin commands *****/
		// !msg
		else if ( $this->isCommand($command, 'msg') && $this->isAdmin($host) ) {
			$message=explode(" ", $message, 2);
			$this->say($message["0"], $message["1"]);
		}

		// !op
		else if ( $this->isCommand($command, 'op') && $this->isAdmin($host) ) {
			$message=explode(" ", $message);
			if (empty($message["0"]) && $recipient != $nick) { 
				$this->mode($recipient, '+o', $nick);
			} else if (count($message) >= 2) {
				$this->mode($message["0"], '+o', $message["1"]);
			} else if (count($message) >= 1 && $recipient != $nick) {
				$this->mode($recipient, '+o', $message["0"]);
			}
		}

		// !deop
		else if ( $this->isCommand($command, 'deop') && $this->isAdmin($host) ) {
			$message=explode(" ", $message);
			if (empty($message["0"]) && $recipient != $nick) { 
				$this->mode($recipient, '-o', $nick);
			} else if (count($message) >= 2) {
				$this->mode($message["0"], '-o', $message["1"]);
			} else if (count($message) >= 1 && $recipient != $nick) {
				$this->mode($recipient, '-o', $message["0"]);
			}
		}

		// !voice
		else if ( $this->isCommand($command, 'voice') && $this->isAdmin($host) ) {
			$message=explode(" ", $message);
			if (empty($message["0"]) && $recipient != $nick) {
				$this->mode($recipient, '+v', $nick);
			} else if (count($message) >= 2) {
				$this->mode($message["0"], '+v', $message["1"]);
			} else if (count($message) >= 1 && $recipient != $nick) {
				$this->mode($recipient, '+v', $message["0"]);
			}
		}

		// !devoice
		else if ( $this->isCommand($command, 'devoice') && $this->isAdmin($host) ) {
			$message=explode(" ", $message);
			if (empty($message["0"]) && $recipient != $nick) {
				$this->mode($recipient, '-v', $nick);
			} else if (count($message) >= 2) {
				$this->mode($message["0"], '-v', $message["1"]);
			} else if (count($message) >= 1 && $recipient != $nick) {
				$this->mode($recipient, '-v', $message["0"]);
			}
		}

		// !quiet
		else if ( $this->isCommand($command, 'quiet') && $this->isAdmin($host)  ) {
			$message=explode(" ", $message);
			if (empty($message["0"]) && $recipient != $nick) {
				$this->mode($recipient, '+q', $nick);
			} else if (count($message) >= 2) {
				$this->mode($message["0"], '+q', $message["1"]);
			} else if (count($message) >= 1 && $recipient != $nick) {
				$this->mode($recipient, '+q', $message["0"]);
			}
		}

		// !unquiet
		else if ( $this->isCommand($command, 'unquiet') && $this->isAdmin($host) ) {
			$message=explode(" ", $message);
			if (empty($message["0"]) && $recipient != $nick) {
				$this->mode($recipient, '-q', $nick);
			} else if (count($message) >= 2) {
				$this->mode($message["0"], '-q', $message["1"]);
			} else if (count($message) >= 1 && $recipient != $nick) {
				$this->mode($recipient, '-q', $message["0"]);
			}
		}

		// !fstart
		else if ( $this->isCommand($command, 'fstart') && $this->isAdmin($host) ) {
			$this->game_wait = 0;
			$this->commands($recipient, "!start", $nick, $host);
		}

		// !fstop
		else if ( $this->isCommand($command, 'fstop') && $this->isAdmin($host) ) {
			if ($this->ongoing) {
				$this->endGame($this->channel);
			}
		}

		// !fday
		else if ( $this->isCommand($command, 'fday') && $this->isAdmin($host) ) {
			if ($this->isNight && $this->ongoing) {
				$length_night_original = $length["night"];

				$length["night"] = 0;
				$this->runMaintenance();
				$length["night"] = $length_night_original;
			}
		}

		// !fnight
		else if ( $this->isCommand($command, 'fnight') && $this->isAdmin($host) ) {
			if ($this->isDay && $this->ongoing) {
				$length_day_original = $length["day"];

				$length["day"] = 0;
				$this->runMaintenance();
				$length["day"] = $length_day_original;
			}
		}

		// !fjoin
		else if ( $this->isCommand($command, 'fjoin') && $this->isAdmin($host) ) {
			// Trim off everything after a space
			$space = strpos($message, " ");
			if ($space !== false) {
				$message = substr($message, 0, $space);
			}

			if ($message == "") {
				$parameters=$this->decode($lang["parameters"], NO_NICK, $message);
				$this->notice($nick, $parameters);
			} else {
				$ongoing = $this->ongoing;
				$this->ongoing = false;
				$this->commands($recipient, "!join", $message, $host);
				$this->ongoing = $ongoing;
			}
		}

		// !fquit and !fleave
		else if ( ( $this->isCommand($command, 'fquit') || $this->isCommand($command, 'fleave') ) && $this->isAdmin($host) ) {
			// Trim off everything after a space
			$space = strpos($message, " ");
			if ( $space !== false ) {
				$message = substr($message, 0, $space);
			}

			if ($message == "") {
				$parameters=$this->decode($lang["parameters"], NO_NICK, $message);
				$this->notice($nick, $parameters);
			} else {
				$this->commands($recipient, "!quit", $message, $host);
			}
		}

		// !fping
		else if ( $this->isCommand($command, 'fping') && $this->isAdmin($host) ) {
			$this->last_ping = time() - $limit["ping"] - 1;
			$this->commands($recipient, "!ping", $nick, $host);
		}

		// !frestart
		else if ( $this->isCommand($command, 'frestart') && $this->isAdmin($host) ) {
			// Hack from -> http://debuggable.com/posts/restarting-a-php-command-line-script:4988968d-b9d0-4fe8-af30-05544834cda3
			//echo "\nphpwolf: RESTARTING";
			//die(exec('php ' . str_replace(' ', '\ ', getcwd()) . '/' . implode(' ', $GLOBALS['argv']) . ' > /dev/null &'));
		}

		// !access
		else if ( $this->isCommand($command, 'access') ) {
			global $admins;
			$args = explode(" ", $message);
			if (count($args) < 2) {
				$this->say($recipient, "$nick, There are \002two\002 arguments required for this command, " . count($args) . " given.");
			} else if (!$this->isAdmin($host) ) {
				$this->say($recipient, $nick . ", You are not a bot admin.");
			} else {
				switch($args[0]) {
					case '+admin':
						array_push($admins, $args[1]);
						$this->say($recipient, $nick . ", Operation executed successfully.");
						break;
					case '-admin':
						$pos = array_search($args[1], $admins);
						unset($admins[$pos]);
						$this->say($recipient, $nick . ", Operation executed successfully.");
						break;
				}
			}
		}

		// !execute
		else if ( $this->isCommand($command, 'execute') && $this->isAdmin($host) ) {
			eval($message);
		}

		/***** Private Role Commands *****/
		// !see
		else if ( $this->isCommand($command, 'see', NO_OPERATOR) ) {
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

				if (!$this->isPlaying($person)) {
					$not_playing=$this->decode($lang["not_playing"], NO_NICK, $person);
					$this->notice($nick, $not_playing);
				} else if ( $nick == $person ) {
					$seer_see_self=$this->decode($lang["seer_see_self"], NO_NICK, $person);
					$this->say($nick, $seer_see_self);
				} else {
					// parse this person
					$person_roles = $this->getRolesNice($person, false, true, true);

					// Tell the seer who this guy is
					$seer_see = $this->decode($lang["seer_see"], NO_NICK, $person, $person_roles);
					$this->say($nick, $seer_see);

					// Add to list of seers that have seen tonight
					array_push($this->seer_seen, $nick);
				}
			}
		}

		// !kill
		else if ( $this->isCommand($command, 'kill', NO_OPERATOR) ) {
			if ($recipient == $this->channel || $this->isDay ) {
				// ignore this
			} else if ( !in_array($nick, $this->wolf) ) {
				$this->say($nick, "You are not a wolf. good thing too!");
			} else {
				$person = $this->nickAutocomplete($message);

				if (!$this->isPlaying($person)) {
					$not_playing=$this->decode($lang["not_playing"], NO_NICK, $person);
					$this->notice($nick, $not_playing);
				} else {
					// Check if the victim is a wolf
					if (in_array('wolf', $this->wolf)) {
						$this->say($nick, "You can't kill a wolf!");
					} else {
						// This person has been selected for death.
						$this->wolf_killed[$nick] = $person;

						// Send Message
						$wolf_kill = $this->decode($lang["wolf_kill"], NO_NICK, $person);
						$this->say($nick, $wolf_kill);
					}
				}
			}
		}

		// !visit
		else if ( $this->isCommand($command, 'visit', NO_OPERATOR) ) {
			if ($recipient == $this->channel) {
				// ignore this
			} else if ($this->isDay) {
				// It's daytime
				$harlot_day=$this->decode($lang["harlot_day"]);
				$this->notice($nick, $harlot_day);
			} else if ( !in_array($nick, $this->harlot) ) {
				// This person isn't the harlot
				$harlot_visit_not=$this->decode($lang["harlot_visit_not"], NO_NICK, $person);
				$this->notice($nick, $harlot_visit_not);
			} else if ( isset($this->harlot_visited[$nick])) {
				// You've already visited
				$harlot_visited=$this->decode($lang["harlot_visited"], NO_NICK, $this->harlot_visited[$nick]);
				$this->notice($nick, $harlot_visited);
			} else {
				$person = $this->nickAutocomplete($message);

				if ( !$this->isPlaying($person) ) {
					$not_playing=$this->decode($lang["not_playing"], NO_NICK, $person);
					$this->notice($nick, $not_playing);
				} else {
					// This person has been selected for death.
					$this->harlot_visited[$nick] = $person;

					if ($nick == $person) {
						// harlot is visiting themselves
						$harlot_visit_self=$this->decode($lang["harlot_visit_self"]);
						$this->notice($nick, $harlot_visit_self);
					} else {
						// Send Message
						$harlot_visit = $this->decode($lang["harlot_visit"], NO_NICK, $person);
						$this->say($nick, $harlot_visit);

						// Send Message to hostess
						$harlot_visit = $this->decode($lang["harlot_visit"], NO_NICK, $nick);
						$this->say($person, $harlot_visit);
					}

				}
			}
		}

		// !shoot
		else if ( $this->isCommand($command, 'shoot') ) {
			if ($recipient == $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ( !in_array($nick, $this->gunner) ) {
				// This person isn't the gunner
				$gunner_shoot_not=$this->decode($lang["gunner_shoot_not"]);
				$this->notice($nick, $gunner_shoot_not);
			} else if ($this->isNight) {
				// It is nighttime, you can't aim!
				$gunner_night=$this->decode($lang["gunner_night"]);
				$this->notice($nick, $gunner_night);
			} else if ( empty($this->gunner_bullets[$nick]) )  {
				// You've ran out of bullets!
				$gunner_nobullets=$this->decode($lang["gunner_nobullets"]);
				$this->say($nick, $gunner_nobullets);
			} else {
				$person = $this->nickAutocomplete($message);

				if ( !$this->isPlaying($person) ) {
					// The shot person isn't playing
					$not_playing=$this->decode($lang["not_playing"], NO_NICK, $person);
					$this->notice($nick, $not_playing);
				} else if ($nick == $person) {
					// You can't shoot yourself
					$gunner_shoot_self=$this->decode($lang["gunner_shoot_self"]);
					$this->say($recipient, $gunner_shoot_self);
				} else {
					$this->gunner_bullets[$nick]--;

					if ( in_array($nick, $this->drunk) ) {
						$event = $this->chance("gun_shot_drunk");
					} else {
						$event = $this->chance("gun_shot");
					}

					if ($event == 1) {
						// Hit!
						$gunner_shoot = $this->decode($lang["gunner_shoot"], $nick, $person);
						$this->say($recipient, $gunner_shoot);

						if ( in_array($person, $this->wolf) ) {
							$gunner_shoot_wolf = $this->decode($lang["gunner_shoot_wolf"], $nick, $person);
							$this->say($recipient, $gunner_shoot_wolf);

							$this->removePlayer($this->channel, $person);
						} else {
							if ($this->chance("manslaughter")) {
								$gunner_shoot_manslaughter = $this->decode($lang["gunner_shoot_manslaughter"], $nick, $person);
								$this->say($recipient, $gunner_shoot_manslaughter);

								$this->removePlayer($this->channel, $person);
							} else {
								$gunner_shoot_villager = $this->decode($lang["gunner_shoot_villager"], NO_NICK, $person);
								$this->say($recipient, $gunner_shoot_villager);

								array_push($this->gunner_shot, $person);
							}
						}
					} else if ($event == 2) {
						// Miss!
						$gunner_miss = $this->decode($lang["gunner_miss"], $nick);
						$this->say($recipient, $gunner_miss);
					} else if ($event == 3) {
						// Suicide!
						$gunner_suicide = $this->decode($lang["gunner_suicide"], $nick);
						$this->say($recipient, $gunner_suicide);

						$this->removePlayer($this->channel, $nick);
					}
				}
			}
		}

		// !guard
		else if ( $this->isCommand($command, 'guard', NO_OPERATOR) ) {
			if ($recipient != $user["nick"]) {
				// ignore this
			} else if ( !in_array($nick, $this->angel) ) {
				// This person isn't the angel
				$angel_guard_not=$this->decode($lang["angel_guard_not"]);
				$this->notice($nick, $angel_guard_not);
			} else if ($this->isDay) {
				// It's daytime
				$angel_day=$this->decode($lang["angel_day"]);
				$this->notice($nick, $angel_day);
			} else if ( isset($this->angel_guarded[$nick]) ) {
				// You've already guarded
				$angel_guarded=$this->decode($lang["angel_guarded"], NO_NICK, $this->angel_guarded[$nick]);
				$this->say($nick, $angel_guarded);
			} else {
				$person = $this->nickAutocomplete($message);

				if ( !$this->isPlaying($person) ) {
					// The visited guarding person isn't playing
					$not_playing=$this->decode($lang["not_playing"], NO_NICK, $person);
					$this->notice($nick, $not_playing);
				} else if ($nick == $person) {
					// Guarding self isn't allowed
					$angel_guard_self=$this->decode($lang["angel_guard_self"]);
					$this->say($nick, $angel_guard_self);
				} else {
					// This person has been selected for death.
					$this->angel_guarded[$nick] = $person;

					// Send Message
					$angel_guard = $this->decode($lang["angel_guard"], NO_NICK, $person);
					$this->say($nick, $angel_guard);

					// Send Message to protected villager
					$angel_guard_notify = $this->decode($lang["angel_guard_notify"], NO_NICK, $nick);
					$this->say($person, $angel_guard_notify);

				}
			}
		}

		// !id
		else if ( $this->isCommand($command, 'id') ) {
			if ($recipient != $user["nick"]) {
				$this->say($nick, "This feature is not available in a pm.");
			} else if ( !in_array($nick, $this->detective) ) {
				// This person isn't the detective
				$detective_id_not=$this->decode($lang["detective_id_not"]);
				$this->say($nick, $detective_id_not);
			} else if ($this->isNight) {
				// It is nighttime, you can't see anything!
				$detective_night=$this->decode($lang["detective_night"]);
				$this->say($nick, $detective_night);
			} else if ( in_array($nick, $this->detective_ided) )  {
				// You've already seen!
				$detective_ided=$this->decode($lang["detective_ided"]);
				$this->say($nick, $detective_ided);
			} else {
				$person = $this->nickAutocomplete($message);

				if ( !$this->isPlaying($person) ) {
					// The shot person isn't playing
					$not_playing=$this->decode($lang["not_playing"], NO_NICK, $person);
					$this->notice($nick, $not_playing);
				} else if ($nick == $person) {
					// You can't id yourself
					$detective_id_self=$this->decode($lang["detective_id_self"]);
					$this->say($recipient, $detective_id_self);
				} else {
					array_push($this->detective_ided, $nick);

					$roles = $this->getRolesNice($person, true, true, true);

					$detective_id=$this->decode($lang["detective_id"], NO_NICK, $person, $roles);
					$this->say($nick, $detective_id);

					if ( $this->chance("detective_revealed") ) {
						$detective_revealed=$this->decode($lang["detective_revealed"], $nick);
						foreach($this->wolf as $wolf) {
							$this->say($wolf, $detective_revealed);
						}
						foreach($this->traitor as $traitor) {
							$this->say($traitor, $detective_revealed);
						}
					}

				}
			}
		}

		// whoami (needs official locale)
		else if ( $this->isCommand($command, 'whoami', NO_OPERATOR) ) {
			if (!$this->ongoing) {
				//No game is currently running.
				$this->say($nick, $this->decode($lang["game_none"]));
			} else if ( !$this->isPlaying($nick) ) {
				$not_playing = $this->decode($lang["not_playing"], NO_NICK, $person);
				$this->notice($nick, $not_playing);
			} else {
				$this->say($nick, "You currently hold the role(s): " . $this->getRolesNice($nick, true, false, true) );
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

		// Remove player's lynch
		if (isset($this->lynch[$nick])) {
			unset($this->lynch[$nick]);
		}

		// Remove lynches on player
		foreach($this->lynch as $lyncher => $lynchee) {
			if ($lynchee == $nick) {
				unset($this->lynch[$lyncher]);
			}
		}
	}

	/**
	 * Assign player roles
	 */
	public function assignRoles() {
		global $roles, $shots_multiplier;

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
				// NOTE: This will result in an infinite loop if settings are manipulated wrong (ex: more roles than players)
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

					if ($role_name == "gunner") {
						$this->gunner_bullets[$players_sorted[$player_index]] = ceil($shots_multiplier * count($this->players) );
					}

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
				$answer=$this->listPlayers(false, $nick);
				$players_list = $this->decode($lang["players_list"], NO_NICK, $answer);
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
				$answer=$this->listPlayers(true, $nick);
				$players_list = $this->decode($lang["players_list"], NO_NICK, $answer);
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
				$answer=$this->listPlayers(false, $nick);
				$players_list = $this->decode($lang["players_list"], NO_NICK, $answer);
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
				$answer=$this->listPlayers(true, $nick);
				$players_list = $this->decode($lang["players_list"], NO_NICK, $answer);
				$this->say($nick, $players_list);
			break;

			case 'gunner': // Gunner
				// Role Assign
				$gunner_assign = $this->decode($lang["gunner_assign"], NO_NICK, $this->gunner_bullets[$nick]);
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
				$answer=$this->listPlayers(false, $nick);
				$players_list = $this->decode($lang["players_list"], NO_NICK, $answer);
				$this->say($nick, $players_list);
			break;

			case 'detective': // Detective
				// Role Assign
				$detective_assign = $this->decode($lang["detective_assign"]);
				$this->say($nick, $detective_assign);

				$detective_assign2 = $this->decode($lang["detective_assign2"]);
				$this->say($nick, $detective_assign2);

				$detective_assign3 = $this->decode($lang["detective_assign3"]);
				$this->say($nick, $detective_assign3);
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
		$day_turn = $this->decode($lang["day_turn"], NO_NICK, $this->formatSeconds($lasted_time));
		$this->say($this->channel, $day_turn);

		if ($victim == null) {
			$day_nokill = $this->decode($lang["day_nokill"]);
			$this->say($this->channel, $day_nokill);

		// If the harlot is selected, and he/she is not home...
		} else if ( isset($this->harlot_visited[$victim]) && $this->harlot_visited[$victim] != $victim && in_array($victim, $this->harlot) ) {
			$day_harlot = $this->decode($lang["day_harlot"]);
			$this->say($this->channel, $day_harlot);

		// Victim was protected by GA
		} else if ( in_array($victim, $this->angel_guarded) ) {
			$day_angel = $this->decode($lang["day_angel"], NO_NICK, $victim);
			$this->say($this->channel, $day_angel);

		} else {
			$dead_guy_roles = $this->getRolesNice($victim, false, false, false);

			$day_victim = $this->decode($lang["day_victim"], NO_NICK, $victim, $dead_guy_roles);
			$this->say($this->channel, $day_victim);
			$this->removePlayer($this->channel, $victim);
		}

		// If the harlot visits a wolf!
		foreach($this->harlot as $harlot) {
			if ( isset($this->harlot_visited[$harlot]) && in_array($this->harlot_visited[$harlot], $this->wolf) ) {
				$day_harlot_dead = $this->decode($lang["day_harlot_dead"], NO_NICK, $harlot);
				$this->say($this->channel, $day_harlot_dead);
				$this->removePlayer($this->channel, $harlot);
			}
		}

		// If the GA guards a wolf!
		foreach($this->angel as $angel) {
			// 50% chance of death
			if ( $this->chance('guardian_angel_dies') ) {
				if ( isset($this->angel_guarded[$angel]) && in_array($this->angel_guarded[$angel], $this->wolf) ) {
					$day_angel_dead = $this->decode($lang["day_angel_dead"], NO_NICK, $angel);
					$this->say($this->channel, $day_angel_dead);
					$this->removePlayer($this->channel, $angel);
				}
			}
		}

		// Night has ended, we can reset the night actions
		$this->seer_seen = array();
		$this->wolf_killed = array();
		$this->harlot_visited = array();
		$this->angel_guarded = array();

		// Check if game has finished
		if (count($this->wolf) == 0 && count($this->traitor) == 0) {
			// All evildoers are dead
			$this->endGame($this->channel, 1);
		} else if (count($this->players) / 2 <= count($this->wolf) + count($this->traitor) ) {
			// Wolves equal villagers
			$this->endGame($this->channel, 2);
		} else {
			$amount = count($this->players);

			// If the number is odd, subtract 1
			if ($amount % 2 != 0) {
				$amount--;
			}
			// Lynch message, and how many required to kill a victim.
			$day_vote = $this->decode($lang["day_vote"], NO_NICK, $amount / 2 + 1);
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

		// Reset goat
		$this->goat_switch = true;

		if ($victim == null) {
			// I don't believe there's a message for no kill at night
			/*$night_nokill = $this->decode($lang["night_nokill"]);
			$this->say($this->channel, $night_nokill);*/
		} else {
			$dead_guy_roles = $this->getRolesNice($victim, false, false, false);

			$night_victim = $this->decode($lang["night_victim"], NO_NICK, $victim, $dead_guy_roles);
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
			$night_turn = $this->decode($lang["night_turn"], NO_NICK, $this->formatSeconds($lasted_time));
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
		$end_times = $this->decode($lang["end_times"], NO_NICK, $this->formatSeconds($total), $this->formatSeconds($this->day_time), $this->formatSeconds($this->night_time));
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

		$this->wait_extended = 0;

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
		$this->gunner_bullets = array();
		$this->angel_guarded = array();
		$this->detective_ided = array();

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
				// If the time limit has run out
				if ($lasted_time >= $length["night"]) {
					$first_kill = reset($this->wolf_killed);
					if ( !empty($first_kill) ) {
						$this->turnDay($lasted_time, $first_kill);
					} else {
						$this->turnDay($lasted_time);
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
								if (count($this->angel_guarded) == count($this->angel)) {
									$i_condition++;
								}
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
				// If the time limit has run out
				if ($lasted_time >= $length["day"]) {
					$this->turnNight($lasted_time);
				} else {

					foreach($this->lynch as $lyncher => $lynchee) {
						if (isset($$lynchee)) {
							$$lynchee = $$lynchee +1;
						} else {
							$$lynchee = 1;
						}
					}

					// Lynch message, and how many required to kill a victim.
					$amount = floor(count($this->players) / 2) + 1 - count($this->gunner_shot);
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
	 * @param Socket $socket Socket of IRC Class
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
					$nick = $data["3"];
				case 'QUIT': // Alias PART
				case 'PART':
					// Remove from peoples list
					$key = array_search($nick, $this->people);
					unset($this->people[$key]);

					$recipient = $data["2"];

					// Quit update for game
					if (isset($this->players[$nick])) {
						$this->commands($recipient, "!quit", $nick, $host);
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

					foreach($this->role_names as $role) {
						foreach($this->$role as $key => &$person) {
							if ($nick == $person) {
								$this->{$role}[$key] = $new_nick;
							}
						}
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