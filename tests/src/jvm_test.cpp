#include <gtest/gtest.h>
#include <jvm.h>
#include <command.h>
#include "stub.h"
#include "addr_pri.h"

namespace test {
class JVMTest : public testing::Test {
public:
  std::shared_ptr<Command> cmd;
  void SetUp() override {
    constexpr int32_t kCommandSize = 6;
    const char* argv[kCommandSize] = {"./build/java", "-Xjre",
                                      "/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre",
                                      "-cp",
                                      TEST_PATH "/test_materials",
                                      "com.sample.ch02.ClassReaderTest"};
    cmd = Command::ParseCommand(kCommandSize, const_cast<char**>(argv));
    JVM::CreateVM(cmd);
  }
};

static void InterpretStub(runtime::Thread* thread) {
  return;
}
#ifdef linux
private_access_detail::ACCESS_PRIVATE_STATIC_FUN(JVM, void(runtime::Thread*), Interpret);
#endif

TEST_F(JVMTest, Current) {
  auto jvm = JVM::Current();
  ASSERT_NE(jvm, nullptr);
}

TEST_F(JVMTest, Start) {
//无法hook到Interpret函数，macos无法进行测试
#ifdef linux
  auto JVM_Interpret = get_private_static_fun::JVM::JVMInterpret();
  Stub stub;
  stub.set(JVM_Interpret, InterpretStub);
#elif __APPLE__
  JVM::Current()->Start();
#endif
}
}
