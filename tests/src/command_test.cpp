#include <gtest/gtest.h>
#include <command.h>

namespace test {
class CommandTest : public testing::Test {
public:
  static constexpr uint32_t kCommandSize = 6;
  const char* argv_[kCommandSize] = {"./build/java", "-Xjre",
                                    "/Library/Java/JavaVirtualMachines/adoptopenjdk-8.jdk/Contents/Home/jre",
                                    "-cp",
                                    TEST_PATH "/test_materials",
                                    "com.sample.ch02.ClassReaderTest"};
  void SetUp() override {

  }
};

TEST_F(CommandTest, ParseCommand) {
  auto command = Command::ParseCommand(kCommandSize, const_cast<char**>(argv_));
  ASSERT_NE(command, nullptr);
  ASSERT_FALSE(command->GetAppClassPath().empty());
  ASSERT_FALSE(command->GetJrePath().empty());
  ASSERT_TRUE(command->GetArgs().empty());
  ASSERT_FALSE(command->GetClassName().empty());
}
}//namespace test
