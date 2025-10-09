#include "websocket_client.h"
#include "config.h"
#include "wifi_manager.h"
#include "led_indicator.h"

WebSocketClient wsClient;

WebSocketClient::WebSocketClient() : isConnected(false)
{
}

bool WebSocketClient::connect(const IPAddress &serverIP)
{
    Serial.println("Connecting to WebSocket server...");

    // Set WebSocket event handler
    webSocket.onEvent(webSocketEventWrapper);

    // Connect to WebSocket server
    webSocket.begin(serverIP, WEBSOCKET_PORT, WEBSOCKET_PATH);

    // Set reconnect interval
    webSocket.setReconnectInterval(WEBSOCKET_RECONNECT_INTERVAL);

    // Wait for connection
    unsigned long startTime = millis();

    while (millis() - startTime < WEBSOCKET_TIMEOUT && !isConnected)
    {
        webSocket.loop();
        delay(100);
    }

    if (!isConnected)
    {
        Serial.println("Failed to connect to WebSocket server");
    }

    wsClient.sendAreButton();

    return isConnected;
}

bool WebSocketClient::isWebSocketConnected()
{
    return isConnected;
}

void WebSocketClient::loop()
{
    if (isConnected)
    {
        webSocket.loop();
    }
}

void WebSocketClient::sendAreButton()
{
    if (isConnected)
    {
        StaticJsonDocument<JSON_BUFFER_SIZE_SMALL> doc;
        doc["type"] = "button";

        String message;
        serializeJson(doc, message);

        webSocket.sendTXT(message);
        Serial.println("ARE button press sent to server");
    }
}

void WebSocketClient::sendButtonPress()
{
    if (isConnected)
    {
        StaticJsonDocument<JSON_BUFFER_SIZE_SMALL> doc;
        doc["type"] = "buttonPress";
        doc["timestamp"] = millis();

        String message;
        serializeJson(doc, message);

        webSocket.sendTXT(message);
        Serial.println("Button press sent to server");
    }
}

void WebSocketClient::sendMessage(const String &message)
{
    if (isConnected)
    {
        webSocket.sendTXT(message.c_str());
        Serial.print("Message sent to server: ");
        Serial.println(message);
    }
}

void WebSocketClient::handleMessage(const char *message)
{
    StaticJsonDocument<JSON_BUFFER_SIZE_LARGE> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (!error)
    {
        String messageType = doc["type"];

        if (messageType == "ledControl")
        {
            String action = doc["action"];
            if (action == "on")
            {
                ledIndicator.turnOn();
            }
            else if (action == "off")
            {
                ledIndicator.turnOff();
            }
        }
    }
}

void WebSocketClient::disconnect()
{
    webSocket.disconnect();
    isConnected = false;
}

void WebSocketClient::reset()
{
    disconnect();
    authToken = "";
}

// Static wrapper function to handle WebSocket events
void WebSocketClient::webSocketEventWrapper(WStype_t type, uint8_t *payload, size_t length)
{
    wsClient.webSocketEvent(type, payload, length);
}

void WebSocketClient::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("WebSocket Disconnected");
        isConnected = false;
        ledIndicator.setConnectionStatus(false);
        break;

    case WStype_CONNECTED:
        Serial.print("WebSocket Connected to: ");
        Serial.println((char *)payload);
        isConnected = true;

        // Turn on LED to indicate successful connection
        ledIndicator.setConnectionStatus(true);
        break;

    case WStype_TEXT:
        Serial.print("WebSocket message received: ");
        Serial.println((char *)payload);

        // Handle server messages
        handleMessage((char *)payload);
        break;

    case WStype_BIN:
        Serial.println("WebSocket binary message received");
        break;

    case WStype_ERROR:
        Serial.println("WebSocket Error");
        isConnected = false;
        ledIndicator.setConnectionStatus(false);
        break;

    default:
        break;
    }
}