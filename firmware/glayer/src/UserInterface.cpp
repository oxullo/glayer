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
        delay(50);
        left.set_led(false);
        right.set_led(false);
        delay(100);
    }
}

void UserInterface::reset()
{
}

void UserInterface::update()
{
}

void UserInterface::set_fatal_error(uint8_t error)
{
    while(1) {
        for (uint8_t i = 0 ; i < error ; ++i) {
            left.set_led(true);
            right.set_led(true);
            delay(50);
            left.set_led(false);
            right.set_led(false);
            delay(200);
        }

        delay(2000);
    }
}
