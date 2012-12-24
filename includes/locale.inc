<?php
class locale {
	private $directory;
	private $extension;

	private $languages;

	private $langauge;

	/**
	 * Constructor for language handling
	 * 
	 * @param mixed $directory Folder languages files are located
	 * @param mixed $extension File extension of locale files
	 */
	public function __construct($directory, $extension) {
		$this->languages = array();

		$this->setDirectory($directory);
		$this->setFileExtension($extension);

		$this->cullLangs();
	}

	/**
	 * Set the directory of the locale files
	 * 
	 * @param mixed $directory Directory/Folder
	 */
	public function setDirectory($directory) {
		$this->directory = $directory;
	}

	/**
	 * Get the directory of the locale files
	 * 
	 * @return mixed Directory/Folder
	 */
	public function getDirectory() {
		return $this->directory;
	}

	/**
	 * Set the file extension of the locale files
	 * 
	 * @param mixed $extension File extension
	 */
	public function setFileExtension($extension) {
		$this->extension = $extension;
	}

	/**
	 * Get the file extension of the locale files
	 *
	 * @return mixed File extension
	 */
	public function getFileExtension() {
		return $this->extension;
	}

	/**
	 * Cull or compile the langs together
	 */
	public function cullLangs() {
		$directory = $this->directory;
		$extension = $this->extension;

		// Search for files in the directory to use as languages
		foreach(scandir($directory) as $lang) {
			// Include the file if the language is a file and ends in ".inc";
			if ( is_file($directory . '/' . $lang) && preg_match("/^.*" . $extension . "$/i", $lang) == 1 ) {

				include($directory . '/' . $lang);

				// Cut out the extension
				$language = substr($lang, 0, strlen($lang) - strlen($extension));

				// Add it to stack of loaded languages
				array_push($this->languages, $language);
			}
		}
	}

	/**
	 * Synonym for locale::cullLangs()
	 */
	public function scanLangs() {
		$this->cullLangs();
	}

	/**
	 * Set the current language
	 * 
	 * @param mixed $language The language we will use for this session
	 */
	public function setLanguage($language) {
		$this->language = $language;
	}

	/**
	 * Get the current language
	 *
	 * @return string The language used for the session
	 */
	public function getLanguage() {
		return $this->language;
	}

	/**
	 * Decode a locale string
	 * 
	 * @param mixed $string Locale string
	 * @return string String with bolded words
	 */
	public function decode($string) {
		$return = str_replace("<bold>", chr(2), $string);
		$return = str_replace("</bold>", chr(2), $return);

		return $return;
	}

	/**
	 * Get a the string of the current language
	 *
	 * @param mixed $langauge The language of the string
	 */
	public function getString($string) {
		return $this->$language[$string];
	}

	/**
	 * Get a desired string with replaced/subsituted values
	 * 
	 * @param mixed $string Locale string
	 * @param mixed $sub1 Answer 1
	 * @param mixed $sub2 Answer 2
	 * @param mixed $sub3 Answer 3
	 * @param mixed $sub4 Answer 4
	 * @return string String with bolded words and correct answer values
	 */
	public function getSubbedString($string, $sub1, $sub2=null, $sub3=null, $sub4=null) {
		$return = $this->getString($string);
		$return = $this->decode($return);

		if (isset($sub1))
			$return = str_replace("%answer1%", $sub1, $return);
		if (isset($sub2))
			$return = str_replace("%answer2%", $sub2, $return);
		if (isset($sub3))
			$return = str_replace("%answer3%", $sub3, $return);
		if (isset($sub4))
			$return = str_replace("%answer4%", $sub4, $return);

		return $return;
	}
}
?>