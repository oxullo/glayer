/*
Glayer MP3 player

Copyright (C) 2018  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
