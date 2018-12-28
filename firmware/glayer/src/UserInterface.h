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


#ifndef SRC_USERINTERFACE_H_
#define SRC_USERINTERFACE_H_

#include "ArcadeButton.h"

class UserInterface {
public:
    typedef enum Action {
        ACTION_NONE,
        ACTION_PREVIOUS_TRACK,
        ACTION_NEXT_TRACK,
        ACTION_DECREASE_VOLUME,
        ACTION_INCREASE_VOLUME
    } Action;

    UserInterface();

    void begin();
    void reset();
    UserInterface::Action update();
    void blink();
    void set_fatal_error(uint8_t error);

private:
    ArcadeButton left, right;
};

#endif /* SRC_USERINTERFACE_H_ */
