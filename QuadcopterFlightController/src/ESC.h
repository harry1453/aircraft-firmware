#ifndef ESC_H
#define ESC_H

#include <stdint.h>
#include "DShot.h"

namespace ESC {
    const uint16_t MAX_THROTTLE = 2000;

    /**
     * @param pins The pins attached to each motor in order: FL, FR, RL, RR
     */
    void init(const uint8_t pins[DShot::ESC_COUNT]);
    void loop();

    const uint8_t FL = 0;
    const uint8_t FR = 1;
    const uint8_t RL = 2;
    const uint8_t RR = 3;

    /**
     * Sets the throttle level
     * @param throttle The throttle level. Must be in range 0-2000.
     */
    void setThrottle(uint8_t motor, uint16_t throttle);
}

#endif //ESC_H
