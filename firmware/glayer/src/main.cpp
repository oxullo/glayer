#include <Arduino.h>

#include "ArcadeButton.h"
#include "LightBarrier.h"


const int LED_UP_PIN = 16;
const int PB_UP_PIN = 17;
const int LED_DN_PIN = 18;
const int PB_DN_PIN = 19;

const int LB_LED_PIN = 14;
const int LB_COLLECTOR_PIN = 15;

ArcadeButton button_up(PB_UP_PIN, LED_UP_PIN), button_down(PB_DN_PIN, LED_DN_PIN);
LightBarrier light_barrier(LB_LED_PIN, LB_COLLECTOR_PIN);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if (light_barrier.check_card()) {
        Serial.println("CARD IN");
        delay(500);
    }
}
