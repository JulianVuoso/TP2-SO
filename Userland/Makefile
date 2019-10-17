include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule library processA processB

sampleCodeModule:
	cd SampleCodeModule; make

library:
	cd Library; make

processA:
	cd ProcessA; make

processB:
	cd ProcessB; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	cd ProcessA; make clean
	cd ProcessB; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule library processA processB all clean
