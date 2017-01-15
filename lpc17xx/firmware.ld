/* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* THE AUTHORS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. USE IT AT YOUR OWN RISK */

/* This linker script is for firmware.bin, which is compatible with the sdcard bootloader.
 */

MEMORY
{
        /* LPC1768 : 512k ROM + 64k SRAM */
        /*------------------------------ */

	/* On-chip ROM is a readable (r), executable region (x) */
	/* On-chip SRAM is a readable (r), writable (w) and */
	/* executable region (x) */

	/* vector table */
	VECT (rx) : ORIGIN = 0x00004000, LENGTH = 4k

	/* ROM. Skips the remaining 4k flash sectors so they can be used for persistent data */
	IROM (rx) : ORIGIN = 0x00010000, LENGTH = 0x70000 /* 512k - 0x10000 */

	/* local static RAM - 32k for LPC1756 */
	IRAM0 (rwx) : ORIGIN = 0x10000000, LENGTH = 32736 /* 32k-32: 32 bytes at top reserved by IAP */

	/* AHB SRAM - 16k for LPC1756 - often used for USB */
	IRAM1 (rwx) : ORIGIN = 0x2007C000, LENGTH = 16k
	IRAM2 (rwx) : ORIGIN = 0x20080000, LENGTH = 16k
}

/* SECTION command : Define mapping of input sections */
/* into output sections. */

SECTIONS
{
	/******************************************/
	/* code section */

	.isr_vector_section :
	{
		KEEP(*(.isr_vector .isr_vector.*))
	} >VECT

	/* "normal" code */
	.text :
	{
		*(.text .text.*)
		*(.gnu.linkonce.t.*)
		*(.glue_7)
		*(.glue_7t)
		*(.gcc_except_table)
		*(.rodata .rodata*)
		*(.gnu.linkonce.r.*)
	} >IROM

	/******************************************/
	/* .ctors .dtors are used for c++ constructors/destructors */
	.ctors :
	{
		. = ALIGN(4);
		PROVIDE(__ctors_start = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		PROVIDE(__ctors_end = .);
	} >IROM

	.dtors :
	{
		. = ALIGN(4);
		PROVIDE(__dtors_start = .);
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		PROVIDE(__dtors_end = .);

		. = ALIGN(4);
		/* End Of .text section */
		_etext = .;
                _sifastcode = .;
	} >IROM
	
        /**************************************************/
        /* fastcode - copied at startup & executed in RAM */

        .fastcode :
        {
          . = ALIGN (4);
          _sfastcode = . ;

          *(.glue_7t) *(.glue_7)
          *(.fastcode)
          
          /* add other modules here ... */

          . = ALIGN (4);
          _efastcode = . ;
          _sidata = .;
        } >IRAM0 AT>IROM

	/******************************************/
	/* This used for USB RAM section */
	.usb_ram (NOLOAD):
	{
		*.o (USB_RAM)
	} > IRAM1

        /******************************************/
	/* data section */
	.data :
  	{
        	_sidata = LOADADDR (.data);
  		. = ALIGN(4);
		_sdata = .;

		*(vtable vtable.*)
		*(.data .data.*)
		*(.gnu.linkonce.d*)

		. = ALIGN(4);
		_edata = . ;
	} >IRAM0 AT>IROM

	/******************************************/
	/* For no-init variables section */
	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = . ;
		__bss_start__ = .;

		*(.bss .bss.*)
		*(.gnu.linkonce.b*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = . ;
		__bss_end__ = .;
	} >IRAM0

	/******************************************/
	/* For stack section */
  	.stackarea (NOLOAD) :
  	{
	    . = ALIGN(8);
	    _sstack = .;

		*(.stackarea .stackarea.*)

		. = ALIGN(8);
		_estack = .;

		. = ALIGN(4);
		_end = . ;
		PROVIDE (end = .);

  	} > IRAM0

	/******************************************/
	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	/* .comment       0 : { *(.comment) } */
	/* DWARF debug sections.
	 Symbols in the DWARF debugging sections are relative to the beginning
	 of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
}
