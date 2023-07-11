#pragma once


#include <runtime/frame.h>
#include <memory>
#include <functional>
namespace native {
typedef typename std::function<void(std::shared_ptr<runtime::Frame>)> NativeMethod;
void init();
void emptyNativeMethod(std::shared_ptr<runtime::Frame> frame);
void registerNativeMethod(std::string className, std::string methodName, std::string methodDescriptor, NativeMethod method);
NativeMethod findNativeMethod(std::string className, std::string methodName, std::string methodDescriptor);
}// namespace native