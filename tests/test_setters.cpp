#include "packets/setters.hpp"
#include "test_helpers.hpp"
#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>

using namespace CLifx::Set;
using CLifx::Packet;

TEST(SetPowerPayloadTest, MessageType) {
    EXPECT_EQ(PowerPayload::MessageType, 21);
}
TEST(SetPowerPayloadTest, RoundTrip) {
    PowerPayload original;
    original.level = 65535;

    auto restored = UnpackFromBuffer<PowerPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.level, 65535);
}

TEST(SetLabelPayloadTest, MessageType) {
    EXPECT_EQ(LabelPayload::MessageType, 24);
}
TEST(SetLabelPayloadTest, RoundTrip) {
    LabelPayload original;
    original.label.fill(0);
    const char *name = "Office Lamp";
    std::copy(name, name + std::strlen(name), original.label.begin());

    auto restored = UnpackFromBuffer<LabelPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.label, original.label);
}

TEST(RebootPayloadTest, MessageType) {
    EXPECT_EQ(RebootPayload::MessageType, 38);
}
TEST(RebootPayloadTest, PackProducesEmptyBuffer) {
    RebootPayload original;
    auto buffer = PackToBuffer(original);
    EXPECT_TRUE(buffer.empty());
}

TEST(SetLocationPayloadTest, MessageType) {
    EXPECT_EQ(LocationPayload::MessageType, 49);
}
TEST(SetLocationPayloadTest, RoundTrip) {
    LocationPayload original;
    original.location.fill(0x11);
    original.label.fill(0);
    const char *name = "Garage";
    std::copy(name, name + std::strlen(name), original.label.begin());
    original.updated_at = 555ULL;

    auto restored = UnpackFromBuffer<LocationPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.location, original.location);
    EXPECT_EQ(restored.label, original.label);
    EXPECT_EQ(restored.updated_at, 555ULL);
}

TEST(SetGroupPayloadTest, MessageType) {
    EXPECT_EQ(GroupPayload::MessageType, 52);
}
TEST(SetGroupPayloadTest, RoundTrip) {
    GroupPayload original;
    original.group.fill(0x22);
    original.label.fill(0);
    const char *name = "Downstairs";
    std::copy(name, name + std::strlen(name), original.label.begin());
    original.updated_at = 777ULL;

    auto restored = UnpackFromBuffer<GroupPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.group, original.group);
    EXPECT_EQ(restored.label, original.label);
    EXPECT_EQ(restored.updated_at, 777ULL);
}

TEST(SetColorPayloadTest, MessageType) {
    EXPECT_EQ(ColorPayload::MessageType, 102);
}
TEST(SetColorPayloadTest, RoundTrip) {
    ColorPayload original;
    original.color.hue = 0;
    original.color.saturation = 65535;
    original.color.brightness = 65535;
    original.color.kelvin = 3500;
    original.duration = 1000;

    auto restored = UnpackFromBuffer<ColorPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.color.hue, 0);
    EXPECT_EQ(restored.color.saturation, 65535);
    EXPECT_EQ(restored.color.brightness, 65535);
    EXPECT_EQ(restored.color.kelvin, 3500);
    EXPECT_EQ(restored.duration, 1000U);
}

TEST(SetWaveformPayloadTest, MessageType) {
    EXPECT_EQ(WaveformPayload::MessageType, 103);
}
TEST(SetWaveformPayloadTest, RoundTrip) {
    WaveformPayload original;
    original.transient = true;
    original.color.hue = 100;
    original.color.saturation = 200;
    original.color.brightness = 300;
    original.color.kelvin = 400;
    original.period = 1000;
    original.cycles = 5.5F;
    original.skew_ratio = -1000;
    original.waveform = 1;

    auto restored = UnpackFromBuffer<WaveformPayload>(PackToBuffer(original));

    EXPECT_TRUE(restored.transient);
    EXPECT_EQ(restored.color.hue, 100);
    EXPECT_EQ(restored.color.saturation, 200);
    EXPECT_EQ(restored.color.brightness, 300);
    EXPECT_EQ(restored.color.kelvin, 400);
    EXPECT_EQ(restored.period, 1000U);
    EXPECT_FLOAT_EQ(restored.cycles, 5.5F);
    EXPECT_EQ(restored.skew_ratio, -1000);
    EXPECT_EQ(restored.waveform, 1);
}

