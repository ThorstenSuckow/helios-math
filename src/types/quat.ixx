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
     * @brief Marker tag for intrinsic Euler-angle composition.
     */
    struct Intrinsic{};

    /**
     * @brief Marker tag for extrinsic Euler-angle composition.
     */
    struct Extrinsic{};

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
         * @brief Creates a quaternion from Euler angles.
         *
         * @tparam TFrameRelation Rotation composition mode (`Intrinsic` or `Extrinsic`).
         * @param xAngle Rotation angle around the x-axis in radians.
         * @param yAngle Rotation angle around the y-axis in radians.
         * @param zAngle Rotation angle around the z-axis in radians.
         * @return Quaternion representing the composed rotation.
         */
        template<typename TFrameRelation>
        requires (std::same_as<TFrameRelation, Intrinsic> || std::same_as<TFrameRelation, Extrinsic>)
        static constexpr quat<T> fromEulerAngles(const T xAngle, const T yAngle, const T zAngle) noexcept {

            const auto x = vec3<T>{static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)};
            const auto y = vec3<T>{static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)};
            const auto z = vec3<T>{static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)};

            if constexpr (std::same_as<TFrameRelation, Intrinsic>) {
                return quat<T>(x, xAngle) * quat<T>(y, yAngle) * quat<T>(z, zAngle);
            } else {
                return quat<T>(z, zAngle) * quat<T>(y, yAngle) * quat<T>(x, xAngle);
            }

        }

        /**
         * @brief Returns the identity quaternion.
         *
         * @return Quaternion representing no rotation.
         */
        static constexpr quat<T> identity() noexcept {
            return quat<T>{vec3<T>{static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)}, static_cast<T>(1)};
        }

        /**
         * @brief Creates a quaternion from axis-angle representation.
         *
         * @param v Rotation axis (normalized internally).
         * @param angle Rotation angle in radians.
         * @return Quaternion representing the axis-angle rotation.
         */
        static constexpr quat<T> fromAxisAngle(const helios::math::vec3f v, const float angle) noexcept {

            const auto rad = angle * static_cast<T>(0.5);

            return quat<T>{
                std::sin(rad) * v.normalize(),
                std::cos(rad)
            };

        }

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
            return quat<T>{v_ * static_cast<T>(-1), w_};
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
            return static_cast<T>(std::sqrt(
                v_[0] * v_[0] + v_[1] * v_[1] + v_[2] * v_[2] + w_ * w_
            ));
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
         * @return Rotated vector.
         */
        [[nodiscard]] constexpr vec3<T> rotate(const vec3<T> v) const noexcept {
            const auto q_norm = normalized();
            const auto other = quat<T>{v, 0};

            return (q_norm * other * q_norm.conjugate()).v() ;
        }


    };

    /**
     * @brief Single-precision quaternion.
     */
    using quatf = quat<float>;

    /**
     * @brief Double-precision quaternion.
     */
    using quatd = quat<double>;

}
