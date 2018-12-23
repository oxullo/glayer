/*
 * ArcadeButton.h
 *
 *  Created on: 21 Dec 2018
 *      Author: xi
 */

#ifndef SRC_ARCADEBUTTON_H_
#define SRC_ARCADEBUTTON_H_

#include <stdint.h>

class ArcadeButton {
public:
	ArcadeButton(int button_pin, int led_pin);

	typedef enum Action {
	    ACTION_NONE,
	    ACTION_PRESSED,
	    ACTION_RELEASED
	} Action;

	void begin();
	Action update();

private:
	int button_pin_;
	int led_pin_;
	int last_state_;
	uint32_t ts_last_action_;
};

#endif /* SRC_ARCADEBUTTON_H_ */
