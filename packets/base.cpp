#include "base.hpp"
#include <cstring>

namespace CLifx {

// Bit-field layout constants for FrameHeader
static constexpr std::uint16_t LIFX_PROTOCOL      = 1024U;
static constexpr std::uint16_t PROTOCOL_MASK       = 0x0FFFU;
static constexpr std::uint16_t ADDRESSABLE_BIT_POS = 12U;
static constexpr std::uint16_t TAGGED_BIT_POS      = 13U;
static constexpr std::uint16_t ORIGIN_BIT_POS      = 14U;

// Bit-field layout constants for FrameAddress
static constexpr std::size_t   FRAME_ADDR_RESERVED = 6U;

// ── HSBK ─────────────────────────────────────────────────────────────────────

HSBK::HSBK() : hue(0), saturation(0), brightness(0), kelvin(0) {}

void HSBK::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, hue);
    write(buffer, offset, saturation);
    write(buffer, offset, brightness);
    write(buffer, offset, kelvin);
}

void HSBK::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    hue        = read<std::uint16_t>(buffer, offset);
    saturation = read<std::uint16_t>(buffer, offset);
    brightness = read<std::uint16_t>(buffer, offset);
    kelvin     = read<std::uint16_t>(buffer, offset);
}

// ── Tile ──────────────────────────────────────────────────────────────────────

Tile::Tile()
    : accel_meas_x(0), accel_meas_y(0), accel_meas_z(0),
      user_x(0.0F), user_y(0.0F),
      width(0), height(0),
      device_version_vendor(0), device_version_product(0),
      firmware_build(0), firmware_version_minor(0), firmware_version_major(0),
      reserved1(0), reserved2(0), reserved3(0), reserved4(0), reserved5(0) {}

void Tile::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, accel_meas_x);
    write(buffer, offset, accel_meas_y);
    write(buffer, offset, accel_meas_z);
    write(buffer, offset, static_cast<std::int16_t>(0));   // reserved1
    write(buffer, offset, user_x);
    write(buffer, offset, user_y);
    write(buffer, offset, width);
    write(buffer, offset, height);
    write(buffer, offset, static_cast<std::uint8_t>(0));   // reserved2
    write(buffer, offset, device_version_vendor);
    write(buffer, offset, device_version_product);
    write(buffer, offset, static_cast<std::uint32_t>(0));  // reserved3
    write(buffer, offset, firmware_build);
    write(buffer, offset, static_cast<std::uint64_t>(0));  // reserved4
    write(buffer, offset, firmware_version_minor);
    write(buffer, offset, firmware_version_major);
    write(buffer, offset, static_cast<std::uint32_t>(0));  // reserved5
}

void Tile::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    accel_meas_x           = read<std::int16_t>(buffer, offset);
    accel_meas_y           = read<std::int16_t>(buffer, offset);
    accel_meas_z           = read<std::int16_t>(buffer, offset);
    offset += sizeof(std::int16_t);                                  // reserved1
    user_x                 = read<float>(buffer, offset);
    user_y                 = read<float>(buffer, offset);
    width                  = read<std::uint8_t>(buffer, offset);
    height                 = read<std::uint8_t>(buffer, offset);
    offset += sizeof(std::uint8_t);                                  // reserved2
    device_version_vendor  = read<std::uint32_t>(buffer, offset);
    device_version_product = read<std::uint32_t>(buffer, offset);
    offset += sizeof(std::uint32_t);                                 // reserved3
    firmware_build         = read<std::uint64_t>(buffer, offset);
    offset += sizeof(std::uint64_t);                                 // reserved4
    firmware_version_minor = read<std::uint16_t>(buffer, offset);
    firmware_version_major = read<std::uint16_t>(buffer, offset);
    offset += sizeof(std::uint32_t);                                 // reserved5
}

// ── FrameHeader ───────────────────────────────────────────────────────────────

