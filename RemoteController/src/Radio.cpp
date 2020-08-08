#include "Radio.h"

#include <RF24.h>
#include "Hardware.h"

const uint8_t address[6] = "airHP";

namespace Radio {
    namespace { // private
        RF24 radio(Hardware::RADIO_CE_PIN, Hardware::RADIO_CS_PIN);
    }

    void init() {
        radio.begin();
        radio.setAutoAck(true);
        radio.enableAckPayload();
        radio.enableDynamicPayloads();
        radio.setPALevel(RF24_PA_LOW);
        radio.setDataRate(RF24_250KBPS);
        radio.openWritingPipe(address);
        radio.stopListening();
    }

    bool transmitControls(AircraftStatus* aircraftStatus, ControlsStatus* controlsStatus) {
        if (radio.write(controlsStatus, sizeof(ControlsStatus))) {
            if (radio.isAckPayloadAvailable()) {
                radio.read(aircraftStatus, sizeof(AircraftStatus));
                return true;
            }
        }
        return false;
    }
}
