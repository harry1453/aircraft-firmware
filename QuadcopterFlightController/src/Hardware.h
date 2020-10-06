#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <stdint.h>

namespace Hardware {
    // Flight parameters
    const double MAX_ANGLE = 35.0 / 180.0 * PI;

    // PID parameters - these are betaflight's defaults
//    const double PitchKp = 50;
//    const double PitchKi = 50;
//    const double PitchKd = 27;
//    const double PitchRange = 100;
//    const double YawKp = 65;
//    const double YawKi = 45;
//    const double YawKd = 0;
//    const double YawRange = 25;
//    const double RollKp = 46;
//    const double RollKi = 45;
//    const double RollKd = 25;
//    const double RollRange = 100;
    const double PitchKp = 105;
    const double PitchKi = 30;
    const double PitchKd = 16;
    const double PitchRange = 200;
    const double YawKp = 105;
    const double YawKi = 30;
    const double YawKd = 16;
    const double YawRange = 100;
    const double RollKp = 105;
    const double RollKi = 30;
    const double RollKd = 16;
    const double RollRange = 200;

    const uint8_t MOTOR_FL_PIN = 5;
    const uint8_t MOTOR_FR_PIN = 6;
    const uint8_t MOTOR_RL_PIN = 10;
    const uint8_t MOTOR_RR_PIN = 9;

    const uint8_t RADIO_CE_PIN = 7;
    const uint8_t RADIO_CS_PIN = 8;

    const uint8_t BATTERY_INPUT_PIN = A0;
}

#endif //HARDWARE_H
