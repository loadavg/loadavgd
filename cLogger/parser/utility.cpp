/*
 * log file parser
 * grabs a loadavg log file and returns it as a vector
 *
 */

#include "parser.h"



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

void LoadAvg::printVectorArray (vector <string> &v) 
{
    //cout << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) 
    {
        cout << v[i];
        //if (i != last) 
            cout << endl;
    }
    //cout << "]";
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
 * prints out the vector 
 *
 */

// http://stackoverflow.com/questions/10750057/c-printing-out-the-contents-of-a-vector

void LoadAvg::saveVector (vector <string> &v, ofstream &outputFile) 
{
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) 
    {
        outputFile << v[i];
        outputFile << endl;
    }

    outputFile << endl;

}

void LoadAvg::saveHeader (string data, ofstream &outputFile) 
{

        outputFile << "#" << endl;
        outputFile << "# " << data << endl;
        outputFile << "#" << endl;
        outputFile << endl;

}


void LoadAvg::printLoadAvg(void) 
{

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








