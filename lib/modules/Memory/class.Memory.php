<?php
/**
* LoadAvg - Server Monitoring & Analytics
* http://www.loadavg.com
*
* Memory Module for LoadAvg
* 
* @version SVN: $Id$
* @link https://github.com/loadavg/loadavg
* @author Karsten Becker
* @copyright 2014 Sputnik7
*
* This file is licensed under the Affero General Public License version 3 or
* later.
*/

class Memory extends LoadAvg
{
	public $logfile; // Stores the logfile name & path

	/**
	 * __construct
	 *
	 * Class constructor, appends Module settings to default settings
	 *
	 */
	public function __construct()
	{
		$this->setSettings(__CLASS__, parse_ini_file(strtolower(__CLASS__) . '.ini', true));
	}

	/**
	 * logMemoryUsageData
	 *
	 * Retrives data and logs it to file
	 *
	 * @param string $type type of logging default set to normal but it can be API too.
	 * @return string $string if type is API returns data as string
	 *
	 */

	public function logData( $type = false )
	{
		$class = __CLASS__;
		$settings = LoadAvg::$_settings->$class;

		exec("free -o | grep Mem | awk -F' ' '{print $3}'", $memory);
		//exec("free -o | grep Mem | awk -F' ' '{print $3 - $6 - $7}'", $memory);

		$memory = implode(chr(26), $memory);

		//exec("free -o | grep Mem | awk -F' ' '{print $3}'", $swap);
		exec("free -o | grep Swap | awk -F' ' '{print $3}'", $swap);

		$swap = implode(chr(26), $swap);

		exec("free -o | grep Mem | awk -F' ' '{print $2}'", $totalmemory);
		//exec("free -o | grep Mem | awk -F' ' '{print $3 - $6 - $7}'", $memory);

		$totalmemory = implode(chr(26), $totalmemory);

	    $string = time() . '|' . $memory . '|' . $swap . '|' . $totalmemory . "\n";

		if ( $type == "api") {
			return $string;
		} else {
			$filename = sprintf($this->logfile, date('Y-m-d'));
			$this->safefilerewrite($filename,$string,"a",true);
		}
	}


}
