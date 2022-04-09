#include "clifx.h"
#include <unistd.h>

clifx::clifx(){
    servaddr = {0};
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("Error in creating socket");
        exit(EXIT_FAILURE);
    }
    broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0){
        perror("Error in setting socket options");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(LIFX_PORT);
    servaddr.sin_addr.s_addr = INADDR_BROADCAST;
}

clifx::~clifx(){
    close(sockfd);
}
void clifx::encodeHeader(struct header_t * head, uint16_t sz, bool tag, bool addr, uint32_t src, uint8_t tgt[8], bool res, bool ack, uint8_t seq, uint16_t type){
    head->size          = (sz >> 8) | (sz << 8);
    head->protocol      = 1024;
    head->tagged        = tag;
    head->addressable   = addr;
    head->origin        = 0b00;
    head->source        = src;
    for (int i = 0; i < 8; i++){
        head->target[i] = tgt[i];
    }
    for (int i = 0; i < 6; i++){
        head->reserved1[i] = 0x00;
    }
    head->res_required  = res;
    head->ack_required  = ack;
    head->sequence      = seq;
    head->reserved2     = 0;
    head->msgtype       = type;
    head->reserved3     = 0;

}

///////////////////////////////////////
//             setters               //
///////////////////////////////////////



//setLightPower -- packet 117
void clifx::setLightPower(bool onOff, uint32_t duration, uint8_t target[8]){
    struct powmsg_t msg;
    encodeHeader(&msg.header, 42, 0, 1, 3, target, 0, 1, 1, 117);
    msg.payload.power    = 0xFF * onOff;
    msg.payload.duration = duration;
    int len = sendto(sockfd, &msg, sizeof(msg), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (len < 0){
        perror("Error in sending power message");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}


//setColor -- packet 102
void clifx::setColor(uint16_t hue, float saturation, float brightness, uint16_t kelvin, uint32_t duration, uint8_t target[8]){
    struct colmsg_t msg;
    encodeHeader(&msg.header, 49, 0, 1, 3, target, 0, 1, 1, 102);
    msg.payload.reserved    = 0;
    msg.payload.hue         = 0xFFFF * ((float) hue / 360.);
    msg.payload.saturation  = 0xFFFF * saturation;
    msg.payload.brightness  = 0xFFFF * brightness;
    msg.payload.kelvin      = kelvin;
    msg.payload.duration    = duration;
    int len = sendto(sockfd, &msg, sizeof(msg), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (len < 0){
        perror("Error in sending color message");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}
//setWaveform -- packet 103
//setWaveformOptional -- packet119
//setInfared -- packet 122
//setHevCycle -- packet 143



///////////////////////////////////////
//             getters               //
///////////////////////////////////////

//getService -- packet 2
//char* getService(){
//    uint8_t target[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
//    struct stdmsg_t msg;
//    encodeHeader(&msg.header, );
//}
