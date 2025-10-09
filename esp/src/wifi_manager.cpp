#include "wifi_manager.h"
#include "config.h"
#include "led_indicator.h"
#include <WiFi.h>

WiFiManager wifiManager;

WiFiManager::WiFiManager() : isConnected(false)
{
}

bool WiFiManager::connect()
{
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECTION_TIMEOUT)
    {
        delay(1000);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        isConnected = true;
        Serial.println();
        Serial.println("WiFi connected successfully!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

        // Indicate successful WiFi connection
        ledIndicator.blinkWiFiConnected();

        return true;
    }
    else
    {
        isConnected = false;
        Serial.println();
        Serial.println("Failed to connect to WiFi!");
        return false;
    }
}

bool WiFiManager::isWiFiConnected()
{
    bool currentStatus = (WiFi.status() == WL_CONNECTED);
    isConnected = currentStatus;
    return currentStatus;
}

IPAddress WiFiManager::getIPAddress()
{
    return WiFi.localIP();
}

String WiFiManager::getMACAddress()
{
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(buf);
}

IPAddress WiFiManager::getBroadcastIP()
{
    IPAddress broadcastIP = WiFi.localIP();
    broadcastIP[3] = 255; // Set last octet to 255 for broadcast
    return broadcastIP;
}

void WiFiManager::disconnect()
{
    WiFi.disconnect();
    isConnected = false;
}

void WiFiManager::printStatus()
{
    if (isWiFiConnected())
    {
        Serial.println("WiFi Status: Connected");
        Serial.print("IP Address: ");
        Serial.println(getIPAddress());
        Serial.print("MAC Address: ");
        Serial.println(getMACAddress());
    }
    else
    {
        Serial.println("WiFi Status: Disconnected");
    }
}