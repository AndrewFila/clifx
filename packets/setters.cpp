#include "setters.hpp"

namespace CLifx::Set {

// ── PowerPayload ──────────────────────────────────────────────────────────────

PowerPayload::PowerPayload() : level(0) {}

void PowerPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, level);
}

void PowerPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    level = read<std::uint16_t>(buffer, offset);
}

// ── LabelPayload ──────────────────────────────────────────────────────────────

LabelPayload::LabelPayload() : label{} {}

void LabelPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, label);
}

void LabelPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    label = read<std::array<char, LABEL_LENGTH>>(buffer, offset);
}

// ── RebootPayload ─────────────────────────────────────────────────────────────

RebootPayload::RebootPayload() = default;

void RebootPayload::Pack(std::vector<std::uint8_t> & /*buffer*/, size_t & /*offset*/) const {}

void RebootPayload::Unpack(const std::vector<std::uint8_t> & /*buffer*/, size_t & /*offset*/) {}

// ── LocationPayload ───────────────────────────────────────────────────────────

LocationPayload::LocationPayload() : location{}, label{}, updated_at(0) {}

void LocationPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, location);
    write(buffer, offset, label);
    write(buffer, offset, updated_at);
}

void LocationPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    location   = read<std::array<std::uint8_t, UUID_LENGTH>>(buffer, offset);
    label      = read<std::array<char, LABEL_LENGTH>>(buffer, offset);
    updated_at = read<std::uint64_t>(buffer, offset);
}

// ── GroupPayload ──────────────────────────────────────────────────────────────

GroupPayload::GroupPayload() : group{}, label{}, updated_at(0) {}

void GroupPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, group);
    write(buffer, offset, label);
    write(buffer, offset, updated_at);
}

void GroupPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    group      = read<std::array<std::uint8_t, UUID_LENGTH>>(buffer, offset);
    label      = read<std::array<char, LABEL_LENGTH>>(buffer, offset);
    updated_at = read<std::uint64_t>(buffer, offset);
}

// ── ColorPayload ──────────────────────────────────────────────────────────────

ColorPayload::ColorPayload() : duration(0), reserved(0) {}

void ColorPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint8_t>(0)); // reserved
    color.Pack(buffer, offset);
    write(buffer, offset, duration);
}

void ColorPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    offset += sizeof(std::uint8_t);                       // reserved
    color.Unpack(buffer, offset);
    duration = read<std::uint32_t>(buffer, offset);
}

// ── WaveformPayload ───────────────────────────────────────────────────────────

WaveformPayload::WaveformPayload()
    : transient(false), period(0), cycles(0.0F), skew_ratio(0),
      waveform(0), reserved(0) {}

void WaveformPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint8_t>(0)); // reserved
    write(buffer, offset, static_cast<std::uint8_t>(transient ? 1U : 0U));
    color.Pack(buffer, offset);
    write(buffer, offset, period);
    write(buffer, offset, cycles);
    write(buffer, offset, skew_ratio);
    write(buffer, offset, waveform);
}

void WaveformPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    offset    += sizeof(std::uint8_t);                    // reserved
    transient  = static_cast<bool>(read<std::uint8_t>(buffer, offset));
    color.Unpack(buffer, offset);
    period     = read<std::uint32_t>(buffer, offset);
    cycles     = read<float>(buffer, offset);
    skew_ratio = read<std::int16_t>(buffer, offset);
    waveform   = read<std::uint8_t>(buffer, offset);
}

// ── LightPowerPayload ─────────────────────────────────────────────────────────

LightPowerPayload::LightPowerPayload() : level(0), duration(0) {}

void LightPowerPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, level);
    write(buffer, offset, duration);
}

void LightPowerPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    level    = read<std::uint16_t>(buffer, offset);
    duration = read<std::uint32_t>(buffer, offset);
}

// ── WaveformOptionalPayload ───────────────────────────────────────────────────

WaveformOptionalPayload::WaveformOptionalPayload()
    : transient(false), period(0), cycles(0.0F), skew_ratio(0), waveform(0),
      set_hue(false), set_saturation(false), set_brightness(false),
      set_kelvin(false), reserved(0) {}

void WaveformOptionalPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint8_t>(0)); // reserved
    write(buffer, offset, static_cast<std::uint8_t>(transient ? 1U : 0U));
    color.Pack(buffer, offset);
    write(buffer, offset, period);
    write(buffer, offset, cycles);
    write(buffer, offset, skew_ratio);
    write(buffer, offset, waveform);
    write(buffer, offset, static_cast<std::uint8_t>(set_hue        ? 1U : 0U));
    write(buffer, offset, static_cast<std::uint8_t>(set_saturation ? 1U : 0U));
    write(buffer, offset, static_cast<std::uint8_t>(set_brightness ? 1U : 0U));
    write(buffer, offset, static_cast<std::uint8_t>(set_kelvin     ? 1U : 0U));
}

void WaveformOptionalPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    offset        += sizeof(std::uint8_t);                // reserved
    transient      = static_cast<bool>(read<std::uint8_t>(buffer, offset));
    color.Unpack(buffer, offset);
    period         = read<std::uint32_t>(buffer, offset);
    cycles         = read<float>(buffer, offset);
    skew_ratio     = read<std::int16_t>(buffer, offset);
    waveform       = read<std::uint8_t>(buffer, offset);
    set_hue        = static_cast<bool>(read<std::uint8_t>(buffer, offset));
    set_saturation = static_cast<bool>(read<std::uint8_t>(buffer, offset));
    set_brightness = static_cast<bool>(read<std::uint8_t>(buffer, offset));
    set_kelvin     = static_cast<bool>(read<std::uint8_t>(buffer, offset));
}

// ── InfraredPayload ───────────────────────────────────────────────────────────

InfraredPayload::InfraredPayload() : brightness(0) {}

void InfraredPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, brightness);
}

void InfraredPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    brightness = read<std::uint16_t>(buffer, offset);
}

// ── HevCyclePayload ───────────────────────────────────────────────────────────

HevCyclePayload::HevCyclePayload() : enable(false), duration_s(0) {}

void HevCyclePayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint8_t>(enable ? 1U : 0U));
    write(buffer, offset, duration_s);
}

void HevCyclePayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    enable     = static_cast<bool>(read<std::uint8_t>(buffer, offset));
    duration_s = read<std::uint32_t>(buffer, offset);
}

// ── HevCycleConfigurationPayload ──────────────────────────────────────────────

HevCycleConfigurationPayload::HevCycleConfigurationPayload()
    : indication(false), duration_s(0) {}

void HevCycleConfigurationPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint8_t>(indication ? 1U : 0U));
    write(buffer, offset, duration_s);
}

void HevCycleConfigurationPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    indication = static_cast<bool>(read<std::uint8_t>(buffer, offset));
    duration_s = read<std::uint32_t>(buffer, offset);
}

// ── ColorZonesPayload ─────────────────────────────────────────────────────────

ColorZonesPayload::ColorZonesPayload()
    : start_index(0), end_index(0), duration(0), apply(0) {}

void ColorZonesPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, start_index);
    write(buffer, offset, end_index);
    color.Pack(buffer, offset);
    write(buffer, offset, duration);
    write(buffer, offset, apply);
}

void ColorZonesPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    start_index = read<std::uint8_t>(buffer, offset);
    end_index   = read<std::uint8_t>(buffer, offset);
    color.Unpack(buffer, offset);
    duration    = read<std::uint32_t>(buffer, offset);
    apply       = read<std::uint8_t>(buffer, offset);
}

// ── MulitiZoneEffectPayload ───────────────────────────────────────────────────

MulitiZoneEffectPayload::MulitiZoneEffectPayload()
    : instanceid(0), type(0), speed(0), duration(0), parameters{},
      reserved1(0), reserved2(0), reserved3(0) {}

void MulitiZoneEffectPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, instanceid);
    write(buffer, offset, type);
    write(buffer, offset, static_cast<std::uint16_t>(0)); // reserved1
    write(buffer, offset, speed);
    write(buffer, offset, duration);
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved2
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved3
    for (const auto param : parameters) { write(buffer, offset, param); }
}

void MulitiZoneEffectPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    instanceid = read<std::uint32_t>(buffer, offset);
    type       = read<std::uint8_t>(buffer, offset);
    offset    += sizeof(std::uint16_t);                    // reserved1
    speed      = read<std::uint32_t>(buffer, offset);
    duration   = read<std::uint64_t>(buffer, offset);
    offset    += sizeof(std::uint32_t);                    // reserved2
    offset    += sizeof(std::uint32_t);                    // reserved3
    for (auto &param : parameters) { param = read<std::uint32_t>(buffer, offset); }
}

