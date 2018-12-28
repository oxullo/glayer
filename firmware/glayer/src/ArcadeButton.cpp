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

#include "ArcadeButton.h"

namespace {
    const uint32_t DEBOUNCE_TIME = 80;
    const uint32_t LONGPRESS_THRESHOLD = 1200;
}

ArcadeButton::ArcadeButton() :
	button_pin_(-1), led_pin_(-1), last_state_(-1),
	ts_debounce_(0), ts_pressed_(0)
{
}

void ArcadeButton::begin(int button_pin, int led_pin)
{
    button_pin_ = button_pin;
    led_pin_ = led_pin;

    last_state_ = HIGH;

    pinMode(button_pin, INPUT_PULLUP);
    pinMode(led_pin, OUTPUT);
}

ArcadeButton::Action ArcadeButton::update()
{
    int current_state = digitalRead(button_pin_);

    if (millis() - DEBOUNCE_TIME > ts_debounce_) {
        if (current_state != last_state_) {
            ts_debounce_ = millis();
            last_state_ = current_state;

            digitalWrite(led_pin_, !current_state);

            if (current_state == HIGH) {
                if (millis() - LONGPRESS_THRESHOLD > ts_pressed_) {
                    return ACTION_RELEASED;
                } else {
                    return ACTION_CLICKED;
                }
            } else {
                ts_pressed_ = millis();
                return ACTION_PRESSED;
            }
        } else if (current_state == LOW
                && (millis() - LONGPRESS_THRESHOLD > ts_pressed_)) {
            return ACTION_LONGPRESS;
        }
    }

    return ACTION_NONE;
}

void ArcadeButton::set_led(bool on)
{
    digitalWrite(led_pin_, on ? HIGH : LOW);
}
