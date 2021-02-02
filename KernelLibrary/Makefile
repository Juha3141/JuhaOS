NASM = nasm
CC = g++
LD = ld
AR = ar
OBJCOPY = objcopy
TEMPRORYFOLDER = ../Temprory
LIBFOLDER = KernelLibrary
SOURCESFOLDER = Sources
HEADERSFOLDER = Headers

CCOPTION = -m64 -fpack-struct=1 -mcmodel=large -ffreestanding -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Wno-char-subscripts -Wno-unused-parameter -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -W -Wall -I $(HEADERSFOLDER)/ -nostdlib -masm=intel
OCCOPTION = -m64 -fpack-struct=1 -O5 -mcmodel=large -ffreestanding -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Wno-char-subscripts -Wno-unused-parameter -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -W -Wall -I $(HEADERSFOLDER)/ -nostdlib -masm=intel
TARGETOBJECTS = \
sys/DescriptorTables.obj \
sys/Hardware.obj \
sys/ISR.obj \
sys/MemoryManagement.obj \
sys/Task.obj \
string.obj \
TextScreen.obj
ASSEMBLYFILE = Assembly.asm
TARGET = libkernel.a

prepare:
	mkdir $(TEMPRORYFOLDER)/$(LIBFOLDER)
	mkdir $(TEMPRORYFOLDER)/$(LIBFOLDER)/sys

clean:
	rm -rf $(TEMPRORYFOLDER)/$(LIBFOLDER)

all: prepare $(TARGETOBJECTS)
	nasm $(SOURCESFOLDER)/$(ASSEMBLYFILE) -f elf64 -o $(TEMPRORYFOLDER)/$(LIBFOLDER)/Assembly.obj
	cd $(TEMPRORYFOLDER)/$(LIBFOLDER); $(AR) rcs ../$(TARGET) $(TARGETOBJECTS) Assembly.obj

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.cpp -c $(CCOPTION) -o $(TEMPRORYFOLDER)/$(LIBFOLDER)/$@