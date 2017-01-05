#pragma once

#include <mbed.h>

#define ISR(f) void f()
inline void cli() {__disable_irq();}
inline void sei() {__enable_irq();}
