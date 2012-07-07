<?php
function SplitByEveryNthOccurrence($string, $needle, $nth, $array = array()) {
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
	return $array;

}
?>
