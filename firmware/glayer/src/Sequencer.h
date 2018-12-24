/*
 * Sequencer.h
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
 */

#ifndef SRC_SEQUENCER_H_
#define SRC_SEQUENCER_H_

#include <Adafruit_VS1053.h>


class Sequencer {
public:
    Sequencer();

    void begin(Adafruit_VS1053_FilePlayer *player);
    void reset();
    bool start(const char *folder);
    void next();
    void previous();

private:
    Adafruit_VS1053_FilePlayer *player_;
    char current_folder_[18];
    char files_map_[32][16];
    uint8_t current_track_ptr_;
    uint8_t entries_count_;

    void play_current();
};

#endif /* SRC_SEQUENCER_H_ */
