#pragma once


#include <runtime/frame.h>
#include <memory>
#include <functional>
namespace native {
typedef typename std::function<void(std::shared_ptr<runtime::Frame>)> NativeMethod;
void init();
void EmptyNativeMethod(std::shared_ptr<runtime::Frame> frame);
void RegisterNativeMethod(std::string className, std::string methodName, std::string methodDescriptor, NativeMethod method);
NativeMethod FindNativeMethod(std::string className, std::string methodName, std::string methodDescriptor);
}// namespace native