TEST(SetLightPowerPayloadTest, MessageType) {
    EXPECT_EQ(LightPowerPayload::MessageType, 117);
}
TEST(SetLightPowerPayloadTest, RoundTrip) {
    LightPowerPayload original;
    original.level = 65535;
    original.duration = 500;

    auto restored = UnpackFromBuffer<LightPowerPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.level, 65535);
    EXPECT_EQ(restored.duration, 500U);
}

TEST(SetWaveformOptionalPayloadTest, MessageType) {
    EXPECT_EQ(WaveformOptionalPayload::MessageType, 119);
}
TEST(SetWaveformOptionalPayloadTest, RoundTrip) {
    WaveformOptionalPayload original;
    original.transient = true;
    original.color.hue = 10;
    original.color.saturation = 20;
    original.color.brightness = 30;
    original.color.kelvin = 40;
    original.period = 2000;
    original.cycles = 3.0F;
    original.skew_ratio = 500;
    original.waveform = 2;
    original.set_hue = true;
    original.set_saturation = false;
    original.set_brightness = true;
    original.set_kelvin = false;

    auto restored = UnpackFromBuffer<WaveformOptionalPayload>(PackToBuffer(original));

    EXPECT_TRUE(restored.transient);
    EXPECT_EQ(restored.color.hue, 10);
    EXPECT_EQ(restored.color.saturation, 20);
    EXPECT_EQ(restored.color.brightness, 30);
    EXPECT_EQ(restored.color.kelvin, 40);
    EXPECT_EQ(restored.period, 2000U);
    EXPECT_FLOAT_EQ(restored.cycles, 3.0F);
    EXPECT_EQ(restored.skew_ratio, 500);
    EXPECT_EQ(restored.waveform, 2);
    EXPECT_TRUE(restored.set_hue);
    EXPECT_FALSE(restored.set_saturation);
    EXPECT_TRUE(restored.set_brightness);
    EXPECT_FALSE(restored.set_kelvin);
}

TEST(SetInfraredPayloadTest, MessageType) {
    EXPECT_EQ(InfraredPayload::MessageType, 122);
}
TEST(SetInfraredPayloadTest, RoundTrip) {
    InfraredPayload original;
    original.brightness = 32000;

    auto restored = UnpackFromBuffer<InfraredPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.brightness, 32000);
}

TEST(SetHevCyclePayloadTest, MessageType) {
    EXPECT_EQ(HevCyclePayload::MessageType, 143);
}
TEST(SetHevCyclePayloadTest, RoundTrip) {
    HevCyclePayload original;
    original.enable = true;
    original.duration_s = 3600;

    auto restored = UnpackFromBuffer<HevCyclePayload>(PackToBuffer(original));

    EXPECT_TRUE(restored.enable);
    EXPECT_EQ(restored.duration_s, 3600U);
}

TEST(SetHevCycleConfigurationPayloadTest, MessageType) {
    EXPECT_EQ(HevCycleConfigurationPayload::MessageType, 146);
}
TEST(SetHevCycleConfigurationPayloadTest, RoundTrip) {
    HevCycleConfigurationPayload original;
    original.indication = false;
    original.duration_s = 1800;

    auto restored = UnpackFromBuffer<HevCycleConfigurationPayload>(PackToBuffer(original));

    EXPECT_FALSE(restored.indication);
    EXPECT_EQ(restored.duration_s, 1800U);
}

TEST(SetColorZonesPayloadTest, MessageType) {
    EXPECT_EQ(ColorZonesPayload::MessageType, 501);
}
TEST(SetColorZonesPayloadTest, RoundTrip) {
    ColorZonesPayload original;
    original.start_index = 0;
    original.end_index = 7;
    original.color.hue = 999;
    original.color.saturation = 888;
    original.color.brightness = 777;
    original.color.kelvin = 3500;
    original.duration = 250;
    original.apply = 1;

    auto restored = UnpackFromBuffer<ColorZonesPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.start_index, 0);
    EXPECT_EQ(restored.end_index, 7);
    EXPECT_EQ(restored.color.hue, 999);
    EXPECT_EQ(restored.color.saturation, 888);
    EXPECT_EQ(restored.color.brightness, 777);
    EXPECT_EQ(restored.color.kelvin, 3500);
    EXPECT_EQ(restored.duration, 250U);
    EXPECT_EQ(restored.apply, 1);
}

