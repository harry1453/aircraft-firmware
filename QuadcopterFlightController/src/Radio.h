#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>

namespace Radio {
    struct ControlsStatus {
        uint8_t throttle;
        int8_t pitch;
        int8_t yaw;
        int8_t roll;
    };

    struct AircraftStatus {
        uint16_t battery;
        bool goodSignal;
    };

    extern AircraftStatus aircraftStatus;
    extern ControlsStatus controls;

    void init();
    void loop();
}

#endif //RADIO_H
