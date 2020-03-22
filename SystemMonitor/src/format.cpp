#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) 
{ 
  long time_hours = seconds/3600;
  long time_left = seconds%3600;
  long time_minutes = {time_left/60};
  long time_seconds = {time_left%60};
  
  string time_hours_str = to_string(time_hours);
  string time_minutes_str = to_string(time_minutes);
  string time_seconds_str = to_string(time_seconds);
  
  return time_hours_str + ":" + time_minutes_str + ":" + time_seconds_str; 
}