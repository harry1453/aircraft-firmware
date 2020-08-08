#include "IMU.h"

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

namespace IMU {
    namespace { // private
        Quaternion q;           // [w, x, y, z]         quaternion container
        VectorInt16 aa;         // [x, y, z]            accel sensor measurements
        VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
        VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
        VectorFloat gravity;    // [x, y, z]            gravity vector
        float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

        uint8_t fifoBuffer[64]; // FIFO storage buffer
        uint16_t packetSize1;
        uint16_t packetSize2;

        MPU6050 imu1(0x68); // TODO average between the two
        MPU6050 imu2(0x69); // TODO average between the two
    }

    Attitude attitude;

    void init() {
        Wire.begin();
        Wire.setClock(400000);
        imu1.initialize();
        if (!imu1.testConnection()) {
            Serial.println("Could not connect to IMU1");
            return;
        }
        imu2.initialize();
        if (!imu2.testConnection()) {
            Serial.println("Could not connect to IMU2");
            return;
        }

        auto devStatus = imu1.dmpInitialize();
        imu1.setXGyroOffset(220);
        imu1.setYGyroOffset(76);
        imu1.setZGyroOffset(-85);
        imu1.setZAccelOffset(1788);
        if (devStatus != 0) {
            Serial.println("Could not initialize IMU1 (Error code " + String(devStatus));
            return;
        }
        devStatus = imu2.dmpInitialize();
        imu2.setXGyroOffset(220);
        imu2.setYGyroOffset(76);
        imu2.setZGyroOffset(-85);
        imu2.setZAccelOffset(1788);
        if (devStatus != 0) {
            Serial.println("Could not initialize IMU2 (Error code " + String(devStatus));
            return;
        }

        imu1.CalibrateAccel(6);
        imu1.CalibrateGyro(6);
        imu1.PrintActiveOffsets();

        imu2.CalibrateAccel(6);
        imu2.CalibrateGyro(6);
        imu2.PrintActiveOffsets();

        imu1.setDMPEnabled(true);
        imu2.setDMPEnabled(true);

        packetSize1 = imu1.dmpGetFIFOPacketSize();
        packetSize2 = imu2.dmpGetFIFOPacketSize();
    }

    void loop() {
        auto fifoCount = imu1.getFIFOCount();
        auto status = imu1.getIntStatus();
        if (fifoCount < packetSize1) return;
        if ((status & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
            imu1.resetFIFO();
            return;
        }
        if (status & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
            while (fifoCount >= packetSize1) {
                imu1.getFIFOBytes(fifoBuffer, packetSize1);
                fifoCount -= packetSize1;
            }

            imu1.dmpGetQuaternion(&q, fifoBuffer);
            imu1.dmpGetGravity(&gravity, &q);
            imu1.dmpGetYawPitchRoll(ypr, &q, &gravity);
            if (/*TODO pitchRollInverted*/ true) {
                attitude.pitch = ypr[2] * 180/M_PI /* todo configurable */ * -1;
                attitude.yaw = ypr[0] * 180/M_PI;
                attitude.roll = ypr[1] * 180/M_PI /* todo configurable */ * -1;
            } else {
                attitude.pitch = ypr[1] * 180/M_PI;
                attitude.yaw = ypr[0] * 180/M_PI;
                attitude.roll = ypr[2] * 180/M_PI;
            }
        }
    }
}
