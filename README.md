# clifx

**Description**

clifx is a C++20 implementation of the [LIFX LAN protocol](https://lan.developer.lifx.com/docs) message layer — it packs and unpacks LIFX packets to and from raw byte buffers.

clifx is a **library only**. It has no networking code, no sockets, and no CLI — it just builds and parses the bytes that go on the wire. This is intentional: a byte-packing library can be shared as-is between very different consumers (a desktop CLI over POSIX sockets, an ESP32 device over lwIP, etc.), each of which owns its own transport layer.

**What's in the repo**

- `utils/packable.hpp` — the `Utils::Packable` interface (`Pack`/`Unpack`) and the `read`/`write` byte helpers every packet type is built on.
- `packets/base.hpp` / `base.cpp` — the LIFX frame layout: `FrameHeader`, `FrameAddress`, `ProtocolHeader`, `Header`, `Packet`, plus shared value types `HSBK` (hue/saturation/brightness/kelvin) and `Tile`.
- `packets/getters.hpp` / `getters.cpp` — `CLifx::Get::*` payloads: the `State*` responses devices send back (e.g. `StatePower`, `StateColor`/`LightState`, `StateGroup`).
- `packets/setters.hpp` / `setters.cpp` — `CLifx::Set::*` payloads: the `Set*` commands you send to a device (e.g. `SetPower`, `SetColor`, `SetWaveform`).

Getters and setters are split into separate `Get`/`Set` namespaces because several message pairs share a name (both a `Get::PowerPayload` and a `Set::PowerPayload` exist, for the `StatePower` and `SetPower` messages respectively).

**Building**

```sh
cmake -B build
cmake --build build
```

This produces `libclifx.a`. `CMakeLists.txt` exposes the repo root as a public include directory, so a consuming project can point at this repo (as a subdirectory, submodule, or installed package) and include headers as shown below.

**Usage**

Every packet type implements `Pack(buffer, offset)` / `Unpack(buffer, offset)`. Fields must be read/written in the order they're declared in the header — the wire format is positional, not named.

*Building a `SetColor` packet:*

```cpp
#include "packets/setters.hpp"
#include <vector>

int main() {
    CLifx::Packet pkt;
    pkt.header.frame_header.tagged = true;      // true = broadcast to all devices
    pkt.header.frame_header.source = 0x12345678; // arbitrary ID identifying this client

    auto *payload      = pkt.setPayload<CLifx::Set::ColorPayload>();
    payload->color.hue        = 0;       // 0   =   0° (red)
    payload->color.saturation = 0xFFFF;  // full saturation
    payload->color.brightness = 0xFFFF;  // full brightness
    payload->color.kelvin     = 3500;
    payload->duration         = 1000;    // ms to transition over

    std::vector<std::uint8_t> buffer;
    std::size_t offset = 0;
    pkt.Pack(buffer, offset);
    // buffer now holds a complete LIFX packet ready to send over UDP
    // to 255.255.255.255:56700 (broadcast) or a device's IP:56700.
}
```

*Parsing a received `StateService` (discovery) response:*

```cpp
#include "packets/getters.hpp"

void handleResponse(const std::vector<std::uint8_t> &buffer) {
    CLifx::Packet pkt;
    std::size_t offset = 0;
    pkt.header.Unpack(buffer, offset); // unpack the header first...

    if (pkt.header.protocol_header.type == CLifx::Get::ServicePayload::MessageType) {
        CLifx::Get::ServicePayload service;
        service.Unpack(buffer, offset); // ...then the payload that follows it
        // service.port now holds the device's service port
    }
}
```

`Packet::Unpack` will also unpack `payload` for you, but only if one is already assigned — since the payload type depends on the message type read from the header, unpack the header first, look at `protocol_header.type`, then construct and unpack the matching payload (as above), or assign `pkt.payload` yourself before calling `pkt.Unpack(...)` if you already know the type.

**Message types**

A representative sample of what's implemented — see `packets/getters.hpp` / `packets/setters.hpp` for the full list, including MultiZone and Tile messages.

| Purpose            | Set (`CLifx::Set::`)       | State (`CLifx::Get::`)     |
|--------------------|-----------------------------|-----------------------------|
| Power               | `PowerPayload` (21)         | `PowerPayload` (22)         |
| Label               | `LabelPayload` (24)         | `LabelPayload` (25)         |
| Color               | `ColorPayload` (102)        | `ColorPayload` (107)        |
| Waveform            | `WaveformPayload` (103)     | —                            |
| Light power         | `LightPowerPayload` (117)   | —                            |
| Infrared            | `InfraredPayload` (122)     | `InfraredPayload` (121)     |
| Discovery           | —                            | `ServicePayload` (3)        |

**Not included (by design)**

- Sockets / UDP transport — bring your own (POSIX sockets, lwIP, etc.)
- Device discovery logic — build it on top of `Get::ServicePayload`
- A CLI or GUI — see the companion `clifx-cli` project

**Future plans**

- Round out remaining LIFX message payloads not yet covered.
- A separate `clifx-cli` project (POSIX sockets) that depends on this library.
- A separate `clifx-hw-esp32` project (lwIP) that depends on this library.
