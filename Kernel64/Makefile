NASM = nasm
CC = g++
LD = ld
OBJCOPY = objcopy

SOURCESFOLDER = Sources
HEADERSFOLDER = Headers
TEMPRORYFOLDER = ../Temprory
KERNEL64FOLDER = Kernel64
KERNELLIB = $(TEMPRORYFOLDER)/libkernel.a
KERNELLIBHEADERS = ../KernelLibrary/Default/Headers/
GRAPHICSLIBHEADERS = ../KernelLibrary/GraphicSystem/Headers/

CCOPTION = -m64 -fpack-struct=1 -mcmodel=large -ffreestanding -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Wno-char-subscripts -Wno-unused-parameter -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -W -Wall -I $(HEADERSFOLDER)/ -I  $(KERNELLIBHEADERS) -I $(GRAPHICSLIBHEADERS) -l $(KERNELLIB) -nostdlib -masm=intel
TARGETOBJECTS = \
Main.obj \
TextUserInterface.obj
ENTRYFILE = Entry.asm

prepare:
	mkdir $(TEMPRORYFOLDER)/$(KERNEL64FOLDER)
	mkdir $(TEMPRORYFOLDER)/$(KERNEL64FOLDER)/sys

clean:
	rm -rf $(TEMPRORYFOLDER)/$(KERNEL64FOLDER)

all: prepare $(TARGETOBJECTS)
	nasm $(SOURCESFOLDER)/$(ENTRYFILE) -f elf64 -o $(TEMPRORYFOLDER)/$(KERNEL64FOLDER)/Entry.obj
	cd $(TEMPRORYFOLDER)/$(KERNEL64FOLDER); $(LD) -m elf_x86_64 -T ../../$(KERNEL64FOLDER)/Linker.ld -o ../Kernel64.elf Entry.obj $(TARGETOBJECTS) -L ../ -lkernel -lkernelgraphics
	cd $(TEMPRORYFOLDER)/; $(OBJCOPY) -O binary Kernel64.elf Kernel64.bin

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.cpp -c $(CCOPTION) -o $(TEMPRORYFOLDER)/$(KERNEL64FOLDER)/$@