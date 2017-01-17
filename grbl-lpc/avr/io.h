#pragma once
#include <stdint.h>

struct DummyReg {
    operator uint8_t() {return 0;}
    uint8_t operator=(uint8_t x) {return x;}
    uint8_t operator&=(uint8_t x) {return *this = *this & x;}
    uint8_t operator|=(uint8_t x) {return *this = *this | x;}
};

extern DummyReg NotUsed;
extern DummyReg PCICR;
extern DummyReg PCIE1;

