#include "packets/getters.hpp"
#include "test_helpers.hpp"
#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>

using namespace CLifx::Get;

TEST(GetServicePayloadTest, MessageType) {
    EXPECT_EQ(ServicePayload::MessageType, 3);
}
TEST(GetServicePayloadTest, RoundTrip) {
    ServicePayload original;
    original.service = 1;
    original.port = 56700;

    auto restored = UnpackFromBuffer<ServicePayload>(PackToBuffer(original));

    EXPECT_EQ(restored.service, 1);
    EXPECT_EQ(restored.port, 56700U);
}

TEST(HostFirmwarePayloadTest, MessageType) {
    EXPECT_EQ(HostFirmwarePayload::MessageType, 15);
}
TEST(HostFirmwarePayloadTest, RoundTrip) {
    HostFirmwarePayload original;
    original.build = 1111111111ULL;
    original.version_minor = 2;
    original.version_major = 3;

    auto restored = UnpackFromBuffer<HostFirmwarePayload>(PackToBuffer(original));

    EXPECT_EQ(restored.build, 1111111111ULL);
    EXPECT_EQ(restored.version_minor, 2);
    EXPECT_EQ(restored.version_major, 3);
}

TEST(WifiInfoPayloadTest, MessageType) {
    EXPECT_EQ(WifiInfoPayload::MessageType, 17);
}
TEST(WifiInfoPayloadTest, RoundTrip) {
    WifiInfoPayload original;
    original.signal = 12.5F;

    auto restored = UnpackFromBuffer<WifiInfoPayload>(PackToBuffer(original));

    EXPECT_FLOAT_EQ(restored.signal, 12.5F);
}

TEST(WifiFirmwarePayloadTest, MessageType) {
    EXPECT_EQ(WifiFirmwarePayload::MessageType, 19);
}
TEST(WifiFirmwarePayloadTest, RoundTrip) {
    WifiFirmwarePayload original;
    original.build = 222222ULL;
    original.version_minor = 4;
    original.version_major = 5;

    auto restored = UnpackFromBuffer<WifiFirmwarePayload>(PackToBuffer(original));

    EXPECT_EQ(restored.build, 222222ULL);
    EXPECT_EQ(restored.version_minor, 4);
    EXPECT_EQ(restored.version_major, 5);
}

TEST(GetPowerPayloadTest, MessageType) {
    EXPECT_EQ(PowerPayload::MessageType, 22);
}
TEST(GetPowerPayloadTest, RoundTrip) {
    PowerPayload original;
    original.level = 65535;

    auto restored = UnpackFromBuffer<PowerPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.level, 65535);
}

TEST(GetLabelPayloadTest, MessageType) {
    EXPECT_EQ(LabelPayload::MessageType, 25);
}
TEST(GetLabelPayloadTest, RoundTrip) {
    LabelPayload original;
    original.label.fill(0);
    const char *name = "Kitchen Light";
    std::copy(name, name + std::strlen(name), original.label.begin());

    auto restored = UnpackFromBuffer<LabelPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.label, original.label);
}

TEST(VersionPayloadTest, MessageType) {
    EXPECT_EQ(VersionPayload::MessageType, 33);
}
TEST(VersionPayloadTest, RoundTrip) {
    VersionPayload original;
    original.vendor = 1;
    original.product = 55;

    auto restored = UnpackFromBuffer<VersionPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.vendor, 1U);
    EXPECT_EQ(restored.product, 55U);
}

TEST(InfoPayloadTest, MessageType) {
    EXPECT_EQ(InfoPayload::MessageType, 35);
}
TEST(InfoPayloadTest, RoundTrip) {
    InfoPayload original;
    original.time = 1000;
    original.uptime = 2000;
    original.downtime = 3000;

    auto restored = UnpackFromBuffer<InfoPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.time, 1000ULL);
    EXPECT_EQ(restored.uptime, 2000ULL);
    EXPECT_EQ(restored.downtime, 3000ULL);
}

TEST(GetLocationPayloadTest, MessageType) {
    EXPECT_EQ(LocationPayload::MessageType, 50);
}
TEST(GetLocationPayloadTest, RoundTrip) {
    LocationPayload original;
    original.location.fill(0xAB);
    original.label.fill(0);
    const char *name = "Home";
    std::copy(name, name + std::strlen(name), original.label.begin());
    original.updated_at = 123456789ULL;

    auto restored = UnpackFromBuffer<LocationPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.location, original.location);
    EXPECT_EQ(restored.label, original.label);
    EXPECT_EQ(restored.updated_at, 123456789ULL);
}

