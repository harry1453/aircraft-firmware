#include <Arduino.h>
#include <Adafruit_SSD1306.h>

const uint8_t CELL_1_INPUT = A0;
const uint8_t CELL_2_INPUT = A1;
const uint8_t CELL_3_INPUT = A2;

Adafruit_SSD1306 display;

void setup() {
//    analogReference(DEFAULT);
    pinMode(CELL_1_INPUT, INPUT);
    pinMode(CELL_2_INPUT, INPUT);
    pinMode(CELL_3_INPUT, INPUT);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
}

void loop() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.write((String("Voltage: ") + String(analogRead(CELL_1_INPUT))).c_str());
    delay(1000);
}
