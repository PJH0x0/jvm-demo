#include <cstdio>
#include <iostream>
#include <stdio.h>

#include <ClassReader.h>
#include <ClassPathParser.h>
#include "include/cmd.h"
#include <getopt.h>
#include <memory>

using namespace JVM;

using std::cout;
using std::endl;
using std::shared_ptr;

#define VERSION 1.0

struct option cmdOption[] = {{"version", no_argument, NULL, 'v'},
                             {"help", no_argument, NULL, 'h'},
                             {"cp", required_argument, NULL, 'c'},
                             {"classpath", required_argument, NULL, 'c'},
                             {"Xms", required_argument, NULL, 100},
                             {"Xmx", required_argument, NULL, 101},
                             {"Xss", required_argument, NULL, 102},
                             {"Xjre", required_argument, NULL, 103},
                             {0, 0, 0, 0}};

shared_ptr<cmd> parseCmd(int argc, char *argv[]) {
  int opt;
  int optionIndex = 0;
  const char *optString = "?vh";
  shared_ptr<cmd> parseResult = std::make_shared<cmd>();
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
bool checkClassMagic(const unsigned char* data) {
    unsigned char magic[] = {
      static_cast<unsigned char>(0xca), 
      static_cast<unsigned char>(0xfe), 
      static_cast<unsigned char>(0xba), 
      static_cast<unsigned char>(0xbe)};
    for (int i = 0; i < sizeof(magic); i++) {
      if (*(data + i) != magic[i]) return false;
    }
    return true;
}
static void startJVM(shared_ptr<cmd> startCmd) {
  shared_ptr<ClassPathParser> parser = std::make_shared<ClassPathParser>(startCmd->jrePath, startCmd->userClassPath);
  shared_ptr<ClassData> data = parser->readClass(startCmd->className);
  if (data->readErrno == JVM::SUCCEED) {
    for (int i = 0; i < data->size; i++) {
      std::printf("%x ", *(data->data + i));
    }
    cout << endl;
  } else {
    cout << "readClass failed reason: " << data->readErrno << endl;
  }
}

int main(int argc, char *argv[]) {

  shared_ptr<cmd> startCmd = parseCmd(argc, argv);
  if (startCmd->versionFlag) {
    cout << "version " << VERSION << endl;
  } else if (startCmd->helpFlag || startCmd->className == "") {
    cout << "help" << endl;
  } else {
    printf("classpath = %s, class = %s \n", startCmd->userClassPath.c_str(),
           startCmd->className.c_str());
    cout << "class args: " << endl;
    for (auto arg : startCmd->args) {
      cout << arg << " ";
    }
    
    startJVM(startCmd);
  }
  

  return 0;
}