TEST(GetGroupPayloadTest, MessageType) {
    EXPECT_EQ(GroupPayload::MessageType, 53);
}
TEST(GetGroupPayloadTest, RoundTrip) {
    GroupPayload original;
    original.group.fill(0xCD);
    original.label.fill(0);
    const char *name = "Living Room";
    std::copy(name, name + std::strlen(name), original.label.begin());
    original.updated_at = 987654321ULL;

    auto restored = UnpackFromBuffer<GroupPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.group, original.group);
    EXPECT_EQ(restored.label, original.label);
    EXPECT_EQ(restored.updated_at, 987654321ULL);
}

TEST(EchoRequestPayloadTest, MessageType) {
    EXPECT_EQ(EchoRequestPayload::MessageType, 58);
}
TEST(EchoRequestPayloadTest, RoundTrip) {
    EchoRequestPayload original;
    for (std::size_t i = 0; i < original.payload.size(); ++i) {
        original.payload[i] = static_cast<std::uint8_t>(i);
    }

    auto restored = UnpackFromBuffer<EchoRequestPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.payload, original.payload);
}

TEST(EchoResponsePayloadTest, MessageType) {
    EXPECT_EQ(EchoResponsePayload::MessageType, 59);
}
TEST(EchoResponsePayloadTest, RoundTrip) {
    EchoResponsePayload original;
    for (std::size_t i = 0; i < original.payload.size(); ++i) {
        original.payload[i] = static_cast<std::uint8_t>(i * 2);
    }

    auto restored = UnpackFromBuffer<EchoResponsePayload>(PackToBuffer(original));

    EXPECT_EQ(restored.payload, original.payload);
}

TEST(GetButtonConfigPayloadTest, MessageType) {
    EXPECT_EQ(ButtonConfigPayload::MessageType, 911);
}
TEST(GetButtonConfigPayloadTest, RoundTrip) {
    ButtonConfigPayload original;
    original.haptic_duration_ms = 250;
    original.backlight_on_color.hue = 100;
    original.backlight_on_color.kelvin = 3500;
    original.backlight_off_color.hue = 200;
    original.backlight_off_color.kelvin = 2700;

    auto restored = UnpackFromBuffer<ButtonConfigPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.haptic_duration_ms, 250);
    EXPECT_EQ(restored.backlight_on_color.hue, 100);
    EXPECT_EQ(restored.backlight_on_color.kelvin, 3500);
    EXPECT_EQ(restored.backlight_off_color.hue, 200);
    EXPECT_EQ(restored.backlight_off_color.kelvin, 2700);
}

TEST(GetColorPayloadTest, MessageType) {
    EXPECT_EQ(ColorPayload::MessageType, 107);
}
TEST(GetColorPayloadTest, RoundTrip) {
    ColorPayload original;
    original.color.hue = 1000;
    original.color.saturation = 2000;
    original.color.brightness = 3000;
    original.color.kelvin = 4000;
    original.power = 65535;
    original.label.fill(0);
    const char *name = "Bedroom";
    std::copy(name, name + std::strlen(name), original.label.begin());

    auto restored = UnpackFromBuffer<ColorPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.color.hue, 1000);
    EXPECT_EQ(restored.color.saturation, 2000);
    EXPECT_EQ(restored.color.brightness, 3000);
    EXPECT_EQ(restored.color.kelvin, 4000);
    EXPECT_EQ(restored.power, 65535);
    EXPECT_EQ(restored.label, original.label);
}

TEST(GetLightPowerPayloadTest, MessageType) {
    EXPECT_EQ(LightPowerPayload::MessageType, 118);
}
TEST(GetLightPowerPayloadTest, RoundTrip) {
    LightPowerPayload original;
    original.level = 32768;

    auto restored = UnpackFromBuffer<LightPowerPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.level, 32768);
}

TEST(GetInfraredPayloadTest, MessageType) {
    EXPECT_EQ(InfraredPayload::MessageType, 121);
}
TEST(GetInfraredPayloadTest, RoundTrip) {
    InfraredPayload original;
    original.brightness = 12345;

    auto restored = UnpackFromBuffer<InfraredPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.brightness, 12345);
}

TEST(HevCyclePayloadTest, MessageType) {
    EXPECT_EQ(HevCyclePayload::MessageType, 144);
}
TEST(HevCyclePayloadTest, RoundTrip) {
    HevCyclePayload original;
    original.duration_s = 100;
    original.remaining_s = 50;
    original.last_power = true;

    auto restored = UnpackFromBuffer<HevCyclePayload>(PackToBuffer(original));

    EXPECT_EQ(restored.duration_s, 100U);
    EXPECT_EQ(restored.remaining_s, 50U);
    EXPECT_TRUE(restored.last_power);
}

