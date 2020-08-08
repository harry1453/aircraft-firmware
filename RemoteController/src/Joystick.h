#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

namespace Joystick {
    class Axis {
    public:
        Axis(const uint8_t pin, bool inverted);
        void init() const;

        int8_t getPosition() const;

    private:
        uint8_t pin;
        bool inverted;
    };

    class VirtualThrottle {
    public:
        VirtualThrottle(Axis throttle, unsigned long updateInterval /* in ms */);
        void init();

        uint8_t getPosition();

    private:
        Axis throttle;
        unsigned long updateInterval;
        unsigned long lastUpdate;
        uint8_t currentPosition;
    };
}

#endif //JOYSTICK_H
