#ifndef M_TYPES_H
#define M_TYPES_H
#include <cstdint>
/////////////////////
//     enums       //
/////////////////////
enum waveform {
    SAW,
    SINE,
    HALF_SINE,
    TRIANGLE,
    PULSE
};

//not yet in use
//enum services{
//    UDP = 1,
//    RESERVED1 = 2,
//    RESERVED2 = 3,
//    RESERVED3 = 4,
//    RESERVED4 = 5
//};
//enum direction {
//    LEFT,
//    RIGHT
//};
//enum lightLastHevCycleResult{
//    SUCESS,
//    BUSY,
//    INTERRUPTED_BY_RESET,
//    INTERRUPTED_BY_HOMEKIT,
//    INTERRUPTED_BY_LAN,
//    INTERRUPTED_BY_CLOUD,
//    NONE = 255
//};
//enum multiZoneApplicationRequest{
//    NO_APPLY,
//    APPLY,
//    APPLY_ONLY
//};
//enum multiZoneEffectType{
//    OFF,
//    MOVE,
//    RESERVED1,
//    RESERVED2
//};
//enum multiZoneExtendedApplicationRequest{
//    NO_APPLY,
//    APPLY,
//    APPLY_ONLY
//};
//enum tileEffectType{
//    OFF,
//    RESERVED1,
//    MORPH,
//    FLAME,
//    RESERVED2
//};

/////////////////////
//     header      //
/////////////////////
struct header_t {
    //frame header
    uint16_t size;
    uint16_t protocol:12;
    uint8_t  addressable:1;
    uint8_t  tagged:1;
    uint8_t  origin:2;
    uint32_t source;

    //frame address
    uint8_t  target[8];
    uint8_t  reserved1[6];
    uint8_t  res_required:1;
    uint8_t  ack_required:1;
    uint8_t  reserved2:6;
    uint8_t  sequence;

    //protocol header
    uint64_t reserved3:64;
    uint16_t msgtype;
    uint16_t reserved4:16;
}__attribute__((packed));

//payload types
struct powpl_t {
    uint16_t power;
    uint32_t duration;
} __attribute__((packed));

struct colpl_t {
    uint8_t reserved;
    uint16_t hue;
    uint16_t saturation;
    uint16_t brightness;
    uint16_t kelvin;
    uint32_t duration;
} __attribute__((packed));

struct wfpl_t {
    uint8_t reserved;
    uint8_t transient;
    uint16_t hue;
    uint16_t saturation;
    uint16_t brightness;
    uint16_t kelvin;
    uint32_t period;
    float    cycles;
    int16_t  skew_ratio;
    uint8_t  waveform;
} __attribute__((packed));

/////////////////
//message types//
/////////////////

//standard message size 36
struct stdmsg_t {
    struct header_t header;
};

//power message size 42
struct powmsg_t {
    struct header_t header;
    struct powpl_t payload;
} __attribute__((packed));

//color message size 49
struct colmsg_t {
    struct header_t header;
    struct colpl_t payload;
} __attribute__((packed));

struct wfmsg_t {
    struct header_t header;
    struct wfpl_t payload;
} __attribute__((packed));

#endif /* M_TYPES_H */
