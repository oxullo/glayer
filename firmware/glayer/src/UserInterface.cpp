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

#include "UserInterface.h"

namespace {
const int RIGHT_LED_PIN = 16;
const int RIGHT_BTN_PIN = 17;
const int LEFT_LED_PIN = 18;
const int LEFT_BTN_PIN = 19;
}

UserInterface::UserInterface()
{
}

void UserInterface::begin()
{
    left.begin(LEFT_BTN_PIN, LEFT_LED_PIN);
    right.begin(RIGHT_BTN_PIN, RIGHT_LED_PIN);

    for (uint8_t i = 0 ; i < 5 ; ++i) {
        left.set_led(true);
        right.set_led(true);
        delay(20);
        left.set_led(false);
        right.set_led(false);
        delay(100);
    }
}

void UserInterface::reset()
{
    left.set_led(false);
    right.set_led(false);
}

UserInterface::Action UserInterface::update()
{
    ArcadeButton::Action left_action = left.update();
    ArcadeButton::Action right_action = right.update();

    if (left_action == ArcadeButton::ACTION_LONGPRESS) {
        return UserInterface::ACTION_DECREASE_VOLUME;
    } else if (left_action == ArcadeButton::ACTION_CLICKED) {
        return UserInterface::ACTION_PREVIOUS_TRACK;
    } else if (right_action == ArcadeButton::ACTION_LONGPRESS) {
        return UserInterface::ACTION_INCREASE_VOLUME;
    } else if (right_action == ArcadeButton::ACTION_CLICKED) {
        return UserInterface::ACTION_NEXT_TRACK;
    } else {
        return UserInterface::ACTION_NONE;
    }
}

void UserInterface::blink()
{
    left.set_led(true);
    delay(50);
    right.set_led(true);
    delay(50);
    left.set_led(false);
    delay(50);
    right.set_led(false);
}

void UserInterface::set_fatal_error(uint8_t error)
{
    for (uint8_t j = 0 ; j < 5 ; ++j) {
        for (uint8_t i = 0 ; i < error ; ++i) {
            left.set_led(true);
            right.set_led(true);
            delay(100);
            left.set_led(false);
            right.set_led(false);
            delay(200);
        }

        delay(2000);
    }

    NVIC_SystemReset();
}