TEST(MulitiZoneEffectPayloadTest, MessageType) {
    EXPECT_EQ(MulitiZoneEffectPayload::MessageType, 508);
}
TEST(MulitiZoneEffectPayloadTest, RoundTrip) {
    MulitiZoneEffectPayload original;
    original.instanceid = 99;
    original.type = 3;
    original.speed = 750;
    original.duration = 30000;
    for (std::size_t i = 0; i < original.parameters.size(); ++i) {
        original.parameters[i] = static_cast<std::uint32_t>(i * 2);
    }

    auto restored = UnpackFromBuffer<MulitiZoneEffectPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.instanceid, 99U);
    EXPECT_EQ(restored.type, 3);
    EXPECT_EQ(restored.speed, 750U);
    EXPECT_EQ(restored.duration, 30000ULL);
    EXPECT_EQ(restored.parameters, original.parameters);
}

TEST(SetExtendedColorZonesPayloadTest, MessageType) {
    EXPECT_EQ(ExtendedColorZonesPayload::MessageType, 510);
}
TEST(SetExtendedColorZonesPayloadTest, RoundTrip) {
    ExtendedColorZonesPayload original;
    original.duration = 400;
    original.apply = 1;
    original.zone_index = 3;
    original.colors_count = 5;
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        original.colors[i].brightness = static_cast<std::uint16_t>(i);
    }

    auto restored = UnpackFromBuffer<ExtendedColorZonesPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.duration, 400U);
    EXPECT_EQ(restored.apply, 1);
    EXPECT_EQ(restored.zone_index, 3);
    EXPECT_EQ(restored.colors_count, 5);
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        EXPECT_EQ(restored.colors[i].brightness, original.colors[i].brightness);
    }
}

TEST(SetRPowerPayloadTest, MessageType) {
    EXPECT_EQ(RPowerPayload::MessageType, 817);
}
TEST(SetRPowerPayloadTest, RoundTrip) {
    RPowerPayload original;
    original.relay_index = 3;
    original.level = 12345;

    auto restored = UnpackFromBuffer<RPowerPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.relay_index, 3);
    EXPECT_EQ(restored.level, 12345);
}

TEST(SetButtonConfigPayloadTest, MessageType) {
    EXPECT_EQ(ButtonConfigPayload::MessageType, 910);
}
TEST(SetButtonConfigPayloadTest, RoundTrip) {
    ButtonConfigPayload original;
    original.haptic_duration_ms = 300;
    original.backlight_on_color.hue = 50;
    original.backlight_on_color.kelvin = 3200;
    original.backlight_off_color.hue = 150;
    original.backlight_off_color.kelvin = 4000;

    auto restored = UnpackFromBuffer<ButtonConfigPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.haptic_duration_ms, 300);
    EXPECT_EQ(restored.backlight_on_color.hue, 50);
    EXPECT_EQ(restored.backlight_on_color.kelvin, 3200);
    EXPECT_EQ(restored.backlight_off_color.hue, 150);
    EXPECT_EQ(restored.backlight_off_color.kelvin, 4000);
}

TEST(UserPositionPayloadTest, MessageType) {
    EXPECT_EQ(UserPositionPayload::MessageType, 703);
}
TEST(UserPositionPayloadTest, RoundTrip) {
    UserPositionPayload original;
    original.tile_index = 1;
    original.user_x = 1.5F;
    original.user_y = -1.5F;

    auto restored = UnpackFromBuffer<UserPositionPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.tile_index, 1);
    EXPECT_FLOAT_EQ(restored.user_x, 1.5F);
    EXPECT_FLOAT_EQ(restored.user_y, -1.5F);
}

