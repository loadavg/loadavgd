<?php
/**
* LoadAvg - Server Monitoring & Analytics
* http://www.loadavg.com
*
* Main controller class for LoadAvg 2.0
*
* @link https://github.com/loadavg/loadavg
* @author Karsten Becker
* @copyright 2014 Sputnik7
*
* This file is licensed under the Affero General Public License version 3 or
* later.
*/

class LoadAvg
{
	public static $_settings; // storing standard settings and/or loaded modules settings
	public static $_classes; // storing loaded modules classes
	public static $_modules; // storing loaded modules
	public static $current_date; // current date
	private static $_timezones; // Cache of timezones

	// Periodas
	public static $period;
	public static $period_minDate;
	public static $period_maxDate;
	public static $settings_ini;
	public $timeStart = null, $timeFinish = null;


	/**
	 * setSettings
	 *
	 * Stores the standard settings
	 *
	 * @param string $module name of the module
	 * @param array $args array of module settings
	 */
	public function setSettings($module, $args)
	{
		@self::$_settings->$module = $args;
	}

	/**
	 * __construct
	 *
	 * Class constructor
	 *
	 */

	public function __construct()
	{

		date_default_timezone_set("UTC");
		self::$settings_ini = "settings.ini.php";

		$this->setSettings('general',
			parse_ini_file(APP_PATH . '/config/' . self::$settings_ini, true)
		);

		date_default_timezone_set(self::$_settings->general['timezone']);

		self::$current_date = (isset($_GET['logdate']) && !empty($_GET['logdate'])) ? $_GET['logdate'] : date("Y-m-d");

		foreach ( self::$_settings->general['modules'] as $key => &$value ) {
			if ( $value == "true" ) {
				try {
					require_once $key . DIRECTORY_SEPARATOR . 'class.' . $key . '.php';
					self::$_classes[$key] = new $key;
				} catch (Exception $e) {
					throw Exception( $e->getMessage() );
				}
			}
		}

		if (is_dir(HOME_PATH . '/lib/modules/')) {

			foreach (glob(HOME_PATH . "/lib/modules/*/class.*.php") as $filename) {
				$filename = explode(".", basename($filename));
				self::$_modules[$filename[1]] = strtolower($filename[1]);
			}
		}

	}

	/**
	 * is_dir_empty
	 *
	 * Checks if specified directory is empty or not.
	 *
	 * @param string $dir path to directory
	 */

	private function is_dir_empty($dir) {
		if (!is_readable($dir)) return NULL;
		return (count(scandir($dir)) == 2);
	}

	/**
	 * createFirstLogs
	 *
	 * Creates first log files for every loaded modules 
	 * only run once - after installation - and may not be needed as modules will do this themselves
	 * if the file isnt there they create it
	 *
	 */

	public function createFirstLogs()
	{

		//only does it if DIR is empty ?
		if ( $this->is_dir_empty(HOME_PATH . '/' . self::$_settings->general['logs_dir']) ) {

			$loaded = self::$_settings->general['modules'];
			$logdir = HOME_PATH . '/' . self::$_settings->general['logs_dir'];

			$test_nested = false;

			// Check for each module we have loaded
			foreach ( $loaded as $module => $value ) {
				if ( $value == "false" ) continue;

				$moduleSettings = self::$_settings->$module;

				// Check if loaded module needs loggable capabilities
				if ( $moduleSettings['module']['logable'] == "true" ) {
					foreach ( $moduleSettings['logging']['args'] as $args) {

						$args = json_decode($args);
						$class = self::$_classes[$module];
						
						$caller = $args->function;

						//skip network interfaces as they have nested logs and work differently
						//later need to skip all other nested logs as we check those below
						
						if ( $args->logfile == "network_%s_%s.log" )
						{
							$test_nested = true;
						}
						else
						{
							$caller = sprintf($args->function, sprintf("'". $args->logfile . "'", date('Y-m-d')));
							$caller = $args->function;
							
							//dont work for network ?
							$class->logfile = $logdir . sprintf($args->logfile, date('Y-m-d'));
							$class->logfile = $logdir . $args->logfile;

							$class->$caller();	
						}
					}
				}

				//network interface is off at install time so dont really matter at this point

				if ($test_nested == true) {

					//now do nested charts 
					foreach (LoadAvg::$_settings->general['network_interface'] as $interface => $value) {
																					
								$caller = sprintf($args->function, sprintf("'". $args->logfile . "'", date('Y-m-d') , $interface  ));
								$caller = $args->function;
								
								//dont work for network ?
								$class->logfile = $logdir . sprintf($args->logfile, date('Y-m-d') , $interface );
								$class->logfile = $logdir . $args->logfile;

								$class->$caller();	
					}
				}
			}
		}
	}

