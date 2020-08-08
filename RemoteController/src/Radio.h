#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>

namespace Radio {
    struct AircraftStatus {
        uint16_t battery;
        bool goodSignal;
    };

    struct ControlsStatus {
        uint8_t throttle;
        int8_t pitch;
        int8_t yaw;
        int8_t roll;
    };

    void init();

    /**
     * Transmit controls to the aircraft
     * @param aircraftStatus Pointer to a struct that will be filled in with the status of the aircraft
     * @param controlsStatus Pointer to a struct that describes the controls to send to the aircraft
     * @return Whether the transmission was successful (successfully transmitted and acknowledged by the aircraft)
     */
    bool transmitControls(AircraftStatus* aircraftStatus, ControlsStatus* controlsStatus);
}

#endif //RADIO_H
