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
        PID pitchPid(&pitchInput, &pitchOutput, &pitchSetpoint, Hardware::PitchKp, Hardware::PitchKi, Hardware::PitchKd, MANUAL,
                     DIRECT);

        double yawSetpoint, yawInput, yawOutput;
        /**
         * YAW PID
         * Positive output = spin up front right and rear left motors more (these are the ACW props so turn CW)
         * Negative output = spin up front left and rear right motors more (these are the CW props so turn ACW)
         */
        PID yawPid(&yawInput, &yawOutput, &yawSetpoint, Hardware::YawKp, Hardware::YawKi, Hardware::YawKd, MANUAL, DIRECT);

        double rollSetpoint, rollInput, rollOutput;
        /**
         * ROLL PID
         * Positive output = spin up left motors more (tilt to the right)
         * Negative output = spin up right motors more (tilt to the left)
         */
        PID rollPid(&rollInput, &rollOutput, &rollSetpoint, Hardware::RollKp, Hardware::RollKi, Hardware::RollKd, MANUAL, DIRECT);
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

        yawSetpoint =
                Hardware::MAX_ANGLE * ((double) Radio::controls.yaw) / 128; // TODO MAX_ANGLE is inappropriate for yaw
        yawInput = IMU::attitude.yaw;
        yawPid.Compute();

        rollSetpoint = Hardware::MAX_ANGLE * ((double) Radio::controls.roll) / 128;
        rollInput = IMU::attitude.roll;
        rollPid.Compute();

        if (Radio::controls.throttle == 0) {
            ESC::setThrottle(ESC::FL, 0);
            ESC::setThrottle(ESC::FR, 0);
            ESC::setThrottle(ESC::RL, 0);
            ESC::setThrottle(ESC::RR, 0);
        } else {
            double fl = Radio::controls.throttle - pitchOutput - yawOutput + rollOutput;
            double fr = Radio::controls.throttle - pitchOutput + yawOutput - rollOutput;
            double rl = Radio::controls.throttle + pitchOutput + yawOutput + rollOutput;
            double rr = Radio::controls.throttle + pitchOutput - yawOutput - rollOutput;

            double maxMotor = fl;
            if (fr > maxMotor) maxMotor = fr;
            if (rl > maxMotor) maxMotor = rl;
            if (rr > maxMotor) maxMotor = rr;

            if (maxMotor > 2000) {
                double diff = maxMotor - 2000;
                fl -= diff;
                fr -= diff;
                rl -= diff;
                rr -= diff;
            }

            if (fl < 0) fl = 0;
            if (fr < 0) fr = 0;
            if (rl < 0) rl = 0;
            if (rr < 0) rr = 0;

            ESC::setThrottle(ESC::FL, fl);
            ESC::setThrottle(ESC::FR, fr);
            ESC::setThrottle(ESC::RL, rl);
            ESC::setThrottle(ESC::RR, rr);

            Serial.println("INPUT PYR:\t" + String(pitchInput) + "\t" + String(yawInput) + "\t" + String(rollInput) +
                           "\tSETPOINT PYR:\t" + String(pitchSetpoint) + "\t" + String(yawSetpoint) + "\t" +
                           String(rollSetpoint) + "\tOUTPUT PYR:\t" + String(pitchOutput) + "\t" + String(yawOutput) +
                           "\t" + String(rollOutput) + "\tMOTORS:\t" + String(fl, 0) + "\t" +
                           String(fr, 0) + "\t" +
                           String(rl, 0) + "\t" + String(rr, 0));
        }
    }
}
