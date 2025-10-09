#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager
{
private:
    bool isConnected;

public:
    WiFiManager();

    // Initialize and connect to WiFi
    bool connect();

    // Check if currently connected
    bool isWiFiConnected();

    // Get current IP address
    IPAddress getIPAddress();

    // Get MAC address
    String getMACAddress();

    // Get broadcast IP address
    IPAddress getBroadcastIP();

    // Disconnect from WiFi
    void disconnect();

    // Print connection status
    void printStatus();
};

extern WiFiManager wifiManager;

#endif // WIFI_MANAGER_H