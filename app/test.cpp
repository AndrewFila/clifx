#include "../packets/setters.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

static constexpr std::uint16_t LIFX_PORT = 56700U;
static constexpr int SCAN_TIMEOUT = 2; // seconds
static constexpr std::size_t MAX_DEVICES = 16U;
static constexpr std::uint32_t SOURCE_ID = 0xC11FCAFE;

// GetService (type 2) has no payload — just set the type directly
static std::vector<std::uint8_t> buildGetService() {
    CLifx::Packet pkt;
    pkt.header.frame_header.tagged = true;
    pkt.header.frame_header.source = SOURCE_ID;
    pkt.header.protocol_header.type = 2U;
    std::vector<std::uint8_t> buf;
    std::size_t offset = 0;
    pkt.Pack(buf, offset);
    return buf;
}

static std::vector<std::uint8_t> buildSetColor(const CLifx::HSBK &color) {
    CLifx::Packet pkt;
    pkt.header.frame_header.tagged = true;
    pkt.header.frame_header.source = SOURCE_ID;
    auto *payload = pkt.setPayload<CLifx::Set::ColorPayload>();
    payload->color = color;
    payload->duration = 500U;
    std::vector<std::uint8_t> buf;
    std::size_t offset = 0;
    pkt.Pack(buf, offset);
    return buf;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in bindAddr{};
    bindAddr.sin_family      = AF_INET;
    bindAddr.sin_port        = htons(LIFX_PORT);
    bindAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, reinterpret_cast<sockaddr *>(&bindAddr), sizeof(bindAddr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    // Broadcast GetService
    sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family      = AF_INET;
    broadcastAddr.sin_port        = htons(LIFX_PORT);
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;
    auto getServiceBuf = buildGetService();
    ssize_t sent = sendto(sockfd, getServiceBuf.data(), getServiceBuf.size(), 0,
                          reinterpret_cast<sockaddr *>(&broadcastAddr), sizeof(broadcastAddr));
    if (sent < 0) {
        perror("sendto");
        close(sockfd);
        return 1;
    }
    std::cout << "Sent " << sent << " byte GetService broadcast.\n";

    // Wait up to SCAN_TIMEOUT seconds for StateService (type 3) responses
    timeval tv{};
    tv.tv_sec = SCAN_TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    std::cout << "Scanning for LIFX devices...\n";
    std::vector<std::string> devices;
    std::uint8_t recvBuf[256];
    sockaddr_in senderAddr{};
    socklen_t addrLen = sizeof(senderAddr);

    while (devices.size() < MAX_DEVICES) {
        ssize_t n = recvfrom(sockfd, recvBuf, sizeof(recvBuf), 0,
                             reinterpret_cast<sockaddr *>(&senderAddr), &addrLen);
        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "Scan complete (timeout).\n";
            } else {
                perror("recvfrom");
            }
            break;
        }

        // Message type sits at byte 32 of the header
        // (FrameHeader=8, FrameAddress=16, ProtocolHeader reserved=8, then type)
        static constexpr int TYPE_OFFSET = 32;
        if (n < TYPE_OFFSET + 2) { continue; }
        std::uint16_t msgType = 0;
        std::memcpy(&msgType, recvBuf + TYPE_OFFSET, sizeof(msgType));
        std::cout << "  Received packet type " << msgType
                  << " from " << inet_ntoa(senderAddr.sin_addr) << "\n";
        if (msgType != 3U) { continue; } // not StateService

        std::string ip = inet_ntoa(senderAddr.sin_addr);
        bool already = false;
        for (const auto &d : devices) {
            if (d == ip) {
                already = true;
                break;
            }
        }
        if (!already) {
            devices.push_back(ip);
            std::cout << "  Found device: " << ip << "\n";
        }
    }

    if (devices.empty()) {
        std::cout << "No devices found.\n";
        close(sockfd);
        return 0;
    }

    std::cout << "\nSelect a color:\n"
              << "  1) Red\n"
              << "  2) Green\n"
              << "  3) Blue\n"
              << "> ";
    int choice = 0;
    std::cin >> choice;

    CLifx::HSBK color;
    color.saturation = 0xFFFFU;
    color.brightness = 0xFFFFU;
    color.kelvin = 3500U;
    switch (choice) {
    case 1:
        color.hue = 0U;
        break; // Red   (  0°)
    case 2:
        color.hue = 21845U;
        break; // Green (120°)
    case 3:
        color.hue = 43690U;
        break; // Blue  (240°)
    default:
        std::cout << "Invalid choice.\n";
        close(sockfd);
        return 0;
    }

    auto setColorBuf = buildSetColor(color);
    for (const auto &ip : devices) {
        sockaddr_in target{};
        target.sin_family = AF_INET;
        target.sin_port = htons(LIFX_PORT);
        inet_aton(ip.c_str(), &target.sin_addr);
        sendto(sockfd, setColorBuf.data(), setColorBuf.size(), 0,
               reinterpret_cast<sockaddr *>(&target), sizeof(target));
        std::cout << "Sent to " << ip << "\n";
    }

    close(sockfd);
    return 0;
}
