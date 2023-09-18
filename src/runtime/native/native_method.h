#pragma once


#include <runtime/frame.h>
#include <memory>
#include <functional>
namespace native {
typedef typename std::function<void(runtime::Frame*)> NativeMethod;
void init();
void EmptyNativeMethod(runtime::Frame* frame);
void RegisterNativeMethod(std::string class_name, std::string method_name, std::string method_descriptor, NativeMethod method);
NativeMethod FindNativeMethod(std::string class_name, std::string method_name, std::string method_descriptor);
}// namespace native