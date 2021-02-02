NASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

SOURCESFOLDER = Sources
HEADERSFOLDER = Headers
KERNEL32FOLDER = Kernel32
TEMPRORYFOLDER = ../Temprory

CCOPTION = -m32 -ffreestanding -W -Wall -I $(HEADERSFOLDER)/ -nostdlib -nostdinc -Wno-unused-parameter -Wno-pointer-to-int-cast
TARGETOBJECTS = Main.obj Paging.obj
ENTRYFILE = Entry.asm
ASSEMBLYFILE = Assembly.asm

prepare:
	mkdir $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)

clean:
	rm -rf $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)

all: prepare $(TARGETOBJECTS)
	nasm $(SOURCESFOLDER)/$(ENTRYFILE) -f elf32 -o $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)/Entry.obj
	nasm $(SOURCESFOLDER)/$(ASSEMBLYFILE) -f elf32 -o $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)/Assembly.obj
	cd $(TEMPRORYFOLDER)/; $(OBJCOPY) -S -O binary Kernel32.elf Kernel.bin -j .text -j .data -j .rodata -j .rodata1 -j .bss
	cd $(TEMPRORYFOLDER)/$(KERNEL32FOLDER); $(LD) -m elf_i386 -T ../../$(KERNEL32FOLDER)/Linker.ld -o ../Kernel32.elf Entry.obj $(TARGETOBJECTS) Assembly.obj -nostdlib

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.c -c $(CCOPTION) -o $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)/$@