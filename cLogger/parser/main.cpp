/*
 * log file parser
 * grabs a loadavg log file and returns it as a vector
 *
 */

#include "parser.h"



/*
 * main function
 *
 */

// usage
// parser logs/disk_usage_2014-11-17.log


int main ( int argc, char *argv[] )
{

  LoadAvg loadavg;

  string inputFileName,outputFileName;

  bool status;

  if ( argc != 2 ) // argc should be 2 for correct execution
  {
    // We print argv[0] assuming it is the program name
    cout<<"usage: "<< argv[0] <<" <filename>\n";
    return 0;
  }
  else 
  {    
    inputFileName = argv[1];
  }

  if ( argv[2]) // if output file is specified
  {    
    outputFileName = argv[2];
  }

  cout << "-----------------------------" << endl;
  cout << "- starting log file parsing -" << endl;
  cout << "-----------------------------" << endl;
  cout << endl;

  //set output file
  outputFileName = "conf/copylog.log";

  //begin timers
  time_t beginTime, endTime; 

  time(&beginTime);

  cout << "Start time : " << beginTime << endl;


  //vector<double> theLoad;
  //loadavg.getLoadAvg(theLoad); 
  //cout << "Load : ";
  //loadavg.printVector(theLoad); 
  //cout << endl;
  //loadavg.printLoadAvg();


  //set the config file name here
  loadavg.setLogfile(inputFileName.c_str());

  //process the file
  cout << endl;

  status = loadavg.processLog(); 

  //see if it worked
  cout << endl;

  if (status)
      cout << "Parse successful" << endl;
  else
      cout << "Parse failed" << endl;


  cout << "-----------------------------" << endl;

  cout << "copying file" << endl;

  //copy the file - should do this from array
  status = loadavg.copyLog(outputFileName); 



  time(&endTime);
  cout << "End time : " << endTime << endl;
  double timeSpan = loadavg.difftime(endTime,beginTime);
  cout << "Time elapsed: " << timeSpan << " ms" << endl;

  cout << "-----------------------------" << endl;

}







