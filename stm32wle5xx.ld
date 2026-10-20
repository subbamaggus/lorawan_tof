MEMORY
{
  FLASH (rx)    : ORIGIN = 0x08000000, LENGTH = 256K
  RAM (xrw)     : ORIGIN = 0x20000000, LENGTH = 64K
}

ENTRY(Reset_Handler)

SECTIONS
{
  .vector_table :
  {
    . = ALIGN(4);
    KEEP(*(.vector_table)) /* Vektortabelle ganz nach vorne */
    . = ALIGN(4);
  } > FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)           /* Programmcode */
    *(.text*)
    *(.rodata)         /* Konstanten/Strings */
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  _estack = ORIGIN(RAM) + LENGTH(RAM); /* Stack-Ende am RAM-Ende */
}
