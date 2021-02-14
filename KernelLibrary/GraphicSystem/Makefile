NASM = nasm
CC = g++
LD = ld
AR = ar
OBJCOPY = objcopy
TEMPRORYFOLDER = ../../Temprory
LIBFOLDER = KernelLibrary/GraphicSystem/
SOURCESFOLDER = Sources
HEADERSFOLDER = Headers
DEFAULTLIBHEADERSFOLDER = ../Default/Headers

CCOPTION = -O0 -m64 -fpack-struct=1 -mcmodel=large -ffreestanding -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Wno-char-subscripts -Wno-unused-parameter -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -W -Wall -I $(HEADERSFOLDER)/ -I $(DEFAULTLIBHEADERSFOLDER) -nostdlib -masm=intel
TARGETOBJECTS = \
Graphics.obj \
WindowSystem.obj \
WindowSystemLibrary.obj
TARGET = libkernelgraphics.a

prepare:
	mkdir $(TEMPRORYFOLDER)/$(LIBFOLDER)
	mkdir $(TEMPRORYFOLDER)/$(LIBFOLDER)/sys

clean:
	rm -rf $(TEMPRORYFOLDER)/$(LIBFOLDER)

all: prepare $(TARGETOBJECTS)
	cd $(TEMPRORYFOLDER)/$(LIBFOLDER); $(AR) rcs ../../$(TARGET) $(TARGETOBJECTS)

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.cpp -c $(CCOPTION) -o $(TEMPRORYFOLDER)/$(LIBFOLDER)/$@ -L ../ -lkernel