#include "command.h"
struct option cmdOption[] = {{"version", no_argument, NULL, 'v'},
                             {"help", no_argument, NULL, 'h'},
                             {"cp", required_argument, NULL, 'c'},
                             {"classpath", required_argument, NULL, 'c'},
                             {"Xms", required_argument, NULL, 100},
                             {"Xmx", required_argument, NULL, 101},
                             {"Xss", required_argument, NULL, 102},
                             {"Xjre", required_argument, NULL, 103},
                             {0, 0, 0, 0}};

std::shared_ptr<Command> Command::parseCmd(int argc, char *argv[]) {
  int opt;
  int optionIndex = 0;
  const char *optString = "?vh";
  std::shared_ptr<Command> parseResult = std::make_shared<Command>();
  while ((opt = getopt_long_only(argc, argv, optString, cmdOption,
                                 &optionIndex)) != -1) {
    switch (opt) {
    case 'v':
      parseResult->versionFlag = true;
      break;
    case 'h':
      parseResult->helpFlag = true;
      break;
    case 'c':
      parseResult->userClassPath = optarg;
      break;
    case '?':
      parseResult->helpFlag = true;
      break;
    case 100:
      break;
    case 101:
      break;
    case 102:
      break;
    case 103:
      parseResult->jrePath = optarg;
      break;
    }
  }
  if (optind < argc) {
    parseResult->className = argv[optind++];
    int argIndex = 0;
    while (optind < argc) {
      parseResult->args.push_back(argv[optind++]);
    }
  }
  return parseResult;
}