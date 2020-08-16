#include "Radio.h"

#include <RF24.h>
#include "Hardware.h"

const uint8_t address[6] = "airHP";

namespace Radio {
    namespace { // private
        RF24 radio(Hardware::RADIO_CE_PIN, Hardware::RADIO_CS_PIN);

        unsigned long lastReceived;

        void tryReceiveControls() {
            radio.writeAckPayload(1, &aircraftStatus, sizeof(AircraftStatus));
            if (radio.available()) {
                radio.read(&controls, sizeof(ControlsStatus));
//                controls.throttle *= 3;
                lastReceived = millis();
            }
            if (millis() - lastReceived >= 1000) {
                controls.throttle = 0;
            }
        }
    }

    AircraftStatus aircraftStatus;
    ControlsStatus controls;

    void init() {
        pinMode(Hardware::BATTERY_INPUT_PIN, INPUT);
        radio.begin();
        radio.setAutoAck(true);
        radio.enableAckPayload();
        radio.enableDynamicPayloads();
        radio.setPALevel(RF24_PA_MAX);
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
