# Makefile for GRBL port to LPC17xx
#
# Copyright 2017 Todd Fleming
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

INCLUDES =                                        \
    -I CMSIS_5/CMSIS/Core/Include                 \
    -I CMSIS_5/CMSIS/Driver/Include               \
    -I CMSIS_5/Device/ARM/ARMCM3/Include          \
    -I grbl                                       \
    -I grbl-lpc                                   \
    -I lpc17xx                                    \
    -I VCOM_lib                                   \

# Compile all .c and .cpp files in these directories
# Hack: .c files are compiled as if they were c++.
SRC_DIRS =          \
	grbl            \
    grbl-lpc        \
    VCOM_lib        \

# Compile all .c files in these directories, except ones on the exclude list.
# These files get extra -Wno-* flags to reduce spam.
CMSIS_SRC_DIRS =                                  \
    NXP_LPC/LPC1700/CMSIS/Driver                  \
    lpc17xx                                       \

# These error out because we haven't set up the #defines they need.
# To include one, remove it from this list and add the appropriate
# #defines to RTE_Components.h
CMSIS_EXCLUDE_OBJECTS =             \
    build/cmsis/CAN_LPC17xx.o       \
    build/cmsis/EMAC_LPC17xx.o      \
    build/cmsis/I2S_LPC17xx.o       \
    build/cmsis/MCI_LPC177x_8x.o    \
    build/cmsis/OTG_LPC17xx.o       \
    build/cmsis/PIN_LPC177x_8x.o    \
    build/cmsis/SPI_LPC17xx.o       \
    build/cmsis/USBD_LPC17xx.o      \
    build/cmsis/USBH_LPC17xx.o      \

AS      = arm-none-eabi-as
CC      = arm-none-eabi-gcc
CXX     = arm-none-eabi-g++
LD      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size 

CFLAGS =                                \
	-DCORE_M3                           \
	-DLPC175x_6x                        \
    -c                                  \
    -fdata-sections                     \
    -ffunction-sections                 \
    -fmessage-length=0                  \
    -fno-builtin                        \
    -fno-delete-null-pointer-checks     \
    -fno-exceptions                     \
    -fomit-frame-pointer                \
    -funsigned-char                     \
    -mcpu=cortex-m3                     \
    -MMD                                \
    -MP                                 \
    -mthumb                             \
    -Os                                 \
    -Wall                               \
    -Wextra                             \

# Disable warnings in CMSIS
CMSIS_CFLAGS = $(CFLAGS)                \
	-Wno-incompatible-pointer-types     \
	-Wno-maybe-uninitialized            \
	-Wno-sign-compare                   \
	-Wno-strict-aliasing                \
	-Wno-switch                         \
	-Wno-unused-but-set-variable        \
	-Wno-unused-variable                \
    -Wno-int-conversion                 \
    -Wno-missing-field-initializers     \
    -Wno-unused-function                \
    -Wno-unused-parameter               \

# Disable warnings in .c files compiled as C++
C_AS_CPP_CFLAGS = $(CFLAGS)             \
    -Wno-unused-parameter               \

CXXFLAGS = $(CFLAGS)                    \
    -fno-rtti                           \
    -std=gnu++14                        \

LIBS = -Wl,--start-group -lgcc -lc -lm -Wl,--end-group

VPATH = $(SRC_DIRS) $(CMSIS_SRC_DIRS)

GET_OBJECTS = $(addprefix $(1),$(addsuffix .o,$(basename $(notdir $(wildcard $(foreach dir,$(2),$(dir)/*.c) $(foreach dir,$(2),$(dir)/*.cpp))))))

SRC_OBJECTS = $(call GET_OBJECTS,build/src/,$(SRC_DIRS))

CMSIS_OBJECTS = $(filter-out $(CMSIS_EXCLUDE_OBJECTS),$(call GET_OBJECTS,build/cmsis/,$(CMSIS_SRC_DIRS)))

ifeq ($(shell echo $$OS),$$OS)
    MAKEDIR = @if not exist "$(1)" mkdir "$(1)"
    RM = @if exist "$(1)" rmdir /S /Q "$(1)"
else
    MAKEDIR = $(SHELL) -c "mkdir -p \"$(1)\""
    RM = $(SHELL) -c "rm -rf \"$(1)\""
endif

build/cmsis/%.o : %.c
	$(call MAKEDIR,build)
	$(call MAKEDIR,build/cmsis)
	$(CC) $(CMSIS_CFLAGS) $(INCLUDES) -o $@ $<

# hack: compile .c as c++
build/src/%.o : %.c
	$(call MAKEDIR,build)
	$(call MAKEDIR,build/src)
	$(CXX) $(CXXFLAGS) $(C_AS_CPP_CFLAGS) $(INCLUDES) -o $@ $<

build/src/%.o : %.cpp
	$(call MAKEDIR,build)
	$(call MAKEDIR,build/src)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

.PHONY: default
default: build/grbl.hex build/firmware.bin

build/grbl.elf: $(SRC_OBJECTS) $(CMSIS_OBJECTS) lpc17xx/grbl.ld
	$(LD) -mcpu=cortex-m3 -mthumb -specs=nosys.specs -T$(filter %.ld, $^) -o $@ $(filter %.o, $^) $(LIBS)

build/firmware.elf: $(SRC_OBJECTS) $(CMSIS_OBJECTS) lpc17xx/firmware.ld
	$(LD) -mcpu=cortex-m3 -mthumb -specs=nosys.specs -T$(filter %.ld, $^) -o $@ $(filter %.o, $^) $(LIBS)

build/%.bin : build/%.elf
	$(OBJCOPY) -O binary $^ $@

build/%.hex : build/%.elf
	$(OBJCOPY) -O ihex $^ $@

.PHONY: flash
flash: build/grbl.hex
	fm COM(15, 115200) DEVICE(LPC1769, 0.000000, 0) HARDWARE(BOOTEXEC, 50, 100) ERASEUSED(build\grbl.hex, PROTECTISP) HEXFILE(build\grbl.hex, NOCHECKSUMS, NOFILL, PROTECTISP)

.PHONY: clean
clean:
	$(call RM,build)

-include $(wildcard build/src/*.d build/cmsis/*.d)
