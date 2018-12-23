#include <Arduino.h>

const int LED_UP_PIN = 16;
const int PB_UP_PIN = 17;
const int LED_DN_PIN = 18;
const int PB_DN_PIN = 19;

void setup()
{
    pinMode(LED_UP_PIN, OUTPUT);
    pinMode(LED_DN_PIN, OUTPUT);
    pinMode(PB_UP_PIN, INPUT_PULLUP);
    pinMode(PB_DN_PIN, INPUT_PULLUP);
}

void loop()
{
    digitalWrite(LED_UP_PIN, !digitalRead(PB_UP_PIN));
    digitalWrite(LED_DN_PIN, !digitalRead(PB_DN_PIN));
}
