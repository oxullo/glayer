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


#ifndef SRC_ARCADEBUTTON_H_
#define SRC_ARCADEBUTTON_H_

#include <stdint.h>

class ArcadeButton {
public:
    typedef enum Action {
        ACTION_NONE,
        ACTION_PRESSED,
        ACTION_CLICKED,
        ACTION_RELEASED,
        ACTION_LONGPRESS
    } Action;

    ArcadeButton();

    void begin(int button_pin, int led_pin);
    Action update();
    void set_led(bool on);

private:
    int button_pin_;
    int led_pin_;
    int last_state_;
    uint32_t ts_debounce_;
    uint32_t ts_pressed_;
};

#endif /* SRC_ARCADEBUTTON_H_ */
