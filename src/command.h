#pragma once

#include <getopt.h>
#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;


struct Command {
  bool helpFlag = false;
  bool versionFlag = false;
  // string cpOption = "";
  string userClassPath = "";
  string className = "";
  string jrePath = "";
  vector<string> args;
  static std::shared_ptr<Command> parseCmd(int argc, char *argv[]);
};

