
#include "gtest/gtest.h"
#include <rtda/LocalVars.h>

using namespace rtda;

namespace unit_test {
  class LocalVarsTest : public testing::Test {
    protected:
    static rtda::LocalVars localVars;
    LocalVarsTest(){}
    ~LocalVarsTest(){}
  };
  rtda::LocalVars LocalVarsTest::localVars(20);
  TEST_F(LocalVarsTest, LocalVars_set) {
    localVars.setInt(0, 100);
    localVars.setInt(1, -100);
    localVars.setLong(2, 2997924580);
    localVars.setLong(4, -2997924580);
    localVars.setFloat(6, 3.1415926f);
    localVars.setDouble(18, 2.71828182845);
    localVars.setRef(10, nullptr);
    EXPECT_EQ(localVars.getInt(0), 100);
    EXPECT_EQ(localVars.getInt(1), -100);
    EXPECT_EQ(localVars.getLong(2), 2997924580);
    EXPECT_EQ(localVars.getLong(4), -2997924580);
    EXPECT_EQ(localVars.getFloat(6), 3.1415926f);
    EXPECT_EQ(localVars.getDouble(18), 2.71828182845);
    EXPECT_EQ(localVars.getRef(10), nullptr);
  }
  TEST_F(LocalVarsTest, LocalVars_testFatal) {
    rtda::LocalVars vars(20);
    EXPECT_DEATH(vars.setInt(20, 100), "setInt out of index");
    EXPECT_DEATH(vars.setLong(19, 0x0FFFFFFFFFFF1), "setLong out of index");
    EXPECT_DEATH(vars.setFloat(20, 3.14), "setFloat out of index");
    EXPECT_DEATH(vars.setDouble(19, 2.71828182845), "setDouble out of index");
  }

}