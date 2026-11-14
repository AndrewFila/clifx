# clifx

[![Build](https://github.com/AndrewFila/clifx/actions/workflows/build.yaml/badge.svg)](https://github.com/AndrewFila/clifx/actions/workflows/build.yaml)
[![Tests](https://github.com/AndrewFila/clifx/actions/workflows/tests.yml/badge.svg)](https://github.com/AndrewFila/clifx/actions/workflows/tests.yml)
[![Coverage](https://codecov.io/gh/AndrewFila/clifx/badge.svg)](https://codecov.io/gh/AndrewFila/clifx)
[![Release](https://img.shields.io/github/v/release/AndrewFila/clifx)](https://github.com/AndrewFila/clifx/releases)
[![License: GPL-3.0](https://img.shields.io/badge/License-GPL--3.0-blue.svg)](LICENSE)

A C++20 library for the [LIFX LAN protocol](https://lan.developer.lifx.com/docs).
It builds and parses the packets LIFX bulbs send and receive over your local
network, with no cloud account and no LIFX app involved.

clifx only does the bytes. It has no sockets, no discovery loop and no CLI,
so the same code can run on a desktop over POSIX sockets or on a
microcontroller over lwIP. You bring the transport.

## Example

Turn every light on the network red:

```cpp
#include "setters.hpp"
#include <vector>

CLifx::Packet pkt;
pkt.header.frame_header.tagged = true;       // broadcast to all devices
pkt.header.frame_header.source = 0x12345678; // any ID for this client

auto *color = pkt.setPayload<CLifx::Set::ColorPayload>();
color->color.hue        = 0;      // red
color->color.saturation = 0xFFFF;
color->color.brightness = 0xFFFF;
color->color.kelvin     = 3500;
color->duration         = 1000;   // fade over 1 second

std::vector<std::uint8_t> buffer;
std::size_t offset = 0;
pkt.Pack(buffer, offset);
// send buffer over UDP to 255.255.255.255:56700
```

Reading a reply works the other way around. Unpack the header first, check
the message type, then unpack the payload that matches it:

```cpp
#include "getters.hpp"

void handleResponse(const std::vector<std::uint8_t> &buffer) {
    CLifx::Packet pkt;
    std::size_t offset = 0;
    pkt.header.Unpack(buffer, offset);

    if (pkt.header.protocol_header.type == CLifx::Get::ServicePayload::MessageType) {
        CLifx::Get::ServicePayload service;
        service.Unpack(buffer, offset);
        // service.port is the device's service port
    }
}
```

`Packet::Unpack` will also unpack the payload for you if you've already
assigned one, which is handy when you know what type of reply to expect.

## Using it in your project

With CMake's `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(clifx
  GIT_REPOSITORY https://github.com/AndrewFila/clifx.git
  GIT_TAG v1.1.0)
FetchContent_MakeAvailable(clifx)

target_link_libraries(your_target PRIVATE clifx)
```

`add_subdirectory` on a submodule or checkout works the same way. The tests
are skipped when clifx is pulled into another project; set
`CLIFX_BUILD_TESTS=ON` if you want them.

## What's supported

Messages are split into `CLifx::Set::` (commands you send) and `CLifx::Get::`
(the `State*` replies devices send back). Several pairs share a name, like
`Set::PowerPayload` and `Get::PowerPayload`, which is why they live in
separate namespaces.

| Purpose                | Set (`CLifx::Set::`)           | State (`CLifx::Get::`)                                  |
| ---------------------- | ------------------------------ | ------------------------------------------------------- |
| Power                  | `PowerPayload` (21)            | `PowerPayload` (22)                                     |
| Label                  | `LabelPayload` (24)            | `LabelPayload` (25)                                     |
| Color                  | `ColorPayload` (102)           | `ColorPayload` (107)                                    |
| Waveform               | `WaveformPayload` (103)        | —                                                       |
| Light power            | `LightPowerPayload` (117)      | `LightPowerPayload` (118)                               |
| Infrared               | `InfraredPayload` (122)        | `InfraredPayload` (121)                                 |
| Discovery              | —                              | `ServicePayload` (3)                                    |
| Echo                   | —                              | `EchoRequestPayload` (58) / `EchoResponsePayload` (59)  |
| MultiZone query        | —                              | `GetColorZonesPayload` (502) / `StateZonePayload` (503) |
| Tile framebuffer       | `CopyFrameBufferPayload` (716) | `State64Payload` (711)                                  |
| Button config (Switch) | `ButtonConfigPayload` (910)    | `ButtonConfigPayload` (911)                             |

That's a sample. `packets/getters.hpp` and `packets/setters.hpp` have the
full list, including the rest of the MultiZone and Tile messages.

## Layout

- `utils/packable.hpp`: the `Pack`/`Unpack` interface and byte helpers
  everything is built on
- `packets/base.hpp`: the frame layout (`FrameHeader`, `FrameAddress`,
  `ProtocolHeader`, `Packet`) and shared types like `HSBK` and `Tile`
- `packets/getters.hpp`: `CLifx::Get::*` reply payloads
- `packets/setters.hpp`: `CLifx::Set::*` command payloads

Fields are packed in the order they're declared. The wire format is
positional, so the order in the headers matters.

## Building and testing

```sh
cmake -B build
cmake --build build
cd build && ctest --output-on-failure
```

GoogleTest is fetched automatically. Every payload has a round-trip test
(pack it, unpack it, check nothing changed), plus tests for the fiddly parts:
the bit-packed fields in `FrameHeader`, the reserved bytes in `FrameAddress`
and the size field in `Packet`. CI also runs sanitizers, clang-tidy and
clang-format on every push.

## Related

- [clifx-hw-esp32](https://github.com/AndrewFila/clifx-hw-esp32): a
  hardware light controller on an ESP32, still early
- clifx-tui: a terminal UI for your lights, coming soon

## License

GPL-3.0
