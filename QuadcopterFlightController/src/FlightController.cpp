#include "FlightController.h"

#include <PID_v1.h>
#include "Hardware.h"
#include "ESC.h"
#include "IMU.h"
#include "Radio.h"
#include "Mixer.h"

namespace FlightController {
    namespace { // private
        double pitchSetpoint, pitchInput, pitchOutput;
        /**
         * PITCH PID
         * Positive output = spin up rear motors more (tilt forwards)
         * Negative output = spin up front motors more (tilt backwards)
         */
        PID pitchPid(&pitchInput, &pitchOutput, &pitchSetpoint, Hardware::PitchKp, Hardware::PitchKi, Hardware::PitchKd, P_ON_E, DIRECT);

        double yawSetpoint, yawInput, yawOutput;
        /**
         * YAW PID
         * Positive output = spin up front right and rear left motors more (these are the ACW props so turn CW)
         * Negative output = spin up front left and rear right motors more (these are the CW props so turn ACW)
         */
        PID yawPid(&yawInput, &yawOutput, &yawSetpoint, Hardware::YawKp, Hardware::YawKi, Hardware::YawKd, P_ON_E, DIRECT);

        double rollSetpoint, rollInput, rollOutput;
        /**
         * ROLL PID
         * Positive output = spin up left motors more (tilt to the right)
         * Negative output = spin up right motors more (tilt to the left)
         */
        PID rollPid(&rollInput, &rollOutput, &rollSetpoint, Hardware::RollKp, Hardware::RollKi, Hardware::RollKd, P_ON_E, DIRECT);

        double doubleMap(double x, double in_min, double in_max, double out_min, double out_max) {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }
    }

    void init() {
        pitchPid.SetSampleTime(10);
        yawPid.SetSampleTime(10);
        rollPid.SetSampleTime(10);

        pitchPid.SetOutputLimits(-Hardware::PitchRange, Hardware::PitchRange);
        yawPid.SetOutputLimits(-Hardware::YawRange, Hardware::YawRange);
        rollPid.SetOutputLimits(-Hardware::RollRange, Hardware::RollRange);

        pitchPid.SetMode(AUTOMATIC);
        yawPid.SetMode(AUTOMATIC);
        rollPid.SetMode(AUTOMATIC);
    }

    void loop() {
        pitchSetpoint = Hardware::MAX_ANGLE * ((double) Radio::controls.pitch) / 128;
        pitchInput = IMU::attitude.pitch;
        pitchPid.Compute();

        yawSetpoint = Hardware::MAX_ANGLE * ((double) Radio::controls.yaw) / 128; // TODO MAX_ANGLE is inappropriate for yaw
        yawInput = IMU::attitude.yaw;
//        yawPid.Compute();

        rollSetpoint = Hardware::MAX_ANGLE * ((double) Radio::controls.roll) / 128;
        rollInput = IMU::attitude.roll;
        rollPid.Compute();

        if (Radio::controls.throttle == 0) {
            ESC::setThrottle(ESC::FL, 0);
            ESC::setThrottle(ESC::FR, 0);
            ESC::setThrottle(ESC::RL, 0);
            ESC::setThrottle(ESC::RR, 0);

            Serial.println("INPUT PYR:\t" + String(pitchInput) + "\t" + String(yawInput) + "\t" + String(rollInput));
        } else {
            yawOutput = Radio::controls.yaw;

            double throttle = ((double) Radio::controls.throttle) * ESC::MAX_THROTTLE / 255;
            double fl = throttle - pitchOutput - yawOutput + rollOutput;
            double fr = throttle - pitchOutput + yawOutput - rollOutput;
            double rl = throttle + pitchOutput + yawOutput + rollOutput;
            double rr = throttle + pitchOutput - yawOutput - rollOutput;

            double maxMotor = fl;
            if (fr > maxMotor) maxMotor = fr;
            if (rl > maxMotor) maxMotor = rl;
            if (rr > maxMotor) maxMotor = rr;

            double minMotor = fl;
            if (fr < minMotor) minMotor = fr;
            if (rl < minMotor) minMotor = rl;
            if (rr < minMotor) minMotor = rr;

            if (maxMotor - minMotor > ESC::MAX_THROTTLE) {
                fl = doubleMap(fl, minMotor, maxMotor, 0, ESC::MAX_THROTTLE);
                fr = doubleMap(fr, minMotor, maxMotor, 0, ESC::MAX_THROTTLE);
                rl = doubleMap(rl, minMotor, maxMotor, 0, ESC::MAX_THROTTLE);
                rr = doubleMap(rr, minMotor, maxMotor, 0, ESC::MAX_THROTTLE);
            } else if (maxMotor > ESC::MAX_THROTTLE) {
                double diff = maxMotor - ESC::MAX_THROTTLE;
                fl -= diff;
                fr -= diff;
                rl -= diff;
                rr -= diff;
            } else if (minMotor < 0) {
                fl -= minMotor;
                fr -= minMotor;
                rl -= minMotor;
                rr -= minMotor;
            }

            ESC::setThrottle(ESC::FL, fl);
            ESC::setThrottle(ESC::FR, fr);
            ESC::setThrottle(ESC::RL, rl);
            ESC::setThrottle(ESC::RR, rr);

//            Serial.print("\tSETPOINT PYR:\t" + String(pitchSetpoint) + "\t" + String(yawSetpoint) + "\t" +
//                           String(rollSetpoint) + "\tOUTPUT PYR:\t" + String(pitchOutput) + "\t" + String(yawOutput) +
//                           "\t" + String(rollOutput) + "\tMOTORS:\t" + String(fl, 0) + "\t" +
//                           String(fr, 0) + "\t" +
//                           String(rl, 0) + "\t" + String(rr, 0));
        }
//        Serial.println();
    }
}
