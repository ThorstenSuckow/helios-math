/**
 * @file utils.ixx
 * @brief Misc math utility functions.
 */
module;

#include <cmath>
#include <numbers>
#include <type_traits>

export module helios.math.utils;

export namespace helios::math {

    /**
     * @brief A small epsilon value used for length-based comparisons.
     *
     * @details This constant represents the minimum threshold value for length
     * comparisons to account for numerical precision errors in floating-point
     * calculations. It is often used to avoid treating very small values as
     * significant, effectively filtering out noise in computations that involve
     * vector-lengths.
     */
    constexpr float EPSILON_LENGTH = 1e-6f;

    /**
     * @brief Converts an angle from degrees to radians.
     *
     * @param angle The angle value in degrees.
     * @return The converted angle value in radians.
     */
    constexpr float radians(const float angle) noexcept {
        return angle * static_cast<float>(std::numbers::pi)/180.0f;
    }

    /**
     * @brief Converts an angle from radians to degrees.
     *
     * @param radians The angle value in radians.
     * @return The converted angle value in degrees.
     */
    constexpr float degrees(const float radians) noexcept {
        return radians * 180.0f/static_cast<float>(std::numbers::pi);
    }

    /**
     * @brief Calculates the shortest angle between from and to.
     *
     * @details Calculates the shortest angle between from and two.
     * The result is in the range +/- 180 degrees.
     *
     * @param from
     * @param to
     *
     * @return The shortest angle between from and to
     */
    constexpr float shortestAngle(const float from, const float to) noexcept {

        const float x = to - from;

        // consteval required for MSVC since constexpr
        // for std::floor only works on Mac
        if consteval {
            const float y = (x + 180.0f) / 360.0f;
            const auto yi = static_cast<long long>(y);
            const float floored = static_cast<float>(y >= 0.0f ? yi : yi - 1);
            return x - 360.0f * floored;
        }

        return x - 360.0f * std::floor((x + 180.0f) / 360.0f);
    }


}