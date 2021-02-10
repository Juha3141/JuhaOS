NASM = nasm
KERNEL16FOLDER = Kernel16
KERNEL32FOLDER = Kernel32
KERNEL64FOLDER = Kernel64
MAINFOLDER = MainKernel
KERNELLIBRARYFOLDER = KernelLibrary
TEMPRORYFOLDER = Temprory
IMGFILESFOLDER = Floppy/*
IMGFOLDER = $(TEMPRORYFOLDER)/img
ISOFOLDER = $(TEMPRORYFOLDER)/iso

KERNELLOADER = KernelLoader.asm
BOOTLOADER = BootLoader.bin
KERNEL16 = Kernel16.bin
KERNEL32 = Kernel32.bin
KERNEL64 = Kernel64.bin
MAINKERNEL = SOUL
SYSTEM = System

MEMSIZE = 4096

CREATEKERNELBINFOLDER = CreateKernelBin
CREATEKERNELBINSOURCE = $(CREATEKERNELBINFOLDER)/CreateKernelBin.c
CREATEKERNELBIN = $(CREATEKERNELBINFOLDER)/CreateKernelBin.out

TARGET = OS.iso
IMGTARGET = OS.img

prepare:
	rm -rf iso

all: prepare BuildKernelLibrary BuildKernel16 BuildKernel32 BuildKernel64 BuildMainKernel BuildTarget

BuildKernel16:
	make -C $(KERNEL16FOLDER) all

BuildKernel32:
	make -C $(KERNEL32FOLDER) all

BuildKernel64:
	make -C $(KERNEL64FOLDER) all

BuildMainKernel:
	make -C $(MAINFOLDER) all

BuildKernelLibrary:
	make -C $(KERNELLIBRARYFOLDER) all

BuildTarget:
	mkdir $(IMGFOLDER)
	mkdir $(ISOFOLDER)

	mkdosfs -C $(IMGTARGET) 1440
	dd status=noxfer conv=notrunc if=$(TEMPRORYFOLDER)/$(KERNEL16FOLDER)/$(BOOTLOADER) of=$(IMGTARGET)
	mount -t vfat -o loop $(IMGTARGET) $(IMGFOLDER)

	cp $(TEMPRORYFOLDER)/$(KERNEL16FOLDER)/$(KERNEL16) $(IMGFOLDER)
	cp $(TEMPRORYFOLDER)/$(KERNEL32) $(IMGFOLDER)
	cp $(TEMPRORYFOLDER)/$(KERNEL64) $(IMGFOLDER)
	cp $(TEMPRORYFOLDER)/$(MAINKERNEL) $(IMGFOLDER)
	
	sleep 0.1
	umount $(IMGFOLDER)
	cp $(IMGTARGET) $(ISOFOLDER)/$(SYSTEM)
	mkisofs -quiet -V 'JUHAOS' -input-charset iso8859-1 -o $(TARGET) -b $(SYSTEM) $(ISOFOLDER)
	rm -rf $(IMGFOLDER)
	rm -rf $(ISOFOLDER)

clean:
	make -C $(KERNEL16FOLDER) clean
	make -C $(KERNEL32FOLDER) clean
	make -C $(KERNEL64FOLDER) clean
	make -C $(MAINFOLDER) clean
	make -C $(KERNELLIBRARYFOLDER) clean
	rm -rf $(TEMPRORYFOLDER)/*
	rm -rf $(TARGET)
	rm -rf $(IMGTARGET)
	rm -rf $(IMGFOLDER)
	rm -rf $(ISOFOLDER)
	rm -rf BootLoader.bin

run:
	qemu-system-x86_64 -cdrom $(TARGET) -boot d -m $(MEMSIZE)

debugrun:
	qemu-system-x86_64 -cdrom $(TARGET) -boot d -m $(MEMSIZE) -d int -no-reboot
