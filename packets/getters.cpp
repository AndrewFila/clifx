#include "getters.hpp"

namespace CLifx::Get {

// ── ServicePayload ────────────────────────────────────────────────────────────

ServicePayload::ServicePayload() : service(0), port(0) {}

void ServicePayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, service);
    write(buffer, offset, port);
}

void ServicePayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    service = read<std::uint8_t>(buffer, offset);
    port    = read<std::uint32_t>(buffer, offset);
}

// ── HostFirmwarePayload ───────────────────────────────────────────────────────

HostFirmwarePayload::HostFirmwarePayload()
    : build(0), version_minor(0), version_major(0), reserved(0) {}

void HostFirmwarePayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, build);
    write(buffer, offset, static_cast<std::uint64_t>(0)); // reserved
    write(buffer, offset, version_minor);
    write(buffer, offset, version_major);
}

void HostFirmwarePayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    build         = read<std::uint64_t>(buffer, offset);
    offset       += sizeof(std::uint64_t);                 // reserved
    version_minor = read<std::uint16_t>(buffer, offset);
    version_major = read<std::uint16_t>(buffer, offset);
}

// ── WifiInfoPayload ───────────────────────────────────────────────────────────

WifiInfoPayload::WifiInfoPayload()
    : signal(0.0F), reserved1(0), reserved2(0), reserved3(0) {}

void WifiInfoPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, signal);
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved1
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved2
    write(buffer, offset, static_cast<std::int16_t>(0));  // reserved3
}

void WifiInfoPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    signal  = read<float>(buffer, offset);
    offset += sizeof(std::uint32_t);                       // reserved1
    offset += sizeof(std::uint32_t);                       // reserved2
    offset += sizeof(std::int16_t);                        // reserved3
}

// ── WifiFirmwarePayload ───────────────────────────────────────────────────────

WifiFirmwarePayload::WifiFirmwarePayload()
    : build(0), version_minor(0), version_major(0), reserved(0) {}

void WifiFirmwarePayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, build);
    write(buffer, offset, static_cast<std::uint64_t>(0)); // reserved
    write(buffer, offset, version_minor);
    write(buffer, offset, version_major);
}

void WifiFirmwarePayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    build         = read<std::uint64_t>(buffer, offset);
    offset       += sizeof(std::uint64_t);                 // reserved
    version_minor = read<std::uint16_t>(buffer, offset);
    version_major = read<std::uint16_t>(buffer, offset);
}

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

// ── VersionPayload ────────────────────────────────────────────────────────────

VersionPayload::VersionPayload() : vendor(0), product(0), reserved(0) {}

void VersionPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, vendor);
    write(buffer, offset, product);
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved
}

void VersionPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    vendor  = read<std::uint32_t>(buffer, offset);
    product = read<std::uint32_t>(buffer, offset);
    offset += sizeof(std::uint32_t);                       // reserved
}

// ── InfoPayload ───────────────────────────────────────────────────────────────

InfoPayload::InfoPayload() : time(0), uptime(0), downtime(0) {}

void InfoPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, time);
    write(buffer, offset, uptime);
    write(buffer, offset, downtime);
}

void InfoPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    time     = read<std::uint64_t>(buffer, offset);
    uptime   = read<std::uint64_t>(buffer, offset);
    downtime = read<std::uint64_t>(buffer, offset);
}

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

// ── EchoRequestPayload ────────────────────────────────────────────────────────

EchoRequestPayload::EchoRequestPayload() : payload{} {}

void EchoRequestPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, payload);
}

void EchoRequestPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    payload = read<std::array<std::uint8_t, ECHO_PAYLOAD_LENGTH>>(buffer, offset);
}

// ── ColorPayload (LightState) ─────────────────────────────────────────────────

ColorPayload::ColorPayload()
    : power(0), label{}, reserved1(0), reserved2(0) {}

void ColorPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    color.Pack(buffer, offset);
    write(buffer, offset, static_cast<std::int16_t>(0));  // reserved1
    write(buffer, offset, power);
    write(buffer, offset, label);
    write(buffer, offset, static_cast<std::uint64_t>(0)); // reserved2
}

void ColorPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    color.Unpack(buffer, offset);
    offset += sizeof(std::int16_t);                        // reserved1
    power   = read<std::uint16_t>(buffer, offset);
    label   = read<std::array<char, LABEL_LENGTH>>(buffer, offset);
    offset += sizeof(std::uint64_t);                       // reserved2
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

HevCyclePayload::HevCyclePayload() : duration_s(0), remaining_s(0), last_power(false) {}

void HevCyclePayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, duration_s);
    write(buffer, offset, remaining_s);
    write(buffer, offset, static_cast<std::uint8_t>(last_power ? 1U : 0U));
}

void HevCyclePayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    duration_s  = read<std::uint32_t>(buffer, offset);
    remaining_s = read<std::uint32_t>(buffer, offset);
    last_power  = static_cast<bool>(read<std::uint8_t>(buffer, offset));
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

// ── LastHevCycleResultPayload ─────────────────────────────────────────────────

LastHevCycleResultPayload::LastHevCycleResultPayload() : result(0) {}

void LastHevCycleResultPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, result);
}

void LastHevCycleResultPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    result = read<std::uint8_t>(buffer, offset);
}

// ── ColorZonesPayload (StateMultiZone) ────────────────────────────────────────

