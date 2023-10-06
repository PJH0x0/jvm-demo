
#include <gtest/gtest.h>
#include <runtime/local_vars.h>

using namespace runtime;

namespace test {
  class LocalVarsTest : public testing::Test {
    protected:
    static runtime::LocalVars local_vars_;
    LocalVarsTest(){}
    ~LocalVarsTest(){}
  };
  runtime::LocalVars LocalVarsTest::local_vars_(20);
  TEST_F(LocalVarsTest, LocalVars_set) {
    local_vars_.SetInt(0, 100);
    local_vars_.SetInt(1, -100);
    local_vars_.SetLong(2, 2997924580);
    local_vars_.SetLong(4, -2997924580);
    local_vars_.SetFloat(6, 3.1415926f);
    local_vars_.SetDouble(18, 2.71828182845);
    local_vars_.SetRef(10, nullptr);
    EXPECT_EQ(local_vars_.GetInt(0), 100);
    EXPECT_EQ(local_vars_.GetInt(1), -100);
    EXPECT_EQ(local_vars_.GetLong(2), 2997924580);
    EXPECT_EQ(local_vars_.GetLong(4), -2997924580);
    EXPECT_EQ(local_vars_.GetFloat(6), 3.1415926f);
    EXPECT_EQ(local_vars_.GetDouble(18), 2.71828182845);
    EXPECT_EQ(local_vars_.GetRef(10), nullptr);
  }
  TEST_F(LocalVarsTest, LocalVars_testFatal) {
    runtime::LocalVars vars(20);
    EXPECT_DEATH(vars.SetInt(20, 100), "SetInt out of index");
    EXPECT_DEATH(vars.SetLong(19, 0x0FFFFFFFFFFF1), "SetLong out of index");
    EXPECT_DEATH(vars.SetFloat(20, 3.14), "SetFloat out of index");
    EXPECT_DEATH(vars.SetDouble(19, 2.71828182845), "SetDouble out of index");
  }

}