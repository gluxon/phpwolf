<?php
class random {
	
	/**
	 * Get a random integer (only pos num)
	 * 
	 * @param mixed $min Minimium number
	 * @param mixed $max Maximum number
	 * @return mixed Random integer
	 */
	public function getRandInt($min, $max) {
		$random = fopen("http://www.random.org/integers/?num=1&col=1&base=10&format=plain&rnd=new&min=$min&max=$max", 'r');
		if ($random != false) {
			$int = fread($random, strlen($max));
			fclose($random);
		} else {
			$int = rand($min, $max);
		}
		return $int;
	}
}
?>