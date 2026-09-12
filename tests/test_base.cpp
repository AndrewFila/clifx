#include "packets/base.hpp"
#include "test_helpers.hpp"
#include <gtest/gtest.h>

using CLifx::FrameAddress;
using CLifx::FrameHeader;
using CLifx::Header;
using CLifx::HSBK;
using CLifx::Packet;
using CLifx::ProtocolHeader;
using CLifx::Tile;

TEST(HSBKTest, RoundTrip) {
    HSBK original;
    original.hue = 1111;
    original.saturation = 2222;
    original.brightness = 3333;
    original.kelvin = 4444;

    auto buffer = PackToBuffer(original);
    EXPECT_EQ(buffer.size(), 8U);

    auto restored = UnpackFromBuffer<HSBK>(buffer);
    EXPECT_EQ(restored.hue, 1111);
    EXPECT_EQ(restored.saturation, 2222);
    EXPECT_EQ(restored.brightness, 3333);
    EXPECT_EQ(restored.kelvin, 4444);
}

TEST(TileTest, RoundTrip) {
    Tile original;
    original.accel_meas_x = -100;
    original.accel_meas_y = 200;
    original.accel_meas_z = -300;
    original.user_x = 1.5F;
    original.user_y = -2.5F;
    original.width = 8;
    original.height = 8;
    original.device_version_vendor = 1;
    original.device_version_product = 55;
    original.firmware_build = 123456789ULL;
    original.firmware_version_minor = 3;
    original.firmware_version_major = 2;

    auto buffer = PackToBuffer(original);
    auto restored = UnpackFromBuffer<Tile>(buffer);

    EXPECT_EQ(restored.accel_meas_x, -100);
    EXPECT_EQ(restored.accel_meas_y, 200);
    EXPECT_EQ(restored.accel_meas_z, -300);
    EXPECT_FLOAT_EQ(restored.user_x, 1.5F);
    EXPECT_FLOAT_EQ(restored.user_y, -2.5F);
    EXPECT_EQ(restored.width, 8);
    EXPECT_EQ(restored.height, 8);
    EXPECT_EQ(restored.device_version_vendor, 1U);
    EXPECT_EQ(restored.device_version_product, 55U);
    EXPECT_EQ(restored.firmware_build, 123456789ULL);
    EXPECT_EQ(restored.firmware_version_minor, 3);
    EXPECT_EQ(restored.firmware_version_major, 2);
}

TEST(FrameHeaderTest, DefaultsMatchLifxProtocolConstants) {
    FrameHeader header;
    EXPECT_EQ(header.protocol, 1024);
    EXPECT_TRUE(header.addressable);
    EXPECT_FALSE(header.tagged);
    EXPECT_EQ(header.origin, 0);
}

TEST(FrameHeaderTest, RoundTrip) {
    FrameHeader original;
    original.size = 100;
    original.tagged = true;
    original.source = 0xABCDEF01;

    auto buffer = PackToBuffer(original);
    EXPECT_EQ(buffer.size(), 8U);

    auto restored = UnpackFromBuffer<FrameHeader>(buffer);
    EXPECT_EQ(restored.size, 100);
    EXPECT_EQ(restored.protocol, 1024);
    EXPECT_TRUE(restored.tagged);
    EXPECT_TRUE(restored.addressable);
    EXPECT_EQ(restored.origin, 0);
    EXPECT_EQ(restored.source, 0xABCDEF01U);
}

TEST(FrameHeaderTest, BitPackingLayoutMatchesLifxSpec) {
    FrameHeader header;
    header.tagged = true;
    header.addressable = true;
    header.origin = 0;

    auto buffer = PackToBuffer(header);
    // bytes 2-3: protocol(12 bits) | addressable(1) | tagged(1) | origin(2), little-endian
    auto word = static_cast<std::uint16_t>(static_cast<std::uint16_t>(buffer[2]) |
                                           (static_cast<std::uint16_t>(buffer[3]) << 8));
    EXPECT_EQ(word & 0x0FFFU, 1024U);
    EXPECT_EQ((word >> 12) & 0x1U, 1U); // addressable
    EXPECT_EQ((word >> 13) & 0x1U, 1U); // tagged
    EXPECT_EQ((word >> 14) & 0x3U, 0U); // origin
}

TEST(FrameAddressTest, RoundTrip) {
    FrameAddress original;
    original.target = 0x0102030405060708ULL;
    original.res_required = true;
    original.ack_required = false;
    original.sequence = 42;

    auto buffer = PackToBuffer(original);
    EXPECT_EQ(buffer.size(), 16U);

    auto restored = UnpackFromBuffer<FrameAddress>(buffer);
    EXPECT_EQ(restored.target, 0x0102030405060708ULL);
    EXPECT_TRUE(restored.res_required);
    EXPECT_FALSE(restored.ack_required);
    EXPECT_EQ(restored.sequence, 42);
}

TEST(FrameAddressTest, ReservedBytesAreZeroed) {
    FrameAddress original;
    auto buffer = PackToBuffer(original);
    for (std::size_t i = 8; i < 14; ++i) {
        EXPECT_EQ(buffer[i], 0) << "byte " << i;
    }
}

TEST(ProtocolHeaderTest, RoundTrip) {
    ProtocolHeader original;
    original.type = 102;

    auto buffer = PackToBuffer(original);
    EXPECT_EQ(buffer.size(), 12U);

    auto restored = UnpackFromBuffer<ProtocolHeader>(buffer);
    EXPECT_EQ(restored.type, 102);
}

TEST(HeaderTest, RoundTrip) {
    Header original;
    original.frame_header.tagged = true;
    original.frame_header.source = 0x11223344;
    original.frame_address.target = 0x0102030405060708ULL;
    original.frame_address.sequence = 7;
    original.protocol_header.type = 102;

    auto buffer = PackToBuffer(original);
    EXPECT_EQ(buffer.size(), 36U);

    auto restored = UnpackFromBuffer<Header>(buffer);
    EXPECT_TRUE(restored.frame_header.tagged);
    EXPECT_EQ(restored.frame_header.source, 0x11223344U);
    EXPECT_EQ(restored.frame_address.target, 0x0102030405060708ULL);
    EXPECT_EQ(restored.frame_address.sequence, 7);
    EXPECT_EQ(restored.protocol_header.type, 102);
}

TEST(PacketTest, PackWithNoPayloadWritesHeaderOnlySize) {
    Packet pkt;
    pkt.header.protocol_header.type = 2; // GetService

    std::vector<std::uint8_t> buffer;
    std::size_t offset = 0;
    pkt.Pack(buffer, offset);

    EXPECT_EQ(buffer.size(), 36U);
    EXPECT_EQ(offset, 36U);

    auto size = static_cast<std::uint16_t>(static_cast<std::uint16_t>(buffer[0]) |
                                           (static_cast<std::uint16_t>(buffer[1]) << 8));
    EXPECT_EQ(size, 36U);
}
