#ifndef LOADAVG_H
#define LOADAVG_H
/*
 * log file parser
 * grabs a loadavg log file and returns it as a vector
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
    void saveVector (std::vector <std::string> &, ofstream &);

    /**
     *   explode string into vector
     **/    
    vector<string> explode(std::string const &, char);
    /**
     *   parse 1 line from log file
     **/
    string Parser(std::istream&);
    /**
     *   process log file
     **/    
    bool processLog (void); 
    bool copyLog (string);

    void printLoadAvg(void);
    bool getLoadAvg(vector<double> &); 

    double difftime(clock_t ,clock_t );

    void sleep(int);

};

#endif
