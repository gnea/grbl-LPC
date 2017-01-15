#pragma once

// Copyright 2017 Todd Fleming
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "LPC17xx.h"
#include <atomic>

static const uint32_t leds = (1 << 21) | (1 << 20) | (1 << 19) | (1 << 18);

inline void debug_init()
{
    LPC_GPIO1->FIODIR = leds;
    LPC_GPIO1->FIOPIN = 0;
}

inline void debug_enter()
{
    LPC_GPIO1->FIOPIN = (LPC_GPIO1->FIOPIN | (1 << 21)) ^ (1 << 20);
}

inline void debug_exit()
{
    LPC_GPIO1->FIOPIN &= ~(1 << 21);
}

inline void xdebug_enter() {}
inline void xdebug_exit() {}
