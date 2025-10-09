#include <Adafruit_NeoPixel.h>
#include "led_indicator.h"
#include "config.h"

#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

LEDIndicator ledIndicator;

LEDIndicator::LEDIndicator() : ledPin(LED_PIN), currentState(false)
{
}

void LEDIndicator::begin()
{
    pinMode(ledPin, OUTPUT);
    turnOff();
    pixels.begin();
    pixels.clear();

    blink(1, 200, 200); // Initial blink to indicate startup
}

void LEDIndicator::turnOn()
{
    digitalWrite(ledPin, HIGH);
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Green
    pixels.show();
    currentState = true;
}

void LEDIndicator::turnOff()
{
    digitalWrite(ledPin, LOW);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turned off LED
    pixels.show();
    currentState = false;
}

void LEDIndicator::toggle()
{
    currentState = !currentState;
    digitalWrite(ledPin, currentState ? HIGH : LOW);
    if (currentState)
    {
        pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Green
    }
    else
    {
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turned off LED
    }
    pixels.show();
}

void LEDIndicator::setState(bool state)
{
    currentState = state;
    digitalWrite(ledPin, state ? HIGH : LOW);
    if (state)
    {
        pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Green
    }
    else
    {
        pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Turned off LED
    }
    pixels.show();
}

void LEDIndicator::blinkWiFiConnected()
{
    // 3 medium blinks to indicate WiFi connection
    blink(3, 200, 200);
}

void LEDIndicator::blinkAckReceived()
{
    // 5 fast blinks to indicate token received
    blink(5, 100, 100);
}

void LEDIndicator::blinkSocketConnected()
{
    // 2 fast blinks to indicate WebSocket connection
    blink(2, 100, 100);
}

void LEDIndicator::blinkAuthSuccess()
{
    // 3 slow blinks to indicate successful authentication
    for (int i = 0; i < 3; i++)
    {
        turnOff();
        delay(500);
        turnOn();
        delay(500);
    }
}

void LEDIndicator::setConnectionStatus(bool connected)
{
    setState(connected);
}

void LEDIndicator::blink(int count, int onTime, int offTime)
{
    for (int i = 0; i < count; i++)
    {
        turnOn();
        delay(onTime);
        turnOff();
        delay(offTime);
    }
}

bool LEDIndicator::getState()
{
    return currentState;
}