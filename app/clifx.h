#ifndef CLIFX_H
#define CLIFX_H

#include "m_types.h"
#include <iostream>
#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define LIFX_PORT 56700

class clifx{
private:
    struct sockaddr_in servaddr;
    int sockfd;
    int broadcast;
    void encodeHeader(struct header_t *, uint16_t, bool, bool, uint32_t, uint8_t[8], bool, bool, uint8_t, uint16_t);
public:
    clifx();
    ~clifx();
    void setLightPower(bool, uint32_t, uint8_t[8]);
    void setColor(uint16_t, float, float, uint16_t, uint32_t, uint8_t[8]);
    void setWaveform(bool, uint16_t, float, float, uint16_t, uint32_t, float, float, uint8_t, uint8_t[8]);
};

#endif /* CLIFX_H */
