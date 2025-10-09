#include "udp_handler.h"
#include "config.h"
#include "wifi_manager.h"
#include "led_indicator.h"

UDPHandler udpHandler;

UDPHandler::UDPHandler() : ackReceived(false)
{
}

bool UDPHandler::broadcastPresence()
{
    if (!wifiManager.isWiFiConnected())
    {
        Serial.println("WiFi not connected, cannot broadcast UDP");
        return false;
    }

    Serial.println("Broadcasting UDP packet to announce presence...");

    String message = createAnnouncementMessage();
    IPAddress broadcastIP = wifiManager.getBroadcastIP();

    udp.beginPacket(broadcastIP, UDP_BROADCAST_PORT);
    udp.print(message);
    udp.endPacket();

    Serial.print("UDP broadcast sent to: ");
    Serial.print(broadcastIP);
    Serial.print(":");
    Serial.println(UDP_BROADCAST_PORT);
    Serial.print("Message: ");
    Serial.println(message);

    return true;
}

bool UDPHandler::listenForAck()
{
    Serial.println("Listening for server acknowledgment...");

    udp.begin(UDP_LISTEN_PORT);

    unsigned long startTime = millis();

    while (millis() - startTime < UDP_TIMEOUT && !ackReceived)
    {
        int packetSize = udp.parsePacket();
        IPAddress remoteIP = udp.remoteIP();
        if (packetSize)
        {
            Serial.print("Received UDP packet of size ");
            Serial.println(packetSize);

            char packetBuffer[UDP_PACKET_BUFFER_SIZE];
            int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
            if (len > 0)
            {
                packetBuffer[len] = '\0';

                Serial.print("UDP packet contents: ");
                Serial.println(packetBuffer);

                if (parseAckResponse(packetBuffer))
                {
                    serverIP = remoteIP;
                    ledIndicator.blinkAckReceived();
                    break;
                }
            }
        }
        delay(100);
    }

    udp.stop();

    if (!ackReceived)
    {
        Serial.println("No acknowledgment received within timeout period");
    }

    return ackReceived;
}

IPAddress UDPHandler::getServerIP()
{
    return serverIP;
}

bool UDPHandler::isAckReceived()
{
    return ackReceived;
}

void UDPHandler::resetAck()
{
    ackReceived = false;
}

String UDPHandler::createAnnouncementMessage()
{
    StaticJsonDocument<JSON_BUFFER_SIZE_MEDIUM> doc;
    doc["type"] = "presence";

    String message;
    serializeJson(doc, message);
    return message;
}

bool UDPHandler::parseAckResponse(const char *message)
{
    StaticJsonDocument<JSON_BUFFER_SIZE_LARGE> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (!error)
    {
        if (doc["type"] == "ack")
        {
            ackReceived = true;
            Serial.println("Acknowledgment received");
            return true;
        }
    }
    else
    {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
    }

    return false;
}