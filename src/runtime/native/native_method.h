#pragma once


#include <runtime/frame.h>
#include <memory>
#include <functional>
namespace native {
typedef typename std::function<void(std::shared_ptr<runtime::Frame>)> NativeMethod;
void init();
void EmptyNativeMethod(std::shared_ptr<runtime::Frame> frame);
void RegisterNativeMethod(std::string class_name, std::string method_name, std::string method_descriptor, NativeMethod method);
NativeMethod FindNativeMethod(std::string class_name, std::string method_name, std::string method_descriptor);
}// namespace native