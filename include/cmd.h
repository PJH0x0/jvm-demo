#ifndef __CMD_H__
#define __CMD_H__ 1
#include<string>
#include <vector>
#include <getopt.h>
using std::vector;
using std::string;

struct cmd {
    bool helpFlag = false;
    bool versionFlag = false;
    //string cpOption = "";
    string classPath = "";
    string className = "";
    string jrePath = "";
    vector<string> args;
};


#endif