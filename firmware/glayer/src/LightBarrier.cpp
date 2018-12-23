/*
 * LightBarrier.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
 */

#include <Arduino.h>

#include "LightBarrier.h"

namespace {
    const uint16_t PULSE_DURATION_US = 50;
}

LightBarrier::LightBarrier(int led_pin, int collector_pin) :
    led_pin_(led_pin), collector_pin_(collector_pin)
{
    pinMode(led_pin, OUTPUT);
    pinMode(collector_pin, INPUT_PULLUP);
}

bool LightBarrier::check_card()
{
    digitalWrite(led_pin_, HIGH);
    delayMicroseconds(PULSE_DURATION_US);
    uint8_t current_status = digitalRead(collector_pin_);
    digitalWrite(led_pin_, LOW);

    return current_status == HIGH;
}
