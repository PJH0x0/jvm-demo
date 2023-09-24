#include <runtime/thread.h>
#include <runtime/oo/method.h>
#include <runtime/oo/class.h>
#include <gtest/gtest.h>
#include <classpath/class_reader.h>
#include <classfile/class_parser.h>
#include <runtime/local_vars.h>
#include <runtime/operand_stack.h>
#include <runtime/frame.h>

namespace unit_test {
class FrameTest : public testing::Test {
public:
  std::shared_ptr<const classfile::ClassFile> class_file_;
  void SetUp() override {
    std::string class_dir = TEST_PATH "/test_materials";
    classpath::DirClassReader reader(class_dir);
    std::string class_name = "com/sample/ch02/ClassReaderTest";
    std::string class_path = classpath::ClassNameToClassPath(class_name);
    auto data = reader.ReadClass(class_path);
    ASSERT_EQ(data->GetReadErrno(), classpath::kSucceed);
    class_file_ = classfile::Parse(data);
  }
};

TEST_F(FrameTest, LocalVars_OperandStack) {
  auto cf_methods = class_file_->GetMethods();
  std::vector<runtime::Method*> methods;
  for (const auto& cf_method : cf_methods) {
    auto method = new runtime::Method(cf_method, new runtime::Class);
    methods.push_back(method);
  }
  for (auto method : methods) {
    auto frame = runtime::Thread::Create()->CreateFrame(method);
    ASSERT_NE(frame, nullptr);
    auto& local_vars = frame->GetLocalVars();
    //ASSERT_NE(local_vars, nullptr);
    ASSERT_EQ(local_vars.GetSize(), method->GetMaxLocals());
    auto& operand_stack = frame->GetOperandStack();
    //ASSERT_NE(operand_stack, nullptr);
    //ASSERT_EQ(operand_stack., 0);
    ASSERT_TRUE(operand_stack.IsEmpty());
    free(method);
  }
}

TEST_F(FrameTest, NextPc) {
  auto cf_methods = class_file_->GetMethods();
  std::vector<runtime::Method*> methods;
  for (const auto& cf_method : cf_methods) {
    auto method = new runtime::Method(cf_method, new runtime::Class);
    methods.push_back(method);
  }
  for (auto method : methods) {
    auto frame = runtime::Thread::Create()->CreateFrame(method);
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->NextPc(), 0);
    frame->SetNextPc(100);
    ASSERT_EQ(frame->NextPc(), 100);
    frame->RevertNextPc();
    ASSERT_EQ(frame->NextPc(), 0);
    free(method);
  }
}

TEST_F(FrameTest, Method) {
  auto cf_methods = class_file_->GetMethods();
  std::vector<runtime::Method*> methods;
  for (const auto& cf_method : cf_methods) {
    auto method = new runtime::Method(cf_method, new runtime::Class);
    methods.push_back(method);
  }
  for (auto method : methods) {
    auto frame = runtime::Thread::Create()->CreateFrame(method);
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->GetMethod(), method);
    free(method);
  }
}
}// namespace unit_test
