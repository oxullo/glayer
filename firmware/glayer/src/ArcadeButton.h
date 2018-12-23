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
    typedef enum Action {
        ACTION_NONE,
        ACTION_PRESSED,
        ACTION_CLICKED,
        ACTION_RELEASED,
        ACTION_LONGPRESS
    } Action;

	ArcadeButton();

	void begin(int button_pin, int led_pin);
	Action update();
	void set_led(bool on);

private:
	int button_pin_;
	int led_pin_;
	int last_state_;
	uint32_t ts_debounce_;
	uint32_t ts_pressed_;
};

#endif /* SRC_ARCADEBUTTON_H_ */
