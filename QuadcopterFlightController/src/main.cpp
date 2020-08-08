#include <Arduino.h>
#include <Wire.h>

#include "Hardware.h"
#include "IMU.h"
#include "ESC.h"
#include "Radio.h"
#include "FlightController.h"

void setup() {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("Booting...");
    const uint8_t ESC_PINS[DShot::ESC_COUNT] = {Hardware::MOTOR_FL_PIN, Hardware::MOTOR_FR_PIN, Hardware::MOTOR_RL_PIN, Hardware::MOTOR_RR_PIN};
    ESC::init(ESC_PINS);
    IMU::init();
    Radio::init();
    FlightController::init();
    Serial.println("Initialized");
}

void loop() {
    Radio::loop();
    IMU::loop();
    FlightController::loop();
    ESC::loop();
}