	/*
	 * used when we turn modules on and off
	 * this needs to only build the log file for modules that have no log file in /logs
	 * also be great to pass the module over if we know 
	 * what module has changed or been enabled
	 */

	public static function rebuildLogs()
	{

			echo "Rebuild Logs  \n";

			$loaded = self::$_settings->general['modules'];

			$logdir = HOME_PATH . '/' . self::$_settings->general['logs_dir'];

			// Check for each module we have loaded
			foreach ( $loaded as $module => $value ) {
				if ( $value == "false" ) continue;

				$moduleSettings = self::$_settings->$module;

				// Check if loaded module needs loggable capabilities
				if ( $moduleSettings['module']['logable'] == "true" ) {
					foreach ( $moduleSettings['logging']['args'] as $args) {
						$args = json_decode($args);
						$class = self::$_classes[$module];

						$caller = $args->function;

						$class->logfile = $logdir . $args->logfile;

						//what does this do ? run args function ?
						$class->$caller();
					}
				}
			}

	}


	/*
	 * used to test if log files are being created by logger
	 * needs better testing currently a bit of a hack
	 * as we just test if the log directory is empty or not
	 */

 	function testLogs( $mode = true)
	{

			$loaded = self::$_settings->general['modules'];
			$logdir = HOME_PATH . '/' . self::$_settings->general['logs_dir'];

			$test_worked = false;
			$test_nested = false;

			if ( $this->is_dir_empty($logdir))
				return false;

			// Check for each module we have loaded
			foreach ( $loaded as $module => $value ) {
				if ( $value == "false" ) continue;

				$moduleSettings = self::$_settings->$module;

				// Check if loaded module needs loggable capabilities
				if ( $moduleSettings['module']['logable'] == "true" ) {
					
					foreach ( $moduleSettings['logging']['args'] as $args) {
						
						$args = json_decode($args);
						$class = self::$_classes[$module];
						
						$caller = $args->function;

						//skip network interfaces as they have nested logs and work differently
						//later need to skip all nested logs as we check those below
						
						if ( $args->logfile == "network_%s_%s.log" )
						{
							$test_nested = true;
						}
						else
						{
							$filename = ( $logdir . sprintf($args->logfile, date('Y-m-d')) );

							if (file_exists($filename)) {
						    	$test_worked = true;
							}

							if ($mode == true)
								echo "Log: $filename Status: $test_worked  \n";		
						}
					}
				}
			}

			if ($test_nested == true) {

				//now do nested charts 
				foreach (LoadAvg::$_settings->general['network_interface'] as $interface => $value) {

					if (  !( isset(LoadAvg::$_settings->general['network_interface'][$interface]) 
						&& LoadAvg::$_settings->general['network_interface'][$interface] == "true" ) )
						continue;

					$filename = ( $logdir . sprintf($args->logfile, date('Y-m-d') , $interface ) );
																				
					if (file_exists($filename)) {
				    	$test_worked = true;
					}

					if ($mode == true)
						echo "Log: $filename Status: $test_worked  \n";
				}
			}

			return $test_worked;
	}


	


	/**
	 * checkWritePermissions
	 *
	 * Checks if specified file has write permissions.
	 *
	 * @param string $file path to file
	 */

	public function checkWritePermissions( $file )
	{
		if ( is_writable( $file ) )
			return true;
		else
			return false;
	}




	/**
	 * parseInfo
	 *
	 * Parses ini file data for a module into lines of text for legend display
	 *
	 * @param array $info array with info lines from the classes INI file
	 * @param array $variables variables to format lines
	 * @param string $class class name of module
	 * @return array $return formatted info lines
	 */

