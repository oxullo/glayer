/*
 * ArcadeButton.cpp
 *
 *  Created on: 21 Dec 2018
 *      Author: xi
 */

#include <Arduino.h>

#include "ArcadeButton.h"

namespace {
    const uint32_t DEBOUNCE_TIME = 80;
    const uint32_t LONGPRESS_THRESHOLD = 1500;
}

ArcadeButton::ArcadeButton() :
	button_pin_(-1), led_pin_(-1), last_state_(-1), ts_last_action_(0)
{
}

void ArcadeButton::begin(int button_pin, int led_pin)
{
    button_pin_ = button_pin;
    led_pin_ = led_pin;

    pinMode(button_pin, INPUT_PULLUP);
    pinMode(led_pin, OUTPUT);
}

ArcadeButton::Action ArcadeButton::update()
{
    int current_state = digitalRead(button_pin_);

    if (current_state != last_state_ && (millis() - DEBOUNCE_TIME > ts_last_action_)) {
        ts_last_action_ = millis();
        last_state_ = current_state;

        return current_state == LOW ? ACTION_PRESSED : ACTION_RELEASED;
    } else {
        if (current_state == LOW && (millis() - LONGPRESS_THRESHOLD > ts_last_action_)) {
            return ACTION_LONGPRESS;
        } else {
            return ACTION_NONE;
        }
    }
}

void ArcadeButton::set_led(bool on)
{
    digitalWrite(led_pin_, on ? HIGH : LOW);
}
