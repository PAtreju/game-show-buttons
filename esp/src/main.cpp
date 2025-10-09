#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "udp_handler.h"
#include "websocket_client.h"
#include "led_indicator.h"

// Global state variables
bool systemInitialized = false;
unsigned long lastRetryTime = 0;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms debounce delay

// Function declarations
void initializeSystem();
void handleReconnection();
void resetSystem();
void handleButtonPress();

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting ESP32 Game Show Button...");

  // Initialize GPIO0 as input with internal pull-down
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize LED
  ledIndicator.begin();

  // Initialize system
  initializeSystem();
}

void loop()
{
  // Check button state with debouncing
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (currentButtonState != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  // if ((millis() - lastDebounceTime) > debounceDelay)
  // {
  // Button state has been stable for debounce period
  if (currentButtonState == LOW && lastButtonState == HIGH)
  {
    // Button was just pressed (transition from HIGH to LOW)
    Serial.println("Button pressed!");
    handleButtonPress();
  }
  // }

  lastButtonState = currentButtonState;

  // Handle WebSocket events if connected
  if (wsClient.isWebSocketConnected())
  {
    wsClient.loop();
  }

  // Handle reconnection logic
  handleReconnection();

  delay(100);
}

void initializeSystem()
{
  // Step 1: Connect to WiFi
  if (!wifiManager.connect())
  {
    Serial.println("WiFi connection failed. Will retry in main loop.");
    return;
  }

  // Step 2: Broadcast UDP packet to announce presence
  if (!udpHandler.broadcastPresence())
  {
    Serial.println("UDP broadcast failed. Will retry in main loop.");
    return;
  }

  // Step 3: Listen for token response
  if (!udpHandler.listenForAck())
  {
    Serial.println("No acknowledgment received. Will retry in main loop.");
    return;
  }

  // Step 4: Connect to WebSocket with received token
  if (!wsClient.connect(udpHandler.getServerIP()))
  {
    Serial.println("WebSocket connection failed. Will retry in main loop.");
    return;
  }

  systemInitialized = true;
  Serial.println("System initialization completed successfully!");
}

void handleReconnection()
{
  // Check if we need to retry the connection process
  if (!systemInitialized || !wifiManager.isWiFiConnected() ||
      !udpHandler.isAckReceived() || !wsClient.isWebSocketConnected())
  {

    if (millis() - lastRetryTime > RETRY_INTERVAL)
    {
      lastRetryTime = millis();
      Serial.println("Retrying connection process...");

      // Reset states if needed
      if (!wifiManager.isWiFiConnected())
      {
        Serial.println("WiFi disconnected, resetting system...");
        resetSystem();
        initializeSystem();
      }
      else if (!udpHandler.isAckReceived())
      {
        Serial.println("Acknowledgment not received, retrying UDP process...");
        udpHandler.broadcastPresence();
        if (udpHandler.listenForAck())
        {
          wsClient.connect(udpHandler.getServerIP());
        }
      }
      else if (!wsClient.isWebSocketConnected())
      {
        Serial.println("WebSocket disconnected, attempting reconnection...");
        wsClient.connect(udpHandler.getServerIP());
      }
    }
  }
}

void resetSystem()
{
  systemInitialized = false;
  udpHandler.resetAck();
  wsClient.reset();
  ledIndicator.turnOff();
}

// Example function to handle button press (call this when button is pressed)
void handleButtonPress()
{
  if (wsClient.isWebSocketConnected())
  {
    wsClient.sendButtonPress();
  }
  else
  {
    Serial.println("Cannot send button press - WebSocket not connected");
  }
}