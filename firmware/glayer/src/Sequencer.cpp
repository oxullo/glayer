/*
 * Sequencer.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
 */

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

    File folder_root = SD.open(folder);

    folder_root.rewindDirectory();

    File entry;
    entries_count_ = 0;
    while(1) {
        entry = folder_root.openNextFile();

        if (!entry || entries_count_ == 32) {
            break;
        }

        if (strstr(entry.name(), ".MP3") == NULL || entry.name()[0] == '_') {
            Serial.print(F("Skipping file: "));
            Serial.println(entry.name());
            entry.close();

            continue;
        }
        strcpy(files_map_[entries_count_], entry.name());
        ++entries_count_;
        entry.close();
    }
    folder_root.close();

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
