/*
 * log file parser
 * grabs a loadavg log file and returns it as a vector
 *
 */

#include "parser.h"

//these are currently GLOBALS!!
vector<string> basicArray;
vector<string> pluginArray;
vector<string> optionsArray;
vector<string> includeArray;

//need to move to this ASAP
typedef struct collectLog {

  vector<string> basicArray;
  vector<string> pluginArray;
  vector<string> optionsArray;
  vector<string> includeArray;

} collectLog_t;



LoadAvg::LoadAvg () {
  logFile = "";

}

LoadAvg::LoadAvg (string filename) {
  logFile = filename;
}


/*
 * set or get logfile to process
 *
 */
void LoadAvg::setLogfile(string filename) {
    logFile = filename;
}

string LoadAvg::getLogfile(void) {
    return logFile;
}


/*
 * process the log file
 *
 */

bool LoadAvg::processLog (void) 
{

  ifstream iFile(logFile); 

  if (iFile.fail() || !iFile.good() ) {
    cout << "File not found: " << logFile << endl;
    return false;
  }

  int i = 0;
  string Next;

  cout << "Parsing file : " << logFile << endl;

  bool keeplooking = false;

  while (!iFile.eof()) {

    // grab a line from the file
    Next = this->Parser(iFile); 

    if (!Next.empty()) {

      //skip whats turned off for now
      if ( Next.front() != '#' )
      {

        bool match = false;

        //keeplooking means we found a <Plugin
        //used to mark a dataset
        if (keeplooking == false )
        {
          size_t found = Next.find("LoadPlugin");
          if (found!=std::string::npos) {
            pluginArray.push_back(Next);  
            match = true; 
          }
          else {
            size_t found = Next.find("<Plugin");
            if (found!=std::string::npos) {
              optionsArray.push_back(Next);  
              match = true; 
              keeplooking = true;
            }
          }
        }


        // means a basic variable
        if (keeplooking == false && match == false ) {

          size_t found = Next.find("Include");
          if (found!=std::string::npos) {
            includeArray.push_back(Next);  
            match = true; 
          }


          else {
            basicArray.push_back(Next);  
            match = true; 
          }
        }

        //means keep parsing until we find a closing </Plugin>
        if (keeplooking == true && match == false )
        {
          size_t found = Next.find("</Plugin>");
          if (found!=std::string::npos) {
            optionsArray.push_back(Next);  
            keeplooking = false;
          }
          else {
            optionsArray.push_back(Next);  
          }
        }

            /*
             * code to parse basic array data sets

            auto v = this->explode(Next, ' ');
            
            //now put it back togeather!
            string s;
            s = accumulate(begin(v), end(v), s);
            basicArray.push_back(s);            
            */

            i++;
      }
    }

  }

  cout << "-----------------------------" << endl;
  cout << "Basic array : " << basicArray.size() << endl;
  cout << "-----------------------------" << endl;
  this->printVectorArray(basicArray);
  cout << endl;

  cout << "-----------------------------" << endl;
  cout << "Plugin array : " << pluginArray.size() << endl;
  cout << "-----------------------------" << endl;
  this->printVectorArray(pluginArray);
  cout << endl;

  cout << "-----------------------------" << endl;
  cout << "Options array : " << optionsArray.size() << endl;
  cout << "-----------------------------" << endl;
  this->printVectorArray(optionsArray);
  cout << endl;

  cout << "-----------------------------" << endl;
  cout << "Includes array : " << includeArray.size() << endl;
  cout << "-----------------------------" << endl;
  this->printVectorArray(includeArray);
  cout << endl;

  cout << "-----------------------------" << endl;
  cout << "Total lines : " << i << endl;
  cout << "-----------------------------" << endl;

  iFile.close();

  if ( i > 0)
    return true;
  else
    return false;
}




/*
 * process the log file and copy to new location
 *
 */

bool LoadAvg::copyLog (string outputFile) 
{


  //grab the output file
  //
  ofstream oFile(outputFile); 

  if (oFile.fail() || !oFile.good() ) {
    cout << "File not found: " << logFile << endl;
    return false;
  } else {
    cout << "Output file : " << outputFile << endl;    
  }


  cout << "Copying data to file: ";
  cout << endl;

  this->saveHeader("Writing out the config file",oFile);

  this->saveHeader("Writing out the core settings",oFile);
  this->saveVector(basicArray,oFile);

  this->saveHeader("Writing out the plugins",oFile);
  this->saveVector(pluginArray,oFile);

  this->saveHeader("Writing out the plugin settings",oFile);
  this->saveVector(optionsArray,oFile);
 
   this->saveHeader("Writing out additional includes",oFile);
  this->saveVector(includeArray,oFile);

  cout << "Copyed data to file: ";
  cout << endl;

  oFile.close();

  return true;
  
}


/*
 * reads in a line from the log file 
 *
 */

string LoadAvg::Parser(istream& In) 
{
  
  string nextLine;
  getline(In, nextLine, '\n'); // eat a line
  
  while ( In && ( nextLine.length() == 0 ) ){ // don't accept an empty one
    getline(In, nextLine, '\n');
  }
  
  return nextLine;
}

/*
 * reads in a line from the log file 
 *
 */

void LoadAvg::resetParser( ) 
{
  
  basicArray.clear();
  pluginArray.clear();
  optionsArray.clear();
  includeArray.clear();

}








/*
 * runs the main parser to parse and clean log files
 *
 */

bool LoadAvg::runParser (string inputFile, string outputFile) 
{

  bool status;
 
  cout << "-----------------------------" << endl;
  cout << "- starting log file parsing -" << endl;
  cout << "-----------------------------" << endl;
  cout << endl;

  //begin timers
  time_t beginTime, endTime; 

  time(&beginTime);

  cout << "Start time : " << beginTime << endl;

  //set the config file name here
  this->setLogfile(inputFile);

  //process the file
  cout << endl;

  status = this->processLog(); 

  //see if it worked
  cout << endl;

  if (status)
      cout << "Parse successful" << endl;
  else
      cout << "Parse failed" << endl;


  cout << "-----------------------------" << endl;

  cout << "Copying new settings to file" << endl;

  //copy the file - should do this from array
  status = this->copyLog(outputFile); 



  time(&endTime);
  cout << "End time : " << endTime << endl;
  double timeSpan = this->difftime(endTime,beginTime);
  cout << "Time elapsed: " << timeSpan << " ms" << endl;

  cout << "-----------------------------" << endl;


  return true;
  
}