FrameHeader::FrameHeader()
    : size(0), protocol(LIFX_PROTOCOL), addressable(true), tagged(false),
      origin(0), source(0) {}

void FrameHeader::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, size);
    std::uint16_t packed = 0U;
    packed |= protocol & PROTOCOL_MASK;
    packed |= static_cast<std::uint16_t>(addressable ? 1U : 0U) << ADDRESSABLE_BIT_POS;
    packed |= static_cast<std::uint16_t>(tagged ? 1U : 0U)      << TAGGED_BIT_POS;
    packed |= static_cast<std::uint16_t>(origin & 0x3U)         << ORIGIN_BIT_POS;
    write(buffer, offset, packed);
    write(buffer, offset, source);
}

void FrameHeader::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    size        = read<std::uint16_t>(buffer, offset);
    auto packed = read<std::uint16_t>(buffer, offset);
    protocol    = packed & PROTOCOL_MASK;
    addressable = static_cast<bool>((packed >> ADDRESSABLE_BIT_POS) & 0x1U);
    tagged      = static_cast<bool>((packed >> TAGGED_BIT_POS)      & 0x1U);
    origin      = (packed >> ORIGIN_BIT_POS) & 0x3U;
    source      = read<std::uint32_t>(buffer, offset);
}

// ── FrameAddress ──────────────────────────────────────────────────────────────

FrameAddress::FrameAddress()
    : target(0), res_required(false), ack_required(false), sequence(0),
      reserved1{}, reserved2(0) {}

void FrameAddress::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, target);
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved1 (6 bytes)
    write(buffer, offset, static_cast<std::uint16_t>(0));
    std::uint8_t packed = 0U;
    packed |= static_cast<std::uint8_t>(res_required ? 1U : 0U);
    packed |= static_cast<std::uint8_t>(ack_required ? 1U : 0U) << 1U;
    write(buffer, offset, packed);
    write(buffer, offset, sequence);
}

void FrameAddress::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    target      = read<std::uint64_t>(buffer, offset);
    offset     += FRAME_ADDR_RESERVED;
    auto packed = read<std::uint8_t>(buffer, offset);
    res_required = static_cast<bool>(packed & 0x1U);
    ack_required = static_cast<bool>((packed >> 1U) & 0x1U);
    sequence    = read<std::uint8_t>(buffer, offset);
}

// ── ProtocolHeader ────────────────────────────────────────────────────────────

ProtocolHeader::ProtocolHeader() : type(0), reserved1(0), reserved2(0) {}

void ProtocolHeader::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint64_t>(0)); // reserved1
    write(buffer, offset, type);
    write(buffer, offset, static_cast<std::uint16_t>(0)); // reserved2
}

void ProtocolHeader::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    offset += sizeof(std::uint64_t);                       // reserved1
    type    = read<std::uint16_t>(buffer, offset);
    offset += sizeof(std::uint16_t);                       // reserved2
}

// ── Header ────────────────────────────────────────────────────────────────────

Header::Header() = default;

void Header::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    frame_header.Pack(buffer, offset);
    frame_address.Pack(buffer, offset);
    protocol_header.Pack(buffer, offset);
}

void Header::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    frame_header.Unpack(buffer, offset);
    frame_address.Unpack(buffer, offset);
    protocol_header.Unpack(buffer, offset);
}

// ── BasePayload ───────────────────────────────────────────────────────────────

BasePayload::BasePayload() = default;

// ── Packet ────────────────────────────────────────────────────────────────────

Packet::Packet() = default;

void Packet::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    size_t start = offset;
    header.Pack(buffer, offset);
    if (payload) { payload->Pack(buffer, offset); }
    auto total = static_cast<std::uint16_t>(offset - start);
    std::memcpy(buffer.data() + start, &total, sizeof(std::uint16_t));
}

void Packet::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    header.Unpack(buffer, offset);
    if (payload) { payload->Unpack(buffer, offset); }
}

} // namespace CLifx
