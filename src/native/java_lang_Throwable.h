#pragma once

#include <rtda/frame.h>

namespace native {
struct StackTraceElement {
  private:
  std::string fileName;
  std::string className;
  std::string methodName;
  int32_t lineNumber;
  public:
  StackTraceElement(std::string fileName, std::string className, 
                    std::string methodName, int32_t lineNumber)
                    : fileName(fileName), className(className), 
                      methodName(methodName), lineNumber(lineNumber) {
  }
  std::string getFileName() { return fileName; }
  std::string getClassName() { return className; }
  std::string getMethodName() { return methodName; }
  int32_t getLineNumber() { return lineNumber; }
};
void fillInStackTrace(std::shared_ptr<rtda::Frame> frame);
} // namespace rtda