	public function parseInfo( $info, $variables, $class )
	{
		$return = array();
		foreach ( $info as $line ) {
			$line = json_decode($line);

			if (isset($line->type) && isset($line->filename)) {
				$return['info']['line'][] = array("type" => "file", "file" => 'modules' . DIRECTORY_SEPARATOR . $class . DIRECTORY_SEPARATOR . $line->filename);
				continue;
			}

			if ( strstr($line->args, "|")) {
				$lineArgs = explode("|", $line->args);
				$args = array();
				foreach ($lineArgs as $arg) {
					$args[] = $variables[$arg];
				}
				$line = vsprintf($line->format, $args);
			} else {
				$line = sprintf($line->format, $variables[$line->args]);
			}
			$return['info']['line'][] = array( "type" => "line", "formatted_line" => $line );
		}

		return $return;
	}



	/**
	 * safefilewrite
	 *
	 * Writes data to INI file and locks the file
	 *
	 * @param string $fileName filename
	 * @param array $dataToSave data to save to file
	 */

	public function safefilerewrite($fileName, $dataToSave, $mode = "w", $logs = false )
	{    

		//if file is new and is a logfile then we need to make it chmod 777
		//or we have issues between flies create using app and ones using cron
		//cron gives root permissions and app gives appache permissions
		$exists = file_exists ( $fileName );

		if ($fp = fopen($fileName, $mode))
	    {
	        $startTime = microtime();
	        do
	        {
	        	$canWrite = flock($fp, LOCK_EX);
	        	// If lock not obtained sleep for 0 - 100 milliseconds, to avoid collision and CPU load
	        	if(!$canWrite) usleep(round(rand(0, 100)*1000));
	        } while ((!$canWrite)and((microtime()-$startTime) < 1000));

	        //file was locked so now we can store information
	        if ($canWrite)
	        {
	        	fwrite($fp, $dataToSave);
	            //flock($fp, LOCK_UN);
	        }

	        fclose($fp);

	        //if its a new log file fix permissions
	        if (!$exists && $logs==true ) {
	        	//echo "fix logs";
				chmod($fileName, 0777);
			}

	        return true;
	    }
	    else
	    {
	    	return false;
	    }

	}


	/**
	 * getLoggerInterval
	 *
	 * User login, checks username and password from default settings to match.
	 *
	 * @param string $username the username
	 * @param string $password the password
	 */

	public function getLoggerInterval( ) 
	{

		$interval = LoadAvg::$_settings->general['logger_interval'];

		if  ( $interval ) {

			$interval = $interval * 60;
			return $interval;

		} else {

			return false;
		}

	}




	/**
	 * sendApiData
	 *
	 * If API is activated sends data to server
	 *
	 * @param array $data array of data to send to the server
	 * @return string $result message returned from the server
	 */

	public function sendApiData( $data ) {

		// for debugging
		//var_dump($data); //exit;
		//echo 'DEBUG: ' .  json_encode($data);

		$url = self::$_settings->general['api']['url'];

		$user_url = $url . '/users/';
		$server_url = $url . '/servers/';

		//validate API access here
		if ( self::$_settings->general['api']['server_token'] && self::$_settings->general['api']['key'] ) {		
		$ch =  curl_init($server_url . self::$_settings->general['api']['server_token'] . '/' . self::$_settings->general['api']['key'] . '/v');
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		$account_valid = curl_exec($ch);
		} else
			$account_valid = 'false';

		//get server id from server token
		if ( self::$_settings->general['api']['server_token'] ) {			
		$ch =  curl_init($server_url . self::$_settings->general['api']['server_token'] . '/t');
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		$server_exists = curl_exec($ch);
		} else
			$server_exists = 'false';

		//echo $server_url.json_decode($server_exists)->id.'/data';

		//validation needs to happen on the sever this is still insecure!
		//need to pass api token and server token over in data push

		if( $server_exists != 'false' && $account_valid != 'false' ) 
		{

			//file_put_contents("file.txt", json_encode($data)); test data
			$curl = curl_init();

			// Set some options - we are passing in a useragent too here
			curl_setopt_array($curl, array(
		    CURLOPT_RETURNTRANSFER => 1,
		    CURLOPT_URL => $server_url.json_decode($server_exists)->id.'/data',
		    CURLOPT_USERAGENT => 'LoadAvg Client',
		    CURLOPT_POST => 1,
		    CURLOPT_POSTFIELDS => array(
		      'data' => json_encode($data),
		    )
			));

			// Send the request & save response to $resp
			$resp = curl_exec($curl);

			// Close request to clear up some resources
			curl_close($curl);

			//used for debugging to file
			//file_put_contents("file.txt",$resp);
			
			return true;
		}

		return null;
	}


