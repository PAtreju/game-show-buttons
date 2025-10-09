#ifndef UDP_HANDLER_H
#define UDP_HANDLER_H

#include <WiFiUdp.h>
#include <ArduinoJson.h>

class UDPHandler
{
private:
    WiFiUDP udp;
    IPAddress serverIP;
    bool ackReceived;

public:
    UDPHandler();

    // Broadcast UDP packet to announce device presence
    bool broadcastPresence();

    // Listen for acknowledgment from server
    bool listenForAck();

    IPAddress getServerIP();

    // Check if acknowledgment was received
    bool isAckReceived();

    // Reset acknowledgment state
    void resetAck();

    // Create device announcement JSON message
    String createAnnouncementMessage();

    // Parse acknowledgment response from server
    bool parseAckResponse(const char *message);
};

extern UDPHandler udpHandler;

#endif // UDP_HANDLER_H