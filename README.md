![GitHub Logo](https://github.com/gnea/gnea-Media/blob/master/Grbl%20Logo/Grbl%20Logo%20250px.png?raw=true)

***
Old releases are in the `Release` tab. See [cprezzi's branch](https://github.com/cprezzi/grbl-LPC) for more recent releases.
Note: cprezzi's branch disables current control and has defaults more suitable for other boards.
***
This is GRBL 1.1 ported to the LPC1769. It can run on Smoothieboard.

Usage notes:
* This uses a different usb-serial driver than Smoothieware. Windows 10 should recognize it automatically.
  If it doesn't, try installing VCOM_lib/usbser.inf.
* This doesn't pass the sdcard to the host. Once installed you need to use a micro sdcard adaptor to replace or change it.
* Only tested with lasers with PWM. Non-PWM spindle control not ported.
* These are defaults for easy-to-change config values.
  * WPos enabled for LaserWeb compatability ($10=0)
  * Laser mode: ON ($32)
  * Minimum S value: 0.0 ($31)
  * Maximum S value: 1.0 ($30)
* Hard limits not yet ported
* Control inputs not yet ported (e.g. Cycle Start and Safety Door switches)

New configuration settings
* $33 is PWM frequency in Hz
* $34 is PWM off value in %
* $35 is PWM min value in %
* $36 is PWM max value in %
* $140, $141, $142 are X, Y, Z current (amps)
  * Default to 0.0 A to avoid burning out your motors
  * Your motors will likely stall if you don't set these!

**Build notes:**
* You should use virtual machines, if you use multiple toolchains on the same PC.
* Install make if not already there (for Windows see http://gnuwin32.sourceforge.net/packages/make.htm)
* Install the ARM embeded toolchain (see https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* Include ```make``` and the ```arm-none-eabi-*``` tools in your path.
* Run ```git submodule init``` and ```git submodule update``` before building.
* ```make``` produces 2 files:
  * ```build/firmware.bin```: this is compatible with the sdcard bootloader.
  * ```build/grbl.hex```: this is not compatible with the sdcard bootloader. It loads using Flash Magic 
    and is primarilly for developers who don't want to keep swapping sdcards. If you flash this,
    then you'll have to reflash the bootloader if you want to go back.

***
Grbl is a no-compromise, high performance, low cost alternative to parallel-port-based motion control for CNC milling. This version of Grbl runs on an Arduino with a 328p processor (Uno, Duemilanove, Nano, Micro, etc).

The controller is written in highly optimized C utilizing every clever feature of the AVR-chips to achieve precise timing and asynchronous operation. It is able to maintain up to 30kHz of stable, jitter free control pulses.

It accepts standards-compliant g-code and has been tested with the output of several CAM tools with no problems. Arcs, circles and helical motion are fully supported, as well as, all other primary g-code commands. Macro functions, variables, and most canned cycles are not supported, but we think GUIs can do a much better job at translating them into straight g-code anyhow.

Grbl includes full acceleration management with look ahead. That means the controller will look up to 16 motions into the future and plan its velocities ahead to deliver smooth acceleration and jerk-free cornering.

* [Licensing](https://github.com/gnea/grbl/wiki/Licensing): Grbl is free software, released under the GPLv3 license.

* For more information and help, check out our **[Wiki pages!](https://github.com/gnea/grbl/wiki)** If you find that the information is out-dated, please to help us keep it updated by editing it or notifying our community! Thanks!

* Lead Developer: Sungeun "Sonny" Jeon, Ph.D. (USA) aka @chamnit

* Built on the wonderful Grbl v0.6 (2011) firmware written by Simen Svale Skogsrud (Norway).

***

### Official Supporters of the Grbl CNC Project
![Official Supporters](https://github.com/gnea/gnea-Media/blob/master/Contributors.png?raw=true)


***

## Update Summary for v1.1
- **IMPORTANT:** Your EEPROM will be wiped and restored with new settings. This is due to the addition of two new spindle speed '$' settings.

- **Real-time Overrides** : Alters the machine running state immediately with feed, rapid, spindle speed, spindle stop, and coolant toggle controls. This awesome new feature is common only on industrial machines, often used to optimize speeds and feeds while a job is running. Most hobby CNC's try to mimic this behavior, but usually have large amounts of lag. Grbl executes overrides in realtime and within tens of milliseconds.

- **Jogging Mode** : The new jogging commands are independent of the g-code parser, so that the parser state doesn't get altered and cause a potential crash if not restored properly. Documentation is included on how this works and how it can be used to control your machine via a joystick or rotary dial with a low-latency, satisfying response.

- **Laser Mode** : The new "laser" mode will cause Grbl to move continuously through consecutive G1, G2, and G3 commands with spindle speed changes. When "laser" mode is disabled, Grbl will instead come to a stop to ensure a spindle comes up to speed properly. Spindle speed overrides also work with laser mode so you can tweak the laser power, if you need to during the job. Switch between "laser" mode and "normal" mode via a `$` setting.

	- **Dynamic Laser Power Scaling with Speed** : If your machine has low accelerations, Grbl will automagically scale the laser power based on how fast Grbl is traveling, so you won't have burnt corners when your CNC has to make a turn! Enabled by the `M4` spindle CCW command when laser mode is enabled!

- **Sleep Mode** : Grbl may now be put to "sleep" via a `$SLP` command. This will disable everything, including the stepper drivers. Nice to have when you are leaving your machine unattended and want to power down everything automatically. Only a reset exits the sleep state.

- **Significant Interface Improvements**: Tweaked to increase overall performance, include lots more real-time data, and to simplify maintaining and writing GUIs. Based on direct feedback from multiple GUI developers and bench performance testing. _NOTE: GUIs need to specifically update their code to be compatible with v1.1 and later._

	- **New Status Reports**: To account for the additional override data, status reports have been tweaked to cram more data into it, while still being smaller than before. Documentation is included, outlining how it has been changed. 
	- **Improved Error/Alarm Feedback** : All Grbl error and alarm messages have been changed to providing a code. Each code is associated with a specific problem, so users will know exactly what is wrong without having to guess. Documentation and an easy to parse CSV is included in the repo.
	- **Extended-ASCII realtime commands** : All overrides and future real-time commands are defined in the extended-ASCII character space. Unfortunately not easily type-able on a keyboard, but helps prevent accidental commands from a g-code file having these characters and gives lots of space for future expansion.
	- **Message Prefixes** : Every message type from Grbl has a unique prefix to help GUIs immediately determine what the message is and parse it accordingly without having to know context. The prior interface had several instances of GUIs having to figure out the meaning of a message, which made everything more complicated than it needed to be.

- New OEM specific features, such as safety door parking, single configuration file build option, EEPROM restrictions and restoring controls, and storing product data information.
 
- New safety door parking motion as a compile-option. Grbl will retract, disable the spindle/coolant, and park near Z max. When resumed, it will perform these task in reverse order and continue the program. Highly configurable, even to add more than one parking motion. See config.h for details.

- New '$' Grbl settings for max and min spindle rpm. Allows for tweaking the PWM output to more closely match true spindle rpm. When max rpm is set to zero or less than min rpm, the PWM pin D11 will act like a simple enable on/off output.

- Updated G28 and G30 behavior from NIST to LinuxCNC g-code description. In short, if a intermediate motion is specified, only the axes specified will move to the stored coordinates, not all axes as before.

- Lots of minor bug fixes and refactoring to make the code more efficient and flexible.

- **NOTE:** Arduino Mega2560 support has been moved to an active, official Grbl-Mega [project](http://www.github.com/gnea/grbl-Mega/). All new developments here and there will be synced when it makes sense to.


-

```
List of Supported G-Codes in Grbl v1.1:
  - Non-Modal Commands: G4, G10L2, G10L20, G28, G30, G28.1, G30.1, G53, G92, G92.1
  - Motion Modes: G0, G1, G2, G3, G38.2, G38.3, G38.4, G38.5, G80
  - Feed Rate Modes: G93, G94
  - Unit Modes: G20, G21
  - Distance Modes: G90, G91
  - Arc IJK Distance Modes: G91.1
  - Plane Select Modes: G17, G18, G19
  - Tool Length Offset Modes: G43.1, G49
  - Cutter Compensation Modes: G40
  - Coordinate System Modes: G54, G55, G56, G57, G58, G59
  - Control Modes: G61
  - Program Flow: M0, M1, M2, M30*
  - Coolant Control: M7*, M8, M9
  - Spindle Control: M3, M4, M5
  - Valid Non-Command Words: F, I, J, K, L, N, P, R, S, T, X, Y, Z
```

-------------
Grbl is an open-source project and fueled by the free-time of our intrepid administrators and altruistic users. If you'd like to donate, all proceeds will be used to help fund supporting hardware and testing equipment. Thank you!

[![Donate](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CUGXJHXA36BYW)
