#include "../utils/packable.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace CLifx {

// Protocol-defined fixed sizes
inline constexpr std::size_t LABEL_LENGTH = 32;
inline constexpr std::size_t UUID_LENGTH = 16;
inline constexpr std::size_t ECHO_PAYLOAD_LENGTH = 64;
inline constexpr std::size_t MULTIZONE_COLOR_COUNT = 8;
inline constexpr std::size_t EXTENDED_COLOR_COUNT = 82;
inline constexpr std::size_t TILE_COUNT = 16;
inline constexpr std::size_t TILE_PALETTE_COUNT = 16;
inline constexpr std::size_t TILE_COLOR_COUNT = 64;
inline constexpr std::size_t MULTIZONE_PARAM_COUNT = 8;

class HSBK : public Utils::Packable {
  public:
    HSBK();
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t hue;
    std::uint16_t saturation;
    std::uint16_t brightness;
    std::uint16_t kelvin;
};

class Tile : public Utils::Packable {
  public:
    Tile();
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::int16_t accel_meas_x;
    std::int16_t accel_meas_y;
    std::int16_t accel_meas_z;
    float user_x;
    float user_y;
    std::uint8_t width;
    std::uint8_t height;
    std::uint32_t device_version_vendor;
    std::uint32_t device_version_product;
    std::uint64_t firmware_build;
    std::uint16_t firmware_version_minor;
    std::uint16_t firmware_version_major;

  private:
    std::int16_t reserved1;
    std::uint8_t reserved2;
    std::uint32_t reserved3;
    std::uint64_t reserved4;
    std::uint32_t reserved5;
};

class FrameHeader : public Utils::Packable {
  public:
    FrameHeader();
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t size;
    std::uint16_t protocol;
    bool addressable;    // 1bit
    bool tagged;         // 1bit
    std::uint8_t origin; // 2bit
    std::uint32_t source;
};
class FrameAddress : public Utils::Packable {
  public:
    FrameAddress();
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint64_t target;
    bool res_required;
    bool ack_required;
    std::uint8_t sequence;

  private:
    std::uint8_t reserved1[6];
    std::uint8_t reserved2;
};
class ProtocolHeader : public Utils::Packable {
  public:
    ProtocolHeader();
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    std::uint16_t type;

  private:
    std::uint64_t reserved1;
    std::uint16_t reserved2;
};

class Header : public Utils::Packable {
  public:
    Header();
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    FrameHeader frame_header;
    FrameAddress frame_address;
    ProtocolHeader protocol_header;
};

class BasePayload : public Utils::Packable {
  public:
    BasePayload();
    virtual ~BasePayload() = default;
    virtual void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override = 0;
    virtual void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override = 0;
};

class Packet : public Utils::Packable {
  public:
    Packet();
    void Pack(std::vector<std::uint8_t> &buffer, size_t &offset) const override;
    void Unpack(const std::vector<std::uint8_t> &buffer, size_t &offset) override;
    template <typename T> T *setPayload() {
        header.protocol_header.type = T::MessageType;
        this->payload = std::make_unique<T>();
        return static_cast<T *>(payload.get());
    }
    Header header;
    std::unique_ptr<BasePayload> payload;
};
} // namespace CLifx
