OUTPUT_FORMAT(elf32-i386)
ENTRY(Main32)
SECTIONS {
  . = 0x10000;
  .text 0x10000 : {
    *(.text .stub .text.* .gnu.linkonce.t.*)
  }
  .data : {
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .rodata : {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  .rodata1 : {
    *(.rodata1)
  }
  .bss : {
    *(.bss .bss.* .gnu.linkonce.b.*)
  }
}