<?php
class TokenBucket {
	/* An implementation of the token bucket algorithm.
	 * bucket = TokenBucket(80, 0.5)
	 * bucket.consume(1) */

	private $capacity;
	private $tokens;
	private $fill_rate;
	private $timestamp;

	public function __construct($tokens, $fill_rate) {
		/* Tokens is the total tokens in the bucket. fill_rate is the
		 * rate in tokens/second that the bucket will be refilled. */
		$this->capacity = $tokens;
		$this->tokens = $tokens;
		$this->fill_rate = $fill_rate;
		$this->timestamp = time();
	}

	public function consume($tokens) {
		/* Consume tokens from the bucket. Returns True if there were
		 * sufficient tokens, otherwise False. */
		if ( $tokens <= $this->tokens() ) {
			$this->tokens -= $tokens;
			return true;
		}
		return false;
	}

	public function tokens() {
		$now = time();
		if ($this->tokens < $this->capacity) {
			$delta = $this->fill_rate * ($now - $this->timestamp);
			$this->tokens = min($this->capacity, $this->tokens + $delta);
		}
		$this->timestamp = $now;
		return $this->tokens;
	}
}

?>