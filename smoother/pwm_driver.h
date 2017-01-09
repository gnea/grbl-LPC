// PWM Driver - LPC176x
//
// Copyright 2017 Brett Fleming
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

#include <stdint.h>

typedef struct _PWM_Channel_Config {
    volatile uint32_t* MRn; //LPC_PWM1->MR1 through MR6
    uint32_t PCR_Enable_Mask;
    uint32_t LER_Enable_Mask;
} PWM_Channel_Config;

extern const PWM_Channel_Config PWM1_CH1;

void pwm_init(PWM_Channel_Config* channel, uint32_t period, uint32_t width);
void pwm_set_period(uint32_t period);
void pwm_set_width(PWM_Channel_Config* channel, uint32_t width);
void pwm_enable(PWM_Channel_Config* channel);
void pwm_disable(PWM_Channel_Config* channel);

