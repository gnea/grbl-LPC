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

#include "grbl.h"

static constexpr unsigned flash_sector = 15;                      // Last 4k sector
static constexpr unsigned flash_addr = 0xF000;                    // Last 4k sector
static constexpr unsigned flash_size = 1024;                      // Only using 1k of a 4k sector
static char *flash_memory = (char *)flash_addr;                   // Flash memory
static char flash_buffer[flash_size] __attribute__((aligned(4))); // Copy of flash memory
using Iap = void(unsigned[], unsigned[]);                         // IAP entry point function
static const Iap *iap = (Iap *)0x1FFF1FF1;                        // IAP entry point address

void eeprom_init()
{
    memcpy(flash_buffer, flash_memory, flash_size);  // Copy flash memory into local flash buffer
}

void eeprom_commit()
{
    if (!memcmp(flash_buffer, flash_memory, flash_size))
        return; // No changes to commit
    unsigned prepCommand[5] = {
        50,                     // Prepare sector(s) for write operation
        flash_sector,           // Start sector
        flash_sector,           // End sector
    };
    unsigned eraseCommand[5] = {
        52,                     // Erase sector(s)
        flash_sector,           // Start sector
        flash_sector,           // End sector
        SystemCoreClock / 1000, // CPU clock frequency in kHz
    };
    unsigned writeCommand[5] = {
        51,                     // Copy RAM to Flash
        flash_addr,             // Destination flash address (256-byte boundary)
        (unsigned)flash_buffer, // Source RAM address (word boundary)
        flash_size,             // Number of bytes to write (must be: 256, 512, 1024, 4096)
        SystemCoreClock / 1000, // CPU clock frequency in kHz
    };
    unsigned output[5];
    // Run In-Application Programming (IAP) routines
    iap(prepCommand, output);
    iap(eraseCommand, output);
    iap(prepCommand, output);
    iap(writeCommand, output);
}

unsigned char eeprom_get_char(unsigned int addr)
{
    return flash_buffer[addr];
}

void eeprom_put_char(unsigned int addr, unsigned char new_value)
{
    flash_buffer[addr] = new_value;
}

static unsigned char memcpy_with_checksum(char *dest, char *src, unsigned size)
{
    unsigned char checksum = 0;
    while (size--)
    {
        checksum = ((checksum << 1) || (checksum >> 7)) + *src;
        *dest++ = *src++;
    }
    return checksum;
}

void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size)
{
    auto checksum = memcpy_with_checksum(flash_buffer + destination, source, size);
    flash_buffer[destination + size] = checksum;
}

int memcpy_from_eeprom_with_checksum(char *destination, unsigned int source, unsigned int size)
{
    auto checksum = memcpy_with_checksum(destination, flash_buffer + source, size);
    return checksum == flash_buffer[source + size];
}
