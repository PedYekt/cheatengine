#include "cheatengine/memory/value_types.hpp"

namespace cheatengine {

SearchValue SearchValue::fromInt32(std::int32_t value)
{
    SearchValue sv;
    sv.type_ = ValueType::INT32;
    sv.data_ = toBytes(value);
    return sv;
}

SearchValue SearchValue::fromInt64(std::int64_t value)
{
    SearchValue sv;
    sv.type_ = ValueType::INT64;
    sv.data_ = toBytes(value);
    return sv;
}

SearchValue SearchValue::fromFloat32(float value)
{
    SearchValue sv;
    sv.type_ = ValueType::FLOAT32;
    sv.data_ = toBytes(value);
    return sv;
}

SearchValue SearchValue::fromFloat64(double value)
{
    SearchValue sv;
    sv.type_ = ValueType::FLOAT64;
    sv.data_ = toBytes(value);
    return sv;
}

SearchValue SearchValue::fromBytes(const std::vector<std::uint8_t>& bytes)
{
    SearchValue sv;
    sv.type_ = ValueType::BYTES;
    sv.data_ = bytes;
    return sv;
}

} // namespace cheatengine
