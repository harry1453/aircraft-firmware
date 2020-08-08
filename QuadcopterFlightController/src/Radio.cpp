#include "Radio.h"

#include <RF24.h>
#include "Hardware.h"

const uint8_t address[6] = "airHP";

namespace Radio {
    namespace { // private
        struct AircraftStatus {
            uint16_t battery;
            bool goodSignal;
        };

        RF24 radio(Hardware::RADIO_CE_PIN, Hardware::RADIO_CS_PIN);
        AircraftStatus aircraftStatus;

        void tryReceiveControls() {
            radio.writeAckPayload(1, &aircraftStatus, sizeof(AircraftStatus));
            if (radio.available()) {
                radio.read(&controls, sizeof(ControlsStatus));
//                controls.throttle = ((int) controls.throttle) * 2000 / 255;
            }
        }
    }

    ControlsStatus controls;

    void init() {
        pinMode(Hardware::BATTERY_INPUT_PIN, INPUT);
        radio.begin();
        radio.setAutoAck(true);
        radio.enableAckPayload();
        radio.enableDynamicPayloads();
        radio.setPALevel(RF24_PA_LOW);
        radio.setDataRate(RF24_250KBPS);
        radio.openReadingPipe(1, address);
        radio.startListening();
    }

    void loop() {
        aircraftStatus.goodSignal = radio.testRPD();
        aircraftStatus.battery = analogRead(Hardware::BATTERY_INPUT_PIN);
        tryReceiveControls();
    }
}
