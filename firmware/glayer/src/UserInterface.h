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
    UserInterface();

    void begin();
    void reset();
    void update();
    void set_fatal_error(uint8_t error);

private:
    ArcadeButton left, right;
};

#endif /* SRC_USERINTERFACE_H_ */
