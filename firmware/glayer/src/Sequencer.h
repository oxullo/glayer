/*
 * Sequencer.h
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
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
