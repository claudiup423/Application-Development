#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "read_file.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;

string LinuxParser::OperatingSystem() 
{
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() 
{
  string os, version, kernel;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return version + " " + kernel;
}

vector<int> LinuxParser::Pids() 
{
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() 
{ 
  float total_memory = ReadFile::get_value<int>("MemTotal:", LinuxParser::kMeminfoFilename);
  float freed_memory = ReadFile::get_value<int>("MemFree:", LinuxParser::kMeminfoFilename);

  return (total_memory - freed_memory)/total_memory;
}

long LinuxParser::UpTime() 
{ 
  long up_time = ReadFile::get_value<long>(LinuxParser::kUptimeFilename);
  return up_time;
}

int LinuxParser::TotalProcesses() {return ReadFile::get_value<int>("processes", LinuxParser::kStatFilename);}

int LinuxParser::RunningProcesses() { return ReadFile::get_value<int>("procs_running", LinuxParser::kStatFilename);}

long LinuxParser::UpTime(int pid) 
{ 
  vector<string> status_list = ReadFile::get_value<vector<string>>(pid);
  long int start_time = stol(status_list[21])/sysconf(_SC_CLK_TCK);
  long int  uptime =  LinuxParser::UpTime() - start_time;
  return uptime;
}

long LinuxParser::ActiveJiffies(int pid) 
{
  vector <string> status_list = ReadFile::get_value<vector<string>>(pid);
  long int utime{stol(status_list[13])};
  long int stime{stol(status_list[14])};
  long int cutime{stol(status_list[15])};
  long int cstime{stol(status_list[16])};
  long clock = (utime + stime + cutime + cstime);
  return clock;
}

string LinuxParser::Uid(int pid) 
{ 
  string key, value, uid, user, line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (getline(stream, line)){
  		std::istringstream linestream(line);
    	while (linestream >> key >> value) {
        	if (key == "Uid:") { 
              	uid = value;
               }
        	}
      	}
    }
  return uid;
}

vector<string> LinuxParser::CpuUtilization()
{ 
  string line, cpu, cpu_time;
  vector<string> cpu_utilizations;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream  >> cpu;

      while (linestream >> cpu_time) {
          cpu_utilizations.emplace_back(cpu_time);
      }
  }
  return cpu_utilizations;
} 

string LinuxParser::Command(int pid) 
{ 
  string command, line;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) 
  {
    while (getline(stream, line))
    {
  		istringstream linestream(line);
      replace(line.begin(), line.end(), ' ', '*');
    	while (linestream >> command) 
      {
			replace(command.begin(), command.end(), '*', ' ');
          	return command;
      }
    }
  }
  return string(); 
}

string LinuxParser::Ram(int pid) 
{  
  string key, value, kilob, line;
  int vmsize;
  ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
  		istringstream linestream(line);
    	while (linestream >> key >> value >> kilob) {
        	if (key == "VmSize:") 
          { 
            vmsize = stoi(value); 
            vmsize = vmsize/1000;
            return to_string(vmsize);
          }
        	}
      	}
    }
  return to_string(vmsize); 
}

string LinuxParser::User(int pid) 
{ 
  string usr, passwd, uid, line;
  string uid_ = LinuxParser::Uid(pid);
  ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (getline(stream, line)){
        replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
    	while (linestream >> usr >> passwd >> uid) {
               if (uid==uid_){
                return usr;
               }
        	}
      	}
    }
  return usr;
}