TEST(Set64PayloadTest, MessageType) {
    EXPECT_EQ(Set64Payload::MessageType, 715);
}
TEST(Set64PayloadTest, RoundTrip) {
    Set64Payload original;
    original.tile_index = 0;
    original.length = 1;
    original.x = 0;
    original.y = 0;
    original.width = 8;
    original.duration = 100;
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        original.colors[i].hue = static_cast<std::uint16_t>(i);
        original.colors[i].kelvin = 3500;
    }

    auto restored = UnpackFromBuffer<Set64Payload>(PackToBuffer(original));

    EXPECT_EQ(restored.tile_index, 0);
    EXPECT_EQ(restored.length, 1);
    EXPECT_EQ(restored.x, 0);
    EXPECT_EQ(restored.y, 0);
    EXPECT_EQ(restored.width, 8);
    EXPECT_EQ(restored.duration, 100U);
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        EXPECT_EQ(restored.colors[i].hue, original.colors[i].hue);
        EXPECT_EQ(restored.colors[i].kelvin, original.colors[i].kelvin);
    }
}

TEST(CopyFrameBufferPayloadTest, MessageType) {
    EXPECT_EQ(CopyFrameBufferPayload::MessageType, 716);
}
TEST(CopyFrameBufferPayloadTest, RoundTrip) {
    CopyFrameBufferPayload original;
    original.tile_index = 1;
    original.length = 2;
    original.src_fb_index = 0;
    original.dst_fb_index = 1;
    original.src_x = 3;
    original.src_y = 4;
    original.dst_x = 5;
    original.dst_y = 6;
    original.width = 8;
    original.height = 8;
    original.duration = 250;

    auto restored = UnpackFromBuffer<CopyFrameBufferPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.tile_index, 1);
    EXPECT_EQ(restored.length, 2);
    EXPECT_EQ(restored.src_fb_index, 0);
    EXPECT_EQ(restored.dst_fb_index, 1);
    EXPECT_EQ(restored.src_x, 3);
    EXPECT_EQ(restored.src_y, 4);
    EXPECT_EQ(restored.dst_x, 5);
    EXPECT_EQ(restored.dst_y, 6);
    EXPECT_EQ(restored.width, 8);
    EXPECT_EQ(restored.height, 8);
    EXPECT_EQ(restored.duration, 250U);
}

TEST(SetTileEffectPayloadTest, MessageType) {
    EXPECT_EQ(TileEffectPayload::MessageType, 719);
}
TEST(SetTileEffectPayloadTest, RoundTrip) {
    TileEffectPayload original;
    original.instanceid = 7;
    original.type = 1;
    original.speed = 4000;
    original.duration = 0;
    original.palette_count = 3;
    original.palette[0].hue = 111;
    original.palette[1].hue = 222;
    original.palette[2].hue = 333;

    auto restored = UnpackFromBuffer<TileEffectPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.instanceid, 7U);
    EXPECT_EQ(restored.type, 1);
    EXPECT_EQ(restored.speed, 4000U);
    EXPECT_EQ(restored.duration, 0ULL);
    EXPECT_EQ(restored.palette_count, 3);
    EXPECT_EQ(restored.palette[0].hue, 111);
    EXPECT_EQ(restored.palette[1].hue, 222);
    EXPECT_EQ(restored.palette[2].hue, 333);
}

// --- Packet integration: setPayload + a full Pack through a real payload ---

TEST(PacketIntegrationTest, SetPayloadAssignsMessageType) {
    Packet pkt;
    auto *payload = pkt.setPayload<PowerPayload>();
    ASSERT_NE(payload, nullptr);
    EXPECT_EQ(pkt.header.protocol_header.type, PowerPayload::MessageType);
}

TEST(PacketIntegrationTest, PackWritesCorrectTotalSize) {
    Packet pkt;
    pkt.header.frame_header.tagged = true;
    pkt.header.frame_header.source = 0xC0FFEE;
    auto *payload = pkt.setPayload<ColorPayload>();
    payload->color.hue = 0;
    payload->color.saturation = 65535;
    payload->color.brightness = 65535;
    payload->color.kelvin = 3500;
    payload->duration = 1000;

    std::vector<std::uint8_t> buffer;
    std::size_t offset = 0;
    pkt.Pack(buffer, offset);

    // Header (36) + SetColor payload (1 reserved + 8 HSBK + 4 duration = 13)
    EXPECT_EQ(buffer.size(), 49U);
    EXPECT_EQ(offset, 49U);

    auto size = static_cast<std::uint16_t>(static_cast<std::uint16_t>(buffer[0]) |
                                           (static_cast<std::uint16_t>(buffer[1]) << 8));
    EXPECT_EQ(size, 49U);
}
