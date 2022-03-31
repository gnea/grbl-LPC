/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The cpu_map.h files serve as a central pin mapping selection file for different
   processor types or alternative pin layouts. This version of Grbl officially supports
   only the Arduino Mega328p. */


#ifndef cpu_map_h
#define cpu_map_h


#ifdef CPU_MAP_ATMEGA328P // (Arduino Uno) Officially supported by Grbl.

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEP_DDR        DDRD
  #define STEP_PORT       PORTD
  #define X_STEP_BIT      2  // Uno Digital Pin 2
  #define Y_STEP_BIT      3  // Uno Digital Pin 3
  #define Z_STEP_BIT      4  // Uno Digital Pin 4
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     DDRD
  #define DIRECTION_PORT    PORTD
  #define X_DIRECTION_BIT   5  // Uno Digital Pin 5
  #define Y_DIRECTION_BIT   6  // Uno Digital Pin 6
  #define Z_DIRECTION_BIT   7  // Uno Digital Pin 7
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    DDRB
  #define STEPPERS_DISABLE_PORT   PORTB
  #define STEPPERS_DISABLE_BIT    0  // Uno Digital Pin 8
  #define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR        DDRB
  #define LIMIT_PIN        PINB
  #define LIMIT_PORT       PORTB
  #define X_LIMIT_BIT      1  // Uno Digital Pin 9
  #define Y_LIMIT_BIT      2  // Uno Digital Pin 10
  #ifdef VARIABLE_SPINDLE // Z Limit pin and spindle enabled swapped to access hardware PWM on Pin 11.
    #define Z_LIMIT_BIT    4 // Uno Digital Pin 12
  #else
    #define Z_LIMIT_BIT    3  // Uno Digital Pin 11
  #endif
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits
  #define LIMIT_INT        PCIE0  // Pin change interrupt enable pin
  #define LIMIT_INT_vect   PCINT0_vect
  #define LIMIT_PCMSK      PCMSK0 // Pin change interrupt register

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR    DDRB
  #define SPINDLE_ENABLE_PORT   PORTB
  // Z Limit pin and spindle PWM/enable pin swapped to access hardware PWM on Pin 11.
  #ifdef VARIABLE_SPINDLE
    #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
      // If enabled, spindle direction pin now used as spindle enable, while PWM remains on D11.
      #define SPINDLE_ENABLE_BIT    5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
    #else
      #define SPINDLE_ENABLE_BIT    3  // Uno Digital Pin 11
    #endif
  #else
    #define SPINDLE_ENABLE_BIT    4  // Uno Digital Pin 12
  #endif
  #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
    #define SPINDLE_DIRECTION_DDR   DDRB
    #define SPINDLE_DIRECTION_PORT  PORTB
    #define SPINDLE_DIRECTION_BIT   5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
  #endif

  // Define flood and mist coolant enable output pins.
  #define COOLANT_FLOOD_DDR   DDRC
  #define COOLANT_FLOOD_PORT  PORTC
  #define COOLANT_FLOOD_BIT   3  // Uno Analog Pin 3
  #define COOLANT_MIST_DDR    DDRC
  #define COOLANT_MIST_PORT   PORTC
  #define COOLANT_MIST_BIT    4  // Uno Analog Pin 4

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       DDRC
  #define CONTROL_PIN       PINC
  #define CONTROL_PORT      PORTC
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     PCMSK1 // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       DDRC
  #define PROBE_PIN       PINC
  #define PROBE_PORT      PORTC
  #define PROBE_BIT       5  // Uno Analog Pin 5
  #define PROBE_MASK      (1<<PROBE_BIT)

  // Variable spindle configuration below. Do not change unless you know what you are doing.
  // NOTE: Only used when variable spindle is enabled.
  #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
  #ifndef SPINDLE_PWM_MIN_VALUE
    #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
  #endif
  #define SPINDLE_PWM_OFF_VALUE     0
  #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
  #define SPINDLE_TCCRA_REGISTER    TCCR2A
  #define SPINDLE_TCCRB_REGISTER    TCCR2B
  #define SPINDLE_OCR_REGISTER      OCR2A
  #define SPINDLE_COMB_BIT          COM2A1

  // Prescaled, 8-bit Fast PWM mode.
  #define SPINDLE_TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.
  // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
  // #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
  // #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
  #define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)

  // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
  #define SPINDLE_PWM_DDR     DDRB
  #define SPINDLE_PWM_PORT    PORTB
  #define SPINDLE_PWM_BIT     3    // Uno Digital Pin 11

