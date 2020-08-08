#ifndef IMU_H
#define IMU_H

namespace IMU {
    struct Attitude {
        /**
         * PITCH
         * Positive angle = tilting forwards
         * Negative angle = tilting backwards
         *
         *                      angle of quad
         *                     /
         *                    /
         *                   /
         *                  / pitch angle
         * [back QUAD front] --------------- horizontal axis
         */
        double pitch;


        /**
         * YAW
         * Positive angle = turned CW of zero position
         * Negative angle = turned ACW of zero position
         *
         *            quad pos
         *                    \
         *                     \
         *           yaw angle  \
         *                    F  \   F
         * zero pos----------   QUAD
         *                    R      R
         */
        double yaw;

        /**
         * ROLL
         * Positive angle = tilting to the left
         * Negative angle = tilting to the right
         *
         *                      angle of quad
         *                     /
         *                    /
         *                   /
         *                  / pitch angle
         * [left QUAD right] --------------- horizontal axis
         */
        double roll;
    };

    extern Attitude attitude;

    void init();
    void loop();
}

#endif //IMU_H
