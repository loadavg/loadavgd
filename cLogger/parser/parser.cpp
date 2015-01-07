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


//need to move to this ASAP
typedef struct collectLog {

  vector<string> basicArray;
  vector<string> pluginArray;
  vector<string> optionsArray;

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
            basicArray.push_back(Next);  
            match = true; 
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






