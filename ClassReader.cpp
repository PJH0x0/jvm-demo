#include "include/ClassReader.h"
#include "ZipLib/ZipFile.h"
#include <fstream>
#include <iostream>
#include <istream>


std::shared_ptr<ClassData> ZipClassReader::readClass(const string& className) {
    std::shared_ptr<ClassData> classData = std::make_shared<ClassData>();
    ZipArchive::Ptr archive = ZipFile::Open(absPath);
    size_t entries = archive->GetEntriesCount();
    for (size_t i = 0; i < entries; i++) {
        auto entry = archive->GetEntry((int)i);
        if (entry->GetFullName() == className) {
            std::istream* decompressionsStream = entry->GetDecompressionStream();
            size_t size = entry->GetSize();
            classData->data = (char*)malloc(sizeof(char) * size);
            if (decompressionsStream->read(classData->data, sizeof(char) * size)) {
                std::cout << "Read success" << std::endl;
            }
            break;
        }
    }
    return classData;
}

ZipClassReader::~ZipClassReader() {

}