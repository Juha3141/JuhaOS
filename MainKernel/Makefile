NASM = nasm
CC = g++
LD = ld
OBJCOPY = objcopy

SOURCESFOLDER = Sources
HEADERSFOLDER = Headers
TEMPRORYFOLDER = ../Temprory
MAINFOLDER = MainKernel
KERNELLIB = $(TEMPRORYFOLDER)/libkernel.a
KERNELLIBHEADERFOLDER = ../KernelLibrary/Headers

CCOPTION = -m64 -fpack-struct=1 -mcmodel=large -ffreestanding -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Wno-char-subscripts -Wno-unused-parameter -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -W -Wall -I $(HEADERSFOLDER)/ -I  $(KERNELLIBHEADERFOLDER) -l $(KERNELLIB) -nostdlib -masm=intel
TARGETOBJECTS = \
Main.obj

prepare:
	mkdir $(TEMPRORYFOLDER)/$(MAINFOLDER)
	mkdir $(TEMPRORYFOLDER)/$(MAINFOLDER)/sys

clean:
	rm -rf $(TEMPRORYFOLDER)/$(MAINFOLDER)

all: prepare $(TARGETOBJECTS)
	cd $(TEMPRORYFOLDER)/$(MAINFOLDER); $(LD) -m elf_x86_64 -T ../../$(MAINFOLDER)/Linker.ld -o ../SOUL $(TARGETOBJECTS) -nostdlib -L ../ -lkernel

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.cpp -c $(CCOPTION) -o $(TEMPRORYFOLDER)/$(MAINFOLDER)/$@