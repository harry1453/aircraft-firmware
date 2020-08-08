#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <Adafruit_SSD1306.h>

namespace Display {
    class Display {
    public:
        Display(const uint8_t busIndex);
        void init();

    private:
        uint8_t busIndex;
        Adafruit_SSD1306 display;
    };
}

#endif //DISPLAY_H
