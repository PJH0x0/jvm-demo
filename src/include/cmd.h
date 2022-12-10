#ifndef __CMD_H__
#define __CMD_H__ 1
#include <getopt.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

struct cmd {
  bool helpFlag = false;
  bool versionFlag = false;
  // string cpOption = "";
  string classPath = "";
  string className = "";
  string jrePath = "";
  vector<string> args;
};

#endif