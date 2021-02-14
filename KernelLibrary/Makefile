TEMPRORYFOLDER = ../Temprory
LIBFOLDER = KernelLibrary

all:
	mkdir $(TEMPRORYFOLDER)/$(LIBFOLDER)
	make -C Default/ all
	make -C GraphicSystem/ all

clean:
	make -C Default/ clean
	make -C GraphicSystem/ clean
	rm -rf $(TEMPRORYFOLDER)/$(LIBFOLDER)