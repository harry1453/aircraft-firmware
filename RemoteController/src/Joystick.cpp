#include "Joystick.h"

#include <Arduino.h>

namespace Joystick {
    Axis::Axis(const uint8_t pin, bool inverted) {
        this->pin = pin;
        this->inverted = inverted;
    }

    void Axis::init() const {
        pinMode(this->pin, INPUT);
    }

    int8_t Axis::getPosition() const {
        return map(this->inverted ? 1023 - analogRead(this->pin) : analogRead(this->pin), 0, 1023, -128, 127);
    }

    VirtualThrottle::VirtualThrottle(Axis throttle, unsigned long updateInterval) : throttle(throttle) {
        this->updateInterval = updateInterval;
        this->currentPosition = 0;
        this->lastUpdate = 0;
    }

    void VirtualThrottle::init() {
        this->throttle.init();
        this->lastUpdate = millis();
    }

    uint8_t VirtualThrottle::getPosition() {
        if (millis() - lastUpdate >= this->updateInterval) {
            this->lastUpdate += this->updateInterval;
            int newPosition = this->currentPosition + (this->throttle.getPosition() / 16);
            if (newPosition <= 0) {
                this->currentPosition = 0;
            } else if (newPosition >= 255) {
                this->currentPosition = 255;
            } else {
                this->currentPosition = newPosition;
            }
        }
        return this->currentPosition;
    }
}