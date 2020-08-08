#include "DShot.h"

#include <Arduino.h>

#define NOP asm("NOP");

#define NOP1 NOP
#define NOP2 NOP NOP
#define NOP3 NOP2 NOP
#define NOP4 NOP3 NOP
#define NOP5 NOP4 NOP
#define NOP6 NOP5 NOP
#define NOP7 NOP6 NOP
#define NOP8 NOP7 NOP
#define NOP9 NOP8 NOP
#define NOP10 NOP9 NOP
#define NOP11 NOP10 NOP
#define NOP12 NOP11 NOP
#define NOP13 NOP12 NOP
#define NOP14 NOP13 NOP
#define NOP15 NOP14 NOP
#define NOP16 NOP15 NOP
#define NOP17 NOP16 NOP
#define NOP18 NOP17 NOP
#define NOP19 NOP18 NOP
#define NOP20 NOP19 NOP
#define NOP21 NOP20 NOP
#define NOP22 NOP21 NOP
#define NOP23 NOP22 NOP
#define NOP24 NOP23 NOP
#define NOP25 NOP24 NOP
#define NOP26 NOP25 NOP
#define NOP27 NOP26 NOP
#define NOP28 NOP27 NOP
#define NOP29 NOP28 NOP
#define NOP30 NOP29 NOP
#define NOP31 NOP30 NOP
#define NOP32 NOP31 NOP
#define NOP33 NOP32 NOP
#define NOP34 NOP33 NOP
#define NOP35 NOP34 NOP
#define NOP36 NOP35 NOP
#define NOP37 NOP36 NOP
#define NOP38 NOP37 NOP
#define NOP39 NOP38 NOP
#define NOP40 NOP39 NOP

namespace DShot {
    namespace { // private
        void sendPacket(uint8_t i) {
            volatile uint8_t *reg = portRegisters[i];
            uint8_t reg_mask = pinBitMasks[i];
            uint8_t inv_mask = ~pinBitMasks[i];
            uint16_t value = throttles[i];
            uint8_t oldSREG = SREG;

            cli();

            for (uint8_t j = 16; j > 0; --j) {
                // For 3340ns total bit time we need a total of 34 NOPs (DShot300)
                if (value & 0x8000) {
                    *reg |= reg_mask; // send 1
                    NOP33
                    * reg &= inv_mask; // send 0
                    NOP1
                } else {
                    *reg |= reg_mask; // send 1
                    NOP12
                    * reg &= inv_mask; // send 0
                    NOP19
                }
                value <<= 1;
            }

            *reg &= inv_mask; // send 0

            SREG = oldSREG; // turn interrupts back on
        }
    }

    volatile uint8_t pins[ESC_COUNT];
    volatile uint8_t pinBitMasks[ESC_COUNT];
    volatile uint8_t* portRegisters[ESC_COUNT];
    volatile uint16_t throttles[ESC_COUNT];

    void init() {
//            cli(); // stop interrupts
//            TCCR1A = 0; // set entire TCCR1A register to 0
//            TCCR1B = 0; // same for TCCR1B
//            TCNT1  = 0; // initialize counter value to 0
//            // set compare match register for 500 Hz increments
//            OCR1A = 31999; // = 16000000 / (1 * 500) - 1 (must be <65536)
//            // turn on CTC mode
//            TCCR1B |= (1 << WGM12);
//            // Set CS12, CS11 and CS10 bits for 1 prescaler
//            TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
//            // enable timer compare interrupt
//            TIMSK1 |= (1 << OCIE1A);
//            sei(); // allow interrupts
    }

    void sendPackets() {
        for (uint8_t i = 0; i < 4; i++) {
            sendPacket(i); // TODO send in parallel?
        }
    }

//        ISR(TIMER1_COMPA_vect){
//            sendPackets();
//        }

    uint16_t createPacket(uint16_t throttle) {
        uint8_t csum = 0;
        throttle <<= 1;
        // Indicate as command if less than 48
        if (throttle <48)
            throttle |= 1;
        uint16_t csum_data = throttle;
        for (uint8_t i=0; i<3; i++){
            csum ^= csum_data;
            csum_data >>= 4;
        }
        csum &= 0xf;
        return (throttle<<4)|csum;
    }
}

