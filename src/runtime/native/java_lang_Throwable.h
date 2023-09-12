#pragma once

#include <runtime/frame.h>

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
  std::string GetFileName() { return fileName; }
  std::string GetClassName() { return className; }
  std::string GetMethodName() { return methodName; }
  int32_t GetLineNumber() { return lineNumber; }
};
void FillInStackTrace(std::shared_ptr<runtime::Frame> frame);
} // namespace runtime