#ifndef DSHOT_H
#define DSHOT_H

#include <stddef.h>
#include <stdint.h>

namespace DShot {
    const size_t ESC_COUNT = 4;
    extern volatile uint8_t pins[ESC_COUNT];
    extern volatile uint8_t pinBitMasks[ESC_COUNT];
    extern volatile uint8_t* portRegisters[ESC_COUNT];
    extern volatile uint16_t throttles[ESC_COUNT];

    void init();
    void sendPackets();
    uint16_t createPacket(uint16_t throttle);

    const uint16_t DSHOT_COMMAND_MOTOR_STOP = 0;
    const uint16_t DSHOT_COMMAND_BEEP1 = 1;
    const uint16_t DSHOT_COMMAND_BEEP2 = 2;
    const uint16_t DSHOT_COMMAND_BEEP3 = 3;
    const uint16_t DSHOT_COMMAND_BEEP4 = 4;
    const uint16_t DSHOT_COMMAND_BEEP5 = 5;
    const uint16_t DSHOT_COMMAND_ESC_INFO = 6;
    const uint16_t DSHOT_COMMAND_SPIN_DIRECTION_1 = 7;
    const uint16_t DSHOT_COMMAND_SPIN_DIRECTION_2 = 8;
    const uint16_t DSHOT_COMMAND_3D_MODE_OFF = 9;
    const uint16_t DSHOT_COMMAND_3D_MODE_ON = 10;
    const uint16_t DSHOT_COMMAND_SETTINGS_REQUEST = 11;
    const uint16_t DSHOT_COMMAND_SAVE_SETTINGS = 12;
    const uint16_t DSHOT_COMMAND_SPIN_DIRECTION_NORMAL = 20;
    const uint16_t DSHOT_COMMAND_SPIN_DIRECTION_REVERSED = 21;
    const uint16_t DSHOT_COMMAND_LED0_ON = 22;
    const uint16_t DSHOT_COMMAND_LED1_ON = 23;
    const uint16_t DSHOT_COMMAND_LED2_ON = 24;
    const uint16_t DSHOT_COMMAND_LED3_ON = 25;
    const uint16_t DSHOT_COMMAND_LED0_OFF = 26;
    const uint16_t DSHOT_COMMAND_LED1_OFF = 27;
    const uint16_t DSHOT_COMMAND_LED2_OFF = 28;
    const uint16_t DSHOT_COMMAND_LED3_OFF = 29;
    const uint16_t DSHOT_COMMAND_TOGGLE_AUDIO_STREAM = 30;
    const uint16_t DSHOT_COMMAND_TOGGLE_SILENT_MODE = 31;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_TELEMETRY_DISABLE = 32;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_TELEMETRY_ENABLE = 33;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_CONTINUOUS_ERPM_TELEMETRY = 34;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_CONTINUOUS_ERPM_PERIOD_TELEMETRY = 35;

    // Executed at any time
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_TEMPERATURE_TELEMETRY = 42;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_VOLTAGE_TELEMETRY = 43;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_CURRENT_TELEMETRY = 44;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_CONSUMPTION_TELEMETRY = 45;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_ERPM_TELEMETRY = 46;
    const uint16_t DSHOT_COMMAND_SIGNAL_LINE_ERPM_PERIOD_TELEMETRY = 47;
}

#endif //DSHOT_H
