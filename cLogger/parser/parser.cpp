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
 * explodes the string into a vector 
 *
 */

 // http://www.cplusplus.com/faq/sequences/strings/split/

vector<string> LoadAvg::explode(string const & s, char delim)
{
    vector<string> result;
    istringstream iss(s);

    for (string token; getline(iss, token, delim); )
    {
        result.push_back(move(token));
    }

    return result;
}

/*
 * prints out the vector 
 *
 */

// http://stackoverflow.com/questions/10750057/c-printing-out-the-contents-of-a-vector

void LoadAvg::printVector (vector <string> &v) 
{
    cout << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) 
    {
        cout << v[i];
        if (i != last) 
            cout << ", ";
    }
    cout << "]";
    //return out;
}

void LoadAvg::printVector (vector <double> &v) 
{
    cout << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) 
    {
        cout << v[i];
        if (i != last) 
            cout << ", ";
    }
    cout << "]";
    //return out;
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

  while (!iFile.eof()) {

    //Next = loadavg.Parser(iFile); // grab a line from the file
    Next = this->Parser(iFile); // grab a line from the file

    if (!Next.empty()) {

      auto v = this->explode(Next, '|');

      cout << "Line[" << i << "] = ";
      this->printVector(v);
      cout << endl;

      i++;
    }
  }

  cout << "Total lines : " << i << endl;

  iFile.close();

  return true;
}


/*
 * prints out the vector 
 *
 */

// http://stackoverflow.com/questions/10750057/c-printing-out-the-contents-of-a-vector

void LoadAvg::saveVector (vector <string> &v, ofstream &outputFile) 
{
    outputFile << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) 
    {
        outputFile << v[i];
        if (i != last) 
            outputFile << ", ";
    }
    outputFile << "]";
    //outputFile << endl;

    //return out;
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

      auto v = this->explode(Next, '|');

      this->saveVector(v,oFile);
      oFile << endl;

      cout << ".";

      i++;
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


void LoadAvg::printLoadAvg(void) 
{
  
  //int getloadavg(double loadavg[], int nelem);
  //int load = getloadavg(double loadavg[], 5);

  double load[3];
  if (getloadavg(load, 3) != -1) /*getloadavg is the function used to calculate and obtain the load average*/
  {
      cout << "load average :" << load[0] << " | " << load[1] << " | " << load[2]  << endl;
  }
}

bool LoadAvg::getLoadAvg(vector<double> & theLoad) 
{
  
  //int getloadavg(double loadavg[], int nelem);
  //int load = getloadavg(double loadavg[], 5);

  double load[3];
  if (getloadavg(load, 3) != -1) /*getloadavg is the function used to calculate and obtain the load average*/
  {

      theLoad = { load[0], load[1], load[2] };

      return true;
  }

  return false;
}


double LoadAvg::difftime(time_t clock1,time_t clock2)
{
    double diffticks=clock1-clock2;
    //double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
    //return diffms;

    return diffticks;
} 

// you need to include ctime
void LoadAvg::sleep(int timer)
{

  std::this_thread::sleep_for(std::chrono::milliseconds(timer));
  /*
  int stime=time();

  while(true)
  {
    if (time()-stime==timer) 
      break;
  }
  */
}


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

  outputFileName = "copylog.log";

  time_t beginTime, endTime; 

  time(&beginTime);

  cout << "Start time : " << beginTime << endl;


  vector<double> theLoad;

  loadavg.getLoadAvg(theLoad); 

  cout << "Load : ";

  loadavg.printVector(theLoad); 

  cout << endl;

  //loadavg.printLoadAvg();



  loadavg.setLogfile(inputFileName.c_str());

  status = loadavg.processLog(); 

  if (status)
      cout << "Parse successful" << endl;
  else
      cout << "Parse failed" << endl;


  cout << "----------------------------" << endl;

  cout << "copy file" << endl;

  status = loadavg.copyLog(outputFileName); 

  loadavg.sleep(1000);

  loadavg.printLoadAvg();




  time(&endTime);

  cout << "End time : " << endTime << endl;

  double timeSpan = loadavg.difftime(endTime,beginTime);

  cout << "Time elapsed: " << timeSpan << " ms" << endl;

}







