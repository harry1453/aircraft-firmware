#include "ESC.h"

#include <Arduino.h>

namespace ESC {
    void init(const uint8_t pins[DShot::ESC_COUNT]) {
        for (uint8_t i = 0; i < DShot::ESC_COUNT; i++) {
            digitalWrite(pins[i], LOW);
            pinMode(pins[i], OUTPUT);
            DShot::pinBitMasks[i] = digitalPinToBitMask(pins[i]);
            DShot::portRegisters[i] = portOutputRegister(digitalPinToPort(pins[i]));

            DShot::throttles[i] = DShot::createPacket(0);
//        this->enable3dMode(true);
//        this->setDirection(true);
        }
    }

    void setThrottle(uint8_t motor, uint16_t throttle) {
        if (motor >= DShot::ESC_COUNT) return;
        if (throttle == 0) {
            DShot::throttles[motor] = DShot::createPacket(DShot::DSHOT_COMMAND_MOTOR_STOP);
        } else if (throttle <= 2000) {
            DShot::throttles[motor] = DShot::createPacket(throttle+47);
        }
    }

    void loop() {
        DShot::sendPackets();
    }
}
