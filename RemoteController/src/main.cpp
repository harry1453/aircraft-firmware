#include <Arduino.h>
#include <Wire.h>

#include "Hardware.h"
#include "Radio.h"
#include "Joystick.h"
#include "Display.h"

Joystick::VirtualThrottle throttle(Joystick::Axis(Hardware::JOYSTICK_L_Y, false), 25);
Joystick::Axis pitch(Hardware::JOYSTICK_R_Y, true);
Joystick::Axis yaw(Hardware::JOYSTICK_R_X, false);
Joystick::Axis roll(Hardware::JOYSTICK_L_X, true);

//Display::Display topDisplay(0);
//Display::Display bottomDisplay(1);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Radio::init();
    throttle.init();
    pitch.init();
    yaw.init();
    roll.init();
//    topDisplay.init();
//    bottomDisplay.init();
}

Radio::ControlsStatus controlsStatus;
Radio::AircraftStatus aircraftStatus;

void loop() {
    controlsStatus.throttle = throttle.getPosition();
    controlsStatus.pitch = pitch.getPosition();
    controlsStatus.yaw = yaw.getPosition();
    controlsStatus.roll = roll.getPosition();
    if (Radio::transmitControls(&aircraftStatus, &controlsStatus)) {
        Serial.println(String(controlsStatus.throttle) + "\t" + String(controlsStatus.pitch) + "\t" +
        "");
//                       String(controlsStatus.yaw) + "\t" + String(controlsStatus.roll) + "\t" +
//                       String(aircraftStatus.battery) + "\t" + String(aircraftStatus.goodSignal));
//                       String(aircraftStatus.millis)+ "\t" + String(aircraftStatus.error)+ "\t" + String(aircraftStatus.output));
    } else {
        Serial.println("Error transmitting");
    }
    delay(10);
}
