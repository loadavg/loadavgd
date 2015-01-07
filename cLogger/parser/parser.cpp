/*
 * log file parser
 * grabs a loadavg log file and returns it as a vector
 *
 */

#include "parser.h"


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

  vector<string> basicArray;
  vector<string> pluginArray;
  vector<string> optionsArray;

  cout << "Parsing file : " << logFile << endl;

  while (!iFile.eof()) {

    //Next = loadavg.Parser(iFile); // grab a line from the file
    Next = this->Parser(iFile); // grab a line from the file

    if (!Next.empty()) {

      if ( Next.front() != '#' )
      {

            //trying a basic parse here of data
            auto v = this->explode(Next, ' ');
            string s;
            s = accumulate(begin(v), end(v), s);

            basicArray.push_back(s);            
            
            //as opposed to just the data
            //myArray.push_back(Next);            

            i++;
      }
    }

  }

  this->printVectorArray(basicArray);

  cout << "Array size : " << basicArray.size() << endl;

  cout << "Total lines : " << i << endl;

  iFile.close();

  return true;
}




/*
 * process the log file and copy to new location
 *
 */

bool LoadAvg::copyLog (string outputFile) 
{


  //grab the input file
  ifstream iFile(logFile); 

  if (iFile.fail() || !iFile.good() ) {
    cout << "File not found: " << logFile << endl;
    return false;
  } else {
    cout << "Source file : " << logFile << endl;    
  }

  //grab the output file
  //
  ofstream oFile(outputFile); 
  //ofstream oFile(outputFile, std::ofstream::out | std::ofstream::app); //write at end of file / append

  if (oFile.fail() || !oFile.good() ) {
    cout << "File not found: " << logFile << endl;
    return false;
  } else {
    cout << "Output file : " << outputFile << endl;    
  }



  int i = 0;
  string Next;

  cout << "Copying file: ";

  while (!iFile.eof()) {

    //Next = loadavg.Parser(iFile); // grab a line from the file
    Next = this->Parser(iFile); // grab a line from the file

    if (!Next.empty()) {

      if ( Next.front() != '#' )
      {
          auto v = this->explode(Next, '|');

          this->saveVector(v,oFile);
          oFile << endl;

          //progress report one dot per line copied
          cout << ".";

          i++;
      }

    }
  }

  cout << endl;
  cout << "Total lines : " << i << endl;

  iFile.close();
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






