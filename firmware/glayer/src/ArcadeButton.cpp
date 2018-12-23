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
