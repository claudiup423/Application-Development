#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();         
  long UpTime();                     
  int TotalProcesses();              
  int RunningProcesses();
  void ProcessUpdate(int pid);         
  std::string Kernel();               
  std::string OperatingSystem();      

 private:
  Processor cpu = {};
  std::vector<Process> processes = {};
};

#endif