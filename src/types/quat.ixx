/**
 * @file quat.ixx
 * @brief Quaternion type for floating-point scalar types.
 */
module;


#include <cmath>
#include <concepts>

export module helios.math.types:quat;

import :vec3;
import :mat4;


export namespace helios::math {

    /**
     * @brief Quaternion with scalar part `w` and vector part `v`.
     *
     * @tparam T Floating-point scalar type.
     */
    template<typename T>
    requires std::floating_point<T>
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
        constexpr quat<T> operator*(const quat<T>& other) const noexcept {

            const auto v = v_.cross(other.v_) + w_ * other.v_ + other.w_ * v_;

            const auto w = w_ * other.w_ - dot(v_, other.v_);

            return quat<T>{v, w};
        }

        /**
         * @brief Returns the quaternion conjugate.
         *
         * @return Conjugated quaternion with negated vector part.
         */
        [[nodiscard]] constexpr quat<T> conjugate() const noexcept {
            return quat<T>{-v_, w_};
        }


        /**
         * @brief Returns the scalar part.
         *
         * @return Scalar component `w`.
         */
        [[nodiscard]] constexpr T w() const noexcept {
            return w_;
        }

        /**
         * @brief Returns the vector part.
         *
         * @return Vector component `v`.
         */
        [[nodiscard]] constexpr vec3<T> v() const noexcept {
            return v_;
        }

        /**
         * @brief Returns the Euclidean norm of the quaternion.
         *
         * @return Quaternion length.
         */
        [[nodiscard]] constexpr T length() const noexcept {
            return static_cast<T>(std::sqrt(v_.x * v_.x + v_.y * v_.y + v_.z * v_.z + w_ * w_));
        }

        /**
         * @brief Returns a normalized copy of the quaternion.
         *
         * @return Unit-length quaternion.
         */
        [[nodiscard]] constexpr quat<T> normalized() const noexcept {
            const auto l = length();
            return quat<T>{v_ * (static_cast<T>(1) / l),  w_ / l};
        }

        /**
         * @brief Converts the (normalized) quaternion to a 4x4 rotation matrix.
         *
         *
         * @return Rotation matrix derived from the normalized quaternion.
         */
        [[nodiscard]] constexpr mat4<T> rotationMatrix() const noexcept {

            const auto q_norm = normalized();
            const auto v      = q_norm.v();
            const auto w           = q_norm.w();

            const auto x2 = v[0] * v[0];
            const auto y2 = v[1] * v[1];
            const auto z2 = v[2] * v[2];
            const auto xy = v[0] * v[1];
            const auto xz = v[0] * v[2];
            const auto yz = v[1] * v[2];
            const auto wx =    w * v[0];
            const auto wy =    w * v[1];
            const auto wz =    w * v[2];

            return mat4<T>{
                static_cast<T>(1) - static_cast<T>(2) * (y2 + z2),
                static_cast<T>(2) * (xy + wz),
                static_cast<T>(2) * (xz - wy),
                static_cast<T>(0),

                static_cast<T>(2) * (xy - wz),
                static_cast<T>(1) - static_cast<T>(2) * (x2 + z2),
                static_cast<T>(2) * (yz + wx),
                static_cast<T>(0),

                static_cast<T>(2) * (xz + wy),
                static_cast<T>(2) * (yz - wx),
                static_cast<T>(1) - static_cast<T>(2) * (x2 + y2),
                static_cast<T>(0),

                static_cast<T>(0),
                static_cast<T>(0),
                static_cast<T>(0),
                static_cast<T>(1)
            };

        }

        /**
         * @brief Rotates a vector by this (normalized) quaternion.
         *
         * @param v Vector to rotate.
         * @return Rotated vector encoded in the result value.
         */
        constexpr quat<T> rotate(const vec3<T> v) const noexcept {
            const auto q_norm = normalized();
            const auto other = quat<T>{v, 0};

            return (q_norm * other * q_norm.conjugate()).v() ;
        }


    };



}
