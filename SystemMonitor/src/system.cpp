#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return this -> cpu; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  processes = {};
  vector<int> pids = LinuxParser::Pids();
  for (int pid : pids)
  {
    Process p(pid);
    processes.push_back(p);
  }
  std::sort(processes.begin(), processes.end());
  return processes; 
}

std::string System::Kernel() { return LinuxParser::Kernel();}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }