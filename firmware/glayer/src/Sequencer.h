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


#ifndef SRC_SEQUENCER_H_
#define SRC_SEQUENCER_H_

#include <Adafruit_VS1053.h>

#define MAX_ENTRIES         32
#define MAX_ENTRY_LENGTH    16


class Sequencer {
public:
    Sequencer();

    void begin(Adafruit_VS1053_FilePlayer *player);
    void reset();
    bool start(const char *folder);
    void next();
    void previous();
    void check();

private:
    Adafruit_VS1053_FilePlayer *player_;
    char current_folder_[MAX_ENTRY_LENGTH];
    char files_map_[MAX_ENTRIES][MAX_ENTRY_LENGTH];
    uint8_t current_track_ptr_;
    uint8_t entries_count_;

    void play_current();
    void load_entries(const char* folder);
    void sort_entries();
};

#endif /* SRC_SEQUENCER_H_ */
