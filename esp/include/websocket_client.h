#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <WebSocketsClient.h>
#include <ArduinoJson.h>

class WebSocketClient
{
private:
    WebSocketsClient webSocket;
    bool isConnected;
    String authToken;

    // Event handler callback
    static void webSocketEventWrapper(WStype_t type, uint8_t *payload, size_t length);
    void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

public:
    WebSocketClient();

    // Connect to WebSocket server
    bool connect(const IPAddress &serverIP);

    // Check if connected
    bool isWebSocketConnected();

    // Handle WebSocket loop
    void loop();

    // Send ARE button event
    void sendAreButton();

    // Send button press event
    void sendButtonPress();

    // Send custom message
    void sendMessage(const String &message);

    // Handle incoming messages
    void handleMessage(const char *message);

    // Disconnect from server
    void disconnect();

    // Reset connection state
    void reset();
};

extern WebSocketClient wsClient;

#endif // WEBSOCKET_CLIENT_H