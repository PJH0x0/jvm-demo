#pragma once

#include <getopt.h>
#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;


struct Command {
  bool help_flag_ = false;
  bool version_flag_ = false;
  // string cpOption = "";
  string user_class_path_;
  string class_name_;
  string jre_path_;
  vector<string> args;
  static std::shared_ptr<Command> ParseCommand(int argc, char *argv[]);
};

