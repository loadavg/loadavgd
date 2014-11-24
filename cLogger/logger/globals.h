#ifndef LOADAVG_GLOBALS
#define LOADAVG_GLOBALS
/*
 * log file logger
 * logs data to a log file
 *
 */

//for basename
//#include <libgen.h>

#include <unistd.h>

/*
 * main LoadAvg class
 *
 */

/* Find out where are we on the server*/

/*
$script_path = realpath(basename(getenv("SCRIPT_NAME")));
$slash = explode('/', getenv("SCRIPT_NAME"));
$current_filename = $slash[count($slash) - 1]; 
$host_url = str_replace($current_filename, "", getenv("SCRIPT_NAME"));
$ROOT_PATH = dirname ($host_url);

//add trailing slash
if ( $ROOT_PATH != "/") $ROOT_PATH = $ROOT_PATH . "/";
*/

/* Set script version */
string loadavg_version = "2.0";

/* Set Application Globals */

string SCRIPT_VERSION = loadavg_version;
string SCRIPT_ROOT = get_current_dir_name();        //ROOT_PATH
string HOME_PATH = get_current_dir_name();          //realpath(dirname(__FILE__)
string APP_PATH = SCRIPT_ROOT + "/app";             //realpath(dirname(__FILE__) . '/app'
string LOG_PATH = SCRIPT_ROOT + "/logs";             //realpath(dirname(__FILE__) . '/app'

#endif
