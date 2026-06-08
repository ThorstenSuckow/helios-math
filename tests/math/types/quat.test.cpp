#include <gtest/gtest.h>
#include <numbers>
import helios.math;

namespace math = helios::math;

TEST(QuatTests, signedAxisTest) {


    const auto q1 = math::quatf::fromAxisAngle(
        math::Z_AXISf, math::radians(90.0f));
    const auto q2 = math::quatf::fromAxisAngle(
    math::Z_AXISf,math::radians(-90.0f));

    const auto r1 = q1.rotate(math::Y_AXISf);

    EXPECT_TRUE(q1.rotate(math::Y_AXISf).same(math::X_AXISf * -1.0f));
    EXPECT_TRUE(q2.rotate(math::Y_AXISf).same(math::X_AXISf));

    const auto mat1 = q1.rotationMatrix();
    const auto mat2 = q2.rotationMatrix();

    EXPECT_TRUE((mat1 * math::Y_AXISf.toVec4()).same(math::X_AXISf.toVec4() * -1.0f));
    EXPECT_TRUE((mat2 * math::Y_AXISf.toVec4()).same(math::X_AXISf.toVec4()));
}

