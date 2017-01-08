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

void delay_init();

// Get current time in clock cycles
inline uint32_t get_time()
{
    return LPC_TIM3->TC;
}

// Wait until get_time() is numCycles past startTime. Handles timer wrap.
inline void delay_loop(uint32_t startTime, uint32_t numCycles)
{
    while (get_time() - startTime < numCycles)
        ;
}

inline void delay_us(uint32_t us)
{
    delay_loop(get_time(), uint32_t(uint64_t(SystemCoreClock) * us / 1'000'000));
}

inline void delay_ms(uint32_t ms)
{
    delay_loop(get_time(), uint32_t(uint64_t(SystemCoreClock) * ms / 1000));
}
