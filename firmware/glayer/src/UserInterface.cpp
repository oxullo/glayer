/*
 * UserInterface.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
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
