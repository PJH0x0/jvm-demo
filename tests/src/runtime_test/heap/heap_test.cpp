#include <gtest/gtest.h>
#include <runtime/alloc/heap.h>
#include <runtime/oo/class.h>
#include <glog/logging.h>

namespace test {
class HeapTest : public testing::Test {
  protected:
  heap::Heap mHeap;
  HeapTest() : mHeap(4097) {}
  ~HeapTest() override {}
  void SetUp() override {
    
  }

};

TEST_F(HeapTest, Heap_Heap) {
  LOG(WARNING) << "heap base " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.GetHeapBase());
  LOG(WARNING) << "heap size " << std::hex << "0x" << mHeap.GetAbsoluteMaxSize();
  LOG(WARNING) << "fresh_start_ " << std::hex << "0x"<< reinterpret_cast<uintptr_t>(mHeap.GetFreshStart());
  LOG(WARNING) << "fresh_size_ " << std::hex << "0x" << mHeap.GetFreshSize();
  LOG(WARNING) << "from_ptr_ " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.GetFromPtr());
  LOG(WARNING) << "to_ptr_ " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.GetToPtr());
  LOG(WARNING) << "survivor_size_ " << std::hex << "0x" << mHeap.GetSurvivorSize();
  LOG(WARNING) << "old_start_ " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.GetOldStart());
  LOG(WARNING) << "fresh_pos_ " << std::hex << "0x " << reinterpret_cast<uintptr_t>(mHeap.GetFreshPos());
  LOG(WARNING) << "old_pos_ " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.GetOldPos());
  LOG(WARNING) << "oldSize " << std::hex <<"0x" << (mHeap.GetHeapBase() + mHeap.GetAbsoluteMaxSize() - mHeap.GetOldStart());
  LOG(WARNING) << "fresh_size_" << std::hex << "0x" << (mHeap.GetOldStart() - mHeap.GetFreshStart());
  LOG(WARNING) << "object header size " << std::hex << "0x" << sizeof(runtime::Object);
  LOG(WARNING) << "Class size" << std::hex << "0x" << sizeof(runtime::Class);
        mHeap.AllocObject(nullptr, nullptr, sizeof(runtime::Object));
  LOG(WARNING) << "fresh_pos_ " << std::hex << "0x" << reinterpret_cast<uintptr_t>(mHeap.GetFreshPos());

}
}//namespace test