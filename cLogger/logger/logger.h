#ifndef LOADAVG_H
#define LOADAVG_H
/*
 * log file logger
 * logs data to a log file
 *
 */

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ostream>

#include <stdlib.h>
#include <time.h>

#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

#include "globals.h"
/*
 * main LoadAvg class
 *
 */

class LoadAvg {

  private:
    /**
     *   name of log fiel to process
     **/
    std::string logFile;

  public:
    /**
     *   default constructor
     **/
    LoadAvg ();
    /**
     *   constructor with log file name
     **/
    LoadAvg (std::string);
    /**
     *   set log file name.
     **/
    void setLogfile(std::string);
    /**
     *   get log file name.
     **/    
    std::string getLogfile(void);
    /**
     *   print out a vecotr.
     **/
    void printVector (std::vector <std::string> &); 
    void printVector (std::vector <double> &); 
    void saveVector (std::vector <double> &, ofstream &);
   

    void printLoadAvg(void);
    bool getLoadAvg(vector<double> &); 

    bool updateLogfile (time_t,vector<double>& );

    double difftime(clock_t ,clock_t );

    void sleep(int);

};

#endif
