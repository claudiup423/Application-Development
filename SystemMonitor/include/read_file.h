#ifndef READ_FILE_H
#define READ_FILE_H
#include <fstream>
#include <regex>
#include <string>

#include "linux_parser.h"

using std::ifstream;
using std::istringstream;
using std::string;

namespace ReadFile {
template <typename T>
T get_value(string const &filter, string const &file
) {
  string line, key;
  T value;
  ifstream stream(LinuxParser::kProcDirectory + file);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filter) {
          return value;
        }
      }
    }
  }
  return value;
};
template <typename T>
T get_value(string const &filename) {
  string line;
  T value;

  ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

template <typename T>
T get_value(int proc)
{
  string value, line;
  T status_list;
  ifstream stream(LinuxParser::kProcDirectory + std::to_string(proc) + LinuxParser::kStatFilename);
  if (stream.is_open())
  {
    getline(stream, line);
    istringstream linestream(line);
    while (linestream >> value) 
    {
			status_list.push_back(value);
    }
  }
  return status_list;
};
}  // namespace ParserHelper
#endif