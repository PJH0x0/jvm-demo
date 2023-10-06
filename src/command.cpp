#include "command.h"
struct option cmd_option[] = {{"version",   no_argument,       NULL, 'v'},
                              {"help",      no_argument,       NULL, 'h'},
                              {"cp",        required_argument, NULL, 'c'},
                              {"classpath", required_argument, NULL, 'c'},
                              {"Xms",       required_argument, NULL, 100},
                              {"Xmx",       required_argument, NULL, 101},
                              {"Xss",       required_argument, NULL, 102},
                              {"Xjre",      required_argument, NULL, 103},
                              {0, 0, 0,                              0}};

std::shared_ptr<Command> Command::ParseCommand(int argc, char** argv) {
  int opt;
  int option_index = 0;
  const char* opt_string = "?vh";
  std::shared_ptr<Command> parse_result = std::make_shared<Command>();
  while ((opt = getopt_long_only(argc, argv, opt_string, cmd_option,
                                 &option_index)) != -1) {
    switch (opt) {
      case 'v':
        parse_result->version_flag_ = true;
        break;
      case 'h':
        parse_result->help_flag_ = true;
        break;
      case 'c':
        parse_result->app_class_path_ = optarg;
        break;
      case '?':
        parse_result->help_flag_ = true;
        break;
      case 100:
        break;
      case 101:
        break;
      case 102:
        break;
      case 103:
        parse_result->jre_path_ = optarg;
        break;
      default:
        break;
    }
  }
  if (optind < argc) {
    parse_result->class_name_ = argv[optind++];
    int argIndex = 0;
    while (optind < argc) {
      parse_result->args_.push_back(argv[optind++]);
    }
  }
  return parse_result;
}

bool Command::PrintHelp() const {
  return help_flag_;
}

bool Command::PrintVersion() const {
  return version_flag_;
}

const string& Command::GetAppClassPath() const {
  return app_class_path_;
}

const string& Command::GetClassName() const {
  return class_name_;
}

const string& Command::GetJrePath() const {
  return jre_path_;
}

const vector<string>& Command::GetArgs() const {
  return args_;
}