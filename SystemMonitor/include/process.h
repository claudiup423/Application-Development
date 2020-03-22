#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
using std::string;

class Process {
 public:
  Process(int);
  int Pid();                              
  string User();                      
  string Command();                  
  float CpuUtilization();                  
  string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const; 

 private:
  int pid{0};
  string command{};
  float time_seconds{0.0}, total_time{0.0}, cpu{0.0};
};

#endif