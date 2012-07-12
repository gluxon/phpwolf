<?php
function SplitByEveryNthOccurrence($string, $needle, $nth, $array = array()) {
	if (substr_count($string, ' ') <= $nth) {
		array_push($array, $string);
	} else {
		$ex = explode($needle, $string, $nth + 2);
		$cut_off = $ex[$nth+1];
		unset($ex[$nth+1]);
		$complete = implode($needle, $ex);
	
		array_push($array, $complete);
		
		if (substr_count($cut_off, ' ') >= $nth) {
			$array = SplitByEveryNthOccurrence($cut_off, $needle, $nth, $array);
		} else {
			array_push($array, $cut_off);
		}
	}
	return $array;

}
?>
