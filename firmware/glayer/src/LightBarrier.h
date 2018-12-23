/*
 * LightBarrier.h
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
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