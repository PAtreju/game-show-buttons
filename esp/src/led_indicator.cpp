#include "led_indicator.h"
#include "config.h"

LEDIndicator ledIndicator;

LEDIndicator::LEDIndicator() : ledPin(LED_PIN), currentState(false)
{
}

void LEDIndicator::begin()
{
    pinMode(ledPin, OUTPUT);
    turnOff();
}

void LEDIndicator::turnOn()
{
    digitalWrite(ledPin, HIGH);
    currentState = true;
}

void LEDIndicator::turnOff()
{
    digitalWrite(ledPin, LOW);
    currentState = false;
}

void LEDIndicator::toggle()
{
    currentState = !currentState;
    digitalWrite(ledPin, currentState ? HIGH : LOW);
}

void LEDIndicator::setState(bool state)
{
    currentState = state;
    digitalWrite(ledPin, state ? HIGH : LOW);
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