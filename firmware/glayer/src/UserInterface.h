/*
 * UserInterface.h
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
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
    void set_fatal_error(uint8_t error);

private:
    ArcadeButton left, right;
};

#endif /* SRC_USERINTERFACE_H_ */