// ── ExtendedColorZonesPayload ─────────────────────────────────────────────────

ExtendedColorZonesPayload::ExtendedColorZonesPayload()
    : duration(0), apply(0), zone_index(0), colors_count(0), colors{} {}

void ExtendedColorZonesPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, duration);
    write(buffer, offset, apply);
    write(buffer, offset, zone_index);
    write(buffer, offset, colors_count);
    for (const auto &hsbk : colors) { hsbk.Pack(buffer, offset); }
}

void ExtendedColorZonesPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    duration     = read<std::uint32_t>(buffer, offset);
    apply        = read<std::uint8_t>(buffer, offset);
    zone_index   = read<std::uint16_t>(buffer, offset);
    colors_count = read<std::uint8_t>(buffer, offset);
    for (auto &hsbk : colors) { hsbk.Unpack(buffer, offset); }
}

// ── RPowerPayload ─────────────────────────────────────────────────────────────

RPowerPayload::RPowerPayload() : relay_index(0), level(0) {}

void RPowerPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, relay_index);
    write(buffer, offset, level);
}

void RPowerPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    relay_index = read<std::uint8_t>(buffer, offset);
    level       = read<std::uint16_t>(buffer, offset);
}

// ── UserPositionPayload ───────────────────────────────────────────────────────

UserPositionPayload::UserPositionPayload()
    : tile_index(0), user_x(0.0F), user_y(0.0F), reserved(0) {}

void UserPositionPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, tile_index);
    write(buffer, offset, static_cast<std::uint16_t>(0)); // reserved
    write(buffer, offset, user_x);
    write(buffer, offset, user_y);
}

void UserPositionPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    tile_index  = read<std::uint8_t>(buffer, offset);
    offset     += sizeof(std::uint16_t);                   // reserved
    user_x      = read<float>(buffer, offset);
    user_y      = read<float>(buffer, offset);
}

// ── Set64Payload ──────────────────────────────────────────────────────────────

Set64Payload::Set64Payload()
    : tile_index(0), length(0), x(0), y(0), width(0), duration(0),
      colors{}, reserved(0) {}

void Set64Payload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, tile_index);
    write(buffer, offset, length);
    write(buffer, offset, static_cast<std::uint8_t>(0)); // reserved
    write(buffer, offset, x);
    write(buffer, offset, y);
    write(buffer, offset, width);
    write(buffer, offset, duration);
    for (const auto &hsbk : colors) { hsbk.Pack(buffer, offset); }
}

void Set64Payload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    tile_index = read<std::uint8_t>(buffer, offset);
    length     = read<std::uint8_t>(buffer, offset);
    offset    += sizeof(std::uint8_t);                    // reserved
    x          = read<std::uint8_t>(buffer, offset);
    y          = read<std::uint8_t>(buffer, offset);
    width      = read<std::uint8_t>(buffer, offset);
    duration   = read<std::uint32_t>(buffer, offset);
    for (auto &hsbk : colors) { hsbk.Unpack(buffer, offset); }
}

// ── TileEffectPayload ─────────────────────────────────────────────────────────

TileEffectPayload::TileEffectPayload()
    : instanceid(0), type(0), speed(0), duration(0), palette_count(0),
      palette{}, reserved1(0), reserved2(0), reserved3(0), reserved4(0) {}

void TileEffectPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint8_t>(0));  // reserved1
    write(buffer, offset, static_cast<std::uint8_t>(0));  // reserved2
    write(buffer, offset, instanceid);
    write(buffer, offset, type);
    write(buffer, offset, speed);
    write(buffer, offset, duration);
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved3
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved4
    write(buffer, offset, palette_count);
    for (const auto &hsbk : palette) { hsbk.Pack(buffer, offset); }
}

void TileEffectPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    offset       += sizeof(std::uint8_t);                  // reserved1
    offset       += sizeof(std::uint8_t);                  // reserved2
    instanceid    = read<std::uint32_t>(buffer, offset);
    type          = read<std::uint8_t>(buffer, offset);
    speed         = read<std::uint32_t>(buffer, offset);
    duration      = read<std::uint64_t>(buffer, offset);
    offset       += sizeof(std::uint32_t);                 // reserved3
    offset       += sizeof(std::uint32_t);                 // reserved4
    palette_count = read<std::uint8_t>(buffer, offset);
    for (auto &hsbk : palette) { hsbk.Unpack(buffer, offset); }
}

} // namespace CLifx::Set
