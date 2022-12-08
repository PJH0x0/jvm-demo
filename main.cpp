#include <iostream>
#include<stdio.h>

#include <getopt.h>
#include <memory>
#include "include/cmd.h"
#include "include/ClassReader.h"

using std::shared_ptr;
using std::cout;
using std::endl;

#define VERSION 1.0

struct option cmdOption[] = {
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"cp", required_argument, NULL, 'c'},
    {"classpath", required_argument, NULL, 'c'},
    {"Xms", required_argument, NULL, 100},
    {"Xmx", required_argument, NULL, 101},
    {"Xss", required_argument, NULL, 102},
    {"Xjre", required_argument, NULL, 103},
    {0, 0, 0, 0}
};

shared_ptr<cmd> parseCmd(int argc, char* argv[]) {
    int opt;
    int optionIndex = 0;
    const char* optString = "?vh";
    shared_ptr<cmd> parseResult = std::make_shared<cmd>();
    while ((opt = getopt_long_only(argc, argv, optString, cmdOption, &optionIndex)) != -1)
    {
        switch (opt) {
            case 'v':
                parseResult->versionFlag = true;
                break;
            case 'h':
                parseResult->helpFlag = true;
                break;
            case 'c':
                parseResult->classPath = optarg;
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

int main(int argc, char* argv[]) {

    ZipClassReader reader("/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/src.zip");
    std::shared_ptr<ClassData> result = reader.readClass("java/util/ArrayList.java");
    std::cout << *(result->data) << std::endl;
    shared_ptr<cmd> startCmd = parseCmd(argc, argv);
    if (startCmd->versionFlag) {
        cout << "version " << VERSION << endl;
    } else if (startCmd->helpFlag || startCmd->className == "") {
        cout << "help" << endl;
    } else {
        printf("classpath = %s, class = %s \n", startCmd->classPath.c_str(), startCmd->className.c_str());
        cout << "class args: " << endl;
        for (auto arg : startCmd->args) {
            cout << arg << " ";
        }
        cout << endl;
    }
    
    return 0;
}