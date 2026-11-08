MEMORY
{
  FLASH (rx)    : ORIGIN = 0x08000000, LENGTH = 256K
  RAM (xrw)     : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Definition des Stack-Endes am Ende des RAMs */
_estack = ORIGIN(RAM) + LENGTH(RAM);

ENTRY(Reset_Handler)

SECTIONS
{
  /* CRITICAL: Die Vektortabelle MUSS an die Adresse 0x08000000 */
  .vector_table :
  {
    . = ALIGN(4);
    KEEP(*(.vector_table)) 
    . = ALIGN(4);
  } > FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)           
    *(.text*)
    *(.rodata)         
    *(.rodata*)
    *(.glue_7)         
    *(.glue_7t)
    *(.eh_frame)
    . = ALIGN(4);
  } > FLASH

  /* Verhindert Fehlermeldungen bei manchen GCC-Versionen */
  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > FLASH
  .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > FLASH
}