	/**
	 * testApiConnection
	 *
	 * Test if API connection is working
	 *
	 * @return string $result message returned from the server
	 */

	public static function testApiConnection( $echo = false ) {

		$url = self::$_settings->general['api']['url'];

		$user_url = $url . '/users/';
		$server_url = $url . '/servers/';
		
		//validate users api key
		if ( self::$_settings->general['api']['key'] ) {
			$ch =  curl_init($user_url . self::$_settings->general['api']['key'] . '/va');
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			$user_exists = curl_exec($ch);
		} else
			$user_exists = 'false';


		//val;idate server token
		if ( self::$_settings->general['api']['server_token'] ) {
			$ch =  curl_init($server_url . self::$_settings->general['api']['server_token'] . '/vs');
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			$server_exists = curl_exec($ch);
		} else
			$server_exists = 'false';		

		//validate api key against server token
		if ( self::$_settings->general['api']['server_token'] && self::$_settings->general['api']['key'] ) {
			$ch =  curl_init($server_url . self::$_settings->general['api']['server_token'] . '/' . self::$_settings->general['api']['key'] . '/v');
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			$server_valid = curl_exec($ch);
		} else
			$server_valid = 'false';

		if ($echo) {

			echo ($user_exists == 'false' ?  "API Key : INVALID \n" :  "API Key : VALID \n");

			echo ($server_exists == 'false' ?  "Server Token : INVALID \n" :  "Server Token : VALID \n");
			
			echo ($server_valid == 'false' ?  "Server Access : INVALID \n" :  "Server Access : VALID \n");

		}

		//return server valid status
		if($server_valid == 'false') 
			return false;
		else
			return true;
	}




		


	/**
	 * getTime
	 *
	 * Sets startTime of page load
	 *
	 */

	public function getTime()
	{
		$time = microtime();
		$time = explode(' ', $time);
		$time = $time[1] + $time[0];
		return $time;
	}

	/**
	 * setStartTime
	 *
	 * Sets startTime of page load
	 *
	 */

	public function setStartTime()
	{
		$time = microtime();
		$time = explode(' ', $time);
		$time = $time[1] + $time[0];
		$this->timeStart = $time;
	}

	/**
	 * setFinishTime
	 *
	 * Sets finish time of page load
	 *
	 */

	public function setFinishTime()
	{
		$time = microtime();
		$time = explode(' ', $time);
		$time = $time[1] + $time[0];
		$this->timeFinish = $time;
	}

	/**
	 * getPageLoadTime
	 *
	 * Returns page load time
	 *
	 * @return string $time page load time
	 */

	public function getPageLoadTime()
	{
		return round( ( $this->timeFinish - $this->timeStart ), 4 );
	}

	/**
	 * getDates
	 *
	 * Gets date range from logfiles to populate the select box from topbar
	 * NOTE: Was changed to static
	 *
	 * @return array $return array with list of dates
	 */

	//grabs dates of ALL log files to be safe but would be faster if it did just one module

	public static function getDates()
	{
		$dates = array();

		foreach ( glob(dirname(__FILE__) . "/../logs/*.log") as $file ) {
			preg_match("/([0-9-]+)/", basename($file), $output_array);
		
			if ( isset( $output_array[0] ) && !empty( $output_array[0] ) )
				$dates[] = $output_array[0];
		}

 		//get rid of all duplicate dates
		$dates = array_unique($dates);

		//need to properly sort the array before returning it
		asort ($dates);

		return $dates;
	}

	/**
	 * logUpdateCheck
	 *
	 * Logs to file every check for update(s).
	 *
	 */

	public function logUpdateCheck( $response )
	{
		$fh = fopen(dirname(__FILE__) . '/../logs/update.log', 'a+');
		$logLine = "Update check at " . date("Y-m-d H:i:s a") . " ---- Response: " . $response . PHP_EOL;
		if ( $fh ) {
			fwrite($fh, $logLine);
			fclose($fh);
		}

	}




}
