 
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) 
{
  this -> pid = pid;
  this -> cpu = Process::CpuUtilization();
  this -> command = LinuxParser::Command(this -> pid);
}

// TODO: Return this process's ID
int Process::Pid() { return this -> pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
  long int hertz = sysconf(_SC_CLK_TCK);
  this -> total_time = LinuxParser::ActiveJiffies(this -> pid)/ hertz; 
  this -> time_seconds = LinuxParser::UpTime(this -> pid);
  return this -> total_time/ this -> time_seconds;
}

string Process::Command() { return this -> command; }

string Process::Ram() { return LinuxParser::Ram(this -> pid); }

string Process::User() { return LinuxParser::User(this -> pid); }

long int Process::UpTime() { return LinuxParser::UpTime(this -> pid); }

bool Process::operator<(Process const& a) const {return this -> cpu > a.cpu;}
