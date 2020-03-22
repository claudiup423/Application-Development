#include "processor.h"

float Processor::Utilization() 
{  
  std::vector<std::string> jiffies = LinuxParser::CpuUtilization();
  
  for (size_t i = 0; i<jiffies.size();i++){
    if (i!=LinuxParser::kGuest_ and i!=LinuxParser::kGuestNice_){
    	total_jiffies += stof(jiffies[i]);
    }
    if (i==LinuxParser::kIdle_ or i==LinuxParser::kIOwait_){
    	idle_jiffies += stof(jiffies[i]);
    }
  }
  util_jiffies = total_jiffies - idle_jiffies;
  
  bool curr_util = true;
  if (curr_util){
    usage = (util_jiffies-util_jiffies_prev)/(total_jiffies-total_jiffies_prev);
    total_jiffies_prev = total_jiffies;
    util_jiffies_prev = util_jiffies;
  }
  else{
    usage = util_jiffies/total_jiffies;
  }
  return usage; 
} 