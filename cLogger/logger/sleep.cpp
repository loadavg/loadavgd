cool scueduler

http://codereview.stackexchange.com/questions/21336/c-task-scheduler

http://www.experts-exchange.com/Programming/System/Linux/Q_20932913.html

http://blog.coralbits.com/2014/03/c-cron-class-implementation.html

#include <iostream>
#include <chrono>
#include <thread>

int main()
{
  std::thread timer([]() {
    std::this_thread::sleep_for(std::chrono::minutes(5));
    std::cout << "hello, world!" << std::endl;
    //Or call to function, or whatever you want
  });
  std::cout << "thread begun..." << std::endl;
  //code to do while waiting here.
  timer.join(); //Before program finishes. Basically "Wait for thread end"
}


#include <thread>
#include <chrono>

int main() {

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(60));
    // call your c++ code
  }

}


/// c based scheduler

#include <pthread.h>
int main(int argc, char *argv[]) {
  pthread_t thread1;
  int variables=10;
  pthread_create( &thread1, NULL, scheduler, (void*)&variables);
  while(1){
    .... do stuff as main program.
  }
  return 0;
}

void *scheduler (void* variables) {
  int vars;
  int* p_vars = (int*) variables;
  vars = *p_vars;
  while (1){
    .. do scheduler stuff
    sleep (vars);
  }
}



// sleep for no of seconds
void sleep(unsigned int seconds)
{
	clock_t goal = seconds*CLOCKS_PER_SEC + clock();
	while (goal > clock());
}



