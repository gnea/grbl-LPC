// Current control
//
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

#include "current_control.h"
#include "grbl.h"
#include "Driver_I2C.h"

#undef min
#undef max
#include <algorithm>

#ifdef CURRENT_I2C
extern ARM_DRIVER_I2C CURRENT_I2C;
static const uint8_t wiperRegs[] = CURRENT_WIPERS;
#endif

void current_init()
{
#ifdef CURRENT_I2C
    CURRENT_I2C.Initialize(nullptr);
    CURRENT_I2C.PowerControl(ARM_POWER_FULL);
    CURRENT_I2C.Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
    CURRENT_I2C.Control(ARM_I2C_BUS_CLEAR, 0);

#ifdef CURRENT_MCP44XX_ADDR
    uint8_t init1[] = {0x40, 0xff};
    uint8_t init2[] = {0xA0, 0xff};
    CURRENT_I2C.MasterTransmit(CURRENT_MCP44XX_ADDR, init1, 2, false);
    while (CURRENT_I2C.GetStatus().busy)
        ;
    CURRENT_I2C.MasterTransmit(CURRENT_MCP44XX_ADDR, init2, 2, false);
    while (CURRENT_I2C.GetStatus().busy)
        ;
#endif

    set_current(0, settings.current[0]);
    set_current(1, settings.current[1]);
    set_current(2, settings.current[2]);
    set_current(3, DEFAULT_A_CURRENT);
#endif
}

void set_current(uint8_t motor, float amps)
{
#if defined(CURRENT_I2C) && defined(CURRENT_MCP44XX_ADDR)
    uint8_t command[] = {
        uint8_t(wiperRegs[motor] << 4),
        uint8_t(std::min(255.0, std::max(0.0, std::round(amps * CURRENT_FACTOR)))),
    };
    CURRENT_I2C.MasterTransmit(CURRENT_MCP44XX_ADDR, command, sizeof(command), false);
    while (CURRENT_I2C.GetStatus().busy)
        ;
#endif
}
