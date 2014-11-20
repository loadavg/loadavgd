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
		$this->setSettings(__CLASS__, parse_ini_file(strtolower(__CLASS__) . '.ini.php', true));
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

		$timestamp = time();

		/* 
			need to optimize by grab this data directly from /proc/meminfo in a single call
			egrep --color 'Mem|Cache|Swap' /proc/meminfo
		*/
		
		exec( "egrep 'MemTotal|MemFree|SwapTotal|SwapFree' /proc/meminfo | awk -F' ' '{print $2}'", $sysmemory );

		$totalmemory = $sysmemory[0];
		$freememory = $sysmemory[1];
		$memory = $totalmemory - $freememory;

		$totalswap = $sysmemory[2];
		$freeswap = $sysmemory[3];
		$swap = $totalswap - $freeswap;


		/*
		old way of getting memory usage

		$memory = exec("free -o | grep Mem | awk -F' ' '{print $3}'");
		$totalmemory = exec("free -o | grep Mem | awk -F' ' '{print $2}'");
		$swap = exec("free -o | grep Swap | awk -F' ' '{print $3}'");
		*/
	    
	    $string = $timestamp . '|' . $memory . '|' . $swap . '|' . $totalmemory . "\n";

	    //echo 'DATA:'  . $string .  "\n" ;

		$filename = sprintf($this->logfile, date('Y-m-d'));
		$this->safefilerewrite($filename,$string,"a",true);

		if ( $type == "api")
			return $string;
		else
			return true;
	}

}
