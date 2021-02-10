OUTPUT_FORMAT(elf64-x86-64)
ENTRY(Main)
SECTIONS {
  . = 0x400000;
  .text 0x400000 : {
    *(.text .stub .text.*)
  }
  .data : {
    *(.data .data.*)
  }
  .rodata : {
  
    *(.rodata .rodata.*)
  }
  .rodata1 : {
    *(.rodata1)
  }
  .bss : {
    *(.bss .bss.*)
  }
}