TEST(HevCycleConfigurationPayloadTest, MessageType) {
    EXPECT_EQ(HevCycleConfigurationPayload::MessageType, 147);
}
TEST(HevCycleConfigurationPayloadTest, RoundTrip) {
    HevCycleConfigurationPayload original;
    original.indication = true;
    original.duration_s = 7200;

    auto restored = UnpackFromBuffer<HevCycleConfigurationPayload>(PackToBuffer(original));

    EXPECT_TRUE(restored.indication);
    EXPECT_EQ(restored.duration_s, 7200U);
}

TEST(LastHevCycleResultPayloadTest, MessageType) {
    EXPECT_EQ(LastHevCycleResultPayload::MessageType, 149);
}
TEST(LastHevCycleResultPayloadTest, RoundTrip) {
    LastHevCycleResultPayload original;
    original.result = 2;

    auto restored = UnpackFromBuffer<LastHevCycleResultPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.result, 2);
}

TEST(GetColorZonesQueryPayloadTest, MessageType) {
    EXPECT_EQ(GetColorZonesPayload::MessageType, 502);
}
TEST(GetColorZonesQueryPayloadTest, RoundTrip) {
    GetColorZonesPayload original;
    original.start_index = 2;
    original.end_index = 6;

    auto restored = UnpackFromBuffer<GetColorZonesPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.start_index, 2);
    EXPECT_EQ(restored.end_index, 6);
}

TEST(StateZonePayloadTest, MessageType) {
    EXPECT_EQ(StateZonePayload::MessageType, 503);
}
TEST(StateZonePayloadTest, RoundTrip) {
    StateZonePayload original;
    original.zones_count = 16;
    original.zone_index = 4;
    original.color.hue = 1234;
    original.color.saturation = 2345;
    original.color.brightness = 3456;
    original.color.kelvin = 4567;

    auto restored = UnpackFromBuffer<StateZonePayload>(PackToBuffer(original));

    EXPECT_EQ(restored.zones_count, 16);
    EXPECT_EQ(restored.zone_index, 4);
    EXPECT_EQ(restored.color.hue, 1234);
    EXPECT_EQ(restored.color.saturation, 2345);
    EXPECT_EQ(restored.color.brightness, 3456);
    EXPECT_EQ(restored.color.kelvin, 4567);
}

TEST(GetColorZonesPayloadTest, MessageType) {
    EXPECT_EQ(ColorZonesPayload::MessageType, 506);
}
TEST(GetColorZonesPayloadTest, RoundTrip) {
    ColorZonesPayload original;
    original.count = 8;
    original.index = 0;
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        original.colors[i].hue = static_cast<std::uint16_t>(i * 100);
        original.colors[i].saturation = static_cast<std::uint16_t>(i * 10);
        original.colors[i].brightness = static_cast<std::uint16_t>(i * 20);
        original.colors[i].kelvin = 3500;
    }

    auto restored = UnpackFromBuffer<ColorZonesPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.count, 8);
    EXPECT_EQ(restored.index, 0);
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        EXPECT_EQ(restored.colors[i].hue, original.colors[i].hue);
        EXPECT_EQ(restored.colors[i].saturation, original.colors[i].saturation);
        EXPECT_EQ(restored.colors[i].brightness, original.colors[i].brightness);
        EXPECT_EQ(restored.colors[i].kelvin, original.colors[i].kelvin);
    }
}

TEST(MultiZoneEffectPayloadTest, MessageType) {
    EXPECT_EQ(MultiZoneEffectPayload::MessageType, 509);
}
TEST(MultiZoneEffectPayloadTest, RoundTrip) {
    MultiZoneEffectPayload original;
    original.instanceid = 42;
    original.type = 1;
    original.speed = 500;
    original.duration = 60000;
    for (std::size_t i = 0; i < original.parameters.size(); ++i) {
        original.parameters[i] = static_cast<std::uint32_t>(i);
    }

    auto restored = UnpackFromBuffer<MultiZoneEffectPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.instanceid, 42U);
    EXPECT_EQ(restored.type, 1);
    EXPECT_EQ(restored.speed, 500U);
    EXPECT_EQ(restored.duration, 60000ULL);
    EXPECT_EQ(restored.parameters, original.parameters);
}

TEST(GetExtendedColorZonesPayloadTest, MessageType) {
    EXPECT_EQ(ExtendedColorZonesPayload::MessageType, 512);
}
TEST(GetExtendedColorZonesPayloadTest, RoundTrip) {
    ExtendedColorZonesPayload original;
    original.zones_count = 82;
    original.zone_index = 0;
    original.colors_count = 82;
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        original.colors[i].hue = static_cast<std::uint16_t>(i);
        original.colors[i].kelvin = 3500;
    }

    auto restored = UnpackFromBuffer<ExtendedColorZonesPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.zones_count, 82);
    EXPECT_EQ(restored.zone_index, 0);
    EXPECT_EQ(restored.colors_count, 82);
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        EXPECT_EQ(restored.colors[i].hue, original.colors[i].hue);
        EXPECT_EQ(restored.colors[i].kelvin, original.colors[i].kelvin);
    }
}

