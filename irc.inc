<?php
class irc {

	protected $socket;

	/**
	 * Get the socket variable (access function)
	 *
	 * @return mixed socket variable
	 */
	public function getSocket() {
		return $this->socket;
	}

	/**
	 * Set the socket variable (access function)
	 *
	 * @param mixed $socket socket variable
	 */
	public function setSocket($socket) {
		$this->socket = $socket;
	}

	/**
	 * Connects to an IRC server
	 *
	 * @param mixed $address Address of server
	 * @param mixed $port Port of server
	 */
	public function connect($address, $port) {
		$this->socket = fsockopen($address, $port);
	}

	/**
	 * Sends raw data
	 *
	 * @param mixed $data Data to send
	 */
	public function sendData($data) {
		fwrite($this->socket, $data . "\n");
	}

	/**
	 * Sends a message to a room or user
	 *
	 * @param mixed $recipient An IRC room or user
	 * @param mixed $message The message to be sent (requires a licked stamp!)
	 */
	public function say($recipient, $message) {
		$this->sendData("PRIVMSG $recipient :$message");
	}

	/**
	 * Sends password to NickServ
	 *
	 * @param mixed $password Password
	 */
	public function sendPassword($password) {
		if (!empty($password)) {
			$this->sendData("PASS $password");
		}
	}

	/**
	 * Sends the irc ident
	 *
	 * @param mixed $nick Nick
	 * @param mixed $mode Mode
	 * @param mixed $unused This is unused
	 * @param mixed $realname Realname
	 */
	public function sendIdent($nick, $mode, $unused, $realname) {
		$this->sendData("NICK $nick");
		// USER <user> <mode> <unused> <realname> (RFC 2812)
		$this->sendData("USER $nick $mode $unused :$realname");
	}

	/**
	 * Joins the specified IRC channel
	 *
	 * @param mixed $channel Channel
	 */
	public function joinChannel($channel) {
		$this->sendData("JOIN $channel");
	}

	/**
	 * Parts the specified IRC channel
	 *
	 * @param mixed $channel Channel
	 */
	public function partChannel($channel) {
		$this->sendData("PART $channel");
	}

	/**
	 * Retrieves Data (essentially fgets)
	 *
	 * @param mixed $length Max length
	 * @return mixed Raw data
	 */
	public function getData($length=0) {
		if (empty($length)) {
			return fgets($this->socket);
		} else {
			return fgets($this->socket, $length);
		}
	}

	/**
	 * Explode the data into words
	 *
	 * @param mixed $data getData() raw output
	 * @return array Array of previously raw data
	 */
	public function explodeData($data) {
		return $data=explode(' ', $data);
	}

	/**
	 * Keep IRC bot alive, respond to ping events
	 *
	 * @param array $explodeData explodeData() return
	 */
	public function PingPong($explodeData) {
		if ($explodeData["0"] == "PING") {
			$this->sendData('PONG ' . $explodeData["1"]);
		}
	}

	/**
	 * Return true or false if event is occuring
	 *
	 * @param mixed $event Event Id
	 * @param $explodeData explodeData() return
	 * @return boolean True or false answer
	 */
	public function event($event, $explodeData) {
		if (!empty($explodeData["1"]) && $explodeData["1"] == $event) {
			return true;
		}
	}

}
?>