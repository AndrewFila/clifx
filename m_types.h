#ifndef M_TYPES_H
#define M_TYPES_H
#include <cstdint>


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


#endif /* M_TYPES_H */
