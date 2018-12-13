#include <Arduino.h>

const int PIN_LB_LED = 14;
const int PIN_LB_PHD = 15;
const uint16_t PULSE_DURATION = 50;
const uint16_t POLLING_PERIOD = 100;

uint8_t last_status = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(PIN_LB_LED, OUTPUT);
    pinMode(PIN_LB_PHD, INPUT_PULLUP);
}

void loop()
{
    digitalWrite(PIN_LB_LED, HIGH);
    delayMicroseconds(PULSE_DURATION);
    uint8_t current_status = digitalRead(PIN_LB_PHD);
    digitalWrite(PIN_LB_LED, LOW);

    if (current_status != last_status) {
        Serial.print("Trigger: ");
        Serial.println(current_status);

        last_status = current_status;
    }

    delay(POLLING_PERIOD);
}
