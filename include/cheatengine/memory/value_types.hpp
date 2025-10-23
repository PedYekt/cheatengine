#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

namespace cheatengine {

enum class ValueType {
    INT32,
    INT64,
    FLOAT32,
    FLOAT64,
    BYTES
};

class SearchValue {
public:
    ValueType type() const noexcept { return type_; }
    const std::vector<std::uint8_t>& data() const noexcept { return data_; }

    static SearchValue fromInt32(std::int32_t value);
    static SearchValue fromInt64(std::int64_t value);
    static SearchValue fromFloat32(float value);
    static SearchValue fromFloat64(double value);
    static SearchValue fromBytes(const std::vector<std::uint8_t>& bytes);

    template <typename T>
    static SearchValue create(T value)
    {
        if constexpr (std::is_integral_v<T>) {
            if constexpr (sizeof(T) == sizeof(std::int32_t)) {
                return fromInt32(static_cast<std::int32_t>(value));
            } else if constexpr (sizeof(T) == sizeof(std::int64_t)) {
                return fromInt64(static_cast<std::int64_t>(value));
            } else {
                static_assert(always_false<T>::value, "Unsupported integral size for SearchValue::create");
            }
        } else if constexpr (std::is_floating_point_v<T>) {
            if constexpr (sizeof(T) == sizeof(float)) {
                return fromFloat32(static_cast<float>(value));
            } else if constexpr (sizeof(T) == sizeof(double)) {
                return fromFloat64(static_cast<double>(value));
            } else {
                static_assert(always_false<T>::value, "Unsupported floating-point size for SearchValue::create");
            }
        } else {
            static_assert(always_false<T>::value, "Unsupported type for SearchValue::create");
        }
    }

private:
    template <typename>
    struct always_false : std::false_type {
    };

    template <typename T>
    static std::vector<std::uint8_t> toBytes(const T& value)
    {
        std::vector<std::uint8_t> bytes(sizeof(T));
        std::memcpy(bytes.data(), &value, sizeof(T));
        return bytes;
    }

    ValueType type_{ValueType::BYTES};
    std::vector<std::uint8_t> data_;
};

} // namespace cheatengine
