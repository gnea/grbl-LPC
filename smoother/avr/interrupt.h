#pragma once

#include <LPC17xx.h>

#define ISR(f) void f()
inline void cli() {__disable_irq();}
inline void sei() {__enable_irq();}
