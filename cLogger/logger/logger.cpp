/*
 * log file parser
 * grabs a loadavg log file and returns it as a vector
 *
 */

#include "logger.h"


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






bool LoadAvg::updateLogfile (time_t theTime,vector<double>& v)
{

  ofstream oFile(logFile, std::ofstream::out | std::ofstream::app); //write at end of file / append

  if (oFile.fail() || !oFile.good() ) {
    cout << "File not found: " << logFile << endl;
    return false;
  } else {
    cout << "Saving to file : " << logFile << endl;    
  }

  oFile << theTime << "|";

  this->saveVector(v,oFile);

  oFile << endl;

  oFile.close();

  return true;
}


void LoadAvg::saveVector (vector <double> &v, ofstream &outputFile) 
{

    int setPrecision = 2;
    outputFile.precision(setPrecision);

    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) 
    {
        outputFile << fixed << v[i];
        if (i != last) 
            outputFile << "|";
    }
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
  string outputFileName;
  bool status;

  if ( argc != 2 ) // argc should be 2 for correct execution
  {
    // We print argv[0] assuming it is the program name
    cout<<"usage: "<< argv[0] <<" <filename>\n";
    return 0;
  }
  else 
  {    
    outputFileName = argv[1];
  }

  //set the log file
  //outputFileName = "copylog.log";
  loadavg.setLogfile(outputFileName.c_str());

  //grab the time and start timer
  time_t beginTime, endTime; 
  time(&beginTime);

  cout << "Start time : " << beginTime << endl;




  //get the load
  vector<double> theLoad;
  loadavg.getLoadAvg(theLoad); 
  cout << "Load : ";
  loadavg.printVector(theLoad);
  cout << endl;

  //saves time and load to logfile
  loadavg.updateLogfile ( beginTime, theLoad );



  time(&endTime);
  cout << "End time : " << endTime << endl;
  double timeSpan = loadavg.difftime(endTime,beginTime);
  cout << "Time elapsed: " << timeSpan << " ms" << endl;

}