ColorZonesPayload::ColorZonesPayload() : count(0), index(0), colors{} {}

void ColorZonesPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, count);
    write(buffer, offset, index);
    for (const auto &hsbk : colors) { hsbk.Pack(buffer, offset); }
}

void ColorZonesPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    count = read<std::uint8_t>(buffer, offset);
    index = read<std::uint8_t>(buffer, offset);
    for (auto &hsbk : colors) { hsbk.Unpack(buffer, offset); }
}

// ── MultiZoneEffectPayload ────────────────────────────────────────────────────

MultiZoneEffectPayload::MultiZoneEffectPayload()
    : instanceid(0), type(0), speed(0), duration(0), parameters{},
      reserved1(0), reserved2(0), reserved3(0) {}

void MultiZoneEffectPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, instanceid);
    write(buffer, offset, type);
    write(buffer, offset, static_cast<std::uint16_t>(0)); // reserved1
    write(buffer, offset, speed);
    write(buffer, offset, duration);
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved2
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved3
    for (const auto param : parameters) { write(buffer, offset, param); }
}

void MultiZoneEffectPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    instanceid  = read<std::uint32_t>(buffer, offset);
    type        = read<std::uint8_t>(buffer, offset);
    offset     += sizeof(std::uint16_t);                   // reserved1
    speed       = read<std::uint32_t>(buffer, offset);
    duration    = read<std::uint64_t>(buffer, offset);
    offset     += sizeof(std::uint32_t);                   // reserved2
    offset     += sizeof(std::uint32_t);                   // reserved3
    for (auto &param : parameters) { param = read<std::uint32_t>(buffer, offset); }
}

// ── ExtendedColorZonesPayload ─────────────────────────────────────────────────

ExtendedColorZonesPayload::ExtendedColorZonesPayload()
    : zones_count(0), zone_index(0), colors_count(0), colors{} {}

void ExtendedColorZonesPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, zones_count);
    write(buffer, offset, zone_index);
    write(buffer, offset, colors_count);
    for (const auto &hsbk : colors) { hsbk.Pack(buffer, offset); }
}

void ExtendedColorZonesPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    zones_count  = read<std::uint16_t>(buffer, offset);
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

// ── DeviceChainPayload ────────────────────────────────────────────────────────

DeviceChainPayload::DeviceChainPayload()
    : start_index(0), tile_devices{}, tile_devices_count(0) {}

void DeviceChainPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, start_index);
    for (const auto &tile : tile_devices) { tile.Pack(buffer, offset); }
    write(buffer, offset, tile_devices_count);
}

void DeviceChainPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    start_index = read<std::uint8_t>(buffer, offset);
    for (auto &tile : tile_devices) { tile.Unpack(buffer, offset); }
    tile_devices_count = read<std::uint8_t>(buffer, offset);
}

// ── Get64Payload ──────────────────────────────────────────────────────────────

Get64Payload::Get64Payload()
    : tile_index(0), length(0), x(0), y(0), width(0), reserved(0) {}

void Get64Payload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, tile_index);
    write(buffer, offset, length);
    write(buffer, offset, static_cast<std::uint8_t>(0)); // reserved
    write(buffer, offset, x);
    write(buffer, offset, y);
    write(buffer, offset, width);
}

void Get64Payload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    tile_index = read<std::uint8_t>(buffer, offset);
    length     = read<std::uint8_t>(buffer, offset);
    offset    += sizeof(std::uint8_t);                    // reserved
    x          = read<std::uint8_t>(buffer, offset);
    y          = read<std::uint8_t>(buffer, offset);
    width      = read<std::uint8_t>(buffer, offset);
}

// ── TileEffectPayload (StateTileEffect) ───────────────────────────────────────

TileEffectPayload::TileEffectPayload()
    : instanceid(0), type(0), speed(0), duration(0), palette_count(0), palette{},
      reserved1(0), reserved2(0), reserved3(0) {}

void TileEffectPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, static_cast<std::uint8_t>(0));  // reserved1
    write(buffer, offset, instanceid);
    write(buffer, offset, type);
    write(buffer, offset, speed);
    write(buffer, offset, duration);
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved2
    write(buffer, offset, static_cast<std::uint32_t>(0)); // reserved3
    write(buffer, offset, palette_count);
    for (const auto &hsbk : palette) { hsbk.Pack(buffer, offset); }
}

void TileEffectPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    offset       += sizeof(std::uint8_t);                  // reserved1
    instanceid    = read<std::uint32_t>(buffer, offset);
    type          = read<std::uint8_t>(buffer, offset);
    speed         = read<std::uint32_t>(buffer, offset);
    duration      = read<std::uint64_t>(buffer, offset);
    offset       += sizeof(std::uint32_t);                 // reserved2
    offset       += sizeof(std::uint32_t);                 // reserved3
    palette_count = read<std::uint8_t>(buffer, offset);
    for (auto &hsbk : palette) { hsbk.Unpack(buffer, offset); }
}

// ── SensorAmbientLightPayload ─────────────────────────────────────────────────

SensorAmbientLightPayload::SensorAmbientLightPayload() : lux(0.0F) {}

void SensorAmbientLightPayload::Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const {
    write(buffer, offset, lux);
}

void SensorAmbientLightPayload::Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) {
    lux = read<float>(buffer, offset);
}

} // namespace CLifx::Get
