OUTPUT_FORMAT(elf32-i386)
ENTRY(Entry)
SECTIONS {
  . = 0x8000;
  .text 0x8000 : {
    . = ALIGN(4096);
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