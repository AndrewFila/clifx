#include "base.hpp"
#include <array>

namespace CLifx::Set {

// Device
class PowerPayload : public BasePayload {
  public:
    PowerPayload();
    static constexpr std::uint16_t MessageType = 21; // SetPower
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t level;
};

class LabelPayload : public BasePayload {
  public:
    LabelPayload();
    static constexpr std::uint16_t MessageType = 24; // SetLabel
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::array<char, LABEL_LENGTH> label;
};

class RebootPayload : public BasePayload {
  public:
    RebootPayload();
    static constexpr std::uint16_t MessageType = 38; // SetReboot
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
};

class LocationPayload : public BasePayload {
  public:
    LocationPayload();
    static constexpr std::uint16_t MessageType = 49; // SetLocation
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::array<std::uint8_t, UUID_LENGTH> location;
    std::array<char, LABEL_LENGTH>        label;
    std::uint64_t                         updated_at;
};

class GroupPayload : public BasePayload {
  public:
    GroupPayload();
    static constexpr std::uint16_t MessageType = 52; // SetGroup
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::array<std::uint8_t, UUID_LENGTH> group;
    std::array<char, LABEL_LENGTH>        label;
    std::uint64_t                         updated_at;
};

// Light
class ColorPayload : public BasePayload {
  public:
    ColorPayload();
    static constexpr std::uint16_t MessageType = 102; // SetColor
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    HSBK          color;
    std::uint32_t duration;
  private:
    std::uint8_t reserved;
};

class WaveformPayload : public BasePayload {
  public:
    WaveformPayload();
    static constexpr std::uint16_t MessageType = 103; // SetWaveform
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    bool          transient;
    HSBK          color;
    std::uint32_t period;
    float         cycles;
    std::int16_t  skew_ratio;
    std::uint8_t  waveform;
  private:
    std::uint8_t reserved;
};

class LightPowerPayload : public BasePayload {
  public:
    LightPowerPayload();
    static constexpr std::uint16_t MessageType = 117; // SetLightPower
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t level;
    std::uint32_t duration;
};

class WaveformOptionalPayload : public BasePayload {
  public:
    WaveformOptionalPayload();
    static constexpr std::uint16_t MessageType = 119; // SetWaveformOptional
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    bool          transient;
    HSBK          color;
    std::uint32_t period;
    float         cycles;
    std::int16_t  skew_ratio;
    std::uint8_t  waveform;
    bool          set_hue;
    bool          set_saturation;
    bool          set_brightness;
    bool          set_kelvin;
  private:
    std::uint8_t reserved;
};

class InfraredPayload : public BasePayload {
  public:
    InfraredPayload();
    static constexpr std::uint16_t MessageType = 122; // SetInfrared
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t brightness;
};

class HevCyclePayload : public BasePayload {
  public:
    HevCyclePayload();
    static constexpr std::uint16_t MessageType = 143; // SetHevCycle
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    bool          enable;
    std::uint32_t duration_s;
};

class HevCycleConfigurationPayload : public BasePayload {
  public:
    HevCycleConfigurationPayload();
    static constexpr std::uint16_t MessageType = 146; // SetHevCycleConfiguration
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    bool          indication;
    std::uint32_t duration_s;
};

// MultiZone
class ColorZonesPayload : public BasePayload {
  public:
    ColorZonesPayload();
    static constexpr std::uint16_t MessageType = 501; // SetColorZones
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t  start_index;
    std::uint8_t  end_index;
    HSBK          color;
    std::uint32_t duration;
    std::uint8_t  apply;
};

class MulitiZoneEffectPayload : public BasePayload {
  public:
    MulitiZoneEffectPayload();
    static constexpr std::uint16_t MessageType = 508; // SetMultiZoneEffect
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint32_t                                    instanceid;
    std::uint8_t                                     type;
    std::uint32_t                                    speed;
    std::uint64_t                                    duration;
    std::array<std::uint32_t, MULTIZONE_PARAM_COUNT> parameters;
  private:
    std::uint16_t reserved1;
    std::uint32_t reserved2;
    std::uint32_t reserved3;
};

class ExtendedColorZonesPayload : public BasePayload {
  public:
    ExtendedColorZonesPayload();
    static constexpr std::uint16_t MessageType = 510; // SetExtendedColorZones
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint32_t                          duration;
    std::uint8_t                           apply;
    std::uint16_t                          zone_index;
    std::uint8_t                           colors_count;
    std::array<HSBK, EXTENDED_COLOR_COUNT> colors;
};

// Relay
class RPowerPayload : public BasePayload {
  public:
    RPowerPayload();
    static constexpr std::uint16_t MessageType = 817; // SetRPower
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t  relay_index;
    std::uint16_t level;
};

// Tile
class UserPositionPayload : public BasePayload {
  public:
    UserPositionPayload();
    static constexpr std::uint16_t MessageType = 703; // SetUserPosition
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t tile_index;
    float        user_x;
    float        user_y;
  private:
    std::uint16_t reserved;
};

class Set64Payload : public BasePayload {
  public:
    Set64Payload();
    static constexpr std::uint16_t MessageType = 715; // Set64
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint8_t                        tile_index;
    std::uint8_t                        length;
    std::uint8_t                        x;
    std::uint8_t                        y;
    std::uint8_t                        width;
    std::uint32_t                       duration;
    std::array<HSBK, TILE_COLOR_COUNT>  colors;
  private:
    std::uint8_t reserved;
};

class TileEffectPayload : public BasePayload {
  public:
    TileEffectPayload();
    static constexpr std::uint16_t MessageType = 719; // SetTileEffect
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
    std::uint8_t  reserved2;
    std::uint32_t reserved3;
    std::uint32_t reserved4;
};

} // namespace CLifx::Set
