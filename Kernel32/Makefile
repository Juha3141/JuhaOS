NASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

SOURCESFOLDER = Sources
HEADERSFOLDER = Headers
KERNEL32FOLDER = Kernel32
TEMPRORYFOLDER = ../Temprory

CCOPTION = -m32 -ffreestanding -W -Wall -I $(HEADERSFOLDER)/ -nostdlib -nostdinc -Wno-unused-parameter -Wno-pointer-to-int-cast -masm=intel
TARGETOBJECTS = Main.obj Paging.obj
ASSEMBLYFILE = Assembly.asm

prepare:
	mkdir $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)

clean:
	rm -rf $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)

all: prepare $(TARGETOBJECTS)
	nasm $(SOURCESFOLDER)/$(ASSEMBLYFILE) -f elf32 -o $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)/Assembly.obj
	cd $(TEMPRORYFOLDER)/$(KERNEL32FOLDER); $(LD) -m elf_i386 -T ../../$(KERNEL32FOLDER)/Linker.ld -o ../Kernel32.elf $(TARGETOBJECTS) Assembly.obj -nostdlib
	cd $(TEMPRORYFOLDER)/; $(OBJCOPY) -O binary Kernel32.elf Kernel32.bin

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.c -c $(CCOPTION) -o $(TEMPRORYFOLDER)/$(KERNEL32FOLDER)/$@