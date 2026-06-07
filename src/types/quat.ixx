/**
 * @file quat.ixx
 * @brief Quaternion type for numeric scalar types.
 */
module;

export module helios.math.types:quat;

import helios.math.concepts;
import :vec3;

export namespace helios::math {

    /**
     * @brief Quaternion with scalar part `w` and vector part `v`.
     *
     * @tparam T Numeric scalar type.
     */
    template<typename T>
    requires concepts::IsNumeric<T>
    class quat {

        T w_;

        vec3<T> v_;

    public:

        /**
         * @brief Constructs a quaternion from vector and scalar parts.
         *
         * @param v Vector (imaginary) part.
         * @param w Scalar (real) part.
         */
        explicit constexpr quat(const vec3<T> v, const T w) noexcept
            : w_(w), v_(v) {}


        /**
         * @brief Multiplies this quaternion by another quaternion.
         *
         * @param other Right-hand quaternion operand.
         * @return Product quaternion.
         */
        constexpr quat<T> operator*(const quat<T>& other) {

            const auto v = v_.cross(other.v_) + w_ * other.v_ + other.w_ * v_;

            const auto w = w_ * other.w_ - dot(v_, other.v_);

            return {v, w};
        }

        /**
         * @brief Returns the scalar part.
         *
         * @return Scalar component `w`.
         */
        constexpr T w() const noexcept {
            return w_;
        }

        /**
         * @brief Returns the vector part.
         *
         * @return Vector component `v`.
         */
        constexpr vec3<T> v() const noexcept {
            return v_;
        }

    };

}