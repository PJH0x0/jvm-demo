#include <gtest/gtest.h>
#include <runtime/alloc/heap.h>
#include <runtime/oo/class.h>
#include <glog/logging.h>

namespace unit_test {
class HeapTest : public testing::Test {
  protected:
  heap::Heap mHeap;
  HeapTest() : mHeap(0) {}
  ~HeapTest() override {}
  void SetUp() override {
    
  }

};

TEST_F(HeapTest, Heap_Heap) {
  LOG(WARNING) << "heap base " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.heapBase);
  LOG(WARNING) << "heap size " << std::hex << "0x" << mHeap.absoluteMaxSize;
  LOG(WARNING) << "freshStart " << std::hex << "0x"<< reinterpret_cast<uintptr_t>(mHeap.freshStart);
  LOG(WARNING) << "freshSize " << std::hex << "0x" << mHeap.freshSize;
  LOG(WARNING) << "fromPtr " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.fromPtr);
  LOG(WARNING) << "toPtr " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.toPtr);
  LOG(WARNING) << "survivorSize " << std::hex << "0x" << mHeap.survivorSize;
  LOG(WARNING) << "oldStart " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.oldStart);
  LOG(WARNING) << "freshPos " << std::hex << "0x " << reinterpret_cast<uintptr_t>(mHeap.freshPos);
  LOG(WARNING) << "oldPos " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.oldPos);
  LOG(WARNING) << "oldSize " << std::hex <<"0x" << (mHeap.heapBase + mHeap.absoluteMaxSize - mHeap.oldStart);
  LOG(WARNING) << "freshSize" << std::hex << "0x" << (mHeap.oldStart - mHeap.freshStart);
  LOG(WARNING) << "object header size " << std::hex << "0x" << sizeof(runtime::Object);
  LOG(WARNING) << "Class size" << std::hex << "0x" << sizeof(runtime::Class);
  mHeap.allocObject(nullptr, nullptr, sizeof(runtime::Object));
  LOG(WARNING) << "freshPos " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.freshPos);

}
}//namespace unit_test