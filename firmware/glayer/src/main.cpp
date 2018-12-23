#include <Arduino.h>

#include "ArcadeButton.h"

const int LED_UP_PIN = 16;
const int PB_UP_PIN = 17;
const int LED_DN_PIN = 18;
const int PB_DN_PIN = 19;

ArcadeButton button_up(PB_UP_PIN, LED_UP_PIN), button_down(PB_DN_PIN, LED_DN_PIN);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if (button_up.update() != ArcadeButton::ACTION_NONE) {
        Serial.println("Button UP");
    }
}
