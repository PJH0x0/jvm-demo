#ifndef __CLASS_PARSER_H__
#define __CLASS_PARSER_H__ 1
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <vector>
using std::string;

typedef unsigned char byte;
struct ClassData {
    char* data;
    string errMessage;
};
class ClassReader {
    public:
    virtual std::shared_ptr<ClassData> readClass(const string& className) = 0;
    virtual string toString() = 0;
    virtual ~ClassReader() {}
};

class DirClassReader : public ClassReader {
    private:
    string absDir;
    public:
    DirClassReader(string _absDir) : absDir(_absDir){}
    std::shared_ptr<ClassData> readClass(const string& className);
    string toString();
};
class ZipClassReader : public ClassReader {
    private:
    string absPath;
    public:
    ZipClassReader(string _absPath) : absPath(_absPath) {}
    std::shared_ptr<ClassData> readClass(const string& className);
    string toString();
    ~ZipClassReader();
};
class CompositeClassReader : public ClassReader {
    private:
    std::vector<ClassReader> readers;
    public:
    std::shared_ptr<ClassData> readClass(const string& className);
    string toString();
};

//class WildcardEntry : public CompositeEntry


#endif