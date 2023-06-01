#pragma once

#include "rtda/frame.h"
#include <memory>
#include <functional>
namespace native {
typedef typename std::function<void(std::shared_ptr<rtda::Frame>)> NativeMethod;
void getClass(std::shared_ptr<rtda::Frame> frame);
void getPrimitiveClass(std::shared_ptr<rtda::Frame> frame);
void getName0(std::shared_ptr<rtda::Frame> frame);
void desiredAssertionStatus0(std::shared_ptr<rtda::Frame> frame);
void init();
void emptyNativeMethod(std::shared_ptr<rtda::Frame> frame);
void registerNativeMethod(std::string className, std::string methodName, std::string methodDescriptor, NativeMethod method);
NativeMethod findNativeMethod(std::string className, std::string methodName, std::string methodDescriptor);
}// namespace native