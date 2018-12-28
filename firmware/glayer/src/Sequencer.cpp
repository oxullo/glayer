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


#include <string.h>

#include <Arduino.h>
#include <SD.h>

#include "Sequencer.h"


Sequencer::Sequencer() :
    player_(NULL), current_track_ptr_(0), entries_count_(0)
{
}

void Sequencer::begin(Adafruit_VS1053_FilePlayer* player)
{
    player_ = player;
}

void Sequencer::reset()
{
    player_->stopPlaying();
    entries_count_ = 0;
}

bool Sequencer::start(const char* folder)
{
    Serial.print(F("Selecting playback folder: "));
    Serial.println(folder);

    strcpy(current_folder_, folder);
    current_track_ptr_ = 0;

    load_entries(folder);
    sort_entries();

    Serial.print(F("Entries count: "));
    Serial.println(entries_count_);

    for (uint8_t i = 0 ; i < entries_count_ ; ++i) {
        Serial.println(files_map_[i]);
    }

    if (entries_count_ != 0) {
        play_current();
        return true;
    } else {
        return false;
    }
}

void Sequencer::next()
{
    if (entries_count_ == 0) {
        return;
    }

    current_track_ptr_ = (current_track_ptr_ + 1) % entries_count_;

    play_current();
}

void Sequencer::previous()
{
    if (entries_count_ == 0) {
        return;
    }

    if (current_track_ptr_ == 0) {
        current_track_ptr_ = entries_count_ - 1;
    } else {
        --current_track_ptr_;
    }

    play_current();
}

void Sequencer::check()
{
    if (!player_->playingMusic && entries_count_) {
        Serial.println(F("Track playback terminated"));
        next();
    }
}

void Sequencer::play_current()
{
    char scratch_buffer[64];

    sprintf(scratch_buffer, "%s/%s", current_folder_,
            files_map_[current_track_ptr_]);

    Serial.print(F("Playing file: "));
    Serial.println(scratch_buffer);

    player_->stopPlaying();
    player_->startPlayingFile(scratch_buffer);
}

void Sequencer::load_entries(const char* folder)
{
    File folder_root = SD.open(folder);
    folder_root.rewindDirectory();

    File entry;
    entries_count_ = 0;

    while (1) {
        entry = folder_root.openNextFile();

        if (!entry || entries_count_ == MAX_ENTRIES) {
            break;
        }

        if (strstr(entry.name(), ".MP3") == NULL || entry.name()[0] == '_') {
            Serial.print(F("Skipping file: "));
            Serial.println(entry.name());
        } else {
            strcpy(files_map_[entries_count_], entry.name());
            ++entries_count_;
        }

        entry.close();
    }

    folder_root.close();
}

void Sequencer::sort_entries()
{
    uint8_t swapped = 0;
    char scratch_buffer[MAX_ENTRY_LENGTH];

    do {
        swapped = 0;
        for (uint8_t i=1 ; i <= entries_count_ - 1 ; ++i) {
            if (strcmp(files_map_[i-1], files_map_[i]) > 0) {
                strcpy(scratch_buffer, files_map_[i]);
                strcpy(files_map_[i], files_map_[i-1]);
                strcpy(files_map_[i-1], scratch_buffer);

                swapped = 1;
            }
        }
    } while (swapped);
}