TEST(GetRPowerPayloadTest, MessageType) {
    EXPECT_EQ(RPowerPayload::MessageType, 818);
}
TEST(GetRPowerPayloadTest, RoundTrip) {
    RPowerPayload original;
    original.relay_index = 2;
    original.level = 65535;

    auto restored = UnpackFromBuffer<RPowerPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.relay_index, 2);
    EXPECT_EQ(restored.level, 65535);
}

TEST(DeviceChainPayloadTest, MessageType) {
    EXPECT_EQ(DeviceChainPayload::MessageType, 702);
}
TEST(DeviceChainPayloadTest, RoundTrip) {
    DeviceChainPayload original;
    original.start_index = 0;
    original.tile_devices_count = 2;
    original.tile_devices[0].width = 8;
    original.tile_devices[0].height = 8;
    original.tile_devices[0].firmware_build = 111ULL;
    original.tile_devices[1].width = 8;
    original.tile_devices[1].height = 8;
    original.tile_devices[1].firmware_build = 222ULL;

    auto restored = UnpackFromBuffer<DeviceChainPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.start_index, 0);
    EXPECT_EQ(restored.tile_devices_count, 2);
    EXPECT_EQ(restored.tile_devices[0].width, 8);
    EXPECT_EQ(restored.tile_devices[0].firmware_build, 111ULL);
    EXPECT_EQ(restored.tile_devices[1].firmware_build, 222ULL);
}

TEST(Get64PayloadTest, MessageType) {
    EXPECT_EQ(Get64Payload::MessageType, 707);
}
TEST(Get64PayloadTest, RoundTrip) {
    Get64Payload original;
    original.tile_index = 1;
    original.length = 1;
    original.x = 0;
    original.y = 0;
    original.width = 8;

    auto restored = UnpackFromBuffer<Get64Payload>(PackToBuffer(original));

    EXPECT_EQ(restored.tile_index, 1);
    EXPECT_EQ(restored.length, 1);
    EXPECT_EQ(restored.x, 0);
    EXPECT_EQ(restored.y, 0);
    EXPECT_EQ(restored.width, 8);
}

TEST(State64PayloadTest, MessageType) {
    EXPECT_EQ(State64Payload::MessageType, 711);
}
TEST(State64PayloadTest, RoundTrip) {
    State64Payload original;
    original.tile_index = 1;
    original.x = 0;
    original.y = 0;
    original.width = 8;
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        original.colors[i].hue = static_cast<std::uint16_t>(i * 5);
        original.colors[i].kelvin = 3500;
    }

    auto restored = UnpackFromBuffer<State64Payload>(PackToBuffer(original));

    EXPECT_EQ(restored.tile_index, 1);
    EXPECT_EQ(restored.x, 0);
    EXPECT_EQ(restored.y, 0);
    EXPECT_EQ(restored.width, 8);
    for (std::size_t i = 0; i < original.colors.size(); ++i) {
        EXPECT_EQ(restored.colors[i].hue, original.colors[i].hue);
        EXPECT_EQ(restored.colors[i].kelvin, original.colors[i].kelvin);
    }
}

TEST(GetTileEffectPayloadTest, MessageType) {
    EXPECT_EQ(TileEffectPayload::MessageType, 720);
}
TEST(GetTileEffectPayloadTest, RoundTrip) {
    TileEffectPayload original;
    original.instanceid = 5;
    original.type = 2;
    original.speed = 3000;
    original.duration = 0;
    original.palette_count = 2;
    original.palette[0].hue = 100;
    original.palette[1].hue = 200;

    auto restored = UnpackFromBuffer<TileEffectPayload>(PackToBuffer(original));

    EXPECT_EQ(restored.instanceid, 5U);
    EXPECT_EQ(restored.type, 2);
    EXPECT_EQ(restored.speed, 3000U);
    EXPECT_EQ(restored.duration, 0ULL);
    EXPECT_EQ(restored.palette_count, 2);
    EXPECT_EQ(restored.palette[0].hue, 100);
    EXPECT_EQ(restored.palette[1].hue, 200);
}

TEST(SensorAmbientLightPayloadTest, MessageType) {
    EXPECT_EQ(SensorAmbientLightPayload::MessageType, 402);
}
TEST(SensorAmbientLightPayloadTest, RoundTrip) {
    SensorAmbientLightPayload original;
    original.lux = 350.25F;

    auto restored = UnpackFromBuffer<SensorAmbientLightPayload>(PackToBuffer(original));

    EXPECT_FLOAT_EQ(restored.lux, 350.25F);
}
