#include "base.hpp"
#include <array>

namespace CLifx::Get {

// Discovery
class ServicePayload : public BasePayload {
  public:
    ServicePayload();
    static constexpr std::uint16_t MessageType = 3; // StateService
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t  service;
    std::uint32_t port;
};

// Device
class HostFirmwarePayload : public BasePayload {
  public:
    HostFirmwarePayload();
    static constexpr std::uint16_t MessageType = 15; // StateHostFirmware
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint64_t build;
    std::uint16_t version_minor;
    std::uint16_t version_major;
  private:
    std::uint64_t reserved;
};

class WifiInfoPayload : public BasePayload {
  public:
    WifiInfoPayload();
    static constexpr std::uint16_t MessageType = 17; // StateWifiInfo
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    float signal;
  private:
    std::uint32_t reserved1;
    std::uint32_t reserved2;
    std::int16_t  reserved3;
};

class WifiFirmwarePayload : public BasePayload {
  public:
    WifiFirmwarePayload();
    static constexpr std::uint16_t MessageType = 19; // StateWifiFirmware
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint64_t build;
    std::uint16_t version_minor;
    std::uint16_t version_major;
  private:
    std::uint64_t reserved;
};

class PowerPayload : public BasePayload {
  public:
    PowerPayload();
    static constexpr std::uint16_t MessageType = 22; // StatePower
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t level;
};

class LabelPayload : public BasePayload {
  public:
    LabelPayload();
    static constexpr std::uint16_t MessageType = 25; // StateLabel
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::array<char, LABEL_LENGTH> label;
};

class VersionPayload : public BasePayload {
  public:
    VersionPayload();
    static constexpr std::uint16_t MessageType = 33; // StateVersion
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint32_t vendor;
    std::uint32_t product;
  private:
    std::uint32_t reserved;
};

class InfoPayload : public BasePayload {
  public:
    InfoPayload();
    static constexpr std::uint16_t MessageType = 35; // StateInfo
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint64_t time;
    std::uint64_t uptime;
    std::uint64_t downtime;
};

class LocationPayload : public BasePayload {
  public:
    LocationPayload();
    static constexpr std::uint16_t MessageType = 50; // StateLocation
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::array<std::uint8_t, UUID_LENGTH>  location;
    std::array<char, LABEL_LENGTH>         label;
    std::uint64_t                          updated_at;
};

class GroupPayload : public BasePayload {
  public:
    GroupPayload();
    static constexpr std::uint16_t MessageType = 53; // StateGroup
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::array<std::uint8_t, UUID_LENGTH>  group;
    std::array<char, LABEL_LENGTH>         label;
    std::uint64_t                          updated_at;
};

class EchoRequestPayload : public BasePayload {
  public:
    EchoRequestPayload();
    static constexpr std::uint16_t MessageType = 58; // EchoRequest
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::array<std::uint8_t, ECHO_PAYLOAD_LENGTH> payload;
};

// Light
class ColorPayload : public BasePayload {
  public:
    ColorPayload();
    static constexpr std::uint16_t MessageType = 107; // LightState
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    HSBK                           color;
    std::uint16_t                  power;
    std::array<char, LABEL_LENGTH> label;
  private:
    std::int16_t  reserved1;
    std::uint64_t reserved2;
};

class InfraredPayload : public BasePayload {
  public:
    InfraredPayload();
    static constexpr std::uint16_t MessageType = 121; // StateInfrared
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t brightness;
};

class HevCyclePayload : public BasePayload {
  public:
    HevCyclePayload();
    static constexpr std::uint16_t MessageType = 144; // StateHevCycle
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint32_t duration_s;
    std::uint32_t remaining_s;
    bool          last_power;
};

class HevCycleConfigurationPayload : public BasePayload {
  public:
    HevCycleConfigurationPayload();
    static constexpr std::uint16_t MessageType = 147; // StateHevCycleConfiguration
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    bool          indication;
    std::uint32_t duration_s;
};

class LastHevCycleResultPayload : public BasePayload {
  public:
    LastHevCycleResultPayload();
    static constexpr std::uint16_t MessageType = 149; // StateLastHevCycleResult
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t result;
};

// MultiZone
class ColorZonesPayload : public BasePayload {
  public:
    ColorZonesPayload();
    static constexpr std::uint16_t MessageType = 506; // StateMultiZone
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t                           count;
    std::uint8_t                           index;
    std::array<HSBK, MULTIZONE_COLOR_COUNT> colors;
};

class MultiZoneEffectPayload : public BasePayload {
  public:
    MultiZoneEffectPayload();
    static constexpr std::uint16_t MessageType = 509; // StateMultiZoneEffect
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint32_t                                  instanceid;
    std::uint8_t                                   type;
    std::uint32_t                                  speed;
    std::uint64_t                                  duration;
    std::array<std::uint32_t, MULTIZONE_PARAM_COUNT> parameters;
  private:
    std::uint16_t reserved1;
    std::uint32_t reserved2;
    std::uint32_t reserved3;
};

class ExtendedColorZonesPayload : public BasePayload {
  public:
    ExtendedColorZonesPayload();
    static constexpr std::uint16_t MessageType = 512; // StateExtendedColorZones
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t                          zones_count;
    std::uint16_t                          zone_index;
    std::uint8_t                           colors_count;
    std::array<HSBK, EXTENDED_COLOR_COUNT> colors;
};

// Relay
class RPowerPayload : public BasePayload {
  public:
    RPowerPayload();
    static constexpr std::uint16_t MessageType = 818; // StateRPower
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t  relay_index;
    std::uint16_t level;
};

// Tile
class DeviceChainPayload : public BasePayload {
  public:
    DeviceChainPayload();
    static constexpr std::uint16_t MessageType = 702; // StateDeviceChain
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t              start_index;
    std::array<Tile, TILE_COUNT> tile_devices;
    std::uint8_t              tile_devices_count;
};

class Get64Payload : public BasePayload {
  public:
    Get64Payload();
    static constexpr std::uint16_t MessageType = 707; // Get64
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t tile_index;
    std::uint8_t length;
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t width;
  private:
    std::uint8_t reserved;
};

class TileEffectPayload : public BasePayload {
  public:
    TileEffectPayload();
    static constexpr std::uint16_t MessageType = 720; // StateTileEffect
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint32_t                        instanceid;
    std::uint8_t                         type;
    std::uint32_t                        speed;
    std::uint64_t                        duration;
    std::uint8_t                         palette_count;
    std::array<HSBK, TILE_PALETTE_COUNT> palette;
  private:
    std::uint8_t  reserved1;
    std::uint32_t reserved2;
    std::uint32_t reserved3;
};

class SensorAmbientLightPayload : public BasePayload {
  public:
    SensorAmbientLightPayload();
    static constexpr std::uint16_t MessageType = 402; // StateSensorAmbientLight
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    float lux;
};

} // namespace CLifx::Get
