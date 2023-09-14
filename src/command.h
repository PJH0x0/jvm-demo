#pragma once

#include <getopt.h>
#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;


class Command {
public:
  Command() : help_flag_(false), version_flag_(false) {}

  bool PrintHelp() const;

  bool PrintVersion() const;

  const string& GetUserClassPath() const;

  const string& GetClassName() const;

  const string& GetJrePath() const;

  const vector<string>& GetArgs() const;

  static std::shared_ptr<Command> ParseCommand(int argc, char *argv[]);
private:
  bool help_flag_;
  bool version_flag_;
  // string cpOption = "";
  string user_class_path_;
  string class_name_;
  string jre_path_;
  vector<string> args_;
};

