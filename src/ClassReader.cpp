#include "include/ClassReader.h"
#include "ZipFile.h"
#include <iostream>
#include <istream>
#include <string.h>
#include <string>


std::shared_ptr<ClassData> ZipClassReader::readClass(const string& className) {
    std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();
    ZipArchive::Ptr archive = ZipFile::Open(absPath);
    size_t entries = archive->GetEntriesCount();
    for (size_t i = 0; i < entries; i++) {
        auto entry = archive->GetEntry((int)i);
        if (entry->GetFullName() == className) {
            std::cout << "class name = " << className << std::endl;
            std::istream* decompressionsStream = entry->GetDecompressionStream();
            size_t size = entry->GetSize();
            std::cout << "size = " << size << std::endl;
            char* tmp = (char*)malloc(sizeof(char) * size);
            memset(tmp, 0, size);
            if (decompressionsStream->read(tmp, size)) {
                classData->data = tmp;
                std::cout << "classData->data" << std::endl;
                for (int i = 0; i < size; i++) {
                    std::cout << "  " << *(tmp+i);
                }
            } else {
                classData->data = nullptr;
            }
            break;
        }
    }
    return classData;
}
std::string ZipClassReader::toString() {
    return absPath;
}
ZipClassReader::~ZipClassReader() {

}