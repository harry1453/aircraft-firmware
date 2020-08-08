#include "Display.h"

#include <Wire.h>

namespace Display {
    namespace { // private
        const uint8_t I2C_MULTIPLEXER_ADDRESS = 0x70;

        void selectI2Cbus(const uint8_t busIndex) {
            if (busIndex > 7) return;

            Wire.beginTransmission(I2C_MULTIPLEXER_ADDRESS);
            Wire.write(1u << busIndex);
            Wire.endTransmission();
        }
    }

    Display::Display(const uint8_t busIndex) {
        this->busIndex = busIndex;
    }

    void Display::init() {
        selectI2Cbus(this->busIndex);
        this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C, true, false);
    }
}
