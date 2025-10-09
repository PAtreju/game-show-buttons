#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

#include <Arduino.h>

class LEDIndicator
{
private:
    int ledPin;
    bool currentState;

public:
    LEDIndicator();

    // Initialize LED
    void begin();

    // Basic LED control
    void turnOn();
    void turnOff();
    void toggle();
    void setState(bool state);

    // Status indication patterns
    void blinkWiFiConnected();                // 3 medium blinks
    void blinkAckReceived();                  // 5 fast blinks
    void blinkSocketConnected();              // 2 fast blinks
    void blinkAuthSuccess();                  // 3 slow blinks
    void setConnectionStatus(bool connected); // Solid on/off

    // Custom blink pattern
    void blink(int count, int onTime, int offTime);

    // Get current state
    bool getState();
};

extern LEDIndicator ledIndicator;

#endif // LED_INDICATOR_H