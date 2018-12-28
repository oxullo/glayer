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


#ifndef SRC_LIGHTBARRIER_H_
#define SRC_LIGHTBARRIER_H_

class LightBarrier {
public:
    LightBarrier(int led_pin, int collector_pin);

    bool check_card();

private:
    int led_pin_;
    int collector_pin_;
};

#endif /* SRC_LIGHTBARRIER_H_ */
