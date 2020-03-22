#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <iostream>
#include "linux_parser.h"
#include <string>
#include <vector>


class Processor {
 public:
  float Utilization(); 

  
 private:
   float total_jiffies{0.0}, total_jiffies_prev{0.0}, idle_jiffies{0.0}, util_jiffies{0.0}, util_jiffies_prev{0.0}, usage{0.0};

};

#endif