#endif // end of CPU_MAP_ATMEGA328P


#ifdef CPU_MAP_SMOOTHIEBOARD // (Smoothieboards)

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEP_DDR        LPC_GPIO2->FIODIR
  #define STEP_PORT       LPC_GPIO2->FIOPIN
  #define X_STEP_BIT      0
  #define Y_STEP_BIT      1
  #define Z_STEP_BIT      2
  #define A_STEP_BIT      3
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)|(1<<A_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     LPC_GPIO0->FIODIR
  #define DIRECTION_PORT    LPC_GPIO0->FIOPIN
  #define X_DIRECTION_BIT   5
  #define Y_DIRECTION_BIT   11
  #define Z_DIRECTION_BIT   20
  #define A_DIRECTION_BIT   22
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)|(1<<A_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    LPC_GPIO0->FIODIR
  #define STEPPERS_DISABLE_PORT   LPC_GPIO0->FIOPIN
  #define X_DISABLE_BIT           4
  #define Y_DISABLE_BIT           10
  #define Z_DISABLE_BIT           19
  #define A_DISABLE_BIT           21
  #define STEPPERS_DISABLE_MASK   ((1<<X_DISABLE_BIT)|(1<<Y_DISABLE_BIT)|(1<<Z_DISABLE_BIT)|(1<<A_DISABLE_BIT))

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR         LPC_GPIO1->FIODIR
  #define LIMIT_PIN         LPC_GPIO1->FIOPIN
  #define LIMIT_PORT        LPC_GPIO1->FIOPIN
  #define X_LIMIT_BIT       24  // X-MIN=24, X-MAX=25
  #define Y_LIMIT_BIT       26  // Y-MIN=26, Y-MAX=27
  #define Z_LIMIT_BIT	    28  // Z-MIN=28, Z-MAX=29
  #define A_LIMIT_BIT       29  // reuse Z-MAX (P1.29)
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<A_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR        LPC_GPIO1->FIODIR
  #define SPINDLE_ENABLE_PORT       LPC_GPIO1->FIOPIN
  #define SPINDLE_ENABLE_BIT        30  // P1.30
  #define SPINDLE_DIRECTION_DDR     LPC_GPIO1->FIODIR
  #define SPINDLE_DIRECTION_PORT    LPC_GPIO1->FIOPIN
  #define SPINDLE_DIRECTION_BIT     31  // P1.31

  // Define flood and mist coolant enable output pins.
  #define COOLANT_FLOOD_DDR   LPC_GPIO2->FIODIR
  #define COOLANT_FLOOD_PORT  LPC_GPIO2->FIOPIN
  #ifndef SPINDLE_PWM_PIN_2_4
    #define COOLANT_FLOOD_BIT   4  // SMALL MOSFET Q8 (P2.4)
  #else
    #define COOLANT_FLOOD_BIT   5  // SMALL MOSFET Q8 (P2.5)
  #endif
  #define COOLANT_MIST_DDR    LPC_GPIO2->FIODIR
  #define COOLANT_MIST_PORT   LPC_GPIO2->FIOPIN
  #define COOLANT_MIST_BIT    6  // SMALL MOSFET Q9 (P2.6)
  #define ENABLE_M7           // enables COOLANT MIST

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       LPC_GPIO1->FIODIR
  #define CONTROL_PIN       LPC_GPIO1->FIOPIN
  #define CONTROL_PORT      LPC_GPIO1->FIOPIN
  #define CONTROL_RESET_BIT         NotUsed  // Not needed as there is a special RESET pin on the Smoothiebaord
  #define CONTROL_FEED_HOLD_BIT     22  // P1.22
  #define CONTROL_CYCLE_START_BIT   23  // P1.23
  #define CONTROL_SAFETY_DOOR_BIT   22  // P1.22 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     NotUsed // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       LPC_GPIO2->FIODIR
  #define PROBE_PIN       LPC_GPIO2->FIOPIN
  #define PROBE_PORT      LPC_GPIO2->FIOPIN
  #define PROBE_BIT       11  // P2.11
  #define PROBE_MASK      (1<<PROBE_BIT)

  // The LPC17xx has 6 PWM channels. Each channel has 2 pins. It can drive both pins simultaneously to the same value.
  //
  // PWM Channel      PWM1_CH1  PWM1_CH2  PWM1_CH3  PWM1_CH4  PWM1_CH5  PWM1_CH6
  // Primary pin      P1.18     P1.20     P1.21     P1.23     P1.24     P1.26
  // Secondary pin    P2.0      P2.1      P2.2      P2.3      P2.4      P2.5
  #ifdef SPINDLE_PWM_PIN_2_4
    #define SPINDLE_PWM_CHANNEL         PWM1_CH5    // MOSFET3 (P2.4)
  #else
    #define SPINDLE_PWM_CHANNEL         PWM1_CH6    // BED MOSFET (P2.5)
  #endif
  #define SPINDLE_PWM_USE_PRIMARY_PIN   false
  #define SPINDLE_PWM_USE_SECONDARY_PIN true

  // Stepper current control
  #define CURRENT_I2C Driver_I2C1       // I2C driver for current control. Comment out to disable (for C3d boards!)
  #define CURRENT_MCP44XX_ADDR 0b0101100  // Address of MCP44XX
  #define CURRENT_WIPERS {0, 1, 6, 7};    // Wiper registers (X, Y, Z, A)
  #define CURRENT_FACTOR 113.33           // Convert amps to digipot value

  // Variable spindle configuration below. Do not change unless you know what you are doing.
  // NOTE: Only used when variable spindle is enabled.
  #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
  #ifndef SPINDLE_PWM_MIN_VALUE
    #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
  #endif
  //#define SPINDLE_PWM_OFF_VALUE     0 // Defined in config.h
  #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
  #define SPINDLE_TCCRA_REGISTER    TCCR2A
  #define SPINDLE_TCCRB_REGISTER    TCCR2B
  #define SPINDLE_OCR_REGISTER      OCR2A
  #define SPINDLE_COMB_BIT          COM2A1
#endif // end of CPU_MAP_SMOOTHIEBOARD


#ifdef CPU_MAP_C3D_REMIX // (Cohesion3D Remix Boards)

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEP_DDR        LPC_GPIO2->FIODIR
  #define STEP_PORT       LPC_GPIO2->FIOPIN
  #define X_STEP_BIT      0
  #define Y_STEP_BIT      1
  #define Z_STEP_BIT      2
  #define A_STEP_BIT      3
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)|(1<<A_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     LPC_GPIO0->FIODIR
  #define DIRECTION_PORT    LPC_GPIO0->FIOPIN
  #define X_DIRECTION_BIT   5
  #define Y_DIRECTION_BIT   11
  #define Z_DIRECTION_BIT   20
  #define A_DIRECTION_BIT   22
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)|(1<<A_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    LPC_GPIO0->FIODIR
  #define STEPPERS_DISABLE_PORT   LPC_GPIO0->FIOPIN
  #define X_DISABLE_BIT           4
  #define Y_DISABLE_BIT           10
  #define Z_DISABLE_BIT           19
  #define A_DISABLE_BIT           21
  #define STEPPERS_DISABLE_MASK   ((1<<X_DISABLE_BIT)|(1<<Y_DISABLE_BIT)|(1<<Z_DISABLE_BIT)|(1<<A_DISABLE_BIT))

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR         LPC_GPIO1->FIODIR
  #define LIMIT_PIN         LPC_GPIO1->FIOPIN
  #define LIMIT_PORT        LPC_GPIO1->FIOPIN
  #define X_LIMIT_BIT       24  // X-MIN=24, X-MAX=25
  #define Y_LIMIT_BIT       26  // Y-MIN=26, Y-MAX=27
  #define Z_LIMIT_BIT       28  // Z-MIN=28, Z-MAX=29
  #define A_LIMIT_BIT       29  // reuse p1.29 from Z-MAX
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<A_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR        LPC_GPIO1->FIODIR
  #define SPINDLE_ENABLE_PORT       LPC_GPIO1->FIOPIN
  #define SPINDLE_ENABLE_BIT        30  // P1.30
  #define SPINDLE_DIRECTION_DDR     LPC_GPIO1->FIODIR
  #define SPINDLE_DIRECTION_PORT    LPC_GPIO1->FIOPIN
  #define SPINDLE_DIRECTION_BIT     31  // P1.31

  // Define flood and mist coolant enable output pins.
  #define COOLANT_FLOOD_DDR   LPC_GPIO2->FIODIR
  #define COOLANT_FLOOD_PORT  LPC_GPIO2->FIOPIN
  #define COOLANT_FLOOD_BIT   6  // MOSFET 2.6
  #define COOLANT_MIST_DDR    LPC_GPIO2->FIODIR
  #define COOLANT_MIST_PORT   LPC_GPIO2->FIOPIN
  #define COOLANT_MIST_BIT    7  // MOSFET 2.7
  #define ENABLE_M7           // enables COOLANT MIST

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       NotUsed
  #define CONTROL_PIN       NotUsed
  #define CONTROL_PORT      NotUsed
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     NotUsed // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       LPC_GPIO1->FIODIR
  #define PROBE_PIN       LPC_GPIO1->FIOPIN
  #define PROBE_PORT      LPC_GPIO1->FIOPIN
  #define PROBE_BIT       27  // reuse P1.27 from Y-MAX
  #define PROBE_MASK      (1<<PROBE_BIT)

  // The LPC17xx has 6 PWM channels. Each channel has 2 pins. It can drive both pins simultaneously to the same value.
  //
  // PWM Channel      PWM1_CH1  PWM1_CH2  PWM1_CH3  PWM1_CH4  PWM1_CH5  PWM1_CH6
  // Primary pin      P1.18     P1.20     P1.21     P1.23     P1.24     P1.26
  // Secondary pin    P2.0      P2.1      P2.2      P2.3      P2.4      P2.5
  #ifdef SPINDLE_PWM_PIN_2_4
    #define SPINDLE_PWM_CHANNEL         PWM1_CH5    // MOSFET3 (P2.4)
  #else
    #define SPINDLE_PWM_CHANNEL         PWM1_CH6    // BED MOSFET (P2.5)
  #endif
  #define SPINDLE_PWM_USE_PRIMARY_PIN   false
  #define SPINDLE_PWM_USE_SECONDARY_PIN true

  // Variable spindle configuration below. Do not change unless you know what you are doing.
  // NOTE: Only used when variable spindle is enabled.
  #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
  #ifndef SPINDLE_PWM_MIN_VALUE
    #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
  #endif
  //#define SPINDLE_PWM_OFF_VALUE     0 // Defined in config.h
  #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
  #define SPINDLE_TCCRA_REGISTER    TCCR2A
  #define SPINDLE_TCCRB_REGISTER    TCCR2B
  #define SPINDLE_OCR_REGISTER      OCR2A
  #define SPINDLE_COMB_BIT          COM2A1
#endif // end of CPU_MAP_C3D_REMIX


#ifdef CPU_MAP_C3D_MINI // (Cohesion3D Mini Boards)

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEP_DDR        LPC_GPIO2->FIODIR
  #define STEP_PORT       LPC_GPIO2->FIOPIN
  #define X_STEP_BIT      0
  #define Y_STEP_BIT      1
  #define Z_STEP_BIT      2
  #define A_STEP_BIT      3
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)|(1<<A_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     LPC_GPIO0->FIODIR
  #define DIRECTION_PORT    LPC_GPIO0->FIOPIN
  #define X_DIRECTION_BIT   5
  #define Y_DIRECTION_BIT   11
  #define Z_DIRECTION_BIT   20
  #define A_DIRECTION_BIT   22
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)|(1<<A_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    LPC_GPIO0->FIODIR
  #define STEPPERS_DISABLE_PORT   LPC_GPIO0->FIOPIN
  #define X_DISABLE_BIT           4
  #define Y_DISABLE_BIT           10
  #define Z_DISABLE_BIT           19
  #define A_DISABLE_BIT           21
  #define STEPPERS_DISABLE_MASK   ((1<<X_DISABLE_BIT)|(1<<Y_DISABLE_BIT)|(1<<Z_DISABLE_BIT)|(1<<A_DISABLE_BIT))

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR         LPC_GPIO1->FIODIR
  #define LIMIT_PIN         LPC_GPIO1->FIOPIN
  #define LIMIT_PORT        LPC_GPIO1->FIOPIN
  #define X_LIMIT_BIT       24  // X-MIN=24, X-MAX=25
  #define Y_LIMIT_BIT       26  // Y-MIN=26, Y-MAX=27
  #define Z_LIMIT_BIT       28  // Z-MIN=28, Z-MAX=29
  #define A_LIMIT_BIT       29  // reuse p1.29 from Z-MAX
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<A_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR        NotUsed
  #define SPINDLE_ENABLE_PORT       NotUsed
  #define SPINDLE_ENABLE_BIT        30  // P1.30
  #define SPINDLE_DIRECTION_DDR     LPC_GPIO1->FIODIR
  #define SPINDLE_DIRECTION_PORT    LPC_GPIO1->FIOPIN
  #define SPINDLE_DIRECTION_BIT     31  // P1.31

  // Define flood and mist coolant enable output pins.
  #define COOLANT_FLOOD_DDR   NotUsed
  #define COOLANT_FLOOD_PORT  NotUsed
  #define COOLANT_FLOOD_BIT   6  // MOSFET 3 (P2.6)
  #define COOLANT_MIST_DDR    LPC_GPIO2->FIODIR
  #define COOLANT_MIST_PORT   LPC_GPIO2->FIOPIN
  #define COOLANT_MIST_BIT    7  // MOSFET 2 (P2.7)
  #define ENABLE_M7           // enables COOLANT MIST

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       NotUsed
  #define CONTROL_PIN       NotUsed
  #define CONTROL_PORT      NotUsed
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     NotUsed // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       LPC_GPIO1->FIODIR
  #define PROBE_PIN       LPC_GPIO1->FIOPIN
  #define PROBE_PORT      LPC_GPIO1->FIOPIN
  #define PROBE_BIT       27  // reuse P1.27 from Y-MAX
  #define PROBE_MASK      (1<<PROBE_BIT)

  // The LPC17xx has 6 PWM channels. Each channel has 2 pins. It can drive both pins simultaneously to the same value.
  //
  // PWM Channel      PWM1_CH1  PWM1_CH2  PWM1_CH3  PWM1_CH4  PWM1_CH5  PWM1_CH6
  // Primary pin      P1.18     P1.20     P1.21     P1.23     P1.24     P1.26
  // Secondary pin    P2.0      P2.1      P2.2      P2.3      P2.4      P2.5
  #ifdef SPINDLE_PWM_PIN_2_4
    #define SPINDLE_PWM_CHANNEL         PWM1_CH5    // MOSFET3 (P2.4)
  #else
    #define SPINDLE_PWM_CHANNEL         PWM1_CH6    // BED MOSFET (P2.5)
  #endif
  #define SPINDLE_PWM_USE_PRIMARY_PIN   false
  #define SPINDLE_PWM_USE_SECONDARY_PIN true

  // Variable spindle configuration below. Do not change unless you know what you are doing.
  // NOTE: Only used when variable spindle is enabled.
  #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
  #ifndef SPINDLE_PWM_MIN_VALUE
    #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
  #endif
  //#define SPINDLE_PWM_OFF_VALUE     0 // Defined in config.h
  #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
  #define SPINDLE_TCCRA_REGISTER    TCCR2A
  #define SPINDLE_TCCRB_REGISTER    TCCR2B
  #define SPINDLE_OCR_REGISTER      OCR2A
  #define SPINDLE_COMB_BIT          COM2A1
#endif // end of CPU_MAP_C3D_MINI


#ifdef CPU_MAP_MKS_SBASE // (MKS SBASE Boards)

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEP_DDR        LPC_GPIO2->FIODIR
  #define STEP_PORT       LPC_GPIO2->FIOPIN
  #define X_STEP_BIT      0
  #define Y_STEP_BIT      1
  #define Z_STEP_BIT      2
  #define A_STEP_BIT      3
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)|(1<<A_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     LPC_GPIO0->FIODIR
  #define DIRECTION_PORT    LPC_GPIO0->FIOPIN
  #define X_DIRECTION_BIT   5
  #define Y_DIRECTION_BIT   11
  #define Z_DIRECTION_BIT   20
  #define A_DIRECTION_BIT   22
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)|(1<<A_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    LPC_GPIO0->FIODIR
  #define STEPPERS_DISABLE_PORT   LPC_GPIO0->FIOPIN
  #define X_DISABLE_BIT           4
  #define Y_DISABLE_BIT           10
  #define Z_DISABLE_BIT           19
  #define A_DISABLE_BIT           21
  #define STEPPERS_DISABLE_MASK   ((1<<X_DISABLE_BIT)|(1<<Y_DISABLE_BIT)|(1<<Z_DISABLE_BIT)|(1<<A_DISABLE_BIT))

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR         LPC_GPIO1->FIODIR
  #define LIMIT_PIN         LPC_GPIO1->FIOPIN
  #define LIMIT_PORT        LPC_GPIO1->FIOPIN
  #define X_LIMIT_BIT       24  // X-MIN=24, X-MAX=25
  #define Y_LIMIT_BIT       26  // Y-MIN=26, Y-MAX=27
  #define Z_LIMIT_BIT       28  // Z-MIN=28, Z-MAX=29
  #define A_LIMIT_BIT       29  // reuse p1.29
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<A_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR        LPC_GPIO1->FIODIR
  #define SPINDLE_ENABLE_PORT       LPC_GPIO1->FIOPIN
  #define SPINDLE_ENABLE_BIT        30  // P1.30
  #define SPINDLE_DIRECTION_DDR     LPC_GPIO1->FIODIR
  #define SPINDLE_DIRECTION_PORT    LPC_GPIO1->FIOPIN
  #define SPINDLE_DIRECTION_BIT     31  // P1.31

  // Define flood and mist coolant enable output pins.
  #define COOLANT_FLOOD_DDR   LPC_GPIO2->FIODIR
  #define COOLANT_FLOOD_PORT  LPC_GPIO2->FIOPIN
  #define COOLANT_FLOOD_BIT   6  // MOSFET 2.6
  #define COOLANT_MIST_DDR    LPC_GPIO2->FIODIR
  #define COOLANT_MIST_PORT   LPC_GPIO2->FIOPIN
  #define COOLANT_MIST_BIT    7  // MOSFET 2.7
  #define ENABLE_M7           // enables COOLANT MIST

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       LPC_GPIO1->FIODIR
  #define CONTROL_PIN       LPC_GPIO1->FIOPIN
  #define CONTROL_PORT      LPC_GPIO1->FIOPIN
  #define CONTROL_RESET_BIT         NotUsed  // Not needed as there is a special RESET pin on the Smoothiebaord
  #define CONTROL_FEED_HOLD_BIT     22  // P1.22
  #define CONTROL_CYCLE_START_BIT   23  // P1.23
  #define CONTROL_SAFETY_DOOR_BIT   22  // P1.22 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     NotUsed // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       LPC_GPIO1->FIODIR
  #define PROBE_PIN       LPC_GPIO1->FIOPIN
  #define PROBE_PORT      LPC_GPIO1->FIOPIN
  #define PROBE_BIT       27  // reuse P1.27 from Y-MAX
  #define PROBE_MASK      (1<<PROBE_BIT)

  // The LPC17xx has 6 PWM channels. Each channel has 2 pins. It can drive both pins simultaneously to the same value.
  //
  // PWM Channel      PWM1_CH1  PWM1_CH2  PWM1_CH3  PWM1_CH4  PWM1_CH5  PWM1_CH6
  // Primary pin      P1.18     P1.20     P1.21     P1.23     P1.24     P1.26
  // Secondary pin    P2.0      P2.1      P2.2      P2.3      P2.4      P2.5
  #define SPINDLE_PWM_CHANNEL           PWM1_CH6    // BED MOSFET (P2.5)
  #define SPINDLE_PWM_USE_PRIMARY_PIN   false
  #define SPINDLE_PWM_USE_SECONDARY_PIN true
  #ifdef SPINDLE_PWM_PIN_1_23
    #define SPINDLE_PWM_CHANNEL           PWM1_CH4    // MOSFET3 (P1.23)
    #define SPINDLE_PWM_USE_PRIMARY_PIN   true
    #define SPINDLE_PWM_USE_SECONDARY_PIN false
  #endif
  #ifdef SPINDLE_PWM_PIN_2_4
    #define SPINDLE_PWM_CHANNEL           PWM1_CH5    // MOSFET3 (P1.23)
    #define SPINDLE_PWM_USE_PRIMARY_PIN   false
    #define SPINDLE_PWM_USE_SECONDARY_PIN true
  #endif

  // Stepper current control
  #define CURRENT_I2C Driver_I2C1       // I2C driver for current control. Comment out to disable (for C3d boards!)
  #define CURRENT_MCP44XX_ADDR 0b0101100  // Address of MCP44XX
  #define CURRENT_WIPERS {0, 1, 2, 3};    // Wiper registers (X, Y, Z, E0)
  #define CURRENT_FACTOR 113.33           // Convert amps to digipot value

  // Variable spindle configuration below. Do not change unless you know what you are doing.
  // NOTE: Only used when variable spindle is enabled.
  #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
  #ifndef SPINDLE_PWM_MIN_VALUE
    #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
  #endif
  //#define SPINDLE_PWM_OFF_VALUE     0 // Defined in config.h
  #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
  #define SPINDLE_TCCRA_REGISTER    TCCR2A
  #define SPINDLE_TCCRB_REGISTER    TCCR2B
  #define SPINDLE_OCR_REGISTER      OCR2A
  #define SPINDLE_COMB_BIT          COM2A1
#endif // end of CPU_MAP_MKS_SBASE


#ifdef CPU_MAP_AZTEEG_X5 // (Azteeg X5 Boards)  not tested yet!

  // Define serial port pins and interrupt vectors.
  #define SERIAL_RX     USART_RX_vect
  #define SERIAL_UDRE   USART_UDRE_vect

  // Define step pulse output pins. NOTE: All step bit pins must be on the same port.
  #define STEP_DDR        LPC_GPIO2->FIODIR
  #define STEP_PORT       LPC_GPIO2->FIOPIN
  #define X_STEP_BIT      1
  #define Y_STEP_BIT      2
  #define Z_STEP_BIT      3
  #define A_STEP_BIT      0
  #define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)|(1<<A_STEP_BIT)) // All step bits

  // Define step direction output pins. NOTE: All direction pins must be on the same port.
  #define DIRECTION_DDR     LPC_GPIO0->FIODIR
  #define DIRECTION_PORT    LPC_GPIO0->FIOPIN
  #define X_DIRECTION_BIT   11
  #define Y_DIRECTION_BIT   20
  #define Z_DIRECTION_BIT   22
  #define A_DIRECTION_BIT   5
  #define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)|(1<<A_DIRECTION_BIT)) // All direction bits

  // Define stepper driver enable/disable output pin.
  #define STEPPERS_DISABLE_DDR    LPC_GPIO0->FIODIR
  #define STEPPERS_DISABLE_PORT   LPC_GPIO0->FIOPIN
  #define X_DISABLE_BIT           10
  #define Y_DISABLE_BIT           19
  #define Z_DISABLE_BIT           21
  #define A_DISABLE_BIT           4
  #define STEPPERS_DISABLE_MASK   ((1<<X_DISABLE_BIT)|(1<<Y_DISABLE_BIT)|(1<<Z_DISABLE_BIT)|(1<<A_DISABLE_BIT))

  // Define homing/hard limit switch input pins and limit interrupt vectors.
  // NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
  #define LIMIT_DDR         LPC_GPIO1->FIODIR
  #define LIMIT_PIN         LPC_GPIO1->FIOPIN
  #define LIMIT_PORT        LPC_GPIO1->FIOPIN
  #define X_LIMIT_BIT       24  // X-MIN=24, X-MAX=27
  #define Y_LIMIT_BIT       25  // Y-MIN=25, Y-MAX=28
  #define Z_LIMIT_BIT       26  // Z-MIN=26, Z-MAX=29
  #define A_LIMIT_BIT       27  // reuse p1.27, as X-MAX is not used
  #define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)|(1<<A_LIMIT_BIT)) // All limit bits

  // Define spindle enable and spindle direction output pins.
  #define SPINDLE_ENABLE_DDR        LPC_GPIO1->FIODIR
  #define SPINDLE_ENABLE_PORT       LPC_GPIO1->FIOPIN
  #define SPINDLE_ENABLE_BIT        30  // P1.30
  #define SPINDLE_DIRECTION_DDR     LPC_GPIO1->FIODIR
  #define SPINDLE_DIRECTION_PORT    LPC_GPIO1->FIOPIN
  #define SPINDLE_DIRECTION_BIT     31  // P1.31

  // Define flood and mist coolant enable output pins.
  #define COOLANT_FLOOD_DDR   LPC_GPIO2->FIODIR
  #define COOLANT_FLOOD_PORT  LPC_GPIO2->FIOPIN
  #define COOLANT_FLOOD_BIT   4  // FAN MOSFET (P2.4)
  #define COOLANT_MIST_DDR    LPC_GPIO2->FIODIR
  #define COOLANT_MIST_PORT   LPC_GPIO2->FIOPIN
  #define COOLANT_MIST_BIT    7  // BED MOSFET (P2.7)
  #define ENABLE_M7           // enables COOLANT MIST

  // Define user-control controls (cycle start, reset, feed hold) input pins.
  // NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
  #define CONTROL_DDR       NotUsed
  #define CONTROL_PIN       NotUsed
  #define CONTROL_PORT      NotUsed
  #define CONTROL_RESET_BIT         0  // Uno Analog Pin 0
  #define CONTROL_FEED_HOLD_BIT     1  // Uno Analog Pin 1
  #define CONTROL_CYCLE_START_BIT   2  // Uno Analog Pin 2
  #define CONTROL_SAFETY_DOOR_BIT   1  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
  #define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
  #define CONTROL_INT_vect  PCINT1_vect
  #define CONTROL_PCMSK     NotUsed // Pin change interrupt register
  #define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
  #define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

  // Define probe switch input pin.
  #define PROBE_DDR       NotUsed
  #define PROBE_PIN       NotUsed
  #define PROBE_PORT      NotUsed
  #define PROBE_BIT       5  // Uno Analog Pin 5
  #define PROBE_MASK      (1<<PROBE_BIT)

  // The LPC17xx has 6 PWM channels. Each channel has 2 pins. It can drive both pins simultaneously to the same value.
  //
  // PWM Channel      PWM1_CH1  PWM1_CH2  PWM1_CH3  PWM1_CH4  PWM1_CH5  PWM1_CH6
  // Primary pin      P1.18     P1.20     P1.21     P1.23     P1.24     P1.26
  // Secondary pin    P2.0      P2.1      P2.2      P2.3      P2.4      P2.5
  #ifdef SPINDLE_PWM_PIN_2_4
    #define SPINDLE_PWM_CHANNEL         PWM1_CH5    // MOSFET3 (P2.4)
  #else
    #define SPINDLE_PWM_CHANNEL         PWM1_CH6    // BED MOSFET (P2.5)
  #endif
  #define SPINDLE_PWM_USE_PRIMARY_PIN   false
  #define SPINDLE_PWM_USE_SECONDARY_PIN true

  // Stepper current control via SPI not ported yet!

  // Variable spindle configuration below. Do not change unless you know what you are doing.
  // NOTE: Only used when variable spindle is enabled.
  #define SPINDLE_PWM_MAX_VALUE     255 // Don't change. 328p fast PWM mode fixes top value as 255.
  #ifndef SPINDLE_PWM_MIN_VALUE
    #define SPINDLE_PWM_MIN_VALUE   1   // Must be greater than zero.
  #endif
  //#define SPINDLE_PWM_OFF_VALUE     0 // Defined in config.h
  #define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
  #define SPINDLE_TCCRA_REGISTER    TCCR2A
  #define SPINDLE_TCCRB_REGISTER    TCCR2B
  #define SPINDLE_OCR_REGISTER      OCR2A
  #define SPINDLE_COMB_BIT          COM2A1
#endif // end of CPU_MAP_AZTEEG_X5


/*
#ifdef CPU_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and edit one of the available cpu
  // map files and modify it to your needs. Make sure the defined name is also changed in
  // the config.h file.
#endif
*/

#endif
