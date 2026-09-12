#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

template <typename T> std::vector<std::uint8_t> PackToBuffer(const T &value) {
    std::vector<std::uint8_t> buffer;
    std::size_t offset = 0;
    value.Pack(buffer, offset);
    return buffer;
}

template <typename T> T UnpackFromBuffer(const std::vector<std::uint8_t> &buffer) {
    T value;
    std::size_t offset = 0;
    value.Unpack(buffer, offset);
    return value;
}
